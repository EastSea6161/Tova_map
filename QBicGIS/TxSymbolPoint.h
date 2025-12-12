#pragma once
#include "ITxSymbol.h"
#include "TxPointDecoSymbol.h"

/**
*@brief 사각형 심볼
*@details 사각형 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolPoint : public ITxSymbol
{
public:
    TxSymbolPoint(char cSymbol= 48, double dSize=10.0, Gdiplus::Color color=Gdiplus::Color(255,0,0) );
public:
    virtual ~TxSymbolPoint();

public:
    virtual TxSymbolType GetType();

private:
    char     m_cSymbol;  // 심볼 ID : 향후, 다양한 심볼을 위한
    double   m_dSize;    // 심볼 크기
	bool	 m_bFixedSize;

    Gdiplus::Color m_clrPoint;

public:
    int  Symbol();
    void Symbol(char nSymbol);
public:
    double Size();
    void   Size(double dSize);
public:
    Gdiplus::Color Color();
    void     Color(Gdiplus::Color color);    
public:
    virtual double DrawOrderOptionValue();

public:
	//jin 추가 기능  고정크기 심볼...
	bool FixedSize(){
		return m_bFixedSize;
	}
	void FixedSize(bool bFixedSize) {
		m_bFixedSize = bFixedSize;
	}

public:
    bool operator < (const TxSymbolPoint& right) const;

private:
    std::map<int, ITxPointDecoSymbolPtr> m_mapDeco;
public:
    std::map<int, ITxPointDecoSymbolPtr> GetDecoSymbol();    
public:
    void SetDecoSymbol(ITxPointDecoSymbolPtr spDecoSym, int nIdx = 0);
    void RemoveDecoSymbol();
    void RemoveDecoSymbol(int nIdx);
public:
    static std::shared_ptr<TxSymbolPoint> CreatePointSymbol(char cSymbol=48, double dSize=8.0, Gdiplus::Color color=Gdiplus::Color(0,0,0))
    {
        return std::shared_ptr<TxSymbolPoint> (new TxSymbolPoint(cSymbol, dSize, color));
    }
};

typedef std::shared_ptr<TxSymbolPoint> TxSymbolPointPtr;