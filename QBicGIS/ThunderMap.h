#pragma once

#include "IThunderMap.h"

#include "ITxMapServer.h"
#include "TxSQLMapServer.h"
#include "ThunderMapDrawProcess.h"
#include "ITxLayer.h"
#include "TxImageLayer.h"
#include "TxImageTmsLayer.h"

#include "TxFeatureLayer.h"
#include "TxFeatureFileUserLayer.h"
#include "TxFeatureUserLayer.h"
#include "TxLayerInfo.h"
#include "ThunderMapDisp.h"
#include "IThunderMapCommand.h"
#include "ThunderMapCommandMPan.h"
#include "TxMapEnumType.h"

#include "ITxSymbol.h"

#include "TxSymbolPoint.h"
#include "TxSymbolPointCircle.h"
#include "TxSymbolPointQuad.h"
#include "TxSymbolPointRect.h"
#include "TxSymbolPointImage.h"

#include "TxSymbolLine.h"
#include "TxSymbolDirectionLine.h"

#include "TxSymbolPolygon.h"
#include "ITxLabelSymbol.h"

#include "TxTables.h"
#include "TxSpatialOper.h"

#include "TxMakeCurve.h"

class TxTooltipView;
/**
*@brief 맵 제어
*@details 맵 제어
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
// 모든 커맨드는 여기서 제어한다.
class AFX_EXT_CLASS ThunderMap : public IThunderMap
{
public:
    ThunderMap ();
    virtual ~ThunderMap();

//레이어를 총괄관리
private:
    TxMapLayerManagerPtr m_spMapLayerManager;

public:
	/**
	*@brief 레이어 추가
	*@param ITxLayerPtr spLayer
	*@return return : 레이어 ID, 실패(-1)
	*/
    int AddLayer(ITxLayerPtr spLayer); //★ return : 레이어 ID, 실패(-1)
	/**
	*@brief 레이어 추가
	*@param ITxLayerPtr spLayer
	*@param int nLayerID		레이어 ID
	*@param int nLayerOrder		레이어 오더
	*@return return : 레이어 ID, 실패(-1)
	*/
    int AddLayer(ITxLayerPtr spLayer, int nLayerID, int nLayerOrder = -1); //★ return : 레이어 ID, 실패(-1)

public:
	/**
	*@brief 레이어 관리
	*@return 
	*/
    virtual TxMapLayerManagerPtr LayerManager() {return m_spMapLayerManager;}

public:
	/**
	*@brief 레이어 설정
	*@param int nLayerID 레이어 ID
	*@return 레이어 ID
	*/
    ITxLayerPtr GetLayer   (int nLayerID);
	/**
	*@brief 레이어 제거
	*@param int nLayerID 레이어 ID
	*/
    void RemoveLayer(int nLayerID);
	/**
	*@brief 레이어 이동
	*@param int nLayerID 레이어 ID
	*@param int nLayerOrder 움직일 범위
	*/
    void MoveLayerOrder  (int nLayerID, int nLayerOrder=0);
    void LayerOrderByID();
// 지도 드로잉
private:
    HWND m_hwnd;
public:
	/**
	*@brief 핸들 설정
	*@return 핸들 
	*/
    virtual HWND GetHwnd() {
        CWnd* pHandleWnd = CWnd::FromHandlePermanent(m_hwnd); 
        if (pHandleWnd == nullptr)
            return nullptr;

        return m_hwnd;
    }
private:
    ThunderMapdrawProcessPtr m_spThunderDraw;
private:    
	/**
	*@brief 맵 그리기 설정
	*@return 
	*/
    virtual ThunderMapdrawProcessPtr GetThunderDxDraw();
public:
	/**
	*@brief 윈도우 셋팅
	*@param HWND hwnd 핸들 설정
	*/
    void SetWindow(HWND hwnd);
	/**
	*@brief 작성
	*/
    void Draw();
	/**
	*@brief 사이즈 조절
	*@param UINT width	가로 길이
	*@param UINT height 세로 길이
	*/
    void Resize(UINT width, UINT height);

private:
    // 휘발성 객체를 만들어야 한다. 잠시 보관용
    std::map<int, ITxLayerPtr> m_mapForeLayer;    
    std::map<int, ITxLayerPtr> m_mapFixForeLayer;

public:
    void ClearForegroundLayer();
    void RemoveForegroundLayer(int nZorder);
public:
	/**
	*@brief 우위 레이어 추가
	*@param std::vector<TxFeatureDispPtr>& _vecFeature
	*@param bool bShowLabel								라벨 표시
	*@param double dLabelMaxScale						최대 규모
	*/
    TxFeatureUserLayerPtr AddForegroundLayer(std::vector<TxFeatureDispPtr>& _vecFeature, bool bShowLabel=false, double dLabelMaxScale=TxConst::MaxDbl());
	/**
	*@brief 우위 레이어 추가
	*@param TxFeatureDispPtr _spFeature
	*@param bool bShowLabel								라벨 표시
	*@param double dLabelMaxScale						최대 규모
	*/
    TxFeatureUserLayerPtr AddForegroundLayer(TxFeatureDispPtr _spFeature, bool bShowLabel=false, double dLabelMaxScale=TxConst::MaxDbl());
public:
	/**
	*@brief 우위 레이어 설정
	*@param int nZorder						
	*@param ITxFeatureUserLayerPtr spLayer
	*/
    void SetForegroundLayer(ITxFeatureUserLayerPtr spLayer, int nZorder, bool bFlash=true);
public:
	/**
	*@brief 새로고침
	*/
    void ForegroundRefresh();
public:
    void Flash( std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeInterval=100, int nCount=3);
// 마우스 이벤트
public:
		/**
	*@brief 마우스 왼쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonUp     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 왼쪽버튼 더블 클릭
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnLButtonDblClk (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 움직일때
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMouseMove     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonUp     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 오른쪽버튼 더블 클릭
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnRButtonDblClk (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 버튼 누름상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMButtonDown   (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 버튼 뗀 상태
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMButtonUp     (UINT nFlags, CPoint point);
	/**
	*@brief 마우스 휠 움직일 때
	*@param UNIT nFlags
	*@param short zDelta 
	*@param CPoint point 
	*/
    virtual void OnMouseWheel    (UINT nFlags, short zDelta, CPoint point);
	/**
	*@brief 마우스를 올려 놓았을 때
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMouseHover    (UINT nFlags, CPoint point);
	/**
	*@brief 마우스를 올려 놓았다가 밖으로 빨 때
	*@param UNIT nFlags
	*@param CPoint point 
	*/
    virtual void OnMouseLeave();
private:
    ThunderMapCommandMPanPtr     m_spThunderMapMPanCommand;

// 좌표정보
private:
    ThunderMapDispPtr m_spThunderMapDisp;
private:
	/**
	*@brief 맵 거리
	*@return 맵 거리
	*/
    virtual ThunderMapDispPtr GetThunderMapDisp() {return m_spThunderMapDisp;}

private:
    IThunderMapCommandPtr m_spMapCommand; // 지도 명령
public:
	/**
	*@brief 맵 명령
	*@return 맵 명령어
	*/
    EnumTxMapCommand MapCommand();
	/**
	*@brief 맵 명령 선택
	*@param EnumTxMapCommand enumMapCommand
	*/
    void MapCommand(EnumTxMapCommand enumMapCommand);

public:
	/**
	*@brief 맵 크기 설정
	*@param const TxEnvelope& _oEnvelope 정도 설정
	*/
    virtual void SetMapExtent(const TxEnvelope& _oEnvelope);

public:
	/**
	*@brief 화면 다시 그리기
	*/
    virtual void Refresh();

#pragma region MapUtilFunction
	/**
	*@brief 맵으로 화면 이동
	*@param CPoint point
	*@return 맵 반환
	*/
    virtual TxPoint ScreenToMap(CPoint point);
	/**
	*@brief 픽셀과의 거리
	*@return 픽셀과의 거리 반환
	*/
    virtual double  PixelDist();
	/**
	*@brief 버텍스와의 거리
	*@return 버텍스와의 거리 반환
	*/
    virtual double  PointDist();
#pragma endregion MapUtilFunction

public:
	/**
	*@brief 초기화
	*/
    void MapDisplayUndo();
	/**
	*@brief 새로고침
	*/
    void MapDisplayRedo();
	/**
	*@brief 초기화 가능 확인
	*@return 가능 여부 반환
	*/
	bool CanMapDisplayUndo();
	/**
	*@brief 새로고침 반환 여부
	*@return 가능 여부 반환
	*/
	bool CanMapDisplayRedo();
public:
	/**
	*@brief 맵 표현 구역 설정
	*@return 맵 반환
	*/
	double GetMapDisplayScale();
	/**
	*@brief 맵 표현 픽셀,거리 설정
	*@return 맵 반환
	*/
	double GetMapDisplayPixelToDist();

	//@100528 추가 
	//@스케일 설정
	void SetMapDisplayScale(double dScale);

public:
	/**
	*@brief 맵 구역 설정
	*@return 맵 반환
	*/
	TxEnvelope GetMapDisplayEnvelope();
private:
    int TESTID;
    std::shared_ptr<TxTooltipView> m_spTxTooltipView;
private:
    bool m_bEnableTooltip;
public:
	/**
	*@brief 맵 툴팁 가능 여부
	*@return 가능 여부 반환
	*/
    bool EnableMapTooltip() {
        return m_bEnableTooltip;
    }
	/**
	*@brief 맵 툴팁 가능 여부
	*@param bool bEnable 가능 여부 전달
	*/
    void EnableMapTooltip(bool bEnable) {
        m_bEnableTooltip = bEnable;
    }
private:
	/**
	*@brief 인코딩된 그리드 아이디
	*@param LPCWSTR pwszFormat	들어오는 포멧
	*@param GUID *pGUID			나가는 아이디
	*@return 
	*/
    HRESULT GetGdiplusEncoderClsid(__in LPCWSTR pwszFormat, __out GUID *pGUID);
public:
	/**
	*@brief 맵 이미지 작성
	*@param CString strPath	파일 경로
	*@return 성공시 ture, 실패시 false 반환
	*/
    bool MapWriteImage(CString strPath);
public:
	/**
	*@brief 로딩 메세지 표현
	*@param CString strMsg
	*/
    void ShowLoadingMessage(CString strMsg = _T("Loading..."));
	/**
	*@brief 로딩 메세지 숨기기
	*/
    void HideLoadingMessage();

public:
    void MapDrawOptionFlicker(bool bFlicker);
public:
    int  GetMapDrawStatus();

	/*****************************************************************************
	* 2019.03.08 : 지도 배경색상 변경기능 추가
	*****************************************************************************/
protected: 
	Gdiplus::Color m_crBackground;
public:
	void SetBackgroundColor(Gdiplus::Color crBackground);
	Gdiplus::Color GetBackgroundColor();
	/****************************************************************************/
};

typedef std::shared_ptr<ThunderMap> ThunderMapPtr;