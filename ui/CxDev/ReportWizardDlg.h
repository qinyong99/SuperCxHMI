#if !defined(AFX_REPORTWIZARDDLG_H__F94EBE04_625A_46F4_8D10_958C814E9244__INCLUDED_)
#define AFX_REPORTWIZARDDLG_H__F94EBE04_625A_46F4_8D10_958C814E9244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportWizardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportWizardDlg dialog

#define CXMSG_DIALOG_APPLY WM_USER + 502

class CReportWizardDlg : public CDialog
{
// Construction
public:
	CReportWizardDlg(CWnd* pParent = NULL);   // standard constructor

	enum
	{
		ReportLayout_Columnar,
		ReportLayout_Tabular,
		ReportLayout_Justified,
	};

	enum
	{
		ReportOrientation_Portrait,
		ReportOrientation_Landscape,
	};

	

// Dialog Data
	//{{AFX_DATA(CReportWizardDlg)
	enum { IDD = IDD_REPORT_WIZARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	int m_nCurrentPage;

	ICxDataConnectionPtr m_pDataConnection;
	ICxDataRecordsetPtr m_pDataRecordset;
	CArray<CReportField, CReportField> m_arrFields;

	int m_nReportLayout;
	int m_nReportOrientation;
	
	CDialog* m_pDialog;
	IPropertyPagePtr m_pPropertyPage;

	void OnShowCurrentPage();
	HRESULT ShowOleProertyPage(CLSID clsidPage, IUnknown* pUnk);
	BOOL ShowDialogPage(CRuntimeClass* pDialogClass, UINT nIDTemplate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportWizardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportWizardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnNext();
	afx_msg void OnBack();
	afx_msg void OnFinish();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BEGIN_INTERFACE_PART(PageSite, IPropertyPageSite)
		STDMETHOD(OnStatusChange)(DWORD dwFlags);
		STDMETHOD(GetLocaleID)(LCID* pLocaleID);
		STDMETHOD(GetPageContainer)(IUnknown** ppUnk);
		STDMETHOD(TranslateAccelerator)(LPMSG pMsg);
	END_INTERFACE_PART(PageSite)

	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTWIZARDDLG_H__F94EBE04_625A_46F4_8D10_958C814E9244__INCLUDED_)
