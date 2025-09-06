#include "stdafx.h"
#include "utils.h"

#pragma warning(push)
#pragma warning(disable : 4996)

using namespace std;

namespace WTLSample::Utils
{

string ws2s(const wstring &wstr) { return wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(wstr); }

wstring s2ws(const string &str) { return wstring_convert<codecvt_utf8<wchar_t>>().from_bytes(str); }

void ShowInfoMsgBox(const std::string &infoMsg, HWND parentWnd)
{
    MessageBoxA(parentWnd, infoMsg.data(), "License notification", MB_OK | MB_ICONINFORMATION);
}

void ShowErrorMsgBox(const std::string &errorMsg, HWND parentWnd)
{
    MessageBoxA(parentWnd, errorMsg.data(), "Error", MB_OK | MB_ICONERROR);
}

void ShowWarningMsgBox(const std::string &warningMsg, HWND parentWnd)
{
    MessageBoxA(parentWnd, warningMsg.data(), "Warning", MB_OK | MB_ICONWARNING);
}

string GetWinError(const char *functionName, unsigned long *code)
{
    DWORD errorCode = GetLastError();
    if (code)
        *code = errorCode;

    string title(functionName);

    if (errorCode == ERROR_SUCCESS)
    {
        if (!title.empty())
            title.append(" : ");
        title.append("operation completed successfully");
        return title;
    }

    LPTSTR msgBuf = nullptr;
    DWORD formatResult = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&msgBuf, 0, NULL);

    if (formatResult == 0)
        GetWinError("FormatMessage");

    std::wstring errorText = std::wstring(CT2CW(msgBuf));
    LocalFree(msgBuf);
    msgBuf = nullptr;

    if (title.empty())
        title = "System error";
    else
        title.append(" failed with error");

    stringstream strs;
    strs << title << ' ' << errorCode << " (0x" << hex << errorCode << ')' << " : "
         << ws2s(errorText);
    return strs.str();
}

} // namespace WTLSample::Utils

#pragma warning(pop)
