// InterDefineCostPublicDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "afxdialogex.h"

#include "InterDefineCostPublicPage.h"
#include "InterDefineCostPublicSector.h"

// KInterDefineCostPublicDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterDefineCostPublicPage, CXTPPropertyPage)

KInterDefineCostPublicPage::KInterDefineCostPublicPage(std::vector<KInterDefineCostPublicSector*> a_vecPublicRecord)
	:	CXTPPropertyPage(KInterDefineCostPublicPage::IDD),
		m_vecPublicRecord(a_vecPublicRecord)
{
}

KInterDefineCostPublicPage::~KInterDefineCostPublicPage()
{
}

void KInterDefineCostPublicPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nNoOfRoute);
	DDV_MinMaxInt(pDX, m_nNoOfRoute, 2, 10);
	DDX_Control(pDX, IDC_SPIN2, m_spinMaxNoOfRoute);
	DDX_Control(pDX, IDC_REPORT_PUBLIC, m_wndPublicReport);
	DDX_Control(pDX, IDC_SPIN3,         m_spinMaxNoOfZoneConnectors);
	DDX_Text   (pDX, IDC_EDIT11, m_nMaxNoOfZoneConnectors);
	DDV_MinMaxInt(pDX, m_nMaxNoOfZoneConnectors, 1, 10);
	DDX_Control(pDX, IDC_EDIT12, m_edtMaxWalkingTimeToStation);
	DDX_Text   (pDX, IDC_EDIT12, m_dMaxWalkingTimeToStation);

	DDX_Control(pDX, IDC_EDIT2,  m_edtTaxiWaitingTime);
	DDX_Text   (pDX, IDC_EDIT2,  m_dTaxiWaitingTime);

	DDX_Control(pDX, IDC_EDIT3,  m_edtTaxiStoppingTime);
	DDX_Text   (pDX, IDC_EDIT3,  m_dTaxiStoppingTime);

	DDX_Control(pDX, IDC_EDIT13, m_editTimeOfBoarding);
	DDX_Text   (pDX, IDC_EDIT13, m_dTimeOfBoarding);
}


BEGIN_MESSAGE_MAP(KInterDefineCostPublicPage, CXTPPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1,  &KInterDefineCostPublicPage::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT11, &KInterDefineCostPublicPage::OnEnChangeEdit11)
END_MESSAGE_MAP()


void KInterDefineCostPublicPage::MaxNoOfZoneConnectors( int a_nMaxNoOfZoneConnectors )
{
	m_nMaxNoOfZoneConnectors = a_nMaxNoOfZoneConnectors;
}

int KInterDefineCostPublicPage::MaxNoOfZoneConnectors( void )
{
	return m_nMaxNoOfZoneConnectors;
}

void KInterDefineCostPublicPage::MaxWalkingTimeToStation( double a_dMaxWalkingTimeToStation )
{
	m_dMaxWalkingTimeToStation = a_dMaxWalkingTimeToStation;
}

double KInterDefineCostPublicPage::MaxWalkingTimeToStation( void )
{
	return m_dMaxWalkingTimeToStation;
}

void KInterDefineCostPublicPage::TimeOfBoarding( double a_dTimeOfBoarding )
{
	m_dTimeOfBoarding = a_dTimeOfBoarding;
}

double KInterDefineCostPublicPage::TimeOfBoarding( void )
{
	return m_dTimeOfBoarding;
}


void KInterDefineCostPublicPage::TaxiWaitingTime( double a_dTaxiWaitingTime )
{
	m_dTaxiWaitingTime = a_dTaxiWaitingTime;
}

double KInterDefineCostPublicPage::TaxiWaitingTime( void )
{
	return m_dTaxiWaitingTime;
}

void KInterDefineCostPublicPage::TaxiStoppingTime( double a_dTaxiStoppingTime )
{
	m_dTaxiStoppingTime = a_dTaxiStoppingTime;
}

double KInterDefineCostPublicPage::TaxiStoppingTime( void )
{
	return m_dTaxiStoppingTime;
}


// KInterDefineCostPublicDlg 메시지 처리기입니다.

BOOL KInterDefineCostPublicPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	m_spinMaxNoOfRoute.SetRange32(2, 10);
	m_spinMaxNoOfZoneConnectors.SetRange32(1, 10);

	m_edtTaxiWaitingTime.EnableRealNumber(true);
	m_edtTaxiStoppingTime.EnableRealNumber(true);

	m_editTimeOfBoarding.EnableRealNumber(true);
	m_edtMaxWalkingTimeToStation.EnableRealNumber(true);

	InitPublicReportControl();
	UpdatePublicReportRecord();

	return TRUE;
}

void KInterDefineCostPublicPage::InitPublicReportControl()
{
	/*Setting Report header*/
	m_wndPublicReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndPublicReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndPublicReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndPublicReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	/*Setting Report tree*/
	m_wndPublicReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndPublicReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	/*Header*/
	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = m_wndPublicReport.AddColumn(new CXTPReportColumn(0, _T("MODE"), 80));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndPublicReport.AddColumn(new CXTPReportColumn(1, _T("기본요금"), 80));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = m_wndPublicReport.AddColumn(new CXTPReportColumn(2, _T("기본거리"), 80));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = m_wndPublicReport.AddColumn(new CXTPReportColumn(3, _T("추가거리"), 80));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = m_wndPublicReport.AddColumn(new CXTPReportColumn(4, _T("추가요금"), 80));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	/*Setting Report Contorl*/
	m_wndPublicReport.AllowEdit(TRUE);
	m_wndPublicReport.EditOnClick(TRUE);
	m_wndPublicReport.SetMultipleSelection(FALSE);
}

void KInterDefineCostPublicPage::UpdatePublicReportRecord()
{
	/*update record*/
	int nCountPublicReport = m_vecPublicRecord.size();
	for(int i = 0; i < nCountPublicReport; i++)
	{
		KInterDefineCostPublicSector* pPublicSector = m_vecPublicRecord[i];

		CXTPReportRecord* pReportRecord = m_wndPublicReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);

		pRecordItem->SetCaption(pPublicSector->GetModeName());
		pRecordItem->SetEditable(FALSE);
		pRecordItem->SetItemData((DWORD_PTR)pPublicSector);

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPublicSector->GetBaseCharge(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPublicSector->GetBaseDistance(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPublicSector->GetAddDistance(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPublicSector->GetAddCharge(), _T("%.2f")));
	}

	/*Populate Report Contorl*/
	m_wndPublicReport.Populate();
	m_wndPublicReport.ExpandAll(TRUE);
}

void KInterDefineCostPublicPage::GetRecords()
{
	CXTPReportRecord*			pRecord;
	CXTPReportRecordItem*		pRecordItem;
	CXTPReportRecordItemNumber*	pRecordItemNumber;

	CXTPReportRecords* pRecords = m_wndPublicReport.GetRecords();
	int nCountRecord = pRecords->GetCount();

	for (int i = 0; i < nCountRecord; ++i)
	{
		pRecord		= pRecords->GetAt(i);
		pRecordItem	= pRecord->GetItem(0);

		KInterDefineCostPublicSector* pInterDefineCostPublicSector = (KInterDefineCostPublicSector*)pRecordItem->GetItemData();

		pRecordItemNumber		= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		double dblBaseCharge	= pRecordItemNumber->GetValue();

		pRecordItemNumber		= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		double dblBaseDistance	= pRecordItemNumber->GetValue();

		pRecordItemNumber		= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		double dblAddDistance	= pRecordItemNumber->GetValue();

		pRecordItemNumber		= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
		double dblAddCharge		= pRecordItemNumber->GetValue();

		pInterDefineCostPublicSector->SetBaseCharge(dblBaseCharge);
		pInterDefineCostPublicSector->SetBaseDistance(dblBaseDistance);
		pInterDefineCostPublicSector->SetAddCharge(dblAddCharge);
		pInterDefineCostPublicSector->SetAddDistance(dblAddDistance);
	}
}

void KInterDefineCostPublicPage::OnEnChangeEdit1()
{
	int nValue = GetDlgItemInt(IDC_EDIT1);
	if (nValue < 2)
	{
		SetDlgItemText(IDC_EDIT1, _T("2"));
	}

	if (nValue > 10)
	{
		SetDlgItemText(IDC_EDIT1, _T("10"));
	}
}

void KInterDefineCostPublicPage::OnEnChangeEdit11()
{
	int nValue = GetDlgItemInt(IDC_EDIT11);
	if (nValue < 1)
	{
		SetDlgItemText(IDC_EDIT11, _T("1"));
	}

	if (nValue > 10)
	{
		SetDlgItemText(IDC_EDIT11, _T("10"));
	}
}
