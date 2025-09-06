#pragma once

#include "BaseSample.h"

#include <LicenseSpring/BundleManager.h>

class BundleSample : public BaseSample
{
public:
    BundleSample(LSAuthMethod authMethod, LicenseSpring::BundleManager::ptr_t manager);
    ~BundleSample() override;

    void runOnline(bool deactivateAndRemove = false) override;
    void runOffline(bool deactivateAndRemove = false) override;
private:
    LicenseSpring::BundleManager::ptr_t m_bundleManager;
};
