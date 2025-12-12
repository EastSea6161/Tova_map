// KSelectedPathCompareRegionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KSelectedPathCompareRegionDlg.h"
#include "afxdialogex.h"


// KSelectedPathCompareRegionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSelectedPathCompareRegionDlg, KResizeDialogEx)

KSelectedPathCompareRegionDlg::KSelectedPathCompareRegionDlg(bool bPathEdit, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(IDD_6929_KSelectedPathCompareRegion, pParent), m_bPathEdit(bPathEdit)
{

}

KSelectedPathCompareRegionDlg::~KSelectedPathCompareRegionDlg()
{
}

void KSelectedPathCompareRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KSelectedPathCompareRegionDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KSelectedPathCompareRegionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KSelectedPathCompareRegionDlg 메시지 처리기입니다.


void KSelectedPathCompareRegionDlg::SetInterPathRegionInfos(std::vector<KInterPathRegionInfoWrapper>& arr)
{
	m_arrRegionPathInfo = arr;
}



BOOL KSelectedPathCompareRegionDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);


	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO));
	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);

	KReportCtrlSetting::Default(m_wndReport);

	InitReportHeader();

	UpdateReportData();

	m_wndReport.GetPaintManager()->SetFixedRowHeight(FALSE);
	m_wndReport.AdjustLayout();
	m_wndReport.RedrawControl();

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSelectedPathCompareRegionDlg::OnBnClickedOk()
{

	KResizeDialogEx::OnOK();
}



void KSelectedPathCompareRegionDlg::InitReportHeader()
{

	int nColumnIndex = 0;
	CXTPReportColumn* pColumn = nullptr;

	try
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Destination"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("종점"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Path"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("경로 구분"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Departure\nTerminal"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("출발 터미널"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Arrival\nTerminal"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("도착 터미널"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("TotalTravelTime"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("기종점간\n통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Cost"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("기종점간\n통행비용"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nInVehTime"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n차내통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nSetupTime"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n대기시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nTransferTime"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n환승시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nProcessTime"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n탑승수속시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nTotalTravelTime"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n총 통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("MainMode\nCost"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("주수단\n통행비용"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("AccessMode\nAvgTravelTime"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("출발 접근수단\n평균 통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("AccessMode\nAvgCost"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("출발 접근수단\n평균 통행비용"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("EgressMode\nAvgTravelTime"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("도착 접근수단\n평균 통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("EgressMode\nAvgCost"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("도착 접근수단\n평균 통행비용"));
		}

		if (!m_bPathEdit)
		{
			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Trip"), 30));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				pColumn->SetCaption(_T("통행량(인)"));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}



void KSelectedPathCompareRegionDlg::UpdateReportData()
{
	m_wndReport.ResetContent();
	
	CXTPReportRecord*     pRecord = nullptr;

	//std::map<KRegionPathDetail*, STRegionPathCostTime> mapPathDetail;
	std::vector<STRegionPathCostTime> arrPathCostTime;

	AggregateRetionPath(arrPathCostTime);
	for (STRegionPathCostTime& rpct : arrPathCostTime)
	{
		pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
		if (REGION_MODE_CODE_AUTO == rpct.pPathInfo->nMode)
		{
			AddAutoModeRecord(pRecord, rpct);
		}
		else
		{
			CalcRegionPathTimeAndCost(rpct);
			AddOtherModeRecord(pRecord, rpct);
		}
	}

	m_wndReport.Populate();
}



void KSelectedPathCompareRegionDlg::AggregateRetionPath(std::vector<STRegionPathCostTime>& arrPathCostTime)
{
	std::map<KRegionPathDetail*, STRegionPathCostTime> mapPathDetail;
	for (KInterPathRegionInfoWrapper& wr : m_arrRegionPathInfo)
	{
		std::map<KRegionPathDetail*, STRegionPathCostTime>::iterator itPathDetail = mapPathDetail.find(wr.pRegionPathDetail);
		if (mapPathDetail.end() == itPathDetail)
		{
			STRegionPathCostTime rpct;
			rpct.pPathInfo = wr.pRegionPathInfo;
			rpct.pPathDetail = wr.pRegionPathDetail;
			rpct.strDest = wr.strDestNode;
			rpct.strMode = wr.pRegionPathInfo->strModeName;
			rpct.strPathName = wr.strPathName;
			rpct.strFromTerminal = wr.strFromTerminal;
			rpct.strToTerminal = wr.strToTerminal;
			if (wr.pRegionPathInfo->nMode == REGION_MODE_CODE_AUTO)
			{
				rpct.dTotalTime = wr.pRegionPathDetail->dMainTime;
				rpct.dTotalCost = wr.pRegionPathDetail->dMainCost;
				rpct.dMainTotalTime = wr.pRegionPathDetail->dMainTime;
				rpct.dMainTotalCost = wr.pRegionPathDetail->dMainCost;
			}
			else
			{
				rpct.dMainInVehTime = wr.pRegionPathDetail->dMainTime;
				rpct.dMainWaitTime = wr.pRegionPathDetail->dMainWaitTime;
				rpct.dMainTransTime = wr.pRegionPathDetail->dMainTransferTime;
				rpct.dMainProcTime = wr.pRegionPathDetail->dMainProcessTime;
				rpct.dMainTotalTime = rpct.dMainInVehTime + rpct.dMainWaitTime + rpct.dMainTransTime + rpct.dMainProcTime;
				rpct.dMainTotalCost = wr.pRegionPathDetail->dMainCost;
			}
			rpct.dTrip = wr.pRegionPathDetail->dTrip;
			mapPathDetail.insert({ wr.pRegionPathDetail, rpct });
		}
	}

	std::map<int, std::vector<STRegionPathCostTime>> mapPathCostTime;
	for (auto& act : mapPathDetail)
	{
		std::map<int, std::vector<STRegionPathCostTime>>::iterator itPathCost = mapPathCostTime.find(act.second.pPathInfo->nMode);
		if (mapPathCostTime.end() == itPathCost)
		{
			std::vector<STRegionPathCostTime> arrTemp;
			arrTemp.push_back(act.second);
			mapPathCostTime.insert({ act.second.pPathInfo->nMode, arrTemp });
		}
		else
		{
			itPathCost->second.push_back(act.second);
		}
	}

	for (auto& aArr : mapPathCostTime)
	{
		arrPathCostTime.insert(arrPathCostTime.end(), aArr.second.begin(), aArr.second.end());
	}
}


void KSelectedPathCompareRegionDlg::CalcRegionPathTimeAndCost(STRegionPathCostTime& rpct)
{
	double dAccTotalTime = 0;
	double dAccTotalCost = 0;
	double dAccTotalTrip = 0;

	double dEgrTotalTime = 0;
	double dEgrTotalCost = 0;
	double dEgrTotalTrip = 0;

	
	// 출발 접근
	for (auto& aAccAuto : rpct.pPathDetail->mapAccAutoAssign)
	{
		GetInterPathTimeAndCost(aAccAuto.second, dAccTotalTime, dAccTotalCost, dAccTotalTrip);
	}

	for (auto& aAccBus : rpct.pPathDetail->mapAccBusAssign)
	{
		GetInterPathTimeAndCost(aAccBus.second, dAccTotalTime, dAccTotalCost, dAccTotalTrip);
	}

	for (auto& aAccRail : rpct.pPathDetail->mapAccRailAssign)
	{
		GetInterPathTimeAndCost(aAccRail.second, dAccTotalTime, dAccTotalCost, dAccTotalTrip);
	}

	for (auto& aAccBR : rpct.pPathDetail->mapAccBusRailAssign)
	{
		GetInterPathTimeAndCost(aAccBR.second, dAccTotalTime, dAccTotalCost, dAccTotalTrip);
	}

	// 도착 접근
	for (auto& aEgrAuto : rpct.pPathDetail->mapEgrAutoAssign)
	{
		GetInterPathTimeAndCost(aEgrAuto.second, dEgrTotalTime, dEgrTotalCost, dEgrTotalTrip);
	}

	for (auto& aEgrBus : rpct.pPathDetail->mapEgrBusAssign)
	{
		GetInterPathTimeAndCost(aEgrBus.second, dEgrTotalTime, dEgrTotalCost, dEgrTotalTrip);
	}

	for (auto& aEgrRail : rpct.pPathDetail->mapEgrRailAssign)
	{
		GetInterPathTimeAndCost(aEgrRail.second, dEgrTotalTime, dEgrTotalCost, dEgrTotalTrip);
	}

	for (auto& aEgrBR : rpct.pPathDetail->mapEgrBusRailAssign)
	{
		GetInterPathTimeAndCost(aEgrBR.second, dEgrTotalTime, dEgrTotalCost, dEgrTotalTrip);
	}

	rpct.dAvgAccessTime = dAccTotalTime / dAccTotalTrip;
	rpct.dAvgAccessCost = dAccTotalCost / dAccTotalTrip;

	rpct.dAvgEgressTime = dEgrTotalTime / dEgrTotalTrip;
	rpct.dAvgEgressCost = dEgrTotalCost / dEgrTotalTrip;

	rpct.dTotalTime = rpct.dMainTotalTime + rpct.dAvgAccessTime + rpct.dAvgEgressTime;
	rpct.dTotalCost = rpct.dMainTotalCost + rpct.dAvgAccessCost + rpct.dAvgEgressCost;
}


void KSelectedPathCompareRegionDlg::GetInterPathTimeAndCost(std::vector<KInterPathEditInfo>& arrInfo, double& dTotalTime, double& dTotalCost, double& dTotalTrip)
{
	for (KInterPathEditInfo& path : arrInfo)
	{
		if (path.emInterRegDetailMode == KEMInterRegDetailModeAuto)
		{
			dTotalTime += (path.oTRegAutoSubData.dAutoTime * path.oTRegAutoSubData.dTrip);
			dTotalCost += (path.oTRegAutoSubData.dAutoCost * path.oTRegAutoSubData.dTrip);
			dTotalTrip += path.oTRegAutoSubData.dTrip;
		}
		else if (path.emInterRegDetailMode == KEMInterRegDetailModeBus || path.emInterRegDetailMode == KEMInterRegDetailModeBusRail)
		{
			dTotalTime += (path.oTBusSubData.dInVehTime + path.oTBusSubData.dWaitTime 
				+ path.oTBusSubData.dTransferTime + path.oTBusSubData.dAccessWalkTime
				+ path.oTBusSubData.dEgressWalkTime) * path.oTBusSubData.dTrip;
			dTotalCost += (path.oTBusSubData.dCost * path.oTBusSubData.dTrip);
			dTotalTrip += path.oTBusSubData.dTrip;
		}
		else if (path.emInterRegDetailMode == KEMInterRegDetailModeRail)
		{
			dTotalTime += (path.oTRailSubData.dInVehTime + path.oTRailSubData.dWaitTime
				+ path.oTRailSubData.dTransferTime + path.oTRailSubData.dAccessWalkTime
				+ path.oTRailSubData.dEgressWalkTime) * path.oTRailSubData.dTrip;
			dTotalCost += (path.oTRailSubData.dCost * path.oTRailSubData.dTrip);
			dTotalTrip += path.oTRailSubData.dTrip;
		}
	}
}



void KSelectedPathCompareRegionDlg::AddAutoModeRecord(CXTPReportRecord* pRecord, STRegionPathCostTime& rpct)
{
	CXTPReportRecordItem* pItem = nullptr;

	// 종점
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strDest));
	pItem->SetAlignment(DT_CENTER);

	// 주수단
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strMode));
	pItem->SetAlignment(DT_CENTER);

	// 경로구분
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strPathName));
	pItem->SetAlignment(DT_CENTER);


	// 출발 터미널
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 도착 터미널
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 기종점간 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTotalTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 기종점간 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTotalCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 차내통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 대기시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 환승시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 탑승수속시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);


	// 주수단 총 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainTotalTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainTotalCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);


	// 출발 접근수단 평균 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 출발 접근수단 평균 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 도착 접근수단 평균 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);

	// 도착 접근수단 평균 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
	pItem->SetAlignment(DT_CENTER);


	// 통행량(인)
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTrip, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);
}


void KSelectedPathCompareRegionDlg::AddOtherModeRecord(CXTPReportRecord* pRecord, STRegionPathCostTime& rpct)
{
	CXTPReportRecordItem* pItem = nullptr;
	// 종점
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strDest));
	pItem->SetAlignment(DT_CENTER);

	// 주수단
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strMode));
	pItem->SetAlignment(DT_CENTER);

	// 경로구분
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strPathName));
	pItem->SetAlignment(DT_CENTER);

	
	// 출발 터미널
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strFromTerminal));
	pItem->SetAlignment(DT_CENTER);

	// 도착 터미널
	pItem = pRecord->AddItem(new CXTPReportRecordItemText(rpct.strToTerminal));
	pItem->SetAlignment(DT_CENTER);

	// 기종점간 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTotalTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 기종점간 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTotalCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);


	// 주수단 차내통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainInVehTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 대기시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainWaitTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 환승시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainTransTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 탑승수속시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainProcTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);


	// 주수단 총 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainTotalTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 주수단 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dMainTotalCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 출발 접근수단 평균 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dAvgAccessTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 출발 접근수단 평균 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dAvgAccessCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 도착 접근수단 평균 통행시간
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dAvgEgressTime, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);

	// 도착 접근수단 평균 통행비용
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dAvgEgressCost, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);


	// 통행량(인)
	pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(rpct.dTrip, _T("%.2f")));
	pItem->SetAlignment(DT_CENTER);
}
