// CO2EmissionEquationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CO2EmissionEquationDlg.h"
#include "afxdialogex.h"

#include "DBaseCO2EmissionEquation.h"
#include "DefineNamesDefaultPara.h"
#include "CustomReportRecordItem.h"
#include "Target.h"
#include "ParseExpression.h"

#include "ExpressionChecker.h"
#include "CO2EmissionsEquationFnDlg.h"

/* define user message */
#define UM_FUNCTIONEDIT			WM_USER + 1001

// KCO2EmissionEquationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCO2EmissionEquationDlg, KResizeDialogEx)

KCO2EmissionEquationDlg::KCO2EmissionEquationDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCO2EmissionEquationDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_pExpressionChecker = new KExpressionChecker;
}

KCO2EmissionEquationDlg::~KCO2EmissionEquationDlg()
{
	if (m_pExpressionChecker != nullptr)
	{
		delete m_pExpressionChecker;
		m_pExpressionChecker = nullptr;
	}
}

void KCO2EmissionEquationDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReport);
}


BEGIN_MESSAGE_MAP(KCO2EmissionEquationDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,     &KCO2EmissionEquationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KCO2EmissionEquationDlg::OnBnClickedCancel)
	
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,         IDC_REPORT, &KCO2EmissionEquationDlg::OnReportCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KCO2EmissionEquationDlg::OnItemButtonClick)

	ON_MESSAGE(UM_FUNCTIONEDIT, &KCO2EmissionEquationDlg::OnFunctionEdit)

END_MESSAGE_MAP()


void KCO2EmissionEquationDlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

void KCO2EmissionEquationDlg::InitExpressionChecker( void )
{
	m_pExpressionChecker->AddField(_T("Speed"));

	m_pExpressionChecker->AddFunction(_T("SQRT"));
	m_pExpressionChecker->AddFunction(_T("EXP"));
	m_pExpressionChecker->AddFunction(_T("LN"));
	m_pExpressionChecker->AddFunction(_T("LOG"));
	m_pExpressionChecker->AddFunction(_T("ABS"));

	m_pExpressionChecker->AddOperator(_T("+"));
	m_pExpressionChecker->AddOperator(_T("-"));
	m_pExpressionChecker->AddOperator(_T("*"));
	m_pExpressionChecker->AddOperator(_T("/"));
	m_pExpressionChecker->AddOperator(_T("^"));
	m_pExpressionChecker->AddOperator(_T("("));
	m_pExpressionChecker->AddOperator(_T(")"));
}

// KCO2EmissionEquationDlg 메시지 처리기입니다.

BOOL KCO2EmissionEquationDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		InitExpressionChecker();

		if (KDBaseCO2EmissionEquation::IsEmptyAssignResult(m_pTarget)) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("통행배정 수행 후 실행가능한 메뉴입니다."));
			}
			else {
				ThrowException(_T("You must execute Traffic Assignment first."));
			}
		}
			//ThrowException(_T("Assignment 수행 후 실행가능한 메뉴입니다."));

		KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationInfo(m_mapDefCO2EquationInfo);

		if (m_mapDefCO2EquationData.size() == 0)
			KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationData(m_mapDefCO2EquationData);

		SetResizeCtrl();
		KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE, FALSE);
				
		InitReportHeader();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		OnCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KCO2EmissionEquationDlg::OnBnClickedOk()
{
	try
	{
		IsValidInputReport();
		ApplyReportData();		
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		return;
	}	

	KResizeDialogEx::OnOK();
}


void KCO2EmissionEquationDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KCO2EmissionEquationDlg::InitReportHeader( void )
{
	try
	{
		std::map<int, std::map<int, CString>>::iterator fSysIter  = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		std::map<int, std::map<int, CString>>::iterator fUserIter = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

		std::map<int, CString> mapData;

		if (fUserIter != m_mapDefCO2EquationInfo.end())
		{
			mapData = fUserIter->second;
		}
		else if (fSysIter !=  m_mapDefCO2EquationInfo.end())
		{
			mapData = fSysIter->second;
		}
		else
		{
			TxLogDebugException();
			throw 1;
		}

		m_wndReport.GetColumns()->Clear();

		int  nItemIndex = 0;
		BOOL bAutoSize  = TRUE;

		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Mode"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행수단"));
        }

		std::map<int, CString>::iterator bIter = mapData.begin();
		while (bIter != mapData.end()) {
			pColumn->GetEditOptions()->AddConstraint(bIter->second, bIter->first);

			++bIter;
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Vehicle"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("차종구분"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("PCE"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("승용차 환산계수"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Fuel"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("연료종류"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Ratio(%)"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("구성비(%)"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Speed Gap"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("경계속도"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Use All Exp"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통합산출식"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Under Expression"), 150, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_LEFT);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("산출식(경계속도 미만)"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("AndOver Expression"), 150, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_LEFT);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("산출식(경계속도 이상)"));
        }

		UpdateReportData();
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("ReportCtrl 초기화에 실패했습니다."));
	}
}

void KCO2EmissionEquationDlg::UpdateReportData( void )
{
	std::map<int, std::vector<KCO2EquationData>>::iterator fSysIter  = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::vector<KCO2EquationData>>::iterator fUserIter = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::vector<KCO2EquationData> vecData;

	if (fUserIter != m_mapDefCO2EquationData.end())
	{
		vecData = fUserIter->second;
	}
	else if (fSysIter != m_mapDefCO2EquationData.end())
	{
		vecData = fSysIter->second;
	}
	else
	{
		TxLogDebugException();
		throw 1;
	}

	m_wndReport.ResetContent();

	int nSize = vecData.size();

	CXTPReportRecord*            pRecord = nullptr;
	CXTPReportRecordItem*        pItem   = nullptr;
	CXTPReportRecordItemControl* pButton = nullptr;

	for (int i = 0; i < nSize; ++i)
	{
		KCO2EquationData oData = vecData[i];

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		//Select
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(oData.bIsSelcted);

		//Mode
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(oData.nModeID);

		//Vehicle
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(oData.strNameVeh));

		//PCE
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oData.dPCE, _T("%.3f")));

		//fuel
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(oData.strNameFuel));

		//ratio
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oData.dRatio, _T("%.3f")));

		//speed gap
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oData.dSpdGap, _T("%.3f")));

		//use all exp
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(oData.bIsSpdGapAllSelected);

		//under expression
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oData.strExpUnder));
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (pButton != nullptr)
		{
			pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("편집"));
				pButton->SetSize(CSize(22, 0));
			}
			else {
				pButton->SetCaption(_T("Edit"));
				pButton->SetSize(CSize(22, 0));
			}
		}

		//andover expression
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oData.strExpAndOver));
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (pButton != nullptr)
		{
			pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("편집"));
				pButton->SetSize(CSize(22, 0));
			}
			else {
				pButton->SetCaption(_T("Edit"));
				pButton->SetSize(CSize(22, 0));
			}
		}

		if (oData.bIsSpdGapAllSelected)
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	}

	m_wndReport.Populate();
}

void KCO2EmissionEquationDlg::IsValidInputReport( void )
{
	std::map<int, std::map<int, CString>>::iterator fSysIter  = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, CString>>::iterator fUserIter = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, CString> mapData;

	if (fUserIter != m_mapDefCO2EquationInfo.end())
	{
		mapData = fUserIter->second;
	}
	else if (fSysIter !=  m_mapDefCO2EquationInfo.end())
	{
		mapData = fSysIter->second;
	}

	int nCount = m_wndReport.GetRecords()->GetCount();

	CXTPReportRecord*           pRecord = nullptr;
	CXTPReportRecordItem*       pItem   = nullptr;
	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	int nModeID;

	for (int i = 0; i < nCount; ++i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);

		nModeID = pRecord->GetItem(1)->GetItemData();

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(2);

		if (pText->GetValue().IsEmpty()) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("차종구분을 입력해 주세요."));
			}
			else {
				ThrowException(_T("Please enter the Vehicle."));
			}
		}
			//ThrowException(_T("Vehicle Name을 입력해주세요."));

		//PCE
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);

		if (pNumber->GetValue() == 0) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("승용차 환산계수는 0이 될 수 없습니다."));
			}
			else {
				ThrowException(_T("PCE value cannot be zero."));
			}
		}
			//ThrowException(_T("PCE값은 0이 될 수 없습니다."));

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(4);

		if (pText->GetValue().IsEmpty()) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("연료종류를 입력해 주세요."));
			}
			else {
				ThrowException(_T("Please enter the Fuel."));
			}
		}
			//ThrowException(_T("Fuel Name을 입력해주세요."));

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(8);

		if (pText->GetValue().IsEmpty()) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("산출식(경계속도 미만)을 입력해 주세요."));
			}
			else {
				ThrowException(_T("Please enter the Under Expression."));
			}
		}
			//ThrowException(_T("Under Expression을 입력해주세요."));

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(9);

		if (pText->GetValue().IsEmpty() && pText->GetBackgroundColor() != NOTCHANGE_COLUMN_COLOR) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("산출식(경계속도 이상)을 입력해 주세요."));
			}
			else {
				ThrowException(_T("Please enter the AndOver Expression."));
			}
		}
			//ThrowException(_T("AndOver Expression을 입력해주세요."));
	}
}

void KCO2EmissionEquationDlg::ApplyReportData( void )
{
	m_setModeID.clear();

	std::map<int, std::vector<KCO2EquationData>>::iterator fSysIter  = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::vector<KCO2EquationData>>::iterator fUserIter = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
	
	std::vector<KCO2EquationData> vecData;
	std::vector<KCO2EquationData> vecResult;
	
	int nMasterCD;

	if (fUserIter != m_mapDefCO2EquationData.end())
	{
		nMasterCD = KDefaultParameterMasterCode::USER_MASETER_CODE;
		vecData   = fUserIter->second;
	}
	else if (fSysIter != m_mapDefCO2EquationData.end())
	{
		nMasterCD = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
		vecData   = fSysIter->second;
	}

	std::map<int, double> mapRatioSum;

	std::map<int, double>::iterator fRatioSumIter, bRatioSumIter;

	CXTPReportRecord*           pRecord = nullptr;
	CXTPReportRecordItem*       pItem   = nullptr;
	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	int nRecord = m_wndReport.GetRecords()->GetCount();
	int nSize   = vecData.size();

	if (nRecord != nSize) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("데이터에 오류가 존재합니다."));
		}
		else {
			ThrowException(_T("An error exists in the data."));
		}
	}
		//ThrowException(_T("데이터에 오류가 있습니다."));
	
	for (int i = 0; i < nRecord; ++i)
	{
		KCO2EquationData oData;

		pRecord = m_wndReport.GetRecords()->GetAt(i);

		//Select
		pItem = pRecord->GetItem(0);
		oData.bIsSelcted = pItem->IsChecked() ? TRUE : FALSE;

		//Mode
		pItem = pRecord->GetItem(1);
		oData.nModeID = pItem->GetItemData();

		if (oData.bIsSelcted)
			m_setModeID.insert(oData.nModeID);

		//Vehicle
		pText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
		oData.strNameVeh = pText->GetValue();

		//PCE
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
		oData.dPCE = pNumber->GetValue();

		//Fuel
		pText = (CXTPReportRecordItemText*)pRecord->GetItem(4);
		oData.strNameFuel = pText->GetValue();

		//ratio
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
		oData.dRatio = pNumber->GetValue();

		if (oData.bIsSelcted)
		{
			fRatioSumIter = mapRatioSum.find(oData.nModeID);

			if (fRatioSumIter == mapRatioSum.end())
			{
				mapRatioSum.insert(std::make_pair(oData.nModeID, oData.dRatio));
			}
			else
			{
				double dTemp = fRatioSumIter->second;
				fRatioSumIter->second = dTemp + oData.dRatio;
			}
		}		

		//spd gap
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
		oData.dSpdGap = pNumber->GetValue();

		//Use allexp
		pItem = pRecord->GetItem(7);
		oData.bIsSpdGapAllSelected = pItem->IsChecked() ? TRUE : FALSE;

		//under expression
		pText = (CXTPReportRecordItemText*)pRecord->GetItem(8);
		oData.strExpUnder = pText->GetValue();

		//andover expession
		pText = (CXTPReportRecordItemText*)pRecord->GetItem(9);
		oData.strExpAndOver = pText->GetValue();

		vecResult.push_back(oData);
	}

	if (m_setModeID.size() == 0) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(_T("선택한 항목이 없습니다."));
		}
		else {
			ThrowException(_T("No any selected mode."));
		}
	}
		//ThrowException(_T("선택한 항목이 없습니다."));

	bRatioSumIter = mapRatioSum.begin();

	while (bRatioSumIter != mapRatioSum.end())
	{
		// 2019.01.17 : 실수와 정수 비교 수정(100.0000001과 100은 다름)
		//if (bRatioSumIter->second != 100)
		if(std::fabs(bRatioSumIter->second - 100.0) > std::numeric_limits<float>::epsilon())
		{
			//ThrowException(_T("선택한 수단의 비율 합이 100이 아닙니다."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("선택한 수단의 비율 합이 100이 아닙니다."));
			}
			else {
				ThrowException(_T("The sum of the proportions of the selected mode(s) is not 100."));
			}
		}

		++bRatioSumIter;
	}

	m_mapDefCO2EquationData.erase(nMasterCD);
	m_mapDefCO2EquationData.insert(std::make_pair(nMasterCD, vecResult));
}

void KCO2EmissionEquationDlg::OnReportCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (nIndex != 7)
		return;

	CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItem   = nullptr;

	if (pItemNotify->pItem->IsChecked())
	{
		pItem = pRecord->GetItem(9);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	}
	else
	{
		pItem = pRecord->GetItem(9);
		pItem->SetBackgroundColor(RGB(255, 255, 255));
	}
}

void KCO2EmissionEquationDlg::OnItemButtonClick( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	if (pItemNotify->pItem->GetBackgroundColor() == NOTCHANGE_COLUMN_COLOR)
		return;

	PostMessage(UM_FUNCTIONEDIT, (WPARAM)(pItemNotify->pItem));
}

LRESULT KCO2EmissionEquationDlg::OnFunctionEdit( WPARAM a_wParam, LPARAM a_lParam )
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)a_wParam;

	KCO2EmissionsEquationFnDlg dlg;
	dlg.Expression(pItem->GetCaption(NULL));
	dlg.FieldList(m_pExpressionChecker->FieldList());
	dlg.FunctionList(m_pExpressionChecker->FunctionList());
	dlg.OperatorList(m_pExpressionChecker->OperatorList());

	if(dlg.DoModal() == IDOK)
	{
		((CXTPReportRecordItemText*)pItem)->SetValue(dlg.Expression());
	}

	return 0;
}
