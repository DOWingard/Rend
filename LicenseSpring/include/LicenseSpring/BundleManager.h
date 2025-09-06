#ifndef BUNDLEMANAGER_H
#define BUNDLEMANAGER_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include "BaseManager.h"

namespace LicenseSpring
{
/// \brief Class for performing basic requests to LicenseSpring backend and managing license
/// bundles.
class LS_API BundleManager : public BaseManager
{
public:
    using ptr_t = std::shared_ptr<BundleManager>;
    using BaseManager::getSSOUrl;
    using BaseManager::createOfflineActivationFile;

    /// \brief Creates BundleManager object and initializes it with given configuration.
    /// \param config Configuration object to use
    /// \param storage Storage for local license, by default local license is saved in a file
    /// \throw ConfigurationException If config is null, or cryptography key is empty in case of
    /// OAuth authentication
    /// \return Shared pointer to BundleManager object.
    static ptr_t create(Configuration::ptr_t config, LicenseStorage::ptr_t storage = nullptr);

    /// \brief Construct BundleManager object and initialize it with given configuration.
    /// \throw ConfigurationException If config is null
    BundleManager(Configuration::ptr_t config, LicenseStorage::ptr_t storage = nullptr);

    /// \brief Destroy bundle and its configuration if there is no more references to the
    /// configuration.
    ~BundleManager() override = default;

    /// \brief  Load the local license bundle into memory (lazily).
    /// \details The first time this is called, it reads the bundle file from disk decrypts it and
    /// constructs a map of product-code → License instances. Subsequent calls simply return the
    /// cached map.
    /// \return A const reference to the map of product code to License::ptr_t.
    /// \throws LocalLicenseException if the bundle file cannot be read or is corrupted.
    const std::map<std::string, License::ptr_t> &getCurrentBundle();

    /// \brief Activate license bundle with the given LicenseID.
    /// \details Send activate bundle request to the backend, creates a local license bundle file if
    /// successful.
    /// \param licenseID License identifier that needs to be activated
    /// \param deviceVariables An std::vector of device variables to be sent to the LicenseSpring
    /// \return A const reference to the map of product code to License::ptr_t.
    /// \throw InvalidCredentialException If user and password combination is incorrect
    /// \throw ProductNotFoundException In case configured product does not exist
    /// \throw LicenseNotFoundException In case provided license was not found
    /// \throw ProductNotBundleException Configured product is not a bundle.
    /// \throw BundleMismatchException In case activation response contains wrong product codes.
    /// \throw LicenseStateException If bundle is disabled or expired
    /// \throw LicenseNoAvailableActivationsException If bundle was already activated maximum
    /// number of times
    /// \throw CannotBeActivatedNowException If start_date field is set for the bundle and current
    /// date is behind start date
    /// \throw SignatureMismatchException In case signature returned by LicenseSpring server is not
    /// valid
    /// \throw NoInternetException In case the server is unreachable
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    const std::map<std::string, License::ptr_t> &activateBundle(const LicenseID &licenseID,
        const std::vector<DeviceVariable> &deviceVariables = std::vector<DeviceVariable>());

    /// \brief Activate bundle using SSO.
    /// \details You can omit accountCode param if you already set customer account code through the
    /// Configuration.
    /// \param authData id_token or code you get after user authorization with SSO
    /// \param accountCode Customer account code
    /// \param useAuthCode Whether to use auth code instead of id_token for activation
    /// \return A const reference to the map of product code to License::ptr_t.
    /// errors.
    /// \throw SSOException In case customer was not found or SSO is not enabled
    /// \throw SSOTokenException In case SSO token is invalid or expired
    /// \throw ProductNotFoundException In case configured product does not exist
    /// \throw ProductNotBundleException Configured product is not a bundle.
    /// \throw BundleMismatchException In case activation response contains wrong product codes.
    /// \throw LicenseNotFoundException In case provided license was not found
    /// \throw LicenseStateException If license is disabled or experid
    /// \throw LicenseNoAvailableActivationsException If license was already activated maximum
    /// number of times
    /// \throw CannotBeActivatedNowException If start_date field is set for the license and current
    /// date is behind start date
    /// \throw SignatureMismatchException In case signature returned by LicenseSpring server is not
    /// valid
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    /// \note There are multiple methods with the same name and different parameters.
    const std::map<std::string, License::ptr_t> &activateBundle(const std::string &authData,
        const std::string &accountCode = std::string(), bool useAuthCode = true);

    /// \brief Activate bundle offline with provided activation file.
    /// \details Default location for activation files is Desktop location, default file name is
    /// 'ls_activation.lic'.
    /// \param activationResponseFile Activation response file path, empty means use default
    /// location and file name
    /// \return A const reference to the map of product code to License::ptr_t.
    /// \throw SignatureMismatchException In case signature inside activation file is not valid or
    /// license does not belong to current device.
    /// \throw LocalLicenseException If invalid activation file provided
    /// \throw ProductMismatchException If license product code does not correspond to configuration
    /// product code
    /// \throw DeviceNotLicensedException If license file does not belong to current device.
    /// \throw BundleMismatchException In case activation response contains wrong product codes.
    const std::map<std::string, License::ptr_t> &activateBundleOffline(
        const std::wstring &activationResponseFile = std::wstring());

    /// \brief Sends deactivate bundle request to the backend
    /// \param removeLocalData If true this method will remove local bundle file and folders
    /// created by the SDK.
    /// \return True if successfully deactivated, false otherwise.
    /// \throw LicenseNotFoundException In case provided license was not found,
    /// for example removed by admin using the platform or management API)
    /// \throw LicenseStateException In case license is disabled or already inactive
    /// \throw DeviceNotLicensedException In case license does not belong to current computer
    /// \throw DeviceBlacklistedException If device has been blacklisted by product manager
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw LicenseServerException In case of internal server error, the backend returned http
    /// error code >= 500
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    bool deactivateBundle(const LicenseID &licenseID, bool removeLocalData = false);

    /// \brief Creates offline bundle deactivation request file.
    /// \param licenseID License bundle identifier
    /// \param deactivationRequestFile Deactivation request file path, null means use default path
    /// (Desktop)/ls_deactivation.req
    /// \return Path to offline activation request file.
    std::wstring deactivateBundleOffline(
        const LicenseID &licenseID, const std::wstring &deactivationRequestFile = std::wstring());

    /// \brief Online bundle check, which syncs the bundle with the backend. Throws exceptions in
    /// case of errors.
    /// \param licenseID License bundle identifier
    /// \param filter Optional, you can filter install files by environment and channel.
    /// \param includeExpiredFeatures Optional, when set to true, the license check includes expired
    /// license features.
    /// \throw LicenseStateException In case bundle disabled, was reset or expired
    /// \throw DeviceNotLicensedException In case bundle does not belong to current computer
    /// \throw DeviceBlacklistedException If device has been blacklisted by product manager
    /// \throw MaxFloatingReachedException If no more floating slots are available
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw BundleMismatchException In case activation response contains wrong product codes.
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    void checkBundle(const LicenseID &licenseID,
        const InstallFileFilter &filter = InstallFileFilter(), bool includeExpiredFeatures = false);

protected:
    /// \brief Load zero‐or‐more License instances from disk.
    /// \details Default in BaseManager returns a single‐license vector; here we override to load an
    /// entire bundle.
    /// \return A map of product code to License::ptr_t.
    std::map<std::string, License::ptr_t> loadLicensesFromStorage() override;
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // BUNDLEMANAGER_H
