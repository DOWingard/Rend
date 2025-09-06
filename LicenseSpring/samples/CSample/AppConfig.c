#include "AppConfig.h"

// Provide your LicenseSpring credentials here, please keep them safe
const char *ApiKey = "";       // your LicenseSpring API key (UUID)
const char *SharedKey = "";    // your LicenseSpring Shared key
const char *ClientId = "";     // OAuth client_id
const char *ClientSecret = ""; // OAuth client_secret
const char *ProductCode = ""; // product code that you specified in LicenseSpring for your application

LSConfiguration *CreateLicenseSpringConfig()
{
    // Optionally you can provide full path where license file will be stored, hardwareID and other
    // options
    LSExtendedOptions *options = CreateLSExtendedOptions();
    options->collectNetworkInfo(options, true);
    // options->enableLogging( options, true );
    // options->enableVMDetection( options, true );

    // list all smart card serial numbers and pick one
    // LSSerialList hardwareKeySerials = ListAvailableHardwareKeys();
    // LSHardwareKeyOptions *hwKeyOptions = CreateLSHardwareKeyOptions();
    // hwKeyOptions->setPin(hwKeyOptions, "123456");  // 123456 is the default pin
    // hwKeyOptions->setTargetSerial(hwKeyOptions, "87654321"); // insert the correct serial number, e.g. hardwareKeySerials[0]
    // options->setHardwareKeyOptions(options, hwKeyOptions);
    // FreeLSSerialList(&hardwareKeySerials);
    // FreeLSHardwareKeyOptions(hwKeyOptions);

    // Uncomment the following block and provide your OAuth credentials if you want to use OAuth
    // In that case, you must provide your own password for the license file encryption key
    // generation It's preferred to read these values from somewhere on startup rather than
    // embedding them in the binary

    // options->setCryptoProviderKey(options, "this is a placeholder key! use your own for production!");
    // LSConfiguration* pConfiguration =
    //    CreateLSConfigurationOAuth(ClientId, ClientSecret, ProductCode, APP_NAME, APP_VERSION, options);

    // Comment or remove this function call if using OAuth
    LSConfiguration *pConfiguration =
        CreateLSConfiguration(ApiKey, SharedKey, ProductCode, APP_NAME, APP_VERSION, options);

    FreeLSExtendedOptions(options);

    return pConfiguration;
}
