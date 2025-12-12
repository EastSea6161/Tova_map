#pragma once
#include "ThunderMapCtrl.h"
#include "TxEnvelope.h"
#include "ThunderMapDisp.h"
#include "TxMapMessage.h"
#include "TxVector.h"

/*
MapMode의 의미 :
1. 현재 지도 커서의 의미
2. 지도의 비지니스에는 화면확대, 축소, 이동과 관련된 화면모드와
개발자가 정의한 모드(KEMMapModeBusiness)가 있음.
3. Editing 중간에 화면확대, 축소, 이동을 한 후 다시 
에디팅 모드로 돌아갈려고 하면 어떻게 해야 하는가 ?
*/
// enum KEMMapViewMode 
// {
//     // 확대, 축소, 이동
//     KEMMapViewModeNone         = 0 ,
//     KEMMapViewModeZoomIn       = 1 ,    
//     KEMMapViewModeZoomOut      = 2 ,
//     KEMMapViewModePan          = 3 ,   
//     KEMMapViewModeMPan         = 4 ,  /*0~10번은 예약*/
//     KEMMapViewModeBusiness     =11 
// };

enum EnumMapViewCursor
{
	CURSOR_ARROW, 
	CURSOR_IDENTIFY,
	CURSOR_ADD00,
	CURSOR_PEN, //CURSOR_PEN
	CURSOR_USER01,
	CURSOR_USER02,
	CURSOR_USER03,
	CURSOR_USER04,
	CURSOR_USER05,
	CURSOR_USER06,
	CURSOR_USER07,
	CURSOR_USER08,
	CURSOR_USER09,
	CURSOR_USER10
};

// 클래스의 목적 : 지도와 관련된 기능
/**
*@brief 맵 보기
*@details 지도와 관련 기능
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS IThunderMapView 
{
public:
	IThunderMapView() {
	}
	virtual ~IThunderMapView () {}
protected:
	virtual ThunderMapCtrl* GetThunderMapCtrl() = 0;
public:
	virtual ThunderMapPtr   GetThunderMap() = 0;

public:
	virtual	EnumTxMapCommand ThunderMapCommand();
	virtual void MapCommandZoomIn();
	virtual void MapCommandZoomOut();
	virtual void MapCommandPan();

public:
	virtual void MapCommandZoomExtent(const TxEnvelope& _oEnvelope);

protected:
	/**
	*@brief 맵 명령어 처음 화면으로
	*/
	virtual void MapCommandDispUndo();
	/**
	*@brief 맵 명령어 다시 보기
	*/
	virtual void MapCommandDispRedo();
	/**
	*@brief 맵 처음화면으로 돌아가기 가능여부
	*@return 가능 여부 반환
	*/
	virtual bool MapCanDispUndo();
	/**
	*@brief 맵 새로고침 가능여부
	*@return 가능 여부 반환
	*/
	virtual bool MapCanDispRedo();
public:
	/**
	*@brief 맵 규모 추출
	*@return 규모 반환
	*/
	virtual double MapGetScale();
	/**
	*@brief 맵의 픽셀->거리 추출
	*@return 값 반환
	*/
	virtual double MapGetPixelToDist();

	//@100528 추가 
	//@스케일 설정
	virtual void MapSetScale(double dScale);

public:
	/**
	*@brief 맵 새로고침
	*/
	virtual void MapRefresh();
public:
	/**
	*@brief 맵 레이어 추가
	*@param ITxLayerPtr spLayer	레이어 정보
	*@return 레이어 ID / 실패 -1
	*/
	virtual int  MapAddLayer(ITxLayerPtr spLayer); 
	/**
	*@brief 맵 레이어 추가
	*@param ITxLayerPtr spLayer	레이어 정보
	*@param int nLayerID		레이어 ID
	*@param int nLayerOrder		
	*@return 레이어 ID / 실패 -1
	*/
	virtual int  MapAddLayer(ITxLayerPtr spLayer, int nLayerID, int nLayerOrder = -1);

public:
	/**
	*@brief 우위처리 레이어 추가
	*@param TxFeatureDispPtr spFeatureDisp	표시되는 특성
	*@param bool bShowLabel					레이블 보임 여부
	*@param double dDrawLabelMaxScale		최대 규모로 레이블 생성
	*/
	virtual TxFeatureUserLayerPtr MapAddForegroundLayer(TxFeatureDispPtr spFeatureDisp,                bool bShowLabel=false, double dDrawLabelMaxScale=TxConst::MaxDbl());
	/**
	*@brief 우위처리 레이어 추가
	*@param std::vector<TxFeatureDispPtr>& vecFeatureDisp	표시되는 특성
	*@param bool bShowLabel									레이블 보임 여부
	*@param double dDrawLabelMaxScale						최대 규모로 레이블 생성
	*/
	virtual TxFeatureUserLayerPtr MapAddForegroundLayer(std::vector<TxFeatureDispPtr>& vecFeatureDisp, bool bShowLabel=false, double dDrawLabelMaxScale=TxConst::MaxDbl());

public:
	/**
	*@brief 우위 처리 레이어
	*@param int nZorder						
	*@param ITxFeatureUserLayerPtr spLayer	레이어 특성
	*/
	virtual void MapForegroundLayer(ITxFeatureUserLayerPtr spLayer, int nZorder, bool bFlash = true);
    virtual void MapClearForegroundLayer();
    virtual void MapRemoveForegroundLayer(int nZorder);
	/**
	*@brief 우위 처리 새로고침
	*/
	virtual void MapForegroundRefresh();

public:
	/**
	*@brief 레이어 제거
	*@param int nLayerID	레이어 ID
	*/
	virtual void MapRemoveLayer(int nLayerID);
	/**
	*@brief 레이어 이동
	*@param int nLayerID		레이어 ID
	*@param int nLayerOrder		레이어 명령
	*/
	virtual void MapMoveLayerOrder(int nLayerID, int nLayerOrder = 0 );

    //★ 레이어 ID로 드로잉 순서를 정렬
    virtual void MapLayerOrderByID();
public:
	/**
	*@brief 맵 레이어 추출
	*@param int nLayerID	레이어 ID
	*@return 레이어 반환
	*/
	virtual ITxLayerPtr            MapGetLayer       (int nLayerID);
	/**
	*@brief 맵 레이어 특성 추출
	*@param int nLayerID 레이어 ID
	*@return 레이어 특성 반환
	*/
	virtual ITxFeatureLayerPtr     MapGetFeatureLayer(int nLayerID);
	/**
	*@brief 맵 유저 레이어 특성 추출
	*@param int nLayerID 레이어 ID
	*@return 유저 레이어 특성 반환
	*/
	virtual ITxFeatureUserLayerPtr MapGetFeatureUserLayer(int nLayerID);
public:
	/**
	*@brief 맵 유저 파일 레이어 특성 추출
	*@param int nLayerID 레이어 ID
	*@return 유저 레이어 반환
	*/
	virtual TxFeatureFileUserLayerPtr MapGetFileUserLayer(int nLayerID);
public:
	/**
	*@brief 맵 레이어 관리
	*@return 맵 관리 반환
	*/
	virtual TxMapLayerManagerPtr MapGetLayerManager();
	/**
	*@brief 맵 레이어
	*@return 맵 레이어 반환(벡터)
	*/
	virtual std::vector<ITxLayerPtr> MapLayers();
	/**
	*@brief 최대 확장
	*@param TxEnvelope& oEnvelope 범위
	*@return 엔티티 반환
	*/
	virtual size_t MapGetFullExtent(TxEnvelope& oEnvelope);
public:
	/**
	*@brief 레이어 라벨
	*@param int nLayerID	레이어 ID
	*@param bool bShow		보여주기 여부
	*@param bool bRefresh	새로고침 여부
	*/
	virtual void MapLayerLabel(int nLayerID, bool bShow, bool bRefresh=false);
public:
	/**
	*@brief 레이어 보여주기
	*@param int nLayerID	레이어 ID
	*@param bool bShow		보여주기 여부
	*/
	virtual void MapShowLayer(int nLayerID, bool bShow);
	/**
	*@brief 레이어 보여주기
	*@param int nLayerID			레이어 ID
	*@param bool bShow				보여주기 여부
	*@param double a_dMinDrawScale	최소 규모 작성
	*@param double dMaxDrawScale	최대 규모 작성
	*/
	virtual void MapShowLayer(int nLayerID, bool bShow, double a_dMinDrawScale, double dMaxDrawScale);
	/**
	*@brief 레이어 규모
	*@param int nLayerID			레이어 ID
	*@param double a_dMinDrawScale	최소규모 작성
	*@param double a_dMaxDrawScale	최대규모 작성
	*/
	virtual void MapLayerScale(int nLayerID, double a_dMinDrawScale, double a_dMaxDrawScale);
public:
	/**
	*@brief 레이어 초기화
	*@param int a_nLayerID	레이어 ID
	*/
	virtual void MapClearLayer(int a_nLayerID);
	/**
	*@brief 심볼 설정
	*@param int nLayerID			레이어 ID
	*@param ITxSymbolPtr spSymbol	심볼
	*@param bool a_bVisible			보여주기 여부
	*/

	virtual void MapSetSymbol(int nLayerID, ITxSymbolPtr spSymbol, bool a_bVisible=true);
	/**
	*@brief 심볼 설정
	*@param int nLayerID			레이어 ID
	*@param __int64 nxFeatureID		특성 ID
	*@param ITxSymbolPtr spSymbol	심볼
	*/

    //★ 레이어 검색을 하기 때문에 많은 양의 Entity 심볼 설정을 할 때는 사용하지 마세요.
    //★ ITxFeatureLayer -> FeatureSymbol 함수를 사용
	virtual void MapSetSymbol(int nLayerID, __int64 nxFeatureID, ITxSymbolPtr spSymbol);
	/**
	*@brief 레이어 설정
	*@param int nLayerID			레이어 ID
	*@param	__int64 nxFeatureID		특성 ID
	*@param ITxLabelPtr spLabel		라벨
	*/

    //★ 레이어 검색을 하기 때문에 많은 양의 Entity 라벨 설정을 할 때는 사용하지 마세요.
    //★ ITxFeatureLayer -> FeatureLabel 함수를 사용
	virtual void MapSetLabel (int nLayerID, __int64 nxFeatureID, ITxLabelPtr spLabel);
	/**
	*@brief 라벨 심볼 설정
	*@param int nLayerID					레이어 ID
	*@param ITxLabelSymbolPtr spLabelSymbol	라벨 심볼
	*@param double dLabelMaxScale			최대 스케일
	*/
	virtual void MapSetLabelSymbol(int nLayerID, ITxLabelSymbolPtr spLabelSymbol, double dLabelMaxScale);
	/**
	*@brief 규모 추출
	*@param TxEnvelope &oEnvelope
	*/
	virtual void MapGetEnvelope(TxEnvelope &oEnvelope);
public:
	/**
	*@brief 지도의 모드는 사용자 모드, 확대, 축소, 이동 모드로 구성. 
	*/
	virtual void MapBusinessMode();
    virtual void MapSetBusinessMode() {
        //★ Naming 변경
        MapBusinessMode();
    }
public:
	/**
	*@brief 보이기
	*@param std::vector<TxFeatureDispPtr>& vecFeatureDisp	특성표출
	*@param int nTimeEllipse								주기
	*@param int nCount										횟수
	*/
	virtual void Flash(std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeEllipse=100, int nCount=3);
public:
	/**
	*@brief 피드백
	*@param int a_nBusinessFeatureType	비지니스 특성 타입
	*@param std::vector<__int64>		__int64형 벡터
	*/
	virtual void MapFeedback(int a_nBusinessFeatureType, std::vector<__int64>)=0;
	/**
	*@brief 피드백
	*@param int a_nBusinessFeatureType	비지니스 특성 타입
	*@param __int64						__int64형 자료
	*/
	virtual void MapFeedback(int a_nBusinessFeatureType, __int64)=0;

public:
	/**
	*@brief 지도의 현재 보여지는 이미지 저장
	*@param CString strPath	경로 설정
	*@return 성공시 경로에 저장 / 실패시 false 반환
	*/
	bool MapWriteImage(CString strPath);
public:
	/**
	*@brief 로딩중 메세지 전달
	*@param CString strMsg
	*/
	void ShowLoadingMessage(CString strMsg = _T("Loading..."));
	/**
	*@brief 로딩중 메세지 전달
	*/
	void HideLoadingMessage();

public:
    void MapDrawOptionFlicker(bool bFlicker);

public:
    void MapTooltipEnable(bool bEnable);
};
