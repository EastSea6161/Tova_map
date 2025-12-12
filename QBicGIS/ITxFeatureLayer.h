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
#include "ITxLayer.h"
#include "TxTables.h"
#include "TxGdiplusBitmap.h"

class ThunderMapdrawProcess;
/**
*@brief 툴팁
*@details 더 큰 큐모의 툴팁
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxExtendTooltip
{
public:
	ITxExtendTooltip() {
		m_strTooltip = _T("");
	}
	virtual ~ITxExtendTooltip() {}

protected:
	CString m_strTooltip;
public:
	/**
	*@brief 툴팁
	*@return 툴팁명 반환
	*/
	virtual CString Tooltip() {
		return m_strTooltip;
	}

	/**
	*@brief 툴팁
	*@param CString strTooltip 툴팁명 입력
	*/
	virtual void Tooltip(CString strTooltip) {
		m_strTooltip = strTooltip;
	}
};
typedef std::shared_ptr<ITxExtendTooltip> ITxExtendTooltipPtr;
/**
*@brief 규모정도 정보
*@details 규모정도 정보
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxExtendInfo
{
public:
	ITxExtendInfo() {
	}
	virtual ~ITxExtendInfo() {}

public:
	//★ 상속 받은 곳에서 정의
	virtual int ExtendType() = 0;
};
typedef std::shared_ptr<ITxExtendInfo> ITxExtendInfoPtr;

//###################################################################################################################
/**
*@brief 레이어 특성
*@details 레이어 특성
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxFeatureLayer : public ITxLayer, public std::enable_shared_from_this<ITxFeatureLayer>
{
public:
	ITxFeatureLayer (TxLayerInfoPtr spLayerInfo);    
	virtual ~ITxFeatureLayer();

private:
    bool m_bDrawExpired;
protected:
    bool DrawExpired(const TxEnvelope& oDrawMapEnv, const TxEnvelope& oDrawScrEnv);  
public:
    void SetDrawExpire();

protected:
    TxGdiplusBitmapPtr m_spLayerBitmap;
    TxEnvelope         m_oDrawMapEnv;
    TxEnvelope         m_oDrawScrEnv;
protected:
    TxGdiplusBitmapPtr GetLayerImage();
    void SetLayerImage(TxGdiplusBitmapPtr spBitmap, const TxEnvelope& oDrawMapEnv, const TxEnvelope& oDrawScrEnv);

protected:
    bool m_bDrawOverlapFilter;
public:
    bool DrawOverlapFilter() {
        return m_bDrawOverlapFilter;
    }
    void DrawOverlapFilter(bool bFilter) {
        m_bDrawOverlapFilter = bFilter;
    }

protected:
    bool m_bUseSortDraw;
public:
    bool UseSizeSortDraw() {
        return m_bUseSortDraw;
    }
    void UseSizeSortDraw(bool bSorDraw) {
        m_bUseSortDraw = bSorDraw;
    }
protected:
	ITxLabelSymbolPtr m_spLabelSymbol;
public:
	/**
	*@brief 라벨심볼
	*@return 라벨 심볼 반환
	*/
	virtual ITxLabelSymbolPtr LabelSymbol() const {
        return m_spLabelSymbol;
    }
	/**
	*@brief 라벨 심볼
	*@param ITxLabelSymbolPtr spLabelSymbol 라벨 심볼
	*/
	virtual void LabelSymbol(ITxLabelSymbolPtr spLabelSymbol);
public:
	/**
	*@brief 라벨 보이기
	*@param bool bShow 라벨 보이기 유무
	*/
	virtual void ShowLabel(bool bShow);
	/**
	*@brief 라벨 보이기
	*@return 라벨 보이기 여부
	*/
	virtual bool ShowLabel();
protected:
	ITxSymbolPtr m_spLayerSymbol;
public:
	//-------------------------------------------------------
	/**
	*@brief 레이어 심볼 표출
	*@param ITxSymbolPtr spSymbol 해당 레이어심볼
	*/
	virtual void LayerDispSymbol(ITxSymbolPtr spSymbol);
	/**
	*@brief 레이어 심볼 표출
	*@return 레이어 심볼
	*/
	virtual ITxSymbolPtr LayerDispSymbol() {
		return m_spLayerSymbol;
	}
	/**
	*@brief 심볼(동일기능 복사)
	*@param ITxSymbolPtr spSymbol 심볼
	*/
	virtual void Symbol(ITxSymbolPtr spSymbol);
	/**
	*@brief 심볼(동일기능 복사)
	*@return 심볼 반환
	*/
	virtual ITxSymbolPtr Symbol();
protected:
private:
	double m_dLabelMaxScale;
public:
	/**
	*@brief 라벨초기화 표출
	*/
	void ClearLabelScaleDisp();
	/**
	*@brief 라벨 표출 모드 확인
	*@return 참 거짓 반환
	*/
	bool IsScaleLabelDispMode();
public:
	/**
	*@brief 라벨을 최대 규모로 그리기
	*@param double dScale 최대값 입력할 값
	*/
	void   DrawLabelMaxScale(double dScale);
	/**
	*@brief 라벨을 최대 규모로 그리기
	*@return 최대값 반환
	*/
	double DrawLabelMaxScale();

	//*******************************************************************************
protected:
	std::map<__int64, ITxSymbolPtr> m_mapFeatureSymbol;
	std::map<__int64, ITxLabelPtr>  m_mapLabel;
public:
	/**
	*@brief 심볼 특성
	*@param __int64 nxID			심볼 ID
	*@param ITxSymbolPtr _spSymbol	적용할 심볼
	*/
	virtual void FeatureSymbol(__int64 nxID, ITxSymbolPtr _spSymbol);
	/**
	*@brief 심볼 특성 제거
	*@param __int64 nxID 심볼 ID
	*/
	virtual void RemoveFeatureSymbol(__int64 nxID);
	/**
	*@brief 심볼 특성 초기화
	*/
	virtual void ClearFeatureSymbol();

private:
    bool m_bLabelFilter;
public:
    bool LabelFilter() {
        return m_bLabelFilter;
    }
    void LabelFilter(bool bLabelFilter) {
        m_bLabelFilter = bLabelFilter;
    }
public:
	/**
	*@brief 라벨 특성
	*@param __int64 nxID			라벨 ID
	*@param ITxLabelPtr _spLabel	적용할 라벨
	*/
	virtual void FeatureLabel(__int64 nxID, ITxLabelPtr _spLabel);
	/**
	*@brief 라벨 특성 초기화
	*/
	virtual void ClearFeatureLabel();
	/**
	*@brief 라벨 특성 제거
	*@param __int64 nxID	라벨 ID
	*/
	virtual void RemoveFeatureLabel( __int64 nxID );
	//*******************************************************************************
public:
	/**
	*@brief 레이어 특성 확인
	*@return 참 반환
	*/
	virtual bool IsFeatureLayer() {return true;}

public:
	/**
	*@brief 특성 추출
	*@param __int64 nxID 가져올 ID
	*@return 0
	*/
	virtual TxFeaturePtr   GetFeature(__int64 nxID) = 0;
    virtual ITxGeometryPtr GetGeometry(__int64 nxID) = 0;
	/**
	*@brief 특성 찾기
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeaturePtr>& vecFeature	특성 벡터
	*@param bool bIncludeHide						정의 숨김
	*/
	virtual void FindFeature(TxEnvelope oEnvelope, std::vector<TxFeaturePtr>& vecFeature, bool bIncludeHide=true) = 0;
	/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeaturePtr>& vecFeature	특성 벡터
	*@param bool bIncludeHide						정의 숨김
	*/
protected:
	virtual void FindFeatureDisp(TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, bool bIncludeHide=true) = 0;
public:
	/**
	*@brief 기하학 추출
	*@param std::map<__int64, ITxGeometryPtr>& mapGeometry 
	*/
	virtual void GetGeometry(std::map<__int64, ITxGeometryPtr>& mapGeometry) = 0;
protected:
	/**
	*@brief 특성 찾기 표출
	*@param TxEnvelope oEnvelope					정도
	*@param std::vector<TxFeaturePtr>& vecFeature	특성 벡터
	*@param double dMinExtent						최소 규모
	*@param ThunderMapdrawProcess*					맵 프로세스
	*/
	virtual void FindFeatureDisp( TxEnvelope oEnvelope, std::vector<TxFeatureDispPtr>& vecFeature, double dMinExtent, ThunderMapdrawProcess*) = 0;

public:
	/**
	*@brief 규모
	*@return 0
	*/
	virtual TxEnvelope Extent() = 0;
	/**
	*@brief 엔티티 개수
	*@return 0
	*/
	virtual __int64    EntityCount() = 0;

	//-------------------------------------------------------------------------------
private:
	/*현재 레이어의 툴팁 사용 플래그*/
	bool m_bEnableTooltip;
public:
	/**
	*@brief 맵 툴팁 사용 여부
	*@param bool bEnable 사용 여부 확인
	*/
	void EnableMapTooltip(bool bEnable) {
		m_bEnableTooltip = bEnable;
	}
	/**
	*@brief 맵 툴팁 사용 여부
	*@return 사용 여부 반환
	*/
	bool EnableMapTooltip() {
		return m_bEnableTooltip;
	}
private:
	/*지도의 툴팁을 라벨 또는 확장형(ITxExtendTooltipPtr) */
	bool m_bUseLabelMapTooltip;
public:
	/**
	*@brief 지도 툴팁 라벨 표현
	*/
	void UseLabelMapTooltip() {
		m_bUseLabelMapTooltip = true; 
	}

	/**
	*@brief 지도 툴팁 확장형 데이터 정보로 표현
	*/
	void UseExtendMapTooltip() {
		m_bUseLabelMapTooltip = false;
	}

	/**
	*@brief 지도의 툴팁을 라벨 정보로 표현
	*/
	bool IsUseLabelMapTooltip() {
		return m_bUseLabelMapTooltip;
	}

	/**
	*@brief 지도의 툴팁을 확장형 데이터 정보로 표현
	*/
	bool IsUseExtendMapTooltip() {
		return !m_bUseLabelMapTooltip;
	}
protected:
	std::map<__int64, ITxExtendTooltipPtr> m_mapExTooltip;
public:
	/**
	*@brief 툴팁 특성
	*@param __int64 nxID					해당 ID
	*@param ITxExtendTooltipPtr spTooltip	툴팁 정보
	*/
	virtual void FeatureExTooltip(__int64 nxID, ITxExtendTooltipPtr spTooltip);
	/**
	*@brief 툴팁 초기화
	*/
	virtual void ClearExTooltip();
public:
	/**
	*@brief 툴팁 특성
	*@param __int64 nxID 인덱스 ID
	*@return 툴팁 인덱스 반환
	*/
	virtual ITxExtendTooltipPtr FeatureExTooltip (__int64 nxID);
	/**
	*@brief 툴팁 제거
	*@param __int64 nxID	인덱스 ID
	*/
	virtual void RemoveExTooltip(__int64 nxID);

	//-------------------------------------------------------------------------------
	//★★★★★★★★★★★★★★★★★ Util 기능 ★★★★★★★★★★★★★★★★★ 
	//-------------------------------------------------------------------------------
	//★ 확장 정보 : Feature의 확장 정보 --------------------------------------------
protected:
	std::map<__int64, ITxExtendInfoPtr> m_mapExtendInfo; 
public:
	/**
	*@brief 특성 확장 정보
	*@param __int64 nxID			인덱스 ID
	*@param ITxExtendInfoPtr spInfo 확장 정보
	*/
	virtual void FeatureExtendInfo(__int64 nxID, ITxExtendInfoPtr spInfo);
	/**
	*@brief 확장정보 초기화
	*/
	virtual void ClearExtendInfo();
public:
	/**
	*@brief 특성 확장 정보
	*@param __int64 nxID	인덱스 ID
	*@return 확장 정보 반환
	*/
	virtual ITxExtendInfoPtr FeatureExtendInfo (__int64 nxID);
	/**
	*@brief 확장 정보 제거	
	*@param __int64 nxID	인덱스 ID
	*/
	virtual void RemoveExtendInfo(__int64 nxID);
	//-------------------------------------------------------------------------------

	//★ 특정 문자키와 TxID 그룹 ----------------------------------------------------
protected:
	std::map<CString, std::set<__int64>> m_mapKeySet;
public:
	/**
	*@brief 키 추가
	*@param CString strKey	추가할 키 명
	*@param __int64 nxTxID	인덱스 ID
	*/
	virtual void AddKeySet(CString strKey, __int64 nxTxID);
	/**
	*@brief 키 추가
	*@param CString strKey					추가할 키 명
	*@param std::set<__int64>& setAddTxID	인덱스 ID
	*/
	virtual void AddKeySet(CString strKey, std::set<__int64>& setAddTxID);
	/**
	*@brief 키 제거
	*@param CString strKey 키 명칭
	*/
	virtual void RemoveKeySet (CString strKey);
	/**
	*@brief 키 재설정
	*@param CString strKey					수정할 키 명
	*@param std::set<__int64>& setTxID		인덱스 ID
	*/
	virtual void ReplaceKeySet(CString strKey, std::set<__int64>& setTxID);
	/**
	*@brief 키 초기화
	*@param CString strKey 키명칭	
	*/
	virtual void ClearKeySet(CString strKey);
	/**
	*@brief 키 전체 초기화
	*/
	virtual void ClearKeySet();
public:	
	/**
	*@brief 키 찾기
	*@param CString strKey	키 명칭
	*@return 키 정보 반환
	*/
	virtual std::set<__int64> FindKeySet(CString strKey); 
	//-------------------------------------------------------------------------------

	friend class ThunderMapdrawProcess;
	friend class ThunderMap;
};

typedef std::shared_ptr<ITxFeatureLayer> ITxFeatureLayerPtr;