/**
* @file IOTableView.cpp
* @brief KIOTableView 구현 파일
* @author 
* @date 2011.03.31
* @remark
*/



#include "stdafx.h"
#include "KmzApp.h"
#include "IOTableView.h"
#include "DisplayColumnSetDlg.h"
#include "ChildFrm.h"
#include "IOTableController.h"
#include "IOTableRecord.h"
#include "MapView.h"
#include "KStatisticsDlg.h"
#include "StatisticsBaseDefineDlg.h"
#include "MultiStatisticsBaseDefineDlg.h"
#include "ApplyModeChoiceModelDataDlg.h"
#include "ApplyDistributionModelDataDlg.h"
#include "ApplyGenerationModelDataDlg.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "Target.h"
#include "Project.h"


IMPLEMENT_DYNCREATE(KIOTableView, CXTPReportView)

enum {
	MENU_DEFAULT = 0,
	MENU_PURPOSE = 1,
	MENU_MODE,
	MENU_PA
};

KIOTableView::KIOTableView()
	:	m_pTable(NULL),
		m_pRecord(NULL),
		m_pRecordset(NULL),
		m_pController(NULL),
		m_strFilter(_T("")),
		m_strOrder(_T("")),
		m_bApplyFilter(false),
		m_nTableDisplayType(0),
		m_nPageRecordCount(1000),
		m_nPageCount(1),
		m_nCurPage(1),
		m_nFilterPageCount(1),
		m_nFilterCurPage(1)
{
	m_pMapView = NULL;
}

KIOTableView::~KIOTableView()
{
	if(NULL != m_pTable)
	{
		m_pTable->Unregister(this);
	}

	if(NULL != m_pRecordset)
	{
		delete m_pRecordset;
	}
	m_pRecordset = NULL;
}



BEGIN_MESSAGE_MAP(KIOTableView, CXTPReportView)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND_RANGE(ID_TABLEVIEW_MENU, ID_TABLEVIEW_RECORDVALUE, &KIOTableView::OnTableview)
	ON_UPDATE_COMMAND_UI(ID_TABLEVIEW_RECORDVALUE, &KIOTableView::OnUpdateTableviewRecordvalue)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLEVIEW_SHOWALL, ID_TABLEVIEW_SHOWSELECTED, &KIOTableView::OnUpdateTableViewDisplay)
	ON_COMMAND(ID_TABLEVIEWMENU_DISPLAYCOLUMNSET, &KIOTableView::OnTableviewmenuDisplaycolumnset)
	ON_COMMAND(ID_TABLEVIEWMENU_SELECTBYATTRIBUTES, &KIOTableView::OnTableviewmenuSelectbyattributes)
	ON_COMMAND(ID_TABLEVIEWMENU_QUICKSEARCH, &KIOTableView::OnTableViewMenuQuickSearch)
	ON_COMMAND(ID_TABLEVIEWMENU_CALCULATEFIELD, &KIOTableView::OnTableviewmenuCalculatefield)
	ON_COMMAND(ID_TABLEVIEWMENU_ADD, &KIOTableView::OnTableviewmenuAddField)
	ON_COMMAND(ID_TABLEVIEWMENU_IMPORT, &KIOTableView::OnTableviewmenuImport)
	ON_UPDATE_COMMAND_UI(ID_TABLEVIEWMENU_IMPORT, &KIOTableView::OnUpdateTableviewmenuImport)

	ON_COMMAND(ID_TABLEVIEWMENU_EXPORT, &KIOTableView::OnTableviewmenuExport)
	ON_UPDATE_COMMAND_UI(ID_TABLEVIEWMENU_EXPORT, &KIOTableView::OnUpdateTableviewmenuExport)

	ON_UPDATE_COMMAND_UI(ID_TABLEVIEWMENU_GENERATEGEOMETRY, &KIOTableView::OnUpdateTableviewmenuGenerateGeometery)
	ON_COMMAND(ID_TABLEVIEWMENU_GENERATEGEOMETRY, &KIOTableView::OnTableviewmenuGenerateGeometery)

	ON_COMMAND(ID_TABLEVIEWMENU_ODMATRIX, &KIOTableView::OnTableviewmenuODMatrix)
	ON_UPDATE_COMMAND_UI(ID_TABLEVIEWMENU_ODMATRIX, &KIOTableView::OnUpdateTableviewmenuODMatrix)
	ON_COMMAND(ID_COLUMNMENU_DISPLAYCOLUMNSET, &KIOTableView::OnColumnmenuDisplaycolumnset)

	ON_UPDATE_COMMAND_UI(ID_TABLEMENU_ZONE_DISTANCE, &KIOTableView::OnUpdateTableviewmenuCalculateZoneDist)
	ON_COMMAND(ID_TABLEMENU_ZONE_DISTANCE, &KIOTableView::OnTableviewmenuCalculateZoneDist)

	ON_COMMAND(ID_COLUMNMENU_RENAMECOLUMN, &KIOTableView::OnColumnmenuRenamecolumn)
	ON_COMMAND(ID_COLUMNMENU_DELETECOLUMN, &KIOTableView::OnColumnmenuDeletecolumn)
	ON_COMMAND(ID_COLUMNMENU_EDITCOLUMN,   &KIOTableView::OnColumnmenuEditcolumn)
	ON_UPDATE_COMMAND_UI(ID_COLUMNMENU_RENAMECOLUMN, &KIOTableView::OnUpdateColumnmenuRenamecolumn)
	ON_UPDATE_COMMAND_UI(ID_COLUMNMENU_DELETECOLUMN, &KIOTableView::OnUpdateColumnmenuDeletecolumn)
	ON_UPDATE_COMMAND_UI(ID_COLUMNMENU_EDITCOLUMN,   &KIOTableView::OnUpdateColumnmenuEditcolumn)

	ON_COMMAND(ID_COLUMNMENU_APPLYMODELDATA,  &KIOTableView::OnColumnmenuApplyModelData)
	ON_UPDATE_COMMAND_UI(ID_COLUMNMENU_APPLYMODELDATA, &KIOTableView::OnUpdateColumnmenuApplyModelData)
	ON_COMMAND(ID_COLUMNMENU_STATISTICS_SINGLE, &KIOTableView::OnColumnmenuStatisticsSinglecolumn)
	ON_COMMAND(ID_COLUMNMENU_STATISTICS_MULTI,  &KIOTableView::OnColumnmenuStatisticsMulticolumn)
	ON_COMMAND(ID_RECORD_ZOOMTO, &KIOTableView::OnRecordZoomto)
	ON_COMMAND(ID_RECORD_PANTO, &KIOTableView::OnRecordPanto)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RECORD_ZOOMTO, ID_RECORD_PANTO, &KIOTableView::OnUpdateRecord)
	ON_NOTIFY(NM_RCLICK, XTP_ID_REPORT_CONTROL, &KIOTableView::OnReportRowRButtonClick)
	// DBLClick
	ON_NOTIFY(NM_DBLCLK, XTP_ID_REPORT_CONTROL, &KIOTableView::OnReportRowDblClick)

	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, XTP_ID_REPORT_CONTROL, &KIOTableView::OnReportColumnRButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_SORTORDERCHANGED, XTP_ID_REPORT_CONTROL, &KIOTableView::OnReportSortOrderChanged)
	ON_WM_ACTIVATE()
    ON_COMMAND(ID_RECORD_START_NODE, &KIOTableView::OnRecordStartNode)
    ON_UPDATE_COMMAND_UI(ID_RECORD_START_NODE, &KIOTableView::OnUpdateRecordStartNode)
    ON_COMMAND(ID_RECORD_END_NODE, &KIOTableView::OnRecordEndNode)
    ON_UPDATE_COMMAND_UI(ID_RECORD_END_NODE, &KIOTableView::OnUpdateRecordEndNode)
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_ACCESSLINESTATION_MODIFY, &KIOTableView::OnAccessLineStationModify)
	ON_COMMAND(ID_ACCESSLINESTATION_DELETE, &KIOTableView::OnAccessLineStationDelete)
    ON_COMMAND(ID_TRANSIT_TRANSITLINEINFO, &KIOTableView::OnTransitTransitlineinfo)
	ON_COMMAND(ID_TRANSIT_TRANSITLINE_THEMES, &KIOTableView::OnTransitTransitlineThemes)
	ON_UPDATE_COMMAND_UI(ID_TRANSIT_TRANSITLINE_THEMES, &KIOTableView::OnUpdateTransitTransitlineThemes)
    ON_COMMAND(ID_CHART_TRANSIT, &KIOTableView::OnChartTransit)
	ON_COMMAND(ID_TERMINAL_SCHEDULE_EDIT, &KIOTableView::OnTerminalScheduleEdit)
	ON_COMMAND(ID_TRANSIT_STATIONODCAL, &KIOTableView::OnTransitODCalculator)

	ON_COMMAND(ID_TABLE_CHART_TRANSIT, &KIOTableView::OnTableChartTransit)
	ON_COMMAND(ID_TABLE_TRANSITLINE_THEMES, &KIOTableView::OnTableTransitLineThemes)
	ON_COMMAND(ID_TABLE_TRANSIT_STATIONODCAL, &KIOTableView::OnTableTransitODCalculator)
    ON_COMMAND(ID_TRANSIT_INFLUENCEAREA, &KIOTableView::OnTransitInfluenceArea)

    ON_COMMAND(ID_TABLE_ADDDATA, &KIOTableView::OnTableviewAddData)
    ON_COMMAND(ID_RECORDDELETE_DELETEROW, &KIOTableView::OnTableviewDeleteRowData)    
    ON_COMMAND(ID_TABLE_RESET_DATA, &KIOTableView::OnTableviewTableResetData)   
END_MESSAGE_MAP()


// KIOTableView 그리기입니다.

void KIOTableView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// KIOTableView 진단입니다.

#ifdef _DEBUG
void KIOTableView::AssertValid() const
{
	CXTPReportView::AssertValid();
}

#ifndef _WIN32_WCE
void KIOTableView::Dump(CDumpContext& dc) const
{
	CXTPReportView::Dump(dc);
}
#endif
#endif //_DEBUG



void KIOTableView::SetTable(KIOTable* pTable, int nTableDisplayType)
{
	ASSERT(NULL != pTable);

	m_pTable = pTable;
	m_pTable->Register(this);
	
	m_nTableDisplayType = 0/*nTableDisplayType*/;

	CXTPReportControl& wndReport = GetReportCtrl();    
	KReportCtrlSetting::Default(wndReport, FALSE, TRUE);
	
	CString strTableName = m_pTable->Name();
	if (strTableName.CompareNoCase(TABLE_TRANSIT) == 0) {
		KReportCtrlSetting::SelectionEnableFalse(wndReport);
	} 
	else if (strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0 || // 2016.08.03 명지대 요청으로 OD 테이블은 sort 되지 않도록
		strTableName.CompareNoCase(TABLE_MODE_OD) == 0 ||
		strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0) {
		//wndReport.GetReportHeader()->AllowColumnSort(FALSE);

		m_nTableDisplayType = 1;
	}
   
	ReloadData();
}


void KIOTableView::UpdateView(void)
{
	ReloadData();
}


LPCTSTR KIOTableView::GetTableName(void)
{
	if(NULL != m_pTable)
	{
		return m_pTable->Name();
	}
	return _T("");
}


bool KIOTableView::SetFilter(LPCTSTR strFilter)
{
	m_strFilter = strFilter;
	m_bApplyFilter = true;

	try {
		if (IsPageView() == true) { // 이 페이지가 활성화 되었나 확인
			int nRecordCount = m_pTable->RecordCount(m_strFilter); // 해당 쿼리의 카운트 가져오기

			if (nRecordCount % m_nPageRecordCount == 0) {
				m_nFilterPageCount = nRecordCount / m_nPageRecordCount;
			} else {
				m_nFilterPageCount = nRecordCount / m_nPageRecordCount + 1;
			}

			if (m_nFilterPageCount < 1)
				m_nFilterPageCount = 1;

			if (m_nFilterCurPage > m_nFilterPageCount)
				m_nFilterCurPage = m_nFilterPageCount;
		}

		UpdateRecord();
	}
	catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return true;
	}
	catch (...) {
		TxLogDebugException();
		return true;
	}

	return false;
}


void KIOTableView::SetOriginalFilter( LPCTSTR strFilter )
{
	m_strOriginalFilter = strFilter;
}



void KIOTableView::SetOrder(LPCTSTR strOrder)
{
	m_strOrder = strOrder;
	UpdateRecord();
}


void KIOTableView::SetController(KIOTableController* pController)
{
	m_pController= pController;
}


void KIOTableView::ApplyFilter(bool bApply)
{
	m_bApplyFilter = bApply;
	UpdateRecord();
}


void KIOTableView::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
    try
    {
    	ReloadData();
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

void KIOTableView::ReloadData(void)
{
    try
    {
		if (IsPageView() == true) {
			int nRecordCount = m_pTable->RecordCount();
			if (nRecordCount % m_nPageRecordCount == 0) {
				m_nPageCount = nRecordCount / m_nPageRecordCount;
			} else {
				m_nPageCount = nRecordCount / m_nPageRecordCount + 1;
			}

			if (m_nPageCount < 1)
				m_nPageCount = 1;

			if (m_nCurPage > m_nPageCount)
				m_nCurPage = m_nPageCount;

			if (m_strFilter.GetLength() > 0) {
				int nRecordCount = m_pTable->RecordCount(m_strFilter);
				if (nRecordCount % m_nPageRecordCount == 0) {
					m_nFilterPageCount = nRecordCount / m_nPageRecordCount;
				} else {
					m_nFilterPageCount = nRecordCount / m_nPageRecordCount + 1;
				}

				if (m_nFilterPageCount < 1)
					m_nFilterPageCount = 1;

				if (m_nFilterCurPage > m_nFilterPageCount)
					m_nFilterCurPage = m_nFilterPageCount;
			}
			else {
				m_nFilterPageCount = m_nPageCount;
				m_nFilterCurPage   = m_nCurPage;
			}
		}

        CXTPReportControl& wndReport = GetReportCtrl();

        wndReport.GetColumns()->Clear();
        wndReport.ResetContent(TRUE);
        UpdateColumns(m_pTable->Columns(), wndReport);

        UpdateRecord();
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

void KIOTableView::RegisterMapView(KMapView* pMapView)
{
	m_pMapView = pMapView;
}

void KIOTableView::ActivatedFrameWndNotifyProcess()
{
	if (m_pController != NULL)
	{
		m_pController->ActivatedFrameWndNotifyProcess();
	}
}

void KIOTableView::DeActivatedFrameWndNotifyProcess()
{
	if (m_pController != NULL)
	{
		m_pController->DeActivatedFrameWndNotifyProcess();
	}
}

void KIOTableView::UpdateColumns(const KIOColumns* a_pIOColumns, CXTPReportControl& a_wndReport)
{
	ASSERT(NULL != a_pIOColumns);

	int nColumnCount   = a_pIOColumns->ColumnCount();
    int nPkColumnCount = 0;

	for(int i = 0; i < nColumnCount; ++i)
	{
        KIOColumn*        pIOColumn     = a_pIOColumns->GetColumn(i);
		CXTPReportColumn* pReportColumn = new CXTPReportColumn(i, a_pIOColumns->GetColumn(i)->Caption(), 70);

        pReportColumn = a_wndReport.AddColumn(pReportColumn);
		pReportColumn->SetInternalName       (pIOColumn->Name());		
		pReportColumn->SetAutoSize           (TRUE);
        pReportColumn->SetHeaderAlignment    (DT_CENTER);
        pReportColumn->SetAlignment          (DT_CENTER);
		pReportColumn->SetSortIncreasing     (FALSE);
		#pragma region nombara_20120926
		if(pIOColumn->Visible() == false)
		{
			pReportColumn->SetVisible(FALSE);
		}	
		#pragma endregion nombara_20120926

        CString strColumnInfo = pIOColumn->ColumnInfo();
        if (strColumnInfo.GetLength() > 0)
            pReportColumn->SetTooltip(strColumnInfo);

        switch ( pIOColumn->DataType() )
        {
        case KEMIODataTypeInteger :
            if (pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
            {
                pReportColumn->SetWidth(100);
            }
        	break;
        case  KEMIODataTypeDouble :
            pReportColumn->SetAlignment(DT_RIGHT);
            break;
        default:
            if (pIOColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
            {
                pReportColumn->SetWidth(150);
            }
            else
            {
                pReportColumn->SetWidth(100);
            }
            
            break;
        }    

        if (pIOColumn->PrimaryKey())
        {
            nPkColumnCount ++;
        }
	}

    a_wndReport.SetFreezeColumnsCount(nPkColumnCount);
}


void KIOTableView::clearColumns(CXTPReportColumns* pColumns)
{
	pColumns->Clear();
}


void KIOTableView::UpdateRecord()
{
	CXTPReportControl& wndReport = GetReportCtrl();
	wndReport.ResetContent(TRUE);

	// KIOTableRecord를 직접 삭제하면 오류 발생한다. Report control 자체적으로 삭제 하는 듯...
	//if(NULL != m_pRecord)
	//{
	//	wndReport.RemoveRecordEx(m_pRecord, TRUE, TRUE);
	//	delete m_pRecord;
	//	m_pRecord = NULL;
	//}

	if (nullptr == m_pTable) {
		wndReport.Populate();
		return;
	}

	if(NULL != m_pRecordset)
	{
        try
        {
            delete m_pRecordset;	
        }
        catch (KExceptionPtr ex)
        {
            TxExceptionPrint(ex);
        }
        catch (...)
        {
        	TxLogDebugException();
        }		
		m_pRecordset = NULL;
	}

	if (IsPageView() == true) {
		if(true == m_bApplyFilter) {
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeStatic, m_pTable->Columns(), _T(""), m_strFilter, m_strOrder, (m_nFilterCurPage-1)*m_nPageRecordCount, m_nPageRecordCount);
		}
		else {
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeStatic, m_pTable->Columns(), _T(""), _T(""), m_strOrder, (m_nCurPage-1)*m_nPageRecordCount, m_nPageRecordCount);
		}
	}
	else {
		if(true == m_bApplyFilter) {
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeDynamic, m_strFilter, m_strOrder);
		}
		else {
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeDynamic, _T(""), m_strOrder);
		}
	}

	if (nullptr == m_pRecordset) {
		wndReport.Populate();
		return;
	}

	KIOTableOwner* pOwner = m_pTable->GetOwner();
	if(NULL != pOwner)
	{
		KCodeManager* pCodeManager = pOwner->GetCodeManager();
		// KIOTableRecord를 직접 삭제하면 오류 발생한다. Report control 자체적으로 삭제 하는 듯...    
		int           nRecordCount = m_pRecordset->GetRowCount();
		m_pRecord = new KIOTableRecord(pCodeManager, m_pRecordset);
		wndReport.SetVirtualMode(m_pRecord, nRecordCount);
	}
	else 
	{
		QBicDelete(m_pRecordset);
	}

	wndReport.Populate();
}


void KIOTableView::setColumnDisplay(void)
{
	KDisplayColumnSetDlg dlg;
	dlg.Table(m_pTable);

	CXTPReportColumns* pColumns = m_wndReport.GetColumns();
	int nColumnCount = pColumns->GetCount();
	for(int i = 0; i < nColumnCount; ++i )
	{
		CXTPReportColumn* pColumn = pColumns->GetAt( i );
		dlg.addColumn( pColumn );
	}

	dlg.DoModal();	
}


// KIOTableView 메시지 처리기입니다.
int KIOTableView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPReportView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CXTPReportControl& wndReport = GetReportCtrl();

	wndReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
	wndReport.GetReportHeader()->AllowColumnRemove  (TRUE);
    wndReport.GetReportHeader()->AllowColumnSort    (TRUE);

	wndReport.SetFullColumnScrolling (TRUE);
	wndReport.ShowRowNumber          (TRUE);
	wndReport.SetGridStyle           (TRUE, xtpReportGridSmallDots);
	wndReport.FocusSubItems          (FALSE);	
    //wndReport.SetCu
	wndReport.SetVirtualMode(NULL, 0);

	return 0;
}


void KIOTableView::OnClose()
{
	if(NULL != m_pMapView)
	{
		m_pMapView->NotifyIOTableViewClosed(m_pTable->Name());
	}

	if(NULL != m_pController)
	{
		m_pController->ViewClosed();
	}

	if(NULL != m_pTable)
	{
		m_pTable->Unregister(this);
	}
	CXTPReportView::OnClose();
}


void KIOTableView::OnTableview(UINT nID)
{
	if(ID_TABLEVIEW_SHOWALL == nID)
	{
		ApplyFilter(false);
	}
	else if(ID_TABLEVIEW_SHOWSELECTED == nID)
	{
		ApplyFilter(true);
	}
}


void KIOTableView::OnUpdateTableViewDisplay(CCmdUI* pCmdUI)
{
	if(ID_TABLEVIEW_SHOWALL == pCmdUI->m_nID)
	{
		pCmdUI->SetCheck(m_bApplyFilter == false ? TRUE : FALSE);
	}
	else if(ID_TABLEVIEW_SHOWSELECTED == pCmdUI->m_nID)
	{
		pCmdUI->SetCheck(m_bApplyFilter == true ? TRUE : FALSE);
	}
}


void KIOTableView::OnUpdateTableviewRecordvalue(CCmdUI *pCmdUI)
{
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) ) {
		CXTPControlLabel* pLabel = (CXTPControlLabel*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		int nTotal  = TableRecordCount();
		int nSelect = ViewRecordCount();

		CString strCaption;
		strCaption.Format(_T("%d / %d"), nSelect, nTotal);

		pLabel->SetCaption(strCaption);
	}

	/*
	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) )
	{
		CXTPControlLabel* pLabel = 
			(CXTPControlLabel*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );

		if(NULL == m_pRecordset)
		{
			pLabel->SetCaption(_T("0 / 0"));
		}
		else
		{
			int nTotal  = m_pTable->RecordCount();
			int nSelect = m_pRecordset->GetRowCount();

			CString strCaption;
			strCaption.Format(_T("%d / %d"), nSelect, nTotal);

			pLabel->SetCaption(strCaption);
		}
	}
	*/
}


void KIOTableView::OnTableviewmenuDisplaycolumnset()
{
	setColumnDisplay();
}


void KIOTableView::OnTableviewmenuSelectbyattributes()
{
	if(NULL != m_pController)
	{
		m_pController->SelectRow(m_strOriginalFilter);
	}
}

void KIOTableView::OnTableViewMenuQuickSearch()
{
	if (m_pController != nullptr)
	{
		m_pController->QuickSearch();
	}
}


void KIOTableView::OnTableviewmenuCalculatefield()
{
	if(NULL != m_pController)
	{
        if(m_bApplyFilter)
        {
            m_pController->CalculateField(m_strOriginalFilter);
        }
        else
        {
		    m_pController->CalculateField();
        }
	}
}

void KIOTableView::OnTableviewmenuCreategraph()
{
	// TODO:
}

void KIOTableView::OnTableviewmenuAddField()
{
	if(NULL != m_pController)
	{
		m_pController->AddField();
	}
}

void KIOTableView::OnTableviewmenuImport()
{
	if(NULL != m_pController)
	{
		if(m_pController->ImportTable())
		{
			ReloadData();
			m_pMapView->FullExtentMap();
		}
	}
}

void KIOTableView::OnUpdateTableviewmenuImport(CCmdUI* pCmdUI)
{
	CString strTableName = m_pTable->Name(); 

	if(strTableName.CompareNoCase( TABLE_ACCESS_LINE_STATION ) == 0)
	{
		pCmdUI->Enable(FALSE);
	}
}

void KIOTableView::OnTableviewmenuExport()
{
	if(NULL != m_pController)
	{
		if (m_bApplyFilter)
		{
			m_pController->ExportTable(m_strFilter);
		}
		else
		{
			m_pController->ExportTable();
		}
	}
}

void KIOTableView::OnUpdateTableviewmenuExport(CCmdUI* pCmdUI)
{
	if (nullptr != m_pTable) {
		if (m_pTable->RecordCount() < 1) {
			pCmdUI->Enable(FALSE);
		} else {
			pCmdUI->Enable(TRUE);
		}
	}
}

void KIOTableView::OnTableviewmenuODMatrix()
{
// 	if (NULL != m_pController)
// 	{
// 		m_pController->ViewODMatrix();
// 	}

	try
	{
		CString strTableName = m_pTable->Name();

		m_pMapView->ShowODMatrix(strTableName);
	}
	catch(...)
	{
		TxLogDebugException();
	}

}

void KIOTableView::OnUpdateTableviewmenuODMatrix(CCmdUI* pCmdUI)
{
	if( m_pTable->TableType() == KEMIOTableTypeODData )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void KIOTableView::OnColumnmenuDisplaycolumnset()
{
	setColumnDisplay();	
}
 

void KIOTableView::OnColumnmenuDeletecolumn()
{
	if (NULL != m_pController)
	{
		if (NULL != m_pClickedColumn)
		{
			CString strColumnName = _T("");
			strColumnName = m_pClickedColumn->GetInternalName();

			m_strFilter = _T("");
			m_strOrder = _T("");

			m_pController->DeleteColumn(strColumnName);
			m_pClickedColumn = NULL;
		}
	}
}


void KIOTableView::OnColumnmenuRenamecolumn()
{
	try
	{
		if (nullptr != m_pClickedColumn)
		{
			CString strColumnName = _T("");
			strColumnName = m_pClickedColumn->GetInternalName();

			m_pController->RenameField(strColumnName);

			m_pClickedColumn = nullptr;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KIOTableView::OnColumnmenuEditcolumn()
{
	try
	{
		if (nullptr != m_pClickedColumn)
		{
			CString strColumnName = _T("");
			strColumnName = m_pClickedColumn->GetInternalName();

			m_pController->EditField(strColumnName);

			m_pClickedColumn = nullptr;
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KIOTableView::OnUpdateColumnmenuRenamecolumn(CCmdUI *pCmdUI)
{
	try
	{
// 		CString strColumnName = _T("");
// 		strColumnName = m_pClickedColumn->GetInternalName();
// 
// 		if (m_pController->IsUserDefineColumn(strColumnName))
// 		{
// 			pCmdUI->Enable(TRUE);
// 		}
// 		else
// 		{
// 			pCmdUI->Enable(FALSE);
// 		}
	}
	catch (...)
	{
		pCmdUI->Enable(FALSE);
		TxLogDebugException();	
	}
}


void KIOTableView::OnUpdateColumnmenuDeletecolumn(CCmdUI *pCmdUI)
{
	try
	{
		CString strColumnName = _T("");
		strColumnName = m_pClickedColumn->GetInternalName();

		if (m_pController->IsUserDefineColumn(strColumnName))
		{
			pCmdUI->Enable(TRUE);
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}
	}
	catch (...)
	{
		pCmdUI->Enable(FALSE);
		TxLogDebugException();	
	}
}


void KIOTableView::OnUpdateColumnmenuEditcolumn(CCmdUI *pCmdUI)
{
	try
	{
// 		CString strColumnName = _T("");
// 		strColumnName = m_pClickedColumn->GetInternalName();
// 
// 		if (m_pController->IsUserDefineColumn(strColumnName))
// 		{
// 			pCmdUI->Enable(TRUE);
// 		}
// 		else
// 		{
// 			pCmdUI->Enable(FALSE);
// 		}
	}
	catch (...)
	{
		pCmdUI->Enable(FALSE);
		TxLogDebugException();	
	}
}


void KIOTableView::OnColumnmenuApplyModelData()
{
	CString strClickedColumnName = _T("");

	if(NULL != m_pController)
	{
		if(NULL != m_pClickedColumn)
		{
			strClickedColumnName = m_pClickedColumn->GetInternalName();
		}
	}

	const KIOColumns* pColumns = m_pTable->Columns();
	int nIndex = pColumns->GetColumnIndex(strClickedColumnName);
	CString strColumnDisplayName = _T("");
	if(-1 == nIndex)
	{
		;
	}
	else 
	{
		KIOColumn* pColumn = pColumns->GetColumn(nIndex);

		strColumnDisplayName = pColumn->Caption();
	}

	CMainFrameWnd* pMainFrame   = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame  = (CChildFrame*)(pMainFrame->MDIGetActive());
	KTarget*       pTarget      = pChildFrame->GetTarget();
	KProject*      pProject = ((CKmzApp*)AfxGetApp())->GetProject();

	CString strTableName = m_pTable->Name();
	if( strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
	{
		KApplyModeChoiceModelDataDlg dlg;
		dlg.SetTarget(pTarget);
		dlg.SetColumnName(strClickedColumnName);
		dlg.SetColumnDisplayName(strColumnDisplayName);
		dlg.SetProject(pProject);

		if( dlg.DoModal() != IDOK)
		{

		}
	}
	else if(strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
	{
		KApplyDistributionModelDataDlg dlg;
		dlg.SetTarget(pTarget);
		dlg.SetColumnName(strClickedColumnName);
		dlg.SetColumnDisplayName(strColumnDisplayName);
		dlg.SetProject(pProject);

		if( dlg.DoModal() != IDOK)
		{

		}
	}
	else if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
	{
		KApplyGenerationModelDataDlg dlg;
		dlg.SetTarget(pTarget);
		dlg.SetColumnName(strClickedColumnName);
		dlg.SetColumnDisplayName(strColumnDisplayName);
		dlg.SetProject(pProject);

		if( dlg.DoModal() != IDOK)
		{

		}
	}
}


void KIOTableView::OnUpdateColumnmenuApplyModelData(CCmdUI *pCmdUI)
{
	CString strClickedColumnName = _T("");

	if(NULL != m_pController)
	{
		if(NULL != m_pClickedColumn)
		{
			strClickedColumnName = m_pClickedColumn->GetInternalName();
		}
	}

	CString strTableName = m_pTable->Name();
	if( strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else if(strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
	{
		pCmdUI->Enable(TRUE);
	}
	else 
	{
		pCmdUI->Enable(FALSE);
	}

	bool bDoubleTypeCheck = false;
	const KIOColumns* pColumns = m_pTable->Columns();
	int nIndex = pColumns->GetColumnIndex(strClickedColumnName);

	if(-1 == nIndex)
	{
		bDoubleTypeCheck = false;
	}
	else 
	{
		KIOColumn* pColumn = pColumns->GetColumn(nIndex);

		if( KEMIODataTypeDouble == pColumn->DataType())
		{
			bDoubleTypeCheck = true;
		}
	}

	if(bDoubleTypeCheck == false)
	{
		pCmdUI->Enable(FALSE);
		return;
	}
}


//OnColumnmenuStatisticscolumn
void KIOTableView::OnColumnmenuStatisticsSinglecolumn()
{
    /*if(NULL != m_pController)
    {
        if(NULL != m_pClickedColumn)
        {
            CString strColumnName = _T("");
            strColumnName = m_pClickedColumn->GetInternalName();
            KStatisticsDlg dlg;
            dlg.SetInitValue(m_pTable, strColumnName);
            dlg.DoModal();
            
            m_pClickedColumn = NULL;
        }
    }*/

    
    /*KStatisticsDlg dlg;
    if (m_bApplyFilter)
    {
        dlg.SetInitValue(m_pTable, _T(""), m_strFilter);
    }
    else
    {
        dlg.SetInitValue(m_pTable);
    }
    
    dlg.DoModal();*/

	CMainFrameWnd* pMainFrame   = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame  = (CChildFrame*)(pMainFrame->MDIGetActive());
	KTarget*       pTarget      = pChildFrame->GetTarget();

	KStatisticsBaseDefineDlg defineDlg;
	defineDlg.SetTarget(pTarget);
	defineDlg.SetTable(m_pTable);
	if (m_bApplyFilter)
	{
		defineDlg.SetFilter(m_strFilter);
	}

	if( IDOK == defineDlg.DoModal() )
	{

	}
}


void KIOTableView::OnColumnmenuStatisticsMulticolumn()
{
	CMainFrameWnd* pMainFrame   = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame  = (CChildFrame*)(pMainFrame->MDIGetActive());
	KTarget*       pTarget      = pChildFrame->GetTarget();

	KMultiStatisticsBaseDefineDlg defineDlg;
	defineDlg.SetTarget(pTarget);
	defineDlg.SetTable(m_pTable);
	if (m_bApplyFilter)
	{
		defineDlg.SetFilter(m_strFilter);
	}

	if( IDOK == defineDlg.DoModal() )
	{

	}
}


void KIOTableView::OnRecordZoomto()
{
	if(NULL != m_pController)
	{
        CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
        if (pSelectedRows->GetCount() == 0)
            return;

        CXTPReportRow* pReportRow  = pSelectedRows->GetAt(0);
        
		if(NULL != pReportRow)
		{
			int nIndex = pReportRow->GetIndex();
			const KIORow* pRow = m_pRecordset->GetRow(nIndex);
			
            //복사하여 넘겨 줘야 한다.(다이나믹테이블 문제)
            KIORow oRow = *pRow;
			m_pController->ZoomTo(&oRow);
		}
	}
}


void KIOTableView::OnRecordPanto()
{
	if(NULL != m_pController)
	{
        CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
        if (pSelectedRows->GetCount() == 0)
            return;

        CXTPReportRow* pReportRow = pSelectedRows->GetAt(0);
        
		if(NULL != pReportRow)
		{
			int   nIndex       = pReportRow->GetIndex();
			const KIORow* pRow = m_pRecordset->GetRow(nIndex);

            //복사하여 넘겨 줘야 한다.(다이나믹테이블 문제)
			KIORow        oRow = *pRow;
			m_pController->PanTo(&oRow);
		}
	}
}

void KIOTableView::OnRecordStartNode()
{
    if(NULL == m_pController)
        return;

    CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
    if (pSelectedRows->GetCount() == 0)
        return;

    CXTPReportRow*          pRow          = pSelectedRows->GetAt(0);
    if (pRow == NULL)
        return;
    
    int   nIndex       = pRow->GetIndex();
}


void KIOTableView::OnUpdateRecordStartNode(CCmdUI *pCmdUI)
{
    CString strTableName = m_pTable->Name();
    if (strTableName.CompareNoCase(TABLE_NODE) == 0)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


void KIOTableView::OnRecordEndNode()
{
    if(NULL == m_pController)
        return;
}


void KIOTableView::OnUpdateRecordEndNode(CCmdUI *pCmdUI)
{
    CString strTableName = m_pTable->Name();
    if (strTableName.CompareNoCase(TABLE_NODE) == 0)
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}

void KIOTableView::OnUpdateRecord(CCmdUI *pCmdUI)
{
	// todo : Network view의 표출 여부와 현재 Table의 종류에 따라서 활성 여부 설정
}


void KIOTableView::OnReportRowRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	if( NULL == pItem->pColumn )
	{
		return;
	}

    pItem->pRow->SetSelected(TRUE);

	CMenu oMenu;
	oMenu.LoadMenu( IDR_POPUP_TABLETABVIEW );

	CString strTableName = m_pTable->Name();
	int nMenu = 0;

	if(strTableName.CompareNoCase(TABLE_ACCESS_LINE_STATION) == 0)
	{
		nMenu = 2;
	}
    else if(strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) //★ 필요
    {
        nMenu = 3;
    }
    else if(strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0)
    {
        nMenu = 3;
    }
    else if(strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0)
    {
        nMenu = 4;
    }
    else if(strTableName.CompareNoCase(TABLE_SED_POP) == 0)
    {
        nMenu = 4;
    }
    else if(m_pTable->TableType() == KEMIOTableTypeODData)
    {
        nMenu = 4;
    }
    else
    {
        nMenu = 0;
    }

	CMenu* pPopup = oMenu.GetSubMenu( nMenu );
	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
	pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, 
		pItem->pt.x, pItem->pt.y, this );

	*pResult = 0;
}


void KIOTableView::OnReportColumnRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_TABLE_COLUMN );
	//CMenu* pPopup = menu.GetSubMenu( 0 );

	CXTPReportColumn* pClickedHeader = pItem->pColumn;
	m_pClickedColumn = pClickedHeader;
	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
	/*pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, 
		pItem->pt.x, pItem->pt.y, this );*/

	CString strTableName = m_pTable->Name();
	int nMenu = 0;
	if( strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
	{
		nMenu = MENU_MODE;
	}
	else if(strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
	{
		nMenu = MENU_PURPOSE;
	}
	else if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
	{
		nMenu = MENU_PA;
	}
	else 
	{
		nMenu = MENU_DEFAULT;
	}

	pCommandBars->TrackPopupMenuEx( menu.GetSubMenu( nMenu ), TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this );

	*pResult = 0;
}


void KIOTableView::OnReportSortOrderChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	//XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	//ASSERT(pItemNotify != NULL);

	CXTPReportColumns* pColumns = m_wndReport.GetColumns();
	int nColumnCount = pColumns->GetCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(i);
		if(pColumn->HasSortTriangle() == TRUE)
		{
			if(pColumn->IsSortedDecreasing() == TRUE)
			{
				m_strOrder.Format(_T(" %s DESC "), pColumn->GetInternalName());
			}
            else if (pColumn->IsSortedIncreasing() == TRUE)
            {
                m_strOrder.Format(_T(" %s ASC "), pColumn->GetInternalName());
            }
			else
			{
				m_strOrder = _T("");
			}
			UpdateRecord();
			break;
		}
	}
}


void KIOTableView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	__super::OnActivate(nState, pWndOther, bMinimized);

	//if(WM_ACTIVATE == nState && NULL != m_pMapView)
	//{
	//	m_pMapView->NotifyIOTableViewActivated(m_pTable->Name());
	//}
}


void KIOTableView::OnContextMenu( CWnd* pWnd, CPoint point )
{
//     CWnd* pwndControl = &m_wndControl;
// 
//     if( pWnd == pwndControl )
//     {
// 		CString strClickedColumnName = _T("");
// 
// 		if(NULL != m_pController)
// 		{
// 			if(NULL != m_pClickedColumn)
// 			{
// 				strClickedColumnName = m_pClickedColumn->GetInternalName();
// 			}
// 		}
// 
// 		CString strTableName = m_pTable->Name();
// 		if( strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
// 		{
// 			pCmdUI->Enable(TRUE);
// 		}
// 		else if(strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
// 		{
// 			pCmdUI->Enable(TRUE);
// 		}
// 		else if(strTableName.CompareNoCase(TABLE_ZONE) == 0)
// 		{
// 			pCmdUI->Enable(TRUE);
// 		}
// 		else 
// 		{
// 			pCmdUI->Enable(FALSE);
// 		}
// 
// 		bool bDoubleTypeCheck = false;
// 		const KIOColumnCollection* pColumns = m_pTable->Columns();
// 		int nIndex = pColumns->GetColumnIndex(strClickedColumnName);
// 
// 		if(-1 == nIndex)
// 		{
// 			bDoubleTypeCheck = false;
// 		}
// 		else 
// 		{
// 			KIOColumn* pColumn = pColumns->GetColumn(nIndex);
// 
// 			if( KEMIODataTypeDouble == pColumn->DataType())
// 			{
// 				bDoubleTypeCheck = true;
// 			}
// 		}
// 
// 		if(bDoubleTypeCheck == false)
// 		{
// 			pCmdUI->Enable(FALSE);
// 			return;
// 		}
// 
// 
// 
// 
//         CString strSelectedName = GetCurrentSelectedLayerName();
//         CMenu menu;
//         menu.LoadMenu( IDR_POPUP_RENDERER );
//         CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
//         CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
//         if ( strSelectedName.CompareNoCase(TABLE_NODE) == 0 )
//         {
//             pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(1), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
//         }
//         else if ( strSelectedName.CompareNoCase(TABLE_LINK) == 0 )
//         {
//             pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(2), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
//         }
//         else if ( strSelectedName.CompareNoCase(TABLE_ZONE) == 0 )
//         {
//             pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(3), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
//         }
//         else
//         {
//             //pCommandBars->TrackPopupMenuEx( menu.GetSubMenu(0), TPM_RIGHTBUTTON, point.x, point.y, pFrame );
//         }        
//     }
//     else 
//     {
//         CWnd::OnContextMenu( pWnd, point );
//     }
}


void KIOTableView::OnTableviewmenuCalculateZoneDist()
{
	if (NULL != m_pController)
	{
		m_pController->ViewCalculateZoneDistance();
	}
}

void KIOTableView::OnUpdateTableviewmenuCalculateZoneDist( CCmdUI* pCmdUI )
{
	CString strTableName = m_pTable->Name();
	
	if(strTableName == TABLE_PARAMETER_OD)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


void KIOTableView::OnTableviewmenuGenerateGeometery()
{	
}

void KIOTableView::OnUpdateTableviewmenuGenerateGeometery( CCmdUI *pCmdUI )
{
	pCmdUI->Enable(FALSE);
}

void KIOTableView::OnReportRowDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	if( NULL == pItem->pColumn ) {
		return;
	}

	pItem->pRow->SetSelected(TRUE);
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
	if (pSelectedRows->GetCount() == 0)
		return;

	CXTPReportRow* pRow = pSelectedRows->GetAt(0);
	if(NULL == pRow)
		return;		

	int nIndex = pRow->GetIndex();
	const KIORow* pIORow = m_pRecordset->GetRow(nIndex);

	if (m_pController) 	{
		m_pController->ShowProperties(pIORow);
	}
}


void KIOTableView::OnAccessLineStationModify()
{
	try
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		CXTPReportRow*          pSelectedRow  = pSelectedRows->GetAt(0);
		int   nIndex          = pSelectedRow->GetIndex();
		const KIORow*           pIORow        = m_pRecordset->GetRow(nIndex);

		Integer nxZoneID = pIORow->GetItem(0)->GetValueAsInteger64();
		Integer nxLineID = pIORow->GetItem(1)->GetValueAsInteger64();

		m_pController->ShowAccessLineStationModify(nxZoneID, nxLineID);
	}
	catch (...)
	{
		return;
	}
}


void KIOTableView::OnAccessLineStationDelete()
{
	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			if (AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO) == IDNO)
			{
				throw 1;
			}
		}
		else {
			if (AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO) == IDNO)
			{
				throw 1;
			}
		}
		
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		CXTPReportRow*          pSelectedRow  = pSelectedRows->GetAt(0);
		int   nIndex          = pSelectedRow->GetIndex();
		const KIORow*           pIORow        = m_pRecordset->GetRow(nIndex);

		Integer nxZoneID = pIORow->GetItem(0)->GetValueAsInteger64();
		Integer nxLineID = pIORow->GetItem(1)->GetValueAsInteger64();

		m_pController->DeleteAccessLineStation(nxZoneID, nxLineID);
	}
	catch (...)
	{
		return;
	}
}

void KIOTableView::OnTransitTransitlineinfo()
{
    try
    {
        CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
        if (pSelectedRows->GetCount() == 0)
            return;

        CXTPReportRow*          pRow          = pSelectedRows->GetAt(0);
        if (pRow == NULL)
            return;

        int            nIndex      = pRow->GetIndex();
        const KIORow*  pIORow      = m_pRecordset->GetRow(nIndex);
        
        CString strTableName = m_pTable->Name();

        const KIOItem* pItem = nullptr;
        if(strTableName.CompareNoCase(TABLE_ACCESS_LINE_STATION) == 0)
        {
            pItem       = pIORow->GetItem(1);
        }
        else
        {
            pItem       = pIORow->GetItem(0);
        }
        
        Integer        nxTransitID = pItem->GetValueAsInteger64();
        m_pController->ShowTransitLineInfo(nxTransitID);
    }
    catch(...)
    {
        TxLogDebugException();
    }
}


void KIOTableView::OnTransitTransitlineThemes()
{
	try
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		if (pSelectedRows->GetCount() == 0)
			return;

		CXTPReportRow*          pRow          = pSelectedRows->GetAt(0);
		if (pRow == NULL)
			return;

		int            nIndex      = pRow->GetIndex();
		const KIORow*  pIORow      = m_pRecordset->GetRow(nIndex);

		CString strTableName = m_pTable->Name();

		const KIOItem* pItem = nullptr;
		if(strTableName.CompareNoCase(TABLE_ACCESS_LINE_STATION) == 0)
		{
			pItem       = pIORow->GetItem(1);
		}
		else
		{
			pItem       = pIORow->GetItem(0);
		}
		Integer        nxTransitID = pItem->GetValueAsInteger64();
		m_pMapView->ZoomTransitExtent(nxTransitID);
		m_pMapView->ShowTransitLineThemes(nxTransitID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KIOTableView::OnUpdateTransitTransitlineThemes(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif

	pCmdUI->Enable(TRUE);
}

void KIOTableView::OnChartTransit()
{
    try
    {
        if (_tcsicmp(m_pTable->Name(), TABLE_TRANSITLINKS) == 0 || _tcsicmp(m_pTable->Name(), TABLE_TRANSITNODES) == 0) //★ 필요
        {
            CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
            CXTPReportRow*          pSelectedRow  = pSelectedRows->GetAt(0);
            int   nIndex          = pSelectedRow->GetIndex();
            const KIORow*           pIORow        = m_pRecordset->GetRow(nIndex);

            Integer nxTransitID   = pIORow->GetItem(0)->GetValueAsInteger64();
            m_pMapView->ZoomTransitExtent(nxTransitID);
            m_pController->ShowTransitChart(nxTransitID);
        }        
    }
    catch (...)
    {
        return;
    }
}

void KIOTableView::RedrawReport()
{
    try
    {
        CXTPReportControl& wndReport = GetReportCtrl();
        wndReport.RedrawControl();
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


void KIOTableView::OnTerminalScheduleEdit()
{
	try
	{
		m_pMapView->ShowTerminalScheduleEditView();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KIOTableView::OnTableviewAddData()
{
    if (m_pController)
    {
        m_pController->AddRow();
    }
}

void KIOTableView::OnTableviewDeleteRowData()
{
    
	if (KmzSystem::GetLanguage() == KEMKorea) {
		INT_PTR nResult = AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO);
		if (nResult != IDYES)
			return;
	}
	else {
		INT_PTR nResult = AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO);
		if (nResult != IDYES)
			return;
	}
    try
    {
        if (m_pController == nullptr) throw 1;

        CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
        if (pSelectedRows->GetCount() == 0) throw 2;

        CXTPReportRow* pReportRow = pSelectedRows->GetAt(0);
        if (pReportRow == nullptr) throw 3;

        int nIndex = pReportRow->GetIndex();
        const 
            KIORow* pIORow = m_pRecordset->GetRow(nIndex);
        
        // 중요 : 복사하여 사용한다. 
        // 컨트롤러에서 메시지를 띄운경우 Redraw가 생겨서 값이 변경 된다.
        // 다이나믹의 경우 현재 자신이 갖고 있는 유일한 Row의 멤버의 주소를 리턴하기 때문에
        // 값이 바뀔수 있다.
        KIORow oIORow = *pIORow;
        m_pController->DeleteRowData(&oIORow);
    }
    catch (int e)
    {
        if (e == 2)
        {
            AfxMessageBox(_T("선택된 정보가 존재하지 않습니다."));
        }
        else if (e == 3)
        {
            AfxMessageBox(_T("선택된 정보가 존재하지 않습니다."));
        }
        else
        {
            AfxMessageBox(_T("오류가 발생하였습니다."));
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
        AfxMessageBox(_T("오류가 발생하였습니다."));
    }
    catch (...)
    {
    	TxLogDebugException();
        AfxMessageBox(_T("오류가 발생하였습니다."));
    }    
}

void KIOTableView::OnTableviewTableResetData()
{
    INT_PTR nResult = AfxMessageBox(_T("테이블 데이터를 모두 삭제하고 초기화 합니다.\n계속 진행하시겠습니까?"), MB_YESNO);
    if (nResult != IDYES)
        return;

    try
    {
        if (m_pController == nullptr) throw 1;

        m_pController->TableResetData();

        if (m_pTable)
            m_pTable->Notify();

        RedrawReport();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        AfxMessageBox(_T("오류가 발생하였습니다."));
    }
    catch (...)
    {
        TxLogDebugException();
        AfxMessageBox(_T("오류가 발생하였습니다."));
    }   
}

void KIOTableView::OnTransitODCalculator()
{
	try
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		if (pSelectedRows->GetCount() == 0)
			return;

		CXTPReportRow*          pRow          = pSelectedRows->GetAt(0);
		if (pRow == NULL)
			return;

		int            nIndex      = pRow->GetIndex();
		const KIORow*  pIORow      = m_pRecordset->GetRow(nIndex);

		CString strTableName = m_pTable->Name();

		const KIOItem* pItem = nullptr;
		if(strTableName.CompareNoCase(TABLE_ACCESS_LINE_STATION) == 0)
		{
			pItem       = pIORow->GetItem(1);
		}
		else
		{
			pItem       = pIORow->GetItem(0);
		}
		Integer        nxTransitID = pItem->GetValueAsInteger64();
		m_pMapView->ZoomTransitExtent(nxTransitID);
		m_pMapView->ShowTransitODCalculator(nxTransitID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KIOTableView::OnTableTransitLineThemes()
{
	m_pMapView->ShowTransitLineThemes(-1);
}

void KIOTableView::OnTableTransitODCalculator()
{
	m_pMapView->ShowTransitODCalculator(-1);
}

void KIOTableView::OnTableChartTransit()
{
	m_pController->ShowTransitChart(-1);
}


void KIOTableView::OnTransitInfluenceArea()
{
    m_pMapView->TransitInfluenceArea();
}

bool KIOTableView::IsPageView()
{
	if (m_nTableDisplayType == 1)
		return true;

	return false;
}

int KIOTableView::PageCount()
{
	if (m_bApplyFilter == false)
		return m_nPageCount;
	else
		return m_nFilterPageCount;
}

int KIOTableView::PageNumber()
{
	if (m_bApplyFilter == false)
		return m_nCurPage;
	else
		return m_nFilterCurPage;
}

void KIOTableView::MovePage( int nPage )
{
	if (nPage < 1)
		nPage = 1;

	if (m_bApplyFilter == false) {
		if (m_nCurPage == nPage)
			return;

		if (nPage > m_nPageCount)
			return;

		m_nCurPage = nPage;
	} 
	else {
		if (m_nFilterCurPage == nPage)
			return;

		if (nPage > m_nFilterPageCount)
			return;

		m_nFilterCurPage = nPage;
	}

	UpdateRecord();
}

int KIOTableView::TableRecordCount()
{
	if (m_pTable == nullptr)
		return 0;

	try 
	{
		return m_pTable->RecordCount();	
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
	
	return 0;
}

int KIOTableView::ViewRecordCount()
{   
	if (m_pTable == nullptr)
		return 0;

	try 
	{
		if (m_bApplyFilter == true) {
			return m_pTable->RecordCount(m_strFilter);
		} 
		else {
			return m_pTable->RecordCount();
		}   
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
 
	return 0;
}
