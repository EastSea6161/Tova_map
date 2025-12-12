// TimeODInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODInputDlg.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//^#include "DBaseConnector.h"
#include "DefineNames.h"
#include "CustomReportRecordItem.h"

#include "TimeODSelectSurveyCodeDlg.h"

enum 
{
	_0_COLUMN_CHECKOD = 0,
	_1_COLUMN_FIELDNAME,
	_2_COLUMN_TIME
};

// KTimeODInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODInputDlg, CXTPResizeDialog)

KTimeODInputDlg::KTimeODInputDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KTimeODInputDlg::IDD, pParent)
{

}

KTimeODInputDlg::~KTimeODInputDlg()
{
}

void KTimeODInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_ReportCtrl);
}


BEGIN_MESSAGE_MAP(KTimeODInputDlg, CXTPResizeDialog)
	ON_COMMAND(ID_SELECT_CANCEL, &KTimeODInputDlg::OnSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KTimeODInputDlg::OnSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT,  OnReportColumnRButtonClick)

	ON_BN_CLICKED(IDOK, &KTimeODInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODInputDlg 메시지 처리기입니다.

BOOL KTimeODInputDlg::OnInitDialog()
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


void KTimeODInputDlg::SetResizeComponent()
{
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KTimeODInputDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	CXTPReportRecordItemEditOptions* pFieldNameEditOptions  = NULL;
	CXTPReportRecordItemEditOptions* pParaODEditOptions     = NULL;

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECKOD,      _T("Select OD"),  30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FIELDNAME,    _T("Field Name"), 40));
	pFieldNameEditOptions = pColumn->GetEditOptions();
	pFieldNameEditOptions->AddComboButton(TRUE);
	pFieldNameEditOptions->m_bAllowEdit = TRUE;
	pFieldNameEditOptions->m_bExpandOnSelect  = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_TIME,         _T("Time"),       40));
	pParaODEditOptions = pColumn->GetEditOptions();
	pParaODEditOptions->AddComboButton(TRUE);
	pParaODEditOptions->m_bAllowEdit = TRUE;
	pParaODEditOptions->m_bExpandOnSelect  = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행시간 속성 컬럼"));
    }
	InputNameGridComboData(pFieldNameEditOptions);
	InputTimeGridComboData(pParaODEditOptions);
}


void KTimeODInputDlg::InitReportData()
{
	CXTPReportColumn*                pFieldNameColumn      = m_ReportCtrl.GetColumns()->GetAt(_1_COLUMN_FIELDNAME);
	CXTPReportRecordItemEditOptions* pFieldNameEditOptions = pFieldNameColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pFieldNameConstraints = pFieldNameEditOptions->GetConstraints();

	CXTPReportColumn*                pParaODColumn         = m_ReportCtrl.GetColumns()->GetAt(_2_COLUMN_TIME);
	CXTPReportRecordItemEditOptions* pParaODEditOptions    = pParaODColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pParaODConstraints    = pParaODEditOptions->GetConstraints();

	DWORD_PTR dwDefault  = NULL;
	CString   strDefault = _T("");

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	size_t nModeCount = m_vecInputModeInfo.size();
	BOOL bSelect      = FALSE;
	for(size_t i= 0; i< nModeCount; i++)
	{
		TInputModeInfo &oTInputMode =  m_vecInputModeInfo[i];

		pRecord =  m_ReportCtrl.AddRecord(new CXTPReportRecord());
		pItem   =  pRecord->AddItem(new CXTPReportRecordItemText( oTInputMode.oTMode.strModeName ));
		
		bSelect = oTInputMode.bSelected;
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bSelect);
		pItem->SetItemData((DWORD_PTR) &oTInputMode);
		pItem->SetAlignment(DT_LEFT);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		if( true == FindODColumn(oTInputMode.pModeODColumn, pFieldNameConstraints) )
		{
			pItem->SetItemData((DWORD_PTR)oTInputMode.pModeODColumn);
		}
		pItem->SetAlignment(DT_LEFT);

		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		if( true == FindODColumn(oTInputMode.pParaODColumn, pParaODConstraints) )
		{
			pItem->SetItemData((DWORD_PTR)oTInputMode.pParaODColumn);
		}

		pItem->SetAlignment(DT_LEFT);
	}

	m_ReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
}


bool KTimeODInputDlg::FindODColumn(KIOColumn* a_pColumn, CXTPReportRecordItemConstraints* a_pConstraints)
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


void KTimeODInputDlg::OnSelectCancel()
{
	CheckboxControl(FALSE);
}


void KTimeODInputDlg::OnSelectAll()
{
	CheckboxControl(TRUE);
}


void KTimeODInputDlg::CheckboxControl(BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_ReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECKOD);
		pItemCheck->SetChecked(a_bCheck);
	}

	m_ReportCtrl.RedrawControl();
}


void KTimeODInputDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECKOD == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KTimeODInputDlg::InputNameGridComboData( CXTPReportRecordItemEditOptions* a_pFieldNameEditOptions )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_MODE_OD);

	if( NULL == pODTable )
	{
		return;
	}

	const KIOColumns* pModeODColumns = pODTable->Columns();

	KIOColumn*       pColumn    = NULL;
	int              nTypeCount =  0;
	
	for(int i= 0; i< pModeODColumns->ColumnCount(); i++)
	{
		pColumn = pModeODColumns->GetColumn(i);

		if( KEMIODataTypeDouble != pColumn->DataType())
		{
			continue;
		}

		CXTPReportRecordItemConstraint* pConstraint = a_pFieldNameEditOptions->AddConstraint(    pColumn->Caption(), (DWORD_PTR)pColumn ); 
		nTypeCount++;
	}

	if( nTypeCount < 1 )
	{
		// grod combo 에 데이터가 하나라도 들어가 있지 않으면 오류가 발생 하여, null 데이터를 넣어 준다. 
		CXTPReportRecordItemConstraint* pConstraint = a_pFieldNameEditOptions->AddConstraint(    _T(""), (DWORD_PTR)pColumn ); 
	}
}


void KTimeODInputDlg::InputTimeGridComboData( CXTPReportRecordItemEditOptions* a_pParaODEditOptions )
{
	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pODTable      = pIOTables->FindTable(TABLE_PARAMETER_OD); 

	if( NULL == pODTable )
	{
		return;
	}

	const KIOColumns* pParaODColumns = pODTable->Columns();

	KIOColumn*       pColumn = NULL;
	int              nTypeCount =  0;

	for( int i= 0; i< pParaODColumns->ColumnCount(); i++ )
	{
		pColumn = pParaODColumns->GetColumn(i);

		if( KEMIODataTypeDouble != pColumn->DataType() )
		{
			continue;
		}

		CXTPReportRecordItemConstraint* pConstraint = a_pParaODEditOptions->AddConstraint(    pColumn->Caption(), (DWORD_PTR)pColumn ); 
		nTypeCount++;
	}

	if( nTypeCount < 1 )
	{
		CXTPReportRecordItemConstraint* pConstraint = a_pParaODEditOptions->AddConstraint(    _T(""), (DWORD_PTR)pColumn ); 
	}
}


void KTimeODInputDlg::OnBnClickedOk()
{
	CXTPReportRecords*    pRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	
	int nSelectedCount = 0;

	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_0_COLUMN_CHECKOD);

		TInputModeInfo* pTInputModeInfo = (TInputModeInfo* )pItem->GetItemData();
		pTInputModeInfo->bSelected = pItem->IsChecked();

		if(TRUE == pItem->IsChecked())
		{
			nSelectedCount++;
		}

		pItem = pRecord->GetItem(_1_COLUMN_FIELDNAME);
		KIOColumn* pModeODColumn = (KIOColumn*)pItem->GetItemData();

		pItem = pRecord->GetItem(_2_COLUMN_TIME);
		KIOColumn* pParaODColumn = (KIOColumn*)pItem->GetItemData();

		pTInputModeInfo->pModeODColumn       = pModeODColumn;
		pTInputModeInfo->pParaODColumn       = pParaODColumn;
	}

	if( nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 기종점 통행 데이터가 없습니다."));
		return;
	}

	CXTPResizeDialog::OnOK();
}