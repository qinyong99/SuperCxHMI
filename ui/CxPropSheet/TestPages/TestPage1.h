// TestPage1.h : Declaration of the CTestPage1
// 所属项目: AutoCx的TestPages工程
// 创建日期: 2001.10.8
// 创 建 人：孙越洋 
// 功能描述：测试CxPropSheet工程的测试属性页对象 
// 修改记录：
/////////////////////////////////////////////////////////////////////////////

#ifndef __TESTPAGE1_H_
#define __TESTPAGE1_H_

#include "..\..\cxole\cxole.h"
#include "resource.h"       // main symbols
#include <stdio.h>

EXTERN_C const CLSID CLSID_TestPage1;

/////////////////////////////////////////////////////////////////////////////
// CTestPage1
class ATL_NO_VTABLE CTestPage1 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTestPage1, &CLSID_TestPage1>,
	public IPropertyPageImpl<CTestPage1>,
	public CDialogImpl<CTestPage1>
{
public:
	CTestPage1() 
	{
		m_dwTitleID = IDS_TITLETestPage1;
		m_dwHelpFileID = IDS_HELPFILETestPage1;
		m_dwDocStringID = IDS_DOCSTRINGTestPage1;
	}

	enum {IDD = IDD_TESTPAGE1};

DECLARE_REGISTRY_RESOURCEID(IDR_TESTPAGE1)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTestPage1) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CTestPage1)
	CHAIN_MSG_MAP(IPropertyPageImpl<CTestPage1>)
	COMMAND_ID_HANDLER(IDC_BUTTON1, OnTest)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void)
	{
		ATLTRACE(_T("CTestPage1::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			// Do something interesting here
			// ICircCtl* pCirc;
			// m_ppUnk[i]->QueryInterface(IID_ICircCtl, (void**)&pCirc);
			// pCirc->put_Caption(CComBSTR("something special"));
			// pCirc->Release();
		}
		m_bDirty = FALSE;
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////
	//功能:开始专用接口的测试
	//////////////////////////////////////////////////////////////////////
	LRESULT OnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		ATLTRACE(_T("CTestPage1::OnTest\n"));
		ICxPropertyPageSite* pPageSite = NULL;
		HRESULT hr = m_pPageSite->QueryInterface(IID_ICxPropertyPageSite, (void **)&pPageSite);
		if(hr != S_OK)
			return hr;
		static i = 0;
		pPageSite->SetDynamicIcon(i++);
		if(i > 3)
			i = 0;
		return 0;
	}
};

#endif //__TESTPAGE1_H_
