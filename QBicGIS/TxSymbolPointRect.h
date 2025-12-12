#pragma once
#include "ITxSymbol.h"
#include <bitset>
/**
*@brief 직사각형 점 심볼
*@details 직사각형 점 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
//★ Circle 형태
//★ UsePixelDist : 원의 반경 단위가 픽셀인가 ? 지도의 단위인 Meter인가 ?
//★ 픽셀로 표현할 경우 지도의 확대 축소와 상관없이 일정하게 보임 
class AFX_EXT_CLASS TxSymbolPointRect : public ITxSymbol
{
public:
    TxSymbolPointRect(double dWidth, double dOutlineWidth, Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0)) {
        m_bsFlag[EnumFlagDrawFilled]  = false;
        m_bsFlag[EnumFlagDrawOutLine] = true;

        m_dWidth        = dWidth;
        m_dOutlineWidth = dOutlineWidth;
        m_clrOutLine    = clrOutLine;
        m_bUsePixelDist = false;
    }

    TxSymbolPointRect(double dWidth=10.0, Gdiplus::Color clrFill=Gdiplus::Color(255,255,255), Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0), double dOutlineWidth=1.0) {
        m_clrFill    = clrFill;
        m_clrOutLine = clrOutLine;
        m_bsFlag[EnumFlagDrawFilled]  = true;
        m_bsFlag[EnumFlagDrawOutLine] = true;

        m_dWidth        = dWidth;
        m_dOutlineWidth = dOutlineWidth;
        m_bUsePixelDist = false;
    }
public:
    virtual ~TxSymbolPointRect() {};

public:
	/**
	*@brief 타입 설정
	*@return 타입 설정
	*/
    virtual TxSymbolType GetType() {
        return enSymbolPointTypeRect;
    }

private:
    Gdiplus::Color m_clrFill;
    Gdiplus::Color m_clrOutLine;

    double m_dWidth;
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
	*@brief 길이
	*@return 길이
	*/
    double Width() {
        return m_dWidth;
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
        return m_nDrawOrder * m_dWidth;
    }

public:
	/**
	*@brief 포인트 심볼 생성
	*@param double dSize				사이즈(길이)
	*@param double dOutlineWidth		선 두께
	*@param Gdiplus::Color clrOutLine	선 색상
	*@return 심볼 반환
	*/
    static ITxSymbolPtr CreatePointSymbol(double dSize = 20.0, double dOutlineWidth = 1, Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0))
    {
        std::shared_ptr<TxSymbolPointRect> spSymbol = std::shared_ptr<TxSymbolPointRect>(new TxSymbolPointRect(dSize, dOutlineWidth, clrOutLine)); {
            spSymbol->UsePixelDist(true);
        }
        return spSymbol;
    }
};

typedef std::shared_ptr<TxSymbolPointRect> TxSymbolPointRectPtr;