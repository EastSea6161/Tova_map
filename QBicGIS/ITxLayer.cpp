#include "stdafx.h"
#include "ITxLayer.h"
#include "TxSymbolPoint.h"
#include "TxSymbolLine.h"

ITxLayer::ITxLayer(TxLayerInfoPtr spLayerInfo) : m_spLayerInfo(spLayerInfo)
{
    m_dMinDrawScale = 0.0;
    m_dMaxDrawScale = TxConst::MaxDbl();

    m_bsFlag[EnumFlagDisplay]      = true;
    m_bsFlag[EnumFlagLabelDisplay] = false;
}

ITxLayer::~ITxLayer()
{    
}

void ITxLayer::ClearScaleDisp()
{
    m_dMinDrawScale = 0.0;
    m_dMaxDrawScale = TxConst::MaxDbl();
}

bool ITxLayer::IsScaleDispMode()
{
    //★ 스케일 디스플레이 모드 인지 체크
    if (m_dMinDrawScale != 0 || m_dMaxDrawScale != TxConst::MaxDbl())
        return true;

    return false;
}
