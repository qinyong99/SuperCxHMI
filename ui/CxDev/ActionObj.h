//////////////////////////////////////////////////////////////////////
// ActionObj.h: interface for the CActionObj class.

#if !defined(AFX_ACTIONOBJ_H__8120467A_8676_4966_9484_8E37574AFD7A__INCLUDED_)
#define AFX_ACTIONOBJ_H__8120467A_8676_4966_9484_8E37574AFD7A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Action.h"

class CActionObjAdd : public CAction  
{
public:
	CActionObjAdd(LPCTSTR lpszActionName, CFormDoc* pDoc, CDevObj* pObj, BOOL bAddOrDel, BOOL bChild = FALSE);
	virtual ~CActionObjAdd();

public:
	virtual void Undo();
	virtual void Redo();
	virtual BOOL CanRedo(); 

	void UndoEx(BOOL& bFormObjChanged);
	void RedoEx(BOOL& bFormObjChanged);

protected:
	virtual BOOL OnSaveInfo(CArchive& ar, LPSTORAGE pStorage);
	virtual BOOL OnLoadInfo(CArchive& ar, LPSTORAGE pStorage);

protected:
	CString m_strObjName;
	BOOL m_bAdd;
	BOOL m_bChild;

	CDevObj* m_pObject;
	
	friend class CActionObjAddSet;
};

//多个对象同时被删除
class CActionObjAddSet : public CAction
{
public:
	CActionObjAddSet(LPCTSTR lpszActionName, CFormDoc* pDoc, BOOL bAdd);
	~CActionObjAddSet();

public:
	void AddObj(CDevObj* pObj);
	void ObjAddComplete();

	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

protected:

protected:
	CTypedPtrList<CObList, CActionObjAdd*> m_listActions;
	BOOL m_bAdd;
};

//组合和解组合
class CActionObjGroup : public CAction
{
public:
	CActionObjGroup(LPCTSTR lpszActionName, CFormDoc* pDoc, BOOL bGroup);
	~CActionObjGroup();

public:
	virtual void Redo();
	virtual void Undo();
	virtual BOOL CanRedo() {return TRUE;}

public:
	void SetGroupObj(CDevObj* pObj);
	void AddSubObj(CDevObj* pObj);
	void ObjAddComplete();

protected:

protected:
	BOOL m_bGroup;
	CActionObjAdd* m_pGroupObj;
	CActionObjAddSet m_setSubObj;
};

//移动回退
class CActionObjMove : public CAction
{
public:
	DECLARE_DYNAMIC(CActionObjMove)

	CActionObjMove(CFormDoc* pDoc, CDevObj* pObj) 
		: CAction(pDoc, "移动")
	{
		ASSERT(pDoc != NULL);
		ASSERT(pObj != NULL);

		m_rcPosition = pObj->GetPositionRect();
		m_strObjName = pObj->GetDisplayName();
	}

public:
	virtual BOOL CanUnit(CAction* pAct);
	virtual void Unit(CAction* pAct);

	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

protected:
	CRectF m_rcPosition;
	CString m_strObjName;
};

//多个对象的回退
class CActionObjMoveSet : public CAction
{
	DECLARE_DYNAMIC(CActionObjMoveSet)

public:
	CActionObjMoveSet(LPCTSTR lpszActionName, CFormDoc* pDoc, BOOL bChild = FALSE);
	~CActionObjMoveSet();
	void AddObj(CDevObj* pObj);

public:
	virtual void SetUnitState(BOOL b = TRUE);
	virtual BOOL CanUnit(CAction* pAct);
	virtual void Unit(CAction* pAct);

	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

protected:
	CTypedPtrList<CObList, CActionObjMove*> m_listActions;
	BOOL m_bChild;
};

// 对象改变回退，2004.3.2日增加名字改变回退
class CActionObj : public CAction
{
public:
	CActionObj(LPCTSTR lpszActionName, CFormDoc* pDoc, CDevObj* pObj, BOOL bChild = FALSE) 
		: CAction(pDoc, lpszActionName)
	{
		ASSERT(pDoc != NULL);
		ASSERT(pObj != NULL);

		m_pObject = pObj;
		m_strObjName = pObj->GetDisplayName();
		m_strNewName = m_strObjName;
		m_bChild = bChild;

		if (!m_bChild)
			SaveData();

		m_bSaveRedoInfo = FALSE;
	}

	~CActionObj()
	{
		if (m_bSaveRedoInfo)
			m_bufPool.DeleteActionData(this, 1);
	}

public:
	virtual BOOL OnSaveInfo(CArchive& ar, LPSTORAGE pStorage);
	virtual BOOL OnLoadInfo(CArchive& ar, LPSTORAGE pStorage);

	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() {return TRUE; }

	void SetNewName(LPCTSTR lpszName); // 可能也改变对象的名称属性

protected:
	CDevObj* m_pObject;
	CString m_strObjName;
	CString m_strNewName;
	BOOL m_bSaveRedoInfo;

	BOOL m_bChild;
};

//多个对象的回退
class CActionObjSet : public CAction
{
public:
	CActionObjSet(LPCTSTR lpszActionName, CFormDoc* pDoc);
	~CActionObjSet();

public:
	void AddObj(CDevObj* pObj);
	void ObjAddComplete();

	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

protected:
	CTypedPtrList<CObList, CActionObj*> m_listActions;
};

//移动回退
class CActionName : public CAction
{
public:
	CActionName(CFormDoc* pDoc, CDevObj* pObj, CReportSection* pSection, CString strOldName) 
		: CAction(pDoc, _T("改变名称"))
	{
		m_strOldName = strOldName;

		if (pSection != NULL)
			m_strNewName = pSection->GetName();
		else if (pObj != NULL)
			m_strNewName = pObj->GetDisplayName();
		else
			m_strNewName = pDoc->GetName();

		m_bObject = (pObj != NULL);
		m_pSection = pSection;
	}

public:
	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

protected:
	CString m_strOldName;
	CString m_strNewName;

	BOOL m_bObject;
	CReportSection* m_pSection;
};

class CActionDoc : public CAction
{
public:
	CActionDoc(LPCTSTR lpszActionName, CFormDoc* pDoc)
		: CAction(pDoc, lpszActionName)
	{
		ASSERT(pDoc != NULL);
		SaveData();
		
		m_bSaveRedoInfo = FALSE;
	}
	~CActionDoc()
	{
		if (m_bSaveRedoInfo)
			m_bufPool.DeleteActionData(this, 1);
	}
	
public:
	virtual BOOL OnSaveInfo(CArchive& ar, LPSTORAGE pStorage);
	virtual BOOL OnLoadInfo(CArchive& ar, LPSTORAGE pStorage);
	
	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }
	
protected:
	BOOL m_bSaveRedoInfo;
};

class CActionDocSize : public CAction
{
public:
	CActionDocSize(CFormDoc* pDoc, CSizeF szNew, CSizeF szOld)
		: CAction(pDoc, _T("改变尺寸"))
	{
		ASSERT(pDoc != NULL);

		m_szNew = szNew;
		m_szOld = szOld;
	}
	
public:
	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }

	CSizeF m_szNew;
	CSizeF m_szOld;
};

class CActionSection : public CAction
{
public:
	CActionSection(LPCTSTR lpszActionName, CFormDoc* pDoc, CReportSection* pSection)
		: CAction(pDoc, CString(lpszActionName))
	{
		ASSERT(pDoc != NULL);
		ASSERT(pSection != NULL);

		m_pSection = pSection;
		SaveData();

		m_bSaveRedoInfo = FALSE;
	}
	~CActionSection()
	{
		if (m_bSaveRedoInfo)
			m_bufPool.DeleteActionData(this, 1);
	}
	
public:
	virtual BOOL OnSaveInfo(CArchive& ar, LPSTORAGE pStorage);
	virtual BOOL OnLoadInfo(CArchive& ar, LPSTORAGE pStorage);
	
	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }
	
protected:
	CReportSection* m_pSection;
	BOOL m_bSaveRedoInfo;
};

class CActionSectionLineMove : public CAction
{
public:
	CActionSectionLineMove(CFormDoc* pDoc, int nSectionLine, POINT ptNew, POINT ptOld) : 
		CAction(pDoc, CString("移动栏")),
		m_actionObjectsMoved(NULL, pDoc, TRUE)

	{
		ASSERT(pDoc != NULL);
		
		m_nSectionLine = nSectionLine;
		m_ptNew = ptNew;
		m_ptOld = ptOld;
	}
	
public:
	void AddObj(CDevObj* pObj);
		
	virtual void Redo(); 
	virtual void Undo();
	virtual BOOL CanRedo() { return TRUE; }
	
protected:
	CActionObjMoveSet m_actionObjectsMoved;

	int m_nSectionLine;
	CPoint m_ptNew;
	CPoint m_ptOld;
};


#endif // !defined(AFX_ACTIONOBJ_H__8120467A_8676_4966_9484_8E37574AFD7A__INCLUDED_)
