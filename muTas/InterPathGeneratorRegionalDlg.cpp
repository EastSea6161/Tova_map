// InterPathGeneratorRegionalDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGeneratorRegionalDlg.h"
#include "afxdialogex.h"
#include "Target.h"


// KInterPathGeneratorRegionalDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGeneratorRegionalDlg, KResizeDialogEx)

	KInterPathGeneratorRegionalDlg::KInterPathGeneratorRegionalDlg(KTarget* a_pTarget, TAssignRule a_oParameter, std::vector<TInterAutoCost> a_vecAutoCost,
	std::vector<TInterTaxiCost> a_vecTaxiCost,std::map<int, TTerminalProcessTime> a_mapTerminal, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGeneratorRegionalDlg::IDD, pParent)
	,m_pTarget(a_pTarget)
	,m_oRegionalParameter(a_oParameter)
	,m_vecRegionalAutoCost(a_vecAutoCost)
	,m_vecRegionalTaxiCost(a_vecTaxiCost)
	,m_mapRegionalProcessTime(a_mapTerminal)
{

}

KInterPathGeneratorRegionalDlg::~KInterPathGeneratorRegionalDlg()
{
}

void KInterPathGeneratorRegionalDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_AUTO, m_wndReportAuto);
	//DDX_Control(pDX, IDC_REPORT_TAXI, m_wndReportTaxi);
	DDX_Control(pDX, IDC_REPORT_TRANSIT, m_wndReportTermianl);
}


BEGIN_MESSAGE_MAP(KInterPathGeneratorRegionalDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterPathGeneratorRegionalDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterPathGeneratorRegionalDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterPathGeneratorRegionalDlg 메시지 처리기입니다.


BOOL KInterPathGeneratorRegionalDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	ReSizeComponent();

	KReportCtrlSetting::Default(m_wndReportAuto, TRUE);
	//KReportCtrlSetting::Default(m_wndReportTaxi, TRUE);
	KReportCtrlSetting::Default(m_wndReportTermianl, TRUE);

	InitAuotoHeader();
	//InitTaxiHeader();
	InitTerminalHeader();

	UpdateAutoData();
	//UpdateTaxiData();
	UpdateTerminalData();

	InitParameterControl();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterPathGeneratorRegionalDlg::InitAuotoHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportAuto.AddColumn(new CXTPReportColumn(0, _T("Fuel Type"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wndReportAuto.AddColumn(new CXTPReportColumn(1, _T("Price(ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("가격(￦/ℓ)"));
        }

		pColumn = m_wndReportAuto.AddColumn(new CXTPReportColumn(2, _T("Ratio"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("구성비"));
        }

		pColumn = m_wndReportAuto.AddColumn(new CXTPReportColumn(3, _T("Fuel Efficiency(Km/ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연비(Km/ℓ)"));
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
// 
// void KInterPathGeneratorRegionalDlg::InitTaxiHeader( void )
// {
// 	try
// 	{
// 		CXTPReportColumn* pColumn = nullptr;
// 
// 		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(0, _T("Taxi"), 70, FALSE));
// 		pColumn->SetHeaderAlignment(DT_CENTER);
// 
// 		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(1, _T("Base Fare"), 100));
// 		pColumn->SetHeaderAlignment(DT_CENTER);
// 
// 		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(2, _T("Base Dist(㎞)"), 100));
// 		pColumn->SetHeaderAlignment(DT_CENTER);
// 
// 		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(3, _T("Dist-Base Fare"), 100));
// 		pColumn->SetHeaderAlignment(DT_CENTER);
// 
// 		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(4, _T("Unit Dist"), 100));
// 		pColumn->SetHeaderAlignment(DT_CENTER);
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebug(ex->GetErrorMessage());
// 	}
// 	catch(...)
// 	{
// 		TxLogDebugException();
// 	}
// }

void KInterPathGeneratorRegionalDlg::InitTerminalHeader( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportTermianl.AddColumn(new CXTPReportColumn(0, _T("Mode"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("수단명"));
        }

		pColumn = m_wndReportTermianl.AddColumn(new CXTPReportColumn(1, _T("Access"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("탑승수속시간"));
        }

		pColumn = m_wndReportTermianl.AddColumn(new CXTPReportColumn(2, _T("Egress"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("하차시간"));
        }

		pColumn = m_wndReportTermianl.AddColumn(new CXTPReportColumn(3, _T("ModeTransfer"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("환승가능여부"));
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

void KInterPathGeneratorRegionalDlg::UpdateAutoData( void )
{
	try
	{
		m_wndReportAuto.ResetContent();

		size_t nCount = m_vecRegionalAutoCost.size();
		for (size_t i = 0 ; i < nCount; i++)
		{
			TInterAutoCost oAuto = m_vecRegionalAutoCost[i];

			CXTPReportRecord*		pRecord = m_wndReportAuto.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oAuto.strName));
			pItem->SetItemData(oAuto.nType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAuto.dPrice, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAuto.dRation, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oAuto.dfule, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
		}
		m_wndReportAuto.Populate();
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
// 
// void KInterPathGeneratorRegionalDlg::UpdateTaxiData( void )
// {
// 	try
// 	{
// 		m_wndReportTaxi.ResetContent();
// 
// 		size_t nCount = m_vecRegionalTaxiCost.size();
// 		for (size_t i = 0; i <nCount; i++)
// 		{
// 			TInterTaxiCost oTaxi = m_vecRegionalTaxiCost[i];
// 			CXTPReportRecord*		pRecord = m_wndReportTaxi.AddRecord(new CXTPReportRecord);
// 			CXTPReportRecordItem*	pItem	=  nullptr;
// 
// 			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTaxi.strName));
// 			pItem->SetItemData(oTaxi.nType);
// 			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
// 			pItem->SetEditable(FALSE);
// 			pItem->SetAlignment(DT_CENTER);
// 
// 			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dBasefare, _T("%.2f")));
// 			pItem->SetEditable(TRUE);
// 			pItem->SetAlignment(DT_CENTER);
// 
// 			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dBaseDist, _T("%.2f")));
// 			pItem->SetEditable(TRUE);
// 			pItem->SetAlignment(DT_CENTER);
// 
// 			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dAddFare, _T("%.2f")));
// 			pItem->SetEditable(TRUE);
// 			pItem->SetAlignment(DT_CENTER);
// 
// 			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dAddDist, _T("%.2f")));
// 			pItem->SetEditable(TRUE);
// 			pItem->SetAlignment(DT_CENTER);
// 		}
// 
// 		m_wndReportTaxi.Populate();
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebug(ex->GetErrorMessage());
// 	}
// 	catch(...)
// 	{
// 		TxLogDebugException();
// 	}
// }

void KInterPathGeneratorRegionalDlg::UpdateTerminalData( void )
{
	try
	{
		m_wndReportTermianl.ResetContent();

		AutoType iter = m_mapRegionalProcessTime.begin();
		AutoType end  = m_mapRegionalProcessTime.end();
		while(iter != end)
		{
			TTerminalProcessTime oTerminalInfo = iter->second;

			CXTPReportRecord* pRecord	= m_wndReportTermianl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTerminalInfo.strModeName));
			pItem->SetItemData(oTerminalInfo.nModeID);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTerminalInfo.dAccess, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTerminalInfo.dEgress, _T("%.0f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->HasCheckbox(TRUE);
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			if (oTerminalInfo.nModeTransfer == 1)
			{
				pItem->SetChecked(TRUE);
			}

			++iter;
		}

		m_wndReportTermianl.Populate();
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

void KInterPathGeneratorRegionalDlg::InitParameterControl( void )
{
	try
	{
		CString strValue(_T(""));
		strValue.Format(_T("%.2f"), m_oRegionalParameter.dFirstPathTime);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
		SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

		strValue.Format(_T("%.2f"), m_oRegionalParameter.dSecondePathLength);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
		SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

		if (1 == m_oRegionalParameter.nInvehicle)
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
		}

		if (1 == m_oRegionalParameter.nWating)
		{
			CheckDlgButton(IDC_CHECK_WATING, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_WATING, FALSE);
		}

		if (1 == m_oRegionalParameter.nTransfer)
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
		}

		if (1 == m_oRegionalParameter.nAETime)
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
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

bool KInterPathGeneratorRegionalDlg::SaveAutoValue( void )
{
	m_vecRegionalAutoCost.clear();

	try
	{
		CXTPReportRecords*				pRecords = m_wndReportAuto.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nSumRation = 0;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TInterAutoCost oAutoCost;

			pRecord				= pRecords->GetAt(i);
			pItemTXT			= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oAutoCost.nType		= (int)pItemTXT->GetItemData();
			oAutoCost.strName	= pItemTXT->GetCaption(nullptr);

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oAutoCost.dPrice	= pItemNum->GetValue();

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oAutoCost.dRation	= pItemNum->GetValue();

            /************************************************************************/
            /* pItemNum 에서 값을 가져오면
            가령 0.35 라고 Grid 상에는 0.35라고 보이지만
            dRatio 변수에 담고 나면 0.344444444444444444449 라고 담아지게 되는 경우가 발생한다.
            이를 위해 변환 로직 수행
            (Ratio 인 즉 소수점 두자인 데이터를 컨트롤하고 있는 상황이기때문에 단위를 % 로 변환한후 100% 인지 확인하는 방식으로 0.344444444449 => 35%  )
            */ 
            /************************************************************************/

            int nRatio = (int)(oAutoCost.dRation * 100.0 + 0.5);
            nSumRation = nSumRation + nRatio;

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oAutoCost.dfule		= pItemNum->GetValue();

			m_vecRegionalAutoCost.push_back(oAutoCost);
		}

		if (nSumRation < 100)
		{
			AfxMessageBox(_T("비율의 합이 1보다 작습니다."));
			return false;
		}
		else if (nSumRation > 100)
		{
			AfxMessageBox(_T("비율의 합이 1보다 큽니다."));
			return false;
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

	return true;
}
 
// void KInterPathGeneratorRegionalDlg::SaveTaxiValue( void )
// {
// 	m_vecRegionalTaxiCost.clear();
// 
// 	try
// 	{
// 		CXTPReportRecords*				pRecords = m_wndReportTaxi.GetRecords();
// 		CXTPReportRecord*				pRecord  = nullptr;
// 		CXTPReportRecordItemText*		pItemTXT = nullptr;
// 		CXTPReportRecordItemNumber*		pItemNum = nullptr;
// 
// 		int nRecordCount = pRecords->GetCount();
// 		for (int i = 0; i <nRecordCount; i++)
// 		{
// 			TInterTaxiCost oTaxiCost;
// 
// 			pRecord				= pRecords->GetAt(i);
// 			pItemTXT			= (CXTPReportRecordItemText*)pRecord->GetItem(0);
// 			oTaxiCost.nType		= (int)pItemTXT->GetItemData();
// 			oTaxiCost.strName	= pItemTXT->GetCaption(nullptr);
// 
// 			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
// 			oTaxiCost.dBaseDist	= pItemNum->GetValue();
// 
// 			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
// 			oTaxiCost.dBasefare	= pItemNum->GetValue();
// 
// 			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
// 			oTaxiCost.dAddFare	= pItemNum->GetValue();
// 
// 			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
// 			oTaxiCost.dAddDist	= pItemNum->GetValue();
// 
// 			m_vecRegionalTaxiCost.push_back(oTaxiCost);
// 		}
// 	}
// 	catch (KExceptionPtr ex)
// 	{
// 		TxLogDebug(ex->GetErrorMessage());
// 	}
// 	catch(...)
// 	{
// 		TxLogDebugException();
// 	}
// }

void KInterPathGeneratorRegionalDlg::SaveTermianlValue( void )
{
	m_mapRegionalProcessTime.clear();

	try
	{
		CXTPReportRecords*				pRecords = m_wndReportTermianl.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItem*			pItem	 = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TTerminalProcessTime oTerminal;

			pRecord					= pRecords->GetAt(i);
			pItemTXT				= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oTerminal.nModeID		= (int)pItemTXT->GetItemData();
			oTerminal.strModeName	= pItemTXT->GetCaption(nullptr);

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oTerminal.dAccess	= pItemNum->GetValue();

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oTerminal.dEgress	= pItemNum->GetValue();

			pItem				= pRecord->GetItem(3);
			if (pItem->IsChecked() == TRUE)
			{
				oTerminal.nModeTransfer = 1;
			}
			else
			{
				oTerminal.nModeTransfer = 0;
			}

			m_mapRegionalProcessTime.insert(std::make_pair(oTerminal.nModeID, oTerminal));
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

bool KInterPathGeneratorRegionalDlg::SaveParameterValue( void )
{
	try
	{
		CString strValue(_T(""));

		GetDlgItemText(IDC_EDIT_PATHTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
			return false;
		}
		m_oRegionalParameter.dFirstPathTime = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
			return false;
		}
		m_oRegionalParameter.dSecondePathLength = _ttof(strValue);

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
		{
			m_oRegionalParameter.nInvehicle = 1;
		}
		else
		{
			m_oRegionalParameter.nInvehicle = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
		{
			m_oRegionalParameter.nWating = 1;
		}
		else
		{
			m_oRegionalParameter.nWating = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
		{
			m_oRegionalParameter.nTransfer = 1;
		}
		else
		{
			m_oRegionalParameter.nTransfer = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
		{
			m_oRegionalParameter.nAETime = 1;
		}
		else
		{
			m_oRegionalParameter.nAETime = 0;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


void KInterPathGeneratorRegionalDlg::OnBnClickedOk()
{
	try
	{
		if (SaveAutoValue() == false)
		{
			return ;
		}
		//SaveTaxiValue();
		SaveTermianlValue();
		if (SaveParameterValue() == false)
		{
			AfxMessageBox(_T("잘못된 값을 입력하였습니다."));
			return;
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
	
	KResizeDialogEx::OnOK();
}


void KInterPathGeneratorRegionalDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterPathGeneratorRegionalDlg::GetParameterValue( TAssignRule& a_oParameter, std::vector<TInterAutoCost>& a_vecAutoCost, std::vector<TInterTaxiCost>& a_vecTaxiCost,std::map<int, TTerminalProcessTime>& a_mapTerminal )
{
	a_oParameter	= m_oRegionalParameter;
	a_vecAutoCost	= m_vecRegionalAutoCost;
	a_vecTaxiCost	= m_vecRegionalTaxiCost;
	a_mapTerminal	= m_mapRegionalProcessTime;
}

void KInterPathGeneratorRegionalDlg::ReSizeComponent( void )
{
	try
	{
		SetResize(IDC_STATIC_AUTO,			SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT_AUTO,			SZ_TOP_LEFT,SZ_TOP_RIGHT);

// 		SetResize(IDC_STATIC_TAXI,			SZ_TOP_LEFT, SZ_TOP_LEFT);
// 		SetResize(IDC_REPORT_TAXI,			SZ_TOP_LEFT,SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_TRANSIT,		SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT_TRANSIT,		SZ_TOP_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_PARA,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_CONDITION,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_FIRST,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_PATHTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_TIME,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_INVEHICLE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_WATING,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_TRANSFER,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_ACCESSEGRESS,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_SECOND,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_PATHLENGTH,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_SEPRATOR4,			SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

		SetResize(IDOK,						SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

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
