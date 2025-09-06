#include <LicenseSpring/C-interface/LicenseHandler.h>

void CheckIfError(LSLicenseHandler *lh, bool exitApp);

void CheckLicenseLocal(LSLicenseHandler *lh);
void UpdateAndCheckLicense(LSLicenseHandler *lh);

void PrintLicense(LSLicenseHandler *lh);
void PrintUpdateInfo(LSLicenseHandler *lh);

void CleanUp(LSLicenseHandler *lh);
void CleanUpLocal(LSLicenseHandler *lh);
