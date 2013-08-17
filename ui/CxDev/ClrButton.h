//////////////////////////////////////////////////////////////////////
// ClrButton.h : header file

#if !defined(AFX_CLRBUTTON_H__C0E35BE5_80D5_463F_A1A1_FE74A2DF6C2A__INCLUDED_)
#define AFX_CLRBUTTON_H__C0E35BE5_80D5_463F_A1A1_FE74A2DF6C2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CClrButton window

#include "../CxComObj/CxComObj.h"

#define CPN_SELCHANGE        0x8000	/* Colour Picker Selection change */

struct NMCOLORBUTTON
{
	NMHDR hdr;
	BOOL fColorValid;
	COLORREF clr;
};

class CClrButton : public CComboBox
{
	// Construction
public:
	CClrButton();
	
	// Attributes
private:
	OLE_COLOR m_clr;
	OLE_COLOR m_clrDefault;
	
	// Operations
public:
	void SetColor(OLE_COLOR clr);
	OLE_COLOR GetColor();
	
	void SetDefaultColor (OLE_COLOR clrDefault);
	OLE_COLOR GetDefaultColor();

	void SendNotification (UINT nCode, COLORREF clr, BOOL bColorValid);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ClrButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CClrButton();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(ClrButton)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLRBUTTON_H__C0E35BE5_80D5_463F_A1A1_FE74A2DF6C2A__INCLUDED_)
