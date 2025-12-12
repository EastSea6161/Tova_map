#include "StdAfx.h"
#include "ScreenDrawEditMultiLinkSelect.h"
#include "TDrawLinkInfo.h"


KScreenDrawEditMultiLinkSelect::KScreenDrawEditMultiLinkSelect( IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver/*=nullptr*/ )
                               :IScreenDrawEdit(a_mapRequest), m_pMapSelectionReceiver(a_pMapSelectionReceiver), m_emStatus(TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode)
{
    a_mapRequest->SetMapMode(KEMMapModeScreenDrawEdit);
}

KScreenDrawEditMultiLinkSelect::~KScreenDrawEditMultiLinkSelect()
{
    try
    {
        if (m_spScreenDrawFlashLink != nullptr)
            m_spScreenDrawFlashLink.reset();
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

void KScreenDrawEditMultiLinkSelect::CommandFeedback( CString a_strResult )
{
    try
    {
        if (a_strResult.CompareNoCase(_T("C")) == 0)
        {
            StopEditing();
            return;
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

void KScreenDrawEditMultiLinkSelect::Canceled()
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

ThemeLayerDrawEdit KScreenDrawEditMultiLinkSelect::GetType()
{
    return KEMMapLinkPathSelect;
}


void KScreenDrawEditMultiLinkSelect::Draw()
{
    try
    {
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
        else
        {// Nothing
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

void KScreenDrawEditMultiLinkSelect::OnMapLButtonDown( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    try
    {
        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
        {
            if (nullptr != m_spScreenDrawFlashLink) {
                m_spScreenDrawFlashLink->MouseDown(nFlags, X, Y, mapX, mapY);
            }
        }
        else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus) {
            if (nullptr != m_spScreenDrawFlashLinkPath) {
                m_spScreenDrawFlashLinkPath->MouseDown(nFlags, X, Y, mapX, mapY);
            }
        }
        else { // Nothing
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

void KScreenDrawEditMultiLinkSelect::OnMapLButtonDblClk( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    try
    {
        StopEditing();
        m_pMapSelectionReceiver->MapSelectionFeedback(-1);
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

void KScreenDrawEditMultiLinkSelect::OnMapMouseMove( UINT nFlags, long X, long Y, double mapX, double mapY )
{
    if (nFlags & MK_MBUTTON) {
        return;
    }

    try
    {
        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus) {
            if (nullptr != m_spScreenDrawFlashLink)
            {
                m_spScreenDrawFlashLink->MouseMove(nFlags, X, Y, mapX, mapY);
            }
        }
        else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus) {
            if (nullptr != m_spScreenDrawFlashLinkPath)
            {
                m_spScreenDrawFlashLinkPath->MouseMove(nFlags, X, Y, mapX, mapY);
            }
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

void KScreenDrawEditMultiLinkSelect::StartEditing()
{
    try
    {
        m_bScreenDrawEditing    = true;
        m_emStatus = TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode;
        m_vecSelectedLink.clear();

        m_spScreenDrawComposite = KScreenDrawCompositePtr(new KThemeLayerUnion(m_pIMapView));
        m_spScreenDrawFlashLink = KScreenDrawFlashLinkPtr(new KScreenDrawFlashLink(m_pIMapView, this));

        if (m_spScreenDrawFlashLinkPath != nullptr)
        {
            m_spScreenDrawFlashLinkPath.reset();
        }
        
        LinkSelectCommandMsg();

        m_pIMapView->SetMapCursor(IDC_EditingEditTool);
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

void KScreenDrawEditMultiLinkSelect::LinkSelectCommandMsg()
{
    try
    {
        KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
        ImChampModalCommandPtr spCommand(new ImChampCommand(_T("Select Link:"), this));
        if (pCommandWindow)
            pCommandWindow->Command(spCommand, KEMModeless);
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

void KScreenDrawEditMultiLinkSelect::StopEditing()
{
    try
    {
        m_vecSelectedLink.clear();

        if (m_spScreenDrawComposite != nullptr)
            m_spScreenDrawComposite.reset();

        if (m_spScreenDrawFlashLink != nullptr)
            m_spScreenDrawFlashLink.reset();

        if (m_spScreenDrawFlashLinkPath != nullptr)
            m_spScreenDrawFlashLinkPath.reset();

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

void KScreenDrawEditMultiLinkSelect::MapSelectionFeedback( Integer a_nxID )
{
    try
    {
        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
        {
            m_emStatus = TUserLinkPathSelectMode::KEMUserPathSelectMode;
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, a_nxID));

            //선택된 링크 저장 및 Draw
            // 정보 저장 및 Draw
            std::vector<Integer> vecSelection;
            vecSelection.push_back(a_nxID);

            AddDrawCompositeLink(vecSelection);

            if (m_pMapSelectionReceiver != nullptr)
            {
                m_pMapSelectionReceiver->MapSelectionFeedback(m_vecSelectedLink);
            }
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


void KScreenDrawEditMultiLinkSelect::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
    try
    {
        size_t nxCount = a_vecSelection.size();
        if (nxCount == 0)
            return;

        if (TUserLinkPathSelectMode::KEMUserFirstLinkSelectMode == m_emStatus)
        {
        }
        else if(TUserLinkPathSelectMode::KEMUserPathSelectMode == m_emStatus)
        {
            Integer nxID = a_vecSelection[nxCount-1];
            m_spScreenDrawFlashLinkPath = KScreenDrawFlashLinkPathPtr(new KScreenDrawFlashLinkPath(m_pIMapView, this, nxID));

            // 정보 저장 및 Draw
            AddDrawCompositeLink(a_vecSelection);

            if (m_pMapSelectionReceiver != nullptr)
            {
                m_pMapSelectionReceiver->MapSelectionFeedback(m_vecSelectedLink);
            }
        }
        else
        { // Nothing
        }
    }
    catch (...)
    {
        TxLogDebugException();
    }   
}

void KScreenDrawEditMultiLinkSelect::MapSelectionNoneFeedback()
{
}

void KScreenDrawEditMultiLinkSelect::AddDrawCompositeLink( std::vector<Integer> a_vecSelectedLink )
{   
    try
    {
        size_t nxExistCount = m_vecSelectedLink.size();
        size_t nxCount      = a_vecSelectedLink.size();

        if (nxCount == 0)
            return;

        if (m_spScreenDrawComposite == nullptr)
            return;

        std::vector<TDrawLinkInfo> vecDrawLinkInfo;

        for(size_t i=0; i<nxExistCount; i++)
        {
            Integer nxLinkID = m_vecSelectedLink[i];
            TDrawLinkInfo oDrawLinkInfo;
            oDrawLinkInfo.TLinkID = nxLinkID;
            oDrawLinkInfo.TWidth  = 4.0;

            if (i==0)
            {
                oDrawLinkInfo.TColorRGB = RGB(0, 0, 255);
            }
            else
            {
                oDrawLinkInfo.TColorRGB = RGB(255, 0, 0);
            }

            CString strLabel;
            strLabel.Format(_T("%I64d"), nxLinkID);
            oDrawLinkInfo.TLabel = strLabel;

            vecDrawLinkInfo.push_back(oDrawLinkInfo);
        }

        for(size_t i=0; i<nxCount; i++)
        {
            Integer nxLinkID = a_vecSelectedLink[i];
            TDrawLinkInfo oDrawLinkInfo;
            oDrawLinkInfo.TLinkID = nxLinkID;
            oDrawLinkInfo.TWidth  = 4.0;

            if (nxCount-1 == i)
            {
                oDrawLinkInfo.TColorRGB = RGB(0, 0, 255);
            }
            else
            {
                oDrawLinkInfo.TColorRGB = RGB(255, 0, 0);
            }

            CString strLabel;
            strLabel.Format(_T("%I64d"), nxLinkID);
            oDrawLinkInfo.TLabel = strLabel;

            vecDrawLinkInfo.push_back(oDrawLinkInfo);

            // *****기존 정보 선택 정보 추가*******
            m_vecSelectedLink.push_back(nxLinkID);
        }

        m_spScreenDrawComposite->SetLinkDraw(vecDrawLinkInfo, true, true, true);
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
