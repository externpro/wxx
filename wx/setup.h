/*
 *  Copyright (C) 2010 Utah State University Research Foundation.
 *  All rights reserved.  No part of this program may be
 *  photocopied, reproduced, or translated to another program
 *  language without the prior written consent of Utah State
 *  University Research Foundation.
 *
 *  Filename: setup.h
 *  Description: This header is included by wxx source code. It includes the
 *    appropriate setup.h
 *  Author: Scott M Anderson
 */

#ifdef _MSC_VER
# ifdef _UNICODE
#  error "Unicode support missing in wxWidgets/wxx/wx/setup.h"
# endif
# ifdef WIN64
#  ifdef _DEBUG
#   include "../../lib/vc_extern_x64/mswd/wx/setup.h"
#  else // NDEBUG
#   include "../../lib/vc_extern_x64/msw/wx/setup.h"
#  endif
# else // WIN32
#  ifdef _DEBUG
#   include "../../lib/vc_extern_win32/mswd/wx/setup.h"
#  else // NDEBUG
#   include "../../lib/vc_extern_win32/msw/wx/setup.h"
#  endif
# endif
#else
# error "This file should only be included when using Microsoft Visual C++"
#endif // _MSC_VER

#ifndef WXXLIBS_DEFINED
#define WXXLIBS_DEFINED

// wxWidgets includes <windows.h> in their wx/msw/wrapwin.h header - and they
// do a good job of defining some things (like NOMINMAX) in wrapwin.h, but
// there is one thing we've found that we need to define that they don't --
// windows.h includes winsock.h (WinSock 1.1). This conflicts with includes of
// winsock2.h. Define _WINSOCKAPI_ here so the preprocessor thinks winsock.h
// is already included.
#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <wx/version.h>
#if wxVERSION_NUMBER >= 2900
# error "No support (yet) for versions greater than wxWidgets 2.8"
#elif wxVERSION_NUMBER >= 2800
# define wxVersion "28"
#else
# error "Please update to at least wxWidgets 2.8!"
#endif

#ifdef _UNICODE
# define wxUnicode "u"
#else
# define wxUnicode ""
#endif

#if defined _DEBUG
# define wxRuntime "d"
#elif defined _DLL
# define wxRuntime ""
#else
# define wxRuntime "s"
#endif

#if _MSC_VER >= 1600 // VC 10.0 (aka 2010)
# define wxCompiler "vc100"
#elif _MSC_VER >= 1400 // VC 8.0 (aka 2005)
# define wxCompiler "vc80"
#else
# error "Unsupported compiler"
#endif

// Tell the linker which wx libraries to include
#pragma comment(lib, "wxbase" wxVersion wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxbase" wxVersion wxCompiler wxUnicode wxRuntime "_net")
#pragma comment(lib, "wxbase" wxVersion wxCompiler wxUnicode wxRuntime "_odbc")
#pragma comment(lib, "wxbase" wxVersion wxCompiler wxUnicode wxRuntime "_xml")
#pragma comment(lib, "wxexpat" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxjpeg" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxpng" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxregex" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxtiff" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxzlib" "_" wxCompiler wxUnicode wxRuntime)
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_adv")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_aui")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_core")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_dbgrid")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_gl")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_html")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_media")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_qa")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_richtext")
#pragma comment(lib, "wxmsw" wxVersion wxCompiler wxUnicode wxRuntime "_xrc")

// wxCore requires the following system libraries
#pragma comment(lib, "rpcrt4")
#pragma comment(lib, "comctl32")

#endif // WXXLIBS_DEFINED
