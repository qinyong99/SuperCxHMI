/********************************************************************
	created:	2003/04/12
	created:	12:04:2003   10:50
	file base:	PPTooltip
	file ext:	h
	author:		Eugene Pustovoyt
	
	purpose:	
*********************************************************************/

#ifndef _PPTOOLTIP_H
#define _PPTOOLTIP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPToolTip.h : header file
//
#define PPTOOLTIP_USE_SHADE

#ifdef PPTOOLTIP_USE_SHADE
#include "CeXDib.h"
#endif

#define PPTOOLTIP_CLASSNAME    _T("CPPTooltip")  // Window class name

#define UDM_TOOLTIP_FIRST		   (WM_USER + 100)
#define UDM_TOOLTIP_DISPLAY		   (UDM_TOOLTIP_FIRST) //User was changed the data

//The styles
#define PPTOOLTIP_ANCHOR				0x00000001
#define PPTOOLTIP_SHADOW				0x00000002
#define PPTOOLTIP_ROUNDED				0x00000004
#define PPTOOLTIP_BALLOON				0x00000007
#define PPTOOLTIP_VCENTER_ALIGN			0x00000008
#define PPTOOLTIP_BOTTOM_ALIGN			0x00000010
#define PPTOOLTIP_ICON_VCENTER_ALIGN	0x00000020
#define PPTOOLTIP_ICON_BOTTOM_ALIGN		0x00000040
#define PPTOOLTIP_SHOW_INACTIVE			0x00000100 //Show tooltip for inacive window
#define PPTOOLTIP_SHOW_DISABLED			0x00000200 //Show tooltip for disabled control

//The behaviours
#define PPTOOLTIP_MULTIPLE_SHOW			0x00000001 //Multiple show for single control
#define PPTOOLTIP_TRACKING_MOUSE		0x00000002 //Tracking for mouse
#define PPTOOLTIP_CLOSE_LEAVEWND		0x00000004 //Close tooltip if mouse leave the control
#define PPTOOLTIP_NOCLOSE_OVER			0x00000008 //No close tooltip if mouse over him
#define PPTOOLTIP_DISABLE_AUTOPOP		0x00000010 //Disables autopop tooltip from timer

//#define PPTOOLTIP_NO_CLOSE_OVER		0x00000002 //No closes tooltip if the mouse over the control
//#define PPTOOLTIP_NO_CLOSE_MOVE		0x00000004 //No closes for any mouse's move.
//#define PPTOOLTIP_TRACK_MOUSE			0x00000008 //Tracking window for the mouse's move

//The masks
#define PPTOOLTIP_MASK_STYLES			0x0001	// The styles for the tooltip gets from the structures
#define PPTOOLTIP_MASK_EFFECT			0x0002	// The background's type for the tooltip gets from the structures
#define PPTOOLTIP_MASK_COLORS			0x0004	// The background's colors for the tooltip gets from the structures
#define PPTOOLTIP_MASK_DIRECTION		0x0008  // The align for the tooltip gets from the structures
#define PPTOOLTIP_MASK_BEHAVIOUR		0x0010  // The behaviour for the tooltip gets from the structures
//#define PPTOOLTIP_MASK_GRANULARITY		0x0010	// The effect's granularity

// This info structure
typedef struct tagPPTOOLTIP_INFO
{
    HWND		hWnd;			// Window handle of the control
    UINT		nIDTool;		// ID of tool   
    CRect		rectBounds;		// Bounding rect for toolinfo to be displayed
	HICON		hIcon;			// The icon of the tooltip
	CString		sTooltip;		// The string of the tooltip
	UINT        nMask;			// The mask 
	UINT		nStyles;		// The tooltip's styles
	UINT        nDirection;		// Direction display the tooltip relate cursor point
	UINT		nEffect;		// The color's type or effects
	UINT        nBehaviour;		// The tooltip's behaviour
	BYTE        nGranularity;	// The effect's granularity
	COLORREF	crBegin;		// Begin Color
	COLORREF    crMid;			// Mid Color
	COLORREF	crEnd;			// End Color
} PPTOOLTIP_INFO;

typedef struct tagPPTOOLTIP_NAME_RES
{
	CString sName;	// Name of the resource (IDB_ - Bitmap, IDI_ - Icon)
	UINT nID;		// ID of the resource 
	BYTE nTypeRes;	// TYPE_RES_ICON = Icon, TYPE_RES_BITMAP = bitmap, TYPE_RES_TRAN_BITMAP = transparent bitmap
	COLORREF crMask;// Color mask of the resource 
} PPTOOLTIP_NAME_RES;

// This structure sent to PPToolTip parent in a WM_NOTIFY message
typedef struct tagNM_PPTOOLTIP_DISPLAY {
    NMHDR hdr;
	CPoint * pt;
	PPTOOLTIP_INFO * ti;
} NM_PPTOOLTIP_DISPLAY;

/////////////////////////////////////////////////////////////////////////////
// CPPToolTip window

class CPPToolTip : public CWnd
{
// Construction
public:
	virtual BOOL Create(CWnd* pParentWnd, BOOL bBalloonSize = TRUE);
	CPPToolTip();
	virtual ~CPPToolTip();

// Attributes
public:
	enum {	PPTTSZ_ROUNDED_CX = 0,
			PPTTSZ_ROUNDED_CY,
			PPTTSZ_MARGIN_CX,
			PPTTSZ_MARGIN_CY,
			PPTTSZ_SHADOW_CX,
			PPTTSZ_SHADOW_CY,
			PPTTSZ_WIDTH_ANCHOR,
			PPTTSZ_HEIGHT_ANCHOR,
			PPTTSZ_MARGIN_ANCHOR,
			PPTTSZ_BORDER_CX,
			PPTTSZ_BORDER_CY,

			PPTTSZ_MAX_SIZES
		};

	enum {	TYPE_RES_ICON = 0,
			TYPE_RES_BITMAP,
			TYPE_RES_MASK_BITMAP,

			MAX_TYPES_RES
		};

	enum {	PPTOOLTIP_COLOR_0 = 0,
			PPTOOLTIP_COLOR_1,
			PPTOOLTIP_COLOR_2,
			PPTOOLTIP_COLOR_3,
			PPTOOLTIP_COLOR_4,
			PPTOOLTIP_COLOR_5,
			PPTOOLTIP_COLOR_6,
			PPTOOLTIP_COLOR_7,
			PPTOOLTIP_COLOR_8,
			PPTOOLTIP_COLOR_9,
			PPTOOLTIP_COLOR_10,
			PPTOOLTIP_COLOR_11,
			PPTOOLTIP_COLOR_12,
			PPTOOLTIP_COLOR_13,
			PPTOOLTIP_COLOR_14,
			PPTOOLTIP_COLOR_15,
			PPTOOLTIP_COLOR_16,
			PPTOOLTIP_COLOR_FG,
			PPTOOLTIP_COLOR_BK_BEGIN,
			PPTOOLTIP_COLOR_BK_MID,
			PPTOOLTIP_COLOR_BK_END,
			PPTOOLTIP_COLOR_LINK,		// Color for hyperlink text
			PPTOOLTIP_COLOR_VISITED,	// Color for visited hyperlink text
			PPTOOLTIP_COLOR_HOVER,		// Color for hover hyperlink text
			PPTOOLTIP_COLOR_SHADOW,		// Color for the shadow
			PPTOOLTIP_COLOR_BORDER,		// Color for border of the tooltip

			PPTOOLTIP_MAX_COLORS
		};

	enum {	PPTOOLTIP_LEFT_TOP = 0,
			PPTOOLTIP_RIGHT_TOP,
			PPTOOLTIP_LEFT_BOTTOM,
			PPTOOLTIP_RIGHT_BOTTOM,

			PPTOOLTIP_MAX_DIRECTIONS
		};

	enum {	PPTOOLTIP_EFFECT_SOLID = 0,
			PPTOOLTIP_EFFECT_HGRADIENT,
			PPTOOLTIP_EFFECT_VGRADIENT,
			PPTOOLTIP_EFFECT_HCGRADIENT,
			PPTOOLTIP_EFFECT_VCGRADIENT,
			PPTOOLTIP_EFFECT_3HGRADIENT,
			PPTOOLTIP_EFFECT_3VGRADIENT,
#ifdef PPTOOLTIP_USE_SHADE
			PPTOOLTIP_EFFECT_NOISE,
			PPTOOLTIP_EFFECT_DIAGSHADE,
			PPTOOLTIP_EFFECT_HSHADE,
			PPTOOLTIP_EFFECT_VSHADE,
			PPTOOLTIP_EFFECT_HBUMP,
			PPTOOLTIP_EFFECT_VBUMP,
			PPTOOLTIP_EFFECT_SOFTBUMP,
			PPTOOLTIP_EFFECT_HARDBUMP,
			PPTOOLTIP_EFFECT_METAL,
#endif
			PPTOOLTIP_MAX_EFFECTS
		};

protected:
	enum {	PPTOOLTIP_SHOW = 0x100, //the identifier of the timer for show the tooltip
			PPTOOLTIP_HIDE = 0x101  //the identifier of the timer for hide the tooltip
		};

	enum {	PPTOOLTIP_TOOL_NOEXIST = -1, //the current tool is not exist
			PPTOOLTIP_TOOL_HELPER = -2   //the current tool is a helper
		};

//	typedef CArray<PPTOOLTIP_HOTRECT, PPTOOLTIP_HOTRECT&> m_arrToolsWnd;
//	CMap <HWND, HWND, m_arrToolsWnd, m_arrToolsWnd&> m_mapTools;
	CArray<PPTOOLTIP_INFO, PPTOOLTIP_INFO> m_arrTools; //Tools Archive
	CArray<PPTOOLTIP_NAME_RES, PPTOOLTIP_NAME_RES> m_arrNameRes; //Names of the resource

	HWND   m_hNotifyWnd; // Handle to window for notification about change data
	CWnd * m_pParentWnd; // The pointer to the parent window
	UINT m_nLastDirection;
	CUIntArray m_nLengthLines; //The real length of the lines in the tooltip
	CUIntArray m_nHeightLines; //The real heigth of the lines in the tooltip

	int	m_nIndexCurrentWnd;
	int m_nIndexDisplayWnd;

    LOGFONT m_LogFont;                  // Current font in use

	//Default setting
	COLORREF m_crColor [PPTOOLTIP_MAX_COLORS]; //The indexing colors
	UINT m_nSizes [PPTTSZ_MAX_SIZES]; //All sizes 
	UINT m_nStyles;
	UINT m_nDirection;
	UINT m_nEffect;
	UINT m_nGranularity;
	UINT m_nBehaviour;	 //The tooltip's behaviour 

	UINT m_nTimeAutoPop; //Retrieve the length of time the tool tip window remains visible if the pointer is stationary within a tool's bounding rectangle
	UINT m_nTimeInitial; //Retrieve the length of time the pointer must remain stationary within a tool's bounding rectangle before the tool tip window appears

	//The properties of the current tooltip
	CPoint  m_ptOriginal;

	CRgn	m_rgnToolTip;
	CRgn    m_rgnShadow;

	CSize m_szToolIcon; //the size of the current icon
	CSize m_szTextTooltip; //the size of the tooltip's text

	CImageList m_imgTooltip; //the image list 
	CSize m_szImage;

	PPTOOLTIP_INFO m_pToolInfo; //info of the current tooltip

#ifdef PPTOOLTIP_USE_SHADE
	void SetShade(CRect rect, UINT shadeID = 0, BYTE granularity = 8, BYTE coloring = 0, COLORREF color = 0);
	CCeXDib m_dNormal,m_dh,m_dv;
#endif


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPToolTip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	//Functions for the styles
	void  SetStyles(DWORD nStyles, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Sets New Style
	void  ModifyStyles(DWORD nAddStyles, DWORD nRemoveStyles, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Modifies styles
	DWORD GetStyles(int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Gets current Styles
	void  SetDefaultStyles(int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Sets default styles

	//functions for color
	void SetColor(int nIndex, COLORREF crColor); //Sets the color
	COLORREF GetColor(int nIndex); //Gets the color
	void SetDefaultColors(); //Sets default colors
	void SetGradientColors(COLORREF crBegin, COLORREF crMid, COLORREF crEnd, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Sets the gradient's colors
	void GetGradientColors(COLORREF & crBegin, COLORREF & crMid, COLORREF & crEnd, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST); //Gets the gradient's colors

	//functions for tool
	void AddTool(CWnd * pWnd, UINT nIdText, HICON hIcon = NULL, LPCRECT lpRectTool = NULL, UINT nIDTool = 0); //Adds tool
	void AddTool(CWnd * pWnd, UINT nIdText, UINT nIdIcon, CSize szIcon = CSize(0, 0), LPCRECT lpRectTool = NULL, UINT nIDTool = 0); //Adds tool
	void AddTool(CWnd * pWnd, CString sTooltipText, HICON hIcon = NULL, LPCRECT lpRectTool = NULL, UINT nIDTool = 0); //Adds tool
	void AddTool(CWnd * pWnd, CString sTooltipText, UINT nIdIcon, CSize szIcon = CSize(0, 0), LPCRECT lpRectTool = NULL, UINT nIDTool = 0); //Adds tool
	void AddTool(PPTOOLTIP_INFO ti); //Adds tool
	int  FindTool(CPoint & pt);
	int  FindTool(CWnd * pWnd, LPCRECT lpRect = NULL);
	int  FindTool(UINT nIDTool);
	BOOL GetTool(int nIndexTool, PPTOOLTIP_INFO & ti);
	BOOL RemoveTool(int nIndexTool);  //Removes specified tool
	void RemoveAllTools(); // Removes all tools
	BOOL IsExistTool(int nIndexTool);
	void SetAtTool(int nIndexTool, PPTOOLTIP_INFO & ti);

	//Functions for resources name
	void AddNameOfResource(CString sName, UINT nID, BYTE nTypeRes = TYPE_RES_MASK_BITMAP, COLORREF crMask = RGB(255, 0, 255));
	int  FindIdOfResource(CString sName);
	int  FindNameOfResource(UINT nID);
	void RemoveNameOfResource(int nIndex);
	void RemoveAllNamesOfResource();

	//Functions for images
	void SetImageList(UINT nIdBitmap, int cx, int cy, int nCount, COLORREF crMask = RGB(255, 0, 255));
	void SetImageList(HBITMAP hBitmap, int cx, int cy, int nCount, COLORREF crMask = RGB(255, 0, 255));
	CImageList * GetImageList(CSize & sz);

	//Functions for mask
	void  SetMaskTool(int nIndexTool, UINT nMask = 0);
	void  ModifyMaskTool(int nIndexTool, UINT nAddMask, UINT nRemoveMask);
	UINT  GetMaskTool(int nIndexTool);

	//functions for effects
	void SetEffectBk(UINT nEffect, BYTE nGranularity = 2, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);
	UINT GetEffectBk(int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);
	UINT GetEffectBk(BYTE & nGranularity, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);

	//functions for
	void  SetNotify(HWND hWnd);
	void  SetNotify(BOOL bParentNotify = TRUE);
	BOOL  GetNotify(); //Is enabled notification

	//functions for delay
	void SetDelayTime(DWORD dwDuration, UINT nTime);
	UINT GetDelayTime(DWORD dwDuration) const;

	//functions for sizes
	void SetSize(int nSizeIndex, UINT nValue);
	UINT GetSize(int nSizeIndex);
	void SetDefaultSizes(BOOL bBalloonSize = TRUE);

	//functions for direction
	void SetDirection(UINT nDirection = PPTOOLTIP_RIGHT_BOTTOM, int nIndex = PPTOOLTIP_TOOL_NOEXIST);
	UINT GetDirection(int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);

	// 
	void SetBehaviour(UINT nBehaviour = 0, int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);
	UINT GetBehaviour(int nIndexTool = PPTOOLTIP_TOOL_NOEXIST);
//	void SetMultipleShow(BOOL bMultiple = TRUE, CWnd * pWnd = NULL);
//	BOOL IsMultipleShow(CWnd * pWnd = NULL);

	//Functions for the fonts
	BOOL  SetFont(CFont & font); //set font
	BOOL  SetFont(LPLOGFONT lf); //set font
	BOOL  SetFont(LPCTSTR lpszFaceName, int nSizePoints = 8,
									BOOL bUnderline = FALSE, BOOL bBold = FALSE,
									BOOL bStrikeOut = FALSE, BOOL bItalic = FALSE); //set font
	void  SetDefaultFont(); //set default fonts
	void  GetFont(CFont & font);
	void  GetFont(LPLOGFONT lf);

	void RelayEvent(MSG* pMsg);

	//The help tooltip functions
	void ShowHelpTooltip(CPoint & pt, UINT nIdText, HICON hIcon = NULL); //show tooltip as helper
	void ShowHelpTooltip(CPoint & pt, UINT nIdText, UINT nIdIcon); //show tooltip as helper
	void ShowHelpTooltip(CPoint & pt, CString sTooltipText, HICON hIcon = NULL); //show tooltip as helper
	void ShowHelpTooltip(CPoint & pt, CString sTooltipText, UINT nIdIcon); //show tooltip as helper
	void ShowHelpTooltip(CPoint & pt, PPTOOLTIP_INFO & ti); //show tooltip as helper

	static short GetVersionI()		{return 0x14;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.4");}
	// Generated message map functions
protected:
	void KillTimers(UINT nIDTimer = NULL);
	
	CSize PrintTitleString(CDC * pDC, CRect rect, CString str, BOOL bCalculate = TRUE); //Prints the title string
	void  DrawHorzLine(CDC * pDC, int xStart, int xEnd, int y) const;

	void PrepareDisplayToolTip(CPoint * pt); //Notifying parent, Determines the size of tooltip's rect
	void DisplayToolTip(CPoint * pt, CRect * rect);
	
	void SetNewToolTip(int nIndexTool, BOOL bWithDelay = TRUE);
	void Pop();

	BOOL IsEnabledIndexTool(int nIndex);
	BOOL IsCursorInToolTip() const;
	inline BOOL IsVisible() const { return ((GetStyle() & WS_VISIBLE) == WS_VISIBLE); }

	CSize GetTooltipSize(CString str); //Gets max rectangle for display tooltip text
	CSize GetSizeIcon(HICON hIcon) const;
	void CalculateInfoBoxRect(CPoint * pt, CRect * rect);

	CString SearchBeginOfTag(CString & str, int & nIndex);
	void SearchEndOfTag(CString & str, int & nIndex);
	CString GetNameOfTag(CString & str, int & nIndex);
	CString GetPropertiesOfTag(CString & str, int & nIndex);
	CString GetStringValue(CString & str, int & nIndex);
	UINT GetUIntValue(CString & str, int & nIndex, UINT nDefValue);

	LPLOGFONT GetSystemToolTipFont() const;

	int   GetNextHorizDirection(int nDirection) const;
	int   GetNextVertDirection(int nDirection) const;
	BOOL  TestHorizDirection(int x, int cx, int w_cx, int nDirection, LPRECT rect) const;
	BOOL  TestVertDirection(int y, int cy, int w_cy, int nDirection, LPRECT rect) const;

	CRect GetWindowRegion(CRgn * rgn, CSize sz, CPoint pt);
	void  FillGradient(CDC * pDC, CRect rect, COLORREF colorStart, COLORREF colorFinish, BOOL bHorz = TRUE);

	LRESULT SendNotify(CPoint * pt, PPTOOLTIP_INFO & ti);

	void  OnRedrawClient(CDC * pDC);
	void  OnDraw(CDC * pDC, CRect rect);
	void  OnDrawBackground(CDC * pDC, CRect * pRect);
	void  OnDrawShadow(CDC * pDC);
		
	CSize DrawIconFromImageList(CDC * pDC, CPoint pt, CSize sz, CImageList & img, int nIndex = 0, BOOL bCalculate = TRUE);
	CSize DrawResource(CString sName, CDC * pDC, CPoint pt, int nMaxHeight, CSize szResource, BOOL bCalculate);
	CSize DrawBitmap(CDC * pDC, CPoint pt, int nMaxHeight, UINT nID, BOOL bUseMask, COLORREF crMask, CSize szBitmap, BOOL bCalculate);
	CSize DrawIcon(CDC * pDC, CPoint pt, int nMaxHeight, UINT nID, CSize szIcon, BOOL bCalculate);

	HWND GetWndFromPoint(CPoint & pt, BOOL bGetDisabled = TRUE) const;

//	HICON GetIconFromResources(CString sResName, CSize szIcon = CSize(0, 0)) const;
	HICON GetIconFromResources(UINT nID, CSize szIcon = CSize(0, 0)) const;
	HBITMAP GetBitmapFromResources(UINT nID) const;

	//{{AFX_MSG(CPPToolTip)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif










































