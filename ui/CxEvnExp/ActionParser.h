// ActionParser.h: interface for the CActionParser class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ACTIONPARSER_H_
#define __ACTIONPARSER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CActionParser  
{
public:
	CActionParser(LPCTSTR lpszScript);
	virtual ~CActionParser();

	BOOL IsExpertGenerate(CString& strAction);
	BOOL HasFunction(LPCTSTR lpszFunc, CStringArray& arrParams);
	BOOL HasAssignment(LPCTSTR lpszLeft, CString& strRight);

	

private:
	void GetPosLine(int nPos, CString& strLine);

	CString m_strScript;
};

#endif // __ACTIONPARSER_H_
