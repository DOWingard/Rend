#include "UserBasedSample.h"
#include "LicenseFunctions.h"
#include <stdio.h>

void RunOnlineUserBased(LSLicenseHandler *lh, bool deactivateAndRemove)
{
    if (lh->isLicenseExists(lh))
        CheckLicenseLocal(lh);
    else
    {
        const char *userId = "someone@test.com";
        LSLicenseID licenseId = CreateLicenseIDfromUser(userId, 0); // set user credential to test
        if (!licenseId.password)
        {
            licenseId = lh->getTrialLicenseByEmail(lh, userId);
            CheckIfError(lh, true);
            printf("Got trial license for: %s\n", licenseId.user);
        }

        lh->activateLicense(lh, licenseId);
        CheckIfError(lh, true);
        printf("License activated successfully\n");
    }

    if (!lh->isLicenseExists(lh))
        return;

    UpdateAndCheckLicense(lh);

    PrintLicense(lh);

    PrintUpdateInfo(lh);

    if (deactivateAndRemove)
        CleanUp(lh);
}

void RunOfflineUserBased(LSLicenseHandler *lh, bool deactivateAndRemove)
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

            const char *userId = "someone@test.com";
            const char *userPassword = "";
            LSLicenseID id = CreateLicenseIDfromUser(userId, userPassword);

            printf("Creating offline activation request file...\n");
            const wchar_t *file = lh->createOfflineActivationFile(lh, id, L"");
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
