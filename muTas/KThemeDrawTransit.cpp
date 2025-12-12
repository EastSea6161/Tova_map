#include "StdAfx.h"
#include "Project.h"
#include "KThemeDrawTransit.h"
#include "BulkDBaseTransit.h"

KThemeDrawTransit::KThemeDrawTransit(IMapView* pMapView) : IThemeLayer(pMapView)
{  
    //★ 링크레이어-> Geometry 정보 조회
    {
        ITxFeatureLayerPtr spLayer = m_pIMapView->MapGetFeatureLayer(KLayerID::Link());
        if (spLayer == nullptr)
            return;

        spLayer->GetGeometry(m_mapLinkGeo);        
    }

    m_nLayerID = KLayerID::KThemeDrawTransit();
    m_pIMapView->AddUserLayer(m_nLayerID, true);
}

void KThemeDrawTransit::Initialize()
{    
}

KThemeDrawTransit::~KThemeDrawTransit(void)
{//ThemeLayerDrawTransit
    m_pIMapView->MapRemoveLayer(m_nLayerID);
}

void KThemeDrawTransit::Create()
{
    ITxFeatureUserLayerPtr spUserLayer = m_pIMapView->MapGetFeatureUserLayer(m_nLayerID);
    if (spUserLayer == nullptr)
        return;

    spUserLayer->Delete();
    if (m_mapLinkGeo.size() == 0)
        return;

    __int64 TxID(1000);
    std::vector<TxFeatureDispPtr> vecLineFeature;
    
    std::map<double, TxOffsetInfoPtr> mapOffsetPtr;
    TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(0.0, 200000));

    for (auto iter = m_mapLinkTransitInfo.begin(); iter != m_mapLinkTransitInfo.end(); ++iter) {
        __int64 nxLinkID = iter->first;
        ITxGeometryPtr spLink = nullptr; {
            auto iFind  = m_mapLinkGeo.find(nxLinkID);
            if ( iFind != m_mapLinkGeo.end()) {
                spLink = iFind->second;
            }
        }

        if (spLink == nullptr)
            continue;

        std::vector<TSimStyle> vecSimStyle;
        std::vector<TDrawTransitLinkInfo>& vecLink = iter->second; 
        for (size_t i = 0; i < vecLink.size(); i++) {
            TDrawTransitLinkInfo& oInfo = vecLink[i];

            Gdiplus::Color  clrLine;

            double dIdWidth(0.5);
            double dEmpty(0.0);
            GetTransitRouteStyle(oInfo.TTransitID, OUT clrLine, dIdWidth, dEmpty);

            TSimStyle oTSimStyle(clrLine, dIdWidth);
            vecSimStyle.push_back(oTSimStyle);
        }

        std::sort(vecSimStyle.begin(), vecSimStyle.end(), SortColorDesc);

        double dWidth(2);
        for (size_t i=0; i<vecSimStyle.size(); i++) {
            TSimStyle& oInfo = vecSimStyle[i];

            Gdiplus::Color &clrLine  = oInfo.TRGBColor;
            double         &dIdWidth = oInfo.dWidth;
            
            TxID++;
            {
                if (i <= 100) {
                    //dWidth = 2.0 + 0.5/*2*/ * i;
                    
                    if (i == 0) {
                        if (dIdWidth > 2.0) {
                            dWidth = dIdWidth;
                        }
                        else {
                            dWidth = 2.0;
                        }
                    }
                    else {
                        dWidth += dIdWidth;
                    }
                }
                else {
                    continue; //★ 무의미하다 : 그리지 않는다.
                    /*dWidth = dWidth + 0.5 * 50;*/
                }

                TxSymbolLinePtr spSymbol(new TxSymbolLine(dWidth, clrLine)); {
                    spSymbol->SetOffset(spOffset);
                }

                TxFeatureDispPtr spFeatureDisp = TxFeatureDispPtr(new TxFeatureDisp(TxID, spLink)); {
                    spFeatureDisp->Symbol(spSymbol);
                }

                vecLineFeature.push_back(spFeatureDisp);
            }
        }
    }

    //spUserLayer->DrawOverlapFilter(false);
    spUserLayer->PutFeatureDisp(vecLineFeature);
}

KEMThemeLayer KThemeDrawTransit::GetType()
{
    return ThemeLayerDrawTransit;
}

void KThemeDrawTransit::UpdateTransitDraw( KTarget* a_pTarget, TTransitInfo a_TransitInfo )
{
    Integer nxTransitID  = a_TransitInfo.TTransitID;

    std::map<Integer, TTransitInfo>::iterator iterTransit = m_mapDrawTransit.begin();
    std::map<Integer, TTransitInfo>::iterator endTransit  = m_mapDrawTransit.end();

    // 이미 검색된 경우 체크
    iterTransit     =  m_mapDrawTransit.find(nxTransitID);
    if (iterTransit != endTransit) {
        RemoveTransitDraw(a_TransitInfo.TTransitID);
    }

    AddTransitDraw(a_pTarget, a_TransitInfo);
}


void KThemeDrawTransit::AddTransitDraw( KTarget* a_pTarget, TTransitInfo a_TransitInfo )
{
    Integer nxTransitID  = a_TransitInfo.TTransitID;

    // 이미 검색된 경우 체크
    auto iFind =  m_mapDrawTransit.find(nxTransitID);
    if (iFind != m_mapDrawTransit.end()) {
        return;
    } else {
        m_mapDrawTransit.insert(std::make_pair(nxTransitID, a_TransitInfo));
    }
    
    // 노선을 경유하는 링크 정보 조회
    std::vector<TDrawTransitLinkInfo> vecTransitLinkInfo; {
        KBulkDBaseTransit::TransitLinkPathQuery(a_pTarget, nxTransitID, vecTransitLinkInfo);
    }
    
    auto iter = m_mapLinkTransitInfo.begin();
    auto end  = m_mapLinkTransitInfo.end();

    // 노선을 경유하는 링크 정보 -> 링크를 기준으로 하는 std::map 생성
    for (size_t i=0; i<vecTransitLinkInfo.size(); i++) {
        // 노선 경유 링크 정보
        TDrawTransitLinkInfo& oDrawTransitInfo = vecTransitLinkInfo[i];
        Integer               nxLinkID         = oDrawTransitInfo.TLinkID;

        // 링크 기준 std::map 검색
        iter = m_mapLinkTransitInfo.find(nxLinkID);
        if (iter != end) {// 기존에 존재
            std::vector<TDrawTransitLinkInfo>& vecDrawTransitInfo = iter->second;
            vecDrawTransitInfo.push_back(oDrawTransitInfo);
        }
        else {// 신규 생성
            std::vector<TDrawTransitLinkInfo> vecDrawTransitInfo;
            vecDrawTransitInfo.push_back(oDrawTransitInfo);
            m_mapLinkTransitInfo.insert(std::make_pair(nxLinkID, vecDrawTransitInfo));
        }
    }   
}

void KThemeDrawTransit::RemoveTransitDraw( Integer a_nxTransitID )
{   
    std::map<Integer, TTransitInfo>::iterator iterTransit = m_mapDrawTransit.find(a_nxTransitID);
    std::map<Integer, TTransitInfo>::iterator endTransit  = m_mapDrawTransit.end();

    if (iterTransit != endTransit) {
        m_mapDrawTransit.erase(iterTransit);
    }

    std::map<Integer, std::vector<TDrawTransitLinkInfo>>::iterator iterLink = m_mapLinkTransitInfo.begin();
    std::map<Integer, std::vector<TDrawTransitLinkInfo>>::iterator endLink  = m_mapLinkTransitInfo.end();

    while (iterLink != m_mapLinkTransitInfo.end()) {
        std::vector<TDrawTransitLinkInfo>& vecDrawTransitLinkInfo = iterLink->second;

        std::vector<TDrawTransitLinkInfo>::iterator iter = vecDrawTransitLinkInfo.begin();
               
        while (iter != vecDrawTransitLinkInfo.end())
        {
            TDrawTransitLinkInfo& oDrawTransitLinkInfo = *iter;
            bool bErasedTransitLinkInfo = false;
            if (oDrawTransitLinkInfo.TTransitID == a_nxTransitID) {
                iter = vecDrawTransitLinkInfo.erase(iter);
                continue;
            }

            ++iter;                
        }
        
        ++iterLink;                        
    }

    iterLink = m_mapLinkTransitInfo.begin();
    while (iterLink != m_mapLinkTransitInfo.end())
    {
        std::vector<TDrawTransitLinkInfo>& vecDrawTransitLinkInfo = iterLink->second;
        if (vecDrawTransitLinkInfo.size() < 1) {
            iterLink = m_mapLinkTransitInfo.erase(iterLink);
            continue;
        }
        ++iterLink;
    }
}

void KThemeDrawTransit::RemoveAllTransitDraw()
{
    m_mapDrawTransit.clear();
    m_mapLinkTransitInfo.clear();

    Create();
}

Gdiplus::Color KThemeDrawTransit::GetTransitRouteColor( Integer a_nxTransitID )
{
    COLORREF rgbColor = RGB(158, 190, 234);
    std::map<Integer, TTransitInfo>::iterator iter = m_mapDrawTransit.begin();
    std::map<Integer, TTransitInfo>::iterator end  = m_mapDrawTransit.end();

    iter = m_mapDrawTransit.find(a_nxTransitID);
    if (iter != end) {                    
       rgbColor = iter->second.TRGBColor;
    }         

    Gdiplus::Color clrLine; {
        clrLine.SetFromCOLORREF(rgbColor);
    }

    return clrLine;
}

void KThemeDrawTransit::GetTransitRouteStyle(Integer a_nxTransitID, OUT Gdiplus::Color &a_clrLine, double &a_dWidth, double &a_dOffset)
{
    COLORREF rgbColor = RGB(158, 190, 234);
    a_dWidth  = 0.5;
    a_dOffset = 0.0;

    std::map<Integer, TTransitInfo>::iterator iter = m_mapDrawTransit.begin();
    std::map<Integer, TTransitInfo>::iterator end = m_mapDrawTransit.end();

    iter = m_mapDrawTransit.find(a_nxTransitID);
    if (iter != end) {
        TTransitInfo &oTTransitInfo = iter->second;
        rgbColor  = oTTransitInfo.TRGBColor;
        a_dWidth  = oTTransitInfo.dWidth;
        a_dOffset = oTTransitInfo.dOffset;
    }

    a_clrLine.SetFromCOLORREF(rgbColor);
}
