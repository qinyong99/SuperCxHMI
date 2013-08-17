// FormatPage.cpp : Implementation of CFormatPage
#include "stdafx.h"
#include "CxDynObjs.h"
#include "FormatPage.h"

//获取页面类型
HRESULT CFormatPage::CrackPropertyType()
{
	IDynamicFrm* pDynamicFrm = NULL;
	HRESULT hr = m_pPageSite->QueryInterface(IID_IDynamicFrm, (void **)&pDynamicFrm);
	if (FAILED(hr))
		return hr;
	
	CDynamicPropInfo* pPropInfo = NULL;
	pDynamicFrm->get_PropertyInfo((long *)&pPropInfo);
	
	//获取接口属性的页面类型
	m_pPropInfo = pPropInfo;
	
	pDynamicFrm->Release();
	
	return S_OK;
}

HRESULT CFormatPage::SetPageSite(IPropertyPageSite *pPageSite)
{
	HRESULT hr = IPropertyPageImpl<CFormatPage>::SetPageSite(pPageSite);
	if (FAILED(hr))
		return hr;
	
	hr = CrackPropertyType();
	
	return hr;		
}

HRESULT CFormatPage::Apply(void)
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IFormatDynamic> spDynamic(m_ppUnk[i]);
		ATLASSERT(spDynamic != NULL);
				
		CComBSTR bstr;
		
		int nDataType;
		if (IsDlgButtonChecked(IDC_ENABLEFORMAT))
		{
			//格式字符串
			GetDlgItemText(IDC_FORMAT, (BSTR&)bstr);
			spDynamic->put_Format(bstr);
			nDataType = 0;
		}
		else
		{
			nDataType = ::SendMessage(GetDlgItem(IDC_DATATYPE), CB_GETCURSEL, 0, 0);
			nDataType += 1;

			CComVariant varWholeDigits;
			CComVariant varDecimalDigits;
			if (nDataType == 1)
			{
				//总位数
				GetDlgItemText(IDC_WHOLEDIGITS, (BSTR&)bstr);
				if (bstr.Length() == 0)
				{
					::MessageBox(m_hWnd, _T("请输入数字位数！"),  _T("提示"), MB_OK);
					::SetFocus(GetDlgItem(IDC_WHOLEDIGITS));
					return S_FALSE;
				}
				OnCreateExpression(&bstr, 2);
				varWholeDigits = bstr;

				//小数位数
				GetDlgItemText(IDC_DECIMALDIGITS, (BSTR&)bstr);
				if (bstr.Length() == 0)
				{
					::MessageBox(m_hWnd, _T("请输入小数位数！"),  _T("提示"), MB_OK);
					::SetFocus(GetDlgItem(IDC_DECIMALDIGITS));
					return S_FALSE;
				}
				OnCreateExpression(&bstr, 2);
				varDecimalDigits = bstr;
			}
			else
			{
				//行数
				GetDlgItemText(IDC_LINES, (BSTR&)bstr);
				if (bstr.Length() == 0)
				{
					::MessageBox(m_hWnd, _T("请输入行数！"),  _T("提示"), MB_OK);
					::SetFocus(GetDlgItem(IDC_LINES));
					return S_FALSE;
				}
				OnCreateExpression(&bstr, 2);
				varWholeDigits = bstr;
				
				//行字符数
				GetDlgItemText(IDC_CHARSPERLINE, (BSTR&)bstr);
				if (bstr.Length() == 0)
				{
					::MessageBox(m_hWnd, _T("请输入行字符数！"),  _T("提示"), MB_OK);
					::SetFocus(GetDlgItem(IDC_CHARSPERLINE));
					return S_FALSE;
				}
				OnCreateExpression(&bstr, 2);
				varDecimalDigits = bstr;
			}
			
			//对齐
			long nJustify = ::SendMessage(GetDlgItem(IDC_JUSTIFY), CB_GETCURSEL, 0, 0);
			spDynamic->SetNumericFormat(varWholeDigits, varDecimalDigits, nJustify);
		}
		spDynamic->put_SourceDataType(nDataType);
		
		BOOL b;
		if (m_pPropInfo->bSupportMouseInput)
		{
			b = ::SendMessage(GetDlgItem(IDC_ENABLEMOUSEINPUT), BM_GETCHECK, 0, 0L);
			spDynamic->put_EnableMouseInput(b);
		}
	}

	m_bDirty = FALSE;
	return S_OK;
}

HRESULT CFormatPage::HelperSetText(int nIDDlgItem, CComVariant &var)
{
	USES_CONVERSION;
	
	if (var.vt != VT_BSTR)
		var.ChangeType(VT_BSTR);
	
	if (var.vt != VT_BSTR)
		return S_FALSE;
	
	SetDlgItemText(nIDDlgItem, W2T(var.bstrVal));
	
	return S_OK;
}

HRESULT CFormatPage::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	USES_CONVERSION;

	//调用基类实现
	HRESULT hr = IPropertyPageImpl<CFormatPage>::SetObjects(nObjects, ppUnk);
	if (SUCCEEDED(hr))
	{
		CComQIPtr<IFormatDynamic> spDynamic(m_ppUnk[0]);
		ATLASSERT(spDynamic != NULL);
		
		int nDataType;
		spDynamic->get_SourceDataType(&nDataType);
		if (nDataType == 0)
		{
			::SendMessage(GetDlgItem(IDC_ENABLEFORMAT), BM_SETCHECK, BST_CHECKED, 0); 
			::SendMessage(GetDlgItem(IDC_DATATYPE), CB_SETCURSEL, 0, 0);
		}
		else
		{
			::SendMessage(GetDlgItem(IDC_DATATYPE), CB_SETCURSEL, nDataType - 1, 0);
		}
		
		CComBSTR bstr;
		spDynamic->get_Format(&bstr);
		SetDlgItemText(IDC_FORMAT, W2T(bstr));

		CComVariant varWholeDigits;
		CComVariant varDecimalDigits;
		int nJustify;
		spDynamic->GetNumericFormat(&varWholeDigits, &varDecimalDigits, &nJustify);
	
		//总位数和小数位数
		if (nDataType == 1)
		{
			if (varWholeDigits.vt == VT_BSTR)
				OnCreateExpression(&varWholeDigits.bstrVal, 1);
			HelperSetText(IDC_WHOLEDIGITS, varWholeDigits);
			
			if (varDecimalDigits.vt == VT_BSTR)
				OnCreateExpression(&varDecimalDigits.bstrVal, 1);
			HelperSetText(IDC_DECIMALDIGITS, varDecimalDigits);

			SetDlgItemText(IDC_LINES, "1");
			SetDlgItemText(IDC_CHARSPERLINE, "10");
		}
		//行数和行字符数
		else if (nDataType == 2)
		{
			if (varWholeDigits.vt == VT_BSTR)
				OnCreateExpression(&varWholeDigits.bstrVal, 1);
			HelperSetText(IDC_LINES, varWholeDigits);
			
			if (varDecimalDigits.vt == VT_BSTR)
				OnCreateExpression(&varDecimalDigits.bstrVal, 1);
			HelperSetText(IDC_CHARSPERLINE, varDecimalDigits);
			
			SetDlgItemText(IDC_WHOLEDIGITS, "7");
			SetDlgItemText(IDC_DECIMALDIGITS, "2");
		}
		else
		{
			SetDlgItemText(IDC_LINES, "1");
			SetDlgItemText(IDC_CHARSPERLINE, "10");
			SetDlgItemText(IDC_WHOLEDIGITS, "7");
			SetDlgItemText(IDC_DECIMALDIGITS, "2");
		}
		
		//对齐
		::SendMessage(GetDlgItem(IDC_JUSTIFY), CB_SETCURSEL, nJustify, 0);

		BOOL bHandled;
		OnClickedFormat(0, 0, 0, bHandled);
		OnSelchangeDataType(0, 0, 0, bHandled);

		if (spDynamic != NULL)
		{
			BOOL b;
			
			if (m_pPropInfo->bSupportMouseInput)
			{
				spDynamic->get_EnableMouseInput(&b);
				::SendMessage(GetDlgItem(IDC_ENABLEMOUSEINPUT), BM_SETCHECK, b, 0L); 
			}
			else
			{
				::ShowWindow(GetDlgItem(IDC_ENABLEMOUSEINPUT), SW_HIDE);
			}
		}
	}

	return hr;
}

LRESULT CFormatPage::OnClickedFormat(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	CComBSTR bstr;
	GetDlgItemText(IDC_FORMAT, (BSTR&)bstr);
	if (bstr.m_str == NULL || bstr == "")
	{
		bstr = "%s";
		SetDlgItemText(IDC_FORMAT, W2T(bstr));
	}

	if (IsDlgButtonChecked(IDC_ENABLEFORMAT))
	{
		::EnableWindow(GetDlgItem(IDC_FORMAT), true);
		::EnableWindow(GetDlgItem(IDC_DATATYPE), false);
		::EnableWindow(GetDlgItem(IDC_WHOLEDIGITS), false);
		::EnableWindow(GetDlgItem(IDC_DECIMALDIGITS), false);
		::EnableWindow(GetDlgItem(IDC_STATIC_LINES), false);
		::EnableWindow(GetDlgItem(IDC_STATIC_CHARSPERLINE), false);
		::EnableWindow(GetDlgItem(IDC_JUSTIFY), false);
		::EnableWindow(GetDlgItem(IDC_PARA_EXP1), false);
		::EnableWindow(GetDlgItem(IDC_PARA_EXP2), false);
	}
	else
	{
		::EnableWindow(GetDlgItem(IDC_FORMAT), false);
		::EnableWindow(GetDlgItem(IDC_DATATYPE), true);
		::EnableWindow(GetDlgItem(IDC_WHOLEDIGITS), true);
		::EnableWindow(GetDlgItem(IDC_DECIMALDIGITS), true);
		::EnableWindow(GetDlgItem(IDC_STATIC_LINES), true);
		::EnableWindow(GetDlgItem(IDC_STATIC_CHARSPERLINE), true);
		::EnableWindow(GetDlgItem(IDC_JUSTIFY), true);
		::EnableWindow(GetDlgItem(IDC_PARA_EXP1), true);
		::EnableWindow(GetDlgItem(IDC_PARA_EXP2), true);
	}

	return S_OK;
}

LRESULT CFormatPage::OnSelchangeDataType(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	int n = ::SendMessage(GetDlgItem(IDC_DATATYPE), CB_GETCURSEL, 0, 0);

	::ShowWindow(GetDlgItem(IDC_STATIC_WHOLEDIGITS), n != 1);
	::ShowWindow(GetDlgItem(IDC_STATIC_DECIMALDIGITS), n != 1);
	::ShowWindow(GetDlgItem(IDC_STATIC_LINES), n == 1);
	::ShowWindow(GetDlgItem(IDC_STATIC_CHARSPERLINE), n == 1);

	::ShowWindow(GetDlgItem(IDC_WHOLEDIGITS), n != 1);
	::ShowWindow(GetDlgItem(IDC_DECIMALDIGITS), n != 1);
	::ShowWindow(GetDlgItem(IDC_LINES), n == 1);
	::ShowWindow(GetDlgItem(IDC_CHARSPERLINE), n == 1);

	return 0;
}

LRESULT CFormatPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SendMessage(GetDlgItem(IDC_DATATYPE), CB_ADDSTRING, 0, (LPARAM)_T("数字"));
	::SendMessage(GetDlgItem(IDC_DATATYPE), CB_ADDSTRING, 0, (LPARAM)_T("字符-数字"));
	::SendMessage(GetDlgItem(IDC_JUSTIFY), CB_ADDSTRING, 0, (LPARAM)_T("左边"));
	::SendMessage(GetDlgItem(IDC_JUSTIFY), CB_ADDSTRING, 0, (LPARAM)_T("居中"));
	::SendMessage(GetDlgItem(IDC_JUSTIFY), CB_ADDSTRING, 0, (LPARAM)_T("右边"));

	return 0;
}

#include "../CxScripCrt/CxScripCrt.h"

LRESULT CFormatPage::OnClickedExp1(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;
	
	int n = ::SendMessage(GetDlgItem(IDC_DATATYPE), CB_GETCURSEL, 0, 0);

	CComBSTR bstr;
	UINT nIDText = (n == 0 ? IDC_WHOLEDIGITS : IDC_LINES);
	GetDlgItemText(nIDText, (BSTR&)bstr);

	if (OnCreateExpression(&bstr))
		SetDlgItemText(nIDText, W2T(bstr));	
	
	return 0;		
}

LRESULT CFormatPage::OnClickedExp2(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	USES_CONVERSION;
	
	int n = ::SendMessage(GetDlgItem(IDC_DATATYPE), CB_GETCURSEL, 0, 0);
	
	CComBSTR bstr;
	UINT nIDText = (n == 0 ? IDC_DECIMALDIGITS : IDC_CHARSPERLINE);
	GetDlgItemText(nIDText, (BSTR&)bstr);
	
	if (OnCreateExpression(&bstr))
		SetDlgItemText(nIDText, W2T(bstr));	

	return 0;
}

BOOL CFormatPage::OnCreateExpression(BSTR* pbstr, int iFlag)
{
	CComPtr<IDynamicFrm> spDynamicFrm;
	HRESULT hr = m_pPageSite->QueryInterface(IID_IDynamicFrm, (void **)&spDynamicFrm);
	if (SUCCEEDED(hr))
	{
		CDynamicPropInfo* pPropInfo = NULL;
		spDynamicFrm->get_PropertyInfo((long *)&pPropInfo);
		if (pPropInfo != NULL && pPropInfo->pfnExpression)
			pPropInfo->pfnExpression(pbstr, iFlag);
		else if (iFlag == 0)
			spDynamicFrm->OnCreateExpression(pbstr);
	}
	else if (iFlag == 0)
	{
		CComPtr<ICxExpressionCreator> spExprCrt;
		HRESULT hr = spExprCrt.CoCreateInstance(CLSID_CxExpressionCreator);
		if (FAILED(hr)) 
			return hr;

		spExprCrt->put_Expression(*pbstr);

		spExprCrt->AddGeneralSymbolPage(CX_ALL_SYMBOL);

		hr = spExprCrt->ShowDialog();
		if (FAILED(hr)) 
			return hr;

		if (*pbstr != NULL)
			SysFreeString(*pbstr);
		spExprCrt->get_Expression(pbstr);
	}

	return TRUE;
}