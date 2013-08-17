// CxDrawObj.h: interface for the ICxOleObjecImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CXDRAWOBJ_H_
#define __CXDRAWOBJ_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "gdiplus.h"
#include "math.h"
#include "Windowsx.h"
//#include "CxCombo.h"

#define DegToRadConst  0.0174533				//3.1415926 / 180 = 0.0174533

#define ROUND(f) (int)((f) >= 0 ? f + .4999999 : f - .4999999)  //四舍五入

#define OLETRANSLATEGDIPLUSCOLOR(gdiplusColor)\
	Color gdiplusColor;\
	COLORREF colorref##gdiplusColor;\
	OleTranslateColor(m_##gdiplusColor, 0, &colorref##gdiplusColor);\
	gdiplusColor.SetFromCOLORREF(colorref##gdiplusColor);


// Button values for mouse events
#define LEFT_BUTTON     0x01
#define RIGHT_BUTTON    0x02
#define MIDDLE_BUTTON   0x04

inline short _GetShiftState()
{
	BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
	BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
	BOOL bAlt   = (GetKeyState(VK_MENU) < 0);

	return (short)(bShift + (bCtrl << 1) + (bAlt << 2));
}

//EXTERN_C const CLSID CLSID_CxReturnInteger;
// = {0x82B02373,0xB5BC,0x11CF,{0x81,0x0F,0x00,0xA0,0xC9,0x03,0x00,0x74}};

#if (WINVER < 0x0500)
__declspec(selectany) struct
{
	enum { cxWidth = 32, cyHeight = 32 };
	int xHotSpot;
	int yHotSpot;
	unsigned char arrANDPlane[cxWidth * cyHeight / 8];
	unsigned char arrXORPlane[cxWidth * cyHeight / 8];
} _Hand_CursorData = 
{
	5, 0, 
	{
		0xF9, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 
		0xF0, 0xFF, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xF0, 0x01, 0xFF, 0xFF, 
		0xF0, 0x00, 0xFF, 0xFF, 0x10, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 
		0x80, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xC0, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x7F, 0xFF, 
		0xE0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 
		0xF8, 0x01, 0xFF, 0xFF, 0xF8, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	},
	{
		0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
		0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0xC0, 0x00, 0x00, 0x06, 0xD8, 0x00, 0x00, 
		0x06, 0xDA, 0x00, 0x00, 0x06, 0xDB, 0x00, 0x00, 0x67, 0xFB, 0x00, 0x00, 0x77, 0xFF, 0x00, 0x00, 
		0x37, 0xFF, 0x00, 0x00, 0x17, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00, 
		0x0F, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x03, 0xFC, 0x00, 0x00, 
		0x03, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}
};
#endif //(WINVER < 0x0500)


/////////////////////////////////////////////////////////////////////
//自定义的属性映象
struct CXPROPMAP_ENTRY
{
	LPCOLESTR szDesc;
	DISPID dispid;
};

#define BEGIN_CXPROP_MAP()\
	static CXPROPMAP_ENTRY* GetCxPropMap()\
	{\
		static CXPROPMAP_ENTRY pCxPropMap[] = \
		{

#define CXPROP_ENTRY(szDesc, dispid)\
			{OLESTR(szDesc), dispid},

#define END_CXPROP_MAP()\
			{NULL, 0}\
		};\
		return pCxPropMap;\
	}

//////////////////////////////////////////////////////////////////////////////
//自定义的两个函数
inline HRESULT CxIPersistStream_Load(LPSTREAM pStm, CXPROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	ATLASSERT(pMap != NULL);
	HRESULT hr = S_OK;
	DWORD dwVer;
	hr = pStm->Read(&dwVer, sizeof(DWORD), NULL);
	if (FAILED(hr))
		return hr;
	if (dwVer > _ATL_VER)
		return E_FAIL;

	CComPtr<IDispatch> pDispatch = NULL;
	if (FAILED(pUnk->QueryInterface(IID_IDispatch, (void**)&pDispatch)))
	{
		ATLTRACE(_T("Failed to get a dispatch pointer for property #%i\n"));
		hr = E_FAIL;
		return hr;
	}

	for (int i = 0; pMap[i].dispid != 0; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

	//	if (pMap[i].dispid == CXPROPID_FlowDirect)
	//		continue;
	
		CComVariant var;

		hr = var.ReadFromStream(pStm);
		if (FAILED(hr))
			break;

		if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			hr = E_FAIL;
			break;
		}
	}
	return hr;
}

inline HRESULT CxIPersistStream_Save(LPSTREAM pStm,
	BOOL /* fClearDirty */, CXPROPMAP_ENTRY* pMap,
	void* pThis, IUnknown* pUnk)
{
	ATLASSERT(pMap != NULL);
	DWORD dw = _ATL_VER;
	HRESULT hr = pStm->Write(&dw, sizeof(DWORD), NULL);
	if (FAILED(hr))
		return hr;

	CComPtr<IDispatch> pDispatch;
	if (FAILED(pUnk->QueryInterface(IID_IDispatch, (void**)&pDispatch)))
	{
		ATLTRACE(_T("Failed to get a dispatch pointer for property #%i\n"));
		hr = E_FAIL;
		return hr;
	}

	for (int i = 0; pMap[i].dispid != 0; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		CComVariant var;
		
		if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			hr = E_FAIL;
			break;
		}

		hr = var.WriteToStream(pStm);
		if (FAILED(hr))
			break;
	}
	return hr;
}


/////////////////////////////////////////////////////////////////////
//自定义的属性页映象
struct CXPAGEMAP_ENTRY
{
	LPCOLESTR szDesc;
	const CLSID* pclsidPropPage;
};

#define BEGIN_CXPAGE_MAP()\
	static CXPAGEMAP_ENTRY* GetCxPageMap()\
	{\
		static CXPAGEMAP_ENTRY pCxPageMap[] = \
		{

#define CXSHEET_ENTRY(szDesc)\
			{OLESTR(szDesc), &CLSID_NULL},
#define CXPAGE_ENTRY(clsid)\
			{NULL, &clsid},

#define END_CXPAGE_MAP()\
			{NULL, NULL}\
		};\
		return pCxPageMap;\
	}
	
///////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CCxDrawObjectBase
{
public:
	CCxDrawObjectBase()
	{
		m_fRotateAngle = 0;						
		m_fHorScale = 1.0;							
		m_fVerScale = 1.0;
				
		m_pObjectPathOrgin = NULL;
	}
		
	~CCxDrawObjectBase()
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			if (m_arLinkInfo[i].pDynamicObject != NULL)
				m_arLinkInfo[i].pDynamicObject->Release();
		}
		if (m_pObjectPath != NULL)
			delete m_pObjectPath;
		if (m_pObjectPathOrgin != NULL)
			delete m_pObjectPathOrgin;
	}

public:
	//////////////////////////////////////////////////////
	//定义公用成员变量
	//修改记录: 卢文江10.24增加
	//////////////////////////////////////////////////////
	enumMousePointer m_enumMousePointer;

	OLE_COLOR m_clrEdgeColor;				//边界颜色
	OLE_COLOR m_clrHighLightEdge;			//边界高亮色
	enumEdgeStyle m_enumEdgeStyle;				//边界型式
	enumLineJoinStyle m_enumLineJoinStyle;		//接头类型
	float m_fEdgeWidth;							//边界线宽

	OLE_COLOR m_clrFillColor;		//前景颜色
	OLE_COLOR m_clrHighLightFill;			//前景高亮色
	//Gdiplus::Color m_clrBackgroundColor;		//背景颜色
	enumFillStyle m_enumFillStyle;				//前景类型
	CComBSTR m_BrushImageFileName;				//位图画刷文件名

	CComBSTR m_bstrDescription;      // 描述文本

	//enumBackgroundStyle m_enumBackgroundStyle;	//背景方式
	enumVerticalDirection m_enumVerFillDirection;	//垂直填充方向
	enumHorizontalDirection m_enumHorFillDirection;	//水平填充方向
	long m_nVerFillPercent;					//垂直填充百分比
	long m_nHorFillPercent;					//水平填充百分比

	float m_fRotateAngle;						//旋转角度
	float m_fHorScale;							//水平缩放因子
	float m_fVerScale;							//垂直缩放因子
	enumScaleBasePoint m_enumScaleBase;		//变换基点
	Gdiplus::PointF m_ptRotateBase;			//旋转基点

//	enumMouseInputMode m_enumMouseInputMode;  //鼠标输入模式

	Gdiplus::GraphicsPath *m_pObjectPath;				//对象路径
	Gdiplus::GraphicsPath *m_pObjectPathOrgin;

	Gdiplus::Brush* m_pFillBrush;
	
//	RECT m_rcRect;
	
public:

	virtual HRESULT DoDraw(HDC hdcDest)
	{
		using namespace Gdiplus;
		Graphics graphics(hdcDest);
		FillObjectRgn(&graphics, m_pObjectPath);
		DrawObjectEdge(&graphics, m_pObjectPath);

		return S_OK;
	}

	inline HRESULT ICxDrawObject_Draw(HDC hdcDest)
	{
		return DoDraw(hdcDest);
	}

	virtual HRESULT DoPrint(HDC hdcDest)
	{
		using namespace Gdiplus;
		Graphics graphics(hdcDest);
		graphics.SetPageUnit(UnitPixel);
		FillObjectRgn(&graphics, m_pObjectPath);
		DrawObjectEdge(&graphics, m_pObjectPath);
		
		return S_OK;
	}
	
	inline HRESULT ICxDrawObject_Print(HDC hdcDest)
	{
		return DoPrint(hdcDest);
	}

	void ReSetRotateBasePoint()
	{
		Gdiplus::RectF rcPos;
		m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
		m_ptRotateBase.X =  rcPos.X + rcPos.Width / 2;
		m_ptRotateBase.Y =  rcPos.Y + rcPos.Height / 2;
	}

	virtual void ReSetObjectPath()
	{
		using namespace Gdiplus;

		if (m_pObjectPathOrgin == NULL)
		{
			m_pObjectPathOrgin = m_pObjectPath->Clone(); 
		}
		else
		{
			delete m_pObjectPath;
			m_pObjectPath = m_pObjectPathOrgin->Clone();
		}
	
		if (m_fHorScale == 1 && m_fVerScale == 1 && m_fRotateAngle == 0)
			return;
		
		Matrix mtTansMatrix;
		POINT ptScaleBase = GetScaleBasePoint();
		mtTansMatrix.Translate((float)ptScaleBase.x, (float)ptScaleBase.y);
		mtTansMatrix.Scale(m_fHorScale, m_fVerScale);//, MatrixOrderAppend);
		mtTansMatrix.Translate((float)-ptScaleBase.x, (float)-ptScaleBase.y);//, MatrixOrderAppend);
		mtTansMatrix.RotateAt(m_fRotateAngle, m_ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);
	}

	//通知容器变化
	HRESULT FireViewChange()
	{
		if (m_spDrawObjectAdviseSink)
			m_spDrawObjectAdviseSink->OnViewChange();
		return S_OK;
	}

	HRESULT FireDynamicChange()
	{
		if (m_spDrawObjectAdviseSink)
			m_spDrawObjectAdviseSink->OnDynamicChange();
		return S_OK;
	}

	BOOL ClientIsRunMode()
	{
	//	return TRUE;
		return (m_spDrawObjectSite->IsRunMode() == S_OK);
	}

	long GetClientPrivilege()
	{
		long lLayer;
		m_spDrawObjectSite->GetClientPrivilege(&lLayer);
		return lLayer;
	}

	void SetClientPrivilege(long lLayer)
	{
		m_spDrawObjectSite->SetClientPrivilege(lLayer);
	}

	long GetClientLayer()
	{
		long lLayer;
		m_spDrawObjectSite->GetClientLayer(&lLayer);
		return lLayer;
	}

	void SetClientLayer(long lLayer)
	{
		m_spDrawObjectSite->SetClientLayer(lLayer);
	}

	BOOL GetClientLocked()
	{
		BOOL bLocked;
		m_spDrawObjectSite->GetClientLocked(&bLocked);
		return bLocked;
	}
	
	void SetClientLocked(BOOL bLocked)
	{
		m_spDrawObjectSite->SetClientLocked(bLocked);
	}

	BOOL GetClientVisibility()
	{
		BOOL bVisible;
		m_spDrawObjectSite->GetClientVisibility(&bVisible);
		return bVisible;
	}

	void SetClientVisibility(BOOL bVisible)
	{
		m_spDrawObjectSite->SetClientVisibility(bVisible);
	}

	BSTR GetClientName()
	{
		BSTR bstrName;
		m_spDrawObjectSite->GetClientName(&bstrName);
		return bstrName;
	}

	void SetClientName(BSTR bstrName)
	{
		m_spDrawObjectSite->SetClientName(bstrName);
	}

	BSTR GetClientToolTipText()
	{
		BSTR bstrText;
		m_spDrawObjectSite->GetClientToolTip(&bstrText);
		return bstrText;
	}

	void SetClientToolTipText(BSTR bstrText)
	{
		m_spDrawObjectSite->SetClientToolTip(bstrText);
	}

	virtual void AddObjectToPath()
	{
	}

	HRESULT SavePathData(LPSTREAM pStm)
	{
		using namespace Gdiplus;

		PathData pathData;
		if (ClientIsRunMode() && m_pObjectPathOrgin != NULL)
			m_pObjectPathOrgin->GetPathData(&pathData);
		else
			m_pObjectPath->GetPathData(&pathData);
		pStm->Write(&pathData.Count, sizeof(INT), NULL);
		pStm->Write(pathData.Points, sizeof(PointF) * pathData.Count, NULL);
		pStm->Write(pathData.Types, sizeof(BYTE) * pathData.Count, NULL);

		return S_OK;
	}

	HRESULT LoadPathData(LPSTREAM pStm)
	{
		using namespace Gdiplus;

		INT count;
		pStm->Read(&count, sizeof(INT), NULL);
		PointF* points = new PointF[count];
		pStm->Read(points, sizeof(PointF) * count, NULL);
		BYTE* types = new BYTE[count];
		pStm->Read(types, sizeof(BYTE) * count, NULL);

		m_pObjectPath = new Gdiplus::GraphicsPath(points, types, count);

		delete [] points;
		delete [] types;

		return S_OK;
	}

	virtual POINT GetScaleBasePoint()
	{
		using namespace Gdiplus;
	
		RECTF rectF;
		ICxDrawObject_GetPositionRect(&rectF);
		POINT ptScaleBase;
		DefaultHandle(rectF, m_enumScaleBase, &ptScaleBase);
		
		return ptScaleBase;
	}

	///////////////////////////////////////////////////////////////////////////
	//功能: 画边界
	//参数: graphics [in] : Graphics指针
	//		   ObjectPath [in] : 对象路径指针 
	///////////////////////////////////////////////////////////////////////////
	inline void DrawObjectEdge(Gdiplus::Graphics *graphics, Gdiplus::GraphicsPath *ObjectPath)
	{
		using namespace Gdiplus;

		//透明线直接返回
		if (m_enumEdgeStyle == EdgeStyleTransparence)
			return;

		OLETRANSLATEGDIPLUSCOLOR(clrEdgeColor);
		Pen EdgePen(clrEdgeColor, m_fEdgeWidth);

		if (m_enumLineJoinStyle == LineJoinStyleRound)
		{
			EdgePen.SetLineJoin(LineJoinRound);
			EdgePen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		}
		else if (m_enumLineJoinStyle == LineJoinStyleSquare)
		{
			EdgePen.SetLineJoin(LineJoinMiter);
			EdgePen.SetLineCap(LineCapSquare, LineCapSquare, DashCapFlat);
		}
			
		if (m_enumEdgeStyle >= 0/*DashStyleSolid*/ && m_enumEdgeStyle <= 4/*DashStyleDashDotDot*/)
		{
			EdgePen.SetDashStyle((DashStyle)m_enumEdgeStyle);
			graphics->DrawPath(&EdgePen, ObjectPath);
			return;
		}
		
		if (m_enumEdgeStyle == EdgeStylePipe)
		{
			OLETRANSLATEGDIPLUSCOLOR(clrHighLightEdge);
			BYTE R1 = clrEdgeColor.GetRed();
			BYTE G1 = clrEdgeColor.GetGreen();
			BYTE B1 = clrEdgeColor.GetBlue();
			BYTE R_setp = BYTE((clrHighLightEdge.GetRed() - R1) / m_fEdgeWidth);
			BYTE G_setp = BYTE((clrHighLightEdge.GetGreen() - G1) / m_fEdgeWidth);
			BYTE B_setp = BYTE((clrHighLightEdge.GetBlue() - B1) / m_fEdgeWidth);
			
			for (int i = 1; i <= ROUND(m_fEdgeWidth); i++)
			{
				EdgePen.SetColor(Color(BYTE(R1 + R_setp * i),  
										BYTE(G1 + G_setp * i),
										BYTE(B1 + B_setp * i)));
				EdgePen.SetWidth(m_fEdgeWidth - i + 1);

				graphics->DrawPath(&EdgePen, ObjectPath);
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////
	//功能: 填充对象区域
	//参数: graphics [in] : Graphics指针
	//		   ObjectPath [in] : 对象路径指针 
	///////////////////////////////////////////////////////////////////////////
	inline void FillObjectRgn(Gdiplus::Graphics *graphics, Gdiplus::GraphicsPath *pObjectPath)
	{
		using namespace Gdiplus;

		Brush* pBrush = CreateBrush(pObjectPath);
		if (pBrush == NULL)
			return;

		RECT* prcFillBound = NULL;
		if (m_spDrawObjectSite != NULL)
			m_spDrawObjectSite->GetFillBoundRect(&prcFillBound);
		        		
		if (m_nVerFillPercent == 100 && m_nHorFillPercent == 100 && prcFillBound == NULL)
		{
			graphics->FillPath(pBrush, pObjectPath);
		}
		else
		{
			Rect BoundRect;
			pObjectPath->GetBounds(&BoundRect, NULL, NULL);
			if (m_nVerFillPercent != 100)
			{
				switch (m_enumVerFillDirection)
				{
					case VerticalDirectionFromTop:
					{
						BoundRect.Height = BoundRect.Height * m_nVerFillPercent / 100;
					}
					break;
					case VerticalDirectionFromBottom:
					{
						long Height = BoundRect.Height * m_nVerFillPercent / 100;
						BoundRect.Y = BoundRect.Y + (BoundRect.Height - Height);
						BoundRect.Height = Height;
					}
					break;
					case VerticalDirectionFromCenter:
					{
						long Height = BoundRect.Height * m_nVerFillPercent / 100;
						BoundRect.Y = BoundRect.Y + (BoundRect.Height - Height) / 2;
						BoundRect.Height = Height;
					}
					break;
				}
			}

			if (m_nHorFillPercent != 100)
			{
				switch (m_enumHorFillDirection)
				{
					case HorizontalDirectionFromLeft:
					{
						BoundRect.Width = BoundRect.Width * m_nHorFillPercent / 100;
					}
					break;
					case HorizontalDirectionFromRight:
					{
						long Width = BoundRect.Width * m_nHorFillPercent / 100;
						BoundRect.X = BoundRect.X + (BoundRect.Width - Width);
						BoundRect.Width = Width;
					}
					break;
					case HorizontalDirectionFromCenter:
					{
						long Width = BoundRect.Width * m_nHorFillPercent / 100;
						BoundRect.X = BoundRect.X + (BoundRect.Width - Width) / 2;
						BoundRect.Width = Width;
					}
					break;
				}
			}
			
			Region rgnObjectRegion(pObjectPath);
			rgnObjectRegion.Intersect(BoundRect);
			if (prcFillBound != NULL)
			{
				BoundRect.X = prcFillBound->left;
				BoundRect.Y = prcFillBound->top;
				BoundRect.Width = prcFillBound->right - prcFillBound->left;
				BoundRect.Height = prcFillBound->bottom - prcFillBound->top;
				rgnObjectRegion.Intersect(BoundRect);
			}

			graphics->FillRegion(pBrush, &rgnObjectRegion);
		}

		delete pBrush;
	}

	void DrawPathDataShape(HDC hDC, Gdiplus::PathData& pathData)
	{
		using namespace Gdiplus;

		int nStartLast;
		POINT* pPoint = new POINT[pathData.Count];
		for (int i = 0; i < pathData.Count; i++)
		{
			pPoint[i].x = ROUND(pathData.Points[i].X);
			pPoint[i].y = ROUND(pathData.Points[i].Y);
		}
		for (i = 0; i < pathData.Count; i++)
		{
			BYTE pathType = pathData.Types[i] & PathPointTypePathTypeMask;

			switch (pathType)
			{
			case PathPointTypeLine:
				LineTo(hDC, pPoint[i].x, pPoint[i].y);
				break;

			case PathPointTypeBezier:
				PolyBezierTo(hDC, pPoint + i, 3);
				i += 2;
				break;

			case PathPointTypeStart:
				MoveToEx(hDC, pPoint[i].x, pPoint[i].y, NULL);
				nStartLast = i;
				continue;
			}
			
			if (pathData.Types[i] & PathPointTypeCloseSubpath)
				LineTo(hDC, pPoint[nStartLast].x, pPoint[nStartLast].y);
		}

		delete [] pPoint;
	}

	///////////////////////////////////////////////////////////////////////////
	//功能: 设置画刷
	//参数: ObjectPath [in] : 对象的路径
	///////////////////////////////////////////////////////////////////////////
	inline Gdiplus::Brush* CreateBrush(Gdiplus::GraphicsPath *ObjectPath)
	{
		using namespace Gdiplus;
		
		switch (m_enumFillStyle)
		{
			case FillStyleSolid:					
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					return new SolidBrush(clrFillColor);
				}
			case FillStyleTransparence:				
				{
					return NULL;
				}
			case FillStyleGradientCenterLight:			
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					Color colors[] = {clrFillColor};
					int count = 1;
					PathGradientBrush* brush = new PathGradientBrush(ObjectPath);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					brush->SetCenterColor(clrHighLightFill);
					brush->SetSurroundColors(colors, &count);
					return brush;
				}
			case FillStyleGradientAroundLight:			
				{
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					Color colors[] = {clrHighLightFill};
					int count = 1;
					PathGradientBrush* brush = new PathGradientBrush(ObjectPath);
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					brush->SetCenterColor(clrFillColor);
					brush->SetSurroundColors(colors, &count);
					return brush;
				}
			case FillStyleGradientHorMiddleLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					Color colors[] = {clrFillColor};
					int count = 1;
					PathGradientBrush* brush = new PathGradientBrush(ObjectPath);
					brush->SetCenterColor(clrHighLightFill);
					brush->SetSurroundColors(colors, &count);
					brush->SetFocusScales(0, 1);
					return brush;
				}
			case FillStyleGradientHorSideLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					
					Color colors[] = {clrHighLightFill};
					int count = 1;
					
					PathGradientBrush *brush = new PathGradientBrush(ObjectPath);
					brush->SetCenterColor(clrFillColor);
					brush->SetSurroundColors(colors, &count);
					brush->SetFocusScales(0, 1);
					return brush;
				}
			case FillStyleGradientVerMiddleLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					Color colors[] = {clrFillColor};
					int count = 1;
					PathGradientBrush* brush = new PathGradientBrush(ObjectPath);
					brush->SetCenterColor(clrHighLightFill);
					brush->SetSurroundColors(colors, &count);
					brush->SetFocusScales(1, 0);
					return brush;
				}
				
			case FillStyleGradientVerSideLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					Color colors[] = {clrHighLightFill};
					int count = 1;
					PathGradientBrush* brush = new PathGradientBrush(ObjectPath);
					brush->SetCenterColor(clrFillColor);
					brush->SetSurroundColors(colors, &count);
					brush->SetFocusScales(1, 0);
					return brush;
				}
			case FillStyleGradientHorLeftLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X, rcObject.Y), PointF(rcObject.X + rcObject.Width, rcObject.Y), clrHighLightFill, clrFillColor);
				}
			case FillStyleGradientHorRightLight:	
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X + rcObject.Width, rcObject.Y), PointF(rcObject.X, rcObject.Y), clrHighLightFill, clrFillColor);
				}
			case FillStyleGradientVerTopLight:			
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X, rcObject.Y), PointF(rcObject.X,  rcObject.Y + rcObject.Height), clrHighLightFill, clrFillColor);
				}
			case FillStyleGradientVerBottomLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X, rcObject.Y + rcObject.Height), PointF(rcObject.X, rcObject.Y), clrHighLightFill, clrFillColor);
				}

			case FillStyleGradientLeftTopLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X, rcObject.Y), PointF(rcObject.X + rcObject.Width, rcObject.Y + rcObject.Height), clrHighLightFill, clrFillColor);
				}
			case FillStyleGradientRightTopLight:		
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X + rcObject.Width, rcObject.Y), PointF(rcObject.X, rcObject.Y + rcObject.Height), clrHighLightFill, clrFillColor);
				}
				
			case FillStyleGradientRightBottomLight:	
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X + rcObject.Width, rcObject.Y + rcObject.Height), PointF(rcObject.X, rcObject.Y), clrHighLightFill, clrFillColor);
				}

			case FillStyleGradientLeftBottomLight:			
				{
					OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
					OLETRANSLATEGDIPLUSCOLOR(clrHighLightFill);
					RectF rcObject;
					ObjectPath->GetBounds(&rcObject, NULL, NULL);
					return new LinearGradientBrush(PointF(rcObject.X, rcObject.Y + rcObject.Height), PointF(rcObject.X + rcObject.Width, rcObject.Y), clrHighLightFill, clrFillColor);
				}
			case FillStyleImage:
			{
				if (m_BrushImageFileName.m_str == NULL)
					return NULL;
					
				Image brushImage(m_BrushImageFileName);
				return new TextureBrush(&brushImage);
			}
		}

		if (m_enumFillStyle >= FillStyleHatchHorizontal && m_enumFillStyle <= FillStyleHatchDiagonalCross)
		{
			OLETRANSLATEGDIPLUSCOLOR(clrFillColor);
			return new HatchBrush(HatchStyle(m_enumFillStyle - 2), clrFillColor, 0);
		}

		return NULL;
	}

	virtual void ICxDrawObject_InitNew(void)
	{
		m_enumScaleBase = ScaleBasePointCenter;	
		m_enumLineJoinStyle = LineJoinStyleRound;
		m_enumFillStyle = FillStyleSolid;
		GetDefaultFillStyle(m_enumFillStyle);									
		m_enumEdgeStyle = EdgeStyleSolid;					//实线边界
		m_fEdgeWidth = 1;											//边界线宽为1
		m_nHorFillPercent = 100;									//水平填充度为100%
		m_nVerFillPercent = 100;									//垂直填充度为100%
		m_enumVerFillDirection = VerticalDirectionFromBottom;			//垂直填充方向
		m_enumHorFillDirection = HorizontalDirectionFromLeft;			//水平填充方向
		
		
		m_enumMousePointer = MousePointerArrow;
		
		GetDefaultEdgeColor(m_clrEdgeColor);
		GetDefaultFillColor(m_clrFillColor);
		m_clrHighLightEdge = RGB(255, 255, 255);
		m_clrHighLightFill = RGB(255, 255, 255);

		m_pObjectPath = new Gdiplus::GraphicsPath;
		AddObjectToPath();
		ReSetRotateBasePoint();
	}

	HRESULT DoLoad(DWORD dwVer, LPSTREAM pStm)
	{
		LoadDynamicProperty(pStm);
		LoadPathData(pStm);

		return S_OK;
	}
	
	HRESULT DoSave(DWORD dwVer, LPSTREAM pStm)
	{
		SaveDynamicProperty(pStm);
		SavePathData(pStm);

		return S_OK;
	}

	#define ABSOLUTEVALUE(a) ((a) < 0 ? (-a) : (a))

	virtual HRESULT ICxDrawObject_MoveTo(RECTF rect, BOOL bUniformScale)
	{
		using namespace Gdiplus;

		if (ClientIsRunMode() && m_pObjectPathOrgin != NULL)
		{
			delete m_pObjectPath;
			m_pObjectPath = m_pObjectPathOrgin;
			m_pObjectPathOrgin = NULL;
		}

		RectF rcPos;
		m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
		float fScaleX;
		float fScaleY;

		if (rcPos.Width == 0)
			fScaleX = 1;
		else
			fScaleX = (rect.right - rect.left) / rcPos.Width;
		if (rcPos.Height == 0)
			fScaleY = 1;
		else
			fScaleY = (rect.bottom - rect.top) / rcPos.Height;

	//	if (ABSOLUTEVALUE(fScaleX) < 0.0002)
	//		fScaleX = 1 / rcPos.Width;
	//	if (ABSOLUTEVALUE(fScaleY) < 0.0002)
	//		fScaleY = 1 / rcPos.Height;

		if (fScaleX == 0)
			fScaleX = 1 / rcPos.Width;
		if (fScaleY == 0)
			fScaleY = 1 / rcPos.Height;

		// 如果是统一缩放尺寸，使用最大值做因子，减少失真损失, 线宽等比例改变
		if (bUniformScale)
		{
			if (ABSOLUTEVALUE(rcPos.Width) < ABSOLUTEVALUE(rcPos.Height))
				m_fEdgeWidth *= ABSOLUTEVALUE(fScaleY);
			else
				m_fEdgeWidth *= ABSOLUTEVALUE(fScaleX);
		}

		Matrix mtTansMatrix;
		mtTansMatrix.Translate(-rcPos.X, -rcPos.Y);
		mtTansMatrix.Scale(fScaleX, fScaleY, MatrixOrderAppend);
		mtTansMatrix.Translate(rect.left, rect.top, MatrixOrderAppend);
		mtTansMatrix.TransformPoints(&m_ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);

		if (ClientIsRunMode())
			ReSetObjectPath();
	
		return S_OK;
	}

	virtual HRESULT ICxDrawObject_DrawTrackerShape(HDC hdcDest, POINT* points, int ptcount)
	{
		using namespace Gdiplus;
		if (points == NULL && ptcount == 0)
		{
			PathData pathData;
			m_pObjectPath->GetPathData(&pathData);
			DrawPathDataShape(hdcDest, pathData);
		}
		return S_OK;
	}
	
	virtual HRESULT ICxDrawObject_DrawTrackerHandleShape(HDC hdcDest, long nHandle, POINT point)
	{
		using namespace Gdiplus;
	
		GraphicsPath* pObjectPath = m_pObjectPath->Clone();

		RectF rcPos;
		pObjectPath->GetBounds(&rcPos, NULL, NULL);

		RECTF rectF;
		rectF.left = rcPos.X;
		rectF.top = rcPos.Y;
		rectF.right = rcPos.X + rcPos.Width;
		rectF.bottom = rcPos.Y + rcPos.Height;
		DefaultMoveHandleTo(&rectF, nHandle, point);

		float fScaleX = (rectF.right - rectF.left) / rcPos.Width;
		float fScaleY = (rectF.bottom - rectF.top) / rcPos.Height;

		if (fScaleX == 0)
			fScaleX = 1 / rcPos.Width;
		if (fScaleY == 0)
			fScaleY = 1 / rcPos.Height;

		Matrix mtTansMatrix;
		mtTansMatrix.Translate(-rcPos.X, -rcPos.Y);
		mtTansMatrix.Scale(fScaleX, fScaleY, MatrixOrderAppend);
		mtTansMatrix.Translate(rectF.left, rectF.top, MatrixOrderAppend);
		pObjectPath->Transform(&mtTansMatrix);

		PathData pathData;
		pObjectPath->GetPathData(&pathData);
		DrawPathDataShape(hdcDest, pathData);

		delete pObjectPath;
		
		return S_OK;
	}

	virtual HRESULT ICxDrawObject_DrawRotateShape(HDC hdcDest, long lAngle)
	{
		using namespace Gdiplus;
	
		GraphicsPath* pObjectPath = m_pObjectPath->Clone();
		
		Matrix mtTansMatrix;
		mtTansMatrix.RotateAt((float)lAngle, m_ptRotateBase);
		pObjectPath->Transform(&mtTansMatrix);

		PathData pathData;
		pObjectPath->GetPathData(&pathData);
		DrawPathDataShape(hdcDest, pathData);

		delete pObjectPath;
		
		return S_OK;
	}

	virtual HRESULT ICxDrawObject_RotateAt(POINTF point, float fAngle)
	{
		using namespace Gdiplus;

		if (ClientIsRunMode() && m_pObjectPathOrgin != NULL)
		{
			delete m_pObjectPath;
			m_pObjectPath = m_pObjectPathOrgin;
			m_pObjectPathOrgin = NULL;
		}
	
		PointF ptRotateBase(point.x, point.y);
		Matrix mtTansMatrix;
		mtTansMatrix.RotateAt(fAngle, ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);
		mtTansMatrix.TransformPoints(&m_ptRotateBase);

		if (ClientIsRunMode())
			ReSetObjectPath();

		return S_OK;
	}

	virtual HRESULT ICxDrawObject_DrawRotateAtShape(HDC hdcDest, POINTF point, long lAngle)
	{
		using namespace Gdiplus;
	
		GraphicsPath* pObjectPath = m_pObjectPath->Clone();
		
		PointF ptRotateBase(point.x, point.y);
		Matrix mtTansMatrix;
		mtTansMatrix.RotateAt((float)lAngle, ptRotateBase);
		pObjectPath->Transform(&mtTansMatrix);

		PathData pathData;
		pObjectPath->GetPathData(&pathData);
		DrawPathDataShape(hdcDest, pathData);

		delete pObjectPath;
		
		return S_OK;
	}

	//此函数一般由组合对象调用
	virtual HRESULT ICxDrawObject_ScaleAt(POINTF point, float fScaleX, float fScaleY)
	{
		using namespace Gdiplus;

		//在运行模式下要变动对象原始路径
		if (ClientIsRunMode() && m_pObjectPathOrgin != NULL)
		{
			delete m_pObjectPath;
			m_pObjectPath = m_pObjectPathOrgin;
			m_pObjectPathOrgin = NULL;
		}

		RectF rcPos;
		m_pObjectPath->GetBounds(&rcPos, NULL, NULL);

		if (fScaleX == 0)
			fScaleX = 1 / rcPos.Width;
		if (fScaleY == 0)
			fScaleY = 1 / rcPos.Height;

		Matrix mtTansMatrix;
		mtTansMatrix.Translate(-point.x, -point.y);
		mtTansMatrix.Scale(fScaleX, fScaleY, MatrixOrderAppend);
		mtTansMatrix.Translate(point.x, point.y, MatrixOrderAppend);
		mtTansMatrix.TransformPoints(&m_ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);

		if (ClientIsRunMode())
			ReSetObjectPath();

		return S_OK;
	}

	virtual HRESULT ICxDrawObject_Rotate(long lAngle)
	{
		using namespace Gdiplus;
	
		Matrix mtTansMatrix;
		mtTansMatrix.RotateAt((float)lAngle, m_ptRotateBase);
		m_pObjectPath->Transform(&mtTansMatrix);

		return S_OK;
	}

	virtual HRESULT ICxDrawObject_HitTest(POINT point, BOOL* pbHitTest)
	{
		using namespace Gdiplus;

		if (/*m_enumLineJoinStyle != FillStyleTransparence && */m_pObjectPath->IsVisible(point.x, point.y))
		{
			*pbHitTest = TRUE;
			return S_OK;
		}

		DashStyle dsPen = (DashStyle)m_enumEdgeStyle;
		if (m_enumEdgeStyle == EdgeStylePipe)
			dsPen = Gdiplus::DashStyleSolid;
		float fPenWidth = m_fEdgeWidth;
		if (m_enumEdgeStyle == EdgeStyleTransparence)
			fPenWidth = 0;

		OLETRANSLATEGDIPLUSCOLOR(clrEdgeColor);
		Pen EdgePen(clrEdgeColor, fPenWidth);
		if (m_enumLineJoinStyle == LineJoinStyleRound)
		{
			EdgePen.SetLineJoin(LineJoinRound);
			EdgePen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		}
		else if (m_enumLineJoinStyle == LineJoinStyleSquare)
		{
			EdgePen.SetLineJoin(LineJoinMiter);
			EdgePen.SetLineCap(LineCapSquare, LineCapSquare, DashCapFlat);
		}
		EdgePen.SetDashStyle(dsPen);

		*pbHitTest = m_pObjectPath->IsOutlineVisible(point.x, point.y, &EdgePen);

		return S_OK;
	}

	virtual HRESULT ICxDrawObject_GetPositionRect(RECTF* pPositionRect)
	{
		using namespace Gdiplus;
		
		RectF rcPosition;
		if (ClientIsRunMode() && m_pObjectPathOrgin != NULL)
			m_pObjectPathOrgin->GetBounds(&rcPosition, NULL, NULL);
		else
			m_pObjectPath->GetBounds(&rcPosition, NULL, NULL);
		pPositionRect->left = rcPosition.X;
		pPositionRect->top = rcPosition.Y;
		if (rcPosition.Width == 0)
			rcPosition.Width = 1;
		if (rcPosition.Height == 0)
			rcPosition.Height = 1;
		pPositionRect->right = rcPosition.X + rcPosition.Width;
		pPositionRect->bottom = rcPosition.Y + rcPosition.Height;

		return S_OK;
	}
	virtual HRESULT ICxDrawObject_GetBoundingRect(RECT* pBoundingRect)
	{
		using namespace Gdiplus;

		DashStyle dsPen = (DashStyle)m_enumEdgeStyle;
		if (m_enumEdgeStyle == EdgeStylePipe)
			dsPen = Gdiplus::DashStyleSolid;
		float fPenWidth = m_fEdgeWidth;
		if (m_enumEdgeStyle == EdgeStyleTransparence)
			fPenWidth = 0;

		OLETRANSLATEGDIPLUSCOLOR(clrEdgeColor);
		Pen EdgePen(clrEdgeColor, fPenWidth);
		if (m_enumLineJoinStyle == LineJoinStyleRound)
		{
			EdgePen.SetLineJoin(LineJoinRound);
			EdgePen.SetLineCap(LineCapRound, LineCapRound, DashCapRound);
		}
		else if (m_enumLineJoinStyle == LineJoinStyleSquare)
		{
			EdgePen.SetLineJoin(LineJoinMiter);
			EdgePen.SetLineCap(LineCapSquare, LineCapSquare, DashCapFlat);
		}
		EdgePen.SetDashStyle(dsPen);

		Rect rcBound;
		m_pObjectPath->GetBounds(&rcBound, NULL, &EdgePen);
		pBoundingRect->left = rcBound.X;
		pBoundingRect->top = rcBound.Y;
		pBoundingRect->right = rcBound.X + rcBound.Width;
		pBoundingRect->bottom = rcBound.Y + rcBound.Height;
	
		return S_OK;
	}

//	HRESULT ICxDrawObject_Intersects(RECT rect, BOOL* pIntersects)
//	{
//		return S_OK;
//	}

	/////////////////////////////////////////////////////////////
	//功能: 规范化指定矩形
	//参数: pRect[in, out] 输入未规范化矩形, 输出规范化矩形
	/////////////////////////////////////////////////////////////
	void NormalizeRect(RECTF *pRect)
	{
		float fTemp;
		if (pRect->left > pRect->right)
		{
			fTemp = pRect->left;
			pRect->left = pRect->right;
			pRect->right = fTemp;
		}
		if (pRect->top > pRect->bottom)
		{
			fTemp = pRect->top;
			pRect->top = pRect->bottom;
			pRect->bottom = fTemp;
		}
	}
	
	//根据两个点求相对角度
	float CalculateAngle(POINTF ptOrigin, POINTF ptCal)
	{
		float dx = ptCal.x - ptOrigin.x;
		float dy = ptOrigin.y - ptCal.y;
		if (dx == 0)
		{
			if (dy < 0)
				return 270;
			return 90;
		}
		return float(fmod(atan2(dy, dx) * 57.29578, 360));
	}

	//根据两个点求相对角度
	int CalculateAngle(POINT ptOrigin, POINT ptCal)
	{
		int dx = ptCal.x - ptOrigin.x;
		int dy = ptOrigin.y - ptCal.y;
		if (dx == 0)
		{
			if (dy < 0)
				return 270;
			return 90;
		}
		return int(atan2(dy, dx) * 57.29578) % 360;
	}

	POINTF CalculateRotatePoint(POINTF ptOrgin, POINTF ptCal, float fAngle)
	{
		fAngle = -CalculateAngle(ptOrgin, ptCal) + fAngle;
		float RR = (float)sqrt((ptOrgin.x - ptCal.x) * (ptOrgin.x - ptCal.x) +
				(ptOrgin.y - ptCal.y) * (ptOrgin.y - ptCal.y));

		POINTF ptRet;
		ptRet.x =  ptOrgin.x + float(RR * cos(fAngle * 0.0174533));
		ptRet.y =  ptOrgin.y + float(RR * sin(fAngle * 0.0174533));

		return ptRet;
	}

	void RECTFToRECT(RECTF* pRectF, RECT* pRect)
	{
		pRect->left = ROUND(pRectF->left);
		pRect->top = ROUND(pRectF->top);
		pRect->right = ROUND(pRectF->right);
		pRect->bottom = ROUND(pRectF->bottom);
	}

	HRESULT DefaultMoveHandleTo(RECTF* pRect, long nHandle, POINT point)
	{
		pRect->left = (float)ROUND(pRect->left);
		pRect->right = (float)ROUND(pRect->right);
		pRect->top = (float)ROUND(pRect->top);
		pRect->bottom = (float)ROUND(pRect->bottom);

		switch (nHandle)
		{
			case 1:
				pRect->left = (float)point.x;
				pRect->top = (float)point.y;
				break;

			case 2:
				pRect->top = (float)point.y;
				break;

			case 3:
				pRect->right = (float)point.x;
				pRect->top = (float)point.y;
				break;

			case 4:
				pRect->right = (float)point.x;
				break;
	
			case 5:
				pRect->right = (float)point.x;
				pRect->bottom = (float)point.y;
				break;
	
			case 6:
				pRect->bottom = (float)point.y;
				break;

			case 7:
				pRect->left = (float)point.x;
				pRect->bottom = (float)point.y;
				break;

			case 8:
				pRect->left = (float)point.x;
				break;

			default:
				ATLASSERT(FALSE);
		}
		
	//	NormalizeRect(pRect);
	
		return S_OK;
	}
	HRESULT DefaultHandle(RECTF rectF, long nHandle, POINT* pHandlePoint/*, long *pHandleAsHandle*/)
	{
		int x, y, xCenter, yCenter;

		RECT rect;
		RECTFToRECT(&rectF, &rect);

		xCenter = (rect.left + rect.right) / 2;
		yCenter = (rect.top + rect.bottom) / 2;

		switch (nHandle)
		{
		default:
			x = y = 0;
			break;
			//ATLASSERT(FALSE);
		
		case 0:
			x = xCenter;
			y = yCenter;
			break;
		
		case 1:
			x = rect.left;
			y = rect.top;
			break;

		case 2:
			x = xCenter;
			y = rect.top;
			break;

		case 3:
			x = rect.right;
			y = rect.top;
			break;

		case 4:
			x = rect.right;
			y = yCenter;
			break;

		case 5:
			x = rect.right;
			y = rect.bottom;
			break;

		case 6:
			x = xCenter;
			y = rect.bottom;
			break;

		case 7:
			x = rect.left;
			y = rect.bottom;
			break;

		case 8:
			x = rect.left;
			y = yCenter;
			break;
		}

		pHandlePoint->x = x;
		pHandlePoint->y = y;
	
		return S_OK;
	}
	HRESULT DefaultHandleCursor(long nHandle, HCURSOR* pHandleCursor)
	{
		LPCTSTR id;
		switch (nHandle)
		{
		default:
			id = IDC_SIZEALL;

		case 1:
		case 5:
			id = IDC_SIZENWSE;
			break;

		case 2:
		case 6:
			id = IDC_SIZENS;
			break;

		case 3:
		case 7:
			id = IDC_SIZENESW;
			break;

		case 4:
		case 8:
			id = IDC_SIZEWE;
			break;
		}
		*pHandleCursor = LoadCursor(NULL, id);
		return S_OK;
	}
	inline HRESULT ICxDrawObject_SetDrawObjectSite(ICxDrawObjectSite* pDrawObjectSite)
	{
		ATLASSERT(pDrawObjectSite == NULL || m_spDrawObjectSite == NULL);
		m_spDrawObjectSite = pDrawObjectSite;
		m_spAmbientDispatch.Release();
		if (m_spDrawObjectSite != NULL)
		{
			m_spDrawObjectSite->QueryInterface(IID_IDispatch,
				(void**) &m_spAmbientDispatch.p);
		}
		return S_OK;
	}
	inline HRESULT ICxDrawObject_GetDrawObjectSite(ICxDrawObjectSite** ppDrawObjectSite)
	{
		*ppDrawObjectSite = m_spDrawObjectSite;
		if (*ppDrawObjectSite)
			(*ppDrawObjectSite)->AddRef();

		return S_OK;
	}
	HRESULT GetAmbientProperty(DISPID dispid, VARIANT& var)
	{
		HRESULT hRes = E_FAIL;
		if (m_spAmbientDispatch.p != NULL)
			hRes = m_spAmbientDispatch.GetProperty(dispid, &var);
		return hRes;
	}
	HRESULT GetAmbientBackColor(OLE_COLOR& Backolor)
	{
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(DISPID_AMBIENT_BACKCOLOR, var);
		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
		Backolor = var.lVal;
		return hRes;
	}
	HRESULT GetDefaultEdgeColor(OLE_COLOR& EdgeColor)
	{
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(CXPROPID_DEFAULT_EDGECOLOR, var);
		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
		EdgeColor = var.lVal;
		return hRes;
	}
	HRESULT GetDefaultFillColor(OLE_COLOR& ForegroundColor)
	{
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(CXPROPID_DEFAULT_FILLCOLOR, var);
		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
		ForegroundColor = var.lVal;
		return hRes;
	}
	HRESULT GetDefaultFillStyle(enumFillStyle& nFillStyle)
	{
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(CXPROPID_DEFAULT_FILLSTYLE, var);
		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
		if (SUCCEEDED(hRes))
			nFillStyle = (enumFillStyle)var.lVal;
		return hRes;
	}
	HRESULT GetDefaultFontColor(OLE_COLOR& FontColor)
	{
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(CXPROPID_DEFAULT_FONTCOLOR, var);
		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
		FontColor = var.lVal;
		return hRes;
	}
//	HRESULT GetDefaultLayer(long& lLayer)
//	{
//		CComVariant var;
//		HRESULT hRes = GetAmbientProperty(CXPROPID_DEFAULT_LAYER, var);
//		ATLASSERT(var.vt == VT_I4 || var.vt == VT_UI4 || FAILED(hRes));
//		lLayer = var.lVal;
//
//		return hRes;
//	}
	//缺省字体使用环境字体
	HRESULT GetDefaultFont(IFont** ppFont)
	{
		// caller MUST Release the font!
		if (ppFont == NULL)
			return E_POINTER;
		*ppFont = NULL;
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(DISPID_AMBIENT_FONT, var);
		ATLASSERT((var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH) || FAILED(hRes));
		if (SUCCEEDED(hRes) && var.pdispVal)
		{
			if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
				hRes = var.pdispVal->QueryInterface(IID_IFont, (void**)ppFont);
			else
				hRes = DISP_E_BADVARTYPE;
		}
		return hRes;
	}

	HRESULT GetDefaultFontDisp(IFontDisp** ppFont)
	{
		// caller MUST Release the font!
		if (ppFont == NULL)
			return E_POINTER;
		*ppFont = NULL;
		CComVariant var;
		HRESULT hRes = GetAmbientProperty(DISPID_AMBIENT_FONT, var);
		ATLASSERT((var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH) || FAILED(hRes));
		if (SUCCEEDED(hRes) && var.pdispVal)
		{
			if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
				hRes = var.pdispVal->QueryInterface(IID_IFontDisp, (void**)ppFont);
			else
				hRes = DISP_E_BADVARTYPE;
		}
		return hRes;
	}

	//动态属性
	HRESULT AddDynamic(DISPID dispid, IUnknown* pUnknown)
	{
		if (HasDynamic(dispid))
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
	HRESULT RemoveDynamic(DISPID dispid)
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
	HRESULT GetDynamic(DISPID dispid, IUnknown **ppUnknown)
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
	BOOL HasDynamic(DISPID dispid)
	{
		if (dispid == DISPID_UNKNOWN)
		{
			if (m_arLinkInfo.GetSize() >= 1)
				return TRUE;
			else 
				return FALSE;
		}
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			if (m_arLinkInfo[i].dispid == dispid)
			{
				return TRUE;
			}
		}

		return FALSE;
	}
	virtual int GetDynamicState(DISPID dispid)
	{
		int nState = 0;
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

		return nState;
	}
	HRESULT LoadDynamicProperty(IStream *pStream)
	{
		HRESULT hr;
		int nSize;
		pStream->Read(&nSize, sizeof(nSize), NULL);
		for(int nIndex = 0; nIndex < nSize; nIndex++)
		{
			DISPID dispid;
			pStream->Read(&dispid, sizeof(DISPID), NULL);

			CComPtr<IUnknown> spObject;
			hr = OleLoadFromStream(pStream, IID_IUnknown, (void**)&spObject);
			if (FAILED(hr))
				return hr;
			AddDynamic(dispid, spObject);
		}

		return S_OK;
	}
	HRESULT SaveDynamicProperty(IStream *pStream)
	{
		HRESULT hr;

		int nSize = m_arLinkInfo.GetSize();;
		pStream->Write(&nSize, sizeof(nSize), NULL);
		for(int nIndex = 0; nIndex < nSize; nIndex++)
		{
			pStream->Write(&m_arLinkInfo[nIndex].dispid, sizeof(DISPID), NULL);
			ICxDynamicObject* pObject = m_arLinkInfo[nIndex].pDynamicObject;
			if (pObject != NULL)
			{
				CComPtr<IPersistStream> spStream;			
				hr = pObject->QueryInterface(IID_IPersistStream, (void**)&spStream);
				if (FAILED(hr))
					return hr;
				if (spStream != NULL)
				{	
					hr = OleSaveToStream(spStream, pStream);
					if (FAILED(hr))
						return hr;
				}
			}
		}

		return S_OK;
	}
	HRESULT	SetDynamicPropertyEngine(ICxScriptEngineSite* pScriptSite)
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			HRESULT hr = m_arLinkInfo[i].pDynamicObject->SetScriptEngineSite(pScriptSite);
			if (FAILED(hr))
				return hr;
		}

		return S_OK;
	}
	HRESULT	InitRunDynamicProperty(ICxScriptEngineSite* pScriptSite, IUnknown* pUnk)
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			CComQIPtr<IDispatch> spDisp(pUnk);
			HRESULT hr = m_arLinkInfo[i].pDynamicObject->InitRun(pScriptSite, spDisp, m_arLinkInfo[i].dispid);
			if (FAILED(hr))
				return hr;
		}

		return S_OK;
	}
	inline HRESULT FakeParseDynamicProperty()
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			m_arLinkInfo[i].pDynamicObject->FakeParseConnection();
		}
		
		return S_OK;
	}
	inline HRESULT ParseDynamicProperty()
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			m_arLinkInfo[i].pDynamicObject->ParseConnection();
		}

		return S_OK;
	}
	inline ICxDynamicObject* GetMouseInputDynamic(DISPID dispid)
	{
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			if (m_arLinkInfo[i].dispid == dispid && 
					m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
				return m_arLinkInfo[i].pDynamicObject;
		}

		return NULL;
	}
	inline BOOL HasMouseInputDynamic()
	{
		DISPID dispid;
		for (int i = 0; i < m_arLinkInfo.GetSize(); i++)
		{
			dispid = m_arLinkInfo[i].dispid;
			if ((dispid == CXPROPID_HorPosition || dispid == CXPROPID_VerPosition || dispid == CXPROPID_RotateAngle) &&
					m_arLinkInfo[i].pDynamicObject->IsEnableMouseInput() == S_OK)
				return TRUE;
		}

		if (m_spDrawObjectSite->OnMouseInput(L"HorizontalPosition", FALSE) == S_OK ||
			m_spDrawObjectSite->OnMouseInput(L"VerticalPosition", FALSE) == S_OK ||
			m_spDrawObjectSite->OnMouseInput(L"RotateAngle", FALSE) == S_OK)
			return TRUE;
			

		return FALSE;
	}
	inline BOOL ProcessMouseInput(UINT msg, WPARAM wParam, LPARAM lParam, IUnknown* pUnk)
	{
		static POINTF ptDragOffset; //相对于对象左上角偏移
		static float fAngleOrgin; //原始旋转角度
		static float fAngleStart; //开始旋转角度
		static POINTF ptOrgin;	//旋转原点
		static BOOL bMouseInput = FALSE;
	
		switch (msg)
		{
		case WM_MOUSEMOVE:
			{
				if (!bMouseInput)
					break;

				POINTF point;
				point.x = (float)GET_X_LPARAM(lParam);
				point.y = (float)GET_Y_LPARAM(lParam);
				RECTF rcPosition;
				ICxDrawObject_GetPositionRect(&rcPosition);
				
				ICxDynamicObject* pDyn = GetMouseInputDynamic(CXPROPID_HorPosition);
				BOOL bClientProcess = m_spDrawObjectSite->OnMouseInput(L"HorizontalPosition", FALSE) == S_OK;
			//	CComVariant varValue;
				BOOL bHandled = FALSE;
				if (pDyn || bClientProcess)
				{
					rcPosition.right = point.x - ptDragOffset.x + rcPosition.right - rcPosition.left;
					rcPosition.left = point.x - ptDragOffset.x;
					FireViewChange();
					ICxDrawObject_MoveTo(rcPosition, FALSE);
					if (pDyn)
						pDyn->InvertParseConnection();
					if (bClientProcess)
					{
					//	CComDispatchDriver dd(pUnk);
					//	dd.GetProperty(CXPROPID_HorPosition, &varValue);
						m_spDrawObjectSite->OnMouseInput(L"HorizontalPosition", TRUE);
					}
					bHandled = TRUE;
				}
				pDyn = GetMouseInputDynamic(CXPROPID_VerPosition);
				bClientProcess = m_spDrawObjectSite->OnMouseInput(L"VerticalPosition", FALSE) == S_OK;
				if (pDyn || bClientProcess)
				{
					rcPosition.bottom = point.y - ptDragOffset.y + rcPosition.bottom - rcPosition.top;
					rcPosition.top = point.y - ptDragOffset.y;
					if (!bHandled)
						FireViewChange();
					ICxDrawObject_MoveTo(rcPosition, FALSE);
					if (pDyn)
						pDyn->InvertParseConnection();
					if (bClientProcess)
					{
					//	CComDispatchDriver dd(pUnk);
					//	dd.GetProperty(CXPROPID_VerPosition, &varValue);
						m_spDrawObjectSite->OnMouseInput(L"VerticalPosition", TRUE);
					}
					bHandled = TRUE;
				}
				if (bHandled)
				{
					FireViewChange();
					return TRUE;
				}
						
				pDyn = GetMouseInputDynamic(CXPROPID_RotateAngle);
				bClientProcess = m_spDrawObjectSite->OnMouseInput(L"RotateAngle", FALSE) == S_OK;
				if (pDyn || bClientProcess)
				{
					float fRotateAngle = CalculateAngle(ptOrgin, point);
					fRotateAngle = fAngleStart - fRotateAngle + fAngleOrgin;
					FireViewChange();
					m_fRotateAngle = float(long(fRotateAngle) % 360);
					if (m_fRotateAngle < 0)
						m_fRotateAngle += 360;
					ATLASSERT(0 <= m_fRotateAngle && 360 >= m_fRotateAngle);
					if (m_fRotateAngle > 180)
						m_fRotateAngle = m_fRotateAngle - 360;
					ReSetObjectPath();
					FireViewChange();
					if (pDyn)
						pDyn->InvertParseConnection();
					if (bClientProcess)
					{
					//	CComDispatchDriver dd(pUnk);
					//	dd.GetProperty(CXPROPID_RotateAngle, &varValue);
						m_spDrawObjectSite->OnMouseInput(L"RotateAngle", TRUE);
					}
					
					return TRUE;
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				if (!HasMouseInputDynamic())
					return FALSE;

				POINTF point;
				point.x = (float)GET_X_LPARAM(lParam);
				point.y = (float)GET_Y_LPARAM(lParam);
			
				RECTF rcPosition;
				ICxDrawObject_GetPositionRect(&rcPosition);
				ptDragOffset.x = point.x - rcPosition.left;
				ptDragOffset.y = point.y - rcPosition.top;

				fAngleOrgin = m_fRotateAngle;
				ptOrgin.x = m_ptRotateBase.X;
				ptOrgin.y = m_ptRotateBase.Y;
				fAngleStart = CalculateAngle(ptOrgin, point);

				bMouseInput = TRUE;
				return TRUE;
			}
			break;
		case WM_LBUTTONUP:
			{
				bMouseInput = FALSE;
			}
		}
		
		return FALSE;
	}
//	HRESULT ICxPerPropertyEdit_OnEnumPropertySelect(DISPID dispID, long lSelect, HWND hParentWnd)
//	{
//		if (CXPROPID_TransBasePoint == dispID && lSelect == CustomPoint)
//		{	
//			m_spDrawObjectSite->GetScaleBasePoint(&m_ptTransBasePt);
//		}
//		return S_OK;
//	}

	inline void SetStandardCursor()
	{
		switch (m_enumMousePointer)
		{
	//	case fmMousePointerDefault:
		case MousePointerArrow:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		case MousePointerHand:
			if (m_hHandCursor == NULL)
			{
				// create or load a cursor
#if (WINVER >= 0x0500)
				m_hHandCursor = ::LoadCursor(NULL, IDC_HAND);
#else
				m_hHandCursor = ::CreateCursor(_Module.GetModuleInstance(), _Hand_CursorData.xHotSpot, _Hand_CursorData.yHotSpot, _Hand_CursorData.cxWidth, _Hand_CursorData.cyHeight, _Hand_CursorData.arrANDPlane, _Hand_CursorData.arrXORPlane);
#endif //!(WINVER >= 0x0500)
				ATLASSERT(m_hHandCursor != NULL);
			}
		
			SetCursor(m_hHandCursor);
			break;
		case MousePointerCross:
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			break;
		case MousePointerIBeam:
			SetCursor(LoadCursor(NULL, IDC_IBEAM));
			break;
		case MousePointerSizeNESW:
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			break;
		case MousePointerSizeNS:
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			break;
		case MousePointerSizeNWSE:
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			break;
		case MousePointerSizeWE:
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			break;
		case MousePointerUpArrow:
			SetCursor(LoadCursor(NULL, IDC_UPARROW));
			break;
		case MousePointerHourglass:
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			break;
		case MousePointerSizeAll:
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			break;
		}
	}

// Attributes
public:	
	typedef struct tagLINKINFO
	{
		DISPID dispid;
		ICxDynamicObject* pDynamicObject;
	} LINKINFO;
	CSimpleArray<LINKINFO> m_arLinkInfo;	//动态属性集

	CComPtr<ICxDrawObjectAdviseSink> m_spDrawObjectAdviseSink;
	CComPtr<ICxDrawObjectSite> m_spDrawObjectSite;
	CComDispatchDriver m_spAmbientDispatch;
	unsigned m_bRequiresSave:1;
	static HCURSOR m_hHandCursor;
//	CComBSTR m_bstrName;
};

__declspec(selectany) HCURSOR CCxDrawObjectBase::m_hHandCursor = NULL;

///////////////////////////////////////////////////////////
template <class T>
class ATL_NO_VTABLE ICxDrawObjectImpl : public ICxDrawObject
{
public:
	unsigned m_iButtonState : 3;        // Which buttons are down?
	unsigned m_iDblClkState : 3;        // Which buttons involved in dbl click?
	
	ICxDrawObjectImpl()
	{
		m_iButtonState = 0;
		m_iDblClkState = 0;
	}
//	m_hHandleCursor = NULL;
	STDMETHOD(SetDrawObjectSite)(ICxDrawObjectSite* pDrawObjectSite)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_SetDrawObjectSite(pDrawObjectSite);
	
	}
	STDMETHOD(GetDrawObjectSite)(ICxDrawObjectSite** ppDrawObjectSite)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_GetDrawObjectSite(ppDrawObjectSite);
	
	}
	STDMETHOD(Draw)(HDC hdcDest)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_Draw(hdcDest);
	}
	STDMETHOD(Print)(HDC hdcDest)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_Print(hdcDest);
	}
	STDMETHOD(GetPositionRect)(RECTF* pPositionRect)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_GetPositionRect(pPositionRect);
	}
	STDMETHOD(GetBoundingRect)(RECT* pBoundingRect)
	{
	//	T* pT = static_cast<T*>(this);
	//	using namespace Gdiplus;
	//	Rect rcBound;
	//	pT->m_pObjectPath->GetBounds(&rcBound, NULL, NULL);
	//	pBoundingRect->left = rcBound.X;
	//	pBoundingRect->top = rcBound.Y;
	//	pBoundingRect->right = rcBound.X + rcBound.Width;
	//	pBoundingRect->bottom = rcBound.Y + rcBound.Height;
	//	int nOffset = pT->m_nEdgeWidth;
	//	InflateRect(pBoundingRect, nOffset, nOffset);
	//	return S_OK;

		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_GetBoundingRect(pBoundingRect);
	}
	STDMETHOD(SetAdvise)(ICxDrawObjectAdviseSink * pAdvSink)
	{
		T* pT = static_cast<T*>(this);
		pT->m_spDrawObjectAdviseSink = pAdvSink;
	
		return S_OK;
	}
	STDMETHOD(GetHandleCount)(long* pHandleCount)
	{
		*pHandleCount = -8;
		return S_OK;
	}
	STDMETHOD(GetHandle)(long nHandle, POINT* pHandlePoint)
	{
		ATLASSERT(nHandle <= -1 && nHandle >= -8);
		T* pT = static_cast<T*>(this);
		RECTF rcRect;
		GetPositionRect(&rcRect);
		return pT->DefaultHandle(rcRect, -nHandle, pHandlePoint);
	}
	STDMETHOD(GetHandleCursor)(long nHandle, HCURSOR* pHandleCursor)
	{
		ATLASSERT(nHandle <= -1 && nHandle >= -8);
		T* pT = static_cast<T*>(this);
		return pT->DefaultHandleCursor(-nHandle, pHandleCursor);
	}
	STDMETHOD(MoveHandleTo)(long nHandle, POINT point)
	{
		ATLASSERT(nHandle <= -1 && nHandle >= -8);
		T* pT = static_cast<T*>(this);
		RECTF rectF;
		GetPositionRect(&rectF);
		pT->DefaultMoveHandleTo(&rectF, -nHandle, point);
		MoveTo(rectF, FALSE);

		return S_OK;
	}
	STDMETHOD(MoveHandlesTo)(POINT* pPoints, int nCount)
	{
		return S_OK;
	}
	STDMETHOD(MoveTo)(RECTF rect, BOOL bUniformScale)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_MoveTo(rect, bUniformScale);
	}
	STDMETHOD(Mirror)(MIRRORMODE mrrormode)
	{
		return S_OK;
	}
	STDMETHOD(Rotate)(long lAngle)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_Rotate(lAngle);
	}
	STDMETHOD(HitTest)(POINT point, BOOL* pHitTest)
	{
	//	RECT rc;
	//	rc.left = point.x;
	//	rc.right = point.x + 1;
	//	rc.top = point.y;
	//	rc.bottom = point.y + 1;
	//	return Intersects(rc, pHitTest);

		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_HitTest(point, pHitTest);
	}
	STDMETHOD(Intersects)(RECT rect, BOOL* pIntersects)
	{
		T* pT = static_cast<T*>(this);
		using namespace Gdiplus;
	
		Region ObjectRegion(pT->m_pObjectPath);
		Rect rc(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
		*pIntersects = ObjectRegion.IsVisible(rc);
	
		return S_OK;
	//	T* pT = static_cast<T*>(this);
	//	return pT->ICxDrawObject_Intersects(rect, pIntersects);
	}
	STDMETHOD(GetClassName)(BSTR* pbstrClassName)
	{
		*pbstrClassName = OLESTR("");
		return S_FALSE;
	}
	inline ICxEventObj* GetEventObject()
	{
		T* pT = static_cast<T*>(this);
		ICxEventObj* pEvent;
		pT->m_spDrawObjectSite->GetEventObject(&pEvent);
		return pEvent;
	}
	inline void SetCapture(BOOL bCapture)
	{
		T* pT = static_cast<T*>(this);
		pT->m_spDrawObjectSite->SetCapture(bCapture);
	}
	inline BOOL GetCapture()
	{
		T* pT = static_cast<T*>(this);
		return pT->m_spDrawObjectSite->GetCapture() == S_OK;
	}
	inline void SetFocus(BOOL bFocus)
	{
		T* pT = static_cast<T*>(this);
		pT->m_spDrawObjectSite->SetFocus(bFocus);
	}
	inline BOOL GetFocus()
	{
		T* pT = static_cast<T*>(this);
		return pT->m_spDrawObjectSite->GetFocus() == S_OK;
	}
	void OnButtonDown(SHORT iButton, POINT point)
	{
		T* pT = static_cast<T*>(this);

		if (m_iButtonState == 0)
			SetCapture(TRUE);

		m_iButtonState |= iButton;

		pT->Fire_Event(CXEVENID_MOUSEDOWN);
		//pT->Fire_MouseDown(iButton, _GetShiftState(), (float)point.x, (float)point.y);

		m_iDblClkState &= ~iButton;
	}
	void OnButtonUp(SHORT iButton, POINT point)
	{
		T* pT = static_cast<T*>(this);

		if (m_iButtonState != 0)
		{
			m_iButtonState &= ~iButton;

			if (m_iButtonState == 0)
				SetCapture(FALSE);

			pT->Fire_Event(CXEVENID_MOUSEUP);
			//pT->Fire_MouseUp(iButton, _GetShiftState(), (float)point.x, (float)point.y);

			if (!(m_iDblClkState & iButton) && (iButton & LEFT_BUTTON))
			{
				BOOL bHit;
				HitTest(point, &bHit);
				if (bHit)
					pT->Fire_Event(CXEVENID_CLICK);
					//pT->Fire_Click();
			}

			m_iDblClkState &= ~iButton;
		}
	}
	void OnButtonDblClk(SHORT iButton, POINT)
	{
		T* pT = static_cast<T*>(this);

		SetCapture(TRUE);
		m_iButtonState |= iButton;

		if (iButton & LEFT_BUTTON)
			pT->Fire_Event(CXEVENID_DBLCLICK);

		m_iDblClkState |= iButton;
	}
	STDMETHOD(OnWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, BOOL bSubObj, LRESULT *plResult)
	{
		T* pT = static_cast<T*>(this);

		switch (msg)
		{
		case WM_MOUSEMOVE:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				BOOL bHit;
				HitTest(point, &bHit);
				if (bHit)
					pT->m_spDrawObjectSite->TrackMouseLeave();

				pT->Fire_Event(CXEVENID_MOUSEMOVE);
				//pT->Fire_MouseMove((short)m_iButtonState, _GetShiftState(), (float)point.x, (float)point.y);
				pT->SetStandardCursor();
			}
			break;
		case WM_LBUTTONDOWN:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				SetFocus(TRUE);
				OnButtonDown(LEFT_BUTTON, point);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonDown(MIDDLE_BUTTON, point);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonDown(RIGHT_BUTTON, point);
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonUp(LEFT_BUTTON, point);
			}
          	break;
		case WM_MBUTTONUP:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonUp(MIDDLE_BUTTON, point);
			}
			break;
		case WM_RBUTTONUP:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonUp(RIGHT_BUTTON, point);
			}
         	break;
		case WM_LBUTTONDBLCLK:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonDblClk(LEFT_BUTTON, point);
			}
			break;
		case WM_MBUTTONDBLCLK:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonDblClk(MIDDLE_BUTTON, point);
			}
			break;
		case WM_RBUTTONDBLCLK:
			{
				POINT point;
				point.x = GET_X_LPARAM(lParam);
				point.y = GET_Y_LPARAM(lParam);
				OnButtonDblClk(RIGHT_BUTTON, point);
			}
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			{
				pT->Fire_Event(CXEVENID_KEYDOWN);

			//	SHORT nCharShort = (USHORT)wParam;
			//	USHORT nShiftState = _GetShiftState();
			//	pT->Fire_KeyDown((SHORT *)&nCharShort, nShiftState);
			//	if (nCharShort != 0)			//有父窗体继续处理
			//		return S_FALSE;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			{
				pT->Fire_Event(CXEVENID_KEYUP);

			//	SHORT nCharShort = (USHORT)wParam;
			//	USHORT nShiftState = _GetShiftState();
			//	pT->Fire_KeyUp((SHORT *)&nCharShort, nShiftState);
			//	if (nCharShort != 0)			//有父窗体继续处理
			//		return S_FALSE;
			}
			break;
		case WM_CHAR:
			{
				pT->Fire_Event(CXEVENID_KEYPRESS); //没有处理双字节的情形，参看CRunView::OnChar
			}
		default:
			return S_FALSE;
		}

		BOOL bMouseInput = pT->ProcessMouseInput(msg, wParam, lParam, pT->GetUnknown());
		//如果是子对象，并且没有处理鼠标输入，返回S_FALSE;
		return bSubObj && !bMouseInput ? S_FALSE : S_OK;
		

	

/*		T* pT = static_cast<T*>(this);
		static POINTF ptDrag;
		static float fAngleOrgin;
		static float fAngleStart;
		float fRotateAngle;	//开始旋转时鼠标点角度
		POINTF ptTemp;
		static POINTF ptOrgin;
		RECTF rcTemp;

		if (bSubObj && pT->m_enumMouseInputMode == 0)
			return S_FALSE;
		
		BOOL bShift = (GetKeyState(VK_SHIFT) < 0);
		BOOL bCtrl  = (GetKeyState(VK_CONTROL) < 0);
		BOOL bAlt   = (GetKeyState(VK_MENU) < 0);
		long Shift = (bShift + (bCtrl << 1) + (bAlt << 2));
		
		BOOL bLeftButton = wParam & MK_LBUTTON;
		BOOL bRightButton = wParam & MK_RBUTTON;
		BOOL bMiddleButton = wParam & MK_MBUTTON;
		int Button = (bLeftButton + (bRightButton << 1) + (bMiddleButton << 2));

		switch (msg)
		{
		case WM_LBUTTONUP:
            pT->Fire_Click();
			if (pT->m_spDrawObjectSite->GetCapture() == S_OK)
				pT->m_spDrawObjectSite->SetCapture(FALSE);
			break;
		case WM_LBUTTONDBLCLK:
			pT->Fire_DblClick();
			break;
		case WM_KEYDOWN:
			pT->Fire_KeyDown(wParam , Shift,  *plResult);
			break;
		case WM_KEYUP:
			pT->Fire_KeyUp(wParam, Shift, *plResult);
			break;
		case WM_LBUTTONDOWN:
			pT->Fire_MouseDown(Button, Shift, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			if (pT->m_enumMouseInputMode != 0)
			{
				pT->m_spDrawObjectSite->SetCapture(TRUE);
				pT->GetPositionRect(&rcTemp);
				ptDrag.x = GET_X_LPARAM(lParam) - rcTemp.left;
				ptDrag.y = GET_Y_LPARAM(lParam) - rcTemp.top;
				ptTemp.x = float(GET_X_LPARAM(lParam));
				ptTemp.y = float(GET_Y_LPARAM(lParam));
				fAngleOrgin = float(pT->m_nRotateAngle);
				ptOrgin.x = pT->m_ptRotateBase.X;
				ptOrgin.y = pT->m_ptRotateBase.Y;
				fAngleStart = pT->CalculateAngle(ptOrgin, ptTemp);
			}
			break;
		case WM_MOUSEMOVE:
			ptTemp.x = float(GET_X_LPARAM(lParam));
			ptTemp.y = float(GET_Y_LPARAM(lParam));
			pT->GetPositionRect(&rcTemp);
			pT->Fire_MouseMove(Button, Shift, ptTemp.x, ptTemp.y);
			if (pT->m_spDrawObjectSite->GetCapture() != S_OK)
				return S_OK;
			switch (pT->m_enumMouseInputMode)
			{
			case HorizonalInput:
				rcTemp.right = ptTemp.x - ptDrag.x + rcTemp.right - rcTemp.left;
				rcTemp.left = ptTemp.x - ptDrag.x;
				
			//	OffsetRect(&rcTemp, ptTemp.x - ptDrag.x, 0);
				break;
			case VerticalInput:
				rcTemp.bottom = ptTemp.y - ptDrag.y + rcTemp.bottom - rcTemp.top;
				rcTemp.top = ptTemp.y - ptDrag.y;
				//OffsetRect(&rcTemp, 0, ptTemp.y - ptDrag.y);
				break;
			case HorAndVerInput:
				rcTemp.right = ptTemp.x - ptDrag.x + rcTemp.right - rcTemp.left;
				rcTemp.left = ptTemp.x - ptDrag.x;
				rcTemp.bottom = ptTemp.y - ptDrag.y + rcTemp.bottom - rcTemp.top;
				rcTemp.top = ptTemp.y - ptDrag.y;
			//	OffsetRect(&rcTemp, ptTemp.x - ptDrag.x, ptTemp.y - ptDrag.y);
				break;
			case RotateInput:
				fRotateAngle = pT->CalculateAngle(ptOrgin, ptTemp);
				fRotateAngle = fAngleStart - fRotateAngle + fAngleOrgin;
				if (fRotateAngle < 0)
					fRotateAngle += 360;
				pT->FireViewChange();
				pT->m_nRotateAngle = (long)fRotateAngle;
				pT->ReSetObjectPath();
				pT->FireViewChange();
				return S_OK;
				break;
			default:
				return S_OK;
			}
			pT->FireViewChange();
			pT->MoveTo(rcTemp);
			pT->FireViewChange();
			break;
		default:
			return S_FALSE;
		}

		return S_OK;
*/
	}

	// IPersist
	STDMETHOD(GetClassID)(CLSID *pClassID)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::GetClassID\n"));
		*pClassID = T::GetObjectCLSID();
		return S_OK;
	}

	// IPersistStream
	STDMETHOD(IsDirty)()
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::IsDirty\n"));
		T* pT = static_cast<T*>(this);
		return (pT->m_bRequiresSave) ? S_OK : S_FALSE;
	}
	STDMETHOD(Load)(DWORD dwkey, LPSTREAM pStm)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::Load\n"));
		T* pT = static_cast<T*>(this);
		
		DWORD dwVer;
		pStm->Read(&dwVer, sizeof(DWORD), NULL);

		if (dwVer != _SUPERCX_OLDVER)
			return pT->DoLoad(dwVer, pStm);

		return LoadOldVer(pStm);

//		pT->LoadDynamicProperty(pStm);
//		pT->LoadPathData(pStm);
//		return pT->IPersistStream_Load(pStm, T::GetCxPropMap());
	}
	STDMETHOD(Save)(LPSTREAM pStm, BOOL fClearDirty)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::Load\n"));
		T* pT = static_cast<T*>(this);

		DWORD dwVer = _SUPERCX_VER;
		pStm->Write(&dwVer, sizeof(DWORD), NULL);
		
		HRESULT hr = pT->DoSave(dwVer, pStm);
		if (SUCCEEDED(hr))
			pT->m_bRequiresSave = FALSE;
		return hr;

		return SaveOldVer(pStm, fClearDirty);

//		pT->SaveDynamicProperty(pStm);
//		pT->SavePathData(pStm);
//		return pT->IPersistStream_Save(pStm, fClearDirty, T::GetCxPropMap());
	}
	STDMETHOD(GetSizeMax)(ULARGE_INTEGER FAR* /* pcbSize */)
	{
		ATLTRACENOTIMPL(_T("IPersistStreamImpl::GetSizeMax"));
	}

	// IPersistStreamInit
	STDMETHOD(InitNew)()
	{
		T* pT = static_cast<T*>(this);
		pT->ICxDrawObject_InitNew();
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamInitImpl::InitNew\n"));
		return S_OK;
//		if (FAILED(hr))
//			return hr;
//		return pT->GetDynamicProperty()->put_Object(pT->GetUnknown());
	}
	STDMETHOD(GetPages)(CAUUID* pPages)
	{
		CXPAGEMAP_ENTRY *pMap = T::GetCxPageMap();
		return GetPagesHelper(pPages, pMap);
	}
	STDMETHOD(DrawTrackerShape)(HDC hdcDest, POINT* pPoints, int nCount)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_DrawTrackerShape(hdcDest, pPoints, nCount);
	}
	STDMETHOD(DrawTrackerHandleShape)(HDC hdcDest, long nHandle, POINT point)
	{
		ATLASSERT(nHandle <= -1 && nHandle >= -8);
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_DrawTrackerHandleShape(hdcDest, -nHandle, point);
	}
	STDMETHOD(InitRun)(BOOL* pbHasDynamic, ICxScriptEngineSite* pScriptSite)
	{
		T* pT = static_cast<T*>(this);
		if (pT->HasDynamic(DISPID_UNKNOWN))
		{
			pT->InitRunDynamicProperty(pScriptSite, pT->GetUnknown());
			*pbHasDynamic = TRUE;
		}
		else
		{
			*pbHasDynamic = FALSE;
		}

		return S_OK;
	}
	STDMETHOD(FakeParseDynamic)()
	{
		T* pT = static_cast<T*>(this);
		pT->FakeParseDynamicProperty();
		return S_OK;
	}
	STDMETHOD(ParseDynamic)()
	{
		T* pT = static_cast<T*>(this);
		pT->ParseDynamicProperty();
		return S_OK;
	}

	STDMETHOD(RotateAt)(POINTF point, float fAngle)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_RotateAt(point, fAngle);
	}

	STDMETHOD(DrawRotateShape)(HDC hdcDest, long lAngle)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_DrawRotateShape(hdcDest, lAngle);
	}
	STDMETHOD(SetRotateBasePoint)(POINT point)
	{
		T* pT = static_cast<T*>(this);
		pT->m_ptRotateBase.X = (float)point.x;
		pT->m_ptRotateBase.Y = (float)point.y;
		return S_OK;
	}

	STDMETHOD(GetRotateBasePoint)(POINT* pPoint)
	{
		T* pT = static_cast<T*>(this);
		pPoint->x = ROUND(pT->m_ptRotateBase.X);
		pPoint->y = ROUND(pT->m_ptRotateBase.Y);
		return S_OK;
	}
	STDMETHOD(ScaleAt)(POINTF point, float fHorScale, float fVerScale)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_ScaleAt(point, fHorScale, fVerScale);
	}

	STDMETHOD(DrawRotateAtShape)(HDC hdcDest, POINTF point, long lAngle)
	{
		T* pT = static_cast<T*>(this);
		return pT->ICxDrawObject_DrawRotateAtShape(hdcDest, point, lAngle);
	}

	STDMETHOD(OnRunModeChange)()
	{
		T* pT = static_cast<T*>(this);

		if (!pT->ClientIsRunMode() && pT->m_pObjectPathOrgin != NULL)
		{
			delete pT->m_pObjectPathOrgin;
			pT->m_pObjectPathOrgin = NULL;
		}

		return S_OK;;
	}
	STDMETHOD(OnTimer)()
	{
		return S_OK;;
	}
	STDMETHOD(OnMouseLeave)()
	{
		T* pT = static_cast<T*>(this);
		pT->Fire_Event(CXEVENID_MOUSELEAVE);
		
		return S_OK;;
	}

	STDMETHOD(HaveDynamic)()
	{
		T* pT = static_cast<T*>(this);
		return pT->HasDynamic(DISPID_UNKNOWN) ? S_OK : S_FALSE;
	}

	STDMETHOD(IsDefaultMousePointer)()
	{
		T* pT = static_cast<T*>(this);
		return pT->m_enumMousePointer == MousePointerDefault ? S_OK : S_FALSE;
	}

	STDMETHOD(LoadStorage)(IStorage* pStorage)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(SaveStorage)(IStorage* pStorage, BOOL fSameAsLoad)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(ExpressLoad)(LPSTREAM pStm, DWORD dwVer)
	{
		return Load(0, pStm);
	}
	STDMETHOD(ExpressSave)(LPSTREAM pStm, DWORD dwVer)
	{
		return Save(pStm, TRUE);
	}
	
protected:
	HRESULT GetPagesHelper(CAUUID* pPages, CXPAGEMAP_ENTRY* pMap)
	{
		ATLASSERT(pMap != NULL);
		if (pMap == NULL)
			return E_POINTER;

		int nCnt = 0;
		//得到页面数目用于分配数组
		for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
		{
			nCnt++;
		}

		pPages->pElems = (GUID*) CoTaskMemAlloc(sizeof(CLSID)*nCnt);
		if (pPages->pElems == NULL)
			return E_OUTOFMEMORY;

		//重置数目添加页面
		nCnt = 0;
		for (i = 0; pMap[i].pclsidPropPage != NULL; i++)
		{
			pPages->pElems[nCnt++] = *pMap[i].pclsidPropPage;
		}
		pPages->cElems = nCnt;
		return S_OK;
	}
///////////////////////////////////////////////////////////////////////////
	HRESULT IPersistStream_Load(LPSTREAM pStm, CXPROPMAP_ENTRY* pMap)
	{
		T* pT = static_cast<T*>(this);
		HRESULT hr = CxIPersistStream_Load(pStm, pMap, pT, pT->GetUnknown());
		if (SUCCEEDED(hr))
			pT->m_bRequiresSave = FALSE;
		return hr;

	}
	HRESULT IPersistStream_Save(LPSTREAM pStm, BOOL fClearDirty, CXPROPMAP_ENTRY* pMap)
	{
		T* pT = static_cast<T*>(this);
		return CxIPersistStream_Save(pStm, fClearDirty, pMap, pT, pT->GetUnknown());
	}

	// 早期版本的存贮功能
	STDMETHOD(LoadOldVer)(LPSTREAM pStm)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::Load\n"));
		T* pT = static_cast<T*>(this);
			
		pT->LoadDynamicProperty(pStm);
		pT->LoadPathData(pStm);
		return pT->IPersistStream_Load(pStm, T::GetCxPropMap());
	}
	STDMETHOD(SaveOldVer)(LPSTREAM pStm, BOOL fClearDirty)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("IPersistStreamImpl::Load\n"));
		T* pT = static_cast<T*>(this);
		
		pT->SaveDynamicProperty(pStm);
		pT->SavePathData(pStm);
		return pT->IPersistStream_Save(pStm, fClearDirty, T::GetCxPropMap());
	}
};

//class CProxy_ICxDrawToolEvents : public IConnectionPointImpl<T, &IID_ICxDrawToolEvents, CComDynamicUnkArray>

template <class T, const IID* piid, class CDV = CComDynamicUnkArray>
class CProxy_ICxObjectBaseEvents : public IConnectionPointImpl<T, piid, CComDynamicUnkArray>
{
public:
	public:
	VOID Fire_Event(DISPID dispid)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
				
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
					DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
	}
	VOID Fire_Click()
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(CXPROPID_CLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
	}
	VOID Fire_DblClick()
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(CXPROPID_DBLCLICK, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
	}
	VOID Fire_KeyDown(SHORT* KeyCode, SHORT Shift)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		
		CComPtr<ICxReturnInteger> spKeyCode;
		spKeyCode.CoCreateInstance(CLSID_CxReturnInteger);
		spKeyCode->put_Value(*KeyCode);
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[1] = spKeyCode;
				pvars[0] = Shift;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(CXPROPID_KEYDOWN, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		spKeyCode->get_Value((int *)KeyCode);
		delete[] pvars;
	}
	VOID Fire_KeyUp(SHORT* KeyCode, SHORT Shift)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();

		CComPtr<ICxReturnInteger> spKeyCode;
		spKeyCode.CoCreateInstance(CLSID_CxReturnInteger);
		spKeyCode->put_Value(*KeyCode);
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[1] = spKeyCode;
				pvars[0] = Shift;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(CXPROPID_KEYUP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		spKeyCode->get_Value((int *)KeyCode);
		delete[] pvars;
	}
	VOID Fire_KeyPress(SHORT* KeyCode, SHORT Shift)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[1] = KeyCode;
				pvars[0] = Shift;
				DISPPARAMS disp = { pvars, NULL, 2, 0 };
				pDispatch->Invoke(CXPROPID_KEYPRESS, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	VOID Fire_MouseDown(int Button, int Shift, single X, single Y)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[3] = Y;
				pvars[2] = X;
				pvars[1] = Shift;
				pvars[0] = Button;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				pDispatch->Invoke(CXPROPID_MOUSEDOWN, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	VOID Fire_MouseUp(int Button, int Shift, single X, single Y)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[3] = Y;
				pvars[2] = X;
				pvars[1] = Shift;
				pvars[0] = Button;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				pDispatch->Invoke(CXPROPID_MOUSEUP, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	VOID Fire_MouseMove(int Button, int Shift, single X, single Y)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[3] = Y;
				pvars[2] = X;
				pvars[1] = Shift;
				pvars[0] = Button;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				pDispatch->Invoke(CXPROPID_MOUSEMOVE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	VOID Fire_MouseLeave(int Button, int Shift, single X, single Y)
	{
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				pvars[3] = Y;
				pvars[2] = X;
				pvars[1] = Shift;
				pvars[0] = Button;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				pDispatch->Invoke(CXPROPID_MOUSELEAVE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
};

template <class T>
class CProxy_ICxDrawObjectEvents : public CProxy_ICxObjectBaseEvents<T, &DIID__ICxDrawObjectEvents>
{

};

//template <class T>
//class ATL_NO_VTABLE ICxPerPropertyEditImpl : public ICxPerPropertyEdit
//{
//public:
//	STDMETHOD(OnEnumPropertySelect)(DISPID dispID, long lSelect, HWND hParentWnd)
//	{
//		T* pT = static_cast<T*>(this);
//		return pT->ICxPerPropertyEdit_OnEnumPropertySelect(dispID, lSelect, hParentWnd);
//	}
//	STDMETHOD(Edit)(DISPID dispID, HWND hParentWnd, int x, int y, int cx, VARIANT* pvaOut)
//	{
//		T* pT = static_cast<T*>(this);
	//	return pT->ICxPerPropertyEdit_Edit(dispID, hParentWnd, x, y, cx, pvaOut);
//				[helpstring("method OnEnumPropertySelect")] HRESULT OnEnumPropertySelect(DISPID dispID, int nSelect, HWND hParentWnd);
  
//	}
//};

/////////////////////////////////////////////////////////////////////////////
// ICxDispatchImpl

template <class T, class IT, const IID* piid, const GUID* plibid = &CComModule::m_libid, WORD wMajor = 1,
WORD wMinor = 0, class tihclass = CComTypeInfoHolder>
class ATL_NO_VTABLE ICxDispatchImpl : public IDispatchImpl<IT, piid, plibid, wMajor, wMinor, tihclass>
{
public:
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pbstrName)
	{
		T* pT = static_cast<T*>(this);
		*pbstrName = pT->GetClientName();
		return S_OK;
	}
	STDMETHOD(put_Name)(/*[in]*/ BSTR bstrName)
	{
		T* pT = static_cast<T*>(this);
		pT->SetClientName(bstrName);
		return S_OK;
	}
	STDMETHOD(IsConnected)(BSTR bstrPropertyName, VARIANT_BOOL* pbHasConnection)
	{
		DISPID dispid;
		GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);

		T* pT = static_cast<T*>(this);
		if (pT->HasDynamic(dispid))
			*pbHasConnection = VARIANT_TRUE;
		else
			*pbHasConnection = VARIANT_FALSE;

		return S_OK;
	}
	STDMETHOD(ConnectObject)(BSTR bstrPropertyName, IUnknown* punkObject)
	{
		DISPID dispid;
		GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);

		T* pT = static_cast<T*>(this);
		pT->AddDynamic(dispid, punkObject);
		pT->FireDynamicChange();

		return S_OK;
	}
	STDMETHOD(ConnectDirect)(BSTR bstrPropertyName, BSTR bstrDataSource)
	{
		DISPID dispid;
		GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);

		return S_OK;
	}
	STDMETHOD(GetConnectObject)(BSTR bstrPropertyName, IUnknown** ppunkObject)
	{
		DISPID dispid;
		GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);

		T* pT = static_cast<T*>(this);
		pT->GetDynamic(dispid, ppunkObject);

		return S_OK;
	}
	STDMETHOD(Disconnect)(BSTR bstrPropertyName)
	{
		DISPID dispid;
		GetIDsOfNames(IID_NULL, &bstrPropertyName, 1, LOCALE_USER_DEFAULT, &dispid);

		T* pT = static_cast<T*>(this);
		pT->RemoveDynamic(dispid);
		pT->FireDynamicChange();

		return S_OK;
	}
	STDMETHOD(GetConnectState)(DISPID dispid, int* pState)
	{
		T* pT = static_cast<T*>(this);
		*pState = pT->GetDynamicState(dispid);

		return S_OK;
	}

	STDMETHOD(get_EdgeColor)(OLE_COLOR *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_clrEdgeColor;

		return S_OK;
	}

	STDMETHOD(put_EdgeColor)(OLE_COLOR newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_clrEdgeColor == newVal)
			return S_OK;

		pT->m_clrEdgeColor = newVal;
		
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_FillColor)(OLE_COLOR *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_clrFillColor;

		return S_OK;
	}

	STDMETHOD(put_FillColor)(OLE_COLOR newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_clrFillColor == newVal)
			return S_OK;

		pT->m_clrFillColor = newVal;
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_HighLightFillColor)(/*[out, retval]*/ OLE_COLOR *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_clrHighLightFill;

		return S_OK;
	}

	STDMETHOD(put_HighLightFillColor)(/*[in]*/ OLE_COLOR newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_clrHighLightFill == newVal)
			return S_OK;

		pT->m_clrHighLightFill = newVal;
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_HighLightEdgeColor)(/*[out, retval]*/ OLE_COLOR *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_clrHighLightEdge;

		return S_OK;
	}

	STDMETHOD(put_HighLightEdgeColor)(/*[in]*/ OLE_COLOR newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_clrHighLightEdge == newVal)
			return S_OK;

		pT->m_clrHighLightEdge = newVal;
		pT->FireViewChange();
		
		return S_OK;
	}

	STDMETHOD(get_FillStyle)(enumFillStyle *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumFillStyle; 
		return S_OK;
	}

	STDMETHOD(put_FillStyle)(enumFillStyle newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumFillStyle == newVal)
			return S_OK;

		pT->m_enumFillStyle = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_BrushImageFileName)(BRUSHFILE *pVal)
	{
		T* pT = static_cast<T*>(this);
        *pVal = (pT->m_BrushImageFileName).Copy();	
		return S_OK;
	}

	STDMETHOD(put_BrushImageFileName)(BRUSHFILE newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_BrushImageFileName != newVal)
		{
			pT->m_BrushImageFileName = newVal;	
			pT->FireViewChange();
		}
		
		return S_OK;
	}

	STDMETHOD(get_EdgeStyle)(enumEdgeStyle *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumEdgeStyle;
		return S_OK;
	}

	STDMETHOD(put_EdgeStyle)(enumEdgeStyle newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumEdgeStyle == newVal)
			return S_OK;

		pT->FireViewChange();
		pT->m_enumEdgeStyle = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_EdgeWidth)(float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_fEdgeWidth;
		return S_OK;
	}

	STDMETHOD(put_EdgeWidth)(float newVal)
	{
		T* pT = static_cast<T*>(this);
		if (newVal < 1 || newVal >60)
			return S_OK;
		if (pT->m_fEdgeWidth == newVal)
			return S_OK;

		pT->FireViewChange();
		pT->m_fEdgeWidth = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_HorizontalFillDirection)(enumHorizontalDirection *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumHorFillDirection;
		return S_OK;
	}

	STDMETHOD(put_HorizontalFillDirection)(enumHorizontalDirection newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumHorFillDirection == newVal)
			return S_OK;

		pT->m_enumHorFillDirection = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_VerticalFillDirection)(enumVerticalDirection *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumVerFillDirection;
		return S_OK;
	}

	STDMETHOD(put_VerticalFillDirection)(enumVerticalDirection newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumVerFillDirection == newVal)
			return S_OK;

		pT->m_enumVerFillDirection = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_VerticalFillPercent)(int *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_nVerFillPercent;
		return S_OK;
	}

	STDMETHOD(put_VerticalFillPercent)(int newVal)
	{
		T* pT = static_cast<T*>(this);
		if ((newVal > 100) || (newVal < 0))
			return S_OK;
		if (pT->m_nVerFillPercent == newVal)
			return S_OK;

		pT->m_nVerFillPercent = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_HorizontalFillPercent)(int *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_nHorFillPercent;
		return S_OK;
	}

	STDMETHOD(put_HorizontalFillPercent)(int newVal)
	{
		T* pT = static_cast<T*>(this);
		if ((newVal > 100) || (newVal < 0))
			return S_OK;
		if (pT->m_nHorFillPercent == newVal)
			return S_OK;

		pT->m_nHorFillPercent = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_ScaleBasePoint)(/*[out, retval]*/ enumScaleBasePoint *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumScaleBase;
		return S_OK;
	}

	STDMETHOD(put_ScaleBasePoint)(/*[in]*/ enumScaleBasePoint newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumScaleBase == newVal)
			return S_OK;

		pT->m_enumScaleBase = newVal;
		if (pT->ClientIsRunMode())
			pT->FireViewChange();	
		return S_OK;
	}

	STDMETHOD(get_RotateAngle)(float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_fRotateAngle;
		return S_OK;
	}

	STDMETHOD(put_RotateAngle)(float newVal)
	{
		using namespace Gdiplus;
		T* pT = static_cast<T*>(this);
		
		if (pT->ClientIsRunMode())
		{
			if (pT->m_fRotateAngle == newVal)
				return S_OK;

			pT->FireViewChange();
			pT->m_fRotateAngle = newVal;
			pT->ReSetObjectPath();
		}
		else
		{
			pT->FireViewChange();

			Matrix mtTansMatrix;
			mtTansMatrix.RotateAt(newVal, pT->m_ptRotateBase);
			pT->m_pObjectPath->Transform(&mtTansMatrix);
		}
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_HorizontalScale)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_fHorScale;
		return S_OK;
	}

	STDMETHOD(put_HorizontalScale)(/*[in]*/ float newVal)
	{
		using namespace Gdiplus;
		T* pT = static_cast<T*>(this);
		
		if (pT->ClientIsRunMode())
		{
			if (pT->m_fHorScale == newVal)
				return S_OK;

			pT->FireViewChange();
			pT->m_fHorScale = newVal;
			pT->ReSetObjectPath();
		}
		else
		{
			pT->FireViewChange();
			if (newVal == 0)
			{
				RectF rcPos;
				pT->m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
				newVal = 1 / rcPos.Width;
			}
			Matrix mtTansMatrix;
			POINT ptScaleBase = pT->GetScaleBasePoint();
			mtTansMatrix.Translate((float)ptScaleBase.x, (float)ptScaleBase.y);
			mtTansMatrix.Scale(newVal, 1);
			mtTansMatrix.Translate((float)-ptScaleBase.x, (float)-ptScaleBase.y);
			mtTansMatrix.TransformPoints(&pT->m_ptRotateBase);
			pT->m_pObjectPath->Transform(&mtTansMatrix);
		}
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_VerticalScale)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_fVerScale;
		return S_OK;
	}

	STDMETHOD(put_VerticalScale)(/*[in]*/ float newVal)
	{
		using namespace Gdiplus;
		T* pT = static_cast<T*>(this);
		
		if (pT->ClientIsRunMode())
		{
			if (pT->m_fVerScale == newVal)
				return S_OK;

			pT->FireViewChange();
			pT->m_fVerScale = newVal;
			pT->ReSetObjectPath();
		}
		else
		{
			pT->FireViewChange();
			if (newVal == 0)
			{
				RectF rcPos;
				pT->m_pObjectPath->GetBounds(&rcPos, NULL, NULL);
				newVal = 1 / rcPos.Height;
			}
			Matrix mtTansMatrix;
			POINT ptScaleBase = pT->GetScaleBasePoint();
			mtTansMatrix.Translate((float)ptScaleBase.x, (float)ptScaleBase.y);
			mtTansMatrix.Scale(1, newVal);
			mtTansMatrix.Translate((float)-ptScaleBase.x, (float)-ptScaleBase.y);
			mtTansMatrix.TransformPoints(&pT->m_ptRotateBase);
			pT->m_pObjectPath->Transform(&mtTansMatrix);
		}
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_JoinStyle)(/*[out, retval]*/ enumLineJoinStyle *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumLineJoinStyle;
		return S_OK;
	}

	STDMETHOD(put_JoinStyle)(/*[in]*/ enumLineJoinStyle newVal)
	{
		T* pT = static_cast<T*>(this);
		if (pT->m_enumLineJoinStyle == newVal)
			return S_OK;

		pT->m_enumLineJoinStyle = newVal;
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}

	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal)
	{
		T* pT = static_cast<T*>(this);
		VARIANT_BOOL oldVal = pT->GetClientVisibility() ? VARIANT_TRUE : VARIANT_FALSE;
		if (oldVal == newVal)
			return S_OK;

		pT->SetClientVisibility(newVal != VARIANT_FALSE);
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_Layer)(/*[out, retval]*/ int *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->GetClientLayer();
		return S_OK;
	}

	STDMETHOD(put_Layer)(/*[in]*/ int newVal)
	{
		T* pT = static_cast<T*>(this);
		int oldVal = pT->GetClientLayer();
		if (oldVal == newVal)
			return S_OK;

		pT->SetClientLayer(newVal);
		pT->FireViewChange();
		return S_OK;
	}

	STDMETHOD(get_Locked)(/*[out, retval]*/ VARIANT_BOOL *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	
	STDMETHOD(put_Locked)(/*[in]*/ VARIANT_BOOL newVal)
	{
		T* pT = static_cast<T*>(this);
		VARIANT_BOOL oldVal = pT->GetClientLocked() ? VARIANT_TRUE : VARIANT_FALSE;
		if (oldVal == newVal)
			return S_OK;
		
		pT->SetClientLocked(newVal != VARIANT_FALSE);
		pT->FireViewChange();
		return S_OK;
	}

//	STDMETHOD(get_MouseInput)(/*[out, retval]*/ enumMouseInputMode *pVal)
//	{
//		T* pT = static_cast<T*>(this);
//		*pVal = pT->m_enumMouseInputMode;
//		return S_OK;
//	}

//	STDMETHOD(put_MouseInput)(/*[in]*/ enumMouseInputMode newVal)
//	{
//		T* pT = static_cast<T*>(this);
//		pT->m_enumMouseInputMode = newVal;
//		return S_OK;
//	}

	STDMETHOD(get_VerticalPosition)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		*pVal = rcPos.top; 
		return S_OK;
	}

	STDMETHOD(put_VerticalPosition)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		if (newVal == rcPos.top)
			return S_OK;

		rcPos.bottom = newVal + rcPos.bottom - rcPos.top;
		rcPos.top = newVal;
		pT->FireViewChange();
		pT->MoveTo(rcPos, FALSE);
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_HorizontalPosition)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		*pVal = rcPos.left;
		return S_OK;
	}

	STDMETHOD(put_HorizontalPosition)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		if (newVal == rcPos.left)
			return S_OK;
		rcPos.right = newVal + rcPos.right - rcPos.left;
		rcPos.left = newVal;
		pT->FireViewChange();
		pT->MoveTo(rcPos, FALSE);
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_Width)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		ATLASSERT(rcPos.right > rcPos.left);
		*pVal = rcPos.right - rcPos.left;
		return S_OK;
	}

	STDMETHOD(put_Width)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		if (fabs(rcPos.right - rcPos.left - newVal) < 0.001f)
			return S_OK;

		if (ABSOLUTEVALUE(newVal) < 0.001f)
			newVal = newVal >= 0 ? 0.001f : -0.001f;

		ATLASSERT(rcPos.right > rcPos.left);
		rcPos.right = rcPos.left + newVal;
		
		pT->FireViewChange();
		pT->MoveTo(rcPos, FALSE);
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_Height)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		ATLASSERT(rcPos.bottom > rcPos.top);
		*pVal = rcPos.bottom - rcPos.top;

		return S_OK;
	}
	STDMETHOD(put_Height)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		RECTF rcPos;
		pT->GetPositionRect(&rcPos);
		if (fabs(rcPos.bottom - rcPos.top - newVal) < 0.001f)
			return S_OK;

		if (ABSOLUTEVALUE(newVal) < 0.001f)
			newVal = newVal >= 0 ? 0.001f : -0.001f;

		ATLASSERT(rcPos.bottom > rcPos.top);
		rcPos.bottom = rcPos.top + newVal;
		
		pT->FireViewChange();
		pT->MoveTo(rcPos, FALSE);
		pT->FireViewChange();

		return S_OK;
	}

	STDMETHOD(get_RoateBasePointX)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_ptRotateBase.X;
		return S_OK;
	}

	STDMETHOD(put_RoateBasePointX)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		pT->m_ptRotateBase.X = newVal;
		return S_OK;
	}

	STDMETHOD(get_RoateBasePointY)(/*[out, retval]*/ float *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_ptRotateBase.Y;
		return S_OK;
	}
	STDMETHOD(put_RoateBasePointY)(/*[in]*/ float newVal)
	{
		T* pT = static_cast<T*>(this);
		pT->m_ptRotateBase.Y = newVal;
		return S_OK;
	}
	STDMETHOD(get_ToolTipText)(/*[out, retval]*/ BSTR *pbstrText)
	{
		T* pT = static_cast<T*>(this);
		*pbstrText = pT->GetClientToolTipText();
		return S_OK;
	}
	STDMETHOD(put_ToolTipText)(/*[in]*/ BSTR bstrText)
	{
		T* pT = static_cast<T*>(this);
		pT->SetClientToolTipText(bstrText);
		return S_OK;
	}
	STDMETHOD(get_Privilege)(/*[out, retval]*/ int *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->GetClientPrivilege();
		return S_OK;
	}
	STDMETHOD(put_Privilege)(/*[in]*/ int newVal)
	{
		T* pT = static_cast<T*>(this);
		pT->SetClientPrivilege(newVal);
		pT->FireViewChange();
		return S_OK;
	}
	STDMETHOD(get_MousePointer)(/*[out, retval]*/ enumMousePointer *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_enumMousePointer;
		return S_OK;
	}
	STDMETHOD(put_MousePointer)(/*[in]*/ enumMousePointer newVal)
	{
		T* pT = static_cast<T*>(this);
		pT->m_enumMousePointer = newVal;
		return S_OK;
	}

	STDMETHOD(get_Description)(/*[out, retval]*/ BSTR *pVal)
	{
		T* pT = static_cast<T*>(this);
		*pVal = pT->m_bstrDescription.Copy();
		return S_OK;
	}
	STDMETHOD(put_Description)(/*[in]*/ BSTR newVal)
	{
		T* pT = static_cast<T*>(this);
		pT->m_bstrDescription = newVal;
		return S_OK;
	}
};

//////////////////////////////////////////////////////////////////////////////////

#endif // __CXDRAWOBJ_H_