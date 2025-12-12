// InterChoiceParamPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterChoiceParamPage.h"
#include "afxdialogex.h"

#include "InterChoiceParam.h"
#include "InterChoiceParamBase.h"

// KInterChoiceParamPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterChoiceParamPage, CXTPPropertyPage)

KInterChoiceParamPage::KInterChoiceParamPage(KInterChoiceParamBase* a_pChoiceParam)
	:	CXTPPropertyPage(KInterChoiceParamPage::IDD),
		m_pChoiceParam(a_pChoiceParam)
{

}

KInterChoiceParamPage::~KInterChoiceParamPage()
{
	GetRecords();
}

void KInterChoiceParamPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndParamReport);
}


BEGIN_MESSAGE_MAP(KInterChoiceParamPage, CXTPPropertyPage)
END_MESSAGE_MAP()


// KInterChoiceParamPage 메시지 처리기입니다.

void KInterChoiceParamPage::InitReportCtrl(void)
{
	/*Setting Report header*/
	m_wndParamReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndParamReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndParamReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndParamReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	/*Setting Report tree*/
	m_wndParamReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndParamReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	/*Header*/
	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = m_wndParamReport.AddColumn(new CXTPReportColumn(0, _T("Attribute"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndParamReport.AddColumn(new CXTPReportColumn(1, _T("Auto"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndParamReport.AddColumn(new CXTPReportColumn(2, _T("Taxi"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndParamReport.AddColumn(new CXTPReportColumn(3, _T("Bus"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = m_wndParamReport.AddColumn(new CXTPReportColumn(4, _T("Rail"), 100));
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	/*Setting Report Contorl*/
	m_wndParamReport.AllowEdit(TRUE);
	m_wndParamReport.EditOnClick(TRUE);
	m_wndParamReport.SetMultipleSelection(FALSE);
}

void KInterChoiceParamPage::UpdateReportData(void)
{
	std::vector<KInterChoiceParam*> vecChoiceParam =  m_pChoiceParam->GetInterChoiceParam();

	int nCntChoiceParam = vecChoiceParam.size();
	for (int i = 0; i < nCntChoiceParam; i++)
	{
		KInterChoiceParam* pChoiceParam = vecChoiceParam[i];

		CXTPReportRecord* pReportRecord = m_wndParamReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);

		pRecordItem->SetCaption(pChoiceParam->Attribute());
		pRecordItem->SetEditable(FALSE);
		pRecordItem->SetItemData((DWORD_PTR)pChoiceParam);

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pChoiceParam->Auto(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pChoiceParam->Taxi(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pChoiceParam->Bus(), _T("%.2f")));

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber(pChoiceParam->Rail(), _T("%.2f")));
	}

	/*Populate Report Contorl*/
	m_wndParamReport.Populate();
	m_wndParamReport.ExpandAll(TRUE);
}

void KInterChoiceParamPage::GetRecords()
{
	CXTPReportRecord*			pRecord;
	CXTPReportRecordItem*		pRecordItem;
	CXTPReportRecordItemNumber* pRecordItemNumber;

	CXTPReportRecords* pRecords = m_wndParamReport.GetRecords();
	int nCountRecord = pRecords->GetCount();

	for (int i = 0; i < nCountRecord; ++i)
	{
		pRecord = pRecords->GetAt(i);
		pRecordItem = pRecord->GetItem(0);

		KInterChoiceParam* pChoiceParam = (KInterChoiceParam*)pRecordItem->GetItemData();

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
		pChoiceParam->Auto(pRecordItemNumber->GetValue());

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
		pChoiceParam->Taxi(pRecordItemNumber->GetValue());

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		pChoiceParam->Bus(pRecordItemNumber->GetValue());

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
		pChoiceParam->Rail(pRecordItemNumber->GetValue());

	}
}


BOOL KInterChoiceParamPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	InitReportCtrl();

	UpdateReportData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
