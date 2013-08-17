////////////////////////////////////////////////////////////////
// ExpropItemDlg.cpp : implementation file

#include "stdafx.h"
#include "DevInc.h"
#include "SymbolPropList.h"
#include "SymbolPropDlg.h"
#include "SymbolDesignDlg.h"

#include "TypeChanger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropDlg dialog


CSymbolPropDlg::CSymbolPropDlg(CSymbolProp* pProp, CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolPropDlg)
	m_strDes = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_type = PROPTYPE_UNKNOWN;
	m_pObject = NULL;
	m_pPropList = NULL;
	m_pProp = pProp;
}


void CSymbolPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolPropDlg)
	DDX_Control(pDX, IDC_EDIT_VALID, m_editValid);
	DDX_Control(pDX, IDC_BUTTON_ENUM, m_btnEnum);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_listType);
	DDX_Text(pDX, IDC_EDIT_DES, m_strDes);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_VALID, m_strValidScript);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolPropDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolPropDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_ENUM, OnButtonEnum)
	ON_BN_CLICKED(IDC_BUTTON_VALEXP, OnButtonValexp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropDlg message handlers

BOOL CSymbolPropDlg::OnInitDialog() 
{
	m_strName = m_pProp->m_strName;
	m_strDes = m_pProp->m_strDescription;
	m_type = m_pProp->m_proptype;
	m_value = m_pProp->m_value;
	
	m_strValidScript = m_pProp->m_strValidScript;
	int nCount = m_pObject->m_arrSubObj.GetSize();
	CString strName;
	CString strOld;
	for (int i = 0; i < nCount; i++)
	{
		strName = m_pObject->m_arrSubObj[i]->GetDisplayName();
		strOld.Format("object%03d", i);
		m_strValidScript.Replace(strOld, strName);
	}
	
	CDialog::OnInitDialog();
	
	int count = CTypeChanger::GetExTypeCount();
	for(i = 0; i < count; i++)
	{
		enumPropType type = CTypeChanger::GetExTypeID(i);
		CString strName = CTypeChanger::GetExTypeName(type);
		int index = m_listType.AddString(strName);
		m_listType.SetItemData(index, (DWORD)type);

		strName = CTypeChanger::GetExTypeName(m_type);
		m_listType.SelectString(-1, strName);
	}

	IDispatchPtr pIValue = GetDlgItem(IDC_VALUE)->GetControlUnknown();
	COleDispatchDriver dd(pIValue, FALSE);
	dd.SetProperty(1, VT_I4, long(m_type));
	dd.SetProperty(2, VT_VARIANT, &m_value);
	
	m_btnEnum.ShowWindow(m_type == PROPTYPE_ENUM ? SW_SHOW : SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolPropDlg::OnOK() 
{
	int index = m_listType.GetCurSel();
	if(index >= 0)
	{
		m_type = (enumPropType)(m_listType.GetItemData(index));
	}

	IDispatchPtr pIValue = GetDlgItem(IDC_VALUE)->GetControlUnknown();
	COleDispatchDriver dd(pIValue, FALSE);
	dd.GetProperty(2, VT_VARIANT, &m_value);

	CDialog::OnOK();

	if (!m_pPropList->IsNameDuplicate(m_strName))
			m_pProp->m_strName = m_strName;
	m_pProp->m_strDescription = m_strDes;
	m_pProp->m_proptype = m_type;
	m_pProp->m_value = m_value;
	CString strName;
	CString strNew;
	int nCount = m_pObject->m_arrSubObj.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		strName = m_pObject->m_arrSubObj[i]->GetDisplayName();
		strNew.Format("object%03d", i);
		m_strValidScript.Replace(strName, strNew);
	}
	m_pProp->m_strValidScript = m_strValidScript;
}

void CSymbolPropDlg::OnSelchangeComboType() 
{
	int index = m_listType.GetCurSel();
	if (index >= 0)
	{
		m_type = (enumPropType)(m_listType.GetItemData(index));
	}

	IDispatchPtr pIValue = GetDlgItem(IDC_VALUE)->GetControlUnknown();
	COleDispatchDriver dd(pIValue, FALSE);
	dd.SetProperty(1, VT_I4, long(m_type));
	dd.SetProperty(2, VT_VARIANT, &m_value);

	m_btnEnum.ShowWindow(m_type == PROPTYPE_ENUM ? SW_SHOW : SW_HIDE);
}

void CSymbolPropDlg::OnButtonEnum() 
{
	// TODO: Add your control notification handler code here
}

void CSymbolPropDlg::OnButtonValexp() 
{
	CString strValid;
	m_editValid.GetWindowText(strValid);
	BSTR bstr = strValid.AllocSysString();

	//用新属性名称暂时代替属性名称
	UpdateData();
	CString strOldName = m_pProp->m_strName;
	m_pProp->m_strName = m_strName;

	CSymbolDesignDlg::m_pExpCrtObject = m_pObject;
	CSymbolDesignDlg::m_pExpCrtList = m_pPropList;
	CSymbolDesignDlg::ExpressionCreatorProc(&bstr);

	m_pProp->m_strName = strOldName;

	strValid = bstr;
	m_editValid.SetWindowText(strValid);
	SysFreeString(bstr);
}
