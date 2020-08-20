// Tokenizer.cpp: implementation of the CTokenizer class.
//
// (Yes, I did write this class myself -RC)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Magenta.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTokenizer::CTokenizer()
{
	// Default constructor does...um..nothing.
	m_szString = _T("");
	m_szDelim = _T(" "); // Default delimiter is a space.
	m_ulPosition = 0;
}

CTokenizer::CTokenizer(CString szString, CString szDelim)
{
	// However, this one initializes the tokenized
	// string and breaks it apart in one fell swoop.
	m_szString = szString;
	m_szDelim = szDelim;
	m_ulPosition = 0;
	SplitString();
}

CTokenizer::~CTokenizer()
{
	// Just invoke Clear() and be done with it.
	this->Clear();
}

// Public methods

CString CTokenizer::GetString()
{
	return m_szString;
}

CString CTokenizer::GetToken()
{
	CString szRet = _T("");

	// Er..simple enough. Return whatever the other method returns and increment.
	szRet = GetTokenNoAdvance();
	Advance();

	return szRet;
}

CString CTokenizer::GetTokenNoAdvance()
{
	ULONG uUpper = (ULONG)m_szaSplit.GetUpperBound();  // Quit complaining, compiler!
	CString szRet = _T("");

	if((m_ulPosition < uUpper+1) && (m_szaSplit.GetSize() > 0))
		szRet = *(CString*)m_szaSplit.GetAt(m_ulPosition);

	return szRet;
}

void CTokenizer::Rewind()
{
	// Just reset the position.
	m_ulPosition = 0;
}

void CTokenizer::BackUp()
{
	if(m_ulPosition != 0)
		m_ulPosition--;
}

void CTokenizer::Advance()
{
	m_ulPosition++;
}

CString CTokenizer::GetDelimiter() 
{
	return m_szDelim;
}

void CTokenizer::SetDelimiter(CString szDelim)
{
	ULONG uPosition;

	uPosition = m_ulPosition;
	m_szDelim = szDelim;

	// Split the string again, then reset the position to where it should be.
	if(m_szString.GetLength() > 0) {
		SplitString();

		if(m_ulPosition > (ULONG)m_szaSplit.GetUpperBound())
			m_ulPosition = (ULONG)m_szaSplit.GetUpperBound();
	}
}

BOOL CTokenizer::HasMoreTokens()
{
	ULONG uUpper = (ULONG)m_szaSplit.GetUpperBound();
	BOOL bRet = FALSE;

	if(m_ulPosition <= uUpper)
		bRet = TRUE;

	return bRet;
}

void CTokenizer::Clear()
{
	INT_PTR i;
	INT_PTR iUpper = m_szaSplit.GetUpperBound();

	// All too easy...save the delimiter, but
	// wipe out the string, the position, and the array.
	m_szString = _T("");
	m_ulPosition = 0;

	// Destroy all of the strings in the array, one by one.
	for(i = 0; i <= iUpper; i++)
		delete (CString*)(m_szaSplit.GetAt(i));

	// Then remove those dead pointers.
	m_szaSplit.RemoveAll();
}

void CTokenizer::SetString(CString szString, int iLimit)
{
	m_szString = szString;
	// Re-split the string, and reset the position.
	m_ulPosition = 0;
	SplitString(iLimit);
}

// Protected methods

// This is the main magic-maker...
void CTokenizer::SplitString(int iLimit)
{
	CString* pszTemp = NULL;	// Temp pointer for the allocated string.
	CString szWalk = _T("");	// The built up string as we walk through the string.
	TCHAR chCurrent;			// The current character
	int i, j;

	// Clean out the array, first.
	for(i = 0; i <= m_szaSplit.GetUpperBound(); i++)
		delete (CString*)(m_szaSplit.GetAt(i));

	// Then remove those dead pointers.
	m_szaSplit.RemoveAll();

	if(m_szString.FindOneOf(m_szDelim) == 0) {
		// Just stuff a copy of the string into the array, and return.
		pszTemp = new CString(m_szString);
		m_szaSplit.Add(pszTemp);
	}
	else {
		// Go through the string character by character, and pull out
		// full strings based on delimiters, etc.
		j = 1;
		for(i = 0; i < m_szString.GetLength(); i++) {
			chCurrent = m_szString.GetAt(i);
			if((chCurrent == m_szDelim) && (j != iLimit)) {
				// Create a new string and add it to the array.
				pszTemp = new CString(szWalk);
				m_szaSplit.Add(pszTemp);
				// Clear the old string
				szWalk.Empty();
				j++;	// Count a string. o/~
			}
			else {
				// Just attach the character to szWalk.
				szWalk += chCurrent;
			}
		}

		// Of course, we also need to add in whatever was left over..
		if(szWalk.GetLength() > 0) {
			pszTemp = new CString(szWalk);
			m_szaSplit.Add(pszTemp);
		}
	}
}