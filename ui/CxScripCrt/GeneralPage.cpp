// GeneralPage.cpp: implementation of the CGeneralPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CxScripCrt.h"
#include "GeneralPage.h"
#include "OEMSetup.h"
#include "PropertyPageDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static LPCTSTR s_pszExtendSymbols[] =
{
	_T("Do [{While | Until} 条件]\r\n[语句]\r\n[Exit Do]\r\n[语句]\r\nLoop"),
	_T("Do\r\n[语句]\r\n[Exit Do]\r\n[语句]\r\nLoop [{While | Until} 条件]"),	
	_T("For counter = 开始 To 结束 [Step 步幅]\r\n[语句]\r\n[Exit For]\r\n[语句]\r\nNext"),
	_T("For Each 元素 In 组\r\n[语句]\r\n[Exit For]\r\n[语句]\r\nNext [元素]"),
	_T("If 条件 Then 语句 [Else 语句]"),
	_T("If 条件 Then\r\n[语句]\r\n[ElseIf 条件 Then\r\n[语句]]\r\n[Else\r\n[语句]]\r\nEnd If"), 
	_T("Select Case 表达式\r\n[Case 表达式值\r\n[语句]]\r\n[Case Else 表达式值\r\n[语句]]\r\nEnd Select"), 
	_T("While 条件\r\n[语句]\r\nWend"),
	_T("With 对象\r\n语句\r\nEnd With"),

//	_T("With 对象\r\n语句\r\nEnd With"),
};

//Set rs = Server.CreateObject("ADODB.Recordset")
//rs.Open 表格或 SQL 查询语句, 数据源或连接字符串, 1, 3
//
//Do While Not rsBBSReply.EOF
//rs.MoveNext
//Loop

//Set rs = Nothing

void GetExtendSymbols(DWORD dwID, CString& strText)
{
	if ((dwID & 0xff000000) != 0)
	{
		dwID >>= 24;
		dwID -= 1;
		ATLASSERT(dwID < sizeof(s_pszExtendSymbols) / sizeof(s_pszExtendSymbols[0]));
		strText = s_pszExtendSymbols[dwID];
	}
}

struct _SymbolNode
{
	LPTSTR szNode;
	DWORD dwData;
};

static _SymbolNode s_nodeCommSymbols[] = 
{	
	{_T("标签..."), CX_GLOBAL_SYMBOL | 0x10000},
	{_T("数据源连接字符串..."), CX_GLOBAL_SYMBOL | 0x20000},
 	{_T("SuperCx SQL 表达式..."), CX_GLOBAL_SYMBOL | 0x30000},
};

static _SymbolNode s_nodeCodeFragSymbols[] = 
{	
	{_T("ADO 数据访问"), CX_SCRIPT_SYMBOL | 0xA000000},
};

static _SymbolNode s_nodeOperAndKeywords[] = 
{	
	{_T("+"), CX_SCRIPT_SYMBOL},
	{_T("-"), CX_SCRIPT_SYMBOL},
	{_T("*"), CX_SCRIPT_SYMBOL},
	{_T("/"), CX_SCRIPT_SYMBOL},
	{_T("Mod"), CX_SCRIPT_SYMBOL},
	{_T("&"), CX_SCRIPT_SYMBOL},
	{_T("<>"), CX_SCRIPT_SYMBOL},
	{_T("<"), CX_SCRIPT_SYMBOL},
	{_T(">"), CX_SCRIPT_SYMBOL},
	{_T("<="), CX_SCRIPT_SYMBOL},
	{_T(">="), CX_SCRIPT_SYMBOL},
	{_T("Not"), CX_SCRIPT_SYMBOL},
	{_T("And"), CX_SCRIPT_SYMBOL},
	{_T("Or"), CX_SCRIPT_SYMBOL},
	{_T("Xor"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeControlFlows[] = 
{	
	{_T("Do...Loop 1"), CX_SCRIPT_SYMBOL | 0x1000000},
	{_T("Do...Loop 2"), CX_SCRIPT_SYMBOL | 0x2000000},
	{_T("For...Next"), CX_SCRIPT_SYMBOL | 0x3000000},
	{_T("For Each...Next"), CX_SCRIPT_SYMBOL | 0x4000000},
	{_T("If...Then...Else 1"), CX_SCRIPT_SYMBOL | 0x5000000},
	{_T("If...Then...Else 2"), CX_SCRIPT_SYMBOL | 0x6000000},
	{_T("Select Case"), CX_SCRIPT_SYMBOL | 0x7000000},
	{_T("While...Wend"), CX_SCRIPT_SYMBOL | 0x8000000},
	{_T("With"), CX_SCRIPT_SYMBOL | 0x9000000},
};

static _SymbolNode s_nodeDeclares[] = 
{	
	{_T("Class"), CX_SCRIPT_SYMBOL},
	{_T("Const"), CX_SCRIPT_SYMBOL},
	{_T("Dim"), CX_SCRIPT_SYMBOL},
	{_T("Private"), CX_SCRIPT_SYMBOL},
	{_T("Public"), CX_SCRIPT_SYMBOL},
	{_T("ReDim"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeMathFunctions[] = 
{	
	{_T("Atn"), CX_SCRIPT_SYMBOL},
	{_T("Cos"), CX_SCRIPT_SYMBOL},
	{_T("Sin"), CX_SCRIPT_SYMBOL},
	{_T("Tan"), CX_SCRIPT_SYMBOL},
	{_T("Exp"), CX_SCRIPT_SYMBOL},
	{_T("Log"), CX_SCRIPT_SYMBOL},
	{_T("Sqr"), CX_SCRIPT_SYMBOL},
	{_T("Randomize"), CX_SCRIPT_SYMBOL}, 
	{_T("Rnd"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeRoundFunctions[] = 
{	
	{_T("Abs"), CX_SCRIPT_SYMBOL},
	{_T("Int"), CX_SCRIPT_SYMBOL},
	{_T("Fix"), CX_SCRIPT_SYMBOL},
	{_T("Round"), CX_SCRIPT_SYMBOL},
	{_T("Sgn"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeStringFunctions[] = 
{	
	{_T("Asc"), CX_SCRIPT_SYMBOL},
	{_T("AscB"), CX_SCRIPT_SYMBOL},
	{_T("AscW"), CX_SCRIPT_SYMBOL},
	{_T("Chr"), CX_SCRIPT_SYMBOL},
	{_T("ChrB"), CX_SCRIPT_SYMBOL},
	{_T("ChrW"), CX_SCRIPT_SYMBOL},
	{_T("Filter"), CX_SCRIPT_SYMBOL},
	{_T("InStr"), CX_SCRIPT_SYMBOL},
	{_T("InStrB"), CX_SCRIPT_SYMBOL},
	{_T("Space"), CX_SCRIPT_SYMBOL},
	{_T("Join"), CX_SCRIPT_SYMBOL},
	{_T("Len"), CX_SCRIPT_SYMBOL},
	{_T("LeftB"), CX_SCRIPT_SYMBOL},
	{_T("LCase"), CX_SCRIPT_SYMBOL},
	{_T("UCase"), CX_SCRIPT_SYMBOL},
	{_T("Left"), CX_SCRIPT_SYMBOL},
	{_T("LeftB"), CX_SCRIPT_SYMBOL},
	{_T("Right"), CX_SCRIPT_SYMBOL},
	{_T("RightB"), CX_SCRIPT_SYMBOL},
	{_T("Replace"), CX_SCRIPT_SYMBOL},
	{_T("Split"), CX_SCRIPT_SYMBOL},
	{_T("StrComp"), CX_SCRIPT_SYMBOL},
	{_T("String"), CX_SCRIPT_SYMBOL},
	{_T("StrReverse"), CX_SCRIPT_SYMBOL},
	{_T("LTrim"), CX_SCRIPT_SYMBOL},
	{_T("Trim"), CX_SCRIPT_SYMBOL},
	{_T("RTrim"), CX_SCRIPT_SYMBOL},
	{_T("Trim"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeStringFormatFunctions[] = 
{	
	{_T("FormatCurrency"), CX_SCRIPT_SYMBOL},
	{_T("FormatDateTime"), CX_SCRIPT_SYMBOL},
	{_T("FormatNumber"), CX_SCRIPT_SYMBOL},
	{_T("FormatPercent"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeConvertFunctions[] = 
{	
	{_T("CBool"), CX_SCRIPT_SYMBOL},
	{_T("CByte"), CX_SCRIPT_SYMBOL},
	{_T("CCur"), CX_SCRIPT_SYMBOL},
	{_T("CDate"), CX_SCRIPT_SYMBOL},
	{_T("CDbl"), CX_SCRIPT_SYMBOL},
	{_T("CInt"), CX_SCRIPT_SYMBOL},
	{_T("CLng"), CX_SCRIPT_SYMBOL},
	{_T("CSng"), CX_SCRIPT_SYMBOL},
	{_T("CStr"), CX_SCRIPT_SYMBOL},
	{_T("Hex"), CX_SCRIPT_SYMBOL},
	{_T("Oct"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeObjectFunctions[] = 
{	
	{_T("CreateObject"), CX_SCRIPT_SYMBOL},
	{_T("GetObject"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeVarFunctions[] = 
{	
	{_T("IsArray"), CX_SCRIPT_SYMBOL},
	{_T("IsDate"), CX_SCRIPT_SYMBOL},
	{_T("IsEmpty"), CX_SCRIPT_SYMBOL},
	{_T("IsNull"), CX_SCRIPT_SYMBOL},
	{_T("IsNumeric"), CX_SCRIPT_SYMBOL},
	{_T("IsObject"), CX_SCRIPT_SYMBOL},
	{_T("TypeName"), CX_SCRIPT_SYMBOL},
	{_T("VarType"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeOutputFunctions[] = 
{	
	{_T("InputBox"), CX_SCRIPT_SYMBOL},
	{_T("MsgBox"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeDateTimeFunctions[] = 
{	
	{_T("Date"), CX_SCRIPT_SYMBOL},
	{_T("Time"), CX_SCRIPT_SYMBOL},
	{_T("DateAdd"), CX_SCRIPT_SYMBOL},
	{_T("DateDiff"), CX_SCRIPT_SYMBOL},
	{_T("DatePart"), CX_SCRIPT_SYMBOL},
	{_T("DateSerial"), CX_SCRIPT_SYMBOL},
	{_T("DateValue"), CX_SCRIPT_SYMBOL},
	{_T("Day"), CX_SCRIPT_SYMBOL},
	{_T("Month"), CX_SCRIPT_SYMBOL},
	{_T("MonthName"), CX_SCRIPT_SYMBOL},
	{_T("Weekday"), CX_SCRIPT_SYMBOL},
	{_T("WeekdayName"), CX_SCRIPT_SYMBOL},
	{_T("Year"), CX_SCRIPT_SYMBOL},
	{_T("Hour"), CX_SCRIPT_SYMBOL},
	{_T("Minute"), CX_SCRIPT_SYMBOL},
	{_T("Second"), CX_SCRIPT_SYMBOL},
	{_T("Now"), CX_SCRIPT_SYMBOL},
	{_T("TimeSerial"), CX_SCRIPT_SYMBOL},
	{_T("TimeValue"), CX_SCRIPT_SYMBOL},
};

static _SymbolNode s_nodeUserFunctions[] = 
{	
	{_T("Login"), CX_HMI_SYMBOL},
	{_T("Logout"), CX_HMI_SYMBOL},
	{_T("ManageUser"), CX_HMI_SYMBOL},
	{_T("ChangePassword"), CX_HMI_SYMBOL},
	{_T("GetUserName"), CX_HMI_SYMBOL},
	{_T("GetUserSecurityArea"), CX_HMI_SYMBOL},
	{_T("GetUserPrivilege"), CX_HMI_SYMBOL},
};

static _SymbolNode s_nodeSystemFunctions[] = 
{	
	{_T("ReadTag"), CX_GLOBAL_SYMBOL},
	{_T("WriteTag"), CX_GLOBAL_SYMBOL},
	{_T("ShowPicture"), CX_HMI_SYMBOL},
	{_T("DownloadRecipe"), CX_GLOBAL_SYMBOL},
	{_T("PlaySound"), CX_GLOBAL_SYMBOL},
	{_T("PrintWindow"), CX_HMI_SYMBOL},
	{_T("GetAlarmCount"), CX_GLOBAL_SYMBOL},
	{_T("GetAlarmCountEx"), CX_GLOBAL_SYMBOL},
	{_T("GetDeviceStatus"), CX_GLOBAL_SYMBOL},
	{_T("SetDeviceStatus"), CX_GLOBAL_SYMBOL},
	{_T("AcknowledgeAlarm"), CX_GLOBAL_SYMBOL},
	{_T("LogUserOperation"), CX_GLOBAL_SYMBOL},
	{_T("LogSOEEvent"), CX_GLOBAL_SYMBOL},
	{_T("SaveTagStatus"), CX_GLOBAL_SYMBOL},
	{_T("ContainerContext"), CX_HMI_SYMBOL},
	{_T("DoEvent"), CX_HMI_SYMBOL},
	{_T("ExitApplication"), CX_HMI_SYMBOL},
	{_T("ExitWindows"), CX_GLOBAL_SYMBOL},
	{_T("LoadPicutreFile"), CX_HMI_SYMBOL},
	{_T("SaveTempPicutreFile"), CX_HMI_SYMBOL},
	{_T("ShowSoftKeyBoard"), CX_HMI_SYMBOL},
	{_T("SetSystemTime"), CX_HMI_SYMBOL},
};

// 常用程序片断
// ADO数据库访问
// 启动其他应用程序
// Excel 访问
// 文件系统访问

CGeneralPage::CGeneralPage()
{
	m_dwSymbolMask = 0;
}

void CGeneralPage::InsertSymbolNode(LPCTSTR lpszNode, _SymbolNode* pSymbols, int nCount, DWORD dwMask)
{
	TV_INSERTSTRUCT tvis;
	
	memset(&tvis, 0, sizeof(TV_INSERTSTRUCT));
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask =  TVIF_TEXT | TVIF_PARAM; 

	HTREEITEM hNodeParent = NULL;
	if (lpszNode != NULL)
		hNodeParent = m_tree.InsertItem(lpszNode, NULL, TVI_LAST);
	tvis.hParent = hNodeParent;

	CString strSystemName = COEMSetup::GetSystemName();

	for (int i = 0; i < nCount; i++)
	{
		if ((dwMask & pSymbols[i].dwData) != 0)
		{
			CString strText = pSymbols[i].szNode;
			strText.Replace(_T("SuperCx"), strSystemName);
			tvis.item.pszText = (LPTSTR)(LPCTSTR)strText;
			tvis.item.lParam = pSymbols[i].dwData;
			m_tree.InsertItem(&tvis);
		}
	}
}

void CGeneralPage::InsertSymbols(DWORD dwSymbolMask)
{
	if ((dwSymbolMask & CX_GLOBAL_SYMBOL) != 0)
		InsertSymbolNode(NULL, s_nodeCommSymbols, sizeof(s_nodeCommSymbols) / sizeof(_SymbolNode));

	if ((dwSymbolMask & CX_SCRIPT_SYMBOL) != 0)
	{
//		InsertSymbolNode(_T("常用代码片断"), 
//			s_nodeCodeFragSymbols, sizeof(s_nodeCodeFragSymbols) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("操作符与关键字"), 
			s_nodeOperAndKeywords, sizeof(s_nodeOperAndKeywords) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("申明"), 
			s_nodeDeclares, sizeof(s_nodeDeclares) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("控制流程"), 
			s_nodeControlFlows, sizeof(s_nodeControlFlows) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("数学函数"), 
			s_nodeMathFunctions, sizeof(s_nodeMathFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("舍入函数"), 
			s_nodeRoundFunctions, sizeof(s_nodeRoundFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("字符串函数"), 
			s_nodeStringFunctions, sizeof(s_nodeStringFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("字符串格式化函数"), 
			s_nodeStringFormatFunctions, sizeof(s_nodeStringFormatFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("日期时间函数"), 
			s_nodeDateTimeFunctions, sizeof(s_nodeDateTimeFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("转换函数"), 
			s_nodeConvertFunctions, sizeof(s_nodeConvertFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("对象函数"), 
			s_nodeObjectFunctions, sizeof(s_nodeObjectFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("变量函数"), 
			s_nodeVarFunctions, sizeof(s_nodeVarFunctions) / sizeof(_SymbolNode));
		InsertSymbolNode(_T("输出函数"), 
			s_nodeOutputFunctions, sizeof(s_nodeOutputFunctions) / sizeof(_SymbolNode));
	}

	if ((dwSymbolMask & CX_HMI_SYMBOL) != 0)
	{
		InsertSymbolNode(_T("用户管理函数"), 
			s_nodeUserFunctions, sizeof(s_nodeUserFunctions) / sizeof(_SymbolNode));
	}

	if ((dwSymbolMask & (CX_GLOBAL_SYMBOL | CX_HMI_SYMBOL)) != 0)
	{
		InsertSymbolNode(_T("系统函数"), 
			s_nodeSystemFunctions, sizeof(s_nodeSystemFunctions) / sizeof(_SymbolNode), dwSymbolMask);
	}
}

CString CGeneralPage::GetQualifyNameSpace(HTREEITEM hItem)
{
	ATLASSERT(hItem != NULL);

	CString strText;

	TV_ITEM tvi;
	memset(&tvi, 0, sizeof(TV_ITEM));
	tvi.mask = TVIF_TEXT|TVIF_PARAM;
	tvi.hItem = hItem;
	tvi.pszText = strText.GetBuffer(128);
	tvi.cchTextMax = 80;
	m_tree.GetItem(&tvi);
	strText.ReleaseBuffer();

	DWORD dwData = tvi.lParam;
	GetExtendSymbols(dwData, strText);

	if (dwData != NULL)
	{
		dwData = dwData >> 16;
		dwData &= 0xff;
		BOOL bReplace = TRUE;
		if (dwData != 0)
		{
			strText.Empty();
			if (!OnBuildSymbol(dwData, strText))
				bReplace = FALSE;
		}

		return strText;
	}

	return CString("");
}

BOOL CGeneralPage::OnBuildSymbol(DWORD dwID, CString& strText)
{
	switch (dwID)
	{
	case 1:
		return OnBuildTag(strText);
	case 2:
		return OnBuildOLEDBSource(strText);
	case 3:
		return OnBuildSQL(strText);
	}

	return FALSE;
}

const CLSID CLSID_CxTagPicker = {0x6CF3993A,0x1D26,0x40BF,{0xB4,0xD8,0x94,0xC4,0xB5,0x4F,0x13,0x90}};

BOOL CGeneralPage::OnBuildTag(CString& strText)
{
	CComPtr<IDispatch> spTagPick;
	HRESULT hr = spTagPick.CoCreateInstance(CLSID_CxTagPicker);
	if (FAILED(hr))
		return FALSE;
	
	VARIANTARG* pvars = new VARIANTARG[7];
	for (int i = 0; i < 7; i++)
		VariantInit(&pvars[i]);
	
	// 原标签
	pvars[0].vt = VT_BSTR;
	pvars[0].bstrVal = strText.AllocSysString();
	// 关联标签组，此处没有
	pvars[1].vt = VT_BSTR;
	pvars[1].bstrVal = SysAllocString(L"");
	// 禁止使用非关联标签
	pvars[2].vt = VT_BOOL;
	pvars[2].boolVal = VARIANT_TRUE;
	// 使用本地配置
	pvars[3].vt = VT_BOOL;
	pvars[3].boolVal = VARIANT_TRUE;
	// Y座标
	pvars[4].vt = VT_I4;
	pvars[4].lVal = 0;
	// X座标
	pvars[5].vt = VT_I4;
	pvars[5].lVal = 0;
	// 父窗口句柄
	pvars[6].vt = VT_I4;
	pvars[6].lVal = (long)m_hWnd;
	
	CComVariant vaRet;
	
	DISPPARAMS dispparams = { pvars, NULL, 7, 0};
	hr = spTagPick->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	
//	SetFocus();
	
	for (i = 0; i < 7; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr) || vaRet.vt != VT_BSTR)
		return FALSE;
	
	strText = vaRet.bstrVal;
	
	return TRUE;
}

const CLSID CLSID_CxDataConnectionPage = {0x6E390124,0x837E,0x447D,{0xAD,0x60,0x29,0xA8,0x4E,0x6E,0xDB,0x19}};
const IID IID_ICxDataConnection = {0x424024A4,0x5846,0x4A64,{0x99,0xDD,0x69,0x58,0x57,0xF1,0xFC,0x5D}};
const IID IID_ICxDataRecordset = {0xBB7E961D,0x0A97,0x48A9,{0xA7,0x64,0xAE,0x4A,0x19,0x24,0xA0,0x4A}};
const CLSID CLSID_CxDataConnection = {0x65F875DF,0x7EC3,0x4DBD,{0x87,0x0E,0x08,0xD4,0x96,0xD4,0x77,0x6C}};
const CLSID CLSID_CxDataRecordset = {0x54A812E0,0x8C18,0x46AF,{0xB5,0x83,0x56,0x83,0xCD,0xB8,0x72,0x9A}};

BOOL CGeneralPage::OnBuildOLEDBSource(CString& strText)
{
	CComPtr<IDispatch> spObject;
	HRESULT hr = spObject.CoCreateInstance(CLSID_CxDataConnection);
	if (FAILED(hr))
		return FALSE;

	CComObject<CPropertyPageDlg>* pDlg = new CComObject<CPropertyPageDlg>;
	pDlg->AddRef();
	
	pDlg->m_bstrCaption = _T("数据源连接字符串");
	pDlg->m_clsidPage = CLSID_CxDataConnectionPage;
	pDlg->m_spObject = spObject;

	int nRet = pDlg->DoModal();
	pDlg->Release();
	
	if (nRet == IDOK)
	{
		CComDispatchDriver dd(spObject);
		CComVariant var;
		dd.GetPropertyByName(L"ConnectionString", &var);
		strText = var.bstrVal;

		return TRUE;
	}
	
	return FALSE;
}

const CLSID CLSID_CxSQLBuilder = {0x32CF356E,0x28E4,0x4D7F,{0x80,0x84,0xFC,0xA6,0xAC,0x42,0xB5,0x5B}};

BOOL CGeneralPage::OnBuildSQL(CString& strText)
{
	CComPtr<IDispatch> spSQLBuilder;
	HRESULT hr = spSQLBuilder.CoCreateInstance(CLSID_CxSQLBuilder);
	if (FAILED(hr))
		return FALSE;
	
	VARIANTARG* pvars = new VARIANTARG[2];
	for (int i = 0; i < 1; i++)
		VariantInit(&pvars[i]);
	
	// 父窗口句柄
	pvars[0].vt = VT_I4;
	pvars[0].lVal = (long)m_hWnd;
	
	CComVariant vaRet;
	
	DISPPARAMS dispparams = { pvars, NULL, 1, 0};
	hr = spSQLBuilder->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, &vaRet, NULL, NULL);
	
	for (i = 0; i < 1; i++)
		VariantClear(&pvars[i]);
	delete [] pvars;
	if (FAILED(hr) || vaRet.vt != VT_BSTR)
		return FALSE;
	
	strText = vaRet.bstrVal;
	
	return TRUE;
}

void CGeneralPage::OnInit()
{
	if (m_dwSymbolMask != 0)
		InsertSymbols(m_dwSymbolMask);
}