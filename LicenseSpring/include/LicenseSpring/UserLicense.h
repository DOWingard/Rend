#ifndef LS_USER_LICENSE_H
#define LS_USER_LICENSE_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include <memory>
#include "CustomField.h"
#include "Customer.h"
#include "LicenseType.h"
#include "ProductDetails.h"
#include "LicenseFeature.h"
#include "APIDef.h"

namespace LicenseSpring
{
/// \brief Contains information about a user-based license for a given user.
class LS_API UserLicense
{
public:
    using ptr_t = std::shared_ptr<UserLicense>;

    /// \brief Default constructor, creates empty user license object.
    UserLicense();

    /// \brief Checker for if the license is active.
    /// \return True if active, false if not.
    bool isActive() const;

    /// \brief Checker for if the license is enabled.
    /// \return True if enabled, false if not.
    bool isEnabled() const;

    /// \brief Checker for if the license is a trial.
    /// \return True if it is a trial, false if not.
    bool isTrial() const;

    /// \brief Checker if license allows unlimited activations.
    /// \return True if license allows unlimited activations, false if not.
    bool isUnlimitedActivationsAllowed() const;

    /// \brief Checker if overage is allowed for Consumption license.
    /// \details Meaningful only for Consumption license type.
    bool isOveragesAllowed() const;

    /// \brief Checker for whether the license is floating or not.
    /// \return True if floating, false if not.
    bool isFloating() const;

    /// \brief Checker for whether the license is allowed to work under virtual machine or not.
    /// \details See product and license settings on the LicenseSpring platform for more details.
    /// \return True if VM is allowed, false if not.
    bool isVMAllowed() const;

    /// \brief Checker for whether the license works only with a hardware key dongle.
    /// \return True if license works only with a hardware key dongle, false if not.
    bool isHardwareKeyAuth() const;

    /// \brief Checker for if the license is air gapped.
    /// \return True if it is air gapped, false if not.
    bool isAirGapped() const;

    /// \brief Maximum number of users that can be assigned to a license.
    /// \return Maximum number of activations for the license.
    uint32_t maxLicenseUsers() const;

    /// \brief How many times license can be activated.
    /// \return Maximum number of activations for the license, -1 if unlimited.
    int32_t maxActivations() const;

    /// \brief How many times license already been activated.
    /// \return Current number of activations for the license.
    uint32_t timesActivated() const;

    /// \brief How many times license has been transferred between devices.
    /// \return Transfer count of the license.
    uint32_t transferCount() const;

    /// \brief Maximum number of allowed license transfers between devices.
    /// \return Maximum device transfer limit of the license, -1 for unlimited device transfers, 0
    /// if device transfers are not allowed.
    int32_t transferLimit() const;

    /// \brief Maximum overage allowed for the license.
    /// \details Meaningful only for Consumption license type, returns maximum possible value in
    /// case of unlimited overages.
    uint32_t maxOverages() const;

    /// \brief Timeout of the floating license in minutes.
    /// \return Timeout of the floating license in minutes.
    uint32_t floatingTimeout() const;

    /// \brief Maximum borrowing period in hours.
    /// \details Meaningful only for floating license.
    /// \return Max borrow time or 0 if license borrowing is not allowed.
    uint32_t maxBorrowTime() const;

    /// \brief Grace period in hours.
    /// \details Positive value if grace period enabled, and 0 if disabled.
    uint32_t gracePeriod() const;

    /// \brief Server-side license id, used for distinguishing user-based licenses of the same
    /// product.
    /// \return server-side license id.
    uint64_t serverId() const;

    /// \brief order store id of the license.
    /// \return Order store id.
    const std::string &orderStoreId() const;

    /// \brief License start date.
    /// \details See https://docs.licensespring.com/docs/license-start-date
    /// \return License start date in a format %Y-%m-%d, if start date is not set for the License
    /// the string is empty.
    const std::string &startDate() const;

    /// \brief License metadata.
    /// \return String of license metadata json.
    const std::string &metadata() const;

    /// \brief License type.
    /// \return LicenseType object.
    LicenseType licenseType() const;

    /// \brief License owner.
    /// \return Information about license owner, commonly it's a person or organization associated
    /// with the license.
    Customer owner() const;

    /// \brief Product details.
    /// \return Information about the product.
    ProductDetails productDetails() const;

    /// \brief List of custom fields associated with the license.
    /// \return List of CustomField objects associated with the license.
    std::vector<CustomField> customFields() const;

    /// \brief List of license features associated with the license.
    /// \return List of LicenseFeature objects associated with the license.
    std::vector<LicenseFeature> features() const;

    /// \brief License validity period in UTC.
    /// \return Time structure of license validity period in UTC.
    tm validityPeriod() const;

    /// \brief Maintenance period in UTC.
    /// \return Time structure of maintenance period in UTC.
    tm maintenancePeriod() const;

    /// \brief Helper method, creates UserLicense object from JSON string.
    /// \return UserLicense object from JSON string.
    static UserLicense fromJsonString(const std::string &jsonString);

private:
    bool m_isActive;
    bool m_isEnabled;
    bool m_isTrial;
    bool m_allowUnlimitedActivations;
    bool m_allowOverages;
    bool m_isFloating; // cloud
    bool m_isOfflineFloating;
    bool m_preventVM;
    bool m_isHardwareKeyAuth;
    bool m_isAirGapped;
    uint32_t m_maxLicenseUsers;
    int32_t m_maxActivations;
    uint32_t m_timesActivated;
    uint32_t m_transferCount;
    int32_t m_transferLimit;
    uint32_t m_maxOverages;
    uint32_t m_floatingTimeout;
    uint32_t m_maxBorrowTime;
    uint32_t m_gracePeriod;
    uint64_t m_serverId;
    std::string m_orderStoreId;
    std::string m_startDate;
    std::string m_metadata;
    tm m_validityPeriod;
    tm m_maintenancePeriod;

    LicenseType m_licenseType;
    Customer m_customer;
    ProductDetails m_productDetails;
    std::vector<CustomField> m_customFields;
    std::vector<LicenseFeature> m_features;
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // LS_USER_LICENSE_H
