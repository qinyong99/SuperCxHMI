// LayoutView.cpp : implementation of the CLayoutView class
//

#include "stdafx.h"
#include "DevInc.h"

#include "BarListCtrl.h"
#include "SymbolLibWnd.h"
#include "SymbolLibDoc.h"
#include "ModuleWnd.h"

#include "DevDoc.h"
#include "ModuleDoc.h"
#include "LayerSettingDlg.h"

#include "ActionObj.h"
#include "TabOrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayoutView
static int NEAR rgiZoomFactor[] =
{
	50, 75, 100, 150, 200, 400
};

CLIPFORMAT CLayoutView::m_cfItemData = NULL;
CLIPFORMAT CLayoutView::m_cfDragInfo = NULL;
BOOL CLayoutView::m_bCanDropEffectCopy = FALSE;

IMPLEMENT_DYNCREATE(CLayoutView, CScrollView)

BEGIN_MESSAGE_MAP(CLayoutView, CScrollView)
	//{{AFX_MSG_MAP(CLayoutView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_CANCEL_EDIT, OnCancelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOMCUSTOM, OnViewZoomCustom)
	ON_COMMAND(ID_OBJECT_MOVEBACK, OnObjectMoveback)
	ON_COMMAND(ID_OBJECT_MOVEFORWARD, OnObjectMoveforward)
	ON_COMMAND(ID_SPACE_EVENHORIZ, OnSpaceEvenhoriz)
	ON_UPDATE_COMMAND_UI(ID_SPACE_EVENHORIZ, OnUpdateSpaceEvenhoriz)
	ON_COMMAND(ID_SPACE_EVENVERT, OnSpaceEvenvert)
	ON_UPDATE_COMMAND_UI(ID_SPACE_EVENVERT, OnUpdateSpaceEvenvert)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLONE, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectAll)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_GROUP_OBJECT, OnGroupObject)
	ON_COMMAND(ID_UNGROUP_OBJECT, OnUngroupObject)
	ON_COMMAND(ID_INSERT_BITMAP, OnInsertBitmap)
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_GROUP_OBJECT, OnUpdateGroupObject)
	ON_UPDATE_COMMAND_UI(ID_UNGROUP_OBJECT, OnUpdateUngroupObject)
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_COMMAND(ID_EDIT_CONVERTTOBMP, OnEditConvertToBmp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CONVERTTOBMP, OnUpdateEditConverttobmp)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_SCRIPT, OnEditScript)
	ON_COMMAND(ID_ADDTO_SYMBOL, OnAddToSymbolLib)
	ON_COMMAND(ID_ADDTO_MODULE, OnAddToModuleLib)
	ON_COMMAND(ID_LAYER_SETUP, OnLayerSetup)
	ON_COMMAND(ID_OBJECT_FORWARD_MOST, OnObjectForwardMost)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEFORWARD, OnUpdateObjectForwardMost)
	ON_COMMAND(ID_OBJECT_BACKWORD_MOST, OnObjectBackwordMost)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_MOVEBACK, OnUpdateObjectBackwordMost)
	ON_COMMAND(ID_ARRAGE_SAMEHIGHT, OnArrageSamehight)
	ON_UPDATE_COMMAND_UI(ID_ARRAGE_SAMEHIGHT, OnUpdateAllSelUpTwo)
	ON_COMMAND(ID_ARRAGE_SAMEWIDTH, OnArrageSamewidth)
	ON_COMMAND(ID_ARRAGE_BOTH, OnArrageBoth)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CLONE, OnEditClone)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_DESIGN_SYMBOL, OnDesignSymbol)
	ON_COMMAND(ID_CREATE_SYMBOL, OnCreateSymbol)
	ON_UPDATE_COMMAND_UI(ID_DESIGN_SYMBOL, OnUpdateDesignSymbol)
	ON_UPDATE_COMMAND_UI(ID_CREATE_SYMBOL, OnUpdateCreateSymbol)
	ON_UPDATE_COMMAND_UI(ID_ADDTO_SYMBOL, OnUpdateAddtoSymbol)
	ON_UPDATE_COMMAND_UI(ID_ADDTO_MODULE, OnUpdateAddtoModule)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_SUBFORM, OnEditSubform)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SUBFORM, OnUpdateEditSubform)
	ON_COMMAND(ID_ADD_SUBMODULE, OnAddSubmodule)
	ON_COMMAND(ID_CREATE_SUBFORM, OnCreateSubform)
	ON_UPDATE_COMMAND_UI(ID_CREATE_SUBFORM, OnUpdateCreateSubform)
	ON_COMMAND(ID_BREAK_SUBFORM, OnBreakSubform)
	ON_UPDATE_COMMAND_UI(ID_BREAK_SUBFORM, OnUpdateBreakSubform)
	ON_COMMAND(ID_BREAK_SYMBOL, OnBreakSymbol)
	ON_UPDATE_COMMAND_UI(ID_BREAK_SYMBOL, OnUpdateBreakSymbol)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_PAGE_SETUP, OnPageSetup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_FORWARD_MOST, OnUpdateObjectForwardMost)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_BACKWORD_MOST, OnUpdateObjectBackwordMost)
	ON_UPDATE_COMMAND_UI(ID_ARRAGE_SAMEWIDTH, OnUpdateAllSelUpTwo)
	ON_UPDATE_COMMAND_UI(ID_ARRAGE_BOTH, OnUpdateAllSelUpTwo)
	ON_COMMAND(ID_TAB_ORDER, OnTabOrder)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID (CXMSG_GRID_SETTING_CHANGED, OnGridSettingChanged)
	ON_COMMAND_RANGE(ID_TRACKER_RESIZE, ID_TRACKER_ROTATE, OnTracker)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TRACKER_RESIZE, ID_TRACKER_ROTATE, OnUpdateTracker)
	ON_COMMAND_RANGE(ID_ALIGN_BOTTOM, ID_ALIGN_TOP, OnAlign)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ALIGN_BOTTOM, ID_ALIGN_TOP, OnUpdateAlign)
	ON_COMMAND_RANGE(ID_HORIZ_CENTER, ID_VERT_CENTER, OnCenter)
	ON_UPDATE_COMMAND_UI_RANGE(ID_HORIZ_CENTER, ID_VERT_CENTER, OnUpdateCenter)
	ON_COMMAND_RANGE(ID_ROTATE_LEFT, ID_ROTATE_RIGHT, OnRotate)
	ON_COMMAND_RANGE(ID_MIRROR_HORIZ, ID_MIRROR_VERT, OnMirror)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ROTATE_LEFT, ID_MIRROR_VERT, OnUpdateRotateOrMirror)
	ON_COMMAND_RANGE(ID_VIEW_ZOOM50, ID_VIEW_ZOOM400, OnZoom)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_ZOOM50, ID_VIEW_ZOOM400, OnUpdateZoom)
	ON_COMMAND_RANGE(ID_LAYER_1, ID_LAYER_16, OnLayer)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LAYER_1, ID_LAYER_16, OnUpdateLayer)
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_MESSAGE(_GetThemeChangedMsg(), OnThemeChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayoutView construction/destruction

CLayoutView::CLayoutView() :
	m_pObjWithFocus(NULL),
	m_pFocusObj(NULL),
	m_hThemeDLL(NULL), m_hTheme(NULL), m_pfnDrawThemeBackground(NULL)
{
	m_bIsMainFormView = FALSE;
	m_bIsFormView = TRUE;

	m_bActive = FALSE;
	m_bDestoy = FALSE;
//'''''''''''''''''''''''''''''''''''''''''''''''''
//	GetClientRect(ghwndDrawSurf, &rcClientDS);         
//	x = rcClientDS.right - rcClientDS.left;         
//	x *= 2540;         
//	x /= GetDeviceCaps(hDCDrawSurf, LOGPIXELSX);         
//	lpmfp->xExt = x;                                // ie. in 0.01mm 
         

	// initialize zoom state (100% from size cache)

	//	pDC->SetViewportExt(m_zoomNum.cx * pDC->GetDeviceCaps(LOGPIXELSX), 
	//			m_zoomNum.cy * pDC->GetDeviceCaps(LOGPIXELSY));

	m_zoomNum = CSize(100, 100);
	m_zoomDenom = CSize(100, 100);

//	m_zoomNum = CSize(1, 1);
//	m_zoomDenom = CSize(1440, 1440);

	m_prevDropEffect = DROPEFFECT_NONE;
	m_bInDrag = FALSE;

	if (m_cfItemData == NULL)
		m_cfItemData = (CLIPFORMAT)::RegisterClipboardFormat(_T("CxDev ItemData"));
	if (m_cfDragInfo == NULL)
		m_cfDragInfo = (CLIPFORMAT)::RegisterClipboardFormat(_T("CxDev DragInfo"));

	m_nDragShapePointSize = 0;
	m_pDragShapePoints = NULL;
	m_pDragShapePointTypes = NULL;

	SetScrollSizes(MM_TEXT, CSize(0, 0));

	m_rectMargin = theApp.m_settingGen.m_rectPageMargin;
}

CLayoutView::~CLayoutView()
{
	m_nDragShapePointSize = 0;
	if (m_pDragShapePoints != NULL)
	{
		delete m_pDragShapePoints;
		m_pDragShapePoints = NULL;
	}
	if (m_pDragShapePointTypes != NULL)
	{
		delete m_pDragShapePointTypes;
		m_pDragShapePointTypes = NULL;
	}
}

BOOL CLayoutView::PreCreateWindow(CREATESTRUCT& cs)
{
	//因为是容器窗口，必须加入以下语句
	cs.style |= WS_CLIPCHILDREN;
	BOOL bRet = CScrollView::PreCreateWindow(cs);
	if (bRet != TRUE || m_bIsMainFormView)
		return bRet;

	if (m_bIsFormView)
	{
		cs.style = WS_CLIPCHILDREN | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION /*| WS_SYSMENU*/ | WS_CHILDWINDOW;

//		cs.style |= WS_OVERLAPPEDWINDOW | WS_CHILDWINDOW | WS_VISIBLE;
//		cs.style &= ~WS_THICKFRAME;
	}
	else
	{
		cs.style = WS_BORDER | WS_CHILD | WS_VISIBLE;
		cs.style &= ~WS_CAPTION;
	}

	cs.dwExStyle |= WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR |
		WS_EX_WINDOWEDGE;

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////
// CLayoutView drawing
void CLayoutView::OnDraw(CDC* pDC)
{
	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CDC dc;
	CDC* pDrawDC = pDC;
	CBitmap bitmap;
	CBitmap* pOldBitmap;

	// only paint the rect that needs repainting
	CRect client;
	pDC->GetClipBox(client);
	client.InflateRect(1, 1);
	CRect rect = client;
	DocToClient(rect);

	if (!pDC->IsPrinting())
	{
		// draw to offscreen bitmap for fast looking repaints
		if (dc.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()))
			{
				OnPrepareDC(&dc, NULL);
				pDrawDC = &dc;

				// offset origin more because bitmap is just piece of the whole drawing
			//	dc.OffsetWindowOrg(1, 1);
				dc.OffsetViewportOrg(-rect.left, -rect.top);
				pOldBitmap = dc.SelectObject(&bitmap);
				dc.SetBrushOrg(rect.left % 8, rect.top % 8);

				// might as well clip to the same rectangle
				dc.IntersectClipRect(client);
			}
		}
	}

	if (pDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;

		TextureBrush tBrush(pDoc->m_pImageBackGround);
		Graphics graphics(pDrawDC->GetSafeHdc());
		graphics.FillRectangle(&tBrush, 
				Rect(client.left, client.top, client.Width(), client.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(pDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDrawDC->FillRect(client, &backBrush);
	}

	if (theApp.m_settingGen.m_bGrid && !pDC->IsPrinting())
		DrawGrid(pDrawDC, &client);

	pDoc->DrawDoc(pDrawDC);

	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnDraw(this, pDrawDC);

	if (pDrawDC != pDC)
	{
		pDC->SetViewportOrg(0, 0);
		pDC->SetWindowOrg(0, 0);
		pDC->SetMapMode(MM_TEXT);
		dc.SetViewportOrg(0, 0);
		dc.SetWindowOrg(0, 0);
		dc.SetMapMode(MM_TEXT);
		pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
			&dc, 0, 0, SRCCOPY);
		dc.SelectObject(pOldBitmap);
	}

	if (!pDC->IsPrinting())
		DrawTracker(pDC);
}

void CLayoutView::UpdateWindow()
{
	CWnd::UpdateWindow();
	DrawTracker(NULL);
}

void CLayoutView::DrawTracker(CDC* pDC)
{
	if (m_bActive && !CSelectTool::c_bTrackingHandle)
	{
		CDC dc;
		dc.Attach(::GetDCEx(m_hWnd, NULL, DCX_CACHE));
		pDC = &dc;

		POSITION posObj = m_selection.GetHeadPosition();
		CDevObj* pObj;
		while (posObj != NULL)
		{
			pObj = m_selection.GetNext(posObj);
			pObj->DrawTracker(pDC, CDevTool::c_enumTrackerState, this);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：绘制网格
//////////////////////////////////////////////////////////////////////////
void CLayoutView::DrawGrid(CDC* pDC, CRect* pRect)
{
	CFormDoc* pDoc = GetDocument();

	CRect rect = pDoc->m_position.GetRECT();

	if (pDoc->m_pFormObj == NULL)
		rect.OffsetRect(-rect.TopLeft());

	COLORREF clrGridColor = theApp.m_settingGen.m_clrGrid;

	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 0, clrGridColor);
	CPen* pOldPen = pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.left, rect.bottom);

	rect.IntersectRect(&rect, pRect);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_GEOMETRIC, 1, clrGridColor);
	pDC->SelectObject(&penDot);

//	CSize szDistace(theApp.m_settingGen.m_nGridSpaceWidth, theApp.m_settingGen.m_nGridSpaceHeight);
//	pDC->DPtoLP(&szDistace);
	int nX = theApp.m_settingGen.m_nGridSpaceWidth;
	int nY = theApp.m_settingGen.m_nGridSpaceHeight;
	for (int x = rect.left / nX * nX; x < rect.right; x += nX)
	{
		for (int y = rect.top / nY * nY; y < rect.bottom; y += nY)
			pDC->SetPixel(CPoint(x, y), clrGridColor);
	}

	pDC->SelectObject(pOldPen);
}

void CLayoutView::AlignToGrid(CPoint& point)
{
	theApp.AlignToGrid(point);

//	if (theApp.m_settingGen.m_bAlignToGrid)
//	{
//		int nGrindWidth = theApp.m_settingGen.m_nGridSpaceWidth;
//		point.x = ROUND((point.x + nGrindWidth / 2) / nGrindWidth) * nGrindWidth;
//		int nGrindHeight = theApp.m_settingGen.m_nGridSpaceHeight;
//		point.y = ROUND((point.y + nGrindHeight / 2) / nGrindHeight) * nGrindHeight;
//	}

//	if (theApp.m_settingGen.m_bAlignToGrid)
//	{
//		CPoint ptOrigin = GetScrollPosition();
//		
//		int nGrindWidth = theApp.m_settingGen.m_nGridSpaceWidth;
//		point.x = ROUND((point.x + ptOrigin.x + nGrindWidth / 2) / nGrindWidth) * nGrindWidth - ptOrigin.x;
//		int nGrindHeight = theApp.m_settingGen.m_nGridSpaceHeight;
//		point.y = ROUND((point.y + ptOrigin.y + nGrindHeight / 2) / nGrindHeight) * nGrindHeight - ptOrigin.y;
//	}

}

void CLayoutView::GetGridSpace(CSize& size)
{
	if (theApp.m_settingGen.m_bAlignToGrid)
	{
		size.cx = theApp.m_settingGen.m_nGridSpaceWidth;
		size.cy = theApp.m_settingGen.m_nGridSpaceHeight;
	}
	else
	{
		size.cx = 1;
		size.cy = 1;
	}
}

BOOL CLayoutView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CLayoutView::OnInitialUpdate()
{
	CFormDoc* pDoc = (CFormDoc *)GetDocument();

	SetScrollInfo();

	if (!m_bIsMainFormView)
	{
		AfxSetWindowText(m_hWnd, pDoc->m_strCaption);
		OnNcPaint();

		if (pDoc->m_enumBorderStyle == BorderStyleNone)
			ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
		if (pDoc->m_enumBorderStyle == BorderStyleSizable)
			ModifyStyle(0, WS_THICKFRAME, SWP_FRAMECHANGED);
	}

	CScrollView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CLayoutView printing

BOOL CLayoutView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLayoutView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLayoutView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLayoutView::OnDestroy()
{
	m_bDestoy = TRUE;
	theApp.NotifySelectChanged();
	
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CScrollView::OnDestroy();

	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}

	if (m_hThemeDLL != NULL)
	{
		CloseThemeData();
		::FreeLibrary(m_hThemeDLL);
		m_hThemeDLL = NULL;
	}
}

BOOL CLayoutView::IsSelected(const CObject* pDocItem) const
{
	CDevObj* pObj = (CDevObj*)pDocItem;
	return m_selection.Find(pObj) != NULL;
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CLayoutView::OnCancelEdit()
{
	// Close any in-place active item on this view.
//	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	if (pActiveItem != NULL)
//	{
//		pActiveItem->Close();
//	}
//	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);

	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnCancel();
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CLayoutView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CScrollView::OnSetFocus(pOldWnd);
}

void CLayoutView::OnSize(UINT nType, int cx, int cy)
{
	if (m_bIsMainFormView)
		CScrollView::OnSize(nType, cx, cy);

	// 2006.10.24 注释掉下面语句，下面语句导致在打开含有控件的文档时调用文档的SetModifiedFlag(TRUE)
//	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	if (pActiveItem != NULL)
//		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// CLayoutView diagnostics

#ifdef _DEBUG
void CLayoutView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLayoutView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CFormDoc* CLayoutView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)));
	return (CFormDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLayoutView message handlers

void CLayoutView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDown(this, nFlags, point);
}

void CLayoutView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonUp(this, nFlags, point);
}

void CLayoutView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnMouseMove(this, nFlags, point);

	CPoint local = point;
	ClientToDoc(local);
	if (CDevTool::c_drawShape != DRAW_SELECTION)
		AlignToGrid(local);
	((CMainFrame *)(theApp.m_pMainWnd))->OnViewMousePosChange(local);
}

void CLayoutView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnRButtonDown(this, nFlags, point);
}

void CLayoutView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnLButtonDblClk(this, nFlags, point);
}

void CLayoutView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
	if (pTool != NULL)
		pTool->OnChar(this, nChar, nRepCnt, nFlags);

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CLayoutView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	// invalidate selections when active status changes
	if (m_bActive != bActivate)
	{
		if (bActivate)  // if becoming active update as if active
			m_bActive = bActivate;
		if (!m_selection.IsEmpty())
			OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_bActive = bActivate;
		if (bActivate) 
			theApp.NotifySelectChanged();
	}
}

BOOL CLayoutView::CanSelect(CDevObj* pObj)
{
	unsigned long ulLayer = 1;
	ulLayer = ulLayer << pObj->m_nLayer;

	//对象不在可见层或在锁定层
	return ((ulLayer & GetDocument()->m_ulVisibleLayer) != NULL &&
		(ulLayer & GetDocument()->m_ulLockLayer) == NULL);
}

//////////////////////////////////////////////////////////////////////////
//功 能：选择对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::Select(CDevObj* pObj, BOOL bAdd, BOOL bUpdate)
{
	CDevTool::SetTrackerState(CDevObj::general, FALSE);

	if ((m_selection.IsEmpty() && pObj == NULL) || (pObj != NULL && !CanSelect(pObj)))
	{
		UpdateFocusObject();
		return;
	}

	if (!bAdd)
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	if (bAdd)
	{
		POSITION pos = m_selection.Find(pObj);
		if (pos != NULL)
		{
			m_selection.RemoveAt(pos);
			UpdateFocusObject();
			return;
		}
	}

	if (pObj == NULL || IsSelected(pObj))
	{
		if (!bAdd)
			theApp.NotifySelectChanged();
		UpdateFocusObject();
		return;
	}

	m_selection.AddTail(pObj);
	InvalidObject(pObj);

	UpdateFocusObject();

	if (!bAdd)
	{
		if (bUpdate)
			UpdateWindow();
		theApp.NotifySelectChanged();
	}
}

void CLayoutView::MutiSelect(CDevObjList* pObjList, BOOL bAdd)
{
	POSITION pos;
	CDevObj* pObj;
	CDevObjList listObjects;

	if (bAdd)
	{
		pos = pObjList->GetHeadPosition();
		while (pos != NULL)
		{
			pObj = pObjList->GetNext(pos);
			if (CanSelect(pObj))
			{
				POSITION pos = m_selection.Find(pObj);
				if (pos != NULL)
				{
					InvalidObject(pObj);
					m_selection.RemoveAt(pos);
				}
				else
				{
					listObjects.AddTail(pObj);
				}
			}
		}

		pObjList = &listObjects;
	}
	else
	{
		OnUpdate(NULL, HINT_UPDATE_SELECTION, NULL);
		m_selection.RemoveAll();
	}

	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pObj = pObjList->GetNext(pos);
		if (bAdd || CanSelect(pObj))
		{
			m_selection.AddTail(pObj);
			InvalidObject(pObj);
		}
	}
	
	UpdateFocusObject();
	theApp.NotifySelectChanged();
}

void CLayoutView::UpdateFocusObject()
{
	if (!m_selection.IsEmpty())
		m_pFocusObj = m_selection.GetTail();
	else
		m_pFocusObj = NULL;

	if (!m_bIsMainFormView)
		GetParent()->SendMessage(WM_SELECT_CHANGED, m_pFocusObj == NULL, 0);
}

//////////////////////////////////////////////////////////////////////////
//功 能：选择指定矩形范围内饿得对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	CFormDoc* pDoc = GetDocument();
	CDevObjList listObjects;
	ClientToDoc(rect);

	CDevObjList* pObjList = pDoc->GetObjects();
	POSITION posObj = pObjList->GetHeadPosition();
	while (posObj != NULL)
	{
		CDevObj* pObj = (CDevObj *)pObjList->GetNext(posObj);
		CRect rcPos = pObj->GetPositionRect().GetRECT();
		if (rcPos.left >= rect.left && rcPos.top >= rect.top && 
			rcPos.right <= rect.right && rcPos.bottom <= rect.bottom)
		{
			listObjects.AddTail(pObj);
		}
	}
	
	MutiSelect(&listObjects, bAdd);
}

//////////////////////////////////////////////////////////////////////////
//功 能：视图工更新
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc);

	switch (lHint)
	{
	case HINT_UPDATE_WINDOW:    // redraw entire window
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
//		Invalidate(FALSE);
		break;
	case HINT_UPDATE_DOCSIZE:
		SetScrollInfo();
		Invalidate(FALSE);
		break;
	case HINT_UPDATE_DRAWOBJ:   // a single object has changed
		InvalidObject((CDevObj*)pHint);
		break;

	case HINT_UPDATE_SELECTION: // an entire selection has changed
		{
			POSITION pos;
			CDevObj* pObj;
			pos = m_selection.GetHeadPosition();
			while (pos != NULL)
			{
				pObj = m_selection.GetNext(pos);
				InvalidObject(pObj);
//				pObj->Invalidate();
			}
		}
		break;

	case HINT_UPDATE_CAPTION:
		if (!m_bIsMainFormView)
		{
			AfxSetWindowText(m_hWnd, pDoc->m_strCaption);
			OnNcPaint();
		}
	//		SetActiveWindow();
	//			
	//		Invalidate(TRUE);
		break;
	case HINT_UPDATE_BORDER:
		if (!m_bIsMainFormView)
		{
			DWORD dwAddStyle = 0;
			DWORD dwRemoveStyle = WS_SYSMENU | WS_CAPTION | WS_THICKFRAME;

			if (pDoc->m_bControlBox)
			{
//				dwAddStyle |= WS_SYSMENU;
//				dwRemoveStyle &= ~WS_SYSMENU;
			}
			
			if (pDoc->m_enumBorderStyle != BorderStyleNone)
			{
				dwAddStyle |= WS_CAPTION;
				dwRemoveStyle &= ~WS_CAPTION;
			}
			if (pDoc->m_enumBorderStyle == BorderStyleSizable)
			{
				dwAddStyle |= WS_THICKFRAME;
				dwRemoveStyle &= ~WS_THICKFRAME;
			}

			ModifyStyle(dwRemoveStyle, dwAddStyle, SWP_FRAMECHANGED);

			// ControlBox 更改后必须使用下面语句刷新6
			RedrawWindow(NULL, NULL, RDW_FRAME | RDW_INVALIDATE);

// 			SetScrollInfo();
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}	
}

//////////////////////////////////////////////////////////////////////////
//功 能：对象无效
//////////////////////////////////////////////////////////////////////////
void CLayoutView::InvalidObject(CDevObj *pObj)
{
	CRect rect = pObj->GetBoundingRect();
	rect.NormalizeRect();

	DocToClient(rect);
	if (IsSelected(pObj))
	{
		if (pObj->GetTrackerState(CDevTool::c_enumTrackerState) == CDevObj::rotate)
		{
			CPoint ptTemp = pObj->GetRotateBasePoint();
			DocToClient(ptTemp);
			if (rect.left > ptTemp.x)
				rect.left = ptTemp.x;
			if (rect.right < ptTemp.x)
				rect.right = ptTemp.x;
			if (rect.top > ptTemp.y)
				rect.top = ptTemp.y;
			if (rect.bottom < ptTemp.y)
				rect.bottom = ptTemp.y;
	//		rect.InflateRect(16, 16); 
		}
		rect.InflateRect(10, 10); 
	}

	rect.InflateRect(1, 1); 
//	InvalidateRect(rect, FALSE);

	RedrawWindow(&rect, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN);
}

void CLayoutView::OnLayer(UINT nID) 
{
	int nLayer = nID - ID_LAYER_1;
	if (nLayer > 29)
		return;

	unsigned long pt = 1;
	pt = pt << nLayer;
	if ((GetDocument()->m_ulVisibleLayer & pt) != 0)
		GetDocument()->m_ulVisibleLayer = GetDocument()->m_ulVisibleLayer ^ pt;
	else
		GetDocument()->m_ulVisibleLayer = GetDocument()->m_ulVisibleLayer | pt;
	Invalidate(FALSE);
}

void CLayoutView::OnUpdateLayer(CCmdUI* pCmdUI) 
{
	int nLayer = pCmdUI->m_nID - ID_LAYER_1;
	if (nLayer > 15)
		return;

	pCmdUI->Enable();
	unsigned long ulLayer = 1;
	ulLayer = ulLayer << nLayer;
	if ((ulLayer & GetDocument()->m_ulVisibleLayer) != 0)
		pCmdUI->SetCheck(true);
	else
		pCmdUI->SetCheck(false);
}

void CLayoutView::ClientToDoc(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

void CLayoutView::ClientToDoc(CSize& size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&size);
}

void CLayoutView::ClientToDoc(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
}

void CLayoutView::DocToClient(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CLayoutView::DocToClient(CSize& size)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&size);
}

void CLayoutView::DocToClient(CRect& rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}

void CLayoutView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	pDC->SetMapMode(MM_ANISOTROPIC);

	if (1/*!pDC->IsPrinting()*/)
	{
		pDC->SetViewportExt(m_zoomNum);
		pDC->SetWindowExt(m_zoomDenom);
	}
	else
	{
			
	}

	CFormObj* pFormObj = GetDocument()->m_pFormObj;
	if (pFormObj != NULL)
	{
		CRect rcPos = pFormObj->m_position.GetRECT();
		pDC->SetWindowOrg(rcPos.TopLeft());
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：缩放
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnZoom(UINT nID)
{
	ASSERT(nID >= ID_VIEW_ZOOM50 && nID <= ID_VIEW_ZOOM400);

	CSize zoomDenom;
	CSize zoomNum;

	// all of our zoom factors use denominator of 100
	zoomDenom.cx = 100;
	zoomDenom.cy = 100;

	// get zoom factor numerator and set it
	ASSERT(nID-ID_VIEW_ZOOM50 < sizeof(rgiZoomFactor)/sizeof(rgiZoomFactor[0]));
	int iZoomFactor = rgiZoomFactor[nID-ID_VIEW_ZOOM50];
	zoomNum.cx = iZoomFactor;
	zoomNum.cy = iZoomFactor;

	// change the zoom factor to new zoom factor
	SetZoomFactor(zoomNum, zoomDenom);
	SetScrollInfo();
}

void CLayoutView::OnUpdateZoom(CCmdUI* pCmdUI)
{
	UINT nID = pCmdUI->m_nID;

	if (m_zoomDenom.cx == 100 && m_zoomDenom.cy == 100 &&
		nID != ID_VIEW_ZOOMCUSTOM)
	{
		ASSERT(nID-ID_VIEW_ZOOM50 <
			sizeof(rgiZoomFactor)/sizeof(rgiZoomFactor[0]));
		int iZoomFactor = rgiZoomFactor[nID-ID_VIEW_ZOOM50];

		if (iZoomFactor == m_zoomNum.cx && iZoomFactor == m_zoomNum.cy)
		{
			pCmdUI->SetCheck(TRUE);
			return;
		}
	}

	// default to not checked
	pCmdUI->SetCheck(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//功 能：定制缩放
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnViewZoomCustom()
{
	// prepare dialog data
	CZoomDlg dlg;
	dlg.m_zoomX = m_zoomNum.cx;
	dlg.m_zoomY = m_zoomNum.cy;

	// launch dialog
	if (dlg.DoModal() == IDOK)
	{
		// set new zoom factors
		CSize zoomNum(dlg.m_zoomX, dlg.m_zoomY);
		SetZoomFactor(zoomNum, m_zoomDenom);
		SetScrollInfo();
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：设置缩放因子
//////////////////////////////////////////////////////////////////////////
BOOL CLayoutView::SetZoomFactor(CSize zoomNum, CSize zoomDenom)
{
	if (zoomDenom != m_zoomDenom || zoomNum != m_zoomNum)
	{
		// sync to new zoom factor
		m_zoomNum = zoomNum;
		m_zoomDenom = zoomDenom;

		GetDocument()->UpdateCtrlItemsPosition();

		// resync to new sizes
		Invalidate();
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：设置卷动信息
//////////////////////////////////////////////////////////////////////////
void CLayoutView::SetScrollInfo()
{
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	
	CSize sizeTotal;
	sizeTotal.cx = (int)pDoc->GetWidth();
	sizeTotal.cy = (int)pDoc->GetHeight();

//	CClientDC dc(NULL);
//	sizeTotal.cx = MulDiv(sizeTotal.cx, dc.GetDeviceCaps(LOGPIXELSX), m_zoomDenom.cx);
//	sizeTotal.cy = MulDiv(sizeTotal.cy, dc.GetDeviceCaps(LOGPIXELSY), m_zoomDenom.cy);
	sizeTotal.cx = MulDiv(sizeTotal.cx, m_zoomNum.cx, m_zoomDenom.cx);
	sizeTotal.cy = MulDiv(sizeTotal.cy, m_zoomNum.cy, m_zoomDenom.cy);

	if (!m_bIsMainFormView)
	{
		((CMiddleWnd *)GetParent())->SetFormSize(sizeTotal.cx, sizeTotal.cy);
		CRect rect;
		GetClientRect(&rect);
		SetScrollSizes(MM_TEXT, rect.Size());
	}
	else
	{
		SetScrollSizes(MM_TEXT, sizeTotal);	
	}
}

void CLayoutView::OnTracker(UINT nID)
{
	CDevTool::SetTrackerState(CDevObj::TrackerState(nID - ID_TRACKER_RESIZE + 1));
}

//////////////////////////////////////////////////////////////////////////
//功 能：更新跟踪框
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnUpdateTracker(CCmdUI* pCmdUI)
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		switch (pCmdUI->m_nID)
		{
		case ID_TRACKER_RESIZE:
			pCmdUI->Enable(pObj->GetTrackerState(CDevObj::resize) == CDevObj::resize);
			break;
		case ID_TRACKER_RESHAPE:
			pCmdUI->Enable(pObj->GetTrackerState(CDevObj::reshape) == CDevObj::reshape);
			break;
		case ID_TRACKER_ROTATE:
			pCmdUI->Enable(pObj->GetTrackerState(CDevObj::rotate) == CDevObj::rotate);
			break;
		}
		pCmdUI->SetRadio(m_selection.GetHead()->GetTrackerState(CDevTool::c_enumTrackerState) == CDevObj::TrackerState(pCmdUI->m_nID - ID_TRACKER_RESIZE + 1));
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：调整选择的对象各方向对齐
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnAlign(UINT nID)
{
	ASSERT(nID >= ID_ALIGN_BOTTOM && nID <= ID_ALIGN_TOP);

	POSITION pos;
	CDevObj* pObj;
	CRectF rcBound, rcItem;
	ASSERT(m_pFocusObj != NULL);
	rcBound = m_pFocusObj->GetPositionRect();

	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T(""), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
		
	switch (nID)
	{
	case ID_ALIGN_BOTTOM:
		pAct->SetActionName(_T("下对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF(0, rcBound.bottom - rcItem.bottom));
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_ALIGN_TOP:
		pAct->SetActionName(_T("上对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF(0, rcBound.top - rcItem.top)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_ALIGN_LEFT:
		pAct->SetActionName(_T("左对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF(rcBound.left - rcItem.left, 0)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_ALIGN_RIGHT:
		pAct->SetActionName(_T("右对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF(rcBound.right - rcItem.right, 0)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_ALIGN_HORIZ_CENTER:
		pAct->SetActionName(_T("居中对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF((rcBound.left + rcBound.right) / 2 - (rcItem.right + rcItem.left) / 2, 0)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_ALIGN_VERT_CENTER:
		pAct->SetActionName(_T("中间对齐"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(0, (rcBound.top + rcBound.bottom) / 2 - (rcItem.bottom + rcItem.top) / 2); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;

	default:
		break;
	}

	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

void CLayoutView::OnUpdateAlign(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() > 1);	
}

void CLayoutView::OnCenter(UINT nID) 
{
	CRect rect = GetDocument()->m_position.GetRECT();
	CRectF rcItem;
	POSITION pos;
	CDevObj* pObj;

	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T(""), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();
	
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	switch (nID)
	{
	case ID_HORIZ_CENTER:
		pAct->SetActionName(_T("窗体水平居中"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF((rect.left + rect.right - rcItem.Width()) / 2 - rcItem.left, 0)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_VERT_CENTER:
		pAct->SetActionName(_T("窗体垂直居中"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(CPointF(0, (rect.top + rect.bottom - rcItem.Height()) / 2 - rcItem.top)); 
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	default:
		break;
	}
	
	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

void CLayoutView::OnUpdateCenter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 0);	
}

void CLayoutView::OnRotate(UINT nID) 
{
	CRectF rcItem;
	POSITION pos;
	CDevObj* pObj;
	
	CActionObjSet* pAct = new CActionObjSet(_T(""), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();
	
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}

	pAct->ObjAddComplete();
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	switch (nID)
	{
	case ID_ROTATE_LEFT:
		pAct->SetActionName(_T("向左旋转"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
				pObj->Rotate(-90, this);
		}
		break;
	case ID_ROTATE_RIGHT:
		pAct->SetActionName(_T("向右旋转"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
				pObj->Rotate(90, this);
		}
		break;
	default:
		break;
	}
	
	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

void CLayoutView::OnMirror(UINT nID) 
{
	CRectF rcItem;
	POSITION pos;
	CDevObj* pObj;
	
	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T(""), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();
	
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	switch (nID)
	{
	case ID_MIRROR_HORIZ:
		pAct->SetActionName(_T("水平翻转"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				float fTemp = rcItem.left;
				rcItem.left = rcItem.right;
				rcItem.right = fTemp;
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	case ID_MIRROR_VERT:
		pAct->SetActionName(_T("垂直翻转"));
		pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			if (!pObj->m_bLocked)
			{
				rcItem = pObj->GetPositionRect();
				float fTemp = rcItem.top;
				rcItem.top = rcItem.bottom;
				rcItem.bottom = fTemp;
				pObj->MoveTo(rcItem, FALSE, this);
			}
		}
		break;
	default:
		break;
	}
	
	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

void CLayoutView::OnUpdateRotateOrMirror(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_selection.GetCount() > 0);	
}

//////////////////////////////////////////////////////////////////////////
//功 能：选择的对象下移一层
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnObjectMoveback() 
{
	CFormDoc* pDoc = GetDocument();
	CDevObjList* pObjList = pDoc->GetObjects();
	CDevObj* pObj = m_selection.GetHead();
	CDevObj* pObjBack = FindObjBackward(pObj);
	
	POSITION posObj = pObjList->Find(pObj);
	POSITION posPosition = pObjList->Find(pObjBack);
	pObjList->RemoveAt(posObj);
	pObjList->InsertBefore(posPosition, pObj);
	pObj->m_nLayer = pObjBack->m_nLayer;

	pDoc->SetModifiedFlag();
	InvalidObject(pObj);
}

//////////////////////////////////////////////////////////////////////////
//功 能：选择的对象上移一层
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnObjectMoveforward() 
{
	CFormDoc* pDoc = GetDocument();
	CDevObjList* pObjList = pDoc->GetObjects();
	CDevObj* pObj = m_selection.GetHead();
	CDevObj* pObjBack = FindObjForward(pObj);
	
	POSITION posObj = pObjList->Find(pObj);
	POSITION posPosition = pObjList->Find(pObjBack);
	pObjList->RemoveAt(posObj);
	pObjList->InsertAfter(posPosition, pObj);
	pObj->m_nLayer = pObjBack->m_nLayer;

	pDoc->SetModifiedFlag();
	InvalidObject(pObj);
}

//////////////////////////////////////////////////////////////////////////
//功 能：取选择对象的最小包络矩形
//////////////////////////////////////////////////////////////////////////
CRect CLayoutView::GetSelectionRect(BOOL bBound)
{
	ASSERT(!m_selection.IsEmpty());

	POSITION pos = m_selection.GetHeadPosition();
	CDevObj* pObj = m_selection.GetNext(pos);

	CRect rcObj;
	if (bBound)
		rcObj = pObj->GetBoundingRect();
	else
		rcObj = pObj->GetPositionRect().GetRECT();
	int nLeft = rcObj.left;
	int nTop = rcObj.top;
	int nRight = rcObj.right;
	int nBottom = rcObj.bottom;

	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		if (bBound)
			rcObj = pObj->GetBoundingRect();
		else
			rcObj = pObj->GetPositionRect().GetRECT();
		if (nLeft > rcObj.left)
			nLeft = rcObj.left;
		if (nTop > rcObj.top)
			nTop = rcObj.top;
		if (nRight < rcObj.right)
			nRight = rcObj.right;
		if (nBottom < rcObj.bottom)
			nBottom = rcObj.bottom;
	}
	
	return CRect(nLeft, nTop, nRight, nBottom);
}

//////////////////////////////////////////////////////////////////////////
//功 能：调整选择的对象水平间隔相等
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnSpaceEvenhoriz() 
{	
	float total = 0;
	CDevObj* pFirst = NULL;
	CDevObj* pLast = NULL;
	CDevObj* pTmp = NULL;
	CDevObjList lstSort;

	//查找最前面和最后面的对象, 计算总间格
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pTmp = m_selection.GetNext(pos);

		//计算总对象宽度
		total += pTmp->GetPositionRect().Width();

		//按从左到右排序
		POSITION posTmp = lstSort.GetHeadPosition();
		if (posTmp != NULL)
		{
			BOOL bFlag;
			
			bFlag = FALSE;
			while(posTmp != NULL)
			{
				POSITION posOld = posTmp;
				CDevObj* pObjTmp = lstSort.GetNext(posTmp);
				if(pObjTmp->GetPositionRect().left > pTmp->GetPositionRect().left)
				{
					lstSort.InsertBefore(posOld, pTmp);
					bFlag = TRUE;
					break;
				}
			}

			if (!bFlag)
				lstSort.AddTail(pTmp);
		}
		else
		{
			lstSort.AddHead(pTmp);
		}

		//确定最左和最右的对象
		if (pFirst == NULL)
			pFirst = pTmp;
		else if (pTmp->GetPositionRect().left < pFirst->GetPositionRect().left)
			pFirst = pTmp;
	
		if (pLast == NULL)
		{
			pLast = pTmp;
		}
		else if (pTmp->GetBoundingRect().right > pLast->GetPositionRect().right)
		{
			pLast = pTmp;
		}
	}

	//计算间隔
	float div;
	if (pLast == pFirst)
	{
		total = pFirst->GetPositionRect().Width();
		div = total / m_selection.GetCount();
	}
	else
	{
		total = pLast->GetPositionRect().right - pFirst->GetPositionRect().left - total;
		div = total / (m_selection.GetCount() - 1);
	}

	//调整位置
	float x;
	if (pLast != pFirst)
		x = pFirst->GetPositionRect().right;
	else
		x = pFirst->GetPositionRect().left;
	x += div;
	pos = lstSort.GetHeadPosition();
	while (pos != NULL)
	{
		pTmp = lstSort.GetNext(pos);
		if (pTmp == pFirst || pTmp == pLast)
			continue;

		CRectF rect = pTmp->GetPositionRect();
		rect.right = x + rect.Width();
		rect.left = x;
		if (!pTmp->m_bLocked)
			pTmp->MoveTo(rect, FALSE, this);

		x += rect.Width() + div;
	}
}

void CLayoutView::OnUpdateSpaceEvenhoriz(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 2);
}

//////////////////////////////////////////////////////////////////////////
//功 能：调整选择的对象垂直间隔相等
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnSpaceEvenvert() 
{
	float total = 0;
	CDevObj* pFirst = NULL;
	CDevObj* pLast = NULL;
	CDevObj* pTmp = NULL;
	CDevObjList lstSort;

	//查找最前面和最后面的对象, 计算总间格
	POSITION pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pTmp = m_selection.GetNext(pos);

		//计算总对象宽度
		total += pTmp->GetPositionRect().Height();

		//按从左到右排序
		POSITION posTmp = lstSort.GetHeadPosition();
		if(posTmp != NULL)
		{
			BOOL bFlag;
			
			bFlag = FALSE;
			while(posTmp != NULL)
			{
				POSITION posOld = posTmp;
				CDevObj* pObjTmp = lstSort.GetNext(posTmp);
				if(pObjTmp->GetPositionRect().top > pTmp->GetPositionRect().top)
				{
					lstSort.InsertBefore(posOld, pTmp);
					bFlag = TRUE;
					break;
				}
			}

			if(!bFlag)
				lstSort.AddTail(pTmp);
		}
		else
		{
			lstSort.AddHead(pTmp);
		}

		//确定最左和最右的对象
		if (pFirst == NULL)
			pFirst = pTmp;
		else if (pTmp->GetPositionRect().top < pFirst->GetPositionRect().top)
			pFirst = pTmp;
	
		if (pLast == NULL)
			pLast = pTmp;
		else if (pTmp->GetBoundingRect().bottom > pLast->GetPositionRect().bottom)
				pLast = pTmp;
	}

	//计算间隔
	float div;
	if(pLast == pFirst)
	{
		total = pFirst->GetPositionRect().Height();
		div = total / m_selection.GetCount();
	}
	else
	{
		total = pLast->GetPositionRect().bottom - pFirst->GetPositionRect().top - total;
		div = total / (m_selection.GetCount() - 1);
	}

	//调整位置
	float y;
	if (pLast != pFirst)
		y = pFirst->GetPositionRect().bottom;
	else
		y = pFirst->GetPositionRect().top;
	y += div;
	pos = lstSort.GetHeadPosition();
	while (pos != NULL)
	{
		pTmp = lstSort.GetNext(pos);
		if (pTmp == pFirst || pTmp == pLast)
			continue;

		CRectF rect = pTmp->GetPositionRect();
		rect.bottom = y + rect.Height();
		rect.top = y;
		if (!pTmp->m_bLocked)
			pTmp->MoveTo(rect, FALSE, this);

		y += rect.Height() + div;
	}
}

void CLayoutView::OnUpdateSpaceEvenvert(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 2);	
}

//////////////////////////////////////////////////////////////////////////
//功 能：拷贝的对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditCopy() 
{
	COleDataSource* pDataSource = GetClipboardData(NULL);
	pDataSource->SetClipboard();
}

//////////////////////////////////////////////////////////////////////////
//功 能：粘贴对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditPaste() 
{
	if (IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
	{
		DoPasteSubmodule(NULL, NULL);
		return;
	}

	if (DoPasteObject(NULL, NULL) == FALSE)
		AfxMessageBox(_T("粘贴失败！"));
}

void CLayoutView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 0);
	
}

void CLayoutView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsClipboardFormatAvailable(m_cfItemData) || IsClipboardFormatAvailable(CDevDoc::m_cfDocData));
}

//////////////////////////////////////////////////////////////////////////
//功 能：删除选择的对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditClear() 
{
	POSITION posObj;
	CDevObj* pObj;

	if (m_selection.IsEmpty())
		return;

	GetDocument()->SetModifiedFlag();

	//制作回退对象
	CActionObjAddSet* pAct = new CActionObjAddSet("删除", GetDocument(), FALSE);
	if (pAct == NULL)
		AfxThrowMemoryException();

	//加入选中对象
	posObj = m_selection.GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = m_selection.GetNext(posObj);
			pAct->AddObj(pObj);
	}
	pAct->ObjAddComplete();
	GetDocument()->m_stackUndo.Push(pAct);

	//保存旧的选择对象
	CDevObjList selection;
	posObj = m_selection.GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = m_selection.GetNext(posObj);
		selection.AddTail(pObj);
	}

	//改变当前选中对象
	m_selection.RemoveAll();
	theApp.NotifySelectChanged();

	//删除选中对象
	posObj = selection.GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = selection.GetNext(posObj);
		pObj->Delete();
	}

	GetDocument()->OnFormObjChange();

	GetDocument()->UpdateAllViews(NULL);
}

void CLayoutView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_selection.IsEmpty());
}

//////////////////////////////////////////////////////////////////////////
//功 能：剪切选择的对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditCut() 
{
	OnEditCopy();
	OnEditClear();
}

void CLayoutView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( m_selection.GetCount() > 0 );
}

//////////////////////////////////////////////////////////////////////////
//功 能：选择所有对象
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditSelectAll() 
{
	MutiSelect(GetDocument()->GetObjects());
//
//	CFormDoc* pDoc = GetDocument();
//	CDevObjList* pObjList = pDoc->GetObjects();
//	POSITION posObj = pObjList->GetHeadPosition();
//	while (posObj != NULL)
//	{
//		CDevObj* pObj = (CDevObj *)pObjList->GetNext(posObj);
//		if (!m_selection.Find(pObj))
//			Select(pObj, TRUE);
//	}
//	theApp.NotifySelectChanged();
}

void CLayoutView::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	CFormDoc* pDoc = GetDocument();
	CDevObjList* pObjList = pDoc->GetObjects();
	pCmdUI->Enable(pObjList->GetHeadPosition() != NULL);	
}

//////////////////////////////////////////////////////////////////////////
//功 能：窗口创建后处理
//////////////////////////////////////////////////////////////////////////
int CLayoutView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

//	if (m_bIsFormView)
//	{
//		HICON hIcon = CreateIcon(AfxGetInstanceHandle(), 0, 0, 0, 0, NULL, NULL);
//
//		HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);   
//		SetIcon(NULL, FALSE);
//		SetIcon(NULL, TRUE);
//	}
	
	// register drop target
	m_dropTarget.Register(this);

	m_hThemeDLL = ::LoadLibrary(_T("uxtheme.dll"));
	if (m_hThemeDLL != NULL)
	{
		m_pfnDrawThemeBackground = (PFN_DrawThemeBackground)::GetProcAddress(m_hThemeDLL, "DrawThemeBackground");
		ASSERT(m_pfnDrawThemeBackground != NULL);
		if(m_pfnDrawThemeBackground != NULL)
		{
			OpenThemeData();
		}
		else
		{
			::FreeLibrary(m_hThemeDLL);
			m_hThemeDLL = NULL;
		}
	}
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//功 能：取对象拖放信息
//////////////////////////////////////////////////////////////////////////
BOOL CLayoutView::GetDragInfo(COleDataObject* pDataObject, BOOL bShape)
{
	STGMEDIUM stgmedium;
	if (!pDataObject->GetData(m_cfDragInfo, &stgmedium))
		return FALSE;
	
	ASSERT(stgmedium.tymed == TYMED_ISTREAM);
	
	IStreamPtr lpStream = stgmedium.pstm;
	ReleaseStgMedium(&stgmedium);

	LARGE_INTEGER l = {0, 0};
	lpStream->Seek(l, STREAM_SEEK_SET, NULL);

	lpStream->Read(&m_rcDragRect, sizeof(CRect), NULL);
	lpStream->Read(&m_ptDragStart, sizeof(CPoint), NULL);

	if (!bShape)
		return TRUE;

	// 重置
	m_nDragShapePointSize = 0;
	if (m_pDragShapePoints != NULL)
	{
		delete m_pDragShapePoints;
		m_pDragShapePoints = NULL;
	}
	if (m_pDragShapePointTypes != NULL)
	{
		delete m_pDragShapePointTypes;
		m_pDragShapePointTypes = NULL;
	}

	lpStream->Read(&m_nDragShapePointSize, sizeof(int), NULL);
	
	m_pDragShapePoints = new CPoint[m_nDragShapePointSize];
	if (m_pDragShapePoints == NULL)
		AfxThrowMemoryException();
	m_pDragShapePointTypes = new BYTE[m_nDragShapePointSize];
	if (m_pDragShapePointTypes == NULL)
		AfxThrowMemoryException();
	
	lpStream->Read(m_pDragShapePoints, sizeof(CPoint) * m_nDragShapePointSize, NULL);
	lpStream->Read(m_pDragShapePointTypes, sizeof(BYTE) * m_nDragShapePointSize, NULL);

	// successfully retrieved pSize & pOffset info
	return TRUE;
}

void CLayoutView::DrawDragShape(CDC* pDC)
{
	int nLastMoveTo = 0;
	for (int i = 0; i < m_nDragShapePointSize; i++)
	{
		switch(m_pDragShapePointTypes[i]) 
		{
		case PT_LINETO: 
			pDC->LineTo(m_pDragShapePoints[i]);
			break;
		case PT_BEZIERTO: 
			pDC->PolyBezierTo(&m_pDragShapePoints[i], 3);
			i+=2;
			break;
		case PT_MOVETO:
			pDC->MoveTo(m_pDragShapePoints[i]);
			nLastMoveTo = i;
			break;
		case PT_LINETO | PT_CLOSEFIGURE:
			pDC->LineTo(m_pDragShapePoints[i]);
			pDC->LineTo(m_pDragShapePoints[nLastMoveTo]);
			break;
		case PT_BEZIERTO | PT_CLOSEFIGURE: 
			pDC->PolyBezierTo(&m_pDragShapePoints[i], 3);
			pDC->LineTo(m_pDragShapePoints[nLastMoveTo]);
			break;
		default:
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//功 能：对象拖放进入
//////////////////////////////////////////////////////////////////////////
DROPEFFECT CLayoutView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	ASSERT(m_prevDropEffect == DROPEFFECT_NONE);

	GetDragInfo(pDataObject);

	if (!m_bCanDropEffectCopy)
	{
		if (!m_bInDrag)
			m_bCanDropEffectCopy = TRUE;
	}

	return OnDragOver(pDataObject, dwKeyState, point);
}

//////////////////////////////////////////////////////////////////////////
//功 能：对象拖放移动
//////////////////////////////////////////////////////////////////////////
DROPEFFECT CLayoutView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDataObject->IsDataAvailable(m_cfItemData) && !pDataObject->IsDataAvailable(CDevDoc::m_cfDocData))
		return DROPEFFECT_NONE;

	CPoint ptDragPoint = point;
	ClientToDoc(ptDragPoint);
	AlignToGrid(ptDragPoint);

	if (!m_bCanDropEffectCopy)
	{
		CRect rect(m_ptDragStart, m_ptDragStart);
		rect.InflateRect(2, 2);

		if (!rect.PtInRect(ptDragPoint))
			m_bCanDropEffectCopy = TRUE;
	}

	// 更新状态栏坐标
	((CMainFrame *)(theApp.m_pMainWnd))->OnViewMousePosChange(ptDragPoint);
	
	CSize scSize;
	if (pDoc != NULL)
	{
		scSize.cx = (int)pDoc->GetWidth();
		scSize.cy = (int)pDoc->GetHeight();
	}
	else
	{
		scSize = GetTotalSize();
	}
	DocToClient(scSize);
	CRect rectScroll(CPoint(0, 0), scSize);

	CRect rectItem = m_rcDragRect;
	rectItem.OffsetRect(ptDragPoint - m_ptDragStart);
	DocToClient(rectItem);
	
	if (rectItem.IsRectEmpty())
	{
		// some apps might have a null size in the object descriptor...
		rectItem.InflateRect(1,1);
	}
	rectItem.OffsetRect(GetDeviceScrollPosition());

	DROPEFFECT de = DROPEFFECT_NONE;
	CRect rectTemp;
	if (rectTemp.IntersectRect(rectScroll, rectItem))
	{
		if (m_bCanDropEffectCopy && (dwKeyState & MK_CONTROL) == MK_CONTROL)
			de = DROPEFFECT_COPY;
		else
			de = DROPEFFECT_MOVE;
	}

	if (ptDragPoint == m_ptDragPoint && m_prevDropEffect != DROPEFFECT_NONE)
		return de;

	// otherwise, cursor has moved -- need to update the drag feedback
//	CClientDC dc(this);
	CDC dc;
	PrepareRopDC(&dc, TRUE);
	CPoint ptOldWindowOrg = dc.GetWindowOrg();

	CPen penGrid(PS_DOT, 0, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&penGrid);

	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		CPoint ptTemp = m_ptDragStart - m_ptDragPoint + ptOldWindowOrg;
		dc.SetWindowOrg(ptTemp);
		DrawDragShape(&dc);
	}

	m_prevDropEffect = de;
	
	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		m_ptDragPoint = ptDragPoint;

		CPoint ptTemp = m_ptDragStart - m_ptDragPoint + ptOldWindowOrg;
 		dc.SetWindowOrg(ptTemp);
		DrawDragShape(&dc);
	}

	dc.SelectObject(pOldPen);
	ReleaseDC(&dc);

	return de;
}

//////////////////////////////////////////////////////////////////////////
//功 能：对象拖放离开
//////////////////////////////////////////////////////////////////////////
void CLayoutView::OnDragLeave() 
{
	CDC dc;
	PrepareRopDC(&dc, TRUE);
	CPoint ptOldWindowOrg = dc.GetWindowOrg();
	
	CPen penGrid(PS_DOT, 0, RGB(0, 0, 0));
	CPen* pOldPen = dc.SelectObject(&penGrid);

	if (m_prevDropEffect != DROPEFFECT_NONE)
	{
		CPoint ptTemp = m_ptDragStart - m_ptDragPoint + ptOldWindowOrg;
		dc.SetWindowOrg(ptTemp);
		DrawDragShape(&dc);

		m_prevDropEffect = DROPEFFECT_NONE;
	}

	dc.SelectObject(pOldPen);
	ReleaseDC(&dc);
}

//////////////////////////////////////////////////////////////////////////
//功 能：粘贴对象
//////////////////////////////////////////////////////////////////////////
BOOL CLayoutView::DoPasteObject(COleDataObject* pDataObject, CPoint* pPoint)
{
	CFormDoc* pDoc;
	STGMEDIUM stgmedium;
	IStoragePtr pStorage;
	COleStreamFile file;
	CSizeF sizeDisplacement;
	int nObjects;
	CDevObj* pObj;
	int nObjType;
	CRectF rectObj;
	CDevObjList listObjects;

	pDoc = GetDocument();

	COleDataObject clipboardData;
	if (pDataObject == NULL)
	{
		if (!clipboardData.AttachClipboard())
			return FALSE;
		pDataObject = &clipboardData;
		sizeDisplacement = CSizeF(10, 10);
	}
	else if (pPoint != NULL)
	{
		sizeDisplacement.cx = float(pPoint->x - m_ptDragStart.x);
		sizeDisplacement.cy = float(pPoint->y - m_ptDragStart.y);
	}
	else 
		sizeDisplacement = CSizeF(0, 0);

	if (!pDataObject->GetData(m_cfItemData, &stgmedium))
	{
		return FALSE;
	}

	ASSERT(stgmedium.tymed == TYMED_ISTORAGE);

	pStorage = stgmedium.pstg;
	ReleaseStgMedium(&stgmedium);

	if (!file.OpenStream(pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return FALSE;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);
	ar.m_pDocument = pDoc;

	CActionObjAddSet* pAct = new CActionObjAddSet("粘贴", pDoc, TRUE);
	if (pAct == NULL)
		AfxThrowMemoryException();

	CMapStringToPtr mapClassNameIndex;
	pDoc->m_pMapClassNameIndex = &mapClassNameIndex;

	BOOL bFormObjAdd = FALSE;
	ar >> nObjects;
	for (int i = 0; i < nObjects; i++)
	{
		ar >> nObjType;
		switch (nObjType)
		{
		case CDevObj::drawobj:
			pObj = new CDrawObj(pDoc);
			break;
		case CDevObj::groupobj:
			pObj = new CGroupObj(pDoc);
			break;
		case CDevObj::symbolobj:
			pObj = new CSymbolObj(pDoc);
			break;
		case CDevObj::ctrlobj:
			pObj = new CCtrlObj(pDoc);
			break;
		case CDevObj::formobj:
			pObj = new CFormObj(pDoc);
			bFormObjAdd = TRUE;
			break;
		case CDevObj::reportobj:
			pObj = new CReportObj(pDoc);
			bFormObjAdd = TRUE;
			break;
		default:
			ASSERT(FALSE);
		}

		if (!pObj->CreateFromClipboard(ar, pStorage))
		{
			AfxThrowOleException(E_FAIL);
		}

		rectObj = pObj->GetPositionRect();
		rectObj.OffsetRect(sizeDisplacement);
		pObj->MoveTo(rectObj, FALSE);
		listObjects.AddTail(pObj);
		
		pAct->AddObj(pObj);
	}

	MutiSelect(&listObjects);

	pDoc->m_pMapClassNameIndex = NULL;

	pAct->ObjAddComplete();
	pDoc->m_stackUndo.Push(pAct);

	CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	if (GetParentFrame() != pMainFrame->MDIGetActive())
		GetParentFrame()->ActivateFrame();
	
	ar.Flush();
	file.Close();

	pDoc->SetModifiedFlag();
	
	if (bFormObjAdd)
		pDoc->OnFormObjChange();
	
	Invalidate();

	return TRUE;
}

///////////////////////////////////////////////////////////////
//功 能：从数据源插入画面模块
///////////////////////////////////////////////////////////////
BOOL CLayoutView::DoPasteSubmodule(COleDataObject* pDataObject, CPoint* pPoint)
{
	CFormDoc* pDoc;
	CSizeF sizeDisplacement;
	STGMEDIUM stgmedium;
	IStoragePtr pStorage;
	COleStreamFile file;
	int nCount;
	int nType;

	pDoc = GetDocument();
	
	COleDataObject clipboardData;
	if (pDataObject == NULL)
	{
		if (!clipboardData.AttachClipboard())
			return FALSE;
		pDataObject = &clipboardData;
	}
	
	if (!pDataObject->GetData(CDevDoc::m_cfDocData, &stgmedium))
	{
		return FALSE;
	}

	else if (pPoint != NULL)
	{
		sizeDisplacement.cx = float(pPoint->x - m_ptDragStart.x);
		sizeDisplacement.cy = float(pPoint->y - m_ptDragStart.y);
	}
	else 
		sizeDisplacement = CSizeF(0, 0);

	ASSERT(stgmedium.tymed == TYMED_ISTORAGE);

	pStorage = stgmedium.pstg;
	ReleaseStgMedium(&stgmedium);

	if (!file.OpenStream(pStorage, _T("Contents"), STGM_READ|
		STGM_SHARE_EXCLUSIVE))
	{
		return FALSE;
	}

	CArchive ar(&file, CArchive::load|CArchive::bNoFlushOnDelete);

	CActionObjAddSet* pAct = new CActionObjAddSet("插入模块", pDoc, TRUE);
	if (pAct == NULL)
		AfxThrowMemoryException();

	ar >> nCount;
	CFormObj* pObj;
	for (int i = 0; i < nCount; i++)
	{
		ar >> nType;
		switch ((enum SUBDOCTYPE)nType)
		{
		case CDevDoc::enumFormDoc:
 			pObj = new CFormObj(pDoc);
			break;
		case CDevDoc::enumReportDoc:
			pObj = new CReportObj(pDoc);
			break;
		default:
			ASSERT(FALSE);
		}

		if (!pObj->CreateFromSubDocumentClipboard(ar, pStorage))
		{
			AfxThrowOleException(E_FAIL);
		}

		CRectF rectObj = pObj->GetPositionRect();
		rectObj.OffsetRect(sizeDisplacement);
		pObj->MoveTo(rectObj, FALSE);

		Select(pObj);
// 		pObj->Invalidate();

		pAct->AddObj(pObj);
	}

	pAct->ObjAddComplete();
	pDoc->m_stackUndo.Push(pAct);
	
	ar.Flush();
	file.Close();

	CMDIFrameWnd* pMainFrame = (CMDIFrameWnd*)AfxGetMainWnd();
	if (GetParentFrame() != pMainFrame->MDIGetActive())
		GetParentFrame()->ActivateFrame();
	else
		theApp.NotifySelectChanged();

	pDoc->SetModifiedFlag();

	pDoc->OnFormObjChange();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：对象拖放释放
//////////////////////////////////////////////////////////////////////////
BOOL CLayoutView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	CDevObj* pObj;

	ASSERT_VALID(this);

	// clean up focus rect
	OnDragLeave();

	ClientToDoc(point);
	AlignToGrid(point);

	GetDragInfo(pDataObject, FALSE);

	// 在同一个窗口移动
	if ((dropEffect & DROPEFFECT_MOVE) && m_bInDrag)
	{
		m_bInDrag = FALSE; // signal drag code that a move happened

		if (point != m_ptDragStart)
		{
			CSizeF sizeDisplacement;
			CRectF rcItem;
				
			CActionObjMoveSet* pAct = new CActionObjMoveSet(_T("移动"), GetDocument());
			if (pAct == NULL)
				AfxThrowMemoryException();
//
			POSITION pos = m_selection.GetHeadPosition();
			while (pos != NULL)
			{
				pObj = m_selection.GetNext(pos);
				pAct->AddObj(pObj);
			}
			
			GetDocument()->m_stackUndo.Push(pAct);

			sizeDisplacement.cx = float(point.x - m_ptDragStart.x);
			sizeDisplacement.cy = float(point.y - m_ptDragStart.y);
			
			pos = m_selection.GetHeadPosition();
			while (pos != NULL)
			{
				pObj = m_selection.GetNext(pos);
				rcItem = pObj->GetPositionRect();
				rcItem.OffsetRect(sizeDisplacement);
				if (!pObj->m_bLocked)
				{
					pObj->MoveTo(rcItem, FALSE, this);
					pObj->Invalidate();
				}
			}

			GetDocument()->SetModifiedFlag();
			theApp.NotifySelectChanged(FALSE, 0, TRUE);
		}
	}
	else
	{
		if (pDataObject->IsDataAvailable(CDevDoc::m_cfDocData))
			return DoPasteSubmodule(pDataObject, &point);

		if (DoPasteObject(pDataObject, &point))
			return TRUE;
	}

	return FALSE;
}

BOOL CLayoutView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// remove drag/drop feedback before scrolling
	if (bDoScroll && m_prevDropEffect != DROPEFFECT_NONE)
	{
		CDC dc;
		PrepareRopDC(&dc, TRUE);
		CPoint ptOldWindowOrg = dc.GetWindowOrg();
		
		CPen penGrid(PS_DOT, 0, RGB(0, 0, 0));
		CPen* pOldPen = dc.SelectObject(&penGrid);
		
		if (m_prevDropEffect != DROPEFFECT_NONE)
		{
			CPoint ptTemp = m_ptDragStart - m_ptDragPoint + ptOldWindowOrg;
			dc.SetWindowOrg(ptTemp);
			DrawDragShape(&dc);
		}
		
		dc.SelectObject(pOldPen);
		ReleaseDC(&dc);

		// erase previous focus rect
		m_prevDropEffect = DROPEFFECT_NONE;
	}

	// do the scroll
	if (!CScrollView::OnScrollBy(sizeScroll, bDoScroll))
		return FALSE;

	// update the position of any in-place active item
	if (bDoScroll)
		UpdateWindow();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：获取选中项目的数据
//////////////////////////////////////////////////////////////////////////
void CLayoutView::GetSelectedObjectData(LPSTGMEDIUM lpStgMedium)
{
	IStoragePtr pStorage;
	HRESULT hResult;
	POSITION posObj;
	CDevObj* pObj;
	COleStreamFile file;
	int nSelectedObjects;

	ASSERT(m_selection.GetCount() > 0);

	hResult = StgCreateDocfile(NULL, STGM_CREATE|STGM_READWRITE|
		STGM_DELETEONRELEASE|STGM_SHARE_EXCLUSIVE, 0, &pStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (!file.CreateStream(pStorage, _T("Contents"), CFile::modeReadWrite|
		CFile::shareExclusive|CFile::modeCreate))
	{
		AfxThrowOleException(E_FAIL);
	}

	CArchive ar(&file, CArchive::store);

	nSelectedObjects = m_selection.GetCount();
	ar << nSelectedObjects;

	CDevObjList* pObjList = GetDocument()->GetObjects();
	posObj = pObjList->GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = pObjList->GetNext(posObj);
		if (m_selection.Find(pObj) != NULL)
		{
			ar << int(pObj->GetObjectType());
			pObj->CopyToClipboard(ar, pStorage);
		}
	}

	lpStgMedium->tymed = TYMED_ISTORAGE;
	lpStgMedium->pstg = pStorage;
	lpStgMedium->pstg->AddRef();
	lpStgMedium->pUnkForRelease = NULL;

	ar.Flush();
	file.Close();
}

STDAPI_(HDC) OleStdCreateDC(DVTARGETDEVICE FAR* ptd)
{
	HDC hdc=NULL;
	LPDEVNAMES lpDevNames;
	LPDEVMODE lpDevMode;
	LPTSTR lpszDriverName;
	LPTSTR lpszDeviceName;
	LPTSTR lpszPortName;
	
	if (ptd == NULL) {
		hdc = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		goto errReturn;
	}
	
	lpDevNames = (LPDEVNAMES) ptd; // offset for size field
	
	if (ptd->tdExtDevmodeOffset == 0) {
		lpDevMode = NULL;
	}else{
		lpDevMode  = (LPDEVMODE) ((LPTSTR)ptd + ptd->tdExtDevmodeOffset);
	}
	
	lpszDriverName = (LPTSTR) lpDevNames + ptd->tdDriverNameOffset;
	lpszDeviceName = (LPTSTR) lpDevNames + ptd->tdDeviceNameOffset;
	lpszPortName   = (LPTSTR) lpDevNames + ptd->tdPortNameOffset;
	
	hdc = CreateDC(lpszDriverName, lpszDeviceName, lpszPortName, lpDevMode);
	
errReturn:
	return hdc;
}

class CDevObjDataSource : public COleDataSource
{
public:
	void SetObjects(CDevObjList& listObjects, CRect rcDragRect, CPoint ptStart)
	{
		m_listObjects.RemoveAll();
		m_listObjects.AddTail(&listObjects);

		m_ptDragStart = ptStart;
		m_rcDragRect = rcDragRect;
	}

	BOOL RenderDragInfoData(LPSTGMEDIUM lpStgMedium)
	{
		ASSERT(AfxIsValidAddress(lpStgMedium, sizeof(STGMEDIUM)));
		
		LPSTREAM lpStream;
		if (::CreateStreamOnHGlobal(NULL, TRUE, &lpStream) != S_OK)
			AfxThrowMemoryException();
		
		ASSERT(lpStream != NULL);

		lpStream->Write(&m_rcDragRect, sizeof(CRect), NULL);
		lpStream->Write(&m_ptDragStart, sizeof(CPoint), NULL);
		
		CClientDC dc(NULL);
		dc.BeginPath();
		
		if (m_listObjects.GetCount() <= 32)
		{
			// 绘制轮廓
			POSITION posObj = m_listObjects.GetHeadPosition();
			while (posObj != NULL)
			{
				CDevObj* pObj = m_listObjects.GetNext(posObj);
				pObj->DrawTrackerShape(&dc);
			}
		}
		else
		{
			dc.MoveTo(m_rcDragRect.left, m_rcDragRect.top);
			dc.LineTo(m_rcDragRect.right, m_rcDragRect.top);
			dc.LineTo(m_rcDragRect.right, m_rcDragRect.bottom);
			dc.LineTo(m_rcDragRect.left, m_rcDragRect.bottom);
			dc.LineTo(m_rcDragRect.left, m_rcDragRect.top);
		}

		dc.EndPath();
		
		int iNumPoints = dc.GetPath(NULL, NULL, 0);
		
		CPoint* pPoints = new CPoint[iNumPoints];
		if (pPoints == NULL)
			AfxThrowMemoryException();
		BYTE* pTypes = new BYTE[iNumPoints];
		if (pTypes == NULL)
			AfxThrowMemoryException();
		
		iNumPoints = dc.GetPath(pPoints, pTypes, iNumPoints);
		
		lpStream->Write(&iNumPoints, sizeof(int), NULL);
		lpStream->Write(pPoints, sizeof(CPoint) * iNumPoints, NULL);
		lpStream->Write(pTypes, sizeof(BYTE) * iNumPoints, NULL);
		
		if (pPoints != NULL)
			delete pPoints;
		if (pTypes != NULL)
			delete pTypes;
	
		
		// setup the STGMEDIUM
		lpStgMedium->tymed = TYMED_ISTREAM;
		lpStgMedium->pstm = lpStream;
		lpStgMedium->pUnkForRelease = NULL;

		return TRUE;
	}

	BOOL RenderItemData(LPSTGMEDIUM lpStgMedium)
	{
		IStoragePtr pStorage;
		HRESULT hResult;
		POSITION posObj;
		CDevObj* pObj;
		COleStreamFile file;
		int nObjects;
		
		ASSERT(m_listObjects.GetCount() > 0);
		
		hResult = StgCreateDocfile(NULL, STGM_CREATE|STGM_READWRITE|
			STGM_DELETEONRELEASE|STGM_SHARE_EXCLUSIVE, 0, &pStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
		
		if (!file.CreateStream(pStorage, _T("Contents"), CFile::modeReadWrite|
			CFile::shareExclusive|CFile::modeCreate))
		{
			AfxThrowOleException(E_FAIL);
		}
		
		CArchive ar(&file, CArchive::store);
		
		nObjects = m_listObjects.GetCount();
		ar << nObjects;
		
		posObj = m_listObjects.GetHeadPosition();
		while (posObj != NULL)
		{
			pObj = m_listObjects.GetNext(posObj);
			ar << int(pObj->GetObjectType());
			pObj->CopyToClipboard(ar, pStorage);
		}
		
		lpStgMedium->tymed = TYMED_ISTORAGE;
		lpStgMedium->pstg = pStorage;
		lpStgMedium->pstg->AddRef();
		lpStgMedium->pUnkForRelease = NULL;
		
		ar.Flush();
		file.Close();
		
		return TRUE;
	}

	///////////////////////////////////////////////////////////////
	//功能:后提交数据
	///////////////////////////////////////////////////////////////
	virtual BOOL OnRenderData(LPFORMATETC lpFormatEtc, LPSTGMEDIUM lpStgMedium)
	{
		if (lpFormatEtc->cfFormat == CLayoutView::m_cfDragInfo)
			return RenderDragInfoData(lpStgMedium);

		if (lpFormatEtc->cfFormat == CLayoutView::m_cfItemData)
			return RenderItemData(lpStgMedium);
		
		return FALSE;
	}
	
	CDevObjList m_listObjects;

	CPoint m_ptDragStart;
	CRect m_rcDragRect;
};

/////////////////////////////////////////////////////////////////////
//功 能：取选择对象的剪贴板数据
/////////////////////////////////////////////////////////////////////
COleDataSource* CLayoutView::GetClipboardData(LPPOINT lpStart, BOOL bDrag)
{
	STGMEDIUM stgMedium;
	CDevObjDataSource* pDataSource;

	pDataSource = new CDevObjDataSource;

	if (bDrag)
	{
		ASSERT(lpStart != NULL);

		CDevObjList objects;

		// 保证分层与原来一致
		CDevObjList* pObjList = GetDocument()->GetObjects();
		POSITION posObj = pObjList->GetHeadPosition();
		while (posObj != NULL)
		{
			CDevObj* pObj = pObjList->GetNext(posObj);
			if (!pObj->m_bLocked && m_selection.Find(pObj) != NULL)
				objects.AddTail(pObj);
		}

		CRect rcBound = GetSelectionRect();
		pDataSource->SetObjects(objects, rcBound, CPoint(*lpStart));

		pDataSource->DelayRenderData(m_cfDragInfo);
		pDataSource->DelayRenderData(m_cfItemData);
	}
	else
	{
		GetSelectedObjectData(&stgMedium);
		pDataSource->CacheData(m_cfItemData, &stgMedium);
	}

	ASSERT_VALID(pDataSource);
	return pDataSource;
}

void CLayoutView::OnFocus(CDevObj* pObj, BOOL bGotFocus)
{
	ASSERT(pObj != NULL);

	if (bGotFocus)
	{
		m_pObjWithFocus = pObj;
	}
	else
	{
		if (m_pObjWithFocus == pObj)
		{
			m_pObjWithFocus = NULL;
		}
	}
}

/////////////////////////////////////////////////////////////////////
//功 能：取焦点对象
/////////////////////////////////////////////////////////////////////
CDevObj* CLayoutView::GetObjectWithFocus() const
{
	return m_pObjWithFocus;
}

/////////////////////////////////////////////////////////////////////
//功 能：无窗口对象释放焦点
/////////////////////////////////////////////////////////////////////
HRESULT CLayoutView::OnWindowlessReleaseFocus()
{
	ASSERT( m_pObjWithFocus != NULL );

	m_pObjWithFocus = NULL;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////
//功 能：无窗口对象设置焦点
/////////////////////////////////////////////////////////////////////
HRESULT CLayoutView::OnWindowlessSetFocus(CDevObj* pObj)
{
   ASSERT(pObj != NULL);
   
   SetFocus();
   m_pObjWithFocus = pObj;

   return S_OK;
}

#include "..\CxPropSheet\CxPropSheetExport.h"
const CLSID CLSID_CxBlankPage = {0x5CA14830,0xC760,0x4A3A,{0xBB,0xB7,0xA7,0x24,0x1B,0xE9,0xA7,0x3B}};

/////////////////////////////////////////////////////////////////////
//功 能：使用属性页编辑对象属性
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditProperties() 
{
	POSITION posObj;
	CDevObj* pObj;
	CArray< CLSID, CLSID& > aclsidCommonPages;
	CArray< CLSID, CLSID& > aclsidPages;
	BOOL bSuccess;
	int iPage;
	int iCommonPage;
	BOOL bFound;
	CLSID clsidPage;

	if (m_selection.GetCount() == 1)
	{
		pObj = m_selection.GetHead();
		if (pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
			OnDesignSymbol();
			return;
		}
		if (pObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
		{
			OnEditSubform();
			return;
		}
	}

	posObj = m_selection.GetHeadPosition();
	if (posObj == NULL)
		return;

	pObj = m_selection.GetNext(posObj);
	bSuccess = pObj->GetPropertyPageCLSIDs(aclsidCommonPages);
	if (!bSuccess)
		return;

	while (posObj != NULL)
	{
		pObj = m_selection.GetNext( posObj );
		aclsidPages.RemoveAll();
		bSuccess = pObj->GetPropertyPageCLSIDs(aclsidPages);
		if (!bSuccess)
			return;
	
		for (iCommonPage = aclsidCommonPages.GetSize()-1; iCommonPage >= 0;	iCommonPage--)
		{
			clsidPage = aclsidCommonPages[iCommonPage];
			bFound = FALSE;
			for (iPage = 0; iPage < aclsidPages.GetSize(); iPage++)
			{
				if (IsEqualCLSID( clsidPage, aclsidPages[iPage]))
					bFound = TRUE;
			}
			if (!bFound)
				aclsidCommonPages.RemoveAt(iCommonPage);
		}
	}

	BOOL bNoPage = aclsidCommonPages.GetSize() == 0;
	if (bNoPage)
	{
		CLSID clsidPage = CLSID_CxBlankPage;
		aclsidCommonPages.Add(clsidPage);	
	}
	
	USES_CONVERSION;
	int iObject;
	IUnknown** ppObjects;
	CLSID* pclsidCommonPages;
	CString strCaption;

	ppObjects = (IUnknown**)_alloca( m_selection.GetCount()*sizeof(
		IUnknown* ) );
	iObject = 0;
	posObj = m_selection.GetTailPosition();
	while (posObj != NULL)
	{
		pObj = m_selection.GetPrev(posObj);
		ppObjects[iObject] = pObj->GetObjectUnknown();
		iObject++;
	}

	pclsidCommonPages = (CLSID*)_alloca(aclsidCommonPages.GetSize() * sizeof(CLSID));
	for (iPage = 0; iPage < aclsidCommonPages.GetSize(); iPage++)
		pclsidCommonPages[iPage] = aclsidCommonPages[iPage];

	if (m_selection.GetCount() > 1)
		strCaption.LoadString(IDS_MULTIPLESELECTION);
	else
		strCaption.Format(IDS_SINGLESELECTION, m_selection.GetHead()->GetDisplayName());

	if (!bNoPage)
	{
		CActionObjSet* pAct = new CActionObjSet(_T("改变属性"), GetDocument());

		POSITION pos = m_selection.GetHeadPosition();
		CDevObj* pObj;
		while (pos != NULL)
		{
			pObj = m_selection.GetNext(pos);
			pAct->AddObj(pObj);
		}
		pAct->ObjAddComplete();

		GetDocument()->m_stackUndo.Push(pAct);
	//	GetDocument()->m_stackUndo.Reset();
	}
//	CxCreatePropertyFrame	

	if (S_OK != CxCreatePropertyFrame(m_hWnd, 0, 0, T2COLE(strCaption),
			m_selection.GetCount(), ppObjects, aclsidCommonPages.GetSize(),
			pclsidCommonPages, GetUserDefaultLCID(), 0, NULL))
	{
//		if (!bNoPage)
//			if (GetDocument()->m_stackUndo.CanUndo())
//				GetDocument()->m_stackUndo.Undo();
	}

//	OleCreatePropertyFrame( m_hWnd, 0, 0, T2COLE( strCaption ),
//		 m_selection.GetCount(), ppObjects, aclsidCommonPages.GetSize(),
//		 pclsidCommonPages, GetUserDefaultLCID(), 0, NULL );

	GetDocument()->SetModifiedFlag();
	theApp.NotifySelectChanged();

	// 2005.11.28 注释下面语句，因与前面语句重复
//	GetDocument()->SetModifiedFlag();
//	theApp.NotifySelectChanged();

	//WINBUG: OleCreatePropertyFrame doesn't return focus to its parent when
	// it closes
	SetFocus();
}

/////////////////////////////////////////////////////////////////////
//功 能：组合对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnGroupObject() 
{
	CGroupObj* pGrpObj;
	CDevObj* pSubObj;
	POSITION pos;

	//保存组合前状态
	CFormDoc *pDoc = GetDocument();
	CActionObjGroup* pAct = new CActionObjGroup(_T("合成组"), pDoc, TRUE);
	if (pAct == NULL)
		AfxThrowMemoryException();
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = m_selection.GetNext(pos);
		pAct->AddSubObj(pSubObj);
	}
	pAct->ObjAddComplete();

	//组合
	pGrpObj = pDoc->AddGroupObj(NULL);
	ASSERT_VALID(pGrpObj);
	CDevObjList* pObjList = GetDocument()->GetObjects();
	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = pObjList->GetNext(pos);
		if (m_selection.Find(pSubObj) != NULL)
		{
			pGrpObj->AddSubObject(pSubObj);
			pDoc->RemoveObject(pSubObj);
			pSubObj->m_pDocument = pGrpObj->m_pDocument;
		}
	}

	//保存组合后状态
	pAct->SetGroupObj(pGrpObj);
	pDoc->m_stackUndo.Push(pAct);

	//选中
	m_selection.RemoveAll();
	Select(pGrpObj);
	GetDocument()->OnFormObjChange();
}

void CLayoutView::OnUpdateGroupObject(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pDevObj = m_selection.GetHead();
		if (pDevObj->IsKindOf(RUNTIME_CLASS(CGroupObj)) && !pDevObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	pCmdUI->Enable(m_selection.GetCount() > 0);	
}

/////////////////////////////////////////////////////////////////////
//功 能：分解组合对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnUngroupObject() 
{
	CGroupObj* pGrpObj;
	CDevObj* pDevObj;

	pDevObj = m_selection.GetHead();
	ASSERT_VALID(pDevObj);
	if (pDevObj->IsKindOf(RUNTIME_CLASS(CGroupObj)))
		pGrpObj = STATIC_DOWNCAST(CGroupObj, pDevObj);
	else
		return;
	ASSERT_VALID(pGrpObj);

	Select(NULL);

	//保存解组前状态
	CFormDoc *pDoc = GetDocument();
	CActionObjGroup* pAct = new CActionObjGroup(_T("分解组"), pDoc, FALSE);
	if(pAct == NULL)
		AfxThrowMemoryException();
	pAct->SetGroupObj(pGrpObj);

	//解组
	CDevObjArray& ObjList = pGrpObj->GetSubObjects();
	CDevObjArray Objs;
	int nCount = ObjList.GetSize(); 
	CDevObj* pSubObj = NULL;
	for(int i = 0; i < nCount; i++)
	{	
		//保存子对象表
		pSubObj = ObjList[i];
		Objs.Add(pSubObj);
	}
	Select(NULL);

	for(i = 0; i < nCount; i++)
	{	
		//移动子对象至文档
		pSubObj = Objs[i];
		pGrpObj->RemoveObject(pSubObj);
		pSubObj->m_pDocument = NULL;
		pSubObj->OnUngrouped();
		pDoc->AddObject(pSubObj);
		pAct->AddSubObj(pSubObj);
		Select(pSubObj, TRUE);
	}
	Objs.RemoveAll();
	pAct->ObjAddComplete();

	pGrpObj->Delete();
	pDoc->m_stackUndo.Push(pAct);

	theApp.NotifySelectChanged();
	GetDocument()->OnFormObjChange();
}

void CLayoutView::OnUpdateUngroupObject(CCmdUI* pCmdUI) 
{
	CDevObj* pDevObj = NULL;
	if (m_selection.GetCount() == 1)
		pDevObj = m_selection.GetHead();

	pCmdUI->Enable(pDevObj != NULL && pDevObj->IsKindOf(RUNTIME_CLASS(CGroupObj)) && !pDevObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)));		
}

/////////////////////////////////////////////////////////////////////
//功 能：插入图像对象
/////////////////////////////////////////////////////////////////////

const CLSID CLSID_DrawImage = {0xA9EF82F7,0x1757,0x405C,{0x88,0x6D,0x40,0x59,0x28,0xE9,0xEF,0xAD}};

void CLayoutView::OnInsertBitmap() 
{
	USES_CONVERSION;

	CDevTool::c_drawShape = DRAW_SELECTION;

	CFileDialog dlg(TRUE);

	dlg.m_ofn.lpstrFilter = 
		_T("所有图片文件\0*.bmp;*.dib;*.jpg;*.gif;*.tif;*.tiff;*.png;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("TIIF 图像 (*.tif;*.tiff)\0*.tif;*.tiff\0")
		_T("PNG 图像 (*.png)\0*.png\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	dlg.m_ofn.lpstrTitle = _T("加载图片");

	if (dlg.DoModal() != IDOK)
		return;                 // stay with old data file

	CFormDoc* pDoc = GetDocument();
	CDrawObj* pObj = pDoc->AddDrawObj(CLSID_DrawImage, NULL);
	ASSERT(pObj != NULL);
	
	COleDispatchDriver driver;
	IDispatchPtr pDispatch = pObj->GetObjectUnknown();
	driver.AttachDispatch(pDispatch, FALSE);
	static BYTE parms[] = VTS_BOOL VTS_I2 VTS_BSTR;
	driver.InvokeHelper(1, DISPATCH_METHOD,
			VT_EMPTY, NULL, parms, TRUE, 0, dlg.GetPathName());

	Select(pObj);
	
	// 2004.3.7 增加
	CString strAction;
	strAction.LoadString(IDS_ADDOBJ);
	CAction* pAct = new CActionObjAdd(strAction, pDoc, pObj, TRUE);
	pDoc->m_stackUndo.Push(pAct);
}

/////////////////////////////////////////////////////////////////////
//功 能：将选择的对象转换成图像对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditConvertToBmp() 
{
	CFormDoc* pDoc = GetDocument();

	CClientDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CRect rect = GetSelectionRect(TRUE);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	COLORREF clrBackColor;
	OleTranslateColor(pDoc->m_clrBackColor, 0, &clrBackColor);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), clrBackColor);
	COLORREF clrRealBackColor = memDC.GetPixel(0, 0);
	memDC.SetWindowOrg(rect.left, rect.top);

	CActionObjGroup* pAct = new CActionObjGroup(_T("转换为图像"), pDoc, TRUE);
	if (pAct == NULL)
		AfxThrowMemoryException();
	
	//注意下面的实现是为了避免对象层次出错
	CDevObj* pObj = NULL;
	CDevObjList* pObjList = pDoc->GetObjects();
	POSITION pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pObj = pObjList->GetNext(pos);
		if (m_selection.Find(pObj) != NULL)
		{
			pAct->AddSubObj(pObj);
			pObj->Draw(&memDC);
		}
	}
	pAct->ObjAddComplete();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pObj->Delete();
	}
	m_selection.RemoveAll();

	//加入位图对象
	CDrawObj* pBmpObj = pDoc->AddDrawObj(CLSID_DrawImage, NULL);
	ASSERT(pBmpObj != NULL);
	
	COleDispatchDriver driver;
	IDispatchPtr pDispatch = pBmpObj->GetObjectUnknown();
	driver.AttachDispatch(pDispatch, FALSE);
	static BYTE parms[] = VTS_BOOL VTS_I2 VTS_I4 VTS_I4;
	CPalette* pPal = memDC.GetCurrentPalette();
	driver.InvokeHelper(2, DISPATCH_METHOD,
			VT_EMPTY, NULL, parms, TRUE, 0, (long)bmp.GetSafeHandle(), (long)(HPALETTE)pPal->GetSafeHandle());
	
	driver.SetProperty(CXPROPID_Transparency, VT_BOOL, VARIANT_TRUE);
	driver.SetProperty(7, VT_COLOR, clrRealBackColor);

	memDC.SelectObject(pOldBmp);

	pBmpObj->MoveTo(rect, FALSE, this);
	
	//加入回退对象
	//保存组合后状态
	pAct->SetGroupObj(pBmpObj);
	pDoc->m_stackUndo.Push(pAct);
	
	Select(pBmpObj);
	GetDocument()->OnFormObjChange();
}

void CLayoutView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDevTool* pTool = CDevTool::FindTool(CDevTool::c_drawShape);
		if (pTool != NULL)
			pTool->OnKeyDown(this, nChar, nRepCnt, nFlags);
		
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CLayoutView::OnNcHitTest(CPoint point)
{
	UINT unRet = CScrollView::OnNcHitTest(point);
	if (m_bIsMainFormView)
		return unRet;

	if (unRet == HTCLIENT)
		return unRet;
	else
		return HTNOWHERE;
}

////////////////////////////////////////////////////////////////////////
//功能:画出非客户区的内容
////////////////////////////////////////////////////////////////////////
void CLayoutView::OnNcPaint()
{
	CScrollView::OnNcPaint();

	if (m_bIsMainFormView || !m_bIsFormView)
		return;

	DWORD dwStyle = GetStyle();

	if ((dwStyle & WS_CAPTION) == 0)
		return;

	CSize szFrame = (dwStyle & WS_THICKFRAME) ?
			CSize(GetSystemMetrics(SM_CXSIZEFRAME),	GetSystemMetrics(SM_CYSIZEFRAME)) :
			CSize(GetSystemMetrics(SM_CXFIXEDFRAME), GetSystemMetrics(SM_CYFIXEDFRAME));
	
	CRect rcCaption;
	GetWindowRect(&rcCaption);
	rcCaption.OffsetRect(-rcCaption.left, -rcCaption.top);
	rcCaption.left += szFrame.cx;
	rcCaption.top += szFrame.cy; 
	rcCaption.right -= szFrame.cx;
	rcCaption.bottom = rcCaption.top + GetSystemMetrics(SM_CYCAPTION)  // height of caption
			- GetSystemMetrics(SM_CYBORDER);

	CWindowDC dc(this);
	
//	if (m_hTheme != NULL)
//		m_pfnDrawThemeBackground(m_hTheme, dc.GetSafeHdc(), 1, 1, &rcCaption, NULL);
//	else
//		DrawCaption(m_hWnd, dc.GetSafeHdc(), &rcCaption, DC_ACTIVE | DC_ICON | DC_TEXT | 0x0020);

	CFormDoc* pDoc = GetDocument();	
	if (!pDoc->m_bControlBox)
		return;

	int cxIcon = GetSystemMetrics(SM_CXSIZE);
	int cyIcon = GetSystemMetrics(SM_CYSIZE);

	// Draw caption buttons. These are all drawn inside a rectangle
	// of dimensions SM_CXSIZE by SM_CYSIZE
	CRect rc(0, 0, cxIcon, cyIcon);
	rc.OffsetRect(rcCaption.TopLeft());
	rc += CPoint(rcCaption.Width() - cxIcon, 0);    // move right
	
	// Close box has a 2 pixel border on all sides but left, which is zero
	rc.DeflateRect(1, 2);
	rc.right -= 1;

	if (m_hTheme != NULL)
		m_pfnDrawThemeBackground(m_hTheme, dc.GetSafeHdc(), 18, m_bActive ? 1 : 5, &rc, NULL);
	else
		dc.DrawFrameControl(&rc, DFC_CAPTION, DFCS_CAPTIONCLOSE);
}

/////////////////////////////////////////////////////////////////////
//功 能：重载创建函数
/////////////////////////////////////////////////////////////////////
BOOL CLayoutView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	if (pContext->m_pCurrentDoc != NULL && pContext->m_pCurrentDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
	{	
		//找出对应的文档
		CFormDoc* pDoc = (CFormDoc*)(pContext->m_pCurrentDoc);
		m_bIsMainFormView = pDoc->IsMainForm();
			
		if (pDoc->m_pFormObj != NULL || pDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
			m_bIsFormView = FALSE;
	}

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CLayoutView::OnUpdateEditConverttobmp(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() > 0)
	{
		POSITION pos = m_selection.GetHeadPosition();
		while(pos != NULL)
		{
			CDevObj* pObj = m_selection.GetNext(pos);
			if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
			{
				pCmdUI->Enable(FALSE);
				return;
			}
		}
		pCmdUI->Enable();
	}
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////
//功 能：右键菜单
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (CDevTool::c_drawShape != DRAW_SELECTION)
		return;

	CPoint local = point;
	ScreenToClient(&local);
	CDevObj* pHitObj = GetObjectAt(local);
	BOOL bShiftState = (GetKeyState(VK_SHIFT) < 0);
	if (pHitObj != NULL)
	{
		if (!IsSelected(pHitObj))
		{
			if (bShiftState)
			{
				Select(pHitObj, TRUE, FALSE);
				theApp.NotifySelectChanged();
			}
			else
			{
				Select(pHitObj, FALSE, FALSE);
			}
		}
		else if (m_pFocusObj != pHitObj)
		{
			Select(pHitObj, TRUE, FALSE);
			Select(pHitObj, TRUE, FALSE);
			theApp.NotifySelectChanged();
		}
	}
	else
	{
		Select(NULL);
	}

	CMenu menu;
	menu.CreatePopupMenu();

	if (m_selection.GetCount() > 0)
	{
		menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("剪切(&T)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_CLONE, _T("复制(&L)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("拷贝(&C)"));
		if (IsClipboardFormatAvailable(m_cfItemData) || IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));
		menu.AppendMenu(MF_STRING, ID_EDIT_CLEAR, _T("删除(&D)"));

		menu.AppendMenu(MF_SEPARATOR);
		
	}
	else if (IsClipboardFormatAvailable(m_cfItemData) || IsClipboardFormatAvailable(CDevDoc::m_cfDocData))
	{
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("粘贴(&P)"));

		menu.AppendMenu(MF_SEPARATOR);
	}
		
	menu.AppendMenu(MF_STRING, ID_VIEW_APPEAR_EXPERT, _T("颜色和线条(&M)"));
	menu.AppendMenu(MF_STRING, ID_VIEW_DYNAMIC_EXPERT, _T("动画(&M)"));
	menu.AppendMenu(MF_STRING, ID_VIEW_ACTION_EXPERT, _T("动作(&A)"));
	
	menu.AppendMenu(MF_SEPARATOR);

	if (m_selection.GetCount() > 0)
		menu.AppendMenu(MF_STRING, ID_EDIT_PROPERTIES, _T("属性(&P)"));
	
	if (m_selection.GetCount() <= 1)
		menu.AppendMenu(MF_STRING, ID_EDIT_SCRIPT, _T("查看脚本(&C)"));
	
	if (m_selection.GetCount() == 1)
	{
		menu.AppendMenu(MF_SEPARATOR);
		
		CDevObj* pObj = m_selection.GetHead();
		if (pObj->GetTrackerState(CDevObj::resize) == CDevObj::resize)
			menu.AppendMenu(MF_STRING, ID_TRACKER_RESIZE, _T("改变尺寸(&S)"));
		if (pObj->GetTrackerState(CDevObj::reshape) == CDevObj::reshape)
			menu.AppendMenu(MF_STRING, ID_TRACKER_RESHAPE, _T("编辑顶点(&F)"));
		if (pObj->GetTrackerState(CDevObj::rotate) == CDevObj::rotate)
			menu.AppendMenu(MF_STRING, ID_TRACKER_ROTATE, _T("自由旋转(&R)"));
	}
	
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		menu.AppendMenu(MF_SEPARATOR);
		if (pObj->IsKindOf(RUNTIME_CLASS(CGroupObj)) && !pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
			menu.AppendMenu(MF_STRING, ID_UNGROUP_OBJECT, _T("分解组(&U)"));
			menu.AppendMenu(MF_SEPARATOR);
		}
		else if (!pObj->IsKindOf(RUNTIME_CLASS(CGroupObj)) || pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
			menu.AppendMenu(MF_STRING, ID_GROUP_OBJECT, _T("合成组(&G)"));
			menu.AppendMenu(MF_SEPARATOR);
		}
		
		if (pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
		{
	//		menu.AppendMenu(MF_STRING, ID_DESIGN_SYMBOL, _T("设计图符(&D)"));
			menu.AppendMenu(MF_STRING, ID_BREAK_SYMBOL, _T("分解图符(&B)"));
		}
		else
		{
			menu.AppendMenu(MF_STRING, ID_CREATE_SYMBOL, _T("生成图符(&B)"));
		}
	//	menu.AppendMenu(MF_SEPARATOR);
		if (pObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
		{
	//		menu.AppendMenu(MF_STRING, ID_EDIT_SUBFORM, _T("编辑模块(&E)"));
			menu.AppendMenu(MF_STRING, ID_BREAK_SUBFORM, _T("分解模块(&B)"));
		}
		else
		{
			menu.AppendMenu(MF_STRING, ID_CREATE_SUBFORM, _T("生成模块(&M)"));
		}
	}
	else
	{
		if (m_selection.GetCount() > 0)
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_GROUP_OBJECT, _T("合成组(&G)"));
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_CREATE_SYMBOL, _T("生成图符(&B)"));
			menu.AppendMenu(MF_STRING, ID_CREATE_SUBFORM, _T("生成模块(&M)"));
		}
	}

	if (m_selection.GetCount() > 0)
	{
		BOOL bInsertCB = TRUE;
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			CDevObj* pObj = m_selection.GetNext(pos);
			if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
			{
				bInsertCB = FALSE;
				break;;
			}
		}

		if (bInsertCB)
		{
			menu.AppendMenu(MF_STRING, ID_EDIT_CONVERTTOBMP, _T("转换为图像(&I)"));
		}
	}

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (m_selection.GetCount() > 0)
	{
		CSymbolLibWnd* pWnd = NULL;
		if (pFrame != NULL)
			pWnd = (CSymbolLibWnd*)(pFrame->GetToolSymbol());
		if (pWnd != NULL && pWnd->CanAddSymbol())
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_ADDTO_SYMBOL, _T("添加到图符库(&A)"));
		}
	}
	else
	{
		CModuleWnd* pWnd = NULL;
		if (pFrame != NULL)
			pWnd = (CModuleWnd*)(pFrame->GetToolModule());
		if (pWnd != NULL && pWnd->CanAddModule())
		{	
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_ADDTO_MODULE, _T("添加到模块库(&M)"));
		}
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, theApp.m_pMainWnd);
}

/////////////////////////////////////////////////////////////////////
//功 能：编辑脚本
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditScript()
{
	CDevObj* pObj = NULL;
	if (!m_selection.IsEmpty())
		pObj = m_selection.GetHead();
	((CMainFrame*)(theApp.m_pMainWnd))->ViewCode(pObj, this);
}

/////////////////////////////////////////////////////////////////////
//功 能：加入选中的对象到图符库
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnAddToSymbolLib() 
{
	CMainFrame* pFrame = (CMainFrame *)(theApp.m_pMainWnd);
	if (pFrame != NULL)
	{
		CSymbolLibWnd* pWnd = (CSymbolLibWnd*)(pFrame->GetToolSymbol());
		if (pWnd != NULL && pWnd->CanAddSymbol())
		{
			OnEditCopy();
			pWnd->DoPaste();
		}
	}	
}

/////////////////////////////////////////////////////////////////////
//功 能：添加到模块库
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnAddToModuleLib() 
{
//	CFormDoc* pDoc = GetDocument();
//	if (pDoc != NULL && pDoc->m_pFormSubDoc != NULL)
//	{
//		DWORD docID = pDoc->m_pFormSubDoc->m_dwDocumentNumber;
//		CDevDoc* pDevDoc = pDoc->m_pFormSubDoc->m_pParentDoc;
//		if (pDevDoc != NULL)
//		{
			CMainFrame* pFrame = (CMainFrame*)(theApp.m_pMainWnd);
			if (pFrame != NULL)
			{
				CModuleWnd* pWnd = (CModuleWnd*)(pFrame->GetToolModule());
				if (pWnd != NULL && pWnd->CanAddModule())
					pWnd->AddFormModule(GetDocument());
			}
//		}	
//	}
}

void CLayoutView::OnUpdateAddtoSymbol(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() > 0)
	{
		CSymbolLibWnd* pWnd = NULL;
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		if (pFrame != NULL)
			pWnd = (CSymbolLibWnd*)(pFrame->GetToolSymbol());
		if (pWnd != NULL && pWnd->CanAddSymbol())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}

	pCmdUI->Enable(FALSE);
}

void CLayoutView::OnUpdateAddtoModule(CCmdUI* pCmdUI) 
{
	CModuleWnd* pWnd = NULL;
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (pFrame != NULL)
		pWnd = (CModuleWnd*)(pFrame->GetToolModule());
	if (pWnd != NULL && pWnd->CanAddModule())
	{	
		pCmdUI->Enable(TRUE);
		return;
	}

	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////
//功 能：层功能设置
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnLayerSetup()
{
	CLayerSettingDlg dlg;
	dlg.m_nCurLayer = GetDocument()->m_nLayer;
	dlg.m_ulLockLayer = GetDocument()->m_ulLockLayer;
	dlg.m_ulVisibleLayer = GetDocument()->m_ulVisibleLayer;
	if (dlg.DoModal() == IDOK)
	{
		GetDocument()->m_nLayer = dlg.m_nCurLayer;
		GetDocument()->m_ulLockLayer = dlg.m_ulLockLayer;
		GetDocument()->m_ulVisibleLayer = dlg.m_ulVisibleLayer;
		
		CDevObj* pObj = NULL;
		POSITION pos = m_selection.GetHeadPosition();
		while (pos != NULL)
		{
			POSITION posOld = pos;
			pObj = m_selection.GetNext(pos);
			unsigned long ulMask = 1;
			ulMask = ulMask << pObj->m_nLayer;
			if ((ulMask & GetDocument()->m_ulLockLayer) != 0 || (ulMask & GetDocument()->m_ulVisibleLayer) == 0)
			{
				Select(NULL);
				break;
			}
		}

		Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////
//功能：返回在指定位置的对象, 如无或被锁定则返回NULL
/////////////////////////////////////////////////////////////////////
CDevObj* CLayoutView::GetObjectAt(POINT pt)
{
	CPoint local = pt;
	ClientToDoc(local);

	return GetDocument()->GetObjectAt(local);
}

/////////////////////////////////////////////////////////////////////
//功能：移动对象到最前面
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnObjectForwardMost() 
{
	ASSERT(m_selection.GetCount() == 1);
	CDevObj* pObj = (CDevObj *)(m_selection.GetHead());
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CDevObjList* pObjList = pDoc->GetObjects();

	int nLayer = 0;
	POSITION pos = pObjList->GetHeadPosition();
	POSITION posObj = NULL;
	while (pos != NULL)
	{
		POSITION posOld;
		posOld = pos;

		CDevObj* pTmp = pObjList->GetNext(pos);
		if (pTmp->m_nLayer > nLayer)
			nLayer = pTmp->m_nLayer;
		
		if (pTmp == pObj)
			posObj = posOld;
	}

	if (posObj != NULL)
	{
		pObj->m_nLayer = nLayer;
		pObjList->RemoveAt(posObj);
		pObjList->AddTail(pObj);
		pDoc->SetModifiedFlag();
		pObj->Invalidate(this);
	}		
}

void CLayoutView::OnUpdateObjectForwardMost(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		if (FindObjForward(pObj) != NULL)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	
	pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////
//功能：移动对象到最后面
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnObjectBackwordMost() 
{
	ASSERT(m_selection.GetCount() == 1);
	CDevObj* pObj = (CDevObj *)(m_selection.GetHead());
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);

	CDevObjList* pObjList = pDoc->GetObjects();

	int nLayer = 0;
	POSITION pos = pObjList->GetHeadPosition();
	POSITION posObj = NULL;
	while (pos != NULL)
	{
		POSITION posOld;
		posOld = pos;

		CDevObj* pTmp = pObjList->GetNext(pos);
		if (pTmp->m_nLayer > nLayer)
			nLayer = pTmp->m_nLayer;
		
		if (pTmp == pObj)
			posObj = posOld;
	}

	if (posObj != NULL)
	{
		pObj->m_nLayer = nLayer;
		pObjList->RemoveAt(posObj);
		pObjList->AddHead(pObj);
		pDoc->SetModifiedFlag();
		pObj->Invalidate(this);
	}		
}

void CLayoutView::OnUpdateObjectBackwordMost(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		if (FindObjBackward(pObj) != NULL)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	
	pCmdUI->Enable(FALSE);	
}

/////////////////////////////////////////////////////////////////////
//功能：查找指定对象前面的对象
//说明：不可见对象被忽略
/////////////////////////////////////////////////////////////////////
CDevObj* CLayoutView::FindObjForward(CDevObj *pObj)
{
	ASSERT(pObj != NULL);
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	CDevObjList* pObjList = pDoc->GetObjects();
	CRect rcObj = pObj->GetBoundingRect();
	CDevObj* pSearchObj;
	CRect rcTemp;

	POSITION posObj;
	for (int i = pObj->m_nLayer; i < 30; i++)
	{
		if (i == pObj->m_nLayer)
		{
			posObj = pObjList->Find(pObj);
			pObjList->GetNext(posObj);
		}
		else
		{
			posObj = pObjList->GetHeadPosition();
		}
		
		while (posObj != NULL)
		{
			pSearchObj = pObjList->GetNext(posObj);
			if (i == pSearchObj->m_nLayer && rcTemp.IntersectRect(pSearchObj->GetBoundingRect(), &rcObj))
				return pSearchObj;
		}
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////
//功能：查找指定对象后面的对象
//说明：不可见对象被忽略
/////////////////////////////////////////////////////////////////////
CDevObj* CLayoutView::FindObjBackward(CDevObj *pObj)
{
	ASSERT(pObj != NULL);
	CFormDoc* pDoc = GetDocument();
	ASSERT(pDoc != NULL);
	CDevObjList* pObjList = pDoc->GetObjects();
	CRect rcObj = pObj->GetBoundingRect();
	CDevObj* pSearchObj;
	CRect rcTemp;

	POSITION posObj;
	for (int i = pObj->m_nLayer; i >= 0; i--)
	{
		if (i == pObj->m_nLayer)
		{
			posObj = pObjList->Find(pObj);
			pObjList->GetPrev(posObj);
		}
		else
		{
			posObj = pObjList->GetTailPosition();
		}
			
		while (posObj != NULL)
		{
			pSearchObj = pObjList->GetPrev(posObj);
			if (i == pSearchObj->m_nLayer && rcTemp.IntersectRect(pSearchObj->GetBoundingRect(), &rcObj))
				return pSearchObj;
		}
	}

	return NULL;
}

void CLayoutView::OnUpdateAllSelUpTwo(CCmdUI* pCmdUI) 
{
	if (m_selection.GetCount() > 1)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

/////////////////////////////////////////////////////////////////////
//功 能：调整选择的多个对象宽度相等
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnArrageSamewidth() 
{
	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T("宽度相等"), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();

	POSITION pos = m_selection.GetHeadPosition();
	CDevObj* pObj;
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	ASSERT(m_pFocusObj != NULL);
	CRectF rcFocus = m_pFocusObj->GetPositionRect();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		if (pObj != m_pFocusObj)
		{
			CRectF rect = pObj->GetPositionRect();
			rect.right = rect.left + rcFocus.Width();
			pObj->MoveTo(rect, FALSE, this);
		}
	}	

	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////
//功 能：调整选择的多个对象高度相等
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnArrageSamehight() 
{
	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T("高度相等"), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();

	POSITION pos = m_selection.GetHeadPosition();
	CDevObj* pObj;
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	ASSERT(m_pFocusObj != NULL);
	CRectF rcFocus = m_pFocusObj->GetPositionRect();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		if (pObj != m_pFocusObj)
		{
			CRectF rect = pObj->GetPositionRect();
			rect.bottom = rect.top + rcFocus.Height();
			pObj->MoveTo(rect, FALSE, this);
		}
	}
	
	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////
//功 能：调整选择的多个对象高度和宽度都相等
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnArrageBoth() 
{
	CActionObjMoveSet* pAct = new CActionObjMoveSet(_T("两者都相等"), GetDocument());
	if (pAct == NULL)
		AfxThrowMemoryException();

	POSITION pos = m_selection.GetHeadPosition();
	CDevObj* pObj;
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		pAct->AddObj(pObj);
	}
	
	GetDocument()->m_stackUndo.Push(pAct);
	GetDocument()->SetModifiedFlag();
	
	ASSERT(m_pFocusObj != NULL);
	CRectF rcFocus = m_pFocusObj->GetPositionRect();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pObj = m_selection.GetNext(pos);
		if (pObj != m_pFocusObj)
		{
			CRectF rect = pObj->GetPositionRect();
			rect.bottom = rect.top + rcFocus.Height();
			rect.right = rect.left + rcFocus.Width();
			pObj->MoveTo(rect, FALSE, this);
		}
	}

	theApp.NotifySelectChanged(FALSE, 0, TRUE);
}

/////////////////////////////////////////////////////////////////////
//功 能：恢复回退功能
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditRedo() 
{
	GetDocument()->m_stackUndo.Redo();
	GetDocument()->SetModifiedFlag();
}

/////////////////////////////////////////////////////////////////////
//功 能：更新恢复回退菜单
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	CString str;

	if (GetDocument()->m_stackUndo.CanRedo())
	{
		str = _T("恢复");
		str += GetDocument()->m_stackUndo.GetNextRedoCmdStr();
		pCmdUI->Enable();
	}
	else
	{
		str = _T("无法恢复");
		pCmdUI->Enable(FALSE);
	}
	str += _T("(&R)\tCtrl+Y");

	pCmdUI->SetText(str);
}

/////////////////////////////////////////////////////////////////////
//功 能：回退功能
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditUndo() 
{
	GetDocument()->m_stackUndo.Undo();
	GetDocument()->SetModifiedFlag();
}

/////////////////////////////////////////////////////////////////////
//功 能：更新回退菜单
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	CString str;

	if (GetDocument()->m_stackUndo.CanUndo())
	{
		str = _T("撤消");
		str += GetDocument()->m_stackUndo.GetNextUndoCmdStr();
		pCmdUI->Enable();
	}
	else
	{
		str = _T("无法撤消");
		pCmdUI->Enable(FALSE);
	}
	str += _T("(&U)\tCtrl+Z");

	pCmdUI->SetText(str);
}

/////////////////////////////////////////////////////////////////////
//功 能：复制对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnEditClone() 
{
	OnEditCopy();	
	OnEditPaste();	
}

/////////////////////////////////////////////////////////////////////
//功 能：不处理缺省光标设置，直接返回
/////////////////////////////////////////////////////////////////////
BOOL CLayoutView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
//功 能：设计图符对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnDesignSymbol() 
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		CActionObj* pAct = new CActionObj(_T("设计图符"), GetDocument(), pObj);

		if (pObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
			if(((CSymbolObj *)pObj)->BeginDesign(this))
			{
				GetDocument()->SetModifiedFlag();
				pAct->SetNewName(pObj->GetDisplayName());
				GetDocument()->m_stackUndo.Push(pAct);
				
				Select(pObj);
				return;
			}
		delete pAct;
	}
}

/////////////////////////////////////////////////////////////////////
//功 能：创建图符对象
/////////////////////////////////////////////////////////////////////
void CLayoutView::OnCreateSymbol() 
{
	CDevObj* pSubObj;
	POSITION pos;

	//保存组合前状态
	CFormDoc *pDoc = GetDocument();
	CActionObjGroup* pAct = new CActionObjGroup(_T("生成图符"), pDoc, TRUE);
	if(pAct == NULL)
		AfxThrowMemoryException();
	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = m_selection.GetNext(pos);
		pAct->AddSubObj(pSubObj);
	}
	pAct->ObjAddComplete();

	//组合
	CSymbolObj* pSymbolObj = pDoc->AddSymbolObj(NULL);
	ASSERT_VALID(pSymbolObj);
	CDevObjList* pObjList = GetDocument()->GetObjects();
	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = pObjList->GetNext(pos);
		if (m_selection.Find(pSubObj) != NULL)
		{
			pSymbolObj->AddSubObject(pSubObj);
			pDoc->RemoveObject(pSubObj);
			pSubObj->m_pDocument = pSymbolObj->m_pDocument;
		}
	}

	//保存组合后状态
	pAct->SetGroupObj(pSymbolObj);
	pDoc->m_stackUndo.Push(pAct);

	//选中
	m_selection.RemoveAll();
	Select(pSymbolObj);
	GetDocument()->OnFormObjChange();

	OnDesignSymbol();
}

void CLayoutView::OnUpdateDesignSymbol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() == 1 && m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CSymbolObj)));	
}

void CLayoutView::OnUpdateCreateSymbol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 1 || (m_selection.GetCount() == 1 && !m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CSymbolObj))));	
}

void CLayoutView::OnBreakSymbol() 
{
	OnUngroupObject();
}

void CLayoutView::OnUpdateBreakSymbol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() == 1 && m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CSymbolObj)));	
}

void CLayoutView::OnEditSubform() 
{
	if (m_selection.GetCount() == 1)
	{
		CDevObj* pObj = m_selection.GetHead();
		if (pObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
		{
			((CFormObj *)pObj)->OnEdit();
		}
	}
}

void CLayoutView::OnUpdateEditSubform(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() == 1 && m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CFormObj)));	
}

void CLayoutView::OnAddSubmodule() 
{
	CFormDoc* pDoc = GetDocument();

	CFormObj* pObj = new CFormObj(pDoc);
	pObj->CreateOrLoad(NULL, IID_NULL, NULL);

	GetDocument()->OnFormObjChange();

	CAction* pAct = new CActionObjAdd("插入模块", pDoc, pObj, TRUE);
	pDoc->m_stackUndo.Push(pAct);
	
	pDoc->SetModifiedFlag();
	
	Select(pObj);

	OnEditSubform();
}

void CLayoutView::OnCreateSubform() 
{
	CDevObj* pSubObj;
	POSITION pos;

	//保存组合前状态
	CFormDoc *pDoc = GetDocument();
	CActionObjGroup* pAct = new CActionObjGroup(_T("生成模块"), pDoc, TRUE);
	if(pAct == NULL)
		AfxThrowMemoryException();

	pos = m_selection.GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = m_selection.GetNext(pos);
		pAct->AddSubObj(pSubObj);
	}
	pAct->ObjAddComplete();

	//组合
	CFormObj* pFormObj = new CFormObj(pDoc);
	pFormObj->CreateOrLoad(NULL, IID_NULL, NULL);
	pSubObj = m_selection.GetHead();
	CRectF rcPos = pSubObj->GetPositionRect();
	pFormObj->MoveTo(rcPos, FALSE);
	
	ASSERT_VALID(pFormObj);
	CDevObjList* pObjList = pDoc->GetObjects();

	pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = pObjList->GetNext(pos);
		if (m_selection.Find(pSubObj) != NULL)
		{
			pDoc->RemoveObject(pSubObj);
			VERIFY(pDoc->GetNamedObjectsMap()->RemoveKey(pSubObj->GetDisplayName()));
		//	pSubObj->m_pDocument = pFormObj->m_pFormDoc;
			pFormObj->AddSubObject(pSubObj);
			pSubObj->SetChildObjectDocument();	// 重新设置子对象的文档指针
		}
	}
	pFormObj->RecalcPosition();
	rcPos = pFormObj->GetPositionRect();
	pFormObj->MoveTo(rcPos, FALSE);

	//保存组合后状态
	pAct->SetGroupObj(pFormObj);
	pDoc->m_stackUndo.Push(pAct);

	//选中
	m_selection.RemoveAll();
	Select(pFormObj);
	GetDocument()->OnFormObjChange();

	OnEditSubform();
}

void CLayoutView::OnUpdateCreateSubform(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() > 1 || (m_selection.GetCount() == 1 && !m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CFormObj))));	
}

void CLayoutView::OnBreakSubform() 
{
	CFormObj* pFormObj;
	CDevObj* pDevObj;

	pDevObj = m_selection.GetHead();
	ASSERT_VALID(pDevObj);
	if (pDevObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
		pFormObj = STATIC_DOWNCAST(CFormObj, pDevObj);
	else
		return;
	ASSERT_VALID(pFormObj);

	Select(NULL);

	//保存解组前状态
	CFormDoc *pDoc = GetDocument();
	CActionObjGroup* pAct = new CActionObjGroup(_T("分解模块"), pDoc, FALSE);
	if(pAct == NULL)
		AfxThrowMemoryException();
	pAct->SetGroupObj(pFormObj);

	//解组
	CDevObjList* pObjList = pFormObj->m_pFormDoc->GetObjects();
	CDevObjList objects;
	CDevObj* pSubObj = NULL;
	POSITION pos = pObjList->GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = pObjList->GetNext(pos);
		objects.AddTail(pSubObj);
	}
	Select(NULL);

	pos = objects.GetHeadPosition();
	while (pos != NULL)
	{
		pSubObj = objects.GetNext(pos);
		pFormObj->RemoveObject(pSubObj);
		pSubObj->m_pDocument = NULL;
		pDoc->AddObject(pSubObj);
		pSubObj->SetChildObjectDocument(); // 重新设置子对象的文档指针
		if (!pSubObj->PutDisplayName(pSubObj->GetDisplayName()))
			pDoc->CreateUniqueObjectName(pSubObj);
	
		pAct->AddSubObj(pSubObj);
		
		Select(pSubObj, TRUE);
	}

	objects.RemoveAll();
	pAct->ObjAddComplete();

	pFormObj->Delete();
	pDoc->m_stackUndo.Push(pAct);

	theApp.NotifySelectChanged();
	GetDocument()->OnFormObjChange();
}

void CLayoutView::OnUpdateBreakSubform(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_selection.GetCount() == 1 && m_selection.GetHead()->IsKindOf(RUNTIME_CLASS(CFormObj)));	
}

BOOL CLayoutView::PreTranslateMessage(MSG* pMsg) 
{
	if (/*(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||*/
		(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) ||
		(pMsg->message >= WM_NCMOUSEMOVE && pMsg->message <= WM_NCMBUTTONDBLCLK))
	{
		if (::IsChild(m_hWnd, pMsg->hwnd))
		{
			if (pMsg->message >= WM_NCMOUSEMOVE && pMsg->message <= WM_NCMBUTTONDBLCLK)
				pMsg->message -=  (WM_NCMOUSEMOVE - WM_MOUSEFIRST); 

			if (pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
			{
				CPoint point = pMsg->pt;
				ScreenToClient(&point);
				LPARAM lParam = (point.y << 16) | point.x;
				SendMessage(pMsg->message, pMsg->wParam, lParam);
			}
			
			return TRUE;
		}
	}

	if (pMsg->message == WM_PAINT)
	{
		if (::IsChild(m_hWnd, pMsg->hwnd))
		{
			UpdateWindow();
//			::SendMessage(pMsg->hwnd, pMsg->message, pMsg->wParam, pMsg->lParam);
//			DrawTracker(NULL);
			return TRUE;
		}
//		{
//			CRect rcWindow;
//			::GetWindowRect(pMsg->hwnd, &rcWindow);
//			InvalidateRect(&rcWindow);
//		}
	}
		
	return CScrollView::PreTranslateMessage(pMsg);
}

void CLayoutView::PrepareRopDC(CDC* pDC, BOOL bPrepare)
{
	ASSERT(pDC->m_hDC == NULL);
	
	pDC->Attach(::GetDCEx(m_hWnd, NULL, DCX_CACHE));

//	pDC->Attach(::GetDC(NULL));
//	
//	CRect rcClient;
//	GetClientRect(&rcClient);
//	CRect rectScreen = rcClient;
//	ClientToScreen(&rectScreen);
	if (bPrepare)
		OnPrepareDC(pDC, NULL);
//	pDC->OffsetViewportOrg(rectScreen.left, rectScreen.top);
//
//	CRgn rgn;
//	rgn.CreateRectRgnIndirect(&rectScreen);
//	pDC->SelectClipRgn(&rgn);


	pDC->SetROP2(R2_NOTXORPEN);	
}

void CLayoutView::EnsureObjectVisible(CDevObj* pObj)
{
	CRect rectView;
	GetClientRect(&rectView);
	ClientToDoc(rectView);

	CPoint point;
	point.x = (int)pObj->GetPositionRect().left;
	point.y = (int)pObj->GetPositionRect().top;

	if (!rectView.PtInRect(point))
		ScrollToPosition(point);
}

void CLayoutView::OnHelpIndex() 
{
	CDevObj* pSelObj = NULL;
	if (!m_selection.IsEmpty())
		pSelObj = m_selection.GetTail();
	
	CString strKeyword;
	if (pSelObj != NULL)
		strKeyword = pSelObj->GetClassName();
	else	
		strKeyword = GetDocument()->GetClassName();

	HH_AKLINK link;
	
	link.cbStruct = sizeof(HH_AKLINK);
	link.fReserved = FALSE;
	link.pszKeywords = strKeyword; 
	link.pszUrl = NULL; 
	link.pszMsgText = NULL; 
	link.pszMsgTitle = NULL; 
	link.pszWindow = NULL;
	link.fIndexOnFail = TRUE ;

	CString strHelpFile = theApp.GetCurrentAppDirectory();
	strHelpFile += _T("CxHlp.chm");
	HtmlHelp(
		NULL,
		strHelpFile,
		HH_KEYWORD_LOOKUP,
		(DWORD)&link);
}

void CLayoutView::OnGridSettingChanged()
{
	OnUpdate(NULL, HINT_UPDATE_WINDOW, NULL);
}

inline int roundleast(int n)
{
	int mod = n%10;
	n -= mod;
	if (mod >= 5)
		n += 10;
	else if (mod <= -5)
		n -= 10;
	return n;
}

void RoundRect(LPRECT r1)
{
	r1->left = roundleast(r1->left);
	r1->right = roundleast(r1->right);
	r1->top = roundleast(r1->top);
	r1->bottom = roundleast(r1->bottom);
}

void MulDivRect(LPRECT r1, LPRECT r2, int num, int div)
{
	r1->left = MulDiv(r2->left, num, div);
	r1->top = MulDiv(r2->top, num, div);
	r1->right = MulDiv(r2->right, num, div);
	r1->bottom = MulDiv(r2->bottom, num, div);
}

void CLayoutView::OnPageSetup() 
{
	CPageSetupDialog dlg;
	PAGESETUPDLG& psd = dlg.m_psd;
	psd.Flags |= PSD_MARGINS | PSD_INHUNDREDTHSOFMILLIMETERS;
	MulDivRect(&psd.rtMargin, m_rectMargin, 2540, 1440);
	RoundRect(&psd.rtMargin);
	// get the current device from the app
	PRINTDLG pd;
	pd.hDevNames = NULL;
	pd.hDevMode = NULL;
	theApp.GetPrinterDeviceDefaults(&pd);
	psd.hDevNames = pd.hDevNames;
	psd.hDevMode = pd.hDevMode;
	if (dlg.DoModal() == IDOK)
	{
		RoundRect(&psd.rtMargin);
		MulDivRect(m_rectMargin, &psd.rtMargin, 1440, 2540);
		theApp.m_settingGen.m_rectPageMargin = m_rectMargin;
		theApp.SelectPrinter(psd.hDevNames, psd.hDevMode);
	}
}

void CLayoutView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CFormDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POINT pt;
	CRect rectMargin;
	CRect rectPage;

	CRect rect = pDoc->m_position.GetRECT();
	
	pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &pt);
	rectMargin.left = pt.x;
	rectMargin.top  = pt.y;
	
	pDC->Escape(GETPHYSPAGESIZE, 0, NULL, &pt);
	rectPage.SetRect(0, 0, pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
	
	rectMargin.right  = pt.x                     // total paper width
		- pDC->GetDeviceCaps(HORZRES) // printable width
		- rectMargin.left;   // left unprtable margin
	
	rectMargin.bottom = pt.y                     // total paper height
		- pDC->GetDeviceCaps(VERTRES) // printable ht
		- rectMargin.top;    // rt unprtable margin
	
	pt.x = pDC->GetDeviceCaps(LOGPIXELSX);    // dpi in X direction
	pt.y = pDC->GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
	
	rectMargin.left = MulDiv(m_rectMargin.left, pt.x, 1440)
		- rectMargin.left;
	rectMargin.top  = MulDiv(m_rectMargin.top, pt.y, 1440)
		- rectMargin.top;
	rectMargin.right  = MulDiv(m_rectMargin.right, pt.x, 1440)
		- rectMargin.right;
	rectMargin.bottom = MulDiv(m_rectMargin.bottom, pt.y, 1440)
		- rectMargin.bottom;

	rectPage.InflateRect(-rectMargin.left, -rectMargin.top, -rectMargin.right, -rectMargin.bottom);

	float fXScale = (float)rectPage.Width() / rect.Width();
	float fYScale = (float)rectPage.Height() / rect.Height();
	float fScale = min(fXScale, fYScale); // max(min(nXScale, nYScale), 1);
	
	int cxBlt = int(rect.Width() * fScale);
	int cyBlt = int(rect.Height() * fScale);
	int xOff = (rectPage.left + rectPage.right) / 2 - cxBlt / 2;
	int yOff = (rectPage.top + rectPage.bottom) / 2 - cyBlt / 2;

	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportOrg(xOff, yOff);
	pDC->SetViewportExt(cxBlt, cyBlt);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	rect.OffsetRect(-rect.left, 0);
	pDC->IntersectClipRect(&rect);

	OnDraw(pDC);
}

UINT CLayoutView::_GetThemeChangedMsg()
{
#ifndef WM_THEMECHANGED
	static const UINT WM_THEMECHANGED = 0x031A;
#endif //!WM_THEMECHANGED
	return WM_THEMECHANGED;
}

LRESULT CLayoutView::OnThemeChanged(WPARAM, LPARAM)
{
	if (m_hThemeDLL != NULL)
	{
		CloseThemeData();
		OpenThemeData();
	}

	return 0;
}

void CLayoutView::OpenThemeData()
{
	ATLASSERT(m_hThemeDLL != NULL);

	PFN_OpenThemeData pfnOpenThemeData = (PFN_OpenThemeData)::GetProcAddress(m_hThemeDLL, "OpenThemeData");
	ATLASSERT(pfnOpenThemeData != NULL);
	if(pfnOpenThemeData != NULL)
		m_hTheme = pfnOpenThemeData(m_hWnd, L"Window");
}

void CLayoutView::CloseThemeData()
{
	ATLASSERT(m_hThemeDLL != NULL);

	if (m_hTheme == NULL)
		return;   // nothing to do

	PFN_CloseThemeData pfnCloseThemeData = (PFN_CloseThemeData)::GetProcAddress(m_hThemeDLL, "CloseThemeData");
	ATLASSERT(pfnCloseThemeData != NULL);
	if(pfnCloseThemeData != NULL)
	{
		pfnCloseThemeData(m_hTheme);
		m_hTheme = NULL;
	}
}

void CLayoutView::OnTabOrder() 
{
	CTabOrderDlg dlg;
	dlg.m_pObjList = GetDocument()->GetObjects();
	if (dlg.DoModal() == IDOK)
		GetDocument()->SetModifiedFlag();
}
