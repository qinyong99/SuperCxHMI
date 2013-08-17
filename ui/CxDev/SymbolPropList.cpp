// TabTableList.cpp : implementation file
//

#include "stdafx.h"
#include "DevInc.h"
#include "SymbolPropList.h"

#include "SymbolPropDlg.h"
#include "SymbolDesignDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropList

CSymbolPropList::CSymbolPropList()
{
// 	m_pInplaceCtrl = NULL;

	m_nSelectColumn = 1;
	m_nEditItem = -1;
	m_nEditColumn = -1;
}

CSymbolPropList::~CSymbolPropList()
{
}


BEGIN_MESSAGE_MAP(CSymbolPropList, CListCtrl)
	//{{AFX_MSG_MAP(CSymbolPropList)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_MESSAGE_VOID(WM_INPLACEEDIT_KILLFOCUS, OnKillFocusEdit)
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolPropList message handlers

void CSymbolPropList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rcItem = lpDrawItemStruct->rcItem;
	int nItemID = lpDrawItemStruct->itemID;
	BOOL bSelected = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	CSymbolProp* pItem = GetPropertyItem(nItemID);
	
	switch (lpDrawItemStruct->itemAction)
	{
		case ODA_SELECT:
		case ODA_DRAWENTIRE:
		{			
			CPen* pOldPen = dc.SelectObject(&m_penGrid);

			dc.FillSolidRect(&rcItem, GetSysColor(COLOR_WINDOW));
			
			CRect rcT = rcItem;
			rcT.right = rcT.left + GetColumnWidth(0);
			CRect r = rcT;

			dc.DrawEdge(&r, bSelected ? BDR_RAISEDINNER : EDGE_RAISED, BF_RECT|BF_ADJUST);
			dc.FillSolidRect(&r, GetSysColor(COLOR_3DFACE));
			
			CFont* pOldFont = dc.SelectObject(bSelected ? &m_fontBold : &m_font);
		
			CString strText;
			strText.Format("%d", nItemID + 1);
			dc.DrawText(strText, -1, &r, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			dc.SelectObject(&m_font);

			// 名称
			rcT.left = rcT.right;
			rcT.right += GetColumnWidth(1);
			r = rcT;

			strText = pItem->m_strName;
			r.left += 4;
			dc.DrawText(strText, r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		
			// 类型
			rcT.left = rcT.right;
			rcT.right += GetColumnWidth(2);
			r = rcT;

			strText = CTypeChanger::GetExTypeName(pItem->m_proptype);
			r.left += 4;				                                                                                                                                                                                                                                                                                                                                                                                                    	rcT.left += 4;
			dc.DrawText(strText, r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			// 描述
			rcT.left = rcT.right;
			rcT.right += GetColumnWidth(3);
			r = rcT;

			strText = pItem->m_strDescription;
			r.left += 4;
			dc.DrawText(strText, r, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

			if (bSelected)
			{
				GetSubItemRect(nItemID, m_nSelectColumn, LVIR_BOUNDS, rcT);
				dc.FrameRect(&rcT, CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH)));

				if (m_nSelectColumn != m_nEditColumn)
				{
					rcT.InflateRect(-1, -1, -1, -1);
					dc.FrameRect(&rcT, CBrush::FromHandle((HBRUSH)::GetStockObject(BLACK_BRUSH)));
				}
			}

			dc.SelectObject(pOldFont);
			dc.SelectObject(pOldPen);
		}
		break;
	}

	
	dc.Detach();
}


int CSymbolPropList::GetPropertyItemCount()
{
	return GetItemCount();
}

BOOL CSymbolPropList::AddPropertyItem(CSymbolProp* pProp, BOOL bInsert)
{
	int nIndex = -1;
	if (bInsert)
		nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	
	if (nIndex == -1)
		nIndex = GetItemCount();

	nIndex = InsertItem(nIndex, " ", 0);
	SetItemData(nIndex, DWORD(pProp));

	if (bInsert && GetNextItem(-1, LVNI_ALL | LVNI_SELECTED) == -1)
	{
		SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		((CSymbolDesignDlg *)GetParent())->OnSelchangedListProp(FALSE);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////
//功能:删除当前行
/////////////////////////////////////////////////////////////
void CSymbolPropList::DeleteSelectPropertyItem()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex != -1)
	{
		CSymbolProp* pItem = GetPropertyItem(nIndex);
		delete pItem;
		
		DeleteItem(nIndex);
	}

	if (GetItemCount() > 0)
	{
		if (GetItemCount() <= nIndex)
			nIndex--;

		BOOL bRet = SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		if (bRet)
			bRet = EnsureVisible(nIndex, FALSE);
	}

	if (nIndex < GetItemCount())
		RedrawItems(nIndex, GetItemCount() - 1);

	((CSymbolDesignDlg *)GetParent())->OnSelchangedListProp(FALSE);
}

/////////////////////////////////////////////////////////////
//功能:返回当前选中的项的对应的编号,<0为无选项
/////////////////////////////////////////////////////////////
int CSymbolPropList::GetSelectPropertyItem()
{
	return GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
}
///////////////////////////////////////////////////////////////////
//功能:容
//参数:nHandle[in]		需要的项的编号
//返回:成功CSymbolProp指针,失败NULL
///////////////////////////////////////////////////////////////////

CSymbolProp* CSymbolPropList::GetPropertyItem(int iIndex)
{
	if (iIndex < 0 || iIndex >= GetItemCount())
		return NULL;

	return (CSymbolProp *)GetItemData(iIndex);
}

///////////////////////////////////////////////////////////////
//功能:编辑当前属性的内体内容
///////////////////////////////////////////////////////////////
void CSymbolPropList::EditSelectPropertyItem()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex == -1)
		return;

	CSymbolProp* pItem = GetPropertyItem(nIndex);

	if (pItem)
	{
		CSymbolPropDlg dlg(pItem);
		dlg.m_pObject = ((CSymbolDesignDlg *)GetParent())->m_pObject;
		dlg.m_pPropList = this;
		enumPropType typeOld = pItem->m_proptype;
		_variant_t valueOld = pItem->m_value;
		if (dlg.DoModal() == IDOK)
		{
			if (pItem->m_proptype != typeOld)
				OnChangeTagType();
					
			if (pItem->m_value != valueOld)
				((CSymbolDesignDlg *)GetParent())->SyncPropAndAdvance();

			RedrawItems(nIndex, nIndex);
		}
	}
}

////////////////////////////////////////////////////////////
//功能:返回当前选中项的数据类型
//返回:数据类型枚举值,如没有选项或选项没设置类型,则返回EXSPROP_NULL
///////////////////////////////////////////////////////////
enumPropType CSymbolPropList::GetSelectItemType()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex != -1)
	{
		CSymbolProp* pItem = GetPropertyItem(nIndex);
		if (pItem != NULL)
			return pItem->m_proptype;
	}
	
	return PROPTYPE_UNKNOWN;
}

//////////////////////////////////////////////////////////
//功能:创建新的唯一扩展变量名
//参数:pnNum[out]		当前名所用的后缀
///////////////////////////////////////////////////////////
CString CSymbolPropList::MakeUniqueTagName(int* pnNum)
{
	int nTry = 0;
	LPTSTR pHeader = _T("prop");
	CString strTry;
	do
	{
		strTry.Format("%s%d", pHeader, ++nTry);
	} while (IsNameDuplicate(strTry));

	if (pnNum != NULL)
		*pnNum = nTry;

	return strTry;
}

//////////////////////////////////////////////////////////
//功能:判定指定名是否和某个已存在的扩展变量名相重
//////////////////////////////////////////////////////////
BOOL CSymbolPropList::IsNameDuplicate(CString& strNew)
{
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp* pItem = GetPropertyItem(i);
		if (strNew == pItem->m_strName)
			return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////
//功能:扩展属性的类型改变扣的通知函数
/////////////////////////////////////////////////////////
void CSymbolPropList::OnChangeTagType()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nIndex == -1)
		return;

	CSymbolProp* pItem = GetPropertyItem(nIndex);
	pItem->m_arrObjNum.RemoveAll();
	((CSymbolDesignDlg *)GetParent())->OnChangeTagType();
}

void CSymbolPropList::OnDestroy() 
{
	int nCount = GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp* pItem = GetPropertyItem(i);
		delete pItem;
	}

	CListCtrl::OnDestroy();
}

ICxEnumExtendProperty* CSymbolPropList::EnumItem()
{
	int nCount = GetItemCount();

	CProp* pProps = new CProp[nCount];
	if (pProps == NULL)
		AfxThrowMemoryException();

	for (int i = 0; i < nCount; i++)
	{
		pProps[i] = *GetPropertyItem(i);
	}

	CEnumExtendProp* pEnum = new CEnumExtendProp(pProps, nCount);


	return (ICxEnumExtendProperty *)&pEnum->m_xEnumProp;
}

void CSymbolPropList::Init() 
{
	m_font.CreateStockObject(DEFAULT_GUI_FONT);
	
	LOGFONT lf;
	m_font.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);
	
	m_penGrid.CreatePen(PS_SOLID, 1, RGB(196, 196, 196));

	InsertColumn(0, _T(""), LVCFMT_LEFT, 20, 0);
	InsertColumn(1, _T("属性"), LVCFMT_LEFT, 80, 1);
	InsertColumn(2, _T("类型"), LVCFMT_LEFT, 44, 2);
	InsertColumn(3, _T("描述"), LVCFMT_LEFT, 105, 3);

	CRect rect(0, 0, 0, 0);
	m_editInplace.Create(WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_NOHIDESEL, rect, this, 101);
	m_editInplace.SetFont(&m_font);
}

void CSymbolPropList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LVHITTESTINFO lvh;
	lvh.pt = point;

	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	
	if (SubItemHitTest(&lvh) != -1)
	{
		if (m_nSelectColumn == lvh.iSubItem)
		{
			if (nIndex == lvh.iItem)
			{
				if (OnEditCell(lvh.iItem, lvh.iSubItem))
					return;
			}
		}
		else
		{
			m_nSelectColumn = lvh.iSubItem;
			if (nIndex == lvh.iItem)
				RedrawItems(lvh.iItem, lvh.iItem);
		}
	}
	
	CListCtrl::OnLButtonDown(nFlags, point);

	if (nIndex != GetNextItem(-1, LVNI_ALL | LVNI_SELECTED))
		((CSymbolDesignDlg *)GetParent())->OnSelchangedListProp(TRUE);
}

void CSymbolPropList::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}

BOOL CSymbolPropList::OnEditCell(int nItem, int nColumn)
{
	if (nColumn == 1 || nColumn == 3)
	{
		CSymbolProp* pItem = GetPropertyItem(nItem);
		m_editInplace.SetWindowText(nColumn == 1 ? pItem->m_strName : pItem->m_strDescription);
		
		CRect rect;
		GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
		
		m_editInplace.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, 
			rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_editInplace.SetSel(0, -1);
		m_editInplace.SetFocus();

		m_nEditItem = nItem;
		m_nEditColumn = nColumn;
	}
	else if (nColumn == 2)
	{
		CPopupList list;
		list.Create(this, &m_font);

		int nCurrent = -1;
		CSymbolProp* pItem = GetPropertyItem(nItem);
		int nCount = CTypeChanger::GetExTypeCount();
		for (int i = 0; i < nCount; i++)
		{
			enumPropType nType = CTypeChanger::GetExTypeID(i);
			list.AddString(CTypeChanger::GetExTypeName(nType));
			if (nType == pItem->m_proptype)
				nCurrent = i;
		}
			
		CRect rect;
		GetSubItemRect(nItem, nColumn, LVIR_BOUNDS, rect);
		POINT ptTemp = {rect.left, rect.bottom};	
		ClientToScreen(&ptTemp);

		m_nEditItem = nItem;
		m_nEditColumn = nColumn;
		RedrawItems(nItem, nItem);
		
		int nSelect = list.Show(ptTemp.x, ptTemp.y, rect.right - rect.left, nCurrent);
		if (nSelect != -1 && nCurrent != nSelect)
		{
			CSymbolProp* pItem = GetPropertyItem(nItem);
			pItem->m_proptype = CTypeChanger::GetExTypeID(nSelect);
			RedrawItems(nItem, nItem);
			OnChangeTagType();
		}

		m_nEditItem = -1;
		m_nEditColumn = -1;
	}

	return TRUE;
}

void CSymbolPropList::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) 
	{
	case VK_TAB:
		PostMessage(WM_KEYDOWN, VK_TAB, 0);
		PostMessage(WM_KEYUP, VK_TAB, 0);
		break;
	case VK_LEFT:
	case VK_RIGHT:
		{
			int nItem = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
			if (nItem != -1)
			{
				if (nChar == VK_LEFT)
				{
					if (m_nSelectColumn > 1)
						m_nSelectColumn--;
				}
				else if (nChar == VK_RIGHT)
				{
					if (m_nSelectColumn < GetHeaderCtrl()->GetItemCount() - 1)
						m_nSelectColumn++;
				}
			}
			
			RedrawItems(nItem, nItem);
		}
		break;

	case VK_RETURN:
		OnEditCell(GetNextItem(-1, LVNI_ALL | LVNI_SELECTED), m_nSelectColumn);
		break;

	default:
		{
			int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

			CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
			
			if (nIndex != GetNextItem(-1, LVNI_ALL | LVNI_SELECTED))
				((CSymbolDesignDlg *)GetParent())->OnSelchangedListProp(TRUE);
		}

		break;
	}
}

void CSymbolPropList::OnKillFocusEdit()
{
	if (m_nEditItem != -1 && !m_editInplace.m_bCacel)
	{
		CString str;
		m_editInplace.GetWindowText(str);
		
		CSymbolProp* pItem = GetPropertyItem(m_nEditItem);
		if (m_nEditColumn == 1)
			pItem->m_strName = str;
		else
			pItem->m_strDescription = str;
	}
	
	m_nEditItem = -1;
	m_nEditColumn = -1;
	
	m_editInplace.m_bCacel = FALSE;
	m_editInplace.SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOSIZE);
}

UINT CSymbolPropList::OnGetDlgCode() 
{
	return DLGC_WANTTAB |  DLGC_WANTARROWS | DLGC_WANTALLKEYS;
}
