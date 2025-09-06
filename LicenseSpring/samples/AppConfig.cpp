#include "AppConfig.h"
#include <LicenseSpring/EncryptString.h>

LicenseSpring::Configuration::ptr_t AppConfig::createLicenseSpringConfig() const
{
    // Optionally you can provide full path where license file will be stored, hardwareID and other
    // options
    LicenseSpring::ExtendedOptions options;
    options.collectNetworkInfo(true);
    // options.enableLogging( true );
    // options.enableVMDetection( true );

    // std::vector<std::string> hardwareKeySerials = LicenseSpring::HardwareKeyOptions::ListAvailableKeys();
    // LicenseSpring::HardwareKeyOptions hwKeyOptions;
    // hwKeyOptions.setPin("123456"); // 123456 is the default pin
    // hwKeyOptions.setTargetSerial("87654321"); // insert the correct serial number, e.g. hardwareKeySerials[0]
    // options.setHardwareKeyOptions(hwKeyOptions);

    // In order to connect to the LS FloatingServer set its address as following
    // options.setAlternateServiceURL( "http://localhost:8080" );
    // If you use FloatingServer v1.1.9 or earlier provide Product name instead of Product code

    // Uncomment the following block call and provide your OAuth credentials if you want to use
    // OAuth In that case, you must provide your own password for the license file encryption key
    // generation It's preferred to read these values from somewhere on startup rather than
    // embedding them in the binary

    // options.getCryptoProvider()->setKey( EncryptStr( "this is a placeholder key! use your own for production!" ) );
    // return LicenseSpring::Configuration::CreateOAuth(
    //     EncryptStr( "" ), // OAuth client_id
    //     EncryptStr( "" ), // OAuth client_secret
    //     EncryptStr( "" ), // product code that you specified in LicenseSpring for your application
    //     appName, appVersion, options );

    // Provide your LicenseSpring credentials here, please keep them safe
    return LicenseSpring::Configuration::Create(
        EncryptStr(""), // your LicenseSpring API key (UUID)
        EncryptStr(""), // your LicenseSpring Shared key
        EncryptStr(""), // product code that you specified in LicenseSpring for your application
        appName, appVersion, options);
}
