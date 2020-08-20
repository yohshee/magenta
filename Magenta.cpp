// Magenta.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Person.h"
#include "Magenta.h"
#include "mdlsmain.h"
#include "ChatWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMagenta

BEGIN_MESSAGE_MAP(CMagenta, CWinApp)
	//{{AFX_MSG_MAP(CMagenta)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	//ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMagenta construction

CMagenta::CMagenta()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMagenta object

CMagenta theApp;

/////////////////////////////////////////////////////////////////////////////
// CMagenta initialization

BOOL CMagenta::InitInstance()
{
	// Initialize sockets
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	if(!AfxInitRichEdit()) {
		AfxMessageBox(_T("Failed to initialize RichEdit library. Can't start without it!"));
		return FALSE;
	}

	// Set up the registry key we're using here, which is our company name.
	// We get the application name for free.
	SetRegistryKey(_T("Faded Twilight"));

	// Standard initialization stuff.
	
#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Create our dialog window and show it.
	CChatWindow* pChatWindow = new CChatWindow;
	m_pMainWnd = pChatWindow;
	BOOL bFlag = pChatWindow->Create();
	
	return bFlag;
}

// Extracts version info from the executable itself.
BOOL CMagenta::GetMagentaVersion(int& iMajor, int& iMinor, int& iBuild)
{
	BOOL bSucc = FALSE;				// OK flag for this thing.
	DWORD dwLength;					// Length of the version info
	DWORD dwFoo;					// Random foo.
	PVOID lpvData;					// Buffer to hold version info block
	UINT uLen;						// Length of fixed version info.
	VS_FIXEDFILEINFO* pFFI;			// Fixed version information pointer.
	TCHAR szExe[256];				// Static buffer for the exe name.

	// Get the pathname of the current process
	if(::GetModuleFileName(NULL, szExe, 256) != 0) {
		// Everything hinges on that, by the way.
		if((dwLength = ::GetFileVersionInfoSize(szExe, &dwFoo)) != 0) {
			if((lpvData = malloc(dwLength)) != NULL) {
				::GetFileVersionInfo(szExe, 0, dwLength, lpvData);
				// Next, query the info.
				if(::VerQueryValue(lpvData, _T("\\"), (PVOID*)&pFFI, &uLen)) {
				// Got it, unpack it, destroy the data, and return true.
					iMajor = HIWORD(pFFI->dwFileVersionMS);
					iMinor = LOWORD(pFFI->dwFileVersionMS);
					iBuild = LOWORD(pFFI->dwFileVersionLS);
					free(lpvData);
					bSucc = TRUE;
				}
			}
		}
	}

	return bSucc;
}

int CMagenta::ExitInstance() 
{
	return CWinApp::ExitInstance();
}
