// ObjectCollection.cpp : implementation file
//

#include "stdafx.h"
#include "RunInc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectCollection

IMPLEMENT_DYNCREATE(CObjectCollection, CCmdTarget)

CObjectCollection::CObjectCollection()
{
	EnableAutomation();

	// To keep the application running as long as an OLE automation
	//  object is active, the constructor calls AfxOleLockApp.

	AfxOleLockApp();
}

CObjectCollection::~CObjectCollection()
{
	// To terminate the application when all objects created with
	//  with OLE automation, the destructor calls AfxOleUnlockApp.

	AfxOleUnlockApp();
}

void CObjectCollection::OnFinalRelease()
{
	// When the last reference for an automation object is released
	//  OnFinalRelease is called.  This implementation deletes the
	//  object.  Add additional cleanup required for your object before
	//  deleting it from memory.

	delete this;
}

BEGIN_MESSAGE_MAP(CObjectCollection, CCmdTarget)
	//{{AFX_MSG_MAP(CObjectCollection)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// {BA0357C0-C377-11cd-9A90-00DD01113F12}
//IMPLEMENT_OLECREATE(CObjectCollection, "mfc.inproc.strcoll",
//	0xba0357c0, 0xc377, 0x11cd, 0x9a, 0x90, 0x0, 0xdd, 0x1, 0x11, 0x3f, 0x12);

BEGIN_DISPATCH_MAP(CObjectCollection, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CObjectCollection)
	DISP_PROPERTY_EX(CObjectCollection, "Count", GetCount, SetNotSupported, VT_I4)
	DISP_PROPERTY_PARAM(CObjectCollection, "Item", GetItem, SetNotSupported, VT_VARIANT, VTS_I4)
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CObjectCollection, "_NewEnum", DISPID_NEWENUM, GetNewEnum, SetNotSupported, VT_UNKNOWN)
	DISP_DEFVALUE(CObjectCollection, "Item")
END_DISPATCH_MAP()

// {4B73D6FC-65A8-472d-A63E-4435121E9C89}
static const IID IID_IObjectCollection = { 0x4B73D6FC, 0x65A8, 0x472d,
	{ 0xA6, 0x3E, 0x44, 0x35, 0x12, 0x1E, 0x9C, 0x89 } };

// Note: we add support for IID_IStringCollect to support typesafe binding
// from VBA.  This IID must match the GUID that is attached to the
// dispinterface in the .ODL file.

BEGIN_INTERFACE_MAP(CObjectCollection, CCmdTarget)
	INTERFACE_PART(CObjectCollection, IID_IObjectCollection, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectCollection message handlers

long CObjectCollection::GetCount()
{	
	return m_arrObjects.GetSize();
}

VARIANT CObjectCollection::GetItem(long nIndex)
{
	VARIANT vaResult;
	VariantInit(&vaResult);
	
	if (nIndex > 0 && nIndex <= m_arrObjects.GetSize())
		VariantCopy(&vaResult, &m_arrObjects[nIndex - 1]);
	
	return vaResult;
}

LPUNKNOWN CObjectCollection::GetNewEnum()
{
	CEnumVariant* pEnum = new CEnumVariant;
	int nCount = m_arrObjects.GetSize();
	VARIANT* pContents = new VARIANT[nCount];
	int i;

	TRY
	{
		for (i = 0; i < nCount; ++i)
		{
			VariantInit(&pContents[i]);
			VariantCopy(&pContents[i], &m_arrObjects[i]);
		}
	}
	CATCH_ALL(e)
	{
		while (--i >= 0)
			VariantClear(&pContents[i]);

		THROW_LAST();
	}
	END_CATCH_ALL
	pEnum->SetContents(pContents, nCount);

	return pEnum->GetInterface(&IID_IUnknown);
}