// LinkDevideSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LinkDevideSelectDlg.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"

// KLinkDevideSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KLinkDevideSelectDlg, KResizeDialogEx)

KLinkDevideSelectDlg::KLinkDevideSelectDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KLinkDevideSelectDlg::IDD, pParent)
{

}

KLinkDevideSelectDlg::~KLinkDevideSelectDlg()
{
}

void KLinkDevideSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReport);
}


BEGIN_MESSAGE_MAP(KLinkDevideSelectDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KLinkDevideSelectDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KLinkDevideSelectDlg 메시지 처리기입니다.
BOOL KLinkDevideSelectDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE);
	InitReportHeader();
	InitReportData();
	m_wndReport.Populate();
		
	SetWindowPos(NULL, m_windowPosition.x, m_windowPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KLinkDevideSelectDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
		
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("ID"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("ID"));
	}

	CXTPReportHeader* pHeader = m_wndReport.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(TRUE);
}

void KLinkDevideSelectDlg::InitReportData()
{
	m_wndReport.ResetContent(TRUE);

	size_t  nxCount = m_vecID.size();
	CString strTemp;

	for (size_t i=0; i<nxCount; i++)
	{
		CXTPReportRecord     *pRecord = NULL;
		CXTPReportRecordItem *pItem   = NULL;

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new KExRecordItemCheck());
		pItem->SetChecked(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)i);
		pItem->SetEditable(TRUE);

		strTemp.Format(_T("%I64d"), m_vecID[i]);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pItem->SetAlignment(DT_CENTER);	
	}
}

void KLinkDevideSelectDlg::OnReportItemClick( NMHDR* pNotify, LRESULT* pResult )
{
	return;


	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotify;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
}


void KLinkDevideSelectDlg::OnBnClickedOk()
{
	m_vecSelectedID.clear();

	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	int nCheckedCount = 0;
	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);
		pItemCheck = pRecord->GetItem(0);

		if(pItemCheck->GetCheckedState() == TRUE )
		{
			m_vecSelectedID.push_back(_ttoi64(pRecord->GetItem(1)->GetCaption(NULL)));
		}
	}

	KResizeDialogEx::OnOK();
}

void KLinkDevideSelectDlg::OnBnClickedCancel()
{
    KResizeDialogEx::OnCancel();
}

void KLinkDevideSelectDlg::SetID( std::vector<Integer>& ar_vecID )
{
	m_vecID.assign(ar_vecID.begin(), ar_vecID.end());
	m_vecSelectedID.clear();
}

std::vector<Integer> KLinkDevideSelectDlg::GetSelectedID()
{
	return m_vecSelectedID;
}


