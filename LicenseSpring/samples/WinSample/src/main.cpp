#include "stdafx.h"
#include "UI/activation_dialog.h"

CAppModule _Module;

int WINAPI _tWinMain(
    HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    HRESULT hRes = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    ATLASSERT(SUCCEEDED(hRes));

    AtlInitCommonControls(ICC_STANDARD_CLASSES);
    _Module.Init(NULL, hInstance);

    int returnCode = 0;
    {
        WTLSample::UI::ActivationDialog dlg;
        returnCode = (int)dlg.DoModal();
    }

    _Module.Term();
    CoUninitialize();

    return returnCode;
}