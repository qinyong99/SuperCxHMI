class CActiveXDocTemplate : public CSingleDocTemplate
{
    enum { IDR_NOTUSED = 0x7FFF };

    CWnd* m_pParentWnd;
    CFrameWnd* m_pFrameWnd;
    CString m_docFile;

public:
    CActiveXDocTemplate(CRuntimeClass* pDocClass,
        CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

    CFrameWnd* CreateDocViewFrame(CWnd* pParentWnd);
    void SaveDocumentFile();

    virtual CFrameWnd* CreateNewFrame(CDocument* pDoc,
        CFrameWnd* pOther);
    virtual CDocument* OpenDocumentFile(
        LPCTSTR lpszPathName, BOOL bVerifyExists = TRUE);

	friend class CActiveXDocControl;
};

/////////////////////////////////////////////////////////////////////////////

class CActiveXDocControl : public COleControl
{
 //   enum { WM_IDLEUPDATECMDUI = 0x0363 };
protected:
    static BOOL m_bDocInitialized;
    CActiveXDocTemplate* m_pDocTemplate;
    CFrameWnd* m_pFrameWnd;

    DECLARE_DYNAMIC(CActiveXDocControl)

protected:
    void AddDocTemplate(CActiveXDocTemplate* pDocTemplate);
    CDocTemplate* GetDocTemplate() { return m_pDocTemplate; }

    //{{AFX_MSG(CActiveXDocControl)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
//     afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnDestroy();
    //}}AFX_MSG
    //{{AFX_DISPATCH(CActiveXDocControl)
    //}}AFX_DISPATCH
    //{{AFX_EVENT(CActiveXDocControl)
    //}}AFX_EVENT

    DECLARE_MESSAGE_MAP()
    DECLARE_DISPATCH_MAP()
    DECLARE_EVENT_MAP()

public:
    CActiveXDocControl();
    virtual ~CActiveXDocControl();

	CFrameWnd* CreateFrame(CDocument* pDoc);

    enum {
    //{{AFX_DISP_ID(CActiveXDocControl)
    //}}AFX_DISP_ID
    };
};
