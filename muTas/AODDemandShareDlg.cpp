// AODDemandShareDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "AODDemandShareDlg.h"
#include "afxdialogex.h"


/* Define Report column index */
#define AODDEMAND_ITERATION		0
#define AODDEMAND_RATE			1


// KAODDemandShareDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAODDemandShareDlg, KDialogEx)

KAODDemandShareDlg::KAODDemandShareDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KAODDemandShareDlg::IDD, pParent)
	, m_nIteration(0)
	, m_strSum(_T(""))
{

}

KAODDemandShareDlg::~KAODDemandShareDlg()
{
}

void KAODDemandShareDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nIteration);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Text(pDX, IDC_EDIT2, m_strSum);
	DDX_Control(pDX, IDC_EDIT1, m_edtIteration);
}


BEGIN_MESSAGE_MAP(KAODDemandShareDlg, KDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &KAODDemandShareDlg::OnEnChangeEdit1)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KAODDemandShareDlg::OnRateValueChanged)
END_MESSAGE_MAP()

int KAODDemandShareDlg::Iteration(void)
{
	return m_nIteration;
}


void KAODDemandShareDlg::Iteration(int nIteration)
{
	m_nIteration = nIteration;
}


std::list<double>& KAODDemandShareDlg::RateList(void)
{
	return m_RateList;
}


void KAODDemandShareDlg::initReportControl(void)
{
	// Add Columns
	CXTPReportColumn* pColumn;
	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(AODDEMAND_ITERATION, _T("Iteration"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수행 횟수"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(AODDEMAND_RATE, _T("Trip Rate"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("배정 비율"));
    }
}


void KAODDemandShareDlg::updateReportRecord(void)
{
	CXTPReportRecord* pRecord;
	CXTPReportRecordItem* pItem;
	int nIteration = 1;
	double dSum = 0.0f;

	m_wndReport.ResetContent();

	if(m_nIteration > 0)
	{
		for(int i = 0; i < m_nIteration-1; ++i)
		{
			pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nIteration++));
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(m_dRate, _T("%.3f")));
			dSum += m_dRate;
		}

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(nIteration));
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(m_dLastRate, _T("%.3f")));
		dSum += m_dLastRate;
	}

	m_strSum.Format(_T("%.3f"), dSum);
	UpdateData(FALSE);

	m_wndReport.Populate();
}


void KAODDemandShareDlg::AutoRateDevide(void)
{
	if(m_nIteration <= 0)
	{
		return;
	}
	// todo : calculate
	int nRate = 1000 / m_nIteration;
	int nLastRate = 1000 - (nRate * (m_nIteration - 1));
	
	m_dRate = (double)nRate / 1000.0f;
	m_dLastRate = (double)nLastRate / 1000.0f;
}


// KAODDemandShareDlg 메시지 처리기입니다.
BOOL KAODDemandShareDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	m_edtIteration.SetLimitText(3);

	UpdateData(FALSE);

	KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE, FALSE);

	AutoRateDevide();
	initReportControl();
	updateReportRecord();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAODDemandShareDlg::OnRateValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	CString strTripRate = pItemNotify->pItem->GetCaption(NULL);
	//pItemNotify->pItem->GetItemData();
	double dRate = _ttof(strTripRate);

	SumTripRate();
}


void KAODDemandShareDlg::SumTripRate()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	double dSum = 0.0;

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(AODDEMAND_RATE));
		
		dSum += pNumber->GetValue();
	}

	m_strSum.Format(_T("%.3f"), dSum);
	UpdateData(FALSE);
}


void KAODDemandShareDlg::OnOK()
{
	UpdateData(TRUE);

	double dSumValue = _ttof(m_strSum);

	if(dSumValue != 1.0)
	{
		AfxMessageBox(_T("배정 비율의 총 합은 1이 되어야 합니다.\r\n비율을 재조정 해주세요."));
		return;
	}

	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	m_RateList.clear();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(AODDEMAND_RATE));
		m_RateList.push_back(pNumber->GetValue());
	}

	KDialogEx::OnOK();
}


void KAODDemandShareDlg::OnEnChangeEdit1()
{
	UpdateData(TRUE);
	AutoRateDevide();
	updateReportRecord();
}


