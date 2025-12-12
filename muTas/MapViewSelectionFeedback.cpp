#include "stdafx.h"
#include "Target.h"
#include "MapViewSelectionFeedback.h"
#include "ImChampFrameWindow.h"
#include "IOTransitTableView.h"

KMapViewSelectionFeedback::KMapViewSelectionFeedback(KMapView* a_pMapView) 
    : m_pMapView(a_pMapView), m_emFeedbackType(MapViewFeedbackTypeNone)
{

}

KMapViewSelectionFeedback::~KMapViewSelectionFeedback()
{
    TxLogDebugVisitor();
}

void KMapViewSelectionFeedback::MapSelectionFeedback( Integer a_nxID)
{
    switch(m_emFeedbackType)
    {
    case MapViewFeedbackTypeLinkSelect:
        break;
    case MapViewFeedbackTypeTransitLinkSelect:
        TransitLinkSelect(a_nxID);
        break;
    case MapViewFeedbackTypeTransitNodeSelect:
        TransitNodeSelect(a_nxID);
        break;
    case MapViewFeedbackTypeNone:
    default:
        return;
    }
}

void KMapViewSelectionFeedback::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
    switch(m_emFeedbackType)
    {
    case MapViewFeedbackTypeNodeSelect:
        NodeSelect(a_vecSelection);
        break;
    case MapViewFeedbackTypeLinkSelect:
        LinkSelect(a_vecSelection);
        break;
    case MapViewFeedbackTypeTransitLinkSelect:
        break;
    case MapViewFeedbackTypeTransitNodeSelect:
        break;
    case MapViewFeedbackTypeNone:
    default:
        return;
    }
}


void KMapViewSelectionFeedback::MapSelectionNoneFeedback()
{
}

void KMapViewSelectionFeedback::TransitLinkSelect( Integer a_nxLinkID )
{
    try
    {
        m_pMapView->TransitLinkSelect(a_nxLinkID);
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

void KMapViewSelectionFeedback::TransitNodeSelect( Integer a_nxNodeID )
{
    try
    {
        m_pMapView->TransitNodeSelect(a_nxNodeID);
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
void KMapViewSelectionFeedback::LinkSelect( std::vector<Integer> a_vecLinkID )
{
    try
    {
        size_t nxCount = a_vecLinkID.size();

        if (nxCount == 0) {
            AfxMessageBox(_T("링크 검색 결과가 없습니다."));
            return;
        }

        KTarget*      pTarget    = m_pMapView->GetTarget();
        KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(pTarget, TABLE_LINK);
        if (pTableView == nullptr)
            return;

        CString strFilter(_T(""));
        strFilter.Append(_T(" link_id in ( "));
                
        for(size_t i=0; i<nxCount; i++) {
            if (i == nxCount -1)
            {
                strFilter.AppendFormat(_T(" '%I64d' "), a_vecLinkID[i]);
            }
            else
            {
                strFilter.AppendFormat(_T(" '%I64d', "), a_vecLinkID[i]);
            }            
        }
        strFilter.Append(_T(" ) "));

        pTableView->SetFilter(strFilter);
        pTableView->ReloadData();

        m_pMapView->AddSelectionLink(a_vecLinkID, false);
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

void KMapViewSelectionFeedback::NodeSelect( std::vector<Integer> a_vecNodeID )
{
    try
    {
        size_t nxCount = a_vecNodeID.size();

        if (nxCount == 0) {
            AfxMessageBox(_T("노드 검색 결과가 없습니다."));
            return;
        }

        KTarget*      pTarget    = m_pMapView->GetTarget();
        KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(pTarget, TABLE_NODE);
        if (pTableView == nullptr)
            return;
        
        CString strFilter(_T(""));
        strFilter.Append(_T(" node_id in ( "));

        for(size_t i=0; i<nxCount; i++)
        {
            if (i == nxCount -1)
            {
                strFilter.AppendFormat(_T(" '%I64d' "), a_vecNodeID[i]);
            }
            else
            {
                strFilter.AppendFormat(_T(" '%I64d', "), a_vecNodeID[i]);
            }            
        }
        strFilter.Append(_T(" ) "));

        pTableView->SetFilter(strFilter);
        pTableView->ReloadData();

        m_pMapView->AddSelectionNode(a_vecNodeID, false);
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
