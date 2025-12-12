// ZoneSelectIntergrationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ZoneSelectIntergrationDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "BulkDBaseDemarcation.h"
#include "KExRecordItem.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_ZONEID,
	_2_COLUMN_NAME
};

#define EMPTY_PARENT_TEXT _T("-")

// KZoneSelectIntergrationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KZoneSelectIntergrationDlg, KDialogEx)

KZoneSelectIntergrationDlg::KZoneSelectIntergrationDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KZoneSelectIntergrationDlg::IDD, pParent)
{

}

KZoneSelectIntergrationDlg::~KZoneSelectIntergrationDlg()
{
}

void KZoneSelectIntergrationDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndZoneReportCtrl);
}


BEGIN_MESSAGE_MAP(KZoneSelectIntergrationDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KZoneSelectIntergrationDlg::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT, OnReportCheckItem)
END_MESSAGE_MAP()


// KZoneSelectIntergrationDlg 메시지 처리기입니다.


void KZoneSelectIntergrationDlg::OnBnClickedOk()
{
	m_vecSelectedZone.clear();

	CXTPReportRecords*    pRecords      = NULL;
	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pItemCheck    = NULL;
	CXTPReportRecordItem* pItemZoneID   = NULL;

	pRecords         = m_wndZoneReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for ( int i=0; i<nRecordCount; i++ )
	{
		pRecord     = pRecords->GetAt(i);
		pItemCheck  = pRecord->GetItem(_0_COLUMN_SELECT);
		pItemZoneID = pRecord->GetItem(_1_COLUMN_ZONEID);

		if ( pItemCheck->IsChecked() )
		{            
			Integer nxParentID = (Integer)pItemCheck->GetItemData();
			Integer nxZoneID   = (Integer)pItemZoneID->GetItemData();

			if( nxParentID == m_nxSelParentID )
			{
				m_vecSelectedZone.push_back(nxZoneID);
			}
		}
	}

	if( m_vecSelectedZone.size() < 2 )
	{
		AfxMessageBox(_T("집합화 할 존이 최소 2개 이상 선택되어야 합니다."));
		return;
	}

	KDialogEx::OnOK();
}


BOOL KZoneSelectIntergrationDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CXTPReportColumn* pColumn = NULL;
	m_wndZoneReportCtrl.SelectionEnable(FALSE);
	m_wndZoneReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid); 

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_ZONEID, _T("Zone ID"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 노드 ID"));
    }

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME,   _T("Name"),    40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	CXTPReportHeader* pHeader = m_wndZoneReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(TRUE);
	pHeader->AllowColumnSort(TRUE);

	std::vector<KZoneInfo> vecZoneInfo;
	if (m_pTarget)
	{
		KBulkDBaseDemarcation::SelectZone(m_pTarget, vecZoneInfo);
		CXTPReportRecord*     pRecord       = NULL;
		CXTPReportRecordItem* pRecordItem   = NULL;

		std::map<Integer, Integer>::iterator iter = m_mapInitialSelectedZone.begin();
		std::map<Integer, Integer>::iterator end  = m_mapInitialSelectedZone.end();
		
		for (size_t i=0; i<vecZoneInfo.size(); i++)
		{
			pRecord = m_wndZoneReportCtrl.AddRecord(new CXTPReportRecord());
			KZoneInfo oZoneInfo = vecZoneInfo[i];

			pRecordItem = pRecord->AddItem(new KExRecordItemCheck());
			Integer nxParentID = -1;

			iter = m_mapInitialSelectedZone.find(oZoneInfo.TZoneID);
			if (iter != end)
			{
				pRecordItem->SetChecked(TRUE);

				nxParentID = iter->second;

				if( nxParentID != m_nxSelParentID )
				{
					pRecordItem->SetEditable(FALSE);
				}

				CString strParentID(_T(""));
				strParentID.Format(_T("%I64d"), nxParentID);
				pRecordItem->SetCaption(strParentID);
				pRecordItem->SetAlignment(xtpColumnTextCenter);
			}
			else
			{
				pRecordItem->SetCaption(EMPTY_PARENT_TEXT);
				pRecordItem->SetAlignment(xtpColumnTextCenter);
			}
			pRecordItem->SetItemData((DWORD_PTR)nxParentID);

			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oZoneInfo.TZoneID, _T("%.0f")));
			pRecordItem->SetItemData((DWORD_PTR)oZoneInfo.TZoneID);

			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(oZoneInfo.TZoneName));
		}
	}

	m_wndZoneReportCtrl.Populate();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KZoneSelectIntergrationDlg::SetInitialSelectedZone( std::map<Integer, Integer> a_mapInitialSelectedZone )
{
	m_mapInitialSelectedZone = a_mapInitialSelectedZone;
}


std::vector<Integer> KZoneSelectIntergrationDlg::GetSelectedParentZones()
{
	return m_vecSelectedZone;
}


void KZoneSelectIntergrationDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( FALSE == pItemNotify->pItem->IsEditable() )
	{
		return;
	}

	CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);

	Integer nxParentID = -1;
	if( TRUE == pItemNotify->pItem->IsChecked() )
	{
		nxParentID = m_nxSelParentID;
		CString strParentID(_T(""));
		strParentID.Format(_T("%I64d"), nxParentID);
		pItem->SetCaption(strParentID);
	}
	else
	{
		nxParentID = -1;
		pItem->SetCaption(EMPTY_PARENT_TEXT);
	}

	pItem->SetItemData((DWORD_PTR)nxParentID);
}