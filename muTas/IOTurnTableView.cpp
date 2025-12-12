
#include "StdAfx.h"
#include "KmzApp.h"
#include "IOTurnTableView.h"
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
#include "BulkDBaseTurn.h"
IMPLEMENT_DYNCREATE(KIOTurnTableView, KIOTableView)


KIOTurnTableView::KIOTurnTableView(void)
{
	m_pTurnSelection    = new KIOTableRecordSelection;
}


KIOTurnTableView::~KIOTurnTableView(void)
{
	if(NULL != m_pTurnSelection)
	{
		delete m_pTurnSelection;
	}
}

void KIOTurnTableView::OnClose()
{
    if (m_pMapView) {
        m_pMapView->RemoveTurnMapDraw();   
    }

    KIOTableView::OnClose();
}

void KIOTurnTableView::OnDraw(CDC* pDC)
{
}


#ifdef _DEBUG
void KIOTurnTableView::AssertValid() const
{
	KIOTableView::AssertValid();
}


#ifndef _WIN32_WCE
void KIOTurnTableView::Dump(CDumpContext& dc) const
{
	KIOTableView::Dump(dc);
}
#endif
#endif


BEGIN_MESSAGE_MAP(KIOTurnTableView, KIOTableView)
    ON_WM_CLOSE()
	ON_NOTIFY (NM_RCLICK,                XTP_ID_REPORT_CONTROL, &KIOTurnTableView::OnReportRowRButtonClick)
	ON_NOTIFY (XTP_NM_REPORT_CHECKED,    XTP_ID_REPORT_CONTROL, &KIOTurnTableView::OnReportCheckItem      )
    ON_NOTIFY (XTP_NM_REPORT_SELCHANGED, XTP_ID_REPORT_CONTROL, &KIOTurnTableView::OnReportSelectChanged )
    ON_COMMAND(ID_TURN_SELECTALLTURN, &KIOTurnTableView::OnTurnSelectAllTurn)
    ON_COMMAND(ID_TURN_DESELECTALLTURN, &KIOTurnTableView::OnTurnDeselectAllTurn)
    ON_COMMAND(ID_TURN_DELETETURN, &KIOTurnTableView::OnTurnDeleteTurn)
END_MESSAGE_MAP()


std::set<Integer> KIOTurnTableView::GetSelectedSet(void)
{
	return m_pTurnSelection->GetSelection();
}


void KIOTurnTableView::RegisterMapView(KMapView* pMapView)
{
	KIOTableView::RegisterMapView(pMapView);
	//pMapView->RefreshTransitMapDraw(m_bShowTransitValueMap);
    //OnTurnSelectAllTurn();
}


void KIOTurnTableView::NotifyTurnCreatedFromMap(Integer nxID)
{	
	UpdateRecord();
    m_pTurnSelection->AddSelect(nxID);
    if(NULL != m_pMapView) {
        m_pMapView->AddTurnMapDraw(nxID, true);
    }
}


void KIOTurnTableView::NotifyTurneModifyedFromMap(Integer nxID)
{	
	UpdateRecord();
    m_pTurnSelection->AddSelect(nxID);
}


void KIOTurnTableView::UpdateRecord(void)
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
		m_pRecord = new KIOSelectionTableRecord(pCodeManager, m_pRecordset, m_pTurnSelection);
		wndReport.SetVirtualMode(m_pRecord, nRecordCount);
	}
	else 
	{
		QBicDelete(m_pRecordset);
	}

	wndReport.Populate();

	//m_pTransitSelection->ClearSelectedTransit();	
}


void KIOTurnTableView::OnReportRowRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{   
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	if( NULL == pItem->pColumn )
	{
		return;
	}

    pItem->pRow->SetSelected(TRUE);

    CMenu menu;
    menu.LoadMenu( IDR_POPUP_TABLETABVIEW );
    CMenu* pPopup = menu.GetSubMenu( 0 );

	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
	pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);

	*pResult = 0;
}


void KIOTurnTableView::OnReportCheckItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	ASSERT(NULL != pItemNotify);
	
	int      nIndex     = pItemNotify->pRow->GetIndex();
	const 
    KIORow*  pRow       = m_pRecordset->GetRow(nIndex);
	
    KIOItem* pIOItem    = pRow->GetItem(0);
    int      nxID = pIOItem->GetValueToInt();

    if(m_pTurnSelection->IsSelected(nxID) == true)
    {
        m_pTurnSelection->RemoveSelect(nxID);
        if(NULL != m_pMapView)
        {
            m_pMapView->RemoveTurnMapDraw(nxID, true);
        }
    }
    else
    {
        m_pTurnSelection->AddSelect(nxID);
        if(NULL != m_pMapView)
        {
            m_pMapView->AddTurnMapDraw(nxID, true);
        }
    }
}



void KIOTurnTableView::OnReportSelectChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    //AfxMessageBox(_T("tt"));
}



void KIOTurnTableView::OnTurnSelectAllTurn()
{
    m_pTurnSelection->ClearSelection();

    int nRowCount = m_pRecordset->GetRowCount();
    for(int i = 0; i < nRowCount; ++i)
    {
        KIRowCPTR  pRow       = m_pRecordset->GetRow(i);
        KIOItem*   pIOItem    = pRow->GetItem       (0);

        Integer   nxID = pIOItem->GetValueAsInteger64();

        m_pTurnSelection->AddSelect(nxID);	
        if(NULL != m_pMapView)
        {
            if (i == nRowCount -1)
            {
                m_pMapView->AddTurnMapDraw(nxID, true);
            }
            else
            {
                m_pMapView->AddTurnMapDraw(nxID, false);
            }            
        }
    }

    m_wndReport.RedrawControl();
}


void KIOTurnTableView::OnTurnDeselectAllTurn()
{
    m_pTurnSelection->ClearSelection();
    m_pMapView->RemoveTurnMapDraw();
    m_wndReport.RedrawControl();
}


void KIOTurnTableView::OnTurnDeleteTurn()
{
    CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());	

    if ( m_pTurnSelection->SelectedCount() == 0)
    {
        AfxMessageBox(_T("회전제약 데이터를 선택해 주세요."));
        return;
    }

    CString strOutMsg;
    strOutMsg.Format(_T("선택된 %d개 회전제약 데이터를 삭제합니다."), m_pTurnSelection->SelectedCount());
    
    if ( AfxMessageBox(strOutMsg, MB_YESNO) == IDNO)
    {
        return;
    }

    KTarget*      pTarget     = pChildFrame->GetTarget();
    std::set<Integer>::iterator iter = m_pTurnSelection->begin();
    while(iter != m_pTurnSelection->end())
    {
        Integer nxID = *iter;
        // 삭제
        KBulkDBaseTurn::DeleteTurn(pTarget, nxID);
        m_pTurnSelection->RemoveSelect(nxID);
           
        iter = m_pTurnSelection->begin();
    }
	
    if(NULL != m_pMapView)
    {
        m_pMapView->RemoveTurnMapDraw();   
    }  

    m_pTable->Notify();
	//UpdateRecord();
}

void KIOTurnTableView::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, Integer a_nxObjectID/*=0*/ )
{
    try
    {
    	KIOTableView::NotifyProcess(a_strSubjectName, a_nxObjectID);
        RedrawTurn();
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

void KIOTurnTableView::RedrawTurn()
{
    if(NULL == m_pMapView) return;

    m_pMapView->RemoveTurnMapDraw();   

    std::set<Integer>::iterator iter = m_pTurnSelection->begin();
    while(iter != m_pTurnSelection->end())
    {
        m_pMapView->AddTurnMapDraw(*iter, false);
        ++iter;
    }

    m_pMapView->MapRefresh();
}
