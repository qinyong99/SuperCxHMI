#if !defined(AFX_RUNCTRL_H__44914769_0AFB_4088_B507_3D4059785A65__INCLUDED_)
#define AFX_RUNCTRL_H__44914769_0AFB_4088_B507_3D4059785A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef L_IMPL_OBJECTSAFETY
#include <objsafe.h>
#endif// L_IMPL_OBJECTSAFETY

// RunCtrl.h : Declaration of the CRunCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CRunCtrl : See RunCtrl.cpp for implementation.

class CDevDoc;

class CRunCtrl : public CActiveXDocControl
{
	DECLARE_DYNCREATE(CRunCtrl)

// Constructor
public:
	CRunCtrl();

	static DWORD m_dwInstanceCount;

	BOOL m_bPrintPreview;

	void PrintDirect(CDocument* pDocument, CRuntimeClass* pViewClass);
	void PrintPreview(CRunView* pPrintView, CDocument* pDocument = NULL, CRuntimeClass* pViewClass = RUNTIME_CLASS(CPrintView));

	CString* m_pPictureString;
	CDevDoc* m_pPictureDoc;

	CBitmap m_bitmap;

	UINT m_nTimer;

	// Drawing function
	virtual void OnDraw(
		CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);

	// Persistence
	virtual void DoPropExchange(CPropExchange* pPX);

	virtual HRESULT OnActivateInPlace(BOOL bUIActivate, LPMSG pMsg);

	virtual BOOL OnSetObjectRects(LPCRECT lprcPosRect, LPCRECT lprcClipRect);

	CString GetCurrentPicturePath();
	CString GetQualityPictureName(LPCTSTR strDoc);

	void ClosePicture();

	int GetNeareastGeneralZoomRatio();

	virtual BOOL IsInvokeAllowed(DISPID dispid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CRunCtrl();

	DECLARE_OLECREATE_EX(CRunCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CRunCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CRunCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CRunCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CRunCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	LRESULT OnReplacePicture(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

#ifdef L_IMPL_OBJECTSAFETY
	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
		STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD __RPC_FAR *pdwSupportedOptions, DWORD __RPC_FAR *pdwEnabledOptions);
		STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	END_INTERFACE_PART(ObjectSafety)
		
	DECLARE_INTERFACE_MAP();
#endif // L_IMPL_OBJECTSAFETY

	BOOL m_bBusying;
	CString m_strCurrentPath;
	CString m_strOpenParameter;

	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput m_gdiStartup;

	void OnShowPicture();

	CStringList m_listGoPictures;
	POSITION m_posGo;
	BOOL m_bGo;

	void UpdateAllRunViewPara();

public:
	
// Dispatch maps
	//{{AFX_DISPATCH(CRunCtrl)
	CString m_server;
	afx_msg void OnServerChanged();
	long m_lPort;
	afx_msg void OnPortChanged();
	CString m_strQueryString;
	afx_msg void OnQueryStringChanged();
	CString m_strDefaultUser;
	afx_msg void OnDefaultUserChanged();
	CString m_strDefaultPassword;
	afx_msg void OnDefaultPasswordChanged();
	BOOL m_bCreateServerObjectAsLocal;
	afx_msg void OnCreateServerObjectAsLocalChanged();
	BOOL m_bLocalServerUseSACProxy;
	afx_msg void OnLocalServerUseSACProxyChanged();
	CString m_picture;
	afx_msg BSTR GetPicture();
	afx_msg void SetPicture(LPCTSTR lpszNewValue);
	afx_msg BSTR GetCurrentVersion();
	BOOL m_bRefreshEnabled;
	afx_msg void OnRefreshEnabledChanged();
	BOOL m_bZoomFitWindow;
	afx_msg void OnZoomFitWindowChanged();
	float m_fZoomRatio;
	afx_msg float GetZoomRatio();
	afx_msg void SetZoomRatio(float fNewValue);
	float m_fZoomX;
	afx_msg float GetZoomX();
	afx_msg void SetZoomX(float fNewValue);
	float m_fZoomY;
	afx_msg float GetZoomY();
	afx_msg void SetZoomY(float fNewValue);
	BOOL m_bAllowMouseZoom;
	afx_msg void OnAllowMouseZoomChanged();
	BOOL m_bAllowScrollBars;
	afx_msg void OnAllowScrollBarsChanged();
	BOOL m_bDragMode;
	afx_msg void OnDragModeChanged();
	afx_msg OLE_HANDLE GetWindowHandle();
	afx_msg BSTR GetCurrentPath();
	afx_msg void SetCurrentPath(LPCTSTR lpszNewValue);
	afx_msg LPDISPATCH GetActiveWindow();
	afx_msg void ClearPicturesCache();
	afx_msg void GoBack();
	afx_msg void GoForward();
	afx_msg BOOL CanBack();
	afx_msg BOOL CanForward();
	afx_msg VARIANT GetApplication(LPCTSTR Item);
	afx_msg void SetApplication(LPCTSTR Item, const VARIANT FAR& newValue);
	afx_msg VARIANT GetFrame(LPCTSTR Item);
	afx_msg void SetFrame(LPCTSTR Item, const VARIANT FAR& newValue);
	afx_msg void SetHighlightMouseObject(boolean bHighlight);
	afx_msg LPDISPATCH GetMainMenu();
	afx_msg LPDISPATCH GetMainForm();
	afx_msg void ZoomIn();
	afx_msg void ZoomOut();
	afx_msg BOOL CanZoomIn();
	afx_msg BOOL CanZoomOut();
	afx_msg void ReloadPicture();
	afx_msg BOOL VerifySoftLock(boolean bAllowDemoStatus);
	afx_msg LPDISPATCH GetInternalObject();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

public:
	
	// Stock events
	void KeyDown(USHORT* pnChar);
	void KeyUp(USHORT* pnChar);
	void MouseMove(UINT /*nFlags*/, CPoint point);
	void ButtonDown(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonUp(USHORT iButton, UINT nFlags, CPoint point);
	void ButtonDblClk(USHORT iButton, UINT nFlags, CPoint point);

	
// Event maps
	//{{AFX_EVENT(CRunCtrl)
	void FirePictrueSize(long lWidth, long lHeight)
		{FireEvent(eventidPictrueSize,EVENT_PARAM(VTS_I4  VTS_I4), lWidth, lHeight);}
	void FireOnQuit()
		{FireEvent(eventidOnQuit,EVENT_PARAM(VTS_NONE));}
	void FireBeforeShowPicture(LPCTSTR lpszPicture, BOOL FAR* pbCancel)
		{FireEvent(eventidBeforeShowPicture,EVENT_PARAM(VTS_BSTR  VTS_PBOOL), lpszPicture, pbCancel);}
	void FireShowPictureComplete(LPCTSTR lpszPicture)
		{FireEvent(eventidShowPictureComplete,EVENT_PARAM(VTS_BSTR), lpszPicture);}
	void FireContainerContext(const VARIANT FAR Tag)
		{FireEvent(eventidContainerContext,EVENT_PARAM(VTS_VARIANT), &Tag);}
	void FireFrameItemChanged(LPCTSTR lpszItem)
		{FireEvent(eventidFrameItemChanged,EVENT_PARAM(VTS_BSTR), lpszItem);}
	void FirePictureClose(LPCTSTR lpszPicture)
		{FireEvent(eventidPictureClose,EVENT_PARAM(VTS_BSTR), lpszPicture);}
	void FireDemoTimeOut()
		{FireEvent(eventidDemoTimeOut,EVENT_PARAM(VTS_NONE));}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()
			
// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CRunCtrl)
	dispidPicture = 1L,
	dispidRefreshEnabled = 2L,
	dispidZoomFitWindow = 3L,
	dispidZoomRatio = 4L,
	dispidAllowMouseZoom = 5L,
	dispidAllowScrollBars = 6L,
	dispidDragMode = 7L,
	dispidWindowHandle = 8L,
	dispidCurrentPath = 9L,
	dispidServer = 10L,
	dispidPort = 11L,
	dispidQueryString = 12L,
	dispidDefaultUser = 13L,
	dispidDefaultPassword = 14L,
	dispidCurrentVersion = 101L,
	dispidZoomX = 102L,
	dispidZoomY = 103L,
	dispidGetActiveWindow = 17L,
	dispidClearPicturesCache = 18L,
	dispidGoBack = 19L,
	dispidGoForward = 20,
	dispidCanBack = 21,
	dispidCanForward = 22L,
	dispidSetHighlightMouseObject = 23L,
	eventidPictrueSize = 1L,
	eventidOnQuit = 2L,
	eventidBeforeShowPicture = 3L,
	eventidShowPictureComplete = 4L,
	eventidContainerContext = 5L,
	eventidFrameItemChanged = 6L,
	eventidPictureClose = 7L,
	eventidDemoTimeOut = 8L,
	//}}AFX_DISP_ID
	};

	friend class CRunView;
	friend class CFormDoc;
	friend class CFormScrEng;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNCTRL_H__44914769_0AFB_4088_B507_3D4059785A65__INCLUDED)
