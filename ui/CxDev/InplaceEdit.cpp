// InplaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "cxdev.h"
#include "InplaceEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInplaceEdit

CInplaceEdit::CInplaceEdit()
{
	m_bCacel = FALSE;
}

CInplaceEdit::~CInplaceEdit()
{
}

BEGIN_MESSAGE_MAP(CInplaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInplaceEdit)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInplaceEdit message handlers

void CInplaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_TAB)
	{
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);
		return;
	}
	if ((nChar == VK_RETURN) || (nChar == VK_TAB))
	{
		GetParent()->SetFocus();
	}
	else if (nChar == VK_ESCAPE)
	{	
		m_bCacel = TRUE;
		GetParent()->SetFocus();
	}	
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInplaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	GetParent()->SendMessage(WM_INPLACEEDIT_KILLFOCUS, 0, 0L);
}

UINT CInplaceEdit::OnGetDlgCode() 
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}
