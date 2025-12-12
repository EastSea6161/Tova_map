// CO2EmissionEquation1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CO2EmissionEquation1Dlg.h"
#include "afxdialogex.h"

#include "DBaseCO2EmissionEquation.h"
#include "DefineNamesDefaultPara.h"
#include "CustomReportRecordItem.h"
#include "Target.h"

#include "ParseExpression.h"

// KCO2EmissionEquation1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCO2EmissionEquation1Dlg, KResizeDialogEx)

KCO2EmissionEquation1Dlg::KCO2EmissionEquation1Dlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCO2EmissionEquation1Dlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bBackStep(false)
{

}

KCO2EmissionEquation1Dlg::~KCO2EmissionEquation1Dlg()
{
}

void KCO2EmissionEquation1Dlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboTravelTime);
	DDX_Control(pDX, IDC_REPORT, m_wndModeReport);
}


BEGIN_MESSAGE_MAP(KCO2EmissionEquation1Dlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,        &KCO2EmissionEquation1Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,    &KCO2EmissionEquation1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KCO2EmissionEquation1Dlg::OnBnClickedBack)
END_MESSAGE_MAP()


// KCO2EmissionEquation1Dlg 메시지 처리기입니다.


void KCO2EmissionEquation1Dlg::InitComboCtrl( void )
{
	m_cboTravelTime.ResetContent();

	try
	{
		/* 2016.07.28 명지대 요청으로 assign 결과 average_time 필드만을 삽입하는게 아니라, Volume 처럼 link테이블에서 double 형인 필드만 삽입한다.
		std::map<int, CString>::iterator bIter = m_mapTravelTime.begin();

		if (m_mapTravelTime.size() == 0)
			ThrowException(_T("데이터가 없습니다."));

		while (bIter != m_mapTravelTime.end())
		{
			nIndex = m_cboTravelTime.InsertString(-1, bIter->second);
			m_cboTravelTime.SetItemData(nIndex, bIter->first);

			++bIter;
		}

		m_cboTravelTime.SetCurSel(0);
		*/

		CString strInitColumnCaption(_T(""));
		if (m_mapTravelTime.size() > 0) {
			auto iterBegin = m_mapTravelTime.begin();
			strInitColumnCaption = iterBegin->second;
		}

		KIOTables*  pIOTables  = m_pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		KIOColumn*  pIOColumn  = nullptr;

		int nInitCur(0);
		int nColumn = pIOColumns->ColumnCount();
		for (int i = 0; i < nColumn; ++i)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->DataType() != KEMIODataTypeDouble)
				continue;

			CString strCaption = pIOColumn->Caption();
			int nIndex = m_cboTravelTime.AddString(strCaption);
			m_cboTravelTime.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			if (strInitColumnCaption.CompareNoCase(strCaption) == 0) {
				nInitCur = i;
			}
		}

		if (m_cboTravelTime.GetCount() > 0) {
			m_cboTravelTime.SetCurSel(nInitCur);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("Travel Time Combo Control 초기화에 실패했습니다."));
	}	
}

void KCO2EmissionEquation1Dlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_SAVE, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT1,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDOK,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,        SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON1,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
}

BOOL KCO2EmissionEquation1Dlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KDBaseCO2EmissionEquation::GetTravelTime(m_pTarget, m_mapTravelTime);
	KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationInfo(m_mapDefCO2EquationInfo);

	KReportCtrlSetting::Default(m_wndModeReport, TRUE, FALSE, FALSE);

	InitComboCtrl();
	SetResizeCtrl();

	InitModeReportHeader(); // InitComboCtrl(); 이후 실행 되야 함

	SetDlgItemText(IDC_EDIT1, _T("CO2_Emission(g)"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCO2EmissionEquation1Dlg::OnBnClickedOk()
{
	try
	{
		if (m_cboTravelTime.GetCurSel() < 0) {
			AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
			GetDlgItem(IDC_COMBO1)->SetFocus();
			return;
		}

		CXTPReportRecordItem* pItem = nullptr;

		int  nRecord    = m_wndModeReport.GetRecords()->GetCount();
		bool bIsChecked = false;

		for (int i = 0; i < nRecord; ++i)
		{
			pItem = m_wndModeReport.GetRecords()->GetAt(i)->GetItem(0);

			if (pItem->IsChecked())
			{
				bIsChecked = true;
				break;
			}
		}

		if (!bIsChecked)
		{
			AfxMessageBox(_T("선택한 항목이 없습니다."));
			return;
		}

		GetDlgItemText(IDC_EDIT1, m_strSaveColumn);

		m_strSaveColumn.Trim();

		if (m_strSaveColumn.IsEmpty())
		{
			AfxMessageBox(_T("저장할 컬럼명을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();

			return;
		}

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang);

		if (!m_strErrMsg.IsEmpty())
		{
			AfxMessageBox(m_strErrMsg);
			return;
		}
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
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	AfxMessageBox(_T("완료되었습니다."));
	KResizeDialogEx::OnOK();
}


void KCO2EmissionEquation1Dlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KCO2EmissionEquation1Dlg::OnBnClickedBack()
{
	m_bBackStep = true;

	KResizeDialogEx::OnOK();
}

void KCO2EmissionEquation1Dlg::ConvertColNameToDispName( std::vector<CString>& a_vecModeVolColumn )
{
	KIOTables*  pIOTables  = m_pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
	KIOColumns* pIOColumns = pIOTable->Columns();
	KIOColumn*  pIOColumn  = nullptr;

	int nSize = a_vecModeVolColumn.size();

	for (int i = 0; i < nSize; ++i)
	{
		CString strColumnName = a_vecModeVolColumn[i];

		pIOColumn = pIOColumns->GetColumn(strColumnName);

		if (pIOColumn == nullptr)
			ThrowException(_T("일치하는 컬럼이 없습니다."));

		a_vecModeVolColumn[i] = pIOColumn->Caption();
	}
}

void KCO2EmissionEquation1Dlg::InitModeReportHeader( void )
{
	m_wndModeReport.GetColumns()->Clear();

	int  nItemIndex = 0;
	bool bAutoSize  = TRUE;

	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select"), 50, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Mode"), 100, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행수단"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Link Volume"), 100, bAutoSize));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("링크교통량 데이터 컬럼"));
    }

	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();

	KIOTables*  pIOTables  = m_pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
	KIOColumns* pIOColumns = pIOTable->Columns();
	KIOColumn*  pIOColumn  = nullptr;

	int nColumn = pIOColumns->ColumnCount();

	for (int i = 0; i < nColumn; ++i)
	{
		pIOColumn = pIOColumns->GetColumn(i);

		if (pIOColumn->DataType() != KEMIODataTypeDouble)
			continue;

		pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
	}

	UpdateModeReportData();
}

void KCO2EmissionEquation1Dlg::UpdateModeReportData( void )
{
	m_wndModeReport.ResetContent();

	std::map<int, std::map<int, CString>>::iterator fSysIter  = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, CString>>::iterator fUserIter = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, CString> mapData;

	if (fUserIter != m_mapDefCO2EquationInfo.end())
	{
		mapData = fUserIter->second;
	}
	else
	{
		mapData = fSysIter->second;
	}

	CXTPReportRecord* pRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	std::set<int>::iterator fSetModeIter;
	
	std::map<int, CString>::iterator bIter = mapData.begin();

	DWORD_PTR dwDefault = m_wndModeReport.GetColumns()->GetAt(2)->GetEditOptions()->GetConstraints()->GetAt(0)->m_dwData;

	while (bIter != mapData.end())
	{
		fSetModeIter = m_setModeID.find(bIter->first);

		if (fSetModeIter == m_setModeID.end())
		{
			++bIter;
			continue;
		}

		pRecord = m_wndModeReport.AddRecord(new CXTPReportRecord);

		//Select
		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);

		//mode
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(bIter->second));
		pItem->SetItemData(bIter->first);

		//link_volume
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(dwDefault);

		++bIter;
	}

	m_wndModeReport.Populate();
}

unsigned __stdcall KCO2EmissionEquation1Dlg::ThreadCaller( void* p )
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("탄소 배출량 산정(Tier 3 기준) 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run CO2 Emission(Tier 3) Module..."));
	}

	KCO2EmissionEquation1Dlg* pDlg = (KCO2EmissionEquation1Dlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KCO2EmissionEquation1Dlg::Run( void )
{
	m_strErrMsg = _T("");

	std::vector<CString> vecUsedColumnName;
	vecUsedColumnName.push_back(_T("speed"));

	std::map<int, std::map<int, CString>>::iterator fSysIter  = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, CString>>::iterator fUserIter = m_mapDefCO2EquationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, CString> mapData;

	if (fUserIter != m_mapDefCO2EquationInfo.end())
	{
		mapData = fUserIter->second;
	}
	else
	{
		mapData = fSysIter->second;
	}

	std::map<int, CString>::iterator fModeIter;

	std::map<int, std::vector<KCO2EquationData>>::iterator fSysDataIter  = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::vector<KCO2EquationData>>::iterator fUserDataIter = m_mapDefCO2EquationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::vector<KCO2EquationData> vecData;

	if (fUserDataIter != m_mapDefCO2EquationData.end())
	{
		vecData = fUserDataIter->second;
	}
	else if (fSysDataIter != m_mapDefCO2EquationData.end())
	{
		vecData = fSysDataIter->second;
	}

	try
	{
		KIOTables*   pIOTables  = m_pTarget->Tables();
		KIOTable*    pIOTable   = pIOTables->FindTable(TABLE_LINK);

		std::map<Integer, double> mapResult;
		std::map<Integer, double> mapLinkSpeed;  //링크별속도
		std::map<Integer, double> mapLinkVolume; //링크Volume
		// 2019.01.17 : 링크 g/km를 g으로 수정
		std::map<Integer, double> mapLinkLength;	// 링크 길이


		std::map<Integer, double>::iterator fResultIter;

		KIOColumn* pTimeIOColumn = (KIOColumn*)m_cboTravelTime.GetItemData(m_cboTravelTime.GetCurSel());
		KDBaseCO2EmissionEquation::GetLinkSpeed(m_pTarget, pTimeIOColumn, mapLinkSpeed);
		KDBaseCO2EmissionEquation::GetLinkLength(m_pTarget, mapLinkLength);


		CXTPReportRecord*           pRecord = nullptr;
		CXTPReportRecordItem*       pItem   = nullptr;
		CXTPReportRecordItemNumber* pNumber = nullptr;
		CXTPReportRecordItemText*   pText   = nullptr;

		CString strEXPUnder, strEXPAndOver, strModeName, strColumnName;
		double  dSpdGap, dRatio;
		int     nModeID;
		
		//int nRecord = m_wndReport.GetRecords()->GetCount();

		int nRecord = vecData.size();
		for (int i = 0; i < nRecord; ++i)
		{
			KCO2EquationData oData = vecData[i];

			if (!oData.bIsSelcted)
				continue;

			nModeID = oData.nModeID;

			if (!IsSelectedMode(nModeID))
				continue;
			

			fModeIter = mapData.find(nModeID);

			if (fModeIter == mapData.end())
			{
				AfxMessageBox(_T("저장할 컬럼명을 입력해 주세요."));
			}

			strModeName = fModeIter->second;
			GetLinkVolumeColumn(strModeName, strColumnName);
			KDBaseCO2EmissionEquation::GetLinkVolume(m_pTarget, strColumnName, mapLinkVolume);

			dRatio  = oData.dRatio;

			dSpdGap = oData.dSpdGap;

			if (oData.bIsSpdGapAllSelected)
			{
				strEXPUnder   = oData.strExpUnder;
				strEXPAndOver = strEXPUnder;
			}
			else
			{
				strEXPUnder   = oData.strExpUnder;
				strEXPAndOver = oData.strExpAndOver;
			}
			std::map<Integer, double>::iterator fVolIter;
			std::map<Integer, double>::iterator bSpdIter = mapLinkSpeed.begin();
			
			// 2019.01.17
			std::map<Integer, double>::iterator itLength;

			while (bSpdIter != mapLinkSpeed.end())
			{
				KParseExpression oParseExpression;
				oParseExpression.SetTable(pIOTable);

				if (bSpdIter->second < dSpdGap)	//미만일시
				{
					oParseExpression.SetCaptionExpression(strEXPUnder);
				}
				else //이상일시
				{
					oParseExpression.SetCaptionExpression(strEXPAndOver);
				}

				CString strRealFn = oParseExpression.GetRealExpression();
				int     nVecField = vecUsedColumnName.size();

				KParser oParser;
				oParser.SetExpr(strRealFn);
				double* dValueArray = new double[1];

				oParser.DefineVar(vecUsedColumnName[0], &dValueArray[0]);
				dValueArray[0] = bSpdIter->second;

				double dResult = oParser.Eval();
				double dVolume = 0.0;
				double dLenght = 0.0;

				fVolIter = mapLinkVolume.find(bSpdIter->first);
				if (fVolIter != mapLinkVolume.end())
				{
					dVolume = fVolIter->second;
				}

				itLength = mapLinkLength.find(bSpdIter->first);
				if (itLength != mapLinkLength.end())
				{
					dLenght = itLength->second;
				}

				//dResult = dResult * dRatio / 100 * fVolIter->second / oData.dPCE;
				dResult = dResult * dRatio / 100 * dVolume / oData.dPCE * dLenght;



				fResultIter = mapResult.find(bSpdIter->first);

				if (fResultIter == mapResult.end())
				{
					mapResult.insert(std::make_pair(bSpdIter->first, dResult));
				}
				else
				{
					double dMapResult = fResultIter->second;
					dResult = dResult + dMapResult;

					mapResult.erase(bSpdIter->first);
					mapResult.insert(std::make_pair(bSpdIter->first, dResult));
				}

				++bSpdIter;
			}
		}

		//Add Column
		bool bDuplColumn = false;

		//KIOTable*   pIOTable   = m_pTarget->Tables()->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		KIOColumn*  pIOColumn  = nullptr;

		int nColumn = pIOColumns->ColumnCount();

		for (int i = 0; i < nColumn; ++i)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			if (m_strSaveColumn.CompareNoCase(pIOColumn->Caption()) == 0)
			{
				bDuplColumn = true;
				break;
			}
		}

		//신규 컬럼생성
		if (!bDuplColumn)
		{
			pIOColumn = CreateResultColumn(m_strSaveColumn, pIOTable, m_strSaveColumn);
		}

		KDBaseCO2EmissionEquation::UpdateResult(m_pTarget, pIOColumn->Name(), mapResult);

		pIOTable->Notify();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_strErrMsg = ex->GetErrorMessage();
	}
	catch (...)
	{
		TxLogDebugException();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
	}
}

KIOColumn* KCO2EmissionEquation1Dlg::CreateResultColumn( CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription )
{
	const KIOColumns* pColumCollection = a_pTable->Columns();

	int nMaxDisplayCount(0);
	int nCount = pColumCollection->ColumnCount();

	if (0 == nCount)
	{
		nMaxDisplayCount = 1;
	}
	else
	{
		for (int i = 0; i < nCount; i++)
		{
			KIOColumn* pColumn = pColumCollection->GetColumn(i);
			int nDisplayCount = pColumn->DisplayOrder();
			if (nMaxDisplayCount < nDisplayCount)
			{
				nMaxDisplayCount = nDisplayCount;
			}
		}
	}

	KIOColumn oColumn;
	oColumn.Name(a_pTable->GetUsableColumnName());
	oColumn.Caption(a_strColumnName);
	oColumn.CodeType(KEMIOCodeTypeIsNothing);
	oColumn.ColumnType(KEMIOColumnTypeUserDefine);
	oColumn.DataType(KEMIODataTypeDouble);
	oColumn.DisplayOrder(nMaxDisplayCount+1);
	oColumn.ColumnInfo(a_strDescription);
	oColumn.PrimaryKey(false);
	oColumn.Visible(true);

	KIOColumn* pColumn = a_pTable->AddColumn(oColumn);
	return pColumn;
}

void KCO2EmissionEquation1Dlg::GetLinkVolumeColumn( CString a_strModeName, CString& a_strColumnName )
{
	a_strColumnName = _T("");

	CXTPReportRecord*         pRecord = nullptr;
	CXTPReportRecordItem*     pItem   = nullptr;
	CXTPReportRecordItemText* pText   = nullptr;

	KIOColumn* pIOColumn = nullptr;
	int        nCount    = m_wndModeReport.GetRecords()->GetCount();

	for (int i = 0; i < nCount; ++i)
	{
		pRecord = m_wndModeReport.GetRecords()->GetAt(i);

		pItem = pRecord->GetItem(0);

		if (!pItem->IsChecked())
			continue;

		pText = (CXTPReportRecordItemText*)pRecord->GetItem(1);

		if (a_strModeName.CompareNoCase(pText->GetValue()) != 0)
			continue;

		//Mode명칭이 같을경우
		pIOColumn = (KIOColumn*)pRecord->GetItem(2)->GetItemData();
		a_strColumnName = pIOColumn->Name();
		break;
	}

	if (a_strColumnName.IsEmpty())
	{
		AfxMessageBox(_T("통행수단 매칭에 실패하였습니다."));
	}
}

bool KCO2EmissionEquation1Dlg::IsSelectedMode( int a_nModeId )
{
	bool bSelected = false;

	CXTPReportRecord* pRecord = nullptr;

	int nRecord = m_wndModeReport.GetRecords()->GetCount();
	int nModeID;

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = m_wndModeReport.GetRecords()->GetAt(i);

		if (!pRecord->GetItem(0)->IsChecked())
			continue;
		
		nModeID = pRecord->GetItem(1)->GetItemData();

		if (nModeID == a_nModeId)
		{
			bSelected = true;
			break;
		}
	}

	return bSelected;
}
