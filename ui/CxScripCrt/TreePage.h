// TreePage.h: interface for the CTreePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREEPAGE_H__576E4895_5983_4F55_97E1_5D6CBD15E5EB__INCLUDED_)
#define AFX_TREEPAGE_H__576E4895_5983_4F55_97E1_5D6CBD15E5EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols

#include "../CxOle/CxHmiObj.h"
#include "../CxOle/CxHmiMisc.h"


/////////////////////////////////////////////////////////////////////////////
// CTreePage

#define WM_SYMBOL_SELECT WM_USER + 100

class CTreeNode
{
public:
	CTreeNode(CTreeNode* pParentNode = NULL)
	{
		m_pParentNode = pParentNode;
	}

	~CTreeNode()
	{
		for (int i = 0; i < m_apChildNodes.GetSize(); i++)
			delete m_apChildNodes[i];

		m_apChildNodes.RemoveAll();
	}

	void AddChildNode(CTreeNode* pNode)
	{
		m_apChildNodes.Add(pNode);
	}

	int GetChildNodeCount()
	{
		return m_apChildNodes.GetSize();
	}

	CTreeNode* GetChildNode(int i)
	{
		return m_apChildNodes[i];
	}

	CTreeNode* m_pParentNode;
	CSimpleArray<CTreeNode *> m_apChildNodes;

	CString m_strName;
	CComPtr<IDispatch> m_spObject;
	CComPtr<IUnknown> m_spunkEumProp;
};

class CTreePageBase : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTreePageBase, &CLSID_CxSymbolTreePage>,
	public IDispatchImpl<ICxSymbolTreePage, &IID_ICxSymbolTreePage, &LIBID_CXSCRIPCRTLib>,
	public CDialogImpl<CTreePageBase>
{
public:
	CTreePageBase()
	{
		m_bWithList = FALSE;

		m_pSymbolTreeSite = NULL;
		m_bDisplayIcon = TRUE;
		m_HasNodeExpanded = FALSE;

		m_pRootNode = NULL;
		m_pPopulatedRootNode = NULL;

		m_hNotifyWindow = NULL;
		m_bDblClickSelect = TRUE;

		m_bObjectHiberarchy = TRUE;
	}
	virtual ~CTreePageBase()
	{
		if (m_hWnd != NULL)
		{
			DestroyWindow();
			m_hWnd = NULL;
		}

		if (m_pPopulatedRootNode)
			delete m_pPopulatedRootNode;
	}

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTreePageBase)
	COM_INTERFACE_ENTRY(ICxSymbolTreePage)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	enum { IDD = IDD_TREE_LIST };

BEGIN_MSG_MAP(CTreePageBase)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	NOTIFY_HANDLER(IDC_TREE, TVN_ITEMEXPANDING, OnItemexpandingNode)
	NOTIFY_CODE_HANDLER(NM_DBLCLK, OnDblclkSymbol)
	NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnSelchangedTree)
	NOTIFY_HANDLER(IDC_LIST, LVN_ITEMCHANGED, OnItemchangedList)
END_MSG_MAP()

public:
	HWND Create(HWND hWndParent);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnItemexpandingNode(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnDblclkSymbol(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangedTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnItemchangedList(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	HRESULT LoadProps(HTREEITEM hParentItem, IDispatch* pIObject);
	HRESULT LoadProps(HTREEITEM hParentItem, ICxEnumExtendProperty* pEumProp);

	STDMETHOD(ExpandNode)(OLE_HANDLE hItem);
	STDMETHOD(HasNodeExpanded)();
	STDMETHOD(SetDefaultNameSpace)(BSTR bstrName);
	STDMETHOD(SetSymbolTreeSite)(IUnknown* pSite);
	STDMETHOD(Populate)();
	STDMETHOD(InsertNode)(OLE_HANDLE hParentItem, BSTR bstrName, 
		IDispatch* pObject, IUnknown* punkEumProp, OLE_HANDLE* phItem);
	STDMETHOD(get_Caption)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Caption)(/*[in]*/ BSTR newVal);

	void InsertNode(HTREEITEM hParentItem, CTreeNode* pNode);

	BOOL GetObjectMembers(ITypeInfo* pTypeInfo, 
		CStringArray& arrProperties, CStringArray& arrMethods);

	BOOL GetObjectMembers(IDispatch* pDisp, ICxEnumExtendProperty* pEnumProp,
		CStringArray& arrProperties, CStringArray& arrMethods);

	BOOL GetObjectMembers(LPCTSTR lpszObject, 
		CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods);

	void DeleteAllChildItem(HTREEITEM hItem);

	virtual void LoadObjectItems(HTREEITEM hParentItem);

	virtual CString GetQualifyNameSpace(HTREEITEM hItem);
	CString GetSelectNameSpace();

	void TruncateNameSpace(CString& strText);

	virtual void OnInit();

	HTREEITEM FindChildItemByName(HTREEITEM hItem, LPCTSTR szItem);
public:
	CSplitterWindow m_winSplitter;

	CTreeViewCtrl m_tree; 
	CListViewCtrl m_list;

	BOOL m_bWithList;

	CString m_strCaption;
	BOOL m_bDisplayIcon;
	BOOL m_HasNodeExpanded;

	ICxSymbolTreeSite* m_pSymbolTreeSite;
	CString m_strDefaultNameSpace;

	CString m_strSelectNameSpace;
	BOOL m_bObjectHiberarchy;

	HWND m_hNotifyWindow;
	BOOL m_bDblClickSelect;

	CTreeNode* m_pRootNode;
	CTreeNode* m_pPopulatedRootNode;
};

typedef CComObject<CTreePageBase> CTreePage;

#endif // !defined(AFX_TREEPAGE_H__576E4895_5983_4F55_97E1_5D6CBD15E5EB__INCLUDED_)
