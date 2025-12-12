#pragma once

#include <bitset>

#include "ITxObject.h"
#include "TxLayerInfo.h"
#include "ITxGeometry.h"
#include "TxSpatialIndex.h"
#include "ITxMapServer.h"
#include "ITxSymbol.h"
#include "TxFeature.h"
#include "ITxLabelSymbol.h"
#include "TxLabel.h"
#include "TxFeatureDisp.h"
/**
*@brief 레이어
*@details 레이어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxLayer : public ITxObject
{   
public:
    enum EnumFlag
    {
        EnumFlagDisplay,
        EnumFlagLabelDisplay
    };
public:
    ITxLayer (TxLayerInfoPtr spLayerInfo);    
    virtual ~ITxLayer();

protected: TxLayerInfoPtr m_spLayerInfo;
public:  
	/**
	*@brief 레이어 정보
	*@return 레이어 정보 반환
	*/
	virtual TxLayerInfoPtr LayerInfo() {return m_spLayerInfo;}

protected:
	/**
	*@brief 초기화
	*/
    virtual void Clear() = 0;
protected:
    std::bitset<4> m_bsFlag;
public:
	/**
	*@brief 레이어 설정
	*@param bool bLayerOn	레이어 설정 On
	*/
    virtual void LayerOn   (bool bLayerOn) {
        m_bsFlag[EnumFlagDisplay] = bLayerOn;
    }    
	/**
	*@brief 레이어 설정
	*@return 설정된 레이어값 반환
	*/
    virtual bool LayerOn() {   
        return m_bsFlag[EnumFlagDisplay];
    }
    
private:
    double m_dMinDrawScale;
    double m_dMaxDrawScale;
public:
	/**
	*@brief 스케일 디스플레이 모드
	*/
    void ClearScaleDisp();
	/**
	*@brief 스케일이 디스플레이 모드인지 체크
	*@return 참 true / 거짓 false
	*/
    bool IsScaleDispMode();
	/**
	*@brief 스케일 최소 규모 설정
	*@param double dScale			스케일 규모
	*/
    void SetDrawMinScale(double dScale) {m_dMinDrawScale = dScale;}
	/**
	*@brief 스케일 최대 규모 설정
	*@param double dScale			스케일 규모
	*/
    void SetDrawMaxScale(double dScale) {m_dMaxDrawScale = dScale;}
public:
	/**
	*@brief 최소 스케일 규모 추출
	*@return 최소 스케일 규모
	*/
    double GetDrawMinScale() {return m_dMinDrawScale;}
	/**
	*@brief 최대 스케일 규모 추출
	*@return 최대 스케일 규모
	*/
    double GetDrawMaxScale() {return m_dMaxDrawScale;}
public:
	/**
	*@brief 드로잉 가능 확인
	*@param double dSacale 스케일 
	*@return 참 true / 거짓 false
	*/
    bool IsDraw(double dScale) {
        if (LayerOn() == true) {
            return IsDrawScale(dScale);
        }
        
        return false;
    }
    /**
	*@brief 드로잉 가능한 스케일 확인
	*@param double dSacale 스케일 
	*@return 참 true / 거짓 false
	*/
    bool IsDrawScale(double dScale) {
        if (m_dMinDrawScale <= dScale && dScale <= m_dMaxDrawScale){
            return true;
        }
        return false;
    }
public:
    virtual bool IsFeatureLayer() = 0;
friend class ThunderMapdrawProcess;
friend class ThunderMap;
};


typedef std::shared_ptr<ITxLayer> ITxLayerPtr;