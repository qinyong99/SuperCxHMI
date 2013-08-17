// FormDoc.h : interface of the CFormDoc class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __FORMDOC_H__
#define __FORMDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DevObjDoc.h"

class CScriptView;
class CFormObj;

// Hints for UpdateAllViews/OnUpdate
const LPARAM HINT_UPDATE_WINDOW = 0;
const LPARAM HINT_UPDATE_DRAWOBJ = 1;
const LPARAM HINT_UPDATE_SELECTION = 2;
const LPARAM HINT_DELETE_SELECTION = 3;
const LPARAM HINT_UPDATE_OLE_ITEMS = 4;
const LPARAM HINT_UPDATE_DOCSIZE = 5;
const LPARAM HINT_UPDATE_CAPTION = 6;
const LPARAM HINT_UPDATE_BORDER = 7;

class CFormDoc : public CDevObjDoc
{
public: // create from serialization only
	CFormDoc(CDevDoc* pDoc = NULL);
	void operator =(CFormDoc &source);
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
		virtual CMultiDocTemplate* GetLayoutTemplate() { return theApp.m_pLayoutTemplate; }

		virtual void OnParentDocTitleChanged();
		virtual void OnParentDocChanged();

		virtual CDocument* LoadChildDocumentFromFile(LPTSTR fileName);
		virtual CDocument* CreateChildDocument(CDevDoc* pDoc);
		virtual CDocument* LoadChildDocument(BOOL bInitFrame = TRUE);
		virtual void Serialize(CArchive& ar);
		
		virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage); 
		virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage); 

		virtual void SaveChildDocument(IStorage* pStorage);
		
		int m_nSubformModule;

	} *m_pFormSubDoc;

	CDevDoc* m_pDocument; //指向所在的CDevDoc文档
	CFormObj* m_pFormObj; //指向所对应的表单对象

	CString m_strScript;

	virtual CString GetClassName() { return _T("UserForm"); }

	int m_nLayer;
	unsigned long m_ulVisibleLayer;	//可见页面
	unsigned long m_ulLockLayer;

	CString m_strSecurityArea; //安全区

	Gdiplus::Image* m_pImageBackGround;

	void BringToFront(CDevObj* pObj);
	void SendToBack(CDevObj* pObj);
	POSITION GetTailPosition() const;
   	CDocItem* GetPrevItem( POSITION& pos ) const;
	
	CDevObj* GetObjectAt(const CPoint& point);
	void SortObjectByLayer(CDevObj* pItem);

	virtual void OnUpateTitle();

	void UpdateCtrlItemsPosition();

	// 脚本相关
	void UpdateObjectsEventStatus();

	BOOL GetObjectEventHandler(LPCTSTR lpszScript, CDevObj* pObj, LPCTSTR lpszEvent, 
		int& nSubBegin, int& nSubBodyBegin, int& nSubBodyEnd, int& nSubEnd);
	BOOL GetObjectEventHandler(LPCTSTR lpszScript, CDevObj* pObj, CStringArray& arrEventSubs);

	BOOL HasObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent = NULL);
	BOOL DeleteObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent);
	BOOL InsertOrReplaceObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent, LPCTSTR lpszParams, LPCTSTR lpszSubBody);
	BOOL GetObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent, CString& strSubBody);

	static CFormDoc* m_pExpCrtDoc;
	static void ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag = 0);

// Operations
public:
	BOOL IsMainForm();

	static CLIPFORMAT m_cfFormDocData;

	virtual CString GetDisplayName();
	BOOL PutDisplayName(LPCSTR strDisplayName, BOOL bValidName = TRUE);

	virtual BOOL IsValidObjectName(LPCTSTR lpszName);

	virtual void InitializeProperty();
	virtual void SerializeProperty(CArchive& ar);

	virtual void PreCloseFrame( CFrameWnd* pFrame );
	BOOL LoadFromFile(LPTSTR strFileName);

	virtual void AddObject(CDevObj* pObj);

	CDrawObj* AddDrawObj(REFCLSID clsid, LPCTSTR pszName,
		REFIID iidPersistanceMedium = IID_NULL, IUnknown* pPersistanceMedium =
		NULL);
	CCtrlObj* AddCtrlObj(REFCLSID clsid, LPCTSTR pszName, CRect rcObj,
		REFIID iidPersistanceMedium = IID_NULL, IUnknown* pPersistanceMedium =
		NULL);
	CGroupObj* AddGroupObj(LPCTSTR pszName, 
		REFIID iidPersistanceMedium = IID_NULL, IUnknown* pPersistanceMedium = NULL);

	CLayoutView* GetLayoutView() const;
	CScriptView* GetScriptView() const;

	virtual	LPUNKNOWN GetObjectUnknown();

	virtual BOOL DoFileSave();  
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace = TRUE);

	void SaveToParentDocument(CDevDoc* pParent= NULL);
	void ReadFromParentDocument();
	
	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);

	void WriteToStorage(IStorage* pStorage);
	void ReadFromStorage(IStorage* pStorage);

	void DisplayAsIcon(CDC* pDC, CRect* pRect);

	int AddAmbientProperty(DISPID dispid, LPCTSTR pszName, const VARIANT& var,
		int vti);
	CAmbientProperty* GetAmbientProperty(int iProp);
	int GetNumAmbientProperties() const;

	CArray<LINKINFO, LINKINFO&> m_arLinkInfo;

	HRESULT HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected);
	HRESULT GetDynamic(DISPID dispid, IUnknown** ppUnknown);
	HRESULT RemoveDynamic(DISPID dispid);
	HRESULT RemoveAllDynamic();
	HRESULT AddDynamic(DISPID dispid, IUnknown* pUnknown);

	BOOL HaveDynamic() { return m_arLinkInfo.GetSize() > 0; };

	//动态属性属性
	HRESULT LoadDynamicProperty(IStream *pStream);
	HRESULT SaveDynamicProperty(IStream *pStream);
	void SerializeDynamicProperty(CArchive& ar);

	virtual HRESULT MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat);
	virtual HRESULT GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName);

	HRESULT GetMenuDisplayStrings(CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut);
	HRESULT GetMenuPredefinedValue(DWORD dwCookie, VARIANT *pVarOut);

	virtual void DrawDoc(CDC* pDC);
	virtual void PrintDoc(CDC* pDC);

	BOOL GetDevObjMembers(CDevObj* pObj, CStringArray& arrWords, CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods);
	BOOL GetDevObjMemberParams(CDevObj* pObj, CStringArray& arrWords, CStringArray& arrParams);
	BOOL GetFormDocObjectsMembers(CStringArray& arrObjects, CStringArray& arrWords, CStringArray& arrProperties, CStringArray& arrMethods);
	
	virtual BOOL GetObjectMembers(CStringArray& arrWords, CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods, BOOL bThisModule = TRUE);
	virtual BOOL GetObjectMemberParams(CStringArray& arrWords, CStringArray& arrParams, BOOL bThisModule = TRUE);

	BOOL GetObjectMemberParams(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrParams);
	BOOL GetObjectMembers(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrProperties,CStringArray& arrMethods); 
	
	virtual void GetObjectNames(CStringArray& arrObjects);
	virtual void GetObjectEvents(LPCTSTR lpszObject, CStringArray& arrEvents);
	virtual void GetObjectEventParams(LPCTSTR lpszObject, LPCTSTR lpszEvent, CStringArray &params);

	void GetObjectEventOrParams(LPUNKNOWN pUnkObject, CStringArray& arrInfos, LPCTSTR lpszEvent = NULL);
	
	BOOL ShowHelpString(LPCTSTR lpszObject);

	virtual void OnSymbolChanged(DWORD dwUpdateMask);
	BOOL m_dwSymbolUpdateMask;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual void OnCloseDocument();
	virtual BOOL SaveModified();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	CUndoStack m_stackUndo;		//回退管理

	CPtrList m_listSubformModule;

	BOOL m_bSetScriptSavePoint;
	CFormDoc* GetTopFormDoc();

	void OnFormObjChange();
	virtual void SetModifiedFlag(BOOL bModified = TRUE);
	virtual void UpdateAllAssocViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

	virtual void SetDocSize(float fWidth, float fHeight, BOOL bObject = FALSE);

	CString GetAssocTagGroupEx(BOOL bIncludeThis);
	BOOL IsAssocTagGroupValid(LPCSTR lpszTagGroup);

	void OnActivate();
	HRESULT StreamSerialize(IStream* pStream);
	virtual ~CFormDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	DECLARE_DISPATCH_MAP()

protected:
	CFontHolder* m_pFontHolder;
	FONTDESC m_descFont;

	CRectF m_position;
	enumMousePointer m_enumMousePointer;

	enumStartUpPosition m_enumStartUpPosition;
	enumBorderStyle m_enumBorderStyle;
	BOOL m_bControlBox;
	BOOL m_bAllowDirectDraw;
	
// Generated message map functions
protected:
	void SaveToFile(CString fileName);
	void Cleanup();
	virtual HRESULT InitAmbientProperties();
	BOOL Initialize(BOOL bDefault = TRUE);
	BOOL GetDispatchIID(IID* piid);
	
	//{{AFX_MSG(CFormDoc)
	afx_msg void OnFileSave();
	afx_msg void OnFileExport();
	afx_msg void OnFileImport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	DECLARE_OLETYPELIB(CFormDoc)

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
	afx_msg void Show();
	afx_msg void Hide();
	afx_msg void Close();
	afx_msg LPUNKNOWN GetHitObject(long lXPos, long lYPos);
	afx_msg LPUNKNOWN GetObjectByName(LPCTSTR lpszName);
	afx_msg LPDISPATCH GetObjectCollection();
	OLE_COLOR m_clrBackColor;
	afx_msg void OnBackColorChanged();
	OLE_COLOR m_clrForeColor;
	afx_msg void OnForeColorChanged();
	CString m_strCaption;
	afx_msg void OnCaptionChanged();
	CString m_strDescription;
	afx_msg void OnDescriptionChanged();
	afx_msg void OnMousePointerChanged();
	afx_msg BSTR GetName();
	afx_msg void SetName(LPCTSTR lNewValue);
	CString m_strMenu;
	afx_msg BSTR GetMenu();
	afx_msg void SetMenu(LPCTSTR lNewValue);
	CString m_strBrushImageFile;
	afx_msg void OnBrushImageFileChanged();
	CString m_strTagGroup;
	afx_msg void OnTagGroupChanged();
	afx_msg BSTR GetAssocTagGroup();
	LPFONTDISP GetFont();
	void SetFont(LPFONTDISP pFontDisp);
	afx_msg VARIANT_BOOL IsConnected(LPCTSTR pszProertyName);
	afx_msg void ConnectObject(LPCTSTR lpszProertyName, LPUNKNOWN punkObject);
	afx_msg void ConnectDirect(LPCTSTR lpszProertyName, LPCTSTR pszDataSource);
	afx_msg LPUNKNOWN GetConnectObject(LPCTSTR lpszProertyName);
	afx_msg void Disconnect(LPCTSTR lpszProertyName);
	afx_msg int GetConnectState(DISPID dispid);
	//}}AFX_DISPATCH

	BEGIN_INTERFACE_PART(CategorizeProperties, ICategorizeProperties)
		STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* ppropcat);
		STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
	END_INTERFACE_PART(CategorizeProperties)

	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CFormDoc, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)

	HRESULT GetDisplayString(DISPID dispid, BSTR *pbstr);    
	HRESULT MapPropertyToPage(DISPID dispid, LPCLSID pClsID);    
	HRESULT GetPredefinedStrings(DISPID dispid        , CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut);    
	HRESULT GetPredefinedValue(DISPID dispid, DWORD dwCookie        , VARIANT *pVarOut);

	BEGIN_INTERFACE_PART(PerPropertyBrowsing, IPerPropertyBrowsing)
		STDMETHOD(GetDisplayString)(DISPID dispid, BSTR *pbstr);
		STDMETHOD(MapPropertyToPage)(DISPID dispid, LPCLSID pClsID);
		STDMETHOD(GetPredefinedStrings)(DISPID dispid, CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut);
		STDMETHOD(GetPredefinedValue)(DISPID dispid, DWORD dwCookie, VARIANT *pVarOut);
	END_INTERFACE_PART(PerPropertyBrowsing)

	BEGIN_INTERFACE_PART(SymbolTreeSite, ICxSymbolTreeSite)
		STDMETHOD(GetObjectMembers)(LPCWSTR lpszObject, 
			LPENUMSTRING* ppEnumObjects, LPENUMSTRING* ppEnumProperties, LPENUMSTRING* ppEnumMethods);
	END_INTERFACE_PART(SymbolTreeSite)

	DECLARE_INTERFACE_MAP()

	//回退操作类
	friend class CFormDoc::CFormSubDoc;
	friend class CDevDoc;
	friend class CDevObj;
	friend class CDrawObj;
	friend class CCtrlItem;
	friend class CFormObj;
	friend class CReportObj;
	friend class CLayoutFrame;
	friend class CLayoutView;
	friend class CScriptFrame;
	friend class CScriptView;
	friend class CMiddleWnd;
	friend class CModuleDoc;
	friend class CPropertyBar;
	friend class CActionName;
};

/////////////////////////////////////////////////////////////////////////////
extern const IID LIBID_CXEXTOBJLib;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __DEVDOC_H__
