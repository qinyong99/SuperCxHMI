// OPCServer.h: interface for the COPCServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPCSERVER_H__BCC752CB_8D70_47DA_BAD3_914173024506__INCLUDED_)
#define AFX_OPCSERVER_H__BCC752CB_8D70_47DA_BAD3_914173024506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../drv/opc/opcda.h"

class COPCItem;
class COPCGroup;
class COPCServer;

class COPCItem
{
public:
	COPCItem();
	virtual ~COPCItem();
	
public:

	CComBSTR m_bstrName; // ItemDataID
	CComBSTR m_bstrItemID;

	COPCGroup* m_pParentGroup;
	COPCServer* m_pServer;
};

class COPCGroup 
{
public:
	COPCGroup(COPCServer* pServer = NULL);
	virtual ~COPCGroup();
	
public:
	void RemoveAll();

	virtual void RefreshGroups();
	virtual void RefreshItems();

	CSimpleArray<COPCGroup *>& GetGroups();
	CSimpleArray<COPCItem *>& GetItems();
	
	BOOL m_bRefreshGroups; 
	BOOL m_bRefreshItems; 
	
	CSimpleArray<COPCGroup * > m_arrGroups;
	CSimpleArray<COPCItem *> m_arrItems;

	CComBSTR m_bstrName; // ItemDataID
	CComBSTR m_bstrItemID;

	COPCGroup* m_pParentGroup;
	COPCServer* m_pServer;
};

class COPCServer
{
public:
	COPCServer();
	virtual ~COPCServer();

public:
	IOPCServer* GetOPCServer();
	IOPCBrowseServerAddressSpace* GetBrowseServerAddressSpace();

	CComPtr<IOPCServer> m_spOPCServer;
	CComPtr<IOPCBrowseServerAddressSpace> m_spOPCBrowseServerAddressSpace;

	void RemoveAll(); 

	CComBSTR m_bstrName;
	CComBSTR m_bstrOPCServerName;
	CComBSTR m_bstrComputer;

	COPCGroup m_group;
};

#endif // !defined(AFX_OPCSERVER_H__BCC752CB_8D70_47DA_BAD3_914173024506__INCLUDED_)
