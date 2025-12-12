#include "StdAfx.h"
#include "afxdialogex.h"
#include "PA2ODDefinePADlg.h"

#include "PA2ODInfo.h"
#include "Purpose.h"

#include "CustomReportRecordItem.h"

IMPLEMENT_DYNAMIC(KPA2ODDefinePADlg, KDialogEx)

KPA2ODDefinePADlg::KPA2ODDefinePADlg(std::list<KPA2ODInfo*> a_lstHomeBasedPA2ODInfo,
	std::list<KPA2ODInfo*> a_lstNotHomeBasedPA2ODInfo,	CWnd* pParent /*=NULL*/)
	: KDialogEx(KPA2ODDefinePADlg::IDD, pParent)
	, m_lstHomeBasedPA2ODInfo(a_lstHomeBasedPA2ODInfo)
	, m_lstNotHBasedPA2ODInfo(a_lstNotHomeBasedPA2ODInfo)
{
}

KPA2ODDefinePADlg::~KPA2ODDefinePADlg(void)
{
}

void KPA2ODDefinePADlg::DoDataExchange(CDataExchange* pDx)
{
	KDialogEx::DoDataExchange(pDx);
	DDX_Control(pDx, IDC_REPORT1, m_wndHomeBasedReport);
	DDX_Control(pDx, IDC_REPORT2, m_wndNotHBasedReport);
	DDX_Control(pDx, IDC_COMBO1,  m_cbHomeBasedReturnType);
	DDX_Radio  (pDx, IDC_RADIO1,  (int&)m_emPA2ODReturnType);
}

BEGIN_MESSAGE_MAP(KPA2ODDefinePADlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KPA2ODDefinePADlg::OnRadioClicked)
	ON_BN_CLICKED(IDOK, &KPA2ODDefinePADlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL KPA2ODDefinePADlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitHomeBasedReturnComboBox();

	CButton* pButton = NULL;
	UINT 	 nID 	 = 1;

	switch (m_emPA2ODReturnType)
	{
	case KEMPA2ODNotIncludeEachReturn : 
		nID = IDC_RADIO1; break;
	case KEMPA2ODIncludeEachReturn    : 
		nID = IDC_RADIO2; break;
	}
	pButton = (CButton*)GetDlgItem(nID);
	pButton->SetCheck(BST_CHECKED);
	
	UpdateCtrlState();
	
	KReportCtrlSetting::Default(m_wndHomeBasedReport, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndNotHBasedReport, TRUE,  FALSE, FALSE);

	InitHomeBasedReportCtrl();
	UpdateHomeBasedReportCtrl();

	InitNotHBasedReportCtrl();
	UpdateNotHBasedReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPA2ODDefinePADlg::InitHomeBasedReturnComboBox( void )
{
	m_cbHomeBasedReturnType.ResetContent();
	int nComboIndex(0);
	int nSelComboIndex(0);
	
	nComboIndex = m_cbHomeBasedReturnType.InsertString(-1, _T("------ select ------"));
	m_cbHomeBasedReturnType.SetItemData(nComboIndex, (DWORD_PTR)NULL);

	std::list<KPA2ODInfo*>::iterator iter, end = m_lstHomeBasedPA2ODInfo.end();

	for (iter = m_lstHomeBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;
		KPurpose* 	pPurpose   = pPA2ODInfo->Purpose();

		nComboIndex = m_cbHomeBasedReturnType.InsertString(-1, pPurpose->PurposeName());
		m_cbHomeBasedReturnType.SetItemData(nComboIndex, (DWORD_PTR)pPurpose);

		if (pPurpose == pPA2ODInfo->ReturnPurpose())
		{
			nSelComboIndex = nComboIndex;
		}
	}
	m_cbHomeBasedReturnType.SetCurSel(nSelComboIndex);
}

void KPA2ODDefinePADlg::InitHomeBasedReportCtrl( void )
{
	int nColumnItemIndex(0);

	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("선택"), 2);
	pReportColumn = m_wndHomeBasedReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(xtpColumnIconCenter);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("목적"), 3);
	pReportColumn = m_wndHomeBasedReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
}

void KPA2ODDefinePADlg::UpdateHomeBasedReportCtrl( void )
{
	std::list<KPA2ODInfo*>::iterator iter, end = m_lstHomeBasedPA2ODInfo.end();

	for (iter = m_lstHomeBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* 		  pPA2ODInfo 	= *iter;
		KPurpose* 			  pPurpose 		= pPA2ODInfo->Purpose();

		CXTPReportRecord* 	  pReportRecord = m_wndHomeBasedReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pRecordItem 	= NULL;

		//Selected
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetItemData((DWORD_PTR)pPA2ODInfo);
		pRecordItem->HasCheckbox(TRUE);

		if (pPA2ODInfo->Selected())
			pRecordItem->SetChecked(TRUE);

		//Purpose
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetCaption(pPurpose->PurposeName());
	}
	m_wndHomeBasedReport.Populate();
}

void KPA2ODDefinePADlg::InitNotHBasedReportCtrl( void )
{
	int nColumnItemIndex(0);

	CXTPReportColumn* pReportColumn = NULL;

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("선택"), 1);
	pReportColumn = m_wndNotHBasedReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(xtpColumnIconCenter);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Go"), 2);
	pReportColumn = m_wndNotHBasedReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Return"), 2);
	pReportColumn = m_wndNotHBasedReport.AddColumn(pReportColumn);
	pReportColumn->GetEditOptions()->AddComboButton(TRUE);
	pReportColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pReportColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pItemEditOptions = pReportColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pItemConstraints = pItemEditOptions->GetConstraints();

	pItemEditOptions->AddConstraint(_T("Null"), 0);

	std::list<KPA2ODInfo*>::iterator iter, end = m_lstNotHBasedPA2ODInfo.end();
	int nComboIndex(1);

	for (iter = m_lstNotHBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;
		KPurpose* pPurpose = pPA2ODInfo->Purpose();

		pItemEditOptions->AddConstraint(pPurpose->PurposeName(), (DWORD_PTR)pPurpose);
	}
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	pReportColumn->GetEditOptions()->m_bConstraintEdit = TRUE;
}

void KPA2ODDefinePADlg::UpdateNotHBasedReportCtrl( void )
{
	std::list<KPA2ODInfo*>::iterator iter, end = m_lstNotHBasedPA2ODInfo.end();

	for (iter = m_lstNotHBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo*           pPA2ODInfo    = *iter;
		KPurpose*             pPurpose      = pPA2ODInfo->Purpose();
		CXTPReportRecord*     pReportRecord = m_wndNotHBasedReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pRecordItem   = NULL;

		//Selected
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetItemData((DWORD_PTR)pPA2ODInfo);
		pRecordItem->HasCheckbox(TRUE);

		if (pPA2ODInfo->Selected())
			pRecordItem->SetChecked(TRUE);

		//Purpose
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText);
		pRecordItem->SetCaption(pPurpose->PurposeName());

		//Return
		pRecordItem = pReportRecord->AddItem(new CCustomReportRecordItem);
		pRecordItem->SetItemData((DWORD_PTR)pPA2ODInfo->ReturnPurpose());
	}
	m_wndNotHBasedReport.Populate();
}

void KPA2ODDefinePADlg::OnRadioClicked( UINT nID )
{
	switch(nID)
	{
	case IDC_RADIO1 : m_emPA2ODReturnType = KEMPA2ODNotIncludeEachReturn; break;
	case IDC_RADIO2 : m_emPA2ODReturnType = KEMPA2ODIncludeEachReturn;    break;
	}
	UpdateCtrlState();
}

void KPA2ODDefinePADlg::UpdateCtrlState( void )
{
	if (KEMPA2ODNotIncludeEachReturn == m_emPA2ODReturnType)
	{
		m_cbHomeBasedReturnType.EnableWindow(TRUE);
	}
	else if (KEMPA2ODIncludeEachReturn == m_emPA2ODReturnType)
	{
		m_cbHomeBasedReturnType.EnableWindow(FALSE);
	}
}



void KPA2ODDefinePADlg::OnBnClickedOk()
{
	//Validate Check
	KPurpose*          pReturnPurpose = NULL;
	int 	           nCurSel 		  = m_cbHomeBasedReturnType.GetCurSel();
	
	if (KEMPA2ODNotIncludeEachReturn == m_emPA2ODReturnType)
		pReturnPurpose = (KPurpose*)m_cbHomeBasedReturnType.GetItemData(nCurSel);

	if (KEMPA2ODNotIncludeEachReturn == m_emPA2ODReturnType && NULL == pReturnPurpose)
	{
		AfxMessageBox(_T("Return Type을 선택하세요."));
		return;
	}
	ApplyHomeBasedInfo();
	ApplyNotHBasedInfo();
	
	KDialogEx::OnOK();
}

void KPA2ODDefinePADlg::PA2ODHomeBasedReturnType( KEMPA2ODReturnType a_emPA2ODReturnType )
{
	m_emPA2ODReturnType = a_emPA2ODReturnType;
}

KEMPA2ODReturnType KPA2ODDefinePADlg::PA2ODHomeBasedReturnType( void )
{
	return m_emPA2ODReturnType;
}

void KPA2ODDefinePADlg::ApplyHomeBasedInfo( void )
{
	KPurpose*          pReturnPurpose = NULL;
	int 	           nCurSel 		  = m_cbHomeBasedReturnType.GetCurSel();

	if (KEMPA2ODNotIncludeEachReturn == m_emPA2ODReturnType)
		pReturnPurpose = (KPurpose*)m_cbHomeBasedReturnType.GetItemData(nCurSel);
	
	CXTPReportRecords* pReportRecords = m_wndHomeBasedReport.GetRecords();
	int                nRecordCount   = pReportRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pReportRecord = pReportRecords->GetAt(i);
		CXTPReportRecordItem* pRecordItem 	= pReportRecord->GetItem(0);
		KPA2ODInfo*           pPA2ODInfo    = (KPA2ODInfo*)pRecordItem->GetItemData();

		if (pRecordItem->IsChecked() == FALSE)
		{
			pPA2ODInfo->Selected(false);
		}
		else
		{
			pPA2ODInfo->Selected(true);
		}
		pPA2ODInfo->ReturnPurpose(pReturnPurpose);
	}
}

void KPA2ODDefinePADlg::ApplyNotHBasedInfo( void )
{
	CXTPReportRecords* pReportRecords = m_wndNotHBasedReport.GetRecords();
	int				   nRecordCount   = pReportRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* 	  pReportRecord = pReportRecords->GetAt(i);
		CXTPReportRecordItem* pRecordItem 	= pReportRecord->GetItem(0);
		KPA2ODInfo* 		  pPA2ODInfo 	= (KPA2ODInfo*)pRecordItem->GetItemData();

		if (pRecordItem->IsChecked() == FALSE)
		{
			pPA2ODInfo->ReturnPurpose(NULL);
			pPA2ODInfo->Selected(false);
			continue;
		}
		pPA2ODInfo->Selected(true);
		pRecordItem = pReportRecord->GetItem(2);
		
		KPurpose* pPurpose = (KPurpose*)pRecordItem->GetItemData();
		pPA2ODInfo->ReturnPurpose(pPurpose);
	}
}
