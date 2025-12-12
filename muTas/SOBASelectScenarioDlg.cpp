// SOBASelectScenarioDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SOBASelectScenarioDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Scenario.h"
#include "MultiAssignInfo.h"
#include "DBaseCheckData.h"
#include "AWarmStartDlg.h"



// KSOBASelectScenarioDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSOBASelectScenarioDlg, KDialogEx)

KSOBASelectScenarioDlg::KSOBASelectScenarioDlg(KTarget* pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(IDD_6650_SOBA_SelectScenarioDlg, pParent)
{
	m_pSelectedInfo = NULL;
	m_pSelWSTarget = NULL;
	m_pSelWSScenario = NULL;

	m_bTurnRestriction = false;
	m_bIncludeTurnVolume = false;
	m_bBusInitialVolume = false;
	m_bPathAnalysis = false;
	m_bIntersectionDelay = false;

}

KSOBASelectScenarioDlg::~KSOBASelectScenarioDlg()
{
}

void KSOBASelectScenarioDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndTree);
}


BEGIN_MESSAGE_MAP(KSOBASelectScenarioDlg, KDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KSOBASelectScenarioDlg::OnReportCheckItemTarget)
	ON_BN_CLICKED(IDOK, &KSOBASelectScenarioDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KSOBASelectScenarioDlg 메시지 처리기입니다.

BOOL KSOBASelectScenarioDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	KDialogEx::UseKeyEscEnter(true, false);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	KReportCtrlSetting::DefaultC(m_wndTree);


	InitTreeReport();
	UpdateTreeData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSOBASelectScenarioDlg::OnReportCheckItemTarget(NMHDR* a_pNotifyStruct, LRESULT* a_pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	KMultiAssignInfoSOBA* pInfo = (KMultiAssignInfoSOBA*)pItemNotify->pItem->GetItemData();

	if (pInfo == nullptr)
	{
		return;
	}

	if (pItemNotify->pItem->IsChecked())
	{
		KTarget* pTarget = pInfo->Target();
		KARunningOptionInfo& ro = pInfo->GetRunningOption();

		// 2019.04.19 수정 : 웜스타트 초기치 선택시 현재 체크되어 있는 시나리오를 선택 할 수 있도록 수정
		KAWarmStartDlg oDlg(pTarget, pInfo->GetInputInfo(), ro.Target(), ro.Scnario());
		oDlg.SetRunningOptionPathAnalysis(ro.IncludePathAnalysis());
		std::set<KTarget*> setCheckedTarget = GetCheckedTarget();
		oDlg.SetCheckedTarget(setCheckedTarget);
		oDlg.SetMultiAssingInfo(m_arrMultiAssingInfo);

		if (oDlg.DoModal() == IDOK)
		{
			m_pSelWSTarget = oDlg.GetWarmTarget();
			m_pSelWSScenario = oDlg.GetWarmScnario();

			// 이미 선택된 시나리오가 웜스타트 초기치로 선택되었으면 해당 시나리오의 실행 옵션을 복사
			// 그렇지 않으면 KAWarmStartDlg의 설정값을 가져온다.
			bool bSelected = false;
			for (KMultiAssignInfoSOBA* pInfo : m_arrMultiAssingInfo)
			{
				if(pInfo->GetSelected() && pInfo->Target() == m_pSelWSTarget)
				{
					KARunningOptionInfo& ro = pInfo->GetRunningOption();
					m_bTurnRestriction = ro.IncludeTurnRestriction();
					m_bIncludeTurnVolume = ro.IncludeTurnVolume();
					m_bBusInitialVolume = ro.IncludeBusInitialVolume();
					m_bPathAnalysis = ro.IncludePathAnalysis();
					m_bIntersectionDelay = ro.IncludeIntersectionDelay();
					bSelected = true;
					break;
				}
			}

			if (!bSelected)
			{
				m_bTurnRestriction = oDlg.isTurnRestriction();
				m_bIncludeTurnVolume = oDlg.isIncludeTurnVolume();
				m_bBusInitialVolume = oDlg.isBusInitialVolume();
				m_bPathAnalysis =  oDlg.isPathAnalysis();
				m_bIntersectionDelay = oDlg.isIntersectionDelay();
			}

			if (NULL != m_pSelWSScenario && NULL != m_pSelWSTarget)
			{
				CString strCaption(_T(""));
				strCaption.Format(_T("%s_%s"), m_pSelWSScenario->GetName(), m_pSelWSTarget->GetName());

				CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
				CXTPReportRecordItem* pItemStatus = pRecord->GetItem(1);
				pItemStatus->SetCaption(_T("선택됨"));
				CXTPReportRecordItem* pItemWarmstart = pRecord->GetItem(2);
				pItemWarmstart->SetCaption(strCaption);
			}

			// 다른 시나리오의 체크를 해제
			CXTPReportRecord*     pScenarioRecord = nullptr;
			CXTPReportRecords*    pTargetRecords = nullptr;
			CXTPReportRecord*     pTargetRecord = nullptr;
			CXTPReportRecordItem* pItem = nullptr;

			int nRecord = m_wndTree.GetRecords()->GetCount();

			for (int i = 0; i < nRecord; ++i)
			{
				pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
				pTargetRecords = pScenarioRecord->GetChilds();

				int nTargetRecord = pTargetRecords->GetCount();

				for (int j = 0; j < nTargetRecord; ++j)
				{
					pTargetRecord = pTargetRecords->GetAt(j);
					pItem = pTargetRecord->GetItem(0);

					if ((pItem != pItemNotify->pItem) && (pItem->IsEditable() == TRUE) && (pItem->IsChecked() == TRUE))
					{
						pItem->SetChecked(FALSE);
						pItem = pTargetRecord->GetItem(1);
						pItem->SetCaption(_T(""));
						pItem = pTargetRecord->GetItem(2);
						pItem->SetCaption(_T(""));
					}
				}
			}
		}
		else
		{
			pItemNotify->pItem->SetChecked(FALSE);
			m_wndTree.RedrawControl();
		}
	}
	else
	{
		CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pItemStatus = pRecord->GetItem(1);
		pItemStatus->SetCaption(_T(""));
		CXTPReportRecordItem* pItemWarmstart = pRecord->GetItem(2);
		pItemWarmstart->SetCaption(_T(""));
	}
}


void KSOBASelectScenarioDlg::OnBnClickedOk()
{
	CXTPReportRecord*     pScenarioRecord = nullptr;
	CXTPReportRecords*    pTargetRecords = nullptr;
	CXTPReportRecord*     pTargetRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	int nRecord = m_wndTree.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
		pTargetRecords = pScenarioRecord->GetChilds();

		int nTargetRecord = pTargetRecords->GetCount();

		for (int j = 0; j < nTargetRecord; ++j)
		{
			pTargetRecord = pTargetRecords->GetAt(j);
			pItem = pTargetRecord->GetItem(0);

			if ((pItem->IsEditable() == TRUE) && (pItem->IsChecked() == TRUE))
			{
				KMultiAssignInfoSOBA* pInfo = (KMultiAssignInfoSOBA*)pItem->GetItemData();
				m_pSelectedInfo = pInfo;
				KARunningOptionInfo& ro = pInfo->GetRunningOption();
				ro.Target(m_pSelWSTarget);
				ro.Scnario(m_pSelWSScenario);
				
				ro.IncludeTurnRestriction(m_bTurnRestriction);
				ro.IncludeTurnVolume(m_bIncludeTurnVolume);
				ro.IncludeBusInitialVolume(m_bBusInitialVolume);
				ro.IncludePathAnalysis(m_bPathAnalysis);
				ro.IncludeIntersectionDelay(m_bIntersectionDelay);
			}
		}
	}

	KDialogEx::OnOK();
}


void KSOBASelectScenarioDlg::SetMultiAssignInfoArray(std::vector<KMultiAssignInfoSOBA*>& arr)
{
	m_arrMultiAssingInfo = arr;
	for (KMultiAssignInfoSOBA* pInfo : arr)
	{
		KARunningOptionInfo& ro = pInfo->GetRunningOption();
		if (pInfo->GetSelected() && ro.Target() != NULL)
		{
			m_setWormStartSource.insert(ro.Target());
		}
	}
}


KMultiAssignInfoSOBA* KSOBASelectScenarioDlg::GetSelectedAssignInfo()
{
	return m_pSelectedInfo;
}


void KSOBASelectScenarioDlg::InitTreeReport(void)
{
	CXTPReportColumn* pColumn = nullptr;

	int nItemIndex(0);

	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("::: Select Target"), 120));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTreeColumn(TRUE);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("분석할 시나리오 선택"));
	}

	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Status"), 130));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("상태정보"));
	}

	pColumn = m_wndTree.AddColumn(new CXTPReportColumn(nItemIndex++, _T("Initial Warm Start Value"), 140));
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("웜 스타트 초기치"));
	}

	m_wndTree.Populate();
}


void KSOBASelectScenarioDlg::UpdateTreeData(void)
{
	m_wndTree.ResetContent();
	m_mapScenarioRecord.clear();
	CString strValue;

	CXTPReportRecord*     pScenarioRecord = nullptr;
	CXTPReportRecord*     pTargetRecord = nullptr;
	CXTPReportRecords*    pTargetRecords = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	bool bFirst = true;
	for (KMultiAssignInfoSOBA* pInfo : m_arrMultiAssingInfo)
	{
		KTarget* pTarget = pInfo->Target();
		KScenario* pScenario = (KScenario*)pTarget->GetParentObject();

		std::map<KScenario*, CXTPReportRecord*>::iterator itScenarioRecord = m_mapScenarioRecord.find(pScenario);
		if (m_mapScenarioRecord.end() == itScenarioRecord)
		{
			pScenarioRecord = m_wndTree.AddRecord(new CXTPReportRecord());
			m_mapScenarioRecord.insert({ pScenario, pScenarioRecord });
			pItem = pScenarioRecord->AddItem(new CXTPReportRecordItemText(pScenario->GetName()));
			pItem = pScenarioRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(FALSE);
			pItem = pScenarioRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetEditable(FALSE);
		}
		else
		{
			pScenarioRecord = itScenarioRecord->second;
		}

		pTargetRecords = pScenarioRecord->GetChilds();
		pTargetRecord = pTargetRecords->Add(new CXTPReportRecord);
		pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(pTarget->GetName()));
		pItem->SetItemData((DWORD_PTR)pInfo);
		pItem->HasCheckbox(TRUE);

		if (bFirst || pInfo->GetSelected())
		{
			bFirst = false;
			pItem->SetChecked(TRUE);
			pItem->SetEditable(FALSE);

			CString strWormstart;
			KARunningOptionInfo& ro = pInfo->GetRunningOption();
			if (ro.Target() != NULL)
			{
				strWormstart = ro.Target()->CaptionScenario() + _T("_") + ro.Target()->CaptionTargetYear();
			}

			pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(_T("선택됨")));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(strWormstart));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			CString strStatus;
			if (!CheckTargetRunnable(pTarget, strStatus))
			{
				pItem->SetEditable(FALSE);
				pInfo->SetError(true);
				pInfo->SetStatusString(strStatus);
			}

			pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText(strStatus));
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem = pTargetRecord->AddItem(new CXTPReportRecordItemText());
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
	}

	m_wndTree.Populate();
	m_wndTree.ExpandAll(TRUE);
}




bool KSOBASelectScenarioDlg::CheckTargetRunnable(KTarget* pTarget, CString& strStatus)
{
	if (KmzSystem::IsPassengerSystem())
	{
		if (KDBaseCheckData::GetCheckInspectionErrorHighwayNoMB(pTarget) == false)
		{
			strStatus = _T("네트워크 데이터 검사 필요");
			return false;
		}

		if (!KDBaseAssignment::ModeChoiceExist(pTarget))
		{
			strStatus = _T("수단 통행 데이터 없음");
			return false;
		}

		if (KDBaseCheckData::GetExistModeODData(pTarget) == false)
		{
			strStatus = _T("수단 통행 데이터 없음");
			return false;
		}	
	}

	if (m_setWormStartSource.find(pTarget) != m_setWormStartSource.end())
	{
		strStatus = _T("이미 웜 스타트 초기치로 선택되었음");
		return false;
	}

	return true;
}



std::set<KTarget*> KSOBASelectScenarioDlg::GetCheckedTarget()
{
	std::set<KTarget*> setCheckedTarget;

	CXTPReportRecord*     pScenarioRecord = nullptr;
	CXTPReportRecords*    pTargetRecords = nullptr;
	CXTPReportRecord*     pTargetRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	int nRecord = m_wndTree.GetRecords()->GetCount();

	for (int i = 0; i < nRecord; ++i)
	{
		pScenarioRecord = m_wndTree.GetRecords()->GetAt(i);
		pTargetRecords = pScenarioRecord->GetChilds();

		int nTargetRecord = pTargetRecords->GetCount();

		for (int j = 0; j < nTargetRecord; ++j)
		{
			pTargetRecord = pTargetRecords->GetAt(j);
			pItem = pTargetRecord->GetItem(0);

			if ((pItem->IsEditable() == FALSE) && (pItem->IsChecked() == TRUE))
			{
				KMultiAssignInfoSOBA*       pDlg = (KMultiAssignInfoSOBA*)pItem->GetItemData();
				KTarget* pTarget = pDlg->Target();
				setCheckedTarget.insert(pTarget);
			}
		}
	}

	return setCheckedTarget;
}