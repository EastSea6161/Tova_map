// KSelectedPathCompareUrbanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KSelectedPathCompareUrbanDlg.h"
#include "afxdialogex.h"




// KSelectedPathCompareUrbanDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSelectedPathCompareUrbanDlg, KResizeDialogEx)

KSelectedPathCompareUrbanDlg::KSelectedPathCompareUrbanDlg(bool bPathEdit, EMPathCompareMode emMode, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(IDD_6929_KSelectedPathCompareUrban, pParent), m_bPathEdit(bPathEdit), m_emCompareMode(emMode)
{

}


KSelectedPathCompareUrbanDlg::~KSelectedPathCompareUrbanDlg()
{
}


void KSelectedPathCompareUrbanDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KSelectedPathCompareUrbanDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KSelectedPathCompareUrbanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KSelectedPathCompareUrbanDlg 메시지 처리기입니다.


BOOL KSelectedPathCompareUrbanDlg::OnInitDialog()
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


void KSelectedPathCompareUrbanDlg::OnBnClickedOk()
{
	
	KResizeDialogEx::OnOK();
}


void KSelectedPathCompareUrbanDlg::SetTransitEditInfos(std::vector<KMTransitEditInfoWrapper>& arrInfo)
{
	m_arrTransitEditInfo = arrInfo;
}


void KSelectedPathCompareUrbanDlg::SetMultimodalResultInfos(std::vector<KMBusAssignInfoWrapper>& arr)
{
	m_arrMultimodalResultInfo = arr;
}


void KSelectedPathCompareUrbanDlg::SetInterPathEditInfos(std::vector<KInterPathEditInfoWrapper>& arr)
{
	m_arrInterPathEditInfo = arr;
}



void KSelectedPathCompareUrbanDlg::InitReportHeader()
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

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Mode"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("통행수단"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Path"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("경로구분"));
		}

		if (EMPathCompareMode_Intermodal == m_emCompareMode)
		{
			pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("TotalTravelTime"), 60));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				pColumn->SetCaption(_T("총 통행시간"));
			}
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("InVehTime"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER | DT_WORDBREAK);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("차내통행시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("WaitTime"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("대기시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("TransferTime"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("환승시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("AccWalkTime"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("출발 접근시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("EgrWalkTime"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("도착 접근시간"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Length"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("통행거리"));
		}

		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Cost"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			pColumn->SetCaption(_T("통행비용"));
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



void KSelectedPathCompareUrbanDlg::UpdateReportData()
{
	m_wndReport.ResetContent();
	if (EMPathCompareMode_Multimodal == m_emCompareMode)
	{
		if (m_bPathEdit)
		{
			UpdateReportMultimodalPathEditInfo();
		}
		else
		{
			UpdateReportMultimodalResultInfo();
		}
	}
	else
	{
		UpdateReportIntermodalUrbanPathInfo();
	}

	m_wndReport.Populate();
}



void KSelectedPathCompareUrbanDlg::UpdateReportMultimodalPathEditInfo()
{
	std::map<KEMTransitAssignMode, std::vector<KMTransitEditInfoWrapper>> mapTransit;
	for (KMTransitEditInfoWrapper& wr : m_arrTransitEditInfo)
	{
		if (NULL == wr.pInfo)
		{
			continue;
		}

		std::map<KEMTransitAssignMode, std::vector<KMTransitEditInfoWrapper>>::iterator it = mapTransit.find(wr.pInfo->emTransitAssignMode);
		if (it == mapTransit.end())
		{
			std::vector<KMTransitEditInfoWrapper> arrTransit;
			arrTransit.push_back(wr);
			mapTransit.insert({ wr.pInfo->emTransitAssignMode, arrTransit });
		}
		else
		{
			it->second.push_back(wr);
		}
	}

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	for (std::map<KEMTransitAssignMode, std::vector<KMTransitEditInfoWrapper>>::iterator it = mapTransit.begin();
		it != mapTransit.end(); ++it)
	{
		std::vector<KMTransitEditInfoWrapper>& arrTransit = it->second;
		for (KMTransitEditInfoWrapper& wr : arrTransit)
		{
			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
			KMTransitEditInfo* pInfo = wr.pInfo;

			CString strText;
			//strText.Format(_T("%I64d"), pInfo->nxDestinationID);
			//pItem = pRecord->AddItem(new CXTPReportRecordItemText(strText));
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strDestNode));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emTransitAssignMode == KEMTransitAssignBus)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스")));
			}
			else if (pInfo->emTransitAssignMode == KEMTransitAssignRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("도시철도")));
			}
			else if (pInfo->emTransitAssignMode == KEMTransitAssignBusRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스+도시철도")));
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("알수없음")));
			}
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strPathName));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emTransitAssignMode == KEMTransitAssignBus || pInfo->emTransitAssignMode == KEMTransitAssignBusRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
			}

		}
	}
}


void KSelectedPathCompareUrbanDlg::UpdateReportMultimodalResultInfo()
{
	std::map<KEMTransitAssignMode, std::vector<KMBusAssignInfoWrapper>> mapInfos;
	for (KMBusAssignInfoWrapper& wr : m_arrMultimodalResultInfo)
	{
		if (NULL == wr.pInfo)
		{
			continue;
		}
		std::map<KEMTransitAssignMode, std::vector<KMBusAssignInfoWrapper>>::iterator it
			= mapInfos.find(wr.pInfo->emTransitAssignMode);
		if (it == mapInfos.end())
		{
			std::vector<KMBusAssignInfoWrapper> arr;
			arr.push_back(wr);
			mapInfos.insert({ wr.pInfo->emTransitAssignMode, arr });
		}
		else
		{
			it->second.push_back(wr);
		}
	}

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	for (std::map<KEMTransitAssignMode, std::vector<KMBusAssignInfoWrapper>>::iterator it = mapInfos.begin();
		it != mapInfos.end(); ++it)
	{
		std::vector<KMBusAssignInfoWrapper>& arr = it->second;
		for (KMBusAssignInfoWrapper& wr : arr)
		{
			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
			KMBusAssignInfo* pInfo = wr.pInfo;

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strDestNode));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emTransitAssignMode == KEMTransitAssignBus)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스")));
			}
			else if (pInfo->emTransitAssignMode == KEMTransitAssignRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("도시철도")));
			}
			else if (pInfo->emTransitAssignMode == KEMTransitAssignBusRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스+도시철도")));
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("알수없음")));
			}
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strPathName));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emTransitAssignMode == KEMTransitAssignBus || pInfo->emTransitAssignMode == KEMTransitAssignBusRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dAssignTrip, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dAssignTrip, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
			}
		}
	}
}


void KSelectedPathCompareUrbanDlg::UpdateReportIntermodalUrbanPathInfo()
{
	std::map<KEMInterMode, std::vector<KInterPathEditInfoWrapper>> mapUrbanPath;

	for (KInterPathEditInfoWrapper& wr : m_arrInterPathEditInfo)
	{
		if (NULL == wr.pInfo)
		{
			continue;
		}

		std::map<KEMInterMode, std::vector<KInterPathEditInfoWrapper>>::iterator it = mapUrbanPath.find(wr.pInfo->emInterMode);
		if (it == mapUrbanPath.end())
		{
			std::vector<KInterPathEditInfoWrapper> arrInterPath;
			arrInterPath.push_back(wr);
			mapUrbanPath.insert({ wr.pInfo->emInterMode, arrInterPath });
		}
		else
		{
			it->second.push_back(wr);
		}		
	}

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem = nullptr;

	for(std::map<KEMInterMode, std::vector<KInterPathEditInfoWrapper>>::iterator it = mapUrbanPath.begin();
		it != mapUrbanPath.end(); ++it)
	{
		std::vector<KInterPathEditInfoWrapper>& arrInterPath = it->second;
		for(KInterPathEditInfoWrapper& wr : arrInterPath)
		{
			if (NULL == wr.pInfo)
			{
				continue;
			}

			KInterPathEditInfo* pInfo = wr.pInfo;

			pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strDestNode));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emInterMode == KEMInterModeAutoTaxi)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("승용차/택시")));
			}
			else if (pInfo->emInterMode == KEMInterModeBus)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스")));
			}
			else if (pInfo->emInterMode == KEMInterModeRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("도시철도")));
			}
			else if (pInfo->emInterMode == KEMInterModeBusRail)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("버스+도시철도")));
			}
			else if (pInfo->emInterMode == KEMInterModeParkRide)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Park & Ride")));
			}
			else if (pInfo->emInterMode == KEMInterModeKissRide)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Kiss & Ride")));
			}
			else if (pInfo->emInterMode == KEMInterModeTaxiTransit)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("택시 & 대중교통")));
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("알수없음")));
			}
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strPathName));
			pItem->SetAlignment(DT_CENTER);

			if (pInfo->emInterMode == KEMInterModeAutoTaxi)
			{
				if (pInfo->emInterModalPathMode == KEMInterModalPathModeUrban)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTAutoSubData.dTravelTime, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dAutoTime, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
				}
				
				
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 차내통행시간
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 대기시간
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 환승시간
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 출발 접근시간
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 도착 접근시간
				pItem->SetAlignment(DT_CENTER);
				if (pInfo->emInterModalPathMode == KEMInterModalPathModeUrban)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTAutoSubData.dTravelLength, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
					if (pInfo->oTAutoSubData.dAutoCost >= pInfo->oTAutoSubData.dTaxiCost)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTAutoSubData.dAutoCost, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
					}
					else
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTAutoSubData.dTaxiCost, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
					}

					if (!m_bPathEdit)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTAutoSubData.dTrip, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
					}
				}
				else
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 통행거리
					pItem->SetAlignment(DT_CENTER);
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dAutoCost, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
					if (!m_bPathEdit)
					{
						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dTrip, _T("%.2f")));
						pItem->SetAlignment(DT_CENTER);
					}
				}
			}
			else if (pInfo->emInterMode == KEMInterModeBus || pInfo->emInterMode == KEMInterModeBusRail)
			{
				double dTotalTime = GetTotalTime(pInfo);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				if (!m_bPathEdit)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTrip, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
				}
			}
			else if(pInfo->emInterMode == KEMInterModeRail)
			{
				double dTotalTime = GetTotalTime(pInfo);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dInVehTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				if (!m_bPathEdit)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTrip, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
				}
			}
			else
			{
				//pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dVehTime, _T("%.2f")));
				//pItem->SetAlignment(DT_CENTER);
				double dTotalTime = GetTotalTime(pInfo);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTotalTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dTravelTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dWaitTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dTransferTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dAccessWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dEgressWalkTime, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dLength, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dVehCost, _T("%.2f")));
				pItem->SetAlignment(DT_CENTER);
				if (!m_bPathEdit)
				{
					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTMultiSubData.dTrip, _T("%.2f")));
					pItem->SetAlignment(DT_CENTER);
				}
			}
		}
	}
}

//
//void KSelectedPathCompareUrbanDlg::UpdateReportInterPathRegionInfos()
//{
//	std::map<int, std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>> mapMode;
//	for (KInterPathRegionInfoWrapper& wr : m_arrInterRegionPathInfo)
//	{
//		if (NULL == wr.pRegionModePathInfo || NULL == wr.pRegionPathDetail)
//		{
//			continue;
//		}
//
//		int nMode = wr.pRegionModePathInfo->nMode;
//
//		std::map<int, std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>>::iterator itMode =
//			mapMode.find(nMode);
//		if (itMode == mapMode.end())
//		{
//			std::vector<KInterPathRegionInfoWrapper> arr;
//			arr.push_back(wr);
//			std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>> mapInterPath;
//			mapInterPath.insert({ wr.emKey, arr });
//			mapMode.insert({ nMode, mapInterPath });
//		}
//		else
//		{
//			std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>& mapInterPath = itMode->second;
//			std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>::iterator itPath = mapInterPath.find(wr.emKey);
//			if (itPath == mapInterPath.end())
//			{
//				std::vector<KInterPathRegionInfoWrapper> arr;
//				arr.push_back(wr);
//				mapInterPath.insert({ wr.emKey, arr });
//			}
//			else
//			{
//				itPath->second.push_back(wr);
//			}
//		}
//	}
//
//	CXTPReportRecord*     pRecord = nullptr;
//	CXTPReportRecordItem* pItem = nullptr;
//
//	for (std::map<int, std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>>::iterator itMode = mapMode.begin();
//		itMode != mapMode.end(); ++itMode)
//	{
//		std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>& mapPath = itMode->second;
//		for (std::map<EMInterRegionPathKey, std::vector<KInterPathRegionInfoWrapper>>::iterator itPath = mapPath.begin();
//			itPath != mapPath.end(); ++itPath)
//		{
//			std::vector<KInterPathRegionInfoWrapper>& arrPath = itPath->second;
//			for (KInterPathRegionInfoWrapper& wr : arrPath)
//			{
//				if (NULL == wr.pRegionModePathInfo || NULL == wr.pRegionPathDetail)
//				{
//					continue;
//				}
//
//				pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
//
//				CString strCaption;
//				//strCaption.Format(_T("%I64d"), wr.pRegionModePathInfo->nxDZoneID);
//				//pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
//				pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strDestNode));
//				pItem->SetAlignment(DT_CENTER);
//
//				if (NULL == wr.pInfo)
//				{
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.pRegionModePathInfo->strModeName + _T("(주요 경로)")));
//				}
//				else
//				{
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.pRegionModePathInfo->strModeName));
//				}
//				
//				pItem->SetAlignment(DT_CENTER);
//
//				pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strPathName1));
//				pItem->SetAlignment(DT_CENTER);
//
//				KRegionPathDetail* pRegionPathDetail = wr.pRegionPathDetail;
//				if (NULL == wr.pInfo)
//				{
//					// 주요경로인 경우에 대한 처리
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
//					pItem->SetAlignment(DT_CENTER);
//
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
//					pItem->SetAlignment(DT_CENTER);
//					if (wr.pRegionModePathInfo->nMode == REGION_MODE_CODE_AUTO)
//					{
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 차내통행시간
//						pItem->SetAlignment(DT_CENTER);
//						
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 대기시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 환승시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 출발 접근시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 도착 접근시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainLength, _T("%.2f")));	// 통행거리
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainCost, _T("%.2f")));	// 통행비용
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dTrip, _T("%.2f")));	// 통행량
//						pItem->SetAlignment(DT_CENTER);
//					}
//					else
//					{
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 통행시간(승용차/택시)
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainTime, _T("%.2f")));	// 차내통행시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainWaitTime, _T("%.2f")));	// 대기시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainTransferTime, _T("%.2f")));	// 환승시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 출발 접근시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 도착 접근시간
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainLength, _T("%.2f")));	// 통행거리
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dMainCost, _T("%.2f")));	// 통행비용
//						pItem->SetAlignment(DT_CENTER);
//
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(wr.pRegionPathDetail->dTrip, _T("%.2f")));	// 통행량
//						pItem->SetAlignment(DT_CENTER);
//					}
//				}
//				else	// if (NULL == wr.pInfo)
//				{
//					KInterPathEditInfo* pInfo = wr.pInfo;
//
//					//switch (wr.emKey)
//					//{
//					//case EMInterRegionPathKey_AccessAuto:  strCaption = _T("승용차(출발 접근)"); break;
//					//case EMInterRegionPathKey_AccessBus:  strCaption = _T("버스(출발 접근)"); break;
//					//case EMInterRegionPathKey_AccessRail: strCaption = _T("도시철도(출발 접근)"); break;
//					//case EMInterRegionPathKey_AccessBusRail: strCaption = _T("버스+도시철도(출발 접근)"); break;
//					//case EMInterRegionPathKey_EgressAuto:  strCaption = _T("승용차(도착 접근)"); break;
//					//case EMInterRegionPathKey_EgressBus:  strCaption = _T("버스(도착 접근)"); break;
//					//case EMInterRegionPathKey_EgressRail: strCaption = _T("도시철도(도착 접근)"); break;
//					//case EMInterRegionPathKey_EgressBusRail: strCaption = _T("버스+도시철도(도착 접근)"); break;
//					//}
//					
//					if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeAuto)
//					{
//						strCaption.Format(_T("승용차(%s 접근)"), (pInfo->emInterAccEgrType == KEMInterAccEgrTypeAccess ? _T("출발") : _T("도착")));
//					}
//					else if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeBus)
//					{
//						strCaption.Format(_T("버스(%s 접근)"), (pInfo->emInterAccEgrType == KEMInterAccEgrTypeAccess ? _T("출발") : _T("도착")));
//					}
//					else if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeRail)
//					{
//						strCaption.Format(_T("도시철도(%s 접근)"), (pInfo->emInterAccEgrType == KEMInterAccEgrTypeAccess ? _T("출발") : _T("도착")));
//					}
//					else if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeBusRail)
//					{
//						strCaption.Format(_T("버스+도시철도(%s 접근)"), (pInfo->emInterAccEgrType == KEMInterAccEgrTypeAccess ? _T("출발") : _T("도착")));
//					}
//
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
//					pItem->SetAlignment(DT_CENTER);
//
//					pItem = pRecord->AddItem(new CXTPReportRecordItemText(wr.strPathName2));
//					pItem->SetAlignment(DT_CENTER);
//
//					if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeAuto)
//					{
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dAutoTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 차내통행시간
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 대기시간
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 환승시간
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 출발 접근시간
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 도착 접근시간
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 통행거리
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dAutoCost, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						if (m_emCompareOption == SPC_InterModalVolumeAnalysis)
//						{
//							pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRegAutoSubData.dTrip, _T("%.2f")));
//							pItem->SetAlignment(DT_CENTER);
//						}
//					}
//					else if(pInfo->emInterRegDetailMode == KEMInterRegDetailModeBus || pInfo->emInterRegDetailMode == KEMInterRegDetailModeBusRail)
//					{
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 통행시간(승용차/택시)
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dInVehTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dWaitTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTransferTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dAccessWalkTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dEgressWalkTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dLength, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dCost, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						if (m_emCompareOption == SPC_InterModalVolumeAnalysis)
//						{
//							pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTBusSubData.dTrip, _T("%.2f")));
//							pItem->SetAlignment(DT_CENTER);
//						}
//					}
//					else if (pInfo->emInterRegDetailMode == KEMInterRegDetailModeRail)
//					{
//						pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));	// 통행시간(승용차/택시)
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dInVehTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dWaitTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTransferTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dAccessWalkTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dEgressWalkTime, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dLength, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dCost, _T("%.2f")));
//						pItem->SetAlignment(DT_CENTER);
//						if (m_emCompareOption == SPC_InterModalVolumeAnalysis)
//						{
//							pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(pInfo->oTRailSubData.dTrip, _T("%.2f")));
//							pItem->SetAlignment(DT_CENTER);
//						}
//					}
//				}
//			}
//		}
//	}
//}


double KSelectedPathCompareUrbanDlg::GetTotalTime(KInterPathEditInfo* pInfo)
{
	double dTotalTime = 0;

	if (NULL != pInfo)
	{
		if (pInfo->emInterMode == KEMInterModeAutoTaxi)
		{
			dTotalTime = pInfo->oTAutoSubData.dTravelTime;
		}
		else if (pInfo->emInterMode == KEMInterModeBus || pInfo->emInterMode == KEMInterModeBusRail)
		{
			dTotalTime = pInfo->oTBusSubData.dInVehTime + pInfo->oTBusSubData.dWaitTime
				+ pInfo->oTBusSubData.dTransferTime + pInfo->oTBusSubData.dAccessWalkTime
				+ pInfo->oTBusSubData.dEgressWalkTime;
		}
		else if (pInfo->emInterMode == KEMInterModeRail)
		{
			dTotalTime = pInfo->oTRailSubData.dInVehTime + pInfo->oTRailSubData.dWaitTime
				+ pInfo->oTRailSubData.dTransferTime + pInfo->oTRailSubData.dAccessWalkTime
				+ pInfo->oTRailSubData.dEgressWalkTime;
		}
		else if(pInfo->emInterMode == KEMInterModeParkRide || pInfo->emInterMode == KEMInterModeKissRide 
			|| pInfo->emInterMode == KEMInterModeTaxiTransit)
		{
			dTotalTime = pInfo->oTMultiSubData.dVehTime + pInfo->oTMultiSubData.dTravelTime
				+ pInfo->oTMultiSubData.dWaitTime + pInfo->oTMultiSubData.dTransferTime
				+ pInfo->oTMultiSubData.dAccessWalkTime + pInfo->oTMultiSubData.dEgressWalkTime;
		}
	}

	return dTotalTime;
}