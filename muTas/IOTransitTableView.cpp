/**
 * @file IOTransitTableView.cpp
 * @brief KIOTransitTableView 구현 파일
 * @author 
 * @date 2011.08.02
 * @remark
 */


#include "StdAfx.h"
#include "KmzApp.h"
#include "IOTransitTableView.h"
//^^ #include "IOItem.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOTable.h"
#include "IOTableController.h"
#include "IOTableRecord.h"
#include "RouteDeleter.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapView.h"
//^^ #include "TableOwner.h"
#include "IOTableRecordSelection.h"
#include "InitRelationProcess.h"

IMPLEMENT_DYNCREATE(KIOTransitTableView, KIOTableView)
    BEGIN_MESSAGE_MAP(KIOTransitTableView, KIOTableView)
        ON_WM_CLOSE()
        ON_NOTIFY (NM_RCLICK,                XTP_ID_REPORT_CONTROL, &KIOTransitTableView::OnReportRowRButtonClick)
        ON_NOTIFY (XTP_NM_REPORT_CHECKED,    XTP_ID_REPORT_CONTROL, &KIOTransitTableView::OnReportCheckItem      )
        ON_NOTIFY (XTP_NM_REPORT_SELCHANGED, XTP_ID_REPORT_CONTROL, &KIOTransitTableView::OnReportSelectChanged )

        ON_COMMAND(ID_TRINSIT_SELECTALLTRANSITLINE,              &KIOTransitTableView::OnTransitSelectAll     )
        ON_COMMAND(ID_TRANSIT_DESELECTALLTRANSITLINE,            &KIOTransitTableView::OnTransitDeSelectAll   )
        ON_COMMAND(ID_TRANSIT_TRANSITLINEDELETE,                 &KIOTransitTableView::OnTransitDelete        )
        ON_COMMAND(ID_TRANSITLINEEDIT_INSERT,                    &KIOTransitTableView::OnTransitEditInsert    )
        ON_COMMAND(ID_TRANSITLINEEDIT_MODIFY,                    &KIOTransitTableView::OnTransitEditModify    )
        ON_COMMAND(ID_TRANSITLINEEDIT_APPEND,                    &KIOTransitTableView::OnTransitEditAppend    )
        ON_UPDATE_COMMAND_UI(ID_TRANSITLINEEDIT_INSERT, &KIOTransitTableView::OnUpdateTransitlineeditInsert)
        ON_UPDATE_COMMAND_UI(ID_TRANSITLINEEDIT_MODIFY, &KIOTransitTableView::OnUpdateTransitlineeditModify)
        ON_UPDATE_COMMAND_UI(ID_TRANSITLINEEDIT_APPEND, &KIOTransitTableView::OnUpdateTransitlineeditAppend)
        ON_COMMAND(ID_TRANSIT_ZOOMEXTENT, &KIOTransitTableView::OnTransitZoomExtent)
        ON_UPDATE_COMMAND_UI(ID_TRANSIT_ZOOMEXTENT, &KIOTransitTableView::OnUpdateTransitZoomExtent)
        ON_COMMAND(ID_TRANSIT_ZOOMFIRST, &KIOTransitTableView::OnTransitZoomFirst)
        ON_UPDATE_COMMAND_UI(ID_TRANSIT_ZOOMFIRST, &KIOTransitTableView::OnUpdateTransitZoomFirst)
        ON_COMMAND(ID_TRANSIT_ZOOMLAST, &KIOTransitTableView::OnTransitZoomLast)
        ON_UPDATE_COMMAND_UI(ID_TRANSIT_ZOOMLAST, &KIOTransitTableView::OnUpdateTransitZoomLast)
        ON_COMMAND(ID_TRANSIT_TRANSITLINEINFO, &KIOTransitTableView::OnTransitLineInfo)
        ON_COMMAND(ID_TRANSIT_TRANSITLINE_THEMES, &KIOTransitTableView::OnTransitLineThemes)
        ON_UPDATE_COMMAND_UI(ID_TRANSIT_TRANSITLINE_THEMES, &KIOTransitTableView::OnUpdateTransitLineThemes)
        ON_COMMAND(ID_CHART_TRANSIT, &KIOTransitTableView::OnTransitChart)
        ON_UPDATE_COMMAND_UI(ID_CHART_TRANSIT, &KIOTransitTableView::OnUpdateTransitChart)
        ON_COMMAND(ID_TRANSIT_LINKVIATRANSIT, &KIOTransitTableView::OnTransitLinkViaTransit)
        ON_COMMAND(ID_TRANSIT_NODEVIATRANSIT, &KIOTransitTableView::OnTransitNodeViaTransit)
        ON_COMMAND(ID_TRANSIT_LINESTYLE, &KIOTransitTableView::OnTransitLinkStyle)
    END_MESSAGE_MAP()


KIOTransitTableView::KIOTransitTableView(void)
{
	m_pTransitSelection    = new KIOTableRecordSelection;
    m_pTransitStyle        = new KIOTableRecordBackStyle;
}

KIOTransitTableView::~KIOTransitTableView(void)
{
	if(NULL != m_pTransitSelection)
	{
		delete m_pTransitSelection;
	}

    QBicDelete(m_pTransitStyle);
}

void KIOTransitTableView::OnClose()
{
    if (m_pMapView) {
        m_pMapView->ClearTransit(false);
    }

    KIOTableView::OnClose();
}


void KIOTransitTableView::OnDraw(CDC* pDC)
{
}


#ifdef _DEBUG
void KIOTransitTableView::AssertValid() const
{
	KIOTableView::AssertValid();
}


#ifndef _WIN32_WCE
void KIOTransitTableView::Dump(CDumpContext& dc) const
{
	KIOTableView::Dump(dc);
}
#endif
#endif


std::set<Integer> KIOTransitTableView::GetSelectedTransit(void)
{
	return m_pTransitSelection->GetSelection();
}


void KIOTransitTableView::RegisterTransitStyle(Integer nRouteID, bool bUseCustColor, COLORREF clrLine, double a_dWidth, double a_dOffset)
{
    TBackStyle oTBackStyle;
    oTBackStyle.bUseCustomClr = bUseCustColor;
    oTBackStyle.TRGBColor = clrLine;
    oTBackStyle.dWidth    = a_dWidth;
    oTBackStyle.dOffset   = a_dOffset;

    m_pTransitStyle->UpdateStyle(nRouteID, oTBackStyle);

    m_pTransitSelection->AddSelect(nRouteID);
}

void KIOTransitTableView::RegisterTransitStyle(std::set<Integer> a_setApplyID, bool bUseCustColor, COLORREF clrLine, double a_dWidth, double a_dOffset)
{
    auto iter = a_setApplyID.begin();
    auto end  = a_setApplyID.end();

    for (; iter != end; ++iter) {
        const Integer &nxRouteID = *iter;

        TBackStyle oTBackStyle;
        oTBackStyle.bUseCustomClr = bUseCustColor;
        oTBackStyle.TRGBColor     = clrLine;
        oTBackStyle.dWidth        = a_dWidth;
        oTBackStyle.dOffset       = a_dOffset;

        m_pTransitStyle->UpdateStyle(nxRouteID, oTBackStyle);

    }

    /*현재 TableView 상에 표출된 리스트에 대해서 적용하기 위해*/
    int nRowCount = m_pRecordset->GetRowCount();
    for (int i = 0; i < nRowCount; ++i)
    {
        KIRowCPTR  pRow = m_pRecordset->GetRow(i);
        KIOItem*   pIOItem = pRow->GetItem(0);

        Integer   nxRouteID = pIOItem->GetValueAsInteger64();

        auto itFind = a_setApplyID.find(nxRouteID);
        if (itFind != a_setApplyID.end()) {
            m_pTransitSelection->AddSelect(nxRouteID);
        }
    }
}

void KIOTransitTableView::RfreshTransitStyle()
{
    m_wndReport.RedrawControl();
    m_pMapView->RefreshTransitMapDraw();
}

void KIOTransitTableView::InitTransitStyle()
{
    m_pTransitStyle->ClearStyle();
    RfreshTransitStyle();
}

bool KIOTransitTableView::IsTransitStyle(Integer nRouteID, OUT COLORREF &clrLine, double &a_dWidth, double &a_dOffset)
{
    TBackStyle oTBackStyle;
    if (m_pTransitStyle->IsStyle(nRouteID, oTBackStyle)) {
        if (oTBackStyle.bUseCustomClr) {
            clrLine = oTBackStyle.TRGBColor;
        }
        else {
            if (oTBackStyle.bEverCustomclr) {
                clrLine = oTBackStyle.TRGBColor;
            }
        }
        
        a_dWidth  = oTBackStyle.dWidth;
        a_dOffset = oTBackStyle.dOffset;
        return true;
    }
    else {

        return false;
    }
}

void KIOTransitTableView::RegisterMapView(KMapView* pMapView)
{
	KIOTableView::RegisterMapView(pMapView);
	pMapView->RefreshTransitMapDraw();
}


void KIOTransitTableView::NotifyTransitCreatedFromMap(Integer nRouteID)
{	
	UpdateRecord();
    m_pTransitSelection->AddSelect(nRouteID);
    m_pMapView->RefreshTransitMapDraw();
}


void KIOTransitTableView::NotifyRouteModifyedFromMap(Integer nRouteID)
{	
	UpdateRecord();
    m_pTransitSelection->AddSelect(nRouteID);
    m_pMapView->RefreshTransitMapDraw();
}


void KIOTransitTableView::UpdateRecord(void)
{
	CXTPReportControl& wndReport = GetReportCtrl();
	wndReport.ResetContent(TRUE);

	if (nullptr == m_pTable) {
		wndReport.Populate();
		return;
	}

	if(NULL != m_pRecordset)
	{
		delete m_pRecordset;
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
		int nRecordCount = m_pRecordset->GetRowCount();
		m_pRecord        = new KIOTransitTableRecord(pCodeManager, m_pRecordset, m_pTransitSelection, m_pTransitStyle);
		wndReport.SetVirtualMode(m_pRecord, nRecordCount);
	}
	else 
	{
		QBicDelete(m_pRecordset);
	}

	wndReport.Populate();
}


void KIOTransitTableView::OnReportRowRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	if( NULL == pItem->pColumn )
	{
		return;
	}

    pItem->pRow->SetSelected(TRUE);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_IOTRANSIT);
	CMenu* pPopup = menu.GetSubMenu( 1 );

	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
	pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);

	*pResult = 0;
}


void KIOTransitTableView::OnReportCheckItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	ASSERT(NULL != pItemNotify);
	
	int      nIndex     = pItemNotify->pRow->GetIndex();
	const 
    KIORow*  pRow       = m_pRecordset->GetRow(nIndex);
	
    KIOItem* pIOItem    = pRow->GetItem(0);
    int      nTransitID = pIOItem->GetValueToInt();

    if(m_pTransitSelection->IsSelected(nTransitID) == true)
    {
        m_pTransitSelection->RemoveSelect(nTransitID);
        if(NULL != m_pMapView)
        {
            m_pMapView->RemoveTransitMapDraw(nTransitID);
        }
    }
    else
    {
        m_pTransitSelection->AddSelect(nTransitID);
        if(NULL != m_pMapView)
        {
            m_pMapView->AddTransitMapDraw(nTransitID);
        }
    }
}


void KIOTransitTableView::OnTransitSelectAll()
{
    /************************************************************************/
    /* 무조건 보이지 않은 데이터까지 체크 하는 것이 아니라,
    현재 띄어진 Row에 대해 모든 체크 하는 방식으로 변경 - jyk:17.11.21*/
    /************************************************************************/
	//m_pTransitSelection->ClearSelection();

	int nRowCount = m_pRecordset->GetRowCount();
	for(int i = 0; i < nRowCount; ++i)
	{
		KIRowCPTR  pRow       = m_pRecordset->GetRow(i);
		KIOItem*   pIOItem    = pRow->GetItem       (0);

		Integer   nxTransitID = pIOItem->GetValueAsInteger64();

		m_pTransitSelection->AddSelect(nxTransitID);	
	}

	m_wndReport.RedrawControl();
	m_pMapView->RefreshTransitMapDraw();
}


void KIOTransitTableView::OnTransitDeSelectAll()
{
    /************************************************************************/
    /* 무조건 보이지 않은 데이터까지 체크 해제 하는 것이 아니라,
       현재 띄어진 Row에 대해 모든 체크 해제 하는 방식으로 변경 - jyk:17.11.21*/
    /************************************************************************/
    //m_pTransitSelection->ClearSelection();
    int nRowCount = m_pRecordset->GetRowCount();
    for (int i = 0; i < nRowCount; ++i)
    {
        KIRowCPTR  pRow = m_pRecordset->GetRow(i);
        KIOItem*   pIOItem = pRow->GetItem(0);

        Integer   nxTransitID = pIOItem->GetValueAsInteger64();

        m_pTransitSelection->RemoveSelect(nxTransitID);
    }

	m_wndReport.RedrawControl();
	m_pMapView->RefreshTransitMapDraw();
}


void KIOTransitTableView::OnTransitDelete()
{
	CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());	

    if ( m_pTransitSelection->SelectedCount() == 0)  {
        AfxMessageBox(_T("삭제할 노선을 선택해 주세요."));
        return;
    }

    CString strOutMsg;
    strOutMsg.Format(_T("선택된 %d개 노선 정보를 삭제합니다."), m_pTransitSelection->SelectedCount());
    
    if ( AfxMessageBox(strOutMsg, MB_YESNO) == IDNO)
    {
        return;
    }

    KTarget*      pTarget     = pChildFrame->GetTarget();
    KRouteDeleter oRouteDeleter(pTarget);

    std::set<Integer>::iterator iter = m_pTransitSelection->begin();
    while(iter != m_pTransitSelection->end())
    {
        Integer nxRouteID = *iter;
        oRouteDeleter.DeleteRoute (nxRouteID);

        m_pTransitSelection->RemoveSelect(nxRouteID);
        //m_pMapView->RemoveTransitDraw(nxRouteID, false);
        iter = m_pTransitSelection->begin();
    }
	
    m_pMapView->StopShapeEdit();
	m_pMapView->RefreshTransitMapDraw();

    m_pTable->Notify();
	KInitRelationProcess::DBRecordNotify(pTarget, m_pTable->Name());

    if (true) {
        KIOTable* pTable = pTarget->Tables()->FindTable(TABLE_TRANSITNODES);
        pTable->Notify();
    }
}


void KIOTransitTableView::OnTransitEditInsert()
{
	CMainFrameWnd* pMainFrame = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame* pChildFrame  = (CChildFrame*)(pMainFrame->MDIGetActive());
	if(pChildFrame->IsMapViewShowed() == false)
	{
		pChildFrame->ShowMapView(true);
	}

	//m_pMapView->NotifyTransitEditInsert(nRouteID);
	m_pMapView->PostMessage(WM_USER + 101, 0);
}


void KIOTransitTableView::OnTransitEditModify()
{
    if ( m_pTransitSelection->SelectedCount() > 1)
    {
        AfxMessageBox(_T("한 개 노선만 선택해 주세요."));
        return;
    }

    if ( m_pTransitSelection->SelectedCount() == 0)
    {
        AfxMessageBox(_T("편집할 노선을 선택해 주세요."));
        return;
    }

	CMainFrameWnd* pMainFrame = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());
	if(pChildFrame->IsMapViewShowed() == false)
	{
		pChildFrame->ShowMapView(true);
	}

	//CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	//int nIndex = pRow->GetIndex();
	//const KIORow* pIORow = m_pRecordset->GetRow(nIndex);
	//const KIOItem* pItem = pIORow->GetItem(0);
	//int nRouteID = pItem->GetValueAsInteger();

	////m_pMapView->NotifyTransitEditModify(nRouteID);
	//m_pMapView->PostMessage(WM_USER + 102, nRouteID);

    std::set<Integer>::iterator iter = m_pTransitSelection->begin();
    while(iter != m_pTransitSelection->end())
    {
        int nRouteID = (int)*iter;
        m_pMapView->PostMessage(WM_USER + 102, nRouteID);
        break;
    }
}


void KIOTransitTableView::OnTransitEditAppend()
{
    if ( m_pTransitSelection->SelectedCount() > 1)
    {
        AfxMessageBox(_T("한 개 노선만 선택해 주세요."));
        return;
    }

    if ( m_pTransitSelection->SelectedCount() == 0)
    {
        AfxMessageBox(_T("편집할 노선을 선택해 주세요."));
        return;
    }
	CMainFrameWnd* pMainFrame = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame* pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());
	if(pChildFrame->IsMapViewShowed() == false)
	{
		pChildFrame->ShowMapView(true);
	}

	/*CXTPReportRow* pRow = m_wndReport.GetFocusedRow();
	int nIndex = pRow->GetIndex();
	const KIORow* pIORow = m_pRecordset->GetRow(nIndex);
	const KIOItem* pItem = pIORow->GetItem(0);
	int nRouteID = pItem->GetValueAsInteger();*/

    std::set<Integer>::iterator iter = m_pTransitSelection->begin();
    while(iter != m_pTransitSelection->end())
    {
        int nRouteID = (int)*iter;
        m_pMapView->PostMessage(WM_USER + 103, nRouteID);
        break;
    }
	//m_pMapView->NotifyTransitEditAppend(nRouteID);
	
}

void KIOTransitTableView::OnReportSelectChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    //AfxMessageBox(_T("tt"));
}


void KIOTransitTableView::OnUpdateTransitlineeditInsert(CCmdUI *pCmdUI)
{
    
}


void KIOTransitTableView::OnUpdateTransitlineeditModify(CCmdUI *pCmdUI)
{
    if ( m_pTransitSelection->SelectedCount() >= 0)
    {
        pCmdUI->Enable(TRUE);
    }
}


void KIOTransitTableView::OnUpdateTransitlineeditAppend(CCmdUI *pCmdUI)
{
    if ( m_pTransitSelection->SelectedCount() >= 0)
    {
        pCmdUI->Enable(TRUE);
    }
}


void KIOTransitTableView::OnTransitZoomExtent()
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
		const KIOItem* pItem       = pIORow->GetItem(0);
		Integer        nxTransitID = pItem->GetValueAsInteger64();

		CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
		CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());

		if(pChildFrame->IsMapViewShowed() == false)
		{
			pChildFrame->ShowMapView(true);
		}

		m_pTransitSelection->AddSelect(nxTransitID);

		m_pMapView->ZoomTransitExtent(nxTransitID);    
		m_pMapView->AddTransitMapDraw(nxTransitID);
		m_wndReport.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}


void KIOTransitTableView::OnUpdateTransitZoomExtent(CCmdUI *pCmdUI)
{
    
}


void KIOTransitTableView::OnTransitZoomFirst()
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
		const KIOItem* pItem       = pIORow->GetItem(0);
		Integer        nxTransitID = pItem->GetValueAsInteger64();

		CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
		CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());

		if(pChildFrame->IsMapViewShowed() == false)
		{
			pChildFrame->ShowMapView(true);
		}

		m_pTransitSelection->AddSelect(nxTransitID);

		m_pMapView->ZoomTransitFirst (nxTransitID);
		m_pMapView->AddTransitMapDraw(nxTransitID);
		m_wndReport.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}


void KIOTransitTableView::OnUpdateTransitZoomFirst(CCmdUI *pCmdUI)
{
    // TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}


void KIOTransitTableView::OnTransitZoomLast()
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
		const KIOItem* pItem       = pIORow->GetItem(0);
		Integer        nxTransitID = pItem->GetValueAsInteger64();

		CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
		CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());

		if(pChildFrame->IsMapViewShowed() == false)
		{
			pChildFrame->ShowMapView(true);
		}

		m_pTransitSelection->AddSelect(nxTransitID);

		m_pMapView->ZoomTransitLast  (nxTransitID);    
		m_pMapView->AddTransitMapDraw(nxTransitID);

		m_wndReport.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}


void KIOTransitTableView::OnUpdateTransitZoomLast(CCmdUI *pCmdUI)
{
    
}


void KIOTransitTableView::OnTransitLineInfo()
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
		const KIOItem* pItem       = pIORow->GetItem(0);
		Integer        nxTransitID = pItem->GetValueAsInteger64();
        
		m_pController->ShowTransitLineInfo(nxTransitID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KIOTransitTableView::OnTransitLineThemes()
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
		const KIOItem* pItem       = pIORow->GetItem(0);
		Integer        nxTransitID = pItem->GetValueAsInteger64();
		m_pMapView->ZoomTransitExtent(nxTransitID);
		m_pMapView->ShowTransitLineThemes(nxTransitID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KIOTransitTableView::OnUpdateTransitLineThemes(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif
	pCmdUI->Enable(TRUE);
}


void KIOTransitTableView::OnTransitChart()
{
	try
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		CXTPReportRow*          pSelectedRow  = pSelectedRows->GetAt(0);
		int   nIndex          = pSelectedRow->GetIndex();
		const KIORow*           pIORow        = m_pRecordset->GetRow(nIndex);

		Integer nxTransitID   = pIORow->GetItem(0)->GetValueAsInteger64();
        m_pMapView->ZoomTransitExtent(nxTransitID);
		m_pController->ShowTransitChart(nxTransitID);
	}
	catch (...)
	{
		return;
	}
}


void KIOTransitTableView::OnUpdateTransitChart(CCmdUI *pCmdUI)
{
// #if _TRANSIT_EXCEPT_VER
// 	pCmdUI->Enable(FALSE);
// #else
// 	pCmdUI->Enable(TRUE);
// #endif
	pCmdUI->Enable(TRUE);
}


void KIOTransitTableView::OnTransitLinkViaTransit()
{
	try
	{
		m_pController->TransitLinkViaTransit();    
	}
	catch (...)
	{
		return;
	}
}

void KIOTransitTableView::OnTransitNodeViaTransit()
{
    try
    {
        m_pController->TransitNodeViaTransit();    
    }
    catch (...)
    {
        return;
    }
}

void KIOTransitTableView::OnTransitLinkStyle()
{
    try
    {
        m_pController->TransitLinkStyle();
    }
    catch (...)
    {
        return;
    }
}

void KIOTransitTableView::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, Integer a_nxObjectID/*=0*/ )
{
    try
    {
        ReloadData();
        m_pMapView->RefreshTransitMapDraw();
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
