// CCtrlItem.h : interface of the CCtrlItem class
//

#ifndef __CTRLITEM_H__
#define __CTRLITEM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DISPID_AMBIENT_ALLOWWINDOWLESSACTIVATION        0x60020000
#define DISPID_AMBIENT_DOCHOSTFLAGS						0x60020012
#define DISPID_AMBIENT_DOCHOSTDOUBLEFLAGS 				0x60020014
#define DISPID_AMBIENT_ALLOWCONTEXTMENU 				0x60020016
#define DISPID_AMBIENT_ALLOWSHOWUI						0x60020018
#define DISPID_AMBIENT_OPTIONKEYPATH					0x6002001a

const int MARGIN_PIXELS = 5;

class CFormDoc;
class CRunView;
class CCtrlObj;

_COM_SMARTPTR_TYPEDEF(ICxExtendedControl, _uuidof(ICxExtendedControl));

class CCtrlItem : public COleClientItem
{
	DECLARE_SERIAL(CCtrlItem)

// Constructors
public:
	CCtrlItem(CDevObjDoc* pContainer = NULL, CCtrlObj* pCtrlObj = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CFormDoc* GetDocument()
		{ return (CFormDoc*)COleClientItem::GetDocument(); }
	CRunView* GetActiveView()
		{ return (CRunView*)COleClientItem::GetActiveView(); }

	CCtrlObj* m_pCtrlObj;    // back pointer to OLE draw object

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);

	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnGetClipRect(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateOrLoad(REFCLSID clsid, 
		REFIID iidPersistanceMedium, IUnknown* pPeristanceMedium);
    BOOL FinishCreate(HRESULT hr);

	void OnFocus(BOOL bGotFocus);
	BOOL TranslateAccelerator(LPMSG pMessage, DWORD dwModifiers);
	

	DWORD HitTest(CPoint point);
	void DisplayAsDefault(BOOL tDefault);
	BOOL Draw(CDC* pDC, LPCRECT prectBounds, DVASPECT nDrawAspect = DVASPECT(-1 ));
	BOOL DrawBackToFront(CDC* pDC, LPCRECT prectBounds);
	BOOL DrawFrontToBack(CDC* pDC, LPCRECT prectBounds);
	DWORD GetActivationPolicy() const;
	
	DWORD OnPointerInactiveMove( CPoint point, DWORD dwKeyState );
	DWORD OnPointerInactiveSetCursor( CPoint point, DWORD dwMouseMessage,
			BOOL* ptHandled );
	BOOL ActsLikeButton() const;
	BOOL ShouldActivateWhenVisible();
	BOOL IsLabelControl() const;
	BOOL OnWindowMessage(const MSG* pMessage);
	BOOL IsWindowless() const;
	BOOL SupportsIPointerInactive() const;

	BOOL IsEnabled();

	ULONG m_nLeakRefCount;

	BOOL m_bDisplayAsDefault;
	BOOL m_bWindowless;
	DWORD m_dwAspectPass2;
	ICxExtendedControlPtr m_pExtendedControl;
	BOOL m_bExtendedControl;
	IDispatch* m_pDispatch;
	IOleControlPtr m_pOleControl;
	IOleInPlaceObjectWindowlessPtr m_pOleInPlaceObjectWindowless;
	IViewObjectExPtr m_pViewObjectEx;
	IPointerInactivePtr m_pPointerInactive;
	DWORD m_dwMiscStatus;
	DWORD m_dwViewStatus;

	IUnknown* GetInterfaceHook( const void* pv );
	BOOL IsInvisibleAtRuntime() const;
	BOOL IsUIActive() const;
	void OnDeactivate();
	void OnDeactivate(BOOL bNoRedraw);
	void ReadItem(CArchive& ar);
	BOOL UpdateExtent();

	HWND GetInPlaceObjectWindow();
	void BringWindowTop();
	void SetFocus();

	void OnActivateUI();

	~CCtrlItem();

protected:
	CDC* m_pDC;
	void Release(OLECLOSE dwCloseOption = OLECLOSE_NOSAVE);

	BEGIN_INTERFACE_PART(AmbientProperties, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(AmbientProperties)

	BEGIN_INTERFACE_PART(OleControlSite, IOleControlSite)
		STDMETHOD(GetExtendedControl)(IDispatch** ppExtendedControl);
		STDMETHOD(LockInPlaceActive)(BOOL tLock);
		STDMETHOD(OnControlInfoChanged)();
		STDMETHOD(OnFocus)(BOOL tGotFocus);
		STDMETHOD(ShowPropertyFrame)();
		STDMETHOD(TransformCoords)(POINTL* pptHiMetric, POINTF* pptContainer,
			DWORD dwFlags);
		STDMETHOD(TranslateAccelerator)(LPMSG pMessage, DWORD dwModifiers);
	END_INTERFACE_PART(OleControlSite)

	BEGIN_INTERFACE_PART(OleInPlaceSiteWindowless, IOleInPlaceSiteWindowless)
		STDMETHOD(ContextSensitiveHelp)(BOOL tEnterMode);
		STDMETHOD(GetWindow)(HWND* phWnd);

		STDMETHOD(CanInPlaceActivate)();
		STDMETHOD(DeactivateAndUndo)();
		STDMETHOD(DiscardUndoState)();
		STDMETHOD(GetWindowContext)(IOleInPlaceFrame** ppFrame,
			IOleInPlaceUIWindow** ppDoc, LPRECT prectPos, LPRECT prectClip,
			LPOLEINPLACEFRAMEINFO pFrameInfo);
		STDMETHOD(OnInPlaceActivate)();
		STDMETHOD(OnInPlaceDeactivate)();
		STDMETHOD(OnPosRectChange)(LPCRECT prectPos);
		STDMETHOD(OnUIActivate)();
		STDMETHOD(OnUIDeactivate)(BOOL tUndoable);
		STDMETHOD(Scroll)(SIZE sizeExtent);

		STDMETHOD(OnInPlaceActivateEx)(BOOL* ptNoRedraw, DWORD dwFlags);
		STDMETHOD(OnInPlaceDeactivateEx)(BOOL tNoRedraw);
		STDMETHOD(RequestUIActivate)();

		STDMETHOD(AdjustRect)(LPRECT prect);
		STDMETHOD(CanWindowlessActivate)();
		STDMETHOD(GetCapture)();
		STDMETHOD(GetDC)(LPCRECT prect, DWORD dwFlags, HDC* phDC);
		STDMETHOD(GetFocus)();
		STDMETHOD(InvalidateRect)(LPCRECT prect, BOOL tErase);
		STDMETHOD(InvalidateRgn)(HRGN hRgn, BOOL tErase);
		STDMETHOD(OnDefWindowMessage)(UINT nMessage, WPARAM wParam,
			LPARAM lParam, LRESULT* plResult);
		STDMETHOD(ReleaseDC)(HDC hDC);
		STDMETHOD(ScrollRect)(int dx, int dy, LPCRECT prectScroll,
			LPCRECT prectClip);
		STDMETHOD(SetCapture)(BOOL tCapture);
		STDMETHOD(SetFocus)(BOOL tFocus);
	END_INTERFACE_PART(OleInPlaceSiteWindowless);

	DECLARE_INTERFACE_MAP()

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	friend class CCtrlObj;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXDEVITEM_H__972EC672_E6D3_11D4_8C60_5254AB20ED22__INCLUDED_)
