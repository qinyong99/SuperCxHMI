// TagPickCtrl.h : Declaration of the CTagPickCtrl

#ifndef __CXTAGPICKCTRL_H_
#define __CXTAGPICKCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "Node.h"
#include "BitmapButton2.h"
#include "TagPickCtrlCP.h"

/////////////////////////////////////////////////////////////////////////////
// CTagPickCtrl
class ATL_NO_VTABLE CTagPickCtrl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICxTagPickCtrl, &IID_ICxTagPickCtrl, &LIBID_CXTAGPICKLib>,
	public CComCompositeControl<CTagPickCtrl>,
	public IPersistStreamInitImpl<CTagPickCtrl>,
	public IOleControlImpl<CTagPickCtrl>,
	public IOleObjectImpl<CTagPickCtrl>,
	public IOleInPlaceActiveObjectImpl<CTagPickCtrl>,
	public IViewObjectExImpl<CTagPickCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CTagPickCtrl>,
	public IConnectionPointContainerImpl<CTagPickCtrl>,
	public IPersistStorageImpl<CTagPickCtrl>,
	public IPersistPropertyBagImpl<CTagPickCtrl>,
	public ISpecifyPropertyPagesImpl<CTagPickCtrl>,
	public IQuickActivateImpl<CTagPickCtrl>,
	public IDataObjectImpl<CTagPickCtrl>,
	public IProvideClassInfo2Impl<&CLSID_CxTagPickCtrl, &DIID__ICxTagPickCtrlEvents, &LIBID_CXTAGPICKLib>,
	public IPropertyNotifySinkCP<CTagPickCtrl>,
	public CComCoClass<CTagPickCtrl, &CLSID_CxTagPickCtrl>,
	public CProxy_ICxTagPickCtrlEvents<CTagPickCtrl>
{
public:
	CTagPickCtrl()
	{
		m_pRootNode = NULL;
		m_pGroupNode = NULL;

		m_bUseLocalSetup = TRUE;
		m_bDisableAssoc = FALSE;
		m_bBrowsed = FALSE;

		m_bThisNodeAssoc = FALSE;

		m_bShowGroupTree = TRUE;
		m_bUseAssocGroup = FALSE;
		m_nViewListMode = 3;
		m_bShowAnalog = TRUE;
		m_bShowDigital = TRUE;
		m_bShowText = TRUE;

		for (int i = 0; i < 8; i++)
			m_arrFilterPattern.Add(CString(_T("")));

		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXTAGPICKCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CTagPickCtrl)
	COM_INTERFACE_ENTRY(ICxTagPickCtrl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IPersistPropertyBag)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CTagPickCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CTagPickCtrl)
	CONNECTION_POINT_ENTRY(DIID__ICxTagPickCtrlEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CTagPickCtrl)
	CHAIN_MSG_MAP(CComCompositeControl<CTagPickCtrl>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	NOTIFY_HANDLER(IDC_TAGGROUP, TVN_ITEMEXPANDING, OnItemexpandingGroup)
	NOTIFY_HANDLER(IDC_TAGGROUP, TVN_SELCHANGED, OnSelchangedGroup)
	NOTIFY_HANDLER(IDC_TAG, LVN_ITEMCHANGED, OnItemchangedTag)
	COMMAND_HANDLER(IDC_PROP, LBN_SELCHANGE, OnSelchangeProp)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	COMMAND_HANDLER(IDC_SHOW_GROUP, BN_CLICKED, OnClickedShowGroup)
	COMMAND_HANDLER(IDC_USE_GROUP, BN_CLICKED, OnClickedUseGroup)
	COMMAND_HANDLER(IDC_VIEW_LISTMODE, BN_CLICKED, OnClickedViewListmode)
	COMMAND_ID_HANDLER(ID_VIEW_SMALLICONS, OnViewSmallIcons)
	COMMAND_ID_HANDLER(ID_VIEW_LARGEICONS, OnViewLargeIcons)
	COMMAND_ID_HANDLER(ID_VIEW_LIST, OnViewList)
	COMMAND_ID_HANDLER(ID_VIEW_DETAILS, OnViewDetails)
	COMMAND_HANDLER(IDC_ANALOG, BN_CLICKED, OnClickedAnalog)
	COMMAND_HANDLER(IDC_DIGITAL, BN_CLICKED, OnClickedDigital)
	COMMAND_HANDLER(IDC_TEXT, BN_CLICKED, OnClickedText)
	COMMAND_HANDLER(IDC_FILTER, BN_CLICKED, OnClickedFilter)
	NOTIFY_HANDLER(IDC_TAG, LVN_COLUMNCLICK, OnColumnclickTag)
	COMMAND_HANDLER(IDC_FILTER_PATTERN, CBN_EDITCHANGE, OnEditchangeFilterPattern)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CTagPickCtrl)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CTagPickCtrl>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

	enum { IDD = IDD_CXTAGPICKCTRL };

public:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnItemexpandingGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangedGroup(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnItemchangedTag(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnSelchangeProp(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnColumnclickTag(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnEditchangeFilterPattern(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);

	LRESULT OnClickedShowGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedUseGroup(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedViewListmode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedAnalog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedDigital(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedText(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedFilter(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnViewSmallIcons(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewLargeIcons(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewList(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnViewDetails(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	void InitGroupTree(BOOL bAssoc);
	HTREEITEM InsertNode(HTREEITEM hParent, CNode* pNode);
	void ExpandNode(HTREEITEM hItem);

	CString GetTagGroup(HTREEITEM hItem);
	HRESULT LoadTagsOfGroup(LPCTSTR szGroupName);
	HRESULT LoadFieldsOfTag(LPCTSTR szTagName);

	void DeleteAllChildItem(HTREEITEM hItem);

	void OnBrowsedChanged();

	void DisplayMatchTag(LPCTSTR szTagName);

	HTREEITEM FindChildItemByName(HTREEITEM hItem, LPCTSTR szItem);
	int FindTagItemByName(LPCTSTR szItem);
	int FindPropItemByName(LPCTSTR szItem);

	void UpdateViewListMode();
	void UpdateFilterPatter();

	static int CALLBACK CompareItems(CTag* pTag1, CTag* pTag2, LPARAM nCol);

// ICxTagPickCtrl
public:

	
private:
	CTreeViewCtrl m_tree;   //标签组
	CListViewCtrl m_listTag;		//标签
	CComboBox  m_cmbProp;	//属性

	CImageList m_ImageList;
	CImageList m_ImageList2;

	CImageList m_LargeImageList;
	CImageList m_SmallImageList;

	CString m_strTagGroup;
	CString	m_strTag;
	CString m_strProp;

	CComBSTR m_bstrSelectedLast;

	BOOL m_bNoNotifyTreeSelect;
	
	CBitmapButton2 m_btnShowGroupTree;
	CBitmapButton2 m_btnUseAssocGroup;
	CBitmapButton2 m_btnViewListMode;
	CBitmapButton2 m_btnAddGroup;
	CBitmapButton2 m_btnAddTag;
	CBitmapButton2 m_btnAnalog;
	CBitmapButton2 m_btnDigital;
	CBitmapButton2 m_btnText;
	CComboBox m_cmbFilterPattern;
	CBitmapButton2 m_btnFilter;

	CSplitterWindow m_winSplitter;

	static bool m_bSortAscending;
	int m_nLastSortColumn;

	CString m_strLastSelect;

public:
	STDMETHOD(get_FilterPattern8)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern8)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern7)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern7)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern6)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern6)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern5)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern5)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern4)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern4)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern3)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern3)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern2)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern2)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_FilterPattern1)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_FilterPattern1)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_ShowText)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ShowText)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_ShowDigital)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ShowDigital)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_ShowAnalog)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ShowAnalog)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_ViewListMode)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_ViewListMode)(/*[in]*/ long newVal);
	STDMETHOD(get_UseAssocGroup)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_UseAssocGroup)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_ShowGroupTree)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ShowGroupTree)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_Browsed)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_Browsed)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_DisableAssoc)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_DisableAssoc)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_UseLocalSetup)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_UseLocalSetup)(/*[in]*/ BOOL newVal);
	STDMETHOD(get_AssocGroup)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_AssocGroup)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Value)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Value)(/*[in]*/ BSTR newVal);

public:
	CRootNode* m_pRootNode;
	CNode* m_pGroupNode;
	
	CString m_strAssocTagGroup; //关联标签组
	BOOL m_bUseLocalSetup;
	BOOL m_bDisableAssoc;

	BOOL m_bShowGroupTree;
	BOOL m_bUseAssocGroup;
	int m_nViewListMode;
	BOOL m_bShowAnalog;
	BOOL m_bShowDigital;
	BOOL m_bShowText;
	CSimpleArray<CString> m_arrFilterPattern;
	
	BOOL m_bThisNodeAssoc;
	
	BOOL m_bBrowsed;
};

BOOL MatchPattern(LPCWSTR lpszString, LPCWSTR lpszPattern, BOOL bCaseSensitive = FALSE);

#endif //__CXTAGPICKCTRL_H_
