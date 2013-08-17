// DocManager2.h: interface for the CDocManager2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCMANAGER2_H__E041CE90_26E5_4DEF_B6FB_F405521A2BBF__INCLUDED_)
#define AFX_DOCMANAGER2_H__E041CE90_26E5_4DEF_B6FB_F405521A2BBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDocManager2 : public CDocManager
{
public:
	// helper for standard commdlg dialogs
	virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
			DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
};

#endif // !defined(AFX_DOCMANAGER2_H__E041CE90_26E5_4DEF_B6FB_F405521A2BBF__INCLUDED_)
