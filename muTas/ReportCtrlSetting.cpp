#include "stdafx.h"
#include "ReportCtrlSetting.h"

void KReportCtrlSetting::Default( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction, BOOL bSelectionEnable )
{
	//CXTPReportPaintManager* pPaint = wndReport.GetPaintManager();
	
	#if _XTPLIB_VERSION_PREFIX >= 1511
	wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnResource);
	wndReport.GetToolTipContext()->SetStyle(xtpToolTipResource);
	wndReport.GetPaintManager()->m_pHyperlinkStyle->SetColor(RGB(0,0,255));
	wndReport.GetPaintManager()->m_pHyperlinkStyle->SetTextDecoration(xtpReportTextDecorationNone);
	wndReport.GetPaintManager()->m_pHyperlinkHoverStyle->SetColor(RGB(0,0,255));
	wndReport.GetPaintManager()->m_pHyperlinkHoverStyle->SetTextDecoration(xtpReportTextDecorationUnderline);
	wndReport.SetScrollMode(xtpReportOrientationAll, xtpReportScrollModeSmooth);
	#else
	wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnOffice2007);
	wndReport.GetToolTipContext()->SetStyle(xtpToolTipOffice2007);
	#endif
	
	wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
	wndReport.GetPaintManager()->SetDrawGridForEmptySpace(FALSE);
	wndReport.GetPaintManager()->SetHeaderRowsDividerStyle(xtpReportFixedRowsDividerBold);
	wndReport.GetPaintManager()->SetFooterRowsDividerStyle(xtpReportFixedRowsDividerBold);
	wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;
	wndReport.GetPaintManager()->m_clrHeaderRowsDivider = RGB(255,0,0);
	wndReport.GetPaintManager()->m_clrFooterRowsDivider = RGB(0,0,255);
	wndReport.GetPaintManager()->m_bShowNonActiveInPlaceButton = TRUE;
	wndReport.GetPaintManager()->m_bGrayIfDisable = TRUE;
	wndReport.GetPaintManager()->m_strNoItems = _T("");

	wndReport.SetGroupRowsBold(TRUE);
	wndReport.FocusSubItems   (TRUE);
	wndReport.SetMultipleSelection(FALSE);

	/*if (bEdit == TRUE)
	{
		wndReport.SelectionEnable(TRUE); // Row 선택시 해당 Row 하늘색으로 포커스 가는 효과 삭제
	}
	else
	{
		wndReport.SelectionEnable(bSelectionEnable);
	}*/
    wndReport.SelectionEnable(bSelectionEnable);
	wndReport.EditOnClick(TRUE);
	
	wndReport.AllowEdit(bEdit);	
	wndReport.GetReportHeader()->AllowColumnRemove (bArrowColumAction);
	wndReport.GetReportHeader()->AllowColumnSort   (bArrowColumAction);
	wndReport.GetReportHeader()->AllowColumnReorder(bArrowColumAction);
}

void KReportCtrlSetting::HideHeader( CXTPReportControl& wndReport )
{
    wndReport.ShowHeader(FALSE);
}


void KReportCtrlSetting::AllowColumnsort( CXTPReportControl& wndReport )
{
    wndReport.GetReportHeader()->AllowColumnSort(TRUE);
}


void KReportCtrlSetting::AllowColumnReorder( CXTPReportControl& wndReport )
{
	wndReport.GetReportHeader()->AllowColumnReorder(TRUE);
}


void KReportCtrlSetting::HorizontalNoLine( CXTPReportControl& wndReport )
{
	wndReport.SetGridStyle(FALSE, xtpReportGridNoLines);    
};


void KReportCtrlSetting::VerticalNoLine( CXTPReportControl& wndReport )
{
	wndReport.SetGridStyle(TRUE, xtpReportGridNoLines);
}


void KReportCtrlSetting::SelectionEnableFalse( CXTPReportControl& wndReport )
{
	wndReport.SelectionEnable(FALSE);
}


void KReportCtrlSetting::AutoColumnSizeFalse( CXTPReportControl& wndReport )
{
	wndReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
}


void KReportCtrlSetting::YouXX( CXTPReportControl& wndReport )
{
	Default(wndReport);
	// 네 맘대로....

}


void KReportCtrlSetting::DefaultB( CXTPReportControl& wndReport, BOOL bEdit)
{
	Default(wndReport, bEdit, FALSE, FALSE);
	wndReport.GetReportHeader()->AllowColumnSort(TRUE);
}

// deprecated function: 디폴트 - 에디팅선택, <컬럼액션불가, 줄선택불가>
void KReportCtrlSetting::DefaultC( CXTPReportControl& wndReport, BOOL bEdit)
{
	Default(wndReport, bEdit, FALSE, FALSE);
}

// deprecated function: 
void KReportCtrlSetting::DefaultD( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);
}


void KReportCtrlSetting::DefaultE( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction)
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);
	wndReport.GetReportHeader()->SetAutoColumnSizing(bArrowColumAction);
}


void KReportCtrlSetting::DefaultF( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.CanCopy();
}


void KReportCtrlSetting::DefaultTree( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.ShowHeader(FALSE);
	wndReport.SetTreeIndent(10);
}

void KReportCtrlSetting::DefaultTreeStructuerDotSize( CXTPReportControl& wndReport, int nSize, BOOL bEdit/*=FALSE*/, BOOL bArrowColumAction/*=FALSE*/ )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureDots;
	wndReport.SetTreeIndent(nSize);
}


void KChartCtrlSetting::DefaultCtrl( CXTPChartControl &wndChart )
{
	wndChart.EnableToolTips(TRUE);
}


void KChartCtrlSetting::DefaultContent(CXTPChartContent* pContent, CString strTitle)
{
	pContent->GetLegend()->SetVisible(TRUE); // 범례? 창 표출 여부
	pContent->GetLegend()->SetVerticalAlignment(xtpChartLegendNear);         //default : xtpChartLegendNear
	pContent->GetLegend()->SetHorizontalAlignment(xtpChartLegendFar); //default : xtpChartLegendFarOutside
	pContent->GetLegend()->SetDirection(xtpChartLegendTopToBottom);          //default : xtpChartLegendTopToBottom - Legend 리스트 나열 방식 (좌->우, 상->하)

// 
// 	xtpChartLegendNearOutside,       //The legend is placed out side of the chart, near the primary Y axis.
// 		xtpChartLegendNear,              //The legend is placed inside of the chart, near the primary Y axis.
// 		xtpChartLegendCenter,            //The legend is placed at the center of the chart.
// 		xtpChartLegendFar,               //The legend is placed inside of the chart, near the secondary Y axis.
// 		xtpChartLegendFarOutside,        //The legend is placed outside of the chart, near the secondary Y axis.


	CXTPChartTitleCollection*  pTitleCollection = pContent->GetTitles();
	CXTPChartTitle*            pTitle           = pTitleCollection->Add(new CXTPChartTitle());
	pTitle->SetText( strTitle );			 // 제목
	
}


void KChartCtrlSetting::LeftLegend(CXTPChartContent* pContent)
{
	//pContent->GetLegend()->SetVerticalAlignment(xtpChartLegendNear);         //default : xtpChartLegendNear
	pContent->GetLegend()->SetHorizontalAlignment(xtpChartLegendNear); //default : xtpChartLegendFarOutside
	// 
	// 	xtpChartLegendNearOutside,       //The legend is placed out side of the chart, near the primary Y axis.
	// 		xtpChartLegendNear,              //The legend is placed inside of the chart, near the primary Y axis.
	// 		xtpChartLegendCenter,            //The legend is placed at the center of the chart.
	// 		xtpChartLegendFar,               //The legend is placed inside of the chart, near the secondary Y axis.
	// 		xtpChartLegendFarOutside,        //The legend is placed outside of the chart, near the secondary Y axis.
}


void KChartCtrlSetting::BottomLegend(CXTPChartContent* pContent)
{
	pContent->GetLegend()->SetVerticalAlignment(xtpChartLegendFar);         //default : xtpChartLegendNear
	//pContent->GetLegend()->SetHorizontalAlignment(xtpChartLegendNear); //default : xtpChartLegendFarOutside
	// 
	// 	xtpChartLegendNearOutside,       //The legend is placed out side of the chart, near the primary Y axis.
	// 		xtpChartLegendNear,              //The legend is placed inside of the chart, near the primary Y axis.
	// 		xtpChartLegendCenter,            //The legend is placed at the center of the chart.
	// 		xtpChartLegendFar,               //The legend is placed inside of the chart, near the secondary Y axis.
	// 		xtpChartLegendFarOutside,        //The legend is placed outside of the chart, near the secondary Y axis.
}


void KChartCtrlSetting::DefaultSeriesRadar(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);
	pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
	CXTPChartSeriesStyle*      pStyle   = pSeries->SetStyle(new CXTPChartRadarAreaSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel   = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(135);		 // jyk: 각도 조절을 해보려 했으나 이방법으로 실패

}


void KChartCtrlSetting::DefaultSeriesBar(CXTPChartSeries* pSeries, CString strSeriesName, BOOL a_bColorEach)
{
	pSeries->SetName(strSeriesName);
	//pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
	CXTPChartSeriesStyle*      pStyle        = pSeries->SetStyle(new CXTPChartBarSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel        = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	CXTPChartBarSeriesLabel*   pBarLabel     = (CXTPChartBarSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(135);
	pBarLabel->SetPosition(xtpChartBarLabelTop); // xtpChartBarLabelCenter
	pStyle->SetColorEach(a_bColorEach);
	//pStyle->SetColor(CXTPChartColor(0,255,0));
}


void KChartCtrlSetting::DefaultSeriesArea(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);
	CXTPChartSeriesStyle*      pStyle   = pSeries->SetStyle(new CXTPChartAreaSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel   = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(45);
}


void KChartCtrlSetting::DefaultSeriesPie(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);

	CXTPChartPieSeriesStyle* pStyle = new CXTPChartPieSeriesStyle();
	pSeries->SetStyle(pStyle);

	CXTPChartPieSeriesLabel* pLabel = (CXTPChartPieSeriesLabel*)pStyle->GetLabel();

	pLabel->SetPosition((XTPChartPieLabelPosition)xtpChartPieLabelInside);
// 	xtpChartPieLabelOutside,    // Outside the pie.
// 		xtpChartPieLabelInside,     // Inside the pie.
// 		xtpChartPieLabelRadial,     // Radial label.
// 		xtpChartPieLabelTwoColumns  // 2 Columns
}


void KChartCtrlSetting::DefaultDiagramRadar(CXTPChartRadarDiagram* pDiagram)
{
	ASSERT (pDiagram);
	pDiagram->GetAxisX()->SetGridSpacingAuto(FALSE);
	pDiagram->GetAxisX()->SetGridSpacing(1);
	// 	pDiagram->GetAxisX()->SetInterlaced(FALSE);
	// 	pDiagram->GetAxisY()->SetInterlaced(FALSE);
	pDiagram->GetAxisX()->GetLabel()->SetAngle(360-30);
}


void KChartCtrlSetting::DefaultDiagram2D(CXTPChartDiagram2D* pDiagram2D, CString strTitleX, CString strTitleY)
{
	ASSERT (pDiagram2D);
	pDiagram2D->SetAllowScroll(TRUE);
	pDiagram2D->SetAllowZoom(TRUE);
	// 		pDiagram2D->GetAxisX()->SetGridSpacingAuto(FALSE);
	// 		pDiagram2D->GetAxisX()->SetGridSpacing(1);
	// 		pDiagram2D->GetAxisX()->SetInterlaced(FALSE);
	// 		pDiagram2D->GetAxisY()->SetInterlaced(FALSE);

	//pDiagram2D->GetAxisY()->GetRange()->SetShowZeroLevel(FALSE);
	pDiagram2D->GetAxisX()->GetRange()->SetSideMargins(TRUE);
	pDiagram2D->GetAxisX()->GetLabel()->SetAngle(360-30);
	pDiagram2D->GetAxisX()->GetLabel()->SetAntialiasing(TRUE);// 있고 없고 차이를 못느낀다.

	pDiagram2D->GetAxisX()->GetTitle()->SetVisible(TRUE);
	pDiagram2D->GetAxisX()->GetTitle()->SetText(strTitleX);  
	pDiagram2D->GetAxisY()->GetTitle()->SetVisible(TRUE);
	pDiagram2D->GetAxisY()->GetTitle()->SetText(strTitleY);
}

void KColorPickerSetting::DefaultCtrl(CXTPColorPicker &wndColorPicker, COLORREF clr)
{
	wndColorPicker.SetColor(clr);
	wndColorPicker.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	wndColorPicker.ShowText(FALSE);
	wndColorPicker.SetTheme(xtpControlThemeDefault);
}
