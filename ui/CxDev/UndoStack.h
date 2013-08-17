// UndoStack.h: interface for the CUndoStack class.
//功能: 管理可回退操作的类
//编制: 孙越洋
//时间: 2002.3.18
//所属工程: CxDev
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNDOSTACK_H__B22AE483_1ED8_4FBC_A1B2_4FCF05B6D7FF__INCLUDED_)
#define AFX_UNDOSTACK_H__B22AE483_1ED8_4FBC_A1B2_4FCF05B6D7FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CAction;

class CUndoStack
{
public:
	CUndoStack();
	virtual ~CUndoStack();

	void Undo();
	void Redo();
	BOOL Push(CAction* pAction);
	BOOL Reset();
	CString& GetNextUndoCmdStr();
	CString& GetNextRedoCmdStr();
	BOOL CanUndo();
	BOOL CanRedo();

protected:
	CTypedPtrList<CObList, CAction*> m_listUndo;
	CTypedPtrList<CObList, CAction*> m_listRedo;
};

#endif // !defined(AFX_UNDOSTACK_H__B22AE483_1ED8_4FBC_A1B2_4FCF05B6D7FF__INCLUDED_)
