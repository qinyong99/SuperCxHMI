////////////////////////////////////////////////////////////////
// SymbolDesignDlg.cpp : implementation file
//功能:设置组合对象对话框类
//作者:孙越洋
//时间:2001.12.10
//所属工程:CxDev
//说明:构造,创建(Create)后,用SetDefault()初始化有关参数
////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DevInc.h"
#include "CxDevIF.h"
#include "SymbolLibDoc.h"
#include "SymbolDesignDlg.h"

#include "TypeInfoBrowse.h"
// #include "TypeChanger.h"

#include "../CxDynObjs/CxDynObjs.h"
#include "../CxDynObjs/CxDynObjs_i.c"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ENABLE_TIMER	101

/////////////////////////////////////////////////////////////////////////////
// CSymbolDesignDlg dialog
CSymbolDesignDlg::CSymbolDesignDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolDesignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolDesignDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_pObject = NULL;
	m_nLastSelItem = -1;
	m_bDisplayAllProp = FALSE;
	m_pPreviewWnd = NULL;

	m_pPropValueEditObj = NULL;
}


void CSymbolDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolDesignDlg)
	DDX_Control(pDX, IDC_GROUPTAG_LIST, m_listProp);
	DDX_Control(pDX, IDC_BUTTON_ADVANCE, m_btnAdvance);
	DDX_Control(pDX, IDC_CHECK_ALL, m_btnCheck);
	DDX_Control(pDX, IDC_TREE_PROP, m_treeProps);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolDesignDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolDesignDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DELLINE, OnDelline)
	ON_BN_CLICKED(IDC_EDITLINE, OnEditline)
	ON_BN_CLICKED(IDC_INSERT_LINE, OnInsertline)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnButtonAdvance)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PROP, OnSelchangedTreeProp)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PROP, OnClickTreeProp)
	ON_WM_MEASUREITEM()
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE_PROP, OnKeydownTreeProp)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_TREEITEMCLICK, OnPropTreeItemClick)
	ON_MESSAGE(WM_OBJECT_SELECTED, OnObjectSelected)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CSymbolDesignDlg, CDialog)
	ON_EVENT(CSymbolDesignDlg, IDC_PROP_VALUE, 1, OnPropValueChanged, VTS_NONE)
	ON_EVENT(CSymbolDesignDlg, IDC_PROP_VALUE, 2, OnPropValueBeginEdit, VTS_NONE)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolDesignDlg message handlers

void CSymbolDesignDlg::OnOK() 
{
	ASSERT(m_pObject);
	
	m_pObject->RemoveAllExProp();

	UpdateData();
	SaveSubObjectChoice();

	if (m_pObject->m_pDocument != NULL && m_pObject->m_pDocument->IsKindOf(RUNTIME_CLASS(CSymbolLibDoc)))
		m_pObject->PutDisplayName(m_strName, FALSE);
	else
		m_pObject->PutDisplayName(m_strName, TRUE);
	
	int nCount = m_listProp.GetPropertyItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CSymbolProp* pVar = m_listProp.GetPropertyItem(i);
	
		//动态重新初始化
		for (int j = 0; j < pVar->m_arrObjNum.GetSize(); j++)
		{
			if (pVar->m_arrObjNum[j].m_pDynamicObject)
				pVar->m_arrObjNum[j].m_pDynamicObject->InitRun(NULL, NULL, DISPID_UNKNOWN);
		}
		
		m_pObject->AddExtendProperty(pVar);
	}

	m_pObject->UpdateAllExPropValue();

	CDialog::OnOK();
}

BOOL CSymbolDesignDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ASSERT(m_pObject != NULL);		//对应对象应已经存在

	m_listProp.Init();
	//在列表中加入项目
	m_strName = m_pObject->GetDisplayName();
	int nCount = m_pObject->GetExPropCount();
	for(int i = 0; i < nCount; i++)
	{
		CSymbolProp* pItem = new CSymbolProp;
		if (m_pObject->GetExPropInfo(i, pItem))
			m_listProp.AddPropertyItem(pItem);

		if (i == 0)
			m_listProp.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	// 加上下面两句，否则 SetCheck 无效
	m_treeProps.ModifyStyle(TVS_CHECKBOXES, 0);
	m_treeProps.ModifyStyle(0, TVS_CHECKBOXES);
	
 	InitSubObjectProperyTree();
 	InitPreview();

	OnSelchangedListProp(FALSE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolDesignDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pPreviewWnd != NULL)
	{
		m_pPreviewWnd->DestroyWindow();
		delete m_pPreviewWnd;
		m_pPreviewWnd = NULL;
	}
}

/////////////////////////////////////////////////////////////
//功能：删除当前行
/////////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnDelline()
{
	m_listProp.DeleteSelectPropertyItem();

	m_nLastSelItem = m_listProp.GetSelectPropertyItem();
}

////////////////////////////////////////////////////////////
//功能：在对话框中编辑当前行中的内容
////////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnEditline() 
{
	m_listProp.EditSelectPropertyItem();
}

////////////////////////////////////////////////////////////
//功能：新建扩展属性
////////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnInsertline() 
{
	CSymbolProp* pItem = new CSymbolProp;
	int nNum;
	pItem->m_strName = m_listProp.MakeUniqueTagName(&nNum);
	pItem->m_strDescription.Format("%s%d","新属性", nNum);
	pItem->m_proptype = PROPTYPE_COLOR;
	m_listProp.AddPropertyItem(pItem, TRUE);

	m_nLastSelItem = m_listProp.GetSelectPropertyItem();
}

// 功能：初始化属性树
void CSymbolDesignDlg::InitSubObjectProperyTree()
{
	ASSERT(m_pObject != NULL);

	m_treeProps.DeleteAllItems();
	m_arrPropName.RemoveAll();
	
	CDevObjArray& arrObj = m_pObject->GetSubObjects();
	CDevObj* pObj;
	for(int i = 0; i < arrObj.GetSize(); i++)
	{
		pObj = arrObj[i];
		CString strName = pObj->GetDisplayName();
		HTREEITEM hItem = m_treeProps.InsertItem(strName, 0, 0);
		m_treeProps.SetItemData(hItem, (DWORD)i);
	}
}

void CSymbolDesignDlg::UpdateSubObjectProperyTree()
{
	ASSERT(m_pObject != NULL);

	HTREEITEM hSelectItem;
	HTREEITEM hObjectItem;
	HTREEITEM hPropItem;
	HTREEITEM hTemp;

	m_treeProps.SetRedraw(FALSE);

	hSelectItem = m_treeProps.GetSelectedItem();
	if ((hTemp = m_treeProps.GetParentItem(hSelectItem)) != NULL)
		m_treeProps.SelectItem(hTemp);
	
	hObjectItem = m_treeProps.GetRootItem();
	while (hObjectItem != NULL)
	{
		m_treeProps.SetCheck(hObjectItem, FALSE);

		// 删除子对象属性
		hPropItem = m_treeProps.GetChildItem(hObjectItem);
		while (hPropItem != NULL)
		{
			hTemp = m_treeProps.GetNextSiblingItem(hPropItem);
			m_treeProps.DeleteItem(hPropItem);
			hPropItem = hTemp;
		}

		// 插入子对象属性
		if (m_bDisplayAllProp || m_listProp.GetSelectItemType() != PROPTYPE_UNKNOWN)
		{
			CDevObjArray& arrObj = m_pObject->GetSubObjects();
			int nObjIndex = m_treeProps.GetItemData(hObjectItem);
			CDevObj* pObj = arrObj[nObjIndex];

			enumPropType enumType = m_bDisplayAllProp ? PROPTYPE_UNKNOWN : m_listProp.GetSelectItemType();
			AddSubObjectProperties(pObj, enumType, hObjectItem);

			int nIndex = m_listProp.GetSelectPropertyItem();
			if (nIndex != -1)
				UpdateSubObjectCheckState(hObjectItem, m_listProp.GetPropertyItem(nIndex));
		}
			
		hObjectItem = m_treeProps.GetNextSiblingItem(hObjectItem);
	}
	
	m_treeProps.SetRedraw(TRUE);
}

// 功能：根据属性的选中状态更新子对象的选中状态
void CSymbolDesignDlg::UpdateSubObjectItem()
{
	BOOL bCanCheck = m_listProp.GetSelectPropertyItem() != -1;

	HTREEITEM hObjectItem = m_treeProps.GetRootItem();
	while (hObjectItem != NULL)
	{
		BOOL bCheck = FALSE;

		//检查子项是不有选中项
		HTREEITEM hPropItem = m_treeProps.GetChildItem(hObjectItem);
		while (hPropItem != NULL)
		{
			if (m_treeProps.GetCheck(hPropItem))
			{
				if (bCanCheck)
					bCheck = TRUE;
				else
					m_treeProps.SetCheck(hPropItem, FALSE);
							
				break;
			}
			hPropItem = m_treeProps.GetNextSiblingItem(hPropItem);
		}

		m_treeProps.SetCheck(hObjectItem, bCheck);
		
		hObjectItem = m_treeProps.GetNextSiblingItem(hObjectItem);
	}
}

///////////////////////////////////////////////////////////////////
//功能：用户改变扩展属性表中的选项后的处理, 变更子对象树中的内容
///////////////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnSelchangedListProp(BOOL bSavePrev)
{
	if (bSavePrev)
		SaveSubObjectChoice();

	UpdateSubObjectProperyTree();

	m_nLastSelItem = m_listProp.GetSelectPropertyItem();
	GetDlgItem(IDC_EDITLINE)->EnableWindow(m_nLastSelItem != -1);
	GetDlgItem(IDC_DELLINE)->EnableWindow(m_nLastSelItem != -1);
}

//////////////////////////////////////////////////////////////
//功能:在指定树项下加入对应的对象属性子项
//参数:pObj[in]		要加入属性所在的对象
//	enumType[in]	选择加入的属性类型()
//	hParent[in]		要加入项的父树项
//返回:加入子项的个数
//////////////////////////////////////////////////////////////
int CSymbolDesignDlg::AddSubObjectProperties(CDevObj *pObj, enumPropType enumType, HTREEITEM hParent)
{
	USES_CONVERSION;

	HTREEITEM hSubItem = NULL;
	CPropInfoArray* pPropArray = GetSubObjectProperties(pObj, enumType);
	int nCounts = pPropArray->GetSize();
	for (int i = 0; i < nCounts; i++)
	{
		CPropInfo& propinfo = (*pPropArray)[i];

		if (propinfo.m_dispid == CXPROPID_Name)
			continue;

		BOOL bAdded = FALSE;
		for (int j = 0; j < i; j++)
		{
			CPropInfo& propTemp = (*pPropArray)[j];
			if (propTemp.m_dispid == propinfo.m_dispid)
			{
				bAdded = TRUE;
				break;
			}
		}
		if (bAdded)
			continue;

		if (!pObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)))
			hSubItem = m_treeProps.InsertItem(OLE2T(propinfo.m_bstrDesc), hParent);
		else
			hSubItem = m_treeProps.InsertItem(OLE2T(propinfo.m_bstrName), hParent);
		int nIndex = m_arrPropName.Add(CString(OLE2T(propinfo.m_bstrName)));
		m_treeProps.SetItemData(hSubItem, (DWORD)nIndex);
	}
	delete pPropArray;

	return nCounts;
}

//功能: 更新指定对象节点的选中状态
void CSymbolDesignDlg::UpdateSubObjectCheckState(HTREEITEM hObjectItem, CSymbolProp* pProp)
{
 	int nObjIndex = m_treeProps.GetItemData(hObjectItem);
	BOOL bChecked = FALSE;

	HTREEITEM hPropItem = m_treeProps.GetChildItem(hObjectItem);
	while (hPropItem != NULL)
	{
		int nCount = pProp->m_arrObjNum.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			int nPropIndex = m_treeProps.GetItemData(hPropItem);
			CString strProp = m_arrPropName[nPropIndex];
			
			if (pProp->m_arrObjNum[i].m_nSubIndex == nObjIndex &&
				pProp->m_arrObjNum[i].m_strPropName == strProp)
			{
				m_treeProps.SetCheck(hPropItem, TRUE);
				bChecked = TRUE;
				break;
			}
			else
			{
				m_treeProps.SetCheck(hPropItem, FALSE);
			}
		}

		hPropItem = m_treeProps.GetNextSiblingItem(hPropItem);
	}

	m_treeProps.SetCheck(hObjectItem, bChecked);
}

///////////////////////////////////////////////////////////
//功能:查找指定的连接属性连接关系数组中是否有和指定子对象及
//		指定属性的连接
///////////////////////////////////////////////////////////
int FindRelatedProp(int nSubObj, 
		CArray<CSymbolProp::CRelatedProp, CSymbolProp::CRelatedProp&>& arrRelation, CString& strPropName)
{
	int nSize = arrRelation.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (arrRelation[i].m_nSubIndex == nSubObj && arrRelation[i].m_strPropName == strPropName)
			return i;
	}

	return -1;
}

// 保存用户在树图中选择相关属性的结果到内存
void CSymbolDesignDlg::SaveSubObjectCheckState(HTREEITEM hObjectItem, CSymbolProp *pProp)
{
	int nObjIndex = m_treeProps.GetItemData(hObjectItem);

	HTREEITEM hPropItem = m_treeProps.GetChildItem(hObjectItem);
	while (hPropItem != NULL)
	{
		int nPropIndex = m_treeProps.GetItemData(hPropItem);
		ASSERT(nPropIndex < m_arrPropName.GetSize() && nPropIndex >= 0);	//子对象索引号越界错

		// 得到子对象属性名称
		CString& strProp = m_arrPropName[nPropIndex];
		int nIndex = FindRelatedProp(nObjIndex, pProp->m_arrObjNum, strProp);

		if (m_treeProps.GetCheck(hPropItem))
		{	
			if (nIndex == -1)
			{	
				CSymbolProp::CRelatedProp prop;
				prop.m_nSubIndex = nObjIndex;
				prop.m_strPropName = strProp;
				pProp->m_arrObjNum.Add(prop);
			}
		}
		else
		{	
			if (nIndex >= 0)
			{	
				pProp->m_arrObjNum.RemoveAt(nIndex);
			}
		}

		hPropItem = m_treeProps.GetNextSiblingItem(hPropItem);
	}
}


// 保存用户对应于当前扩展变量的和子对象各种属性的关系
void CSymbolDesignDlg::SaveSubObjectChoice()
{
	if (m_nLastSelItem == -1)
		return;

	CSymbolProp* pProp = m_listProp.GetPropertyItem(m_nLastSelItem);
	if (pProp == NULL)
		return;

	//加入新的选项, 删除没被选中的项
	HTREEITEM hObjectItem = m_treeProps.GetRootItem();
	while (hObjectItem != NULL)
	{
		SaveSubObjectCheckState(hObjectItem, pProp);
		hObjectItem = m_treeProps.GetNextSiblingItem(hObjectItem);
	}
}

// 改变列出的子对象是否按同类型过滤
void CSymbolDesignDlg::OnCheckAll()
{
	int nState = m_btnCheck.GetCheck();
	m_bDisplayAllProp = nState != 0;

	SyncPropAndAdvance();
	SaveSubObjectChoice();
	UpdateSubObjectProperyTree();
}

enumPropType CSymbolDesignDlg::GetObjectPropType(CSymbolProp::CRelatedProp *pRelation, LPARAM lParam)
{
	USES_CONVERSION;
	ASSERT(pRelation != NULL);

	//获取子对象类型如必要转化为扩展类型
	CDevObjArray& arrSubObj = m_pObject->GetSubObjects();
	CDevObj* pSubObj = arrSubObj[pRelation->m_nSubIndex];

	if (pSubObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
	{	
		//组合对象
		int nSize = ((CSymbolObj *)pSubObj)->GetExPropCount();
		for(int i = 0; i < nSize; i++)
		{
			CSymbolProp ppt;
			((CSymbolObj *)pSubObj)->GetExPropInfo(i, &ppt);
			if (ppt.m_strName == pRelation->m_strPropName)
				return ppt.m_proptype;
		}
	}
	
	return CTypeInfoBrowse::CrackPropertyType(pSubObj->GetObjectUnknown(), T2OLE(pRelation->m_strPropName), lParam);
}

EXTERN_C const CLSID CLSID_CxExpressionCreator;
//const CLSID CLSID_ExpressionCreator = {0x227008D5,0xF29A,0x4E28,{0xB2,0xB3,0x96,0x4D,0x38,0x03,0x8C,0xB5}};

CSymbolObj* CSymbolDesignDlg::m_pExpCrtObject = NULL;
CSymbolPropList* CSymbolDesignDlg::m_pExpCrtList = NULL;

void CSymbolDesignDlg::ExpressionCreatorProc(BSTR* pbstrExpression, int iFlag) 
{
	ASSERT(m_pExpCrtObject != NULL);

	CComBSTR bstrExpression;
	bstrExpression.Attach(*pbstrExpression);
	
	switch (iFlag)
	{
	case 0:
		{
			CComPtr<ICxExpressionCreator> spExprCrt;
			CComPtr<ICxSymbolTreePage> spSymbolTreePage;

			spExprCrt.CoCreateInstance(CLSID_CxExpressionCreator);

			spExprCrt->put_UseTagSymbol(VARIANT_FALSE);
			spExprCrt->put_Expression(bstrExpression);
			spExprCrt->AddGeneralSymbolPage(CX_ALL_SYMBOL);

			spExprCrt->GetMainSymbolTreePage(&spSymbolTreePage);
			if (spSymbolTreePage != NULL)
			{
				COleDispatchDriver ddTemp(spSymbolTreePage, FALSE);
				
				OLE_HANDLE hItem = NULL;
				static BYTE BASED_CODE parms[] =
					VTS_HANDLE VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;

				CString strName = "symbol";//m_pExpCrtObject->GetDisplayName();
				CComQIPtr<IDispatch> spObject = m_pExpCrtObject->GetObjectUnknown();
				IUnknown* pUnk = NULL;
				if (m_pExpCrtList)
					pUnk = (IUnknown *)m_pExpCrtList->EnumItem();
				ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_HANDLE, &hItem, parms, 
						NULL, strName, spObject, pUnk);
				if (pUnk)
					pUnk->Release();

				for (int i = 0; i < m_pExpCrtObject->m_arrSubObj.GetSize(); i++)
				{
					CDevObj* pObj = m_pExpCrtObject->m_arrSubObj[i];
					strName = pObj->GetDisplayName();
					spObject = pObj->GetObjectUnknown();
					ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, hItem, strName, spObject, NULL);
				}
			}
			
			spExprCrt->ShowDialog();
			spExprCrt->get_Expression(&bstrExpression);


// 			IDispatchPtr pIExprCrt;
// 			pIExprCrt.CreateInstance(CLSID_CxExpressionCreator);
// 
// 			COleDispatchDriver dd(pIExprCrt, FALSE);
// 	
// 			dd.SetProperty(2, VT_BSTR, strExpression);
// 
// 			IDispatchPtr pIExprTreepage;
// 			dd.InvokeHelper(4, DISPATCH_METHOD, VT_DISPATCH, &pIExprTreepage, NULL);
// 			if (pIExprTreepage)
// 			{
// 				COleDispatchDriver ddTemp(pIExprTreepage, FALSE);
// 				CString strCaption(_T("图符"));
// 				ddTemp.SetProperty(1, VT_BSTR, strCaption);
// 				if (m_pExpCrtObject)
// 				{
// 					static BYTE BASED_CODE parms[] =
// 						VTS_BSTR VTS_DISPATCH VTS_UNKNOWN;
// 					CString strName = "symbol";//m_pExpCrtObject->GetDisplayName();
// 					IDispatch* pIObj = (IDispatch *)m_pExpCrtObject->GetObjectUnknown();
// 					IUnknown* pUnk = NULL;
// 					if (m_pExpCrtList)
// 						pUnk = (IUnknown *)m_pExpCrtList->EnumItem();
// 					ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strName, pIObj, pUnk);
// 					if (pUnk)
// 						pUnk->Release();
// 					for (int i = 0; i < m_pExpCrtObject->m_arrSubObj.GetSize(); i++)
// 					{
// 						CDevObj* pObj = m_pExpCrtObject->m_arrSubObj[i];
// 						strName = pObj->GetDisplayName();
// 						pIObj = (IDispatch *)pObj->GetObjectUnknown();
// 						ddTemp.InvokeHelper(2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strName, pIObj, NULL);
// 					}
// 				}
// 			}
// 
// 			dd.InvokeHelper(1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
// 			dd.GetProperty(2, VT_BSTR, (void*)&strExpression);
		}
		break;
	case 1: //原始->显示
		{
			CString strName;
			CString strOld;
			CString strExpression = bstrExpression;

			int nCount = m_pExpCrtObject->m_arrSubObj.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				strName = m_pExpCrtObject->m_arrSubObj[i]->GetDisplayName();
				strOld.Format("object%03d", i);
				strExpression.Replace(strOld, strName);
			}

			bstrExpression = strExpression;
		}
		break;
	case 2: //显示->原始
		{
			CString strName;
			CString strNew;
			CString strExpression = bstrExpression;

			int nCount = m_pExpCrtObject->m_arrSubObj.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				strName = m_pExpCrtObject->m_arrSubObj[i]->GetDisplayName();
				strNew.Format("object%03d", i);
				strExpression.Replace(strName, strNew);
			}

			bstrExpression = strExpression;
		}
		break;
	}

	*pbstrExpression = bstrExpression.Copy();

	return;
}

HRESULT HelperGetPropertyByName(IDispatch* pObject, LPCTSTR lpszPropName, VARIANT* pVar)
{
	USES_CONVERSION;
	DISPID dwDispID;
	
	LPOLESTR psz = T2OLE(lpszPropName);
	HRESULT hr = pObject->GetIDsOfNames(IID_NULL, &psz, 1, LOCALE_USER_DEFAULT, &dwDispID);
	if (FAILED(hr))
		return hr;
	
	DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
	return pObject->Invoke(dwDispID, IID_NULL,
		LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
		&dispparamsNoArgs, pVar, NULL, NULL);
}

int HelperGetConnectState(IDispatch* pObject, LPCTSTR lpszPropName)
{
	USES_CONVERSION;
	HRESULT hr;
	DISPID dispid;
	LPOLESTR psz = L"GetConnectState";
	
	hr = pObject->GetIDsOfNames(IID_NULL, &psz, 1, LOCALE_USER_DEFAULT, &dispid);
	if (FAILED(hr))
		return -1;

	DISPID dispidProp;
	psz = T2OLE(lpszPropName);
	hr = pObject->GetIDsOfNames(IID_NULL, &psz, 1, LOCALE_USER_DEFAULT, &dispidProp);
	if (FAILED(hr))
		return -1;

	VARIANTARG* pvars = new VARIANTARG[1];
	for (int i = 0; i < 1; i++)
		VariantInit(&pvars[i]);
	pvars[0].vt = VT_I4;
	pvars[0].lVal = dispidProp;
//	int nState;
//	pvars[0].vt = VT_BYREF|VT_I4;
//	pvars[0].plVal = (long *)&nState;
	DISPPARAMS disp = {pvars, NULL, 1, 0};
	CComVariant varResult;
	hr = pObject->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT,
		DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
	delete[] pvars;

	return varResult.lVal;
}

//////////////////////////////////////////////////////////////
//功能:启动高级属性编缉对话框
//////////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnButtonAdvance() 
{
	ASSERT(m_pObject != NULL);
	SaveSubObjectChoice();	//保存最新结果用于以后操作

	CString strProp;
	int nIndex = 0;
	int nSubIndex = -1;
		
	//查找属性列表中所对应的扩展属性
	CSymbolProp* pItem = m_listProp.GetPropertyItem(m_listProp.GetSelectPropertyItem());
	if (pItem == NULL)
		return;

	//查找树图中当前选中的子对象及其属性
	HTREEITEM hItem = m_treeProps.GetSelectedItem();
	if (hItem != NULL)
	{
		HTREEITEM hParent = m_treeProps.GetParentItem(hItem);
		if (hParent == NULL)	//被选中的不是属性项
			return;
	
		nSubIndex = (int)(m_treeProps.GetItemData(hParent));
		if (nSubIndex < 0)	//没有数据
			return;
	
		nIndex = (int)(m_treeProps.GetItemData(hItem));
		strProp = m_arrPropName[nIndex];
	}

	CSymbolProp::CRelatedProp *pRelProp = NULL;
	int nCount = pItem->m_arrObjNum.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		//查找子对象及该属性记录
		if (pItem->m_arrObjNum[i].m_strPropName == strProp &&
			pItem->m_arrObjNum[i].m_nSubIndex == nSubIndex)
		{	
			pRelProp = &(pItem->m_arrObjNum[i]);
			break;
		}
	}

	//创建动态属性编辑器
	IDynamicFrm* pDynamicFrm;
	HRESULT hResult = ::CoCreateInstance(CLSID_DynamicFrm, NULL, 
		CLSCTX_INPROC_SERVER, IID_IUnknown,	reinterpret_cast<void**>(&pDynamicFrm));
	if (FAILED(hResult))
	{
		MessageBox(_T("创建动态框失败！"));
		return;
	}

	//设置动态属性
	CDynamicPropInfo propinfo;
	propinfo.spUnkObject = m_pObject->GetObjectUnknown();
	propinfo.spUnkDynamic = pRelProp->m_pDynamicObject;

	CDevObj* pSubObj = m_pObject->m_arrSubObj[pRelProp->m_nSubIndex];
	IDispatchPtr pDisp = pSubObj->GetObjectUnknown();
	propinfo.bSupportMouseInput = (HelperGetConnectState(pDisp, strProp) & 0x1) != 0;
	HelperGetPropertyByName(pDisp, strProp, &propinfo.varCurrentValue); // 2005.1.5

	m_pExpCrtObject = m_pObject;
	m_pExpCrtList = &m_listProp;
	propinfo.pfnExpression = ExpressionCreatorProc;

	propinfo.bstrPropertyName = pItem->m_strName;
	CEnumPropItemArray* pArrEnumItem = NULL;
	propinfo.nType = GetObjectPropType(pRelProp, (LPARAM)&pArrEnumItem);
	if (propinfo.nType == PROPTYPE_ENUM)
	{
		ASSERT(pArrEnumItem != NULL);
		for (i = 0; i < pArrEnumItem->GetSize(); i++)
		{
			CEnumPropItem item;
			item.m_bstrName = (*pArrEnumItem)[i].m_bstrName;
			item.m_bstrDesc = pSubObj->IsKindOf(RUNTIME_CLASS(CCtrlObj))  
				? (*pArrEnumItem)[i].m_bstrName : (*pArrEnumItem)[i].m_bstrDesc;
			item.m_varValue =(*pArrEnumItem)[i].m_varValue;
			propinfo.m_arrEnumPropItems.Add(item);
		}
	}
	pDynamicFrm->put_PropertyInfo(long(&propinfo));
	if (propinfo.spUnkDynamic == NULL)
	{
		CComBSTR bstrSource = L"Symbol.";
		bstrSource += propinfo.bstrPropertyName;
		pDynamicFrm->put_Source(bstrSource);
	}
	if (pDynamicFrm->EditDynamic(long(m_hWnd), 0, 0) == S_OK)
		pRelProp->m_pDynamicObject = propinfo.spUnkDynamic;


	if (pArrEnumItem)
		delete pArrEnumItem;
	
	//清理
	pDynamicFrm->Release();
}

/////////////////////////////////////////////////////////////////
//功能:根据情况设置"高级"按钮的状态
/////////////////////////////////////////////////////////////////
void CSymbolDesignDlg::SyncPropAndAdvance()
{
	USES_CONVERSION;
	IDispatchPtr pIValue = GetDlgItem(IDC_PROP_VALUE)->GetControlUnknown();
	COleDispatchDriver dd(pIValue, FALSE);
	dd.InvokeHelper(4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);

	CDevObj* pSubObj = NULL;
	BOOL bState = FALSE;
	HTREEITEM hItem = m_treeProps.GetSelectedItem();
	if (hItem != NULL)
	{
		HTREEITEM hParent = m_treeProps.GetParentItem(hItem);
		if (hParent != NULL)
		{
			if (m_treeProps.GetCheck(hItem))
				bState = TRUE;

			int nSubIndex = -1;
			nSubIndex = (int)(m_treeProps.GetItemData(hParent));
			if (nSubIndex >= 0)	
			{
				int nIndex = (int)(m_treeProps.GetItemData(hItem));
				pSubObj = m_pObject->m_arrSubObj[nSubIndex];
				CString strProp = m_arrPropName[nIndex];
				
				CEnumPropItemArray* pArrEnumItem = NULL;
				enumPropType type = CTypeInfoBrowse::CrackPropertyType(pSubObj->GetObjectUnknown(), T2OLE(strProp), (LPARAM)&pArrEnumItem);
				dd.SetProperty(1, VT_I4, long(type));

				if (type == PROPTYPE_ENUM)
				{
					ASSERT(pArrEnumItem != NULL);

					static BYTE BASED_CODE parms[] =
						VTS_BSTR VTS_I4;
					for (int i = 0; i < pArrEnumItem->GetSize(); i++)
					{
						dd.InvokeHelper(3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, 
								OLE2T(pSubObj->IsKindOf(RUNTIME_CLASS(CCtrlObj)) ? (*pArrEnumItem)[i].m_bstrName : (*pArrEnumItem)[i].m_bstrDesc), 
								(*pArrEnumItem)[i].m_varValue);
					}
				}

				_variant_t value;
				CTypeChanger::GetPropertyByName(pSubObj->GetObjectUnknown(), strProp, &value);
				
				int nSelItem = m_listProp.GetSelectPropertyItem();
				if (nSelItem != -1)
				{
					CSymbolProp* pVar = m_listProp.GetPropertyItem(nSelItem);
					if (pVar != NULL)
						dd.SetProperty(6, VT_VARIANT, &pVar->m_value);
				}
				//
				dd.SetProperty(2, VT_VARIANT, &value);
				dd.SetProperty(5, VT_UNKNOWN, pSubObj->GetObjectUnknown());
				
				if (pArrEnumItem)
					delete pArrEnumItem;
			}	
		}
		else
		{
			int nSubIndex = -1;
			nSubIndex = (int)(m_treeProps.GetItemData(hItem));
			pSubObj = m_pObject->m_arrSubObj[nSubIndex];
		}
	}

	if (m_pPreviewWnd != NULL)
		m_pPreviewWnd->SetSelectObject(pSubObj);

	m_btnAdvance.EnableWindow(bState);
}

void CSymbolDesignDlg::OnPropValueBeginEdit()
{
	HTREEITEM hItem = m_treeProps.GetSelectedItem();
	if (hItem != NULL)
	{
		HTREEITEM hParent = m_treeProps.GetParentItem(hItem);
		if (hParent != NULL)
		{
			int nSubIndex = -1;
			nSubIndex = (int)(m_treeProps.GetItemData(hParent));
			if (nSubIndex >= 0)	//没有数据
			{
				int nIndex = (int)(m_treeProps.GetItemData(hItem));
				m_pPropValueEditObj = m_pObject->m_arrSubObj[nSubIndex];
				m_strPropValueEditName = m_arrPropName[nIndex];
			}	
		}
	}
}

void CSymbolDesignDlg::OnPropValueChanged()
{
	if (m_pPropValueEditObj != NULL)
	{
		IDispatchPtr pIValue = GetDlgItem(IDC_PROP_VALUE)->GetControlUnknown();
		COleDispatchDriver dd(pIValue, FALSE);

		_variant_t value;
		dd.GetProperty(2, VT_VARIANT, &value);
		
		CTypeChanger::PutPropertyByName(m_pPropValueEditObj->GetObjectUnknown(), m_strPropValueEditName, &value);
		
		m_pPreviewWnd->Invalidate();

		m_pPropValueEditObj = NULL;
	}
}

void CSymbolDesignDlg::OnSelchangedTreeProp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	OnPropValueChanged();
	SyncPropAndAdvance();
	
	*pResult = 0;
}

LRESULT CSymbolDesignDlg::OnObjectSelected(WPARAM wParam, LPARAM lParam)
{
	CDevObj* pObj = (CDevObj *)wParam;
	CDevObjArray& arrObj = m_pObject->GetSubObjects();
	
	HTREEITEM hRoot = m_treeProps.GetRootItem();
	while (hRoot != NULL)
	{
		int iIndex = (int)m_treeProps.GetItemData(hRoot);
		if (pObj == arrObj[iIndex])
		{
			m_treeProps.SelectItem(hRoot);
			break;
		}
		
		hRoot = m_treeProps.GetNextSiblingItem(hRoot);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////
//功能:初始化预览窗口
///////////////////////////////////////////////////////////////////
BOOL CSymbolDesignDlg::InitPreview()
{
	CRect rect;
	GetDlgItem(IDC_PREVIEW)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_pPreviewWnd = new CSymbolPreviewWnd(m_pObject);
	return m_pPreviewWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, IDC_PREVIEW);
}

///////////////////////////////////////////////////////////////////
//功能:制作子对象属性列表
//参数:enumType[in]		过滤的类型, 如为PROPTY_UNKNOWN则不过滤
///////////////////////////////////////////////////////////////////
CPropInfoArray* CSymbolDesignDlg::GetSubObjectProperties(CDevObj *pSubObj, enumPropType enumType)
{
	ASSERT(pSubObj);
	CPropInfoArray* pPropArray = new CPropInfoArray;
	ASSERT(pPropArray);

	CPropInfo info;
	BSTR bstrName;
	BSTR bstrDocString;

	if (pSubObj->IsKindOf(RUNTIME_CLASS(CSymbolObj)))
	{	//组合对象
		ICxEnumExtendProperty* pIEx = ((CSymbolObj*)pSubObj)->EnumExtendProperty();
		pIEx->Reset();
		LONG nIndex = 0;
		while(pIEx->Next(&nIndex) != S_FALSE)
		{
			pIEx->GetName(&bstrName);
			pIEx->GetDescription(&bstrDocString);
			pIEx->GetType((LONG*)(&info.m_proptype));
			info.m_bstrDesc = bstrDocString;
			info.m_bstrName = bstrName;
			::SysFreeString(bstrName);
			::SysFreeString(bstrDocString);

			if (enumType == info.m_proptype)
				pPropArray->Add(info);
		}
		pIEx->Release();
	}
	
	CTypeInfoBrowse::CrackProperty(pSubObj->GetObjectUnknown(), *pPropArray, enumType);
	
	return pPropArray;
}

void CSymbolDesignDlg::OnClickTreeProp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//树结点被点中（不一定是项目选中），发送通知更新数结点的CHECKBOX
	PostMessage(WM_TREEITEMCLICK, 0, 0);
	
	*pResult = 0;
}

void CSymbolDesignDlg::OnKeydownTreeProp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	
	if (pTVKeyDown->wVKey == VK_SPACE)
		PostMessage(WM_TREEITEMCLICK, 0, 0);
	
	*pResult = 0;
}

void CSymbolDesignDlg::OnPropTreeItemClick()
{
	UpdateSubObjectItem();
	SyncPropAndAdvance();
}

/////////////////////////////////////////////////////////
//功能:扩展属性的类型改变扣的通知函数
/////////////////////////////////////////////////////////
void CSymbolDesignDlg::OnChangeTagType()
{
	UpdateSubObjectProperyTree();
}

void CSymbolDesignDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (nIDCtl == IDC_GROUPTAG_LIST)
		lpMeasureItemStruct->itemHeight = 16;
	
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


