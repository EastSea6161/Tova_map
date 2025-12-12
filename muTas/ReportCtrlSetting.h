#pragma once

class KReportCtrlSetting
{
public:
	KReportCtrlSetting()  {}
	~KReportCtrlSetting() {}
public:
	static void Default        (CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE, BOOL bSelectionEnable = TRUE);
    static void HideHeader     (CXTPReportControl& wndReport);
    static void AllowColumnsort(CXTPReportControl& wndReport);
	static void AllowColumnReorder(CXTPReportControl& wndReport);
	static void HorizontalNoLine( CXTPReportControl& wndReport );
	static void VerticalNoLine( CXTPReportControl& wndReport );
	static void SelectionEnableFalse( CXTPReportControl& wndReport );
	static void AutoColumnSizeFalse( CXTPReportControl& wndReport );

	static void YouXX  (CXTPReportControl& wndReport);
	static void DefaultB(CXTPReportControl& wndReport, BOOL bEdit=FALSE);
	static void DefaultC(CXTPReportControl& wndReport, BOOL bEdit=FALSE);
	static void DefaultD(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	static void DefaultE(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	static void DefaultF(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);

	static void DefaultTree(CXTPReportControl& wndReport, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
	static void DefaultTreeStructuerDotSize(CXTPReportControl& wndReport, int nSize, BOOL bEdit=FALSE, BOOL bArrowColumAction=FALSE);
};


class KChartCtrlSetting
{
public:
	KChartCtrlSetting()  {}
	~KChartCtrlSetting() {}
public:
	static void DefaultCtrl        (CXTPChartControl &wndChart);
	static void DefaultContent     (CXTPChartContent* pContent, CString strTitle=_T(""));
	static void LeftLegend         (CXTPChartContent* pContent);
	static void BottomLegend       (CXTPChartContent* pContent);
	static void DefaultSeriesRadar (CXTPChartSeries* pSeries, CString strSeriesName=_T(""));
	static void DefaultSeriesBar   (CXTPChartSeries* pSeries, CString strSeriesName=_T(""), BOOL a_bColorEach=FALSE);
	static void DefaultSeriesArea  (CXTPChartSeries* pSeries, CString strSeriesName=_T(""));
	static void DefaultSeriesPie   (CXTPChartSeries* pSeries, CString strSeriesName=_T(""));
	static void DefaultDiagramRadar(CXTPChartRadarDiagram* pDiagram);
	static void DefaultDiagram2D   (CXTPChartDiagram2D*    pDiagram, CString strTitleX, CString strTitleY);
};


class KColorPickerSetting
{
public:
	KColorPickerSetting()  {}
	~KColorPickerSetting() {}
public:
	static void DefaultCtrl         (CXTPColorPicker &wndColorPicker, COLORREF clr);
};