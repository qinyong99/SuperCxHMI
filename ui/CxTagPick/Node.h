// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NODE_H_
#define __NODE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNode;
class CNetNode;

class CTag
{
public:
	CTag(CNode* pNode, LPCWSTR szName = NULL);
	virtual ~CTag();

	CSimpleArray<CComBSTR>& GetProps();
	void RefreshProps();

	BSTR GetQualityTag();

	HRESULT LoadFieldsOfTagType();

public:
	CComBSTR m_bstrName;
	CComBSTR m_bstrDesc;
	int m_nTagType;
	CComBSTR m_bstrDevice;
	CComBSTR m_bstrAddress;

	CSimpleArray<CComBSTR> m_arrProps;

	BOOL m_bRefreshProps;
	
	CNode* m_pNode;
};

class CNode
{
public:
	enum
	{
		nodeNone = 0,
		nodeRoot = 1,
		nodeNet = 2,
		nodeTag = 3,
	}; 

	CNode(LPCWSTR szName = NULL, CNetNode* pNetNode = NULL);
	virtual ~CNode();

public:
	CNode* FindChildNode(LPCWSTR szNode);
	CTag* FindTag(LPCWSTR szTag);

	CNode* FindMatchNode(LPCWSTR szTagGroup);
	CTag* FindMatchTag(LPCWSTR szTag);
	void AddChildNode(CNode* pNode);
	virtual void RefreshChildren();
	virtual void RefreshTags();

	virtual int GetNodeType() { return nodeNone; }
	
	CNode* GetParent() { return m_pParentNode; }

	virtual BSTR GetQualityTagGroup();

	void RemoveAll();

	CSimpleArray<CNode *>& GetChildNodes();
	CSimpleArray<CTag *>& GetTags();

public:
	BOOL m_bRefresh; //是否已经刷新过子节点
	BOOL m_bRefreshTag; //是否已经刷新过标签

	CComBSTR m_bstrName;

	CNode* m_pParentNode;
	CNetNode* m_pNetNode;
	CSimpleArray<CNode *> m_apChildNodes; //子节点
	CSimpleArray<CTag *> m_apTags; //标签

	DWORD m_dwID;
};

//////////////////////////////////////////////////////////
//网络节点
class CRootNode : public CNode
{
public:
	CRootNode(BOOL bUseLocalSetup = FALSE);
	virtual ~CRootNode();

	virtual int GetNodeType() { return nodeRoot; }

	virtual void RefreshChildren();

	HRESULT LoadChildrenFormSetup(); 
	HRESULT LoadChildrenFormServer(); 

	CNetNode* m_pOnlyNetNode;

	BOOL m_bUseLocalSetup;
};

//////////////////////////////////////////////////////////
//网络节点
class CNetNode : public CNode  
{
public:
	CNetNode(LPCWSTR szName = NULL, LPCWSTR szComputer = NULL, 
		BOOL bUseSACProxy = FALSE, long lSACProxyPort = 8081);
	virtual ~CNetNode();

	virtual int GetNodeType() { return nodeNet; }

	virtual void RefreshChildren();
	virtual void RefreshTags();
	
	

	ICxDataServer* GetDataServer();
	ICxBrowseServerAddressSpace* GetBrowseServerAddressSpace();

	BOOL Login(LPCWSTR szUser, LPCWSTR szPassword);

	HRESULT LoadLocalNodeFormSetup();
	
public:
	CComPtr<ICxDataServer> m_spDataServer;
	CComPtr<ICxBrowseServerAddressSpace> m_spTagBrowser;
	
	BOOL m_bUseSACProxy;

	CComBSTR m_bstrComputer; //计算计名称
	long m_lSACProxyPort;
	CComBSTR m_bstrUserName;
	CComBSTR m_bstrPassword;
};

//////////////////////////////////////////////////////////
//标签节点
class CTagNode : public CNode

{
public:
	CTagNode(LPCWSTR szName = NULL, CNetNode* pNetNode = NULL);
	virtual ~CTagNode();

	virtual int GetNodeType() { return nodeTag; }
	virtual void RefreshChildren();
	virtual void RefreshTags();

	CComBSTR m_bstrTagGroup;
};

extern CNode* FindMatchNode(CNode* pNode, LPCWSTR szTagGroup);
extern CTag* FindMatchTag(CNode* pNode, LPCWSTR szTag);

#endif // __NODE_H_