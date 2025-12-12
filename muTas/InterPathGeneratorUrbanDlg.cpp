// InterPathGeneratorUrbanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGeneratorUrbanDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "KExRecordItem.h"

// KInterPathGeneratorUrbanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGeneratorUrbanDlg, KResizeDialogEx)

KInterPathGeneratorUrbanDlg::KInterPathGeneratorUrbanDlg(KTarget* a_pTarget, std::vector<TInterAutoCost> a_vecUrbanAuto, std::vector<TInterTaxiCost> a_vecUrbanTaxi, std::vector<TTransitCost> a_vecTransit, TAssignRule a_oUrbanPara, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGeneratorUrbanDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_vecUrbanAutoCost(a_vecUrbanAuto),
	m_vecUrbanTaxiCost(a_vecUrbanTaxi),
	m_vecTransitCost(a_vecTransit),
	m_oUrbanParameter(a_oUrbanPara)
{

}

KInterPathGeneratorUrbanDlg::~KInterPathGeneratorUrbanDlg()
{
}

void KInterPathGeneratorUrbanDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_AUTO, m_wndReportAuto);
	DDX_Control(pDX, IDC_REPORT_TAXI, m_wndReportTaxi);
	DDX_Control(pDX, IDC_REPORT_TRANSIT, m_wndReportTransit);
}


BEGIN_MESSAGE_MAP(KInterPathGeneratorUrbanDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterPathGeneratorUrbanDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterPathGeneratorUrbanDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL KInterPathGeneratorUrbanDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ReSizeComponent();

	KReportCtrlSetting::Default(m_wndReportAuto, TRUE);
	KReportCtrlSetting::Default(m_wndReportTaxi, TRUE);
	KReportCtrlSetting::Default(m_wndReportTransit, TRUE);

	InitReportHeaderAuto();
	InitReportHeaderTaxi();
	InitReportHeaderTransit();

	UpdateReportDataAuto();
	UpdateReportDataTaxi();
	UpdateReportDataTransit();

	InitPathParameter();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterPathGeneratorUrbanDlg::InitReportHeaderAuto( void )
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
        
		pColumn = m_wndReportAuto.AddColumn(new CXTPReportColumn(3, _T("Fuel Efficiency(㎞/ℓ)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연비(㎞/ℓ)"));
        }
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::InitReportHeaderTaxi( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(0, _T("Fuel Type"), 70, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(1, _T("Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("기본운임"));
        }

		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(2, _T("Base Dist(㎞)"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("~Km까지"));
        }

		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(3, _T("Dist-Base Fare"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("추가운임"));
        }

		pColumn = m_wndReportTaxi.AddColumn(new CXTPReportColumn(4, _T("Unit Dist"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("매 ~Km마다"));
        }
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::InitReportHeaderTransit( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(0, _T("Mode"), 120));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시스템 타입명"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(1, _T("Type"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("타입 명칭"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(2, _T("Base Fare"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("기본운임"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(3, _T("Base Dist(㎞)"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("~Km까지"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(4, _T("Dist-Base Fare"), 80));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("추가운임"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(5, _T("Unit Dist"), 70));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("매 ~Km마다"));
        }

		pColumn = m_wndReportTransit.AddColumn(new CXTPReportColumn(6, _T("D.F.P"), 40));
		pColumn->SetTooltip(_T("Discount Fare Policy"));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통합환승할인"));
			pColumn->SetTooltip(_T("통합환승할인"));
        }
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::UpdateReportDataAuto( void )
{
	try
	{
		m_wndReportAuto.ResetContent();

		size_t nCount = m_vecUrbanAutoCost.size();
		for (size_t i = 0 ; i < nCount; i++)
		{
			TInterAutoCost oAuto = m_vecUrbanAutoCost[i];

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
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::UpdateReportDataTaxi( void )
{
	try
	{
		m_wndReportTaxi.ResetContent();

		size_t nCount = m_vecUrbanTaxiCost.size();
		for (size_t i = 0; i <nCount; i++)
		{
			TInterTaxiCost oTaxi = m_vecUrbanTaxiCost[i];
			CXTPReportRecord*		pRecord = m_wndReportTaxi.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTaxi.strName));
			pItem->SetItemData(oTaxi.nType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dBasefare, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dBaseDist, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dAddFare, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTaxi.dAddDist, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);
		}

		m_wndReportTaxi.Populate();
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::UpdateReportDataTransit( void )
{
	try
	{
		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup*	  pModeCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_MODE_CODE_KEY);
		KCodeGroup*	  pTypeCodeGroup	= pCodeMgr->FindCodeGroup(TRANSIT_TYPE_CODE_KEY);

		int nCount = m_vecTransitCost.size();
		for (int i = 0; i < nCount; i++)
		{
			TTransitCost oTransitInfo = m_vecTransitCost[i];
			CXTPReportRecord*		pRecord = m_wndReportTransit.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	=  nullptr;

			CString strModeName = pModeCodeGroup->SingleCodeValue(oTransitInfo.nMode);
			CString strTypeName = pTypeCodeGroup->SingleCodeValue(oTransitInfo.nTType);
			CString strTypeCode(_T(""));
			strTypeCode.Format(_T("%d:%s"), oTransitInfo.nTType, strTypeName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strModeName));
			pItem->SetItemData(oTransitInfo.nMode);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strTypeCode));
			pItem->SetItemData(oTransitInfo.nTType);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTbasicRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTbasicDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTAddRate, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oTransitInfo.dTAddDistance, _T("%.2f")));
			pItem->SetEditable(TRUE);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			if( 1 == oTransitInfo.nTMergeRate )
				pItem->SetChecked(TRUE);
			else
				pItem->SetChecked(FALSE);
		}

		m_wndReportTransit.Populate();
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::InitPathParameter( void )
{
	try
	{
		CString strValue(_T(""));
		strValue.Format(_T("%.2f"), m_oUrbanParameter.dDWTimeTaxi);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXITIME);
		SetDlgItemText(IDC_EDIT_TAXITIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanParameter.dDWTimeBusstop);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUSTIME);
		SetDlgItemText(IDC_EDIT_BUSTIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanParameter.dDwTimeRailStation);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SUBWAYTIME);
		SetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanParameter.dFirstPathTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
		SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanParameter.dSecondePathLength);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
		SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

		if (1 == m_oUrbanParameter.nInvehicle)
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
		}

		if (1 == m_oUrbanParameter.nWating)
		{
			CheckDlgButton(IDC_CHECK_WATING, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_WATING, FALSE);
		}

		if (1 == m_oUrbanParameter.nTransfer)
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
		}

		if (1 == m_oUrbanParameter.nAETime)
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



void KInterPathGeneratorUrbanDlg::OnBnClickedOk()
{
	try
	{
		if (SaveAutoValue() == false)
		{
			return;
		}
		SaveTaxiValue();
		SaveTransitValue();
		if (SaveParameterValue() == false)
		{
			AfxMessageBox(_T("잘못된 값을 입력하였습니다."));
			return; 
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
	KResizeDialogEx::OnOK();
}

void KInterPathGeneratorUrbanDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

bool KInterPathGeneratorUrbanDlg::SaveAutoValue( void )
{
	m_vecUrbanAutoCost.clear();

	try
	{
		CXTPReportRecords*				pRecords = m_wndReportAuto.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nSumRatio(0);

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

			nSumRatio = nSumRatio + nRatio;

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oAutoCost.dfule		= pItemNum->GetValue();

			m_vecUrbanAutoCost.push_back(oAutoCost);
		}

		if (nSumRatio > 100)
		{
			AfxMessageBox(_T("비율의 합(승용차)이 1보다 큽니다."));
			return false;
		}
		else if (nSumRatio < 100)
		{
			AfxMessageBox(_T("비율의 합(승용차)이 1보다 작습니다."));
			return false;
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
	return true;
}

void KInterPathGeneratorUrbanDlg::SaveTaxiValue( void )
{
	m_vecUrbanTaxiCost.clear();

	try
	{
		CXTPReportRecords*				pRecords = m_wndReportTaxi.GetRecords();
		CXTPReportRecord*				pRecord  = nullptr;
		CXTPReportRecordItemText*		pItemTXT = nullptr;
		CXTPReportRecordItemNumber*		pItemNum = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TInterTaxiCost oTaxiCost;

			pRecord				= pRecords->GetAt(i);
			pItemTXT			= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oTaxiCost.nType		= (int)pItemTXT->GetItemData();
			oTaxiCost.strName	= pItemTXT->GetCaption(nullptr);

            pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
            oTaxiCost.dBasefare	= pItemNum->GetValue();

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oTaxiCost.dBaseDist	= pItemNum->GetValue();
            
			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oTaxiCost.dAddFare	= pItemNum->GetValue();

			pItemNum			= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oTaxiCost.dAddDist	= pItemNum->GetValue();

			m_vecUrbanTaxiCost.push_back(oTaxiCost);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KInterPathGeneratorUrbanDlg::SaveTransitValue( void )
{
	m_vecTransitCost.clear();
	try
	{
		CXTPReportRecords*           pRecords		= m_wndReportTransit.GetRecords();
		CXTPReportRecord*            pRecord		= nullptr;
		CXTPReportRecordItemNumber*  pItemNum		= nullptr;
		CXTPReportRecordItem*        pItem			= nullptr;
		CXTPReportRecordItem*        pExtItemCheck	= nullptr;

		int nRecordCount   = pRecords->GetCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			TTransitCost oTransitCost;
			pRecord						= pRecords->GetAt(i);

			pItem						= pRecord->GetItem(0);
			oTransitCost.nMode			= (int)pItem->GetItemData();

			pItem						= pRecord->GetItem(1);
			oTransitCost.nTType			= (int)pItem->GetItemData();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			oTransitCost.dTbasicRate    =  pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			oTransitCost.dTbasicDistance=  pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(4);
			oTransitCost.dTAddRate		=  pItemNum->GetValue();

			pItemNum					= (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			oTransitCost.dTAddDistance	=  pItemNum->GetValue();

			pExtItemCheck				= (CXTPReportRecordItem*)pRecord->GetItem(6);
			if (pExtItemCheck->IsChecked() == TRUE)
			{
				oTransitCost.nTMergeRate = 1;
			}
			else
			{
				oTransitCost.nTMergeRate = 0;
			}

			m_vecTransitCost.push_back(oTransitCost);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

bool KInterPathGeneratorUrbanDlg::SaveParameterValue( void )
{
	try
	{
		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_TAXITIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_TAXITIME)->SetFocus();
			return false;
		}
		m_oUrbanParameter.dDWTimeTaxi = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_BUSTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_BUSTIME)->SetFocus();
			return false;
		}
		m_oUrbanParameter.dDWTimeBusstop = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_SUBWAYTIME)->SetFocus();
			return false;
		}
		m_oUrbanParameter.dDwTimeRailStation = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_PATHTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
			return false;
		}
		m_oUrbanParameter.dFirstPathTime = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
			return false;
		}
		m_oUrbanParameter.dSecondePathLength = _ttof(strValue);

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
		{
			m_oUrbanParameter.nInvehicle = 1;
		}
		else
		{
			m_oUrbanParameter.nInvehicle = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
		{
			m_oUrbanParameter.nWating = 1;
		}
		else
		{
			m_oUrbanParameter.nWating = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
		{
			m_oUrbanParameter.nTransfer = 1;
		}
		else
		{
			m_oUrbanParameter.nTransfer = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
		{
			m_oUrbanParameter.nAETime = 1;
		}
		else
		{
			m_oUrbanParameter.nAETime = 0;
		}

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


void KInterPathGeneratorUrbanDlg::GetParameterValue( std::vector<TInterAutoCost>& a_vecUrbanAuto, std::vector<TInterTaxiCost>& a_vecUrbanTaxi, std::vector<TTransitCost>& a_vecTransit, TAssignRule& a_oUrbanPara )
{
	a_oUrbanPara	= m_oUrbanParameter;
	a_vecUrbanAuto	= m_vecUrbanAutoCost;
	a_vecUrbanTaxi	= m_vecUrbanTaxiCost;
	a_vecTransit	= m_vecTransitCost;
}


void KInterPathGeneratorUrbanDlg::ReSizeComponent( void )
{
	try
	{
		SetResize(IDC_STATIC_AUTO,			SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT_AUTO,			SZ_TOP_LEFT,SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_TAXI,			SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT_TAXI,			SZ_TOP_LEFT,SZ_TOP_RIGHT);

		SetResize(IDC_STATIC_TRANSIT,		SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_REPORT_TRANSIT,		SZ_TOP_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_SEPRATOR3,			SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC_CPARA,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC_TAXTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_TAXITIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_BUSTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_BUSTIME,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_SUBWAYTIME,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_SUBWAYTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC_COND,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_FIRST1,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_PATHTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_TIME2,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_INVEHICLE,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_WATING,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_TRANSFER,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK_ACCESSEGRESS,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_SECOND2,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_PATHLENGTH,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_SEPRATOR4,			SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDOK,						SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDCANCEL,					SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
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
