// TimeODSelectSurveyCodeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODSelectSurveyCodeDlg.h"

enum 
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_SURVEYCODE
};

#define TYPE_PURPOSE_STR _T("목적")
#define TYPE_MODE_STR    _T("수단")

// KTimeODSelectSurveyCodeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTimeODSelectSurveyCodeDlg, CXTPResizeDialog)

KTimeODSelectSurveyCodeDlg::KTimeODSelectSurveyCodeDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KTimeODSelectSurveyCodeDlg::IDD, pParent)
	, m_strTitle(_T(""))
{

}

KTimeODSelectSurveyCodeDlg::~KTimeODSelectSurveyCodeDlg()
{
}

void KTimeODSelectSurveyCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_ReportCtrl);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_strTitle);
}


BEGIN_MESSAGE_MAP(KTimeODSelectSurveyCodeDlg, CXTPResizeDialog)
	ON_COMMAND(ID_SELECT_CANCEL, &KTimeODSelectSurveyCodeDlg::OnSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KTimeODSelectSurveyCodeDlg::OnSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT,  OnReportColumnRButtonClick)

	ON_BN_CLICKED(IDOK, &KTimeODSelectSurveyCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODSelectSurveyCodeDlg 메시지 처리기입니다.


BOOL KTimeODSelectSurveyCodeDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();    

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitTitle();

	QBicReportCtrlSetting::Default(m_ReportCtrl, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::AllowColumnsort(m_ReportCtrl);

	InitReportHeader();
	InitReportData();
	m_ReportCtrl.Populate();

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTimeODSelectSurveyCodeDlg::InitTitle()
{
	if( KEMSurveyDataTypePurpose == m_emSurveyDataType )
	{
		m_strTitle.Format( _T("%s : %s"), TYPE_PURPOSE_STR, m_strName );
	}
	else
	{
		m_strTitle.Format( _T("%s : %s"), TYPE_MODE_STR, m_strName );
	}

	UpdateData(FALSE);
}


void KTimeODSelectSurveyCodeDlg::SetResizeComponent()
{
	SetResize(IDC_STATIC_TITLE,       SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KTimeODSelectSurveyCodeDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("선택"),  30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	pColumn->SetSortable(FALSE);
	
	pColumn = m_ReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_SURVEYCODE, _T("Survey Code"), 70));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("조사자료 매칭 코드"));
    }	
}


void KTimeODSelectSurveyCodeDlg::InitReportData()
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	KTDSurveyCodeSet::iterator iter    = m_setTotalCode.begin();
	KTDSurveyCodeSet::iterator iterEnd = m_setTotalCode.end();
	while( iter != iterEnd )
	{
		int  nCode    = *iter;
		BOOL bSelected = IsSelected(nCode);

		pRecord = m_ReportCtrl.AddRecord(new CXTPReportRecord);

		// check
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bSelected);
		pItem->SetItemData((DWORD_PTR) nCode);

		// survey code
		pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber( (double)nCode, _T("%.0f") ));

		++iter;
	}
}


bool KTimeODSelectSurveyCodeDlg::IsSelected( int &a_nSelectedValue )
{
	KTDSurveyCodeSet::iterator iter = m_setSelectedCode.find(a_nSelectedValue);
	
	if( iter != m_setSelectedCode.end() )
	{
		return true;
	}

	return false;
}


void KTimeODSelectSurveyCodeDlg::OnSelectCancel()
{
	CheckboxControl(FALSE);
}


void KTimeODSelectSurveyCodeDlg::OnSelectAll()
{
	CheckboxControl(TRUE);
}


void KTimeODSelectSurveyCodeDlg::CheckboxControl(BOOL a_bCheck)
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


void KTimeODSelectSurveyCodeDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame       = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KTimeODSelectSurveyCodeDlg::OnBnClickedOk()
{
	CXTPReportRecords*    pRecords = m_ReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nSelectedCount = 0;

	m_setSelectedCode.clear();
	for( int i= 0; i< pRecords->GetCount(); i++ )
	{
		pRecord   = pRecords->GetAt(i);
		pItem     = pRecord->GetItem(_0_COLUMN_CHECK);
		int nCode = pItem->GetItemData();
		
		if(TRUE == pItem->IsChecked())
		{
			m_setSelectedCode.insert(nCode);
			nSelectedCount++;
		}
	}

	if( nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 조사자료 매칭 코드 정보가 없습니다."));
		return;
	}

	CXTPResizeDialog::OnOK();
}
