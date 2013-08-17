// PipeGenPage.cpp : Implementation of CPipeGenPage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "PipeGenPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPipeGenPage

LRESULT CPipeGenPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_editRadius.Attach(GetDlgItem(IDC_RADIUS));
	m_btnRound.Attach(GetDlgItem(IDC_ROUND));
	m_btnLightColor.SubclassWindow(GetDlgItem(IDC_LIGHT_COLOR));
	m_btnDarkColor.SubclassWindow(GetDlgItem(IDC_DARK_COLOR));
	m_btnFlow.Attach(GetDlgItem(IDC_FLOW));
	m_btnFlowDirection.Attach(GetDlgItem(IDC_FLOW_DIRECTION));
	m_btnLiquidColor.SubclassWindow(GetDlgItem(IDC_LIQUID_COLOR));
	m_cmbFlowSpeed.Attach(GetDlgItem(IDC_FLOW_SPEED));
	
	m_btnLightColor.SetDefaultColor(RGB(255, 255, 255));
	m_btnDarkColor.SetDefaultColor(RGB(128, 128, 128));
	m_btnLiquidColor.SetDefaultColor(RGB(0, 0, 255));
	
	m_cmbFlowSpeed.ResetContent();
	m_cmbFlowSpeed.InsertString(0, _T("静止"));
	m_cmbFlowSpeed.InsertString(1, _T("低速"));
	m_cmbFlowSpeed.InsertString(2, _T("中低速"));
	m_cmbFlowSpeed.InsertString(3, _T("中速"));
	m_cmbFlowSpeed.InsertString(4, _T("中高速"));
	m_cmbFlowSpeed.InsertString(5, _T("高速"));
	
	m_bLoadPropery = TRUE;
	
	if (m_nObjects > 0)
	{
		CComQIPtr<IDrawPipe, &IID_IDrawPipe> spPipe(m_ppUnk[0]);
		
		TCHAR szBuf[8];
		
		float f;
		spPipe->get_EdgeWidth((float *)&f);
		sprintf(szBuf, "%.2f", f / 2);
		m_editRadius.SetWindowText(szBuf);
		
		long l;
		spPipe->get_JoinStyle((enumLineJoinStyle *)&l);
		m_btnRound.SetCheck(l == LineJoinStyleRound);
		
		OLE_COLOR clr;
		spPipe->get_HighLightEdgeColor(&clr);
		m_btnLightColor.SetColor(clr);
		
		spPipe->get_EdgeColor(&clr);
		m_btnDarkColor.SetColor(clr);
		
		VARIANT_BOOL b;
		spPipe->get_Flow(&b);
		m_btnFlow.SetCheck(b == VARIANT_TRUE);
		
		spPipe->get_FlowDirection((enumFlowDirection *)&l);
		m_btnFlowDirection.SetCheck(l == FlowDirectionForward);
		
		spPipe->get_LiquidColor(&clr);
		m_btnLiquidColor.SetColor(clr);
		
		spPipe->get_FlowSpeed((enumFlowSpeed *)&l);
		m_cmbFlowSpeed.SetCurSel(l);
	}
	
	m_bLoadPropery = FALSE;
	
	return 0;
}

STDMETHODIMP CPipeGenPage::Apply()
{
	for (UINT i = 0; i < m_nObjects; i++)
	{
		CComQIPtr<IDrawPipe, &IID_IDrawPipe> spPipe(m_ppUnk[i]);
		
		CComBSTR bstr;
		m_editRadius.GetWindowText((BSTR&)bstr);
		CComVariant var = bstr;
		var.ChangeType(VT_R4);
		if (var.vt == VT_R4)
			spPipe->put_EdgeWidth(float(var.fltVal * 2));
		
		spPipe->put_JoinStyle(m_btnRound.GetCheck() ? LineJoinStyleRound : LineJoinStyleSquare);
		spPipe->put_HighLightEdgeColor(m_btnLightColor.GetColor());
		spPipe->put_EdgeColor(m_btnDarkColor.GetColor());
		spPipe->put_Flow(m_btnFlow.GetCheck());
		spPipe->put_FlowDirection(m_btnFlowDirection.GetCheck() ? FlowDirectionForward : FlowDirectionBackward);
		spPipe->put_LiquidColor(m_btnLiquidColor.GetColor());
		
		int iIndex = m_cmbFlowSpeed.GetCurSel();
		spPipe->put_FlowSpeed((enumFlowSpeed)iIndex);
		
	}
	m_bDirty = FALSE;
	
	return S_OK;
}
