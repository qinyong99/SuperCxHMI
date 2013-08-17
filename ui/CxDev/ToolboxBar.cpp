// ExplorerBar.cpp: implementation of the CToolboxBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolboxBar::CToolboxBar()
{
}

CToolboxBar::~CToolboxBar()
{
}

BEGIN_MESSAGE_MAP(CToolboxBar, CSizingControlBar)
    //{{AFX_MSG_MAP(CToolboxBar)
    ON_WM_CREATE()
    ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers
int CToolboxBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;

	/*CRect rect;
	rect.SetRectEmpty();
	if (!m_wndExplorerTree.Create(WS_VISIBLE | WS_CHILD/* | WS_BORDER*//*, 
			rect, this, AFX_IDW_PANE_FIRST))
		return -1;*/


	CRect rect(0, 0, lpCreateStruct->cx, lpCreateStruct->cy);

	if (!m_wndBoxCtrl.Create(NULL, NULL, 0, rect, this, 0, NULL))
		return -1;
	m_wndBoxCtrl.InitWindow();

    return 0;
}

CSize CToolboxBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) //docked 
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;
		CSize size;

        size = baseCSizingControlBar::CalcDynamicLayout(nLength, dwMode);
		if (dwMode & LM_VERTDOCK)
		{
			int nWidth;
			if (::IsWindow(m_wndBoxCtrl.m_hWnd))
			{
				m_wndBoxCtrl.CalcParentWndWidth(size.cx, nWidth);
				size.cx = nWidth;
			}
		}
		return size;
    }

    if (dwMode & LM_MRUWIDTH) return m_szFloat;
    if (dwMode & LM_COMMIT) return m_szFloat; // already committed

    //((dwMode & LM_LENGTHY) ? m_szFloat.cy : m_szFloat.cx) = nLength - 30;
	if (dwMode & LM_LENGTHY)
	{
		m_szFloat.cy = nLength;
	}
	else
	{	//ˮƽsizeing
		int nWidth;
		if (::IsWindow(m_wndBoxCtrl.m_hWnd))
			m_wndBoxCtrl.CalcParentWndWidth(nLength, nWidth);
		else
			nWidth = nLength;
		m_szFloat.cx = nWidth;
	}

    m_szFloat.cx = max(m_szFloat.cx, m_szMin.cx);
    m_szFloat.cy = max(m_szFloat.cy, m_szMin.cy);

    return m_szFloat;
}

void CToolboxBar::NcPaintGripper(CDC* pDC, CRect rcClient)
{
	// paints a simple "two raised lines" gripper
    // override this if you want a more sophisticated gripper
    CRect gripper = rcClient;
    BOOL bHorz = IsHorzDocked();
    
    gripper.DeflateRect(1, 1);
    if (bHorz)
    {   // gripper at left
        gripper.left -= m_cyGripper - 5;
        gripper.right = gripper.left + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper - 5;
        gripper.bottom = gripper.top + 3;
    }
    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));
}

UINT CToolboxBar::OnNcHitTest(CPoint point)
{
    if (IsFloating())
        return baseCSizingControlBar::OnNcHitTest(point);

    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);

    for (int i = 0; i < 4; i++)
        if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
            if (rcEdge.PtInRect(point)) return GetEdgeHTCode(i);
    return HTCLIENT;
}

void CToolboxBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    // compute the the client area
    CRect rcClient = lpncsp->rgrc[0];
    rcClient.DeflateRect(5, 5);

    m_dwSCBStyle &= ~SCBS_EDGEALL;

    switch(m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        rcClient.DeflateRect(m_cyGripper - m_cyGripper/2, 0, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        rcClient.DeflateRect(m_cyGripper - m_cyGripper/2, 0, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        rcClient.DeflateRect(0, m_cyGripper - m_cyGripper/2, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        rcClient.DeflateRect(0, m_cyGripper - m_cyGripper/2, 0, 0);
        break;
    default:
        break;
    }

    if (!IsFloating() && m_pDockBar != NULL)
    {
        CSCBArray arrSCBars;
        GetRowSizingBars(arrSCBars);

        for (int i = 0; i < arrSCBars.GetSize(); i++)
            if (arrSCBars[i] == this)
            {
                if (i > 0)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGELEFT : SCBS_EDGETOP;
                if (i < arrSCBars.GetSize() - 1)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
            }
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcClient.DeflateRect(
            (m_dwSCBStyle & SCBS_EDGELEFT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGETOP) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGERIGHT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_cxEdge : 0);

    lpncsp->rgrc[0] = rcClient;
}


void CToolboxBar::OnSize(UINT nType, int cx, int cy) 
{
    CSizingControlBar::OnSize(nType, cx, cy);
	if (::IsWindow(m_wndBoxCtrl.m_hWnd))
	{
		CRect rect;
		GetClientRect(rect);
		rect.DeflateRect(2, 2);
	
		// need to push non-client borders out of the client area
		::AdjustWindowRectEx(rect,
			m_wndBoxCtrl.GetStyle(), FALSE, WS_EX_CLIENTEDGE);
		m_wndBoxCtrl.SetWindowPos(NULL, rect.left, rect.top,
			rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CToolboxBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    CSizingControlBar::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

	if (m_wndBoxCtrl.GetSelFolder() == 0 && CDevTool::c_drawShape >= DRAW_SELECTION  && CDevTool::c_drawShape <= DRAW_ALMSUM)
	{
		if (m_wndBoxCtrl.GetSelFolder() == 0 && m_wndBoxCtrl.GetSelItem() != CDevTool::c_drawShape - DRAW_SELECTION)
			m_wndBoxCtrl.SetSelItem(CDevTool::c_drawShape - DRAW_SELECTION);
	}
	else
	{
		if (CDevTool::c_drawShape == DRAW_SELECTION  && m_wndBoxCtrl.GetSelItem() > 0)
			m_wndBoxCtrl.SetSelItem(0);

//		if (CDevTool::c_drawShape == DRAW_RECTITEM && 
//			m_wndBoxCtrl.GetSelFolder() == 0 &&
//			CRectItemTool::c_bInterior == TRUE &&
//			IsEqualCLSID(CRectItemTool::c_clsid, CLSID_CxRunCtrl))
//		{
//			m_wndBoxCtrl.SetSelItem(18);
//		}
	}
}

void CToolboxBar::OnDestroy()
{
	if (m_wndBoxCtrl.GetSafeHwnd())
	{
		m_wndBoxCtrl.SaveWindow();
	}
}

