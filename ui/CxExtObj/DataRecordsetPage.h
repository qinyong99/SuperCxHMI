// DataRecordsetPage.h : Declaration of the CDataRecordsetPage

#ifndef __CXDATARECORDSETPAGE_H_
#define __CXDATARECORDSETPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxDataRecordsetPage;

/////////////////////////////////////////////////////////////////////////////
// CDataRecordsetPage
class ATL_NO_VTABLE CDataRecordsetPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDataRecordsetPage, &CLSID_CxDataRecordsetPage>,
	public IPropertyPageImpl2<CDataRecordsetPage>,
	public CDialogImpl<CDataRecordsetPage>
{
public:
	CDataRecordsetPage() 
	{
		m_dwTitleID = IDS_TITLECxDataRecordsetPage;
		m_dwHelpFileID = IDS_HELPFILECxDataRecordsetPage;
		m_dwDocStringID = IDS_DOCSTRINGCxDataRecordsetPage;
	}

	enum {IDD = IDD_CXDATARECORDSETPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXDATARECORDSETPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDataRecordsetPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CDataRecordsetPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CDataRecordsetPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_COMMAND_TYPE, CBN_SELCHANGE, OnSelchangeCommandType)
	COMMAND_HANDLER(IDC_BUILD_SQL, BN_CLICKED, OnClickedBuildSql)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)();

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSelchangeCommandType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedBuildSql(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	void UpdateWindowsState();
	void RefreshDatabaseObject();
	HRESULT GetConnectionString(CComBSTR& bstrText);

	CComboBox m_cmbCommandType;
	CComboBox m_cmdDatabaseObject;
	CEdit m_editCommandText;
	CButton m_btnSQLBuild;
};

#endif //__CXDATARECORDSETPAGE_H_
