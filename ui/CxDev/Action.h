//Action.h: interface for the CAction class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_H__3EABFAE1_B402_449E_9558_6BCCFAFD19C3__INCLUDED_)
#define AFX_ACTION_H__3EABFAE1_B402_449E_9558_6BCCFAFD19C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAction;
class CFormDoc;

class CBufferPool
{
public:
	CBufferPool();
	~CBufferPool();
	HRESULT Init();

public:
	DWORD GetNextActionNumber();
	IStorage* CreateActionStorage(CAction* pAction, IStream** ppStream, int n = 0);
	IStorage* OpenActionStorage(CAction* pAction, IStream** ppStream, int n = 0);
	BOOL DeleteActionData(CAction* pAction, int n = 0);

protected:
	LPSTORAGE m_pStorage;
	DWORD m_dwNextActionNumber;

	friend class CAction;
};

class CAction : public CObject  
{
public:
	DECLARE_DYNAMIC(CAction)

	CAction(CFormDoc* pDoc, LPCTSTR lpszActionName);
	~CAction();

public:
	virtual void SetUnitState(BOOL b = TRUE) { m_bUnit = b; };
	virtual BOOL CanUnit(CAction* pAct)	{ return FALSE; }
	virtual void Unit(CAction* pAct) { ASSERT(FALSE); }
		
	virtual void Redo() = 0;
	virtual void Undo() = 0;
	virtual BOOL CanRedo() = 0;
	CString& GetActionName();
	void SetActionName(LPCSTR szName) { m_strName = szName; }

	static HRESULT CommitBufferPool();

protected:
	CString m_strName;
	CFormDoc* m_pDocument;
	BOOL m_bUnit;

protected:
	BOOL LoadData();
	BOOL SaveData();
	virtual BOOL OnSaveInfo(CArchive& ar, LPSTORAGE pStorage);
	virtual BOOL OnLoadInfo(CArchive& ar, LPSTORAGE pStorage);
	CDevObj* FindObject(CString& strObjName);

	static CBufferPool m_bufPool;
	
private:
	DWORD m_dwActionNumber;

	BOOL m_bSaved;

	friend class CBufferPool;
	friend class CActionObjSet;
};

#endif // !defined(AFX_ACTION_H__3EABFAE1_B402_449E_9558_6BCCFAFD19C3__INCLUDED_)
