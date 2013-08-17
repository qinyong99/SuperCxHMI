// ComboBoxBold.h : header file
//
////////////////////////////////////////////////////////////////////////////

#ifndef _COMBOBOXBOLD_H__
#define _COMBOBOXBOLD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CComboBoxBold window

class CComboBoxBold : 
	public CWindowImpl<CComboBoxBold, CComboBox>,
	public COwnerDraw<CComboBoxBold>
{
// Construction
public:
	CComboBoxBold();
	virtual ~CComboBoxBold();

	BEGIN_MSG_MAP(CComboBoxBold)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CComboBoxBold>, 0)
	END_MSG_MAP()

// Operations
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetIcon(int iconId);
	void SetItemBold(int iItem, BOOL bBold);

	int SelectStringExact(int nStartAfter, LPCTSTR lpszString);
	
private:
	HICON m_hIcon;
	CFont m_font;
	CFont m_fontBold;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _COMBOBOXBOLD_H__
