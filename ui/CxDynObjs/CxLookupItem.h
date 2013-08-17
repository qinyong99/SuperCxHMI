// CxLookupItem.h: interface for the CLookup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOOKUP_H__3ED2DD18_515E_4E41_B43F_705CC0FC0A29__INCLUDED_)
#define AFX_LOOKUP_H__3ED2DD18_515E_4E41_B43F_705CC0FC0A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLookupPage;

class CLookupItem  
{
public:
	CLookupPage* m_pLookupPage;

	CComVariant m_vaInput1;
	CComVariant m_vaInput2;
	CComVariant m_vaOutput;

public:
	CLookupItem()
	{
	}
	CLookupItem(CLookupPage* pLookupPage);
	virtual ~CLookupItem();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);

	virtual HRESULT OnClickOutput(RECT, POINT pt);
	virtual HRESULT OnDblClickOutput(RECT, POINT pt);

	void DrawEllipsisButton(CDCHandle dc, CRect rect);
	HRESULT HelperGetText(CComVariant var, CComBSTR& bstr);
};

class CLookupItemColor : public CLookupItem  
{
public:
	CLookupItemColor()
	{
	}
	CLookupItemColor(CLookupPage* pLookupPage);
	virtual ~CLookupItemColor();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);
};

class CLookupItemEnum : public CLookupItem
{
public:
	CLookupItemEnum(CLookupPage* pLookupPage);
	virtual ~CLookupItemEnum();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);
	virtual HRESULT OnDblClickOutput(RECT, POINT pt);
};

class CLookupItemEnumString : public CLookupItemEnum
{
public:
	CLookupItemEnumString(CLookupPage* pLookupPage);
	virtual ~CLookupItemEnumString();

	virtual HRESULT OnClickOutput(RECT, POINT pt);
	virtual HRESULT OnDblClickOutput(RECT, POINT pt);
};

class CLookupItemBool : public CLookupItem
{
public:
	CLookupItemBool(CLookupPage* pLookupPage);
	virtual ~CLookupItemBool();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);
	virtual HRESULT OnDblClickOutput(RECT, POINT pt);
};

class CLookupItemEdit : public CLookupItem
{
public:
	
	CLookupItemEdit(CLookupPage* pLookupPage);
	virtual ~CLookupItemEdit();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemFakeFont : public CLookupItem
{
public:
	
	CLookupItemFakeFont(CLookupPage* pLookupPage);
	virtual ~CLookupItemFakeFont();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemFakeFontName : public CLookupItem
{
public:
	
	CLookupItemFakeFontName(CLookupPage* pLookupPage);
	virtual ~CLookupItemFakeFontName();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemFile : public CLookupItem
{
public:
	
	CLookupItemFile(CLookupPage* pLookupPage);
	virtual ~CLookupItemFile();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemBrushFile : public CLookupItemFile
{
public:
	
	CLookupItemBrushFile(CLookupPage* pLookupPage);
	virtual ~CLookupItemBrushFile();

	virtual HRESULT OnClickOutput(RECT, POINT pt);
};

class CLookupItemFolder : public CLookupItem
{
public:
	
	CLookupItemFolder(CLookupPage* pLookupPage);
	virtual ~CLookupItemFolder();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemFakePicture : public CLookupItem
{
public:
	
	CLookupItemFakePicture(CLookupPage* pLookupPage);
	virtual ~CLookupItemFakePicture();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemTagGroup : public CLookupItem
{
public:
	
	CLookupItemTagGroup(CLookupPage* pLookupPage);
	virtual ~CLookupItemTagGroup();

	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);

};

class CLookupItemSecurityArea : public CLookupItem
{
public:
	
	CLookupItemSecurityArea(CLookupPage* pLookupPage);
	virtual ~CLookupItemSecurityArea();
	
	virtual HRESULT OnDrawOutput(CDCHandle dc, CRect rect, BOOL bSelected);
	virtual HRESULT OnClickOutput(RECT, POINT pt);
};

#endif // !defined(AFX_LOOKUP_H__3ED2DD18_515E_4E41_B43F_705CC0FC0A29__INCLUDED_)
