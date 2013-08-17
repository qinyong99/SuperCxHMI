/////////////////////////////////////////////////////////////////////
// FormObj.h: interface for the CFormObj class.
//功能:CFormObj 内嵌窗体类
//制作:孙越洋
//时间:2001.12.5
//所属工程:CxDev
//说明:内嵌一个窗体对象到窗体中,成为一个模块的类
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "FormObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CFormObj, CDevObj, 0)

const IID IID_IDrawVariable = {0x07C9658C,0xB87E,0x4E62,{0x84,0x60,0xB1,0x70,0x0B,0xA3,0x35,0x66}};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//功能:构造函数
//参数:pParentDev[in]		所属的DevDoc对象(该参数用于保存对象时使用)
//							如该参数为NULL则表示保在时用其它一般方式
/////////////////////////////////////////////////////////////////////
CFormObj::CFormObj(CDevObjDoc* pContainerDoc) :
	CDevObj(pContainerDoc),
	m_pFormDoc(NULL),
	m_fHorScale(1),							//水平缩放因子
	m_fVerScale(1),
	m_fRotateAngle(0)
{
	EnableAutomation();
	m_strClassName = "Module";
	m_enumScaleBase = ScaleBasePointCenter;
	m_bUniformScale = TRUE;
	m_bTransparency = TRUE;

	CFormDoc* pFormDoc = GetFormDoc();
	if (pFormDoc != NULL)
		pFormDoc->m_listSubformModule.AddTail(this);

	AfxOleLockApp();
}

CFormObj::~CFormObj()
{
	AfxOleUnlockApp();
}

BOOL CFormObj::CreateFormDoc()
{
	m_pFormDoc = new CFormDoc();
	m_pFormDoc->m_pFormObj = this;
	
	if (!m_pFormDoc->OnNewDocument())
	{
		delete m_pFormDoc;
		m_pFormDoc = NULL;
		return FALSE;
	}

	m_pFormDoc->OnUpateTitle();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
//功 能：创建或导入对象]
/////////////////////////////////////////////////////////////////////////
BOOL CFormObj::CreateOrLoad(LPCTSTR pszName,
   REFIID iidPersistanceMedium, IUnknown* pPersistanceMedium, RECTF* pRect)
{
	m_pDocument->CreateUniqueObjectName(this);
	if (pRect != NULL)
	{
		m_position = *pRect;
	}
	else
	{
		m_position.left = 10;
		m_position.top = 10;
		m_position.right = 200;
		m_position.bottom = 200;	
	}

	if (!CreateFormDoc())
		return FALSE;
	
	m_dwObjectNumber = GetNewObjectNumber();
	
	m_pFormDoc->SetDocSize(0, 0, TRUE);

	m_ptRotateBase = m_position.CenterPoint();

	return TRUE;
}

void CFormObj::AddSubObject(CDevObj* pObj)
{
	m_pFormDoc->AddObject(pObj);

	// 2004.8.10 add flow 2 lines
	CNamedDevobjMap* pMap = m_pFormDoc->GetNamedObjectsMap();
	pMap->SetAt(pObj->GetDisplayName(), pObj);
	m_pFormDoc->OnSymbolChanged(UPDATE_SYMBOL_OBJECT);

	RecalcPosition();
	m_ptRotateBase = m_position.CenterPoint();
}

void CFormObj::RemoveObject(CDevObj* pObj)
{
	// 2004.8.10 add flow 2 lines
	CNamedDevobjMap* pMap = m_pFormDoc->GetNamedObjectsMap();
	pMap->RemoveKey(pObj->GetDisplayName());
	m_pFormDoc->OnSymbolChanged(UPDATE_SYMBOL_OBJECT);
	
	m_pFormDoc->RemoveObject(pObj);
}

void CFormObj::CopyToClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szObjectName[64];
	IStoragePtr pSubStorage;

	ar << m_position;
	ar << m_ptRotateBase;
	ar << m_enumScaleBase;
	ar << m_bUniformScale;
	ar << m_fHorScale;							
	ar << m_fVerScale;							
	ar << m_fRotateAngle; 
	ar << m_bTransparency;
	
	ar << m_dwObjectNumber;

	GetObjectName(szObjectName);

	//创建子存储
	hResult = pStorage->CreateStorage(T2COLE(szObjectName), STGM_READWRITE|
		STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
	}
	
	m_pFormDoc->WriteToStorage(pSubStorage);
	
	//拷贝动态属性
	SerializeDynamicProperty(ar);

	CDevObj::CopyToClipboard(ar, pStorage);
}

BOOL CFormObj::CreateFromClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szObjectName[64];
	IStoragePtr pSubStorage;

	m_dwObjectNumber = GetNewObjectNumber();

	ar >> m_position;
	ar >> m_ptRotateBase;
	long lTemp;
	ar >> lTemp;
	m_enumScaleBase = enumScaleBasePoint(lTemp);
	ar >> m_bUniformScale;
	ar >> m_fHorScale;							
	ar >> m_fVerScale;							
	ar >> m_fRotateAngle;
	ar >> m_bTransparency;
	
	DWORD dwObjectNumber = m_dwObjectNumber;
	ar >> m_dwObjectNumber;

	GetObjectName(szObjectName);
	m_dwObjectNumber = dwObjectNumber;

	//打开子存储
	hResult = pStorage->OpenStorage(T2COLE(szObjectName), NULL, STGM_READ|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		ASSERT(false);
		AfxThrowOleException(hResult);
	}
	
	//注意CFormObj与CFormDoc的对等关系
	if (!CreateFormDoc())
		return FALSE;

	m_pFormDoc->ReadFromStorage(pSubStorage);
	
	//创建动态属性
	SerializeDynamicProperty(ar);

	return CDevObj::CreateFromClipboard(ar, pStorage);
}

BOOL CFormObj::CreateFromSubDocumentClipboard(CArchive& ar, IStorage* pStorage)
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szDocumentName[64];
	IStoragePtr pSubStorage;

	ASSERT_VALID(this);
	ASSERT(ar.IsLoading());
	
	m_dwObjectNumber = GetNewObjectNumber();

	CString strName;
	ar >> strName;

	//用于确定所创建的子存储
	DWORD dwDocumentNumber;
	ar >> dwDocumentNumber;
	wsprintf(szDocumentName, _T("SubDoc%lu"), dwDocumentNumber);

	//打开子存储
	hResult = pStorage->OpenStorage(T2COLE(szDocumentName), NULL, STGM_READWRITE|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
	if (FAILED(hResult))
	{
		AfxThrowOleException(hResult);
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

		//注意CFormObj与CFormDoc的对等关系
		if (!CreateFormDoc())
			return FALSE;
	
		VERIFY(m_pFormDoc->CreateFromClipboard(loadArchive, pSubStorage) == TRUE);

		loadArchive.Flush();
		file.Close();
	}
	else
	{
		//注意CFormObj与CFormDoc的对等关系
		if (!CreateFormDoc())
			return FALSE;
		
		m_pFormDoc->ReadFromStorage(pSubStorage);
	}

	// 2004.8.10 remove flow 2 lines
//	CRectF position = m_pFormDoc->m_position;
//	MoveTo(position, FALSE);

	// 2004.8.10 add flow 1 lines
	m_position = m_pFormDoc->m_position;

	m_ptRotateBase = m_position.CenterPoint();

	if (!PutDisplayName(strName))
	{
		CString strBaseName = strName;
		RemoveRightDigits(strBaseName);

		m_pDocument->CreateUniqueObjectName(this, (IsBaseObject() || strBaseName.IsEmpty()) ? LPCTSTR(NULL) : strBaseName); // 使用源对象名字作为基本名
	}
	
	return TRUE;
}

void CFormObj::Serialize(CArchive &ar)
{
	USES_CONVERSION;
	HRESULT hResult;
	TCHAR szObjectName[64];
	IStoragePtr pSubStorage;

	BOOL bAddObject = (m_pDocument == NULL);

	CDevObj::Serialize(ar);

	if (bAddObject && m_pDocument != NULL)
		GetFormDoc()->m_listSubformModule.AddTail(this);

	GetObjectName(szObjectName);

	if (ar.IsStoring())
	{
		ar << m_position;
		ar << m_ptRotateBase;
		ar << m_enumScaleBase;
		ar << m_bUniformScale;
		ar << m_fHorScale;							
		ar << m_fVerScale;							
		ar << m_fRotateAngle;
		ar << m_bTransparency;
		
		//创建子存储
		hResult = ((CFormDoc *)m_pDocument)->m_lpRootStg->CreateStorage(T2COLE(szObjectName), STGM_READWRITE|
			STGM_CREATE|STGM_SHARE_EXCLUSIVE, 0, 0, &pSubStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}
	
		m_pFormDoc->WriteToStorage(pSubStorage);

		CFormDoc* pFormDoc = GetFormDoc();
		if (pFormDoc != NULL)
			pFormDoc->m_pFormSubDoc->m_nSubformModule++;
	}
	else
	{
		ar >> m_position;
		ar >> m_ptRotateBase;
		long lTemp;
		ar >> lTemp;
		m_enumScaleBase = enumScaleBasePoint(lTemp);
		ar >> m_bUniformScale;
		ar >> m_fHorScale;							
		ar >> m_fVerScale;							
		ar >> m_fRotateAngle; 
		ar >> m_bTransparency;
		
		hResult = ((CFormDoc *)m_pDocument)->m_lpRootStg->OpenStorage(T2COLE(szObjectName), NULL, STGM_READWRITE|
			STGM_SHARE_EXCLUSIVE, NULL, 0, &pSubStorage);
		if (FAILED(hResult))
		{
			AfxThrowOleException(hResult);
		}

		//注意CFormObj与CFormDoc的对等关系
		VERIFY(CreateFormDoc() == TRUE);
		m_pFormDoc->ReadFromStorage(pSubStorage);
	}

	//序列化动态属性
	SerializeDynamicProperty(ar);
}

void CFormObj::Draw(CDC* pDC)
{
	if (!IsDrawVisible())
		return;

	ASSERT(m_pFormDoc != NULL);

	if (!m_bTransparency)
	{
		CRect client = m_position.GetRECT();
		if (m_pFormDoc->m_pImageBackGround != NULL)
		{	
			using namespace Gdiplus;

			TextureBrush tBrush(m_pFormDoc->m_pImageBackGround);
			Graphics graphics(pDC->GetSafeHdc());
			graphics.FillRectangle(&tBrush, 
					Rect(client.left, client.top, client.Width(), client.Height()));
		}
		else
		{
			// paint background
			COLORREF clrBackColor;
			OleTranslateColor(m_pFormDoc->m_clrBackColor, 0, &clrBackColor);
			CBrush backBrush(clrBackColor);
			pDC->FillRect(client, &backBrush);
		}
	}

	m_pFormDoc->DrawDoc(pDC);
}

void CFormObj::Print(CDC* pDC)
{
	ASSERT(m_pFormDoc != NULL);
	
	CRect client = m_position.GetRECT();
	if (m_pFormDoc->m_pImageBackGround != NULL)
	{	
		using namespace Gdiplus;
		
		TextureBrush tBrush(m_pFormDoc->m_pImageBackGround);
		Graphics graphics(pDC->GetSafeHdc());
		graphics.SetPageUnit(UnitPixel);
		graphics.FillRectangle(&tBrush, 
			Rect(client.left, client.top, client.Width(), client.Height()));
	}
	else
	{
		// paint background
		COLORREF clrBackColor;
		OleTranslateColor(m_pFormDoc->m_clrBackColor, 0, &clrBackColor);
		CBrush backBrush(clrBackColor);
		pDC->FillRect(client, &backBrush);
	}
	
	m_pFormDoc->PrintDoc(pDC);
}

LPUNKNOWN CFormObj::GetObjectUnknown()
{
	return GetInterface(&IID_IUnknown);

//	ASSERT(m_pFormDoc != NULL);

//	return m_pFormDoc->GetInterface(&IID_IUnknown);
}

void CFormObj::Release()
{
	ASSERT(m_pFormDoc != NULL);

	if (m_pFormDoc != NULL)
	{
		m_pFormDoc->m_pFormObj = NULL;
		m_pFormDoc->OnCloseDocument();
		m_pFormDoc = NULL;
	}

	CDevObj::Release();
}

void CFormObj::Delete(BOOL bAutoDelete)
{
	ASSERT(m_pFormDoc != NULL);

	CFormDoc* pFormDoc = GetFormDoc();
	if (pFormDoc != NULL)
		pFormDoc->m_listSubformModule.RemoveAt(pFormDoc->m_listSubformModule.Find(this));

	CDevObj::Delete(bAutoDelete);
}

void CFormObj::RecalcPosition()
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		CRectF rcObj = pObj->GetPositionRect();
		if (rcObj.left < m_position.left)
			m_position.left = rcObj.left;
		if (rcObj.top < m_position.top)
			m_position.top = rcObj.top;
		if (rcObj.right > m_position.right)
			m_position.right = rcObj.right;
		if (rcObj.bottom > m_position.bottom)
			m_position.bottom = rcObj.bottom;
	}
}

BOOL CFormObj::HitTest(CPoint point, CLayoutView* pView)
{
	return CDevObj::HitTest(point, pView);
//	if (m_bVisible)
//	{
//		int nCount = m_arrSubObj.GetSize();
//		for (int i = 0; i < nCount; i++)
//		{
//			if (m_arrSubObj[i]->HitTest(point, pView))
//				return TRUE;
//		}
//	}
//
//	return FALSE;
}

CRect CFormObj::GetBoundingRect()
{
	CDevObj* pObj;
	CRect rcObj;
	long lLeft;
	long lTop;
	long lRight;
	long lBottom;

	rcObj = m_position.GetRECT();
	lLeft = rcObj.left;
	lTop = rcObj.top;
	lRight = rcObj.right;
	lBottom = rcObj.bottom;

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		rcObj = pObj->GetBoundingRect();
		if (lLeft > rcObj.left)
			lLeft = rcObj.left;
		if (lTop > rcObj.top)
			lTop = rcObj.top;
		if (lRight < rcObj.right)
			lRight = rcObj.right;
		if (lBottom < rcObj.bottom)
			lBottom = rcObj.bottom;
	}
	return CRect(lLeft, lTop, lRight, lBottom);
}

//////////////////////////////////////////////////////////////////
//功能: 移动对象
//////////////////////////////////////////////////////////////////
void CFormObj::MoveTo(const CRectF &position, BOOL bUniformScale, CLayoutView* pView)
{
	ASSERT_VALID(this);

	if (m_bUniformScale)
		bUniformScale = TRUE;

	CRectF rcObj;
	Invalidate(pView);

	float fScaleX = position.Width() / m_position.Width();
	float fScaleY = position.Height() / m_position.Height();

	if (bUniformScale)
	{
		if (ABSOLUTEVALUE(fScaleX) < ABSOLUTEVALUE(fScaleY))
			fScaleX = fScaleX >= 0 ? ABSOLUTEVALUE(fScaleY) : -ABSOLUTEVALUE(fScaleY);
		else
			fScaleY = fScaleY >= 0 ? ABSOLUTEVALUE(fScaleX) : -ABSOLUTEVALUE(fScaleX);
	}

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		rcObj = pObj->GetPositionRect();
		rcObj.OffsetRect(-m_position.TopLeft());
		rcObj.left = rcObj.left * fScaleX;
		rcObj.right = rcObj.right * fScaleX;
		rcObj.top = rcObj.top * fScaleY;
		rcObj.bottom = rcObj.bottom * fScaleY;
		rcObj.OffsetRect(position.TopLeft());
		pObj->MoveTo(rcObj, bUniformScale, pView);
	}
	
	m_ptRotateBase.x = position.left + (m_ptRotateBase.x - m_position.left) * fScaleX;
	m_ptRotateBase.y = position.top + (m_ptRotateBase.y - m_position.top) * fScaleY;

	m_position = position;
//	m_position.NormalizeRect();
	RecalcPosition();
	m_pFormDoc->SetDocSize(0, 0, TRUE);
		
	Invalidate(pView);
}

void CFormObj::DrawRotateAtShape(CDC* pDC, CPointF point, long lAngle)
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->DrawRotateAtShape(pDC, point, lAngle);
	}
}

void CFormObj::Rotate(int lAngle, CLayoutView* pView)
{
	RotateAt(m_ptRotateBase, (float)lAngle, pView);
}

void CFormObj::RotateAt(CPointF point, float fAngle, CLayoutView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->RotateAt(point, fAngle);
	}

	m_ptRotateBase = CalculateRotatePoint(point, m_ptRotateBase, fAngle);

	RecalcPosition();
	Invalidate(pView);
}

void CFormObj::ScaleAt(CPointF point, float fHorScale, float fVerScale, CLayoutView* pView)
{
	ASSERT_VALID(this);

	Invalidate(pView);

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		pObj->ScaleAt(point, fHorScale, fVerScale);
	}

	m_ptRotateBase -= point;
	m_ptRotateBase.x *= fHorScale; 
	m_ptRotateBase.y *= fVerScale; 
	m_ptRotateBase += point;

	RecalcPosition();
	Invalidate(pView);
}

void CFormObj::MoveHandleTo(int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	switch (state)
	{
	case general:
	case resize:
		CDevObj::MoveHandleTo(nHandle, state, point, pView);
		break;
	case rotate:
		{
			if (nHandle == 1)
			{
				CDevObj::MoveHandleTo(nHandle, state, point, pView);
				return;
			}
			CPoint ptBase = GetHandle(1, state, pView);
			CPoint ptOrign = GetHandle(nHandle, state, pView);

			CDevObjList* pObjects = m_pFormDoc->GetObjects();
			POSITION posObj = pObjects->GetHeadPosition();
			CDevObj* pObj;
			while (posObj != NULL)
			{
				pObj = pObjects->GetNext(posObj);
				pObj->RotateAt(m_ptRotateBase, float(CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point)), pView);
			}

			RecalcPosition();
		}
		break;
	default:
		ASSERT(FALSE);
	}

}

void CFormObj::DrawTrackerHandleShape(CDC* pDC, int nHandle, TrackerState state, CPoint point, CLayoutView* pView)
{
	switch (state)
	{
	case general:
	case resize:
		CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
		break;
	case rotate:
		{
			if (nHandle == 1)
			{
				CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
				return;
			}
			CPoint ptBase = GetHandle(1, state, pView);
			CPoint ptOrign = GetHandle(nHandle, state, pView);

			CDevObjList* pObjects = m_pFormDoc->GetObjects();
			POSITION posObj = pObjects->GetHeadPosition();
			CDevObj* pObj;
			while (posObj != NULL)
			{
				pObj = pObjects->GetNext(posObj);
				pObj->DrawRotateAtShape(pDC, m_ptRotateBase, CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
			}
		
//			m_pDrawObject->DrawRotateShape(pDC->m_hDC, CalculateAngle(ptBase, ptOrign) - CalculateAngle(ptBase, point));
			CDevObj::DrawTrackerHandleShape(pDC, nHandle, state, point, pView);
		}
		break;
	default:
		ASSERT(FALSE);
	}
}

CPoint CFormObj::GetRotateBasePoint()
{
	return CPoint(int(m_ptRotateBase.x), int(m_ptRotateBase.y));
}

void CFormObj::SetRotateBasePoint(CPoint point)
{
	m_ptRotateBase.x = (float)point.x;
	m_ptRotateBase.y = (float)point.y;
}

void CFormObj::OnEdit()
{
	POSITION pos = m_pFormDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CView* pView = m_pFormDoc->GetNextView(pos);
		if (pView != NULL)
		{
			CMDIChildWnd* pFrame = (CMDIChildWnd *)pView->GetParentFrame();
			CMDIChildWnd* pActiveFrame = ((CMainFrame *)AfxGetMainWnd())->MDIGetActive();
			if (pFrame != NULL && pFrame != pActiveFrame)
				((CMainFrame *)AfxGetMainWnd())->MDIActivate(pFrame);
			
			return;
		}
	}

	CMDIChildWnd* pNewFrame
			= (CMDIChildWnd*)(GetLayoutTemplate()->CreateNewFrame(m_pFormDoc, NULL));
	ASSERT(pNewFrame != NULL);
	ASSERT_KINDOF(CMDIChildWnd, pNewFrame);
	pNewFrame->InitialUpdateFrame(m_pFormDoc, TRUE);
}

CDevDoc* CFormObj::GetDevDoc()
{
	if (!m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)))
		return NULL;

	CFormDoc* pTopDoc = (CFormDoc *)m_pDocument;
	while (pTopDoc->m_pFormObj != NULL)
		pTopDoc = (CFormDoc *)pTopDoc->m_pFormObj->m_pDocument;

	return pTopDoc->m_pDocument;
}

CFormDoc* CFormObj::GetFormDoc()
{
	if (m_pDocument != NULL && m_pDocument->IsKindOf(RUNTIME_CLASS(CFormDoc)))
	{	
		CFormDoc* pTopDoc = (CFormDoc *)m_pDocument;
		while (pTopDoc->m_pFormObj != NULL)
			pTopDoc = (CFormDoc *)pTopDoc->m_pFormObj->m_pDocument;

		return pTopDoc;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////
//功 能：当最后引用被释放，删除对象
//描 述：与CCmdTarget::OnFinalRelease()实现不一样，如调用CCmdTarget::OnFinalRelease()会出错
/////////////////////////////////////////////////////////////////////////
void CFormObj::OnFinalRelease()
{
 	// When the last reference for an automation object is released
 	//  OnFinalRelease is called.  This implementation deletes the
 	//  object.  Add additional cleanup required for your object before
 	//  deleting it from memory.
 
 	delete this;
}

BEGIN_DISPATCH_MAP(CFormObj, CDevObj)
	//{{AFX_DISPATCH_MAP(CFormObj)
	DISP_PROPERTY_EX_ID(CFormObj, "ForeColor", CXPROPID_ForeColor, GetForeColor, SetForeColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CFormObj, "BackColor", CXPROPID_BackColor, GetBackColor, SetBackColor, VT_COLOR)
	DISP_PROPERTY_EX_ID(CFormObj, "BrushImageFile", CXPROPID_BrushImageFileName, GetBrushImageFile, SetBrushImageFile, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Transparency", CXPROPID_Transparency, m_bTransparency, OnTransparencyChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFormObj, "VisibleLayer", CXPROPID_VisibleLayer, GetVisibleLayer, SetVisibleLayer, VT_I4)
	
	DISP_PROPERTY_EX_ID(CFormObj, "HorizontalPosition", CXPROPID_HorPosition, GetHorizontalPosition, SetHorizontalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "VerticalPosition", CXPROPID_VerPosition, GetVerticalPosition, SetVerticalPosition, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "Width", CXPROPID_Width, GetWidth, SetWidth, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "Height", CXPROPID_Height, GetHeight, SetHeight, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RoateBasePointX", CXPROPID_RoateBasePointX, GetRoateBasePointX, SetRoateBasePointX, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RoateBasePointY", CXPROPID_RoateBasePointY, GetRoateBasePointY, SetRoateBasePointY, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "RotateAngle", CXPROPID_RotateAngle, GetRotateAngle, SetRotateAngle, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "ScaleBasePoint", CXPROPID_ScaleBasePoint, GetScaleBasePoint, SetScaleBasePoint, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "HorizontalScale", CXPROPID_HorScale, GetHorizontalScale, SetHorizontalScale, VT_R4)
	DISP_PROPERTY_EX_ID(CFormObj, "VerticalScale", CXPROPID_VerScale, GetVerticalScale, SetVerticalScale, VT_R4)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "UniformScale", CXPROPID_UniformScale, m_bUniformScale, OnUniformScaleChanged, VT_BOOL)
	DISP_PROPERTY_EX_ID(CFormObj, "Layer", CXPROPID_Layer, GetLayer, SetLayer, VT_I4)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Locked", CXPROPID_Locked, m_bLocked, OnLockedChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Visible", CXPROPID_Visible, m_bVisible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "ToolTipText", CXPROPID_ToolTipText, m_strToolTip, OnToolTipTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY_ID(CFormObj, "Description", CXPROPID_Description, m_strDescription, OnDescriptionChanged, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "MousePointer", CXPROPID_MousePointer, GetMousePointer, SetMousePointer, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "Privilege", CXPROPID_Privilege, GetPrivilege, SetPrivilege, VT_I4)
	DISP_PROPERTY_EX_ID(CFormObj, "TagGroup", CXPROPID_TagGroup, GetTagGroup, SetTagGroup, VT_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "AssocTagGroup", CXPROPID_AssocTagGroup, GetAssocTagGroup, SetNotSupported, VT_BSTR)

	DISP_FUNCTION_ID(CFormObj, "GetObjectByName", CXMETHID_GetObjectByName, GetObjectByName, VT_UNKNOWN, VTS_BSTR)
	DISP_PROPERTY_EX_ID(CFormObj, "Objects", CXPROPID_Objects, GetObjectCollection, SetNotSupported, VT_DISPATCH)

	DISP_PROPERTY_EX_ID(CFormObj, "Name", CXPROPID_Name, GetName, SetName, VT_BSTR)
	DISP_FUNCTION_ID(CFormObj, "IsConnected", CXMETHID_IsConnected, IsConnected, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "ConnectObject", CXMETHID_ConnectObject, ConnectObject, VT_EMPTY, VTS_BSTR VTS_UNKNOWN)
	DISP_FUNCTION_ID(CFormObj, "ConnectDirect", CXMETHID_ConnectDirect, ConnectDirect, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "GetConnectObject", CXMETHID_GetConnectObject, GetConnectObject, VT_UNKNOWN, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "Disconnect", CXMETHID_Disconnect, Disconnect, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CFormObj, "GetConnectState", CXMETHID_GetConnectState, GetConnectState, VT_I4, VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

const extern IID LIBID_CXHMIOBJLib;
IMPLEMENT_OLETYPELIB(CFormObj, LIBID_CXHMIOBJLib, 1, 0)

const IID IID_IFormObject = 
{0x26F09B53,0x43A9,0x445c,{0xBC,0x7A,0xA5,0xCE,0xA0,0x22,0xE3,0x12}};

BEGIN_INTERFACE_MAP(CFormObj, CDevObj)
	INTERFACE_PART(CFormObj, IID_IFormObject, Dispatch)
	INTERFACE_PART(CFormObj, IID_IDispatch, DispatchExtend)
	INTERFACE_PART(CFormObj, IID_ICategorizeProperties, CategorizeProperties)
	INTERFACE_PART(CFormObj, IID_IProvideClassInfo, ProvideClassInfo)
	INTERFACE_PART(CFormObj, IID_IProvideClassInfo2, ProvideClassInfo)
END_INTERFACE_MAP()


//////////////////////////////////////////////////////////////////////
//XDispatchExtend members

STDMETHODIMP_( ULONG ) CFormObj::XDispatchExtend::AddRef()
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)

    return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormObj::XDispatchExtend::Release()
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	return ((IDispatch *)(&pThis->m_xDispatch))->Release();
}

STDMETHODIMP CFormObj::XDispatchExtend::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetIDsOfNames(
	REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs)
{
	USES_CONVERSION;

	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	if (nNames == 1)
	{
		if (!IsEqualIID( iid, IID_NULL))
		{
			return E_INVALIDARG;
		}
		if (ppszNames == NULL)
		{
			return E_INVALIDARG;
		}
		if (pDispIDs == NULL)
		{
			return E_POINTER;
		}

		CString strName = *ppszNames;

		int iIndex = 0;
		CDevObjList* pObjects = pThis->m_pFormDoc->GetObjects();
		POSITION posObj = pObjects->GetHeadPosition();
		CDevObj* pObj;
		while (posObj != NULL)
		{
			pObj = pObjects->GetNext(posObj);
			if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
			{
				if (pObj->GetDisplayName().CompareNoCase(strName) == 0)
				{
					*pDispIDs = CXDISPID_EXTPROP + iIndex;
					return S_OK;
				}
				
				iIndex++;
			}
		}
	}

	return ((IDispatch *)(&pThis->m_xDispatch))->GetIDsOfNames(iid, ppszNames, 
		 nNames, lcid, pDispIDs);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetTypeInfo(
	UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo)
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(IFormObject), ppTypeInfo);
}

STDMETHODIMP CFormObj::XDispatchExtend::GetTypeInfoCount(
   UINT* pnInfoCount )
{
	METHOD_PROLOGUE(CFormObj, DispatchExtend)
	ASSERT_VALID(pThis);

	return ((IDispatch *)(&pThis->m_xDispatch))->GetTypeInfoCount(pnInfoCount);
}

STDMETHODIMP CFormObj::XDispatchExtend::Invoke(
	DISPID dispidMember, REFIID iid, LCID lcid, WORD wFlags,
	DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo,
	UINT* piArgErr)
{
 	METHOD_PROLOGUE(CFormObj, DispatchExtend)

	if ((dispidMember & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
		return ((IDispatch *)(&pThis->m_xDispatch))->Invoke(dispidMember, iid, lcid, wFlags,
			pdpParams, pvarResult, pExceptionInfo, piArgErr);
	else
	{
		int iIndex = dispidMember - CXDISPID_EXTPROP;

		CDevObjList* pObjects = pThis->m_pFormDoc->GetObjects();
		POSITION posObj = pObjects->GetHeadPosition();
		CDevObj* pObj;
		while (posObj != NULL)
		{
			pObj = pObjects->GetNext(posObj);
			if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
			{
				if (iIndex == 0)
				{
					IDispatchPtr pDispatch = pObj->GetObjectUnknown();
					return pDispatch->Invoke(CXPROPID_Value, iid, lcid, wFlags,
						pdpParams, pvarResult, pExceptionInfo, piArgErr);
				}

				iIndex--;
			}
		}
	}

	return DISP_E_MEMBERNOTFOUND;
}

//////////////////////////////////////////////////////////////////////
//XCategorizeProperties members

STDMETHODIMP_( ULONG ) CFormObj::XCategorizeProperties::AddRef()
{
	METHOD_PROLOGUE(CFormObj, CategorizeProperties)

	TRACE("XCategorizeProperties::AddRef().\n");

	return pThis->ExternalAddRef();
}

STDMETHODIMP_( ULONG ) CFormObj::XCategorizeProperties::Release()
{
	METHOD_PROLOGUE(CFormObj, CategorizeProperties)

	TRACE("XCategorizeProperties::Release().\n");

	return pThis->ExternalRelease();
}

STDMETHODIMP CFormObj::XCategorizeProperties::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(CFormObj, CategorizeProperties)

	return pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormObj::XCategorizeProperties::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	METHOD_PROLOGUE(CFormObj, CategorizeProperties)

	return pThis->MapPropertyToCategory(dispid, ppropcat);
}

STDMETHODIMP CFormObj::XCategorizeProperties::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	METHOD_PROLOGUE(CFormObj, CategorizeProperties)

	return pThis->GetCategoryName(propcat, lcid, pbstrName);
}

//XProviderClassInfo
STDMETHODIMP_(ULONG) CFormObj::XProvideClassInfo::AddRef()
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CFormObj::XProvideClassInfo::Release()
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideClassInfo)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CFormObj::XProvideClassInfo::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideClassInfo)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP CFormObj::XProvideClassInfo::GetClassInfo(
	LPTYPEINFO* ppTypeInfo)
{
	METHOD_PROLOGUE_EX(CFormObj, ProvideClassInfo)

	return theApp.m_pHMITypeLib->GetTypeInfoOfGuid(__uuidof(FormObject), ppTypeInfo);
	
//	return pThis->GetTypeInfoOfGuid(GetUserDefaultLCID(), __uuidof(UserForm), ppTypeInfo);
}

STDMETHODIMP CFormObj::XProvideClassInfo::GetGUID(DWORD dwGuidKind,
	GUID* pGUID)
{
	METHOD_PROLOGUE_EX_(CFormObj, ProvideClassInfo)

	if (dwGuidKind == GUIDKIND_DEFAULT_SOURCE_DISP_IID)
	{
		*pGUID = __uuidof(IFormObjectEvents);
		return NOERROR;
	}
	else
	{
		*pGUID = GUID_NULL;
		return E_INVALIDARG;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFormObj message handlers

BOOL CFormObj::GetDispatchIID(IID* piid)
{
	ASSERT( piid != NULL );

	*piid = __uuidof(IFormObject);

	return TRUE;
}

void CFormObj::OnUniformScaleChanged()
{
}

int CFormObj::GetPrivilege()
{
	return m_nPrivilege;
}

void CFormObj::SetPrivilege(int iNewValue)
{
	if (iNewValue < 0 || iNewValue > 1000)
		return;
	
	m_nPrivilege = iNewValue;
}

enumMousePointer CFormObj::GetMousePointer()
{
	return m_pFormDoc->m_enumMousePointer;
}

void CFormObj::SetMousePointer(enumMousePointer fNewValue)
{
	m_pFormDoc->m_enumMousePointer = fNewValue;
}

void CFormObj::OnToolTipTextChanged()
{
}

void CFormObj::OnDescriptionChanged()
{
}

float CFormObj::GetHorizontalPosition()
{
	return m_position.left;
}

void CFormObj::SetHorizontalPosition(float fNewValue)
{
	CRectF rect = m_position;
	rect.OffsetRect(fNewValue - m_position.left, 0);
	MoveTo(rect, FALSE);
}

float CFormObj::GetRoateBasePointX()
{
	return m_ptRotateBase.x;
}

void CFormObj::SetRoateBasePointX(float fNewValue)
{
	m_ptRotateBase.x = fNewValue;
}

float CFormObj::GetRoateBasePointY()
{
	return m_ptRotateBase.y;
}

void CFormObj::SetRoateBasePointY(float fNewValue)
{
	m_ptRotateBase.y = fNewValue;
}

float CFormObj::GetRotateAngle()
{
	return 0;
}

void CFormObj::SetRotateAngle(float fNewValue)
{
	if (IsRunMode())
	{
		RotateAt(m_ptRotateBase, fNewValue - m_fRotateAngle);		
		m_fRotateAngle = fNewValue;
	}
	else
	{
		RotateAt(m_ptRotateBase, fNewValue);
		m_fRotateAngle = 0;
	}
}

CPointF CFormObj::GetScaleBasePointF()
{
	switch (m_enumScaleBase)
	{
	case ScaleBasePointCenter:
		return m_position.CenterPoint();
	case ScaleBasePointLeftTop:
		return m_position.TopLeft();
	case ScaleBasePointTopMiddle:
		return CPointF((m_position.left + m_position.right) / 2, m_position.top);
	case ScaleBasePointRightTop:
		return CPointF(m_position.right, m_position.top);
	case ScaleBasePointRightMiddle:
		return CPointF(m_position.right, (m_position.top + m_position.bottom) / 2);
	case ScaleBasePointRightBottom:
		return CPointF(m_position.right, m_position.bottom);
	case ScaleBasePointBottomMiddle:
		return CPointF((m_position.left + m_position.right) / 2, m_position.bottom);
	case ScaleBasePointLeftBottom:
		return CPointF(m_position.left, m_position.bottom);
	case ScaleBasePointLeftMiddle:
		return CPointF(m_position.left, (m_position.top + m_position.bottom) / 2);
	default:
		ASSERT(FALSE);
	}

	return CPointF(0, 0);
}

enumScaleBasePoint CFormObj::GetScaleBasePoint()
{
	return m_enumScaleBase;
}

void CFormObj::SetScaleBasePoint(enumScaleBasePoint fNewValue)
{
	m_enumScaleBase = fNewValue;
}

float CFormObj::GetHorizontalScale()
{
	return 1;
}

void CFormObj::SetHorizontalScale(float fNewValue)
{
	if (IsRunMode())
	{
		if (fNewValue == 0)
			fNewValue = 1 / m_position.Width();
		float fHorScale = fNewValue / m_fHorScale;		
		m_fHorScale = fNewValue;
		ScaleAt(GetScaleBasePointF(), fHorScale, 1);
	}
	else
	{
		ScaleAt(GetScaleBasePointF(), fNewValue, 1);
	}
}

float CFormObj::GetVerticalScale()
{
	return 1;
}

void CFormObj::SetVerticalScale(float fNewValue)
{
	if (IsRunMode())
	{
		if (fNewValue == 0)
			fNewValue = 1 / m_position.Height();
		float fVerScale = fNewValue / m_fVerScale;		
		m_fVerScale = fNewValue;
		ScaleAt(GetScaleBasePointF(), 1, fVerScale);
	}
	else
	{
		ScaleAt(GetScaleBasePointF(), 1, fNewValue);
	}
}

int CFormObj::GetLayer()
{
	return m_nLayer + 1;
}

void CFormObj::SetLayer(int lLayer)
{
	if (lLayer > 30 || lLayer < 1)
		return;

	m_nLayer = lLayer - 1;
	Invalidate();
}

void CFormObj::OnLockedChanged()
{
	Invalidate();
}

void CFormObj::OnVisibleChanged()
{
	Invalidate();
}

BSTR CFormObj::GetName()
{
	return GetDisplayName().AllocSysString();
}

void CFormObj::SetName(LPCTSTR lpszNewValue)
{
	if (PutDisplayName(lpszNewValue))
		m_pFormDoc->OnFormObjChange();
}

BSTR CFormObj::GetTagGroup()
{
	return m_pFormDoc->m_strTagGroup.AllocSysString();	
}

void CFormObj::SetTagGroup(LPCTSTR lpszNewValue)
{
	m_pFormDoc->m_strTagGroup = lpszNewValue;
}

BSTR CFormObj::GetAssocTagGroup()
{
	return m_pFormDoc->GetAssocTagGroupEx(FALSE).AllocSysString();	
}
	
VARIANT_BOOL CFormObj::IsConnected(LPCTSTR pszPropName)
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
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->IsConnected(L"Value", &bHasConnection);
		}
	}

	return bHasConnection;
}

void CFormObj::ConnectObject(LPCTSTR pszPropName, LPUNKNOWN punkObject)
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
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->ConnectObject(L"Value", punkObject);
		}
	}
}

void CFormObj::ConnectDirect(LPCTSTR pszPropName, LPCTSTR pszDataSource)
{
}

LPUNKNOWN CFormObj::GetConnectObject(LPCTSTR pszPropName)
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
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->GetConnectObject(L"Value", &punkObject);
		}
	}

	return punkObject;
}

void CFormObj::Disconnect(LPCTSTR pszPropName)
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
		CDevObj* pObj = FindVariableObject(pszPropName);
		if (pObj != NULL)
		{
			CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
			spDrawVariable->Disconnect(L"Value");
		}
	}
}

int CFormObj::GetConnectState(DISPID dispid)
{
	int nState = 0;

	if ((dispid & CXDISPID_EXTPROP) != CXDISPID_EXTPROP)
	{
		//只有水平位置，垂直位置，旋转角度支持鼠标输入
		if (dispid == CXPROPID_HorPosition || dispid == CXPROPID_VerPosition || dispid == CXPROPID_RotateAngle)
			nState |= 0x1;

		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			if (m_arLinkInfo[i].dispid == dispid)
			{
				nState |= 0x2; //已经连接
				if (m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
					nState |= 0x4;
				break;
			}
		}
	}
	else
	{
		int iIndex = dispid - CXDISPID_EXTPROP;
		
		CDevObjList* pObjects = m_pFormDoc->GetObjects();
		POSITION posObj = pObjects->GetHeadPosition();
		CDevObj* pObj;
		while (posObj != NULL)
		{
			pObj = pObjects->GetNext(posObj);
			if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
			{
				if (iIndex == 0)
				{
					CComQIPtr<IDrawVariable, &IID_IDrawVariable> spDrawVariable(pObj->GetObjectUnknown());
					spDrawVariable->GetConnectState(0, &nState);
				}
				
				iIndex--;
			}
		}
	}
	
	return nState;
}

// 此函数在开发环境无用, 没有实现
LPUNKNOWN CFormObj::GetObjectByName(LPCTSTR lpszName)
{
	return NULL;
}

// 此函数在开发环境无用, 没有实现
LPDISPATCH CFormObj::GetObjectCollection()
{
	return NULL;
}

float CFormObj::GetVerticalPosition()
{
	return m_position.top;
}

void CFormObj::SetVerticalPosition(float fNewValue)
{
	CRectF rect = m_position;
	rect.OffsetRect(0, fNewValue - m_position.top);
	MoveTo(rect, FALSE);
}

float CFormObj::GetWidth()
{
	return m_position.Width();
}

void CFormObj::SetWidth(float fNewValue)
{
	if (ABSOLUTEVALUE(fNewValue) < 0.1f)
		fNewValue = fNewValue >= 0 ? 0.1f : -0.1f;
	
	CRectF rect = m_position;
	rect.right = m_position.left + fNewValue;
	if (m_bUniformScale)
	{
		float fScaleX = rect.Width() / m_position.Width();
		rect.bottom = rect.top + rect.Height() * fScaleX;
	}
	MoveTo(rect, FALSE);
}

float CFormObj::GetHeight()
{
	return m_position.Height();
}

void CFormObj::SetHeight(float fNewValue)
{
	if (ABSOLUTEVALUE(fNewValue) < 0.1f)
		fNewValue = fNewValue >= 0 ? 0.1f : -0.1f;
	
	CRectF rect = m_position;
	rect.bottom = m_position.top + fNewValue;
	if (m_bUniformScale)
	{
		float fScaleY = rect.Height() / m_position.Height();
		rect.right = rect.left + rect.Width() * fScaleY;
	}
	MoveTo(rect, FALSE);
}

OLE_COLOR CFormObj::GetForeColor()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_clrForeColor;
}

void CFormObj::SetForeColor(OLE_COLOR nNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_clrForeColor = nNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

OLE_COLOR CFormObj::GetBackColor()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_clrBackColor;
}

void CFormObj::SetBackColor(OLE_COLOR nNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_clrBackColor = nNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

BSTR CFormObj::GetBrushImageFile()
{
	ASSERT(m_pFormDoc != NULL);
	
	return m_pFormDoc->m_strBrushImageFile.AllocSysString();
}

void CFormObj::SetBrushImageFile(LPCTSTR lpszNewValue)
{
	ASSERT(m_pFormDoc != NULL);
	
	m_pFormDoc->m_strBrushImageFile = lpszNewValue;
	m_pFormDoc->OnBrushImageFileChanged();	
}

void CFormObj::OnTransparencyChanged()
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->UpdateAllViews(NULL);
}

int CFormObj::GetVisibleLayer()
{
	ASSERT(m_pFormDoc != NULL);

	return m_pFormDoc->m_ulVisibleLayer;
}

void CFormObj::SetVisibleLayer(int iNewValue)
{
	ASSERT(m_pFormDoc != NULL);

	m_pFormDoc->m_ulVisibleLayer = iNewValue;
	m_pFormDoc->UpdateAllViews(NULL);
}

CDevObj::TrackerState CFormObj::GetTrackerState(TrackerState state)
{
	if (state == general || state == reshape)
		return resize;
	return state;
}

HRESULT CFormObj::MapPropertyToCategory(DISPID dispid, PROPCAT* ppropcat)
{
	switch (dispid)
	{
	case CXPROPID_HorPosition:
	case CXPROPID_VerPosition:
	case CXPROPID_Width:
	case CXPROPID_Height:
		*ppropcat = -4;
		break;
	case CXPROPID_BackColor:
	case CXPROPID_ForeColor:
	case CXPROPID_BrushImageFileName:
	case CXPROPID_Transparency:
//	case CXPROPID_GridColor:
		*ppropcat = -5;
		break;
	case CXPROPID_RotateAngle:
	case CXPROPID_RoateBasePointX:
	case CXPROPID_RoateBasePointY:
	case CXPROPID_ScaleBasePoint:
	case CXPROPID_HorScale:
	case CXPROPID_VerScale:
	case CXPROPID_UniformScale:
		*ppropcat = 3;
		break;
	default:
		if (dispid >= CXDISPID_EXTPROP)
			*ppropcat = -6;
		else
			*ppropcat = -2;
		break;
	}
	
	return S_OK;
}

HRESULT CFormObj::GetCategoryName(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
{
	switch (propcat)
	{
	case -2:
		*pbstrName = ::SysAllocString(OLESTR("杂项"));
		break;
	case -5:
		*pbstrName = ::SysAllocString(OLESTR("外观"));
		break;
	case 3:
		*pbstrName = ::SysAllocString(OLESTR("变换"));
		break;
	case -4:
		*pbstrName = ::SysAllocString(OLESTR("位置"));
		break;
	case -6:
		*pbstrName = ::SysAllocString(OLESTR("变量"));
		break;
	default:
		return S_FALSE;
	}	
	
	return S_OK;
}

CDevObj* CFormObj::FindVariableObject(LPCTSTR lpszName)
{
	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
		{
			if (pObj->GetDisplayName().CollateNoCase(lpszName) == 0)
				return pObj;
		}
	}

	return NULL;
}

ICxEnumExtendProperty* CFormObj::EnumExtendProperty()
{
	CArray<CProp, CProp&> arrProp;	

	CDevObjList* pObjects = m_pFormDoc->GetObjects();
	POSITION posObj = pObjects->GetHeadPosition();
	CDevObj* pObj;
	while (posObj != NULL)
	{
		pObj = pObjects->GetNext(posObj);
		if (pObj->IsKindOf(RUNTIME_CLASS(CDrawObj)) && pObj->GetClassName() == _T("Variable"))
		{
			CProp prop;
			prop.m_strName = pObj->GetDisplayName();
			
			COleDispatchDriver dd;
			IDispatchPtr pDispatch = pObj->GetObjectUnknown();
			dd.AttachDispatch(pDispatch, FALSE);
			
			enumPropertyType nType;
			dd.GetProperty(CXPROPID_VariableType, VT_I4, (void*)&nType);
			switch (nType) 
			{
			case PropertyTypeColor:
				prop.m_proptype = PROPTYPE_COLOR;
				break;
			case PropertyTypeBool:
				prop.m_proptype = PROPTYPE_BOOL;
				break;
			case PropertyTypeString:
			default:
				prop.m_proptype = PROPTYPE_STRING;
				break;
			case PropertyTypeNumeric:
				prop.m_proptype = PROPTYPE_NUMBER;
				break;
			}

			dd.GetProperty(CXPROPID_Description, VT_BSTR, (void*)&prop.m_strDescription);
			if (prop.m_strDescription.IsEmpty())
				prop.m_strDescription = prop.m_strName;
			dd.GetProperty(CXPROPID_Value, VT_VARIANT, (void*)&prop.m_value);
 
			prop.m_dispid = CXDISPID_EXTPROP + arrProp.GetSize();

			arrProp.Add(prop);
		}
	}

	if (arrProp.GetSize() == 0)
		return NULL;

	CProp* pProps = new CProp[arrProp.GetSize()];
	if (pProps == NULL)
		AfxThrowMemoryException();
	
	for(int i = 0; i < arrProp.GetSize(); i++)
		pProps[i] = arrProp[i];
	
	CEnumExtendProp* pEnum = new CEnumExtendProp(pProps, arrProp.GetSize());
	
	ICxEnumExtendProperty* pRet = (ICxEnumExtendProperty *)&pEnum->m_xEnumProp;
	pRet->Reset();
	
	return pRet;
}