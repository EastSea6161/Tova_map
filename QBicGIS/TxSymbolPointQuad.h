#pragma once
#include "ITxSymbol.h"
/**
*@brief 원형 점 심볼
*@details 원형 점 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolPointQuad : public ITxSymbol
{
public:
    TxSymbolPointQuad(double dSize=16.0, Gdiplus::Color clrOutLine=Gdiplus::Color(0,0,0), double dOutlineWidth=0.1) {
        m_clrOutLine = clrOutLine;
        m_dSize      = dSize;

        m_dOutlineWidth = dOutlineWidth;
		m_clrFill[0] = Gdiplus::Color(200,50,132,255);
		m_clrFill[1] = Gdiplus::Color(200,38,177,76);
		m_clrFill[2] = Gdiplus::Color(200,255,52,36);
		m_clrFill[3] = Gdiplus::Color(200,255,187,0);
	}
public:
    virtual ~TxSymbolPointQuad() {};

public:
	/**
	*@brief 타입 설정
	*@return 타입 설정
	*/
    virtual TxSymbolType GetType() {
        return enSymbolPointTypeQuad;
    }
public:
	/**
	*@brief 보이기
	*/
    virtual bool Show() {
        return true;
    }
private:
    Gdiplus::Color m_clrOutLine;
    Gdiplus::Color m_clrFill[4];

    double m_dSize;
    double m_dOutlineWidth;
public:
	/**
	*@brief 사이즈
	*@return 사이즈
	*/
    double Size() {
        return m_dSize;
    }
	/**
	*@brief 사이즈
	*@param double dSize 크기
	*/
    void Size(double dSize) {
        m_dSize = dSize;
    }
	/**
	*@brief 테두리 두께
	*@return 테두리 두께
	*/
    double OutlineWidth() {
        return m_dOutlineWidth;
    }
	/**
	*@brief 테두리 두께
	*@param double dWidth 테두리 두께
	*/
    void OutlineWidth(double dWidth) {
        m_dOutlineWidth = dWidth;
    }

public:
	/**
	*@brief 채우기 색
	*@param int nIndex 색 인덱스
	*@return 채우기 색
	*/
    Gdiplus::Color FillColor(int nIndex) {
        return m_clrFill[nIndex%4];
    }
	/**
	*@brief 채우기
	*@param int nIndex 색 인덱스
	*@param bool bDraw 그리기 채우기
	*/
    void FillColor(int nIndex, Gdiplus::Color clrFill) {
        m_clrFill[nIndex%4] = clrFill;
    }
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
    void OutLineColor (Gdiplus::Color clrOutLine) {
        m_clrOutLine = clrOutLine;
    }

public:
	/**
	*@brief 그리기 옵션
	*@return 
	*/
    virtual double DrawOrderOptionValue() {
        return m_nDrawOrder;
    }
};

typedef std::shared_ptr<TxSymbolPointQuad> TxSymbolPointQuadPtr;