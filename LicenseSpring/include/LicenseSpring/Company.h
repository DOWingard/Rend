#ifndef COMPANY_H
#define COMPANY_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include <memory>
#include "APIDef.h"

namespace LicenseSpring
{
/// \brief Class that encapsulates information about a company on the LicenseSpring backend.
class LS_API Company
{
public:
    using ptr_t = std::shared_ptr<Company>;

    /// \brief Constructs a company with no information.
    Company() = default;

    /// \brief Constructs company with given company identifier.
    /// \param id Company identifier
    explicit Company(uint64_t id) : m_id(id) {}

    /// \brief Getter for company identifier.
    /// \return Company identifier.
    uint64_t id() const;

private:
    uint64_t m_id;
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // COMPANY_H
