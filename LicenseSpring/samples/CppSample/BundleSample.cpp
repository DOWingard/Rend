#include "BundleSample.h"

#include <iostream>

using namespace LicenseSpring;

BundleSample::BundleSample(LSAuthMethod authMethod, BundleManager::ptr_t manager) : BaseSample(authMethod), m_bundleManager(manager) {}

BundleSample::~BundleSample()
{
    m_bundleManager.reset();
}

void BundleSample::runOnline(bool deactivateAndRemove)
{
    LicenseID licenseID;
    if (m_authMethod == AuthMethodKeyBased)
    {
        licenseID = LicenseID::fromKey("YOUR-BUNDLE-KEY-HERE");
    }
    else
    {
        const std::string username = "someone@test.com";
        const std::string password = "";
        licenseID = LicenseID::fromUser(username, password);
    }

    // We avoid copying the product code -> license std::map
    const auto &bundle = m_bundleManager->getCurrentBundle();

    if (bundle.empty())
    {
        std::cout << "No local bundle file found, activating online..." << std::endl;
        m_bundleManager->activateBundle(licenseID);
        // BundleManager::activateBundle also returns a const reference to the product code -> license map
    }

    for (auto const &kv : bundle)
    {
        const std::string &productCode = kv.first;
        License::ptr_t license = kv.second;
        PrintLicense(license);
    }

    std::cout << "Performing a bundle check..." << std::endl;
    m_bundleManager->checkBundle(licenseID);
    std::cout << "Bundle check complete." << std::endl;

    if (deactivateAndRemove)
    {
        std::cout << "Deactivating bundle..." << std::endl;
        if (m_bundleManager->deactivateBundle(licenseID, true))
            std::cout << "Bundle deactivated and local data removed." << std::endl;
    }
}

void BundleSample::runOffline(bool deactivateAndRemove)
{
    LicenseID licenseID;
    if (m_authMethod == AuthMethodKeyBased)
    {
        licenseID = LicenseID::fromKey("YOUR-BUNDLE-KEY-HERE");
    }
    else
    {
        const std::string username = "someone@test.com";
        const std::string password = "";
        licenseID = LicenseID::fromUser(username, password);
    }
    const auto &bundle = m_bundleManager->getCurrentBundle();
    if (bundle.empty())
    {
        // Here you can provide a path to the offline activation response file,
        // or use ls_bundle_activation.lic in the default desktop location.
        std::cout << "No local bundle, attempting offline activation..." << std::endl;
        m_bundleManager->activateBundleOffline();
        if (bundle.empty())
        {
            std::cout << "Offline activation failed, creating request file..." << std::endl;
            m_bundleManager->createOfflineActivationFile(licenseID);
            return;
        }
        std::cout << "Offline bundle activated; contains " << bundle.size() << " licenses." << std::endl;
    }

    for (auto const &kv : bundle)
    {
        const std::string &productCode = kv.first;
        License::ptr_t license = kv.second;
        PrintLicense(license);
    }

    if (deactivateAndRemove)
    {
        std::wstring req = m_bundleManager->deactivateBundleOffline(licenseID);
        std::wcout << L"Offline deactivation request written to: " << req << std::endl;
        m_bundleManager->clearLocalStorage();
    }
}
