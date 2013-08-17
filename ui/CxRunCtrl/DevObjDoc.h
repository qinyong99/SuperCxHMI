
#ifndef __DEVOBJDOC_H__
#define __DEVOBJDOC_H__

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
	virtual CView* GetRunView() const;
	virtual CFormScrEng* GetScriptEngine();

// Operations
public:
	int GetCtrlItemCount() { return m_docItemList.GetCount(); }
	int SetItemTabOrder(CCtrlItem* pItem, int nTabOrder);
	CCtrlItem* GetNextTabItem(CCtrlItem* pItem, BOOL bPrevious);
	CCtrlItem* GetDefaultItem();
	CCtrlItem* GetCancelItem();
	
	CAmbientProperty* FindAmbientProperty(LPCOLESTR pszName);
	CAmbientProperty* FindAmbientProperty(DISPID dispid);
	void CreateUniqueObjectName( CDevObj* pObj, LPCTSTR pszBaseName,
		CString& strUniqueName );
	void RemoveObject(CDevObj* pObj);
	CDevObjList* GetObjects();

	virtual void UpdateAllAssocViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

	void UpdateAllViews(CView* pSender, LPARAM lHint = 0L,
		CObject* pHint = NULL);

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
	CDevObj* GetPrevObj(POSITION& pos);
	CDevObj* GetNextObj(POSITION& pos);
	POSITION GetFirstObjPosition();

	virtual void AddObject(CDevObj* pObj);

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

	BOOL m_bObjectsIsDeleting;

	//{{AFX_MSG(CDevObjDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CDevObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DEVOBJDOC_H__)
