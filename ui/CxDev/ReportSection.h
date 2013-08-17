#if !defined(AFX_REPORTSECTION_H__E1CB9F6C_6004_44A9_A101_18ADCB748ACC__INCLUDED_)
#define AFX_REPORTSECTION_H__E1CB9F6C_6004_44A9_A101_18ADCB748ACC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportSection.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReportSection command target

class CReportDoc;

class CReportSection : public CCmdTarget
{
	DECLARE_SERIAL(CReportSection)

	CReportSection();
	CReportSection(CReportDoc* pDoc, LPCTSTR lpszName, LPCTSTR lpszCaption, float fHeight);           // protected constructor used by dynamic creation

// Attributes
public:
	CString m_strName;
	CString m_strCaption;
	float m_fHeight;
	BOOL m_bVisible;
	BOOL m_bKeepTogether;
	enumForcePageBreak m_enumForcePageBreak;

	CReportDoc* m_pDocument;
// Operations
public:
	LPUNKNOWN GetObjectUnknown();

	void Draw(CDC* pDC, LPRECT pRect);
	VOID EmbossedTextOut(CDC* pDC, int x, int y, LPCTSTR lpsz, UINT cb, 
			COLORREF crText, COLORREF crShadow, int cx, int cy, LPCRECT pClipRect);
	
	CArray<LINKINFO, LINKINFO&> m_arLinkInfo;
	
	HRESULT HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected);
	HRESULT GetDynamic(DISPID dispid, IUnknown** ppUnknown);
	HRESULT RemoveDynamic(DISPID dispid);
	HRESULT AddDynamic(DISPID dispid, IUnknown* pUnknown);
	
	//∂ØÃ¨ Ù–‘ Ù–‘
	HRESULT LoadDynamicProperty(IStream *pStream);
	HRESULT SaveDynamicProperty(IStream *pStream);
	void SerializeDynamicProperty(CArchive& ar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportSection)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReportSection();

	virtual void Serialize(CArchive& ar);

	BOOL GetDispatchIID(IID* piid);

	DECLARE_OLETYPELIB(CReportSection)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGroupObj)
	afx_msg float GetHeight();
	afx_msg void SetHeight(float fNewValue);
	afx_msg void OnVisibleChanged();
	afx_msg void OnKeepTogetherChanged();
	afx_msg void OnForcePageBreakChanged();
	afx_msg BSTR GetName();
	afx_msg void SetName(LPCTSTR lNewValue);
	afx_msg VARIANT_BOOL IsConnected(LPCTSTR pszProertyName);
	afx_msg void ConnectObject(LPCTSTR lpszProertyName, LPUNKNOWN punkObject);
	afx_msg void ConnectDirect(LPCTSTR lpszProertyName, LPCTSTR pszDataSource);
	afx_msg LPUNKNOWN GetConnectObject(LPCTSTR lpszProertyName);
	afx_msg void Disconnect(LPCTSTR lpszProertyName);
	afx_msg int GetConnectState(DISPID dispid);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	BEGIN_INTERFACE_PART(CategorizeProperties, ICategorizeProperties)
		STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* ppropcat);
		STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName);
	END_INTERFACE_PART(CategorizeProperties)
		
	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CReportSection, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)
		
	DECLARE_INTERFACE_MAP()

	friend class CReportDoc;
	friend class CPropertyBar;
	friend class CActionName;
	friend class CActionSection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORTSECTION_H__E1CB9F6C_6004_44A9_A101_18ADCB748ACC__INCLUDED_)
