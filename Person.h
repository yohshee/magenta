// Person.h: interface for the CPerson class.
// This implements an abstraction for an online chatter.
// Note: this is stripped down from the original version since there's...
// no need for the other fields anymore, really.
// And yes, I do know I'm breaking traditional class customs here...
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSON_H__04AE2AA5_E3FE_4A02_BA27_3734E35CAD17__INCLUDED_)
#define AFX_PERSON_H__04AE2AA5_E3FE_4A02_BA27_3734E35CAD17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include "Magenta.h"
#include "Constants.h"

class CPerson : public CObject  
{
public:
	CPerson();
	virtual ~CPerson();

	CString szName;
	CString szIP;
	BOOL bConnected;
	NameType ntType;
};

#endif // !defined(AFX_PERSON_H__04AE2AA5_E3FE_4A02_BA27_3734E35CAD17__INCLUDED_)
