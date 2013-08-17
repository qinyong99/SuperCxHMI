// ModuleDoc.h : header file

#if !defined(AFX_MODULEDOC_H__DE1B9024_F833_4538_B948_2CF2EAE78FDE__INCLUDED_)
#define AFX_MODULEDOC_H__DE1B9024_F833_4538_B948_2CF2EAE78FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CModuleDoc document
class CModuleDoc : public COleDocument
{
public:
	CModuleDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CModuleDoc)

	class CModule : public CObject
	{
	public:
		CModule() 
		{
			m_pDocument = NULL;
		}
		CModule(CModuleDoc* pDoc, CDevDoc::SUBDOCTYPE type, LPCSTR szName)
		{
			ASSERT(pDoc != NULL);
			m_pDocument = pDoc;
			m_dwModuleNumber = m_pDocument->GetNextModuleNumber();
			m_type = type;
			m_strName = szName;
		}

		virtual void Serialize(CArchive& ar)
		{
			if (ar.IsStoring())
			{
				int nType = int(m_type);
				ar << nType;
				ar << m_strName;
				ar << m_rcPosition;
				ar << m_dwModuleNumber;
			}
			else
			{
				int nType;
				ar >> nType;
				m_type = CDevDoc::SUBDOCTYPE(nType);
				ar >> m_strName;
				ar >> m_rcPosition;
				ar >> m_dwModuleNumber;

				m_pDocument = (CModuleDoc *)ar.m_pDocument;
				if (m_dwModuleNumber >= m_pDocument->m_dwNextModuleNumber)
					m_pDocument->m_dwNextModuleNumber = m_dwModuleNumber + 1;
			}
		}

		void GetModuleName(LPTSTR szModuleName) const
		{
			ASSERT(szModuleName != NULL);
			wsprintf(szModuleName, _T("CModule %lu"), m_dwModuleNumber);
		}

		CDevDoc::SUBDOCTYPE m_type; //模块类型
		CString m_strName; //模块名称
		CRectF m_rcPosition;
		DWORD m_dwModuleNumber;
		CModuleDoc* m_pDocument;
	};

// Attributes
public:
	CPtrArray m_arrModule;
	DWORD m_dwNextModuleNumber;

	CImageList m_listImage;
	CImageList m_listImageLarge;

	DWORD GetNextModuleNumber() { return m_dwNextModuleNumber++; } 

// Operations
public:
	IStorage* CreateModuleStorage(CModule* pModule);

	void CreateUniqueName(LPCTSTR pszBaseName, CString& strUniqueName);
	void CreateModuleBitmap(CModuleDoc::CModule* pModule);
	
	void SetModuleName(LPCSTR szName, LPCSTR szNewName);
	CModule* AddFormModule(CFormDoc* pDoc);
	BOOL DeleteModule(LPCSTR szName);

	int GetModuleCount();
	CModule* GetModule(int iIndex);
	CModule* GetModule(LPCSTR szName);
	
	CDevDoc::CSubDoc* GetSubDocument(LPCSTR szName); 
	BOOL DoPaste(COleDataObject* pDataObject);

	static BOOL DeleteLib(CString strLibName);
	static CModuleDoc* NewLib(CString strName);
	static void RenameLib(LPCTSTR lpszOldName, LPCTSTR lpszNewName);

	BOOL SaveLib();
	
	static CString GetDefaultPath();
	static const TCHAR m_szModuleLibFileExt[];
	
	BOOL CloseLib();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL OpenLib(CString strLibName);

	virtual ~CModuleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace);
	//{{AFX_MSG(CModuleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULEDOC_H__DE1B9024_F833_4538_B948_2CF2EAE78FDE__INCLUDED_)
