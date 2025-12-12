// LinkAnalysisTableODDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LinkAnalysisTableODDlg.h"
#include "afxdialogex.h"

enum
{
	_0_COLUMN_OZONEID = 0,
	_1_COLUMN_DZONEID,
	_2_COLUMN_VOLUME
};

// KLinkAnalysisTableODDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLinkAnalysisTableODDlg, KDialogEx)

KLinkAnalysisTableODDlg::KLinkAnalysisTableODDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KLinkAnalysisTableODDlg::IDD, pParent)
{

}

KLinkAnalysisTableODDlg::~KLinkAnalysisTableODDlg()
{
}

void KLinkAnalysisTableODDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT3,        m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KLinkAnalysisTableODDlg, KDialogEx)
END_MESSAGE_MAP()

BOOL KLinkAnalysisTableODDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultC(m_wndReportCtrl);
	InitReportHeader();
	InitReportData();
	m_wndReportCtrl.Populate();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KLinkAnalysisTableODDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_OZONEID,   _T("Origin"), 35));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기점"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_DZONEID,   _T("Destination"),  35));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("종점"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_VOLUME,    _T("Volume"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("교통량(PCU)"));
    }

	m_wndReportCtrl.Populate();
}


void KLinkAnalysisTableODDlg::InitReportData()
{
	m_wndReportCtrl.ResetContent(TRUE);
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	int nIdx = 0;

	size_t nxCount = m_vecLinkODVolume.size();

	for (size_t i = 0; i < nxCount; i++)
	{
		TLinkODVolume &oTLinkODVolume = m_vecLinkODVolume[i];

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		CString strOZoneID = _T("");
		strOZoneID.Format(_T("%I64d"), oTLinkODVolume.nxOZoneID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strOZoneID));
		pItem->SetAlignment(xtpColumnTextCenter);

		CString strDZoneID = _T("");
		strDZoneID.Format(_T("%I64d"), oTLinkODVolume.nxDZoneID);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strDZoneID));
		pItem->SetAlignment(xtpColumnTextCenter);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTLinkODVolume.dVolume, _T("%f")));
		pItem->SetAlignment(xtpColumnTextRight);
	}

	m_wndReportCtrl.Populate();
}


void KLinkAnalysisTableODDlg::ClearReportData()
{
	m_wndReportCtrl.ResetContent(TRUE);
	m_wndReportCtrl.Populate();
}