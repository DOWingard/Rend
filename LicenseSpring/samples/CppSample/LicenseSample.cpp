#include "LicenseSample.h"

#include <iostream>
#include <thread>

namespace
{
std::string TmToStr(const tm &dateTime)
{
    return LicenseSpring::TmToString(dateTime, "%d-%m-%Y %H:%M:%S");
};
} // namespace

using namespace LicenseSpring;

LicenseSample::LicenseSample(LSAuthMethod authMethod, LicenseManager::ptr_t manager) : BaseSample(authMethod), m_licenseManager(manager) {}

LicenseSample::~LicenseSample()
{
    m_licenseManager.reset();
}

void LicenseSample::runOnline(bool deactivateAndRemove)
{
    auto license = m_licenseManager->getCurrentLicense();

    if (license == nullptr)
    {
        // Set a license key, or username and password combination
        LicenseID licenseID;
        if (m_authMethod == AuthMethodKeyBased)
        {
            licenseID = LicenseID::fromKey("");
        }
        else
        {
            const std::string username = "someone@test.com";
            const std::string password = "";
            licenseID = LicenseID::fromUser(username, password);
        }

        if (licenseID.isEmpty())
        {
            licenseID = m_licenseManager->getTrialLicense();
            std::cout << "Got trial license: " << licenseID.id() << std::endl;
        }

        license = m_licenseManager->activateLicense(licenseID);
        std::cout << "License activated successfully" << std::endl;
    }
    else
    {
        checkLicenseLocal(license);
        license = m_licenseManager->getCurrentLicense(); // update if local check initiated relink
    }

    if (license->isFloating())
        setupAutomaticLicenseUpdates(license);
    else
        updateAndCheckLicense(license);

    PrintLicense(license);

    printUpdateInfo();

    if (deactivateAndRemove)
        cleanUp(license);
}

void LicenseSample::runOffline(bool deactivateAndRemove)
{
    auto license = m_licenseManager->getCurrentLicense();

    if (license == nullptr)
    {
        // You can provide here path to offline activation response file,
        // or use default desktop loaction and file name ls_activation.lic
        license = m_licenseManager->activateLicenseOffline();
        if (license == nullptr)
        {
            std::cout << "Offline activation error\n";

            // Set a license key, or username and password combination
            LicenseID licenseID;
            if (m_authMethod == AuthMethodKeyBased)
            {
                licenseID = LicenseID::fromKey("");
            }
            else
            {
                const std::string username = "someone@test.com";
                const std::string password = "";
                licenseID = LicenseID::fromUser(username, password);
            }

            if (licenseID.isEmpty())
            {
                licenseID = m_licenseManager->getTrialLicense();
                std::cout << "Got trial license: " << licenseID.id() << std::endl;
            }

            createOfflineActivationRequest(licenseID);
            return;
        }
        else
            std::cout << "Offline activation succeeded" << std::endl;
    }

    checkLicenseLocal(license);
    license = m_licenseManager->getCurrentLicense(); // update if local check initiated relink

    updateOfflineLicense(license);

    PrintLicense(license);

    if (license->type() == LicenseTypeConsumption)
        license->updateConsumption();

    if (deactivateAndRemove)
        cleanUpLocal(license);
}

void LicenseSample::checkLicenseLocal(License::ptr_t license)
{
    std::cout << "License successfully loaded, performing local check of the license..."
              << std::endl;
    // it's highly recommended to perform a localCheck (offline check) on every startup
    // to be ensure that license file wasn't copied from another computer and license in a valid
    // state
    try
    {
        bool verifySignature = false;
        license->localCheck(
            verifySignature); // throws exceptions in case of errors, see documentation
    }
    catch (const DeviceNotLicensedException &ex)
    {
        // Below is an example on how to upgrade to new or other device id algorithm
        std::cout << "Local check failed: " << ex.what() << std::endl;
        std::cout << "Trying to upgrade to newer device id algorithm..." << std::endl;
        license = m_licenseManager->relinkLicense(WinCryptographyId);
        std::cout << "License successfully linked to new device id." << std::endl;
        return;
    }
    catch (const FloatingTimeoutExpiredException &ex)
    {
        std::cout << ex.what() << std::endl;
        auto endDate = TmToStr(license->floatingEndDateTime());
        std::cout << "Registration of this floating license has expired at: " << endDate
                  << std::endl;
        std::cout << "Trying to register floating license..." << std::endl;
        license->registerFloatingLicense(); // this call is equivalent to online license check
        std::cout << "License successfully checked in" << std::endl;
        return;
    }
    std::cout << "Local validation successful" << std::endl;
}

void LicenseSample::updateAndCheckLicense(License::ptr_t license)
{
    // Increase consumption license
    if (license->type() == LicenseTypeConsumption)
    {
        license->updateConsumption(1);
        license->syncConsumption(); // this call is not necessary, consumption will be synced during
                                    // online check
    }

    // Increase consumption features
    for (const auto &feature : license->features())
    {
        if (feature.featureType() == FeatureTypeConsumption)
            license->updateFeatureConsumption(feature.code(), 1);
    }
    license->syncFeatureConsumption(); // this call is not necessary, features will be synced during
                                       // online check

    // Register floating features
    for (const auto &feature : license->features())
    {
        if (!(feature.isFloating() || feature.isOfflineFloating()))
            continue;

        std::cout << "Registering floating feature " << feature.code() << std::endl;
        license->registerFloatingFeature(feature.code());
        // need to reload feature
        auto updatedFeature = license->feature(feature.code());
        std::cout << updatedFeature.toString() << std::endl;

        std::cout << "Releasing floating feature " << feature.code() << std::endl;
        license->releaseFloatingFeature(feature.code());
        updatedFeature = license->feature(feature.code());
        std::cout << updatedFeature.toString() << std::endl;
    }

    // Sync license with the platform
    std::cout << "Checking license online..." << std::endl;
    bool includeExpiredFeatures = false;
    license->check(
        InstallFileFilter(), includeExpiredFeatures); // throws exceptions in case of errors
    std::cout << "License successfully checked" << std::endl;
    if (license->isGracePeriodStarted())
    {
        std::cout << "Grace period started!" << std::endl;
        std::cout << license->gracePeriodHoursRemaining() << " hours till the end of grace period."
                  << std::endl;
        std::cout << "Grace period end date time: " << TmToStr(license->gracePeriodEndDateTime())
                  << std::endl;
    }

    // Example of sending and getting custom data to the LS backend (see device variables on the
    // platform)
    std::cout << "Sending custom data to the LicenseSpring..." << std::endl;
    auto approxCoreCount = std::thread::hardware_concurrency();
    license->addDeviceVariable("CPU_Cores", std::to_string(approxCoreCount));
    license->sendDeviceVariables();
    // bool argument below means request variables list from the backend
    auto deviceVariables = license->getDeviceVariables(true);
    for (const auto &variable : deviceVariables)
    {
        std::cout << "Device variable: " << variable.name() << ", value: " << variable.value()
                  << ", last time updated: " << TmToStr(variable.dateTimeUpdated()) << std::endl;
    }
    std::cout << "Operation completed successfully" << std::endl;
}

void LicenseSample::setupAutomaticLicenseUpdates(License::ptr_t license)
{
    // You can borrow floating license for some period of time or till some date time
    // During borrowing period there is no need to check-in license by floating timeout
    // license->borrow();
    // return;

    // Setup a watchdog (background thread), it will automatically check license online
    // This especially useful for floating license, because it's required to keep it alive by doing
    // periodic online checks. The watchdog will call a provided callback in case of errors.

    std::weak_ptr<License> wpLicense(license);
    license->setupLicenseWatchdog([wpLicense](const LicenseSpringException &ex) {
        // Attention, do not capture License::ptr_t (shared_ptr), this will lead to problems.

        std::cout << std::endl << "License check failed: " << ex.what() << std::endl;

        if (ex.getCode() == eMaxFloatingReached)
        {
            std::cout << "Application cannot use this license at the moment because floating "
                         "license limit reached."
                      << std::endl;
            exit(0);
        }

        // Ignore other errors and continue running watchdog if possible
        if (auto pLicense = wpLicense.lock())
        {
            if (pLicense->isValid())
                pLicense->resumeLicenseWatchdog();
        }
    });

    // Let background thread update the license before printing
    std::this_thread::sleep_for(std::chrono::seconds(2));
}

void LicenseSample::setupAutomaticFloatingFeatureUpdates(License::ptr_t license)
{
    // Set up a watchdog (background thread), it will automatically check all registered floating
    // license features The watchdog will call a provided callback in case of errors.

    std::weak_ptr<License> wpLicense(license);
    license->setupFeatureWatchdog([wpLicense](const LicenseSpringException &ex) {
        // Attention, do not capture License::ptr_t (shared_ptr), this will lead to problems.

        std::cout << std::endl << "License check failed: " << ex.what() << std::endl;

        // Ignore other errors and continue running watchdog if possible
        if (auto pLicense = wpLicense.lock())
        {
            if (pLicense->isValid())
                pLicense->resumeFeatureWatchdog();
        }
    });

    // set up your own floating features here
    std::vector<std::string> featureCodes{"floating-feature-1", "floating-feature-2"};

    try
    {
        for (const std::string &featureCode : featureCodes)
            license->registerFloatingFeature(featureCode);
    }
    catch (const LicenseSpring::MaxFloatingReachedException &e)
    {
        std::cout << "Error while checking floating feature: max floating reached!" << std::endl;
    }
    catch (const LicenseSpring::LicenseSpringException &e)
    {
        std::cout << "Error while checking floating feature: " << ' ' << e.getCode() << ' '
                  << e.what() << std::endl;
    }

    // sleep for 100 seconds and check features in the meantime
    std::this_thread::sleep_for(std::chrono::seconds(100));
    try
    {
        // released features are automatically removed from the watchdog
        for (const std::string &featureCode : featureCodes)
            license->releaseFloatingFeature(featureCode);
    }
    catch (const LicenseSpring::LicenseSpringException &e)
    {
        std::cout << "Error while releasing floating feature: " << ' ' << e.getCode() << ' '
                  << e.what() << std::endl;
    }

    license->stopFeatureWatchdog();
}

void LicenseSample::cleanUp(License::ptr_t license)
{
    if (license->deactivate(true))
        std::cout << "License deactivated successfully." << std::endl << std::endl;
}

void LicenseSample::cleanUpLocal(License::ptr_t license)
{
    auto filePath = license->deactivateOffline();
    m_licenseManager->clearLocalStorage();
    std::cout << "To finish deactivation process please upload deactivation request file to the "
                 "LicenseSpring portal."
              << std::endl;
    std::cout << "File path: " << filePath.c_str() << std::endl << std::endl;
}

void LicenseSample::createOfflineActivationRequest(const LicenseID &licenseID)
{
    std::cout << "Creating offline activation request file..." << std::endl;
    auto filePath = m_licenseManager->createOfflineActivationFile(licenseID);
    std::cout << "File created: " << std::endl;
    std::wcout << filePath << std::endl;
    std::cout << "Please upload that request file to LicenseSpring offline activation portal to "
                 "get response file.\n";
    std::cout << "Offline activation portal address: https://offline.licensespring.com"
              << std::endl;
}

void LicenseSample::updateOfflineLicense(LicenseSpring::License::ptr_t license)
{
    // Here is an example how to update offline license.
    // You can download refresh file from LicenseSpring platform, see Devices section of particular
    // license. Choose active device for which you would like to update the license and press
    // "Download license refresh file" button. You may also create refresh file using Management
    // API.

    // Assign license refresh file path below
    std::wstring refreshFilePath = L"license_refresh.lic";
    if (license->updateOffline(refreshFilePath))
        std::cout << "\nLicense refresh file successfully applied\n";
}

void LicenseSample::printUpdateInfo()
{
    if (!m_licenseManager)
        return;

    auto license = m_licenseManager->getCurrentLicense();
    if (license == nullptr)
        return;

    auto versionList = m_licenseManager->getVersionList(license->id());
    if (versionList.empty())
        return;

    std::cout << std::endl << "------------- Update info -------------" << std::endl;
    std::cout << "Total app versions available: " << versionList.size() << std::endl;

    auto installFile = m_licenseManager->getInstallationFile(
        license->id(), versionList.at(versionList.size() - 1));
    if (installFile)
    {
        std::cout << "Latest installation package information" << std::endl;
        printProductVersionInfo(installFile);
    }
}

void LicenseSample::printProductVersionInfo(LicenseSpring::InstallationFile::ptr_t installFile)
{
    if (installFile == nullptr)
        return;
    std::cout << "Product version: " << installFile->version() << std::endl;
    std::cout << "Release date: " << installFile->releaseDate() << std::endl;
    std::cout << "Required version for update: " << installFile->requiredVersion() << std::endl;
    std::cout << "URL for downloading: " << installFile->url() << std::endl;
    std::cout << "Md5 hash: " << installFile->md5Hash() << std::endl;
    std::cout << "Environment: " << installFile->environment() << std::endl;
    std::cout << "Eula Link: " << installFile->eulaLink() << std::endl;
    std::cout << "Release Notes Link: " << installFile->releaseNotesLink() << std::endl;
    std::cout << "Size: " << installFile->size() << std::endl;
    std::cout << "Channel: " << installFile->channel() << std::endl;
    std::cout << std::endl;
}
