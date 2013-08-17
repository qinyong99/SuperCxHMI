#if !defined(AFX_OPCDATASOURCE_H__E8780F96_F206_4B0D_8659_01F994B4CE33__INCLUDED_)
#define AFX_OPCDATASOURCE_H__E8780F96_F206_4B0D_8659_01F994B4CE33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OPCDataSource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COPCDataSource dialog

class COPCDataSource : public CDialog
{
// Construction
public:
	COPCDataSource(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COPCDataSource)
	CListBox	m_lbName;
	CEdit	m_editComputer;
	CEdit	m_editOPCServer;
	enum { IDD = IDD_OPC_DATASOURCE };
	//}}AFX_DATA


	class COPCServerItem
	{
	public:
		CString m_strName;
		CString m_strOPCServer;
		CString m_strComputer;
	};

	BOOL IsUniqueSeverName(LPCTSTR lpszName)
	{
		for (int i = 0; i < m_lbName.GetCount(); i++)
		{
			COPCServerItem* pItem = (COPCServerItem *)m_lbName.GetItemData(i);	
			if (pItem != NULL && pItem->m_strName == lpszName)
				return FALSE;
		}

		return TRUE;
	}
	
	BOOL CreateUniqueSeverName(CString& strNewName)
	{
		ULONG nTag;
		BOOL bCollisionFound;
		CString strUniqueName;
		
		nTag = 1;
		do
		{
			strUniqueName.Format("%s%u", _T("OPCServer"), nTag);
			bCollisionFound = FALSE;

			if (!IsUniqueSeverName(strUniqueName))
			{
				bCollisionFound = TRUE;
				nTag++;
			}
		} while (bCollisionFound);

		strNewName = strUniqueName;

		return TRUE;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPCDataSource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(COPCDataSource)
	afx_msg void OnAddServer();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeList1();
	afx_msg void OnRemoveServer();
	afx_msg void OnEditServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPCDATASOURCE_H__E8780F96_F206_4B0D_8659_01F994B4CE33__INCLUDED_)
