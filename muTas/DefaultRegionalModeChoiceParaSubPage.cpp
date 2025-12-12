// DefaultRegionalModeChoiceParaSubPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultRegionalModeChoiceParaSubPage.h"
#include "afxdialogex.h"


// KDefaultRegionalModeChoiceParaSubPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultRegionalModeChoiceParaSubPage, KDialogEx)

KDefaultRegionalModeChoiceParaSubPage::KDefaultRegionalModeChoiceParaSubPage(CWnd* pParent /*=NULL*/)
	: KDialogEx(KDefaultRegionalModeChoiceParaSubPage::IDD, pParent)
{

}

KDefaultRegionalModeChoiceParaSubPage::~KDefaultRegionalModeChoiceParaSubPage()
{
}

void KDefaultRegionalModeChoiceParaSubPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KDefaultRegionalModeChoiceParaSubPage, KDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KDefaultRegionalModeChoiceParaSubPage 메시지 처리기입니다.

BOOL KDefaultRegionalModeChoiceParaSubPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	InitReportHeader();
	UpdateReportData();

	SetBackgroundColor(RGB(255,255,255));
	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDefaultRegionalModeChoiceParaSubPage::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KDefaultRegionalModeChoiceParaSubPage::SetParameter( KRegionalChoiceParameter a_oRegionalParameter )
{
	m_oRegionalParameter = a_oRegionalParameter;
	m_mapModeRegionalChoiceValue = m_oRegionalParameter.mapModeRegionalChoiceValue;
}

void KDefaultRegionalModeChoiceParaSubPage::GetParameter( KRegionalChoiceParameter& ar_oRegionalParameter )
{
	m_mapModeRegionalChoiceValue.clear();

	try
	{
		CXTPReportRecords*				pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			TRegionalParameter oInfo = {};

			pRecord						= pRecords->GetAt(i);
			pItemTXT					= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oInfo.TstrModeName			= pItemTXT->GetCaption(nullptr);
			oInfo.TnModeCode			= pItemTXT->GetItemData();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oInfo.TdInvehicleTime		= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oInfo.TdWaitingTime			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oInfo.TdTransferTime		= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oInfo.TdProcessTime			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oInfo.TdFrequency			= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			oInfo.TdCost				= pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(7);
			oInfo.TdDummy				= pItemNum->GetValue();

			m_mapModeRegionalChoiceValue.insert(std::make_pair(oInfo.TnModeCode, oInfo));
		}

		m_oRegionalParameter.mapModeRegionalChoiceValue = m_mapModeRegionalChoiceValue;

		ar_oRegionalParameter = m_oRegionalParameter;
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

void KDefaultRegionalModeChoiceParaSubPage::InitReportHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("명칭"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("InVehicle Time"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);

        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("차내통행시간"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Setup Time"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);

        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("대기시간"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Transfer Time"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);

        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("환승시간"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("Process Time"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("탑승수속시간"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(5, _T("Frequency"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("운행횟수"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(6, _T("Cost"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행비용"));
        }

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(7, _T("Dummy"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("더미"));
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

void KDefaultRegionalModeChoiceParaSubPage::UpdateReportData( void )
{
	try
	{
		m_wndReportCtrl.ResetContent();

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = m_mapModeRegionalChoiceValue.begin();
		AutoType end  = m_mapModeRegionalChoiceValue.end();
		while(iter != end)
		{
			TRegionalParameter oInfo = iter->second;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oInfo.TstrModeName));
			pItem->SetItemData((int)oInfo.TnModeCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdInvehicleTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdWaitingTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdTransferTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdProcessTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdFrequency, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdCost, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdDummy, _T("%.6f")));
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

void KDefaultRegionalModeChoiceParaSubPage::UpdateReportData( std::map<int, TRegionalParameter> a_mapRegionalModeChoicePara )
{
	try
	{
		m_wndReportCtrl.ResetContent();

		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = a_mapRegionalModeChoicePara.begin();
		AutoType end  = a_mapRegionalModeChoicePara.end();
		while(iter != end)
		{
			TRegionalParameter oInfo = iter->second;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oInfo.TstrModeName));
			pItem->SetItemData((int)oInfo.TnModeCode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdInvehicleTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdWaitingTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdTransferTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdProcessTime, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdFrequency, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdCost, _T("%.6f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.TdDummy, _T("%.6f")));
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
