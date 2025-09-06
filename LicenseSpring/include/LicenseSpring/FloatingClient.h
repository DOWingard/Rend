#ifndef LS_FLOATING_CLIENT_H
#define LS_FLOATING_CLIENT_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include "License.h"
#include "BaseManager.h"
#include "FloatingServerInfo.h"

namespace LicenseSpring
{
/// \brief Class for performing requests to LicenseSpring Floating server.
/// \see https://docs.licensespring.com/docs/floating-server
class LS_API FloatingClient : public BaseManager
{
public:
    using ptr_t = std::shared_ptr<FloatingClient>;

    /// \brief Creates FloatingClient object and initialize it with given configuration.
    /// \param config Configuration object to use
    /// \param storage Storage for local license, by default local license is saved in a file
    /// \throw ConfigurationException If config is null
    /// \return Shared pointer to FloatingClient object.
    static ptr_t create(
        Configuration::ptr_t config, LicenseStorage::ptr_t storage = nullptr);

    /// \brief Construct FloatingClient object and initialize it with given configuration.
    /// \param config Configuration object to use
    /// \param storage Storage for local license, by default local license is saved in a file
    /// \throw ConfigurationException If config is null
    FloatingClient(Configuration::ptr_t config, LicenseStorage::ptr_t storage);

    /// \brief Destroy FloatingClient and its configuration if there is no more references to the
    /// configuration.
    ~FloatingClient() override = default;

    /// \brief Change current configuration.
    /// \param config Current configuration object to reconfigure
    /// \warning This method is not thread safe.
    void reconfigure(Configuration::ptr_t config) override;

    /// \brief Authenticates the current Floating Server V2 user and caches the JWT token.
    /// \param username Username of the user who needs to authenticate to Floating Server V2
    /// \param password Password of the user who needs to authenticate to Floating Server V2
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// \throw InvalidCredentialException If user and password combination is incorrect
    /// \note Works only on Floating server V2, and throws exceptions otherwise.
    void authenticateUser(const std::string &username, const std::string &password);

    /// \brief Fetches all available licenses from the Floating server.
    /// \details Returns a std::vector of LicenseID objects of all licenses activated on the Floating
    /// server, with the current product code.
    /// \note Works only on Floating server V2, and throws exceptions otherwise.
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw InvalidCredentialException If authentication token is missing or expired
    /// \throw LicenseSpringException If trying to fetch licenses on Floating server v1.
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    std::vector<LicenseID> getAllLicenses();

    /// \brief Register client with given id.
    /// \details Send register license request to the Floating server.
    /// \param id User or instance id for license registration, it can be anything - email, host or
    /// domain name, handle, etc.
    /// \param licenseID License identifier, optional, if not specified this method tries to
    /// register to default license for the product.
    /// \return Shared pointer to License if registration succeeded, throws exceptions in case of
    /// errors.
    /// \throw LicenseNotFloatingException In case provided license is not floating
    /// \throw ProductNotFoundException In case configured product does not exist
    /// \throw LicenseNotFoundException In case provided license was not found
    /// \throw LicenseStateException If license is disabled or experid
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw InvalidCredentialException If authentication token is missing or expired
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    const License::ptr_t registerLicense(
        const std::string &id, const LicenseID &licenseID = LicenseID());

    /// \brief Allows to register and borrow license at once.
    /// \details Can throw the same exceptions as on registration and License::borrow, see those
    /// functions for more details.
    /// \param id User or instance id for license registration, it can be anything - email, host or
    /// domain name, handle, etc.
    /// \param borrowEndDateTime Borrow end date time in format "%Y-%m-%dT%H:%M:%SZ", for example
    /// "2022-05-28T15:30:00Z"
    /// \return Shared pointer to License if registration and borrowing succeeded, throws exceptions
    /// in case of errors.
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw InvalidCredentialException If authentication token is missing or expired
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    const License::ptr_t borrowLicense(
        const std::string &id, const std::string &borrowEndDateTime = std::string());

    /// \brief Unregister client with given id.
    /// \param id User or instance id, it can be anything - email, host or domain name, handle, etc.
    /// \param licenseID License id, optional, if not specified this method unegister from default
    /// configured license for the product.
    /// \return True if license unregistered successfully.
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw InvalidCredentialException If authentication token is missing or expired
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    bool unregisterLicense(const std::string &id, const LicenseID &licenseID = LicenseID());

    /// \brief Helper method, it checks connection to the Floating server.
    /// \param throwExceptions Indicates whether this method should throw exceptions
    /// \return True if SDK is able to connect to the Floating server and false otherwise.
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    bool isOnline(bool throwExceptions = false) const;

    /// \brief Get Floating Server information (obsolete).
    /// \details This method is obsolete.
    /// \return Shared pointer to FloatingServerInfo if succeeded, throws exceptions in case of
    /// errors.
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    FloatingServerInfo::ptr_t getServerInfo();

private:
    bool m_isFloatingServerV2;

    void detectFloatingServerVersion();
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // LS_FLOATING_CLIENT_H
