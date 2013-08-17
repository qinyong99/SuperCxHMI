// ActionParser.cpp: implementation of the CActionParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActionParser::CActionParser(LPCTSTR lpszScript)
{
	m_strScript = lpszScript;
}

CActionParser::~CActionParser()
{

}

// 返回指定位置所在行
void CActionParser::GetPosLine(int nPos, CString& strLine)
{
	LPCTSTR pBuf = m_strScript.GetBuffer(m_strScript.GetLength());

	// 到行头
	LPCTSTR p1 = pBuf + nPos;
	while (--p1 >= pBuf)
	{
		if (*p1 == '\n')
		{
			p1++;
			break;
		}
	}  

	// 到行尾
	LPCTSTR p2 = pBuf + nPos;
	while (*++p2 != '\0')
	{
		if (*p2 == '\n') 
		{
			p2--;
			break;
		}
	} 

	strLine = CString(p1, p2 - p1 + 1);
}

static TCHAR szEventExpert[] = _T("动作专家");

// 分析是否由专家产生
BOOL CActionParser::IsExpertGenerate(CString& strAction)
{
	int nFind = m_strScript.Find(szEventExpert);
	if (nFind != -1)
	{
		LPCTSTR pch = m_strScript;
		pch += nFind;
		pch += lstrlen(szEventExpert);
		if (*pch++ == _T('('))
		{
			LPCTSTR p1 = pch;																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																									
											
			if ((pch = _tcschr(pch, ')')) != NULL)
			{
				strAction = CString(p1, pch - p1);
				strAction.TrimLeft();
				strAction.TrimRight();																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																							
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

// 是否实现函数p
BOOL CActionParser::HasFunction(LPCTSTR lpszFunc, CStringArray& arrParams)
{
	int nRet = FALSE;

	// 忽略大小写
	CString strScipt = m_strScript;
	strScipt.MakeUpper();
	CString strFind = lpszFunc;
	strFind.MakeUpper();

	int nFind = strScipt.Find(strFind);
	if (nFind != -1)
	{
		CString strLine;
		GetPosLine(nFind, strLine);
		strLine.TrimLeft();
		// 第一个字符串必须未函数名称
		if (strFind.CompareNoCase(strLine.Left(strFind.GetLength())) != 0)
			return FALSE;

		LPCTSTR pch = strLine;
		pch += lstrlen(lpszFunc);

		// 过滤掉空格
		while (*pch == _T(' ') || *pch == _T('\t'))
			pch++;
		
		if (*pch++ == _T('('))
		{
			LPCTSTR pPreSeperator = pch;
			BOOL bString = FALSE;
			int nBraceCount = 1;
			while (*pch != _T('\0'))
			{
				if (*pch == _T('"'))
				{
					bString = !bString;
				}
				else if (!bString)
				{
					if (*pch == _T('('))
					{
						nBraceCount++;
					}
					else if (*pch == _T(')'))
					{
						nBraceCount--;
						if (nBraceCount < 1)
						{
							CString strWord(pPreSeperator, pch - pPreSeperator);
							strWord.TrimLeft();
							strWord.TrimRight();
							arrParams.Add(strWord);
							if (!strWord.IsEmpty())
							{
								pch++;
								// 过滤掉空格
								while (*pch == _T(' ') || *pch == _T('\t'))
									pch++;
								// 允许注释
								if (*pch == _T(' ') || *pch == _T('\'') || *pch == _T('\r') || *pch == _T('\t') || *pch == _T('\0'))
									nRet = TRUE;
							}
							break;
						}
					}
					else if (nBraceCount == 1)
					{
						if (*pch == _T(','))
						{
							CString strWord(pPreSeperator, pch - pPreSeperator);
							strWord.TrimLeft();
							strWord.TrimRight();
							arrParams.Add(strWord);
							if (strWord.IsEmpty())
								break;
							pPreSeperator = pch + 1;
						}
					}
				}

				pch++;
			}
		}
	}

	if (!nRet)
		arrParams.RemoveAll();

	return nRet;
}

// 是否含有赋值语句
BOOL CActionParser::HasAssignment(LPCTSTR lpszLeft, CString& strRight)
{
	int nRet = FALSE;

	// 忽略大小写
	CString strScipt = m_strScript;
	strScipt.MakeUpper();
	CString strFind = lpszLeft;
	strFind.MakeUpper();

	int nFind = strScipt.Find(strFind);
	if (nFind != -1)
	{
		CString strLine;
		GetPosLine(nFind, strLine);
		strLine.TrimLeft();
		// 第一个字符串必须为赋值语句的左边
		if (strFind.CompareNoCase(strLine.Left(strFind.GetLength())) != 0)
			return FALSE;
		
		LPCTSTR pch = strLine;
		pch += lstrlen(lpszLeft);

		// 过滤掉空格
		while (*pch == _T(' ') || *pch == _T('\t'))
			pch++;

		// 找到'='符号
		if (*pch++ == _T('='))
		{
			// 过滤掉空格
			while (*pch == _T(' ') || *pch == _T('\t'))
				pch++;
			
			LPCTSTR pPreSeperator = pch;
			BOOL bString = FALSE;
			while (1)
			{
				if (*pch == _T('"'))
				{
					bString = !bString;
				}
				else if (!bString)
				{
					if (*pch == _T(' ') || *pch == _T('\'') || *pch == _T('\r') || *pch == _T('\t') || *pch == _T('\0'))
					{
						CString strWord(pPreSeperator, pch - pPreSeperator);
						strWord.TrimLeft();
						strWord.TrimRight();
						if (!strWord.IsEmpty())
						{
							strRight = strWord;
							nRet = TRUE;
						}
						break;
					}
				}
				pch++;
			}
		}
	}
	
	return nRet;
}