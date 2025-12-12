// DefaultInterModeChoiceParaSubPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultInterModeChoiceParaSubPage.h"
#include "afxdialogex.h"
#include "DBaseInterModal.h"


// KDefaultInterModeChoiceParaSubPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultInterModeChoiceParaSubPage, KDialogEx)

KDefaultInterModeChoiceParaSubPage::KDefaultInterModeChoiceParaSubPage(int a_nPageID, CString& strObjName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDefaultInterModeChoiceParaSubPage::IDD, pParent)
	, m_nPageID(a_nPageID), m_strObjName(strObjName)
{
}

KDefaultInterModeChoiceParaSubPage::~KDefaultInterModeChoiceParaSubPage()
{
}

void KDefaultInterModeChoiceParaSubPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
}


BEGIN_MESSAGE_MAP(KDefaultInterModeChoiceParaSubPage, KDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KDefaultInterModeChoiceParaSubPage 메시지 처리기입니다.

void KDefaultInterModeChoiceParaSubPage::SetParameterData( std::map<int, TUrbanChoiceValues> a_mapInterModeChoicePara )
{
	m_mapInterModeChoicePara = a_mapInterModeChoicePara;
}

BOOL KDefaultInterModeChoiceParaSubPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, TRUE);
	InitReportHeader();
	UpdateReportData();

	SetBackgroundColor(RGB(255,255,255));
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDefaultInterModeChoiceParaSubPage::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		//pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("Variable/Alternatives"), 40));
		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("Mode"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("변수"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("PassengerCar"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("승용차"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(2, _T("Taxi"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("택시"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(3, _T("Bus Transit"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("버스"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(4, _T("Rail Transit"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("도시철도"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(5, _T("Composite Mode"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("버스+도시철도"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(6, _T("Park & Ride"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(7, _T("Kiss & Ride"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(8, _T("Taxi & Transit"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("택시 & 대중교통"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDefaultInterModeChoiceParaSubPage::UpdateReportData( void )
{
	try
	{
		m_ctrlReport.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapInterModeChoicePara.begin();
		AutoType itEnd = m_mapInterModeChoicePara.end();

		while (iter != itEnd)
		{
			TUrbanChoiceValues &oChoiceValue = iter->second;

			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oChoiceValue.strCofficientName));
			pItem->SetItemData((int)oChoiceValue.nCofficientCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetCaption(oChoiceValue.strCoefficientDisplay);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dAuto, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dTaxi, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dHighwayTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dRailTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dCompositMode, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dParkAndRide, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dKissAndRide, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dTaxiAndTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			++iter;
		}

		m_ctrlReport.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultInterModeChoiceParaSubPage::UpdateReportData( std::map<int, TUrbanChoiceValues> a_mapInterModeChoicePara )
{
	try
	{
		m_ctrlReport.ResetContent();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = a_mapInterModeChoicePara.begin();
		AutoType itEnd = a_mapInterModeChoicePara.end();

		while (iter != itEnd)
		{
			TUrbanChoiceValues &oChoiceValue = iter->second;

			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oChoiceValue.strCofficientName));
			pItem->SetItemData((int)oChoiceValue.nCofficientCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetCaption(oChoiceValue.strCoefficientDisplay);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dAuto, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dTaxi, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dHighwayTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dRailTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dCompositMode, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dParkAndRide, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dKissAndRide, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oChoiceValue.dTaxiAndTransit, _T("%.8f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			++iter;
		}

		m_ctrlReport.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

}


void KDefaultInterModeChoiceParaSubPage::GetNewParameterData(std::map<int, TUrbanChoiceValues> &a_mapNewInterModeChoicePara)
{
	try
	{
		CXTPReportRecords*          pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItem*       pItem    = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;
		
		int nRecordCnt            = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord  = pRecords->GetAt(i);

			TUrbanChoiceValues oChoiceValue;

			pItem   = pRecord->GetItem(0);
			int nCoefficientID = (int)pItem->GetItemData();
			oChoiceValue.nCofficientCode   = nCoefficientID;
			oChoiceValue.strCofficientName = ((CXTPReportRecordItemText*)pItem)->GetCaption(nullptr);

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oChoiceValue.dAuto           = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oChoiceValue.dTaxi           = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oChoiceValue.dHighwayTransit = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oChoiceValue.dRailTransit    = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oChoiceValue.dCompositMode   = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			oChoiceValue.dParkAndRide    = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(7);
			oChoiceValue.dKissAndRide    = pItemNum->GetValue();

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(8);
			oChoiceValue.dTaxiAndTransit = pItemNum->GetValue();

			// 2019.04.04 수정
			oChoiceValue.strObjName = m_strObjName;
			oChoiceValue.strObjNameEng = m_strObjName;

			a_mapNewInterModeChoicePara.insert(std::make_pair(nCoefficientID, oChoiceValue));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDefaultInterModeChoiceParaSubPage::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


