// Lookup.cpp: implementation of the CLookup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxDynObjs.h"
#include "LookupPage.h"
#include "PopupList.h"
#include "CxLookupItem.h"

#include "../CxComObj/CxComObj.h"
#include "../CxComObj/CxComObj_i.c"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CLookupItem::CLookupItem(CLookupPage* pLookupPage) :
	m_pLookupPage(pLookupPage)
{

}

CLookupItem::~CLookupItem()
{

}

void CLookupItem::DrawEllipsisButton(CDCHandle dc, CRect rect)
{
	dc.DrawFrameControl(&rect, DFC_BUTTON, DFCS_BUTTONPUSH);
	
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNTEXT));
	HPEN hOldPen = dc.SelectPen(pen);

	RECT rcT;
	rcT.left = (rect.left + rect.right) / 2 - 1;
	rcT.right = rcT.left + 2;
	rcT.top = (rect.top + rect.bottom) / 2 + 2;
	rcT.bottom = rcT.top + 2;
	
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	OffsetRect(&rcT, -4, 0);
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	OffsetRect(&rcT, 8, 0);
	dc.Rectangle(rcT.left, rcT.top, rcT.right, rcT.bottom);
	
	dc.SelectPen(hOldPen);
}

//获取字串
HRESULT CLookupItem::HelperGetText(CComVariant var, CComBSTR& bstr)
{
	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
		
	if (var.vt != VT_BSTR)
		return S_FALSE;

	bstr = var.bstrVal;
	
	return S_OK;
}

HRESULT CLookupItem::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	return S_OK;
}

HRESULT CLookupItem::OnClickOutput(RECT, POINT pt)
{
	return S_OK;
}

HRESULT CLookupItem::OnDblClickOutput(RECT rect, POINT pt)
{
	return OnClickOutput(rect, pt);
}

//////////////////////////////////////////////////////////////////////
// CLookupItemColor Class
//////////////////////////////////////////////////////////////////////
CLookupItemColor::CLookupItemColor(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{

}

CLookupItemColor::~CLookupItemColor()
{

}

HRESULT CLookupItemColor::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	HPEN hOldPen = dc.SelectPen(m_pLookupPage->m_penGrid);

	COLORREF clrRect = 0;
	OleTranslateColor(m_vaOutput.lVal, 0, &clrRect);
	CBrush brush;
	brush.CreateSolidBrush(clrRect);
	HBRUSH hOldBrush = dc.SelectBrush(brush);

	rect.InflateRect(-2, -2);
	dc.Rectangle(&rect);
	
	dc.SelectBrush(hOldBrush);
	dc.SelectPen(hOldPen);

	return S_OK;
}

//派生类重载基类函数实现处理单击第3列的事件
HRESULT CLookupItemColor::OnClickOutput(RECT rect, POINT pt)
{
	CComPtr<ICxColorPicker2> p;		
	HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
	if (FAILED(hr)) 
		return hr;

	OLE_COLOR clr;
	m_pLookupPage->m_list.ClientToScreen(&rect);
	OLE_COLOR clrDefault = m_pLookupPage->m_pPropInfo->varCurrentValue.lVal;
	hr = p->Pick((long)m_pLookupPage->m_list.m_hWnd, rect, clrDefault, clr, &clr);
	if (FAILED(hr)) 
		return hr;
	
	if (hr == S_OK)
	{
		m_vaOutput.vt = VT_I4;
		m_vaOutput.lVal = clr;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemEnum Class
//////////////////////////////////////////////////////////////////////

CLookupItemEnum::CLookupItemEnum(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
	m_vaOutput = 0;
}

CLookupItemEnum::~CLookupItemEnum()
{

}

//派生类重载基类函数实现画第3列
HRESULT CLookupItemEnum::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	int nCurVal = -1;

	CEnumPropItemArray& arrEnumPropItems = m_pLookupPage->m_pPropInfo->m_arrEnumPropItems;
	for (int i = 0; i < arrEnumPropItems.GetSize(); i++)
	{
		if (arrEnumPropItems[i].m_varValue == m_vaOutput)
			nCurVal = i;
	}

	CString strText;
	if (nCurVal != -1)
	{
		strText = m_pLookupPage->m_pPropInfo->m_arrEnumPropItems[nCurVal].m_bstrDesc;
	}
	else
	{
		if (m_vaOutput.vt == VT_BSTR)
			strText = m_vaOutput.bstrVal;
		else
			strText = "**error**";
	}

	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	rcT.InflateRect(-4, 0);

	dc.DrawText(strText, -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}

	return S_OK;
}

HRESULT CLookupItemEnum::OnClickOutput(RECT rect, POINT pt)
{
	USES_CONVERSION;
	
	if (pt.x < rect.right - (rect.bottom - rect.top) - 1)
		return S_FALSE;
		
	CPopupList list;
	list.Create(m_pLookupPage->m_list, m_pLookupPage->m_font);

	CEnumPropItemArray& arrEnumPropItems = m_pLookupPage->m_pPropInfo->m_arrEnumPropItems;
	for (int i = 0; i < arrEnumPropItems.GetSize(); i++)
		list.AddString(OLE2T(arrEnumPropItems[i].m_bstrDesc));

	POINT ptTemp = {rect.left, rect.bottom};	
	ClientToScreen(m_pLookupPage->m_list, &ptTemp);

	int nSelect = list.Show(ptTemp.x, ptTemp.y, rect.right - rect.left, m_vaOutput.iVal);
	if (nSelect != -1)
		m_vaOutput = arrEnumPropItems[nSelect].m_varValue;

	return S_OK;
}

HRESULT CLookupItemEnum::OnDblClickOutput(RECT, POINT pt)
{
	int nCurValue = -1;

	CEnumPropItemArray& arrEnumPropItems = m_pLookupPage->m_pPropInfo->m_arrEnumPropItems;
	for (int i = 0; i < arrEnumPropItems.GetSize(); i++)
	{
		if (arrEnumPropItems[i].m_varValue == m_vaOutput)
		{
			nCurValue = i;
			break;
		}
	}

	if (nCurValue == -1) 
		return S_FALSE; 
	
	if (++nCurValue == arrEnumPropItems.GetSize())
		nCurValue = 0;
	
	m_vaOutput = arrEnumPropItems[nCurValue].m_varValue;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemEnumString Class
//////////////////////////////////////////////////////////////////////

CLookupItemEnumString::CLookupItemEnumString(CLookupPage* pLookupPage) :
	CLookupItemEnum(pLookupPage)
{
}

CLookupItemEnumString::~CLookupItemEnumString()
{
	
}

HRESULT CLookupItemEnumString::OnClickOutput(RECT rect, POINT pt)
{
	USES_CONVERSION;

	if (!m_pLookupPage->m_editInplace.IsWindowVisible())
	{
		CComBSTR bstr;
		HelperGetText(m_vaOutput, bstr);
		m_pLookupPage->m_editInplace.SetWindowText(OLE2T(bstr));
		
		CRect rcT = rect;
		rcT.right -= rcT.Height();
		m_pLookupPage->m_editInplace.SetWindowPos(NULL, rcT.left, rcT.top, rcT.right - rcT.left, 
			rcT.bottom - rcT.top, SWP_SHOWWINDOW);
		m_pLookupPage->m_editInplace.SetFocus();
	}

	m_pLookupPage->m_editInplace.SetSelAll();

	if (pt.x < rect.right - (rect.bottom - rect.top) - 1)
		return S_FALSE;
	
	CPopupList list;
	list.Create(m_pLookupPage->m_list, m_pLookupPage->m_font);
	
	CEnumPropItemArray& arrEnumPropItems = m_pLookupPage->m_pPropInfo->m_arrEnumPropItems;
	for (int i = 0; i < arrEnumPropItems.GetSize(); i++)
		list.AddString(OLE2T(arrEnumPropItems[i].m_bstrDesc));
	
	POINT ptTemp = {rect.left, rect.bottom};	
	ClientToScreen(m_pLookupPage->m_list, &ptTemp);
	
	int nSelect = list.Show(ptTemp.x, ptTemp.y, rect.right - rect.left, m_vaOutput.iVal, &m_pLookupPage->m_editInplace);
	
	if (nSelect != -1)
	{
		m_pLookupPage->m_nEditItem = -1;
		m_pLookupPage->m_list.SetFocus();

		m_vaOutput = arrEnumPropItems[nSelect].m_varValue;
	}

	m_pLookupPage->m_list.SetFocus();
	m_pLookupPage->m_list.Invalidate(); // 加入此句，避免动画专家不能正常刷新
	
	return S_OK;
}

HRESULT CLookupItemEnumString::OnDblClickOutput(RECT rect, POINT pt)
{
	USES_CONVERSION;

	CComBSTR bstr;
	HelperGetText(m_vaOutput, bstr);
	m_pLookupPage->m_editInplace.SetWindowText(OLE2T(bstr));
	
	CRect rcT = rect;
	rcT.right -= rcT.Height();
	m_pLookupPage->m_editInplace.SetWindowPos(NULL, rcT.left, rcT.top, rcT.right - rcT.left, 
		rcT.bottom - rcT.top, SWP_SHOWWINDOW);
	m_pLookupPage->m_editInplace.SetSelAll();
	m_pLookupPage->m_editInplace.SetFocus();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemEdit Class
//////////////////////////////////////////////////////////////////////

CLookupItemEdit::CLookupItemEdit(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemEdit::~CLookupItemEdit()
{

}

HRESULT CLookupItemEdit::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;

	CComBSTR bstr;
	HelperGetText(m_vaOutput, bstr);
	rect.InflateRect(-4, 0);
	dc.DrawText(W2T(bstr), -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	return S_OK;
}

HRESULT CLookupItemEdit::OnClickOutput(RECT rect, POINT pt)
{
	USES_CONVERSION;

	CComBSTR bstr;
	HelperGetText(m_vaOutput, bstr);
	m_pLookupPage->m_editInplace.SetWindowText(OLE2T(bstr));

	m_pLookupPage->m_editInplace.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, 
		rect.bottom - rect.top, SWP_SHOWWINDOW);
	m_pLookupPage->m_editInplace.SetSelAll();
	m_pLookupPage->m_editInplace.SetFocus();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemBool Class
//////////////////////////////////////////////////////////////////////

CLookupItemBool::CLookupItemBool(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
	m_vaOutput = TRUE;
}

CLookupItemBool::~CLookupItemBool()
{

}

//派生类重载基类函数实现画第3列
HRESULT CLookupItemBool::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	rcT.InflateRect(-4, 0);

	CString strText = m_vaOutput.boolVal ? _T("True") : _T("False");
	dc.DrawText(strText, -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		dc.DrawFrameControl(&rcT, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
	}

	return S_OK;
}

HRESULT CLookupItemBool::OnClickOutput(RECT rect, POINT pt)
{
	USES_CONVERSION;

	if (pt.x < rect.right - (rect.bottom - rect.top) - 1)
		return S_FALSE;
	
	CPopupList list;
	list.Create(m_pLookupPage->m_list, m_pLookupPage->m_font);
	
	list.AddString(_T("True"));
	list.AddString(_T("False"));
	
	POINT ptTemp = {rect.left, rect.bottom};	
	ClientToScreen(m_pLookupPage->m_list, &ptTemp);
	int nSelect = list.Show(ptTemp.x, ptTemp.y, rect.right - rect.left, m_vaOutput.boolVal ? 0 : 1);
	if (nSelect != -1)
		m_vaOutput = nSelect == 0;

	return S_OK;
}

HRESULT CLookupItemBool::OnDblClickOutput(RECT, POINT pt)
{
	m_vaOutput = m_vaOutput.boolVal != VARIANT_FALSE ? VARIANT_FALSE : VARIANT_TRUE;
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemFakeFont Class
//////////////////////////////////////////////////////////////////////

CLookupItemFakeFont::CLookupItemFakeFont(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemFakeFont::~CLookupItemFakeFont()
{
}

HRESULT CLookupItemFakeFont::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;

	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	rcT.InflateRect(-4, 0);

	CComVariant var = m_vaOutput;
	var.ChangeType(VT_BSTR);
	dc.DrawText(W2CT(var.bstrVal), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}

	return S_OK;
}

HRESULT CLookupItemFakeFont::OnClickOutput(RECT, POINT pt)
{
	LOGFONT logfont;
	HFONT hFont;

	CHOOSEFONT cf; 
	memset(&cf, 0, sizeof CHOOSEFONT);
	cf.lStructSize = sizeof CHOOSEFONT;
	cf.hInstance = _Module.m_hInst;
	cf.hwndOwner = m_pLookupPage->m_hWnd;
	cf.lpLogFont = &logfont;
	cf.lpTemplateName = "IDD_FONTDIALOG";
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_ENABLETEMPLATE;

	if (((m_vaOutput.vt == VT_DISPATCH) || (m_vaOutput.vt == VT_UNKNOWN)))
	{	
		CComQIPtr<IFont> spFont(m_vaOutput.pdispVal);
		
		if (spFont != NULL)
		{
			spFont->get_hFont(&hFont);
			GetObject(hFont, sizeof(LOGFONT), &logfont); 
			cf.Flags |= CF_INITTOLOGFONTSTRUCT;
		}
	}

	if (ChooseFont(&cf))
	{
		USES_CONVERSION;

		FONTDESC fd;
		fd.cbSizeofstruct = sizeof(FONTDESC);
		fd.lpstrName = T2OLE(logfont.lfFaceName);
		fd.sWeight = (short)logfont.lfWeight;
		fd.sCharset = logfont.lfCharSet;
		fd.fItalic = logfont.lfItalic;
		fd.fUnderline = logfont.lfUnderline;
		fd.fStrikethrough = logfont.lfStrikeOut;

		long lfHeight = logfont.lfHeight;
		if (lfHeight < 0)
			lfHeight = -lfHeight;

		int ppi;
		HDC hdc = m_pLookupPage->GetDC();
		ppi = GetDeviceCaps(hdc, LOGPIXELSY);
		m_pLookupPage->ReleaseDC(hdc);
		
		fd.cySize.Lo = lfHeight * 720000 / ppi;
		fd.cySize.Hi = 0;

		CComPtr<IDispatch> spFont;			
		HRESULT hr = OleCreateFontIndirect(&fd, IID_IDispatch, (void**)&spFont);
		if (spFont != NULL)
			m_vaOutput = spFont;
		return hr;
	}

	return S_FALSE;
}


//////////////////////////////////////////////////////////////////////
// CLookupItemFakeFontName Class
//////////////////////////////////////////////////////////////////////

CLookupItemFakeFontName::CLookupItemFakeFontName(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemFakeFontName::~CLookupItemFakeFontName()
{
}

HRESULT CLookupItemFakeFontName::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;

	CComBSTR bstr;
	HelperGetText(m_vaOutput, bstr);
	rect.InflateRect(-4, 0);
	dc.DrawText(W2T(bstr), -1, &rect, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

	return S_OK;
}

HRESULT CLookupItemFakeFontName::OnClickOutput(RECT rect, POINT pt)
{
	CComVariant var;
	var.Copy(&m_vaOutput);
	var.ChangeType(VT_BSTR);

	CComBSTR bstr = var.bstrVal;
	
	CComPtr<ICxFontCombo> p;		
	HRESULT hr = p.CoCreateInstance(CLSID_CxFontCombo); 
	if (FAILED(hr)) 
		return hr;

	HWND hParentWnd = m_pLookupPage->m_list;
	POINT point = {rect.left, rect.bottom};	
	ClientToScreen(hParentWnd, &point);
	hr = p->PickFont((long)hParentWnd, point.x, point.y, rect.right - rect.left, bstr, &bstr); 				
	if (FAILED(hr)) 
		return hr;

	if (bstr == var.bstrVal)
		return S_FALSE;

	m_vaOutput = bstr;

	return hr;		

}

//////////////////////////////////////////////////////////////////////
// CLookupItemFile Class
//////////////////////////////////////////////////////////////////////

CLookupItemFile::CLookupItemFile(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemFile::~CLookupItemFile()
{
}

HRESULT CLookupItemFile::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;
	
	CComVariant var = m_vaOutput;
	var.ChangeType(VT_BSTR);
	
	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	dc.DrawText(W2CT(var.bstrVal), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}
	
	return S_OK;
}

HRESULT CLookupItemFile::OnClickOutput(RECT, POINT pt)
{
	USES_CONVERSION;

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pLookupPage->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;


	memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 

	ofn.lpstrFilter = 
		_T("所有文件\0*.*\0");
	ofn.lpstrTitle = _T("选择文件");
	
	if (GetOpenFileName(&ofn))
	{
		m_vaOutput = ofn.lpstrFile;		
	}
	
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemBrushFile Class
//////////////////////////////////////////////////////////////////////

CLookupItemBrushFile::CLookupItemBrushFile(CLookupPage* pLookupPage) :
	CLookupItemFile(pLookupPage)
{
}

CLookupItemBrushFile::~CLookupItemBrushFile()
{
}

HRESULT CLookupItemBrushFile::OnClickOutput(RECT, POINT pt)
{
	USES_CONVERSION;
	
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pLookupPage->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 
	
	ofn.lpstrFilter = 
		_T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	ofn.lpstrTitle = _T("选择文件");
	
	if (GetOpenFileName(&ofn))
	{
		m_vaOutput = ofn.lpstrFile;		
	}
	
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemFolder Class
//////////////////////////////////////////////////////////////////////

CLookupItemFolder::CLookupItemFolder(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemFolder::~CLookupItemFolder()
{
}

HRESULT CLookupItemFolder::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;
	
	CComVariant var = m_vaOutput;
	var.ChangeType(VT_BSTR);
	
	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	dc.DrawText(W2CT(var.bstrVal), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}
	
	return S_OK;
}

HRESULT CLookupItemFolder::OnClickOutput(RECT, POINT pt)
{
	USES_CONVERSION;

	BROWSEINFO bi;
	memset(&bi, 0, sizeof BROWSEINFO);
	bi.ulFlags = 0; //BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_EDITBOX
	bi.hwndOwner = m_pLookupPage->m_hWnd;
	bi.pszDisplayName = (TCHAR*)_alloca(MAX_PATH * sizeof TCHAR);
	memset(bi.pszDisplayName, 0, MAX_PATH * sizeof TCHAR);

	CComVariant var;
	var.Copy(&m_vaOutput);
	var.ChangeType(VT_BSTR);
	lstrcpy(bi.pszDisplayName, W2T(var.bstrVal)); 

	LPITEMIDLIST pidl = 0;
	pidl = SHBrowseForFolder(&bi);
	if (pidl)
	{
		SHGetPathFromIDList(pidl, bi.pszDisplayName);
		
		CComPtr<IMalloc> pMalloc;
		SHGetMalloc(&pMalloc);
		if (pMalloc) pMalloc->Free(pidl);

		m_vaOutput = bi.pszDisplayName;	
		return S_OK;		
	}
	
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemFakePicture Class
//////////////////////////////////////////////////////////////////////

CLookupItemFakePicture::CLookupItemFakePicture(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemFakePicture::~CLookupItemFakePicture()
{
}

HRESULT CLookupItemFakePicture::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;

	CComVariant var = m_vaOutput;
	CComBSTR bstr;
	if (var.vt == VT_BSTR)
		bstr = var.bstrVal;

	if (var.vt == VT_DISPATCH || var.vt == VT_UNKNOWN)
	{	
		bstr = L"(空)";
		CComQIPtr<IPicture> pPicture(var.pdispVal);
		if (pPicture.p)
		{
			short type = 0;
			HRESULT hr = pPicture->get_Type(&type);
			if (SUCCEEDED(hr))
				switch (type)
				{
					case PICTYPE_UNINITIALIZED: 
						bstr = L"(未初始化)"; 
						break;
					case PICTYPE_NONE:			 
						break;
					case PICTYPE_BITMAP:			 
						bstr = L"(位图)"; 
						break;
					case PICTYPE_METAFILE:		 
						bstr = L"(元文件)"; 
						break;
					case PICTYPE_ICON:			 
						bstr = L"(图标)"; 
						break;
					case PICTYPE_ENHMETAFILE:	 
						bstr = L"(增强元文件)"; 
						break;
					default:							 
						bstr = L"(未知格式)"; 
						break;			
				}
		}
	}

	if (bstr.Length() == 0)
		bstr = L"(空)";

	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	dc.DrawText(W2CT(bstr), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}

	return S_OK;
}

HRESULT CLookupItemFakePicture::OnClickOutput(RECT, POINT pt)
{
	USES_CONVERSION;

	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof OPENFILENAME);			
	ofn.lStructSize = sizeof OPENFILENAME;
	ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
	ofn.nMaxFile = _MAX_PATH;	
	ofn.hwndOwner = m_pLookupPage->m_hWnd;				
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	
	memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 

	ofn.lpstrFilter = 
		_T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.wmf;*.emf;*.ico;*.cur\0")
		_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
		_T("GIF 图像 (*.gif)\0*.gif\0")
		_T("JPEG 图像 (*.jpg)\0*.jpg\0")
		_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
		_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
		_T("所有文件(*.*)\0*.*\0");
	ofn.lpstrTitle = _T("加载图片");
		
	if (GetOpenFileName(&ofn))
	{
		CComPtr<IDispatch> spPicture;
		HRESULT hr = OleLoadPicturePath(T2OLE(ofn.lpstrFile), 0, 0, 0, IID_IDispatch, (void**)&spPicture);
		if (spPicture != NULL)
			m_vaOutput = spPicture;	
		
		return hr;
	}	
	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemTagGroup Class
//////////////////////////////////////////////////////////////////////

CLookupItemTagGroup::CLookupItemTagGroup(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemTagGroup::~CLookupItemTagGroup()
{
}

HRESULT CLookupItemTagGroup::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;
	
	CComVariant var = m_vaOutput;
	var.ChangeType(VT_BSTR);
	
	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	dc.DrawText(W2CT(var.bstrVal), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}
	
	return S_OK;
}

const CLSID CLSID_CxTagPicker = {0x6CF3993A,0x1D26,0x40BF,{0xB4,0xD8,0x94,0xC4,0xB5,0x4F,0x13,0x90}};

HRESULT CLookupItemTagGroup::OnClickOutput(RECT, POINT pt)
{
	CComPtr<IDispatch> spTagPick;
	HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
	if (FAILED(hr))
		return S_FALSE;

	VARIANTARG* pvars = new VARIANTARG[7];
	for (int i = 0; i < 7; i++)
		VariantInit(&pvars[i]);

	//关联标签组
	CComBSTR bstr;

	CComDispatchDriver dd(m_pLookupPage->m_pPropInfo->spUnkObject);
	CComVariant vaAssocTagGroup;
	hr = dd.GetPropertyByName(L"AssocTagGroup", &vaAssocTagGroup);
	if (SUCCEEDED(hr))
	{
		bstr = vaAssocTagGroup.bstrVal;
	}

	// 原标签组
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = SysAllocString(m_vaOutput.bstrVal);
	// 关联标签组
	pvars[1].vt = VT_BSTR;
	pvars[1].bstrVal = bstr;
	bstr.m_str = NULL;
	// 禁止使用非关联标签
	pvars[2].vt = VT_BOOL;
	pvars[2].boolVal = VARIANT_FALSE;
	// 使用本地配置
	pvars[3].vt = VT_BOOL;
	pvars[3].boolVal = VARIANT_FALSE;
	// Y座标
	pvars[4].vt = VT_I4;
	pvars[4].lVal = 0;
	// X座标
	pvars[5].vt = VT_I4;
	pvars[5].lVal = 0;
	// 父窗口句柄
	pvars[6].vt = VT_I4;
	pvars[6].lVal = (long)m_pLookupPage->m_hWnd;
	
	CComVariant vaRet = L"";

	DISPPARAMS dispparams = { pvars, NULL, 7, 0};
	hr = spTagPick->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	for (i = 0; i < 7; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr))
		return FALSE;

	if (vaRet.vt == VT_BSTR)
	{
		m_vaOutput = vaRet;
		return S_OK;
	}

	return S_FALSE;
}

//////////////////////////////////////////////////////////////////////
// CLookupItemSecurityArea Class
//////////////////////////////////////////////////////////////////////

CLookupItemSecurityArea::CLookupItemSecurityArea(CLookupPage* pLookupPage) :
	CLookupItem(pLookupPage)
{
}

CLookupItemSecurityArea::~CLookupItemSecurityArea()
{
}

HRESULT CLookupItemSecurityArea::OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected)
{
	USES_CONVERSION;
	
	CComVariant var = m_vaOutput;
	var.ChangeType(VT_BSTR);
	
	CRect rcT = rect;
	if (bSelected)
		rcT.right -= (rcT.bottom - rcT.top);
	dc.DrawText(W2CT(var.bstrVal), -1, &rcT, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	
	rcT = rect;
	if (bSelected)
	{
		rcT.left = rcT.right - (rcT.bottom - rcT.top) - 2;
		DrawEllipsisButton(dc, rcT);
	}
	
	return S_OK;
}

HRESULT CLookupItemSecurityArea::OnClickOutput(RECT, POINT pt)
{
	CComPtr<IDispatch> spTagPick;
	HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
	if (FAILED(hr))
		return S_FALSE;

	VARIANTARG* pvars = new VARIANTARG[5];
	for (int i = 0; i < 5; i++)
		VariantInit(&pvars[i]);

	// 原标签组
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = SysAllocString(m_vaOutput.bstrVal);;
	// 使用本地配置
	pvars[1].vt = VT_BOOL;
	pvars[1].boolVal = VARIANT_FALSE;
	// Y座标
	pvars[2].vt = VT_I4;
	pvars[2].lVal = 0;
	// X座标
	pvars[3].vt = VT_I4;
	pvars[3].lVal = 0;
	// 父窗口句柄
	pvars[4].vt = VT_I4;
	pvars[4].lVal = (long)m_pLookupPage->m_hWnd;
	
	CComVariant vaRet = L"";

	DISPPARAMS dispparams = { pvars, NULL, 5, 0};
	hr = spTagPick->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	for (i = 0; i < 5; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr))
		return FALSE;

	if (vaRet.vt == VT_BSTR)
	{
		m_vaOutput = vaRet;
		return S_OK;
	}

	return S_FALSE;
}
