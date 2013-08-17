#if !defined(AFX_DEVOBJDOC_H__E108B7B6_B71E_43DA_B67D_CEDAC04F019C__INCLUDED_)
#define AFX_DEVOBJDOC_H__E108B7B6_B71E_43DA_B67D_CEDAC04F019C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////
// DevObjDoc.h : header file
//用于管理CDrawObj对象的文档类其类
//工程:CxDev
//作者:孙越洋
//时间:2001.12.3
//说明:库文档和主及子窗文档对象的基类
///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDevObjDoc document

class CDevObjDoc : public COleDocument
{
protected:
	CDevObjDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDevObjDoc)
	CNamedDevobjMap* GetNamedObjectsMap() { return &m_mapNamedObjects; }

// Attributes
public:
	DWORD m_dwVer;

	UINT m_dwNextObjectNumber;

// Operations
public:
	int GetCtrlItemCount() { return m_docItemList.GetCount(); }

	int SetItemTabOrder(CCtrlItem* pItem, int nTabOrder);
	CCtrlItem* GetNextTabItem(CCtrlItem* pItem);

	CAmbientProperty* FindAmbientProperty(LPCOLESTR pszName);
	CAmbientProperty* FindAmbientProperty(DISPID dispid);

	virtual CString GetDisplayName() { return CString(""); }
	
	void CreateUniqueObjectName(CDevObj* pObj, LPCTSTR lpszBaseName = NULL);
	virtual BOOL IsValidObjectName(LPCTSTR lpszName);

	void RemoveObject(CDevObj* pObj);
	CDevObjList* GetObjects() { return &m_objects; }

	virtual void AddObject(CDevObj* pObj);

	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);

	virtual void UpdateAllAssocViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

	void UpdateAllViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

	virtual void OnSymbolChanged(DWORD dwUpdateMask){}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevObjDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CSymbolObj* AddSymbolObj(LPCTSTR pszName, 
		REFIID iidPersistanceMedium = IID_NULL, IUnknown* pPersistanceMedium = NULL);
	CDevObj* GetPrevObj(POSITION& pos);
	CDevObj* GetNextObj(POSITION& pos);
	POSITION GetFirstObjPosition();
	virtual ~CDevObjDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CTypedPtrArray<CObArray, CAmbientProperty*> m_apAmbientProperties;
	CDevObjList m_objects;
	CNamedDevobjMap m_mapNamedObjects;
	
	CMapStringToPtr* m_pMapClassNameIndex;

	//{{AFX_MSG(CDevObjDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CDevObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVOBJDOC_H__E108B7B6_B71E_43DA_B67D_CEDAC04F019C__INCLUDED_)
