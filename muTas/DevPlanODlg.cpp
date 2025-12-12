// DevPlanODlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanODlg.h"
#include "afxdialogex.h"


// KDevPlanODlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanODlg, KDialogEx)

KDevPlanODlg::KDevPlanODlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanODlg::IDD, pParent)
{

}

KDevPlanODlg::~KDevPlanODlg()
{
}

void KDevPlanODlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_REPORT1, m_wndPAReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndPurposeReport);
	DDX_Control(pDX, IDC_REPORT4, m_wndModeReport);
}


BEGIN_MESSAGE_MAP(KDevPlanODlg, KDialogEx)
END_MESSAGE_MAP()

// KDevPlanODlg 메시지 처리기입니다.

BOOL KDevPlanODlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitPAReport();
	UpdatePAReport();

	InitPurposeReport();
	UpdatePurposeReport();

	InitModeReport();
	UpdateModeReport();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDevPlanODlg::InitPAReport( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndPAReport.AddColumn(new CXTPReportColumn(0, _T(""), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	m_wndPAReport.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndPAReport.AddColumn(new CXTPReportColumn(1, _T("Name"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndPAReport.AddColumn(new CXTPReportColumn(2, _T("Out Field Name"), 3));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	CXTPReportHeader* pHeader = m_wndPAReport.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}

void KDevPlanODlg::UpdatePAReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타(A)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Production")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무(P)_1")));

	pRecord = m_wndPAReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Attraction")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무(A)_1")));

	m_wndPAReport.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndPAReport.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndPAReport.SetGroupRowsBold(TRUE);    
	m_wndPAReport.Populate();
}

void KDevPlanODlg::InitPurposeReport( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndPurposeReport.AddColumn(new CXTPReportColumn(0, _T(""), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	m_wndPurposeReport.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndPurposeReport.AddColumn(new CXTPReportColumn(1, _T("Name"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndPurposeReport.AddColumn(new CXTPReportColumn(2, _T("Out Field Name"), 3));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	CXTPReportHeader* pHeader = m_wndPurposeReport.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}

void KDevPlanODlg::UpdatePurposeReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("출근_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("등교_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("학원_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("쇼핑_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("귀가_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("기타_1")));

	pRecord = m_wndPurposeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Purpose_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("업무_1")));

	m_wndPurposeReport.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndPurposeReport.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndPurposeReport.SetGroupRowsBold(TRUE);    
	m_wndPurposeReport.Populate();
}

void KDevPlanODlg::InitModeReport( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(0, _T(""), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	m_wndModeReport.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(1, _T("Name"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(2, _T("Out Field Name"), 3));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	CXTPReportHeader* pHeader = m_wndModeReport.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}

void KDevPlanODlg::UpdateModeReport( void )
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Mode_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Auto")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Auto_1")));

	pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Mode_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Bus_1")));

	pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Mode_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Subway")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Subway_1")));

	pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Mode_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Taxi")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Taxi_1")));

	pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Mode_OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Transit")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Transit_1")));

	m_wndModeReport.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndModeReport.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndModeReport.SetGroupRowsBold(TRUE);    
	m_wndModeReport.Populate();
}
