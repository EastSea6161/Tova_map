// LinkSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ObjectIDSelectDlg.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"

// KLinkSelectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KObjectIDSelectDlg, KResizeDialogEx)

	KObjectIDSelectDlg::KObjectIDSelectDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KObjectIDSelectDlg::IDD, pParent), m_windowPosition(0, 0)
{
	m_nxSelectedID     = 0;
}

KObjectIDSelectDlg::~KObjectIDSelectDlg()
{
}

void KObjectIDSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_reportCtrl);
}


BEGIN_MESSAGE_MAP(KObjectIDSelectDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
	ON_BN_CLICKED(IDOK, &KObjectIDSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KLinkSelectDlg 메시지 처리기입니다.

BOOL KObjectIDSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


BOOL KObjectIDSelectDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	KReportCtrlSetting::Default(m_reportCtrl, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AllowColumnsort(m_reportCtrl);

	InitReportHeader();
	InitReportData();
	m_reportCtrl.Populate();

	m_reportCtrl.EnableDragDrop(_T("ReportSample:frmDragDrop"), xtpReportAllowDrag );
	SetWindowPos(NULL, m_windowPosition.x, m_windowPosition.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KObjectIDSelectDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

/*	m_reportCtrl.SelectionEnable(FALSE);*/

	pColumn = m_reportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_reportCtrl.AddColumn(new CXTPReportColumn(1, _T("ID"), 25));
	pColumn->SetHeaderAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("ID"));
	}

	CXTPReportHeader* pHeader = m_reportCtrl.GetReportHeader();
// 	pHeader->AllowColumnRemove(FALSE);
// 	pHeader->AllowColumnReorder(FALSE);
// 	pHeader->AllowColumnSort(TRUE);
}

void KObjectIDSelectDlg::InitReportData()
{
	m_reportCtrl.ResetContent(TRUE);

	size_t  nxCount = m_vecID.size();
	CString strTemp;

	for (size_t i=0; i<nxCount; i++)
	{
		CXTPReportRecord     *pRecord = NULL;
		CXTPReportRecordItem *pItem   = NULL;

		pRecord = m_reportCtrl.AddRecord(new CXTPReportRecord());
		pItem   = pRecord->AddItem(new KExRecordItemCheck());
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)i);

		strTemp.Format(_T("%I64d"), m_vecID[i]);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
		pItem->SetAlignment(DT_CENTER);	
	}
}

void KObjectIDSelectDlg::SetLink( std::vector<Integer>& ar_vecLinkID )
{
	m_vecID.clear();
	m_vecID.reserve(ar_vecLinkID.size());
	m_vecID.assign(ar_vecLinkID.begin(), ar_vecLinkID.end());
}

void KObjectIDSelectDlg::OnReportItemClick( NMHDR* pNotify, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotify;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if( 0 == nIndex )
	{
		ClearCheck();
		pItemNotify->pItem->SetChecked(TRUE);
	}
}

void KObjectIDSelectDlg::ClearCheck()
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_reportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(0);
		pItemCheck->SetChecked(FALSE);
	}

	m_reportCtrl.RedrawControl();
}

void KObjectIDSelectDlg::OnBnClickedOk()
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_reportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	int nCheckedCount = 0;
	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);
		pItemCheck = pRecord->GetItem(0);

		if(pItemCheck->GetCheckedState() == TRUE )
		{
			m_nxSelectedID = _ttoi64(pRecord->GetItem(1)->GetCaption(NULL));
			nCheckedCount ++;
		}
	}

	if (nCheckedCount == 0)
	{
		AfxMessageBox(_T("선택해 주세요."));
		return;
	}

	if (nCheckedCount > 1)
	{
		AfxMessageBox(_T("한개만 선택가능합니다."));
		return;
	}

	KResizeDialogEx::OnOK();
}






