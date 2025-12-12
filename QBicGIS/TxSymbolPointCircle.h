#pragma once
#include "ITxSymbol.h"
#include <bitset>
/**
*@brief 원형 점 심볼
*@details 원형 점 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
//★ Circle 형태
//★ UsePixelDist : 원의 반경 단위가 픽셀인가 ? 지도의 단위인 Meter인가 ?
//★ 픽셀로 표현할 경우 지도의 확대 축소와 상관없이 일정하게 보임 
class AFX_EXT_CLASS TxSymbolPointCircle : public ITxSymbol
{
public:
    TxSymbolPointCircle(double dRadius, double dOutlineWidth, Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0)) {
        m_bsFlag[EnumFlagDrawFilled]  = false;
        m_bsFlag[EnumFlagDrawOutLine] = true;
        
        m_dRadius       = dRadius;
        m_dOutlineWidth = dOutlineWidth;
        m_clrOutLine    = clrOutLine;
        m_bUsePixelDist = false;

        m_dYShiftRatio  = 0;
        m_dXShiftRatio  = 0;
    }

    TxSymbolPointCircle(double dRadius=10.0, Gdiplus::Color clrFill=Gdiplus::Color(255,255,255), Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0), double dOutlineWidth=1.0) {
        m_clrFill    = clrFill;
        m_clrOutLine = clrOutLine;
        m_bsFlag[EnumFlagDrawFilled]  = true;
        m_bsFlag[EnumFlagDrawOutLine] = true;

        m_dRadius       = dRadius;
        m_dOutlineWidth = dOutlineWidth;
        m_bUsePixelDist = false;

        m_dYShiftRatio  = 0;
        m_dXShiftRatio  = 0;
    }
public:
    virtual ~TxSymbolPointCircle() {};

public:
	/**
	*@brief 타입 설정
	*@return 타입 설정
	*/
    virtual TxSymbolType GetType() {
        return enSymbolPointTypeCircle;
    }

private:
    Gdiplus::Color m_clrFill;
    Gdiplus::Color m_clrOutLine;

    double m_dRadius;
    double m_dOutlineWidth;

private:
    bool m_bUsePixelDist;
public:
	/**
	*@brief 픽셀 거리
	*@return 픽셀 거리
	*/
    bool UsePixelDist() {
        return m_bUsePixelDist;
    }
	/**
	*@brief 픽셀 거리
	*@param bool bUsePixelDist 픽셀 거리
	*/
    void UsePixelDist(bool bUsePixelDist) {
        m_bUsePixelDist = bUsePixelDist;
    }

public:
	/**
	*@brief 반지름
	*@return 반지름
	*/
    double Radius() {
        return m_dRadius;
    }

    void Radius(double dRadius) {
        m_dRadius = dRadius;
    }
	/**
	*@brief 테두리 두께
	*@return 테두리 두께
	*/
    double OutlineWidth() {
        return m_dOutlineWidth;
    }
private:
    enum EnumFlag
    {
        EnumFlagDrawFilled,
        EnumFlagDrawOutLine
    };
    std::bitset<2> m_bsFlag;
public:
	/**
	*@brief 채우기 색
	*@return 채우기 색
	*/
    Gdiplus::Color FillColor() {
        return m_clrFill;
    }
	/**
	*@brief 테두리 색
	*@return 테두리 색
	*/
    Gdiplus::Color OutLineColor() {
        return m_clrOutLine;
    }

public:
	/**
	*@brief 채우기 
	*@return 채우기
	*/
    bool Fill() {
        return m_bsFlag[EnumFlagDrawFilled];
    }
	/**
	*@brief 채우기
	*@param bool bDraw 그리기 채우기
	*/
    void Fill(bool bDraw) {
        m_bsFlag[EnumFlagDrawFilled]  = bDraw;        
    }
	/**
	*@brief 테두리 그리기
	*@return 테두리 그리기
	*/
    bool DrawOutLine() {
        return m_bsFlag[EnumFlagDrawOutLine];
    }
	/**
	*@brief 테두리 그리기
	*@param bool bDraw 그리기 여부
	*/
    void DrawOutLine(bool bDraw) {
        m_bsFlag[EnumFlagDrawOutLine] = bDraw;
    }

//★ Hatch ---------------------------------------------------------------
protected:
    TxHatchInfoPtr m_spHatchInfo;
public:
	/**
	*@brief 해치 제거
	*/
    void RemoveHatch() {
        if (m_spHatchInfo != nullptr) {
            m_spHatchInfo.reset();
        }
    }
	/**
	*@brief 해치 세팅
	*/
    void SetHatch(TxHatchInfoPtr spHatch) {
        m_spHatchInfo = spHatch;
    }
public:
	/**
	*@brief 해치 정보
	*@return 해치 정보 반환
	*/
    TxHatchInfoPtr HatchInfo() {
        return m_spHatchInfo;
    }

public:
	/**
	*@brief 그리기 옵션
	*@return 
	*/
    virtual double DrawOrderOptionValue() {
        return m_nDrawOrder * m_dRadius;
    }

private:
    double m_dYShiftRatio;
public:
    double YShiftRatio() {
        return m_dYShiftRatio;
    }
    void   YShiftRatio(double dRatio) {
        m_dYShiftRatio = dRatio;
    }
private:
    double m_dXShiftRatio;
public:
    double XShiftRatio() {
        return m_dXShiftRatio;
    }
    void   XShiftRatio(double dRatio) {
        m_dXShiftRatio = dRatio;
    }
public:
	/**
	*@brief 포인트 심볼 생성
	*@param double dRadius				반지름
	*@param double dOutlineWidth		선 두께
	*@param Gdiplus::Color clrOutLine	선 색상
	*@return 심볼 반환
	*/
    static std::shared_ptr<TxSymbolPointCircle> CreatePointSymbol(double dRadius, double dOutlineWidth, Gdiplus::Color clrOutLine) 
    {
        std::shared_ptr<TxSymbolPointCircle> spSymbol = std::shared_ptr<TxSymbolPointCircle>(new TxSymbolPointCircle(dRadius, dOutlineWidth, clrOutLine)); {
            spSymbol->UsePixelDist(true);
        }
        return spSymbol;
    }
	/**
	*@brief 포인트 심볼 생성
	*@param double dRadius				반지름
	*@param Gdiplus::Color clrFill		채우는 색
	*@param Gdiplus::Color clrOutLine	선 색상
	*@param double dOutlineWidth		선 두께
	*@return 심볼 반환
	*/
    static std::shared_ptr<TxSymbolPointCircle> CreatePointSymbol(double dRadius, Gdiplus::Color clrFill, Gdiplus::Color clrOutLine, double dOutlineWidth) 
    {
        std::shared_ptr<TxSymbolPointCircle> spSymbol = std::shared_ptr<TxSymbolPointCircle>(new TxSymbolPointCircle(dRadius, clrFill, clrOutLine, dOutlineWidth)); {
            spSymbol->UsePixelDist(true);
        }
        return spSymbol;
    }
};

typedef std::shared_ptr<TxSymbolPointCircle> TxSymbolPointCirclePtr;