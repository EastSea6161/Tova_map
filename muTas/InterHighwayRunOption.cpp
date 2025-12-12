// InterHighwayRunOption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterHighwayRunOption.h"
#include "afxdialogex.h"
#include "Target.h"
#include "KBulkDBaseUtil.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"

// KInterHighwayRunOption 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterHighwayRunOption, KResizeDialogEx)

KInterHighwayRunOption::KInterHighwayRunOption(KTarget* a_pTarget, int& a_nChkTurn, TInterRunOptionInfo& a_oRunOption, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterHighwayRunOption::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nChkTurn(a_nChkTurn)
	, m_oRunOption(a_oRunOption)
{

}

KInterHighwayRunOption::~KInterHighwayRunOption()
{
}

void KInterHighwayRunOption::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndModeReport);
	
}


BEGIN_MESSAGE_MAP(KInterHighwayRunOption, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KInterHighwayRunOption::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterHighwayRunOption::OnBnClickedCancel)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItemMode)
	ON_BN_CLICKED(IDC_BUTTON1, &KInterHighwayRunOption::OnBnClickedButtonDef)
	ON_BN_CLICKED(IDC_CHECK_FREIGHT, &KInterHighwayRunOption::OnBnClickedCheckFreight)
	ON_BN_CLICKED(IDC_CHECK_REGION_BUS, &KInterHighwayRunOption::OnBnClickedCheckRegionBus)
END_MESSAGE_MAP()

// KInterHighwayRunOption 메시지 처리기입니다.


BOOL KInterHighwayRunOption::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CString strNotExists = _T("Data not exists");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strNotExists = _T("데이터 존재하지 않음");
	}
	CString strEmpty     = _T("");

	bool bExistTurn    = KBulkDBaseUtil::ExitstTurnData(m_pTarget->GetDBaseConnection());
	bool bExistTransit = KBulkDBaseUtil::ExistTransitData(m_pTarget->GetDBaseConnection()); 


	if (bExistTurn == true)
	{
		GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STC_TRUN)->SetWindowTextW(strEmpty);
		if (m_nChkTurn == 1)
		{
			CheckDlgButton(IDC_CHECK1, TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STC_TRUN)->SetWindowTextW(strNotExists);
	}
	
	if (bExistTransit == true)
	{
		GetDlgItem(IDC_CHECK_URBAN_BUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_STC_URBAN_BUS)->SetWindowTextW(strEmpty);

		if (m_oRunOption.nUseBusIV == 1)
		{
			CheckDlgButton(IDC_CHECK_URBAN_BUS, TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_CHECK_URBAN_BUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_STC_URBAN_BUS)->SetWindowTextW(strNotExists);
	}

	if (m_oRunOption.nUseRegionalBusIV == 1)
	{
		CheckDlgButton(IDC_CHECK_REGION_BUS, TRUE);

		if (m_oRunOption.nMinLine == 0)
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		}
		else
		{
			CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		}

		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);

	}
	else
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
	}
	

	if (m_oRunOption.nUseFreightIV == 1)
	{
		CheckDlgButton(IDC_CHECK_FREIGHT, TRUE);
	}

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndModeReport);

	InitReportHeader();
	UpdateReportData(m_oRunOption.mapInterFOPMode);

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterHighwayRunOption::OnBnClickedOk()
{
	if (IsDlgButtonChecked(IDC_CHECK1))
	{
		m_nChkTurn = 1;
	}
	else
	{
		m_nChkTurn = 0;
	}

	if (IsDlgButtonChecked(IDC_CHECK_URBAN_BUS))
	{
		m_oRunOption.nUseBusIV = 1;
	}
	else
	{
		m_oRunOption.nUseBusIV = 0;
	}

	if (IsDlgButtonChecked(IDC_CHECK_REGION_BUS))
	{
		m_oRunOption.nUseRegionalBusIV = 1;
	}
	else
	{
		m_oRunOption.nUseRegionalBusIV = 0;
	}

	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2);
	if (nCheck == IDC_RADIO1)
	{
		m_oRunOption.nMinLine = 0;
	}
	else
	{
		m_oRunOption.nMinLine = 1;
	}

	SaveFInitialVolume();

	if (IsDlgButtonChecked(IDC_CHECK_FREIGHT))
	{
		m_oRunOption.nUseFreightIV = 1;
	}
	else
	{
		m_oRunOption.nUseFreightIV = 0;
	}

	if (m_oRunOption.nUseFreightIV == 1 && m_oRunOption.mapInterFOPMode.size() < 1)
	{
		AfxMessageBox(_T("초기교통량 배정 통행수단이 선택되지 않았습니다."));
		return;
	}
	
	KResizeDialogEx::OnOK();
}

void KInterHighwayRunOption::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterHighwayRunOption::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(0,		_T("Select"),		20));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(1,		_T("Mode"),			40));
	pColumn->SetHeaderAlignment(DT_CENTER);			
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(2,		_T("Matrix"),		40));
	pColumn->SetHeaderAlignment(DT_CENTER);			
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(3,		_T("Occupancy"),	40));
	pColumn->SetHeaderAlignment(DT_CENTER);			
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("평균 재차인원"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(4,		_T("PCE"),			40));
	pColumn->SetHeaderAlignment(DT_CENTER);			
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("승용차 환산계수"));
    }
}

void KInterHighwayRunOption::UpdateReportData(std::map<int, KInterOccupancePCEMode> a_mapInterFOPMode )
{
	m_wndModeReport.ResetContent();
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_FREIGHT);
	try
	{
		CXTPReportRecords*			 pRecords	  = m_wndModeReport.GetRecords();
		CXTPReportRecord*			 pRecord	  = nullptr;
		CXTPReportRecordItem*        pItem   	  = nullptr;
		CXTPReportRecordItem*		 pSelItem	  = nullptr;
		CXTPReportRecordItemComboEx* pItemCombo   = nullptr;

		std::map<int, std::vector<CString>> mapModeChocieColumns;
		KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, mapModeChocieColumns);

		AutoType iter = a_mapInterFOPMode.begin();
		AutoType end  = a_mapInterFOPMode.end();
		while(iter != end)
		{
			KInterOccupancePCEMode oInfo = iter->second;

			std::vector<CString> vecModeColumn;
			AutoType findCol = mapModeChocieColumns.find(oInfo.nModeID);
			AutoType endCol  = mapModeChocieColumns.end();
			if (findCol != endCol)
			{
				vecModeColumn = findCol->second;
			}
			size_t nColumnCount = vecModeColumn.size();

			pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);

			//check box
			pSelItem = pRecord->AddItem(new CXTPReportRecordItem);
			pSelItem->HasCheckbox(TRUE);
			pSelItem->SetAlignment(xtpColumnIconCenter);
			if (nColumnCount < 1)
			{
				pSelItem->SetEditable(FALSE);
				pSelItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
			else
			{
				if (oInfo.nUse == 1)
				{
					pSelItem->SetChecked(TRUE);
				}
				else
				{
					pSelItem->SetChecked(FALSE);
				}

				pSelItem->SetEditable(bCheck);
				pSelItem->SetBackgroundColor(RGB(255,255,255));
			}

			//mode name
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oInfo.pMode->ModeName()));
			pItem->SetItemData((DWORD_PTR)oInfo.pMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			
			//matrix
			pItemCombo		 = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
			const KIOColumns* pColumns	= pTable->Columns();

			int nIndex       = 0;
			int nCurrentIndex   = 0;
			for (size_t i = 0; i < nColumnCount; i++)
			{
				CString strColunmName = vecModeColumn[i];
				KIOColumn*    pColumn = pColumns->GetColumn(strColunmName);
				nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);

				CString strCompareColumnName = pColumn->Caption();
				if(pColumn == oInfo.pColumn)
				{
					nCurrentIndex = nIndex;
				}
			}
			pItemCombo->SetCurSel(nCurrentIndex);
			pItemCombo->SetAlignment(xtpColumnTextCenter);
			
			if (pSelItem->IsChecked() == TRUE)
			{
				pItemCombo->SetEditable(TRUE);
				pItemCombo->SetBackgroundColor(RGB(255,255,255));
			}
			else
			{
				pItemCombo->SetEditable(FALSE);
				pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}

			//Occupancy
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dOccupancy, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);

			if (pSelItem->IsChecked() == TRUE)
			{
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);
			}
			else
			{
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}
			
			//PCE
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.dPCE, _T("%.3f")));
			pItem->SetAlignment(xtpColumnTextCenter);

			if (pSelItem->IsChecked() == TRUE)
			{
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);
			}
			else
			{
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}
			
			++iter;
		}

		m_wndModeReport.Populate();
		m_wndModeReport.ExpandAll(TRUE);

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

void KInterHighwayRunOption::OnReportCheckItemMode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if(nIndex == 0)
		{
			if(TRUE == pItemNotify->pRow->GetRecord()->GetItem(0)->IsChecked())
			{
				pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(RGB(255,255,255));
				pItemNotify->pRow->GetRecord()->GetItem(3)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(3)->SetBackgroundColor(RGB(255,255,255));
				pItemNotify->pRow->GetRecord()->GetItem(4)->SetEditable(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(4)->SetBackgroundColor(RGB(255,255,255));
			}
			else
			{
				pItemNotify->pRow->GetRecord()->GetItem(2)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(2)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItemNotify->pRow->GetRecord()->GetItem(3)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(3)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItemNotify->pRow->GetRecord()->GetItem(4)->SetEditable(FALSE);
				pItemNotify->pRow->GetRecord()->GetItem(4)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			}
		}
		m_wndModeReport.RedrawControl();
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

void KInterHighwayRunOption::SaveFInitialVolume( void )
{
	try
	{
		std::map<int, KInterOccupancePCEMode> mapOPMode;

		CXTPReportRecords* pRecords	= m_wndModeReport.GetRecords();
		int nRecordCount			= pRecords->GetCount();

		for(int i = 0; i < nRecordCount; i++)
		{
			KInterOccupancePCEMode oInfo;

			CXTPReportRecord*     pRecord  = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem	   = pRecord->GetItem(0);

			
			if (pRecord->GetItem(0)->IsChecked() == TRUE)
			{
				oInfo.nUse	  = 1;
			}
			else
			{
				oInfo.nUse    = 0;
			}
			
			CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)(pRecord->GetItem(1));
			KMode*					  pMode	    = (KMode*)pTextItem->GetItemData();
			
			oInfo.nModeID = pMode->ModeID();
			oInfo.pMode	  = pMode;

			CXTPReportRecordItemComboEx* pCombo  = (CXTPReportRecordItemComboEx*)(pRecord->GetItem(2));
			KIOColumn*					 pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());
			oInfo.pColumn = pColumn;

			CXTPReportRecordItemNumber* pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(3));
			oInfo.dOccupancy	= pNumber->GetValue();

			pNumber   = (CXTPReportRecordItemNumber*)(pRecord->GetItem(4));
			oInfo.dPCE	= pNumber->GetValue();

			mapOPMode.insert(std::make_pair(oInfo.nModeID, oInfo));
		}

		m_oRunOption.mapInterFOPMode = mapOPMode;

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

void KInterHighwayRunOption::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_CHECK1,					SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_STC_TRUN,					SZ_TOP_LEFT,    SZ_TOP_LEFT);	
		
		SetResize(IDC_CHECK_URBAN_BUS,			SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_STC_URBAN_BUS,			SZ_TOP_LEFT,    SZ_TOP_LEFT);	

		SetResize(IDC_CHECK_REGION_BUS,			SZ_TOP_LEFT,    SZ_TOP_LEFT);
		SetResize(IDC_RADIO1,					SZ_TOP_LEFT,    SZ_TOP_LEFT);	
		SetResize(IDC_RADIO2,					SZ_TOP_LEFT,    SZ_TOP_LEFT);	
		SetResize(IDC_CHECK_FREIGHT,			SZ_TOP_LEFT,    SZ_TOP_LEFT);	

		SetResize(IDC_REPORT,					  SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR2,				  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_RIGHT);

		SetResize(IDC_BUTTON1,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);

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


void KInterHighwayRunOption::OnBnClickedButtonDef()
{
	try
	{
		std::map<int, KInterOccupancePCEMode> mapOPMode;
		KDBaseDefaultInterRunOption::GetModeInfo(m_pTarget, mapOPMode);
		UpdateReportData(mapOPMode);
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


void KInterHighwayRunOption::OnBnClickedCheckFreight()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_FREIGHT);
	CXTPReportRecords* pRecords	= m_wndModeReport.GetRecords();
	int nRecordCount			= pRecords->GetCount();

	for(int i = 0; i < nRecordCount; i++)
	{
		CXTPReportRecord*     pRecord  = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem	   = pRecord->GetItem(0);
		pItem->SetEditable(bCheck);
	}
	m_wndModeReport.Populate();
}


void KInterHighwayRunOption::OnBnClickedCheckRegionBus()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_REGION_BUS);
	if (bCheck == FALSE)
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE); //임시로 FALSE
	}
}
