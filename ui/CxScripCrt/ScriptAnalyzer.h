// ScriptAnalyzer.h: interface for the CScriptAnalyzer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCRIPTANALYZER_H__
#define _SCRIPTANALYZER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CScriptAnalyzer  
{
public:
	CScriptAnalyzer(LPCTSTR lpszText = NULL);
	~CScriptAnalyzer();

	enum
	{
		SBT_Sub = 0, 
		SBT_Function, 
		SBT_Class, 
	};

	class CScriptBlock
	{
	public:
		CScriptBlock()
		{
			Init();
		}

		void Init()
		{
			m_nType = -1;

			m_nCommentBegin = -1;
			m_nBegin = -1;
			m_nBodyBegin = -1;
			m_nBodyEnd = -1;
			m_nEnd = -1;
		}

		CScriptBlock(const CScriptBlock& sb)
		{
			InternalCopy(sb);
		}

		CScriptBlock& operator=(const CScriptBlock& sb)
		{
			InternalCopy(sb);
			return *this;
		}

		void InternalCopy(const CScriptBlock& sb)
		{
			m_nType = sb.m_nType;
			m_strName = sb.m_strName;

			m_nCommentBegin = sb.m_nCommentBegin;
			m_nBegin = sb.m_nBegin;
			m_nBodyBegin = sb.m_nBodyBegin;
			m_nBodyEnd = sb.m_nBodyEnd;
			m_nEnd = sb.m_nEnd;
		}
			
		int m_nType; 
		CString m_strName;

		int m_nCommentBegin;
		int m_nBegin;
		int m_nBodyBegin;
		int m_nBodyEnd;
		int m_nEnd;
	};

	void AnalyzeText(LPCTSTR lpszText);

	BOOL FindBlockBegin(LPCTSTR p, int nType, CString& strBlockName, int& nLineStart, int& nLineEnd);
	BOOL FindBlockEnd(LPCTSTR p, int nType, int& nLineStart, int& nLineEnd);

	void Analyze();

	CString GetContext(int nPos);

	int GetFirstBlockBegin();
	BOOL GetGeneralBlock(CStringArray& arrBlocks);
	int GetGeneralBlockBodyBegin(LPCTSTR lpszName);

	BOOL GetObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent, 
		int& nSubBegin, int& nSubBodyBegin, int& nSubBodyEnd, int& nSubEnd);
	BOOL GetObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent, CString& strSubBody);
	BOOL GetObjectEventHandler(LPCTSTR lpszObject, CStringArray& arrEventSubs);
	
	BOOL HasObjectEventHandler(LPCTSTR lpszObject, LPCTSTR lpszEvent = NULL);

	virtual BOOL IsEventHandler(LPCTSTR lpszSubName);

	void GetInsertPosition(LPCTSTR lpszName, int& nStart, int& nEnd);

protected:
	LPCTSTR m_pText;
	LPCTSTR m_pTextEnd;

	CSimpleArray<CScriptBlock> m_arrBlocks;
};

#endif // _SCRIPTANALYZER_H__
