// DrawImage.cpp : Implementation of CDrawImage
#include "stdafx.h"
#include "CxDrawShapes.h"
#include "DrawImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDrawImage

int GetCodecClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(int j = 0; j < int(num); ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         return j;  // Success
      }    
   } // for

   return -1;  // Failure

} // GetCodecClsid


void Draw3DParallelogram(Graphics *graphics, PointF* point, BOOL bRaised = TRUE)
{
	PointF ptCenter;
	ptCenter.X = (point[0].X + point[2].X) / 2;
	ptCenter.Y = (point[0].Y + point[2].Y) / 2;

	Color color;
	color.SetFromCOLORREF(GetSysColor(COLOR_BTNHILIGHT)); 
	Pen penHilight(color, 2);
	color.SetFromCOLORREF(GetSysColor(COLOR_BTNSHADOW)); 
	Pen penShadow(color, 2);
	
	for (int i = 1; i <= 4; i++)
	{
		PointF ptSideCenterToCenter;
		ptSideCenterToCenter.X = (point[i - 1].X + point[i % 4].X) / 2 - ptCenter.X;
		ptSideCenterToCenter.Y = (point[i - 1].Y + point[i % 4].Y) / 2 - ptCenter.Y;
		BOOL bShadow =  (ptSideCenterToCenter.X >= 0 && ptSideCenterToCenter.Y >= 0) ||
			(ptSideCenterToCenter.X > 0 && ptSideCenterToCenter.Y < 0 && ptSideCenterToCenter.X >= -ptSideCenterToCenter.Y) ||
			(ptSideCenterToCenter.X < 0 && ptSideCenterToCenter.Y > 0 && -ptSideCenterToCenter.X <= ptSideCenterToCenter.Y);
		if (!bRaised)
			bShadow = !bShadow;
		
		graphics->DrawLine(bShadow ? &penShadow : &penHilight, point[i - 1], point[i % 4]);
	}
}

HRESULT CDrawImage::Draw(HDC hdcDest, BOOL bPrint)
{
	if (m_apImage.GetSize() < 1)
		return S_FALSE;
	
	Graphics graphics(hdcDest);
	if (bPrint)
		graphics.SetPageUnit(UnitPixel);

	PointF point[4];
	m_pObjectPath->GetPathPoints(point, 4);
	point[2] = point[3];
	
	long lCurrentImage = m_lCurrentImage;
	if (m_bHoverImageDisplayed == VARIANT_TRUE && m_bMouseOn)
		lCurrentImage = m_lCurrentImage + 1;
	
	if (m_bDownImageDisplayed == VARIANT_TRUE && m_enumButtonState == ButtonStateDown)
	{
		if (m_bHoverImageDisplayed == VARIANT_TRUE)
			lCurrentImage = m_lCurrentImage + 2;
		else
			lCurrentImage = m_lCurrentImage + 1;
	}
	
	if (lCurrentImage < m_apImage.GetSize())
	{
		Image* pImage = m_apImage[lCurrentImage];
		
		//绘制图片
		if (m_bTransparency == VARIANT_TRUE)
		{
			OLETRANSLATEGDIPLUSCOLOR(clrTransparent)
			ImageAttributes imageAttr;
 			imageAttr.SetColorKey(clrTransparent, clrTransparent, ColorAdjustTypeBitmap);
			
			graphics.DrawImage(pImage, point, 3,
				0, 0, float(pImage->GetWidth()), float(pImage->GetHeight()), UnitPixel, &imageAttr);
		}
		else
		{
#ifdef _DEBUG
			DWORD dwTick = GetTickCount();
// 			for (int i = 0; i < 100; i++)
#endif
			graphics.DrawImage(pImage, point, 3); 

#ifdef _DEBUG
			dwTick = GetTickCount() - dwTick;
			dwTick++;
#endif
		}
	}
	
	m_pObjectPath->GetPathPoints(point, 4);
	
	if (m_enumButtonStyle == BitmapButtonStylePush && m_bDownImageDisplayed != VARIANT_TRUE)
	{
		if (m_enumButtonState == ButtonStateUp)
			Draw3DParallelogram(&graphics, point);
		
		if (ClientIsRunMode() && m_enumButtonState == ButtonStateDown)
			Draw3DParallelogram(&graphics, point, FALSE);
	}
	else if (m_enumButtonStyle == BitmapButtonStyleMultiState  && m_bDownImageDisplayed != VARIANT_TRUE)
	{
		if (m_enumButtonState == ButtonStateUp)
			Draw3DParallelogram(&graphics, point);
		else if (m_enumButtonState == ButtonStateDown)
			Draw3DParallelogram(&graphics, point, FALSE);
	}
	else
	{
		//绘制边界
		switch (m_enumFrameStyle)
		{
		case FrameStyleSunken:
			Draw3DParallelogram(&graphics, point, FALSE);
			break;
		case FrameStyleRaised:
			Draw3DParallelogram(&graphics, point);
			break;
		case FrameStyleEtched:
		case FrameStyleBump:
		case FrameStyleRectangle:
			{
				Color color;
				
				if (m_enumFrameStyle == FrameStyleRectangle)
				{
					COLORREF clrFrame;
					OleTranslateColor(m_clrFrame, NULL, &clrFrame);
					color.SetFromCOLORREF(clrFrame);
				}
				else
				{
					color.SetFromCOLORREF(m_enumFrameStyle == FrameStyleEtched ?
						GetSysColor(COLOR_BTNHILIGHT) : GetSysColor(COLOR_BTNSHADOW)); 
				}
				
				Pen pen(color, 1);
				for (int i = 1; i <= 4; i++)
					graphics.DrawLine(&pen, point[i - 1], point[i % 4]);
			}
			break;
		default:
			break;
		}
		
	}
	
	return S_OK;
}

HRESULT CDrawImage::DoDraw(HDC hdcDest)
{
	return Draw(hdcDest);
}

HRESULT CDrawImage::DoPrint(HDC hdcDest)
{
	return Draw(hdcDest, TRUE);
}

HRESULT CDrawImage::DoLoad(DWORD dwVer, LPSTREAM pStm)
{
//	dwVer = _SUPERCX_VER_0102;

	if (dwVer != _SUPERCX_VER_0102)
		pStm->Read(&dwVer, sizeof(dwVer), NULL);

	m_dwLoadVer = dwVer;

	if (dwVer == _SUPERCX_VER_0102)
	{
		int nCount;
		pStm->Read(&nCount, sizeof(int), NULL);

		BitmapData bitmapData;
		Bitmap* pBitmap;
		for (int i = 0; i < nCount; i++)
		{
			pStm->Read(&bitmapData, 8, NULL);
			pBitmap = new Bitmap(bitmapData.Width, bitmapData.Height, PixelFormat32bppARGB);
			Rect rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
			pBitmap->LockBits(&rect,
				ImageLockModeWrite,
				PixelFormat32bppARGB,
				&bitmapData);
			pStm->Read(bitmapData.Scan0, 4 * bitmapData.Width * bitmapData.Height, NULL);
			pBitmap->UnlockBits(&bitmapData);
			m_apImage.Add(pBitmap);
			m_apImageStream.Add(NULL);
		}
	}
	else
	{
		m_spDrawObjectSite->SetNeedStorage();
	}

	pStm->Read(&m_bTransparency, sizeof(m_bTransparency), NULL);
	pStm->Read(&m_clrTransparent, sizeof(m_clrTransparent), NULL);
	pStm->Read(&m_lCurrentImage, sizeof(m_lCurrentImage), NULL);
	
	pStm->Read(&m_bDownImageDisplayed, sizeof(m_bDownImageDisplayed), NULL);
	pStm->Read(&m_bHoverImageDisplayed, sizeof(m_bHoverImageDisplayed), NULL);
	
	pStm->Read(&m_enumFrameStyle, sizeof(m_enumFrameStyle), NULL);
	pStm->Read(&m_clrFrame, sizeof(m_clrFrame), NULL);

	pStm->Read(&m_enumButtonStyle, sizeof(m_enumFrameStyle), NULL);
	pStm->Read(&m_enumButtonState, sizeof(m_clrTransparent), NULL);

	pStm->Read(&m_ptRotateBase, sizeof(m_ptRotateBase), NULL);
	pStm->Read(&m_enumScaleBase, sizeof(m_enumScaleBase), NULL);
	
	pStm->Read(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.ReadFromStream(pStm);
	
	return CCxDrawObjectBase::DoLoad(dwVer, pStm);
}

HRESULT CDrawImage::DoSave(DWORD dwVer, LPSTREAM pStm)
{
 	dwVer = m_dwLoadVer;
	pStm->Write(&dwVer, sizeof(dwVer), NULL);

	if (dwVer == _SUPERCX_VER_0102)
	{
		int nCount = m_apImage.GetSize();
		pStm->Write(&nCount, sizeof(int), NULL);
		
		BitmapData bitmapData;
		Bitmap* pBitmap;
		for (int i = 0; i < nCount; i++)
		{
			pBitmap = (Bitmap *)m_apImage[i];
			
			Rect rect(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());
			pBitmap->LockBits(&rect,
				ImageLockModeRead,
				PixelFormat32bppARGB,
				&bitmapData);
			pStm->Write(&bitmapData, 8, NULL);
			pStm->Write(bitmapData.Scan0, 4 * bitmapData.Width * bitmapData.Height, NULL);
			pBitmap->UnlockBits(&bitmapData);
		}
	}
	else
	{
		m_spDrawObjectSite->SetNeedStorage();
	}
	
	pStm->Write(&m_bTransparency, sizeof(m_bTransparency), NULL);
	pStm->Write(&m_clrTransparent, sizeof(m_clrTransparent), NULL);
	pStm->Write(&m_lCurrentImage, sizeof(m_lCurrentImage), NULL);
	
	pStm->Write(&m_bDownImageDisplayed, sizeof(m_bDownImageDisplayed), NULL);
	pStm->Write(&m_bHoverImageDisplayed, sizeof(m_bHoverImageDisplayed), NULL);
	
	pStm->Write(&m_enumFrameStyle, sizeof(m_enumFrameStyle), NULL);
	pStm->Write(&m_clrFrame, sizeof(m_clrFrame), NULL);

	pStm->Write(&m_enumButtonStyle, sizeof(m_enumButtonStyle), NULL);
	pStm->Write(&m_enumButtonState, sizeof(m_clrTransparent), NULL);
	
	pStm->Write(&m_ptRotateBase, sizeof(m_ptRotateBase), NULL);
	pStm->Write(&m_enumScaleBase, sizeof(m_enumScaleBase), NULL);
	
	pStm->Write(&m_enumMousePointer, sizeof(m_enumMousePointer), NULL);
	m_bstrDescription.WriteToStream(pStm);

	return CCxDrawObjectBase::DoSave(dwVer, pStm);
}

STDMETHODIMP CDrawImage::LoadStorage(IStorage* pStorage)
{
	HRESULT hr;
	WCHAR szImageName[16];

	int nCount = 0;
	while (1)
	{
		swprintf(szImageName, L"Image %d", nCount++);

		CComPtr<IStream> spStream;
		hr = pStorage->OpenStream(szImageName, NULL, 
			STGM_READ|STGM_SHARE_EXCLUSIVE, 0, &spStream);
		if (FAILED(hr))
			break;

		STATSTG stat;
		hr = spStream->Stat(&stat, STATFLAG_NONAME);
		if (hr != S_OK)
			return hr;
	
		IStream* pImageStream;
		hr = ::CreateStreamOnHGlobal(NULL, TRUE, &pImageStream);
		if (hr != S_OK)
		{
			if (pImageStream != NULL)
				pImageStream->Release();
			return hr;
		}

		hr = spStream->CopyTo(pImageStream, stat.cbSize, NULL, NULL);
		if (hr != S_OK)
		{
			if (pImageStream != NULL)
				pImageStream->Release();
			return hr;
		}

		Image* pImage = Bitmap::FromStream(pImageStream);
		m_apImage.Add(pImage);
		m_apImageStream.Add(pImageStream);
	}

	return S_OK;
}

STDMETHODIMP CDrawImage::SaveStorage(IStorage* pStorage, BOOL fSameAsLoad)
{
	// image/bmp
	// image/jpeg
	// image/gif
	// image/tiff
	// image/png

	// Get the CLSID of the TIFF encoder.
	CLSID encoderClsid;
	GetCodecClsid(L"image/png", &encoderClsid);
//	GetCodecClsid(L"image/jpeg", &encoderClsid);

	WCHAR szImageName[16];

	for (int i = 0; i < m_apImage.GetSize(); i++)
	{
		swprintf(szImageName, L"Image %d", i);

		CComPtr<IStream> spStream;
		HRESULT hr = pStorage->CreateStream(szImageName,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE,
			0, 0, &spStream);
		if (FAILED(hr))
			return hr;

		Image* pImage = m_apImage[i];
		pImage->Save(spStream, &encoderClsid);
	}

	return S_OK;
}

void CDrawImage::AddObjectToPath()
{
	m_pObjectPath->AddRectangle(Gdiplus::Rect(0, 0, 200, 200));
}

//////////////////////////////////////////////////////////////
//功能: 取对象名字串
//////////////////////////////////////////////////////////////
STDMETHODIMP CDrawImage::GetClassName(BSTR* pbstrClassName)
{
	*pbstrClassName = OLESTR("Image");
	return S_OK;
}

STDMETHODIMP CDrawImage::LoadImage(VARIANT_BOOL bPrimary, short nIndex, BSTR bstrFileName)
{
	Bitmap* pBitmap = Bitmap::FromFile(bstrFileName);
	if (!pBitmap)
		return S_FALSE;

	m_apImage.Add(pBitmap);
	m_apImageStream.Add(NULL);
	if (m_apImage.GetSize() == 1 || bPrimary == VARIANT_TRUE)
	{
		ATLASSERT(m_pObjectPath != NULL);
		m_pObjectPath->Reset();
		AddObjectToPath();

		RECTF rcPos;
		rcPos.left = rcPos.top = 10;
		rcPos.right = rcPos.left + pBitmap->GetWidth();
		rcPos.bottom = rcPos.top + pBitmap->GetHeight();
		MoveTo(rcPos, FALSE);
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::LoadFromHBITMAP(VARIANT_BOOL bPrimary, short nIndex, long hbm, long hpal)
{
	Bitmap* pBitmap = Bitmap::FromHBITMAP((HBITMAP)hbm, (HPALETTE)hpal);
	if (!pBitmap)
		return S_FALSE;

	m_apImage.Add(pBitmap);
	m_apImageStream.Add(NULL);
	if (m_apImage.GetSize() == 1 || bPrimary == VARIANT_TRUE)
	{
		ATLASSERT(m_pObjectPath != NULL);
		m_pObjectPath->Reset();
		AddObjectToPath();

		RECTF rcPos;
		rcPos.left = rcPos.top = 10;
		rcPos.right = rcPos.left + pBitmap->GetWidth();
		rcPos.bottom = rcPos.top + pBitmap->GetHeight();
		MoveTo(rcPos, FALSE);
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::AddImage(BSTR bstrFileName)
{
	Image* pImage = Image::FromFile(bstrFileName);
	if (!pImage)
		return S_FALSE;

	m_apImage.Add(pImage);
	m_apImageStream.Add(NULL);
	if (m_apImage.GetSize() == 1)
	{
		ATLASSERT(m_pObjectPath != NULL);
		m_pObjectPath->Reset();
		AddObjectToPath();

		RECTF rcPos;
		rcPos.left = rcPos.top = 10;
		rcPos.right = rcPos.left + pImage->GetWidth();
		rcPos.bottom = rcPos.top + pImage->GetHeight();
		MoveTo(rcPos, FALSE);
	}
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_Transparency(VARIANT_BOOL *pVal)
{
	*pVal = m_bTransparency;
	return S_OK;
}

STDMETHODIMP CDrawImage::put_Transparency(VARIANT_BOOL newVal)
{
	BOOL bNewVal = (newVal != VARIANT_FALSE ? VARIANT_TRUE : VARIANT_FALSE);
	if (m_bTransparency == bNewVal)
		return S_OK;

	m_bTransparency = bNewVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_TransparentColor(OLE_COLOR *pVal)
{
	*pVal = m_clrTransparent;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_TransparentColor(OLE_COLOR newVal)
{
	if (m_clrTransparent == newVal)
		return S_OK;

	m_clrTransparent = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_CurrentImage(long *pVal)
{
	*pVal = m_lCurrentImage + 1;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_CurrentImage(long newVal)
{
    if (newVal > m_apImage.GetSize() || newVal < 1)
		return S_OK;

	newVal--; 

	if (m_lCurrentImage == newVal)
		return S_OK;
	
	m_lCurrentImage = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::DeleteImage(long lIndex)
{
	ATLASSERT(m_apImageStream.GetSize() == m_apImage.GetSize());

	if (lIndex >= 0 && lIndex < m_apImageStream.GetSize())
	{
		IStream* pStream = m_apImageStream[lIndex];
		if (pStream != NULL)
			pStream->Release();
		m_apImageStream.RemoveAt(lIndex);
	}

	m_apImage.RemoveAt(lIndex);

	if (m_lCurrentImage == lIndex)
	{
		if (m_lCurrentImage > 0)
			m_lCurrentImage--;
	}

	return S_OK;
}

STDMETHODIMP CDrawImage::get_ImageCount(long *pVal)
{
	*pVal = m_apImage.GetSize();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_DownImageDisplayed(VARIANT_BOOL *pVal)
{
	*pVal = m_bDownImageDisplayed;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_DownImageDisplayed(VARIANT_BOOL newVal)
{
	BOOL bNewVal = (newVal != VARIANT_FALSE ? VARIANT_TRUE : VARIANT_FALSE);
	if (m_bDownImageDisplayed == bNewVal)
		return S_OK;

	m_bDownImageDisplayed = bNewVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_HoverImageDisplayed(VARIANT_BOOL *pVal)
{
	*pVal = m_bHoverImageDisplayed;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_HoverImageDisplayed(VARIANT_BOOL newVal)
{
	m_bHoverImageDisplayed = (newVal != VARIANT_FALSE ? VARIANT_TRUE : VARIANT_FALSE);

	return S_OK;
}

STDMETHODIMP CDrawImage::get_ButtonStyle(enumBitmapButtonStyle *pVal)
{
	*pVal = m_enumButtonStyle;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_ButtonStyle(enumBitmapButtonStyle newVal)
{
	m_enumButtonStyle = newVal;

	return S_OK;
}

STDMETHODIMP CDrawImage::get_ButtonState(enumButtonState *pVal)
{
	*pVal = m_enumButtonState;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_ButtonState(enumButtonState newVal)
{
	if (m_enumButtonState == newVal)
		return S_OK;

	m_enumButtonState = newVal;
	FireViewChange();

	return S_OK;
}

STDMETHODIMP CDrawImage::get_FrameStyle(enumFrameStyle *pVal)
{
	*pVal = m_enumFrameStyle;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_FrameStyle(enumFrameStyle newVal)
{
	if (m_enumFrameStyle == newVal)
		return S_OK;

	m_enumFrameStyle = newVal;
	FireViewChange();
	
	return S_OK;
}

STDMETHODIMP CDrawImage::get_FrameColor(/*[out, retval]*/ OLE_COLOR *pVal)
{
	*pVal = m_clrFrame;

	return S_OK;
}

STDMETHODIMP CDrawImage::put_FrameColor(/*[in]*/ OLE_COLOR newVal)
{
	if (m_clrFrame == newVal)
		return S_OK;

	m_clrFrame = newVal;
	FireViewChange();
	
	return S_OK;
}