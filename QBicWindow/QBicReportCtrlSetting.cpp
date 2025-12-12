#include "stdafx.h"
#include "QBicReportCtrlSetting.h"

void QBicReportCtrlSetting::Default( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction, BOOL bSelectionEnable )
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
    wndReport.SelectionEnable(bSelectionEnable); {
        //     m_wndReport.GetPaintManager()->m_clrHighlight = RGB(255, 0, 0); // Report 리스트에서 선택되는 도중의 포커스 색
        //     m_wndReport.GetPaintManager()->m_clrSelectedRow = RGB(255, 0, 0);  // Report 리스트에서 선택되는 도중의 포커스 색이 아니라, 포커스를 잃었을때 최종 선택된 Row의 포커스색
    }
	wndReport.EditOnClick(TRUE);
	
	wndReport.AllowEdit(bEdit);	
	wndReport.GetReportHeader()->AllowColumnRemove (bArrowColumAction);
	wndReport.GetReportHeader()->AllowColumnSort   (bArrowColumAction);
	wndReport.GetReportHeader()->AllowColumnReorder(bArrowColumAction);

	wndReport.HeaderRowsEnableSelection(bSelectionEnable);
}

void QBicReportCtrlSetting::HideHeader( CXTPReportControl& wndReport )
{
    wndReport.ShowHeader(FALSE);
}


void QBicReportCtrlSetting::AllowColumnsort( CXTPReportControl& wndReport )
{
    wndReport.GetReportHeader()->AllowColumnSort(TRUE);
}


void QBicReportCtrlSetting::AllowColumnReorder( CXTPReportControl& wndReport )
{
	wndReport.GetReportHeader()->AllowColumnReorder(TRUE);
}


void QBicReportCtrlSetting::HorizontalNoLine( CXTPReportControl& wndReport )
{
	wndReport.SetGridStyle(FALSE, xtpReportGridNoLines);    
};


void QBicReportCtrlSetting::VerticalNoLine( CXTPReportControl& wndReport )
{
	wndReport.SetGridStyle(TRUE, xtpReportGridNoLines);
}


void QBicReportCtrlSetting::SelectionEnableFalse( CXTPReportControl& wndReport )
{
	wndReport.SelectionEnable(FALSE);
}


void QBicReportCtrlSetting::AutoColumnSizeFalse( CXTPReportControl& wndReport )
{
	wndReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
}


void QBicReportCtrlSetting::YouXX( CXTPReportControl& wndReport )
{
	Default(wndReport);
	// 네 맘대로....

}


void QBicReportCtrlSetting::DefaultZ(CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction, BOOL bSelectionEnable)
{
    Default(wndReport, bEdit, bArrowColumAction, bSelectionEnable);

    /************************************************************************/
    /* ReportCtrl 내에 TabKey를 막기 위한                                    */
    /************************************************************************/
    wndReport.GetPaintManager()->m_bHideSelection = TRUE; // Report Ctrl 자체의 Focus가 Kill 되는경우 Report내의 선택상태바 자체를 안보이게 해준다.
    wndReport.FocusSubItems(FALSE); // 이것을 FALSE로 해야 되는것이 최종 핵심이네.   
}


void QBicReportCtrlSetting::DefaultB( CXTPReportControl& wndReport, BOOL bEdit)
{
	Default(wndReport, bEdit, FALSE, FALSE);
	wndReport.GetReportHeader()->AllowColumnSort(TRUE);
}

// deprecated function: 디폴트 - 에디팅선택, <컬럼액션불가, 줄선택불가>
void QBicReportCtrlSetting::DefaultC( CXTPReportControl& wndReport, BOOL bEdit)
{
	Default(wndReport, bEdit, FALSE, FALSE);
}

// deprecated function: 
void QBicReportCtrlSetting::DefaultD( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);
}


void QBicReportCtrlSetting::DefaultE( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction)
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSmallDots);
	wndReport.GetReportHeader()->SetAutoColumnSizing(bArrowColumAction);
}


void QBicReportCtrlSetting::DefaultF( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.CanCopy();
}


void QBicReportCtrlSetting::DefaultTree( CXTPReportControl& wndReport, BOOL bEdit, BOOL bArrowColumAction )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.ShowHeader(FALSE);
	wndReport.SetTreeIndent(10);
}

void QBicReportCtrlSetting::DefaultTreeStructuerDotSize( CXTPReportControl& wndReport, int nSize, BOOL bEdit/*=FALSE*/, BOOL bArrowColumAction/*=FALSE*/ )
{
	Default(wndReport, bEdit, bArrowColumAction);
	wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureDots;
	wndReport.SetTreeIndent(nSize);
}


void QBicChartCtrlSetting::DefaultCtrl( CXTPChartControl &wndChart )
{
	wndChart.EnableToolTips(TRUE);
}


void QBicChartCtrlSetting::DefaultContent(CXTPChartContent* pContent, CString strTitle)
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


void QBicChartCtrlSetting::LeftLegend(CXTPChartContent* pContent)
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


void QBicChartCtrlSetting::BottomLegend(CXTPChartContent* pContent)
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


void QBicChartCtrlSetting::DefaultSeriesRadar(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);
	pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
	CXTPChartSeriesStyle*      pStyle   = pSeries->SetStyle(new CXTPChartRadarAreaSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel   = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(135);		 // jyk: 각도 조절을 해보려 했으나 이방법으로 실패

}


void QBicChartCtrlSetting::DefaultSeriesBar(CXTPChartSeries* pSeries, CString strSeriesName, BOOL a_bColorEach)
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


void QBicChartCtrlSetting::DefaultSeriesLine(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);
	CXTPChartSeriesStyle*      pStyle   = pSeries->SetStyle(new CXTPChartLineSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel   = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(45);
}


void QBicChartCtrlSetting::DefaultSeriesArea(CXTPChartSeries* pSeries, CString strSeriesName)
{
	pSeries->SetName(strSeriesName);
	CXTPChartSeriesStyle*      pStyle   = pSeries->SetStyle(new CXTPChartAreaSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel   = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(45);
}


void QBicChartCtrlSetting::DefaultSeriesPoint(CXTPChartSeries* pSeries, CString strSeriesName, BOOL a_bColorEach)
{
	pSeries->SetName(strSeriesName);
	//pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
	CXTPChartPointSeriesStyle* pStyle        = (CXTPChartPointSeriesStyle*)pSeries->SetStyle(new CXTPChartPointSeriesStyle());
	CXTPChartPointSeriesLabel* pLabel        = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	CXTPChartBarSeriesLabel*   pBarLabel     = (CXTPChartBarSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(135);
	pBarLabel->SetPosition(xtpChartBarLabelTop); // xtpChartBarLabelCenter
	pStyle->SetColorEach(a_bColorEach);
	//pStyle->SetColor(CXTPChartColor(0,255,0));
	pStyle->GetMarker()->SetSize(8);
}


void QBicChartCtrlSetting::DefaultSeriesStackedBar(CXTPChartSeries* pSeries, CString strSeriesName, BOOL a_bColorEach)
{
	pSeries->SetName(strSeriesName);
	//pSeries->SetArgumentScaleType(xtpChartScaleQualitative/*xtpChartScaleNumerical*/);
	CXTPChartStackedBarSeriesStyle* pStyle    = (CXTPChartStackedBarSeriesStyle*)pSeries->SetStyle(new CXTPChartStackedBarSeriesStyle());
	CXTPChartPointSeriesLabel*      pLabel    = (CXTPChartPointSeriesLabel*)pStyle->GetLabel();
	CXTPChartBarSeriesLabel*        pBarLabel = (CXTPChartBarSeriesLabel*)pStyle->GetLabel();
	pLabel->SetAngle(135);
	pBarLabel->SetPosition(xtpChartBarLabelCenter); // xtpChartBarLabelTop, xtpChartBarLabelCenter
	pStyle->SetColorEach(a_bColorEach);
	//pStyle->SetColor(CXTPChartColor(0,255,0));
}


void QBicChartCtrlSetting::DefaultDiagramRadar(CXTPChartRadarDiagram* pDiagram)
{
	ASSERT (pDiagram);
	pDiagram->GetAxisX()->SetGridSpacingAuto(FALSE);
	pDiagram->GetAxisX()->SetGridSpacing(1);
	// 	pDiagram->GetAxisX()->SetInterlaced(FALSE);
	// 	pDiagram->GetAxisY()->SetInterlaced(FALSE);
	pDiagram->GetAxisX()->GetLabel()->SetAngle(360-30);
}


void QBicChartCtrlSetting::DefaultDiagram2D(CXTPChartDiagram2D* pDiagram2D, CString strTitleX, CString strTitleY)
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




void QBicChartCtrlSetting::UpdateLabels(CXTPChartControl* pctrlChart, BOOL a_bShow)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartPointSeriesStyle* pStyle =  (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());

			pStyle->GetLabel()->SetVisible(a_bShow);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}


void QBicChartCtrlSetting::UpdateAxisX(CXTPChartControl* pctrlChart, BOOL a_bShow)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartPointSeriesStyle* pStyle   = (CXTPChartPointSeriesStyle*)(pSeriesCollection->GetAt(i)->GetStyle());
			CXTPChartDiagram2D*        pDiagram = (CXTPChartDiagram2D*)(pSeriesCollection->GetAt(i)->GetDiagram());
			pDiagram->GetAxisX()->SetVisible(a_bShow);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}


void QBicChartCtrlSetting::UpdateAxisXLabelAngle(CXTPChartControl* pctrlChart, int a_nAngle)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartSeries*    pSeries =  (CXTPChartSeries*)(pSeriesCollection->GetAt(i));

			CXTPChartDiagram2D* pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
			pDiagram2D->GetAxisX()->GetLabel()->SetAngle(a_nAngle);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}


void QBicChartCtrlSetting::UpdateAxisYShowZeroLevel(CXTPChartControl* pctrlChart, BOOL a_bShowZeroLevel)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartSeries*    pSeries =  (CXTPChartSeries*)(pSeriesCollection->GetAt(i));

			CXTPChartDiagram2D* pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
			pDiagram2D->GetAxisY()->GetRange()->SetShowZeroLevel(a_bShowZeroLevel);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}


void QBicChartCtrlSetting::UpdateAxisXSideMargins(CXTPChartControl* pctrlChart, BOOL a_bSideMargins)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartSeries*    pSeries =  (CXTPChartSeries*)(pSeriesCollection->GetAt(i));

			CXTPChartDiagram2D* pDiagram2D = (CXTPChartDiagram2D*)pSeries->GetDiagram();
			pDiagram2D->GetAxisX()->GetRange()->SetSideMargins(a_bSideMargins);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}


void QBicChartCtrlSetting::UpdateMarkerSize(CXTPChartControl* pctrlChart, int a_nSize)
{
	try
	{
		CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

		for (int i = 0; i < pSeriesCollection->GetCount(); i++)
		{
			CXTPChartPointSeriesStyle* pStyle = (CXTPChartPointSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

			pStyle->GetMarker()->SetSize(a_nSize);
		}
	}
	catch (...)
	{
		//TxLogDebugException();	
	}
}

void QBicChartCtrlSetting::UpdateMarkerType(CXTPChartControl* pctrlChart, XTPChartMarkerType emMarkerType)
{
    try
    {
        CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

        for (int i = 0; i < pSeriesCollection->GetCount(); i++)
        {
            CXTPChartPointSeriesStyle* pStyle = (CXTPChartPointSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

            pStyle->GetMarker()->SetType(emMarkerType);
        }
    }
    catch (...)
    {
        //TxLogDebugException();	
    }
}

void QBicChartCtrlSetting::UpdateMarkerBorderVisible(CXTPChartControl* pctrlChart, BOOL a_bVisible)
{
    try
    {
        CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

        for (int i = 0; i < pSeriesCollection->GetCount(); i++)
        {
            CXTPChartPointSeriesStyle* pStyle = (CXTPChartPointSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

            pStyle->GetMarker()->SetBorderVisible(a_bVisible);
        }
    }
    catch (...)
    {
        //TxLogDebugException();	
    }
}

void QBicChartCtrlSetting::UpdateMarkerFillStyle(CXTPChartControl* pctrlChart, XTPChartFillMode emFillMode)
{
    try
    {
        CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

        for (int i = 0; i < pSeriesCollection->GetCount(); i++)
        {
            CXTPChartPointSeriesStyle* pStyle = (CXTPChartPointSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

            pStyle->GetMarker()->GetFillStyle()->SetFillMode(emFillMode);

//             xtpChartFillEmpty = 0,
//             xtpChartFillSolid = xtpChartFillEmpty + 1,
//             xtpChartFillGradient = xtpChartFillSolid + 1,
//             xtpChartFillHatch = xtpChartFillGradient + 1

        }
    }
    catch (...)
    {
        //TxLogDebugException();	
    }
}

void QBicChartCtrlSetting::MarkerVisible(CXTPChartControl* pctrlChart, BOOL a_bVisible)
{
    try
    {
        CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

        for (int i = 0; i < pSeriesCollection->GetCount(); i++)
        {
            CXTPChartPointSeriesStyle* pStyle = (CXTPChartPointSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

            pStyle->GetMarker()->SetVisible(a_bVisible);
        }
    }
    catch (...)
    {
        //TxLogDebugException();	
    }
}

void QBicChartCtrlSetting::ZoomDiagram2D(CXTPChartControl* pctrlChart, int nDiagramIdx, BOOL bZoomX, BOOL bZoomY)
{
    CXTPChartContent* pContent = pctrlChart->GetContent();
    int nDiagramCnt = pContent->GetPanels()->GetCount();
    if (nDiagramCnt > nDiagramIdx) {
        CXTPChartDiagram2D* pDiagram2D = (CXTPChartDiagram2D*)pContent->GetPanels()->GetAt(nDiagramIdx);

        ZoomDiagram2D(pDiagram2D, bZoomX, bZoomY);
    }
}


void QBicChartCtrlSetting::ZoomDiagram2D(CXTPChartDiagram2D* pDiagram2D, BOOL bZoomX, BOOL bZoomY)
{
    pDiagram2D->GetAxisX()->SetAllowZoom(bZoomX);
    pDiagram2D->GetAxisY()->SetAllowZoom(bZoomY);

    if (pDiagram2D->GetSecondaryAxisX()->IsVisible() == TRUE) {
        pDiagram2D->GetSecondaryAxisX()->SetAllowZoom(bZoomX);
    }

    if (pDiagram2D->GetSecondaryAxisY()->IsVisible() == TRUE) {
        pDiagram2D->GetSecondaryAxisY()->SetAllowZoom(bZoomY);
    }
}


void QBicChartCtrlSetting::UpdateLineThickness(CXTPChartControl* pctrlChart, int nThickness)
{
    try
    {
        CXTPChartSeriesCollection* pSeriesCollection = pctrlChart->GetContent()->GetSeries();

        for (int i = 0; i < pSeriesCollection->GetCount(); i++)
        {
            CXTPChartLineSeriesStyle* pStyle = (CXTPChartLineSeriesStyle*)pSeriesCollection->GetAt(i)->GetStyle();

            CXTPChartLineStyle* pLineStyle = pStyle->GetLineStyle();
            pLineStyle->SetThickness(nThickness);
        }
    }
    catch (...)
    {
        //TxLogDebugException();	
    }
}

void QBicColorPickerSetting::DefaultCtrl(CXTPColorPicker &wndColorPicker, COLORREF clr)
{
	wndColorPicker.SetColor(clr);
	wndColorPicker.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	wndColorPicker.ShowText(FALSE);
	wndColorPicker.SetTheme(xtpControlThemeDefault);
}


//#################################################################################################
int QBicReportCtrlUtil::GetHeaderIConID( XTP_NM_REPORTRECORDITEM* pRptRecordItem, int _nIConWidth )
{
    int nID(-1);
    CXTPReportRow* pRow = pRptRecordItem->pRow;

    if (pRow != nullptr) {
        return -1;
    }

    CXTPReportColumn* pColumn = pRptRecordItem->pColumn;
    if (pColumn != nullptr) 
    {
        nID = pColumn->GetIconID(); 
        if (nID == -1)
            return -1;

        bool bShowText(true);
        CXTPReportColumnDisplayOptions* pDispOption = pColumn->GetDisplayOptions(); {
            CXTPReportColumnDisplaySettings* pSetting = pDispOption->Column();
            if ( pSetting->IsShowIcon() == FALSE)
                return -1;

            if (pSetting->IsShowText() == FALSE)
                bShowText = false;
        }

        CRect rect = pColumn->GetRect();
        int   nCaptionWidth(0); 

        if (bShowText == true) {
            CXTPReportControl*      pControl      = pColumn->GetControl();
            CXTPReportPaintManager* pPaintManager = pControl->GetPaintManager();
            CClientDC dc(pControl);
            CXTPFontDC font(&dc, &pPaintManager->m_fontCaption);
            nCaptionWidth = pColumn->GetCaptionWidth(&dc);
        }                               

        int   nIconWidth = _nIConWidth + 1;
        CRect rcIcon; {
            int nAlign = pColumn->GetHeaderAlignment();   

            if (nAlign == DT_LEFT) {
                rcIcon.SetRect(0, 0, nIconWidth, nIconWidth);
            } 
            else if (nAlign == DT_RIGHT) {
                int nWidth = rect.Width();
                if (nWidth <= nCaptionWidth + nIconWidth) {
                    rcIcon.SetRect(0, 0, nIconWidth, nIconWidth);
                } else {
                    int x = (nWidth - (nCaptionWidth + nIconWidth));
                    rcIcon.SetRect(x, 0, x+nIconWidth, nIconWidth);
                }
            } 
            else if (nAlign == DT_CENTER) {
                int nWidth = rect.Width();
                if (nWidth <= nCaptionWidth + nIconWidth) {
                    rcIcon.SetRect(0, 0, nIconWidth, nIconWidth);
                } else {
                    int x = (nWidth - (nCaptionWidth + nIconWidth)) / 2;
                    rcIcon.SetRect(x, 0, x+nIconWidth, nIconWidth);
                }
            } 
            else if (nAlign == xtpColumnIconCenter) {
                int x = rect.Width() - nIconWidth;
                rcIcon.SetRect(x, 0, x+nIconWidth, nIconWidth);
            } 
            else {
                rcIcon.SetRect(0, 0, nIconWidth, nIconWidth);
            }                
        }

        CPoint pt = pRptRecordItem->pt;
        if (rcIcon.PtInRect(pt) == FALSE)
            return -1;
    }

    return nID;
}

void QBicReportCtrlUtil::SelectAllRows(CXTPReportControl &wndReport)
{
    // 전체 선택 효과 (아래 2개 설정이 되었을때 가능함. 특히 SetMultipleSelection(TRUE)
    //wndReport.SelectionEnable(TRUE);
    //wndReport.SetMultipleSelection(TRUE);

    if (wndReport.IsSelectionEnabled() == FALSE || wndReport.IsMultipleSelection() == FALSE) {
        return;
    }

    CXTPReportRows* pRows = wndReport.GetRows();
    for (int i = 0; i < pRows->GetCount(); i++)
    {
        CXTPReportRow* pRow = pRows->GetAt(i);
        pRow->SetSelected(TRUE);
    }

    wndReport.RedrawControl();
}

BOOL QBicReportCtrlUtil::CopyReportToClip(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator/*= _T("\t")*/)
{
    std::set<int> setEmpty;
    return CopyReportToClip(a_wndReport, setEmpty, a_strSeparator);
}

BOOL QBicReportCtrlUtil::CopyReportToClip(CXTPReportControl &a_wndReport, std::set<int> &a_setExcept, LPCTSTR a_strSeparator/*= _T("\t")*/)
{
    CXTPReportSelectedRows* pSelectedRows = a_wndReport.GetSelectedRows();
    const int nSelRowCount = pSelectedRows->GetCount();

    if (nSelRowCount < 1) {
        return FALSE;
    }

    CWaitCursor cw; {
        // 부하를 준다.
        for (int i = 0; i < 1000000; i++)
        {
            CString strTest(_T("addd"));
            strTest.MakeUpper();
        }
    }

    CString strResult(_T(""));
    /************************************************************************/
    /* 파일 선택 Row 내용 꺼내오기                                           */
    /************************************************************************/

    // 1. 헤더 정보
    WriteReportHead(a_wndReport, a_strSeparator, a_setExcept, OUT strResult);

    // 2. 본문 내용
    WriteReportData(a_wndReport, a_strSeparator, a_setExcept, OUT strResult);

    // 3. 클립보드로 복사
    try
    {
        if (a_wndReport.OpenClipboard() == TRUE) {

            EmptyClipboard();

            int     nLen = (strResult.GetLength() + 1) * sizeof(wchar_t); //int     nLen = strResult.GetLength() * sizeof(TCHAR) + 1;
            HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen);
            LPBYTE  pGlobalData = (LPBYTE)GlobalLock(hGlobal);

            USES_CONVERSION_EX;
            CopyMemory(pGlobalData, T2CW_EX(strResult, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);

            SetClipboardData(CF_UNICODETEXT, hGlobal);

            GlobalUnlock(hGlobal);
            GlobalFree(hGlobal);

            CloseClipboard();
            return TRUE;
        }
    }
    catch (...) {
    }

    CloseClipboard();
    return FALSE;
}

void QBicReportCtrlUtil::WriteReportHead(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator, std::set<int> &a_setExcept, OUT CString &a_strHead)
{
    try
    {
        int nCIdx(0);

        CXTPReportColumns* pColumns = a_wndReport.GetColumns();
        for (int i = 0; i < pColumns->GetCount(); i++)
        {
            if (a_setExcept.find(i) != a_setExcept.end()) {
                continue;
            }

            CXTPReportColumn* pColumn = pColumns->GetAt(i);

            if (0 == nCIdx) {
                a_strHead.Append(pColumn->GetCaption());
            }
            else { // 구분자가 포함된
                a_strHead.AppendFormat(_T("%s%s"), a_strSeparator, pColumn->GetCaption());
            }

            nCIdx++;
        }

        a_strHead.Append(_T("\r\n"));
    }
    catch (...) {
    }
}

void QBicReportCtrlUtil::WriteReportData(CXTPReportControl &a_wndReport, LPCTSTR a_strSeparator, std::set<int> &a_setExcept, OUT CString &a_strData)
{
    try
    {
        CXTPReportSelectedRows* pSelectedRows = a_wndReport.GetSelectedRows();
        const int nSelRowCount = pSelectedRows->GetCount();

        for (int r = 0; r < nSelRowCount; r++)
        {
            CXTPReportRow*     pRow = pSelectedRows->GetAt(r);
            CXTPReportRecord*  pRecord = pRow->GetRecord();

            int nCIdx(0);

            CXTPReportColumns* pColumns = a_wndReport.GetColumns();
            for (int i = 0; i < pColumns->GetCount(); i++)
            {
                if (a_setExcept.find(i) != a_setExcept.end()) {
                    continue;
                }

                if (nCIdx == 0) {
                    a_strData.Append(pRecord->GetItem(i)->GetCaption());
                }
                else {
                    a_strData.AppendFormat(_T("%s%s"), a_strSeparator, pRecord->GetItem(i)->GetCaption());
                }

                nCIdx++;
            }

            a_strData.Append(_T("\r\n"));
        }
    }
    catch (...) {
    }
}

void QBicTrackCtrlSetting::DefaultCtrl(CXTPTrackControl& wndTrack)
{
    wndTrack.GetReportHeader()->SetAutoColumnSizing(FALSE);
    wndTrack.m_bSortedDragDrop = TRUE;
    wndTrack.AllowEdit(TRUE);
    wndTrack.SetMultipleSelection(FALSE);
    wndTrack.SelectionEnable(FALSE);
    wndTrack.EnableMarkup(TRUE);
    //wndTrack.GetColumns()->GetFirstVisibleColumn()->SetAutoSize(TRUE);
    //wndTrack.m_bFreeHeightMode = TRUE;
    //wndTrack.EnableDragDrop(_T("DialogTrackView"), xtpReportAllowDrag | xtpReportAllowDrop);
    wndTrack.m_bShowWorkArea = FALSE;

    wndTrack.GetReportHeader()->SetLastColumnExpand(TRUE, TRUE);
    wndTrack.GetReportHeader()->SetAutoColumnSizing(TRUE);

    wndTrack.GetReportHeader()->AllowColumnRemove(FALSE);
    wndTrack.GetReportHeader()->AllowColumnSort(FALSE);
    wndTrack.GetReportHeader()->AllowColumnReorder(FALSE);
}
