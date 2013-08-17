//////////////////////////////////////////////////////////////////////
// UndoStack.cpp: implementation of the CUndoStack class.

#include "stdafx.h"
#include "DevInc.h"
#include "UndoStack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUndoStack::CUndoStack()
{

}

CUndoStack::~CUndoStack()
{
	CAction* pAct;
	while(!m_listUndo.IsEmpty())
	{
		pAct = m_listUndo.RemoveHead();
		delete pAct;
	}

	while(!m_listRedo.IsEmpty())
	{
		pAct = m_listRedo.RemoveHead();
		delete pAct;
	}
}

//////////////////////////////////////////////////////////////////////
//功能:回退
//返回:需刷新的文档区域
//////////////////////////////////////////////////////////////////////
void CUndoStack::Undo()
{
	if (m_listUndo.IsEmpty())
		return;

	CAction* pAct = m_listUndo.RemoveTail();
	if(pAct->CanRedo())
	{
		m_listRedo.AddTail(pAct);
		pAct->Undo();
	}
	else
	{
		pAct->Undo();
		delete pAct;
	}
}

/////////////////////////////////////////////////////////////////////
//功能:重作
//返回:需刷新的文档区域
/////////////////////////////////////////////////////////////////////
void CUndoStack::Redo()
{
	if(m_listRedo.IsEmpty())
		return;

	CAction* pAct = m_listRedo.RemoveTail();
	m_listUndo.AddTail(pAct);
	pAct->Redo();
}

/////////////////////////////////////////////////////////////////////
//功能:加入新的Action
/////////////////////////////////////////////////////////////////////
BOOL CUndoStack::Push(CAction* pAction)
{
	ASSERT(pAction != NULL);

	// 最后的动作
	CAction* pLastAction = NULL;
	if (!m_listUndo.IsEmpty() && m_listRedo.IsEmpty())
		pLastAction = m_listUndo.GetTail();

	// 相互能合并
	if (pLastAction && pLastAction->CanUnit(pAction) && pAction->CanUnit(pLastAction))
	{
		pLastAction->Unit(pAction);
		delete pAction;
		return TRUE;
	}
		
	m_listUndo.AddTail(pAction);

	//只保留最后100次操作
	if (m_listUndo.GetCount() > 100)
	{
		CAction* pAct = m_listUndo.GetHead();
		delete pAct;
		m_listUndo.RemoveHead();
	}
	
	POSITION pos = m_listRedo.GetHeadPosition();
//	if (pos == NULL)
//		return TRUE;

	while (pos != NULL)
	{
		CAction* pAct = m_listRedo.GetNext(pos);
		delete pAct;
	}
	m_listRedo.RemoveAll();
//	CAction::CommitBufferPool();

	return TRUE;
}

////////////////////////////////////////////////////////////////////
//功能:复位
////////////////////////////////////////////////////////////////////
BOOL CUndoStack::Reset()
{
	POSITION pos = m_listRedo.GetHeadPosition();
	while (pos != NULL)
	{
		CAction* pAct = m_listRedo.GetNext(pos);
		delete pAct;
	}
	m_listRedo.RemoveAll();

	pos = m_listUndo.GetHeadPosition();
	while(pos != NULL)
	{
		CAction* pAct = m_listUndo.GetNext(pos);
		delete pAct;
	}
	m_listUndo.RemoveAll();

	return TRUE;
}

////////////////////////////////////////////////////////////////////
//功能:取下一个可回退的命令名
////////////////////////////////////////////////////////////////////
CString& CUndoStack::GetNextUndoCmdStr()
{
	static CString strRet;
	if (m_listUndo.IsEmpty())
		return strRet;

	CAction* pAct = m_listUndo.GetTail();
	return pAct->GetActionName();
}

////////////////////////////////////////////////////////////////////
//功能:取下一个可再作的命令名
////////////////////////////////////////////////////////////////////
CString& CUndoStack::GetNextRedoCmdStr()
{
	static CString strRet;
	if(m_listRedo.IsEmpty())
		return strRet;

	CAction* pAct = m_listRedo.GetTail();
	return pAct->GetActionName();
}

////////////////////////////////////////////////////////////////////
//功能:判定是否可以回退
////////////////////////////////////////////////////////////////////
BOOL CUndoStack::CanUndo()
{
	if (m_listUndo.IsEmpty())
		return FALSE;
	else
		return TRUE;
}

////////////////////////////////////////////////////////////////////
//功能:判别定是否可以重作
////////////////////////////////////////////////////////////////////
BOOL CUndoStack::CanRedo()
{
	if (m_listRedo.IsEmpty())
		return FALSE;
	else
		return TRUE;
}
