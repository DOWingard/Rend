#pragma once

#include <LicenseSpring/License.h>
#include <LicenseSpring/PODTypes.h>

/// A generic “sample” that can be run online/offline.
class BaseSample {
public:
    BaseSample(LSAuthMethod authMethod);
    virtual ~BaseSample() = default;
    virtual void runOnline(bool deactivateAndRemove = false) = 0;
    virtual void runOffline(bool deactivateAndRemove = false) = 0;

    static void PrintLicense(LicenseSpring::License::ptr_t license);

protected:
    bool m_authMethod;
};