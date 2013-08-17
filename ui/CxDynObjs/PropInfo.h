// PropInfo.h
//本文件用于定义在编辑动态属性时需要传递的类型信息

#ifndef __CXDYNAMICINFO_H_
#define __CXDYNAMICINFO_H_

#include "CxHmiMisc.h"

#define CSTATE_DISABLE -1
#define CSTATE_DISCONNECT_NOTSUPPORTMOUSEINPUT	0x0  //没有关联，不支持鼠标输入
#define CSTATE_DISCONNECT_SUPPORTMOUSEINPUT	0x1	 //没有关联，支持鼠标输入
#define CSTATE_CONNECT_NOTSUPPORTMOUSEINPUT	0x2	 //关联，不支持鼠标输入
#define CSTATE_CONNECT_SUPPORTMOUSEINPUT		0x3  //关联，支持鼠标输入
#define CSTATE_CONNECT_ENABLEMOUSEINPUT		0x7  //关联，支持鼠标输入并使能


//iFlag == 0; //创建表达式
//iFlag == 1; //由原始表达式到显示表达式
//iFlag == 2; //由显示表达式到原始表达式
typedef void (*ExpressionCreatorProc)(BSTR* pbstrExpression, int iFlag = 0);

class CPropInfo
{
public:
	CComBSTR m_bstrName;
	CComBSTR m_bstrDesc;
	enumPropType m_proptype;
	DISPID m_dispid;
	CComVariant m_value;
public:
	CPropInfo()
	{
		m_dispid = 0;
		m_proptype = PROPTYPE_UNKNOWN;
	}

	CPropInfo(const CPropInfo& propinfo)
	{
		InternalCopy(propinfo);
	}
	
	CPropInfo& operator = (CPropInfo& propinfo)
	{
		InternalCopy(propinfo);
		return *this;
	}

	void InternalCopy(const CPropInfo& propinfo)
	{
		m_bstrName = propinfo.m_bstrName;
		m_bstrDesc = propinfo.m_bstrDesc;
		m_proptype = propinfo.m_proptype;
		m_dispid = propinfo.m_dispid;
		m_value = propinfo.m_value;
	}
};

typedef CSimpleArray<CPropInfo> CPropInfoArray;

class CEnumPropItem
{
public:
	CComBSTR m_bstrName;
	CComBSTR m_bstrDesc;
	CComVariant m_varValue;
	
	CEnumPropItem()
	{
	}
	
	CEnumPropItem(const CEnumPropItem& item)
	{
		InternalCopy(item);
	}
	
	CEnumPropItem& operator = (CEnumPropItem& item)
	{
		InternalCopy(item);
		return *this;
	}
	
	void InternalCopy(const CEnumPropItem& item)
	{
		m_bstrName = item.m_bstrName;
		m_bstrDesc = item.m_bstrDesc;
		m_varValue = item.m_varValue;
	}
};

typedef CSimpleArray<CEnumPropItem> CEnumPropItemArray;

class CDynamicPropInfo
{
public:
	CComPtr<IUnknown> spUnkObject; //对象
	CComPtr<IUnknown> spUnkDynamic; //动态属性对象
	ExpressionCreatorProc pfnExpression;
	CComBSTR bstrPropertyName;	//属性名称
	CComVariant varCurrentValue; //当前值
	BOOL bSupportMouseInput;
	enumPropType nType;
	CEnumPropItemArray m_arrEnumPropItems;
};


#endif //__CXDYNAMICINFO_H_