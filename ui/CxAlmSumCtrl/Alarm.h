// Alarm.h: interface for the CAlarm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ALARM_H_
#define __ALARM_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


typedef enum
{	
	AlarmStatus_OK = 0,
	AlarmStatus_LOLO = 1,
	AlarmStatus_HIHI = 2,
	AlarmStatus_LO = 3,
	AlarmStatus_HI = 4,
	AlarmStatus_RateOfChange = 5,
	AlarmStatus_ChangeOfState = 6,
	AlarmStatus_ChangeFromNormal = 7,
} enumAlarmStatus;

typedef enum
{	
	AlarmPriority_Low = 0,
	AlarmPriority_Midium = 1,
	AlarmPriority_High = 2,
} enumAlarmPriority;

typedef enum
{	
	AlarmField_Ack = 0,
	AlarmField_DateIn = 1,
	AlarmField_TimeIn = 2,
	AlarmField_DateOut = 3,
	AlarmField_TimeOut = 4,
	AlarmField_Node = 5,
	AlarmField_TagName = 6,
	AlarmField_Status = 7,
	AlarmField_Description = 8,
	AlarmField_Value = 9,
	AlarmField_Priority = 10,
	AlarmField_Area = 11,
	AlarmField_Extension1 = 12,
	AlarmField_Extension2 = 13,
} enumAlarmField;

#define NUM_STATUSS 11
#define NUM_PRIORITYS 3
#define NUM_FIELDS 14

static _TCHAR *_gszStatusLabel[NUM_STATUSS] =
{
	_T("正常"), _T("低低"),  _T("低"), _T("高"), _T("高高"),
	_T("小偏差"), _T("大偏差"), _T("变化率"), _T("开"), _T("关"), _T("改变"),
};

static _TCHAR *_gszPriorityLabel[NUM_PRIORITYS] =
{
	_T("低(0-333)"), _T("中(334-666)"), _T("高(667-999)"),
};

static _TCHAR *_gszColumnLabel[NUM_FIELDS] =
{
	_T("确认"), _T("开始日期"), _T("开始时间"), _T("确认日期"), _T("确认时间"),
	_T("节点"), _T("标签"), _T("状态"), _T("描述"),
	_T("值"), _T("优先级"), _T("报警区"), _T("扩展1"), _T("扩展2"),
	
};

static int _gnColumnWidth[NUM_FIELDS] =
{
	40, 90, 90, 90, 90,
	80, 180, 60, 150,
	60, 60, 120, 120, 120,
};

static int _gnColumnFmt[NUM_FIELDS] =
{
	LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
	LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_LEFT,
	LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT
};

class CForeFont
{
public:
	CForeFont(IFontDisp* pFont, OLE_COLOR clr)
	{
		m_spFont = pFont;
		m_clr = clr;
	}

	OLE_COLOR m_clr;
	CComPtr<IFontDisp> m_spFont;
};

class CBackColor
{
public:
	CBackColor(OLE_COLOR clr)
	{
		m_clr = clr;
	}

	OLE_COLOR m_clr;
};

class CColumn
{
public:
	CColumn(enumAlarmField enumField)
	{
		m_enumAlarmField = enumField;
		m_strCaption = _gszColumnLabel[enumField];
		m_nWidth = _gnColumnWidth[enumField];
		m_nFormat = _gnColumnFmt[enumField];
	}

	void Draw(HDC hDC, RECT rcBound, BOOL bPush);
	void SetIdealWidth(HDC hdc);
		
	CString m_strCaption;
	int m_nWidth;
	int m_nFormat;
	enumAlarmField m_enumAlarmField;
};

class CAlmSum;

class CAlarm  
{
public:
	CAlarm(CAlmSum* pAlmSum)
	{
		m_pAlmSum = pAlmSum;

		m_font = AtlGetStockFont(DEFAULT_GUI_FONT);
		m_clrFore = GetSysColor(COLOR_WINDOWTEXT);
		m_clrBack = GetSysColor(COLOR_WINDOW);

		m_bAck = FALSE;
		m_bAckRequired = FALSE;
//		m_strNode = _T("ThisNode");
	}
	virtual ~CAlarm()
	{
	}

public:
	BOOL m_bAck;
	BOOL m_bAckRequired;
	FILETIME m_timeIn;
	FILETIME m_timeOut;
	CString m_strNode;
	CString m_strTag;
	CString m_strStatus;
	CString m_strDesc;
	CComVariant m_vaValue;
	DWORD m_dwSeverity; //优先级别
	CString m_strArea;
	CString m_strExtension1;
	CString m_strExtension2;
	
	CFontHandle m_font; // 
	OLE_COLOR m_clrFore;
	OLE_COLOR m_clrBack;

	CAlmSum* m_pAlmSum;

	void UpdateDrawStyle();
	
	void DrawBackground(HDC hDC, RECT rcBound, BOOL bSelected = FALSE);
	void DrawField(enumAlarmField nField, HDC hDC, RECT rcBound, BOOL bSelected = FALSE);

	static int CompareAlarm(CAlarm* pAlarm1, CAlarm* pAlarm2, enumAlarmField nField);
};

#endif // __ALARM_H_
