// ShowPictureDlg.cpp: implementation of the CShowPictureDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShowPictureDlg.h"
#include "EventExpert.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShowPictureDlg::CShowPictureDlg()
{
	m_pExpert = NULL;
}

CShowPictureDlg::~CShowPictureDlg()
{
}

LRESULT CShowPictureDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_editPicture.Attach(GetDlgItem(IDC_PICTURE));
	
	return S_OK;
}

LRESULT CShowPictureDlg::OnClickedSelectPicture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CFileDialog dlg(TRUE, _T("grf"), m_strPictureFile,
		OFN_FILEMUSTEXIST,
		_T("画面文件 (*.grf)\0*.grf\0所有文件 (*.*)\0*.*\0\0"));

	CString strFolder;
	GetPictureFolder(strFolder.GetBuffer(MAX_PATH));
	strFolder.ReleaseBuffer();

	TCHAR ch = strFolder[strFolder.GetLength() - 1];
	if (ch != '\\' && ch != '/')
		strFolder += _T('\\');
	
	dlg.m_ofn.lpstrInitialDir = strFolder;
	if (dlg.DoModal() == IDOK)
	{
		m_strPictureFile = dlg.m_szFileName;
		
		CString strTemp = m_strPictureFile.Right(4);
		if (strTemp.CollateNoCase(_T(".grf")) == 0)
			m_strPictureFile = m_strPictureFile.Left(m_strPictureFile.GetLength() - 4);

		if (m_strPictureFile.GetLength() > strFolder.GetLength())
		{
			strTemp = m_strPictureFile.Left(strFolder.GetLength());
			if (strTemp.CollateNoCase(strFolder) == 0)
				m_strPictureFile.Delete(0, strFolder.GetLength());
		}

		m_editPicture.SetWindowText(m_strPictureFile);
	}
	
	return 0;
}

LRESULT CShowPictureDlg::OnChangePicture(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();
	
	return 0;
}

void CShowPictureDlg::OnShow()
{
	CActionParser parser(m_pExpert->m_strActionScript);
	CStringArray arrParams;
	if (parser.HasFunction("ShowPicture", arrParams))
	{
		if (arrParams.GetSize() == 1)
		{
			CString strWord = arrParams[0];
			if (strWord[0] == _T('"') && strWord[strWord.GetLength() - 1] == _T('"'))
				m_strPictureFile = strWord.Mid(1, strWord.GetLength() - 2);
		}
	}
	
	m_editPicture.SetWindowText(m_strPictureFile);
}

void CShowPictureDlg::OnOK()
{
	int nLen = m_editPicture.GetWindowTextLength();
	if (nLen != -1)
	{
		m_editPicture.GetWindowText(m_strPictureFile.GetBuffer(nLen + 1), nLen + 1);
		m_strPictureFile.ReleaseBuffer();
	}
	
	CString strActionScript = _T("\tShowPicture(\"");
	strActionScript += m_strPictureFile;
	strActionScript += _T("\")");
	
	m_pExpert->UpdateActionScript(_T("显示画面"), strActionScript);		
}