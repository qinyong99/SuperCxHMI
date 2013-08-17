// ReportView.cpp: implementation of the CReportView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"

#include "ActionObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CReportView, CLayoutView)

BEGIN_MESSAGE_MAP(CReportView, CLayoutView)
	//{{AFX_MSG_MAP(CReportView)
	ON_WM_LBUTTONDOWN()
// 	ON_COMMAND(ID_ADD_SUBREPORT, OnAddSubReport)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CLayoutView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CLayoutView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CLayoutView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportView construction/destruction

CReportView::CReportView()
{
	m_pSelectSection = NULL;

//	HDC hDCScreen = ::GetDC(NULL);
//	ASSERT(hDCScreen != NULL);
//	int cxPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSX);
//	int cyPixelsPerInch = GetDeviceCaps(hDCScreen, LOGPIXELSY);
//	::ReleaseDC(NULL, hDCScreen);
//
//	m_zoomNum = CSize(cxPixelsPerInch * 254, cyPixelsPerInch * 254); // 0.1mm
//	m_zoomDenom = CSize(10000, 10000);
}

CReportView::~CReportView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CReportView diagnostics

#ifdef _DEBUG
void CReportView::AssertValid() const
{
	CLayoutView::AssertValid();
}

void CReportView::Dump(CDumpContext& dc) const
{
	CLayoutView::Dump(dc);
}

CReportDoc* CReportView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CReportDoc)));
	return (CReportDoc *)m_pDocument;
}
#endif //_DEBUG

BOOL CReportView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CLayoutView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CReportView drawing
void CReportView::OnDraw(CDC* pDC)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CLayoutView::OnDraw(pDC);

	if (/*m_selection.IsEmpty() && */m_pSelectSection != NULL)
	{
		CPen penGrid(PS_SOLID, 0, RGB(128, 128, 128)/*GetSysColor(COLOR_HIGHLIGHT)*/);
		CPen* pOldPen = pDC->SelectObject(&penGrid);
		int uMode = pDC->SetBkMode(TRANSPARENT/*OPAQUE*/);  
		pDC->SetROP2(R2_MERGEPEN);

		CRect rect = pDoc->GetSectionRect(m_pSelectSection).GetRECT();
		DocToClient(rect);

		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.top);
		pDC->LineTo(rect.right, rect.bottom);

		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom);
		pDC->LineTo(rect.right, rect.bottom);
		
		pDC->SetBkMode(uMode);
		pDC->SelectObject(pOldPen);
	}
}

void CReportView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetViewportExt(m_zoomNum);
	pDC->SetWindowExt(m_zoomDenom);
}

void CReportView::Select(CDevObj* pObj, BOOL bAdd, BOOL bUpdate)
{
	CLayoutView::Select(pObj, bAdd, bUpdate);

	if (/*(pObj == NULL && !bAdd) || */!m_selection.IsEmpty())
	{
		SelectSection(NULL);
	}
}

void CReportView::SelectSection(CReportSection* pSection)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CReportSection* pOldSection = m_pSelectSection;
	m_pSelectSection = pSection;
	if (pOldSection != m_pSelectSection)
	{
		CRect rect;
		
		if (pOldSection != NULL)
		{
			rect = pDoc->GetSectionRect(pOldSection).GetRECT();
			DocToClient(rect);
			rect.InflateRect(1, 1);
			InvalidateRect(&rect, FALSE);
		}
		
		if (m_pSelectSection != NULL)
		{	
			rect = pDoc->GetSectionRect(m_pSelectSection).GetRECT();
			DocToClient(rect);
			rect.InflateRect(1, 1);
			InvalidateRect(&rect, FALSE);
		}
		
		if (!m_selection.IsEmpty())
		{
			if (m_pSelectSection != NULL)
				Select(NULL);
		}
		else
		{
			theApp.NotifySelectChanged();
		}

		GetParent()->SendMessage(WM_SELECT_CHANGED,	m_pSelectSection == NULL, 0);
	}
}

void CReportView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint local = point;
	ClientToDoc(local);

	CReportSection* pSelectSection = HitTestSection(local);

	if (CDevTool::c_drawShape != DRAW_SELECTION || 
			!m_selection.IsEmpty() || 
			!DoSpecialLButtonDown(nFlags, point))
		CLayoutView::OnLButtonDown(nFlags, point);

	if (CDevTool::c_drawShape == DRAW_SELECTION)
	{
		if (m_selection.IsEmpty())
			SelectSection(pSelectSection);
	}
}

BOOL CReportView::DoSpecialLButtonDown(UINT nFlags, const CPoint& point)
{
	CReportDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!m_selection.IsEmpty())
		return FALSE;

	CPoint local = point;
	CPoint pointOffset;
	ClientToDoc(local);
	int nHitLine = HitTestSectionLine(local, &pointOffset);

	if (nHitLine == -1)
		return FALSE;
	
	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	// set capture to the window which received this message
	SetCapture();
	ASSERT(this == (CLayoutView *)CWnd::GetCapture());
	UpdateWindow();

	CDC dc;
	CDC* pDrawDC = &dc;
	PrepareRopDC(pDrawDC);
	ASSERT_VALID(pDrawDC);

	CPen penGrid(PS_DOT, 0, RGB(0, 0, 0));
	CPen* pOldPen = pDrawDC->SelectObject(&penGrid);

	CPoint pointSave = point;
	CPoint pointOld = point;
	CPoint pointNew = point;
	CPoint pointTemp;

	BOOL bMouseUp;
	BOOL bMoved = FALSE;

	CSize sizeGrid;
	GetGridSpace(sizeGrid);

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (this != (CLayoutView *)CWnd::GetCapture())
			break;

		switch (msg.message)
		{
		// handle movement/accept messages
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		case WM_KEYDOWN:
			if (msg.message == WM_KEYDOWN)
			{
				// handle cancel messages
				if (msg.wParam == VK_ESCAPE)
				{
					if (bMoved)
					{
						pointTemp = pointOld;
						ClientToDoc(pointTemp);
						AlignToGrid(pointTemp);
						DrawTrackerLine(pDrawDC, nHitLine, pointTemp);
					}
					pointNew = pointSave;
					goto ExitLoop;
				}
				BOOL bChange = TRUE;
				CPoint ptOffset;
				
				switch (msg.wParam)
				{
				case VK_LEFT:
					ptOffset = CPoint(-sizeGrid.cx, 0);
					break;
				case VK_UP:
					ptOffset = CPoint(0, -sizeGrid.cy);
					break;
				case VK_RIGHT:
					ptOffset = CPoint(sizeGrid.cx, 0);
					break;
				case VK_DOWN:
					ptOffset = CPoint(0, sizeGrid.cy);
					break;
				default:
					bChange = FALSE;
					break;
				}
				if (bChange == FALSE)
					break;
				ClientToDoc(pointNew);
				pointNew.Offset(ptOffset);
				DocToClient(pointNew);

				pointTemp = pointNew;
				ClientToScreen(&pointTemp);
				SetCursorPos(pointTemp.x, pointTemp.y);
			}
			else
			{
				pointNew = CPoint((int)(short)LOWORD(msg.lParam), (int)(short)HIWORD(msg.lParam));
				CPoint pt = pointNew;
				ClientToDoc(pt);
				AlignToGrid(pt);
				((CMainFrame *)(theApp.m_pMainWnd))->OnViewMousePosChange(pt);
			}
			
			bMouseUp = (msg.message == WM_LBUTTONUP);
			if (pointNew != pointOld || bMouseUp)
			{
				if (bMoved)
				{
					pointTemp = pointOld;
					ClientToDoc(pointTemp);
					AlignToGrid(pointTemp);
					DrawTrackerLine(pDrawDC, nHitLine, pointTemp);
				}
				if (msg.message != WM_LBUTTONUP)
					bMoved = TRUE;
			}
			if (bMouseUp)
			{
				pointOld = pointNew;
				goto ExitLoop;
			}

			if (pointNew != pointOld)
			{
				pointOld = pointNew;
				pointTemp = pointOld;
				ClientToDoc(pointTemp);
				AlignToGrid(pointTemp);
//				pointTemp -= pointOffset;
				DrawTrackerLine(pDrawDC, nHitLine, pointTemp);
			}
			break;

		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				pointTemp = pointOld;
				ClientToDoc(pointTemp);
				AlignToGrid(pointTemp);
//				pointTemp -= pointOffset;
				DrawTrackerLine(pDrawDC, nHitLine, pointTemp);
			}
			pointNew = pointSave;
			goto ExitLoop;

		// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	pDrawDC->SelectObject(pOldPen);

	ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	if (pointSave != pointNew)
	{
		ClientToDoc(pointNew);
		AlignToGrid(pointNew);

		CSizeF szOld;
		szOld.cx = pDoc->GetWidth();
		szOld.cy = pDoc->GetHeight();
		pointNew = MoveSectionLineTo(nHitLine, pointNew);
	
		theApp.NotifySelectChanged();
		pDoc->SetModifiedFlag();

		if (nHitLine == 0 && pointNew.x != pointSave.x)
		{
			CSizeF szNew;
			szNew.cx = pDoc->GetWidth();
			szNew.cy = pDoc->GetHeight();
			CActionDocSize* pAct = new CActionDocSize(pDoc, szNew, szOld);
			
			pDoc->m_stackUndo.Push(pAct);
		}
		else if (pointNew.y != pointSave.y)
		{
			CActionSectionLineMove* pAct 
					= new CActionSectionLineMove(pDoc, nHitLine, pointNew, pointSave);

			int nOffset = pointNew.y - pointSave.y;
			CDevObjList* pObjList = pDoc->GetObjects();
			POSITION posObj = pObjList->GetHeadPosition();
			while (posObj != NULL)
			{
				CDevObj* pObj = (CDevObj *)pObjList->GetNext(posObj);
				
				CRectF rcPos = pObj->GetPositionRect();
				if (rcPos.top >= pointSave.y)
				{
					pAct->AddObj(pObj);

					rcPos.OffsetRect(0, float(nOffset));
					pObj->MoveTo(rcPos, FALSE, this);
				}
			}

			pDoc->m_stackUndo.Push(pAct);
		}
	}

	return TRUE;
}

BOOL CReportView::DoSpecialMouseMove(UINT nFlags, const CPoint& point)
{
	if (!m_selection.IsEmpty())
		return FALSE;

	CPoint local = point;
	ClientToDoc(local);

	int nHitLine = HitTestSectionLine(local);
	if (nHitLine != -1)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(nHitLine == 0 ? IDC_SIZEWE : IDC_SIZENS));
		return TRUE;
	}

	return FALSE;
}

BOOL CReportView::HitTestLine(CPoint point1, CPoint point2, CPoint point, CPoint* pOffset)
{
	if (point1.x == point2.x)
	{
		if (((point.y >= point1.y &&  point.y <= point2.y) || 
			(point.y <= point1.y &&  point.y >= point2.y)) &&
			abs(point.x - point1.x) < 2)
		{
			if (pOffset != NULL)
			{
				pOffset->x = point.x - point1.x;
				pOffset->y = 0;
			}
			return TRUE;
		}
	}
	
	if (point1.y == point2.y)
	{
		if (((point.x >= point1.x &&  point.x <= point2.x) || 
			(point.x <= point1.x &&  point.x >= point2.x)) &&
			abs(point.y - point1.y) < 2)
		{
			if (pOffset != NULL)
			{
				pOffset->x = 0;
				pOffset->y = point.y - point1.y;
			}
			
			return TRUE;
		}
	}

	return FALSE;
}

CReportSection* CReportView::HitTestSection(CPoint point)
{
	CReportDoc* pDoc = GetDocument();
	
	CRect rect(0, 0, ROUND(pDoc->GetWidth()), ROUND(pDoc->GetHeight()));
		
	if (pDoc->m_secReportHeader.m_bVisible)
	{
		rect.bottom = int(rect.top + pDoc->m_secReportHeader.m_fHeight);
		
		if (rect.PtInRect(point))
			return &pDoc->m_secReportHeader;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageHeader.m_bVisible)
	{
		rect.bottom = int(rect.top + pDoc->m_secPageHeader.m_fHeight);
		
		if (rect.PtInRect(point))
			return &pDoc->m_secPageHeader;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secDetail.m_bVisible)
	{
		rect.bottom = int(rect.top + pDoc->m_secDetail.m_fHeight);
		
		if (rect.PtInRect(point))
			return &pDoc->m_secDetail;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageFooter.m_bVisible)
	{
		rect.bottom = int(rect.top + pDoc->m_secPageFooter.m_fHeight);
		
		if (rect.PtInRect(point))
			return &pDoc->m_secPageFooter;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secReportFooter.m_bVisible)
	{
		rect.bottom = int(rect.top + pDoc->m_secReportFooter.m_fHeight);
		
		if (rect.PtInRect(point))
			return &pDoc->m_secReportFooter;
	}
	
	return NULL;
}

int CReportView::HitTestSectionLine(CPoint point, CPoint* pOffset)
{
	CReportDoc* pDoc = GetDocument();
	
	int nHitLine = -1;

	CRect rect(0, 0, ROUND(pDoc->GetWidth()), ROUND(pDoc->GetHeight()));

	if (HitTestLine(CPoint(rect.right, rect.top), CPoint(rect.right, rect.bottom), point, pOffset))
		nHitLine = 0;
	
	if (pDoc->m_secReportHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportHeader.m_fHeight);
			
		if (HitTestLine(CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), point, pOffset))
			nHitLine = 1;
	
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageHeader.m_fHeight);
		
		if (HitTestLine(CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), point, pOffset))
			nHitLine = 2;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secDetail.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secDetail.m_fHeight);
		
		if (HitTestLine(CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), point, pOffset))
			nHitLine = 3;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageFooter.m_fHeight);
		
		if (HitTestLine(CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), point, pOffset))
			nHitLine = 4;
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secReportFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportFooter.m_fHeight);
		
		if (HitTestLine(CPoint(rect.left, rect.bottom), CPoint(rect.right, rect.bottom), point, pOffset))
			nHitLine = 5;
	}

	return nHitLine;
}

void CReportView::DrawTrackerLine(CDC* pDC, int nLine, CPoint point)
{
	CReportDoc* pDoc = GetDocument();
	
	CRect rect(0, 0, ROUND(pDoc->GetWidth()), ROUND(pDoc->GetHeight()));

	if (nLine == 0)
	{
		pDC->MoveTo(max(point.x, rect.left), rect.top);
		pDC->LineTo(max(point.x, rect.left), rect.bottom);
	
		return;
	}

	if (pDoc->m_secReportHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportHeader.m_fHeight);

		if (nLine == 1)
		{
			pDC->MoveTo(rect.left, max(point.y, rect.top));
			pDC->LineTo(rect.right, max(point.y, rect.top));
		
			return;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageHeader.m_fHeight);
		
		if (nLine == 2)
		{
			pDC->MoveTo(rect.left, max(point.y, rect.top));
			pDC->LineTo(rect.right, max(point.y, rect.top));

			return;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secDetail.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secDetail.m_fHeight);
		
		if (nLine == 3)
		{
			pDC->MoveTo(rect.left, max(point.y, rect.top));
			pDC->LineTo(rect.right, max(point.y, rect.top));
			return;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageFooter.m_fHeight);
		
		if (nLine == 4)
		{
			pDC->MoveTo(rect.left, max(point.y, rect.top));
			pDC->LineTo(rect.right, max(point.y, rect.top));
			return;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secReportFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportFooter.m_fHeight);

		if (nLine == 5)
		{
			pDC->MoveTo(rect.left, max(point.y, rect.top));
			pDC->LineTo(rect.right, max(point.y, rect.top));
			return;
		}
	}
}

CPoint CReportView::MoveSectionLineTo(int nLine, CPoint point)
{
	CReportDoc* pDoc = GetDocument();

	CPoint pointMove = point;
	CRect rect(0, 0, ROUND(pDoc->GetWidth()), ROUND(pDoc->GetHeight()));

	if (nLine == 0)
	{
		pointMove.x = max(point.x, rect.left);
		// 改变了宽度
		pDoc->SetWidth(float(pointMove.x -  rect.left));
		
		return pointMove;
	}

	if (pDoc->m_secReportHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportHeader.m_fHeight);
		
		if (nLine == 1)
		{
			pointMove.y = max(point.y, rect.top);
			pDoc->m_secReportHeader.m_fHeight = float(pointMove.y - rect.top);
			pDoc->ReCalcReportHeight(TRUE);

			return pointMove;
		}
		
		rect.top = rect.bottom;
	}

	if (pDoc->m_secPageHeader.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageHeader.m_fHeight);
		
		if (nLine == 2)
		{
			pointMove.y = max(point.y, rect.top);
			pDoc->m_secPageHeader.m_fHeight = float(pointMove.y - rect.top);
			pDoc->ReCalcReportHeight(TRUE);
			
			return pointMove;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secDetail.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secDetail.m_fHeight);
		
		if (nLine == 3)
		{
			pointMove.y = max(point.y, rect.top);
			pDoc->m_secDetail.m_fHeight = float(pointMove.y - rect.top);
			pDoc->ReCalcReportHeight(TRUE);
			
			return pointMove;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secPageFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secPageFooter.m_fHeight);
		
		if (nLine == 4)
		{
			pointMove.y = max(point.y, rect.top);
			pDoc->m_secPageFooter.m_fHeight = float(pointMove.y - rect.top);
			pDoc->ReCalcReportHeight(TRUE);
			
			return pointMove;
		}
		
		rect.top = rect.bottom;
	}
	
	if (pDoc->m_secReportFooter.m_bVisible)
	{
		rect.bottom = ROUND(rect.top + pDoc->m_secReportFooter.m_fHeight);

		if (nLine == 5)
		{
			pointMove.y = max(point.y, rect.top);
			pDoc->m_secReportFooter.m_fHeight = float(pointMove.y - rect.top);
			pDoc->ReCalcReportHeight(TRUE);
			
			return pointMove;
		}
	}

	ASSERT(FALSE);

	return pointMove;
}

void CReportView::OnAddSubReport()
{
	CReportDoc* pDoc = GetDocument();
	
	CRectF position;
	if (m_pSelectSection != NULL)
		position = pDoc->GetSectionRect(m_pSelectSection);
	else
		position = pDoc->GetSectionRect(&pDoc->m_secDetail);

	CReportObj* pObj = new CReportObj(pDoc);
	pObj->CreateOrLoad(NULL, IID_NULL, NULL, &position);
	
	GetDocument()->OnFormObjChange();
	
	CAction* pAct = new CActionObjAdd("插入子报表", pDoc, pObj, TRUE);
	pDoc->m_stackUndo.Push(pAct);
	
	pDoc->SetModifiedFlag();
	
	Select(pObj);
	
	OnEditSubform();
}

