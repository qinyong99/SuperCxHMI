
#ifndef __DEVDOC_H__
#define __DEVDOC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDevDoc document
class CRunCtrl;

#include "../../svr/CxSAC/CxSAC.h"
#include "../../svr/CxDatSes/CxDatSes.h"

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
		CString m_strName;
		CDevDoc* m_pParentDoc;
		CDocument* m_pChildDoc;
	public:
		virtual SUBDOCTYPE GetType() = 0;

		BOOL IsFirstSubDoc();

		virtual CDocument* LoadChildDocument();
		virtual void UpdateDataSession();
	}; 

	// 框架对象，可以用来保存运行控件变量
	CTypedPtrMap<CMapStringToPtr, CString, VARIANT*> m_mapFrameItems;

	CRunCtrl* m_pRunCtrl;; //用来显示的框架

	CPtrList m_listSubDoc;
	CFormDoc* m_pMainForm;

	DWORD m_dwVer;

	CMapStringToPtr m_mapNameToSubDocument;
	CMapWordToPtr m_mapNumberToSubDocument;

	ICxDataSession* m_pIDataSession;
	
	void UpdateDataSession();

// Operations
public:
	VARIANT GetFrame(LPCTSTR pszItem); 
	void SetFrame(LPCTSTR pszItem, const VARIANT FAR& newValue);

	CSubDoc* FindSubDocumentByName(LPCTSTR lpszName);
	CSubDoc* FindSubDocumentByNumber(DWORD dwNumber);

	void ResetAllRunView();
	void SetAllRunView(BOOL bTimerEnable, BOOL bZoomFitWindow, 
		float fZoomRatio, float fZoomX, float fZoomY, BOOL bAllowMouseZoom, BOOL bAllowScrollBars, BOOL bDragMode);

	CRunView* GetActiveRunView();

	void SetDefaultTitle();

	//子文档流存储处理
	CString GetStorageName(DWORD dwSubDocumentNuber) const;
	LPSTORAGE OpenSubDocumentStorage(CSubDoc* pSubDoc);

	static CDocument* ShowPicture(CRunCtrl* pRunCtrl, LPCTSTR lpszFileName);
	static CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	static void CloseAllDocuments(BOOL bEndSession);

	void RemoveAllSubDocument();

	virtual BOOL OpenCacheFile(LPCTSTR lpszPathName);

	BOOL CanReplacePicture();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDevDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDevDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DEVDOC_H__)
