#include "StdAfx.h"
#include "DefineNames.h"
#include "ShapeEditController.h"
#include "MapView.h"


KShapeEditController::KShapeEditController(KMapView* a_pMapView) :
m_pMapView(a_pMapView), m_nxEditTransitID(0)
{
    TxLogDebugVisitor();

    m_emShapeEditingTarget = KEMShapeEditingLayerNode;
    m_emShapeEditingTask   = KEMShapeEditingTaskInsert;
}

KShapeEditController::~KShapeEditController()
{
    //StopMapEditing();
}

void KShapeEditController::StartMapEditing()
{  
    RefreshTargetTask();
	
    KCommandWnd*    pCommandWindow = m_pMapView->GetCommandView();
    if (pCommandWindow)
	{
		pCommandWindow->CommandLogMsg(_T("Start Editing"));
		pCommandWindow->SetDefaultCommand();
	}	
}

void KShapeEditController::RefreshTargetTask()
{
    if (m_spShapeEdit != NULL)
    {
        m_spShapeEdit->StopEditing();
        m_spShapeEdit.reset();
    }

    // 노드레이어 편집
    if (m_emShapeEditingTarget == KEMShapeEditingLayerNode)
    {
        // 추가 모드
        if (m_emShapeEditingTask == KEMShapeEditingTaskInsert)
        {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditNodeInsert(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }
        // 수정 모드
        if (m_emShapeEditingTask == KEMShapeEditingTaskSingleEdit)
        {
            m_spShapeEdit = KShapeEditPtr(new /*KShapeEditNodeEdit*/KShapeEditNodeProperties(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }

		if (m_emShapeEditingTask == KEMShapeEditingTaskDelete)
		{
			m_spShapeEdit = KShapeEditPtr(new KShapeEditNodeDelete(m_pMapView, this));
			m_spShapeEdit->StartEditing();
			return;
		}

        if (m_emShapeEditingTask == KEMShapeEditingTaskMove)
        {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditNodeMove(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }
    }

    if (m_emShapeEditingTarget == KEMShapeEditingLayerLink)
    {
		if (m_emShapeEditingTask == KEMShapeEditingTaskSingleEdit)
		{
			m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkEdit(m_pMapView, this, 1));
			m_spShapeEdit->StartEditing();
			return;
		}

        if (m_emShapeEditingTask == KEMShapeEditingTaskDelete) {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkDelete(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }

        if (m_emShapeEditingTask == KEMShapeEditingTaskSingleDel) {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkEdit(m_pMapView, this, 0));
            m_spShapeEdit->StartEditing();
            return;
        }

        if (m_emShapeEditingTask == KEMShapeEditingTaskInsert) {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkInsert(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }

		if (m_emShapeEditingTask == KEMShapeEditingTaskLinkOneWayInsert) {
			m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkInsert(m_pMapView, this, false));
			m_spShapeEdit->StartEditing();
			return;
		}

		if (m_emShapeEditingTask == KEMShapeEditingTaskLinkTwoWayInsert) {
			m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkInsert(m_pMapView, this));
			m_spShapeEdit->StartEditing();
			return;
		}

        if (m_emShapeEditingTask == KEMShapeEditingTaskLinkOneWayVertex) {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkOneWayVertex(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }

        if (m_emShapeEditingTask == KEMShapeEditingTaskLinkTwoWayVertex) {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditLinkTwoWayVertex(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }
    }

    if (m_emShapeEditingTarget == KEMShapeEditingLayerDemarcation)
    {
        if (m_emShapeEditingTask == KEMShapeEditingTaskMultiSelect)
        {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditDemarcationVertex(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }
    }

    if (m_emShapeEditingTarget == KEMShapeEditingLayerTurn)
    {
        if (m_emShapeEditingTask == KEMShapeEditingTaskInsert)
        {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditTurnInsert(m_pMapView, this));
            m_spShapeEdit->StartEditing();
            return;
        }
		else
		{
			m_spShapeEdit = KShapeEditPtr(new KShapeEditTurnDeletes(m_pMapView, this));
			m_spShapeEdit->StartEditing();
			return;
		}
    }	

	if (m_emShapeEditingTarget == KEMShapeEditingLayerTransit)
	{
        if (m_emShapeEditingTask == KEMShapeEditingTaskInsert)
        {
		    m_spShapeEdit = KShapeEditPtr(new KShapeEditTransitInsert(m_pMapView, this));
		    m_spShapeEdit->SetEditID(0);
		    m_spShapeEdit->StartEditing();
        }
        else
        {
            m_spShapeEdit = KShapeEditPtr(new KShapeEditTransitEdit(m_pMapView, this));
            m_spShapeEdit->SetEditID(m_nxEditTransitID);
            m_spShapeEdit->StartEditing();
        }
	}
}


void KShapeEditController::SetEditingTarget( KEMShapeEditingTarget a_emShapeEditingLayer )
{
    m_emShapeEditingTarget = a_emShapeEditingLayer;
    //RefreshTargetTask();
}

void KShapeEditController::SetEditingTask( KEMShapeEditingTask a_emShapeEditingTask )
{
    m_emShapeEditingTask  = a_emShapeEditingTask;
    RefreshTargetTask();
}

void KShapeEditController::StartOperation( void )
{
    
}

void KShapeEditController::AbortOperation()
{
    try
    {
        KCommandWnd*  pCommandWindow = m_pMapView->GetCommandView();
		if (pCommandWindow)
			pCommandWindow->CommandLogMsg(_T("Abort Operation...."));
    }
    catch( ... )
    {
        TxLogDebugWarning();
    }
}


void KShapeEditController::StopMapEditing()
{
    try {
        m_pMapView->MapForegroundRefresh();
    }
    catch(...) {

    }    
}

void KShapeEditController::Draw()
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus()) {
        m_spShapeEdit->Draw();
    }
}

void KShapeEditController::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_dDownMapX = mapX;
    m_dDownMapY = mapY;

    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapLButtonDown(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditController::OnMapLButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapLButtonUp(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditController::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapLButtonDblClk(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditController::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapMouseMove(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditController::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapRButtonDown(nFlags, X, Y, mapX, mapY);
    }
}

void KShapeEditController::OnMapRButtonUp( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spShapeEdit == NULL)
        return;

    if (m_spShapeEdit->IsEditingStatus())
    {
        m_spShapeEdit->OnMapRButtonUp(nFlags, X, Y, mapX, mapY);
    }
}


void KShapeEditController::Sleep()
{
	if (m_spShapeEdit == NULL)
		return;

	if (m_spShapeEdit->IsEditingStatus())
	{
		m_spShapeEdit->Sleep();
	}
}

WORD KShapeEditController::GetCursorID()
{
	if (m_spShapeEdit == NULL)
		return 0;

	return m_spShapeEdit->GetCursorID();
}

void KShapeEditController::OnShapeEditPopupCommand(UINT nID)
{
	if (m_spShapeEdit != NULL)
		m_spShapeEdit->OnShapeEditPopupCommand(nID);
}

bool KShapeEditController::GetShapeEditPopupInfo(UINT& nMenuID, int& nPos)
{
	if (m_spShapeEdit == NULL)
		return false;

	return m_spShapeEdit->GetShapeEditPopupInfo(nMenuID, nPos);
}

double KShapeEditController::GetPixelToRealDist(double a_dPixelCount)
{
    return m_pMapView->GetPixelToRealDist(a_dPixelCount);
}

void KShapeEditController::SetDefaultNodeInfo( std::map<CString, CString>& ar_mapKeyValue )
{
	m_mapDefaultNodeInfo.clear();
	m_mapDefaultNodeInfo.insert(ar_mapKeyValue.begin(), ar_mapKeyValue.end());
}

void KShapeEditController::SetDefaultLinkInfo( std::map<CString, CString>& ar_mapKeyValue )
{
	m_mapDefaultLinkInfo.clear();
	m_mapDefaultLinkInfo.insert(ar_mapKeyValue.begin(), ar_mapKeyValue.end());
}

void KShapeEditController::SetDefaultZoneInfo( std::map<CString, CString>& ar_mapKeyValue )
{
	m_mapDefaultZoneInfo.clear();
	m_mapDefaultZoneInfo.insert(ar_mapKeyValue.begin(), ar_mapKeyValue.end());
}

void KShapeEditController::SetDefaultTurnInfo( std::map<CString, CString>& ar_mapKeyValue )
{
	m_mapDefaultTurnInfo.clear();
	m_mapDefaultTurnInfo.insert(ar_mapKeyValue.begin(), ar_mapKeyValue.end());
}

void KShapeEditController::SetDefaultTransitInfo( std::map<CString, CString>& ar_mapKeyValue )
{
    m_mapDefaultTransitInfo.clear();
    m_mapDefaultTransitInfo.insert(ar_mapKeyValue.begin(), ar_mapKeyValue.end());
}

CString KShapeEditController::GetDefaultNodeValue( CString a_strKey )
{
	CString strReturnValue;
	std::map<CString, CString>::iterator iter = m_mapDefaultNodeInfo.find(a_strKey);
	std::map<CString, CString>::iterator end  = m_mapDefaultNodeInfo.end();

	if (iter != end)
		strReturnValue = iter->second;

	return strReturnValue;	
}

CString KShapeEditController::GetDefaultLinkValue( CString a_strKey )
{
	CString strReturnValue;
	std::map<CString, CString>::iterator iter = m_mapDefaultLinkInfo.find(a_strKey);
	std::map<CString, CString>::iterator end  = m_mapDefaultLinkInfo.end();

	if (iter != end)
		strReturnValue = iter->second;

	return strReturnValue;	
}

CString KShapeEditController::GetDefaultZoneValue( CString a_strKey )
{
	CString strReturnValue;
	std::map<CString, CString>::iterator iter = m_mapDefaultZoneInfo.find(a_strKey);
	std::map<CString, CString>::iterator end  = m_mapDefaultZoneInfo.end();

	if (iter != end)
		strReturnValue = iter->second;

	return strReturnValue;	
}

CString KShapeEditController::GetDefaultTurnValue( CString a_strKey )
{
	CString strReturnValue;
	std::map<CString, CString>::iterator iter = m_mapDefaultTurnInfo.find(a_strKey);
	std::map<CString, CString>::iterator end  = m_mapDefaultTurnInfo.end();

	if (iter != end)
		strReturnValue = iter->second;

	return strReturnValue;	
}

CString KShapeEditController::GetDefaultTransitValue( CString a_strKey )
{
    CString strReturnValue;
    std::map<CString, CString>::iterator iter = m_mapDefaultTransitInfo.find(a_strKey);
    std::map<CString, CString>::iterator end  = m_mapDefaultTransitInfo.end();

    if (iter != end)
        strReturnValue = iter->second;

    return strReturnValue;	
}
