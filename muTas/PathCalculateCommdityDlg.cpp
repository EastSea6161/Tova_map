// PathCalculateCommdityDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathCalculateCommdityDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "PurposeInfoCollection.h"
#include "IncludeIOTable.h"
#include "PurposeManager.h"
#include "PAPurpose.h"


#define PathFind_PURPOSE    0
#define PathFind_SELECTION	1


// KPathCalculateCommdityDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPathCalculateCommdityDlg, CDialogEx)

KPathCalculateCommdityDlg::KPathCalculateCommdityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KPathCalculateCommdityDlg::IDD, pParent)
{

}

KPathCalculateCommdityDlg::~KPathCalculateCommdityDlg()
{
}

void KPathCalculateCommdityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KPathCalculateCommdityDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KPathCalculateCommdityDlg::OnBnClickedOk)
END_MESSAGE_MAP()



BOOL KPathCalculateCommdityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	initReportControl();
	updatePurposeRecord();

	return TRUE;
}

void KPathCalculateCommdityDlg::SetInfo(KTarget* a_pTarget, std::map<int, bool> a_mapSelectCommdity)
{
	m_pTarget = a_pTarget;
	m_mapSelectCommdity = a_mapSelectCommdity;
}


void KPathCalculateCommdityDlg::initReportControl( void )
{
	// Report Header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	// Report tree 설정
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

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

void KPathCalculateCommdityDlg::updatePurposeRecord()
{
	KPurposeInfoCollection purposeInfos(m_pTarget);
	std::list<KPurpose*>::iterator iter = purposeInfos.begin();
	std::list<KPurpose*>::iterator end  = purposeInfos.end();

	while(iter != end)
	{
		KPurpose* pPurpose = *iter;
		CString strCommdityname = pPurpose->PurposeName();
		int     nComdityID      = pPurpose->PurposeID();

		CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->SetCaption(strCommdityname);
		pItem->SetItemData((DWORD_PTR)pPurpose);
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		std::map<int, bool>::iterator iterFind = m_mapSelectCommdity.find(nComdityID);
		std::map<int, bool>::iterator endFind  = m_mapSelectCommdity.end();
		if (iterFind != endFind)
		{
			if (iterFind->second)
			{
				pItem->SetChecked(TRUE);
			}
			else
			{
				pItem->SetChecked(FALSE);
			}
		}
		++iter;
	}
	m_wndReport.Populate();
}

void KPathCalculateCommdityDlg::OnBnClickedOk()
{
	 CDialogEx::OnOK();
}

void KPathCalculateCommdityDlg::SelectCommdityList( std::map<int, bool>& a_mapSelectCommdity, CString& a_strEditBox )
{

	int     nSeq       = 0;
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		bool bCheck = true;
		CXTPReportRecord* pRecord   = pRecords->GetAt(i);
		CXTPReportColumn* pColumn   = m_wndReport.GetColumns()->GetAt(PathFind_PURPOSE);
		CXTPReportRecordItem* pItem = pRecord->GetItem(PathFind_SELECTION);
		if (pItem->IsChecked() == FALSE)
		{
			bCheck = false;
		}
		pItem = pRecord->GetItem(PathFind_PURPOSE);
		CString strCommdityName = pItem->GetCaption(pColumn);
		
		if ( true == bCheck )
		{
			if (nSeq == 0)
			{
				a_strEditBox += strCommdityName;
				nSeq++;
			} 
			else
			{
				a_strEditBox += _T(", ");
				a_strEditBox += strCommdityName;
			}
		}
		KPurposeInfoCollection purposeInfos(m_pTarget);
		std::list<KPurpose*>::iterator iter = purposeInfos.begin();
		std::list<KPurpose*>::iterator end  = purposeInfos.end();
		while(iter != end)
		{
			KPurpose* pPurPose = *iter;
			if (pPurPose->PurposeName() == strCommdityName)
			{
				int nPurposeID = pPurPose->PurposeID();

				std::map<int, bool>::iterator iterComm = a_mapSelectCommdity.find(nPurposeID);
				if(iterComm != a_mapSelectCommdity.end())
				{
					iterComm->second = bCheck;
				}
				else
				{
					a_mapSelectCommdity.insert(std::make_pair(nPurposeID, bCheck));
				}
			}
			++iter;
		}
	}
}
