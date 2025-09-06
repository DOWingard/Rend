#ifndef LS_USER_LICENSE_DATA_H
#define LS_USER_LICENSE_DATA_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include <memory.h>
#include "UserLicense.h"
#include "LicenseUser.h"
#include "APIDef.h"

namespace LicenseSpring
{
/// \brief Contains information about a license assigned to a user for the current product.
class LS_API UserLicensesData
{
public:
    using ptr_t = std::shared_ptr<UserLicensesData>;

    /// \brief Default constructor, creates empty object.
    UserLicensesData();

    /// \brief Constructs UserLicensesData with given UserLicense and LicenseUsers pointers.
    /// \param userLicense Shared pointer to UserLicense
    /// \param licenseUser Shared pointer to LicenseUser
    UserLicensesData(const UserLicense::ptr_t &userLicense, const LicenseUser::ptr_t &licenseUser);

    /// \brief License data.
    /// \return UserLicense object.
    UserLicense::ptr_t userLicense() const;

    /// \brief User data.
    /// \return LicenseUser object.
    LicenseUser::ptr_t licenseUser() const;

private:
    UserLicense::ptr_t m_license;
    LicenseUser::ptr_t m_user;
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // LS_USER_LICENSE_DATA_H