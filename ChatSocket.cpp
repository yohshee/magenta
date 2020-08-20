// ChatSocket.cpp : implementation file
//
// This is just a veeeery simple override of the original socket.
// All it does is delegate to the main window.
// Inspired by some sample code in the Windows 2000 Systems Programming Black Book.

#include "stdafx.h"
#include "Magenta.h"
#include "ChatSocket.h"
#include "ChatWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChatSocket

CChatSocket::CChatSocket()
{
}

CChatSocket::~CChatSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CChatSocket, CSocket)
	//{{AFX_MSG_MAP(CChatSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CChatSocket member functions

void CChatSocket::OnReceive(int nErrorCode) 
{
	CChatWindow* pWindow = (CChatWindow*)AfxGetMainWnd();
	
	// Not much to do; just notify the main window that we got one!
	if(pWindow != NULL)
		pWindow->ProcessPacket();

	CSocket::OnReceive(nErrorCode);
}

void CChatSocket::OnClose(int nErrorCode) 
{
	CChatWindow* pWindow = (CChatWindow*)AfxGetMainWnd();

	// Pass along error handling to the main window.
	if(pWindow != NULL)
		pWindow->ProcessClose(nErrorCode);

	CSocket::OnClose(nErrorCode);
}
