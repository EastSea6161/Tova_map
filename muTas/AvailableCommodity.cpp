// AvailableCommodity.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AvailableCommodity.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "PurposeInfoCollection.h"

#include "PurposeManager.h"
#include "PAPurpose.h"

#define PathFind_PURPOSE    0
#define PathFind_SELECTION	1


// KAvailableCommodity 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAvailableCommodity, KDialogEx)

KAvailableCommodity::KAvailableCommodity(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAvailableCommodity::IDD, pParent)
{

}

KAvailableCommodity::~KAvailableCommodity()
{
}

void KAvailableCommodity::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KAvailableCommodity, KDialogEx)
	ON_BN_CLICKED(IDOK, &KAvailableCommodity::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL KAvailableCommodity::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initReportControl();
	updatePurposeRecord();

	return TRUE;
}


void KAvailableCommodity::SetTarget(KTarget* a_pTarget)
{
	m_pTarget = a_pTarget;
}

void KAvailableCommodity::initReportControl( void )
{
	// Report Header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(PathFind_PURPOSE, _T("Commdity"), 150));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(PathFind_SELECTION, _T("Selection"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	// Report control 설정
	m_wndReport.AllowEdit(FALSE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
}

void KAvailableCommodity::updatePurposeRecord()
{
	m_lstAvailiablePurpose.clear();

	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter = purposeInfos.begin();
	std::list<KPurpose*>::iterator end  = purposeInfos.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;
		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(pPurpose->PurposeName());
		pItem->SetItemData((DWORD_PTR)pPurpose);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		STPathAvailableCommdityInfo oPathCommdity;
		oPathCommdity.nCommdityCode = pPurpose->PurposeID();
		oPathCommdity.strCommdityName = pPurpose->PurposeName();

		m_lstAvailiablePurpose.push_back(oPathCommdity);
		++iter;
	}
	m_wndReport.Populate();
}



// KAvailableCommodity 메시지 처리기입니다.


void KAvailableCommodity::OnBnClickedOk()
{
	KDialogEx::OnOK();
}

void KAvailableCommodity::AvailableCommdityList(std::list<STPathAvailableCommdityInfo>& a_AvailableCommodityInfo)
{
	
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportColumn* pColumn   = m_wndReport.GetColumns()->GetAt(0);
		CXTPReportRecordItem* pItem = pRecord->GetItem(1);
		
		bool bUseCommdity = false;
		if(pItem->IsChecked() == TRUE)
		{
			bUseCommdity = true;
		}
		pItem = pRecord->GetItem(PathFind_PURPOSE);
		CString strCommdityName = pItem->GetCaption(pColumn);

		std::list<STPathAvailableCommdityInfo>::iterator iter = m_lstAvailiablePurpose.begin();
		std::list<STPathAvailableCommdityInfo>::iterator end  = m_lstAvailiablePurpose.end();
		for (iter; iter != end; iter++)
		{
			STPathAvailableCommdityInfo oSTPathInfo;
			oSTPathInfo = *iter;
			if (oSTPathInfo.strCommdityName == strCommdityName)
			{
				oSTPathInfo.bSelected = bUseCommdity;
				a_AvailableCommodityInfo.push_back(oSTPathInfo);
			}
			
		}
		
	}
}
