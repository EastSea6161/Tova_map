// DevPlanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanDlg.h"
#include "afxdialogex.h"

#include "DBaseDevPlan.h"

#include "DevPlanImportDirectInputStep1Dlg.h"
#include "DevPlanImportDirectInputStep2Dlg.h"

// KDevPlanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanDlg, KDialogEx)

KDevPlanDlg::KDevPlanDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanDlg::IDD, pParent)
	, m_emDevPlanMigrationType(KEMDevPlanMigrationDirectInput)
	, m_pTarget(a_pTarget)
	, m_bIsImport(false)
{
}

KDevPlanDlg::~KDevPlanDlg()
{
}

void KDevPlanDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndPlanReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndDirectInputReport);
	DDX_Radio  (pDX, IDC_RADIO1,  (int&)m_emDevPlanMigrationType);
}


BEGIN_MESSAGE_MAP(KDevPlanDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KDevPlanDlg::OnBnClickedImportMigration)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1, OnReportItemClick)
END_MESSAGE_MAP()


// KDevPlanDlg 메시지 처리기입니다.


BOOL KDevPlanDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	InitPlanReport();
	UpdatePlanReport();

	InitDirectInputReport();
	UpdateDirectInputReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDevPlanDlg::InitPlanReport( void )
{
	m_wndPlanReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndPlanReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndPlanReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndPlanReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
	
	//m_wndPlanReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = m_wndPlanReport.AddColumn(new CXTPReportColumn(0, _T("Selected"), 2));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(xtpColumnIconCenter);

	pCXTPColumn = m_wndPlanReport.AddColumn(new CXTPReportColumn(1, _T("No"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	pCXTPColumn = m_wndPlanReport.AddColumn(new CXTPReportColumn(2, _T("Plan Name"), 3));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	m_wndPlanReport.SetMultipleSelection(FALSE);
}

void KDevPlanDlg::UpdatePlanReport( void )
{
	CXTPReportRecord*     pCXTPRecord = NULL;
	CXTPReportRecordItem* pCXTPItem = NULL;

	std::map<int, CString> mapPlanInfo;

	KDBaseDevPlan::LstPlanInfo(m_pTarget, mapPlanInfo);

	std::map<int, CString>::iterator iter;

	for (iter = mapPlanInfo.begin(); iter != mapPlanInfo.end(); ++iter)
	{
		int     nPlanIndex   = iter->first;
		CString strPlanName  = iter->second;
		CString strPlanIndex;

		strPlanIndex.Format(_T("%d"), nPlanIndex);

		pCXTPRecord = m_wndPlanReport.AddRecord(new CXTPReportRecord);

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->HasCheckbox(TRUE);

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(strPlanIndex);

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(strPlanName);
	}

	m_wndPlanReport.Populate();
	m_wndPlanReport.ExpandAll(TRUE);
}

void KDevPlanDlg::InitDirectInputReport( void )
{
	m_wndDirectInputReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndDirectInputReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndDirectInputReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndDirectInputReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	//m_wndDirectInputReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = 
		m_wndDirectInputReport.AddColumn(new CXTPReportColumn(0, _T("Plan Zone ID"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	pCXTPColumn = 
		m_wndDirectInputReport.AddColumn(new CXTPReportColumn(1, _T("Zone ID"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	pCXTPColumn = 
		m_wndDirectInputReport.AddColumn(new CXTPReportColumn(2, _T("Migration Rate(%)"), 2));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	m_wndDirectInputReport.AllowEdit(TRUE);
	m_wndDirectInputReport.EditOnClick(TRUE);
	m_wndDirectInputReport.SetMultipleSelection(FALSE);
}

void KDevPlanDlg::UpdateDirectInputReport( void )
{
	if (!m_bIsImport)
	{
		m_wndDirectInputReport.Populate();
		return;
	}
	std::ifstream ifs(_T("ZoneParameter.dat"), std::ios::binary);

	__int64 nxPlanZoneID;
	__int64 nxFromZoneID;
	double  dMigrationRate;
	CString strValue;

	CXTPReportRecord*		pCXTPRecord = NULL;
	CXTPReportRecordItem*	pCXTPItem	= NULL;

	while (!ifs.eof())
	{
		ifs.read(reinterpret_cast<char*>(&nxPlanZoneID),   sizeof(__int64));
		ifs.read(reinterpret_cast<char*>(&nxFromZoneID),   sizeof(__int64));
		ifs.read(reinterpret_cast<char*>(&dMigrationRate), sizeof(double));

		if(ifs.eof())
			break;

		pCXTPRecord = m_wndDirectInputReport.AddRecord(new CXTPReportRecord);

		strValue.Format(_T("%I64d"), nxPlanZoneID);
		pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strValue));

		strValue.Format(_T("%I64d"), nxFromZoneID);
		pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strValue));

		strValue.Format(_T("%.2f"), dMigrationRate);
		pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strValue));
	}

	ifs.close();

	m_wndDirectInputReport.Populate();
}

void KDevPlanDlg::OnBnClickedImportMigration()
{
	bool bSelected = false;
	CString strSelPlanID;

	CXTPReportRecords* pRecords = m_wndPlanReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
		{
			strSelPlanID = pRecord->GetItem(1)->GetCaption(NULL);

			bSelected = true;
			break;
		}
	}


	if (!bSelected)
	{
		CString strMsg = _T("반영계획을 선택하세요.");
		AfxMessageBox(strMsg);

		return;
	}

	CString strNameZoneParam;

	strNameZoneParam = _T("ZoneParameter") + strSelPlanID + _T(".dat");

	CString strFileName(_T(""));
	BOOL bContainHeader(FALSE);
	int nSeparatorType(0);
	int nEncodingType(0);

	while (true)
	{
		KDevPlanImportDirectInputStep1Dlg dlg1(
			bContainHeader, nSeparatorType, nEncodingType, strFileName);

		if (IDCANCEL == dlg1.DoModal())
			return;

		bContainHeader = dlg1.ContainHeader();
		nSeparatorType = dlg1.SeparatorType();
		nEncodingType  = dlg1.EncodingType();
		strFileName    = dlg1.FileName();

		TCHAR tcSeparator;
		switch (nSeparatorType)
		{
		case 0 : tcSeparator = _T(',');
			break;
		case 1 : tcSeparator = _T(';');
			break;
		case 2 : tcSeparator = _T('|');
			break;
		case 3 : tcSeparator = _T('	'); 
			break;
		default: ASSERT(FALSE);
			break;
		}

		KEMCSVEncoding emCSVEncoding;
		switch( nEncodingType )
		{
		case 0 : emCSVEncoding = KEMCSVEncodingANSI; 
			break;
		case 1 : emCSVEncoding = KEMCSVEncodingUTF8; 
			break;
		case 2 : emCSVEncoding = KEMCSVEncodingUTF16LE; 
			break;
		default : ASSERT( FALSE ); 
			break;
		}

		KDevPlanImportDirectInputStep2Dlg dlg2(
			bContainHeader, emCSVEncoding, tcSeparator, strFileName);

		if(IDCANCEL == dlg2.DoModal())
			return;

		if (dlg2.GoBack())
			continue;

		break;
	}

	m_bIsImport = true;
	UpdateDirectInputReport();
}

std::set<int> KDevPlanDlg::PlanList( void )
{
	m_setPlanID.clear();

	CXTPReportRecords* pRecords = m_wndPlanReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (!pItem->IsChecked())
			continue;
		
		pItem = pRecord->GetItem(1);
		CString strIndex = pItem->GetCaption(NULL);

		m_setPlanID.insert(_ttoi(strIndex));
	}

	return m_setPlanID;
}

void KDevPlanDlg::OnReportItemClick( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	if(!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	if(pItemNotify->pColumn->GetItemIndex() == 0)
	{
		CXTPReportRecord*  pCurrent = pItemNotify->pItem->GetRecord();
		CXTPReportRecords* pRecords = pCurrent->GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);

			if(pRecord == pCurrent)
				continue;

			CXTPReportRecordItem* pItem = pRecord->GetItem(0);
			pItem->SetChecked(FALSE);

		}
	}
}


