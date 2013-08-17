// ExplorerBar.h: interface for the CExplorerBar class.
//
//////////////////////////////////////////////////////////////////////


#ifndef __EXPLORERBAR_H__
#define __EXPLORERBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//加入工具按此顺序加入
#define EXPBAR_EXPTREE	0		//工程工具代号
#define EXPBAR_MODULE	2		//模式工具代号
#define EXPBAR_SYMBOL	1		//符号工具代号
#define EXPBAR_NULL		3		//无效工具代号

class CExplorerBar : public CTabBar  
{
public:
	CExplorerBar();
	virtual ~CExplorerBar();

// Attributes
public:
	void DestroyAllChild();
	CWnd* GetToolWnd(int nWhich);
//	CExplorerTree m_wndExplorerTree;
//	CSymbolLibWnd m_wndSymbolLib;
//	CModuleWnd m_wndModuleLib;

// Overridables


protected:
    //{{AFX_MSG(CExplorerBar)
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
protected:
	virtual void OnAddPages();
};

#endif // !__EXPLORERBAR_H__
