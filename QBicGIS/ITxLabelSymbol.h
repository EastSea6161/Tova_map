#pragma once
/**
*@brief 레이어 심볼
*@details 레이어 심볼
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
// 전략 : 라벨은 레이어 단위로 설정 가능하도록 한다.
class AFX_EXT_CLASS ITxLabelSymbol
{
public:
    ITxLabelSymbol(double dLabelSize=9.0, Gdiplus::Color color = Gdiplus::Color::Black, CString strFontName=_T("나눔고딕코딩")) {
        m_dLabelSize  = dLabelSize;
        m_clrLabel    = color;
        m_strFontName = strFontName;
        
        m_bFill   = false;
        m_clrFill = Gdiplus::Color::Yellow;
        m_fntStyle = Gdiplus::FontStyleRegular;
    }

    virtual ~ITxLabelSymbol() {
    }

private:
    CString m_strFontName;
public:
	/**
	*@brief 폰트 명칭
	*@return 폰트 명칭
	*/
    CString FontName() const {return m_strFontName;}
	/**
	*@brief 폰트 명칭
	*@param CString _fontName 폰트 명칭
	*/
    void    FontName(CString _fontName) {m_strFontName = _fontName;}
private:
    Gdiplus::FontStyle m_fntStyle;

public:
    Gdiplus::FontStyle FontStyle() {
        return m_fntStyle;
    }

    void FontStyle(Gdiplus::FontStyle fntStyle) {
        m_fntStyle = fntStyle;
    }

private:
    double m_dLabelSize;
public:
	/**
	*@brief 라벨 크기
	*@return 라벨 크기
	*/
    double LabelSize() const {return m_dLabelSize;}
	/**
	*@brief 라벨 크기
	*@param double dLabelSize 라벨 크기
	*/
    void   LabelSize(double dLabelSize) {m_dLabelSize = dLabelSize;}

private:
    Gdiplus::Color m_clrLabel;

public:
	/**
	*@brief 색상
	*@return 색상
	*/
    Gdiplus::Color Color() const         {return m_clrLabel; }
	/**
	*@brief 색상
	*@param Gdiplus::Color color 색상
	*/
    void     Color(Gdiplus::Color color) {m_clrLabel = color;}    

private:
    bool m_bFill;
public:
    bool BackgoundFill() {
        return m_bFill;
    }
    void BackgoundFill(bool bFill) {
        m_bFill = bFill;
    }
private:
    Gdiplus::Color m_clrFill;
public:
    Gdiplus::Color BackgroundColor() {
        return m_clrFill;
    }

    void BackgroundColor(Gdiplus::Color clrFill) {
        m_clrFill = clrFill;
    }
};

typedef std::shared_ptr<ITxLabelSymbol> ITxLabelSymbolPtr;