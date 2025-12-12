#pragma once

class KMapConfig
{
protected:
    KMapConfig() {}
    ~KMapConfig(){}

public:
    static double Offset() {
        return 0.6;
    }
};

class KLayerID 
{
public:
    KLayerID() {}
    ~KLayerID(){}

public:
    //★ 숫자가 작을 수록 먼저 그린다.
    static int Zone()                         { return 11;   }  //★ 
    static int Link()                         { return 100;   } //★        

    static int KThemeDrawLink()               { return 1203; }    
    static int Node()                         { return 1500; } //★ 
    static int KThemeDrawLink_Union()         { return 1505; }
    static int KThemeDrawUserLine_Union()     { return 1506; }        
    static int Turn()                         { return 2000; } //★ 
    static int KScreenDrawUserPoint()         { return 2011; }
    static int KThemeDrawTransit()            { return 2020; }
    static int KThemeTransitPathInfo()        { return 2023; }
    static int KEditForeDrawTransitPath()     { return 2024; }
    static int KThemeDrawNode()               { return 2025; }
    static int KThemeLayerDrawNode_Union()    { return 2026; }
    static int KThemeDrawStation_Union()      { return 2030; }
    static int KScreenDrawScreenLine()        { return 2205; }
    static int KScreenDrawCordonLine()        { return 2206; }    
    static int KThemDrawLinkViaPathVolume()   { return 2208; }
    static int KThemDrawZoneToZonePathVolume(){ return 2209; }
    static int KThemeDrawZoneAccessBoundary() { return 2210; }
    static int KThemDrawIntersection()        { return 2211; }
    static int KThemeDrawTransitShortestPath(){ return 2212; }  
    static int KThemeDrawHighwayShortestPath(){ return 2213; }

    static int KScreenDrawLinkSelection()     { return 3001; }
    static int KThemeDrawNodeSelection()      { return 3002; }    

public:
    //★ 기존 Tas의 드로잉 방식이 아닌 새로운 방법으로 그리는 레이어
    //★ 즉, MapView에서 중앙에서 컨트롤 하지 않고.. 각 기능에서 컨트롤 한다.

    //★ 인터모달 - 타겟 리전 편집
    static int LayerID_TagetRegionRail()     { return 4000;}
    static int LayerID_TagetRegionHighway()  { return 4001;}
    static int LayerID_TagetRegionStnRail()  { return 4002;}
    static int LayerID_TagetRegionStnHigh()  { return 4003;}

    //★ Selected Link Analysis(선택 정보)
    static int LayerID_SelectedLinkAnalysis()  { return 4010;}

    //★ 1000번 대
    static int LayerID_TransitInfluenceArea() { return 15;}


//★ Editing은 최상단에 적용
public:
    static int LayerID_LinkVertexEdit() {return 10000001;}
};