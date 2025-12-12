#pragma once

#include "IncludeDirect2d.h"

#include "ITxLayer.h"
#include "TxFeatureLayer.h"
#include "TxMapLayerManager.h"
#include "ThunderMapDisp.h"

#include "TxPointDecoSymbol.h"
#include "TxPointDecoSymbolCircle.h"

#include "TxLineDecoSymbol.h"
#include "TxLineDecoSymbolArrow.h"
#include "TxLineDecoSymbolCircle.h"
#include "TxLineDecoSymbolCircleCross.h"

#include "TxCachedBitmap.h"

/**
*@brief 테이블 정보
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TLabelInfo
{ 
public:
    TLabelInfo(CString strLabel, TxPoint oPt, double dAngle=0.0) {
        m_strLabel = strLabel;
        m_oPt      = oPt;
        m_dAngle   = dAngle;
    }
    ~TLabelInfo() {}
public:
    CString m_strLabel;
    TxPoint m_oPt;
    double  m_dAngle;
};

typedef std::shared_ptr<TLabelInfo> TLabelInfoPtr;

class TPointDecoInfo {
public:
    TPointDecoInfo(ITxPointDecoSymbolPtr spSymbol, TxPoint oPt) {
        m_oPt = oPt;
        m_spDecoSymbol = spSymbol;
    }
public:
    ITxPointDecoSymbolPtr m_spDecoSymbol;
    TxPoint m_oPt;
};
typedef std::shared_ptr<TPointDecoInfo> TPointDecoInfoPtr;

class TLineDecoInfo {
public:
    TLineDecoInfo(ITxLineDecoSymbolPtr spSymbol, TxPoint oPt, double dAngle=0.0) {
        m_oPt    = oPt;
        m_dAngle = dAngle;

        m_spDecoSymbol = spSymbol;
    }
public:
    ITxLineDecoSymbolPtr m_spDecoSymbol;
    TxPoint m_oPt;
    double  m_dAngle;
};
typedef std::shared_ptr<TLineDecoInfo> TLineDecoInfoPtr;

/**
*@brief 맵 그리기 프로세스
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class ThunderMapdrawProcess
{
private:
    Gdiplus::Point m_pArrowPts[4];

private:
    BLENDFUNCTION m_bf;
private:
    bool m_bFirstTime;
    bool m_bFlicker;

private:
    int  m_nFlickerGap;
    bool m_bAlphaFlicker;
public:
    void MapDrawOptionFlicker(bool bFlicker) {
        m_bFlicker = bFlicker;
    }

private:
    HWND m_hwnd;
public:
    ThunderMapdrawProcess (HWND hWnd, Gdiplus::Color crBackground = Gdiplus::Color::White);
    ~ThunderMapdrawProcess();
public:
	/**
	*@brief 스레드 시작
	*/
    void ThreadStart();
private:
	/**
	*@brief 스레드 종료
	*/
    void ThreadClose();

private:
	/**
	*@brief 맵 작성 프로세스 시작
	*@param void *pParam
	*@return 
	*/
    static unsigned __stdcall MapDrawingThreadProcess(void *pParam);
	/**
	*@brief 맵 작성 프로세스
	*@return 
	*/
    unsigned MapDrawingProcess();    
	/**
	*@brief 
	*@param size_t nxCommandID
	*@param CDC& oMapDC
	*@param Graphics& xGraphics
	*@param ITxFeatureLayerPtr spLayer
	*@param std::vector<TxFeatureDispPtr>& vecDrawFeature
	*@param Bitmap* pBmpStack
	*@return 
	*/
    void MapGDIPlusDrawing( size_t& nxSumDrawCount, bool bFlicker, __int64& nxFlickerTick,
                            size_t nxCommandID, 
                            CDC& oMapDC, 
                            Graphics& oGraphicLayer, 
                            ITxFeatureLayerPtr spLayer, 
                            std::vector<TxFeatureDispPtr>& vecDrawFeature, 
                            CRect& rect, 
                            Bitmap* pBmpStack = nullptr, Bitmap* pBmpLayer = nullptr);

    void MapGDIPlusDrawing( 
                            size_t nxCommandID, 
                            CDC& oMapDC, 
                            Graphics& oGraphicLayer, 
                            ITxFeatureLayerPtr spLayer, 
                            std::vector<TxFeatureDispPtr>& vecDrawFeature, 
                            CRect& rect);
    
private:
    CCriticalSection Lock;
    HANDLE m_hMapDrawingProcess;
    HANDLE m_hEvents[2];
    HANDLE m_hTerminateEvent; // 종료(소멸)         
    HANDLE m_hDrawStartEvent; // 그리기 작업을 시작해도 됩니다.
    HANDLE m_hDrawEndEvent;   // 그리는 작업이 종료되었습니다.

    HANDLE m_hStopDrawEndEvent;  // 명령을 수행해도 될까요 ?
private:
    int    MapProcessStatus;  // 0-드로잉 강제 종료, 드로잉 중(1), 2-드로잉 작업완료
	/**
	*@brief 권한 확인
	*@param int nStatus
	*/
    void   SetMapDrawStatus(int nStatus);
public:
	/**
	*@brief 권한 입력
	*/
    int    GetMapDrawStatus();
private:
    size_t MapCommandID;

public:
	/**
	*@brief 그리기
	*@param ThunderMapDispPtr spThunderMapDisp
	*/
    void Draw(ThunderMapDispPtr spThunderMapDisp);
	/**
	*@brief 그만 그리기
	*@param int nWho
	*/
    void StopDraw(int nWho = 9);
public:
	/**
	*@brief 사이즈 재설정
	*@param UINT width 가로
	*@param UINT height 세로
	*/
    void Resize(UINT width, UINT height);
private:
    TxMapLayerManagerPtr m_spLayerManager;
public:
    void SetLayerManager(TxMapLayerManagerPtr spLayerManager) {
        m_spLayerManager = spLayerManager;
    }
private:
    ThunderMapDispPtr m_spThunderMapDisp;
private:
	/**
	*@brief 시간
	*@return 시간 반환
	*/
    CString ToTime(); 
    __int64 GetMapTickCount();
private:
	/**
	*@brief 화면 품질
	*@param Graphics& graphics
	*@param int nFlag
	*/
    void DisplayQuality(Graphics& graphics, int nFlag);
    void DisplayQualityLabel(Graphics& graphics);
private:
    Bitmap*       m_pBitmapBackground; // 지도이미지
    CachedBitmap* m_pCachedBackground;
public:
	/**
	*@brief 지도 이미지
	*@return 지도 이미지 반환
	*/
    Bitmap* MapImage();
public:
	/**
	*@brief 
	*@param std::vector<ITxLayerPtr>& _vecFlashLayer
	*@param ThunderMapDispPtr spThunderMapDisp
	*/
    void ForegroundRefresh(std::vector<ITxLayerPtr>& _vecFlashLayer, ThunderMapDispPtr spThunderMapDisp);
	/**
	*@brief 
	*@param std::vector<TxFeatureDispPtr>& vecFeatureDisp
	*@param int nTimeEllipse
	*@param int nCount
	*/
    void Flash( std::vector<TxFeatureDispPtr>& vecFeatureDisp, int nTimeInterval=100, int nCount=3 );

public:
	/**
	*@brief 
	*@param Bitmap& oDest 
	*@param Bitmap* pOrigin 
	*/
    void MakeGrayScale(Bitmap& oDest, Bitmap* pOrigin);

private:
	/**
	*@brief 그리기위한 종류
	*@param TxFeatureDispPtr& spLeft 
	*@param TxFeatureDispPtr& spRight
	*@return 
	*/
    static bool DrawSizeDesc( TxFeatureDispPtr& spLeft, TxFeatureDispPtr& spRight );
    static bool DrawIDAsc( TxFeatureDispPtr& spLeft, TxFeatureDispPtr& spRight );

private:
    bool    m_bShowLoadingMessage;
    CString m_strLoadingMessage;

public:
	/**
	*@brief 로딩 메세지
	*@param CString strMsg 메세지
	*/
    void ShowLoadingMessage(CString strMsg = _T("Loading...")) {
        m_strLoadingMessage = strMsg;
    }
	/**
	*@brief 로딩 메세지 숨기기
	*/
    void HideLoadingMessage() {
        m_bShowLoadingMessage = false;
    }

	/*****************************************************************************
	* 2019.03.08 : 지도 배경색상 변경기능 추가
	*****************************************************************************/
	void SetBackgroundColor(Gdiplus::Color crBackground);
	Gdiplus::Color GetBackgroundColor();
	/****************************************************************************/


private:
	/**
	*@brief 로딩 메세지 그리기
	*@param Gdiplus::Graphics& graphics
	*/
    void DrawLoadingMessage(Gdiplus::Graphics& graphics);

private:
    Gdiplus::ImageAttributes m_gdiImgAtt00;
    Gdiplus::ColorMap        m_gdiClrMap;
	
	/*****************************************************************************
	* 2019.03.08 : 지도 배경색상 변경기능 추가
	*****************************************************************************/
	Gdiplus::Color m_crBackground;
	/****************************************************************************/
};


typedef std::shared_ptr<ThunderMapdrawProcess> ThunderMapdrawProcessPtr;
