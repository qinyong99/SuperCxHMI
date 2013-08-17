// RunDoc.h : interface of the CFormDoc class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __FORMDOC_H__
#define __FORMDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HINT_UPDATE_WINDOW      0
const LPARAM HINT_UPDATE_DRAWOBJ = 1;

int GetShiftState();

class CFormDoc : public CDevObjDoc
{
	enum { IDR_NOTUSED = 0x7FFF };

protected: // create from serialization only
	CFormDoc(CDevDoc* pDoc = NULL);
	DECLARE_DYNCREATE(CFormDoc)

// Attributes
public:
	class CFormSubDoc : public CDevDoc::CSubDoc
	{
	public:
		CFormSubDoc();

	public:
		virtual CDevDoc::SUBDOCTYPE GetType() { return CDevDoc::enumFormDoc; }

		virtual CFormDoc* CreateFormDoc();
		
		virtual CDocument* LoadChildDocument();
		virtual void Serialize(CArchive& ar);

		virtual void UpdateDataSession();

	} *m_pFormSubDoc;

	CDevDoc* m_pDocument; //指向所在的CDevDoc文档
	CFormObj* m_pFormObj; //指向所对应的表单对象
	
	IStoragePtr m_pStorage;

	ITypeInfo* m_ptinfoInt;
	ITypeInfo* m_ptinfoCls;
	ITypeInfo* m_ptlSubObj;

	GUID m_libidSubObj;
	CLSID m_clsidSubObj;
	IID m_iidSubObj;

	Gdiplus::Image* m_pImageBackGround;

	//对象名称=》dispid
	CTypedPtrMap<CMapStringToPtr, CString, void *> m_mapNameToDispid;
	//对象自动化接口数组
//	CTypedPtrArray<CPtrArray, IDispatch*> m_apObjectDispatchs;

	IDispatch** m_ppObjects;

	CDevObjList m_dynamicobjects; //有动态属性枢要刷新的对象
	CDevObjList* GetDynamicObjects() { return &m_dynamicobjects; }
	CDevObjList m_timerobjects;
	CDevObjList* GetTimerObjects() { return &m_timerobjects; }

	CString m_strScript;

	int m_nLayer;
	unsigned long m_ulVisibleLayer;	//可见页面
	unsigned long m_ulLockLayer;

	CString m_strSecurityArea; //安全区

	IDispatch* m_pTagProvider; //标签提供者

	CFormScrEng* m_pScriptEngine;

	CPtrList m_listSubformModule;

	void UpdateCtrlItemsPosition();

	virtual CView* GetRunView() const;
	virtual CFormScrEng* GetScriptEngine();
	CString GetDefaultServer();

	HRESULT GetItemDispatch(IUnknown** ppDispatch);
	BOOL m_bParsedScript;

// Operations
public:
	BOOL IsMainForm();
	CFormDoc* GetMainForm();

	CString GetDisplayName();
	BOOL PutDisplayName(LPCSTR strDisplayName, BOOL bValidName = TRUE);

	CDevObj* GetMenuObject();

	virtual void SerializeProperty(CArchive& ar);
	
	void SaveProperty();   // 保存属性
	void RestoreProperty();   // 还原属性

	void ReadFromStorage(IStorage* pStorage);

	void ReadFromParentDocument();
	void UpdateDataSession();
	void ReadyToRun();
	void OnDynamicChange(CDevObj* pObj);

	CAmbientProperty* FindAmbientProperty(DISPID dispid);
	CAmbientProperty* FindAmbientProperty(LPCOLESTR pszName);

	HRESULT BuildTypeInfo();

	void ShowFrameWindow(BOOL bInitial, BOOL bPopup = TRUE);
	CFrameWnd* CreateNewFrame();

	// Firing events
	void AFX_CDECL FireEvent(DISPID dispid, BYTE* pbParams, ...);
	void FireEventV(DISPID dispid, BYTE* pbParams, va_list argList);
	
	virtual void PreCloseFrame(CFrameWnd* pFrame);
	virtual LPOLEITEMCONTAINER GetContainer();

	virtual void UpdateAllAssocViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

	CString GetAssocTagGroupEx(BOOL bIncludeThis);
	BOOL IsAssocTagGroupValid(LPCSTR lpszTagGroup);
	
	IDispatch* GetTagProvider();

	CFormDoc* GetTopFormDoc(); //因为子模块可以嵌套，此函数用于返回顶层模块文档

	void OnPositionChanged();
	void ReSyncFormPosition();

	BOOL IsAllowDirectDraw() { return m_bAllowDirectDraw; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

	void OnClose();
	virtual void OnFinalRelease();
	
// Implementation
public:
	virtual ~CFormDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CFontHolder* m_pFontHolder;
	FONTDESC m_descFont;

	CRectF m_position;
	enumMousePointer m_enumMousePointer;

	enumStartUpPosition m_enumStartUpPosition;
	enumBorderStyle m_enumBorderStyle;
	BOOL m_bControlBox;
	BOOL m_bAllowDirectDraw;

	BOOL m_bModalDone;

	void OnCloseForm();

	void Cleanup();
	HRESULT InitAmbientProperties();
	BOOL Initialize();
	BOOL GetDispatchIID(IID* piid);

	typedef struct tagLINKINFO
	{
		DISPID dispid;
		ICxDynamicObject* pDynamicObject;
	} LINKINFO;
	CArray<LINKINFO, LINKINFO&> m_arLinkInfo;

	HRESULT HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected);
	HRESULT GetDynamic(DISPID dispid, IUnknown** ppUnknown);
	HRESULT RemoveDynamic(DISPID dispid);
	HRESULT AddDynamic(DISPID dispid, IUnknown* pUnknown);

	//动态属性属性
	HRESULT LoadDynamicProperty(IStream *pStream);
	HRESULT SaveDynamicProperty(IStream *pStream);
	void SerializeDynamicProperty(CArchive& ar);
	void FakeParseDynamic();
	void ParseDynamic();

	void DrawDoc(CDC* pDC);
	void PrintDoc(CDC* pDC);

// Generated message map functions
protected:
	//{{AFX_MSG(CFormDoc)
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//自动化
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFormDoc)
	afx_msg void OnStartUpPositionChanged();
	afx_msg void OnBorderStyleChanged();
	afx_msg void OnControlBoxChanged();
	afx_msg void OnAllowDirectDrawChanged();
	UINT m_nScanInterval;
	afx_msg void OnScanIntervalChanged();
	afx_msg void OnSecurityAreaChanged();
	afx_msg float GetHorizontalPosition();
	afx_msg void SetHorizontalPosition(float fNewValue);
	afx_msg float GetVerticalPosition();
	afx_msg void SetVerticalPosition(float fNewValue);
	afx_msg float GetWidth();
	afx_msg void SetWidth(float fNewValue);
	afx_msg float GetHeight();
	afx_msg void SetHeight(float fNewValue);
	afx_msg void OnVisibleLayerChanged();
	afx_msg void Show(const VARIANT FAR& Modal);
	afx_msg void Hide();
	afx_msg void Close();
	afx_msg LPUNKNOWN GetHitObject(long lXPos, long lYPos);
	afx_msg LPUNKNOWN GetObjectByName(LPCTSTR lpszName);
	afx_msg LPDISPATCH GetObjectCollection();
	afx_msg OLE_HANDLE GetWindowHandle();
	afx_msg void ForceRefresh();
	afx_msg void BindDataLinksToTagGroup(LPCTSTR lpszGroup);
	OLE_COLOR m_clrBackColor;
	afx_msg void OnBackColorChanged();
	OLE_COLOR m_clrForeColor;
	afx_msg void OnForeColorChanged();
	CString m_strCaption;
	afx_msg void OnCaptionChanged();
	CString m_strDescription;
	afx_msg void OnDescriptionChanged();
	afx_msg void OnMousePointerChanged();
	CString m_strBrushImageFile;
	afx_msg void OnBrushImageFileChanged();
	CString m_strTagGroup;
	afx_msg void OnTagGroupChanged();
	afx_msg BSTR GetAssocTagGroup();
	LPFONTDISP GetFont();
	void SetFont(LPFONTDISP pFontDisp);
	afx_msg BSTR GetName();
	CString m_strMenu;
	afx_msg BSTR GetMenu();
	afx_msg void SetMenu(LPCTSTR lNewValue);
	afx_msg VARIANT_BOOL IsConnected(LPCTSTR pszProertyName);
	afx_msg void ConnectObject(LPCTSTR lpszProertyName, LPUNKNOWN punkObject);
	afx_msg void ConnectDirect(LPCTSTR lpszProertyName, LPCTSTR pszDataSource);
	afx_msg LPUNKNOWN GetConnectObject(LPCTSTR lpszProertyName);
	afx_msg void Disconnect(LPCTSTR lpszProertyName);
	afx_msg int GetConnectState(DISPID dispid);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	DECLARE_OLETYPELIB(CFormDoc)

	//事件
	BEGIN_CONNECTION_PART(CFormDoc, UserFormCP)
		CONNECTION_IID(__uuidof(IUserFormEvents))
	END_CONNECTION_PART(UserFormCP)

	DECLARE_CONNECTION_MAP()
	
	//扩展自动化接口
	BEGIN_INTERFACE_PART(DispatchExtend, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(DispatchExtend)

	BEGIN_INTERFACE_PART(OleContainer, IOleContainer)
		STDMETHOD(ParseDisplayName)(LPBINDCTX, LPOLESTR, ULONG*, LPMONIKER*);
		STDMETHOD(EnumObjects)(DWORD, LPENUMUNKNOWN*);
		STDMETHOD(LockContainer)(BOOL);
	END_INTERFACE_PART(OleContainer)

	BEGIN_INTERFACE_PART(ProvideMultipleClassInfo, IProvideMultipleClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
		STDMETHOD(GetMultiTypeInfoCount)(ULONG *pcti);
		STDMETHOD(GetInfoOfIndex)(ULONG iti, DWORD dwFlags, ITypeInfo** pptiCoClass, DWORD* pdwTIFlags, ULONG* pcdispidReserved, IID* piidPrimary, IID* piidSource);
	END_INTERFACE_PART_STATIC(ProvideMultipleClassInfo)

	BEGIN_INTERFACE_PART(ScriptEngineSite, ICxScriptEngineSite)
		STDMETHOD(EvaluateExpression)(BSTR bstrSource, VARIANT* pvaValidObjects);
		STDMETHOD(Execute)(BSTR bstrStatement);
		STDMETHOD(PreTreatExpression)(BSTR bstrSource, BSTR* pbstrSource);
		STDMETHOD(PreTreatExpressionEx)(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax);
	END_INTERFACE_PART(ScriptEngineSite)

	DECLARE_INTERFACE_MAP()

	friend class CDevDoc;
	friend class CFormDoc::CFormSubDoc;
	friend class CDevObj;
	friend class CGroupObj;
	friend class CDrawObj;
	friend class CCtrlItem;
	friend class CFormObj;
	friend class CRunView;
	friend class CPopupDlg;
	friend class CChildFrame;
	friend class CPopupFrame;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__FORMDOC_H__)
