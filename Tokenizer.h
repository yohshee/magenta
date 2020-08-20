// Tokenizer.h: interface for the CTokenizer class.
//
// Assumption: That the user of this class actually knows what they're
// doing and they're NOT doing things out of order.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOKENIZER_H__31EA6460_8BDC_45B3_BC28_E4457FD03200__INCLUDED_)
#define AFX_TOKENIZER_H__31EA6460_8BDC_45B3_BC28_E4457FD03200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTokenizer : public CObject
{
public:

	// Constructors..we have two of 'em.
	CTokenizer();
	CTokenizer(CString szString, CString szDelim = _T(" "));

	virtual ~CTokenizer();

	// Other methods we'll need.
	CString GetString();								// Returns the string contained in the tokenizer.
	CString GetToken();									// Pulls a token out and advances to the next.
	CString GetTokenNoAdvance();						// Pulls a token out and doesn't advance.
	void SetString(CString szString, int iLimit = 0);	// Sets a new string into the tokenizer
	void Rewind();										// Resets the tokenizer.
	void BackUp();										// Backs up one token.
	void Advance();										// Advances forward one token.
	void SetDelimiter(CString szDelim);					// Sets a new delimiter.
	void Clear();										// Clears out the tokenizer.
	CString GetDelimiter();								// Returns the current delimiter.
	BOOL HasMoreTokens();								// Returns TRUE if there are more tokens left.

protected:
	// Protected members...
	CString			m_szString;			// The string to tokenize
	CString			m_szDelim;			// The delimiter to tokenize by.
	ULONG			m_ulPosition;		// Current position in the tokenized string
	CPtrArray		m_szaSplit;			// The split up string

	void SplitString(int iLimit = 0);   // Does the actual splitting.
};

#endif // !defined(AFX_TOKENIZER_H__31EA6460_8BDC_45B3_BC28_E4457FD03200__INCLUDED_)
