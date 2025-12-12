#include "stdafx.h"
#include "SymbologyThemeLink.h"
#include "Target.h"
#include "MapView.h"
#include "DBaseConnector.h"
#include "ImTasDBase.h"
#include "LayerXmlRenderer.h"
#include "BulkDBaseNode.h"

ISymbologyThemeLink::ISymbologyThemeLink() : m_pTarget(nullptr), m_pMapView(nullptr)
{

}

ISymbologyThemeLink::~ISymbologyThemeLink()
{
    m_pTarget  = nullptr;
    m_pMapView = nullptr;
}

void ISymbologyThemeLink::LayerOffTrick()
{
}

void ISymbologyThemeLink::LoadNodeTypeDefine( std::map<int/*type*/, TNodeTypeDefine> &a_mapTNodeTypeDefine )
{
    try
    {
        a_mapTNodeTypeDefine.clear();

        int nDefineTypeID(-1);
        LoadDefaultNodeTypeDefineID(nDefineTypeID);

        KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

        KProjectDBase::GetNodeTypeDefine(spDBaseConnection, nDefineTypeID, a_mapTNodeTypeDefine);
    }
    catch (...)
    {
        TxLogDebugException();	
    }
}

void ISymbologyThemeLink::LoadDefaultNodeTypeDefineID( int &a_nDefineTypeID )
{
    try
    {
        KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();

        KProjectDBase::GetUsedNodeTypeDefineID(spDBaseConnection, a_nDefineTypeID);
    }
    catch (...)
    {
        TxLogDebugException();	
    }
}

void ISymbologyThemeLink::LoadDefaultNodeTypeDefine( TNodeTypeDefine &a_oTDefaultNodeTypeDefine )
{
    KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection(); //KDBase::GetApplicationDBConnection();
    KProjectDBase::GetDefaultNodeTypeDefine(spDBaseConnection, a_oTDefaultNodeTypeDefine);
}

void ISymbologyThemeLink::MakeNodeDrawInfo( std::map<int, TNodeTypeDefine> &a_mapTNodeTypeDefine, TNodeTypeDefine &a_oTDefaultNodeTypeDefine, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo )
{
    //★ 아마도 이전 버전에서 링크 위에 노드를 그리기 위해서.. 
    /*
    a_vecDrawNodeInfo.clear();

    try
    {
        COLORREF markerColor;
        double   dblSize(3.0);
        long     charIndex(53);
        double   dMinScale, dMaxScale;
        int      nMarkerIndex;
        int      nLabelShow;
        CString  strLabelFields;
        int      nLabelScaleOn;
        double   dLabelOnScale;
        int      nUseTemplate;

        KLayerXmlRenderer oLayerXmlRenderer(m_pTarget);
        oLayerXmlRenderer.LoadNodeProperty(nMarkerIndex, markerColor, dblSize, dMinScale, dMaxScale, 
            nLabelShow, strLabelFields, nLabelScaleOn, dLabelOnScale, nUseTemplate);
        charIndex = nMarkerIndex;

        std::map<Integer, int> mapNodeType;
        KBulkDBaseNode::NodeType(m_pTarget, mapNodeType);

        AutoType iter  = mapNodeType.begin();
        AutoType itEnd = mapNodeType.end();

        while (iter != itEnd)
        {
            Integer nxNodeID  = iter->first;
            int     nNodeType = iter->second;

            TDrawNodeInfo oTDrawNodeInfo;
            oTDrawNodeInfo.TNodeID = nxNodeID;

            CString &strLabel = oTDrawNodeInfo.TLabel;
            strLabel.Format(_T("%I64d"), nxNodeID);

            if (nUseTemplate == 0)
            {
                oTDrawNodeInfo.TSize           = dblSize;
                oTDrawNodeInfo.TCharacterIndex = charIndex;
                oTDrawNodeInfo.TColorRGB       = markerColor;

                a_vecDrawNodeInfo.push_back(oTDrawNodeInfo);
            }
            else
            {
                AutoType itDef = a_mapTNodeTypeDefine.find(nNodeType);
                if(itDef != a_mapTNodeTypeDefine.end())
                {
                    TNodeTypeDefine &oTNodeTypeDefine = itDef->second;

                    oTDrawNodeInfo.TSize           = oTNodeTypeDefine.dSize;
                    oTDrawNodeInfo.TCharacterIndex = oTNodeTypeDefine.nCharacterIndex;
                    oTDrawNodeInfo.TColorRGB       = oTNodeTypeDefine.clrNode;

                    if (oTNodeTypeDefine.bDraw)
                        a_vecDrawNodeInfo.push_back(oTDrawNodeInfo);
                }
                else
                {
                    if (NONE_SYMBOL_INDEX != a_oTDefaultNodeTypeDefine.nCharacterIndex)
                    {
                        oTDrawNodeInfo.TSize           = a_oTDefaultNodeTypeDefine.dSize;
                        oTDrawNodeInfo.TCharacterIndex = a_oTDefaultNodeTypeDefine.nCharacterIndex;
                        oTDrawNodeInfo.TColorRGB       = a_oTDefaultNodeTypeDefine.clrNode;
                    }
                    else
                    {
                        oTDrawNodeInfo.TSize           = 4;
                        oTDrawNodeInfo.TCharacterIndex = 48;
                        oTDrawNodeInfo.TColorRGB       = RGB(217, 217, 217);
                    }

                    a_vecDrawNodeInfo.push_back(oTDrawNodeInfo);
                }
            }			

            ++iter;
        }
    }
    catch (...)
    {
        TxLogDebugException();	
    }
    */
}

void ISymbologyThemeLink::Clear()
{
    if (m_pMapView)
    {
        m_pMapView->ClearLinkThemes(false);
    }
}

void ISymbologyThemeLink::DrawNode( bool a_bLabel/*=false*/ )
{
}
