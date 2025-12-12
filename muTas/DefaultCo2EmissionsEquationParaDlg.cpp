// DefaultCo2EmissionsEquationParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultCo2EmissionsEquationParaDlg.h"
#include "afxdialogex.h"

#include "CustomReportRecordItem.h"
#include "DefineNamesDefaultPara.h"
#include "ExpressionChecker.h"
#include "CO2EmissionsEquationFnDlg.h"
#include "CO2EmissionsEquationModeMangerDlg.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"


/* define user message */
#define UM_FUNCTIONEDIT			WM_USER + 1001

// KDefaultCo2EmissionsEquationParaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultCo2EmissionsEquationParaDlg, KResizeDialogEx)

KDefaultCo2EmissionsEquationParaDlg::KDefaultCo2EmissionsEquationParaDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultCo2EmissionsEquationParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{
	m_pExpressionChecker = new KExpressionChecker;
}

KDefaultCo2EmissionsEquationParaDlg::~KDefaultCo2EmissionsEquationParaDlg()
{
	if (m_pExpressionChecker != nullptr)
	{
		delete m_pExpressionChecker;
		m_pExpressionChecker = nullptr;
	}
}

void KDefaultCo2EmissionsEquationParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KDefaultCo2EmissionsEquationParaDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedCancel)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KDefaultCo2EmissionsEquationParaDlg::OnItemButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KDefaultCo2EmissionsEquationParaDlg::OnReportCheckItem)
	ON_MESSAGE(UM_FUNCTIONEDIT, &KDefaultCo2EmissionsEquationParaDlg::OnFunctionEdit)
	ON_BN_CLICKED(IDC_BUTTON1, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedInitialize)
	ON_BN_CLICKED(IDC_BUTTON2, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedAdd)	
	ON_BN_CLICKED(IDC_BUTTON3, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BUTTON6, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedEditMode)
	ON_BN_CLICKED(IDC_BUTTON5, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_BUTTON4, &KDefaultCo2EmissionsEquationParaDlg::OnBnClickedImport)
END_MESSAGE_MAP()


// KDefaultCo2EmissionsEquationParaDlg 메시지 처리기입니다.

void KDefaultCo2EmissionsEquationParaDlg::OnItemButtonClick( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) a_pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	if (pItemNotify->pItem->GetBackgroundColor() == NOTCHANGE_COLUMN_COLOR)
		return;

	PostMessage(UM_FUNCTIONEDIT, (WPARAM)(pItemNotify->pItem));
}

void KDefaultCo2EmissionsEquationParaDlg::OnReportCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
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

LRESULT KDefaultCo2EmissionsEquationParaDlg::OnFunctionEdit( WPARAM a_wParam, LPARAM a_lParam )
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

BOOL KDefaultCo2EmissionsEquationParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		InitExpressionChecker();

		KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationInfo(m_mapDefCO2EquationInfo);
		KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationData(m_mapDefCO2EquationData);
		
		SetResizeCtrl();
		KReportCtrlSetting::Default(m_wndReport, TRUE, FALSE, FALSE);

		InitReportHeader();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultCo2EmissionsEquationParaDlg::InitExpressionChecker( void )
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

void KDefaultCo2EmissionsEquationParaDlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,   SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDOK,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON1,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON5,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON6,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
}

void KDefaultCo2EmissionsEquationParaDlg::InitReportHeader( void )
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
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행수단"));
        }

		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

		std::map<int, CString>::iterator bIter = mapData.begin();

		while (bIter != mapData.end())
		{
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
		pColumn->SetAlignment(DT_RIGHT);
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

void KDefaultCo2EmissionsEquationParaDlg::UpdateReportData( void )
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
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("편집"));
                pButton->SetSize(CSize(22, 0));
            }
            else {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Edit"));
                pButton->SetSize(CSize(22, 0));
            }			
		}

		//andover expression
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oData.strExpAndOver));
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (pButton != nullptr)
		{
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("편집"));
                pButton->SetSize(CSize(22, 0));
            }
            else {
                pButton->SetAlignment(xtpItemControlUnknown);
                pButton->SetCaption(_T("Edit"));
                pButton->SetSize(CSize(22, 0));
            }		
		}
		
		if (oData.bIsSpdGapAllSelected)
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
	}

	m_wndReport.Populate();
}

void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedOk()
{
	try
	{
		ValidateInput();
		
		ApplyReportData();

		KDBaseDefaultCo2EmissionsEquation::UpdateDefCO2Equation(m_mapDefCO2EquationInfo, m_mapDefCO2EquationData);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("저장하는 과정에서 오류가 발생하였습니다."));
		return;
	}

	AfxMessageBox(_T("적용되었습니다."));
	
	KResizeDialogEx::OnOK();
}


void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KDefaultCo2EmissionsEquationParaDlg::ValidateInput( void )
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

	std::map<int, double> mapRatioSum; //Ratio Sum Check;

	int nCount = m_wndReport.GetRecords()->GetCount();

	CXTPReportRecord*           pRecord = nullptr;
	CXTPReportRecordItem*       pItem   = nullptr;
	CXTPReportRecordItemText*   pText   = nullptr;
	CXTPReportRecordItemNumber* pNumber = nullptr;

	CString strValue;
	double  dRatio;
	int     nModeID;
	std::map<int, double>::iterator fIter;

	for (int i = 0; i < nCount; ++i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);

		nModeID = pRecord->GetItem(1)->GetItemData();
		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
		dRatio  = pNumber->GetValue();

		fIter = mapRatioSum.find(nModeID);

		if (fIter == mapRatioSum.end())
		{
			mapRatioSum.insert(std::make_pair(nModeID, dRatio));
		}
		else
		{
			fIter->second = fIter->second + dRatio;
		}

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(2);

		if (pText->GetValue().IsEmpty())
		{
			ThrowException(_T("Vehicle Name을 입력해주세요."));
		}

		pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);

		if (pNumber->GetValue() == 0)
			ThrowException(_T("PCE값은 0이 될 수 없습니다."));

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(4);

		if (pText->GetValue().IsEmpty())
		{
			ThrowException(_T("Fuel Name을 입력해주세요."));
		}
		
		pText = (CXTPReportRecordItemText*)pRecord->GetItem(8);

		if (pText->GetValue().IsEmpty())
		{
			ThrowException(_T("Under Expression을 입력해주세요."));
		}

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(8);

		if (pText->GetValue().IsEmpty() &&  pText->GetBackgroundColor() != NOTCHANGE_COLUMN_COLOR)
		{
			ThrowException(_T("AndOver Expression을 입력해주세요."));
		}
	}

	std::map<int, double>::iterator bIter = mapRatioSum.begin();
	std::map<int, CString>::iterator fNameIter;

	while (bIter != mapRatioSum.end())
	{
		if (bIter->second != 100)
		{
			nModeID = bIter->first;

			fNameIter = mapData.find(nModeID);

			if (fNameIter == mapData.end())
			{
				TxLogDebugException();
				throw 1;
			}
			else
			{
				strValue.Format(_T("수단[%s]의 합이 100이 아닙니다."), fNameIter->second);
				ThrowException(strValue);
			}
		}

		++bIter;
	}
}

void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedInitialize()
{
	KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationInfo(m_mapDefCO2EquationInfo);
	KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationData(m_mapDefCO2EquationData);

	InitReportHeader();
}

void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedAdd()
{
	try
	{
		CXTPReportRecord*            pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*        pItem   = nullptr;
		CXTPReportRecordItemControl* pButton = nullptr;

		//Select
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		//Mode
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(1);

		//Vehicle
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

		//fuel
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

		//pce
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

		//ratio
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));

		//speed gap
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));

		//use all exp
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		//under expression
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (pButton != nullptr)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("편집"));
				pButton->SetSize(CSize(22, 0));
			}
			else {
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("Edit"));
				pButton->SetSize(CSize(22, 0));
			}	
		}

		//andover expression
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);

		if (pButton != nullptr)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("편집"));
				pButton->SetSize(CSize(22, 0));
			}
			else {
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("Edit"));
				pButton->SetSize(CSize(22, 0));
			}	
		}

		m_wndReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("레코드 추가에 실패했습니다."));
	}
}



void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedDel()
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	int nRecord = m_wndReport.GetRecords()->GetCount();

	for (int i = nRecord - 1; i >= 0; --i)
	{
		pRecord = m_wndReport.GetRecords()->GetAt(i);
		pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
		{
			pRecord->Delete();
		}
	}

	m_wndReport.Populate();
}


void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedEditMode()
{
	//Get Used ModeId
	std::set<int> setUsedModeID;

	CCustomReportRecordItem* pCustom = nullptr;
	
	int nRecord = m_wndReport.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pCustom = (CCustomReportRecordItem*)m_wndReport.GetRecords()->GetAt(i)->GetItem(1);
		setUsedModeID.insert(pCustom->GetItemData());
	}

	//Get Mode Info;
	bool bUserCode;
	std::map<int, std::map<int, CString>>::iterator fSysIter  = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, CString>>::iterator fUserIter = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, CString> mapModeData;

	if (fUserIter != m_mapDefCO2EquationInfo.end())
	{
		mapModeData = fUserIter->second;
		bUserCode   = true;
	}
	else if (fSysIter !=  m_mapDefCO2EquationInfo.end())
	{
		mapModeData = fSysIter->second;
		bUserCode   = false;
	}
	
	KCO2EmissionsEquationModeMangerDlg oDlg;
	oDlg.SetUsedMode(setUsedModeID);
	oDlg.SetModeMap(mapModeData);

	if (oDlg.DoModal() != IDOK)
		return;

	mapModeData = oDlg.GetModeMap();

	//UpdateModeData
	if (bUserCode)
	{
		m_mapDefCO2EquationInfo.erase(KDefaultParameterMasterCode::USER_MASETER_CODE);
		m_mapDefCO2EquationInfo.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, mapModeData));
	}
	else
	{
		m_mapDefCO2EquationInfo.erase(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		m_mapDefCO2EquationInfo.insert(std::make_pair(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, mapModeData));
	}

	ApplyReportData();
	InitReportHeader();
}

void KDefaultCo2EmissionsEquationParaDlg::ApplyReportData( void )
{
	std::vector<KCO2EquationData> vecEquationData;

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	int nRecord = m_wndReport.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		KCO2EquationData oData;

		oData.nMasterCD = KDefaultParameterMasterCode::USER_MASETER_CODE;

		pRecord = m_wndReport.GetRecords()->GetAt(i);

		//Select
		pItem = pRecord->GetItem(0);
		oData.bIsSelcted = pItem->IsChecked() ? TRUE : FALSE;

		//Mode
		oData.nModeID = pRecord->GetItem(1)->GetItemData();

		//Vehicle
		oData.strNameVeh = ((CXTPReportRecordItemText*)pRecord->GetItem(2))->GetValue();

		//PCE
		oData.dPCE = ((CXTPReportRecordItemNumber*)pRecord->GetItem(3))->GetValue();

		//Fuel
		oData.strNameFuel = ((CXTPReportRecordItemText*)pRecord->GetItem(4))->GetValue();

		//Ratio
		oData.dRatio = ((CXTPReportRecordItemNumber*)pRecord->GetItem(5))->GetValue();

		//Speed Gap
		oData.dSpdGap = ((CXTPReportRecordItemNumber*)pRecord->GetItem(6))->GetValue();

		//공통
		pItem = pRecord->GetItem(7);
		oData.bIsSpdGapAllSelected = pItem->IsChecked() ? TRUE : FALSE;

		//under exp
		oData.strExpUnder = ((CXTPReportRecordItemText*)pRecord->GetItem(8))->GetValue();

		//andover exp
		oData.strExpAndOver = ((CXTPReportRecordItemText*)pRecord->GetItem(9))->GetValue();

		vecEquationData.push_back(oData);
	}

	m_mapDefCO2EquationData.erase(KDefaultParameterMasterCode::USER_MASETER_CODE);
	m_mapDefCO2EquationData.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, vecEquationData));
}


void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Equation Parameter");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strExportFile = fileDlg.GetPathName();
		strExportFile.Replace(_T(".txt"), _T(""));
		m_strFile.Format(_T("%s.txt"), strExportFile);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			AfxMessageBox(m_strErrMsg);
			return;
		}

		AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
	}
}

unsigned __stdcall KDefaultCo2EmissionsEquationParaDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefaultCo2EmissionsEquationParaDlg* pDlg = (KDefaultCo2EmissionsEquationParaDlg*)pParameter->GetParameter();
	pDlg->ExportData();

	return 0;
}

void KDefaultCo2EmissionsEquationParaDlg::ExportData( void )
{
	m_strErrMsg = _T("");

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

	std::map<int, CString>::iterator fIter;

	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strFile, nOpenMode);
	CString      strLine;

	try
	{
		CXTPReportRecord*           pRecord = nullptr;
		CXTPReportRecordItem*       pItem   = nullptr;
		CXTPReportRecordItemText*   pText   = nullptr;
		CXTPReportRecordItemNumber* pNumber = nullptr;

		int nModeID, nSelected, nAllExpSelected;
		double dPCE, dRatio, dSpdGap;
		CString strVeh, strModeName, strFuel, strUnderExp, strAndOverExp;

		int nRecord = m_wndReport.GetRecords()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pRecord = m_wndReport.GetRecords()->GetAt(i);

			//Select
			nSelected = pRecord->GetItem(0)->IsChecked() ? 1 : 0;

			//Mode
			nModeID = pRecord->GetItem(1)->GetItemData();;

			fIter = mapData.find(nModeID);

			if (fIter == mapData.end())
				ThrowException(_T("일치하는 수단이 없습니다."));

			strModeName = fIter->second;

			//Veh
			pText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			strVeh = pText->GetValue();

			//pce
			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			dPCE = pNumber->GetValue();

			//fuel
			pText = (CXTPReportRecordItemText*)pRecord->GetItem(4);
			strFuel = pText->GetValue();

			//ratio
			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(5);
			dRatio = pNumber->GetValue();

			//speed gap
			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(6);
			dSpdGap = pNumber->GetValue();

			//use all exp
			nAllExpSelected = pRecord->GetItem(7)->IsChecked() ? 1 : 0;

			//under exp
			pText = (CXTPReportRecordItemText*)pRecord->GetItem(8);
			strUnderExp = pText->GetValue();

			//andover exp
			pText = (CXTPReportRecordItemText*)pRecord->GetItem(9);
			strAndOverExp = pText->GetValue();

			strLine = _T("");
			strLine.AppendFormat(_T("%d, '%s', '%s', %f, '%s', "), nSelected, strModeName, strVeh, dPCE, strFuel);
			strLine.AppendFormat(_T("%f, %f, %d, '%s', '%s'"), dRatio, dSpdGap, nAllExpSelected, strUnderExp, strAndOverExp);
			strLine.Append(_T("\r\n"));

			outFile.WriteString(strLine);
		}

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}


void KDefaultCo2EmissionsEquationParaDlg::OnBnClickedImport()
{
	try
	{
		TCHAR       szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		CFileDialog filedlg( TRUE, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter );

		KScenario* pScenario   = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject    = (KProject*)pScenario->GetParentObject();
		CString    strLocation = pProject->GetLocation();

		if (!strLocation.IsEmpty())
			filedlg.m_ofn.lpstrInitialDir = strLocation;

		if( filedlg.DoModal() != IDOK )
			return;

		m_strFile = filedlg.GetPathName();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ImportThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			AfxMessageBox(m_strErrMsg);
			return;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 내보내기에 실패하였습니다."));
		return;
	}

	AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));
}

unsigned __stdcall KDefaultCo2EmissionsEquationParaDlg::ImportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KDefaultCo2EmissionsEquationParaDlg* pDlg = (KDefaultCo2EmissionsEquationParaDlg*)pParameter->GetParameter();
	pDlg->ImportData();

	return 0;
}

void KDefaultCo2EmissionsEquationParaDlg::ImportData( void )
{
	m_strErrMsg = _T("");
	std::vector<CSVRow> vecRow;

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

	std::map<int, CString>::iterator bIter;

	UINT         nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT         nCodePage = 949;
	CStdioFileEx csvFile;

	CString strLine, strTemp;

	try
	{
		csvFile.SetCodePage(nCodePage);

		if (csvFile.Open(m_strFile, nOpenMode) == FALSE)
			ThrowException(_T("파일 열기를 실패했습니다."));

		int nLine = 0;
		
		int     nValue;
		CString strValue;
		double  dValue;
		
		std::vector<KCO2EquationData> vecImportData;

		while(csvFile.ReadString(strLine))
		{
			++nLine;

			CSVRow row;
			strLine.Trim();

			if (strLine.IsEmpty())
				continue;

			ParseCSVLineString(strLine, row);

			if (row.size() != 10)
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			int nSize = row.size();

			for (int i = 0; i < nSize; ++i)
			{
				row[i].Trim();
			}

			KCO2EquationData oData;

			if (!QBicStringChecker::IsSignedInteger(row[0]))
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			nValue = _ttoi(row[0]);

			oData.bIsSelcted = nValue == 1 ? TRUE : FALSE;

			//Mode;
			strValue = row[1];
			strValue.Trim().Remove(_T('\''));

 			bIter = mapData.begin();
			oData.nModeID = 0;

			while (bIter != mapData.end())
			{
				if (strValue.CompareNoCase(bIter->second) == 0)
				{
					oData.nModeID = bIter->first;
					break;
				}

				++bIter;
			}

			if (oData.nModeID == 0)
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			//Vehcle
			strValue = row[2];
			strValue.Trim().Remove(_T('\''));
			oData.strNameVeh = strValue;
			
			//PCE
			if (!QBicStringChecker::IsNumeric(row[3]))
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			dValue = _ttof(row[3]);
			oData.dPCE = dValue;

			//Fuel
			strValue = row[4];
			strValue.Trim().Remove(_T('\''));
			oData.strNameFuel = strValue;

			//Ratio
			if (!QBicStringChecker::IsNumeric(row[5]))
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			dValue = _ttof(row[5]);
			oData.dRatio = dValue;

			//speedgap
			if (!QBicStringChecker::IsNumeric(row[6]))
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			dValue = _ttof(row[6]);
			oData.dSpdGap = dValue;

			//use all exp
			if (!QBicStringChecker::IsSignedInteger(row[7]))
			{
				strTemp.Format(_T("Line : %d - 데이터 형식이 잘못되었습니다."), nLine);
				ThrowException(strTemp);
			}

			nValue = _ttoi(row[7]);

			oData.bIsSpdGapAllSelected = nValue == 1 ? TRUE : FALSE;

			//exp under
			strValue = row[8];
			strValue.Trim().Remove(_T('\''));
			oData.strExpUnder = strValue;

			//exp andover
			strValue = row[9];
			strValue.Trim().Remove(_T('\''));
			oData.strExpAndOver = strValue;

			vecImportData.push_back(oData);
		}

		csvFile.Close();

		m_mapDefCO2EquationData.clear();
		m_mapDefCO2EquationData.insert(std::make_pair(KDefaultParameterMasterCode::USER_MASETER_CODE, vecImportData));

		UpdateReportData();
	}
	catch (KExceptionPtr ex)
	{
		csvFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		csvFile.Close();
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

void KDefaultCo2EmissionsEquationParaDlg::ParseCSVLineString( CString strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, ',' ) == TRUE )
	{
		if( true == bQuote )
		{
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
			{
				strQuote.Remove( _T('"') );
				strRow = strQuote;
				bQuote = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
			}
			else if( strRow.GetLength() == 1 )
			{
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') )
				{
					bQuote = true;
				}
				else
				{
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}
