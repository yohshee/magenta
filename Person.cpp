// Person.cpp: implementation of the CPerson class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Magenta.h"
#include "Person.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Set a few defaults.
CPerson::CPerson()
{
	szName = _T("");
	szIP = _T("");
	bConnected = FALSE;
	ntType = ntRegular;
}

CPerson::~CPerson()
{

}
