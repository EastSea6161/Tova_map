// ViewChartDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ViewChartDlg.h"
#include "afxdialogex.h"


// KViewChartDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KViewChartDlg, KResizeDialogEx)

KViewChartDlg::KViewChartDlg(std::vector<TDesireMapData> &a_vecDesireMapData, TDesireODInfo &a_oTDesireODInfo, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KViewChartDlg::IDD, pParent)
	, m_vecDesireMapData(a_vecDesireMapData)
	, m_oTDesireODInfo(a_oTDesireODInfo)
{
}

KViewChartDlg::~KViewChartDlg()
{
}

void KViewChartDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHARTCONTROL, m_wndChartControl);
}


BEGIN_MESSAGE_MAP(KViewChartDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO3, &KViewChartDlg::OnBnClickedRadioChart)
	ON_BN_CLICKED(IDCANCEL, &KViewChartDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KViewChartDlg 메시지 처리기입니다.


BOOL KViewChartDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KChartCtrlSetting::DefaultCtrl(m_wndChartControl);

	m_mapPChartContent.insert(std::make_pair(IDC_RADIO1,   CreateRadarChart()));
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO2,   CreateBarChart()));
	m_mapPChartContent.insert(std::make_pair(IDC_RADIO3,   CreateAreaChart()));

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
	int nID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);
	OnBnClickedRadioChart(nID);

	ResizeComponent();

	CRect rect;
	GetWindowRect(rect);
	MoveWindow(rect.left, rect.top, rect.Width()+300, rect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


CXTPChartContent* KViewChartDlg::CreateRadarChart()
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, CreateTitle());
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartRadarDiagram*     pDiagram          = nullptr;

		CString strOZone(_T(""));
		for (size_t i= 0; i< m_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin            = m_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination     = m_vecDesireMapData[i].TVecDestination;

			strOZone.Format(_T("%I64d:%s"), oTOrigin.TID, oTOrigin.TName);

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesRadar(pSeries, oTOrigin.TName);

			pDiagram = (CXTPChartRadarDiagram*)pSeries->GetDiagram();
			KChartCtrlSetting::DefaultDiagramRadar(pDiagram);

			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];
				
				CString strXAxisZoneID;
				strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);

				CXTPChartAxisCustomLabel* pXAxisLabel = new CXTPChartAxisCustomLabel();
				pXAxisLabel->SetAxisValue(strXAxisZoneID);

				CString strXAxisZoneIDLegend;
				strXAxisZoneIDLegend.Format(_T("%I64d:%s"), oTDesireDest.TID, oTDesireDest.TName);				
				pXAxisLabel->SetText(strXAxisZoneIDLegend);

				pDiagram->GetAxisX()->GetCustomLabels()->Add(pXAxisLabel);
			}
		}
	
		//------------------------------------------
		// 	pDiagram->GetAxisX()->SetInterlaced(FALSE);
		// 	pDiagram->GetAxisY()->SetInterlaced(FALSE);
		// 
		// 	pSeries->SetArgumentScaleType(xtpChartScaleNumerical);
		// 	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(1, 8));
		// 	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(2, 9));
		// 	pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(3, 4));
		// 
		// 	pDiagram->GetAxisX()->GetLabel()->GetFormat()->SetPattern(_T("%.f"));

		// 	for (int nMonth = 1; nMonth <= 12; nMonth++)
		// 	{
		// 		pSeries->GetPoints()->Add(new CXTPChartSeriesPoint(COleDateTime(2012, nMonth, 1, 0, 0, 0), 50 + (rand() % 50)));
		// 	}
		// 
		// 	pDiagram->GetAxisX()->GetLabel()->GetFormat()->SetDateTimeFormat(_T("MMMM"));
		// 	pDiagram->GetAxisX()->GetLabel()->GetFormat()->SetDateTimeCategory(xtpChartDateTimeCustom);	
		// 	pDiagram->GetAxisX()->SetDateTimeScaleUnit(xtpChartScaleMonth);
		// 
		// 	pDiagram->GetAxisX()->SetGridSpacingAuto(FALSE);
		// 	pDiagram->GetAxisX()->SetGridSpacing(1);

#pragma region Custom Code
		// Nombar Test Code Start
		// 	pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
		// 	pDiagram->GetAxisX()->SetGridSpacingAuto(FALSE);
		// 	pDiagram->GetAxisX()->SetGridSpacing(1);
		// 
		// 	CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(1, 100); 
		// 	pSeries->GetPoints()->Add(pChartPoint);
		// 
		// 	for (int i=50; i<53; i++)
		// 	{  
		// 		CString strZone;
		// 		strZone.Format(_T("%d"), i);
		// 
		// 		CXTPChartSeriesPoint* pChartPoint = new CXTPChartSeriesPoint(i, i);  
		// 
		// 		CString strLegend;
		// 		strLegend.Format(_T("%d 존지존"), i);
		// 		//pChartPoint->SetLegentText(strLegend);
		// 
		// 		CXTPChartAxisCustomLabel* pLabel = new CXTPChartAxisCustomLabel();
		// 		pLabel->SetAxisValue(strZone);
		// 		pLabel->SetText(strLegend);
		// 		pDiagram->GetAxisX()->GetCustomLabels()->Add(pLabel);
		// 		pSeries->GetPoints()->Add(pChartPoint);
		// 		//delete pChartPoint;
		// 	}

		//pDiagram->GetAxisX()->GetLabel()->SetFormat(_T("%.0f nombara"));
#pragma endregion Custom Code
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KViewChartDlg::CreateBarChart()
{
	/*CXTPChartContent*          pContent         = m_wndChartControl.GetContent();*/
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent( pContent, CreateTitle() );
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartDiagram2D*        pDiagram2D        = nullptr;

		CString strOZone(_T(""));
		for (size_t i= 0; i< m_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin                 = m_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination          = m_vecDesireMapData[i].TVecDestination;

			strOZone.Format(_T("%I64d:%s"), oTOrigin.TID, oTOrigin.TName);

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesBar(pSeries, oTOrigin.TName);
			
			CString strXAxisZoneID(_T(""));
			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];
				CString      &strName           = oTDesireDest.TName;
				//strXAxisZoneID.Format(_T("%I64d\r\n%s"), oTDesireDest.TID, oTDesireDest.TName); // 라벨 각도가 없을 경우 멀티 라인으로
				if(strName.Compare(_T("")) != 0)
					strXAxisZoneID.Format(_T("%s : %I64d"), oTDesireDest.TName, oTDesireDest.TID);  // 라벨 각도가 있을 경우 아이디를 뒷쪽으로
				else
					strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);
			}
		}

		pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
		KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("Destination"), m_oTDesireODInfo.strFieldName);
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CXTPChartContent* KViewChartDlg::CreateAreaChart()
{
	CXTPChartContent* pContent = new CXTPChartContent();
	KChartCtrlSetting::DefaultContent(pContent, CreateTitle());

	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pContent->GetSeries();
		CXTPChartSeries*           pSeries           = nullptr;
		CXTPChartDiagram2D*        pDiagram2D        = nullptr;

		CString strOZone(_T(""));
		for (size_t i= 0; i< m_vecDesireMapData.size(); i++)
		{
			TDesireOrigin  &oTOrigin            = m_vecDesireMapData[i].TOrigin;
			auto           &vecTDestination     = m_vecDesireMapData[i].TVecDestination;

			strOZone.Format(_T("%I64d:%s"), oTOrigin.TID, oTOrigin.TName);

			pSeries = pSeriesCollection->Add(new CXTPChartSeries());
			KChartCtrlSetting::DefaultSeriesArea(pSeries, oTOrigin.TName);

			CString strXAxisZoneID(_T(""));
			for (size_t k= 0; k< vecTDestination.size(); k++)
			{
				TDesireDest  &oTDesireDest      = vecTDestination[k];
				CString      &strName           = oTDesireDest.TName; 
				
		      //strXAxisZoneID.Format(_T("%I64d\r\n%s"), oTDesireDest.TID, oTDesireDest.TName); // 라벨 각도가 없을 경우 멀티 라인으로
				if(strName.Compare(_T("")) != 0)
					strXAxisZoneID.Format(_T("%s : %I64d"), oTDesireDest.TName, oTDesireDest.TID);  // 라벨 각도가 있을 경우 아이디를 뒷쪽으로
				else
					strXAxisZoneID.Format(_T("%I64d"), oTDesireDest.TID);
				

				CXTPChartSeriesPoint*     pChartPoint = new CXTPChartSeriesPoint(strXAxisZoneID, oTDesireDest.TValue);  
				pSeries->GetPoints()->Add(pChartPoint);
			}
		}

		pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
		KChartCtrlSetting::DefaultDiagram2D(pDiagram2D, _T("Destination"), m_oTDesireODInfo.strFieldName);
	}
	catch (...)
	{
		return nullptr;
	}

	return pContent;
}


CString KViewChartDlg::CreateTitle()
{
	CString strTitle(_T(""));
	strTitle.Format(_T("%s : %s"), m_oTDesireODInfo.strTableName, m_oTDesireODInfo.strFieldName);
	return strTitle;
}


void KViewChartDlg::ResizeComponent()
{
	SetResize(IDC_CHARTCONTROL,		  SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1,		  SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_RADIO1,		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO3,		      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,				  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KViewChartDlg::OnBnClickedRadioChart(UINT nID)
{
	try
	{
		AutoType iter = m_mapPChartContent.find(nID);
		if(iter != m_mapPChartContent.end())
		{
			CXTPChartContent* pContent = iter->second;
			m_wndChartControl.SetContent(pContent);
		}
	}
	catch (...)
	{
		
	}
}


void KViewChartDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
