#pragma once
#include "ITxSymbol.h"
#include "TxConst.h"
#include "ThunderMapDrawProcess.h"
#include "TxLineDecoSymbol.h"
/**
*@brief 오프셋
*@details 오프셋
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxOffsetInfo
{
public:
    TxOffsetInfo(double dOffsetDist = 1.0, int nOffsetMaxScale = TxConst::MaxInt(), bool bUseOffsetWidth = true) {
        m_dOffsetDist     = dOffsetDist;        
        m_nOffsetMaxScale = nOffsetMaxScale;

        m_bUseOffsetWidth = bUseOffsetWidth;
		m_bUsemeterDist	= false;
    }
    virtual ~TxOffsetInfo(){}
private:
    //★ 원위치에서 우측방향으로 떨어진 거리
    double m_dOffsetDist;          
private:
    //★ 디스플레이 -> 우측방향 거리 + 선의 두께를 고려할 것인가. 
    //★ 선의 두께가 큰 경우 Offset해도 중복되어 표현된다.
    bool m_bUseOffsetWidth;

	//★ 미터단위로..표출하기여부
	bool m_bUsemeterDist;
private:
    //★ Offset이 적용되는 스케일
    int m_nOffsetMaxScale;

public:
	/**
	*@brief 오프셋 거리
	*@return 오프셋 거리
	*/
    double OffsetDist() {
        return m_dOffsetDist;
    }
	/**
	*@brief 오프셋 거리
	*@param double dDist
	*/
    void OffsetDist(double dDist) {
        m_dOffsetDist = dDist;
    }

public:
	/**
	*@brief 똑바로 된 거리??
	*@return 
	*/
    bool UseRightDirectionWidth() {
        return m_bUseOffsetWidth;
    }
	/**
	*@brief 똑바로 된 거리?
	*@param bool bOffsetWidth
	*/
    void UseRightDirectionWidth(bool bOffsetWidth) { 
        m_bUseOffsetWidth = bOffsetWidth; 
    }

	bool UseMeterDist(){
		return m_bUsemeterDist;
	}

	void UseMeterDist(bool bUseMeterDist) {
		m_bUsemeterDist = bUseMeterDist;
	}

public:
	/**
	*@brief 오프셋 스케일
	*@return 스케일 반환
	*/
    int OffsetScale(){
        return m_nOffsetMaxScale;
    }

public:
	/**
	*@brief 오프셋 설정
	*@param double dOffsetDist
	*@param int nOffsetMaxScale
	*/
    void SetOffset(double dOffsetDist = 1.0, int nOffsetMaxScale = TxConst::MaxInt()) {
        m_dOffsetDist     = dOffsetDist;
        m_nOffsetMaxScale = nOffsetMaxScale;
    }
};
typedef std::shared_ptr<TxOffsetInfo> TxOffsetInfoPtr;


/**
*@brief 선
*@details 선
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxSymbolLine : public ITxSymbol
{
public:
    TxSymbolLine(double dLineWidth=1.0, Gdiplus::Color color=Gdiplus::Color::Black, bool bUseOffset=false);
    virtual ~TxSymbolLine();
public:
    virtual TxSymbolType GetType() {return enSymbolLineType;}

private:
    // DashStyleSolid        = 0,
    // DashStyleDash         = 1,
    // DashStyleDot          = 2,
    // DashStyleDashDot      = 3,
    // DashStyleDashDotDot   = 4,
    Gdiplus::DashStyle m_nDashStyle; 
public:
    Gdiplus::DashStyle GetDashStyle();
public:
    void SetDashStyle(int nStyle/*0:DashStyleSolid, 1:DashStyleDash, 2:DashStyleDot, 3:DashStyleDashDot, 4:DashStyleDashDotDot*/);
private:
    double m_dLineWidth;
public:
	/**
	*@brief 선 두께
	*@return 선 두께
	*/
    double LineWidth() {return m_dLineWidth;}
	/**
	*@brief 선 두께
	*@param double dLineWidth
	*/
    void   LineWidth(double dLineWidth) {m_dLineWidth = dLineWidth;}

private:
    Gdiplus::Color m_clrLine;

public:
	/**
	*@brief 선 색
	*@return 선 색
	*/
    Gdiplus::Color Color()               {return m_clrLine; }
	/**
	*@brief 선 색
	*@param Gdiplus::Color color
	*/
    void     Color(Gdiplus::Color color) {m_clrLine = color;}    

private:
    TxOffsetInfoPtr m_spOffsetInfo;
public:
	/**
	*@brief 오프셋 설정
	*@param TxOffsetInfoPtr spOffsetInfo
	*/
    void SetOffset(TxOffsetInfoPtr spOffsetInfo);
	/**
	*@brief 오프셋 제거
	*/
    void RemoveOffset();

private:
    std::map<int, ITxLineDecoSymbolPtr> m_mapDeco;
public:
    std::map<int, ITxLineDecoSymbolPtr> GetDecoSymbol();    
public:
    void SetDecoSymbol(ITxLineDecoSymbolPtr spDecoSym, int nIdx = 0);
    void RemoveDecoSymbol();
    void RemoveDecoSymbol(int nIdx);
public: 
	/**
	*@brief 그리기 옵션
	*@return 
	*/
    virtual double DrawOrderOptionValue() {
        return m_dLineWidth * m_nDrawOrder;
    }

public:
	/**
	*@brief 선 설정 생성
	*@param double dLineWidth
	*@param Gdiplus::Color clrLine
	*@param bool bUseOffset
	*@return 
	*/
    static std::shared_ptr<TxSymbolLine> CreateLineSymbol(double dLineWidth=1.0, Gdiplus::Color clrLine=Gdiplus::Color::Black, bool bUseOffset=false)
    {
        std::shared_ptr<TxSymbolLine> spSymbol(new TxSymbolLine(dLineWidth, clrLine));
        if (bUseOffset == true) {
            TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(0));
            spSymbol->SetOffset(spOffset);
        }

        return spSymbol;
    }
	/**
	*@brief 오프셋 설정 생성
	*@param double dLineWidth
	*@param Gdiplus::Color clrLine
	*@param bool bUseOffset
	*@return 
	*/
    static std::shared_ptr<TxSymbolLine> CreateOffsetLineSymbol(double dLineWidth=1.0, Gdiplus::Color clrLine=Gdiplus::Color::Black, double dOffset=0)
    {
        std::shared_ptr<TxSymbolLine> spSymbol(new TxSymbolLine(dLineWidth, clrLine));
        TxOffsetInfoPtr spOffset = TxOffsetInfoPtr(new TxOffsetInfo(dOffset));
        spSymbol->SetOffset(spOffset);

        return spSymbol;
    }

friend class ThunderMapdrawProcess;
};


typedef std::shared_ptr<TxSymbolLine> TxSymbolLinePtr;