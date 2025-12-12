
#include "StdAfx.h"
#include "KmzApp.h"
#include "IOIntersectionTableView.h"
//^^ #include "IOItem.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOTable.h"
#include "IOTableController.h"
#include "IOTableRecord.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapView.h"
//^^ #include "TableOwner.h"
#include "IOTableRecordSelection.h"
#include "BulkDBaseIntersection.h"
//^#include "DBaseConnector.h"
#include "IntersectionTopologyPage.h"
#include "IntersectionBasicPage.h"
#include "IntersectionThreeBasicPage.h"
#include "IntersectionExtensionPage.h"

IMPLEMENT_DYNCREATE(KIOIntersectionTableView, KIOTableView)
    
KIOIntersectionTableView::KIOIntersectionTableView(void)
{
    m_pIntersectionSelection = new KIOTableRecordSelection;
}


KIOIntersectionTableView::~KIOIntersectionTableView(void)
{
    if(NULL != m_pIntersectionSelection)
    {
        delete m_pIntersectionSelection;
    }
}

void KIOIntersectionTableView::OnClose()
{
    if (m_pMapView) {
        m_pMapView->ClearIntersectionMapDraw();
    }

    KIOTableView::OnClose();
}

void KIOIntersectionTableView::OnDraw(CDC* pDC)
{
}


#ifdef _DEBUG
void KIOIntersectionTableView::AssertValid() const
{
    KIOTableView::AssertValid();
}


#ifndef _WIN32_WCE
void KIOIntersectionTableView::Dump(CDumpContext& dc) const
{
    KIOTableView::Dump(dc);
}
#endif
#endif


BEGIN_MESSAGE_MAP(KIOIntersectionTableView, KIOTableView)
    ON_WM_CLOSE()
    ON_NOTIFY (NM_RCLICK,                XTP_ID_REPORT_CONTROL, &KIOIntersectionTableView::OnReportRowRButtonClick)
    ON_NOTIFY (XTP_NM_REPORT_CHECKED,    XTP_ID_REPORT_CONTROL, &KIOIntersectionTableView::OnReportCheckItem      )
    ON_NOTIFY (XTP_NM_REPORT_SELCHANGED, XTP_ID_REPORT_CONTROL, &KIOIntersectionTableView::OnReportSelectChanged )
    ON_COMMAND(ID_INTERSECTION_MODIFY, &KIOIntersectionTableView::OnIntersectionModify)
    ON_COMMAND(ID_INTERSECTION_DELETE, &KIOIntersectionTableView::OnIntersectionDelete)
    ON_COMMAND(ID_INTERSECTION_SELECTALLINTERSECTION, &KIOIntersectionTableView::OnIntersectionSelectallIntersection)
    ON_COMMAND(ID_INTERSECTION_DESELECTALLINTERSECTION, &KIOIntersectionTableView::OnIntersectionDeselectAllIntersection)
    ON_COMMAND(ID_INTERSECTION_DELETEINTERSECTION, &KIOIntersectionTableView::OnIntersectionMultiDelete)
    ON_COMMAND(ID_INTERSECTION_NETWORKTOPOLOGYANALYSIS, &KIOIntersectionTableView::OnIntersectionNetworkTopologyAnalysis)
END_MESSAGE_MAP()


std::set<Integer> KIOIntersectionTableView::GetSelectedSet(void)
{
    return m_pIntersectionSelection->GetSelection();
}


void KIOIntersectionTableView::RegisterMapView(KMapView* pMapView)
{
    KIOTableView::RegisterMapView(pMapView);
    //pMapView->RefreshTransitMapDraw(m_bShowTransitValueMap);
    //★ 최초 선택되지 않도록.... OnIntersectionSelectallIntersection();
}


void KIOIntersectionTableView::NotifyIntersectionCreatedFromMap(Integer nxID)
{	
    UpdateRecord();
    m_pIntersectionSelection->AddSelect(nxID);
    //m_pMapView->RefreshTransitMapDraw(m_bShowTransitValueMap);
}


void KIOIntersectionTableView::NotifyIntersectionModifyedFromMap(Integer nxID)
{	
    UpdateRecord();
    m_pIntersectionSelection->AddSelect(nxID);
    //m_pMapView->RefreshTransitMapDraw(m_bShowTransitValueMap);
}


void KIOIntersectionTableView::UpdateRecord(void)
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
		m_pRecord = new KIOSelectionTableRecord(pCodeManager, m_pRecordset, m_pIntersectionSelection);
		wndReport.SetVirtualMode(m_pRecord, nRecordCount);
	}
	else 
	{
		QBicDelete(m_pRecordset);
	}

    wndReport.Populate();
}


void KIOIntersectionTableView::OnReportRowRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{   
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
    if( NULL == pItemNotify->pColumn )
    {
        return;
    }

    pItemNotify->pRow->SetSelected(TRUE);

    CMenu menu;
    menu.LoadMenu( IDR_POPUP_TABLETABVIEW );
    CMenu* pPopup = menu.GetSubMenu( 1 );

    CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
    CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
    pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, pItemNotify->pt.x, pItemNotify->pt.y, this);

    *pResult = 0;
}


void KIOIntersectionTableView::OnReportCheckItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	ASSERT(NULL != pItemNotify);

	try
	{
		int      nIndex     = pItemNotify->pRow->GetIndex();
		const 
			KIORow*  pRow       = m_pRecordset->GetRow(nIndex);

		KIOItem* pIOItem    = pRow->GetItem(0);
		int      nxID = pIOItem->GetValueToInt();

		if(m_pIntersectionSelection->IsSelected(nxID) == true)
		{
			m_pIntersectionSelection->RemoveSelect(nxID);
			if(NULL != m_pMapView)
			{
				m_pMapView->RemoveIntersectionMapDraw(nxID, true);
			}
		}
		else
		{
			m_pIntersectionSelection->AddSelect(nxID);
			if(NULL != m_pMapView)
			{
				m_pMapView->AddIntersectionMapDraw(nxID, true);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIOIntersectionTableView::OnReportSelectChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    //AfxMessageBox(_T("tt"));
}


bool KIOIntersectionTableView::GetIntersection(TIntersectionBase &a_oTSelectIntersection)
{
	if(NULL != m_pController)
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();
		if (pSelectedRows->GetCount() == 0)
			return false;

		CXTPReportRow*          pRow          = pSelectedRows->GetAt(0);

		if(NULL != pRow)
		{
			int nIndex  = pRow->GetIndex();
			const KIORow* pRow = m_pRecordset->GetRow(nIndex);
			KIORow row = *pRow;

			KIOItem* pItem = NULL;
			pItem = row.GetItem(0);
			a_oTSelectIntersection.nxNodeID      = pItem->GetValueAsInteger64();
			
			pItem = row.GetItem(1);
			a_oTSelectIntersection.nType         = pItem->GetValueToInt();

			pItem = row.GetItem(2);
			a_oTSelectIntersection.nxFirstBNode  = pItem->GetValueAsInteger64();

			pItem = row.GetItem(3);
			a_oTSelectIntersection.nxSecondBNode = pItem->GetValueAsInteger64();

			pItem = row.GetItem(4);
			a_oTSelectIntersection.nxThirdBNode  = pItem->GetValueAsInteger64();

			pItem = row.GetItem(5);
			a_oTSelectIntersection.nxFourthBNode = pItem->GetValueAsInteger64();

			pItem = row.GetItem(6);
			a_oTSelectIntersection.nCrossType    = pItem->GetValueToInt();

			return true;
		}
	}

	return false;
}


void KIOIntersectionTableView::GetIntersectionBoundInfo( KDBaseConPtr spDBaseConnection, TIntersectionBase &a_oTSelectIntersection, std::map<Integer, TBoundInfo> &a_mapBoundInfo )
{
	CString strSQL;
	strSQL.Format (_T(" SELECT link_id, fnode_id, lane ")
		_T(" FROM link ")
		_T(" WHERE tnode_id = %I64d "), a_oTSelectIntersection.nxNodeID
		);

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return;
	}

	while( pResult->Next() )
	{
		TBoundInfo oTBoundInfo;

		oTBoundInfo.nxLinkID  = pResult->GetValueInt64(0);
		oTBoundInfo.nxNodeID  = pResult->GetValueInt64(1);
		oTBoundInfo.nLaneCnt  = pResult->GetValueInt  (2);

		a_mapBoundInfo.insert(std::make_pair(oTBoundInfo.nxNodeID, oTBoundInfo));
	}
}


#pragma region old OnIntersectionModify
// void KIOIntersectionTableView::OnIntersectionModify()
// {
// 	CMainFrameWnd*    pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
// 	CChildFrame*      pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());	
// 	KTarget*          pTarget     = pChildFrame->GetTarget();
// 
// 	TIntersectionBase oTSelectIntersection;
// 
// 	if( GetIntersection(oTSelectIntersection) == false )
// 	{
// 		return;
// 	}
// 
// 	std::map<Integer, TBoundInfo> mapBoundInfo;
// 	GetIntersectionBoundInfo(pTarget->GetDBaseConnection(), oTSelectIntersection, mapBoundInfo);
// 
// 	OnRecordZoomto();
// 
// 	int &nIntersectionType = oTSelectIntersection.nType;
// 
// 	CXTPPropertySheet oPropertySheet(_T("Intersection Data Edit"));
// 	CXTPPropertyPageTabNavigator* pTabNavi        = new CXTPPropertyPageTabNavigator();
// 	//CXTPPropertyPageTreeNavigator* pTabNavi        = new CXTPPropertyPageTreeNavigator(TVS_SHOWSELALWAYS | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT);
// 
// 	pTabNavi->SetLayoutStyle(xtpTabLayoutAutoSize);
// 	//pTabNavi->SetLayoutStyle(xtpTabLayoutCompressed);
// 	//pTabNavi->SetLayoutStyle(xtpTabLayoutFixed);
// 	//pTabNavi->SetLayoutStyle(xtpTabLayoutMultiRow);
// 	//pTabNavi->SetLayoutStyle(xtpTabLayoutRotated);
// 	//pTabNavi->SetLayoutStyle(xtpTabLayoutSizeToFit);
// 	std::vector<CXTPPropertyPage*> vecPageArray;
// 
// 	// page - topology 
// 	KIntersectionTopologyPage*     pTopologyPage  = new KIntersectionTopologyPage;
// 	pTopologyPage->SetCaption(_T("Topology"));
// 	pTopologyPage->SetDBConnection(m_pTable->GetDBConnection());
// 	pTopologyPage->SetIntersection(oTSelectIntersection);
// 	pTopologyPage->SetLaneBoundInfo(mapBoundInfo);
// 
// 	vecPageArray.push_back(pTopologyPage);
// 	oPropertySheet.AddPage(pTopologyPage);
// 	
// 	// page - Basic
// 	if( KEMIntersectionTypeThree == nIntersectionType )
// 	{
// 		KIntersectionThreeBasicPage*        pBasicPage     = new KIntersectionThreeBasicPage;
// 		pBasicPage->SetCaption(_T("Basic"));
// 		pBasicPage->SetDBConnection(m_pTable->GetDBConnection());
// 		pBasicPage->SetIntersection(oTSelectIntersection);
// 		pBasicPage->SetLaneBoundInfo(mapBoundInfo);
// 
// 		vecPageArray.push_back(pBasicPage);
// 		oPropertySheet.AddPage(pBasicPage);
// 	}
// 	else if( KEMIntersectionTypeFour == nIntersectionType ) 
// 	{
// 		KIntersectionBasicPage*        pBasicPage     = new KIntersectionBasicPage;
// 		pBasicPage->SetCaption(_T("Basic"));
// 		pBasicPage->SetDBConnection(m_pTable->GetDBConnection());
// 		pBasicPage->SetIntersection(oTSelectIntersection);
// 		pBasicPage->SetLaneBoundInfo(mapBoundInfo);
// 
// 		vecPageArray.push_back(pBasicPage);
// 		oPropertySheet.AddPage(pBasicPage);
// 	}
// 
// 	// page - Extension 
// 	KIntersectionExtensionPage*    pExtensionPage = new KIntersectionExtensionPage;
// 	pExtensionPage->SetCaption(_T("Extension"));
// 	pExtensionPage->SetDBConnection(m_pTable->GetDBConnection());
// 	pExtensionPage->SetIntersection(oTSelectIntersection);
// 
// 	vecPageArray.push_back(pExtensionPage);
// 	oPropertySheet.AddPage(pExtensionPage);
// 
// 	oPropertySheet.SetNavigator(pTabNavi);
// 	oPropertySheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
// 	oPropertySheet.RecalcLayout();
// 	/*oPropertySheet.SetResizable(TRUE);*/
// 	oPropertySheet.SetPageSize(CSize(319, 189));
// 
// 	if(oPropertySheet.DoModal() == IDOK)
// 	{
// 		m_pTable->Notify();
// 	}
// 
// 	std::vector<CXTPPropertyPage*>::iterator itPage, itPageEnd = vecPageArray.end();
// 	for(itPage = vecPageArray.begin(); itPage != itPageEnd; ++itPage)
// 	{
// 		delete *itPage;
// 	}
// }
#pragma endregion old OnIntersectionModify


void KIOIntersectionTableView::OnIntersectionModify()
{
	try
	{
		CMainFrameWnd*    pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
		CChildFrame*      pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());	
		KTarget*          pTarget     = pChildFrame->GetTarget();
 
		TIntersectionBase oTSelectIntersection;
		if( GetIntersection(oTSelectIntersection) == false )
		{
			return;
		}

		OnRecordZoomto();

		std::map<Integer, TBoundInfo> mapBoundInfo;
		GetIntersectionBoundInfo(pTarget->GetDBaseConnection(), oTSelectIntersection, mapBoundInfo);

		m_pController->ShowIntersectionModify(oTSelectIntersection, mapBoundInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KIOIntersectionTableView::OnIntersectionSelectallIntersection()
{
    try
    {
		CWaitCursor cw;
		m_pIntersectionSelection->ClearSelection();

    	int nCount = m_pRecordset->GetRowCount();
        for (int i=0; i<nCount; i++)
        {
            const 
            KIORow*  pRow    = m_pRecordset->GetRow(i);
            KIOItem* pIOItem = pRow->GetItem(0);
            Integer  nxID    = pIOItem->GetValueAsInteger64();

            m_pIntersectionSelection->AddSelect(nxID);
			if(NULL != m_pMapView)
			{
				if (i == nCount -1)
				{
					m_pMapView->AddIntersectionMapDraw(nxID, true);
				}
				else
				{
					m_pMapView->AddIntersectionMapDraw(nxID, false);
				}            
			}
        }

        RedrawReport();
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


void KIOIntersectionTableView::OnIntersectionDeselectAllIntersection()
{
    try
    {
		CWaitCursor cw;
		m_pIntersectionSelection->ClearSelection();

        int nCount = m_pRecordset->GetRowCount();
        for (int i=0; i<nCount; i++)
        {
            const 
            KIORow*  pRow    = m_pRecordset->GetRow(i);
            KIOItem* pIOItem = pRow->GetItem(0);
            Integer  nxID    = pIOItem->GetValueAsInteger64();

            m_pIntersectionSelection->RemoveSelect(nxID);
			if (nullptr != m_pMapView)
			{
				if (nCount -1 == i)
				{
					m_pMapView->RemoveIntersectionMapDraw(nxID, true);
				}
				else
				{
					m_pMapView->RemoveIntersectionMapDraw(nxID, false);
				}
			}
        }

        RedrawReport();
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

void KIOIntersectionTableView::OnIntersectionDelete()
{
    /*OnIntersectionMultiDelete();*/
}

void KIOIntersectionTableView::OnIntersectionMultiDelete()
{
    CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
    CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());	

    if ( m_pIntersectionSelection->SelectedCount() == 0)
    {
        AfxMessageBox(_T("삭제할 교차로 데이터를 선택해 주세요."));
        return;
    }

    CString strOutMsg(_T("선택된 교차로 데이터 정보를 삭제합니다."));
    if (m_pIntersectionSelection->SelectedCount() > 1)
        strOutMsg.Format(_T("선택된 %d개 교차로 데이터를 삭제합니다."), m_pIntersectionSelection->SelectedCount());

    if ( AfxMessageBox(strOutMsg, MB_YESNO) == IDNO) {
        return;
    }

    CWaitCursor cw;  
    
	KTarget* pTarget = pChildFrame->GetTarget();

    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    spDBaseConnection->BeginTransaction();

    try
    {
        std::set<Integer>::iterator iter = m_pIntersectionSelection->begin();
        while(iter != m_pIntersectionSelection->end())
        {
            Integer nxID = *iter;
            // 삭제
            KBulkDBaseIntersection::DeleteIntersection(spDBaseConnection, nxID);
            m_pIntersectionSelection->RemoveSelect(nxID);
            if(NULL != m_pMapView)
            {
                m_pMapView->RemoveIntersectionMapDraw(nxID, true);   
            }       
            iter = m_pIntersectionSelection->begin();
        }

        spDBaseConnection->Commit();
    }
    catch (...)
    {
        spDBaseConnection->RollBack();
    }

    UpdateRecord();
}


void KIOIntersectionTableView::OnIntersectionNetworkTopologyAnalysis()
{
    if (m_pMapView)
    {
        m_pMapView->IntersectionTopologyAnalysis();
    }
}
