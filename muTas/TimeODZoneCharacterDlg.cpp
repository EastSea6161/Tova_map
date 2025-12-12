// TimeODZoneCharacterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODZoneCharacterDlg.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//^^#include "SEDData.h"
//^#include "DBaseConnector.h"
#include "DefineNames.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"

enum 
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_SED,
	_2_COLUMN_NUM
};

// KTimeODZoneCharacterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODZoneCharacterDlg, CXTPResizeDialog)

KTimeODZoneCharacterDlg::KTimeODZoneCharacterDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KTimeODZoneCharacterDlg::IDD, pParent)
{

}

KTimeODZoneCharacterDlg::~KTimeODZoneCharacterDlg()
{
}

void KTimeODZoneCharacterDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_ReportCtrl);
}


BEGIN_MESSAGE_MAP(KTimeODZoneCharacterDlg, CXTPResizeDialog)
	ON_COMMAND(ID_SELECT_CANCEL, &KTimeODZoneCharacterDlg::OnSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KTimeODZoneCharacterDlg::OnSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT,  OnReportColumnRButtonClick)

	ON_BN_CLICKED(IDOK, &KTimeODZoneCharacterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODZoneCharacterDlg 메시지 처리기입니다.

BOOL KTimeODZoneCharacterDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultC(m_ReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_ReportCtrl);
	InitReportHeader();
	InitReportData();
	m_ReportCtrl.Populate();

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTimeODZoneCharacterDlg::SetResizeComponent()
{
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KTimeODZoneCharacterDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	CXTPReportRecordItemEditOptions* pSedEditOptions  = NULL;
	CXTPReportRecordItemEditOptions* pNumEditOptions  = NULL;

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,   _T("선택"),     25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SED,    _T("Sed Field"), 50));
	pSedEditOptions = pColumn->GetEditOptions();
	pSedEditOptions->AddComboButton(TRUE);
	pSedEditOptions->m_bAllowEdit = TRUE;
	pSedEditOptions->m_bExpandOnSelect = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("사회경제지표 데이터 컬럼"));
    }

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NUM,    _T("그룹 개수"),  25));
	pNumEditOptions = pColumn->GetEditOptions();
	pNumEditOptions->AddComboButton(TRUE);
	pNumEditOptions->m_bAllowEdit = TRUE;
	pNumEditOptions->m_bExpandOnSelect = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

	InputSedGridComboData(pSedEditOptions);
	InputNumGridComboData(pNumEditOptions);
}


void KTimeODZoneCharacterDlg::InitReportData()
{
	CXTPReportColumn*                pSedColumn         = m_ReportCtrl.GetColumns()->GetAt(_1_COLUMN_SED);
	CXTPReportRecordItemEditOptions* pSedEditOptions    = pSedColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pSedConstraints    = pSedEditOptions->GetConstraints();

	CXTPReportColumn*                pNumColumn         = m_ReportCtrl.GetColumns()->GetAt(_2_COLUMN_NUM);
	CXTPReportRecordItemEditOptions* pNumEditOptions    = pNumColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pNumConstraints    = pNumEditOptions->GetConstraints();

	DWORD_PTR dwDefault  = NULL;
	CString   strDefault = _T("");

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	size_t nInfoCount = m_vecZoneCharacterInfo.size();
	BOOL bSelect      = FALSE;
	for(size_t i= 0; i< nInfoCount; i++)
	{
		TZoneCharacterInfo &oTZonCharacter =  m_vecZoneCharacterInfo[i];

		pRecord =  m_ReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new KExRecordItemCheck());

		bSelect = oTZonCharacter.bSelected;
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bSelect);
		pItem->SetItemData((DWORD_PTR) &oTZonCharacter);
		pItem->SetAlignment(xtpColumnIconCenter);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		if( true == FindSedColumn(oTZonCharacter.pSedColumn, pSedConstraints) )
		{
			pItem->SetItemData((DWORD_PTR)oTZonCharacter.pSedColumn);
		}
		pItem->SetAlignment(DT_LEFT);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		if( true == FindClassNum(oTZonCharacter.nClassNum, pNumConstraints) )
		{
			pItem->SetItemData((DWORD_PTR)oTZonCharacter.nClassNum);
		}

		pItem->SetAlignment(DT_CENTER);
	}

	m_ReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
}


bool KTimeODZoneCharacterDlg::FindSedColumn(KIOColumn* a_pColumn, CXTPReportRecordItemConstraints* a_pConstraints)
{
	for(int i= 0; i< a_pConstraints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstraint = a_pConstraints->GetAt(i);
		KIOColumn* pColumn = (KIOColumn*)pConstraint->m_dwData;

		if(a_pColumn == pColumn)
		{
			return true;
		}
	}

	return false;
}


bool KTimeODZoneCharacterDlg::FindClassNum(int a_nClassNum, CXTPReportRecordItemConstraints* a_pConstraints)
{
	for(int i= 0; i< a_pConstraints->GetCount(); i++)
	{
		CXTPReportRecordItemConstraint* pConstraint = a_pConstraints->GetAt(i);
		int nNum = pConstraint->m_dwData;

		if(a_nClassNum == nNum)
		{
			return true;
		}
	}

	return false;
}


void KTimeODZoneCharacterDlg::OnSelectCancel()
{
	CheckboxControl(FALSE);
}


void KTimeODZoneCharacterDlg::OnSelectAll()
{
	CheckboxControl(TRUE);
}


void KTimeODZoneCharacterDlg::CheckboxControl(BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_ReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		pItemCheck->SetChecked(a_bCheck);
	}

	m_ReportCtrl.RedrawControl();
}


void KTimeODZoneCharacterDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}

void KTimeODZoneCharacterDlg::InputSedGridComboData( CXTPReportRecordItemEditOptions* a_pSedEditOptions )
{
	KIOTables* pIOTables    = m_pTarget->Tables();
	KIOTable*        pSEDVectorTable  = pIOTables->FindTable(_T("sed_vector")); 

	if( NULL == pSEDVectorTable )
	{
		return;
	}

	const KIOColumns* pSedVectorColumns = pSEDVectorTable->Columns();

	KIOColumn*       pColumn    = NULL;
	int              nTypeCount =  0;

	for( int i= 0; i< pSedVectorColumns->ColumnCount(); i++ )
	{
		pColumn = pSedVectorColumns->GetColumn(i);

		if( KEMIODataTypeDouble != pColumn->DataType() )
		{
			continue;
		}

		CXTPReportRecordItemConstraint* pConstraint = a_pSedEditOptions->AddConstraint(    pColumn->Caption(), (DWORD_PTR)pColumn ); 
		nTypeCount++;
	}

	if( nTypeCount < 1 )
	{
		CXTPReportRecordItemConstraint* pConstraint = a_pSedEditOptions->AddConstraint(    _T(""), (DWORD_PTR)pColumn ); 
	}
}


void KTimeODZoneCharacterDlg::InputNumGridComboData( CXTPReportRecordItemEditOptions* a_pNumEditOptions )
{
	KIOColumn*       pColumn    = NULL;
	int              nTypeCount =  0;

	for( int i= KEMClassStartNum; i<= KEMClassEndNum; i++ )
	{
		int nNum = i;
		CString strNum = _T("");
		strNum.Format(_T("%d"), nNum);

		CXTPReportRecordItemConstraint* pConstraint = a_pNumEditOptions->AddConstraint(    (LPCTSTR)strNum, (DWORD_PTR)nNum ); 
		nTypeCount++;
	}

	if( nTypeCount < 1 )
	{
		CXTPReportRecordItemConstraint* pConstraint = a_pNumEditOptions->AddConstraint(    _T("")); 
	}
}


void KTimeODZoneCharacterDlg::OnBnClickedOk()
{
	CXTPReportRecords*    pRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nSelectedCount = 0;
	std::set<KIOColumn*> setSedColumn;

	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_0_COLUMN_CHECK);

		TZoneCharacterInfo*  pTZoneCharacterInfo = (TZoneCharacterInfo* )pItem->GetItemData();
		pTZoneCharacterInfo->bSelected           = pItem->IsChecked();

		if(TRUE == pItem->IsChecked())
		{
			nSelectedCount++;
		}

		pItem = pRecord->GetItem(_1_COLUMN_SED);
		KIOColumn* pSEDColumn = (KIOColumn*)pItem->GetItemData();

		pItem = pRecord->GetItem(_2_COLUMN_NUM);
		int        nClassNum  = pItem->GetItemData();

		pTZoneCharacterInfo->pSedColumn      = pSEDColumn;
		pTZoneCharacterInfo->nClassNum       = nClassNum;

		if( FALSE == pTZoneCharacterInfo->bSelected )
		{
			continue;
		}

		// 중복 선택 검사
		if( setSedColumn.find(pSEDColumn) != setSedColumn.end() )
		{
			AfxMessageBox(_T("중복하여 선택한 사회경제지표 컬럼이 존재합니다."));
			return;
		}
		
		setSedColumn.insert(pSEDColumn);
	}

	if( nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 정보가 없습니다."));
		return;
	}

	CXTPResizeDialog::OnOK();
}
