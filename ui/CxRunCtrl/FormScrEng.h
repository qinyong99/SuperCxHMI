// FormScrEng.h: interface for the CFormScrEng class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __FORMSCRENG_H__
#define __FORMSCRENG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFormDoc;
class CRunView;
class CDevDoc;

_COM_SMARTPTR_TYPEDEF(ICxEventObj, _uuidof(ICxEventObj));
_COM_SMARTPTR_TYPEDEF(ICxDataEntry, _uuidof(ICxDataEntry));

class CFormScrEng : public CScriptEngine  
{
	DECLARE_DYNCREATE(CFormScrEng)

public:
	CFormScrEng(CFormDoc* pFormDoc = NULL);
	virtual ~CFormScrEng();

// Attributes
public:
	// 最多支持 8 个数据源
	int m_nTagProviderCount;
	CString m_strTagProvider[8];
	IDispatch* m_pTagProvider[8]; 

	ICxEventObjPtr m_pEventObj;
	ICxDataEntryPtr m_pDataEntry;

	DISPPARAMS* m_pdpCurrentEvent;

	CFormDoc* m_pFormDoc;

	CFormDoc* GetDocument();
	CDevDoc* GetDevDoc();

	virtual CFrameWnd* GetFrameWnd();
	
// Operations
public:
	int FindTagProvider(LPCTSTR lpszName);
	IDispatch* GetTagProvider(int iIndex);

	void ActiveEventObject(CRunView* pView);
	HRESULT InitEventObject();
	
	virtual void UnloadScript();

	BOOL IsScriptRunning();

	void PreTreatExpression(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin = NULL, VARIANT* pvaMax = NULL);

	virtual HRESULT FinishInitialize();
	virtual HRESULT AddItemsName();
	virtual HRESULT GetItemDispatch(LPCOLESTR pszName, IUnknown** ppDispatch);

	void GetSourceContextText(DWORD dwCookie, CString& strText);

	HRESULT GetScriptDispatch(LPCOLESTR pszItemName, IDispatch **ppdisp);
	HRESULT InvokeScriptDispatch(LPCOLESTR pszItemName, LPCOLESTR pszMethod, REFIID iid, LCID lcid,
		WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgError);
private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEngine)
	//}}AFX_VIRTUAL

// Implementation
protected:
// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CActiveScript_MsgBox)
	afx_msg LPDISPATCH GetEventObject();
	afx_msg LPDISPATCH GetDataEntry();
	afx_msg VARIANT GetApplication(LPCTSTR Item);
	afx_msg void SetApplication(LPCTSTR Item, const VARIANT FAR& newValue);
	afx_msg VARIANT GetFrame(LPCTSTR Item);
	afx_msg void SetFrame(LPCTSTR Item, const VARIANT FAR& newValue);
	afx_msg BSTR GetPictureDirectory();
	afx_msg void SetPictureDirectory(LPCTSTR lpszNewValue);
	afx_msg BSTR GetSetupDirectory();
	afx_msg void ShowPicture(LPCTSTR pszPicture);
	afx_msg void DoEvent();
	afx_msg void ExitApplication();
	afx_msg void ContainerContext(const VARIANT FAR& Tag);
	afx_msg BOOL Login(const VARIANT FAR& UserName, const VARIANT FAR& Password, const VARIANT FAR& Prompt);
	afx_msg void Logout();
	afx_msg void ManageUser();
	afx_msg void ChangePassword();
	afx_msg BSTR GetUserName();
	afx_msg BSTR GetUserSecurityArea();
	afx_msg long GetUserPrivilege();
	afx_msg VARIANT SetEventSubPara(int nPara, const VARIANT FAR& newValue);
	afx_msg void PrintWindow(const VARIANT FAR& Window,
		const VARIANT FAR& LeftMargin, const VARIANT FAR& TopMargin, 
		const VARIANT FAR& RightMargin, const VARIANT FAR& BottomMargin,
		const VARIANT FAR& HorizontalScale, const VARIANT FAR& VerticalScale,
		const VARIANT FAR& MultipleScale);
	afx_msg VARIANT CreateServerObject(const VARIANT FAR& ClassValue);
	afx_msg VARIANT GetFrameWindow();
	afx_msg VARIANT LoadPicutreFile(LPCTSTR lpszFileName);
	afx_msg BSTR SaveTempPicutreFile(const VARIANT FAR& vaData);
	afx_msg void ShowSoftKeyboard(const VARIANT FAR& Hide, const VARIANT FAR& OnlyNumber, const VARIANT FAR& XPos, const VARIANT FAR& YPos);
	afx_msg BSTR GetOpenParameter(const VARIANT FAR& Item);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	//扩展自动化接口
	BEGIN_INTERFACE_PART(DispatchExtend, IDispatch)
		STDMETHOD(GetIDsOfNames)(REFIID iid, LPOLESTR* ppszNames, UINT nNames,
			LCID lcid, DISPID* pDispIDs);
		STDMETHOD(GetTypeInfo)(UINT iTypeInfo, LCID lcid,
			ITypeInfo** ppTypeInfo);
		STDMETHOD(GetTypeInfoCount)(UINT* pnInfoCount);
		STDMETHOD(Invoke)(DISPID dispidMember, REFIID iid, LCID lcid,
			WORD wFlags, DISPPARAMS* pdpParams, VARIANT* pvarResult,
			EXCEPINFO* pExceptionInfo, UINT* piArgError);
	END_INTERFACE_PART(DispatchExtend)

	BEGIN_INTERFACE_PART(ScriptEngineSite, ICxScriptEngineSite)
		STDMETHOD(EvaluateExpression)(BSTR bstrSource, VARIANT* pvaValidObjects);
		STDMETHOD(Execute)(BSTR bstrStatement);
		STDMETHOD(PreTreatExpression)(BSTR bstrSource, BSTR* pbstrSource);
		STDMETHOD(PreTreatExpressionEx)(BSTR bstrSource, BSTR* pbstrSource, VARIANT* pvaMin, VARIANT* pvaMax);
	END_INTERFACE_PART(ScriptEngineSite)

	DECLARE_INTERFACE_MAP()

	friend class CRunCtrl;
};

#endif // !defined(__FORMSCRENG_H__)
