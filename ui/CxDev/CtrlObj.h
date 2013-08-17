// CtrlObj.h: interface for the CCtrlObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __CTRLOBJ_H__
#define __CTRLOBJ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCtrlObj : public CDevObj  
{
	DECLARE_SERIAL(CCtrlObj)

public:
	CCtrlObj(CDevObjDoc* pContainer = NULL);
	virtual ~CCtrlObj();
	virtual BOOL GetPropertyPageCLSIDs(CArray< CLSID, CLSID& >& aclsidPages);
	virtual void Serialize(CArchive& ar); 

// Implementation
public:
	BOOL Create(REFCLSID clsid);
	BOOL FinishCreate(CCtrlItem* pItem, BOOL bActive = FALSE);

	virtual void Draw(CDC* pDC);
	virtual void DrawBackToFront(CDC* pDC);
	virtual void DrawFrontToBack(CDC* pDC);

	virtual void Invalidate(CLayoutView* pView = NULL);

	virtual void CopyToClipboard(CArchive& ar, IStorage* pStorage);
	virtual BOOL CreateFromClipboard(CArchive& ar, IStorage* pStorage);
	virtual enumObjectType GetObjectType(){ return ctrlobj; }


	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CLayoutView* pView = NULL);
	virtual LPUNKNOWN GetObjectUnknown();

	virtual void Release();
	virtual void Delete(BOOL bAutoDelete = TRUE);

	virtual void SetChildObjectDocument();

	void SetTabIndex(int nTabIndex);

	CCtrlItem* m_pClientItem;
	
	int m_nTabIndex;
	BOOL m_bTabStop;
	BOOL m_bDefault;
	BOOL m_bCancel;

	friend class CCtrlItem;
};

#endif // !defined(__CTRLOBJ_H__)
