// DataEntryDlg.cpp: implementation of the CDataEntryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxHmiObj.h"
#include "CxHmiMisc.h"
#include "DataEntryDlg.h"
#include "EventExpert.h"
#include "ActionParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataEntryDlg::CDataEntryDlg()
{
	m_pExpert = NULL;
	m_nMode = 0;
}

CDataEntryDlg::~CDataEntryDlg()
{
	
}

LRESULT CDataEntryDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	for (int i = 0; i < 4; i++)
	{
		m_apPages[i] = NULL;
	}
	DoDataExchange();
	
	return S_OK;
}

LRESULT CDataEntryDlg::OnClickedEntryMode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (wNotifyCode == BN_CLICKED)
		ActiveEntryModePage(wID - IDC_RADIO1);

	m_pExpert->OnActionScriptChanged();
	
	return 0;
}


LRESULT CDataEntryDlg::OnChangeExpression(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	m_pExpert->OnActionScriptChanged();

	return 0;
}

void CDataEntryDlg::ActiveEntryModePage(int nMode)
{
	CTabPage* pPage = m_apPages[nMode];
	if (pPage == NULL)
	{
		switch (nMode)
		{
		case 0:
		case 2:
			pPage = new CNumericEntryPage;
			((CNumericEntryPage *)pPage)->m_pExpert = m_pExpert;
			break;
		case 1:
			break;
		case 3:
			pPage = new CPushbuttonEntryPage;
			((CPushbuttonEntryPage *)pPage)->m_pExpert = m_pExpert;
			break;
		default:
			break;
		}

		m_apPages[nMode] = pPage;

		if (pPage != NULL)
		{
			CWindow winRect;
			winRect.Attach(GetDlgItem(IDC_RECT));
			RECT rc;
			winRect.GetWindowRect(&rc);
			ScreenToClient(&rc);
			
			pPage->Create(m_hWnd);
			rc.bottom -= 4;
			pPage->MoveTo(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
		}
	}

	switch (nMode)
	{
	case 0:
	case 2:
		((CNumericEntryPage *)pPage)->m_strScript = m_pExpert->m_strActionScript;
		break;
	case 1:
		break;
	case 3:
		((CPushbuttonEntryPage *)pPage)->m_strScript = m_pExpert->m_strActionScript;
		break;
	default:
		break;
	}
	
	for (int i = 0; i < 4; i++)
	{
		pPage = m_apPages[i];
		if (pPage != NULL)
			pPage->Show(nMode == i);
	}
		
}

void CDataEntryDlg::OnFinalMessage(HWND hWnd)
{
	for (int i = 0; i < 4; i++)
	{
		if (m_apPages[i] != NULL)
		{
			delete m_apPages[i];
			m_apPages[i] = NULL;
		}
	}
}

static _TCHAR *_gszModeString[NUM_PAGES] =
{
	_T("DataEntryModeNumeric"), 
	_T("DataEntryModeText"), 
	_T("DataEntryModeSlider"), 
	_T("DataEntryModePushbutton")
};

void CDataEntryDlg::OnShow()
{
	CActionParser parser(m_pExpert->m_strActionScript);
	
	CString strRight;
	if (parser.HasAssignment(_T("DataEntry.var"), strRight))
	{
		if (strRight[0] == _T('"') && strRight[strRight.GetLength() - 1] == _T('"'))
			strRight = strRight.Mid(1, strRight.GetLength() - 2);

		SetDlgItemText(IDC_EXPRESSION, strRight);
	}

	if (parser.HasAssignment(_T("DataEntry.mode"), strRight))
	{
		for (int i = 0; i < 4; i++)
		{
			if (strRight.CompareNoCase(_gszModeString[i]) == 0)
			{
				ActiveEntryModePage(i);
				m_nMode = i;
				DoDataExchange();
				return;
			}
		}
	}

	ActiveEntryModePage(m_nMode);
}

void CDataEntryDlg::OnOK()
{
	USES_CONVERSION;

	DoDataExchange(TRUE);

	CString strActionScript;
	strActionScript.Format(_T("	DataEntry.mode = %s"), _gszModeString[m_nMode]);
	
	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, bstr.m_str); 
	if (bstr.Length() > 0)
	{
		strActionScript += _T("\r\n	DataEntry.var = ");
		strActionScript += _T("\"");
		strActionScript += OLE2T(bstr);
		strActionScript += _T("\"");
	}
	
	
	CTabPage* pPage = m_apPages[m_nMode];
	if (pPage != NULL)
	{
		switch (m_nMode)
		{
		case 0:
		case 2:
			((CNumericEntryPage *)pPage)->m_strScript = strActionScript;
			pPage->Apply();
			strActionScript = ((CNumericEntryPage *)pPage)->m_strScript;
			break;
		case 1:
			break;
		case 3:
			((CPushbuttonEntryPage *)pPage)->m_strScript = strActionScript;
			pPage->Apply();
			strActionScript = ((CPushbuttonEntryPage *)pPage)->m_strScript;
			break;
		default:
			break;
		}
	}

	strActionScript += _T("\r\n	DataEntry.show");
	m_pExpert->UpdateActionScript(_T(" ˝æ› ‰»Î"), strActionScript);
}

LRESULT CDataEntryDlg::OnClickedVar(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;
	
	CComBSTR bstr;
	GetDlgItemText(IDC_EXPRESSION, bstr.m_str); 

	CComQIPtr<ICxExpertPageSite> spExperSite(m_pExpert->m_pPageSite);
	spExperSite->OnGetDataSource(&bstr);

	SetDlgItemText(IDC_EXPRESSION, W2T(bstr));
	
	return 0;
}