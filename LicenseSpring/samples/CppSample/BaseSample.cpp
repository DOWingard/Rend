#include "BaseSample.h"

#include <iostream>

namespace
{
std::string TmToStr(const tm &dateTime)
{
    return LicenseSpring::TmToString(dateTime, "%d-%m-%Y %H:%M:%S");
};
} // namespace

BaseSample::BaseSample(LSAuthMethod authMethod) : m_authMethod(authMethod) {}

void BaseSample::PrintLicense(LicenseSpring::License::ptr_t license)
{
    if (license == nullptr)
        return;

    std::cout << std::endl << "------------- License info -------------" << std::endl;

    auto formatStr = [](std::string &str, const std::string &value) {
        if (value.empty())
            return;
        if (!str.empty())
            str.append(" ");
        str.append(value);
    };

    std::cout << "Product code: " << license->productDetails().productCode() << std::endl;

    std::cout << "Company ID: " << license->company().id() << std::endl;

    const auto &licenseOwner = license->owner();
    {
        std::string ownerInfo; // license owner info string
        formatStr(ownerInfo, licenseOwner.firstName());
        formatStr(ownerInfo, licenseOwner.lastName());
        formatStr(ownerInfo, licenseOwner.email());
        formatStr(ownerInfo, licenseOwner.company());
        if (!ownerInfo.empty())
            std::cout << "Customer information (licensed to): " << ownerInfo << std::endl;
    }

    auto licenseUser = license->licenseUser();
    if (licenseUser)
    {
        std::string userInfo;
        formatStr(userInfo, licenseUser->firstName());
        formatStr(userInfo, licenseUser->lastName());
        formatStr(userInfo, licenseUser->email());
        if (!userInfo.empty())
            std::cout << "License user information: " << userInfo << std::endl;
    }

    if (!license->key().empty())
        std::cout << "Key = " << license->key() << std::endl;

    if (!license->user().empty())
        std::cout << "User = " << license->user() << std::endl;

    if (!license->orderStoreId().empty())
        std::cout << "Order store ID = " << license->orderStoreId() << std::endl;

    std::cout << "Type = " << license->type().toFormattedString() << std::endl;
    std::cout << "Status = " << license->status() << std::endl;
    std::cout << "IsActive = " << license->isActive() << std::endl;
    std::cout << "IsEnabled = " << license->isEnabled() << std::endl;
    std::cout << "IsTrial = " << license->isTrial() << std::endl;
    std::cout << "IsFloating = " << license->isFloating() << std::endl;
    std::cout << "Trial period for current license = " << license->trialPeriod() << std::endl;
    if (license->isFloating())
    {
        std::cout << "Current floating slots count = " << license->floatingInUseCount()
                  << std::endl;
        std::cout << "Overall floating slots count = " << license->maxFloatingUsers() << std::endl;
        auto endDate = TmToStr(license->floatingEndDateTime());
        if (license->isBorrowed())
            std::cout << "The license is borrowed until: " << endDate << std::endl;
        else
        {
            std::cout << "Registration of this floating license expires at: " << endDate
                      << std::endl;
            if (license->maxBorrowTime() > 0)
                std::cout << "License can be borrowed for " << license->maxBorrowTime()
                          << " hours max" << std::endl;
            else
                std::cout << "License borrowing is not allowed" << std::endl;
        }
    }
    std::cout << "IsOfflineActivated = " << license->isOfflineActivated() << std::endl;
    std::cout << "Times activated = " << license->timesActivated() << std::endl;
    std::cout << "Max activations = " << license->maxActivations() << std::endl;
    std::cout << "Transfer count = " << license->transferCount() << std::endl;

    if (license->isDeviceTransferAllowed())
    {
        if (license->isDeviceTransferLimited())
            std::cout << "Device transfer limit = " << license->transferLimit() << std::endl;
        else
            std::cout << "This license has unlimited device transfers" << std::endl;
    }
    else
        std::cout << "Device transfer is not allowed" << std::endl;

    if (!license->startDate().empty())
        std::cout << "Start date = " << license->startDate() << std::endl;
    std::cout << "Validity Period = " << TmToStr(license->validityPeriod()) << std::endl;
    std::cout << "Validity Period UTC = " << TmToStr(license->validityPeriodUtc()) << std::endl;
    std::cout << "Days remaining till license expires = " << license->daysRemaining() << std::endl;
    if (license->type() == LicenseTypeSubscription)
        std::cout << "Subscription grace period = " << license->subscriptionGracePeriod()
                  << std::endl;
    std::cout << "Maintenance period = " << TmToStr(license->maintenancePeriod()) << std::endl;
    std::cout << "Maintenance period UTC = " << TmToStr(license->maintenancePeriodUtc())
              << std::endl;
    std::cout << "Maintenance days remaining = " << license->maintenanceDaysRemaining()
              << std::endl;
    std::cout << "Last online check date = " << TmToStr(license->lastCheckDate()) << std::endl;
    std::cout << "Days passed since last online check = " << license->daysPassedSinceLastCheck()
              << std::endl;
    std::cout << "Metadata = " << license->metadata() << std::endl;

    auto productFeatures = license->features();
    if (!productFeatures.empty())
    {
        std::cout << "Product features available for this license:" << std::endl;
        for (auto feature : productFeatures)
            std::cout << feature.toString() << std::endl;
    }

    auto dataFields = license->customFields();
    if (!dataFields.empty())
    {
        std::cout << "Custom data fields available for this license:" << std::endl;
        for (const auto &field : dataFields)
            std::cout << "Data field - Name: " << field.fieldName()
                      << ", Value: " << field.fieldValue() << std::endl;
    }

    const auto &userData = license->userData();
    if (!userData.empty())
    {
        std::cout << "User data for this license: " << std::endl;
        for (const auto &field : userData)
            std::cout << "Data field - Name: " << field.fieldName()
                      << ", Value: " << field.fieldValue() << std::endl;
    }

    if (license->type() == LicenseTypeConsumption)
    {
        std::cout << "Total consumptions = " << license->totalConsumption() << std::endl;
        if (license->isUnlimitedConsumptionAllowed())
            std::cout << "Max consumptions = " << "Unlimited" << std::endl;
        else
            std::cout << "Max consumptions = " << license->maxConsumption() << std::endl;
        std::cout << "Is overages allowed = " << license->isOveragesAllowed() << std::endl;
        if (license->isOveragesAllowed())
            std::cout << "Max overages = " << license->maxOverages() << std::endl;
        std::cout << "Is negative consumption allowed = " << license->isNegativeConsumptionAllowed()
                  << std::endl;
    }
}