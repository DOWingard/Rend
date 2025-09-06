#ifndef LS_BASE_MANAGER_H
#define LS_BASE_MANAGER_H

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

#include <map>

#include "Configuration.h"
#include "ProductDetails.h"
#include "UserLicensesData.h"
#include "License.h"
#include "LicenseStorage.h"

namespace LicenseSpring
{
class LicenseService;
class DataHandler;

/// \brief Base class for license management.
/// \details Used to manipulate configuration, product details, current license, data location,
/// license file name and path.\n Extended to FloatingClient.h and LicenseManager.h.
class LS_API BaseManager
{
public:
    /// \brief Create and intialize license manager with given configuration.
    /// \param config Configuration object to use
    /// \param storage Storage for local license, by default local license is saved in a file
    /// \throw ConfigurationException If provided config is null
    BaseManager(Configuration::ptr_t config, LicenseStorage::ptr_t storage = nullptr);
    BaseManager(const BaseManager &) = delete;
    BaseManager &operator=(const BaseManager &) = delete;
    /// \brief Destroy LicenseManger with current configuration
    virtual ~BaseManager();

    /// \brief Returns current configuration.
    /// \return Shared pointer to current configuration.
    Configuration::ptr_t currentConfig() const;

    /// \brief Change current configuration.
    /// \param config Current configuration object to reconfigure
    /// \warning This method is not thread safe.
    virtual void reconfigure(Configuration::ptr_t config);

    /// \brief Get basic information from the backend on product which corresponds to the current
    /// configuration.
    /// \param includeLatestVersion If true, also returns latest_version field
    /// \return ProductDetails object of product code given.
    /// \throw ProductNotFoundException If product is not found on the backend of account with
    /// API/shared key combination
    /// \throw ConfigurationException In case of API/shared key combination not found on server
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw SignatureMismatchException In case signature returned by LicenseSpring server is not
    /// valid
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, for example
    /// unexpected server response
    /// \note There are multiple methods with the same name and different parameters.
    ProductDetails getProductDetails(bool includeLatestVersion = false);

    /// \brief Get basic information from the backend on product which corresponds to the current
    /// configuration.
    /// \param includeLatestVersion If true, also returns latest_version field
    /// \param includeCustomFields If true, also returns custom fields data
    /// \param env Filter products by environment string (as defined by installation file
    /// "environment")
    /// \return ProductDetails object of product code given.
    /// \throw ProductNotFoundException If product is not found on the backend of account with
    /// API/shared key combination
    /// \throw ConfigurationException In case of API/shared key combination not found on server
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw LicenseServerException In case of internal server error
    /// \throw SignatureMismatchException In case signature returned by LicenseSpring server is not
    /// valid
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, for example
    /// unexpected server response
    /// \note There are multiple methods with the same name and different parameters.
    ProductDetails getProductDetails(
        bool includeLatestVersion, bool includeCustomFields, const std::string &env);

    /// \brief Checks if license file is damaged or corrupted.
    /// \details Same as BaseManager::getCurrentLicense, but doesn't load license file into memory.
    /// \return True if license file is damaged or corrupted and false otherwise.
    /// \throw LocalLicenseException Thrown if could not read license file
    bool isLicenseFileCorrupted();

    /// \brief Load license from local storage (if not yet loaded).
    /// \details If license has been already loaded into memory, this method just returns existing
    /// pointer to the license.
    /// \return Shared pointer to license or null if no license exists for current configuration and
    /// storage.
    /// \throw LocalLicenseException Thrown if could not read license file or if local license
    /// corrupted
    const License::ptr_t getCurrentLicense();

    /// \brief Resets current license and loads it again from local storage.
    /// \return Shared pointer to license or null if no license exists for current configuration and
    /// storage.
    /// \throw LocalLicenseException Thrown if could not read license file or if local license
    /// corrupted
    const License::ptr_t reloadLicense();

    /// \brief Remove local license, other files and folders created by LicenseSpring, usually
    /// should be called at app uninstallation.
    /// \details This method also removes current license from memory.
    void clearLocalStorage();

    /// \brief Absolute folder path where LicenseSpring data is stored.
    /// \details Data like: log file, temp offline activation data, local license file in case
    /// LicenseFileStorageBase (or derived) is used.\n Default SDK data location on Windows is:
    /// {SystemDrive}:/Users/{UserName}/AppData/Local/LicenseSpring/{ProductCode}\n Default SDK data
    /// location on Linux is: HOME/.LicenseSpring/LicenseSpring/{ProductCode}\n Default SDK data
    /// location on MAC is: ~/Library/Application Support/LicenseSpring/{ProductCode}
    /// \return Absolute folder path (location) of the SDK data.
    const std::wstring &dataLocation() const;

    /// \brief Set absolute or relative path to folder where LicenseSpring data should be stored.
    /// \details If you want set data location to current directory use ".", empty string results in
    /// reset to default location (see dataLocation()). If path provided does not exist, the folders
    /// are created to match the path given.
    /// \param path Absolute or relative folder path
    /// \warning If there's a license in the new location, it will be set as current license,
    /// otherwise the current license will be removed from memory.
    void setDataLocation(const std::wstring &path);

    /// \brief Return license file name with extension, unless filename was explicitly set to be
    /// without extension.
    /// \details Meaningful only if LicenseFileStorageBase (or derived) is used. Default filename is
    /// %License.key.
    /// \return License file name.
    std::wstring licenseFileName() const;

    /// \brief Set license file name, it can be with or without extension.
    /// \details Meaningful only if LicenseFileStorageBase (or derived) is used. Default filename is
    /// %License.key.
    /// \param name License file name
    /// \warning This method also removes current license from memory.
    void setLicenseFileName(const std::wstring &name);

    /// \brief Return concatenated dataLocation() and licenseFileName().
    /// \details If currently using not LicenseFileStorageBase (or derived), then it is the same as
    /// dataLocation().
    /// \return Absolute license file path.
    std::wstring licenseFilePath() const;

protected:
    Configuration::ptr_t m_config;
    std::shared_ptr<DataHandler> m_dataHandler;
    std::shared_ptr<LicenseService> m_pLicenseService;
    std::map<std::string, License::ptr_t> m_cachedLicenses;

    /// \brief Get URL for Single sign-on bundle activation.
    /// \details You can omit accountCode param if you already set customer account code through the
    /// Configuration.
    /// \param accountCode Customer account code
    /// \param useAuthCode Boolean value indicating whether this method should create the
    /// redirect_uri with auth code instead of id_token.
    /// \return URL for Single sign-on.
    /// \throw SSOException In case customer was not found or SSO is not enabled
    /// \throw ProductNotFoundException In case configured product does not exist
    /// \throw NoInternetException If cannot reach the server
    /// \throw NetworkTimeoutException In case of network timeout
    /// \throw OAuthException In case OAuth credentials are invalid or expired
    /// \throw HardwareKeyException In case hardware key dongle is missing or misconfigured
    /// \throw LicenseServerException In case of internal server error
    /// \throw UnknownLicenseSpringException In rare case if something went wrong, please contact
    /// support
    std::string getSSOUrl(const std::string &accountCode = std::string(), bool useAuthCode = true);

    /// \brief Creates offline activation file request.
    /// \param licenseID License or bundle identifier
    /// \param activationRequestFile Activation request file path, null means use default path
    /// (Desktop)/ls_activation.req
    /// \return Path to offline activation request file.
    /// \note There are multiple methods with the same name and different parameters.
    std::wstring createOfflineActivationFile(
        const LicenseID &licenseID, const std::wstring &activationRequestFile = std::wstring());

    /// \brief Creates offline activation file request.
    /// \param licenseID License identifier
    /// \param deviceVariables An std::vector of device variables to be sent to the LicenseSpring
    /// platform during license activation
    /// \param activationRequestFile Activation request file path, null means use default path
    /// (Desktop)/ls_activation.req
    /// \return Path to offline activation request file.
    /// \note There are multiple methods with the same name and different parameters.
    std::wstring createOfflineActivationFile(const LicenseID &licenseID,
        const std::vector<DeviceVariable> &deviceVariables,
        const std::wstring &activationRequestFile = std::wstring());

    /// \brief Load zero or more Licenses from storage.
    /// \details This method should load a single license from storage. Derived classes might load multiple licenses.
    /// \return std::map, where keys are product codes, and values are shared pointers to License.
    virtual std::map<std::string, License::ptr_t> loadLicensesFromStorage();

    /// \brief Reset (delete) members
    void reset();
};
} // namespace LicenseSpring

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // LS_BASE_MANAGER_H
