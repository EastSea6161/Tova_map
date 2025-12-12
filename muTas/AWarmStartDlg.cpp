// AWarmStartDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AWarmStartDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Project.h"
#include "Scenario.h"
#include "ImChampFrameWindow.h"
#include "AInputInfo.h"
#include "DBaseAssignment.h"
#include "ModeManager.h"
#include "DBaseCheckData.h"
#include "..\QBicCommon\QBicTxtReader.h"
#include "MultiAssignInfo.h"


// KAWarmStartDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAWarmStartDlg, KResizeDialogEx)

KAWarmStartDlg::KAWarmStartDlg(KTarget* a_pTarget, std::list<KAInputInfo*> a_lstInputInfo, KTarget* a_pSelTarget, KScenario* a_pSelScnario, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAWarmStartDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_lstInputInfo(a_lstInputInfo)
	, m_pSelTarget(a_pSelTarget)
	, m_pSelScenario(a_pSelScnario)
{
	m_bROPathAnalysis = false;
}

KAWarmStartDlg::~KAWarmStartDlg()
{
}

void KAWarmStartDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KAWarmStartDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT,  OnReportCheckItem)
	ON_BN_CLICKED(IDOK, &KAWarmStartDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KAWarmStartDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KAWarmStartDlg 메시지 처리기입니다.


BOOL KAWarmStartDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	int nIndex(0);
	CString strModeName(_T(""));
	std::list<KAInputInfo*>::iterator itInput,itEnd = m_lstInputInfo.end();
	for(itInput = m_lstInputInfo.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput = *itInput;
		if (pInput->Selected() == true)
		{
			if (nIndex == 0)
			{
				strModeName.Format(_T("%s"), pInput->Mode()->ModeName());
			}
			else
			{
				strModeName.AppendFormat(_T(", %s"), pInput->Mode()->ModeName());
			}

			nIndex++;
		}
	}
	GetDlgItem(IDC_STATIC2)->SetWindowText(strModeName);

	KReportCtrlSetting::Default(m_wndReport);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReport); 

	InitReportHeader();
	InitReportData();

	ResizeComponet();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAWarmStartDlg::OnBnClickedOk()
{
	try
	{
		int nSelCount(0);

		CXTPReportRecords* pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord*     pRecord        = pRecords->GetAt(i);
			CXTPReportRecordItem* pItemSenario	 = pRecord->GetItem(0);
			KScenario* pScenario = (KScenario*)pItemSenario->GetItemData();

			CXTPReportRecords*	  pCRecords		 = pRecord->GetChilds();
			int nCRecordCount = pCRecords->GetCount();
			for (int j = 0; j < nCRecordCount; j++)
			{
				CXTPReportRecord* pCRecord = pCRecords->GetAt(j);
				CXTPReportRecordItem* pItemTarget = pCRecord->GetItem(0);
				if (pItemTarget->IsChecked() == TRUE)
				{
					m_pSelTarget = (KTarget*)pItemTarget->GetItemData();
					m_pSelScenario = pScenario;
					nSelCount++;
				}
			}
		}

		if (nSelCount < 1)
		{
			AfxMessageBox(_T("활성화된 네트워크가 없습니다."));
			return;
		}

		CString path = m_pSelTarget->GetLocation();
		CString argFile = path + m_argFileName;
		readRunOption(argFile);

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


void KAWarmStartDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KAWarmStartDlg::GetWarmData( KTarget* a_pTarget, KScenario* a_pScanrio )
{
	a_pTarget = m_pSelTarget;
	a_pScanrio = m_pSelScenario;
}

KTarget* KAWarmStartDlg::GetWarmTarget()
{
	return m_pSelTarget;
}

KScenario* KAWarmStartDlg::GetWarmScnario()
{
	return m_pSelScenario;
}

void KAWarmStartDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select Warm Target"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	pColumn->SetTreeColumn(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("초기치 선택"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Info"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("상태설명"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(2, _T("Used Mode"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("적용된 통행수단"));
    }

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(3, _T("Date"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("분석 날짜"));
    }
}

void KAWarmStartDlg::InitReportData()
{
	m_wndReport.ResetContent();

	try
	{
		CXTPReportRecord*	pRecord = nullptr;
		CXTPReportRecordItem* pItem         = nullptr;
		CXTPReportRecordItem* pScenarioItem = nullptr;

		KProject* pProject = ImChampFrameWindow::GetProject();

		KScenario* pBaseScenario = pProject->GetBaseScenario();
		KTarget*   pBaseTarget	 = nullptr;

		const KTDTargetMap&          mapBaseTarget = pBaseScenario->GetTargetMap();
		KTDTargetMap::const_iterator citTargetMap  = mapBaseTarget.begin();

		pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pBaseScenario->GetName()));
		pItem->SetAlignment(DT_LEFT);
		pItem->SetItemData((DWORD_PTR)pBaseScenario);

		pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

		AddTreeRecord(pRecord, pBaseScenario);

		//Other Scenario
		const KTDScenarioMap&          scenarioMap = pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator itScenario, itEnd = scenarioMap.end();

		for (itScenario = scenarioMap.begin(); itScenario != itEnd; ++itScenario)
		{
			KScenario* pScenario = itScenario->second;

			if (pScenario == nullptr)
				continue;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR)pScenario);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
			pScenarioItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));

			AddTreeRecord(pRecord, pScenario);

		}
		m_wndReport.Populate();
		m_wndReport.ExpandAll(TRUE);
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


void KAWarmStartDlg::AddTreeRecord( CXTPReportRecord* a_pParent, KScenario* a_pScenario )
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	const KTDTargetMap& targetMap = a_pScenario->GetTargetMap();
	KTDTargetMap::const_iterator itTarget, itEnd = targetMap.end();

	CString strValue;

	int nAddTargetCnt = 0;
	for (itTarget = targetMap.begin(); itTarget != itEnd; ++itTarget)
	{
		KTarget* pTarget = itTarget->second;

		if (pTarget == nullptr || m_pTarget == pTarget)
			continue;

		nAddTargetCnt++;
		pRecord = a_pParent->GetChilds()->Add(new CXTPReportRecord);
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pTarget->GetName()));
		pItem->SetAlignment(DT_LEFT);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetItemData((DWORD_PTR)pTarget);

		CString strMessage(_T(""));
		if (pTarget->IsEmptyData() == true)
		{
			pItem->SetEditable(FALSE);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Data is Empty")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
				pItemTxt->SetValue(_T("네트워크 데이터 없음"));
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
		}
		else 
		{
			CString strStatus;
			CString strStatusEng;
			if (CheckTargetSelectable(pTarget, strStatus, strStatusEng))
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strStatusEng));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
					pItemTxt->SetValue(strStatus);
				}

				if (strStatus.Compare(_T("선택가능")) == 0)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText(GetExistModeName(pTarget)));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemText(GetEndDate(pTarget)));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetEditable(FALSE);
				}
				else
				{
					// 분석 예정인 시나리오의 수단을 가져온다.
					pItem = pRecord->AddItem(new CXTPReportRecordItemText(GetTargetModeName(pTarget)));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
					pItem->SetAlignment(DT_CENTER);
					pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem->SetEditable(FALSE);
				}
			}
			else
			{
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strStatusEng));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
					pItemTxt->SetValue(strStatus);
				}

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);

				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}
		}
	}

	if (nAddTargetCnt == 0)
	{
		m_wndReport.RemoveRecordEx(a_pParent);
	}
}

bool KAWarmStartDlg::CheckMode( KTarget* a_pTarget)
{
	try
	{
		std::set<int> setExistMode;
		std::set<int> setSelectMode;

		std::list<KAInputInfo*>::iterator itInput,itEnd = m_lstInputInfo.end();
		for(itInput = m_lstInputInfo.begin(); itInput != itEnd; ++itInput)
		{
			KAInputInfo* pInput = *itInput;
			if (pInput->Selected() == true)
			{
				setSelectMode.insert(pInput->Mode()->ModeID());
			}
		}

		GetExistModeID(a_pTarget, setExistMode);

		if (setSelectMode.size() != setExistMode.size())
		{
			return false;
		}

		AutoType Siter = setSelectMode.begin();
		AutoType Send  = setSelectMode.end();
		while(Siter != Send)
		{
			int nModeID = *Siter;

			AutoType Efind = setExistMode.find(nModeID);
			AutoType Eend  = setExistMode.end();
			if (Efind == Eend)
			{
				return false;
			}
			Siter++;
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

bool KAWarmStartDlg::CheckTargetBushFile( KTarget* a_pTarget )
{
	try
	{
		CString strBushPath(_T(""));
		strBushPath.Format(_T("%s\\%s\\%s"), a_pTarget->GetLocation(),BUSH_PATHVOLUMN_FOLDER_NAME,_T("*.bsh"));


		CFileFind oCFileFinder;
		if (FALSE == oCFileFinder.FindFile(strBushPath))
		{
			return false;
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

void KAWarmStartDlg::GetExistModeID( KTarget* a_pTarget, std::set<int>& a_setModeInfo )
{
	CString strSQL(_T(""));
	a_setModeInfo.clear();

	try
	{
		strSQL.Format(_T("Select mode_id From %s Order by mode_id"), TABLE_ASSIGN_PATH_INPUT);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			int nModeID = spResultset->GetValueInt(0);
			a_setModeInfo.insert(nModeID);
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

CString KAWarmStartDlg::GetExistModeName( KTarget* a_pTarget )
{
	CString strMode(_T("-"));
	try
	{
		int nIndex(0);
		std::set<int> setExistMode;
		GetExistModeID(a_pTarget, setExistMode);

		KModeManager* pModeManager = m_pTarget->ModeManager();
		int nModeCount = pModeManager->GetModeCount();
		for (int i = 0; i < nModeCount; i++)
		{
			KMode* pMode = pModeManager->GetMode(i);
			AutoType find = setExistMode.find(pMode->ModeID());
			AutoType fend = setExistMode.end();
			if (find != fend)
			{
				if (nIndex == 0)
				{
					strMode.Format(_T("%s"), pMode->ModeName());
				}
				else
				{
					strMode.AppendFormat(_T(", %s"), pMode->ModeName());
				}

				nIndex++;
			}
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

	return strMode;
}

CString KAWarmStartDlg::GetEndDate( KTarget* a_pTarget )
{
	CString strDate(_T("-"));
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Select end_date From %s "), TABLE_ASSIGN_PATH_MODEL);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultset->Next())
		{
			strDate = spResultset->GetValueString(0);
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

	return strDate;
}

void KAWarmStartDlg::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* result )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (0 == nIndex)
		{
			if( TRUE == pItemNotify->pItem->IsChecked() )
			{
				CXTPReportRecords* pRecords = m_wndReport.GetRecords();
				int nRecordCount = pRecords->GetCount();
				for(int i = 0; i < nRecordCount; ++i)
				{
					CXTPReportRecord*     pRecord        = pRecords->GetAt(i);
					CXTPReportRecords*	  pCRecords		 = pRecord->GetChilds();
					
					int nCRecordCount = pCRecords->GetCount();
					for (int j = 0; j < nCRecordCount; j++)
					{
						CXTPReportRecord* pCRecord = pCRecords->GetAt(j);
						CXTPReportRecordItem* pItemSelection = pCRecord->GetItem(0);
						pItemSelection->SetChecked(FALSE);
					}
				}
				pItemNotify->pItem->SetChecked(TRUE);
			}
		}

		m_wndReport.Populate();
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

void KAWarmStartDlg::ResizeComponet( void )
{
	SetResize(IDC_STATIC1,					SZ_TOP_LEFT,    SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,					SZ_TOP_LEFT,    SZ_TOP_RIGHT);	
	SetResize(IDC_REPORT,				    SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR2,				SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDOK,							SZ_BOTTOM_RIGHT,    SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,						SZ_BOTTOM_RIGHT,    SZ_BOTTOM_RIGHT);
}


// 2018.08.02 명지대 요청사항 수정
bool KAWarmStartDlg::isTurnRestriction()
{
	return m_bTurnRestriction;
}


bool KAWarmStartDlg::isIncludeTurnVolume()
{
	return m_bIncludeTurnVolume;
}


bool KAWarmStartDlg::isBusInitialVolume()
{
	return m_bBusInitialVolume;
}


bool KAWarmStartDlg::isPathAnalysis()
{
	return m_bPathAnalysis;
}


bool KAWarmStartDlg::isIntersectionDelay()
{
	return m_bIntersectionDelay;
}


void KAWarmStartDlg::readRunOption(CString& argFileName)
{
	QBicTxtReader reader;
	if (!reader.Open(argFileName))
	{
		return;
	}

	CString line;
	while (reader.ReadLine(line))
	{
		CString optionName = line.Left(11);
		if (optionName.CompareNoCase(_T("run_options")) == 0)
		{
			if (reader.ReadLine(line))
			{
				line.Replace(_T("\t"), _T(""));
				if (line.GetLength() != 5)
				{
					return;
				}

				m_bTurnRestriction = line.GetAt(0) == _T('1');
				m_bBusInitialVolume = line.GetAt(1) == _T('1');
				m_bPathAnalysis = line.GetAt(2) == _T('1');
				m_bIntersectionDelay = line.GetAt(3) == _T('1');
				m_bIncludeTurnVolume = line.GetAt(4) == _T('1');
			}
			
		}
	}
}
// 2018.08.02 명지대 요청사항 수정 끝


// 2019.04.19 수정 : 여러 시나리오 반복실행에서 Bush 파일이 없지만 모형을 돌리는 분석년도(대상)은 선택이 가능하게 한다.
void KAWarmStartDlg::SetCheckedTarget(std::set<KTarget*>& setCheckedTarget)
{
	m_setCheckedTarget = setCheckedTarget;
}


//void KAWarmStartDlg::SetRunningOptionMap(std::map<KTarget*, KARunningOptionInfo>& mapRunningOption)
//{
//	m_setWormStartSource.clear();
//	m_mapRunningOption = mapRunningOption;
//
//	for (auto& ro : m_mapRunningOption)
//	{
//		if (ro.second.Target() != NULL)
//		{
//			m_setWormStartSource.insert(ro.second.Target());
//		}
//	}
//}


void KAWarmStartDlg::SetRunningOptionPathAnalysis(bool bPathAnalysis)
{
	m_bROPathAnalysis = bPathAnalysis;
}




bool KAWarmStartDlg::CheckTargetSelectable(KTarget* pTarget, CString& strStatus, CString& strStatusEng)
{
	// 네트워크 데이터 검사를 수행하지 않았거나, 오류 정보가 남아있는 경우, “경로 교통량 분석결과 없음”으로 표출함 
	if (KDBaseCheckData::GetCheckInspectionErrorHighwayNoMB(pTarget) == false)
	{
		strStatus = _T("경로 교통량 분석결과 없음");
		strStatusEng = _T("No Result of Path Volume Analysis");
		return false;
	}
	
	if (m_bROPathAnalysis && m_setCheckedTarget.find(pTarget) != m_setCheckedTarget.end())
	{
		strStatus = _T("선택가능(분석 예정)");
		strStatusEng = _T("Selectable(To be analyzed)");
		return true;
	}

	if (CheckAssignHistoryExists(pTarget) == false)
	{
		strStatus = _T("경로 교통량 분석결과 없음");
		strStatusEng = _T("No Result of Path Volume Analysis");
		return false;
	}

	if (CheckTargetBushFile(pTarget) == false)
	{
		strStatus = _T("경로 교통량 분석결과 없음");
		strStatusEng = _T("No Result of Path Volume Analysis");
		return false;
	}

	if (CheckMode(pTarget) == false)
	{
		strStatus = _T("통행수단 불일치");
		strStatusEng = _T("Mode Mismatch");
		return false;
	}

	strStatus = _T("선택가능");
	strStatusEng = _T("Selectable");
	return true;
}


bool KAWarmStartDlg::CheckAssignHistoryExists(KTarget* pTarget)
{
	CString strSQL(_T(""));
	int nRecordCount(0);
	bool bExist = false;

	try
	{
		strSQL.Format(_T("Select count(*) CNT From %s"), TABLE_ASSIGN_MODEL);

		KDBaseConPtr spDBConn = pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBConn->ExecuteQuery(strSQL);

		if (spResultset->Next())
			nRecordCount = spResultset->GetValueInt(0);

		if (0 != nRecordCount)
			bExist = true;

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return bExist;
}


void KAWarmStartDlg::SetMultiAssingInfo(std::vector<KMultiAssignInfoSOBA*>& arr)
{
	m_mapMultiAssignInfo.clear();
	for (KMultiAssignInfoSOBA* pInfo : arr)
	{
		KTarget* pTarget = pInfo->Target();
		m_mapMultiAssignInfo.insert({ pTarget, pInfo });
	}
}

CString KAWarmStartDlg::GetTargetModeName(KTarget* pTarget)
{
	CString strName = _T("");
	std::map<KTarget*, KMultiAssignInfoSOBA*>::iterator it = m_mapMultiAssignInfo.find(pTarget);
	if (m_mapMultiAssignInfo.end() == it)
	{
		strName = _T("-");
	}
	else
	{
		KMultiAssignInfoSOBA* pInfo = it->second;
		std::list<KAInputInfo*>& lstInput = pInfo->GetInputInfo();
		for (KAInputInfo* pInput : lstInput)
		{
			if (pInput->Selected())
			{
				KMode* pMode = pInput->Mode();
				strName += pMode->ModeName();
				strName += _T(", ");
			}
		}

		if (strName.GetLength() > 2)
		{
			strName.Delete(strName.GetLength() - 2, 2);
		}
	}

	return strName;
}