#ifndef __DEVDOC_H__
#define __DEVDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevDoc document

class CFormDoc;
class CCodeModuleDoc;

BOOL IsFileExist(LPCTSTR szName);

class CDocHit
{
public:
	enum ChangeType
	{
		changeNew,
		changeRename,
		changeDelete,
		changeContent
	} m_changeType;

	CDocHit()
	{
	}
	CDocHit(ChangeType changeType, LPCSTR szNewName = NULL, LPCSTR szOldName = NULL)
	{
		m_changeType = changeType;
		m_strNewName = szNewName;
		m_strOldName = szOldName;
	}

	CString m_strNewName;
	CString m_strOldName;
};

class CDevDoc : public COleDocument
{
protected:
	CDevDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDevDoc)

// Attributes
public:
	static CPtrList c_listDevDoc;
	
	enum SUBDOCTYPE
	{
		enumFormDoc,
		enumReportDoc,
		enumCodeDoc,
	};

	class CSubDoc : public CObject
	{
	protected:
		CSubDoc();

	public:
		DWORD m_dwDocumentNumber;
		CString m_strName;		//内部名(与FormDoc中的m_strName相同)
		CDevDoc* m_pParentDoc;
		CDocument* m_pChildDoc;
		BOOL m_bNeedSave;

		IStoragePtr m_pStorage; //存储
	
	public:
		virtual SUBDOCTYPE GetType() = 0;

		virtual void OnParentDocTitleChanged() = 0;
		virtual void OnParentDocChanged() = 0;

		BOOL IsFirstSubDoc();
		virtual CDocument* LoadChildDocumentFromFile(LPTSTR fileName);
		BOOL CloseChildDoc();
		virtual CDocument* CreateChildDocument(CDevDoc* pDoc);
		virtual CDocument* LoadChildDocument(BOOL bInitFrame = TRUE);
		void GetDocumentName(LPTSTR lpszDocumenName) const; // gets readable Documen name

		virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage) { ASSERT(FALSE); }; 
		virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage) { return FALSE; }; 
	}; 

	LPSTORAGE m_lpOrigStg;
		
	CPtrList m_listSubDoc;
	BOOL m_bNeedSave;
	DWORD m_dwNextDocumentNumber;  //作为子文档唯一的号码

	int m_nSubformModule;

	DWORD m_dwVer;
	
	static CLIPFORMAT m_cfDocData;

	static UINT m_nUntitledCount;

// Operations
public:
	BOOL IsUniqueSubDocumentName(LPCTSTR szName);
	void CreateUniqueSubDocumentName(LPCTSTR pszBaseName, CString& strUniqueName);

	void SetDefaultTitle();
	CString GetDisplayName();

	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace = TRUE);
	inline BOOL IsSameAsLoad() { return m_bSameAsLoad; }

	//子文档流存储处理
	CString GetStorageName(DWORD dwSubDocumentNuber) const;
	LPSTORAGE OpenSubDocumentStorage(CSubDoc* pSubDoc, LPSTORAGE lpRootStg = NULL);
	LPSTORAGE CreateSubDocumentStorage(CSubDoc* pSubDoc);

	CSubDoc* GetSubDocument(LPCTSTR szName);

	static CDevDoc* CreateNewDocument(BOOL bAddMainForm = TRUE);
	static BOOL SaveAllModified(BOOL bDoPrompt);
	static void ResetNeedSaveAll();
	static void CloseAllDocuments(BOOL bEndSession);
	static CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	static CDocument* FindDocument(LPCTSTR lpszPathName);
	static BOOL DeleteDocumentFile(LPCSTR lpszFileName);

	CSubDoc* AddSubDocument(SUBDOCTYPE doctype);
	
//	CFormDoc* FindFormDoc(LPCTSTR lpszFormName);
	void RemoveAllSubDocument();

	virtual void SaveToStorage(CObject* pObject = NULL);

	void OnSubDocumentChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CSubDoc* GetSubDocument(DWORD dwSubDocumentNuber);
	DWORD GetNewDocumentNumber();

	void GetSubDocumentData(DWORD dwSubDocumentNuber, LPSTGMEDIUM lpStgMedium);
	static void GetSubDocumentData(CDevDoc::CSubDoc** ppSubDoc, int nCount, LPSTGMEDIUM lpStgMedium);
	static void GetSubDocumentData(CDevDoc::CSubDoc* pSubDoc, LPSTGMEDIUM lpStgMedium);
	BOOL DoPasteSubDocument(COleDataObject* pDataObject);

	static CFormDoc* GetActivateSubDoc();
	static CDevDoc* GetActivateDoc();
	BOOL SaveSubDocument(CDevDoc::CSubDoc *pSubDoc);
	BOOL DeleteSubDocument(CDevDoc::CSubDoc* pSubDoc);
	BOOL CloseDocument();
	void ActivateSubDocument(CDevDoc::CSubDoc* pSubDoc);

	CDevDoc::CSubDoc* ImportForm(CString strFileName);


	virtual ~CDevDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevDoc)
	afx_msg void OnFileSaveAll();
	afx_msg void OnAddCode();
	afx_msg void OnAddForm();
	afx_msg void OnAddBlankReport();
	afx_msg void OnAddReportWizard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CDevApp;
	friend class CCodeModuleDoc;
	friend class CFormDoc;
	friend class CExplorerTree;
};

class CSubDocDataSource : public COleDataSource
{
public:
	CSubDocDataSource()
	{
	}

	void AddSubDocument(CDevDoc::CSubDoc* pSubDoc)
	{
		m_arrSubDoc.Add(pSubDoc);
	}
	///////////////////////////////////////////////////////////////
	//功能:后提交数据
	///////////////////////////////////////////////////////////////
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
	{
		if (lpFormatEtc->cfFormat != CDevDoc::m_cfDocData)
			return FALSE;

		CDevDoc::GetSubDocumentData((CDevDoc::CSubDoc **)m_arrSubDoc.GetData(), m_arrSubDoc.GetSize(), lpStgMedium);

		return TRUE;
	}

	CPtrArray m_arrSubDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DEVDOC_H__)
