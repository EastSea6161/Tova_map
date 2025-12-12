#pragma once
#include "ITxLayer.h"
#include "ITxImageLayer.h"

/**
*@brief 이미지 레이어
*@details 이미지 레이어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxImageLayer : public ITxImageLayer
{
public:
    TxImageLayer (TxLayerInfoPtr spLayerInfo) : ITxImageLayer(spLayerInfo) {} 
    virtual ~TxImageLayer() {};

protected:
	/**
	*@brief 초기화
	*/
    virtual void Clear() {};
public:
	/**
	*@brief 이미지 추출
	*@param ThunderMapDispPtr spThunderMapDisp
	*@return nullptr 반환
	*/
    virtual TxGdiplusBitmapPtr GetImage(ThunderMapDispPtr spThunderMapDisp) {
        return nullptr;
    }
};