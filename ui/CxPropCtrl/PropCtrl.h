// PropCtrl.h : Declaration of the CPropCtrl

#ifndef __PROPERTYCTRL_H_
#define __PROPERTYCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>

#include "Property.h"
#include "PropGuids.h"
#include "PropertyCtrlCP.h"

typedef CContainedWindowT<CListViewCtrl> CContainedList;
typedef CContainedWindowT<CEdit> CContainedEdit;
typedef CContainedWindowT<CHeaderCtrl> CContainedHeader;

#define SIGN_OFFSET 17
#define DYNAMIC_WIDTH 32

/////////////////////////////////////////////////////////////////////////////
// CPropCtrl
class ATL_NO_VTABLE CPropCtrl : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IPropertyCtrl, &IID_IPropertyCtrl, &LIBID_CXPROPCTRLLib>,
	public CComControl<CPropCtrl>,
	public IPersistStreamInitImpl<CPropCtrl>,
	public IOleControlImpl<CPropCtrl>,
	public IOleObjectImpl<CPropCtrl>,
	public IOleInPlaceActiveObjectImpl<CPropCtrl>,
	public IViewObjectExImpl<CPropCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CPropCtrl>,
	public IPersistStorageImpl<CPropCtrl>,
	public IConnectionPointContainerImpl<CPropCtrl>,
	public ISpecifyPropertyPagesImpl<CPropCtrl>,
	public IQuickActivateImpl<CPropCtrl>,
	public IDataObjectImpl<CPropCtrl>,
	public IProvideClassInfo2Impl<&CLSID_PropertyCtrl, &DIID__IPropertyCtrlEvents, &LIBID_CXPROPCTRLLib>,
	public IPropertyNotifySinkCP<CPropCtrl>,
	public CComCoClass<CPropCtrl, &CLSID_PropertyCtrl>,
	public CPropertyArray,
	public CProxy_IPropertyCtrlEvents<CPropCtrl>
{
public:
	CPropCtrl();

DECLARE_REGISTRY_RESOURCEID(IDR_PROPERTYCTRL)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPropCtrl)
	COM_INTERFACE_ENTRY(IPropertyCtrl)
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
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(CPropCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CPropCtrl)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
	CONNECTION_POINT_ENTRY(DIID__IPropertyCtrlEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CPropCtrl)
	CHAIN_MSG_MAP(CComControl<CPropCtrl>)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_MEASUREITEM, OnMeasureItem)
	MESSAGE_HANDLER(WM_DRAWITEM, OnDrawItem)
 	MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	COMMAND_ID_HANDLER(ID_ALPHABETIC, OnAlphabetick)
	COMMAND_ID_HANDLER(ID_CATEGORIZED, OnCategorized)
	COMMAND_ID_HANDLER(ID_PROPERTY, OnProperty)
	COMMAND_HANDLER(101, CBN_SELCHANGE, OnSelchangeCombo)
	COMMAND_HANDLER(101, CBN_DROPDOWN, OnDropdownCombo)
	DEFAULT_REFLECTION_HANDLER()
ALT_MSG_MAP(1)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeList)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDownList)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClkList)
	MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenuList)
	MESSAGE_HANDLER(WM_VSCROLL,	OnVScrollList)
// 	MESSAGE_HANDLER(WM_NCLBUTTONDOWN, OnKillFocusEdit)
ALT_MSG_MAP(2)
	MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocusEdit)
	MESSAGE_HANDLER(WM_KEYDOWN,	OnKeyDownEdit)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCodeEdit)
ALT_MSG_MAP(3)
	MESSAGE_HANDLER(WM_PAINT, OnPaintHeader)
	MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessageHeader)
	MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursorHeader)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnCreate(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMeasureItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDrawItem(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnDropdownCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSelchangeCombo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnGetDlgCodeList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDownList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDblClkList(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled);
	LRESULT OnContextMenuList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnVScrollList(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetDlgCodeEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	
	LRESULT OnKeyDownEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocusEdit(UINT nMessage, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnPaintHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMessageHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursorHeader(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
	LRESULT OnAlphabetick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCategorized(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnProperty(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	void OnFinalMessage(HWND hWnd);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IPropertyCtrl
public:
	STDMETHOD(Refresh)();
	STDMETHOD(get_ShowDynamic)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_ShowDynamic)(/*[in]*/ BOOL newVal);

	STDMETHOD(get_NameShowHelpString)(/*[out, retval]*/ BOOL *pVal);
	STDMETHOD(put_NameShowHelpString)(/*[in]*/ BOOL newVal);
	STDMETHOD(AddComboString)(BSTR bstrItemName);
	STDMETHOD(SetSelectedObject)(ULONG nObjects, IUnknown **ppUnk, BSTR bstrItemName, IUnknown** ppEnumUnk);
	STDMETHOD(GetSelectedItemName)(BSTR *pbstrItemName);

public:
	HRESULT CrackPerPropertyBrowsing(IPerPropertyBrowsing* ppb, MEMBERID memid, CProperty*& pprop);
	HRESULT CrackUserDefinedProperty(ITypeInfo* pti, HREFTYPE hrt, CProperty*& pprop);
	HRESULT CrackProperty(ITypeInfo* pti, TYPEDESC& tdesc, CProperty*& pprop);	
	
	HRESULT ExploreTypeInfo(ITypeInfo* pTypeInfo, IPerPropertyBrowsing* ppb, ICategorizeProperties* pCP);
	HRESULT ExploreExtendProperty(ICxEnumExtendProperty* pEnum, ICategorizeProperties* pCP);
	void AddProperty(CProperty*, ICategorizeProperties* pCP, BOOL bPutRef = FALSE);

	void MakeListView();
	void SortArrayCat();
	int Fill_ListView(HWND, int iIndex);
	int CalcNameColumnWidth();	

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

	HRESULT DisplayDynamicDialog(CProperty* pProp, int nIndex);
	HRESULT GetDynamic(IUnknown* punkObj, BSTR bstrPropName, IUnknown** ppunkDyn);
	HRESULT PutDynamic(IUnknown* punkObj, BSTR bstrPropName, IUnknown* punkDyn);
	HRESULT DeleteDynamic(IUnknown* punkObj, BSTR bstrPropName);

	HRESULT OnDraw(ATL_DRAWINFO& di);
	BOOL PreTranslateAccelerator(LPMSG pMsg, HRESULT& hRet);

	void DrawComboItem(DRAWITEMSTRUCT* lpDrawItemStruct);
	void DrawListItem(DRAWITEMSTRUCT* lpDrawItemStruct);

	void UpdateToolBar();
	void ResizeColumns();

	CComboBox m_combo;
	CToolBarCtrl m_wndToolBar;
	CContainedList m_list;
	CContainedEdit m_editInplace;
	CContainedHeader m_wndHeader;

	BOOL m_bShowCat;
	BOOL m_bShowHelpString;

	CFont m_font;
	CFont m_fontBold;
	CFont m_fontWinnding;
	CPen m_penGrid;

	int m_nSignOffset;
	int m_nNameWidth;
	int m_nValueWidth;
	int m_nDynamicWidth;

	CProperty* m_propEdit;
	
	ULONG m_nObjects;
	ULONG m_nObjectClasses;
	IUnknown** m_ppUnk;
};

#endif //__PROPERTYCTRL_H_
