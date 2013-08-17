// PropertyPageImpl2.h: Definition of the IPropertyPageImpl2 class
//
//////////////////////////////////////////////////////////////////////

#ifndef _PROPERTYPAGEIMPL2_H__
#define _PROPERTYPAGEIMPL2_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogSizeHelper.h"

template<class T>
class IPropertyPageImpl2 : public IPropertyPageImpl<T>
{
protected:
	STDMETHOD(GetPageInfo)(PROPPAGEINFO *pPageInfo)
	{
		T* pT = static_cast<T*>(this);
		ATLTRACE2(atlTraceControls,2,_T("IPropertyPageImpl::GetPageInfo\n"));
		
		if (pPageInfo == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Error : PROPPAGEINFO passed == NULL\n"));
			return E_POINTER;
		}
		
		HRSRC hRsrc = FindResource(_Module.GetResourceInstance(),
			MAKEINTRESOURCE(T::IDD), RT_DIALOG);
		if (hRsrc == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Could not find resource template\n"));
			return E_UNEXPECTED;
		}
		
		HGLOBAL hGlob = LoadResource(_Module.GetResourceInstance(), hRsrc);
		DLGTEMPLATE* pDlgTempl = (DLGTEMPLATE*)LockResource(hGlob);
		if (pDlgTempl == NULL)
		{
			ATLTRACE2(atlTraceControls,2,_T("Could not load resource template\n"));
			return E_UNEXPECTED;
		}
		CDialogSizeHelper::GetDialogSize2(pDlgTempl, &m_size);
		
		pPageInfo->cb = sizeof(PROPPAGEINFO);
		pPageInfo->pszTitle = LoadStringHelper(pT->m_dwTitleID);
		pPageInfo->size = m_size;
		pPageInfo->pszHelpFile = LoadStringHelper(pT->m_dwHelpFileID);
		pPageInfo->pszDocString = LoadStringHelper(pT->m_dwDocStringID);
		pPageInfo->dwHelpContext = pT->m_dwHelpContext;
		
		return S_OK;
	}
};

#endif // _PROPERTYPAGEIMPL2_H__
