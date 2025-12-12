#pragma once

#include "TxFeature.h"
#include "TxLabel.h"
#include "ITxSymbol.h"
/**
*@brief 특성 표출
*@details 특성 표출
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxFeatureDisp
{
public:
    TxFeatureDisp();
    TxFeatureDisp(TxFeaturePtr spFeature);
    TxFeatureDisp(__int64 nxFeatureID, ITxGeometryPtr spGeometry);
    TxFeatureDisp(const TxFeatureDisp& right);
    virtual ~TxFeatureDisp() {}

private:
    TxFeaturePtr m_spFeature;
public:
	/**
	*@brief 특성
	*@param TxFeaturePtr spFeature	특성 종류
	*/
    void Feature(TxFeaturePtr spFeature){
        m_spFeature = spFeature;
    }
	/**
	*@brief 특성 확인
	*@return 특성 확인
	*/
    const TxFeaturePtr Feature() {
        return m_spFeature;
    }
    
private:
    ITxLabelPtr m_spLabel;

public:
	/**
	*@brief 라벨
	*@return 라벨 반환
	*/
    ITxLabelPtr Label() const;
	/**
	*@brief 라벨
	*@param ITxLabelPtr spLabel	라벨 반환
	*/
    void Label(ITxLabelPtr spLabel);

private:
    ITxSymbolPtr m_spSymbol;  // Feature 심볼 정의
public:
	/**
	*@brief 심볼
	*@return 심볼 반환
	*/
    ITxSymbolPtr Symbol() const;
	/**
	*@brief 심볼
	*@param ITxSymbolPtr spTxGeometrySymbol	기하학 심볼
	*/
    void Symbol(ITxSymbolPtr spTxGeometrySymbol);     

    friend class ThunderMapdrawProcess;
};

typedef std::shared_ptr<TxFeatureDisp> TxFeatureDispPtr;