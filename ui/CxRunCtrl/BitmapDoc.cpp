// BitmapDoc.cpp : implementation of the CBitmapDoc class
//

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBitmapDoc

IMPLEMENT_DYNCREATE(CBitmapDoc, CDocument)

BEGIN_MESSAGE_MAP(CBitmapDoc, CDocument)
	//{{AFX_MSG_MAP(CBitmapDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapDoc construction/destruction

CBitmapDoc::CBitmapDoc() :
	m_rcMargin(900, 720, 900, 720),
	m_fHorizontalScale(0.0f),
	m_fVerticalScale(0.0f),
	m_bMultipleScale(TRUE)
{
	
}

CBitmapDoc::~CBitmapDoc()
{

}

BOOL CBitmapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapDoc serialization

void CBitmapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBitmapDoc diagnostics

#ifdef _DEBUG
void CBitmapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBitmapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBitmapDoc commands
