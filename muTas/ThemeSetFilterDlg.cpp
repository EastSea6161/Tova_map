// ThemeSetFilterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ThemeSetFilterDlg.h"
#include "afxdialogex.h"

#include "KExRecordItem.h"

#include "Target.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

// KThemeSetFilterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KThemeSetFilterDlg, KDialogEx)

KThemeSetFilterDlg::KThemeSetFilterDlg( KTarget* a_pTarget,
	vector<TFilterInfo*> a_vecFilterInfo, int a_nUserGroupKey, int a_nSysGroupKey, CWnd* pParent /*=NULL*/)
	: KDialogEx(KThemeSetFilterDlg::IDD, pParent)
	, m_nUserGroupKey(a_nUserGroupKey)
	, m_nSysGroupKey(a_nSysGroupKey)
	, m_pTarget(a_pTarget)
{
	m_vecFilterInfo = a_vecFilterInfo;
}

KThemeSetFilterDlg::~KThemeSetFilterDlg()
{
}

void KThemeSetFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndRptCtrl);
}


BEGIN_MESSAGE_MAP(KThemeSetFilterDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KThemeSetFilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &KThemeSetFilterDlg::OnBnClickedChkAll)
END_MESSAGE_MAP()


// KThemeSetFilterDlg 메시지 처리기입니다.


BOOL KThemeSetFilterDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (m_nSysGroupKey == 1)
	{
		SetWindowText(_T("링크 타입 필터링"));
	}

	KReportCtrlSetting::Default(m_wndRptCtrl, FALSE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndRptCtrl);

	InitRptCtrl();
	UpdateRptCtrl();

	UpdateChkAllStatus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KThemeSetFilterDlg::InitRptCtrl( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nIdxColumn(0);

	pColumn = m_wndRptCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("선택"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndRptCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("User_Code_Name"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("타입명"));
    }

	pColumn = m_wndRptCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("System_Code_Name"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 타입명"));
    }

	m_wndRptCtrl.Populate();
}

void KThemeSetFilterDlg::UpdateRptCtrl( void )
{
	int nCntFilter = m_vecFilterInfo.size();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (int i = 0; i < nCntFilter; ++i)
	{
		TFilterInfo* pFilterInfo = m_vecFilterInfo[i];

		pRecord = m_wndRptCtrl.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(pFilterInfo->bChecked);
		pItem->SetItemData((DWORD_PTR)pFilterInfo);


		CString strCaption;
		KCodeManager* pCodeMgr       = m_pTarget->CodeManager();
		KCodeGroup*   pCodeUserGroup = pCodeMgr->FindCodeGroup(m_nUserGroupKey);
		KCodeGroup*   pCodeSysGroup  = pCodeMgr->FindCodeGroup(m_nSysGroupKey);

		strCaption = pCodeUserGroup->SingleCodeValue(pFilterInfo->nCodeUser, 1);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strCaption ));

		strCaption = pCodeSysGroup->SingleCodeValue(pFilterInfo->nCodeSystem, 1);
		
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strCaption ));
	}		

	m_wndRptCtrl.Populate();
}

void KThemeSetFilterDlg::OnBnClickedOk()
{
	CXTPReportRecords* pRecords = m_wndRptCtrl.GetRecords();

	int nCntRecord = pRecords->GetCount();

	int nCntSelected(0);

	//chk Selected
	for (int i = 0; i < nCntRecord; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
			++nCntSelected;
	}

	if (nCntSelected == 0)
	{
		AfxMessageBox(_T("필터 타입을 선택해 주세요."));
		return;
	}

	for (int i = 0; i < nCntRecord; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		TFilterInfo* pFilterInfo = (TFilterInfo*)pItem->GetItemData();

		if (pItem->IsChecked())
		{
			pFilterInfo->bChecked = TRUE;
		}
		else
		{
			pFilterInfo->bChecked = FALSE;
		}
	}

	KDialogEx::OnOK();
}

void KThemeSetFilterDlg::OnBnClickedChkAll()
{
	BOOL bChkAll = IsDlgButtonChecked(IDC_CHECK1);

	CXTPReportRecords* pRecords = m_wndRptCtrl.GetRecords();

	int nCntRecord = pRecords->GetCount();

	for (int i = 0; i < nCntRecord; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		pItem->SetChecked(bChkAll);
	}

	m_wndRptCtrl.RedrawControl();
}

void KThemeSetFilterDlg::UpdateChkAllStatus( void )
{
	BOOL bChkAll = TRUE;

	CXTPReportRecords* pRecords = m_wndRptCtrl.GetRecords();

	int nCntRecord = pRecords->GetCount();

	for (int i = 0; i < nCntRecord; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
			continue;

		bChkAll = FALSE;
		break;
	}

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1);

	if (bChkAll)
	{
		pButton->SetCheck(BST_CHECKED);
	}
	else
	{
		pButton->SetCheck(BST_UNCHECKED);
	}
}

