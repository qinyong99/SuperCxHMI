// ScriptAnalyzer.cpp: implementation of the CScriptAnalyzer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScriptAnalyzer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptAnalyzer::CScriptAnalyzer(LPCTSTR lpszText)
{
	m_pText = NULL;
	m_pTextEnd = NULL;

	AnalyzeText(lpszText);
}

CScriptAnalyzer::~CScriptAnalyzer()
{
	AnalyzeText(NULL);
}

void CScriptAnalyzer::AnalyzeText(LPCTSTR lpszText)
{
	m_arrBlocks.RemoveAll();

	if (m_pText != NULL)
	{
		free((void *)m_pText);
		m_pText = NULL;
	}
	m_pTextEnd = NULL;

	if (lpszText != NULL)
	{
		m_pText = _tcsdup(lpszText);
		m_pTextEnd = m_pText + strlen(m_pText);

		Analyze();
	}
}

BOOL CScriptAnalyzer::FindBlockBegin(LPCTSTR p, int nType, CString& strBlockName, int& nLineStart, int& nLineEnd)
{
	USES_CHINESEVAR_CONVERSION;

	LPCTSTR pStart;
	LPCTSTR pEnd;

	// 关键词前面必须为空
	if (p > m_pText)
	{
		TCHAR ch = *(p - 1);
		if (ch != '\n' && !isspace(ch))
			return FALSE;
	}

	// 到行头
	pStart = p;
	while (pStart > m_pText)
	{
		if (*(pStart - 1) == '\n')
			break;
		pStart--;
	}

	CString strPreWord(pStart, p - pStart);
	strPreWord.TrimLeft();
	strPreWord.TrimRight();

	// 如果前面有词，则必须为 public 或 private
	if (!strPreWord.IsEmpty() && 
		strPreWord.CompareNoCase(_T("PUBLIC")) != 0 && 
		strPreWord.CompareNoCase(_T("PRIVATE")) != 0)
	{
		return FALSE;
	}

	switch (nType)
	{
	case SBT_Sub:
		p += 3;
		break;
	case SBT_Function:
		p += 8;
		break;
	case SBT_Class:
		p += 5;
		break;
	default:
		ATLASSERT(false);
		return FALSE;
	}

	// 必须有空格分隔
	if (!isspace(*p))
		return FALSE;
	p++;

	// 到行尾
	pEnd = p;
	while (pEnd < m_pTextEnd)
	{
		if (*pEnd++ == '\n') 
			break;
	} 
	
	CString strPostWord(p, pEnd - p);
	strPostWord.TrimLeft();
	strPostWord.TrimRight();

	if (strPostWord.IsEmpty())
		return FALSE;

	strPostWord = VARC2A(strPostWord);
	// 第一个字符必须为字母
	if (!isalpha(strPostWord[0]))
		return FALSE;

	int n = 1;
	int nL = strPostWord.GetLength();
	while (n < nL && __iscsym(strPostWord[n]))
		n++;

	strBlockName = VARA2C(strPostWord.Left(n));
	nLineStart = pStart - m_pText;
	nLineEnd = pEnd - m_pText;

	return TRUE;
}

BOOL CScriptAnalyzer::FindBlockEnd(LPCTSTR p, int nType, int& nLineStart, int& nLineEnd)
{
	LPCTSTR pStart;
	LPCTSTR pEnd;

	// 关键词前面必须为空
	ATLASSERT(p > m_pText);
	if (p <= m_pText)
		return FALSE;

	// 关键词前面必须为空
	if (!isspace(*(p - 1)))
		return FALSE;

	// 到行头
	pStart = p - 1;
	while (pStart > m_pText)
	{
		if (*(pStart - 1) == '\n')
			break;
		pStart--;
	}

	CString strPreWord(pStart, p - pStart);
	strPreWord.TrimLeft();
	strPreWord.TrimRight();

	// 前面必须为 END
	if (strPreWord.CompareNoCase(_T("END")) != 0)
	{
		return FALSE;
	}

	switch (nType)
	{
	case SBT_Sub:
		p += 3;
		break;
	case SBT_Function:
		p += 8;
		break;
	case SBT_Class:
		p += 5;
		break;
	default:
		ATLASSERT(false);
		return FALSE;
	}

	// 到行尾
	pEnd = p;
	while (pEnd < m_pTextEnd)
	{
		if (*pEnd++ == '\n') 
			break;
	}
	
	CString strPostWord(p, pEnd - p);
	strPostWord.TrimLeft();
	strPostWord.TrimRight();

	if (!strPostWord.IsEmpty())
		return FALSE;

	nLineStart = pStart - m_pText;
	nLineEnd = pEnd - m_pText;

	return TRUE;
}

void CScriptAnalyzer::Analyze()
{
	LPCTSTR p = m_pText;

	CScriptBlock sb;
	CScriptBlock* psb = NULL;
	int nType = -1;

	LPCTSTR pLineStart = p;
	LPCTSTR pLastCodeEnd = p;
	LPCTSTR pEmptyLineStart = NULL;
	LPCTSTR pCommentLineStart = NULL;
	
	while (p < m_pTextEnd)
	{
		if ((p[0] == _T('S') || p[0] == _T('s')) &&
			(p[1] == _T('U') || p[1] == _T('u')) &&
			(p[2] == _T('B') || p[2] == _T('b')))
		{
			nType = SBT_Sub;		
		}
		else 
		if ((p[0] == _T('F') || p[0] == _T('f')) &&
			(p[1] == _T('U') || p[1] == _T('u')) &&
			(p[2] == _T('N') || p[2] == _T('n')) &&
			(p[3] == _T('C') || p[3] == _T('c')) &&
			(p[4] == _T('T') || p[4] == _T('t')) &&
			(p[5] == _T('I') || p[5] == _T('i')) &&
			(p[6] == _T('O') || p[6] == _T('o')) &&
			(p[7] == _T('N') || p[7] == _T('n')))
		{
			nType = SBT_Function;
		}
		else 
		if ((p[0] == _T('C') || p[0] == _T('c')) &&
			(p[1] == _T('L') || p[1] == _T('l')) &&
			(p[2] == _T('A') || p[2] == _T('a')) &&
			(p[3] == _T('S') || p[3] == _T('s')) &&
			(p[4] == _T('S') || p[4] == _T('s')))
		{
			nType = SBT_Class;
		}
		else
		{
			if (psb == NULL && *p == '\n')
			{
				p++;

				CString strLine(pLineStart, p - pLineStart);
				strLine.TrimLeft();
				strLine.TrimRight();

				if (strLine.IsEmpty())
				{
					if (pEmptyLineStart == NULL)
						pEmptyLineStart = pLineStart;
					pCommentLineStart = NULL;
				}
				else if (strLine[0] == '\'')
				{
					if (pCommentLineStart == NULL)
						pCommentLineStart = pLineStart;
				}
				else
				{
					pLastCodeEnd = p;
					pEmptyLineStart = NULL;
					pCommentLineStart = NULL;
				}

				pLineStart = p;
			}
			else
			{
				p++;
			}
			
			continue;
		}

		if (psb == NULL)
		{
			sb.Init();

			if (FindBlockBegin(p, nType, sb.m_strName, sb.m_nBegin, sb.m_nBodyBegin))
			{
				p = m_pText + sb.m_nBodyBegin;

				sb.m_nType = nType;
				sb.m_nCommentBegin = sb.m_nBegin;

				// 前面有代码和前面没有代码判断方式有差别
				if (pLastCodeEnd > m_pText)
				{
					if (pEmptyLineStart >= pLastCodeEnd)
						sb.m_nCommentBegin = pEmptyLineStart - m_pText;
					else
						sb.m_nCommentBegin = pLastCodeEnd - m_pText;
				}
				else
				{
					if (pCommentLineStart != NULL)
						sb.m_nCommentBegin = pCommentLineStart - m_pText;
				}
								
				m_arrBlocks.Add(sb);
				psb = &m_arrBlocks[m_arrBlocks.GetSize() - 1];

				continue;
			}
		}
		else if (psb->m_nType == nType)
		{
			if (FindBlockEnd(p, nType, psb->m_nBodyEnd, psb->m_nEnd))
			{
				p = m_pText + psb->m_nEnd;
				psb = NULL;

				pLastCodeEnd = p;
				pEmptyLineStart = NULL;
				pCommentLineStart = NULL;

				pLineStart = p;

				continue;
			}
		}

		p++;
	}

	if (psb != NULL)
	{
		psb->m_nBodyEnd = p - m_pText;
		psb->m_nEnd = p - m_pText;
	}
}

CString CScriptAnalyzer::GetContext(int nPos)
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];

		if (nPos < sb.m_nCommentBegin)
			return _T("");

		if (nPos < sb.m_nEnd)
			return sb.m_strName;
	}

	if (m_arrBlocks.GetSize() > 0)
		return m_arrBlocks[m_arrBlocks.GetSize() - 1].m_strName;

	return _T("");
}

BOOL CScriptAnalyzer::HasObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent)
{
	CString strEventSubName = lpszObject;
	strEventSubName += _T('_');
	strEventSubName += lpszEvent;

	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_nType == SBT_Sub && sb.m_strName.CompareNoCase(strEventSubName) == 0)
			return TRUE;
	}
	
	return FALSE;
}

BOOL CScriptAnalyzer::GetGeneralBlock(CStringArray& arrBlocks)
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];

		if (!IsEventHandler(sb.m_strName))
			arrBlocks.Add(sb.m_strName);
	}
	
	return (arrBlocks.GetSize() > 0);
}

BOOL CScriptAnalyzer::GetObjectEventHandler(LPCTSTR lpszObject, CStringArray& arrEventSubs)
{
	CString strEventSubName = lpszObject;
	strEventSubName += _T('_');
	strEventSubName.MakeUpper();

	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_nType != SBT_Sub)
			continue;

		CString strSubName = sb.m_strName;
		strSubName.MakeUpper();

		// 包含即可以
		if (strSubName.Find(strEventSubName, 0) == 0)
			arrEventSubs.Add(strSubName);
	}
	
	return (arrEventSubs.GetSize() > 0);
}

BOOL CScriptAnalyzer::GetObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent, 
		int& nSubBegin, int& nSubBodyBegin, int& nSubBodyEnd, int& nSubEnd)
{
	CString strEventSubName = lpszObject;
	strEventSubName += _T('_');
	strEventSubName += lpszEvent;

	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_nType == SBT_Sub && sb.m_strName.CompareNoCase(strEventSubName) == 0)
		{
			nSubBegin = sb.m_nBegin;
			nSubBodyBegin = sb.m_nBodyBegin;
			nSubBodyEnd = sb.m_nBodyEnd;
			nSubEnd = sb.m_nEnd;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CScriptAnalyzer::GetObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent, CString& strSubBody)
{
	int nSubBegin;
	int nSubBodyBegin;
	int nSubBodyEnd;
	int nSubEnd;
	if (GetObjectEventHandler(lpszObject, lpszEvent, 
		nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd))
	{
		strSubBody = CString(m_pText + nSubBodyBegin, nSubBodyEnd - nSubBodyBegin);
		return TRUE;
	}

	return FALSE;	
}

int CScriptAnalyzer::GetFirstBlockBegin()
{
	if (m_arrBlocks.GetSize() > 0)
	{
		CScriptBlock& sb = m_arrBlocks[0];
		return sb.m_nCommentBegin;
	}

	return -1;
}

int CScriptAnalyzer::GetGeneralBlockBodyBegin(LPCTSTR lpszName)
{
	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_strName.CompareNoCase(lpszName) == 0)
			return sb.m_nBodyBegin;
	}

	return -1;
}

BOOL CScriptAnalyzer::IsEventHandler(LPCTSTR lpszSubName)
{
	CString strSubName = lpszSubName;
	int nIndex = strSubName.Find(_T("_"));
	if (nIndex > 0 && nIndex < strSubName.GetLength() - 1)
		return TRUE;

	return FALSE;
}

// 0xd, 0xa 回车换行
void CScriptAnalyzer::GetInsertPosition(LPCTSTR lpszName, int& nStart, int& nEnd)
{
	// 初始化为末尾
	nStart = m_pTextEnd - m_pText;
	nEnd = nStart;

	for (int i = 0; i < m_arrBlocks.GetSize(); i++)
	{
		CScriptBlock& sb = m_arrBlocks[i];
		if (sb.m_strName.CompareNoCase(lpszName) > 0)
		{
			nStart = sb.m_nCommentBegin;
			nEnd = nStart;

			break;
		}
	}

	// 向前到没有空白行为止
	LPCTSTR p = m_pText + nStart;
	LPCTSTR pLastLine = p;
	while (p > m_pText)
	{
		p--;

		if (*p == '\n')
			pLastLine = p + 1;
		
		if (!isspace(*p))
			break;
	}
	if (p == m_pText)
		pLastLine = p;
	nStart = pLastLine - m_pText;

	// 向后到没有空白行为止
	p = m_pText + nEnd;
	pLastLine = p;
	while (p < m_pTextEnd)
	{
		if (*p == '\n')
			pLastLine = p + 1;
		
		if (!isspace(*p))
			break;

		p++;
	}
	nEnd = pLastLine - m_pText;
}
