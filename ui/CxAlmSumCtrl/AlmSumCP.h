// AlmSumCP.h: interface for the CAlarm class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __ALMSUMCP_H_
#define __ALMSUMCP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T>
class CProxy_ICxAlmSumEvents : public CProxy_ICxObjectBaseEvents<T, &DIID__ICxAlmSumCtrlEvents>
{
public:
	VOID Fire_AlarmAcknowledge(BSTR Source)
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
				pvars[0] = Source;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				pDispatch->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
			}
		}
		delete[] pvars;
	}
};

#endif // __ALMSUMCP_H_