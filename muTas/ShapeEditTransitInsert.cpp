#include "stdafx.h"
#include "ShapeEditTransitInsert.h"
#include "TTransitPath.h"
#include "ShapeEditController.h"

KShapeEditTransitInsert::KShapeEditTransitInsert( IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner /*= NULL*/ ):
    IShapeEdit(a_mapRequest, a_pShapeEditController, a_pShapeEditOwner), m_nxEditTransitID(0), m_emStatus(TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode)
{
    m_nMouseFlag = 0;
}

KShapeEditTransitInsert::~KShapeEditTransitInsert( void )
{
    StopEditing();
}

void KShapeEditTransitInsert::CommandFeedback( CString a_strResult )
{
    //AfxMessageBox(a_strResult);
}

void KShapeEditTransitInsert::Canceled()
{
    StartEditing();
}


void KShapeEditTransitInsert::Draw()
{
    if (m_spForeDrawTransitPath != NULL)
        m_pIMapView->MapForegroundRefresh();

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->Draw();
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->Draw();
        }
    }
}

void KShapeEditTransitInsert::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    m_nMouseFlag = nFlags;

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
}

void KShapeEditTransitInsert::OnMapRButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    m_nMouseFlag = nFlags;

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseDown(nFlags, X, Y, mapX, mapY);
        }
    }
}

void KShapeEditTransitInsert::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLink)
        {
            m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
        }
    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
    {
        if (nullptr != m_spScreenDrawFlashLinkPath)
        {
            m_spScreenDrawFlashLinkPath->MouseMove(nFlags, X, Y, mapX, mapY);
        }
    }
}

void KShapeEditTransitInsert::StartEditing()
{
    m_bEditing = true;    
    m_emStatus = TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode;

    try
    {
        CWnd* pWnd = m_pIMapView->GetMapWindow();
        if (m_spTransitPathEditDlg == nullptr)
        {
            m_spTransitPathEditDlg = KTransitPathInsertDlgPtr(new KTransitPathInsertDlg(pWnd));
            m_spTransitPathEditDlg->Create(KTransitPathInsertDlg::IDD, pWnd);
            m_spTransitPathEditDlg->SetOpener(this);
        }
        
        m_spTransitPathEditDlg->SetEditTransit(m_pIMapView->GetTarget(), m_nxEditTransitID);
        m_spTransitPathEditDlg->SetDefaultValue(m_pShapeEditController->GetDefaultTransitValue());
        m_spTransitPathEditDlg->ShowWindow(SW_SHOW);

        m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

        if (m_spScreenDrawFlashLinkPath != nullptr)
        {
            m_spScreenDrawFlashLinkPath.reset();
        }

        // 현재 사용자가 선택한 노선의 Path Draw
        m_spForeDrawTransitPath = KForeDrawSingleTransitPathPtr(new KEditForeDrawTransitPath(m_pIMapView));  
        RedrawUserSelectedPath();

        m_pIMapView->SetMapCursor(IDC_EditingEditTool);
    }
    catch(...)
    {

    }	
}

WORD KShapeEditTransitInsert::GetCursorID()
{
    return IDC_EditingEditTool;
}


void KShapeEditTransitInsert::SetEditID( Integer a_nxID )
{
    m_nxEditTransitID = a_nxID;
}

void KShapeEditTransitInsert::MapSelectionFeedback( Integer a_nxID )
{
    try
    {
        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
        {
            m_emStatus = TUserLinkPathSelectMode::KEMUserPathSelectMode;
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, a_nxID));

            AutoType& vecTransitPath = m_spTransitPathEditDlg->GetTransitPath();
            TTransitPath oTransitPath;
            oTransitPath.TTransitID = 1;
            oTransitPath.TSeq       = 1;
            oTransitPath.TLinkID    = a_nxID;
            oTransitPath.TStationYN = 0;
            vecTransitPath.push_back(oTransitPath);

            TransitPathPopulate();
        }
        else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
        {
        }
        else
        { // Nothing
        }
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

void KShapeEditTransitInsert::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
    try
    {
        size_t nxCount = a_vecSelection.size();
        if (nxCount == 0)
            return;

        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus) {
        }
        else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus) {
            Integer nxID = a_vecSelection[nxCount-1];
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, nxID));

            AutoType& vecTransitPath = m_spTransitPathEditDlg->GetTransitPath();
            size_t nPreSeq = vecTransitPath.size();

            for(size_t i=0; i<nxCount; i++)
            {
                TTransitPath oTransitPath;
                oTransitPath.TTransitID = 1;
                oTransitPath.TSeq    = nPreSeq + i + 1;
                oTransitPath.TLinkID = a_vecSelection[i];

                if (i == nxCount-1 && m_nMouseFlag & MK_LBUTTON) {
                    oTransitPath.TStationYN = 1;
                }
                else {
                    oTransitPath.TStationYN = 0;
                }

                vecTransitPath.push_back(oTransitPath);
            }

            TransitPathPopulate();
        }
        else {
            // Nothing
        }
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KShapeEditTransitInsert::MapSelectionNoneFeedback()
{
    if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus) {

    }
    else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus) {

    }
    else {
        // Nothing
    }
}

void KShapeEditTransitInsert::TransitPathPopulate()
{
    try
    {
        if (!m_spTransitPathEditDlg->IsWindowVisible())
            m_spTransitPathEditDlg->ShowWindow(SW_SHOW);

        m_spTransitPathEditDlg->TransitPathPopulate();
        RedrawUserSelectedPath();
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KShapeEditTransitInsert::RedrawUserSelectedPath()
{
    try
    {
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        if (m_spForeDrawTransitPath) {
            m_spForeDrawTransitPath->SetTransitPath(false, vecTranistPath);
            m_pIMapView->MapForegroundRefresh();
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KShapeEditTransitInsert::TransitPathEditFeedback()
{
    try
    {
        RedrawUserSelectedPath();
        std::vector<TTransitPath>& vecTranistPath = m_spTransitPathEditDlg->GetTransitPath();
        size_t nxCount = vecTranistPath.size();

        if (nxCount == 0)
        {
            m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));
            m_emStatus = TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode;
        }
        else
        {
            Integer nxID = vecTranistPath[nxCount-1].TLinkID;
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, nxID));
            m_emStatus = TUserLinkPathSelectMode::KEMUserPathSelectMode;
        }
    }
    catch (...)
    {
        TxLogDebugException();
    }    
}

void KShapeEditTransitInsert::TransitPathStationEditFeedback()
{
    try
    {
        RedrawUserSelectedPath();
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KShapeEditTransitInsert::SaveEditing()
{
    try
    {    	
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

void KShapeEditTransitInsert::TransitSaveFeedback(__int64 nxID)
{
    // 이벤트 흐름 순서(다이얼로그에서 함수를 호출했기 때문에 다이얼로그를 닫으면 공유 위반
    // m_spTransitPathEditDlg -> TransitSaveFeedback -> m_spTransitPathEditDlg::OnOK

    m_pIMapView->NotifyNetworkEdior(5, nxID);

    try
    {
        StopEditing();
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
    }
    catch (...)
    {
    	TxLogDebugException();
    }
}

void KShapeEditTransitInsert::StopEditing()
{
    m_bEditing = false;

    try
    {
        if (m_spScreenDrawFlashLink)
            m_spScreenDrawFlashLink.reset();

        if (m_spScreenDrawFlashLinkPath)
            m_spScreenDrawFlashLinkPath.reset();

        if (m_spForeDrawTransitPath)
            m_spForeDrawTransitPath.reset();       

        m_pIMapView->SetMapMode(KEMMapModePan);
        m_pIMapView->MapForegroundRefresh();
    }
    catch(...)
    {
    }	
}

void KShapeEditTransitInsert::TransitPathCommand( int nCommand )
{
    try
    {
    	StopEditing();
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
