// OPCServerPage.h: interface for the COPCServerPage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __OPCSERVERPAGE_H_
#define __OPCSERVERPAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"       // main symbols
#include "OPCServer.h"
#include "TreePage.h"

/////////////////////////////////////////////////////////////////////////////
// COPCServerPage
class COPCServerPage : public CTreePage  
{
public:
	COPCServerPage() 
	{
	}
	virtual ~COPCServerPage() 
	{
		for (int i = 0; i < m_apOPCServers.GetSize(); i++)
		{
			delete m_apOPCServers[i];
		}
		m_apOPCServers.RemoveAll();
	}

	virtual void OnInit();

	void LoadOPCServers();

	virtual void LoadObjectItems(HTREEITEM hParentItem);

private:

	CSimpleArray<COPCServer *> m_apOPCServers;
	
	CImageList m_ImageList;
};

#endif // __OPCSERVERPAGE_H_
