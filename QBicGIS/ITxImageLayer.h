#pragma once

#pragma once
#include <bitset>

#include "ITxObject.h"
#include "TxLayerInfo.h"
#include "ITxGeometry.h"
#include "ITxMapServer.h"
#include "ITxSymbol.h"
#include "TxFeature.h"
#include "ITxLayer.h"
#include "TxGdiplusBitmap.h"
#include "ThunderMapDisp.h"
/**
*@brief 이미지 레이어
*@details 이미지 레이어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxImageLayer : public ITxLayer, public std::enable_shared_from_this<ITxImageLayer>
{
protected:
    int  m_nCoordType; // 0:카텍, 1:UTM-K, 2:UTMK-Mid(중부)
    bool m_bGray;
public:
    bool IsGrayScale() {
        return m_bGray;
    }
    void SetGrayScale(bool bGray) {
        m_bGray = bGray;
    }
public:
    ITxImageLayer (TxLayerInfoPtr spLayerInfo, int nCoordType=0, bool bGray=false) : ITxLayer(spLayerInfo), m_nCoordType(nCoordType), m_bGray(bGray) {}
    virtual ~ITxImageLayer() {};

protected:
    virtual void Clear() = 0;
public:
    virtual TxGdiplusBitmapPtr GetImage(ThunderMapDispPtr spThunderMapDisp) = 0;
public:
    virtual bool IsFeatureLayer() {return false;}
};

typedef std::shared_ptr<ITxImageLayer> ITxImageLayerPtr;