#ifndef _AFX_STDAFX_H_INCLUDED_
#define _AFX_STDAFX_H_INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// ATL
#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlgdi.h>
#include <atlctrls.h>
#include <atlmisc.h>
#include <atlddx.h>

// WinAPI
#include <Shlobj.h>
#include <KnownFolders.h>

// C++ Standard
#include <string>
#include <sstream>
#include <regex>
#include <atomic>
#include <type_traits>
#include <chrono>
#include <thread>
#include <cassert>
#include <filesystem>

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _AFX_STDAFX_H_INCLUDED_