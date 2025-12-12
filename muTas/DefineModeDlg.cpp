/**
 * @file DefineModeDlg.cpp
 * @brief KDefineModeDlg 구현
 * @author jyk@qbicware.com
 * @date 2012.01.05
 * @remark
 */


#include "StdAfx.h"
#include "KmzApp.h"
#include "Mode.h"
#include "model_common.h"
#include "DefineModeDlg.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_INDEX  = 1,
	_2_COLUMN_NAME   = 2
};

/******************************************************************************
* KDefineModeDlg                                                           *
******************************************************************************/
IMPLEMENT_DYNAMIC(KDefineModeDlg, KResizeDialogEx)

/**************************************
* Constructors and Destructor         *
**************************************/
KDefineModeDlg::KDefineModeDlg(CWnd* pParent /*=NULL*/)
: KResizeDialogEx(KDefineModeDlg::IDD, pParent)
{
	
	m_strSelectedModes	   = _T("");
	m_strSelectedModeID	   = _T("");
	m_strSelectedModeGroup = _T("");

	m_strMode = _T("Defined Mode");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_strMode = _T("통행수단");
	}
	m_strClickedMode = _T("");
}

KDefineModeDlg::KDefineModeDlg(std::vector<KMode*> &vecModeInfo)
{

}

KDefineModeDlg::~KDefineModeDlg(void)
{
}



/**************************************
* Operations                          *
**************************************/

void KDefineModeDlg::SetMode(std::vector<KMode*> &a_vecModeInfo)
{
	m_pvecModeInfo = &a_vecModeInfo;
	std::vector<KMode*>::iterator itMode, itModeEnd = m_pvecModeInfo->end();
	for(itMode = m_pvecModeInfo->begin(); itMode != itModeEnd; ++itMode)
	{

		KMode* pMode = *itMode;
		TModeManageInfo* pModeInfo = new TModeManageInfo;

		pModeInfo->emManage		  = EMModeManageNone;
		pModeInfo->strName		  = pMode->ModeName();
		pModeInfo->nModeGroupCode = pMode->ModeGroup();
		int nModeId				  = pMode->ModeID();
		pModeInfo->nDetailModeId  = nModeId;
		//pModeInfo->strSysDefineYN = _T("Y");
		pModeInfo->strNewName	  = _T("");

		m_ModeInfoList.push_back(pModeInfo);

		if(m_nMaxModeId < nModeId)
		{
			m_nMaxModeId = nModeId;
		}
	}

	/*m_nMaxModeId += 1;*/
}


/**************************************
* Overrides                           *
**************************************/
BOOL KDefineModeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CBitmap bmp;
	bmp.LoadBitmap( IDB_3STATE_CHECK );
	
	KReportCtrlSetting::Default(m_reportMode, TRUE, FALSE, TRUE);
	KReportCtrlSetting::AllowColumnsort(m_reportMode);

#pragma region old
//  예전 delete 버전인 한개의 행 삭제를 하다보면 KReportCtrlSetting::Default()로 셋팅한 스타일에서는 제대로 작동 되지 않았다.
// 	m_reportMode.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
// 	m_reportMode.GetReportHeader()->AllowColumnRemove(FALSE);
// 	m_reportMode.GetReportHeader()->AllowColumnSort(TRUE);
// 	m_reportMode.GetReportHeader()->AllowColumnReorder(FALSE);
// 	m_reportMode.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
// 	m_reportMode.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
// 
// 	m_reportMode.SetMultipleSelection(FALSE);
// 	m_reportMode.AllowEdit(TRUE);
// 	m_reportMode.EditOnClick(TRUE);
// 	m_reportMode.SelectionEnable(TRUE);
#pragma endregion old
	m_reportMode.Populate();

	initModeList();
	updateModeList();
	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void KDefineModeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	
	DDX_Text(pDX, IDC_PRJSTC_MODE, m_strMode);
	DDX_Control(pDX, IDC_REPORT1, m_reportMode);
}


BEGIN_MESSAGE_MAP(KDefineModeDlg, KResizeDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &KDefineModeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &KDefineModeDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON2, &KDefineModeDlg::OnBnClickedButtonDel)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT1, &KDefineModeDlg::OnModeValueChanged)
	ON_BN_CLICKED(IDC_CHECK1, &KDefineModeDlg::OnBnClickedCheckSelectAll)
END_MESSAGE_MAP()


void KDefineModeDlg::initModeList(void)
{
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_reportMode.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_reportMode.AddColumn(new CXTPReportColumn(_1_COLUMN_INDEX, _T("No."), 20)); 
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("순서"));
    }

	pColumn = m_reportMode.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Mode"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	//pColumn->SetAlignment(DT_LEFT);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }
}


void KDefineModeDlg::updateModeList(void)
{
    m_reportMode.ResetContent();

	std::list<TModeManageInfo*>::iterator itMode, itEnd = m_ModeInfoList.end();
	int nIdx = 1;
	for(itMode = m_ModeInfoList.begin(); itMode != itEnd; ++itMode)
	{
		TModeManageInfo* pInfo = *itMode;
		if(EMModeManageRemove == pInfo->emManage)
		{
			continue;
		}
		
		CXTPReportRecord*     pRecord = m_reportMode.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = nullptr;

		// select
		pItem  = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);

		// No.
		pItem  = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nIdx, _T("%.0f")));

		// Name
		if(EMModeManageNone == pInfo->emManage )
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strName));
		}
		else
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pInfo->strNewName));
		}

		pItem->SetItemData((DWORD_PTR)pInfo);

		nIdx++;
	}

	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);

	m_reportMode.Populate();
}


/**************************************
* Message handlers                    *
**************************************/
int KDefineModeDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (KResizeDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void KDefineModeDlg::OnBnClickedOk()
{
	
	UpdateData(TRUE);

	KEMModeGroup emGroup = KEMModeGroupODBase;	// jyk: 임시로 default를 KEMModeGroupODBase 잡아 놓음.. 추후 변경 바람

	CString strModeName  = _T("");
	CString strModeId	 = _T("");
	CString strModeGroup = _T("");
	int nModeId = -1;

	m_pvecModeInfo->clear();
	m_strSelectedModes.Empty();

	std::list<TModeManageInfo*>::iterator itMode, itEnd = m_ModeInfoList.end();
	for(itMode = m_ModeInfoList.begin(); itMode != itEnd; ++itMode)
	{
		TModeManageInfo* pModeInfo = *itMode;

		if(EMModeManageNone == pModeInfo->emManage)
		{
			strModeName = pModeInfo->strName;
		}
		else if(EMModeManageNew == pModeInfo->emManage)
		{
			strModeName = pModeInfo->strNewName;
		}
		else if(EMModeManageModify == pModeInfo->emManage)
		{
			strModeName = pModeInfo->strNewName;
		}
		else if(EMModeManageRemove == pModeInfo->emManage)
		{
			delete pModeInfo;
			continue;
		}

		if( (strModeName.Trim()).Compare(_T("")) == 0)
		{
			delete pModeInfo;
			continue;
		}

		nModeId = pModeInfo->nDetailModeId;

		KMode* pMode = new KMode(m_emModeGroup, nModeId, strModeName);

		m_pvecModeInfo->push_back(pMode);

		if(false == m_strSelectedModes.IsEmpty())
		{
			m_strSelectedModes += _T(", ");
			m_strSelectedModeID += _T(", ");
		}

		strModeId.Format(_T("%d"), nModeId);
		strModeGroup.Format(_T("%d"), m_emModeGroup);

		m_strSelectedModes += (strModeName);
		m_strSelectedModeID += (strModeId);
		m_strSelectedModeGroup = strModeGroup;
	}
	UpdateData(FALSE);

	KResizeDialogEx::OnOK();
}


void KDefineModeDlg::OnBnClickedButtonAdd()
{
	TModeManageInfo* pNewModeInfo = new TModeManageInfo;
	pNewModeInfo->emManage = EMModeManageNew;
	pNewModeInfo->strNewName = _T("");
	pNewModeInfo->strSysDefineYN = _T("N");
	pNewModeInfo->nDetailModeId = ++m_nMaxModeId;
	m_ModeInfoList.push_back(pNewModeInfo);

	int nTotalRowCount = m_reportMode.GetRows()->GetCount();

	CXTPReportRecord*     pRecord = m_reportMode.AddRecord(new CXTPReportRecord);
	CXTPReportRecordItem* pItem   = NULL;

	pItem = pRecord->AddItem(new CXTPReportRecordItem());
	pItem->HasCheckbox(TRUE);
	pItem->SetAlignment(xtpColumnIconCenter);
	
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber( (double)(nTotalRowCount +1), _T("%.0f") ));

	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
	pItem->SetItemData((DWORD_PTR)pNewModeInfo);

	m_reportMode.Populate();

	nTotalRowCount = m_reportMode.GetRows()->GetCount();
	CXTPReportRow* pRow = m_reportMode.GetRows()->GetAt(nTotalRowCount - 1);
	pRow->SetSelected(TRUE);
	

	//추가된 컬럼에 Focus주기
	XTP_REPORTRECORDITEM_ARGS args(&m_reportMode, pRow, m_reportMode.GetColumns()->GetAt(_2_COLUMN_NAME));
	m_reportMode.EditItem(&args);
}


void KDefineModeDlg::OnBnClickedButtonDel()
{
	try
	{
		CXTPReportRecords*    pRecords     = m_reportMode.GetRecords();
		CXTPReportRecord*     pRecord      = nullptr;
		CXTPReportRecordItem* pItem        = nullptr;
		TModeManageInfo*      pModeInfo    = nullptr;

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
			pModeInfo    = (TModeManageInfo*)pItem->GetItemData();

			if(EMModeManageNew == pModeInfo->emManage)
			{
				pModeInfo->emManage = EMModeManageRemove;
			}
			else
			{
				pModeInfo->emManage = EMModeManageRemove;
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

		updateModeList();

#pragma region old
// 	CXTPReportRow* pModeRow = m_reportMode.GetSelectedRows()->GetAt(0);
// 	if(NULL == pModeRow)
// 	{
// 		return;
// 	}
// 
// 	int nIndex = pModeRow->GetIndex();
// 
// 	CXTPReportRecord* pModeRecord = pModeRow->GetRecord();
// 	CXTPReportRecordItem* pModeItem = pModeRecord->GetItem(_2_COLUMN_NAME);
// 	TModeManageInfo* pModeInfo = (TModeManageInfo*)(pModeItem->GetItemData());
// 
// 	if(EMModeManageNew == pModeInfo->emManage)
// 	{
// 
// 		//delete pModeInfo;
// 		pModeInfo->emManage = EMModeManageRemove;
// 		m_reportMode.GetRecords()->RemoveRecord(pModeRecord);
// 		m_reportMode.Populate();
// 	}
// 	else 
// 	{
// 
// 		pModeInfo->emManage = EMModeManageRemove;
// 		updateModeList();
// 	}
// 
// 	if(nIndex > 1)	// 선택 행을 삭제 한 후 focus는 이전 행으로 셋팅
// 	{
// 
// 		CXTPReportRow* pRow = m_reportMode.GetRows()->GetAt(nIndex -1);
// 		pRow->SetSelected(TRUE);
// 		m_reportMode.SetFocusedRow(pRow);
// 		m_reportMode.SetTopRow(nIndex -1);
// 	}
#pragma endregion old
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefineModeDlg::OnModeValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CString strMode = pItemNotify->pItem->GetCaption(NULL);

	//숫자, 특수문자 필터링
	int nLen = strMode.GetLength();

	for (int i = 0; i < nLen; ++i)
	{
		bool bOK = false;

		if (isdigit(strMode.GetAt(i)) || isalpha(strMode.GetAt(i)) || strMode.GetAt(i) <= 0 || 127 < strMode.GetAt(i))
			bOK = true;

		if (!bOK)
		{
			AfxMessageBox(_T("특수문자를 입력할 수 없습니다."));

			CXTPReportInplaceEdit* pEdit = m_reportMode.GetInplaceEdit();
			if(NULL != pEdit)
			{
				pEdit->SetSel(0, -1);

				CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);

				pItem->SetCaption(_T(""));
				pItem->SetValue(_T(""));

				m_reportMode.RedrawControl();
			}

			return;
		}
	}

	TModeManageInfo* pEditInfo = (TModeManageInfo*)(pItemNotify->pItem->GetItemData());

	bool bDuplicate = false;
	std::list<TModeManageInfo*>::iterator itMode, itEnd = m_ModeInfoList.end();
	for(itMode = m_ModeInfoList.begin(); itMode != itEnd; ++itMode)
	{
		TModeManageInfo* pModeInfo = *itMode;
		if(EMModeManageNone == pModeInfo->emManage)
		{
			if(pModeInfo->strName.Compare(strMode) == 0)
			{
				bDuplicate = true;
				break;
			}
		}
		else if((EMModeManageNew == pModeInfo->emManage) || (EMModeManageModify == pModeInfo->emManage))
		{
			if(pModeInfo->strNewName.Compare(strMode) ==0)
			{
				bDuplicate = true;
				break;
			}
		}
	}

	if(true == bDuplicate)
	{
		AfxMessageBox(_T("입력한 통행수단 명칭이 이미 존재 합니다.\n다시 입력해 주세요."));
		XTP_REPORTRECORDITEM_ARGS itemArgs(&m_reportMode, pItemNotify->pRow, pItemNotify->pColumn);
		m_reportMode.EditItem(&itemArgs);
		CXTPReportInplaceEdit* pEdit = m_reportMode.GetInplaceEdit();
		if(NULL != pEdit)
		{
			pEdit->SetSel(0, -1);

			CXTPReportRecord*         pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItemText* pItem   = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);

			pItem->SetCaption(_T(""));
			pItem->SetValue(_T(""));

			m_reportMode.RedrawControl();
		}
	}
	else 
	{
		pEditInfo->strNewName = strMode;
		if(EMModeManageNone == pEditInfo->emManage)
		{
			pEditInfo->emManage = EMModeManageModify;
		}
	}
}

void KDefineModeDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if(IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecords*    pReocrds = m_reportMode.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount = pReocrds->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			pRecord = pReocrds->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_reportMode.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KDefineModeDlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_PRJSTC_MODE, SZ_TOP_LEFT, SZ_TOP_LEFT);
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
