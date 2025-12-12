#include "stdafx.h"
#include "ScreenDrawEditSingleLinkSelect.h"

KScreenDrawEditSingleLinkSelect::KScreenDrawEditSingleLinkSelect(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver)
                                :IScreenDrawEdit(a_mapRequest), m_pMapSelectionReceiver(a_pMapSelectionReceiver)
{
    a_mapRequest->SetMapMode(KEMMapModeScreenDrawEdit);
}

KScreenDrawEditSingleLinkSelect::~KScreenDrawEditSingleLinkSelect()
{
    if (m_spScreenDrawFlashLink != nullptr)
        m_spScreenDrawFlashLink.reset();
}

void KScreenDrawEditSingleLinkSelect::CommandFeedback( CString a_strResult )
{
    if (a_strResult.CompareNoCase(_T("C")) == 0) {
        StopEditing();
        return;
    }
}

void KScreenDrawEditSingleLinkSelect::Canceled()
{
    StartEditing();
}

ThemeLayerDrawEdit KScreenDrawEditSingleLinkSelect::GetType()
{
    return KEMMapLinkPathSelect;
}

void KScreenDrawEditSingleLinkSelect::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
    }
}

void KScreenDrawEditSingleLinkSelect::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (MK_MBUTTON & nFlags)
        return;

    if (m_spScreenDrawFlashLink != nullptr) {
        m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
    }
}

void KScreenDrawEditSingleLinkSelect::StartEditing()
{
    try 
    {
        m_bScreenDrawEditing    = true;
    	m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));
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

void KScreenDrawEditSingleLinkSelect::StopEditing()
{
    try
    {
        if (m_spScreenDrawFlashLink != nullptr)
            m_spScreenDrawFlashLink.reset();

        m_bScreenDrawEditing = false;

        m_pIMapView->SetMapMode(KEMMapModePan);
        KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
        if (pCommandWindow)
            pCommandWindow->SetDefaultCommand();

        m_pIMapView->MapRefresh();
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

void KScreenDrawEditSingleLinkSelect::MapSelectionFeedback( Integer a_nxID)
{
    try
    {
    	if (m_pMapSelectionReceiver)
        {
            m_pMapSelectionReceiver->MapSelectionFeedback(a_nxID);            
        }

        StartEditing();
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

void KScreenDrawEditSingleLinkSelect::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
    try
    {
        if (m_pMapSelectionReceiver)
        {
            m_pMapSelectionReceiver->MapSelectionFeedback(a_vecSelection);
        }

        StartEditing();
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

void KScreenDrawEditSingleLinkSelect::MapSelectionNoneFeedback()
{
    try
    {
        if (m_pMapSelectionReceiver)
        {
            m_pMapSelectionReceiver->MapSelectionNoneFeedback();
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


