#pragma once
class KTarget;
class KMapView;

#include "TThemeType.h"
#include "TDrawLinkInfo.h"

#include "ThemeSetFilterDlg.h"
#include "ImTasDBase.h"
#include "TDrawNodeInfo.h"

#include "TThemeType.h"
#include "BulkDBaseLink.h"

enum{ NONE_SYMBOL_INDEX = -99};

class ISymbologyThemeLink
{
public:
    ISymbologyThemeLink();
    virtual ~ISymbologyThemeLink();

protected:
    KTarget*         m_pTarget;
    KMapView*        m_pMapView;

protected:
    virtual void LayerOffTrick();
public:
    virtual void SetTargetMap(KTarget* a_pTarget, KMapView* a_pMapView)=0;
    virtual void Draw(bool a_bLabel)  = 0;
    virtual void Clear();

public:
    virtual void NodeLayerSettingChanged() = 0;

protected:
    void DrawNode(bool a_bLabel=false);
    void LoadNodeTypeDefine(std::map<int/*type*/, TNodeTypeDefine> &a_mapTNodeTypeDefine);
    void LoadDefaultNodeTypeDefineID(int &a_nDefineTypeID);
    void LoadDefaultNodeTypeDefine(TNodeTypeDefine &a_oTDefaultNodeTypeDefine);
    void MakeNodeDrawInfo(std::map<int, TNodeTypeDefine> &a_mapTNodeTypeDefine, TNodeTypeDefine &a_oTDefaultNodeTypeDefine, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo);
};