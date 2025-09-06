#include "KeyBasedSample.h"
#include "UserBasedSample.h"
#include "LicenseFunctions.h"
#include "AppConfig.h"
#include <stdio.h>
#include <stdlib.h>

void PrintConfigInfo(LSConfiguration *pConfiguration)
{
    printf("------------- General info -------------\n");
    const char *appName = pConfiguration->getAppName(pConfiguration);
    const char *appVersion = pConfiguration->getAppVersion(pConfiguration);
    printf("%s  %s\n", appName, appVersion);

    printf("LicenseSpring SDK version: %s\n", pConfiguration->getSdkVersion(pConfiguration));

    int version = pConfiguration->getLicenseSpringAPIVersion(pConfiguration);
    printf("LicenseSpring API version: %d\n", version);

    const char *osVersion = pConfiguration->getOsVersion(pConfiguration);
    printf("Determined OS version: %s\n\n", osVersion);

    printf("------------- Network info -------------\n");
    const char *hostName = pConfiguration->getHostName(pConfiguration);
    printf("Host name: %s\n", hostName);

    const char *ip = pConfiguration->getIP(pConfiguration);
    printf("Local IP:  %s\n", ip);

    const char *mac = pConfiguration->getMAC(pConfiguration);
    printf("MAC address:  %s\n\n", mac);
}

void PrintProductInfo(LSProductDetails *productInfo)
{
    printf("------------- Product info -------------\n");
    const char *productName = productInfo->productName(productInfo);
    printf("Product name: %s\n", productName);

    bool allowed = productInfo->isVMAllowed(productInfo);
    printf("Virtual machines allowed: %s\n", allowed ? "true" : "false");

    allowed = productInfo->isTrialAllowed(productInfo);
    printf("Trial allowed: %s\n", allowed ? "true" : "false");

    int timeout = productInfo->floatingLicenseTimeout(productInfo);
    if (timeout > 0)
        printf("Floating license timeout:  %d min\n", timeout);

    if (productInfo->isTrialAllowed(productInfo))
    {
        int trialDays = productInfo->trialPeriod(productInfo);
        if (trialDays > 0)
            printf("Trial period:  %d day%s\n", trialDays, trialDays > 1 ? "s" : "");
    }

    if (productInfo->authorizationMethod(productInfo) == AuthMethodKeyBased)
        printf("Authorization method: Key-based\n\n");
    else
        printf("Authorization method: User-based\n\n");

    // product-level custom fields
    int len = productInfo->customFields(productInfo, 0);
    if (len > 0)
    {
        printf("Custom data fields available for this product:\n");
        LSCustomField *fields = (LSCustomField *)calloc(len, sizeof(LSCustomField));
        productInfo->customFields(productInfo, fields);
        for (int i = 0; i < len; i++)
        {
            int len = fields[i].nameSize;
            fields[i].name = (char *)calloc(len, sizeof(char));
            len = fields[i].valueSize;
            fields[i].value = (char *)calloc(len, sizeof(char));
        }
        productInfo->customFields(productInfo, fields);
        for (int i = 0; i < len; i++)
        {
            printf("Data field - Name: %s , Value: %s \n", fields[i].name, fields[i].value);
            free(fields[i].name);
            free(fields[i].value);
        }
        free(fields);
    }
}

int main()
{
    LSConfiguration *pConfiguration = CreateLicenseSpringConfig();

    PrintConfigInfo(pConfiguration);

    bool deactivateAndRemove = false; // deactivate and remove license at the end
    LSLicenseHandler *lh = CreateLSLicenseHandler(pConfiguration);

    bool includeLatestVersion = true;
    bool includeCustomFields = true;
    const char *env = "";
    LSProductDetails *productInfo = lh->getProductDetailsWithCustomFieldsAndEnv(
        lh, includeLatestVersion, includeCustomFields, env);
    // old getter
    // LSProductDetails* productInfo = lh->getProductDetails( lh, true );

    CheckIfError(lh, true);
    PrintProductInfo(productInfo);
    int productAuthMethod = productInfo->authorizationMethod(productInfo);
    FreeLSProductDetails(productInfo);

    if (pConfiguration->isVMDetectionEnabled(pConfiguration))
    {
        printf("Checking for virtual machines...\n");
        if (pConfiguration->isVM(pConfiguration))
        {
            printf("Virtual machine detected!\n");
            const char *vmName = pConfiguration->getDetectedVMName(pConfiguration);
            printf("Hypervisor name: %s \n\n", vmName);
        }
        else
            printf("Check passed, VM not detected.\n\n");
    }

    if (productAuthMethod == AuthMethodKeyBased)
    {
        if (lh->isOnline(lh))
            RunOnlineKeyBased(lh, deactivateAndRemove);
        else
            RunOfflineKeyBased(lh, deactivateAndRemove);
    }
    else
    {
        if (lh->isOnline(lh))
            RunOnlineUserBased(lh, deactivateAndRemove);
        else
            RunOfflineUserBased(lh, deactivateAndRemove);
    }

    FreeLSLicenseHandler(lh);
    return 0;
}
