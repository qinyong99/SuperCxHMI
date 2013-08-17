// DownloadRecipeDlg.cpp: implementation of the CDownloadRecipeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DownloadRecipeDlg.h"
#include "EventExpert.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownloadRecipeDlg::CDownloadRecipeDlg()
{
	m_pExpert = NULL;
}

CDownloadRecipeDlg::~CDownloadRecipeDlg()
{
}

LRESULT CDownloadRecipeDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_edit.Attach(GetDlgItem(IDC_RECIPE));
	
	return S_OK;
}

LRESULT CDownloadRecipeDlg::OnClickedSelect(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog dlg(TRUE, _T("rcp"), m_strFile,
		OFN_FILEMUSTEXIST,
		_T("配方文件 (*.rcp)\0*.rcp\0所有文件 (*.*)\0*.*\0\0"));

	TCHAR szDir[MAX_PATH];
	GetRecipeFolder(szDir);
	dlg.m_ofn.lpstrInitialDir = szDir;
	
	if (dlg.DoModal() == IDOK)
	{
		m_strFile = dlg.m_szFileName;
		m_edit.SetWindowText(m_strFile);
	}
	
	return 0;
}

LRESULT CDownloadRecipeDlg::OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();
	
	return 0;
}

void CDownloadRecipeDlg::OnShow()
{
	CActionParser parser(m_pExpert->m_strActionScript);
	CStringArray arrParams;
	if (parser.HasFunction("DownloadRecipe", arrParams))
	{
		if (arrParams.GetSize() == 1)
		{
			CString strWord = arrParams[0];
			if (strWord[0] == _T('"') && strWord[strWord.GetLength() - 1] == _T('"'))
				m_strFile = strWord.Mid(1, strWord.GetLength() - 2);
		}
	}
	
	m_edit.SetWindowText(m_strFile);
}

void CDownloadRecipeDlg::OnOK()
{
	int nLen = m_edit.GetWindowTextLength();
	if (nLen != -1)
	{
		m_edit.GetWindowText(m_strFile.GetBuffer(nLen + 1), nLen + 1);
		m_strFile.ReleaseBuffer();
	}
	
	CString strActionScript = _T("	DownloadRecipe(\"");
	strActionScript += m_strFile;
	strActionScript += _T("\")");
	
	m_pExpert->UpdateActionScript(_T("下载配方"), strActionScript);		
}