#include "StdAfx.h"
#include "IOTerminalScheduleTableView.h"

#include "IOTableRecordSelection.h"
#include "IOTableRecord.h"
#include "DBaseIOTerminalScheduleTableView.h"
#include "BulkDBaseNode.h"
#include "MapView.h"
#include "MainFrameWnd.h"
#include "ChildFrm.h"

#include "IOTableController.h"


IMPLEMENT_DYNCREATE(KIOTerminalScheduleTableView, KIOTableView)

KIOTerminalScheduleTableView::KIOTerminalScheduleTableView(void)
{
	m_pTerminalScheduleSelection = new KIOTableRecordSelection;
}


KIOTerminalScheduleTableView::~KIOTerminalScheduleTableView(void)
{
	if (nullptr != m_pTerminalScheduleSelection)
	{
		m_pTerminalScheduleSelection->ClearSelection();

		delete m_pTerminalScheduleSelection;

		m_pTerminalScheduleSelection = nullptr;
	}
}

void KIOTerminalScheduleTableView::OnClose()
{
    if (m_pMapView) {
        m_pMapView->CacheClearInterRegionalTransit(true);
    }

    KIOTableView::OnClose();
}


void KIOTerminalScheduleTableView::OnDraw( CDC* pDC )
{
}

#ifdef _DEBUG
void KIOTerminalScheduleTableView::AssertValid() const
{
	KIOTableView::AssertValid();
}

#ifndef _WIN32_WCE
void KIOTerminalScheduleTableView::Dump( CDumpContext& dc ) const
{
	KIOTableView::Dump(dc);
}

#endif
#endif

BEGIN_MESSAGE_MAP(KIOTerminalScheduleTableView, KIOTableView)
    ON_WM_CLOSE()
	ON_NOTIFY (NM_RCLICK,                XTP_ID_REPORT_CONTROL, &KIOTerminalScheduleTableView::OnReportRowRButtonClick)
	ON_NOTIFY (XTP_NM_REPORT_CHECKED,    XTP_ID_REPORT_CONTROL, &KIOTerminalScheduleTableView::OnReportCheckItem)
	ON_COMMAND(ID_TERMINALSCHEDULE_SELECTALL,   &KIOTerminalScheduleTableView::OnSelectAllTerminalSchedule)
	ON_COMMAND(ID_TERMINALSCHEDULE_DESELECTALL, &KIOTerminalScheduleTableView::OnDeSelectAllTerminalSchedule)
	ON_COMMAND(ID_TERMINALSCHEDULE_ORGIN_TERMINAL,  &KIOTerminalScheduleTableView::OnOriginTerminalZoomTo)
	ON_COMMAND(ID_TERMINALSCHEDULE_DEST_TERMINAL,   &KIOTerminalScheduleTableView::OnDestTerminalZoomTo)
	ON_COMMAND(ID_TERMINALSCHEDULE_EXTENT_TERMINAL, &KIOTerminalScheduleTableView::OnTerminalExtentTo)
END_MESSAGE_MAP()

void KIOTerminalScheduleTableView::RegisterMapView( KMapView* a_pMapView )
{
	KIOTableView::RegisterMapView(a_pMapView);
}

void KIOTerminalScheduleTableView::UpdateRecord( void )
{
	CXTPReportControl& wndReport = GetReportCtrl();

	if (m_pRecordset != nullptr)
	{
		delete m_pRecordset;
		m_pRecordset = nullptr;
	}

	if (m_pTable != nullptr)
	{
		if (m_bApplyFilter)
		{
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeStatic, m_strFilter, m_strOrder);
		}
		else
		{
			m_pRecordset = m_pTable->GetRecordset(KEMIORecordsetTypeStatic, _T(""), m_strOrder);
		}

		int nRecordCount = m_pRecordset->GetRowCount();

		KCodeManager*  pCodeManager = nullptr;
		KIOTableOwner* pOwner       = m_pTable->GetOwner();

		if (pOwner != nullptr)
			pCodeManager = pOwner->GetCodeManager();

		m_pRecord = new KIOTerminalScheduleTableRecord(pCodeManager, m_pRecordset, m_pTerminalScheduleSelection);

		wndReport.SetVirtualMode(m_pRecord, nRecordCount);
		wndReport.Populate();
	}
}

void KIOTerminalScheduleTableView::OnReportCheckItem( NMHDR* pNMHDR, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	ASSERT(NULL != pItemNotify);

	try
	{
		int           nIndex  = pItemNotify->pRow->GetIndex();
		const KIORow* pRow    = m_pRecordset->GetRow(nIndex);
		KIOItem*      pIOItem = pRow->GetItem(0);
		int           nxID    = pIOItem->GetValueToInt();

		if(m_pTerminalScheduleSelection->IsSelected(nxID) == true)
		{
			m_pTerminalScheduleSelection->RemoveSelect(nxID);
		}
		else
		{
			m_pTerminalScheduleSelection->AddSelect(nxID);
		}

		DrawThemes();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KIOTerminalScheduleTableView::OnSelectAllTerminalSchedule( void )
{
	m_pTerminalScheduleSelection->ClearSelection();

	int nRowCount = m_pRecordset->GetRowCount();

	for (int i = 0; i < nRowCount; ++i)
	{
		KIRowCPTR pRow    = m_pRecordset->GetRow(i);
		KIOItem*  pIOItem = pRow->GetItem(0);

		Integer   nxTerminalID = pIOItem->GetValueAsInteger64();

		m_pTerminalScheduleSelection->AddSelect(nxTerminalID);
	}

	DrawThemes();

	m_wndReport.RedrawControl();
}

void KIOTerminalScheduleTableView::OnDeSelectAllTerminalSchedule( void )
{
	m_pTerminalScheduleSelection->ClearSelection();

	DrawThemes();

	m_wndReport.RedrawControl();
}

void KIOTerminalScheduleTableView::DrawThemes( void )
{
	m_pMapView->CacheClearInterRegionalTransit(true);

	CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)(AfxGetMainWnd());
	CChildFrame*   pChildFrame = (CChildFrame*)(pMainFrame->MDIGetActive());
	KTarget*       pTarget     = pChildFrame->GetTarget();

	if(!pChildFrame->IsMapViewShowed())
		pChildFrame->ShowMapView(true);

	std::set<Integer>          setTerminal = m_pTerminalScheduleSelection->GetSelection();
	std::vector<TTerminalInfo> vecTerminalInfo;

	//전체 Node의 X,Y 좌표를 조회
	std::map<Integer, TNodeCoords> mapAllNodeCoords;
	KBulkDBaseNode::AllNodeCoords(pTarget, mapAllNodeCoords);

	KDBaseIOTerminalScheduleTableView::GetODTerimnalBySEQ(pTarget, setTerminal, vecTerminalInfo);
	std::map<Integer, TNodeCoords>::iterator findIter;
	
	//Generate VecUserPointInfo
	std::vector<TUserPointInfo> vecUserPoint;

	COLORREF clrDefOrginSymbol = RGB(255, 0, 0);
	COLORREF clrDefDestSymbol  = RGB(0, 0, 255);
	double   dDefSymbolSize    = 5;

	TNodeCoords oNodeCoords;

	int nSize = vecTerminalInfo.size();
	
	for (int i = 0; i < nSize; ++i)
	{
		TTerminalInfo oTerminalInfo = vecTerminalInfo[i];

		//OriginID
		findIter = mapAllNodeCoords.find(oTerminalInfo.nxOTerminal);

		if (findIter == mapAllNodeCoords.end())
			ThrowException(_T("Node 테이블에 존재하지 않는 Terminal ID입니다."));

		oNodeCoords = findIter->second;

		TUserPointInfo oUserOriginPointInfo;

		oUserOriginPointInfo.Tx        = oNodeCoords.TdX;
		oUserOriginPointInfo.Ty        = oNodeCoords.TdY;
		oUserOriginPointInfo.TColorRGB = clrDefOrginSymbol;
		oUserOriginPointInfo.TLabel    = _T("");
		oUserOriginPointInfo.TSize     = dDefSymbolSize;

		vecUserPoint.push_back(oUserOriginPointInfo);

		//DestID
		findIter = mapAllNodeCoords.find(oTerminalInfo.nxDTerminal);

		if (findIter == mapAllNodeCoords.end())
			ThrowException(_T("Node 테이블에 존재하지 않는 Terminal ID입니다."));

		oNodeCoords = findIter->second;

		TUserPointInfo oUserDestPointInfo;

		oUserDestPointInfo.Tx        = oNodeCoords.TdX;
		oUserDestPointInfo.Ty        = oNodeCoords.TdY;
		oUserDestPointInfo.TColorRGB = clrDefDestSymbol;
		oUserDestPointInfo.TLabel    = _T("");
		oUserDestPointInfo.TSize     = dDefSymbolSize;

		vecUserPoint.push_back(oUserDestPointInfo);
	}	
	
	//Generate Vector_UserLineInfo;
	std::vector<TUserLineInfo> vecUserLine;
	KMunsellColor oMunsellColor(0, false);

	double dDefaultWidth = 2;

	nSize = vecTerminalInfo.size();

	for (int i = 0; i < nSize; ++i)
	{
		TTerminalInfo oTerminalInfo = vecTerminalInfo[i];
		TUserLineInfo oUserLineInfo;
		
		std::vector<TMapPoint> oPoints;
		TMapPoint oPoint;

		//OTerminal
		findIter = mapAllNodeCoords.find(oTerminalInfo.nxOTerminal);

		if (findIter == mapAllNodeCoords.end())
			ThrowException(_T("Node 테이블에 존재하지 않는 Terminal ID입니다."));

		oPoint.Tx = findIter->second.TdX;
		oPoint.Ty = findIter->second.TdY;

		oPoints.push_back(oPoint);

		//DTerminal
		findIter = mapAllNodeCoords.find(oTerminalInfo.nxDTerminal);

		if (findIter == mapAllNodeCoords.end())
			ThrowException(_T("Node 테이블에 존재하지 않는 Terminal ID입니다."));

		oPoint.Tx = findIter->second.TdX;
		oPoint.Ty = findIter->second.TdY;

		oPoints.push_back(oPoint);

		oUserLineInfo.TColorRGB = oMunsellColor.GetColor((int)oTerminalInfo.nxSeq);
		oUserLineInfo.TWidth    = dDefaultWidth;
		oUserLineInfo.TLabel    = _T("");
		oUserLineInfo.TPoints   = oPoints;

		vecUserLine.push_back(oUserLineInfo);
	}

	m_pMapView->CacheDrawInterRegionalTransit(vecUserLine, vecUserPoint, false, true);
}

void KIOTerminalScheduleTableView::OnReportRowRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	if( NULL == pItem->pColumn )
	{
		return;
	}

	pItem->pRow->SetSelected(TRUE);

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_IOTERMINALSCHEDULE);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
	CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
	pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, 
		pItem->pt.x, pItem->pt.y, this);

	*pResult = 0;
}

void KIOTerminalScheduleTableView::OnOriginTerminalZoomTo( void )
{
	if (m_pController != nullptr)
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();

		if (pSelectedRows->GetCount() == 0)
			return;

		CXTPReportRow* pReportRow = pSelectedRows->GetAt(0);

		if (pReportRow == nullptr)
			return;

		const KIORow* pRow = m_pRecordset->GetRow(pReportRow->GetIndex());

		Integer nxTerminalID = pRow->GetItem(0)->GetValueAsInteger64();

		if (!m_pTerminalScheduleSelection->IsSelected(nxTerminalID))
		{
			m_pTerminalScheduleSelection->AddSelect(nxTerminalID);
			DrawThemes();
		}

		
		Integer nxStartNodeID = pRow->GetItem(1)->GetValueAsInteger64();

		m_pController->ZoomNode(nxStartNodeID);
	}
}

void KIOTerminalScheduleTableView::OnDestTerminalZoomTo( void )
{
	if (m_pController != nullptr)
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();

		if (pSelectedRows->GetCount() == 0)
			return;

		CXTPReportRow* pReportRow = pSelectedRows->GetAt(0);

		if (pReportRow == nullptr)
			return;

		const KIORow* pRow = m_pRecordset->GetRow(pReportRow->GetIndex());

		Integer nxTerminalID = pRow->GetItem(0)->GetValueAsInteger64();

		if (!m_pTerminalScheduleSelection->IsSelected(nxTerminalID))
		{
			m_pTerminalScheduleSelection->AddSelect(nxTerminalID);
			DrawThemes();
		}

		Integer nxEndNodeID = pRow->GetItem(2)->GetValueAsInteger64();

		m_pController->ZoomNode(nxEndNodeID);
	}
}

void KIOTerminalScheduleTableView::OnTerminalExtentTo( void )
{
	if (m_pController != nullptr)
	{
		CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();

		if (pSelectedRows->GetCount() == 0)
			return;

		CXTPReportRow* pReportRow = pSelectedRows->GetAt(0);

		if (pReportRow == nullptr)
			return;

		const KIORow* pRow = m_pRecordset->GetRow(pReportRow->GetIndex());
		
		Integer nxTerminalID = pRow->GetItem(0)->GetValueAsInteger64();

		if (!m_pTerminalScheduleSelection->IsSelected(nxTerminalID))
		{
			m_pTerminalScheduleSelection->AddSelect(nxTerminalID);
			DrawThemes();
		}

		std::vector<Integer> vecNodes;
		
		vecNodes.push_back(pRow->GetItem(1)->GetValueAsInteger64());
		vecNodes.push_back(pRow->GetItem(2)->GetValueAsInteger64());
		
		m_pController->ExtentToNodes(vecNodes);
	}
}

