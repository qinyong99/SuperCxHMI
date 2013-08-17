// DataLinkTool.cpp : Implementation of CDataLinkTool
#include "stdafx.h"
#include "Dtguid.h"
#include "CxDrawTools.h"
#include "DataLinkTool.h"

#include "../CxDynObjs/PropInfo.h"
#include "../CxDynObjs/CxDynObjs.h"
#include "../CxDynObjs/CxDynObjs_i.c"

const CLSID CLSID_CxDataLinkGenPage = {0x08622C0C,0xCC20,0x4730,{0x81,0xFC,0x77,0x00,0xEE,0xCF,0x16,0x58}};

/////////////////////////////////////////////////////////////////////////////
// CDataLinkTool

STDMETHODIMP CDataLinkTool::DoMouseMove(UINT nFlags, POINT point)
{
	if (m_hCursor == NULL)
		m_hCursor = LoadCursor(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDC_HANDLE));

	SetCursor(m_hCursor);
	
	return S_OK;
}

STDMETHODIMP CDataLinkTool::DoLButtonDown(UINT nFlags, POINT point)
{
	IUnknown* pUnknown = NULL;
	HRESULT hr = m_pDrawToolSite->GetDrawObject(CLSID_DrawDataLink, &pUnknown);
	if (FAILED(hr))
		return hr;

	// 移动到指定位置
	CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> spDrawObject(pUnknown);
	RECTF rcPos;
	spDrawObject->GetPositionRect(&rcPos);
	float fXOffset = point.x - (rcPos.right + rcPos.left) / 2;
	float fYOffset = point.y - (rcPos.bottom + rcPos.top) / 2 + 1;
	rcPos.left += fXOffset;
	rcPos.right += fXOffset;
	rcPos.top += fYOffset;
	rcPos.bottom += fYOffset;
	spDrawObject->MoveTo(rcPos, FALSE);
 
	CComQIPtr<IDrawDataLink, &IID_IDrawDataLink> spDataLink(pUnknown);

	spDataLink->put_Value(m_bstrSample);
	spDataLink->ConnectObject(L"Value", m_spFormatDynamic);

	Fire_OnCreate();
	m_spFormatDynamic.Release();

	return S_OK;
}

STDMETHODIMP CDataLinkTool::DoCancel()
{
	Fire_OnCancel();
	m_spFormatDynamic.Release();
		
	return S_OK;
}

//const CLSID CLSID_FormatDynamic = {0x6DC9B1A9,0x0153,0x46C2,{0x90,0x0F,0xD6,0xC1,0x4F,0x05,0x7B,0x12}};
//const CLSID CLSID_CxDataLinkPage = {0xD75318D7,0xF2D8,0x4318,{0x87,0xBE,0xB7,0xC3,0xB9,0x36,0xA9,0xC9}};

#include "..\CxPropSheet\CxPropSheetExport.h"

STDMETHODIMP CDataLinkTool::DoInit()
{
	if (m_spFormatDynamic != NULL)
		m_spFormatDynamic.Release();

	// 创建关联的动态对象
	HRESULT hr = m_spFormatDynamic.CoCreateInstance(CLSID_FormatDynamic);
	if (FAILED(hr))
		return hr;

	CComQIPtr<ICxDynamicObject> spDynamicObject(m_spFormatDynamic);
	if (spDynamicObject != NULL)
	{
		ExpressionCreatorProc pfn = NULL;
		m_pDrawToolSite->GetExpressionCreatorProc(long(&pfn));

		CDynamicPropInfo propinfo;
		memset(&propinfo, 0, sizeof(propinfo));
		propinfo.pfnExpression = pfn;
		spDynamicObject->InitNew(long(&propinfo));
	}

	GUID guidPage = CLSID_CxDataLinkGenPage;
	IUnknown* pUnk = m_spFormatDynamic;

	if (S_OK != CxCreatePropertyFrame(::GetActiveWindow(), 0, 0, L"属性（DataLink）",
		1, &pUnk, 1,
		&guidPage, GetUserDefaultLCID(), 0, NULL))
	{
		DoCancel();
	}
	else
	{
		CComQIPtr<IFormatDynamic> spFormatDynamic(m_spFormatDynamic);
		
		spFormatDynamic->get_DisplaySample(&m_bstrSample);
	}


	return S_OK;
}

STDMETHODIMP CDataLinkTool::DoDraw(HDC hdcDest)
{
	

	return S_OK;
}

