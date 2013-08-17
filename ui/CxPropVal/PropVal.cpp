// PropVal.cpp : Implementation of CPropVal

#include "stdafx.h"
#include "CxPropVal.h"
#include "PropVal.h"
#include <shlobj.h>

/////////////////////////////////////////////////////////////////////////////
// CPropVal

STDMETHODIMP CPropVal::get_Type(long *pVal)
{
	*pVal = (long)m_type;

	return S_OK;
}

STDMETHODIMP CPropVal::put_Type(long newVal)
{
	m_type = (enumPropType)newVal;
	m_edit.EnableWindow(m_type != PROPTYPE_COLOR);
	m_button.EnableWindow(m_type != PROPTYPE_STRING && m_type != PROPTYPE_NUMBER);
//	m_button.EnableWindow(m_type != PROPTYPE_STRING && m_type != PROPTYPE_NUMBER);

	return S_OK;
}

STDMETHODIMP CPropVal::get_Value(VARIANT *pVal)
{
	USES_CONVERSION;

	if (m_type == PROPTYPE_STRING || m_type == PROPTYPE_NUMBER || m_type == PROPTYPE_ENUMSTRING)
	{
		CComBSTR bstrText;
		m_edit.GetWindowText((BSTR&)bstrText);
		m_value = bstrText;
	}

	VariantCopy(pVal, &m_value);

	return S_OK;
}

STDMETHODIMP CPropVal::get_DefaultValue(VARIANT *pVal)
{
	VariantCopy(pVal, &m_valueDefault);
	
	return S_OK;
}

STDMETHODIMP CPropVal::put_DefaultValue(VARIANT newVal)
{
	m_valueDefault = newVal;
	
	return S_OK;
}

STDMETHODIMP CPropVal::put_Value(VARIANT newVal)
{
	m_edit.EnableWindow(m_type != PROPTYPE_COLOR);
	m_button.EnableWindow(m_type != PROPTYPE_STRING && m_type != PROPTYPE_NUMBER);
//	m_button.EnableWindow(TRUE);

	USES_CONVERSION;

	m_value = newVal;
	if (m_value.vt == VT_EMPTY)
		m_value.lVal = 0;

	if (m_type == PROPTYPE_COLOR)
	{
		m_edit.Invalidate();
	}
	if (m_type == PROPTYPE_STRING || m_type == PROPTYPE_NUMBER)
	{
		m_value.ChangeType(VT_BSTR);
		m_edit.SetWindowText(OLE2T(m_value.bstrVal));
	}
	else if (m_type == PROPTYPE_ENUM || m_type == PROPTYPE_ENUMSTRING)
	{
		if (m_arrValues.GetSize() > 0)
		{
			int nSel = -1;
			for (int i = 0; i < m_arrValues.GetSize(); i++)
			{
				if (m_arrValues[i] == m_value)
					nSel = i;
			}
			if (nSel != -1)
				m_edit.SetWindowText(OLE2T(m_arrDescs[nSel]));
			else if (m_value.vt == VT_BSTR)
				m_edit.SetWindowText(OLE2T(m_value.bstrVal));
		}
	}
	else if (m_type == PROPTYPE_BOOL)
	{
		if (m_value.bVal)
			m_edit.SetWindowText(_T("True"));
		else
			m_edit.SetWindowText(_T("False"));
	}
	else if (m_type == PROPTYPE_FAKEFONT)
	{
		CComVariant var;
		var.Copy(&m_value);
		var.ChangeType(VT_BSTR);
		m_edit.SetWindowText(OLE2T(var.bstrVal));
	}
	else if (m_type == PROPTYPE_FAKEFONTNAME)
	{
		CComVariant var;
		var.Copy(&m_value);
		var.ChangeType(VT_BSTR);
		m_edit.SetWindowText(OLE2T(var.bstrVal));
	}
	else if (m_type == PROPTYPE_FAKEPICTURE)
	{
		CComVariant var = m_value;
		CComBSTR bstr;
		if (var.vt == VT_BSTR)
			bstr = var.bstrVal;

		if (var.vt == VT_DISPATCH || var.vt == VT_UNKNOWN)
		{	
			bstr = L"(empty)";
			CComQIPtr<IPicture> pPicture(var.pdispVal);
			if (pPicture.p)
			{
				short type = 0;
				HRESULT hr = pPicture->get_Type(&type);
				if (SUCCEEDED(hr))
				{
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
		}

		if (bstr.m_str == NULL || bstr == "")
			bstr = L"(empty)";

		m_edit.SetWindowText(OLE2T(bstr));
	}
	else if (m_type == PROPTYPE_FAKEFILE ||
		m_type == PROPTYPE_FAKEFOLDER ||
		m_type == PROPTYPE_TAGGROUP ||
		m_type == PROPTYPE_SECURITYAREA)
	{
		CComVariant var;
		var.Copy(&m_value);
		var.ChangeType(VT_BSTR);
		m_edit.SetWindowText(OLE2T(var.bstrVal));
	}
		
	return S_OK;
}

LRESULT CPropVal::OnPaintEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	LRESULT lrRet = m_edit.DefWindowProc(uMsg, wParam, lParam);

	if (m_type == PROPTYPE_COLOR)
	{
		CClientDC dc(m_edit);
		COLORREF clrRect;
		HPEN hOldPen = dc.SelectStockPen(BLACK_PEN);

		OLE_COLOR clr;
		clr = m_value.lVal;
		OleTranslateColor(clr, 0, &clrRect);
		HBRUSH hBrush = ::CreateSolidBrush(clrRect);	
		HBRUSH hOldBrush = dc.SelectBrush(hBrush);

		CRect rcItem;
		m_edit.GetClientRect(&rcItem);
		dc.Rectangle(rcItem);
	
		dc.SelectPen(hOldPen);
		dc.SelectBrush(hOldBrush);
		::DeleteObject(hBrush);
	}

	return lrRet;
}

#include "../CxComObj/CxComObj.h"
#include "../CxComObj/CxComObj_i.c"
#include "PopupList.h"

const CLSID CLSID_CxTagPicker = {0x6CF3993A,0x1D26,0x40BF,{0xB4,0xD8,0x94,0xC4,0xB5,0x4F,0x13,0x90}};

LRESULT CPropVal::OnClickedButton_value(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	Fire_BeginEdit();

	CRect rcEdit;
	m_edit.GetWindowRect(&rcEdit);
	POINT ptTemp = {rcEdit.left, rcEdit.bottom};

	if (m_type == PROPTYPE_COLOR)
	{
		OLE_COLOR clr = m_value.lVal;
		
		CComPtr<ICxColorPicker2> p;		
		HRESULT hr = p.CoCreateInstance(CLSID_CxColorPicker2); 
		if (FAILED(hr)) 
			return 0;
		
		CComVariant varDefault = m_valueDefault;
		varDefault.ChangeType(VT_I4);
		hr = p->Pick((long)m_hWnd, rcEdit, varDefault.lVal, clr, &clr);
		if (FAILED(hr)) 
			return 0;

		if (hr == S_OK)
		{
			m_value.vt = VT_I4;
			m_value.lVal = clr;
			m_edit.Invalidate();
		}
	}
	else if (m_type == PROPTYPE_ENUM || m_type == PROPTYPE_ENUMSTRING || m_type == PROPTYPE_BOOL)
	{
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		
		CPopupList combo;
		HIMAGELIST himages = ImageList_LoadImage
							(_Module.m_hInstResource, MAKEINTRESOURCE(IDB_ARROW), 16, 5, RGB(255,255,255), IMAGE_BITMAP, 0); 	
		if (!combo.Create(m_hWnd, hFont))
			return E_FAIL;
				
		if (m_type == PROPTYPE_ENUM || m_type == PROPTYPE_ENUMSTRING)
		{
			for (int i =0; i < m_arrDescs.GetSize(); i++)
				combo.AddString(OLE2T(m_arrDescs[i]));	
		}
		else
		{
			combo.AddString(_T("True"));
			combo.AddString(_T("False"));
		}
		int nSel;
		if (m_type == PROPTYPE_ENUM || m_type == PROPTYPE_ENUMSTRING)
		{
			nSel = -1;
			for (int i = 0; i < m_arrValues.GetSize(); i++)
			{
				if (m_arrValues[i] == m_value)
					nSel = i;
			}
		}
		else
		{
			nSel = !m_value.bVal;
		}
		nSel = combo.Show(ptTemp.x, ptTemp.y, rcEdit.Width() + 15, nSel);
		if (-1 != nSel)
		{
			put_Value(CComVariant((m_type == PROPTYPE_ENUM || m_type == PROPTYPE_ENUMSTRING) ? m_arrValues[nSel] : nSel == 0));
		}
	}
	else if (m_type == PROPTYPE_FAKEFONT)
	{
		LOGFONT logfont;
		HFONT hFont;

		CHOOSEFONT cf; 
		memset(&cf, 0, sizeof CHOOSEFONT);
		cf.lStructSize = sizeof CHOOSEFONT;
		cf.hInstance = _Module.m_hInst;
		cf.hwndOwner = m_hWnd;
		cf.lpLogFont = &logfont;
		cf.lpTemplateName = "IDD_FONTDIALOG";
		cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_ENABLETEMPLATE;

		if (((m_value.vt == VT_DISPATCH) || (m_value.vt == VT_UNKNOWN)))
		{	
			CComQIPtr<IFont> spFont(m_value.pdispVal);
			
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
			HDC hdc = GetDC();
			ppi = GetDeviceCaps(hdc, LOGPIXELSY);
			ReleaseDC(hdc);
			
			fd.cySize.Lo = lfHeight * 720000 / ppi;
			fd.cySize.Hi = 0;

			CComPtr<IDispatch> pFont;			
			HRESULT hr = OleCreateFontIndirect(&fd, IID_IDispatch, (void**)&pFont);
			if (pFont.p)
				put_Value(CComVariant(pFont.p));
		}
	}
	else if (m_type == PROPTYPE_FAKEPICTURE)
	{
		USES_CONVERSION;
		
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof OPENFILENAME);			
		ofn.lStructSize = sizeof OPENFILENAME;
		ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
		ofn.nMaxFile = _MAX_PATH;	
		ofn.hwndOwner = m_hWnd;				
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
				put_Value(CComVariant(spPicture));
		}	
	}
	else if (m_type == PROPTYPE_FAKEFONTNAME)
	{
		CComVariant var;
		var.Copy(&m_value);
		var.ChangeType(VT_BSTR);

		CComBSTR bstr = var.bstrVal;
		
		CComPtr<ICxFontCombo> p;		
		p.CoCreateInstance(CLSID_CxFontCombo); 
		p->PickFont((long)m_hWnd, ptTemp.x, ptTemp.y, rcEdit.Width() + 15, bstr, &bstr); 				
		if (bstr != var.bstrVal)
			put_Value(CComVariant(bstr));
	}
	else if (m_type == PROPTYPE_FAKEFILE || m_type == PROPTYPE_FAKEBRUSHFILE)
	{
		OPENFILENAME ofn;
		memset(&ofn, 0, sizeof OPENFILENAME);			
		ofn.lStructSize = sizeof OPENFILENAME;
		ofn.lpstrFile =  (TCHAR*) _alloca(_MAX_PATH * sizeof TCHAR);
		ofn.nMaxFile = _MAX_PATH;	
		ofn.hwndOwner = m_hWnd;				
		ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
		
		memset(ofn.lpstrFile, 0, _MAX_PATH * sizeof TCHAR); 

		if (m_type == PROPTYPE_FAKEFILE)
		{
			ofn.lpstrFilter = 
				_T("所有文件\0*.*\0");
			ofn.lpstrTitle = _T("选择文件");
		}
		else
		{
			ofn.lpstrFilter = 
				_T("所有图片文件\0*.bmp;*.dib;*.gif;*.jpg;*.wmf;*.emf;*.ico;*.cur\0")
				_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
				_T("GIF 图像 (*.gif)\0*.gif\0")
				_T("JPEG 图像 (*.jpg)\0*.jpg\0")
				_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
				_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
				_T("所有文件(*.*)\0*.*\0");
			ofn.lpstrTitle = _T("加载图片");
		}
		
		if (GetOpenFileName(&ofn))
		{
			put_Value(CComVariant(ofn.lpstrFile));
		}	
	}
	else if (m_type == PROPTYPE_FAKEFOLDER)
	{
		BROWSEINFO bi;
		memset(&bi, 0, sizeof BROWSEINFO);
		bi.ulFlags = 0; //BIF_NEWDIALOGSTYLE | BIF_USENEWUI | BIF_EDITBOX
		bi.hwndOwner = m_hWnd;
		bi.pszDisplayName = (TCHAR*)_alloca(MAX_PATH * sizeof TCHAR);
		memset(bi.pszDisplayName, 0, MAX_PATH * sizeof TCHAR);

		CComVariant var;
		var.Copy(&m_value);
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

			put_Value(CComVariant(bi.pszDisplayName));
		}
	}
	else if (m_type == PROPTYPE_TAGGROUP)
	{
		CComPtr<IDispatch> spTagPick;
		HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
		if (FAILED(hr))
			return 0;

		VARIANTARG* pvars = new VARIANTARG[7];
		for (int i = 0; i < 7; i++)
			VariantInit(&pvars[i]);

		//关联标签组
		CComBSTR bstr;

		if (m_spObject)
		{
			CComDispatchDriver dd(m_spObject);
			CComVariant vaAssocTagGroup;
			hr = dd.GetPropertyByName(L"AssocTagGroup", &vaAssocTagGroup);
			if (SUCCEEDED(hr))
			{
				bstr = vaAssocTagGroup.bstrVal;
			}
		}

		// 原标签组
		pvars[0].vt = VT_BSTR;
		pvars[0].bstrVal = bstr;
		bstr.m_str = NULL;
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
		pvars[6].lVal = (long)m_hWnd;
		
		CComVariant vaRet = L"";
	
		DISPPARAMS dispparams = { pvars, NULL, 7, 0};
		hr = spTagPick->Invoke(2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
		for (i = 0; i < 7; i++)
			VariantClear(&pvars[i]);
		delete [] pvars;
		if (FAILED(hr))
			return 0;

		if (vaRet.vt == VT_BSTR)
		{
			put_Value(vaRet);
			return 0;
		}	
	}
	else if (m_type == PROPTYPE_SECURITYAREA)
	{
		CComPtr<IDispatch> spTagPick;
		HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
		if (FAILED(hr))
			return FALSE;
		
		VARIANTARG* pvars = new VARIANTARG[5];
		for (int i = 0; i < 5; i++)
			VariantInit(&pvars[i]);
		
		// 原安全区
		CComBSTR bstrArea;
			
		if (m_spObject)
		{
			CComDispatchDriver dd(m_spObject);
			CComVariant vaArea;
			hr = dd.GetPropertyByName(L"SecurityArea", &vaArea);
			if (SUCCEEDED(hr))
			{
				bstrArea = vaArea.bstrVal;
			}
		}
		
		pvars[0].vt = VT_BSTR;
		pvars[0].bstrVal = bstrArea;
		bstrArea.m_str = NULL;
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
		pvars[4].lVal = (long)m_hWnd;
		
		CComVariant vaRet = L"";
		
		DISPPARAMS dispparams = { pvars, NULL, 5, 0};
		hr = spTagPick->Invoke(3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
		for (i = 0; i < 5; i++)
			VariantClear(&pvars[i]);
		delete [] pvars;
		if (FAILED(hr))
			return 0;
		
		if (vaRet.vt == VT_BSTR)
		{
			put_Value(vaRet);
			return 0;
		}	
	}

	Fire_ValueChanged();

	return 0;
}

STDMETHODIMP CPropVal::AddEnumItem(BSTR bstrDesc, VARIANT varValue)
{
	m_arrDescs.Add(CComBSTR(bstrDesc));
	m_arrValues.Add(varValue);

	return S_OK;
}

STDMETHODIMP CPropVal::Reset()
{
	m_arrDescs.RemoveAll();
	m_arrValues.RemoveAll();
	m_type = PROPTYPE_UNKNOWN;
	m_value.Clear();
	
	m_edit.EnableWindow(FALSE);
	m_edit.SetWindowText("");
	m_button.EnableWindow(FALSE);

	return S_OK;
}

STDMETHODIMP CPropVal::get_Object(IUnknown **pVal)
{
	*pVal = m_spObject;
	if (*pVal)
		(*pVal)->AddRef();

	return S_OK;
}

STDMETHODIMP CPropVal::put_Object(IUnknown *newVal)
{
	m_spObject = newVal;

	return S_OK;
}


