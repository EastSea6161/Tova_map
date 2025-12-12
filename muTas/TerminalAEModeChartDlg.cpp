// TerminalAEModeChartDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TerminalAEModeChartDlg.h"
#include "afxdialogex.h"
#include "..\QBicCommon\ColorRangeGen.h"

namespace TerminalAEModeChartENUM
{
	enum
	{
		_0_COLUMN_MODE = 0,
		_1_COLUMN_VOLUME,
		_2_COLUMN_RATIO
	};

	enum
	{
		MODE_AUTO_TEMPID    = 1,
		MODE_TAXI_TEMPID    = 2,
		MODE_BUS_TEMPID     = 3,
		MODE_SUBWAY_TEMPID  = 4,
		MODE_TRANSIT_TEMPID = 5
	};
}

using namespace TerminalAEModeChartENUM;
// KTerminalAEModeChartDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTerminalAEModeChartDlg, KResizeDialogEx)

KTerminalAEModeChartDlg::KTerminalAEModeChartDlg(TTerminalResultInfo &a_oTTerminalResultInfo, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTerminalAEModeChartDlg::IDD, pParent)
	, m_oTTerminalResultInfo(a_oTTerminalResultInfo)
	, m_pParentWnd(pParent)
{
	ConvertChartData(m_oTTerminalResultInfo);
}

KTerminalAEModeChartDlg::~KTerminalAEModeChartDlg()
{
}

void KTerminalAEModeChartDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                m_ctrlReportAcc);
	DDX_Control(pDX, IDC_REPORT3,               m_ctrlReportEgr);
	DDX_Control(pDX, IDC_CHARTCONTROL2,         m_ctrlChart);
}


BEGIN_MESSAGE_MAP(KTerminalAEModeChartDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KTerminalAEModeChartDlg::OnBnClickedCancel)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KTerminalAEModeChartDlg::OnBnClickedRadioDataType)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KTerminalAEModeChartDlg::OnBnClickedRadioChart)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KTerminalAEModeChartDlg::OnBnClickedCheckLabel)
END_MESSAGE_MAP()


// KTerminalAEModeChartDlg 메시지 처리기입니다.
void KTerminalAEModeChartDlg::ConvertChartData(TTerminalResultInfo &a_oTTerminalResultInfo)
{
	try
	{
		TTerminalChartDataInfo oTTerminalChartDataInfo;
		//acc input
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dAccAuto;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("승용차");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("PassengerCar");
		}
		m_mapModeDataAcc.insert(std::make_pair(MODE_AUTO_TEMPID, oTTerminalChartDataInfo));

		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("택시");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Taxi");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dAccTaxi;
		m_mapModeDataAcc.insert(std::make_pair(MODE_TAXI_TEMPID, oTTerminalChartDataInfo));
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("버스");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Bus");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dAccBus;
		m_mapModeDataAcc.insert(std::make_pair(MODE_BUS_TEMPID,  oTTerminalChartDataInfo));

		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("도시철도");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Rail");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dAccRail;
		m_mapModeDataAcc.insert(std::make_pair(MODE_SUBWAY_TEMPID, oTTerminalChartDataInfo));
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("버스+도시철도");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Bus+Rail");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dAccTransit;
		m_mapModeDataAcc.insert(std::make_pair(MODE_TRANSIT_TEMPID, oTTerminalChartDataInfo));

		//egr input
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dEgrAuto;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("승용차");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("PassengerCar");
		}
		m_mapModeDataEgr.insert(std::make_pair(MODE_AUTO_TEMPID, oTTerminalChartDataInfo));
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("택시");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Taxi");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dEgrTaxi;
		m_mapModeDataEgr.insert(std::make_pair(MODE_TAXI_TEMPID, oTTerminalChartDataInfo));
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("버스");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Bus");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dEgrBus;
		m_mapModeDataEgr.insert(std::make_pair(MODE_BUS_TEMPID,  oTTerminalChartDataInfo));

		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("도시철도");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Rail");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dEgrRail;
		m_mapModeDataEgr.insert(std::make_pair(MODE_SUBWAY_TEMPID, oTTerminalChartDataInfo));
		
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTTerminalChartDataInfo.strModeName = _T("버스+도시철도");
		}
		else {
			oTTerminalChartDataInfo.strModeName = _T("Bus+Rail");
		}
		oTTerminalChartDataInfo.dVolume = a_oTTerminalResultInfo.dEgrTransit;
		m_mapModeDataEgr.insert(std::make_pair(MODE_TRANSIT_TEMPID, oTTerminalChartDataInfo));

		AutoType iter  = m_mapModeDataAcc.begin();
		AutoType itEnd = m_mapModeDataAcc.end();
		double dSumvol(0.0);
		while (iter != itEnd)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iter->second;
			dSumvol += oTTerminalChartDataInfo.dVolume;
			++iter;
		}
		
		iter  = m_mapModeDataAcc.begin();
		itEnd = m_mapModeDataAcc.end();
		while (iter != itEnd)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iter->second;
			if (0.0 == dSumvol) {
				oTTerminalChartDataInfo.dRatio = 0.0;
			} else {
				oTTerminalChartDataInfo.dRatio = oTTerminalChartDataInfo.dVolume / dSumvol * 100.0;
			}
			
			++iter;
		}

		AutoType iterB  = m_mapModeDataEgr.begin();
		AutoType itEndB = m_mapModeDataEgr.end();
		dSumvol = 0.0;
		while (iterB != itEndB)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iterB->second;
			dSumvol += oTTerminalChartDataInfo.dVolume;
			++iterB;
		}

		iterB  = m_mapModeDataEgr.begin();
		itEndB = m_mapModeDataEgr.end();
		while (iterB != itEndB)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iterB->second;
			if (0.0 == dSumvol) {
				oTTerminalChartDataInfo.dRatio = 0.0;
			} else {
				oTTerminalChartDataInfo.dRatio = oTTerminalChartDataInfo.dVolume / dSumvol * 100.0;
			}
			
			++iterB;
		}

	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KTerminalAEModeChartDlg::ResizeComponent()
{
	SetResize(IDC_CHECK_ALL,			  SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,   			  SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_REPORT,				  SZ_TOP_LEFT,       SZ_MIDDLE_LEFT);
	SetResize(IDC_SEPRATOR5,	     	  SZ_MIDDLE_LEFT,    SZ_MIDDLE_LEFT);

	SetResize(IDC_CHECK_ALL2,			  SZ_MIDDLE_LEFT,    SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC5,   			  SZ_MIDDLE_LEFT,    SZ_MIDDLE_LEFT);
	SetResize(IDC_REPORT3,				  SZ_MIDDLE_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR3,	     	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC12,	   	          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO1,	   	          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,	   	          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,	     	          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR4,	     	  SZ_TOP_LEFT,       SZ_BOTTOM_LEFT);
	SetResize(IDC_CHARTCONTROL2,	      SZ_TOP_LEFT,       SZ_BOTTOM_RIGHT);

	SetResize(IDC_CHECK_LABEL,		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO3,		          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO4,		          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_TITLE,	     	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
}

BOOL KTerminalAEModeChartDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (nullptr != m_pParentWnd)
	{
		CRect rectParent;
		m_pParentWnd->GetWindowRect(&rectParent);

		CRect rect;
		GetWindowRect(rect);
		MoveWindow(rectParent.left + rectParent.Width(), rectParent.top + 0, rect.Width(), rect.Height());
	}

	CString strTitle(_T(""));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strTitle.AppendFormat(_T("터미널 노드: %I64d"), m_oTTerminalResultInfo.nxTerminalID);
	}
	else {
		strTitle.AppendFormat(_T("Terminal ID: %I64d"), m_oTTerminalResultInfo.nxTerminalID);
	}
	CString &strTerminalName = m_oTTerminalResultInfo.strTerminalName;
	if (!strTerminalName.IsEmpty())
	{
		strTitle.AppendFormat(_T(" - %s"), strTerminalName);
	}
	SetDlgItemText(IDC_STATIC_TITLE, strTitle);

	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_ALL2, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL, BST_CHECKED);

	KReportCtrlSetting::Default(m_ctrlReportAcc, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportEgr, FALSE, FALSE, FALSE);
 	InitReportHeaderAcc();
	InitReportHeaderEgr();
 	UpdateReportDataAcc();
	UpdateReportDataEgr();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO4);
	DrawChart();

 	ResizeComponent();

	return TRUE;  
}


void KTerminalAEModeChartDlg::InitReportHeaderAcc()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE,        _T("Mode"),   40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교통수단"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(_1_COLUMN_VOLUME,      _T("Volume"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행량"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(_2_COLUMN_RATIO,       _T("Ratio(%)"),  30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("비율(%)"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeChartDlg::InitReportHeaderEgr()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(_0_COLUMN_MODE,        _T("Mode"),   40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교통수단"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(_1_COLUMN_VOLUME,      _T("Volume"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통행량"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(_2_COLUMN_RATIO,       _T("Ratio(%)"),  30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("비율(%)"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeChartDlg::UpdateReportDataAcc()
{
	try
	{
		m_ctrlReportAcc.ResetContent();

		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapModeDataAcc.begin();
		AutoType itEnd = m_mapModeDataAcc.end();
		while (iter != itEnd)
		{
			int                    nTempModeID              = iter->first;
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iter->second;

			pRecord = m_ctrlReportAcc.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTTerminalChartDataInfo.strModeName));
			pItem->SetItemData((DWORD_PTR) nTempModeID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalChartDataInfo.dVolume, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalChartDataInfo.dRatio,  _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		m_ctrlReportAcc.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeChartDlg::UpdateReportDataEgr()
{
	try
	{
		m_ctrlReportEgr.ResetContent();

		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL2) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iter  = m_mapModeDataEgr.begin();
		AutoType itEnd = m_mapModeDataEgr.end();
		while (iter != itEnd)
		{
			int                    nTempModeID              = iter->first;
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iter->second;

			pRecord = m_ctrlReportEgr.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTTerminalChartDataInfo.strModeName));
			pItem->SetItemData((DWORD_PTR) nTempModeID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalChartDataInfo.dVolume, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalChartDataInfo.dRatio,  _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		m_ctrlReportEgr.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeChartDlg::OnBnClickedRadioDataType(UINT nID)
{
	try
	{
		DrawChart();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}


void KTerminalAEModeChartDlg::DrawChart()
{
	try
	{
		bool bVolume(false);
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
			bVolume = true;
		else
			bVolume = false;

		std::vector<TMainTerminal> vecBarChartData;

		TMainTerminal oTMainAcc;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTMainAcc.strMainName = _T("터미널 도착");
		}
		else {
			oTMainAcc.strMainName = _T("Access");
		}

		AutoType iterAcc  = m_mapModeDataAcc.begin();
		AutoType itEndAcc = m_mapModeDataAcc.end();
		while (iterAcc != itEndAcc)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iterAcc->second;

			TTerminalBarChartData oTTerminalBarChartData;
			oTTerminalBarChartData.strModeName = oTTerminalChartDataInfo.strModeName;

			if (bVolume)
				oTTerminalBarChartData.dData = oTTerminalChartDataInfo.dVolume;
			else
				oTTerminalBarChartData.dData = oTTerminalChartDataInfo.dRatio;

			oTMainAcc.vecTerminalChartData.push_back(oTTerminalBarChartData);
			++iterAcc;
		}

		vecBarChartData.push_back(oTMainAcc);

		TMainTerminal oTMainEgr;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oTMainEgr.strMainName = _T("터미널 출발");
		}
		else {
			oTMainEgr.strMainName = _T("Egress");
		}

		AutoType iterEgr  = m_mapModeDataEgr.begin();
		AutoType itEndEgr = m_mapModeDataEgr.end();
		while (iterEgr != itEndEgr)
		{
			TTerminalChartDataInfo &oTTerminalChartDataInfo = iterEgr->second;

			TTerminalBarChartData oTTerminalBarChartData;
			oTTerminalBarChartData.strModeName = oTTerminalChartDataInfo.strModeName;

			if (bVolume)
				oTTerminalBarChartData.dData = oTTerminalChartDataInfo.dVolume;
			else
				oTTerminalBarChartData.dData = oTTerminalChartDataInfo.dRatio;

			oTMainEgr.vecTerminalChartData.push_back(oTTerminalBarChartData);
			++iterEgr;
		}

		vecBarChartData.push_back(oTMainEgr);

		CString strUnit(_T(""));
		if (!bVolume)
		{
			strUnit = _T("%");
		}
		CreateChart(vecBarChartData, strUnit);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeChartDlg::CreateChart(std::vector<TMainTerminal> &a_vecBarChartData, CString a_strUnit)
{
	m_mapChartContent.clear();
	m_mapChartContent.insert(std::make_pair(IDC_RADIO3,   CreateBarChart(a_vecBarChartData, FALSE, FALSE, FALSE)));
	m_mapChartContent.insert(std::make_pair(IDC_RADIO4,   CreatePieChart(a_vecBarChartData, a_strUnit)));

	int nID = GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4);
	OnBnClickedRadioChart(nID);
}

 
CXTPChartContent* KTerminalAEModeChartDlg::CreateBarChart(std::vector<TMainTerminal> &a_vecBarChartData, BOOL a_bRotate, BOOL a_bColorEach, BOOL a_bScaleNumeric)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, _T(""));
	pContent->SetPanelDirection(xtpChartPanelVertical);

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;

		if (a_vecBarChartData.size() > 0)
		{
			CXTPChartDiagram2D*        pDiagramA         = (CXTPChartDiagram2D*)pContent->GetPanels()->Add(new CXTPChartDiagram2D());
			KChartCtrlSetting::DefaultDiagram2D(pDiagramA, _T(""), _T(""));
			pDiagramA->SetRotated(a_bRotate);

			for (size_t i= 0; i< a_vecBarChartData.size(); i++)
			{
				CString &strModelName = a_vecBarChartData[i].strMainName;
				std::vector<TTerminalBarChartData> &vecTerminalBarChartData = a_vecBarChartData[i].vecTerminalChartData;

				pSeries = pSeriesCollection->Add(new CXTPChartSeries());
				KChartCtrlSetting::DefaultSeriesBar(pSeries, strModelName, a_bColorEach);

				CString strXAxis(_T(""));

				for (size_t k= 0; k< vecTerminalBarChartData.size(); k++)
				{
					TTerminalBarChartData  &oTData = vecTerminalBarChartData[k];
					CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(oTData.strModeName, oTData.dData);  
					pSeries->GetPoints()->Add(pChartPoint);
				}

				pSeries->SetDiagram(pDiagramA);
				if (a_bScaleNumeric)
				{
					pSeries->SetArgumentScaleType(xtpChartScaleNumerical);
				}
			}
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KTerminalAEModeChartDlg::CreatePieChart(std::vector<TMainTerminal> &a_vecBarChartData, CString a_strUnit)
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, _T(""));
	pContent->SetPanelDirection(xtpChartPanelHorizontal);

	// Legend 위치 재설정
	pContent->GetLegend()->SetVerticalAlignment(xtpChartLegendFarOutside);         
	pContent->GetLegend()->SetHorizontalAlignment(xtpChartLegendCenter); 
	pContent->GetLegend()->SetDirection(xtpChartLegendLeftToRight);      

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;

		for (size_t i= 0; i< a_vecBarChartData.size(); i++)
		{
			CString                            &strModelName            = a_vecBarChartData[i].strMainName;
			std::vector<TTerminalBarChartData> &vecTerminalBarChartData = a_vecBarChartData[i].vecTerminalChartData;

			CXTPChartPieDiagram*      pDiagramA      = (CXTPChartPieDiagram*)pContent->GetPanels()->Add(new CXTPChartPieDiagram());
			
			CXTPChartTitleCollection* pDiagramTtitles = pDiagramA->GetTitles();
			CXTPChartTitle*           pDiagramTtitle = new CXTPChartTitle();
			pDiagramTtitle->SetText(strModelName);
			pDiagramTtitles->Add(pDiagramTtitle);

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesPie(pSeries, strModelName);

			if (0 == i)// 꼼수로 첫번째 판넬에 대한 Legend는 나오지 않게
			{
				pSeries->SetLegendVisible(FALSE);
			}
			else
			{
				pSeries->SetLegendVisible(TRUE);
			}

			CXTPChartSeriesStyle *pStyle =  pSeries->GetStyle();
			pStyle->SetColorEach(TRUE);

			int nRecordCnt = (int)vecTerminalBarChartData.size();
			KColorRangeGen oColorRangeGen;
			oColorRangeGen.setStartColor(RGB ( 234, 158,  69 ));
			oColorRangeGen.setEndColor  (RGB ( 105,   2,   5 ));
			oColorRangeGen.setSize(nRecordCnt);
			oColorRangeGen.createColorRange();

			BOOL bPie(TRUE);
			CXTPChartSeriesPointCollection* pPoints = pSeries->GetPoints();
			CXTPChartSeriesPoint*           pPoint = nullptr;
			for (size_t k= 0; k< vecTerminalBarChartData.size(); k++)
			{
				TTerminalBarChartData  &oTData = vecTerminalBarChartData[k];
				pPoint = pPoints->Add(CreatPiePoint(oTData.strModeName, (k+1), oTData.dData, 1, bPie));
				if (0.0 == oTData.dData)
				{
					pPoint->SetLabelText(_T(""));
					pPoint->SetTooltipText(_T(""));
				}
				else
				{
					CString strValue(_T(""));
					strValue.Format(_T("%.2f%s"), oTData.dData, a_strUnit);
					pPoint->SetLabelText(strValue);
				}

				CXTPChartColor oChartColor;
				oChartColor.SetFromCOLORREF(oColorRangeGen.getNextColor());
				pPoint->SetColor(oChartColor);
			}

			pSeries->SetDiagram(pDiagramA);
		}
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartSeriesPoint* KTerminalAEModeChartDlg::CreatPiePoint(LPCTSTR lpszLegendText, double nYear, double nValue, double dWidth, BOOL bPie)
{
	CXTPChartSeriesPoint* pPoint = new CXTPChartSeriesPoint(nYear, nValue, dWidth);
	pPoint->SetLegentText(lpszLegendText);
	return pPoint;
}


void KTerminalAEModeChartDlg::OnBnClickedRadioChart(UINT nID)
{
	try
	{
		AutoType iter = m_mapChartContent.find(nID);
		if(iter != m_mapChartContent.end())
		{
			CXTPChartContent* pContent = iter->second;

			if (nullptr == pContent)
			{
				return;
			}

			m_ctrlChart.SetContent(pContent);
			UpdateLabels();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}


void KTerminalAEModeChartDlg::OnBnClickedCheckLabel()
{
	UpdateLabels();
}


void KTerminalAEModeChartDlg::UpdateLabels()
{
	BOOL bChecked = IsDlgButtonChecked(IDC_CHECK_LABEL);

	CXTPChartSeriesCollection* pSeriesCollection = m_ctrlChart.GetContent()->GetSeries();

	for (int i = 0; i < pSeriesCollection->GetCount(); i++)
	{
		CXTPChartPointSeriesStyle* pStyle =  (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());

		pStyle->GetLabel()->SetVisible(bChecked);
	}
}


void KTerminalAEModeChartDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
