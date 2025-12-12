#pragma once
/**
*@brief 해치
*@details 해치 정보
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxHatchInfo 
{
public:
    TxHatchInfo(Gdiplus::HatchStyle hatchStyle = Gdiplus::HatchStyleBackwardDiagonal, Gdiplus::Color clrHatch = Gdiplus::Color::Black) {
        m_hatchStyle = hatchStyle;
        m_clrHatch   = clrHatch;
    }
public:
    virtual ~TxHatchInfo() {
    }
    
private:
    Gdiplus::HatchStyle m_hatchStyle;
public:
	/**
	*@brief 해치 스타일
	*@param Gdiplus::HatchStyle hatchStyle 해치 스타일 종류
	*/
    void HatchStyle(Gdiplus::HatchStyle hatchStyle) {
        m_hatchStyle = hatchStyle;
    }
	/**
	*@brief 해치 스타일
	*@return 해치 스타일 반환
	*/
    Gdiplus::HatchStyle HatchStyle() {
        return m_hatchStyle;
    }

private:
    Gdiplus::Color m_clrHatch;
public:
	/**
	*@brief 해치 색
	*@return 해치 색 반환
	*/
    Gdiplus::Color HatchColor() {
        return m_clrHatch;
    }
	/**
	*@brief 해치 색
	*@param Gdiplus::Color clrHatch 해치 색
	*/
    void HatchColor(Gdiplus::Color clrHatch) { 
        m_clrHatch = clrHatch;
    }
};

typedef std::shared_ptr<TxHatchInfo> TxHatchInfoPtr;