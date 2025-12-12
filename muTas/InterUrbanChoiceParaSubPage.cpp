// InterUrbanChoiceParaSubPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterUrbanChoiceParaSubPage.h"
#include "afxdialogex.h"


// KInterUrbanChoiceParaSubPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterUrbanChoiceParaSubPage, KDialogEx)

KInterUrbanChoiceParaSubPage::KInterUrbanChoiceParaSubPage(CWnd* pParent /*=NULL*/)
	: KDialogEx(KInterUrbanChoiceParaSubPage::IDD, pParent)
{

}

KInterUrbanChoiceParaSubPage::~KInterUrbanChoiceParaSubPage()
{
}

void KInterUrbanChoiceParaSubPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KInterUrbanChoiceParaSubPage, KDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KInterUrbanChoiceParaSubPage 메시지 처리기입니다.


BOOL KInterUrbanChoiceParaSubPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	InitReportHeader();
	UpdateReportData(m_oUrbanParameter.mapModeUrbanChoiceValue);

	SetBackgroundColor(RGB(255,255,255));
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterUrbanChoiceParaSubPage::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);

	int a = 1;

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KInterUrbanChoiceParaSubPage::SetParameterData( KUrbanChoiceParameter a_oUrbanChoiceParameter )
{
	m_oUrbanParameter = a_oUrbanChoiceParameter;
}

void KInterUrbanChoiceParaSubPage::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("변수"), 45));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("승용차"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("택시"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("버스"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("도시철도"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(5, _T("버스+도시철도"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(6, _T("Park & Ride"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(7, _T("Kiss & Ride"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(8, _T("택시 & 대중교통"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);
        }
        else {
            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Mode"), 45));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("PassengerCar"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Taxi"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Bus Transit"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("Rail Transit"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(5, _T("Composite Mode"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(6, _T("Park & Ride"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(7, _T("Kiss & Ride"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);

            pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(8, _T("Taxi & Transit"), 40));
            pColumn->SetHeaderAlignment(DT_CENTER);
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

void KInterUrbanChoiceParaSubPage::UpdateReportData(std::map<int, TUrbanChoiceValues> a_mapModeUrbanChoiceValue )
{
	try
	{
		m_wndReportCtrl.ResetContent();

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = a_mapModeUrbanChoiceValue.begin();
		AutoType end  = a_mapModeUrbanChoiceValue.end();
		//// 명지대 추가 - 20161214
		//CString tmp1stColumnCap[10] = {_T("평균 차내통행시간"), _T("평균 대기시간"), _T("평균 환승시간"),
		//_T("평균 통행비용"), _T("주차비용"), _T("평균 도보접근시간"), _T("평균 차내접근시간"), _T("평균 접근비용"),
		//_T("환승횟수"), _T("더미")};
		//int tmpIndex=0;

		while(iter != end)
		{
			TUrbanChoiceValues oChoiceValue = iter->second;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oChoiceValue.strCofficientName));
			pItem->SetCaption(oChoiceValue.strCoefficientDisplay);
			//// 명지대 추가 - 20161214
			//if (KmzSystem::GetLanguage() == KEMKorea) {
			//	if ( tmpIndex < 11 ) {
			//		pItem->SetCaption(tmp1stColumnCap[tmpIndex]);
			//		tmpIndex++;
			//	}
			//}
			pItem->SetItemData((int)oChoiceValue.nCofficientCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

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

		m_wndReportCtrl.Populate();
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

void KInterUrbanChoiceParaSubPage::GetParameterData( KUrbanChoiceParameter& a_oUrbanChoiceParameter )
{
	m_oUrbanParameter.mapModeUrbanChoiceValue.clear();
	try
	{	
		std::map<int, TUrbanChoiceValues> mapModeUrbanChoiceValue;

		CXTPReportRecords*				pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			TUrbanChoiceValues oInfo = {};

			pRecord						= pRecords->GetAt(i);
			pItemTXT					= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oInfo.strCoefficientDisplay = pItemTXT->GetCaption(nullptr);
			oInfo.strCofficientName		= pItemTXT->GetValue();
			oInfo.nCofficientCode		= pItemTXT->GetItemData();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oInfo.dAuto					= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oInfo.dTaxi					= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oInfo.dHighwayTransit		= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oInfo.dRailTransit			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oInfo.dCompositMode			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			oInfo.dParkAndRide			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(7);
			oInfo.dKissAndRide			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(8);
			oInfo.dTaxiAndTransit		= pItemNum->GetValue();
			
			// 2019.04.04 추가
			oInfo.strObjName = m_oUrbanParameter.pPurpose->PurposeName();
			oInfo.strObjNameEng = m_oUrbanParameter.pPurpose->PurposeName();
			// 2019.04.04 추가 끝

			mapModeUrbanChoiceValue.insert(std::make_pair(oInfo.nCofficientCode, oInfo));
		}

		m_oUrbanParameter.mapModeUrbanChoiceValue = mapModeUrbanChoiceValue;

		a_oUrbanChoiceParameter= m_oUrbanParameter;
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
