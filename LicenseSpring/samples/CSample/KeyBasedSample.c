#include "KeyBasedSample.h"
#include "LicenseFunctions.h"
#include <stdio.h>

void RunOnlineKeyBased(LSLicenseHandler *lh, bool deactivateAndRemove)
{
    if (lh->isLicenseExists(lh))
        CheckLicenseLocal(lh);
    else
    {
        LSLicenseID licenseId = CreateLicenseIDfromKey(0); // assign some license key to test
        if (!licenseId.key)
        {
            licenseId = lh->getTrialLicense(lh, 0, 0);
            CheckIfError(lh, true);
            printf("Got trial license: %s\n", licenseId.key);
        }
        lh->activateLicense(lh, licenseId);
        CheckIfError(lh, true);
        printf("License activated successfully\n");
    }

    UpdateAndCheckLicense(lh);

    PrintLicense(lh);

    PrintUpdateInfo(lh);

    if (deactivateAndRemove)
        CleanUp(lh);
}

void RunOfflineKeyBased(LSLicenseHandler *lh, bool deactivateAndRemove)
{
    if (!lh->isLicenseExists(lh))
    {
        // You can provide here path to offline activation response file,
        // or use default desktop loaction and file name ls_activation.lic
        const wchar_t *file = L"";
        lh->activateLicenseOffline(lh, file);
        if (lh->isLicenseExists(lh))
            printf("Offline activation succeeded\n");
        else
        {
            printf("Offline activation error\n");
            CheckIfError(lh, false);
            LSLicenseID licenseId = CreateLicenseIDfromKey(0); // assign some license key to test
            if (!licenseId.key)
            {
                licenseId = lh->getTrialLicense(lh, 0, 0);
                CheckIfError(lh, true);
                printf("Got trial license: %s\n", licenseId.key);
            }
            printf("Creating offline activation request file...\n");
            const wchar_t *file = lh->createOfflineActivationFile(lh, licenseId, L"");
            CheckIfError(lh, true);
            wprintf(L"File created:\n %s \n", file);
            printf("Please upload that request file to the LicenseSpring portal to get response "
                   "file.\n");
            return;
        }
    }

    CheckLicenseLocal(lh);

    PrintLicense(lh);

    if (lh->getLicenseType(lh) == LicenseTypeConsumption)
    {
        lh->updateConsumption(lh, 1, true);
        CheckIfError(lh, true);
    }

    if (deactivateAndRemove)
        CleanUpLocal(lh);
}
