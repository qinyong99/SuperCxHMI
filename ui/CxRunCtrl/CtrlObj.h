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

	virtual void Serialize(CArchive& ar); 

// Implementation
public:
	BOOL Create(REFCLSID clsid);
	BOOL FinishCreate(CCtrlItem* pItem);

	virtual void Draw(CDC* pDC);
	virtual void DrawBackToFront(CDC* pDC);
	virtual void DrawFrontToBack(CDC* pDC);

	virtual void Active(CRunView* pView);
	virtual void Deactive();

	void OnShow(CRunView* pView = NULL);

	virtual BOOL OnWindowMessage(const MSG* pMessage, BOOL bSubObj = FALSE);
	virtual BOOL TranslateAccelerator(const MSG* pMessage);

	virtual BOOL HitTest(CPoint point, const CRunView* pView);

	virtual void Invalidate(CRunView* pView = NULL);

	virtual IDispatch* GetObjectDispatch();
	virtual void MoveTo(const CRectF& position, BOOL bUniformScale, CRunView* pView = NULL);
	virtual LPUNKNOWN GetObjectUnknown();

	void SetExtent(CRunView* pView);

	virtual void Release();

	void SetTabIndex(int nTabIndex);
	
	CCtrlItem* m_pClientItem;
	
	int m_nTabIndex;
	BOOL m_bTabStop;
	BOOL m_bDefault;
	BOOL m_bCancel;

	friend class CCtrlItem;
};

#endif // !defined(__CTRLOBJ_H__)
