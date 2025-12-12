// MATripMatrix.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MATripMatrix.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MultiModalInfo.h"

#include "CustomReportRecordItem.h"
#include "Mode.h"

#define MAMODE_NAME			0
#define MAMODE_CLOLUMN		1	
#define MAMODE_OCCUPANCY    2 
#define MAMODE_PCU			3
#define MAMODE_NETWORKTYPE  4
#define MAMODE_PRELOAD		5


// KMATripMatrix 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMATripMatrix, KResizeDialogEx)

KMATripMatrix::KMATripMatrix(KTarget* a_pTarget,int a_nModelOption,std::list<KMultiModalInfo>& a_lstMAInfo, double& a_dAnalysisTime,CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMATripMatrix::IDD, pParent),
	m_pTarget(a_pTarget),
	m_nModeOption(a_nModelOption),
	m_lstMAInfo(a_lstMAInfo),
	m_dAnalysisTime(a_dAnalysisTime)
{

}

KMATripMatrix::~KMATripMatrix()
{
}

void KMATripMatrix::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndModeReport);
	DDX_Control(pDX, IDC_EDIT_TIME, m_edtAnalysisTime);
}


BEGIN_MESSAGE_MAP(KMATripMatrix, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KMATripMatrix::OnBnClickedOk)
END_MESSAGE_MAP()


// KMATripMatrix 메시지 처리기입니다.


BOOL KMATripMatrix::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	CString strATime = _T("");
	strATime.Format(_T("%0.1f"),m_dAnalysisTime);
	m_edtAnalysisTime.SetWindowText(strATime);
	m_edtAnalysisTime.EnableRealNumber(true);
	m_edtAnalysisTime.EnableMinMax(0.0, 24.0);

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);

	ResizeComponent();
	InitModeReportControl();
	UpdateModeReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KMATripMatrix::InitModeReportControl( void )
{
	
	// Add Columns
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_NAME, _T("Mode"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }
	
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_CLOLUMN, _T("Matrix"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_OCCUPANCY, _T("Occupancy"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균 재차인원"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_PCU, _T("PCE"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_NETWORKTYPE, _T("Network"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입명"));
    }

    CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
    CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pEditOptions->AddConstraint(_T("도로"), 1);
		pEditOptions->AddConstraint(_T("버스"), 2);
		pEditOptions->AddConstraint(_T("도시철도"), 3);
		pEditOptions->AddConstraint(_T("버스+도시철도"), 4);
	}
	else {
		pEditOptions->AddConstraint(_T("Highway"), 1);
		pEditOptions->AddConstraint(_T("Line"), 2);
		pEditOptions->AddConstraint(_T("Rail"), 3);
		pEditOptions->AddConstraint(_T("Line,Rail"), 4);
	}

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(MAMODE_PRELOAD, _T("PreLoad"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("사전 배정"));
    }
}

void KMATripMatrix::UpdateModeReportRecord( void )
{
	std::list<KMultiModalInfo>::iterator iter = m_lstMAInfo.begin();
	std::list<KMultiModalInfo>::iterator iterend = m_lstMAInfo.end();

	for (iter ; iter != iterend ; iter++)
	{
		KMultiModalInfo &oMulitModalInfo = *iter;
		

		CXTPReportRecord* pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			//pItem->SetCaption(_T("Bus"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			//pItem->SetCaption(_T("Subway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			//pItem->SetCaption(_T("Bus+Subway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		else if (oMulitModalInfo.TransitHighwayType() == 0)
		{
			//pItem->SetCaption(_T("Highway"));
			KMode* pMode = oMulitModalInfo.Mode();
			pItem->SetCaption(pMode->ModeName());
		}
		pItem->SetItemData((DWORD_PTR)&oMulitModalInfo);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		KIOColumn* pColumn = oMulitModalInfo.ModeColumn();

		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pColumn->Caption());
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		// Occupancy
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMulitModalInfo.Occupancy(), _T("%f")));

		// PCE
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oMulitModalInfo.PCU(), _T("%f")));

		// NetworkType
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(oMulitModalInfo.NetworkType());
		if (oMulitModalInfo.TransitHighwayType() == 0)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			pItem->SetEditable(FALSE);
		}
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if(oMulitModalInfo.PreLoad() == 1)
		{
			pItem->SetChecked(TRUE);
		}
		if (oMulitModalInfo.TransitHighwayType() == 1)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);

		}
		else if (oMulitModalInfo.TransitHighwayType() == 2)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);
		}
		else if (oMulitModalInfo.TransitHighwayType() == 3)
		{
			pItem->SetChecked(FALSE);
			pItem->SetEditable(FALSE);
		}
	}

	m_wndModeReport.Populate();
	m_wndModeReport.ExpandAll(TRUE);
}


void KMATripMatrix::OnBnClickedOk()
{

	if( false == ApplyInputData())
	{
		AfxMessageBox(_T("모든 도로 통행수단에 사전 배정 옵션이 선택되어 있습니다."));
		return;
	}

	CString strATime;
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT_TIME);
	pButton->GetWindowText(strATime);

	m_dAnalysisTime = _ttof(strATime);

	KResizeDialogEx::OnOK();
}

bool KMATripMatrix::ApplyInputData( void )
{
	int nPreLoad(0);
	int nHighway(0);

	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(MAMODE_NAME);
		KMultiModalInfo* pInfo = (KMultiModalInfo*)(pItem->GetItemData());

		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(MAMODE_OCCUPANCY));
		pInfo->Occupancy(pNumber->GetValue());

		pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(MAMODE_PCU));
		pInfo->PCU(pNumber->GetValue());

		pItem = pRecord->GetItem(MAMODE_NETWORKTYPE);
		pInfo->NetworkType(pItem->GetItemData());

		pItem = pRecord->GetItem(MAMODE_PRELOAD);
		pInfo->PreLoad((pItem->IsChecked() == TRUE ? true : false));

		if (TRUE == pItem->IsChecked())
		{
			nPreLoad++;
		}
		
		if (0 == pInfo->TransitHighwayType())
		{
			nHighway++;
		}
	}

	if (0 != nPreLoad && nPreLoad == nHighway)
	{
		return false;
	}

	return true;
}

void KMATripMatrix::ResizeComponent()
{
	SetResize(IDC_STATIC1,		SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR2,	SZ_TOP_LEFT,  SZ_TOP_RIGHT);	
	SetResize(IDC_SEPRATOR1,	SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT1,		SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,				SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}
