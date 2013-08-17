// TextTool.cpp : Implementation of CTextTool
#include "stdafx.h"
#include "CxDrawTools.h"
#include "TextTool.h"

/////////////////////////////////////////////////////////////////////////////
// CTextTool

STDMETHODIMP CTextTool::DoChar(UINT nChar,UINT nRepCnt,UINT nFlags)
{
	if (m_pDrawTxt == NULL)
		return S_OK;

	USES_CONVERSION;

	HideCaret(m_hWindow);
	if (nChar == 8)
	{
		if (strlen(m_TextBuf) > 1 )
		{
			if (IsDBCSLeadByte(m_TextBuf[strlen(m_TextBuf) - 1]) && IsDBCSLeadByte(m_TextBuf[strlen(m_TextBuf) - 2]))
			{
				m_TextBuf[strlen(m_TextBuf) - 1] = '\0';
			}
			m_TextBuf[strlen(m_TextBuf) - 1] = '\0';
		}
		else if (strlen(m_TextBuf) > 0 )
			m_TextBuf[0] = '\0';
	}
	else if(strlen(m_TextBuf) <= STR_MAX_LENGTH - 2 && (nChar > 31  || nChar == 13))
	{
		if (nChar == 13)
		{
			m_TextBuf[strlen(m_TextBuf)] = 13;
			m_TextBuf[strlen(m_TextBuf)] = 10;
		}
		else
		{
			m_TextBuf[strlen(m_TextBuf)] = nChar;
		}
	}

	m_pDrawTxt->put_Caption(T2BSTR(m_TextBuf));
	ShowCaret(m_hWindow);
	RePosCaret();

	return S_OK;
}

STDMETHODIMP CTextTool::DoMouseMove(UINT nFlags, POINT point)
{
	if (m_pDrawTxt)
	{
//		m_pDrawTxt->put_HorizontalPosition((float)point.x + 4);
//		m_pDrawTxt->put_VerticalPosition((float)point.y - 8);
	}

	SetCursor(LoadCursor(NULL, IDC_IBEAM));

	return S_OK;
}

STDMETHODIMP CTextTool::DoLButtonDown(UINT nFlags, POINT point)
{
	if (m_pDrawTxt == NULL)
	{
		IUnknown* pUnknown = NULL;
		m_pDrawToolSite->GetDrawObject(CLSID_DrawTxt, &pUnknown);
	
		HRESULT hr = pUnknown->QueryInterface(IID_IDrawTxt,
			(void**)&m_pDrawTxt);

//		CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> spDrawObject(pUnknown);
//		RECTF rcPos;
//		spDrawObject->GetPositionRect(&rcPos);
//		float fXOffset = point.x - (rcPos.right + rcPos.left) / 2;
//		float fYOffset = (rcPos.bottom - rcPos.top) / 2;

		m_pDrawTxt->put_HorizontalPosition((float)point.x);
		m_pDrawTxt->put_VerticalPosition((float)point.y - 7.0f);

		m_pDrawToolSite->GetWindow(&m_hWindow);
		CreateCaret(m_hWindow, NULL, 2, 16);
   		ShowCaret(m_hWindow);
		RePosCaret();
		CaptureMouse();
		SetCursor(LoadCursor(NULL, IDC_IBEAM));
	}
	else
	{
		HideCaret(m_hWindow);
		DestroyCaret();

		if (strlen(m_TextBuf) == 0) 
			Fire_OnCancel();
		else
			Fire_OnCreate();
		ReleaseMouse();
		m_pDrawTxt->Release();
		m_pDrawTxt = NULL;
		ZeroMemory(m_TextBuf, STR_MAX_LENGTH);
	}
	return S_OK;
}

STDMETHODIMP CTextTool::DoCancel()
{
	HideCaret(m_hWindow);
	DestroyCaret();

	if (m_pDrawTxt == NULL)
		return S_OK;
	
	ReleaseMouse();
	Fire_OnCancel();
	m_pDrawTxt->Release();
	m_pDrawTxt = NULL;
	ZeroMemory(m_TextBuf, STR_MAX_LENGTH);

	return S_OK;
}

STDMETHODIMP CTextTool::DoDraw(HDC hdcDest)
{
	if (m_pDrawTxt == NULL)
		return S_OK;

//	float xpos, ypos;
//	m_pDrawTxt->get_HorizontalPosition(&xpos);
//	m_pDrawTxt->get_VerticalPosition(&ypos);
//	MoveToEx(hdcDest, (int)xpos, (int)ypos, NULL);
//	LineTo(hdcDest, (int)xpos + 16, (int)ypos);

	return S_OK;
}

void CTextTool::RePosCaret()
{
	long xpos, ypos;
	m_pDrawTxt->GetCharPosition(-1, &xpos, &ypos);
	POINT point;
	point.x = xpos;
	point.y = ypos;
	m_pDrawToolSite->DocToClient(&point);
	xpos = point.x;
	ypos = point.y;
	SetCaretPos(xpos, ypos);
}
