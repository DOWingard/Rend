#pragma once

#include "BaseSample.h"

#include <LicenseSpring/LicenseManager.h>

class LicenseSample : public BaseSample
{
public:
    LicenseSample(LSAuthMethod autoMethod, LicenseSpring::LicenseManager::ptr_t manager);
    ~LicenseSample() override;

    void runOnline(bool deactivateAndRemove = false) override;
    void runOffline(bool deactivateAndRemove = false) override;

    void checkLicenseLocal(LicenseSpring::License::ptr_t license);

    void updateAndCheckLicense(LicenseSpring::License::ptr_t license);
    static void setupAutomaticLicenseUpdates(LicenseSpring::License::ptr_t license);
    static void setupAutomaticFloatingFeatureUpdates(LicenseSpring::License::ptr_t license);

    void cleanUp(LicenseSpring::License::ptr_t license);
    void cleanUpLocal(LicenseSpring::License::ptr_t license);

    void createOfflineActivationRequest(const LicenseSpring::LicenseID &licenseId);
    void updateOfflineLicense(LicenseSpring::License::ptr_t license);

    void printUpdateInfo();
    static void printProductVersionInfo(LicenseSpring::InstallationFile::ptr_t installFile);

private:
    LicenseSpring::LicenseManager::ptr_t m_licenseManager;
};
