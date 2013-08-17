// RichEditView20.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "RichEditView20.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRichEditView20

CRichEditView20::CRichEditView20()
{
}

CRichEditView20::~CRichEditView20()
{
}


BEGIN_MESSAGE_MAP(CRichEditView20, CRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditView20)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditView20 message handlers

BOOL CRichEditView20::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CRichEditCtrl::PreCreateWindow(cs);
}
