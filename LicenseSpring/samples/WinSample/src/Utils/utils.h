#pragma once

#include <string>
#include <minwindef.h>

namespace WTLSample::Utils
{

std::string ws2s(const std::wstring &wstr);
std::wstring s2ws(const std::string &str);

void ShowInfoMsgBox(const std::string &infoMsg, HWND parentWnd = NULL);
void ShowErrorMsgBox(const std::string &errorMsg, HWND parentWnd = NULL);
void ShowWarningMsgBox(const std::string &warningMsg, HWND parentWnd = NULL);

std::string GetWinError(const char *functionName = "", unsigned long *code = nullptr);

} // namespace WTLSample::Utils
