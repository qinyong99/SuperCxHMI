#ifndef __ITEMEDITDIALOG_H_
#define __ITEMEDITDIALOG_H_

#include "resource.h"       // main symbols

class CLookupItem;

class CItemEditDialog:
	public CAxDialogImpl<CItemEditDialog>
{

public:
	CItemEditDialog(CLookupItem* pEditItem, enumPropType nOutputType, CEnumPropItemArray* pArrEnumPropItems)
	{
		ATLASSERT(pEditItem != NULL);

		m_pEditItem = pEditItem;
		m_nOutputType = nOutputType;
		m_pArrEnumPropItems = pArrEnumPropItems;
	}

	enum { IDD = IDD_ITEM_EDIT };

	BEGIN_MSG_MAP(CItemEditDialog)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()
protected:
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
	HRESULT HelperSetText(int nIDDlgItem, CComVariant& var);

public:
	CLookupItem* m_pEditItem;
	CAxWindow m_winOutput;
	enumPropType m_nOutputType;
	CEnumPropItemArray* m_pArrEnumPropItems;
};

#endif //__ITEMEDITDIALOG_H_
