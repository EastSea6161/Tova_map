// CO2EmissionInterpolationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CO2EmissionInterpolationDlg.h"
#include "afxdialogex.h"

#include "CustomReportRecordItem.h"
#include "DBaseCO2EmissionInterpolation.h"
#include "DefineNamesDefaultPara.h"

//#include "IOTableManager.h"
#include "Target.h"

// KCO2EmissionInterpolationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCO2EmissionInterpolationDlg, KResizeDialogEx)

KCO2EmissionInterpolationDlg::KCO2EmissionInterpolationDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KCO2EmissionInterpolationDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KCO2EmissionInterpolationDlg::~KCO2EmissionInterpolationDlg()
{
}

void KCO2EmissionInterpolationDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_COMBO1, m_cboTravelTime);
}


BEGIN_MESSAGE_MAP(KCO2EmissionInterpolationDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,       &KCO2EmissionInterpolationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,   &KCO2EmissionInterpolationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &KCO2EmissionInterpolationDlg::OnBnClickedCheckSelectAll)

	ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KCO2EmissionInterpolationDlg::OnReportItemClick)
END_MESSAGE_MAP()


// KCO2EmissionInterpolationDlg 메시지 처리기입니다.

void KCO2EmissionInterpolationDlg::SetResizeCtrl( void )
{
	SetResize(IDC_REPORT,      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDOK,            SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,        SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_SAVE, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT1,       SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
}

BOOL KCO2EmissionInterpolationDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		SetDlgItemText(IDC_EDIT1, _T("CO2_Emission(g)"));

		SetResizeCtrl();

		if (KDBaseCO2EmissionInterpolation::IsEmptyAssignResult(m_pTarget))
			ThrowException(_T("Assignment 수행 후 실행가능한 메뉴입니다."));

		KDBaseCO2EmissionInterpolation::GetTravelTime(m_pTarget, m_mapTravelTime);
		KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationInfo(m_mapCO2InterpolationInfo);
		KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationData(m_mapCO2InterpolationData);

		KReportCtrlSetting::Default(m_wndReport, TRUE);

		InitComboCtrl();
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
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		OnCancel();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KCO2EmissionInterpolationDlg::InitComboCtrl( void )
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

void KCO2EmissionInterpolationDlg::InitReportHeader( void )
{
	try
	{
		std::map<int, KCO2InterpolationSetting>::iterator fSysIter  = m_mapCO2InterpolationInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		std::map<int, KCO2InterpolationSetting>::iterator fUserIter = m_mapCO2InterpolationInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

		if (fUserIter != m_mapCO2InterpolationInfo.end())
		{
			KCO2InterpolationSetting oSetting = fUserIter->second;

			m_nMaxSpeed = oSetting.nMaxSpeed;
			m_nInterval = oSetting.nInterval;
		}
		else if (fSysIter != m_mapCO2InterpolationInfo.end())
		{
			KCO2InterpolationSetting oSetting = fSysIter->second;

			m_nMaxSpeed = oSetting.nMaxSpeed;
			m_nInterval = oSetting.nInterval;
		}
		else
		{
			ThrowException(_T("DefaultParameter Data Error..."));
		}

		m_wndReport.GetColumns()->Clear();

		int     nItemIndex = 0;
		BOOL    bAutoSize  = FALSE;
		CString strValue;

		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Select"), 50, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Mode"), 60, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행수단"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("PCE"), 60, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
		pColumn->SetEditable(TRUE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("승용차 환산계수"));
        }

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Link Volume"), 80, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교통량 데이터 컬럼"));
        }
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();

		KIOTables*  pIOTables  = m_pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		KIOColumn*  pIOColumn  = nullptr;

		int nIOColumn = pIOColumns->ColumnCount();

		for (int i = 0; i < nIOColumn; ++i)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->DataType() != KEMIODataTypeDouble)
				continue;
			
			pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
		}

		int nColumn = (int)ceil((double)m_nMaxSpeed/m_nInterval);

		for (int i = 0; i < nColumn; ++i)
		{
			strValue.Format(_T("%d"), i * m_nInterval);
			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, strValue, 60, bAutoSize));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetAlignment(DT_RIGHT);
			pColumn->SetEditable(FALSE);
		}

		strValue.Format(_T("%d"), m_nMaxSpeed);
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nItemIndex++, strValue, 60, bAutoSize));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_RIGHT);
		pColumn->SetEditable(FALSE);

		m_wndReport.SetFreezeColumnsCount(3);

		UpdateReportData();
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("ReportCtrl 초기화에 실패했습니다."));
	}
}

void KCO2EmissionInterpolationDlg::UpdateReportData( void )
{
	m_wndReport.ResetContent();

	std::map<int, std::map<int, KCO2InterpolationData>>::iterator fSysIter  = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	std::map<int, std::map<int, KCO2InterpolationData>>::iterator fUserIter = m_mapCO2InterpolationData.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	std::map<int, KCO2InterpolationData> mapData;

	if (fUserIter != m_mapCO2InterpolationData.end())
	{
		mapData = fUserIter->second;
	}
	else if (fSysIter != m_mapCO2InterpolationData.end())
	{
		mapData = fSysIter->second;
	}

	std::map<int, KCO2InterpolationData>::iterator bIter = mapData.begin();

	int nColumn = m_wndReport.GetColumns()->GetCount();

	CXTPReportRecord*         pRecord   = nullptr;
	CXTPReportRecordItem*     pItem     = nullptr;
	CXTPReportRecordItemText* pItemText = nullptr;

	DWORD_PTR dwDefault   = m_wndReport.GetColumns()->GetAt(3)->GetEditOptions()->GetConstraints()->GetAt(0)->m_dwData;
	bool      bAllChecked = true;

	while (bIter != mapData.end())
	{
		KCO2InterpolationData oData = bIter->second;

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(oData.bIsSelected);
		pItem->SetItemData(bIter->first);

		if (bAllChecked && oData.bIsSelected == FALSE)
			bAllChecked = false;

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(oData.strModeName));

		//pce
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oData.dPCE, _T("%.2f")));

		//link_volume
		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(dwDefault);

		std::vector<double> vecData = oData.vecData;

		if ((nColumn - 4) == vecData.size())
		{
			for (int i = 0; i < (nColumn - 4); ++i)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(vecData[i], _T("%.5f")));
			}
		}
		else
		{
			ThrowException(_T("DefaultParameter Data Error..."));
		}

		++bIter;
	}

	m_wndReport.Populate();

	if (bAllChecked)
		CheckDlgButton(IDC_CHECK1, BST_CHECKED);
}

void KCO2EmissionInterpolationDlg::ConvertColNameToDispName( std::vector<CString>& a_vecModeVolColumn )
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

void KCO2EmissionInterpolationDlg::OnBnClickedOk()
{
	try
	{
		if (m_cboTravelTime.GetCurSel() < 0) {
			AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
			GetDlgItem(IDC_COMBO1)->SetFocus();
			return;
		}

		//LinkVolume 중복체크;
		ChkDuplLinkVolume();

		//Save Field Name Check
		GetDlgItemText(IDC_EDIT1, m_strSaveColumn);
		m_strSaveColumn.Trim();

		if (m_strSaveColumn.IsEmpty())
		{
			AfxMessageBox(_T("이름을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		Run();

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
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}

	AfxMessageBox(_T("완료되었습니다."));

	//KResizeDialogEx::OnOK();
}


void KCO2EmissionInterpolationDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KCO2EmissionInterpolationDlg::ChkDuplLinkVolume( void )
{
	std::set<KIOColumn*> setIndex;
	
	CXTPReportRecords*          pRecords  = m_wndReport.GetRecords();
	CXTPReportRecord*           pRecord   = nullptr;
	CXTPReportRecordItem*       pItem     = nullptr;
	CXTPReportRecordItemNumber* pNumber   = nullptr;
	CCustomReportRecordItem*    pCustom   = nullptr;
	KIOColumn*                  pIOColumn = nullptr;

	int nRecord = pRecords->GetCount();
	
	std::set<KIOColumn*>::iterator fIter;

	for (int i = 0; i < nRecord; ++i)
	{
		pRecord = pRecords->GetAt(i);

		//CheckBox
		pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked())
		{
			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);

			if (pNumber->GetValue() == 0)
				ThrowException(_T("PCE값은 0이 될 수 없습니다."));

			pCustom = (CCustomReportRecordItem*)pRecord->GetItem(3);

			pIOColumn = (KIOColumn*)pCustom->GetItemData();
			fIter     = setIndex.find(pIOColumn);

			if (fIter != setIndex.end()) //중복일경우
				ThrowException(_T("중복되는 Link Volume 값이 존재합니다."));

			setIndex.insert(pIOColumn);
		}
	}

	if (setIndex.size() == 0)
		ThrowException(_T("선택한 항목이 없습니다."));
}

unsigned __stdcall KCO2EmissionInterpolationDlg::ThreadCaller( void* p )
{
	QBicSimpleProgressParameter*   pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*            pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("탄소 배출량 산정(예타기준) 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run CO2 Emission(FS Method) Module..."));
	}

	KCO2EmissionInterpolationDlg* pDlg = (KCO2EmissionInterpolationDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KCO2EmissionInterpolationDlg::Run( void )
{
	m_strErrMsg = _T("");

	try
	{
		std::map<Integer, double> mapResult;
		std::map<Integer, double> mapLinkSpeed; //링크별속도
		std::map<Integer, double> mapLinkVolume;
		std::map<int, double>     mapBasicUnit; //원단위
		
		// 2019.01.17 : 링크 g/km를 g으로 수정
		std::map<Integer, double> mapLinkLength;

		std::map<Integer, double>::iterator fResultIter;

		KIOColumn* pTimeIOColumn = (KIOColumn*)m_cboTravelTime.GetItemData(m_cboTravelTime.GetCurSel());
		KDBaseCO2EmissionInterpolation::GetLinkSpeed(m_pTarget, pTimeIOColumn, mapLinkSpeed);

		KDBaseCO2EmissionInterpolation::GetLinkLength(m_pTarget, mapLinkLength);

		CXTPReportRecordItemNumber* pNumber   = nullptr;
		CCustomReportRecordItem*    pCustom   = nullptr;
		KIOColumn*                  pIOColumn = nullptr;

		CString strColumnName;
		Integer nxLinkID;
		double  dSpeed, dResult, dColSpd, dTemp, dPCE;
		double  dGuganLow, dGuganHigh, dGuganLowValue, dGuganHighValue;
		int     nMaxSpeed = m_nMaxSpeed;
		//int     nIdxCombo;

		//선택한 수단에 대한 원단위 계산
		int nRecord = m_wndReport.GetRecords()->GetCount();
		int nColumn = m_wndReport.GetColumns()->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			InitBasicUnit(mapBasicUnit);

			if ((nColumn - 4) != mapBasicUnit.size())
				ThrowException(_T("데이터 오류가 있습니다."));

			pNumber       = (CXTPReportRecordItemNumber*)m_wndReport.GetRecords()->GetAt(i)->GetItem(2);
			dPCE          = pNumber->GetValue();

			pCustom       = (CCustomReportRecordItem*)m_wndReport.GetRecords()->GetAt(i)->GetItem(3);
			pIOColumn     = (KIOColumn*)pCustom->GetItemData();
			KDBaseCO2EmissionInterpolation::GetLinkVolume(m_pTarget, pIOColumn->Name(), mapLinkVolume);

			std::map<Integer, double>::iterator fVolIter;

			std::map<int, double>::iterator bBasicIter = mapBasicUnit.begin();

			std::map<Integer, double>::iterator itLength;

			for (int j = 4; j < nColumn && bBasicIter != mapBasicUnit.end(); ++j, ++bBasicIter)
			{
				pNumber = (CXTPReportRecordItemNumber*)m_wndReport.GetRecords()->GetAt(i)->GetItem(j);
				bBasicIter->second = pNumber->GetValue();
			}

			std::map<Integer, double>::iterator bIter = mapLinkSpeed.begin();

			while (bIter != mapLinkSpeed.end())
			{
				nxLinkID = bIter->first;
				dSpeed   = bIter->second;

				if (dSpeed < 0 || dSpeed > nMaxSpeed)
				{
					dResult = 0;
				}
				else
				{
					bBasicIter = mapBasicUnit.begin();

					while (bBasicIter != mapBasicUnit.end())
					{
						dColSpd = (double)bBasicIter->first;

						if (dColSpd == dSpeed)
						{
							dResult = bBasicIter->second;
							break;
						}
						else
						{
							if (dSpeed > dColSpd)
							{
								dGuganLow      = dColSpd;
								dGuganLowValue = bBasicIter->second;
							}
							else //dSpeed > dColSpd
							{
								dGuganHigh      = dColSpd;
								dGuganHighValue = bBasicIter->second;

								//caclResult
								dTemp   = fabs(dGuganHighValue - dGuganLowValue) / m_nInterval;	// 속도 1증가당 가중치

								if (dGuganHighValue > dGuganLowValue)
								{
									dResult = dGuganLowValue + (dSpeed - dGuganLow) * dTemp;
								}
								else if (dGuganHighValue < dGuganLowValue)
								{
									dResult = dGuganLowValue - (dSpeed - dGuganLow) * dTemp;
								}
								else //구간의 높은지점 낮은지점의 원단위가 같을경우
								{
									dResult = dGuganLowValue;
								}

								break;
							}
						}

						++bBasicIter;
					}
				}

				double dVolume = 0.0;
				double dLength = 0.0;

				fVolIter = mapLinkVolume.find(nxLinkID);
				itLength = mapLinkLength.find(nxLinkID);

				if (fVolIter == mapLinkVolume.end()) 
				{
					ThrowException(_T("if (fVolIter == mapLinkVolume.end())"));
				}
				else
				{
					dVolume = fVolIter->second;
				}

				if (itLength == mapLinkLength.end())
				{
					ThrowException(_T("if (itLength == mapLinkLength.end())"));
				}
				else
				{
					dLength = itLength->second;
				}

				//dResult = dResult * fVolIter->second / dPCE;
				dResult = dResult * dVolume / dPCE * dLength;

				fResultIter = mapResult.find(nxLinkID);

				if (fResultIter == mapResult.end())
				{
					mapResult.insert(std::make_pair(nxLinkID, dResult));
				}
				else
				{
					fResultIter->second = fResultIter->second + dResult;
				}
				++bIter;
			}
		}

		//Add Column
		bool bDuplColumn = false;

		KIOTable*   pIOTable   = m_pTarget->Tables()->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		KIOColumn*  pIONewColumn  = nullptr;

		nColumn = pIOColumns->ColumnCount();

		for (int i = 0; i < nColumn; ++i)
		{
			pIONewColumn = pIOColumns->GetColumn(i);

			if (m_strSaveColumn.CompareNoCase(pIONewColumn->Caption()) == 0)
			{
				bDuplColumn = true;
				break;
			}
		}
		
		//신규 컬럼생성
		if (!bDuplColumn)
		{
			pIONewColumn = CreateResultColumn(m_strSaveColumn, pIOTable, m_strSaveColumn);
		}

		KDBaseCO2EmissionInterpolation::UpdateResult(m_pTarget, pIONewColumn->Name(), mapResult);

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

void KCO2EmissionInterpolationDlg::InitBasicUnit( std::map<int, double>& a_mapBasicUnit )
{
	a_mapBasicUnit.clear();

	int nColumn = (int)ceil((double)m_nMaxSpeed/m_nInterval);
	int nSpeed;

	for (int i = 0; i < nColumn; ++i)
	{
		nSpeed = i * m_nInterval;
		a_mapBasicUnit.insert(std::make_pair(nSpeed, 0.0));
	}

	a_mapBasicUnit.insert(std::make_pair(m_nMaxSpeed, 0.0));
}

KIOColumn* KCO2EmissionInterpolationDlg::CreateResultColumn( CString a_strColumnName, KIOTable* a_pTable, CString a_strDescription )
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

void KCO2EmissionInterpolationDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelect = FALSE;

		if (IsDlgButtonChecked(IDC_CHECK1))
			bSelect = TRUE;

		CXTPReportRecords*    pRecords = m_wndReport.GetRecords();
		CXTPReportRecordItem* pItem    = nullptr;
		
		int nRecord = pRecords->GetCount();

		for (int i = 0; i < nRecord; ++i)
		{
			pItem = pRecords->GetAt(i)->GetItem(0);
			pItem->SetChecked(bSelect);
		}

		m_wndReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KCO2EmissionInterpolationDlg::OnReportItemClick( NMHDR* a_pNMHDR, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) a_pNMHDR;

		if (!pClickedItem->pRow || !pClickedItem->pColumn)
			return;

		int nRowIndex    = pClickedItem->pRow->GetIndex();
		int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

		if (nColumnIndex != 0)
			return;

		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
