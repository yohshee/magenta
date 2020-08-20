// Magenta.h : main header file for the MAGENTA application
//

#if !defined(AFX_MAGENTA_H__F339B1C2_A0A1_4380_9693_12101DC736C0__INCLUDED_)
#define AFX_MAGENTA_H__F339B1C2_A0A1_4380_9693_12101DC736C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMagenta:
// See Magenta.cpp for the implementation of this class
//

class CMagenta : public CWinApp
{
public:
	CMagenta();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMagenta)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	BOOL GetMagentaVersion(int& iMajor, int& iMinor, int& iBuild);

// Implementation

	//{{AFX_MSG(CMagenta)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAGENTA_H__F339B1C2_A0A1_4380_9693_12101DC736C0__INCLUDED_)
