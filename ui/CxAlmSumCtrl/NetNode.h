// NetNode.h: interface for the CNetNode class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NETNODE_H_
#define __NETNODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAlmSum;
class CAlarm;

class CNetNode : public CComObjectRoot
{
public:
	CNetNode(LPCWSTR szName = NULL, LPCWSTR szComputer = NULL);
	virtual ~CNetNode();

BEGIN_COM_MAP(CNetNode)
END_COM_MAP()
		
DECLARE_NOT_AGGREGATABLE(CNetNode)

public:
	ICxDataServer* GetDataServer();

	CComBSTR m_bstrName; //节点名称
	CComBSTR m_bstrComputer; //计算计名称
	CComPtr<ICxDataServer> m_spDataServer;
};

#endif // __NETNODE_H_
