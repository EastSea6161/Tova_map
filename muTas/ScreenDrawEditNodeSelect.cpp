#include "StdAfx.h"
#include "ScreenDrawEditNodeSelect.h"

#include "CommandWnd.h"
#include "ImChampCommand.h"
#include "BulkDBaseLink.h"
#include "Target.h"
#include "IMapFeedback.h"


KScreenDrawEditNodeSelect::KScreenDrawEditNodeSelect( IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver, std::vector<TNodeSelectionInfo>& ar_vecTNodeStation, bool a_bLabel, double a_dDrawScale) 
	: IScreenDrawEdit(a_mapRequest), m_pSlaveMapSelectionReceiver(a_pMapSelectionReceiver)
{
	m_bDrawLabel     = a_bLabel;
	m_dDrawScale     = a_dDrawScale;

	a_mapRequest->SetMapMode(KEMMapModeScreenDrawEdit);	
	m_vecNodeSelectionInfo.assign(ar_vecTNodeStation.begin(), ar_vecTNodeStation.end());
	size_t nxCount = m_vecNodeSelectionInfo.size();
	for(size_t i=0; i<nxCount; i++)
	{
		TNodeSelectionInfo& oTNodeInfo = m_vecNodeSelectionInfo[i];
        m_vecSelectableNodeID.push_back(oTNodeInfo.TNodeID);
	}
}

KScreenDrawEditNodeSelect::~KScreenDrawEditNodeSelect( void )
{	
	StopEditing();
}

void KScreenDrawEditNodeSelect::StartEditing()
{
    m_pIMapView->SetMapMode(KEMMapModeScreenDrawEdit);	
    m_spFlashNode = KScreenDrawFlashNodePtr(new KScreenDrawFlashNode(m_pIMapView, this));
    m_spFlashNode->SetSelectable(m_vecSelectableNodeID);

	m_bScreenDrawEditing = true;
	SelectCommandMsg();

	m_pIMapView->SetMapCursor(IDC_EditingEditTool);
	Draw();
}

void KScreenDrawEditNodeSelect::Draw()
{
    if ( m_spFlashNode == nullptr)
        return;

    m_spFlashNode->Draw();
}

void KScreenDrawEditNodeSelect::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if ( m_spFlashNode == nullptr)
        return;

    if (MK_MBUTTON & nFlags)
        return;

    m_spFlashNode->MouseDown(nFlags, X, Y, mapX, mapY);
}

void KScreenDrawEditNodeSelect::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if ( m_spFlashNode == nullptr)
        return;

    if (MK_MBUTTON & nFlags)
        return;

    m_spFlashNode->MouseMove(nFlags, X, Y, mapX, mapY);
}

void KScreenDrawEditNodeSelect::CommandFeedback( CString a_strResult )
{	
	if (a_strResult.CompareNoCase(_T("C")) == 0)
	{
		StopEditing();
		return;
	}
}

void KScreenDrawEditNodeSelect::Canceled()
{
	StartEditing();
}

void KScreenDrawEditNodeSelect::SelectCommandMsg()
{
	KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
	ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Select:"), this));
	if (pCommandWindow)
		pCommandWindow->Command(spCommand, KEMModeless);

	//m_pIMapView->TasESRICacheRefresh();
}

void KScreenDrawEditNodeSelect::MapSelectionFeedback( Integer a_nxID )
{
	Integer nxObjectKey = a_nxID;

	CString strLog;
	strLog.Format(_T("Selected : %I64d"), nxObjectKey);
	SendModelessCommandMsg(strLog);

	if ( m_pSlaveMapSelectionReceiver != nullptr )
	{
		m_pSlaveMapSelectionReceiver->MapSelectionFeedback(nxObjectKey);
	}
}

void KScreenDrawEditNodeSelect::MapSelectionNoneFeedback()
{
	SendModelessCommandMsg(_T("Select:"));
	if ( m_pSlaveMapSelectionReceiver != nullptr )
	{
		m_pSlaveMapSelectionReceiver->MapSelectionNoneFeedback();
	}
}

void KScreenDrawEditNodeSelect::SaveEditing()
{
}

void KScreenDrawEditNodeSelect::StopEditing()
{
	m_bScreenDrawEditing = false;
	m_pIMapView->SetMapMode(KEMMapModePan);

	KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
	if (pCommandWindow)
		pCommandWindow->SetDefaultCommand();

    m_pIMapView->MapForegroundRefresh();
}


void KScreenDrawEditNodeSelect::ActivatedFrameWndNotifyProcess()
{
	
}

void KScreenDrawEditNodeSelect::DeActivatedFrameWndNotifyProcess()
{
	
}


