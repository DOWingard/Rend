#ifndef LS_C_EXTENDED_OPTIONS_H
#define LS_C_EXTENDED_OPTIONS_H

#include "../APIDef.h"
#include "../PODTypes.h"
#include <stdbool.h>
#include <wchar.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    LS_API typedef struct LSProxySettings
    {
        void *inner;

        void (*setProxyUri)(struct LSProxySettings *self, const char *uri);

        const char *(*getProxyUri)(struct LSProxySettings *self);

        void (*setUser)(struct LSProxySettings *self, const char *user);

        const char *(*getUser)(struct LSProxySettings *self);

        void (*setPassword)(struct LSProxySettings *self, const char *password);

        const char *(*getPassword)(struct LSProxySettings *self);

        bool (*isValid)(struct LSProxySettings *self);

    } LSProxySettings;

    LS_API LSProxySettings *CreateLSProxySettings(
        const char *proxy, const char *user, const char *password);

    LS_API void FreeLSProxySettings(LSProxySettings *settings);

    /// \brief Structure to hold a list of strings (serial numbers).
    /// \details Returned by ListAvailableHardwareKeys. Must be freed using FreeLSSerialList.
    LS_API typedef struct LSHardwareKeySerialList
    {
        /// Array of null-terminated C strings (serial numbers).
        char **serials;
        /// Number of strings in the serials array.
        int count;
    } LSSerialList;

    /// \brief Frees the memory allocated for an LSSerialList structure.
    /// \param list Pointer to the LSSerialList returned by ListAvailableHardwareKeys.
    LS_API void FreeLSSerialList(LSSerialList *list);

    /// \brief C-interface struct mirroring LicenseSpring::HardwareKeyOptions.
    LS_API typedef struct LSHardwareKeyOptions
    {
        void *inner;

        /// Sets the target YubiKey serial number.
        void (*setTargetSerial)(struct LSHardwareKeyOptions *self, const char *serial);

        /// Gets the target YubiKey serial number. Pointer is valid until next call or destruction.
        const char *(*getTargetSerial)(struct LSHardwareKeyOptions *self);

        /// Sets the PIN for the YubiKey.
        void (*setPin)(struct LSHardwareKeyOptions *self, const char *pin);

        /// Gets the PIN for the YubiKey. Pointer is valid until next call or destruction.
        const char *(*getPin)(struct LSHardwareKeyOptions *self);

    } LSHardwareKeyOptions;

    /// \brief Creates a new LSHardwareKeyOptions object.
    /// \return Pointer to the created object. Must be freed with FreeLSHardwareKeyOptions.
    LS_API LSHardwareKeyOptions *CreateLSHardwareKeyOptions();

    /// \brief Frees the memory associated with an LSHardwareKeyOptions object.
    /// \param options Pointer to the object created by CreateLSHardwareKeyOptions.
    LS_API void FreeLSHardwareKeyOptions(LSHardwareKeyOptions *options);

    /// \brief Lists the serial numbers of all connected YubiKeys with PIV support.
    /// \details Corresponds to the static C++ method HardwareKeyOptions::ListAvailableKeys.
    /// \return An LSSerialList structure containing the serial numbers.
    /// The caller is responsible for freeing this structure using FreeLSSerialList.
    /// Returns a list with count=0 on error or if no keys are found.
    LS_API LSSerialList ListAvailableHardwareKeys();

    LS_API typedef struct LSExtendedOptions
    {
        void *inner;

        void (*setLicenseFilePath)(struct LSExtendedOptions *self, const wchar_t *filePath);

        const wchar_t *(*getLicenseFilePath)(struct LSExtendedOptions *self);

        void (*setHardwareID)(struct LSExtendedOptions *self, const char *ID);

        const char *(*getHardwareID)(struct LSExtendedOptions *self);

        void (*collectNetworkInfo)(struct LSExtendedOptions *self, bool collect);

        bool (*isCollectNetworkInfoEnabled)(struct LSExtendedOptions *self);

        void (*enableLogging)(struct LSExtendedOptions *self, bool enable);

        bool (*isLoggingEnabled)(struct LSExtendedOptions *self);

        void (*enableVMDetection)(struct LSExtendedOptions *self, bool enable);

        bool (*isVMDetectionEnabled)(struct LSExtendedOptions *self);

        void (*enableGuardFile)(struct LSExtendedOptions *self, bool enable);

        bool (*isGuardFileEnabled)(struct LSExtendedOptions *self);

        void (*enableSSLCheck)(struct LSExtendedOptions *self, bool enable);

        bool (*isSSLCheckEnabled)(struct LSExtendedOptions *self);

        void (*setUseNativeTLS)(struct LSExtendedOptions *self, bool enable);

        bool (*isNativeTLSEnabled)(struct LSExtendedOptions *self);

        void (*enableLicenseCorruptionCheck)(struct LSExtendedOptions *self, bool enable);

        bool (*isLicenseCorruptionCheckEnabled)(struct LSExtendedOptions *self);

        void (*enableHardwareKey)(struct LSExtendedOptions *self, bool enable);

        bool (*isHardwareKeyEnabled)(struct LSExtendedOptions *self);

        void (*setGracePeriod)(struct LSExtendedOptions *self, uint32_t value);

        uint32_t (*getGracePeriod)(struct LSExtendedOptions *self);

        void (*setNetworkTimeout)(struct LSExtendedOptions *self, long timeout);

        long (*getNetworkTimeout)(struct LSExtendedOptions *self);

        void (*setConnectTimeout)(struct LSExtendedOptions *self, long timeout);

        long (*getConnectTimeout)(struct LSExtendedOptions *self);

        void (*setProxySettings)(
            struct LSExtendedOptions *self, const struct LSProxySettings *settings);

        const struct LSProxySettings *(*getProxySettings)(struct LSExtendedOptions *self);

        void (*setAlternateServiceURL)(struct LSExtendedOptions *self, const char *URL);

        const char *(*getAlternateServiceURL)(struct LSExtendedOptions *self);

        void (*setAlternateKey)(struct LSExtendedOptions *self, const char *key);

        void (*setAlternateKeyInt)(struct LSExtendedOptions *self, int32_t *key, int32_t size);

        enum DeviceIDAlgorithm (*getDeviceIdAlgorithm)(struct LSExtendedOptions *self);

        void (*setDeviceIdAlgorithm)(
            struct LSExtendedOptions *self, enum DeviceIDAlgorithm algorithm);

        void (*setCryptoProviderKey)(struct LSExtendedOptions *self, const char *key);

        void (*setCryptoProviderSalt)(struct LSExtendedOptions *self, const char *salt);

        void (*setHardwareKeyOptions)(
            struct LSExtendedOptions *self, const struct LSHardwareKeyOptions *options);

    } LSExtendedOptions;

    LS_API LSExtendedOptions *CreateLSExtendedOptions();

    LS_API LSExtendedOptions *CreateLSExtendedOptionsByPath(const wchar_t *licenseFilePath);

    LS_API LSExtendedOptions *CreateLSExtendedOptionsEx(
        const wchar_t *licenseFilePath, const char *hardwareID, const LSProxySettings *proxy);

    LS_API void FreeLSExtendedOptions(LSExtendedOptions *options);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // LS_C_EXTENDED_OPTIONS_H
