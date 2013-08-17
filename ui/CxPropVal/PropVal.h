// PropVal.h : Declaration of the CPropVal

#ifndef __CXPROPERTYVALUE_H_
#define __CXPROPERTYVALUE_H_

#include "resource.h"       // main symbols
#include "CxHmiObj.h"
#include "CxHmiMisc.h"

template <class T>
class CProxy_IPropertyValueEvents : public IConnectionPointImpl<T, &DIID__IPropertyValueEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_ValueChanged()
	{
		CComVariant varResult;
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
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		return varResult.scode;
	}

	HRESULT Fire_BeginEdit()
	{
		CComVariant varResult;
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
				VariantClear(&varResult);
				DISPPARAMS disp = { NULL, NULL, 0, 0 };
				pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		return varResult.scode;
	}
};

typedef CContainedWindowT<CEdit> CContainedEdit;
typedef CContainedWindowT<CButton> CContainedButton;

/////////////////////////////////////////////////////////////////////////////
// CPropVal
class ATL_NO_VTABLE CPropVal : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICxPropertyValue, &IID_ICxPropertyValue, &LIBID_CXPROPVALLib>,
	public CComCompositeControl<CPropVal>,
	public IPersistStreamInitImpl<CPropVal>,
	public IOleControlImpl<CPropVal>,
	public IOleObjectImpl<CPropVal>,
	public IOleInPlaceActiveObjectImpl<CPropVal>,
	public IViewObjectExImpl<CPropVal>,
	public IOleInPlaceObjectWindowlessImpl<CPropVal>,
	public IPersistStorageImpl<CPropVal>,
	public ISpecifyPropertyPagesImpl<CPropVal>,
	public IQuickActivateImpl<CPropVal>,
	public IDataObjectImpl<CPropVal>,
	public IConnectionPointContainerImpl<CPropVal>,
	public IProvideClassInfo2Impl<&CLSID_CxPropertyValue, &DIID__IPropertyValueEvents, &LIBID_CXPROPVALLib>,
	public CComCoClass<CPropVal, &CLSID_CxPropertyValue>,
	public CProxy_IPropertyValueEvents<CPropVal>
{
public:
	CPropVal() :
		m_edit(this, 1),
		m_button(this, 2)
	{
		m_bWindowOnly = TRUE;
		CalcExtent(m_sizeExtent);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CXPROPERTYVALUE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPropVal)
	COM_INTERFACE_ENTRY(ICxPropertyValue)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()

BEGIN_PROP_MAP(CPropVal)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CPropVal)
	CONNECTION_POINT_ENTRY(DIID__IPropertyValueEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CPropVal)
	CHAIN_MSG_MAP(CComCompositeControl<CPropVal>)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_BUTTON_VALUE, BN_CLICKED, OnClickedButton_value)
	COMMAND_HANDLER(IDC_EDIT_VALUE, EN_KILLFOCUS, OnKillfocusEdit_value)
	COMMAND_HANDLER(IDC_EDIT_VALUE, EN_SETFOCUS, OnSetfocusEdit_value)
ALT_MSG_MAP(1) 
	MESSAGE_HANDLER(WM_PAINT, OnPaintEdit)
ALT_MSG_MAP(2)	
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

BEGIN_SINK_MAP(CPropVal)
	//Make sure the Event Handlers have __stdcall calling convention
END_SINK_MAP()

	STDMETHOD(OnAmbientPropertyChange)(DISPID dispid)
	{
		if (dispid == DISPID_AMBIENT_BACKCOLOR)
		{
			SetBackgroundColorFromAmbient();
			FireViewChange();
		}
		return IOleControlImpl<CPropVal>::OnAmbientPropertyChange(dispid);
	}



// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// ICxPropertyValue
public:
	enum { IDD = IDD_CXPROPERTYVALUE };

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_edit.SubclassWindow(GetDlgItem(IDC_EDIT_VALUE));
		m_button.SubclassWindow(GetDlgItem(IDC_BUTTON_VALUE));
		HBITMAP hImage = (HBITMAP)::LoadImage(_Module.m_hInstResource, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 16, 16, LR_CREATEDIBSECTION|LR_DEFAULTSIZE|LR_LOADMAP3DCOLORS); 
		m_button.SetBitmap(hImage);

		return 0;
	}
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CRect rc(0, 0, LOWORD(lParam), HIWORD(lParam));
		m_edit.MoveWindow(0, 0, rc.Width() - 16, rc.Height());
		m_button.MoveWindow(rc.Width() - 14, 0, 12, rc.Height());

		return 0;
	}
	LRESULT OnPaintEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedButton_value(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	STDMETHOD(get_DefaultValue)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_DefaultValue)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Object)(/*[out, retval]*/ IUnknown* *pVal);
	STDMETHOD(put_Object)(/*[in]*/ IUnknown* newVal);
	STDMETHOD(Reset)();
	STDMETHOD(AddEnumItem)(BSTR bstrDesc, VARIANT varValue);
	STDMETHOD(get_Value)(/*[out, retval]*/ VARIANT *pVal);
	STDMETHOD(put_Value)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_Type)(/*[out, retval]*/ long *pVal);
	STDMETHOD(put_Type)(/*[in]*/ long newVal);

	CContainedEdit m_edit;
	CContainedButton m_button;

	enumPropType m_type;
	CComVariant m_value;
	CComVariant m_valueDefault;

	CComPtr<IUnknown> m_spObject;

	CSimpleValArray<CComBSTR> m_arrDescs;
	CSimpleValArray<CComVariant> m_arrValues;

	LRESULT OnKillfocusEdit_value(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		Fire_ValueChanged();
		return 0;
	}
	LRESULT OnSetfocusEdit_value(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		Fire_BeginEdit();
		return 0;
	}
};

#endif //__CXPROPERTYVALUE_H_
