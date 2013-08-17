// ReportObj.h: interface for the CReportObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __REPORTOBJ_H__
#define __REPORTOBJ_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDevDoc;
class CReportDoc;

class CReportObj : public CFormObj  
{
	DECLARE_SERIAL(CReportObj)
public:
	CReportObj(CDevObjDoc* pContainerDoc = NULL);
	virtual ~CReportObj();

	virtual void Draw(CDC* pDC);
	virtual void Print(CDC* pDC) { Draw(pDC); }

	
// Attributes
public:
	CReportDoc* GetReportDocument();

// Overrides
public:
	virtual void RecalcPosition() { return; }

	virtual BOOL CreateFormDoc();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportObj)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

protected:

//	DECLARE_OLETYPELIB(CReportObj)
		
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CReportObj)
	afx_msg float GetLeftMargin();
	afx_msg void SetLeftMargin(float fNewValue);
	afx_msg float GetTopMargin();
	afx_msg void SetTopMargin(float fNewValue);
	afx_msg float GetRightMargin();
	afx_msg void SetRightMargin(float fNewValue);
	afx_msg float GetBottomMargin();
	afx_msg void SetBottomMargin(float fNewValue);
	afx_msg void PrintOut();
	afx_msg void PrintPreview();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	BEGIN_INTERFACE_PART(ProvideClassInfo, IProvideClassInfo2)
		INIT_INTERFACE_PART(CFormObj, ProvideClassInfo)
		STDMETHOD(GetClassInfo)(LPTYPEINFO* ppTypeInfo);
		STDMETHOD(GetGUID)(DWORD dwGuidKind, GUID* pGUID);
	END_INTERFACE_PART_STATIC(ProvideClassInfo)
		
	DECLARE_INTERFACE_MAP()

protected:
	
	BOOL GetDispatchIID(IID* piid);
};


inline CReportDoc* CReportObj::GetReportDocument()
{ return (CReportDoc*)m_pFormDoc; }


#endif // __REPORTOBJ_H__
