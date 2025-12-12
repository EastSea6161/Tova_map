#pragma once
#include "ITxSymbol.h"
#include "TxConst.h"
/**
*@brief 폴리곤 심볼
*@details 폴리곤 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolPolygon : public ITxSymbol
{
public:
    TxSymbolPolygon();
    TxSymbolPolygon(Gdiplus::Color clrFill);
    TxSymbolPolygon(Gdiplus::Color clrFill, Gdiplus::Color clrOutLine);
    TxSymbolPolygon(Gdiplus::Color clrFill, Gdiplus::Color clrOutLine, double dOutLineWidth);
public:
    virtual ~TxSymbolPolygon();

public:
    virtual TxSymbolType GetType();

protected:
    Gdiplus::Color m_clrOutLine;
protected:
    double m_dOutLineWidth;
public:
		/**
	*@brief 테두리 두께
	*@return 테두리 두께
	*/
    double OutLineWidth() {
        return m_dOutLineWidth;
    }
	/**
	*@brief 테두리 두께
	*@param double dOutLineWidth 테두리 두께
	*/
    void   OutLineWidth(double dOutLineWidth) {
        m_dOutLineWidth = dOutLineWidth;
    }
public:
	/**
	*@brief 테두리 색
	*@return 테두리 색
	*/
    Gdiplus::Color OutLineColor() {
        return m_clrOutLine;
    }
	/**
	*@brief 테두리 색
	*@param Gdiplus::Color clrOutLine 테두리 색
	*/
    void OutLineColor(Gdiplus::Color clrOutLine) { 
        m_clrOutLine = clrOutLine;
    }

protected:
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
    bool Fill() {
        return m_bsFlag[EnumFlagDrawFilled];
    }
	/**
	*@brief 채우기
	*@param bool bDraw 채우기 유무
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
	*@brief 테두리
	*@return bool bDraw 테두리 그리기 여부
	*/
    void DrawOutLine(bool bDraw) {
        m_bsFlag[EnumFlagDrawOutLine] = bDraw;
    }

protected:    
    Gdiplus::Color m_clrFill;
public:
	/**
	*@brief 채우기 색
	*@return 채우기 색
	*/
    Gdiplus::Color FillColor() {return m_clrFill;}
	/**
	*@brief 채우기
	*@param bool bDraw 그리기 채우기
	*/
    void FillColor(Gdiplus::Color clrFill) { m_clrFill = clrFill;}

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
        return m_nDrawOrder;
    }
public:
	/**
	*@brief 폴리곤 심볼 생성
	*@param Gdiplus::Color clrFill		채우기 색
	*@param Gdiplus::Color clrOutLine	테두리 색
	*@param double dOutLineWidth		테두리 두께
	*@return 심볼 반환
	*/
	static std::shared_ptr<TxSymbolPolygon> CreatePolygonSymbol(Gdiplus::Color clrFill = Gdiplus::Color(255,255,255), Gdiplus::Color clrOutLine = Gdiplus::Color(0,0,0), double dOutLineWidth = 1.0, bool bFilled=true)
	{
        std::shared_ptr<TxSymbolPolygon> spSymbol = std::shared_ptr<TxSymbolPolygon>(new TxSymbolPolygon(clrFill, clrOutLine, dOutLineWidth)); {
            spSymbol->Fill(bFilled);
        }        

		return spSymbol;
	}
};

typedef std::shared_ptr<TxSymbolPolygon> TxSymbolPolygonPtr;

