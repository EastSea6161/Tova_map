// DevZoneSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevZoneSelectDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "BulkDBaseDemarcation.h"
#include "KExRecordItem.h"

// KDevZoneSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevZoneSelectDlg, KDialogEx)

KDevZoneSelectDlg::KDevZoneSelectDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevZoneSelectDlg::IDD, pParent)
{

}

KDevZoneSelectDlg::~KDevZoneSelectDlg()
{
}

void KDevZoneSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndZoneReportCtrl);
}


BEGIN_MESSAGE_MAP(KDevZoneSelectDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KDevZoneSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KDevZoneSelectDlg 메시지 처리기입니다.


void KDevZoneSelectDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnOK();
}


BOOL KDevZoneSelectDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndZoneReportCtrl, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AllowColumnsort(m_wndZoneReportCtrl);

	InitReportHeader();
	UpdateReportData();

	m_wndZoneReportCtrl.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDevZoneSelectDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Zone ID"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndZoneReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Name"),    40));
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KDevZoneSelectDlg::UpdateReportData()
{
	std::vector<KZoneInfo> vecZoneInfo;
	if (m_pTarget)
	{
		KBulkDBaseDemarcation::SelectZone(m_pTarget, vecZoneInfo);
		CXTPReportRecord*     pRecord       = NULL;
		CXTPReportRecordItem* pRecordItem   = NULL;

		std::set<Integer>::iterator iter = m_setInitialSelectedZone.begin();
		std::set<Integer>::iterator end  = m_setInitialSelectedZone.end();

		for (size_t i=0; i<vecZoneInfo.size(); i++)
		{
			pRecord = m_wndZoneReportCtrl.AddRecord(new CXTPReportRecord());
			KZoneInfo oZoneInfo = vecZoneInfo[i];

			pRecordItem = pRecord->AddItem(new KExRecordItemCheck());
			pRecordItem->SetItemData((DWORD_PTR)oZoneInfo.TZoneID);
			pRecordItem->SetAlignment(xtpColumnIconCenter);

			iter = m_setInitialSelectedZone.find(oZoneInfo.TZoneID);
			if (iter != end)
			{
				pRecordItem->SetChecked(TRUE);
			}

			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)oZoneInfo.TZoneID, _T("%.0f")));
			pRecordItem->SetAlignment(DT_CENTER);

			pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(oZoneInfo.TZoneName));
		}
	}
}


void KDevZoneSelectDlg::SetInitialSelectedZone( std::vector<Integer> a_vecInitialSelectedZone )
{
	m_setInitialSelectedZone.clear();

	size_t nxCount = a_vecInitialSelectedZone.size();
	for (size_t i=0; i<nxCount; i++)
	{
		m_setInitialSelectedZone.insert(a_vecInitialSelectedZone[i]);
	}
}


std::vector<Integer> KDevZoneSelectDlg::GetSelectedZone()
{
	std::vector<Integer> vecSelectedZone;

	CXTPReportRecords*    pRecords      = NULL;
	CXTPReportRecord*     pRecord       = NULL;
	CXTPReportRecordItem* pItemCheck    = NULL;
	CXTPReportRecordItem* pItemZoneID   = NULL;

	pRecords         = m_wndZoneReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for ( int i=0; i<nRecordCount; i++ )
	{
		pRecord     = pRecords->GetAt(i);
		pItemCheck  = pRecord->GetItem(0);

		if ( pItemCheck->IsChecked() )
		{            
			vecSelectedZone.push_back((Integer)pItemCheck->GetItemData());
		}
	}

	return vecSelectedZone;
}