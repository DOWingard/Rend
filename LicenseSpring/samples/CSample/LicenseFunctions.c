#include "LicenseFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintString(const char *what, const char *str)
{
    if (what && str && strlen(str) > 0)
        printf("%s = %s\n", what, str);
}

void CheckIfError(LSLicenseHandler *lh, bool exitApp)
{
    if (lh->wasError(lh))
    {
        const char *error = lh->getLastErrorMsg(lh);
        enum LSErrorCode code = lh->getLastError(lh);
        printf("Error occurred. Message: %s Code: %d\n\n", error, code);
        if (exitApp)
            exit(code);
    }
}

void LicenseWatchdogCallback(const char *message, enum LSErrorCode errorCode, void *handler)
{
    printf("License check failed. Message: %s Code: %d\n", message, errorCode);
    LSLicenseHandler *lh = (LSLicenseHandler *)handler;
    if (errorCode == eMaxFloatingReached)
    {
        printf("Application cannot use this license at the moment because floating license limit "
               "reached.\n");
        exit(0);
    }
    if (lh->isLicenseValid(lh) && lh->canIgnoreError(lh))
        lh->resumeLicenseWatchdog(lh);
}

void RegisterAndSetCallback(LSLicenseHandler *lh)
{
    printf("Trying to register floating license...\n");
    lh->registerFloatingLicense(lh);
    CheckIfError(lh, true);
    printf("License successfully checked in.\n");
    printf("Setting up license watchdog...\n");
    lh->setupLicenseWatchdog(lh, LicenseWatchdogCallback, 0);
    CheckIfError(lh, true);
    printf("Watchdog successfully set.\n");
}

void CheckLicenseLocal(LSLicenseHandler *lh)
{
    printf("License successfully loaded, performing local check of the license...\n");
    bool verifySignature = false;
    lh->checkLicenseLocal(lh, verifySignature);
    if (lh->wasError(lh))
    {
        if (lh->getLastError(lh) == eFloatingTimeoutExpired)
        {
            struct tm date = lh->getLicenseFloatingEndDateTime(lh);
            printf("Registration of this floating license has expired at: %d-%d-%d %d:%d:%d\n",
                date.tm_mday, date.tm_mon + 1, date.tm_year + 1900, date.tm_hour, date.tm_min,
                date.tm_sec);
            RegisterAndSetCallback(lh);
        }
        else
            CheckIfError(lh, true);
    }
    printf("Local validation successful.\n");
}

void UpdateAndCheckLicense(LSLicenseHandler *lh)
{
    if (lh->getLicenseType(lh) == LicenseTypeConsumption)
    {
        lh->updateConsumption(lh, 1, true);
        CheckIfError(lh, true);
        lh->syncConsumption(lh);
        CheckIfError(lh, true);
    }

    if (lh->hasLicenseFeatures(lh))
    {
        int len = lh->getLicenseFeatures(lh, 0);
        LSLicenseFeature *features = (LSLicenseFeature *)calloc(len, sizeof(LSLicenseFeature));
        lh->getLicenseFeatures(lh, features);
        for (int i = 0; i < len; i++)
        {
            if (features[i].featureType(&features[i]) == FeatureTypeConsumption)
            {
                const char *code = features[i].code(&features[i]);
                lh->updateFeatureConsumption(lh, code, 1, true);
            }
        }

        // register and release floating features
        for (int i = 0; i < len; i++)
        {
            if (!(features[i].isFloating(&features[i]) ||
                    features[i].isOfflineFloating(&features[i])))
                continue;

            const char *code = features[i].code(&features[i]);
            printf("Registering floating feature %s\n", features[i].code(&features[i]));
            lh->registerFloatingFeature(lh, code, true);

            printf("Releasing floating feature %s\n", features[i].code(&features[i]));
            lh->releaseFloatingFeature(lh, code);
        }
        free(features);
        lh->syncFeatureConsumption(lh, "");
        CheckIfError(lh, true);
    }

    if (lh->isLicenseFloating(lh) && !lh->isLicenseValid(lh))
        RegisterAndSetCallback(lh);
    else
    {
        printf("Checking license online... \n");
        bool includeExpiredFeatures = 0;
        LSInstallationFile *pInstallFile = NULL;
        if (includeExpiredFeatures)
            pInstallFile = lh->checkLicenseEx(lh, 0, 0, includeExpiredFeatures);
        else
            pInstallFile = lh->checkLicense(lh);
        if (pInstallFile)
            FreeLSInstallationFile(pInstallFile);
        CheckIfError(lh, true);
        printf("License successfully checked \n");
    }

    printf("Sending custom data to the LicenseSpring platform... \n");
    LSCustomField field;
    field.name = "DeviceData";
    field.value = "Test data";
    lh->sendDeviceData(lh, &field, 1);
    CheckIfError(lh, true);
    printf("Operation completed successfully\n\n");
}

void PrintLicense(LSLicenseHandler *lh)
{
    if (!lh->isLicenseExists(lh))
        return;

    printf("------------- License info ------------- \n");
    LSCompany *company = lh->getCompany(lh);
    printf("LicenseSpring company ID: %lld\n", company->getId(company));
    FreeLSCompany(company);
    LSCustomer *licenseOwner = lh->getLicenseOwner(lh);
    {
        const char *firstName = licenseOwner->firstName(licenseOwner);
        printf("Licensed to: %s\t", firstName);

        const char *lastName = licenseOwner->lastName(licenseOwner);
        printf("%s\t", lastName);

        const char *email = licenseOwner->email(licenseOwner);
        printf("%s\t", email);

        const char *company = licenseOwner->company(licenseOwner);
        printf("%s\n", company);
    }
    FreeLSCustomer(licenseOwner);

    PrintString("Key", lh->getLicenseKey(lh));

    PrintString("User", lh->getLicenseUser(lh));

    PrintString("Order store ID", lh->getOrderStoreId(lh));

    switch (lh->getLicenseType(lh))
    {
    case (LicenseTypePerpetual):
        printf("Type = perpetual\n");
        break;
    case (LicenseTypeSubscription):
        printf("Type = subscription\n");
        break;
    case (LicenseTypeTimeLimited):
        printf("Type = time-limited\n");
        break;
    case (LicenseTypeConsumption):
        printf("Type = consumption\n");
        break;
    case (LicenseTypeUnknown):
    default:
        printf("Type = unknown\n");
        break;
    }

    PrintString("Status", lh->getLicenseStatusStr(lh));

    bool active = lh->isLicenseActive(lh);
    printf("IsActive = %s\n", active ? "true" : "false");

    bool enabled = lh->isLicenseEnabled(lh);
    printf("IsEnabled = %s\n", enabled ? "true" : "false");

    bool trial = lh->isLicenseTrial(lh);
    printf("IsTrial = %s\n", trial ? "true" : "false");

    bool floating = lh->isLicenseFloating(lh);
    printf("IsFloating = %s\n", floating ? "true" : "false");

    bool offlineActivated = lh->isLicenseOfflineActivated(lh);
    printf("IsOfflineActivated = %s\n", offlineActivated ? "true" : "false");

    int trialPeriod = lh->getLicenseTrialPeriod(lh);
    printf("Trial period for current license = %d\n", trialPeriod);

    int timesActivated = lh->getLicenseTimesActivated(lh);
    printf("Times activated = %d\n", timesActivated);

    int maxActivations = lh->getLicenseMaxActivations(lh);
    printf("Max activations = %d\n", maxActivations);

    unsigned int transferCount = lh->getLicenseTransferCount(lh);
    printf("Transfer count = %d\n", transferCount);

    if (lh->isDeviceTransferAllowed(lh))
    {
        if (lh->isDeviceTransferLimited(lh))
        {
            int transferLimit = lh->getLicenseTransferLimit(lh);
            printf("Device transfer limit = %d\n", transferLimit);
        }
        else
            printf("This license has unlimited device transfers\n");
    }
    else
        printf("Device transfer is not allowed\n");

    PrintString("Start date", lh->getLicenseStartDate(lh));

    struct tm date = lh->getLicenseExpiryDate(lh);
    printf("Expiry Date = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1, date.tm_year + 1900,
        date.tm_hour, date.tm_min, date.tm_sec);
    date = lh->getLicenseExpiryDateUtc(lh);
    printf("Expiry Date UTC = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1,
        date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
    int days = lh->getDaysRemaining(lh);
    printf("Days remaining till license expires = %d\n", days);
    date = lh->getLicenseMaintenancePeriod(lh);
    printf("Maintenance period = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1,
        date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
    date = lh->getLicenseMaintenancePeriodUtc(lh);
    printf("Maintenance period UTC = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1,
        date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
    days = lh->getMaintenanceDaysRemaining(lh);
    printf("Maintenance days remaining = %d\n", days);
    date = lh->getLicenseLastCheckDate(lh);
    printf("Last check date = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1,
        date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
    date = lh->getLicenseLastCheckDateUtc(lh);
    printf("Last check date UTC = %d-%d-%d %d:%d:%d\n", date.tm_mday, date.tm_mon + 1,
        date.tm_year + 1900, date.tm_hour, date.tm_min, date.tm_sec);
    days = lh->getDaysPassedSinceLastCheck(lh);
    printf("Days passed since last online check = %d\n", days);

    if (lh->hasLicenseFeatures(lh))
    {
        printf("Product features available for this license:\n");
        int len = lh->getLicenseFeatures(lh, 0);
        LSLicenseFeature *features = (LSLicenseFeature *)calloc(len, sizeof(LSLicenseFeature));
        lh->getLicenseFeatures(lh, features);
        for (int i = 0; i < len; i++)
        {
            const char *str = features[i].toString(&features[i]);
            printf("%s \n", str);
        }
        free(features);
    }

    if (lh->hasLicenseCustomFields(lh))
    {
        printf("Custom data fields available for this license:\n");
        int len = lh->getLicenseCustomFields(lh, 0);
        LSCustomField *fields = (LSCustomField *)calloc(len, sizeof(LSCustomField));
        lh->getLicenseCustomFields(lh, fields);
        for (int i = 0; i < len; i++)
        {
            int len = fields[i].nameSize;
            fields[i].name = (char *)calloc(len, sizeof(char));
            len = fields[i].valueSize;
            fields[i].value = (char *)calloc(len, sizeof(char));
        }
        lh->getLicenseCustomFields(lh, fields);
        for (int i = 0; i < len; i++)
        {
            printf("Data field - Name: %s , Value: %s \n", fields[i].name, fields[i].value);
            free(fields[i].name);
            free(fields[i].value);
        }
        free(fields);
    }

    if (lh->getLicenseType(lh) == LicenseTypeConsumption)
    {
        int totalConsumption = lh->getLicenseTotalConsumption(lh);
        printf("Total consumptions = %d \n", totalConsumption);

        bool unlimited = lh->isLicenseUnlimitedConsumptionAllowed(lh);
        if (unlimited)
            printf("Max consumptions = Unlimited \n");
        else
        {
            int maxConsumption = lh->getLicenseMaxConsumption(lh);
            printf("Max consumptions = %d \n", maxConsumption);
        }

        bool negative = lh->isLicenseNegativeConsumptionAllowed(lh);
        printf("Is negative consumption allowed = %s \n", negative ? "true" : "false");

        bool allowed = lh->isLicenseOveragesAllowed(lh);
        printf("Is overages allowed = %s \n", allowed ? "true" : "false");
        if (allowed)
        {
            int maxOverages = lh->getLicenseMaxOverages(lh);
            printf("Max overages = %d \n", maxOverages);
        }
    }
}

void PrintUpdateInfo(LSLicenseHandler *lh)
{
    LSLicenseID id = lh->getLicenseID(lh);
    int versionsLen = lh->getVersionList(lh, id, 0, 0);
    if (versionsLen <= 0)
        return;
    printf("\n------------- Update info -------------\n");
    printf("Total app versions available: %d \n", versionsLen);

    int *lengths = (int *)calloc(versionsLen, sizeof(int));
    char **versions = (char **)calloc(versionsLen, sizeof(char *));
    lh->getVersionList(lh, id, lengths, 0);
    for (int i = 0; i < versionsLen; i++)
        versions[i] = (char *)calloc(lengths[i], sizeof(char));
    lh->getVersionList(lh, id, lengths, versions);
    CheckIfError(lh, true);

    LSInstallationFile *installFile = lh->getInstallFile(lh, id, versions[versionsLen - 1]);
    for (int i = 0; i < versionsLen; i++)
        free(versions[i]);
    free(versions);
    free(lengths);

    CheckIfError(lh, true);
    if (installFile)
    {
        const char *version = installFile->getVersion(installFile);
        printf("Latest app version : %s\n", version);

        const char *url = installFile->getUrl(installFile);
        printf("URL for downloading: %s\n", url);

        const char *hash = installFile->getMd5Hash(installFile);
        printf("Md5 hash: %s\n", hash);

        const char *environment = installFile->getEnvironment(installFile);
        printf("Environment: %s\n", environment);

        const char *eulaLink = installFile->getEulaLink(installFile);
        printf("Eula Link: %s\n", eulaLink);

        const char *releaseNotesLink = installFile->getReleaseNotesLink(installFile);
        printf("Release Notes Link: %s\n", releaseNotesLink);

        const char *channel = installFile->getChannel(installFile);
        printf("Channel: %s\n", channel);

        printf("Size: %llu\n", installFile->getSize(installFile));

        FreeLSInstallationFile(installFile);
    }
}

void CleanUp(LSLicenseHandler *lh)
{
    if (lh->deactivateLicense(lh))
        printf("License deactivated successfully.\n");
    else
        CheckIfError(lh, true);
}

void CleanUpLocal(LSLicenseHandler *lh)
{
    const wchar_t *filePath = lh->deactivateLicenseOffline(lh, L"");
    lh->clearLocalStorage(lh);
    printf("To finish deactivation process please upload deactivation request file to the "
           "LicenseSpring portal.\n");
    wprintf(L"File path: %s \n\n", filePath);
}
