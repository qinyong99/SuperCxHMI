#ifndef _SCRIPTCREATORCP_H_
#define _SCRIPTCREATORCP_H_

template <class T>
class CProxy_ICxScriptCreatorEvents : public IConnectionPointImpl<T, &DIID__ICxScriptCreatorEvents, CComDynamicUnkArray>
{
public:
	HRESULT FireEvent(DISPID dispid)
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
				pDispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		return varResult.scode;
	}

	HRESULT Fire_PositionChanged()
	{
		return FireEvent(0x1);
	}

	HRESULT Fire_Modified()
	{
		return FireEvent(0x2);
	}

	HRESULT Fire_SymbolViewToggled()
	{
		return FireEvent(0x3);
	}

	HRESULT Fire_SymbolPageChanged()
	{
		return FireEvent(0x4);
	}
};

#endif // _SCRIPTCREATORCP_H_