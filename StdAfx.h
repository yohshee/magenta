// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__45A730FF_78C1_45C0_B39B_504DFD35A057__INCLUDED_)
#define AFX_STDAFX_H__45A730FF_78C1_45C0_B39B_504DFD35A057__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// I am defining these here to avoid having to define _WIN32_WINNT >= 0x0500. Apparently,
// static MFC does not know how to handle that version correctly. I'd either have to rebuild
// static MFC or...something.
#define WS_EX_LAYERED           0x00080000
#define LWA_ALPHA               0x00000002

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <afxadv.h>			// MFC advanced stuff.
#include <afxmt.h>			// For the CCriticalSection class
#include <afxinet.h>		// Internet stuff.
#include <afxrich.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
#include <htmlhelp.h>		// HTML Help API

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__45A730FF_78C1_45C0_B39B_504DFD35A057__INCLUDED_)
