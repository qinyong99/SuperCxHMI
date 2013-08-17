/////////////////////////////////////////////////////////
//			动态属性----格式页
//
//	创建:wengjiang				日期:2001年9月5日
//
//	修改:
/////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CxDynObjs.h"
#include "FormatDynamic.h"


/////////////////////////////////////////////////////////////////////////////
// CFormatDynamic

STDMETHODIMP CFormatDynamic::GetClassID(CLSID* pClassID)
{
	*pClassID = GetObjectCLSID();
	return S_OK;
}

STDMETHODIMP CFormatDynamic::IsDirty()
{
	return (m_bRequiresSave) ? S_OK : S_FALSE;
}

STDMETHODIMP CFormatDynamic::Load(LPSTREAM pStm)
{
	m_bstrSource.ReadFromStream(pStm);
	m_bstrFormat.ReadFromStream(pStm);

	DWORD dwVer;
	pStm->Read(&dwVer, sizeof(dwVer), NULL);
	// 早期版本
	if (dwVer < _SUPERCX_VER_0102)
	{
		m_iDataType = dwVer;
		int n;
		pStm->Read(&n, sizeof(n), NULL);
		m_varWholeDigits = n;
		pStm->Read(&n, sizeof(n), NULL);
		m_varDecimalDigits = n;
	}
	else
	{
		pStm->Read(&m_iDataType, sizeof(m_iDataType), NULL);
		m_varWholeDigits.ReadFromStream(pStm);
		m_varDecimalDigits.ReadFromStream(pStm);
	}
	
	pStm->Read(&m_iJustify, sizeof(m_iJustify), NULL);
	if (dwVer > _SUPERCX_VER_0201)
		pStm->Read(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
	
	return S_OK;
}

STDMETHODIMP CFormatDynamic::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	m_bstrSource.WriteToStream(pStm);
	m_bstrFormat.WriteToStream(pStm);

	DWORD dwVer = _SUPERCX_VER;

	pStm->Write(&dwVer, sizeof(dwVer), NULL);
	pStm->Write(&m_iDataType, sizeof(m_iDataType), NULL);
	m_varWholeDigits.WriteToStream(pStm);
	m_varDecimalDigits.WriteToStream(pStm);
	pStm->Write(&m_iJustify, sizeof(m_iJustify), NULL);
	if (dwVer > _SUPERCX_VER_0201)
		pStm->Write(&m_bEnableMouseInput, sizeof(m_bEnableMouseInput), NULL);
	
	return S_OK;
}

STDMETHODIMP CFormatDynamic::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = ::SysAllocString(L"Format");
	return S_OK;
}

STDMETHODIMP CFormatDynamic::GetCustomPage(CLSID* pclsidPropPage)
{
	*pclsidPropPage = CLSID_FormatPage;
	return S_OK;
}

STDMETHODIMP CFormatDynamic::get_Source(BSTR *pVal)
{
	*pVal = m_bstrSource.Copy();
	return S_OK;
}

STDMETHODIMP CFormatDynamic::put_Source(BSTR newVal)
{
	m_bstrSource = newVal;
	return S_OK;
}

inline HRESULT CFormatDynamic::ChangeToI4(VARIANT* pvaValue)
{
	if (pvaValue->vt == VT_BSTR)
	{
		USES_CONVERSION;
		USES_CHINESEVAR_CONVERSION;

		BSTR bstr = pvaValue->bstrVal;
		m_pScriptEngineSite->EvaluateExpression(T2W(VARC2A(W2T(bstr))), pvaValue);
		//	if (pvaValue->vt == VT_EMPTY)
		//		m_pScriptEngineSite->EvaluateExpression(bstr, pvaValue);
		
		SysFreeString(bstr); // 必须释放
	}
	return VariantChangeType(pvaValue, pvaValue, 0, VT_I4);
}

#define ZEROPAD    1        /* pad with zero */

STDMETHODIMP CFormatDynamic::Evaluation(VARIANT* pvaInValue, VARIANT* pvaOutValue)
{
	HRESULT hr;
	CComVariant varWholeDigits; 
	CComVariant varDecimalDigits; 

	if (m_iDataType != 0)
	{
		varWholeDigits = m_varWholeDigits;
		hr = ChangeToI4(&varWholeDigits);
		if (FAILED(hr))
			return hr;

		varDecimalDigits = m_varDecimalDigits;
		hr = ChangeToI4(&varDecimalDigits);
		if (FAILED(hr))
			return hr;

		if (m_iDataType == 1)
		{
			// 数字位数要大于小数位数加一位小数点位数的和！
			if (varWholeDigits.intVal <= varDecimalDigits.intVal + 1 && varDecimalDigits.intVal != 0)
				return E_FAIL;
				
			// 主要是解决强制转换成字符串时为科学计数法的问题
			hr = VariantChangeType(pvaInValue, pvaInValue, 0, VT_R8);
			if (FAILED(hr))
				return hr;
			
			// 四舍六入五成双
			double& d = pvaInValue->dblVal;
			VarR8Round(d, varDecimalDigits.intVal, &d);
			
			WCHAR szBuf[32];
			swprintf(szBuf, L"%f", d);
			pvaInValue->vt = VT_BSTR;
			pvaInValue->bstrVal = SysAllocString(szBuf);
		}
		else
		{
			hr = VariantChangeType(pvaInValue, pvaInValue, 0, VT_BSTR);
			if (FAILED(hr))
				return hr;

			// 行数不能小于1，每行字符数不能小于1
			if (m_iDataType == 2 && (varDecimalDigits.intVal < 1 || varDecimalDigits.intVal < 1))
				return E_FAIL;
		}
	}

	switch (m_iDataType)
	{
	case 0://格式化
		{
			LPCOLESTR lpszPercent = NULL;
			CComBSTR bstrVal;

			for (LPCOLESTR lpsz = m_bstrFormat; *lpsz != OLECHAR('\0'); lpsz++)
			{
				if (*lpsz != OLECHAR('%')/* || *(lpsz = ++lpsz) == OLECHAR('%')*/)
					continue;
			
				lpszPercent = lpsz++;
				
				int nFlags = 0;
				if (*lpsz == OLECHAR('0'))
					nFlags |= ZEROPAD;
				
				long lWidth = wcstol(lpsz, NULL, 10);
				while (iswdigit(*lpsz))
					lpsz++;
				
				int nPrecision = 0;
				if (*lpsz == OLECHAR('.'))
				{
					lpsz++;
					nPrecision = wcstol(lpsz, NULL, 10);
					while (iswdigit(*lpsz))
						lpsz++;
				}

				switch (*lpsz)
				{
				case OLECHAR('f'):
				case OLECHAR('F'):
					if (pvaInValue != NULL)
					{
						hr = VariantChangeType(pvaInValue, pvaInValue, 0, VT_R8);
						if (FAILED(hr))
 							return hr;

						OLECHAR szBuf[32];
						OLECHAR szFormat[32];
						memcpy(szFormat, lpszPercent, (lpsz - lpszPercent + 1) * sizeof(OLECHAR));
						szFormat[lpsz - lpszPercent + 1] = OLECHAR('\0');
						swprintf(szBuf, szFormat, pvaInValue->dblVal);

						bstrVal = szBuf;
					}
					else
					{
						for (int i = 0; i < lWidth; i++)
							bstrVal.Append(L"#");
						if (nPrecision > 0 && nPrecision < lWidth - 1)
							bstrVal[lWidth - nPrecision - 1] = OLECHAR('.');
					}
									
					break;
				case OLECHAR('s'):
				case OLECHAR('S'):
					{
						if (pvaInValue != NULL)
						{
				 			hr = VariantChangeType(pvaInValue, pvaInValue, 0, VT_BSTR);
					 		if (FAILED(hr))
 								return hr;

							bstrVal = pvaInValue->bstrVal;
						}
						else
						{
							if (lWidth == 0)
								lWidth = 10;
							for (int i = 0; i < lWidth; i++)
								bstrVal.Append(L"?");
						}
					}
					break;
				default:
					lpszPercent = NULL;
					break;
				}
				
				// 已经找到有效地 %
				if (lpszPercent != NULL)
				{
					lpsz++;
					break;
				}
			}

			if (lpszPercent != NULL)
			{
				int nMaxLen = m_bstrFormat.Length() - (lpsz - lpszPercent) + bstrVal.Length();
				BSTR bstr = SysAllocStringLen(NULL, nMaxLen);
				if (bstr == NULL)
					return E_OUTOFMEMORY;
			
				// 复制 % 前面的字符
				LPOLESTR lpszT = bstr;
				int n = lpszPercent - m_bstrFormat;
				memcpy(lpszT, m_bstrFormat, n * sizeof(OLECHAR));
				lpszT += n;

				// 复制变量字符串
				n = bstrVal.Length();
				memcpy(lpszT, bstrVal, n * sizeof(OLECHAR));
				lpszT += n;

				// 复制 % 后面的字符
				n = m_bstrFormat.Length() - (lpsz - m_bstrFormat);
				memcpy(lpszT, lpsz, n * sizeof(OLECHAR));
				lpszT += n;
				
				VariantClear(pvaOutValue);
				pvaOutValue->vt = VT_BSTR;
				pvaOutValue->bstrVal = bstr;
			}
			else
			{
				
				VariantClear(pvaOutValue);
				// 如果没找到%s或%S，拷贝m_bstrFormat
				pvaOutValue->vt = VT_BSTR;
				pvaOutValue->bstrVal = m_bstrFormat.Copy();
			}
		}
		break;
	case 1://数字
		{
			OLECHAR* p1;
			OLECHAR* p2;

			int iDecimalDigits = varDecimalDigits.intVal;
			// 处理没有小数位的情况，不显示标点
			if (iDecimalDigits == 0)
				iDecimalDigits--;
			int	nDigitLen = varWholeDigits.intVal - iDecimalDigits - 1;

			VariantClear(pvaOutValue);
			pvaOutValue->vt = VT_BSTR;
			
			OLECHAR *pChar = pvaInValue->bstrVal;
			while (*pChar != NULL)
			{
				if (*pChar == OLECHAR('.'))
					break;
				pChar++;
			}
			int nSourceLen = SysStringLen(pvaInValue->bstrVal); //源长度
			int nSourceDotPosition = pChar - pvaInValue->bstrVal; //源小数点位置
			int nSourceDigitLen = nSourceDotPosition; //源整数位数

			pChar = pvaInValue->bstrVal;
			p1 = pChar + max(0, nSourceDigitLen - nDigitLen);
			p2 = pChar + min(nSourceDotPosition + iDecimalDigits + 1, nSourceLen);
			
			//左对齐
			if (m_iJustify == 0)
			{
				pvaOutValue->bstrVal = SysAllocStringLen(p1, p2 - p1);
				return pvaOutValue->bstrVal ? S_OK : E_OUTOFMEMORY;
			}
			else 
			{
				//计算需要补足的空格数
				int n;
				//中间对齐
				if (m_iJustify == 1)
					n = int((varWholeDigits.intVal - (p2 - p1)) / 2);
				//右对齐
				else 
					n = varWholeDigits.intVal - (p2 - p1);	

				BSTR b = SysAllocStringLen(NULL, p2 - p1 + n);
				if (b == NULL)
					return E_OUTOFMEMORY;
				pvaOutValue->bstrVal = b;
				//补足空格
				while (n--)
				{	
					*b = OLECHAR(' ');
					b++;
				}
				memcpy(b, p1, (p2 - p1) * sizeof(OLECHAR));
				return S_OK;
			}
		}
		break;
	//字符-数字
	case 2:
		{
			VariantClear(pvaOutValue);
			pvaOutValue->vt = VT_BSTR;
			pvaOutValue->bstrVal = NULL;

			int nSourceLen = SysStringLen(pvaInValue->bstrVal); //源长度

			//计算实际行数及最后一行字符数
			int nLastLineChars = nSourceLen % varDecimalDigits.intVal; //最后一行字符数
			int nLines = int(nSourceLen / varDecimalDigits.intVal); //实际行数
			if (nLastLineChars != 0) //没有整除
				nLines++;
			else
				nLastLineChars = varDecimalDigits.intVal;
			if (nLines > varWholeDigits.intVal)
			{
				nLines = varWholeDigits.intVal;
				nLastLineChars = varDecimalDigits.intVal;
			}

			//计算需要分配的字符数

			int nSize = (nLines - 1) * (varDecimalDigits.intVal + 1); //加换行字符
			if (m_iJustify == 0)
				nSize += nLastLineChars;
			else if (m_iJustify == 1)
				nSize = nSize + nLastLineChars + (varDecimalDigits.intVal - nLastLineChars) / 2;
			else
				nSize += varDecimalDigits.intVal;

			//分配字符
			BSTR b = SysAllocStringLen(NULL, nSize);
			if (b == NULL)
				return E_OUTOFMEMORY;

			OLECHAR* p1 = b;
			OLECHAR* p2 = pvaInValue->bstrVal;
			//前面的满行
			for (int i = 0; i < nLines - 1; i++)
			{
				memcpy(p1, p2, sizeof(OLECHAR) * varDecimalDigits.intVal);
				p2 += varDecimalDigits.intVal;
				p1 += varDecimalDigits.intVal;
				*p1 = OLECHAR('\n'); //加换行符
				p1++;
			}

			//补足最后一行空格
			OLECHAR* p3 = b + nSize - nLastLineChars;
			while (p1 < p3)
			{	*p1 = OLECHAR(' ');
				p1++;
			}

			//最后的字符串
			memcpy(p1, p2, sizeof(OLECHAR) * nLastLineChars);
			pvaOutValue->bstrVal = b;
		}

		break;
	}

	return S_OK;
}

STDMETHODIMP CFormatDynamic::get_Format(BSTR *pVal)
{
	*pVal = m_bstrFormat.Copy();

	return S_OK;
}

STDMETHODIMP CFormatDynamic::put_Format(BSTR newVal)
{
	m_bstrFormat = newVal;

	return S_OK;
}

STDMETHODIMP CFormatDynamic::get_SourceDataType(int *pVal)
{
	*pVal = m_iDataType;

	return S_OK;
}

STDMETHODIMP CFormatDynamic::put_SourceDataType(int newVal)
{
	m_iDataType = newVal;

	return S_OK;
}

STDMETHODIMP CFormatDynamic::SetNumericFormat(VARIANT vaWholeDigits, VARIANT vaDecimalDigits, int iJustify)
{
	m_varWholeDigits.Copy(&vaWholeDigits);
	m_varWholeDigits.ChangeType(VT_I4);

	m_varDecimalDigits.Copy(&vaDecimalDigits);
	m_varDecimalDigits.ChangeType(VT_I4);

	m_iJustify = iJustify;

	return S_OK;
}

STDMETHODIMP CFormatDynamic::GetNumericFormat(VARIANT* pvaWholeDigits, VARIANT* pvaDecimalDigits, int *pJustify)
{
	VariantCopy(pvaWholeDigits, &m_varWholeDigits);
	VariantCopy(pvaDecimalDigits, &m_varDecimalDigits);
	*pJustify = m_iJustify;

	return S_OK;
}

STDMETHODIMP CFormatDynamic::Clone(IFormatDynamic **ppDynamic)
{
	HRESULT hr = ::CoCreateInstance(
		CLSID_FormatDynamic,         
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IFormatDynamic,
		reinterpret_cast<void**>(ppDynamic));
	if (FAILED(hr))
		return hr;

	(*ppDynamic)->put_Format(m_bstrFormat);
	(*ppDynamic)->put_Source(m_bstrSource);
	(*ppDynamic)->put_SourceDataType(m_iDataType);
	(*ppDynamic)->SetNumericFormat(m_varWholeDigits, m_varDecimalDigits, m_iJustify);\
	(*ppDynamic)->put_EnableMouseInput(m_bEnableMouseInput);
	
	return S_OK;
}

STDMETHODIMP CFormatDynamic::get_DisplaySample(BSTR *pVal)
{
	int iWholeDigits;
	int iDecimalDigits;

	switch (m_iDataType)
	{
	case 0://格式化
		{
			CComVariant var;
			Evaluation(NULL, &var);
			*pVal = var.bstrVal;
			var.vt = VT_EMPTY;
		}
		break;

	case 1: //数字
		if (m_varWholeDigits.vt == VT_I4)
			iWholeDigits = m_varWholeDigits.intVal;
		else
			iWholeDigits = 8;
		if (m_varDecimalDigits.vt == VT_I4)
			iDecimalDigits = m_varDecimalDigits.intVal;
		else
			iDecimalDigits = 2;

		*pVal = SysAllocStringLen(NULL, iWholeDigits);
		memset(*pVal, -1, iWholeDigits * 2);
		_wcsnset(*pVal, OLECHAR('#'), iWholeDigits);
		if (iDecimalDigits > 0)
			(*pVal)[iWholeDigits - iDecimalDigits - 1] = OLECHAR('.');
		break;
		
	case 2: //字符-数字
		{
			if (m_varWholeDigits.vt == VT_I4)
				iWholeDigits = m_varWholeDigits.intVal;
			else
				iWholeDigits = 1;
			if (m_varDecimalDigits.vt == VT_I4)
				iDecimalDigits = m_varDecimalDigits.intVal;
			else
				iDecimalDigits = 10;
			
			int nTotal = (iDecimalDigits + 1) * iWholeDigits - 1;
			*pVal = SysAllocStringLen(NULL, nTotal);
			memset(*pVal, -1, nTotal * 2);
			_wcsnset(*pVal, OLECHAR('?'), nTotal);
			int nLines = iWholeDigits;
			while (--nLines)
				(*pVal)[nLines * (iDecimalDigits + 1) - 1] = OLECHAR('\n');
		}
		break;
	}

	return S_OK;
}

STDMETHODIMP CFormatDynamic::get_EnableMouseInput(BOOL *pVal)
{
	*pVal = m_bEnableMouseInput;
	
	return S_OK;
}

STDMETHODIMP CFormatDynamic::put_EnableMouseInput(BOOL newVal)
{
	m_bEnableMouseInput = newVal;
	
	return S_OK;
}
