/**
 * @file DefinePurposeDlg.cpp
 * @brief KDefinePurposeDlg 구현
 * @author 
 * @date 2010.07.30
 * @remark
 */



#include "stdafx.h"
#include "KmzApp.h"
#include "Purpose.h"
#include "DefinePurposeDlg.h"
#include "PurposeNewDlg.h"
#include "KmzSystem.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_INDEX  = 1,
	_2_COLUMN_NAME   = 2
};

/******************************************************************************
* KDefinePurposeDlg                                                           *
******************************************************************************/
IMPLEMENT_DYNAMIC(KDefinePurposeDlg, KResizeDialogEx)

//static std::vector<KPurpose*> *m_pvecPurposeInfo;
/**************************************
* Constructors and Destructor         *
**************************************/
KDefinePurposeDlg::KDefinePurposeDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefinePurposeDlg::IDD, pParent)
{

	m_strSelectedPurposeNames = _T("");
	m_strSelectedPurposeID = _T("");
	m_strSelectedPurposeGroup = _T("");

	m_strClickedPurpose = _T("");

	m_bPassenger = KmzSystem::IsPassengerSystem();

	if( true == m_bPassenger )
	{
		//m_strPurpose = _T("Defined Purpose");
		m_strPurpose = _T("Trip Purpose");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strPurpose = _T("통행목적");
		}
	}
	else 
	{
		m_strPurpose = _T("Defined Commodity");
	}
}

KDefinePurposeDlg::KDefinePurposeDlg(std::vector<KPurpose*> &vecPurposeInfo)
{
	
}

KDefinePurposeDlg::~KDefinePurposeDlg()
{

}



/**************************************
* Operations                          *
**************************************/
void KDefinePurposeDlg::initializeTreeItems(void)
{
	//m_trPurpose.InsertItem(_T("출근"));
	//m_trPurpose.InsertItem(_T("등교"));
	//m_trPurpose.InsertItem(_T("귀가"));

	/*
	HTREEITEM hParent = m_trPurpose.InsertItem( _T("가정기반") );
	m_trPurpose.InsertItem( _T("출근"), hParent );
	m_trPurpose.InsertItem( _T("쇼핑"), hParent );
	m_trPurpose.Expand(hParent, TVE_EXPAND);

	hParent = m_trPurpose.InsertItem( _T("비가정기반") );
	m_trPurpose.InsertItem( _T("출근"), hParent );
	m_trPurpose.Expand(hParent, TVE_EXPAND);
	*/
}

void KDefinePurposeDlg::SetPurpose(std::vector<KPurpose*> &a_vecPurposeInfo)
{
	m_pVecPurposeInfo = &a_vecPurposeInfo;
	std::vector<KPurpose*>::iterator itPurpose, itPurposeEnd = m_pVecPurposeInfo->end();
	for(itPurpose = m_pVecPurposeInfo->begin(); itPurpose != itPurposeEnd; ++itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;
		TPurposeManageInfo* pPurposeManageInfo = new TPurposeManageInfo();
		
		pPurposeManageInfo->emManage    = EMPurposeManageNone;
		pPurposeManageInfo->strName	    = pPurpose->PurposeName();
		pPurposeManageInfo->nObjectGroupCode = pPurpose->PurposeGroup();
		int nPurposeId				   = pPurpose->PurposeID();
		pPurposeManageInfo->nDetailObjectId  = nPurposeId;
		pPurposeManageInfo->strNewName = _T("");

		m_listPurposeInfoManager.push_back(pPurposeManageInfo);

		if(m_nMaxPurposeId < nPurposeId )
		{
			m_nMaxPurposeId = nPurposeId;
		}
	}
}


/**************************************
* Overrides                           *
**************************************/
BOOL KDefinePurposeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CBitmap bmp;
	bmp.LoadBitmap( IDB_3STATE_CHECK );

	m_ilTreeState.Create( 13, 13, ILC_COLOR24 | ILC_MASK, 4, 1 );
	m_ilTreeState.Add( &bmp, RGB(255, 255, 255) );

	KReportCtrlSetting::Default(m_reportPurpose, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_reportPurpose);

#pragma region old
//  예전 delete 버전인 한개의 행 삭제를 하다보면 KReportCtrlSetting::Default()로 셋팅한 스타일에서는 제대로 작동 되지 않았다.
// 	m_reportPurpose.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_reportPurpose.GetReportHeader()->AllowColumnSort(TRUE);
// 	m_reportPurpose.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_reportPurpose.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_reportPurpose.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
// 
// 	m_reportPurpose.SetMultipleSelection(FALSE);
// 	m_reportPurpose.AllowEdit(TRUE);
// 	m_reportPurpose.EditOnClick(TRUE);
// 	m_reportPurpose.SelectionEnable(TRUE);
#pragma endregion old
	m_reportPurpose.Populate();

	m_bPassenger = KmzSystem::IsPassengerSystem();

	if( true == m_bPassenger )
	{
		//SetWindowText(_T("Define Purpose"));
		SetWindowText(_T("Trip Purpose"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetWindowText(_T("통행목적 설정"));
		}
	}
	else 
	{
		SetWindowText(_T("Define Commodity"));
	}
	
	initPurposeList( m_bPassenger );
	updatePurposeList();
	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void KDefinePurposeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRJSTC_PURPOSE, m_strPurpose);
	//DDX_Control(pDX, IDC_PRJTRE_PURPOSE, m_trPurpose);
	DDX_Control(pDX, IDC_REPORT1, m_reportPurpose);
}



BEGIN_MESSAGE_MAP(KDefinePurposeDlg, KResizeDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &KDefinePurposeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KDefinePurposeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &KDefinePurposeDlg::OnBnClickedButtonDel)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KDefinePurposeDlg::OnPurposeValueChanged)
	ON_BN_CLICKED(IDC_CHECK1, &KDefinePurposeDlg::OnBnClickedCheckSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_BEGINEDIT, IDC_REPORT1, &KDefinePurposeDlg::OnBeginPurposeNameEdit)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, &KDefinePurposeDlg::OnReportItemClick)
END_MESSAGE_MAP()


void KDefinePurposeDlg::initPurposeList(bool bPassengerMode)
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(_1_COLUMN_INDEX, _T("No."), 20)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("순서"));
    }

	if(true == bPassengerMode)
	{
		pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Purpose"), 80)); 
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("통행목적"));
		}
	}
	else
	{
		pColumn = m_reportPurpose.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Commodity"), 80)); 
	}
	
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetAlignment(DT_LEFT);
}

void KDefinePurposeDlg::updatePurposeList(void)
{
    m_reportPurpose.ResetContent();
	std::list<TPurposeManageInfo*>::iterator itPurpose, itEnd = m_listPurposeInfoManager.end();

	int nIdx = 1;
	for(itPurpose = m_listPurposeInfoManager.begin(); itPurpose != itEnd; ++itPurpose)
	{
		TPurposeManageInfo* pInfo = *itPurpose;
		if(EMPurposeManageRemove == pInfo->emManage)
		{
			continue;
		}

		CXTPReportRecord*     pRecord = m_reportPurpose.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = nullptr;

		// select
		pItem  = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		// No.
		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nIdx, _T("%.0f")));

		// Name
		if(EMPurposeManageNone == pInfo->emManage)
		{
			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strName));
		}
		else 
		{
			pItem  = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strNewName));
		}

		// 2019.04.10 수정 : 기본목적(목적ID가 1000보다 작은것)은 이름을 수정 할 수 없도록 한다.
		if (pInfo->nDetailObjectId < 1000)
		{
			pItem->SetEditable(FALSE);
		}

		pItem->SetItemData((DWORD_PTR)pInfo);

		nIdx++;
	}

	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	m_reportPurpose.Populate();
 }

/**************************************
* Message handlers                    *
**************************************/
int KDefinePurposeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (KResizeDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}


void KDefinePurposeDlg::OnBnClickedOk()
{
	/*
	CString strState;
	HTREEITEM hRoot = m_trPurpose.GetRootItem();
	while( NULL != hRoot )
	{
		int nState = m_trPurpose.getStateImage( hRoot );
		strState.Format( _T("State = %d"), nState );
		AfxMessageBox( strState );

		HTREEITEM hItem = m_trPurpose.GetChildItem( hRoot );
		while( NULL != hItem )
		{
			nState = m_trPurpose.getStateImage( hItem );
			strState.Format( _T("State = %d"), nState );
			AfxMessageBox( strState );
			hItem = m_trPurpose.GetNextSiblingItem( hItem );
		}
		
		hRoot = m_trPurpose.GetNextSiblingItem( hRoot );
	}
	*/

	UpdateData(TRUE);
	
	/* 데이터를 추가 할 때 이 데이터는 
	KPurpose* pPurpose = *(m_pvecPurposeInfo->begin());
	KEMPurposeGroup emGroup = pPurpose->PurposeGroup();
	*/
	CString strPurposeName = _T("");
	CString strPurposeId = _T("");
	CString strPurposeGroup = _T("");
	int nPurposeId = -1;
	int nSelectID(0);

	m_pVecPurposeInfo->clear();
	m_strSelectedPurposeNames.Empty();
	
	std::list<TPurposeManageInfo*>::iterator itPurpose, itEnd = m_listPurposeInfoManager.end();
	for(itPurpose = m_listPurposeInfoManager.begin(); itPurpose != itEnd; ++itPurpose)
	{
		TPurposeManageInfo* pPurposeInfo = *itPurpose;

		if(EMPurposeManageNone == pPurposeInfo->emManage)
		{
			strPurposeName = pPurposeInfo->strName;
		}
		else if(EMPurposeManageNew == pPurposeInfo->emManage)
		{
			strPurposeName = pPurposeInfo->strNewName;
		}
		else if(EMPurposeManageModify == pPurposeInfo->emManage)
		{
			strPurposeName = pPurposeInfo->strNewName;
		}
		else if(EMPurposeManageRemove == pPurposeInfo->emManage)
		{
			delete pPurposeInfo;
			continue;
		}

		if( (strPurposeName.Trim()).Compare(_T("")) == 0 )
		{
			delete pPurposeInfo;
			continue;
		}

		if (KmzSystem::IsFreightSystem())
		{
			strPurposeName.Replace(_T(" "), _T("_"));
		}

		nPurposeId = pPurposeInfo->nDetailObjectId;

		KPurpose* pPurpose = new KPurpose(m_emPurposeGroup, nPurposeId, strPurposeName);
		
		m_pVecPurposeInfo->push_back(pPurpose);

		if(false == m_strSelectedPurposeNames.IsEmpty())
		{
			m_strSelectedPurposeNames += _T(", ");
			m_strSelectedPurposeID += _T(", ");
		}

		strPurposeId.Format(_T("%d"), nPurposeId);
		strPurposeGroup.Format(_T("%d"), m_emPurposeGroup);

		m_strSelectedPurposeNames += (strPurposeName);
		m_strSelectedPurposeID += (strPurposeId);
		m_strSelectedPurposeGroup = strPurposeGroup;

		if (nSelectID < _ttoi(strPurposeId))
		{
			nSelectID = _ttoi(strPurposeId);
		}
	}

	m_nSelectMaxPurposeID = nSelectID;

	UpdateData(FALSE);
	KResizeDialogEx::OnOK();
}

/*
void KDefinePurposeDlg::OnBnClickedButtonAdd()
{
	
	bool bDuplicate = false;
	bool bCancel = false;
	CString strPurposeName = _T("");

	do 
	{
		KPurposeNewDlg dlg;
		dlg.m_strPurposeName = _T("");
		if(dlg.DoModal() == IDOK)
		{
			bDuplicate = false;
			std::list<TPurposeManageInfo*>::iterator itPurpose, itEnd = m_PurposeInfoList.end();
			for( itPurpose = m_PurposeInfoList.begin(); itPurpose != itEnd; ++itPurpose)
			{
				TPurposeManageInfo* pPurposeInfo = *itPurpose;
				if(EMPurposeManageNone == pPurposeInfo->emManage)
				{
					if(pPurposeInfo->strName.Compare(dlg.m_strPurposeName) == 0)
					{
						bDuplicate = true;
						break;
					}
				}
				else if((EMPurposeManageNew == pPurposeInfo->emManage) || (EMPurposeManageRemove == pPurposeInfo->emManage))
				{
					if(pPurposeInfo->strNewName.Compare(dlg.m_strPurposeName) == 0)
					{
						bDuplicate = true;
						break;
					}
				}
			}

			if(true == bDuplicate)
			{
				AfxMessageBox(_T("입력한 통행목적 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
			}
			else
			{
				strPurposeName = dlg.m_strPurposeName;
			}
		}
		else
		{
			bCancel = true;
		}
	} while ((false == bCancel) && (true == bDuplicate));

	if(true == bCancel)
	{
		return;
	}

	TPurposeManageInfo* pNewPurposeInfo = new TPurposeManageInfo;
	pNewPurposeInfo->emManage = EMPurposeManageNew;
	pNewPurposeInfo->strNewName = strPurposeName;
	pNewPurposeInfo->strSysDefineYN = _T("N");
	pNewPurposeInfo->nDetailObjectId = -1;
	m_PurposeInfoList.push_back(pNewPurposeInfo);

	CXTPReportRecord* pRecord = m_reportPurpose.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPurposeName));
	pItem->SetItemData((DWORD_PTR)pNewPurposeInfo);

	m_reportPurpose.Populate();

	CXTPReportRow* pRow = m_reportPurpose.GetRows()->GetAt(m_reportPurpose.GetRows()->GetCount() - 1);
	m_reportPurpose.SetFocusedRow(pRow);
}
*/

void KDefinePurposeDlg::OnBnClickedButtonAdd()
{
// 	if(m_reportPurpose.GetRecords()->GetCount() > 0)
// 	{
// 		m_reportPurpose.GetRows()->GetAt(0)->SetSelected(TRUE);
// 	}

	TPurposeManageInfo* pNewPurposeInfo = new TPurposeManageInfo;
	pNewPurposeInfo->emManage        = EMPurposeManageNew;
	pNewPurposeInfo->strNewName      = _T("");
	pNewPurposeInfo->strSysDefineYN  = _T("N");
	pNewPurposeInfo->nDetailObjectId = ++m_nMaxPurposeId;
	m_listPurposeInfoManager.push_back(pNewPurposeInfo);

	int nTotalRowCount = m_reportPurpose.GetRows()->GetCount();
	
	CXTPReportRecord*     pRecord = m_reportPurpose.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItem());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);

	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber( (double)(nTotalRowCount +1), _T("%.0f") ));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetItemData((DWORD_PTR)pNewPurposeInfo);

	m_reportPurpose.Populate();

	nTotalRowCount = m_reportPurpose.GetRows()->GetCount();
	CXTPReportRow* pRow = m_reportPurpose.GetRows()->GetAt(nTotalRowCount - 1);
	pRow->SetSelected(TRUE);
	
	//추가된 컬럼에 Focus주기
	XTP_REPORTRECORDITEM_ARGS args(&m_reportPurpose, pRow, m_reportPurpose.GetColumns()->GetAt(_2_COLUMN_NAME));
	m_reportPurpose.EditItem(&args);
}

void KDefinePurposeDlg::OnBnClickedButtonDel()
{
	try
	{
		CXTPReportRecords*    pRecords     = m_reportPurpose.GetRecords();
		CXTPReportRecord*     pRecord      = nullptr;
		CXTPReportRecordItem* pItem        = nullptr;
		TPurposeManageInfo*   pPurposeInfo = nullptr;
		
		int  nCount = pRecords->GetCount();
		BOOL bChecked(FALSE);
		int  nDelCount(0);

		if (nCount < 1)
			return;
		
		for (int i= 0; i< nCount; i++)
		{
			pRecord      = pRecords->GetAt(i);
			pItem        = pRecord->GetItem(_0_COLUMN_SELECT);
			bChecked     = pItem->IsChecked();

			if (FALSE == bChecked)
				continue;

			pItem        = pRecord->GetItem(_2_COLUMN_NAME);
			pPurposeInfo = (TPurposeManageInfo*)pItem->GetItemData();

			if(EMPurposeManageNew == pPurposeInfo->emManage)
			{
 				pPurposeInfo->emManage = EMPurposeManageRemove;
			}
			else
			{
 				pPurposeInfo->emManage = EMPurposeManageRemove;
			}

			nDelCount++;
		}

		if (nDelCount< 1)
			return;

		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
				return;
		}
		else {
			if (AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO) == IDNO)
				return;
		}

		updatePurposeList();

#pragma region old
// 		CXTPReportRow* pPurposeRow = m_reportPurpose.GetSelectedRows()->GetAt(0);
// 
// 		if(NULL == pPurposeRow)
// 			return;
// 
// 		int nIndex =  pPurposeRow->GetIndex();
// 
// 		CXTPReportRecord*     pPurposeRecord = pPurposeRow->GetRecord();
// 		CXTPReportRecordItem* pPurposeItem   = pPurposeRecord->GetItem(_2_COLUMN_NAME);
// 		TPurposeManageInfo*   pPurposeInfo   = (TPurposeManageInfo*)(pPurposeItem->GetItemData());
// 
// 		if(EMPurposeManageNew == pPurposeInfo->emManage)
// 		{
// 
// 			//delete pPurposeInfo;
// 			pPurposeInfo->emManage = EMPurposeManageRemove;
// 			m_reportPurpose.GetRecords()->RemoveRecord(pPurposeRecord);
// 			m_reportPurpose.Populate();
// 		}
// 		else
// 		{
// 
// 			pPurposeInfo->emManage = EMPurposeManageRemove;
// 			updatePurposeList();
// 		}
// 
// 		if(nIndex > 1)	// 선택 행을 삭제 한 후 focus는 이전 행으로 셋팅
// 		{
// 
// 			CXTPReportRow* pRow = m_reportPurpose.GetRows()->GetAt(nIndex - 1);
// 			pRow->SetSelected(TRUE);
// 			m_reportPurpose.SetFocusedRow(pRow);
// 			m_reportPurpose.SetTopRow(nIndex - 1);
// 		}
#pragma endregion old
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefinePurposeDlg::OnPurposeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CString strPurpose = pItemNotify->pItem->GetCaption(NULL);

	//숫자, 특수문자 필터링
	int nLen = strPurpose.GetLength();

	for (int i = 0; i < nLen; ++i)
	{
		bool bOK = false;

		if (isdigit(strPurpose.GetAt(i)) || isalpha(strPurpose.GetAt(i)) || strPurpose.GetAt(i) <= 0 || 127 < strPurpose.GetAt(i))
			bOK = true;

		if (!bOK)
		{
			AfxMessageBox(_T("특수문자를 입력할 수 없습니다."));

			CXTPReportInplaceEdit* pEdit = m_reportPurpose.GetInplaceEdit();
			if(NULL != pEdit)
			{
				pEdit->SetSel(0, -1);

				CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);

				pItem->SetCaption(_T(""));
				pItem->SetValue(_T(""));

				m_reportPurpose.RedrawControl();
			}

			return;
		}
	}

	TPurposeManageInfo* pEditInfo = (TPurposeManageInfo*)(pItemNotify->pItem->GetItemData());

	bool bDuplicate = false;
	std::list<TPurposeManageInfo*>::iterator itPurpose, itEnd = m_listPurposeInfoManager.end();
	for(itPurpose = m_listPurposeInfoManager.begin(); itPurpose != itEnd; ++itPurpose)
	{
		TPurposeManageInfo* pPurposeInfo = *itPurpose;
		if(EMPurposeManageNone == pPurposeInfo->emManage)
		{
			if(pPurposeInfo->strName.Compare(strPurpose) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
		else if((EMPurposeManageNew == pPurposeInfo->emManage) || (EMPurposeManageModify == pPurposeInfo->emManage))
		{
			if(pPurposeInfo->strNewName.Compare(strPurpose) == 0)
			{
				bDuplicate = true;
				break;
			}
		}

	}

	if(true == bDuplicate)
	{
		AfxMessageBox(_T("입력한 통행목적 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
		XTP_REPORTRECORDITEM_ARGS itemArgs(&m_reportPurpose, pItemNotify->pRow, pItemNotify->pColumn);
		m_reportPurpose.EditItem(&itemArgs);
		CXTPReportInplaceEdit* pEdit = m_reportPurpose.GetInplaceEdit();
		if(NULL != pEdit)
		{
			pEdit->SetSel(0, -1);

			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);

			pItem->SetCaption(_T(""));
			pItem->SetValue(_T(""));

			m_reportPurpose.RedrawControl();
		}
	}
	else
	{
		pEditInfo->strNewName = strPurpose;
		if(EMPurposeManageNone == pEditInfo->emManage)
		{
			pEditInfo->emManage = EMPurposeManageModify;
		}
	}
}


void KDefinePurposeDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if(IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecords*    pReocrds = m_reportPurpose.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount = pReocrds->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			pRecord = pReocrds->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_reportPurpose.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefinePurposeDlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_PRJSTC_PURPOSE, SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_STATIC, SZ_TOP_LEFT, SZ_TOP_RIGHT);

		SetResize(IDC_REPORT1, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_BUTTON2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK1, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		
		SetResize(IDC_STATIC2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
		SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDefinePurposeDlg::OnBeginPurposeNameEdit(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	TPurposeManageInfo* pInfo = (TPurposeManageInfo*)(pItemNotify->pItem->GetItemData());
	if (NULL == pInfo)
	{
		return;
	}

	if (pInfo->nDetailObjectId < 1000)
	{
		AfxMessageBox(_T("기본 통행목적의 이름은 변경 할 수 없습니다."));
	}
}


void KDefinePurposeDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	TPurposeManageInfo* pInfo = (TPurposeManageInfo*)(pItemNotify->pItem->GetItemData());
	if (NULL == pInfo)
	{
		return;
	}

	if (pInfo->nDetailObjectId < 1000)
	{
		AfxMessageBox(_T("기본 통행목적의 이름은 변경 할 수 없습니다."));
	}
}