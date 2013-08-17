#ifndef __CXCATPROP_H_
#define __CXCATPROP_H_

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

#define PROPCAT_Nil -1 
#define PROPCAT_Misc -2 
#define PROPCAT_Font -3 
#define PROPCAT_Position -4 
#define PROPCAT_Appearance -5 
#define PROPCAT_Behavior -6 
#define PROPCAT_Data -7 
#define PROPCAT_List -8 
#define PROPCAT_Text -9 
#define PROPCAT_Scale -10 
#define PROPCAT_DDE -11 


//const IID IID_ICategorizeProperties = {0x4D07FC10,0xF931,0x11CE,{0xB0,0x01,0x00,0xAA,0x00,0x68,0x84,0xE5}}; 

typedef int PROPCAT;

//struct __declspec(uuid("4d07fc10-f931-11ce-b001-00aa006884e5")) __declspec(novtable)
//ICategorizeProperties : public IUnknown
//{
//	virtual HRESULT __stdcall MapPropertyToCategory(
//			/*[in]*/  DISPID dispid,
//			/*[out]*/ PROPCAT* ppropcat) = 0 ;

//	virtual HRESULT __stdcall GetCategoryName(
//			/*[in]*/  PROPCAT propcat,
//			/*[in]*/  LCID lcid,
//			/*[out]*/ BSTR* pbstrName) = 0 ;
//};

struct _CategoryPropNameEntry
{
	PROPCAT propcat;	
	TCHAR* pstr;
};

struct _PropertyToCategoryEntry
{
	DISPID dispid;
	PROPCAT propcat;	
};

#define BEGIN_CATEGORY_PROP_NAME_MAP(x)\
   static _CategoryPropNameEntry* GetCategoryPropNameEntry() {\
   static _CategoryPropNameEntry pMap[] = {
#define CATEGORY_PROP_NAME( propcat, name ) { propcat, name },
#define END_CATEGORY_PROP_NAME_MAP() {0, 0}}; return( &pMap[0] ); }

#define BEGIN_PROPERTY_TO_CATEGORY_NAME_MAP(x)\
   static _PropertyToCategoryEntry* GetPropertyToCategoryEntry() {\
   static _PropertyToCategoryEntry pMap[] = {
#define PROPERTY_TO_CATEGORY( dispid, propcat ) { dispid, propcat },
#define END_PROPERTY_TO_CATEGORY_MAP() {0, 0}}; return( &pMap[0] ); }


template <class T>
class ATL_NO_VTABLE ICxCategorizePropertiesImpl :
	public ICategorizeProperties
{
public:
	
	STDMETHOD(MapPropertyToCategory)(DISPID dispid, PROPCAT* ppropcat )
	{
		_PropertyToCategoryEntry* pEntry = T::GetPropertyToCategoryEntry();
		while (pEntry->propcat) 
		{
			if (pEntry->dispid == dispid)
			{
				*ppropcat = pEntry->propcat;		
				return S_OK;
			}			
			pEntry++;
		}
		
		return E_FAIL;
	}
	STDMETHOD(GetCategoryName)(PROPCAT propcat, LCID lcid, BSTR* pbstrName)
	{
		lcid; //not used
		USES_CONVERSION;
		
		_CategoryPropNameEntry* pEntry = T::GetCategoryPropNameEntry();
		while (pEntry->pstr)
		{			
			if (pEntry->propcat  == propcat)
			{
				*pbstrName = ::SysAllocString(T2W(pEntry->pstr)); 			
				return S_OK;
			}			
			pEntry++;
		}		
		return E_FAIL;
	}
};


#endif //__CXCATPROP_H_

