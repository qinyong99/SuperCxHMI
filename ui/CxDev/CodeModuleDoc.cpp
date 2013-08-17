// CodeModuleDoc.cpp: implementation of the CCodeModuleDoc class.
//功能: 代码模块文档
//制作: 孙越洋
//所属工程: CxDev
//时间: 2002-3=26
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CCodeModuleDoc::CCodeSubDoc::CCodeSubDoc()
{
}


CDocument* CCodeModuleDoc::CCodeSubDoc::LoadChildDocumentFromFile(LPTSTR fileName)
{
	return FALSE;
}

//////////////////////////////////////////////////////////
//功能:创建代码模块文档
//////////////////////////////////////////////////////////
CDocument* CCodeModuleDoc::CCodeSubDoc::CreateChildDocument(CDevDoc* pDoc)
{
	ASSERT(m_pParentDoc);
	if (m_pChildDoc != NULL)
		return m_pChildDoc;

	CCodeModuleDoc* pThisDoc = new CCodeModuleDoc(pDoc);

	pThisDoc->m_pParentDoc = pDoc;
	pThisDoc->m_pMainDocRecord = this;
	pThisDoc->m_pFormSubDoc = this;
	m_strName.Format("codeDoc&d", m_dwDocumentNumber - 1);

	if (!pThisDoc->OnNewDocument())
	{
		delete pThisDoc;
		return NULL;
	}

	m_pChildDoc = pThisDoc;
	CMDIChildWnd* pNewFrame
			= (CMDIChildWnd*)(theApp.m_pCodeModuleTmpl->CreateNewFrame(pThisDoc, NULL));
	ASSERT(pNewFrame != NULL);
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pNewFrame->InitialUpdateFrame(pThisDoc, TRUE);

	return pThisDoc;
}

///////////////////////////////////////////////////////////////
//功能:装入代码模块文档
///////////////////////////////////////////////////////////////
CDocument* CCodeModuleDoc::CCodeSubDoc::LoadChildDocument(BOOL bInitFrame)
{
	ASSERT(bInitFrame == TRUE);

	if (m_pChildDoc != NULL)
		return m_pChildDoc;

	CCodeModuleDoc* pCodeDoc = new CCodeModuleDoc(m_pParentDoc);
	pCodeDoc->m_pMainDocRecord = this;
	m_pChildDoc = pCodeDoc;

	if (!pCodeDoc->OnNewDocument())
	{
		delete pCodeDoc;
		return NULL;
	}

	CMDIChildWnd* pNewFrame
			= (CMDIChildWnd*)(theApp.m_pCodeModuleTmpl->CreateNewFrame(pCodeDoc, NULL));
	ASSERT(pNewFrame != NULL);
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pCodeDoc->ReadFromParentDocument();
	pCodeDoc->SetModifiedFlag(FALSE);

	pNewFrame->InitialUpdateFrame(pCodeDoc, TRUE);

	return pCodeDoc;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCodeModuleDoc::CCodeModuleDoc(CDevDoc* pDoc) :
	CFormDoc(pDoc)
{
	m_pParentDoc = pDoc;
}

CCodeModuleDoc::~CCodeModuleDoc()
{

}

IMPLEMENT_DYNCREATE(CCodeModuleDoc, CFormDoc)


BOOL CCodeModuleDoc::OnNewDocument()
{
	if (m_pDocument != NULL)
	{
		if(m_lpRootStg == NULL)
			m_lpRootStg = m_pDocument->OpenSubDocumentStorage(m_pMainDocRecord);
	}
	else
	{
		if (!CDevObjDoc::OnNewDocument())
			return FALSE;
	}

	return TRUE;
//	return Initialize();
}

BEGIN_MESSAGE_MAP(CCodeModuleDoc, CFormDoc)
	//{{AFX_MSG_MAP(CCodeModuleDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeModuleDoc diagnostics

#ifdef _DEBUG
void CCodeModuleDoc::AssertValid() const
{
	CFormDoc::AssertValid();
}

void CCodeModuleDoc::Dump(CDumpContext& dc) const
{
	CFormDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCodeModuleDoc commands
////////////////////////////////////////////
//////////////////////////////////////////////////////////
//功能:击活该子文档时由Frame调用
//////////////////////////////////////////////////////////
void CCodeModuleDoc::OnActivate()
{
//	if(m_pParentDoc != NULL && m_pMainDocRecord != NULL)
//	{
//		m_pParentDoc->m_dwActivateSubDocID = m_pMainDocRecord->m_dwDocumentNumber;
//		theApp.NotifyDocumentActivate((DWORD)m_pParentDoc, (LPARAM)(m_pParentDoc));
//	}
}

void CCodeModuleDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(IsModified() && m_pMainDocRecord->m_bNeedSave)
		SaveToParentDocument();
	m_pMainDocRecord->m_pChildDoc = NULL;
	m_pMainDocRecord = NULL;

	CDevObjDoc::OnCloseDocument();	
}

void CCodeModuleDoc::OnFileSave() 
{
//	if (m_pDocument != NULL)
//		m_pDocument->SaveSubDocument(m_pFormSubDoc);
	
//	if (m_pMainDocRecord->m_pParentDoc != NULL)
//	{
//		CString strName = m_pMainDocRecord->m_pParentDoc->GetPathName();
//		BOOL flag;
//		if (strName == "")
//		{
//			flag = FALSE;
//		}
//		else
//			flag = TRUE;
//		m_pMainDocRecord->m_pParentDoc->SaveContentOnly();
//		m_pMainDocRecord->m_pParentDoc->m_bSameAsLoad = flag;
//	}
//	SaveToParentDocument();
//	m_pMainDocRecord->m_pParentDoc->SaveContentOnly();	
}
