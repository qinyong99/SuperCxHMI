// ReportFieldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "ReportFieldDlg.h"
#include "ReportWizardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportFieldDlg dialog

IMPLEMENT_DYNCREATE(CReportFieldDlg, CDialog)

CReportFieldDlg::CReportFieldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReportFieldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportFieldDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pReportWizardDlg = NULL;
}


void CReportFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportFieldDlg)
	DDX_Control(pDX, IDC_RIGHT, m_listRight);
	DDX_Control(pDX, IDC_LEFT, m_listLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportFieldDlg, CDialog)
	//{{AFX_MSG_MAP(CReportFieldDlg)
	ON_BN_CLICKED(IDC_RIGHT_ONE, OnRightOne)
	ON_BN_CLICKED(IDC_RIGHT_ALL, OnRightAll)
	ON_BN_CLICKED(IDC_LEFT_ONE, OnLeftOne)
	ON_BN_CLICKED(IDC_LEFT_ALL, OnLeftAll)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(CXMSG_DIALOG_APPLY, OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportFieldDlg message handlers

BOOL CReportFieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pReportWizardDlg = (CReportWizardDlg *)GetParent();
	LoadList();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportFieldDlg::LoadList()
{
	USES_CONVERSION;

	ASSERT(m_pReportWizardDlg != NULL);

	if (m_pReportWizardDlg->m_pDataRecordset->Open() == S_OK)
	{
		long lFieldCount;
		m_pReportWizardDlg->m_pDataRecordset->get_FieldCount(&lFieldCount);
		for (long l = 0; l < lFieldCount; l++)
		{
			ICxDataFieldPtr pDataField;
			CComVariant vaIndex = l + 1;
			m_pReportWizardDlg->m_pDataRecordset->get_Field(vaIndex, &pDataField);
			CComBSTR bstrFieldName;
			pDataField->get_Name(&bstrFieldName);
			long lDefinedSize;
			pDataField->get_DefinedSize(&lDefinedSize);
			enumDataType nType; 
			pDataField->get_Type((enumDataType *)&nType);

			enumTextAlign nAlign = TextAlignLeft;

			switch (nType) 
			{
			case DataTypeEmpty:
				lDefinedSize = 0;
				
				break;
			case DataTypeTinyInt:
			case DataTypeUnsignedTinyInt:
			case DataTypeBoolean:
				lDefinedSize = 3;
				nAlign = TextAlignRight;

				break;
			case DataTypeSmallInt:
			case DataTypeUnsignedSmallInt:
				lDefinedSize = 5;
				nAlign = TextAlignRight;
				
				break;
			case DataTypeInteger:
			case DataTypeUnsignedInt:
			case DataTypeSingle:
			case DataTypeDecimal:
			case DataTypeNumeric:
			case DataTypeDBDate:
			case DataTypeDBTime:
			case DataTypeVariant:
				lDefinedSize = 10;
				nAlign = TextAlignRight;

				break;
			case DataTypeBigInt:
			case DataTypeUnsignedBigInt:
			case DataTypeDouble:
			case DataTypeCurrency:
				lDefinedSize = 20;
				nAlign = TextAlignRight;
				
				break;
			case DataTypeDate:
			case DataTypeDBTimeStamp:
			case DataTypeFileTime:
			case DataTypeDBFileTime:
				lDefinedSize = 23; // 2006-12-19 12:00:00.000
				nAlign = TextAlignRight;
				
				break;
			case DataTypeWChar:
			case DataTypeBSTR:
				lDefinedSize = min(30, lDefinedSize / 2 + 5);
			
				break;
			case DataTypeChar:
				lDefinedSize = min(30, lDefinedSize + 5);

				break;
			default:
				lDefinedSize = min(30, lDefinedSize + 5);
				break;
			}

			CReportField field;
			field.m_strName = bstrFieldName;
			field.m_nAlign = nAlign;
			field.m_nSize = lDefinedSize;
			m_arrFields.Add(field);

			CString strText = bstrFieldName;
			BOOL bSelect = FALSE;
			for (int i = 0; i < m_pReportWizardDlg->m_arrFields.GetSize(); i++)
			{
				if (strText == m_pReportWizardDlg->m_arrFields[i].m_strName)
				{
					bSelect = TRUE;
					break;
				}
			}
			if (!bSelect)
			{
				int nItem = m_listLeft.AddString(strText);
				m_listLeft.SetItemData(nItem, l);
			}
			else
			{
				int nItem = m_listRight.AddString(strText);
				m_listRight.SetItemData(nItem, l);
			}
		}
	}
}

void CReportFieldDlg::OnRightOne() 
{
	int iIndex = m_listLeft.GetCurSel();
	if (iIndex != LB_ERR)
	{
		CString strText;
		m_listLeft.GetText(iIndex, strText);
		if (m_listRight.FindStringExact(-1, strText) == LB_ERR)
		{
			long lDefinedSize = m_listLeft.GetItemData(iIndex);
			m_listLeft.DeleteString(iIndex);

			int nItem = m_listRight.AddString(strText);
			m_listRight.SetItemData(nItem, lDefinedSize);
		}
	}
}

void CReportFieldDlg::OnRightAll() 
{
	for (int i = 0; i < m_listLeft.GetCount(); i++)
	{
		CString strText;
		m_listLeft.GetText(i, strText);
		long lDefinedSize = m_listLeft.GetItemData(i);
				
		int nItem = m_listRight.AddString(strText);
		m_listRight.SetItemData(nItem, lDefinedSize);
	}

	m_listLeft.ResetContent();
}

void CReportFieldDlg::OnLeftOne() 
{
	int iIndex = m_listRight.GetCurSel();
	if (iIndex != LB_ERR)
	{
		CString strText;
		m_listRight.GetText(iIndex, strText);
		if (m_listLeft.FindStringExact(-1, strText) == LB_ERR)
		{
			long lDefinedSize = m_listRight.GetItemData(iIndex);
			m_listRight.DeleteString(iIndex);
			
			int nItem = m_listLeft.AddString(strText);
			m_listLeft.SetItemData(nItem, lDefinedSize);
		}
	}
}

void CReportFieldDlg::OnLeftAll() 
{
	for (int i = 0; i < m_listRight.GetCount(); i++)
	{
		CString strText;
		m_listRight.GetText(i, strText);
		long lDefinedSize = m_listRight.GetItemData(i);
			
		int nItem = m_listLeft.AddString(strText);
		m_listLeft.SetItemData(nItem, lDefinedSize);
	}

	m_listRight.ResetContent();
}

void CReportFieldDlg::OnApply()
{
	ASSERT(m_pReportWizardDlg != NULL);

	m_pReportWizardDlg->m_arrFields.RemoveAll();
	
	for (int i = 0; i < m_listRight.GetCount(); i++)
	{
		CReportField& field = m_arrFields[m_listRight.GetItemData(i)];
		m_pReportWizardDlg->m_arrFields.Add(field);
	}
}
