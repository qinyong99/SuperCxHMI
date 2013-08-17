////////////////////////////////////////////////////////////////////////////////////////////////////
//CxComVariant.h : user-defined VARIANTC class, the define and implement file
//所属项目: AutoCx动态属性页
//创建日期: 2001年11月
//创 建 人: 卢文江
//功能说明: 重载CComVariant类实现VARIANT变量的管理
//修改记录:
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "atlbase.h"

class CxComVariant : public CComVariant
{
public:
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//功能: 重载<操用符
	//说明: 此操作符具有方向性,如var1 < var2,是将var2.vt转化为var1.vt再比较相应的值
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool operator<=(const VARIANT& varSrc) const
	{
		//如果为pointer或数组类型则返回false
		if ((vt & VT_BYREF) == VT_BYREF || (vt & VT_ARRAY) == VT_ARRAY)
			return false;
		
		//两操作数相等
		if (this == &varSrc)
			return false;
				
		switch (vt)
		{
		
		case VT_ERROR:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		case VT_EMPTY:
		case VT_NULL:
			return false;

		case VT_I1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_CY:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:
		case VT_BOOL:
		case VT_DATE:
			{
				CComVariant varLeft(*this);
				CComVariant varRight(varSrc);
				::VariantChangeType(&varLeft, &varLeft, 0, VT_R8);
				::VariantChangeType(&varRight, &varRight, 0, VT_R8);
				return varLeft.dblVal <= varRight.dblVal;
			}
		case VT_BSTR:
			{
				if (bstrVal == NULL && varSrc.bstrVal == NULL)
					return false;
				if (bstrVal != NULL && varSrc.bstrVal != NULL)
					return wcscmp(bstrVal, varSrc.bstrVal) <= 0;
			}

		default:
			ATLASSERT(false);
			// fall through
		}
		
		return false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//功能: 重载>操用符
	//说明: 此操作符具有方向性,如var1 > var2,是将var2.vt转化为var1.vt再比较相应的值
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	bool operator>=(const VARIANT& varSrc) const
	{
		//如果为pointer或数组类型则返回false
		if ((vt & VT_BYREF) == VT_BYREF || (vt & VT_ARRAY) == VT_ARRAY)
			return false;
		
		//两操作数相等
		if (this == &varSrc)
			return false;
				
		switch (vt)
		{
		
		case VT_ERROR:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		case VT_EMPTY:
		case VT_NULL:
			return false;

		case VT_I1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_CY:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:
		case VT_BOOL:
		case VT_DATE:
			{
				CComVariant varLeft(*this);
				CComVariant varRight(varSrc);
				::VariantChangeType(&varLeft, &varLeft, 0, VT_R8);
				::VariantChangeType(&varRight, &varRight, 0, VT_R8);
				return varLeft.dblVal >= varRight.dblVal;
			}
		case VT_BSTR:
			{
				if (bstrVal == NULL && varSrc.bstrVal == NULL)
					return false;
				if (bstrVal != NULL && varSrc.bstrVal != NULL)
					return wcscmp(bstrVal, varSrc.bstrVal) >= 0;
			}

		default:
			ATLASSERT(false);
			// fall through
		}
		
		return false;
	}

	bool operator==(const VARIANT& varSrc) const
	{
		//如果为pointer或数组类型则返回false
		if ((vt & VT_BYREF) == VT_BYREF || (vt & VT_ARRAY) == VT_ARRAY)
			return false;
		
		//两操作数相等
		if (this == &varSrc)
			return false;
				
		switch (vt)
		{
		
		case VT_ERROR:
		case VT_UNKNOWN:
		case VT_DISPATCH:
		case VT_EMPTY:
		case VT_NULL:
			return false;

		case VT_I1:
		case VT_I2:
		case VT_I4:
		case VT_R4:
		case VT_R8:
		case VT_CY:
		case VT_UI1:
		case VT_UI2:
		case VT_UI4:
		case VT_INT:
		case VT_UINT:
		case VT_BOOL:
		case VT_DATE:
			{
				CComVariant varLeft(*this);
				CComVariant varRight(varSrc);
				::VariantChangeType(&varLeft, &varLeft, 0, VT_R8);
				::VariantChangeType(&varRight, &varRight, 0, VT_R8);
				return varLeft.dblVal == varRight.dblVal;
			}
		case VT_BSTR:
			{
				if (bstrVal == NULL && varSrc.bstrVal == NULL)
					return false;
				if (bstrVal != NULL && varSrc.bstrVal != NULL)
					return wcscmp(bstrVal, varSrc.bstrVal) == 0;
			}

		default:
			ATLASSERT(false);
			// fall through
		}
		
		return false;
	}
};