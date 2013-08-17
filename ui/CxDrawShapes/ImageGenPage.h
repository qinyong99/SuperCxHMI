// ImageGenPage.h : Declaration of the CImageGenPage

#ifndef __CXIMAGEGENPAGE_H_
#define __CXIMAGEGENPAGE_H_

#include "resource.h"       // main symbols

EXTERN_C const CLSID CLSID_CxImageGenPage;

/////////////////////////////////////////////////////////////////////////////
// CImageGenPage
class ATL_NO_VTABLE CImageGenPage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImageGenPage, &CLSID_CxImageGenPage>,
	public IPropertyPageImpl<CImageGenPage>,
	public CDialogImpl<CImageGenPage>
{
	class COutWindow : public CWindowImpl<COutWindow>
	{
	public:
		void* m_pParent;
		
		COutWindow()
		{
			m_pParent = NULL;
		}
		
		HWND Create(HWND hParent)
		{
			HWND hwnd = ::GetDlgItem(hParent, IDC_STATIC_OUT);
			RECT rect;
			::GetClientRect(hwnd, &rect);
			::InflateRect(&rect, -8, -14);
			rect.top += 3;
			rect.bottom += 3;
			return CWindowImpl<COutWindow>::Create(hwnd, rect, NULL, WS_CHILD | WS_VISIBLE);
		}
		
		BEGIN_MSG_MAP(COutWindow)
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_ERASEBKGND, OnBackground)
			END_MSG_MAP()
	protected:
		void Paint(HDC hdc)
		{
			ATLASSERT(m_pParent);
			
			RECT rect;
			GetClientRect(&rect);
			
			SIZE size;
			CImageGenPage* pPage = (CImageGenPage*)m_pParent;
			CComQIPtr<ICxDrawObject, &IID_ICxDrawObject> pIBmp(pPage->m_ppUnk[0]);
			int oldMode = ::SetMapMode(hdc, MM_ANISOTROPIC);
			RECT tmpRect;
			SIZE oldViewPort;
			POINT oldOrigin;
			pIBmp->GetBoundingRect(&tmpRect);
			::SetViewportExtEx(hdc, rect.right - rect.left, rect.bottom - rect.top, &oldViewPort);
			::SetWindowExtEx(hdc, tmpRect.right - tmpRect.left, tmpRect.bottom - tmpRect.top, &size);
			::SetWindowOrgEx(hdc, tmpRect.left, tmpRect.top, &oldOrigin);
			
			pIBmp->Draw(hdc);
			
			::SetWindowOrgEx(hdc, oldOrigin.x, oldOrigin.y, NULL);
			::SetWindowExtEx(hdc, size.cx, size.cy, NULL);
			::SetViewportExtEx(hdc, oldViewPort.cx, oldViewPort.cy, NULL);
			::SetMapMode(hdc, oldMode);
		}
		LRESULT OnBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			HDC hdc = (HDC)wParam;
			int w = ::GetDeviceCaps(hdc, HORZRES);
			int h = ::GetDeviceCaps(hdc, VERTRES);
			RECT rect;
			rect.left = rect.top = 0;
			rect.right = w;
			rect.bottom = h;
			
			HBRUSH hBrush = CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
			::FillRect(hdc, &rect, hBrush);
			::DeleteObject((HGDIOBJ)hBrush);
			
			return 0;
		}
		LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
		{
			PAINTSTRUCT info;
			HDC hdc = BeginPaint(&info);
			Paint(hdc);
			EndPaint(&info);
			return 0;
		}
	} m_wndShow;
		
public:
	CImageGenPage() 
	{
		m_dwTitleID = IDS_TITLECxImageGenPage;
		m_dwHelpFileID = IDS_HELPFILECxImageGenPage;
		m_dwDocStringID = IDS_DOCSTRINGCxImageGenPage;
	}

	enum {IDD = IDD_CXIMAGEGENPAGE};

DECLARE_REGISTRY_RESOURCEID(IDR_CXIMAGEGENPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CImageGenPage) 
	COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CImageGenPage)
	CHAIN_MSG_MAP(IPropertyPageImpl<CImageGenPage>)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_COMBO_INDEX, CBN_SELENDOK, OnSelendokCombo_index)
	COMMAND_HANDLER(IDC_BUTTON_ADD, BN_CLICKED, OnClickedButton_add)
	COMMAND_HANDLER(IDC_BUTTON_DELETE, BN_CLICKED, OnClickedButton_delete)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	STDMETHOD(Apply)(void)
	{
		ATLTRACE(_T("CMainPage::Apply\n"));
		for (UINT i = 0; i < m_nObjects; i++)
		{
			//CComQIPtr<IPolyCtl, &IID_IPolyCtl> pPoly(m_ppUnk[i]);
			// Do something interesting here
			// ICircCtl* pCirc;
			// m_ppUnk[i]->QueryInterface(IID_ICircCtl, (void**)&pCirc);
			// pCirc->put_Caption(CComBSTR("something special"));
			// pCirc->Release();
		}
		m_bDirty = FALSE;
		return S_OK;
	}
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_wndShow.m_pParent = this;
		m_wndShow.Create(m_hWnd);
		ResetIndexChoice();
		return 0;
	}
	void ResetIndexChoice()
	{
		//获取图像总数
		long imageCount = 0;
		CComQIPtr<IDrawImage, &IID_IDrawImage> pIBmp(m_ppUnk[0]);
		pIBmp->get_ImageCount(&imageCount);

		//设置图像选择ComboBox
		HWND hwnd = GetDlgItem(IDC_COMBO_INDEX);
		::SendMessage(hwnd, CB_RESETCONTENT, 0, 0);
		TCHAR buffer[20];
		for(int i = 1; i <= imageCount; i++)
		{
			wsprintf(buffer, "%d", i);
			::SendMessage(hwnd, CB_ADDSTRING, 0, (LPARAM)buffer);
		}

		//设置当前选项
		long index = 1;
		pIBmp->get_CurrentImage(&index);
		hwnd = GetDlgItem(IDC_COMBO_INDEX);
		::SendMessage(hwnd, CB_SETCURSEL, index - 1, 0);
	}
	LRESULT OnSelendokCombo_index(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		int index = (int)(::SendMessage(hWndCtl, CB_GETCURSEL, 0, 0));
		CComQIPtr<IDrawImage, &IID_IDrawImage> pIBmp(m_ppUnk[0]);
		pIBmp->put_CurrentImage(index + 1);
		
		m_wndShow.Invalidate(TRUE);
		return 0;
	}
	LRESULT OnClickedButton_add(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OPENFILENAME ofn;
		TCHAR szFileName[_MAX_PATH];
		szFileName[0] = '\0';
		TCHAR szFileTitle[_MAX_FNAME];
		szFileTitle[0] = '\0';

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize          = sizeof(OPENFILENAME);
		ofn.hwndOwner            = m_hWnd;
		ofn.lpstrFilter = 
			_T("所有图片文件\0*.bmp;*.dib;*.jpg;*.gif;*.tif;*.tiff;*.png;*.wmf;*.emf;*.ico;*.cur\0")
			_T("位图 (*.bmp;*.dib)\0*.bmp;*.dib\0")
			_T("JPEG 图像 (*.jpg)\0*.jpg\0")
			_T("GIF 图像 (*.gif)\0*.gif\0")
			_T("TIIF 图像 (*.tif;*.tiff)\0*.tif;*.tiff\0")
			_T("PNG 图像 (*.png)\0*.png\0")
			_T("元文件 (*.wmf;*.emf)\0*.wmf;*.emf\0")
			_T("Icons (*.ico;*.cur)\0*.ico;*.cur\0")
			_T("所有文件(*.*)\0*.*\0");
		ofn.nFilterIndex         = 1L ;
		ofn.lpstrFile            = szFileName ;
		ofn.nMaxFile             = _MAX_PATH;
		ofn.lpstrFileTitle       = szFileTitle ;
		ofn.nMaxFileTitle        = _MAX_FNAME;
		ofn.lpstrTitle  =	_T("加载图片"); ;
		ofn.Flags                = OFN_PATHMUSTEXIST | OFN_SHOWHELP ;
	
		if (GetOpenFileName(&ofn))
		{
			USES_CONVERSION;
			BSTR bstr = T2OLE(ofn.lpstrFile);
			CComQIPtr<IDrawImage, &IID_IDrawImage> pIBmp(m_ppUnk[0]);
			pIBmp->AddImage(bstr);

			ResetIndexChoice();
			m_wndShow.Invalidate(TRUE);
		}		
		
		return 0;
	}
	LRESULT OnClickedButton_delete(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		HWND hwnd = GetDlgItem(IDC_COMBO_INDEX);
		int index = ::SendMessage(hwnd, CB_GETCURSEL, 0, 0);
		CComQIPtr<IDrawImage, &IID_IDrawImage> pIBmp(m_ppUnk[0]);
		pIBmp->DeleteImage(index);
		ResetIndexChoice();
		m_wndShow.Invalidate(TRUE);
		return 0;
	}
};

#endif //__CXIMAGEGENPAGE_H_
