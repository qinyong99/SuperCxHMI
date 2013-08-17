// Action.cpp: implementation of the CAction class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "Action.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufferPool::CBufferPool()
{
	m_pStorage = NULL;
	m_dwNextActionNumber = 0;
}

CBufferPool::~CBufferPool()
{
	if (m_pStorage != NULL)
		m_pStorage->Release();
}

/////////////////////////////////////////////////////////////
//功能:初始化
/////////////////////////////////////////////////////////////
HRESULT CBufferPool::Init()
{
	if (m_pStorage)
		return S_OK;

	return ::StgCreateDocfile(/*L"D:\\temp.dat"*/NULL, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_TRANSACTED|
		STGM_DELETEONRELEASE , 0, &m_pStorage); 
}

/////////////////////////////////////////////////////////////
//功能:取下一个不重号的编号
/////////////////////////////////////////////////////////////
DWORD CBufferPool::GetNextActionNumber()
{
	return m_dwNextActionNumber++;
}

static OLECHAR szStream[] = L"Contents";
//////////////////////////////////////////////////////////////
//功能:为指定的回退对象创建流
//////////////////////////////////////////////////////////////
IStorage* CBufferPool::CreateActionStorage(CAction* pAction, IStream** ppStream, int n)
{
	VERIFY(Init() == S_OK);
	ASSERT(pAction != NULL);
	ASSERT(ppStream != NULL);

	if (pAction->m_dwActionNumber == -1)
		ASSERT(FALSE);		//未初始化

	*ppStream = NULL;

	//创建对应储存名
	LPSTORAGE pStorage = NULL;
	WCHAR strName[256];
	swprintf(strName, L"Action%dNo%d", pAction->m_dwActionNumber, n);

	//准备对象
	HRESULT hr = m_pStorage->CreateStorage(strName, STGM_CREATE | STGM_READWRITE |  
		STGM_SHARE_EXCLUSIVE, 0, 0, &pStorage);
	if (FAILED(hr))
		AfxThrowOleException(hr);

	hr = pStorage->CreateStream(szStream, STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE, 
		0, 0, ppStream);
	if (FAILED(hr))
	{
		pStorage->Release();
		AfxThrowOleException(hr);
	}

	return pStorage;
}

//////////////////////////////////////////////////////////////
//功能:为指定的加退对象打开已创建流
//参数:pAction[in]		要打开储存的回退对象
//		ppStream[out]	打开的流
//返回:为之打开的储存
//////////////////////////////////////////////////////////////
IStorage* CBufferPool::OpenActionStorage(CAction* pAction, IStream** ppStream, int n)
{
	ASSERT(m_pStorage != NULL);
	ASSERT(pAction != NULL);
	ASSERT(ppStream != NULL);
	if (pAction->m_dwActionNumber == -1)
		ASSERT(FALSE);		//未初始化
	
	*ppStream = NULL;
	
	//创建对应流名
	LPSTORAGE pStorage = NULL;
	WCHAR strName[256];
	swprintf(strName, L"Action%dNo%d", pAction->m_dwActionNumber, n);

	//准备对象
	HRESULT hr = m_pStorage->OpenStorage(strName, NULL, STGM_READ | STGM_SHARE_EXCLUSIVE, NULL, 0, &pStorage);
	if (FAILED(hr))
		AfxThrowOleException(hr);
	hr = pStorage->OpenStream(szStream, 0, STGM_READ | STGM_SHARE_EXCLUSIVE, 0, ppStream);
	if (FAILED(hr))
	{
		pStorage->Release();
		AfxThrowOleException(hr);
	}

	return pStorage;
}

/////////////////////////////////////////////////////////////
//功能:删除指定回退对象原来保存的数据
/////////////////////////////////////////////////////////////
BOOL CBufferPool::DeleteActionData(CAction* pAction, int n)
{
	//创建对应流名
	WCHAR strName[256];
	swprintf(strName, L"Action%dNo%d", pAction->m_dwActionNumber, n);

	HRESULT hr = m_pStorage->DestroyElement(strName);

#ifdef _DEBUG
	if (FAILED(hr))
		ASSERT(NULL);		//pAction未保存过
#endif

//	m_pStorage->Commit(STGC_DEFAULT);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//CAction
//////////////////////////////////////////////////////////////////////////////

CBufferPool CAction::m_bufPool;

IMPLEMENT_DYNAMIC(CAction, CObject)

CAction::CAction(CFormDoc* pDoc, LPCTSTR lpszActionName)
	: m_bSaved(FALSE), m_pDocument(pDoc), m_bUnit(FALSE)
{
	m_dwActionNumber = m_bufPool.GetNextActionNumber();
	m_strName = lpszActionName;
}

CAction::~CAction()
{
	if (m_bSaved)
		m_bufPool.DeleteActionData(this);
}

HRESULT CAction::CommitBufferPool()
{
	return m_bufPool.m_pStorage->Commit(STGC_DEFAULT);
}

///////////////////////////////////////////////////////////////
//功能:取回退对象的显示名
///////////////////////////////////////////////////////////////
CString& CAction::GetActionName()
{
	return m_strName;
}

////////////////////////////////////////////////////////////////
//功能:在ar中保存入用于回退或重复的数据
//参数: ar[in]		保存的目的地
//	pStorage[in]	保存用的储存
////////////////////////////////////////////////////////////////
BOOL CAction::OnSaveInfo(CArchive& ar, LPSTORAGE pStorage)
{
#ifdef _DEBUG
	ar << m_dwActionNumber;
#endif

	return TRUE;
}

///////////////////////////////////////////////////////////////
//功能:从ar中读入用于回退或重复的数据
//参数: ar[in]		保存的目的地
//	pStorage[in]	保存用的储存
///////////////////////////////////////////////////////////////
BOOL CAction::OnLoadInfo(CArchive& ar, LPSTORAGE pStorage)
{
#ifdef _DEBUG
	UINT id;
	ar >> id;
	if (id != m_dwActionNumber)	//读入的对象不是保存的对象
		ASSERT(NULL);
#endif

	return TRUE;
}


BOOL CAction::SaveData()
{
	IStreamPtr pStream;
	IStoragePtr pStorage = m_bufPool.CreateActionStorage(this, &pStream);
	if (pStorage == NULL)
		return FALSE;

	m_bSaved = TRUE;

	COleStreamFile file(pStream);
	CArchive ar(&file, CArchive::store);

	if (!OnSaveInfo(ar, pStorage))	//保存数据失败
		return FALSE;

//	pStream->Commit(STGC_DEFAULT);
//	pStorage->Commit(STGC_DEFAULT);
//	m_bufPool.m_pStorage->Commit(STGC_DEFAULT);

	pStorage->Release();

	return TRUE;
}

///////////////////////////////////////////////////////////////
//功能:在对应文档中查找名为strObjName的对象
///////////////////////////////////////////////////////////////
CDevObj* CAction::FindObject(CString& strObjName)
{
	CDevObj* pObj;

	if (m_pDocument->GetNamedObjectsMap()->Lookup(strObjName, pObj))
		return pObj;
	else //对象不存在
	{	
		ASSERT(FALSE);
		return NULL;
	}
}

////////////////////////////////////////////////////////////////////
//功能:装入原来保存的数据
////////////////////////////////////////////////////////////////////
BOOL CAction::LoadData()
{
	IStreamPtr pStream;
	IStoragePtr pStorage = m_bufPool.OpenActionStorage(this, &pStream);
	if (pStorage == NULL)
		return FALSE;

	COleStreamFile file(pStream);
	CArchive ar(&file, CArchive::load);

	if (!OnLoadInfo(ar, pStorage))	//保存数据失败
		return FALSE;

	pStorage->Release();
	return TRUE;
}


