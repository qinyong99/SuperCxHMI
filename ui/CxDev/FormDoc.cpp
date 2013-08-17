// FormDoc.cpp : implementation of the CFormDoc class
//

#include "stdafx.h"
#include "DevInc.h"
#include "CxDevIF.h"

#include "FormObj.h"
#include "ScriptAnalyzer.h"

#include "EnumString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLIPFORMAT CFormDoc::m_cfFormDocData = (CLIPFORMAT)::RegisterClipboardFormat(_T("Form DocData"));

/////////////////////////////////////////////////////////////////////////////
// CFormDoc

IMPLEMENT_DYNCREATE(CFormDoc, CDevObjDoc)

BEGIN_MESSAGE_MAP(CFormDoc, CDevObjDoc)
	//{{AFX_MSG_MAP(CFormDoc)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_EXPORT, OnFileExport)
	ON_COMMAND(ID_FILE_IMPORT, OnFileImport)
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, CDevObjDoc::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, CDevObjDoc::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, CDevObjDoc::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, CDevObjDoc::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, CDevObjDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, CDevObjDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, CDevObjDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFormDoc, CDevObjDoc)
	//{{AFX_DISPATCH_MAP(CFormDoc)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "VisibleLayer", CXPROPID_VisibleLayer, m_ulVisibleLayer, OnVisibleLayerChanged, VT_I4)
	
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "StartUpPosition", CXPROPID_StartUpPosition, m_enumStartUpPosition, OnStartUpPositionChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BorderStyle", CXPROPID_BorderStyle, m_enumBorderStyle, OnBorderStyleChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ControlBox", CXPROPID_ControlBox, m_bControlBox, OnControlBoxChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "AllowDirectDraw", CXPROPID_AllowDirectDraw, m_bAllowDirectDraw, OnAllowDirectDrawChanged, VT_BOOL)

	DISP_FUNCTION_ID(CFormDoc, "Show", 4, Show, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "Hide", 5, Hide, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "Close", 6, Close, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CFormDoc, "GetHitObject", CXMETHID_GetHitObject, GetHitObject, VT_UNKNOWN, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CFormDoc, "GetObjectByName", CXMETHID_GetObjectByName, GetObjectByName, VT_UNKNOWN, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Objects", CXPROPID_Objects, GetObjectCollection, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "SecurityArea", CXPROPID_SecurityArea, m_strSecurityArea, OnSecurityAreaChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ScanInterval", CXPROPID_ScanInterval, m_nScanInterval, OnScanIntervalChanged, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "MousePointer", CXPROPID_MousePointer, m_enumMousePointer, OnMousePointerChanged, VT_I4)

	DISP_PROPERTY_EX_ID(CFormDoc, "HorizontalPosition", CXPROPID_HorPosition, GetHorizontalPosition, SetHorizontalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "VerticalPosition", CXPROPID_VerPosition, GetVerticalPosition, SetVerticalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "Width", CXPROPID_Width, GetWidth, SetWidth, VT_R4)
	DISP_PROPERTY_EX_ID(CFormDoc, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BackColor", CXPROPID_BackColor, m_clrBackColor, OnBackColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "Caption", CXPROPID_Caption, m_strCaption, OnCaptionChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Font", CXPROPID_Font, GetFont, SetFont, VT_FONT)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "ForeColor", CXPROPID_ForeColor, m_clrForeColor, OnForeColorChanged, VT_COLOR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "BrushImageFile", CXPROPID_BrushImageFileName, m_strBrushImageFile, OnBrushImageFileChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormDoc, "TagGroup", CXPROPID_TagGroup, m_strTagGroup, OnTagGroupChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "AssocTagGroup", CXPROPID_AssocTagGroup, GetAssocTagGroup, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormDoc, "Menu", CXPROPID_Menu, GetMenu, SetMenu, VT_BSTR)

	DISP_PROPERTY_EX_ID(CFormDoc, "Name", CXPROPID_Name, GetName, SetName, VT_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CFormDoc, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFormDoc, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICxDev to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.


// {80008000-8000-8000-8000-800080008203}
static const IID IID_IUserForm =
{ 0x80008000, 0x8000, 0x8000, { 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x82, 0x03 } };

const IID IID_ISubUserForm = 
{0x1AE68DD5,0x7CFD,0x42f9,{0xAA,0xA1,0xAB,0xED,0x37,0x6D,0x4E,0x85}};

BEGIN_INTERFACE_MAP(CFormDoc, CDevObjDoc)
	INTERFACE_PART(CFormDoc, IID_IUserForm, Dispatch)
	INTERFACE_PART(CFormDoc, IID_ISubUserForm, Dispatch)
	INTERFACE_PART(CFormDoc, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CFormDoc, IID_IProvideClassInfo2, ProvideClassInfo)
	INTERFACE_PART(CFormDoc, IID_ICategorizeProperties, CategorizeProperties)
	INTERFACE_PART(CFormDoc, IID_IPerPropertyBrowsing, PerPropertyBrowsing)
END_INTERFACE_MAP()

const IID LIBID_CXHMIOBJLib = {0xD75F0066,0xDC61,0x4679,{0xA7,0x39,0x5B,0x04,0x6E,0x2C,0x90,0x88}};

IMPLEMENT_OLETYPELIB(CFormDoc, LIBID_CXHMIOBJLib, 1, 0)

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CFormDoc::XPerPropertyBrowsing::AddRef()
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)
		
	TRACE("XPerPropertyBrowsing::AddRef().\n");
	
	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormDoc::XPerPropertyBrowsing::Release()
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)
		
	TRACE("XPerPropertyBrowsing::Release().\n");
	
	return pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XPerPropertyBrowsing::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)
		
	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XPerPropertyBrowsing::GetDisplayString(DISPID dispid, BSTR *pbstr)
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)
		
	return E_NOTIMPL;
}

STDMETHODIMP CFormDoc::XPerPropertyBrowsing::MapPropertyToPage(DISPID dispid, LPCLSID pClsID)
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)
		
	return E_NOTIMPL;
}

STDMETHODIMP CFormDoc::XPerPropertyBrowsing::GetPredefinedStrings(DISPID dispid, CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut)
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)

	if (dispid == CXPROPID_Menu)
		return pThis->GetMenuDisplayStrings(pcaStringsOut, pcaCookiesOut);
		
	return E_INVALIDARG;
}

STDMETHODIMP CFormDoc::XPerPropertyBrowsing::GetPredefinedValue(DISPID dispid, DWORD dwCookie, VARIANT *pVarOut)
{
	METHOD_PROLOGUE(CFormDoc, PerPropertyBrowsing)

	if (dispid == CXPROPID_Menu)
		return pThis->GetMenuPredefinedValue(dwCookie, pVarOut);
		
	return E_INVALIDARG;
}

////////////////////////////////////////////////////////////////
STDMETHODIMP_( ULONG ) CFormDoc::XCategorizeProperties::AddRef()
{
	METHOD_PROLOGUE(CFormDoc, CategorizeProperties)

	TRACE("XCategorizeProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormDoc::XCategorizeProperties::Release()
{
	METHOD_PROLOGUE(CFormDoc, CategorizeProperties)

	TRACE("XCategorizeProperties::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XCategorizeProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormDoc, CategorizeProperties)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XCategorizeProperties::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	METHOD_PROLOGUE(CFormDoc, CategorizeProperties)

	return pThis->MapPropertyToCategory(dispid, ppropcat);
}

STDMETHODIMP CFormDoc::XCategorizeProperties::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	METHOD_PROLOGUE(CFormDoc, CategorizeProperties)

	return pThis->GetCategoryName(propcat, lcid, pbstrName);
}

/////////////////////////////////////////////////////////////////////////////
// CRunDoc::XProvideClassInfo

STDMETHODIMP_(ULONG) CFormDoc::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CFormDoc::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormDoc::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CFormDoc, ProvideClassInfo)

	if (pThis->IsMainForm())
		return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(UserForm), ppTypeInfo);

	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(SubUserForm), ppTypeInfo);
	
//	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(UserForm), ppTypeInfo);
}

STDMETHODIMP CFormDoc::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CFormDoc, ProvideClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IUserFormEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

///////////////////////////////////////////////////////////////////////////////
// ICxScriptEditorSite
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CFormDoc::XSymbolTreeSite::AddRef()
{
	METHOD_PROLOGUE_(CFormDoc, SymbolTreeSite)
		
	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormDoc::XSymbolTreeSite::Release()
{
	METHOD_PROLOGUE_(CFormDoc, SymbolTreeSite)
		
	return pThis->ExternalRelease();
}

STDMETHODIMP CFormDoc::XSymbolTreeSite::QueryInterface(
	REFIID iid, void** ppInterface)
{
	METHOD_PROLOGUE_(CFormDoc, SymbolTreeSite)
		
	return pThis->ExternalQueryInterface(&iid, ppInterface);
}


int compare(const void *arg1, const void *arg2)
{
   /* Compare all of both strings: */
   return _stricmp( * ( char** ) arg1, * ( char** ) arg2 );
}

void SortStringArray(CStringArray& arrStrings)
{
	qsort((void *)arrStrings.GetData(), (size_t)arrStrings.GetSize(), sizeof(char *), compare );
}

STDMETHODIMP CFormDoc::XSymbolTreeSite::GetObjectMembers(LPCWSTR lpszObject, 
	LPENUMSTRING* ppEnumObjects, LPENUMSTRING* ppEnumProperties, LPENUMSTRING* ppEnumMethods)
{
	METHOD_PROLOGUE(CFormDoc, SymbolTreeSite)

	CStringArray arrWords;

	if (lpszObject != NULL)
	{
		LPCWSTR p1 = lpszObject;
		LPCWSTR p2 = lpszObject;
		if (*p2 != '\0')
		{
			while (1)
			{
				if (*p2 == OLECHAR('.') || *p2 == '\0')
				{
					CString strWord(p1, p2 - p1);
					arrWords.Add(strWord);

					if (*p2 == '\0')
						break;

					p1 = p2 + 1;
				}

				p2++;
			}
		}
	}

	HRESULT hrRet = S_FALSE;
	CStringArray arrObjects;
	CStringArray arrProperties;
	CStringArray arrMethods;

	CFormDoc* pFormDoc = pThis->GetTopFormDoc();

	if (arrWords.GetSize() == 0)
	{
		CPtrList& listSubDoc = pFormDoc->m_pDocument->m_listSubDoc;
		POSITION pos = listSubDoc.GetHeadPosition();
		while (pos != NULL)
		{
			CDevDoc::CSubDoc* pSubDoc = (CDevDoc::CSubDoc *)listSubDoc.GetNext(pos); 
			arrObjects.Add(pSubDoc->m_strName);
		}

		hrRet = S_OK;
	}
	else if (pFormDoc->GetObjectMembers(arrWords, arrObjects, arrProperties, arrMethods))
	{
		SortStringArray(arrObjects);
		hrRet = S_OK;
	}

	if (hrRet == S_OK)
	{
		CEnumString::CreateEnumString(arrObjects, ppEnumObjects);	
		CEnumString::CreateEnumString(arrProperties, ppEnumProperties);	
		CEnumString::CreateEnumString(arrMethods, ppEnumMethods);
	}
	
	return hrRet;
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc construction/destruction

CFormDoc::CFormDoc(CDevDoc* pDoc) :
	m_pDocument(pDoc),
	m_pFormSubDoc(NULL),
	m_pFormObj(NULL),
	m_pFontHolder(NULL)
{
	m_descFont.lpstrName = NULL;
	
	m_dwNextObjectNumber = 1;

	m_enumMousePointer = MousePointerArrow;
	m_nScanInterval = 100;

	m_enumStartUpPosition = StartUpPositionOwner;
	m_enumBorderStyle = BorderStyleSingle;
	m_bControlBox = TRUE;
	m_bAllowDirectDraw = FALSE;

	m_nLayer = 0;
	m_ulVisibleLayer = 0XFFFFFFFF;
	m_ulLockLayer = 0;

	m_pImageBackGround = NULL;

	m_bSetScriptSavePoint = FALSE;

	m_dwSymbolUpdateMask = 0;
}

CFormDoc::~CFormDoc()
{
	if (m_pImageBackGround)
		delete m_pImageBackGround;
}

BOOL CFormDoc::OnNewDocument()
{
	if (m_pDocument != NULL)
	{
		if (m_lpRootStg == NULL)
			m_lpRootStg = m_pDocument->OpenSubDocumentStorage(m_pFormSubDoc);
	}
	else 
	{
		if (!CDevObjDoc::OnNewDocument())
			return FALSE;
	}

	return Initialize();
}

/////////////////////////////////////////////////////////////////////////////
//功 能：窗体属性序列化
///////////////////////////////////////////////////////////////////////////// 
void CFormDoc::SerializeProperty(CArchive& ar)
{
	if (ar.IsLoading())
	{
		ar >> m_clrBackColor;
		ar >> m_clrForeColor;

		ar >> m_strCaption;
				
		ar >> m_strBrushImageFile;
		ASSERT(m_pImageBackGround == NULL);
		if (m_strBrushImageFile != "")
		{
			BSTR bstr = m_strBrushImageFile.AllocSysString();
			m_pImageBackGround = Gdiplus::Image::FromFile(bstr);
			SysFreeString(bstr);
		}

		ar >> m_position;
		
		ar >> m_nScanInterval;
		ar >> m_strSecurityArea;
		ar >> m_strTagGroup;

		if (m_dwVer > _SUPERCX_VER_0203)
			ar >> m_strMenu;

		if (m_dwVer != _SUPERCX_OLDVER)
			ar >> m_strDescription;

		if (m_dwVer > _SUPERCX_VER_0100)
		{
			long lTemp;
			ar >> lTemp;
			m_enumMousePointer = enumMousePointer(lTemp);

			if (m_dwVer > _SUPERCX_VER_0101)
			{
				ar >> lTemp;
				m_enumStartUpPosition = enumStartUpPosition(lTemp);
				ar >> lTemp;
				m_enumBorderStyle = enumBorderStyle(lTemp);

				if (m_dwVer > _SUPERCX_VER_0208)
					ar >> m_bControlBox;

				if (m_dwVer < _SUPERCX_VER_0207)
				{
					BOOL bShowModal;
					ar >> bShowModal;
				}

				if (m_dwVer > _SUPERCX_VER_0209)
					ar >> m_bAllowDirectDraw;
			}
		}
	}
	else
	{
		ar << m_clrBackColor;
		ar << m_clrForeColor;
//		ar << m_clrGridColor;
//		ar << m_bGridEnabled;
		ar << m_strCaption;
		ar << m_strBrushImageFile;
		ar << m_position;
//		ar << m_nWidth;
//		ar << m_nHeight;
		ar << m_nScanInterval;
		ar << m_strSecurityArea;
		ar << m_strTagGroup;

		if (m_dwVer > _SUPERCX_VER_0203)
			ar << m_strMenu;

		ar << m_strDescription;
		ar << m_enumMousePointer;

		ar << m_enumStartUpPosition;
		ar << m_enumBorderStyle;
//		ar << m_bShowModal;
		ar << m_bControlBox;
		ar << m_bAllowDirectDraw;
	}
}

/////////////////////////////////////////////////////////////////////////////
//功 能：文档序列化
/////////////////////////////////////////////////////////////////////////////
void CFormDoc::Serialize(CArchive& ar)
{
	USES_CHINESEVAR_CONVERSION;

	if (ar.IsLoading())	
	{
		Initialize(FALSE);
		if (m_pDocument != NULL && m_pDocument->m_dwVer == _SUPERCX_OLDVER)
			m_dwVer = _SUPERCX_OLDVER;
		else	
			ar >> m_dwVer;
	}
	else
	{
		m_dwVer = _SUPERCX_VER;
		ar << m_dwVer;
	}

	SerializeProperty(ar);
	//序列化动态属性
	SerializeDynamicProperty(ar);

	//脚本序列化
	if (ar.IsStoring())	
	{
		CScriptView* pScriptView = GetScriptView();
		if (pScriptView != NULL)
		{
			pScriptView->BeginUndoAction();
			pScriptView->DeleteEmptyEventHandler();
			pScriptView->EndUndoAction();

			pScriptView->GetText(m_strScript);

			if (m_bSetScriptSavePoint)
				pScriptView->SetSavePoint();
		}

//		CString strTemp;
//		ChineseToASCII(m_strScript, strTemp.GetBuffer(m_strScript.GetLength() * 4));
//		strTemp.ReleaseBuffer();
//
//		ar << strTemp;

		CString strScriptSave = m_strScript;

		m_strScript = VARC2A(m_strScript);
		ar << m_strScript;

		UpdateObjectsEventStatus();

		m_strScript = strScriptSave;
	} 
	else 
	{
//		CString strTemp;
//		ar >> strTemp;
//		
//		ASCIIToChinese(strTemp, m_strScript.GetBuffer(strTemp.GetLength()));
//		m_strScript.ReleaseBuffer();

		ar >> m_strScript;
		m_strScript = VARA2C(m_strScript);
	}

	if (ar.IsStoring())	
	{
		ar << m_nLayer;
		ar << m_ulVisibleLayer;
		ar << m_ulLockLayer;
	}
	else 
	{
		ar >> m_nLayer;
		ar >> m_ulVisibleLayer;
		ar >> m_ulLockLayer;
	}

	//调用基类进行客户项目序列化
	CDevObjDoc::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CFormDoc diagnostics

#ifdef _DEBUG
void CFormDoc::AssertValid() const
{
	CDevObjDoc::AssertValid();
}

void CFormDoc::Dump(CDumpContext& dc) const
{
	CDevObjDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormDoc commands


void CFormDoc::AddObject(CDevObj* pObj)
{
	CDevObjDoc::AddObject(pObj);

	pObj->m_nLayer = m_nLayer;
}

//添加绘制对象
CDrawObj* CFormDoc::AddDrawObj(REFCLSID clsid,
	LPCTSTR pszName, REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	CDrawObj* pObj;

	pObj = NULL;
	try
	{
		pObj = new CDrawObj(this);
		ASSERT_VALID(pObj);

		if (!pObj->Create(clsid))
		{
			ASSERT(FALSE);
			AfxThrowOleException( E_FAIL );
		}
		ASSERT_VALID( pObj );
	}
	catch( ... )
	{
		pObj->Delete();

		throw;
	}
	ASSERT(pObj != NULL);

	//以视图当前的层为对象的层
	pObj->m_nLayer = m_nLayer;
//	SortObjectByLayer(pObj);

	return pObj;
}


/////////////////////////////////////////////////////////////////////////
//功 能：添加控件对象
/////////////////////////////////////////////////////////////////////////
CCtrlObj* CFormDoc::AddCtrlObj(REFCLSID clsid,
	LPCTSTR pszName, CRect rcObj, REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	CCtrlObj* pObj = new CCtrlObj(this);
	pObj->m_position = rcObj;

	ASSERT_VALID(pObj);
	if (pObj->Create(clsid) == TRUE)
	{
		pObj->Invalidate();
	}
	else
	{
		RemoveObject(pObj);
		pObj->Delete();
		pObj = NULL;
	}

	return pObj;
}

CGroupObj* CFormDoc::AddGroupObj(LPCTSTR pszName, REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium)
{
	CGroupObj* pObj;

	pObj = NULL;
	try
	{
		pObj = new CGroupObj(this);
		ASSERT_VALID(pObj);

		if (!pObj->CreateOrLoad(pszName, iidPersistanceMedium,
			pPersistanceMedium))
		{
			AfxThrowOleException(E_FAIL);
		}
		ASSERT_VALID(pObj);

//		pObj->Invalidate();
	}
	catch( ... )
	{
		pObj->Delete();

		throw;
	}
	ASSERT(pObj != NULL);

	return pObj;
}

POSITION CFormDoc::GetTailPosition() const
{
   return m_docItemList.GetTailPosition();
}

CDocItem* CFormDoc::GetPrevItem(POSITION& pos) const
{
   return (CDocItem*)m_docItemList.GetPrev(pos);
}

CDevObj* CFormDoc::GetObjectAt(const CPoint& point)
{
	POSITION posObj; 
	CDevObj* pObj;
	for (int i = 30; i >= 0; i--)
	{
		posObj = m_objects.GetTailPosition();
		while (posObj != NULL)
		{
			pObj = (CDevObj *)m_objects.GetPrev(posObj);
			if (pObj->m_nLayer == i && pObj->HitTest(point, NULL))
			{
				unsigned long ulLayer = 1;
				ulLayer = ulLayer << pObj->m_nLayer;
				if ((m_ulLockLayer & ulLayer) != 0 || (m_ulVisibleLayer & ulLayer) == 0)	//被锁定
					return NULL;
				else
					return pObj;
			}
		}
	}

	return NULL;
}

//得到显示项目的视图，在这是 CCxLayoutView
CLayoutView* CFormDoc::GetLayoutView() const
{
	POSITION posView;
	CView* pView;

	posView = GetFirstViewPosition();
	if (posView == NULL)
		return NULL;

	ASSERT(posView != NULL);

	while (posView != NULL)
	{
		pView = GetNextView(posView);
		ASSERT (pView != NULL);
		if( pView->IsKindOf(RUNTIME_CLASS(CLayoutView)))
		{
			return (STATIC_DOWNCAST(CLayoutView, pView));
		}
	}

	ASSERT(FALSE);

	return NULL;
}

//得到显示项目的视图，在这是 CCxLayoutView
CScriptView* CFormDoc::GetScriptView() const
{
	POSITION posView;
	CView* pView;

	posView = GetFirstViewPosition();
	//ASSERT(posView != NULL);

	while (posView != NULL)
	{
		pView = GetNextView(posView);
		ASSERT (pView != NULL);
		if (pView->IsKindOf(RUNTIME_CLASS(CScriptView)))
		{
			return (STATIC_DOWNCAST(CScriptView, pView));
		}
	}

//	ASSERT(FALSE);

	return NULL ;
}

void CFormDoc::SortObjectByLayer(CDevObj* pObj)
{
	POSITION posOld;
	POSITION posNew;
	CDevObj* pObjPos;

	ASSERT( pObj != NULL );

	posOld = m_objects.Find(pObj);
	ASSERT(posOld != NULL );

	posNew = m_objects.GetTailPosition();
	while (posNew != NULL)
	{
		pObjPos = (CDevObj*)m_objects.GetPrev(posNew);
		if (pObjPos->m_nLayer < pObj->m_nLayer)
			break;
	}

	if (posOld != posNew)
	{
		m_objects.RemoveAt(posOld);
		m_objects.InsertAfter(posNew, pObj);
	}
}

// 初始化属性
void CFormDoc::InitializeProperty()
{
	if (IsMainForm())
	{
		m_position.SetRect(0, 0, (float)theApp.m_settingMainForm.m_nWidth, (float)theApp.m_settingMainForm.m_nHeight);
		m_clrBackColor = theApp.m_settingMainForm.m_clrBack;
		m_clrForeColor = theApp.m_settingMainForm.m_clrFore;
		
		m_strCaption = _T("主窗体");
	}
	else
	{
		m_position.SetRect(0, 0, (float)theApp.m_settingSubForm.m_nWidth, (float)theApp.m_settingSubForm.m_nHeight);
		m_clrBackColor = theApp.m_settingSubForm.m_clrBack;
		m_clrForeColor = theApp.m_settingSubForm.m_clrFore;
		
		if (m_pFormSubDoc != NULL && m_pFormSubDoc->m_pParentDoc != NULL)
		{
			m_strCaption.Format("子窗体%d", m_pFormSubDoc->m_dwDocumentNumber);
			CFormDoc::CFormSubDoc* pSubDoc = (CFormDoc::CFormSubDoc *)m_pFormSubDoc->m_pParentDoc->m_listSubDoc.GetHead();
			if (pSubDoc != NULL && pSubDoc->m_pChildDoc != NULL)
				m_strSecurityArea = ((CFormDoc *)pSubDoc->m_pChildDoc)->m_strSecurityArea;
		}
		else if (m_pFormObj != NULL)
		{
			m_strCaption = _T("模块");
		}
	}	
}

////////////////////////////////////////////////////////////////////////////
//功 能：初始化
//参 数：是否需要进行缺省设置
////////////////////////////////////////////////////////////////////////////
BOOL CFormDoc::Initialize(BOOL bDefault)
{
	//设置窗体的缺省属性
	if (bDefault == TRUE)
		InitializeProperty();
	
	//设置缺省环境属性
	HRESULT hResult = InitAmbientProperties();
	if (FAILED(hResult))
		return FALSE;

	return TRUE;
}

//设置文档标题
void CFormDoc::OnUpateTitle()
{
	if (m_pDocument != NULL)
	{
		CString strDocName = m_pDocument->GetTitle();
		strDocName += _T("-");
		strDocName += GetDisplayName();
		SetTitle(strDocName);
	}
	else if (m_pFormObj != NULL)
	{
		SetTitle(_T("模块"));	
	}
}

////////////////////////////////////////////////////////////////////////////
//功 能：初始化缺省属性
////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::InitAmbientProperties()
{
	CString strName;
	LPCOLESTR pszFaceName;
	CAmbientProperty* pProp;

	//////////////////////////////////////////////////////////////////////////////////
	//环境属性

	//UserMode
	pProp = new CAmbientProperty(DISPID_AMBIENT_USERMODE, "UserMode", COleVariant(
		VAR_FALSE, VT_BOOL ), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//ShowHatching
	pProp = new CAmbientProperty(DISPID_AMBIENT_SHOWHATCHING, "ShowHatching",
		COleVariant(VARIANT_BOOL(-1), VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//ShowGrabHandles
	pProp = new CAmbientProperty(DISPID_AMBIENT_SHOWGRABHANDLES, "ShowGrabHandles",
		COleVariant(VARIANT_BOOL(-1), VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//BackColor，由m_clrBackColor决定
	pProp = new CAmbientProperty(DISPID_AMBIENT_BACKCOLOR, "BackColor",
		COleVariant(long(m_clrBackColor), VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	//ForeColor，由m_clrForeColor决定
	pProp = new CAmbientProperty( DISPID_AMBIENT_FORECOLOR, "ForeColor",
		COleVariant(long(m_clrForeColor), VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	//LocaleID
	pProp = new CAmbientProperty(DISPID_AMBIENT_LOCALEID, "LocaleID", COleVariant(
		long(GetUserDefaultLCID()), VT_I4), VTI_I4, TRUE);
	m_apAmbientProperties.Add(pProp);

	//MessageReflect
	pProp = new CAmbientProperty(DISPID_AMBIENT_MESSAGEREFLECT, "MessageReflect",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//MessageReflect
	pProp = new CAmbientProperty(DISPID_AMBIENT_MESSAGEREFLECT, "MessageReflect",
		COleVariant(VARIANT_FALSE, VT_BOOL), VTI_BOOL, TRUE);
	m_apAmbientProperties.Add(pProp);

	//创建字体
	delete m_pFontHolder;
	m_pFontHolder = new CFontHolder(NULL);
	if( m_pFontHolder == NULL )
	{
		AfxThrowMemoryException();
	}
	m_descFont.cbSizeofstruct = sizeof(m_descFont);
	pszFaceName = L"宋体";
	delete[] m_descFont.lpstrName;
	m_descFont.lpstrName = new OLECHAR[ocslen(pszFaceName)+1];
	if (m_descFont.lpstrName == NULL)
	{
		AfxThrowMemoryException();
	}
	ocscpy(m_descFont.lpstrName, pszFaceName);
	m_descFont.cySize.Lo = 90000L;
	m_descFont.cySize.Hi = 0;
	m_descFont.sWeight = FW_NORMAL;
	m_descFont.sCharset = 0;
	m_descFont.fItalic = FALSE;
	m_descFont.fUnderline = FALSE;
	m_descFont.fStrikethrough = FALSE;
	m_pFontHolder->InitializeFont(&m_descFont);

	COleVariant varFont;
	varFont.vt = VT_DISPATCH;
	varFont.pdispVal = m_pFontHolder->GetFontDispatch();

	//Font
	pProp = new CAmbientProperty(DISPID_AMBIENT_FONT, "Font", varFont,
		VTI_FONT, TRUE);
	m_apAmbientProperties.Add(pProp);
	
	////////////////////////////////////////////////////////////////////////////////////
	//缺省属性

	strName.LoadString(IDS_DEFAULT_EDGECOLOR);
	pProp = new CAmbientProperty(CXPROPID_DEFAULT_EDGECOLOR, strName,
		COleVariant((long)theApp.m_settingObject.m_clrEdgeColor, VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	strName.LoadString(IDS_DEFAULT_FILLCOLOR);
	pProp = new CAmbientProperty(CXPROPID_DEFAULT_FILLCOLOR, strName,
		COleVariant((long)theApp.m_settingObject.m_clrFillColor, VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	strName.LoadString(IDS_DEFAULT_FONTCOLOR);
	pProp = new CAmbientProperty(CXPROPID_DEFAULT_FONTCOLOR, strName,
		COleVariant((long)theApp.m_settingObject.m_clrFontColor, VT_I4), VTI_COLOR,
		TRUE);
	m_apAmbientProperties.Add(pProp);

	return S_OK;
}

BOOL CFormDoc::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );

	if (IsMainForm())
		*piid = __uuidof(IUserForm);
	else
		*piid = __uuidof(ISubUserForm);

	return TRUE;
}

LPUNKNOWN CFormDoc::GetObjectUnknown()
{
	if (m_pFormObj != NULL)
		return m_pFormObj->GetObjectUnknown();
	
	return GetInterface(&IID_IUnknown);
}

void CFormDoc::OnStartUpPositionChanged()
{
	SetModifiedFlag();
}

void CFormDoc::OnControlBoxChanged()
{
	SetModifiedFlag();
	UpdateAllViews(NULL, HINT_UPDATE_BORDER);
}

void CFormDoc::OnAllowDirectDrawChanged()
{
	SetModifiedFlag();
}

void CFormDoc::OnBorderStyleChanged()
{
	SetModifiedFlag();
	UpdateAllViews(NULL, HINT_UPDATE_BORDER);
}

void CFormDoc::OnScanIntervalChanged()
{
	SetModifiedFlag();
}

void CFormDoc::OnSecurityAreaChanged()
{
	SetModifiedFlag();
}

void CFormDoc::OnBackColorChanged() 
{
	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void CFormDoc::OnForeColorChanged() 
{
	SetModifiedFlag();
	UpdateAllViews(NULL);
}

void CFormDoc::OnBrushImageFileChanged()
{
	using namespace Gdiplus;

	if (m_pImageBackGround)
	{
		delete m_pImageBackGround;
		m_pImageBackGround = NULL;
	}
	
	if (m_strBrushImageFile != "")
	{
		BSTR bstr = m_strBrushImageFile.AllocSysString();
		m_pImageBackGround = Image::FromFile(bstr);
		SysFreeString(bstr);
	}
	
	UpdateAllViews(NULL);
	SetModifiedFlag();
}

void CFormDoc::OnTagGroupChanged()
{
	m_strTagGroup.TrimLeft();
	m_strTagGroup.TrimRight();
}

BSTR CFormDoc::GetAssocTagGroup()
{
	return GetAssocTagGroupEx(FALSE).AllocSysString();	
}

LPFONTDISP CFormDoc::GetFont()
{
	if (m_pFontHolder != NULL)
		return m_pFontHolder->GetFontDispatch();
	return NULL;
}

void CFormDoc::SetFont(LPFONTDISP pFontDisp)
{
	ASSERT((pFontDisp == NULL) ||
		AfxIsValidAddress(pFontDisp, sizeof(IDispatch), FALSE));

	m_pFontHolder->InitializeFont(NULL, pFontDisp);
}

////////////////////////////////////////////////////////////////////////////
//功 能：文档名变化
//描 述：直接使用
////////////////////////////////////////////////////////////////////////////
BSTR CFormDoc::GetName()
{
	return GetDisplayName().AllocSysString();
}

void CFormDoc::SetName(LPCTSTR lpszNewValue)
{
	if (PutDisplayName(lpszNewValue))
	{
		OnUpateTitle();
		theApp.NotifySelectChanged();

		CDevDoc* pDevDoc;
		if (m_pFormObj != NULL)
			pDevDoc = m_pFormObj->GetDevDoc();
		else
			pDevDoc = m_pDocument;
		
		CDocHit hit(CDocHit::changeContent, pDevDoc->GetDisplayName());
		theApp.NotifyDocumentChanged(DWORD(pDevDoc), DWORD(&hit));
		pDevDoc->OnSubDocumentChanged();
		
//		CDocHit hit(CDocHit::changeContent, m_pDocument->GetDisplayName());
//		theApp.NotifyDocumentChanged(DWORD(m_pDocument), DWORD(&hit));
	}
}

CString CFormDoc::GetDisplayName()
{
	if (m_pFormSubDoc != NULL)
		return m_pFormSubDoc->m_strName;
	if (m_pFormObj != NULL)
		return m_pFormObj->GetDisplayName();

	return CDevObjDoc::GetDisplayName();
}

BOOL CFormDoc::IsValidObjectName(LPCTSTR lpszName)
{
	if (GetDisplayName().CompareNoCase(lpszName) == 0)
		return FALSE;

	return CDevObjDoc::IsValidObjectName(lpszName);
}

BOOL CFormDoc::PutDisplayName(LPCSTR lpszNewValue, BOOL bValidName)
{
	CString strName = lpszNewValue;
	//主窗口名称不能改变
	if (IsMainForm() && strName != _T("MainForm"))
		return FALSE;

//	if (m_pFormObj != NULL)
//	{
////		return m_pFormObj->PutDisplayName(lpszNewValue);
//		return FALSE;
//	}

	if (IsValidObjectName(strName))
	{
		if (m_pDocument != NULL && m_pDocument->IsUniqueSubDocumentName(strName))
		{
			m_pFormSubDoc->m_strName = strName;
			OnSymbolChanged(UPDATE_SYMBOL_OBJECT);
		}

		return TRUE;
	}

	return FALSE;
}

void CFormDoc::OnCaptionChanged() 
{
	SetModifiedFlag();
	UpdateAllViews(NULL, HINT_UPDATE_CAPTION);
}

void CFormDoc::OnMousePointerChanged()
{
}

void CFormDoc::OnDescriptionChanged() 
{
	SetModifiedFlag();
}

BSTR CFormDoc::GetMenu()
{
	return m_strMenu.AllocSysString();
}

void CFormDoc::SetMenu(LPCTSTR lNewValue)
{
	m_strMenu = lNewValue;
}

float CFormDoc::GetHorizontalPosition()
{
	return m_position.left;
}

void CFormDoc::SetHorizontalPosition(float fNewValue)
{
	if (!IsMainForm())
		m_position.OffsetRect(fNewValue - m_position.left, 0);
}

float CFormDoc::GetVerticalPosition()
{
	return m_position.top;
}

void CFormDoc::SetVerticalPosition(float fNewValue)
{
	if (!IsMainForm())
		m_position.OffsetRect(0, fNewValue - m_position.top);
}

float CFormDoc::GetWidth() 
{
	return m_position.Width();
}

void CFormDoc::SetWidth(float fNewValue) 
{
	m_position.right = m_position.left + fNewValue;

	UpdateAllViews(NULL, HINT_UPDATE_DOCSIZE);
}

float CFormDoc::GetHeight() 
{
	return m_position.Height();
}

void CFormDoc::SetHeight(float fNewValue) 
{
	m_position.bottom = m_position.top + fNewValue;
	
	UpdateAllViews(NULL, HINT_UPDATE_DOCSIZE);
}

void CFormDoc::OnVisibleLayerChanged()
{
	UpdateAllViews(NULL);
}

void CFormDoc::Show() 
{
}

void CFormDoc::Hide() 
{
}

void CFormDoc::Close() 
{
}

// 此函数在开发环境无用, 没有实现
LPUNKNOWN CFormDoc::GetHitObject(long lXPos, long lYPos)
{
	return NULL;
}

// 此函数在开发环境无用, 没有实现
LPUNKNOWN CFormDoc::GetObjectByName(LPCTSTR lpszName)
{
	return NULL;
}

// 此函数在开发环境无用, 没有实现
LPDISPATCH CFormDoc::GetObjectCollection()
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：从存贮装载动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::LoadDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	RemoveAllDynamic();

	int nSize;
	pStm->Read(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		DISPID dispid;
		pStm->Read(&dispid, sizeof(DISPID), NULL);
		IUnknownPtr pObject;
		hr = OleLoadFromStream(pStm, IID_IUnknown, (void**)&pObject);
		AddDynamic(dispid, pObject);
		if (FAILED(hr))
			return hr;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
//功能：往存贮保存动态属性
//////////////////////////////////////////////////////////////////////////////////////
HRESULT CFormDoc::SaveDynamicProperty(IStream *pStm)
{
	HRESULT hr;

	if(!pStm)
	{
		return E_POINTER;
	}

	int nSize = m_arLinkInfo.GetSize();;
	pStm->Write(&nSize, sizeof(nSize), NULL);
	for(int nIndex = 0; nIndex < nSize; nIndex++)
	{
		pStm->Write(&m_arLinkInfo[nIndex].dispid, sizeof(DISPID), NULL);
		ICxDynamicObject* pObject = m_arLinkInfo[nIndex].pDynamicObject;
		if (pObject != NULL)
		{
			IPersistStreamPtr spStream;			
			hr = pObject->QueryInterface(IID_IPersistStream, (void**)&spStream);
			if (FAILED(hr))
				return hr;
			if (spStream != NULL)
			{	
				hr = OleSaveToStream(spStream, pStm);
				if (FAILED(hr))
					return hr;
			}
		}
	}

	return S_OK;
}

void CFormDoc::SerializeDynamicProperty(CArchive& ar)
{
	CArchiveStream stm(&ar);
	LPSTREAM pStream = &stm;

	if (ar.IsStoring())
	{
		SaveDynamicProperty(pStream);
	}
	else
	{
		LoadDynamicProperty(pStream);
	}
}

HRESULT CFormDoc::AddDynamic(DISPID dispid, IUnknown *pUnknown)
{
	VARIANT_BOOL bHasConnected;
	HasDynamic(dispid, &bHasConnected);
	if (bHasConnected == VARIANT_TRUE)
	{
		RemoveDynamic(dispid);
	}

		if (pUnknown == NULL)
		return S_OK;

	LINKINFO linkinfo;
	memset(&linkinfo, 0, sizeof(LINKINFO));
	linkinfo.dispid = dispid;
	//在此增加引用
	ICxDynamicObject* pDynamicObject = NULL;
	pUnknown->QueryInterface(IID_ICxDynamicObject, (void **)&pDynamicObject);
	if (pDynamicObject == NULL)
		return S_FALSE;

	linkinfo.pDynamicObject = pDynamicObject;
	m_arLinkInfo.Add(linkinfo);

	return S_OK;
}

HRESULT CFormDoc::HasDynamic(DISPID dispid, VARIANT_BOOL *pConnected)
{
	*pConnected = VARIANT_FALSE;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			*pConnected = VARIANT_TRUE;
			break;
		}
	}

	return S_OK;
}

HRESULT CFormDoc::GetDynamic(DISPID dispid, IUnknown **ppUnknown)
{
	*ppUnknown = NULL;
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->QueryInterface(IID_IUnknown, (void **)ppUnknown);
			break;
		}
	}

	return S_OK;
}

HRESULT CFormDoc::RemoveDynamic(DISPID dispid)
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		if (m_arLinkInfo[i].dispid == dispid)
		{
			m_arLinkInfo[i].pDynamicObject->Release();
			m_arLinkInfo.RemoveAt(i);
			break;
		}
	}

	return S_OK;
}

HRESULT CFormDoc::RemoveAllDynamic()
{
	for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
	{
		m_arLinkInfo[i].pDynamicObject->Release();
	}
	m_arLinkInfo.RemoveAll();
	
	return S_OK;
}

VARIANT_BOOL CFormDoc::IsConnected(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	VARIANT_BOOL bHasConnection = FALSE;
	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		HasDynamic(dispid, &bHasConnection);
	}
	else
	{
		ASSERT(FALSE);
	}

	return bHasConnection;
}

void CFormDoc::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		AddDynamic(dispid, punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}
}

void CFormDoc::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CFormDoc::GetConnectObject(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	LPUNKNOWN punkObject = NULL;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		
		GetDynamic(dispid, &punkObject);
	}
	else
	{
		ASSERT(FALSE);
	}

	return punkObject;
}

void CFormDoc::Disconnect(LPCTSTR pszPropName)
{
	USES_CONVERSION;

	DISPID dispid;
	LPCOLESTR lpOleStr = T2COLE(pszPropName);
    if (SUCCEEDED(((IDispatch *)(&m_xDispatch))->GetIDsOfNames(IID_NULL, (LPOLESTR*)&lpOleStr, 
		1, LOCALE_USER_DEFAULT, &dispid)))
	{
		RemoveDynamic(dispid);
	}
	else
	{
		ASSERT(FALSE);
	}
}

int CFormDoc::GetConnectState(DISPID dispid)
{
	int nState = 0;
	VARIANT_BOOL bHasConnection;
	HasDynamic(dispid, &bHasConnection);
	if (bHasConnection == VARIANT_TRUE)
		nState |= 0x2;

	return nState;
}

void CFormDoc::BringToFront(CDevObj* pObj)
{
   POSITION posObj;

   ASSERT(pObj != NULL);

   posObj = m_objects.Find(pObj);
   ASSERT(posObj != NULL);

   m_objects.RemoveAt(posObj);
   m_objects.AddHead(pObj);
}

void CFormDoc::SendToBack(CDevObj* pObj)
{
   POSITION posObj;

   ASSERT(pObj != NULL);;

   posObj = m_objects.Find(pObj);
   ASSERT(posObj != NULL);

   m_objects.RemoveAt(posObj );
   m_objects.AddTail(pObj);
}

void CFormDoc::Cleanup()
{
	int iProp;
   
	for (iProp = 0; iProp < m_apAmbientProperties.GetSize(); iProp++)
	{
		  delete m_apAmbientProperties[iProp];
	}
	m_apAmbientProperties.SetSize(0);

	delete m_pFontHolder;
	m_pFontHolder = NULL;

	delete[] m_descFont.lpstrName;
	m_descFont.lpstrName = NULL;
}

void CFormDoc::DeleteContents() 
{
	Cleanup();

	CDevObjDoc::DeleteContents();
}

BOOL CFormDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace /*=TRUE*/)
	// Save the document data to a file
	// pszPathName = path name where to save document file
	// if pszPathName is NULL then the user will be prompted (SaveAs)
	// note: pszPathName can be different than 'm_strPathName'
	// if 'bReplace' is TRUE will change file name if successful (SaveAs)
	// if 'bReplace' is FALSE will not change path name (SaveCopyAs)
{
	CString newName = pszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(" #%;/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				newName += strExt;
			}
		}

		if (!AfxGetApp()->DoPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs.\n");
				do 
				{ 
					e->Delete(); 
				} while (0);
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

////////////////////////////////////////////
//功 能：重载此函数解除文档间关系
//说 明：建立关系在
//	CDocument* CFormDoc::CFormSubDoc::CreateChildDocument(CDevDoc* pDoc)
////////////////////////////////////////////////
void CFormDoc::OnCloseDocument()
{
	//如果是编辑模块窗口则不关闭文档
	if (m_pFormObj != NULL)
		return;

	if (IsModified() && m_pFormSubDoc != NULL && m_pFormSubDoc->m_bNeedSave)
		SaveToParentDocument();
	if (m_pFormSubDoc != NULL)
	{
		m_pFormSubDoc->m_pChildDoc = NULL;
		m_pFormSubDoc = NULL;
	}

	if (m_pDocument != NULL)
	{
		CDocHit hit(CDocHit::changeContent, m_pDocument->GetDisplayName());
		theApp.NotifyDocumentChanged(DWORD(m_pDocument), DWORD(&hit));
	}
		
	CDevObjDoc::OnCloseDocument();
}

void CFormDoc::DisplayAsIcon(CDC* pDC, CRect* pRect)
{
	CRect rect = m_position.GetRECT();

	CRect rcDisplay = *pRect;
	if (rect.Width() * pRect->Height() > rect.Height() * pRect->Width())
	{
		int nHeight = MulDiv(rect.Height(), pRect->Width(), rect.Width());
		rcDisplay.top += (rcDisplay.Height() - nHeight) / 2; 
		rcDisplay.bottom = rcDisplay.top + nHeight;
	}
	else
	{
		int nWidth = MulDiv(rect.Width(), pRect->Height(), rect.Height());
		rcDisplay.left += (rcDisplay.Width() - nWidth) / 2; 
		rcDisplay.right = rcDisplay.left + nWidth;
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);
	
	COLORREF clrBackColor;
	OleTranslateColor(m_clrBackColor, 0, &clrBackColor);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), clrBackColor);

	memDC.SetViewportOrg(-rect.left, -rect.top);

	//绘制所有对象
	POSITION posObj;
	CDevObj* pObj;
	unsigned long ulLayer = 1;
	for (int i = 0; i < 30; i++)
	{
		posObj = m_objects.GetHeadPosition();
		
		while (posObj != NULL)
		{
			pObj = m_objects.GetNext(posObj);
			if (pObj->m_nLayer != i)
				continue;
			
			if ((m_ulVisibleLayer & ulLayer) != 0)
			{
				pObj->Draw(&memDC);
			}
		}
		ulLayer = ulLayer << 1;
	}

	memDC.SetViewportOrg(0, 0);

 	pDC->StretchBlt(rcDisplay.left, rcDisplay.top, rcDisplay.Width(), rcDisplay.Height(), 
 			&memDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);

	memDC.SelectObject(pOldBmp);
}

////////////////////////////////////////////////
//功 能：保存文件处理
//说 明：此函数由OnFileSave调用
////////////////////////////////////////////////
BOOL CFormDoc::DoFileSave()
{
	//如果本文档是子文档，进行特殊处理
	if (m_pDocument != NULL)
	{
		SaveToParentDocument();
	}
	else
	{
		//直接调用基类实现
		if (!CDevObjDoc::DoFileSave())
			return FALSE;
	}
	
	return TRUE;
}

////////////////////////////////////////////
//功 能：保存改动文件处理
////////////////////////////////////////////////
BOOL CFormDoc::SaveModified()
{
	//如果是子文档，调用基类实现
	if (m_pDocument != NULL)
		return CDevObjDoc::SaveModified();

	if (!IsModified())
		return TRUE;        // ok to continue

	// get name/title of document
	CString name;
	name = m_strTitle;
	if (name.IsEmpty())
			VERIFY(name.LoadString(AFX_IDS_UNTITLED));

	CString prompt;
	AfxFormatString1(prompt, AFX_IDP_ASK_TO_SAVE, name);
	switch (AfxMessageBox(prompt, MB_YESNOCANCEL, AFX_IDP_ASK_TO_SAVE))
	{
	case IDCANCEL:
		return FALSE;       // don't continue

	case IDYES:
	//	SaveToParentDocument();
		// If so, either Save or Update, as appropriate
		if (!DoFileSave())
			return FALSE;       // don't continue
		break;

	case IDNO:
		// If not saving changes, revert the document
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;    // keep going

}

void CFormDoc::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	m_dwVer = _SUPERCX_VER;
	ar << m_dwVer;

	SerializeProperty(ar);
	//序列化动态属性
	SerializeDynamicProperty(ar);

	//脚本序列化
	if (ar.IsStoring())	
	{
		CScriptView* pScriptView = GetScriptView();
		if (pScriptView != NULL)
			pScriptView->GetText(m_strScript);
	
		ar << m_strScript;
	} 

	ar << m_nLayer;
	ar << m_ulVisibleLayer;
	ar << m_ulLockLayer;

	//调用基类进行客户项目序列化
	CDevObjDoc::CopyToClipboard(ar, pStorage);
}

BOOL CFormDoc::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{ 
	if (ar.IsLoading())	
		Initialize(FALSE);

	ar >> m_dwVer;

	SerializeProperty(ar);
	//序列化动态属性
	SerializeDynamicProperty(ar);

	ar >> m_strScript;
	ar >> m_nLayer;
	ar >> m_ulVisibleLayer;
	ar >> m_ulLockLayer;

	return CDevObjDoc::CreateFromClipboard(ar, pStorage);
}

CFormDoc* CFormDoc::GetTopFormDoc()
{
	CFormDoc* pFormDoc = this;
	while (pFormDoc->m_pFormObj != NULL)
	{
		CFormObj* pFormObj = pFormDoc->m_pFormObj;
		if (!pFormObj->m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)))
			break;

		pFormDoc = (CFormDoc*)pFormObj->m_pDocument;
	}
	
	return pFormDoc;
}

///////////////////////////////////////////////////////////
//功 能：存入存储
///////////////////////////////////////////////////////////
void CFormDoc::WriteToStorage(IStorage* pStorage)
{
//	ASSERT(m_pDocument != NULL);
//	ASSERT(m_lpRootStg != NULL);

	//临时替换
	IStorage* lpOrginRootRtg = m_lpRootStg;
	m_lpRootStg = pStorage;
	BOOL bSameAsLoad = m_bSameAsLoad;
	m_bSameAsLoad = FALSE;

	BOOL bSetScriptSavePoint = m_bSetScriptSavePoint;
	m_bSetScriptSavePoint = GetTopFormDoc()->m_bSetScriptSavePoint;
	
	SaveToStorage();

	CommitItems(!m_bSameAsLoad);

	m_bSetScriptSavePoint = bSetScriptSavePoint;
	
	m_lpRootStg = lpOrginRootRtg;
	m_bSameAsLoad = bSameAsLoad;
}

void CFormDoc::ReadFromStorage(IStorage* pStorage)
{
	//临时替换
	IStorage* lpOrginRootRtg = m_lpRootStg;
	m_lpRootStg = pStorage;
	LoadFromStorage();
	m_lpRootStg = lpOrginRootRtg;
}

////////////////////////////////////////////
//功 能：保存文档到父文档
//说 明：此函数与CDevObjDoc::SaveToStorage相似
//		 注意处理另存为情况，当第一次创建的文档保存时，作为另存为处理
////////////////////////////////////////////////
void CFormDoc::SaveToParentDocument(CDevDoc* pParent)
{
	if (pParent == NULL)
	{
		ASSERT(m_pDocument != NULL);
		pParent = m_pDocument;
	}
	
	ASSERT(m_lpRootStg != NULL);

	LPSTORAGE lpOrigStg = NULL;
	m_bSameAsLoad = pParent->m_bSameAsLoad;

	if (m_bSameAsLoad == FALSE)
	{
		lpOrigStg = m_lpRootStg;
		m_lpRootStg = m_pDocument->CreateSubDocumentStorage(m_pFormSubDoc);
	}
		
	ASSERT(m_lpRootStg != NULL);

	//创建项目流
	COleStreamFile file;
	CFileException fe;
	if (!file.CreateStream(m_lpRootStg, _T("Contents"),
			CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	//保存到项目流
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	TRY
	{
		if (m_bRemember)
			m_bSetScriptSavePoint = TRUE;

		//保存项目
		Serialize(saveArchive);
		saveArchive.Close();
		file.Close();

		// commit each of the items
		CommitItems(m_bRemember && !m_bSameAsLoad);

		// mark document as clean if remembering the storage
		if (m_bRemember)
		{
			m_bSetScriptSavePoint = FALSE;

			SetModifiedFlag(FALSE);
		}

		// remember correct storage or release save copy as storage
		if (!m_bSameAsLoad)
		{
			if (m_bRemember)
			{
				// Save As case -- m_stgRoot is new storage, forget old storage
				lpOrigStg->Release();
			}
			else
			{
				// Save Copy As case -- m_stgRoot should hook up to m_stgOrig.
				m_lpRootStg->Release();
				m_lpRootStg = lpOrigStg;
			}
		}

		// 提交存储
		SCODE sc = m_lpRootStg->Commit(STGC_ONLYIFCURRENT);
		if (sc != S_OK)
			AfxThrowOleException(sc);
	}
	CATCH_ALL(e)
	{
		if (lpOrigStg != NULL)
		{
			// save as failed: abort new storage, and re-attach original
			m_lpRootStg->Release();
			m_lpRootStg = lpOrigStg;
		}

		file.Abort();   // will not throw an exception
		CommitItems(FALSE); // abort save in progress
		saveArchive.Abort();
		THROW_LAST();
	}
	END_CATCH_ALL
}

////////////////////////////////////////////
//功 能：从父文档中读入
//说 明：此函数与CDevObjDoc::LoadFromStorage相似
////////////////////////////////////////////////
void CFormDoc::ReadFromParentDocument()
{
	ASSERT(m_pDocument != NULL);
		
	if (m_lpRootStg == NULL)
		m_lpRootStg = m_pDocument->OpenSubDocumentStorage(m_pFormSubDoc);

	ASSERT(m_lpRootStg != NULL);

	// open Contents stream
	COleStreamFile file;
	CFileException fe;
	if (!file.OpenStream(m_lpRootStg, _T("Contents"),
			CFile::modeRead|CFile::shareExclusive, &fe) &&
		!file.CreateStream(m_lpRootStg, _T("Contents"),
			CFile::modeRead|CFile::shareExclusive|CFile::modeCreate, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	// load it with CArchive (loads from Contents stream)
	CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	TRY
	{
		if (file.GetLength() != 0)
			Serialize(loadArchive);     // load main contents
		loadArchive.Close();
		file.Close();
		UpdateAllViews(NULL);
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		DeleteContents();   // removed failed contents
		loadArchive.Abort();

		if (e->IsKindOf(RUNTIME_CLASS(COleException)))
			e->ReportError(MB_ICONEXCLAMATION);
	
		THROW_LAST();
	}
	END_CATCH_ALL
}

///////////////////////////////////////////////////////////////////////////////


CFormDoc::CFormSubDoc::CFormSubDoc()
{
}

CFormDoc* CFormDoc::CFormSubDoc::CreateFormDoc()
{
	CFormDoc* pFormDoc = new CFormDoc(m_pParentDoc);

	pFormDoc->m_pFormSubDoc = this;
	if (!pFormDoc->OnNewDocument())
	{
		delete pFormDoc;
		return NULL;
	}

	// 创建名称并更新标题
	if (m_strName.IsEmpty())
	{
		if (IsFirstSubDoc())
			m_strName = _T("MainForm");
		else
			m_pParentDoc->CreateUniqueSubDocumentName(_T("SubForm"), m_strName);
	}
	
	pFormDoc->OnUpateTitle();
	
	m_pChildDoc = pFormDoc;

	return pFormDoc;
}

CDocument* CFormDoc::CFormSubDoc::CreateChildDocument(CDevDoc* pDoc)
{
	ASSERT(m_pParentDoc);
	if (m_pChildDoc != NULL)
		return m_pChildDoc;

	CFormDoc* pFormDoc = CreateFormDoc();
	ASSERT(pFormDoc != NULL);

	CLayoutFrame* pNewFrame
			= (CLayoutFrame*)(GetLayoutTemplate()->CreateNewFrame(pFormDoc, NULL));
	ASSERT(pNewFrame != NULL);
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pNewFrame->InitialUpdateFrame(pFormDoc, TRUE);

	return pFormDoc;
}

CDocument* CFormDoc::CFormSubDoc::LoadChildDocument(BOOL bInitFrame)
{
	CFormDoc* pFormDoc = NULL;
	CLayoutFrame* pNewFrame = NULL;
	BOOL bReadFromParentDocument = TRUE;

	if (m_pChildDoc != NULL)
	{
		pFormDoc = (CFormDoc *)m_pChildDoc;

		if (!bInitFrame)
			return pFormDoc;

		if (pFormDoc->GetFirstViewPosition() != NULL)
			return pFormDoc;

		bReadFromParentDocument = FALSE;
	}
	else
	{
		pFormDoc = CreateFormDoc();
		ASSERT(pFormDoc != NULL);
	}

	if (bInitFrame)
	{
		pNewFrame
			= (CLayoutFrame *)(GetLayoutTemplate()->CreateNewFrame(pFormDoc, NULL));
		ASSERT(pNewFrame != NULL);
		ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	}

	TRY
	{
		if (bReadFromParentDocument)
			pFormDoc->ReadFromParentDocument();

		if (pNewFrame != NULL)
 			pNewFrame->InitialUpdateFrame(pFormDoc, TRUE);
		
		pFormDoc->SetModifiedFlag(FALSE);
	}
	CATCH_ALL(e)
	{
 		pFormDoc->OnCloseDocument();
// 		delete pFormDoc;
		pFormDoc = NULL;

		THROW_LAST();
	}
	END_CATCH_ALL

	return pFormDoc;
}

void CFormDoc::CFormSubDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_dwDocumentNumber;
		ar << m_strName;
		//应该在此判断是否需要保存
//		if (m_pChildDoc != NULL && (m_pChildDoc->IsModified() || !m_pParentDoc->IsSameAsLoad())
//			&& m_bNeedSave == TRUE)
//		{
//			m_nSubformModule = 0;
//			((CFormDoc *)m_pChildDoc)->SaveToParentDocument();
//		}

		if (m_bNeedSave == TRUE)
		{
			if (m_pChildDoc != NULL && (m_pChildDoc->IsModified() || !m_pParentDoc->IsSameAsLoad()))
			{
				m_nSubformModule = 0;
				((CFormDoc *)m_pChildDoc)->SaveToParentDocument();
			}
			else if (!m_pParentDoc->IsSameAsLoad())
			{
				ASSERT(m_pChildDoc == NULL);
				
				IStoragePtr pOldStorage;
				IStoragePtr pNewStorage;

				if (m_pStorage != NULL)
				{
					pOldStorage = m_pStorage;
				}
				else
				{
					ASSERT(m_pParentDoc->m_lpOrigStg != NULL);
					
					pOldStorage = m_pParentDoc->OpenSubDocumentStorage(this, m_pParentDoc->m_lpOrigStg);
				}
				
				pNewStorage = m_pParentDoc->CreateSubDocumentStorage(this);

				pOldStorage->CopyTo(NULL, NULL, NULL, pNewStorage);
				SCODE sc = pNewStorage->Commit(STGC_ONLYIFCURRENT);
				if (sc != S_OK)
					AfxThrowOleException(sc);

				pNewStorage->Release();
				pOldStorage->Release();
			}
		}
		else if (m_pChildDoc != NULL && !m_pParentDoc->IsSameAsLoad())
		{
			// 必须更新子文档的存储，否则会出错
			LPSTORAGE lpOrigStg = ((CFormDoc *)m_pChildDoc)->m_lpRootStg;
			if (lpOrigStg != NULL)
				lpOrigStg->Release();

			((CFormDoc *)m_pChildDoc)->m_lpRootStg = m_pParentDoc->CreateSubDocumentStorage(this);
		}

		ar << m_nSubformModule;
	}
	else
	{
		m_pParentDoc = (CDevDoc *)ar.m_pDocument;
		ar >> m_dwDocumentNumber;
		ar >> m_strName;
		ar >> m_nSubformModule;
		m_pParentDoc->m_nSubformModule += m_nSubformModule;
		//保证号码不重复使用
		if (m_dwDocumentNumber >= m_pParentDoc->m_dwNextDocumentNumber)
			m_pParentDoc->m_dwNextDocumentNumber = m_dwDocumentNumber + 1;
	}
}

void CFormDoc::CFormSubDoc::CopyToClipboard(CArchive& ar, IStorage* pStorage) 
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szDocumentName[64];
	IStoragePtr pSubStorage;

	ASSERT_VALID(this);
	ASSERT(ar.IsStoring());

	ar << m_strName;
	//用于确定所创建的子存储
	ar << m_dwDocumentNumber;
	GetDocumentName(szDocumentName);

	//创建子存储
	hResult = pStorage->CreateStorage(T2COLE(szDocumentName), STGM_READWRITE|
		STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	int bOpen = (m_pChildDoc != NULL);
	ar << bOpen;

	//如果子文档打开
	if (bOpen)
	{
		//创建项目流
		COleStreamFile file;
		CFileException fe;
		if (!file.CreateStream(pSubStorage, _T("Contents"),
				CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate, &fe))
		{
			if (fe.m_cause == CFileException::fileNotFound)
				AfxThrowArchiveException(CArchiveException::badSchema);
			else
				AfxThrowFileException(fe.m_cause, fe.m_lOsError);
		}

		//保存到项目流
		CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);

		((CFormDoc *)m_pChildDoc)->CopyToClipboard(saveArchive, pSubStorage);

		saveArchive.Flush();
		file.Close();
	}
	//如果子文档没有打开，直接拷贝存储
	else
	{
		IStoragePtr pFormStorage;
		if (m_pStorage != NULL)
			pFormStorage = m_pStorage;
		else
			pFormStorage = m_pParentDoc->OpenSubDocumentStorage(this);
		pFormStorage->CopyTo(NULL, NULL, NULL, pSubStorage);
		SCODE sc = pSubStorage->Commit(STGC_ONLYIFCURRENT);
		if (sc != S_OK)
			AfxThrowOleException(sc);
		pFormStorage->Release();
	}
}

BOOL CFormDoc::CFormSubDoc::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szDocumentName[64];
	IStoragePtr pSubStorage;

	ASSERT_VALID(this);
	ASSERT(ar.IsLoading());

	ar >> m_strName;
	//用于确定所创建的子存储
	DWORD dwDocumentNumber = m_dwDocumentNumber;
	ar >> m_dwDocumentNumber;
	GetDocumentName(szDocumentName);
	m_dwDocumentNumber = dwDocumentNumber;
	//为空文挡号码，则创建新号码
	if (m_dwDocumentNumber == -1)
	{
		ASSERT(m_pParentDoc != NULL);
		m_dwDocumentNumber = m_pParentDoc->GetNewDocumentNumber();
	}

	//创建子存储
	hResult = pStorage->OpenStorage(T2COLE(szDocumentName), NULL, STGM_READWRITE|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}

	if (m_pParentDoc != NULL)
	{
		if (IsFirstSubDoc())
		{
			m_strName = _T("MainForm");
		}
		else if (!m_pParentDoc->IsUniqueSubDocumentName(m_strName))
		{
			CString strTemp = m_strName;
			RemoveRightDigits(strTemp);
			m_pParentDoc->CreateUniqueSubDocumentName(strTemp, m_strName);
		}
	}
		

	int bOpen;
	ar >> bOpen;

	if (bOpen)
	{
		//创建项目流
		COleStreamFile file;
		CFileException fe;
		if (!file.OpenStream(pSubStorage, _T("Contents"),
				CFile::modeRead|CFile::shareExclusive, &fe))
		{
			if (fe.m_cause == CFileException::fileNotFound)
				AfxThrowArchiveException(CArchiveException::badSchema);
			else
				AfxThrowFileException(fe.m_cause, fe.m_lOsError);
		}

		//保存到项目流
		CArchive loadArchive(&file, CArchive::load | CArchive::bNoFlushOnDelete);

		CFormDoc* pFormDoc = CreateFormDoc();
		ASSERT(pFormDoc != NULL);

		CMDIChildWnd* pNewFrame
			= (CMDIChildWnd*)(GetLayoutTemplate()->CreateNewFrame(pFormDoc, NULL));
		ASSERT(pNewFrame != NULL);
		ASSERT_KINDOF(CMDIChildWnd, pNewFrame);

		TRY
		{
			VERIFY(((CFormDoc *)pFormDoc)->CreateFromClipboard(loadArchive, pSubStorage) == TRUE);
			
			loadArchive.Flush();
			file.Close();
		}
		CATCH_ALL(e)
		{
			file.Abort();   // will not throw an exception
			loadArchive.Abort();

//			pSubStorage->Release();
			
			THROW_LAST();
		}
		END_CATCH_ALL

		//如果存贮不为空，获取存贮信息
		if (m_pStorage != NULL)
		{
			pFormDoc->WriteToStorage(m_pStorage);
			pFormDoc->SetModifiedFlag(FALSE);
			pFormDoc->OnCloseDocument();
			delete m_pChildDoc;
			return TRUE;
		}
		
		pNewFrame->InitialUpdateFrame(pFormDoc, TRUE);
	}
	else
	{
		IStoragePtr pFormStorage;
		if (m_pStorage != NULL)
		{
			pFormStorage = m_pStorage;
			pFormStorage->AddRef();
		}
		else	//没有指定存储，
		{
			pFormStorage = m_pParentDoc->CreateSubDocumentStorage(this);
		}
		pSubStorage->CopyTo(NULL, NULL, NULL, pFormStorage);
		SCODE sc = pFormStorage->Commit(STGC_ONLYIFCURRENT);
		if (sc != S_OK)
			AfxThrowOleException(sc);
		pFormStorage->Release();
		
		if (m_pStorage == NULL)
		{
		//	pFormStorage->Release();
			pFormStorage = NULL;
			LoadChildDocument();
		}
	}

	return TRUE;
}

void CFormDoc::CFormSubDoc::OnParentDocTitleChanged()
{
	if (m_pChildDoc != NULL)
		((CFormDoc *)m_pChildDoc)->OnUpateTitle();
}

void CFormDoc::CFormSubDoc::OnParentDocChanged()
{
	if (m_pChildDoc != NULL)
		((CFormDoc *)m_pChildDoc)->OnSymbolChanged(UPDATE_SYMBOL_FORM);
}

//////////////////////////////////////////////////http://news.qq.com/a/20071124/000034.htm
//功 能：应该保存到工程文档
////////////////////////////////////////////////////////
void CFormDoc::OnFileSave() 
{
	if (m_pDocument != NULL)
		m_pDocument->SaveSubDocument(m_pFormSubDoc);
	else
		GetTopFormDoc()->OnFileSave();
}

//////////////////////////////////////////////////
//功 能：可以相当于导出到文件
////////////////////////////////////////////////////////
void CFormDoc::OnFileExport() 
{
	CFileDialog saveFileDlg(FALSE);
	saveFileDlg.m_ofn.lpstrFilter = "窗体导出文件(*.fm)\0*.fm\0\0";
	saveFileDlg.m_ofn.lpstrDefExt = "fm";
	if (saveFileDlg.DoModal() == IDOK)
	{
		try
		{
			CString strFileName = saveFileDlg.GetPathName();
			SaveToFile(strFileName);
		}
		catch(CException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

/////////////////////////////////////////////////////////////
//功能:把文档存入一个指定的流中
/////////////////////////////////////////////////////////////
HRESULT CFormDoc::StreamSerialize(IStream *pStream)
{
	return E_FAIL;
}

/////////////////////////////////////////////////////////////
//功能:把窗体以文件的形式保存
/////////////////////////////////////////////////////////////
void CFormDoc::SaveToFile(CString fileName)
{
	//创建文件
	CFile file;
	CFileException fe;

	if(!file.Open(fileName, CFile::modeCreate | CFile::modeWrite, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	//保存到项目流
	CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	TRY
	{
		//保存项目
		BOOL oldFlag = IsModified();
		Serialize(saveArchive);
		SetModifiedFlag(oldFlag);
		saveArchive.Close();
		file.Close();
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		CommitItems(FALSE); // abort save in progress
		saveArchive.Abort();
		THROW_LAST();
	}
	END_CATCH_ALL
}

/////////////////////////////////////////////////////////////
//功能:引入一个原引出的窗体,到所属DevDoc中
/////////////////////////////////////////////////////////////
void CFormDoc::OnFileImport()
{
	ASSERT(m_pDocument);

	CFileDialog openFileDlg(TRUE, "引入");
	openFileDlg.m_ofn.lpstrFilter = "窗体导出文件(*.fm)\0*.fm\0\0";
	openFileDlg.m_ofn.lpstrDefExt = "fm";
	if(openFileDlg.DoModal() == IDOK)
	{
		try
		{
			CString strFileName = openFileDlg.GetPathName();
			m_pDocument->ImportForm(strFileName);
		}
		catch(CException* pe)
		{
			pe->ReportError();
			pe->Delete();
		}
	}
}

//////////////////////////////////////////////////////////////////////
//功能:从一个文件中引入并创建窗体, 如子文档已经存在则返加原来的文档指针
//////////////////////////////////////////////////////////////////////
CDocument* CFormDoc::CFormSubDoc::LoadChildDocumentFromFile(LPTSTR fileName)
{
	if (m_pChildDoc != NULL)
		return m_pChildDoc;

//	CFormDoc* pFormDoc = new CFormDoc(m_pParentDoc);
//
//	m_strName = m_strName;
//	if (!pFormDoc->LoadFromFile(fileName))
//	{
//		delete pFormDoc;
//		return NULL;
//	}
//
//	m_pChildDoc = pFormDoc;
//	pFormDoc->m_pFormSubDoc = this;

	//
	CFormDoc* pFormDoc = CreateFormDoc();
	ASSERT(pFormDoc != NULL);
	
	if (!pFormDoc->LoadFromFile(fileName))
	{
		delete pFormDoc;
		return NULL;
	}
	//

	CMDIChildWnd* pNewFrame
			= (CMDIChildWnd*)(GetLayoutTemplate()->CreateNewFrame(pFormDoc, NULL));
	ASSERT(pNewFrame != NULL);
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pNewFrame->InitialUpdateFrame(pFormDoc, TRUE);

	pFormDoc->m_lpRootStg = m_pParentDoc->CreateSubDocumentStorage(this);

	pFormDoc->SaveToParentDocument();
	return pFormDoc;
}


/////////////////////////////////////////////////////////////
//功能:一个文件中的内容来初始化本对象
//返回:成功TRUE,失败FALSE
/////////////////////////////////////////////////////////////
BOOL CFormDoc::LoadFromFile(LPTSTR strFileName)
{
	BOOL bRet = FALSE;

	//创建文件
	CFile file;
	CFileException fe;
	if(!file.Open(strFileName, CFile::modeRead, &fe))
	{
		if (fe.m_cause == CFileException::fileNotFound)
			AfxThrowArchiveException(CArchiveException::badSchema);
		else
			AfxThrowFileException(fe.m_cause, fe.m_lOsError);
	}

	//装入文件
	CArchive loadArchive(&file, CArchive::load);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;

	TRY
	{
		//装入项目
		Serialize(loadArchive);
		loadArchive.Close();
		file.Close();
		bRet = TRUE;
	}
	CATCH_ALL(e)
	{
		file.Abort();   // will not throw an exception
		CommitItems(FALSE); // abort save in progress
		loadArchive.Abort();
		THROW_LAST();
	}
	END_CATCH_ALL

	return bRet;
}

void CFormDoc::CFormSubDoc::SaveChildDocument(IStorage* pStorage)
{
	if (m_pChildDoc != NULL)
		((CFormDoc *)m_pChildDoc)->WriteToStorage(pStorage);
}

BOOL CFormDoc::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet = FALSE;

	if(CDevObjDoc::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	if(m_pDocument != NULL)
		bRet = m_pDocument->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	return bRet;
}

//////////////////////////////////////////////////////////
//功能:击活该子文档时由Frame调用
//////////////////////////////////////////////////////////
void CFormDoc::OnActivate()
{
//	ASSERT(m_pFormSubDoc != NULL);
	CDevDoc* pDevDoc;
	DWORD dwData;
	if (m_pFormObj != NULL)
	{
		pDevDoc = m_pFormObj->GetDevDoc(); 
		dwData = (DWORD)m_pFormObj;
	}
	else 
	{
		pDevDoc = m_pDocument;
		dwData = (DWORD)m_pFormSubDoc;
	}
	
	theApp.NotifyDocumentActivate((DWORD)pDevDoc, dwData);
}


void CFormDoc::OnFormObjChange()
{
	CDevDoc* pDevDoc;
	if (m_pFormObj != NULL)
		pDevDoc = m_pFormObj->GetDevDoc();
	else
		pDevDoc = m_pDocument;

	CDocHit hit(CDocHit::changeContent, pDevDoc->GetDisplayName());
	theApp.NotifyDocumentChanged(DWORD(pDevDoc), DWORD(&hit));

//	DWORD dwData;
//	if (m_pFormObj != NULL)
//		dwData = (DWORD)m_pFormObj;
//	else 
//		dwData = (DWORD)m_pFormSubDoc;

//	theApp.NotifyDocumentActivate((DWORD)pDevDoc, dwData);
}

//////////////////////////////////////////////////////////////////
//功能:在关闭对应script View前被调用,用于保存view中的文本
//////////////////////////////////////////////////////////////////
void CFormDoc::PreCloseFrame( CFrameWnd* pFrame )
{
	POSITION posItem;
	CCtrlItem* pItem;
	
	posItem = GetStartPosition();
	while (posItem != NULL)
	{
		pItem = (CCtrlItem *)GetNextItem(posItem);
		if (pItem != NULL)
		{
			if (pItem->IsInPlaceActive())
			{
				pItem->Deactivate();
				//如果不加入下句，部分控件无法完全释放，如msform的框架等
				pItem->Close(OLECLOSE_NOSAVE);
			}
		}
	}

	POSITION pos = m_listSubformModule.GetHeadPosition();
	while (pos != NULL)
	{
		CFormObj* pFormObj = (CFormObj *)m_listSubformModule.GetNext(pos);
		pFormObj->m_pFormDoc->PreCloseFrame(pFrame);	
	}

	CDevObjDoc::PreCloseFrame(pFrame);

	CScriptView* pScriptView = GetScriptView();
	if (pScriptView != NULL)
		pScriptView->GetText(m_strScript);
}

void CFormDoc::UpdateCtrlItemsPosition()
{
	POSITION posItem;
	CCtrlItem* pItem;
	
	posItem = GetStartPosition();
	while (posItem != NULL)
	{
		pItem = (CCtrlItem *)GetNextItem(posItem);
		if (pItem != NULL)
		{
			if (pItem->IsInPlaceActive())
				pItem->SetItemRects();
		}
	}
}

//////////////////////////////////////////////////////////////////
//功能:判断是否为主窗口
//////////////////////////////////////////////////////////////////
BOOL CFormDoc::IsMainForm()
{
	if (m_pFormSubDoc)
		return m_pFormSubDoc->IsFirstSubDoc();
	
	return FALSE;
}

//////////////////////////////////////////////////////////////
//功能:设定Form的宽和高
//参数:nWidth[in]		宽
//		nHeight[in]		高
//////////////////////////////////////////////////////////////
void CFormDoc::SetDocSize(float fWidth, float fHeight, BOOL bObject)
{
	if (m_pFormObj != NULL)
	{
		if (!bObject)
		{
			CRectF position = m_pFormObj->m_position;

			position.right = position.left + fWidth;
			position.bottom = position.top + fHeight;
			m_pFormObj->MoveTo(position, FALSE);
		}
	
		m_position = m_pFormObj->m_position;
	}
	else
	{
		if (fWidth < 0 || fHeight < 0)
			return;

		m_position.right = m_position.left + fWidth;
		m_position.bottom = m_position.top + fHeight;
	}

	UpdateAllViews(NULL, HINT_UPDATE_DOCSIZE);
}

void CFormDoc::SetModifiedFlag(BOOL bModified)
{
	if (bModified)
	{
		CFormDoc* pFormDoc = this;
		while (pFormDoc->m_pFormObj != NULL)
		{
			CFormObj* pFormObj = pFormDoc->m_pFormObj;
			pFormDoc = (CFormDoc*)pFormDoc->m_pFormObj->m_pDocument;
			pFormDoc->SetModifiedFlag(bModified);
		}
	}

	CDevObjDoc::SetModifiedFlag(bModified);
}

void CFormDoc::UpdateAllAssocViews(CView* pSender, LPARAM lHint, CObject* pHint)
	// walk through all views
{
	CFormDoc* pFormDoc = this;
	while (pFormDoc->m_pFormObj != NULL)
	{
		CFormObj* pFormObj = pFormDoc->m_pFormObj;
		pFormDoc = (CFormDoc*)pFormDoc->m_pFormObj->m_pDocument;
		((COleDocument *)pFormDoc)->UpdateAllViews(pSender, HINT_UPDATE_DRAWOBJ, pFormObj);
	}
}

//获取关联标签
CString CFormDoc::GetAssocTagGroupEx(BOOL bIncludeThis)
{
	CString strTagGroup;

	CFormDoc* pFormDoc = NULL;
	if (bIncludeThis)
		pFormDoc = this;
	else if (m_pFormObj)
		pFormDoc = (CFormDoc*)m_pFormObj->m_pDocument;
	while (pFormDoc != NULL)
	{
		if (!strTagGroup.IsEmpty())
			strTagGroup.Insert(0, _T('.'));
		strTagGroup.Insert(0, pFormDoc->m_strTagGroup);
		//直到是顶层标签或者没有上级模块止
		if (strTagGroup.Left(4).CompareNoCase(_T("Tags")) == 0 || pFormDoc->m_pFormObj == NULL)
			break;

		pFormDoc = (CFormDoc*)pFormDoc->m_pFormObj->m_pDocument;
	}

	return strTagGroup;
}

BOOL CFormDoc::IsAssocTagGroupValid(LPCSTR lpszTagGroup)
{
	CString strTagGroup(lpszTagGroup);
	if (strTagGroup.IsEmpty())
		return TRUE;

	if (strTagGroup.Left(4).CompareNoCase(_T("Tags")) == 0)
	{
		TCHAR ch = strTagGroup.GetAt(4);
		if (ch == _T('.') || ch == _T('\0'))
			return TRUE;
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//功 能：绘制画面文档
//////////////////////////////////////////////////////////////////////////
void CFormDoc::DrawDoc(CDC *pDC)
{
	POSITION posObj;
	CDevObj* pObj;

	unsigned long ulLayer = 1;
	for (int i = 0; i < 30; i++)
	{
		posObj = m_objects.GetHeadPosition();
		
		while (posObj != NULL)
		{
			pObj = m_objects.GetNext(posObj);
			if (pObj->m_nLayer != i)
				continue;
			
			if ((m_ulVisibleLayer & ulLayer) != 0)
				pObj->Draw(pDC);
		}
		ulLayer = ulLayer << 1;
	}
}

void CFormDoc::PrintDoc(CDC *pDC)
{
	POSITION posObj;
	CDevObj* pObj;
	
	unsigned long ulLayer = 1;
	for (int i = 0; i < 30; i++)
	{
		posObj = m_objects.GetHeadPosition();
		
		while (posObj != NULL)
		{
			pObj = m_objects.GetNext(posObj);
			if (pObj->m_nLayer != i)
				continue;
			
			if ((m_ulVisibleLayer & ulLayer) != 0)
			{
				pObj->Print(pDC);
			}
		}
		ulLayer = ulLayer << 1;
	}
}

///////////////////////////////////////////////////////////////
//功能:把source文档中的内容复制到本对象中
///////////////////////////////////////////////////////////////
void CFormDoc::operator =(CFormDoc &source)
{
	IStorage* pStorage;
	HRESULT hr = StgCreateStorageEx(NULL, STGM_CREATE | STGM_READWRITE | STGM_DELETEONRELEASE | STGM_SHARE_EXCLUSIVE, 
		STGFMT_DOCFILE , 0, NULL, 0, IID_IStorage , (void **)&pStorage);
	if(FAILED(hr))
	{
		AfxThrowOleException(hr);
	}

	COleStreamFile file;
	if(!file.CreateStream(pStorage, _T("Contents")))
	{
		pStorage->Release();
		return;
	}

	CArchive arStore(&file, CArchive::store);
	arStore.m_pDocument = &source;
	BOOL oldflag = source.IsModified();
	source.Serialize(arStore);
	source.SetModifiedFlag(oldflag);
	arStore.Close();

	file.SeekToBegin();
	CArchive arLoad(&file, CArchive::load);
	arLoad.m_pDocument = this;
	Serialize(arLoad);
	SetModifiedFlag();
	arLoad.Close();

	file.Close();
	pStorage->Release();
}

CFormDoc* CFormDoc::m_pExpCrtDoc = NULL;
EXTERN_C const CLSID CLSID_CxExpressionCreator;
//const CLSID CLSID_CxExpressionCreator = {0x65FC5AEC,0xF442,0x421E,{0xBD,0xA1,0xB8,0x44,0xF9,0xC0,0xA0,0xEC}};
//const CLSID CLSID_ExpressionCreator = {0x227008D5,0xF29A,0x4E28,{0xB2,0xB3,0x96,0x4D,0x38,0x03,0x8C,0xB5}};

void CFormDoc::ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag) 
{
	USES_CONVERSION;
	
	ASSERT(m_pExpCrtDoc != NULL);

	CComBSTR bstrExpression;
	bstrExpression.Attach(*pbstrExpression);

	BOOL bShowHistoryPage = ((iFlag & 0xffff0000) != 0);
	iFlag &= 0xffff;

	switch (iFlag)
	{
	case 0:
		{
			CComPtr<ICxExpressionCreator> spExprCrt;
			CComPtr<ICxSymbolTreePage> spSymbolTreePage;

			spExprCrt.CoCreateInstance(CLSID_CxExpressionCreator);

			spExprCrt->put_Expression(bstrExpression);
			spExprCrt->AddGeneralSymbolPage(CX_ALL_SYMBOL);

			spExprCrt->AddSymbolTreePage(L"窗体", &spSymbolTreePage);
			spSymbolTreePage->SetDefaultNameSpace(T2BSTR(m_pExpCrtDoc->GetDisplayName()));
			spSymbolTreePage->SetSymbolTreeSite(&m_pExpCrtDoc->m_xSymbolTreeSite);

			if (m_pExpCrtDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
			{
				CComPtr<ICxSymbolTreePage> spReportSymbolTreePage;
				spExprCrt->AddSymbolTreePage(L"记录集", &spReportSymbolTreePage);

				CReportDoc* pReportDoc = (CReportDoc *)m_pExpCrtDoc;

				COleDispatchDriver ddTemp(spReportSymbolTreePage, FALSE);
				
				OLE_HANDLE hItem = NULL;
				static BYTE BASED_CODE parms[] =
					VTS_HANDLE VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;

				// 数据记录集
				IDispatch* pIObj = (IDispatch *)pReportDoc->m_pDataRecordset;
				ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_HANDLE, &hItem, parms, 
						NULL, _T("DataRecordset"), pIObj, NULL);
			
				if (pReportDoc->m_pDataRecordset->Open() == S_OK)
				{
					long lFieldCount;
					pReportDoc->m_pDataRecordset->get_FieldCount(&lFieldCount);
					for (long l = 0; l < lFieldCount; l++)
					{
						ICxDataFieldPtr pDataField;
						CComVariant vaIndex = l + 1;
						pReportDoc->m_pDataRecordset->get_Field(vaIndex, &pDataField);
						CComBSTR bstrFieldName;
						pDataField->get_Name(&bstrFieldName);
						CString strName = _T("DataRecordset(\"");
						strName += OLE2T(bstrFieldName);
						strName += _T("\")");
						pIObj = (IDispatch *)pDataField;
						ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, NULL, strName, pIObj, NULL);
					}
				}
			}

			spExprCrt->AddOPCServerSymbolPage();

			spExprCrt->ShowDialog();
			bstrExpression.Empty();
			spExprCrt->get_Expression(&bstrExpression);


//			COleDispatchDriver dd(pIExprCrt, FALSE);
//	
//			dd.SetProperty(1, VT_BSTR, strExpression);
//
//			// 添加常用符号叶面
//			dd.InvokeHelper(4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);

//			CString strTagGroup = m_pExpCrtDoc->GetAssocTagGroupEx(TRUE);
//			if (m_pExpCrtDoc->IsAssocTagGroupValid(strTagGroup) && !strTagGroup.IsEmpty())
//			{
//				static BYTE BASED_CODE parms[] = VTS_BSTR;
//				
//				dd.InvokeHelper(7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strTagGroup);
//			}
//
//			if (bShowHistoryPage)
//				dd.InvokeHelper(5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
//
//			// 增加 OPC 服务器页面
//			dd.InvokeHelper(6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
//
//			//添加本文档树状页面
//			IDispatchPtr pIExprTreepage;
//			dd.InvokeHelper(4, DISPATCH_METHOD, VT_DISPATCH, &pIExprTreepage, NULL);
//			if (pIExprTreepage)
//			{
//				COleDispatchDriver ddTemp(pIExprTreepage, FALSE);
//				CString strCaption = m_pExpCrtDoc->m_strCaption;
//				ddTemp.SetProperty(1, VT_BSTR, strCaption);
//				
//				//窗体
//				static BYTE BASED_CODE parms[] =
//					VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;
//				CString strName = m_pExpCrtDoc->GetDisplayName();
//				IDispatch* pIObj = (IDispatch *)m_pExpCrtDoc->GetObjectUnknown();
//				ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strName, pIObj, NULL);
//
//				//加入所有对象
//				CDevObjList& objects = m_pExpCrtDoc->m_objects;
//				POSITION pos = objects.GetHeadPosition();
//				ICxEnumExtendProperty* pEnumExtendProperty = NULL;
//				while (pos != NULL)
//				{
//					CDevObj* pObj = objects.GetNext(pos);
//					strName = pObj->GetDisplayName();
//					pIObj = (IDispatch *)pObj->GetObjectUnknown();
//					pEnumExtendProperty = pObj->EnumExtendProperty();
//					ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strName, pIObj, pEnumExtendProperty);
//					if (pEnumExtendProperty)
//						pEnumExtendProperty->Release();
//				}
//			}
//
//			// 添加记录集页面
//			if (m_pExpCrtDoc->IsKindOf(RUNTIME_CLASS(CReportDoc)))
//			{
//				CReportDoc* pReportDoc = (CReportDoc *)m_pExpCrtDoc;
//
//				IDispatchPtr pIExprTreepage;
//				dd.InvokeHelper(4, DISPATCH_METHOD, VT_DISPATCH, &pIExprTreepage, NULL);
//				
//				if (pIExprTreepage)
//				{
//					COleDispatchDriver ddTemp(pIExprTreepage, FALSE);
//					ddTemp.SetProperty(1, VT_BSTR, "记录集");
//
//					
//					static BYTE BASED_CODE parms[] =
//						VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;
//
//					// 数据记录集
//					IDispatch* pIObj = (IDispatch *)pReportDoc->m_pDataRecordset;
//					ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, 
//							_T("DataRecordset"), pIObj, NULL);
//
//					
//					if (pReportDoc->m_pDataRecordset->Open() == S_OK)
//					{
//						long lFieldCount;
//						pReportDoc->m_pDataRecordset->get_FieldCount(&lFieldCount);
//						for (long l = 0; l < lFieldCount; l++)
//						{
//							ICxDataFieldPtr pDataField;
//							CComVariant vaIndex = l + 1;
//							pReportDoc->m_pDataRecordset->get_Field(vaIndex, &pDataField);
//							CComBSTR bstrFieldName;
//							pDataField->get_Name(&bstrFieldName);
//							CString strName = _T("DataRecordset(\"");
//							strName += OLE2T(bstrFieldName);
//							strName += _T("\")");
//							pIObj = (IDispatch *)pDataField;
//							ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strName, pIObj, NULL);
//						}
//					}
//				}
//			}

//			dd.InvokeHelper(1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
//			dd.GetProperty(2, VT_BSTR, (void*)&strExpression);
		}
		break;
	case 1: //原始->显示
		break;
	case 2: //显示->原始
		break;
	}

	*pbstrExpression = bstrExpression.Copy();
}

void CFormDoc::UpdateObjectsEventStatus()
{
	USES_CHINESEVAR_CONVERSION;

	POSITION pos = m_objects.GetHeadPosition();
	CDevObj* pObj;
	while (pos != NULL)
	{
		pObj = m_objects.GetNext(pos);
		pObj->SetEnventSinkStatus(FALSE);
	}
	
	int nStart = 0;
	int nFind;
	TCHAR* pBuf = m_strScript.GetBuffer(m_strScript.GetLength());
	TCHAR* p = pBuf;
	TCHAR ch;
	while ((nFind = m_strScript.Find( _T( '_' ), nStart)) != -1)
	{
		p = pBuf + nFind;
		do
		{
			ch = *(--p);
		} while ((_istalnum(ch) || ch == '_') && p > pBuf);
		
		ch = *(++p);
		if (_istalpha(ch))
		{
			CString strName(p, pBuf + nFind - p);
			strName = VARA2C(strName);
			if (m_mapNamedObjects.LookupNoCase(strName, pObj))
			{
				pObj->SetEnventSinkStatus(TRUE);
			}
		}
		nStart = nFind + 1;
	}
	
	m_strScript.ReleaseBuffer();
}

// 判断是否有对象的事件相应函数
BOOL CFormDoc::HasObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent)
{
	CString strSubBody;
	return GetObjectEventHandler(pObj, lpszEvent, strSubBody);
}

BOOL CFormDoc::DeleteObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent)
{
	CString strScript;
	CScriptView* pScriptView = GetScriptView();
	if (pScriptView != NULL)
		pScriptView->GetText(strScript);
	else
		strScript = m_strScript;
	
	int nSubBegin;
	int nSubBodyBegin;
	int nSubBodyEnd;
	int nSubEnd;
	if (GetObjectEventHandler(strScript, pObj, lpszEvent, 
		nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd))
	{
		// 去除换行符号
		if (nSubBegin > 0)
		{
			int nCount = 0;
			while (nSubBegin > 1 && (strScript[nSubBegin - 1] == _T('\n') || strScript[nSubBegin - 1] == _T('\r')))
			{
				nSubBegin--;
				nCount++;
			}

			if (nCount >= 2)
				nSubBegin += 2;
		}
		else
		{
			int nLen = strScript.GetLength();
			while (nSubEnd < nLen - 1 && (strScript[nSubEnd] == _T('\n') || strScript[nSubEnd] == _T('\r')))
			{
				nSubEnd++;
			}
		}

		if (pScriptView != NULL)
		{
			pScriptView->SetSelectionStart(nSubBegin);
			pScriptView->SetSelectionEnd(nSubEnd);
			pScriptView->ReplaceSel(_T(""));

//			pScriptView->GotoLine(0);
		}
		else
		{
			m_strScript.Delete(nSubBegin, nSubEnd - nSubBegin);
		}

		SetModifiedFlag();
		return TRUE;
	}
	
	return FALSE;
}

BOOL CFormDoc::InsertOrReplaceObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent, LPCTSTR lpszParams, LPCTSTR lpszSubBody)
{
	CString strScript;
	CScriptView* pScriptView = GetScriptView();
	if (pScriptView != NULL)
		pScriptView->GetText(strScript);
	else
		strScript = m_strScript;
	
	int nSubBegin;
	int nSubBodyBegin;
	int nSubBodyEnd;
	int nSubEnd;
	// 替换
	if (GetObjectEventHandler(strScript, pObj, lpszEvent, 
		nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd))
	{
		CString strSubBody = lpszSubBody;
		strSubBody += _T("\r\n");

		if (pScriptView != NULL)
		{
			pScriptView->SetSelectionStart(nSubBodyBegin);
			pScriptView->SetSelectionEnd(nSubBodyEnd);
			pScriptView->ReplaceSel(strSubBody);
		}
		else
		{
			m_strScript.Delete(nSubBodyBegin, nSubBodyEnd - nSubBodyBegin);
			m_strScript.Insert(nSubBodyBegin, strSubBody);
		}
	}
	// 增加
	else
	{
		CString strEventSubName;
		if (pObj == NULL)
			strEventSubName += GetClassName();
		else
			strEventSubName += pObj->GetDisplayName();
		strEventSubName += _T('_');
		strEventSubName += lpszEvent;

		int nStart = -1;
		int nEnd = -1;
		CScriptAnalyzer sa(strScript);
		sa.GetInsertPosition(strEventSubName, nStart, nEnd);
		ATLASSERT(nStart != -1 && nEnd != -1);
		
		CString strReplaceText;
		if (nStart > 0)
			strReplaceText += _T("\r\n");
		strReplaceText += _T("Sub ");
		if (pObj == NULL)
			strReplaceText += GetClassName();
		else
			strReplaceText += pObj->GetDisplayName();
		strReplaceText += _T("_");
		strReplaceText += lpszEvent;
		strReplaceText += _T("(");
		strReplaceText += lpszParams;
		strReplaceText += _T(")\r\n");
		strReplaceText += lpszSubBody;
		strReplaceText += _T("\r\nEnd Sub\r\n\r\n");

		if (pScriptView != NULL)
		{
			pScriptView->SetSelectionStart(nStart);
			pScriptView->SetSelectionEnd(nEnd);
			pScriptView->ReplaceSel(strReplaceText);

//			pScriptView->GotoLine(pScriptView->GetLineCount() - 3);
		}
		else
		{
			m_strScript.Delete(nStart, nEnd - nStart + 1);
			m_strScript.Insert(nStart, strReplaceText);
		}	
	}
	
	SetModifiedFlag();
	return TRUE;
}

BOOL CFormDoc::GetObjectEventHandler(LPCTSTR lpszScript, CDevObj* pObj, LPCTSTR lpszEvent, 
		int& nSubBegin, int& nSubBodyBegin, int& nSubBodyEnd, int& nSubEnd)
{
	CString strObject;
	if (pObj == NULL)
		strObject = GetClassName();
	else
		strObject = pObj->GetDisplayName();

	CScriptAnalyzer sa(lpszScript);

	return sa.GetObjectEventHandler(strObject, lpszEvent, nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd);
}

BOOL CFormDoc::GetObjectEventHandler(LPCTSTR lpszScript, CDevObj* pObj, CStringArray& arrEventSubs)
{
	CString strObject;
	if (pObj == NULL)
		strObject = GetClassName();
	else
		strObject = pObj->GetDisplayName();

	CScriptAnalyzer sa(lpszScript);

	return sa.GetObjectEventHandler(strObject, arrEventSubs);
}

BOOL CFormDoc::GetObjectEventHandler(CDevObj* pObj, LPCTSTR lpszEvent, CString& strSubBody)
{
	CString strScript;
	CScriptView* pScriptView = GetScriptView();
	if (pScriptView != NULL)
		pScriptView->GetText(strScript);
	else
		strScript = m_strScript;

	if (lpszEvent != NULL)
	{
		int nSubBegin;
		int nSubBodyBegin;
		int nSubBodyEnd;
		int nSubEnd;
		if (GetObjectEventHandler(strScript, pObj, lpszEvent, 
			nSubBegin, nSubBodyBegin, nSubBodyEnd, nSubEnd))
		{
			strSubBody = strScript.Mid(nSubBodyBegin, nSubBodyEnd - nSubBodyBegin);
			return TRUE;
		}
	}
	else
	{
		CStringArray arrEventSubs;
		if (GetObjectEventHandler(strScript, pObj, arrEventSubs))
		{
			for (int i = 0; i < arrEventSubs.GetSize(); i++)
			{
				if (!strSubBody.IsEmpty())
					strSubBody += " ";
				strSubBody += arrEventSubs[i];
			}

			return TRUE;
		}
	}
	
	return FALSE;
}

HRESULT CFormDoc::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	switch (dispid)
	{
		case CXPROPID_HorPosition:
		case CXPROPID_VerPosition:
		case CXPROPID_Width:
		case CXPROPID_Height:
		case CXPROPID_StartUpPosition:
			*ppropcat = -4;
			break;
		case CXPROPID_Caption:
		case CXPROPID_Font:
		case CXPROPID_BackColor:
		case CXPROPID_ForeColor:
		case CXPROPID_BrushImageFileName:
		case CXPROPID_GridEnabled:
		case CXPROPID_GridColor:
		case CXPROPID_BorderStyle:
		case CXPROPID_ControlBox:
			*ppropcat = -5;
			break;
		default:
			*ppropcat = -2;
			break;
	}
		
	return S_OK;
}

HRESULT CFormDoc::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	switch (propcat)
	{
	case -2:
		*pbstrName = ::SysAllocString(OLESTR("杂项"));
		break;
	case -5:
		*pbstrName = ::SysAllocString(OLESTR("外观"));
		break;
	case -4:
		*pbstrName = ::SysAllocString(OLESTR("位置"));
		break;
	default:
		return S_FALSE;
	}	
	
	return S_OK;
}

HRESULT CFormDoc::GetMenuDisplayStrings(CALPOLESTR *pcaStringsOut, CADWORD *pcaCookiesOut)
{
	USES_CONVERSION;

	CStringArray arrMenuStrings;

	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Menu"))
			arrMenuStrings.Add(pObj->GetDisplayName());
	}
	
	int nCount = arrMenuStrings.GetSize();

	if (pcaStringsOut != NULL)
	{
		pcaStringsOut->cElems = nCount;
		pcaStringsOut->pElems = (LPOLESTR *)CoTaskMemAlloc(sizeof(LPOLESTR) * nCount);
		
		for (int i = 0; i < nCount; i++)
			pcaStringsOut->pElems[i] = CoTaskMemAllocString(T2CW(arrMenuStrings[i]));	
	}

	if (pcaCookiesOut != NULL)
	{
		pcaCookiesOut->cElems = nCount;
		pcaCookiesOut->pElems = (DWORD *)CoTaskMemAlloc(sizeof(DWORD) * nCount);

		for (int i = 0; i < nCount; i++)
			pcaCookiesOut->pElems[i] = i;
	}

	return S_OK;
}

HRESULT CFormDoc::GetMenuPredefinedValue(DWORD dwCookie, VARIANT *pVarOut)
{
	CStringArray arrMenuStrings;
	
	POSITION pos = m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CDevObj* pObj = m_objects.GetNext(pos);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Menu"))
			arrMenuStrings.Add(pObj->GetDisplayName());
	}

	if (dwCookie < 0 || dwCookie >= (DWORD)arrMenuStrings.GetSize())
		return E_INVALIDARG;

	pVarOut->vt = VT_BSTR;
	pVarOut->bstrVal = arrMenuStrings[dwCookie].AllocSysString();

	return S_OK;
}

void CFormDoc::GetObjectNames(CStringArray& arrObjects)
{
	//加入窗体对象本身
	arrObjects.Add(GetClassName());

	if (!IsKindOf(RUNTIME_CLASS(CReportDoc)))
	{
		//加入窗体上的所有对象
		CDevObjList* pObjList = GetObjects();
		POSITION pos = pObjList->GetHeadPosition();
		while (pos != NULL)
		{
			CDevObj* pObj = pObjList->GetNext(pos);
			arrObjects.Add(pObj->GetDisplayName());
		}
	}
}

void CFormDoc::GetObjectEventOrParams(LPUNKNOWN pUnkObject, CStringArray& arrInfos, LPCTSTR lpszEvent)
{
	USES_CONVERSION;

	CComPtr<IProvideClassInfo> spProvideClassInfo;
	CComPtr<ITypeInfo> spClassInfo;
	CComPtr<ITypeInfo> spTypeInfo;
	TYPEATTR* pClassTypeAttr = NULL;
	TYPEATTR* pTypeAttr = NULL;
	int nDefaultIndex = -1;
	LONG lDefaultMemberID;

	// 获取类信息
	HRESULT hr = pUnkObject->QueryInterface(&spProvideClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	hr = spProvideClassInfo->GetClassInfo(&spClassInfo);
	if (FAILED(hr))
		goto ONERROR;
	
	spClassInfo->GetTypeAttr(&pClassTypeAttr);
	ASSERT(pClassTypeAttr->typekind == TKIND_COCLASS);

	//遍历类所实现的接口
	int i;
	for (i = 0; i < pClassTypeAttr->cImplTypes; i++)
	{
		int nImplTypeFlags; 
		spClassInfo->GetImplTypeFlags(i, &nImplTypeFlags);
		if ((nImplTypeFlags & IMPLTYPEFLAG_FSOURCE) == 0)	// 不是事件接口
			continue;
		
		// 获取应用类型信息句柄
		HREFTYPE hRef;
		hr = spClassInfo->GetRefTypeOfImplType(i, &hRef);
		if (FAILED(hr))
			goto ONERROR;

		// 获取类型信息
		hr = spClassInfo->GetRefTypeInfo(hRef, &spTypeInfo);
		if (FAILED(hr))
			goto ONERROR;	

		spTypeInfo->GetTypeAttr(&pTypeAttr);

		// 查找Dispatch接口
		if (pTypeAttr->typekind == TKIND_INTERFACE)
		{
			//Get Dispatch Interface's Info;
			hr = spTypeInfo->GetRefTypeOfImplType(-1, &hRef);
			if (FAILED(hr))
				goto ONERROR;	

			CComPtr<ITypeInfo> spTypeInfoTmp;
			hr = spTypeInfo->GetRefTypeInfo(hRef, &spTypeInfoTmp);
			if (FAILED(hr))
				goto ONERROR;

			spTypeInfo->ReleaseTypeAttr(pTypeAttr);
			spTypeInfo = spTypeInfoTmp;
			spTypeInfo->GetTypeAttr(&pTypeAttr);
		}

		ASSERT(pTypeAttr->typekind == TKIND_DISPATCH);

		if (lpszEvent != NULL)
		{
			BSTR bstrNames = T2OLE(lpszEvent); 
			MEMBERID memberid;
			spTypeInfo->GetIDsOfNames(&bstrNames, 1, &memberid);

//			FUNCDESC* pfd;
//			spTypeInfo->GetFuncDesc(nFunction, &pfd);
//
//			BSTR* pNames = new BSTR[pfd->cParams + 1];//函数及参数的名字；
//			memset(pNames, 0, sizeof(LPBSTR) * (pfd->cParams + 1));
//			UINT nNames = 0;
//			pIInfo->GetNames(pFuncDesc->memid, pNames, pFuncDesc->cParams + 1, &nNames);
//
//			spTypeInfo->ReleaseFuncDesc(pfd);

			BSTR bstrParams[32];
			UINT numParams = 0;

			spTypeInfo->GetNames(memberid, bstrParams, 32, &numParams);
			for (UINT nParam = 1; nParam < numParams; nParam++)
			{
				arrInfos.Add(bstrParams[nParam]);
				SysFreeString(bstrParams[nParam]);
			}
		}
		else
		{
			//遍历Dispatch接口中的所有函数
			for (int nFunction = 0; nFunction < pTypeAttr->cFuncs; nFunction++)
			{
				FUNCDESC* pfd;
				spTypeInfo->GetFuncDesc(nFunction, &pfd);
				
				CComBSTR bstrName;
				CComBSTR bstrDocString;
				spTypeInfo->GetDocumentation(pfd->memid, &bstrName, &bstrDocString, NULL, NULL);
				CString strInfo;
				strInfo = bstrName;
				strInfo += _T(" ");
				strInfo += W2T(bstrDocString);
				arrInfos.Add(strInfo);

				// 确定缺省事件
				if (nDefaultIndex == -1 ||
					(pfd->memid < lDefaultMemberID && (pfd->memid >= 0 || lDefaultMemberID >= 3)))
				{
					nDefaultIndex = nFunction;
					lDefaultMemberID = pfd->memid;
				}
				
				spTypeInfo->ReleaseFuncDesc(pfd);
			}
		}
	
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);
		pTypeAttr = NULL;
		spTypeInfo.Release();
	}

ONERROR:
	if (spClassInfo != NULL && pClassTypeAttr != NULL)
		spClassInfo->ReleaseTypeAttr(pClassTypeAttr);
	if (spTypeInfo != NULL && pClassTypeAttr != NULL)
		spTypeInfo->ReleaseTypeAttr(pTypeAttr);

	if (nDefaultIndex > 0)
	{
		CString strDefaultEvent = arrInfos[nDefaultIndex];
		arrInfos.RemoveAt(nDefaultIndex);
		arrInfos.InsertAt(0, strDefaultEvent);
	}
}

void CFormDoc::GetObjectEvents(LPCTSTR lpszObject, CStringArray& arrEvents)
{
	CString strObject = lpszObject;
	LPUNKNOWN pUnkObject = NULL;

	if (strObject.CompareNoCase(GetClassName()) == 0)
	{
		pUnkObject = GetObjectUnknown();
	}
	else
	{
		CDevObj* pObj = NULL;
		if (!GetNamedObjectsMap()->LookupNoCase(strObject, pObj))
			return;

		pUnkObject = pObj->GetObjectUnknown();
	}

	if (pUnkObject ==  NULL)
		return;

	GetObjectEventOrParams(pUnkObject, arrEvents);
}

void CFormDoc::GetObjectEventParams(LPCTSTR lpszObject, LPCTSTR lpszEvent, CStringArray& arrParams)
{
	CString strObject = lpszObject;
	LPUNKNOWN pUnkObject = NULL;
	
	if (strObject.CompareNoCase(GetClassName()) == 0)
	{
		pUnkObject = GetObjectUnknown();
	}
	else
	{
		CDevObj* pObj = NULL;
		if (!GetNamedObjectsMap()->LookupNoCase(strObject, pObj))
			return;
		
		pUnkObject = pObj->GetObjectUnknown();
	}
	
	if (pUnkObject ==  NULL)
		return;
	
	GetObjectEventOrParams(pUnkObject, arrParams, lpszEvent);
}

// 由于模块上面的对象也是模块的成员
BOOL CFormDoc::GetFormDocObjectsMembers(CStringArray& arrWords, 
	CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods)
{
	int nWords = arrWords.GetSize();

	if (nWords == 0)
	{
		POSITION pos = GetObjects()->GetHeadPosition();
		while (pos != NULL)
		{
			CDevObj* pObj = GetObjects()->GetNext(pos);
			arrObjects.Add(pObj->GetDisplayName());
		}
		
		return TRUE;
	}
	else
	{
		CDevObj* pObj = NULL;
		if (nWords > 0 && GetNamedObjectsMap()->LookupNoCase(arrWords[0], pObj))
		{
			arrWords.RemoveAt(0);
			return GetDevObjMembers(pObj, arrObjects, arrWords, arrProperties, arrMethods);
		}
	
	}

	return FALSE;
}

BOOL CFormDoc::ShowHelpString(LPCTSTR lpszObject)
{
	CString strObject = lpszObject;

	if (strObject.CompareNoCase(GetClassName()) == 0)
	{
		return TRUE;
	}
	else
	{
		CDevObj* pObj = NULL;
		if (GetNamedObjectsMap()->LookupNoCase(strObject, pObj))
			return (!pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)));
	}

	return FALSE;
}

BOOL CFormDoc::GetDevObjMembers(CDevObj* pObj, CStringArray& arrWords, 
	CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;

	int nWords = arrWords.GetSize();
	CStringArray arrWordsBack;
	arrWordsBack.Append(arrWords);

	LPUNKNOWN pUnkObject = NULL;
	if (pObj == NULL)
		pUnkObject = GetObjectUnknown();
	else
		pUnkObject = pObj->GetObjectUnknown();

	CComQIPtr<IDispatch> spDisp(pUnkObject);
	CComPtr<ITypeInfo> spTypeInfo;	
	HRESULT hr = spDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);
	if (FAILED(hr)) 
		return FALSE;

	if (GetObjectMembers(spTypeInfo, arrWords, arrProperties, arrMethods) && nWords > 0)
		return TRUE;

	spTypeInfo.Release();

	if (pObj == NULL)
	{
		// 解决把Show方法显示成show的问题
		for (int i = 0; i < arrMethods.GetSize(); i++)
		{
			if (arrMethods[i] == _T("show"))
			{
				arrMethods[i] = _T("Show");
				break;
			}
		}
	}
		
	arrWords.RemoveAll();
	arrWords.Append(arrWordsBack);

	if (pObj == NULL)
	{
		if (GetFormDocObjectsMembers(arrWords, arrObjects, arrProperties, arrMethods) && nWords > 0)
			return TRUE;
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
	{
		hr = spDisp->GetTypeInfo(1, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);
		if (FAILED(hr)) 
			return FALSE;
		
		if (GetObjectMembers(spTypeInfo, arrWords, arrProperties, arrMethods) && nWords > 0)
			return TRUE;

		arrWords.RemoveAll();
		arrWords.Append(arrWordsBack);
	}
	else if (pObj->IsKindOf(RUNTIME_CLASS(CFormObj)))
	{
		CFormObj* pFormObj = (CFormObj *)pObj;
		if (pFormObj->m_pFormDoc->GetFormDocObjectsMembers(arrWords, arrObjects, arrProperties, arrMethods) && nWords > 0)
			return TRUE;
	}
	
	if (nWords == 0 && pObj != NULL)
	{
		//加入扩展属性
		ICxEnumExtendProperty* pEumProp = pObj->EnumExtendProperty();
		if (pEumProp)
		{
			LONG nIndex;
			pEumProp->Reset();
			while (S_FALSE != pEumProp->Next(&nIndex))
			{
				CComBSTR bstrName; 
				hr = pEumProp->GetName(&bstrName);
				if (FAILED(hr))
					break;
					
				arrProperties.Add(W2T(bstrName));
			}
			pEumProp->Release();
		}
	}

	return (nWords == 0);
}

BOOL CFormDoc::GetObjectMembers(CStringArray& arrWords, 
	CStringArray& arrObjects, CStringArray& arrProperties, CStringArray& arrMethods, BOOL bThisModule)
{
	HRESULT hr;
	int nWords = arrWords.GetSize();

	if (bThisModule)
	{
		if (nWords > 0 && (arrWords[0].CompareNoCase(_T("This")) == 0 || GetDisplayName().CompareNoCase(arrWords[0]) == 0))
		{
			arrWords.RemoveAt(0);
			nWords = arrWords.GetSize();
		}
		else
		{
			if (nWords > 0 && m_pDocument != NULL)
			{
				CDevDoc::CSubDoc* pSubDoc = m_pDocument->GetSubDocument(arrWords[0]);
				if (pSubDoc != NULL)
				{
					arrWords.RemoveAt(0);

					CDocument* pDoc = pSubDoc->LoadChildDocument(FALSE);
					if (pDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
						return ((CFormDoc *)pDoc)->GetObjectMembers(arrWords, arrObjects, arrProperties, arrMethods, false);
					
					ASSERT(FALSE);
					return FALSE;
				}
			
//				if (pSubDoc != NULL)
//				{
//					arrWords.RemoveAt(0);
//
//					CComPtr<ITypeInfo> spTypeInfo;
//					if (pSubDoc->GetType() == CDevDoc::enumFormDoc)
//					{
//						if (pSubDoc->IsFirstSubDoc())
//							hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(IUserForm), &spTypeInfo);
//						else
//							hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ISubUserForm), &spTypeInfo);
//					}
//					else if (pSubDoc->GetType() == CDevDoc::enumReportDoc)
//					{
//						hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(IDataReport), &spTypeInfo);
//					}
//					if (FAILED(hr))
//						return FALSE;
//
//					if (GetObjectMembers(spTypeInfo, arrWords, arrProperties, arrMethods))
//					{
//						// 解决把Show方法显示成show的问题
//						for (int i = 0; i < arrMethods.GetSize(); i++)
//						{
//							if (arrMethods[i] == _T("show"))
//							{
//								arrMethods[i] = _T("Show");
//								break;
//							}
//						}
//						return TRUE;
//					}
//
//					return FALSE;
//				}
			}
		}
	}

	// 窗口中的子对象能否解析？
	CDevObj* pObj = NULL;
	if (nWords > 0 && GetNamedObjectsMap()->LookupNoCase(arrWords[0], pObj))
	{
		arrWords.RemoveAt(0);
		return GetDevObjMembers(pObj, arrWords, arrObjects, arrProperties, arrMethods);
	}

	// 窗口对象能否解析？
	if (GetDevObjMembers(NULL, arrWords, arrObjects, arrProperties, arrMethods))
		return TRUE;
//	else if (arrWords[])

	CComPtr<ITypeInfo> spTypeInfo;
	hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ICxGlobalFunctions), &spTypeInfo);
	if (SUCCEEDED(hr))
	{
		if (GetObjectMembers(spTypeInfo, arrWords, arrProperties, arrMethods) && nWords > 0)
			return TRUE;
	}

	return FALSE;
}

BOOL CFormDoc::GetDevObjMemberParams(CDevObj* pObj, CStringArray& arrWords, CStringArray& arrParams)
{
	USES_CONVERSION;
	
	int nWords = arrWords.GetSize();
	
	LPUNKNOWN pUnkObject = NULL;
	if (pObj == NULL)
		pUnkObject = GetObjectUnknown();
	else
		pUnkObject = pObj->GetObjectUnknown();
		
	CComQIPtr<IDispatch> spDisp(pUnkObject);
	CComPtr<ITypeInfo> spTypeInfo;	

	HRESULT hr = spDisp->GetTypeInfo(0, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);
	if (FAILED(hr)) 
		return FALSE;
	
	if (GetObjectMemberParams(spTypeInfo, arrWords, arrParams))
		return TRUE;
	
	spTypeInfo.Release();
	
	if (pObj != NULL && pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
	{
		hr = spDisp->GetTypeInfo(1, LOCALE_SYSTEM_DEFAULT, &spTypeInfo);
		if (FAILED(hr)) 
			return FALSE;
		
		if (GetObjectMemberParams(spTypeInfo, arrWords, arrParams))
			return TRUE;
	}

	return FALSE;
}

BOOL CFormDoc::GetObjectMemberParams(CStringArray& arrWords, CStringArray& arrParams, BOOL bThisModule)
{
	int nWords = arrWords.GetSize();

	if (bThisModule)
	{
		if (nWords > 1 && (arrWords[0].CompareNoCase(_T("This")) == 0 || GetDisplayName().CompareNoCase(arrWords[0]) == 0))
		{
			arrWords.RemoveAt(0);
			nWords = arrWords.GetSize();
		}
		else
		{
			if (nWords > 1 && m_pDocument != NULL)
			{
				CDevDoc::CSubDoc* pSubDoc = m_pDocument->GetSubDocument(arrWords[0]);
				if (pSubDoc != NULL)
				{
					arrWords.RemoveAt(0);

					CDocument* pDoc = pSubDoc->LoadChildDocument(FALSE);
					if (pDoc->IsKindOf(RUNTIME_CLASS(CFormDoc)))
						return ((CFormDoc *)pDoc)->GetObjectMemberParams(arrWords, arrParams, false);
					
					ASSERT(FALSE);
					return FALSE;
				}

//				if (pSubDoc != NULL)
//				{
//					arrWords.RemoveAt(0);
//					
//					CComPtr<ITypeInfo> spTypeInfo;
//					if (pSubDoc->GetType() == CDevDoc::enumFormDoc)
//					{
//						if (pSubDoc->IsFirstSubDoc())
//							hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(IUserForm), &spTypeInfo);
//						else
//							hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(ISubUserForm), &spTypeInfo);
//					}
//					else if (pSubDoc->GetType() == CDevDoc::enumReportDoc)
//					{
//						hr = theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(IDataReport), &spTypeInfo);
//					}
//					if (FAILED(hr))
//						return FALSE;
//					
//					if (GetObjectMemberParams(spTypeInfo, arrWords, arrParams))
//						return TRUE;
//
//					return FALSE;
//				}
			}
		}
	}
	
	// 画面对象
	CDevObj* pObj = NULL;
	if (nWords > 0 && GetNamedObjectsMap()->LookupNoCase(arrWords[0], pObj))
	{
		arrWords.RemoveAt(0);
		return GetDevObjMemberParams(pObj, arrWords, arrParams);
	}

//	if (nWords == 1)
//	{
		// 窗体函数
		if (GetDevObjMemberParams(NULL, arrWords, arrParams))
			return TRUE;
//	}

	return FALSE;
}

//Helper for finding the function index for a DISPID
HRESULT GetFuncIndexOfMemId(ITypeInfo* pTypeInfo, DISPID dispidMember, UINT& nIndex)
{
	HRESULT hr;
	//	CComQIPtr<ITypeInfo2, &IID_ITypeInfo2> spTypeInfo2 = pTypeInfo;
	if (0/*spTypeInfo2 != NULL*/)
	{
		//		hr = spTypeInfo2->GetFuncIndexOfMemId(dispidMember, INVOKE_FUNC, &nIndex);
		//		if (FAILED(hr))
		//			return hr;
		
	}
	else // search for funcdesc
	{
		TYPEATTR* pAttr;
		hr = pTypeInfo->GetTypeAttr(&pAttr);
		if (FAILED(hr))
			return hr;
		FUNCDESC* pFuncDesc = NULL;
		for (int i = 0; i < pAttr->cFuncs; i++)
		{
			hr = pTypeInfo->GetFuncDesc(i, &pFuncDesc);
			if (FAILED(hr))
				return hr;
			if (pFuncDesc->memid == dispidMember)
				break;
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
			pFuncDesc = NULL;
		}
		pTypeInfo->ReleaseTypeAttr(pAttr);
		if (pFuncDesc == NULL)
		{
			return E_FAIL;
		}
		else
		{
			nIndex = i;
			pTypeInfo->ReleaseFuncDesc(pFuncDesc);
		}
	}
	
	return S_OK;
}

BOOL CFormDoc::GetObjectMembers(ITypeInfo* pTypeInfo, CStringArray& arrWords, 
		CStringArray& arrProperties, CStringArray& arrMethods)
{
	USES_CONVERSION;

	BOOL bResult = FALSE;
	int nWords = arrWords.GetSize();

	if (nWords == 0)
	{
		TYPEATTR* pTypeAttr = NULL;
		HRESULT hr = pTypeInfo->GetTypeAttr(&pTypeAttr);
		if (FAILED(hr))
			return FALSE;

		for (int i = 0; i < pTypeAttr->cFuncs; i++)
		{
			FUNCDESC* pfd = 0;
			pTypeInfo->GetFuncDesc(i, &pfd);

			if ((pfd->wFuncFlags & (FUNCFLAG_FRESTRICTED | 
				FUNCFLAG_FHIDDEN/* | FUNCFLAG_FNONBROWSABLE*/)) == 0)
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);
				CString str = OLE2T(bstrName);
				if (pfd->invkind == INVOKE_FUNC)
				{
					arrMethods.Add(OLE2T(bstrName));
					bResult = TRUE;
				}
				else if (pfd->invkind == INVOKE_PROPERTYGET)
				{
					arrProperties.Add(str);
					bResult = TRUE;
				}
			}

			pTypeInfo->ReleaseFuncDesc(pfd);
		}
	

		for (i = 0; i < pTypeAttr->cVars; i++)
		{
			VARDESC* pvd;
			pTypeInfo->GetVarDesc(i, &pvd);
			if (pvd->varkind == VAR_DISPATCH && (pvd->wVarFlags & (VARFLAG_FHIDDEN | 
				VARFLAG_FRESTRICTED/* | VARFLAG_FNONBROWSABLE*/)) == 0   )
			{
				CComBSTR bstrName;
				pTypeInfo->GetDocumentation(pvd->memid, &bstrName, NULL, NULL, NULL);
				CString str = OLE2T(bstrName);
				arrProperties.Add(str);
				bResult = TRUE;
			}
			pTypeInfo->ReleaseVarDesc(pvd);
		}

		pTypeInfo->ReleaseTypeAttr(pTypeAttr);
	}
	else
	{
		BSTR bstrNames = T2OLE(arrWords[0]); 
		DISPID dispidMember;
		if (FAILED(pTypeInfo->GetIDsOfNames(&bstrNames, 1, &dispidMember)))
			return FALSE;

		arrWords.RemoveAt(0);

		UINT nIndex;
		HRESULT hr = GetFuncIndexOfMemId(pTypeInfo, dispidMember, nIndex);
		if (FAILED(hr))
			return FALSE;

		FUNCDESC* pfd = NULL;
		hr = pTypeInfo->GetFuncDesc(nIndex, &pfd);
		if (FAILED(hr))
			return FALSE;

//		CComBSTR bstrName;
//		pTypeInfo->GetDocumentation(pfd->memid, &bstrName, NULL, NULL, NULL);

		TYPEDESC* ptdesc = NULL;
		if (pfd->invkind != INVOKE_FUNC && pfd->cParams > 0)
			ptdesc = &pfd->lprgelemdescParam[0].tdesc;
		else 
			ptdesc = &pfd->elemdescFunc.tdesc;
		if (ptdesc->vt == VT_PTR)
		{
			ptdesc = ptdesc->lptdesc;
		}
		if (ptdesc->vt == VT_USERDEFINED)
		{
			CComPtr<ITypeInfo> spUserDefinedTypeInfo;
			hr = pTypeInfo->GetRefTypeInfo(ptdesc->hreftype, &spUserDefinedTypeInfo);
			if (SUCCEEDED(hr))
			{
				TYPEATTR *pta = NULL;
				spUserDefinedTypeInfo->GetTypeAttr(&pta);
				if (pta && pta->typekind == TKIND_ALIAS)
				{
					if (pta->tdescAlias.vt == VT_USERDEFINED)
					{
						CComPtr<ITypeInfo> spAliasTypeInfo;
						hr = spUserDefinedTypeInfo->GetRefTypeInfo(pta->tdescAlias.hreftype, &spAliasTypeInfo);
						if (SUCCEEDED(hr))
							bResult = GetObjectMembers(spAliasTypeInfo, arrWords, arrProperties, arrMethods);
					}
				}
				else
				{
					
					bResult = GetObjectMembers(spUserDefinedTypeInfo, arrWords, arrProperties, arrMethods);
				}
				
				if (pta)
					spUserDefinedTypeInfo->ReleaseTypeAttr(pta);
			}
		}
	
		if (pfd)
			pTypeInfo->ReleaseFuncDesc(pfd);
	}

	return bResult;
}

BOOL CFormDoc::GetObjectMemberParams(ITypeInfo* pTypeInfo, CStringArray& arrWords, CStringArray& arrParams)
{
	USES_CONVERSION;
	
	BOOL bResult = FALSE;
	int nWords = arrWords.GetSize();
	
	if (nWords > 0)
	{
		BSTR bstrNames = T2OLE(arrWords[0]); 
		DISPID dispidMember;
		if (FAILED(pTypeInfo->GetIDsOfNames(&bstrNames, 1, &dispidMember)))
			return FALSE;
		
		UINT nIndex;
		HRESULT hr = GetFuncIndexOfMemId(pTypeInfo, dispidMember, nIndex);
		if (FAILED(hr))
			return FALSE;
		
		FUNCDESC* pfd = NULL;
		hr = pTypeInfo->GetFuncDesc(nIndex, &pfd);
		if (FAILED(hr))
			return FALSE;
		
		if (nWords == 1)
		{
			if (pfd->invkind == INVOKE_FUNC)
			{
				BSTR bstrParams[32];
				UINT numParams = 0;
				
				pTypeInfo->GetNames(dispidMember, bstrParams, 32, &numParams);
				for (UINT nParam = 1; nParam < numParams; nParam++)
				{
					CString str = bstrParams[nParam];
					if ((int)nParam > pfd->cParams - pfd->cParamsOpt)
						str = "[" + str + "]";
					arrParams.Add(str);
					SysFreeString(bstrParams[nParam]);
					
					//					arrParams.Add(bstrParams[nParam]);
					//					SysFreeString(bstrParams[nParam]);
					
				}
				
				bResult = TRUE;
			}
		}
		else
		{
			arrWords.RemoveAt(0);
			
			TYPEDESC* ptdesc = &pfd->elemdescFunc.tdesc;
			if (ptdesc->vt == VT_PTR)
			{
				ptdesc = ptdesc->lptdesc;
			}
			if (ptdesc->vt == VT_USERDEFINED)
			{
				CComPtr<ITypeInfo> spUserDefinedTypeInfo;
				hr = pTypeInfo->GetRefTypeInfo(ptdesc->hreftype, &spUserDefinedTypeInfo);
				if (SUCCEEDED(hr))
				{
					TYPEATTR *pta = NULL;
					spUserDefinedTypeInfo->GetTypeAttr(&pta);
					if (pta && pta->typekind == TKIND_ALIAS)
					{
						if (pta->tdescAlias.vt == VT_USERDEFINED)
						{
							CComPtr<ITypeInfo> spAliasTypeInfo;
							hr = spUserDefinedTypeInfo->GetRefTypeInfo(pta->tdescAlias.hreftype, &spAliasTypeInfo);
							if (SUCCEEDED(hr))
								bResult = GetObjectMemberParams(spAliasTypeInfo, arrWords, arrParams);
						}
					}
					else
					{
						
						bResult = GetObjectMemberParams(spUserDefinedTypeInfo, arrWords, arrParams);
					}
					
					if (pta)
						spUserDefinedTypeInfo->ReleaseTypeAttr(pta);
				}	
			}
		}
		
		if (pfd)
			pTypeInfo->ReleaseFuncDesc(pfd);
	}
	
	return bResult;
}

void CFormDoc::OnSymbolChanged(DWORD dwUpdateMask)
{
	if (m_dwSymbolUpdateMask == 0)
	{
		CScriptView* pView = GetScriptView();
		if (pView != NULL)
		{
			pView->PostMessage(WM_UPDATE_SYMBOL, 0, 0L);
			m_dwSymbolUpdateMask |= dwUpdateMask;
		}
	}
}