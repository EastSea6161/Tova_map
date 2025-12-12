// InterDefineCostPrivateDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterDefineCostPrivatePage.h"
#include "afxdialogex.h"


// KInterDefineCostPrivateDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterDefineCostPrivatePage, CXTPPropertyPage)

KInterDefineCostPrivatePage::KInterDefineCostPrivatePage(std::vector<KInterDefineCostPrivateSector*> a_vecPrivateRecord)
	:	CXTPPropertyPage(KInterDefineCostPrivatePage::IDD),
		m_vecPrivateRecord(a_vecPrivateRecord)
{
}

KInterDefineCostPrivatePage::~KInterDefineCostPrivatePage()
{
}

void KInterDefineCostPrivatePage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_PRIVATE, m_wndPrivateReport);
	DDX_Text(pDX, IDC_EDIT2, m_nParkFee);
}


BEGIN_MESSAGE_MAP(KInterDefineCostPrivatePage, CXTPPropertyPage)
END_MESSAGE_MAP()



// KInterDefineCostPrivatePage 메시지 처리기입니다.


void KInterDefineCostPrivatePage::ParkFee( int a_nParkFee )
{
	m_nParkFee = a_nParkFee;
}

int KInterDefineCostPrivatePage::ParkFee( void )
{
	return m_nParkFee;
}


BOOL KInterDefineCostPrivatePage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	InitPrivateReportContrl();

	UpdatePrivateReportRecord();
	

	return TRUE;
}


void KInterDefineCostPrivatePage::InitPrivateReportContrl()
{
	/*Setting Report header*/
	m_wndPrivateReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndPrivateReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndPrivateReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndPrivateReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	/*Setting Report tree*/
	m_wndPrivateReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndPrivateReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	/*Header*/
	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = m_wndPrivateReport.AddColumn(new CXTPReportColumn(0, _T("AUTO"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndPrivateReport.AddColumn(new CXTPReportColumn(1, _T("가격"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = m_wndPrivateReport.AddColumn(new CXTPReportColumn(2, _T("구성비율"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = m_wndPrivateReport.AddColumn(new CXTPReportColumn(3, _T("평균연비"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);
	
	/*Setting Report Contorl*/
	m_wndPrivateReport.AllowEdit(TRUE);
	m_wndPrivateReport.EditOnClick(TRUE);
	m_wndPrivateReport.SetMultipleSelection(FALSE);
}


void KInterDefineCostPrivatePage::UpdatePrivateReportRecord()
{
	/*update record*/
	//std::vector<KInterDefineCostPrivateSector*> m_tempPrivateData;
	int nCountPrivateReport = m_vecPrivateRecord.size();
	for (int i = 0; i < nCountPrivateReport; i++)
	{
		KInterDefineCostPrivateSector* pPrivateSector = m_vecPrivateRecord[i];

		CXTPReportRecord* pReportRecord = m_wndPrivateReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetCaption(pPrivateSector->GetName());
		pRecordItem->SetEditable(FALSE);
		pRecordItem->SetItemData((DWORD_PTR)pPrivateSector);
		
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPrivateSector->GetPrice(), _T("%.2f")));
		
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPrivateSector->GetRatio(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pPrivateSector->GetFuelEfficiency(), _T("%.2f")));
	}
	
	/*Populate Report Contorl*/
	m_wndPrivateReport.Populate();
	m_wndPrivateReport.ExpandAll(TRUE);
}

void KInterDefineCostPrivatePage::GetRecords(void)
{
	CXTPReportRecord*			pRecord;
	CXTPReportRecordItem*		pRecordItem;
	CXTPReportRecordItemNumber*	pRecordItemNumber;

	CXTPReportRecords* pRecords = m_wndPrivateReport.GetRecords();
	int nCountRecord = pRecords->GetCount();

	for (int i = 0; i < nCountRecord; ++i)
	{
		pRecord = pRecords->GetAt(i);
		pRecordItem = pRecord->GetItem(0);
		
		KInterDefineCostPrivateSector* pInterDefineCostPrivateSector = (KInterDefineCostPrivateSector*)pRecordItem->GetItemData();

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		double dblPrice = pRecordItemNumber->GetValue();

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		double dblRatio = pRecordItemNumber->GetValue();

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		double dblFuelEfficiency = pRecordItemNumber->GetValue();

		pInterDefineCostPrivateSector->SetPrice(dblPrice);
		pInterDefineCostPrivateSector->SetRatio(dblRatio);
		pInterDefineCostPrivateSector->SetFuelEfficiency(dblFuelEfficiency);

	}
}
