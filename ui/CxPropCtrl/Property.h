// Property.h: interface for the CProperty class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PROPERTY_H__
#define _PROPERTY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CxDynObjs\PropInfo.h"

EXTERN_C const CLSID CLSID_CxDataConnectionPage;
EXTERN_C const CLSID CLSID_CxDataRecordsetPage;

class CPropCtrl;

class CProperty  
{
public:
	CProperty(CPropCtrl* pPropCtrl); 
	virtual ~CProperty(){}

	virtual HRESULT Init() = 0;	
	virtual HRESULT OnPaint(CDCHandle dc, CRect rect, BOOL bSelected) = 0;
	virtual HRESULT OnClick(CRect rect, CPoint pt, int nIndex) = 0;
	virtual HRESULT OnDblClick(CRect rect, int nIndex) = 0;
	virtual HRESULT GetText(CComBSTR& bstr, BOOL& bSame) = 0;	
	virtual HRESULT SetText(CComBSTR& bstr) = 0;

	virtual bool VerifyClass(long uniqueID) = 0;
	virtual enumPropType GetPropType() { return PROPTYPE_UNKNOWN; }

	LPCWSTR GetName() const { return m_name; }
	LPCWSTR GetDesc() const { return m_desc; }
	DISPID GetDISPID() const { return m_dispid; }

public:
	HRESULT HelperGetValue(CComVariant& var, BOOL& bSame);	
	HRESULT HelperPutValue(CComVariant& var);
	
	int GetConnectState();

	void DrawItemText(CDCHandle dc, LPCWSTR lpszText, unsigned int cbSize, CRect rect, 
		COLORREF clrBack = GetSysColor(COLOR_WINDOW), COLORREF clrText = GetSysColor(COLOR_WINDOWTEXT), 
		UINT uFormat = DT_NOPREFIX | DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	void DrawEllipsisButton(CDCHandle dc, CRect rect);

	CComBSTR m_name;
	CComBSTR m_desc;	
	DISPID m_dispid;
	long m_lRef;  //指示有多少对象共有此属性
	BOOL m_bHasPutRef;
	BOOL m_bHasPut;
	BOOL m_bExtend;
	CPropCtrl* m_pPropCtrl;
};

class CPropertyBlank :
	public CProperty
{
public:
	CPropertyBlank(CPropCtrl* pPropCtrl) 
		: CProperty(pPropCtrl) {}
	
	HRESULT Init() { return S_OK; }	
	virtual HRESULT OnPaint(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClick(CRect rect, CPoint pt, int nIndex) { return S_FALSE; }	
	virtual HRESULT OnDblClick(CRect rect, int nIndex) { return S_FALSE; }	

	virtual HRESULT GetText(CComBSTR& bstr, BOOL& bSame);	
	virtual HRESULT SetText(CComBSTR& bstr);
	
	bool VerifyClass(long uniqueID) { return uniqueID & 1; }
	
protected:
	virtual void OnDrawName(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual void OnDrawConnectState(CDCHandle dc, CRect rect, BOOL bSelected);
};

class CPropertyEdit :
	public CPropertyBlank
{
public:
	CPropertyEdit(CPropCtrl* pPropCtrl, enumPropType type = PROPTYPE_NUMBER) 
		: CPropertyBlank(pPropCtrl) { m_type = type;}
	
	virtual HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	virtual HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return m_type; }
	bool VerifyClass(long uniqueID) 
	{
		if ((uniqueID & 2) != 0) return true;
		return CPropertyBlank::VerifyClass(uniqueID);		
	}
	
	virtual HRESULT GetText(CComBSTR& bstr, BOOL& bSame);	
	virtual HRESULT SetText(CComBSTR& bstr);	
protected:
	enumPropType m_type;
};

class CPropertyEnum :
	public CPropertyBlank
{
public:
	CPropertyEnum(CPropCtrl* pPropCtrl) 
		: CPropertyBlank(pPropCtrl) 
	{
		m_bCanEdit = false;
	}
	
	HRESULT Fill(IUnknown *pUnk);

	virtual HRESULT SetText(CComBSTR& bstr);	
	
	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return PROPTYPE_ENUM; }
	bool VerifyClass(long uniqueID) 
	{
		if ((uniqueID & 4) != 0) return true;
		if (m_bCanEdit && (uniqueID & 2) != 0) return true;
		return CPropertyBlank::VerifyClass(uniqueID);		
	}
	
protected:
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);
	
	CEnumPropItemArray m_arrEnumPropItems;
	bool m_bCanEdit;

	friend class CPropCtrl;
};

class CPropertyBool :
	public CPropertyBlank
{
public:
	CPropertyBool(CPropCtrl* pPropCtrl) 
		: CPropertyBlank(pPropCtrl) {}
	
	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return PROPTYPE_BOOL; }
	bool VerifyClass(long uniqueID) 
	{
		if ((uniqueID & 4) != 0) return true;
		return CPropertyBlank::VerifyClass(uniqueID);		
	}
	
protected:
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);
};

class CPropertyColor :
	public CPropertyEdit
{
public:
	CPropertyColor(CPropCtrl* pPropCtrl) 
		: CPropertyEdit(pPropCtrl) {}
	
	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return PROPTYPE_COLOR; }
	bool VerifyClass(long uniqueID) 
	{
		if ((uniqueID & 4) != 0) return true;
		return CPropertyEdit::VerifyClass(uniqueID);		
	}
	
	virtual HRESULT GetText(CComBSTR& bstr, BOOL& bSame);	
	virtual HRESULT SetText(CComBSTR& bstr);	
	
protected:
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);
};

class CPropertyPropPage :
	public CPropertyBlank
{
public:
	CPropertyPropPage(CPropCtrl* pPropCtrl, const CLSID& clsidPage) 
		: CPropertyBlank(pPropCtrl) { m_clsidPage = clsidPage; }
	
	HRESULT Init();
	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	
	virtual bool OnClickImpl();
	
	bool VerifyClass(long uniqueID) 
	{
		if ((uniqueID & 8) != 0) return true;
		return CPropertyBlank::VerifyClass(uniqueID);		
	}
	virtual enumPropType GetPropType() { return PROPTYPE_PROPPAGE; }
	
protected:	
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);
	static DWORD WINAPI ThreadProc(void* pv);
	
	CLSID m_clsidPage;
};

class CPropertyPicture :
	public CPropertyPropPage
{
public:
	CPropertyPicture(CPropCtrl* pPropCtrl) 
		: CPropertyPropPage(pPropCtrl, CLSID_StockPicturePage) {}		
protected:
	HRESULT GetText(CComBSTR& bstr, BOOL& bSame);
};

class CPropertyDataConnection :
	public CPropertyPropPage
{
public:
	CPropertyDataConnection(CPropCtrl* pPropCtrl) 
		: CPropertyPropPage(pPropCtrl, CLSID_CxDataConnectionPage) {}	
	
	virtual bool OnClickImpl();
protected:
	HRESULT GetText(CComBSTR& bstr, BOOL& bSame);
};

class CPropertyDataRecordset :
	public CPropertyPropPage
{
public:
	CPropertyDataRecordset(CPropCtrl* pPropCtrl) 
		: CPropertyPropPage(pPropCtrl, CLSID_CxDataRecordsetPage) {}	
	
	virtual bool OnClickImpl();
protected:
	HRESULT GetText(CComBSTR& bstr, BOOL& bSame);
};

class CPropertyPredefinedValue :	
	public CPropertyEnum
{
public:
	CPropertyPredefinedValue(CPropCtrl* pPropCtrl) 
		: CPropertyEnum(pPropCtrl) 
	{
		m_bCanEdit = TRUE;
	}

	virtual enumPropType GetPropType() { return PROPTYPE_ENUMSTRING; }
	
	HRESULT Fill(IUnknown *pUnk);	
};

class CPropertyCustom :
	public CPropertyEdit
{
public:
	CPropertyCustom(CPropCtrl* pPropCtrl) 
		: CPropertyEdit(pPropCtrl) {}

	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return PROPTYPE_UNKNOWN; }

protected:
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);

	virtual bool OnClickImpl() = 0;
	virtual bool IsEditable() const { return true; }
};

class CPropertyFile :
	public CPropertyCustom
{
public:
	CPropertyFile(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) { }
	
	virtual enumPropType GetPropType() { return PROPTYPE_FAKEFILE; }
protected:	
	virtual bool OnClickImpl();
};

class CPropertyBrushFile :
	public CPropertyCustom
{
public:
	CPropertyBrushFile(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) { }
	
	virtual enumPropType GetPropType() { return PROPTYPE_FAKEBRUSHFILE; }
protected:	
	virtual bool OnClickImpl();
};

class CPropertyFolder :
	public CPropertyCustom
{
public:
	CPropertyFolder(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) {}

	virtual enumPropType GetPropType() { return PROPTYPE_FAKEFOLDER; }
protected:	
	bool OnClickImpl();
};

class CPropertyFakePicture :
	public CPropertyCustom
{	
public:
	CPropertyFakePicture(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) {}

	virtual HRESULT SetText(CComBSTR& bstr);
	virtual HRESULT GetText(CComBSTR& bstr, BOOL& bSame);

	virtual enumPropType GetPropType() { return PROPTYPE_FAKEPICTURE; }
protected:	
	bool OnClickImpl();
};

class CPropertyFakeFont :
	public CPropertyCustom
{
public:
	CPropertyFakeFont(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) {}
	virtual enumPropType GetPropType() { return PROPTYPE_FAKEFONT; }
protected:
	bool OnClickImpl();
	bool IsEditable() const { return false; }
};

class CPropertyFakeFontName :
	public CPropertyEdit
{
public:
	CPropertyFakeFontName(CPropCtrl* pPropCtrl) 
		: CPropertyEdit(pPropCtrl) {}
protected:
	
	HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	HRESULT OnDblClick(CRect rect, int nIndex);
	virtual enumPropType GetPropType() { return PROPTYPE_FAKEFONTNAME; }
	bool VerifyClass(long uniqueID) 
	{
//		if ((uniqueID & 4) != 0) return true;
		return CPropertyEdit::VerifyClass(uniqueID);		
	}

protected:
	void OnDrawValue(CDCHandle dc, CRect rect, BOOL bSelected);	
};

class CPropertyTagGroup :
	public CPropertyCustom
{
public:
	CPropertyTagGroup(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) {}

	virtual enumPropType GetPropType() { return PROPTYPE_TAGGROUP; }
protected:	
	bool OnClickImpl();
};

class CPropertySecurityArea :
	public CPropertyCustom
{
public:
	CPropertySecurityArea(CPropCtrl* pPropCtrl) 
		: CPropertyCustom(pPropCtrl) {}
	
	virtual enumPropType GetPropType() { return PROPTYPE_SECURITYAREA; }
protected:	
	bool OnClickImpl();
};

class CPropertyArray
{
public:
	CSimpleValArray<CProperty *> m_Array;
	
public:
	virtual void AddProperty(CProperty* pprop, BOOL bPutRef = FALSE);
	virtual int Fill_ListView(HWND hList, int iIndex = 0, BOOL bCalNum = FALSE);
	
protected:
	void ClearArray();	
};

class CPropertyCat : 
	public CProperty,
	public CPropertyArray
{
	static LPCWSTR GetCategoryName(PROPCAT pcat);
		
	const PROPCAT m_propcat;
	bool m_bExpand;
	
public:
	CPropertyCat(CPropCtrl* pPropCtrl, ICategorizeProperties* pCP, PROPCAT propcat);
	virtual ~CPropertyCat();
	
	HRESULT Init() {return E_NOTIMPL;}
	virtual HRESULT OnPaint(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClick(CRect rect, CPoint pt, int nIndex);
	virtual HRESULT OnDblClick(CRect rect, int nIndex);
	
	HRESULT GetText(CComBSTR& bstr, BOOL& bSame) {return E_NOTIMPL;}
	HRESULT SetText(CComBSTR& bstr) {return E_NOTIMPL;}	
	bool VerifyClass(long uniqueID) {return (uniqueID & 0x10) != 0;}
	
	bool isExpand() const {return m_bExpand;}
public:	
	PROPCAT GetPropCat() const {return m_propcat;}
	
protected:
	void Collapse(int iIndex);
	void Expand(int iIndex);
};

#endif // _PROPERTY_H__
