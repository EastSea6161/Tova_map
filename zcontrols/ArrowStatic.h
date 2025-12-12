/**
 * @file ArrowStatic.h
 * @brief ZArrowStatic 선언 파일
 * @author 
 * @date 2011.06.14
 * @remark
 */


#pragma once

enum ZEMArrowPosition
{
	ZEMArrowPositionLT = 1,	// left   <top
	ZEMArrowPositionCT = 2, // center <top
	ZEMArrowPositionRT = 3,	// right  <top
	ZEMArrowPositionLC = 4, // left   >center
	ZEMArrowPositionCC = 5, // center >center
	ZEMArrowPositionRC = 6, // right  >center
	ZEMArrowPositionLB = 7, // left   >bottom
	ZEMArrowPositionCB = 8, // center >bottom
	ZEMArrowPositionRB = 9  // right  >bottom
};

/**
 * @brief 주어진 지점을 연결하는 화살표를 그리는 static control class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.14
 */
class AFX_EXT_CLASS ZArrowStatic : public CStatic
{
	DECLARE_DYNAMIC(ZArrowStatic)

public:
	ZArrowStatic();
	virtual ~ZArrowStatic();

protected:
	DECLARE_MESSAGE_MAP()

public :
	void SetPosition(ZEMArrowPosition emStartPosition, ZEMArrowPosition emEndPosition);
    void SetPosition(ZEMArrowPosition emStartPosition, ZEMArrowPosition emMidPosition, ZEMArrowPosition emEndPosition);
	
	void UseMiddlePosition(bool bUse);
	void ArrowLineWidth(float nWidth);	
    void SetArrowCap(float nHeight=5, float nWidth=5, BOOL isCapFilled = FALSE);
	void ArrowLineColor(COLORREF crColor);
    void DashStype(Gdiplus::DashStyle arrowLineDashStype);
protected :
	ZEMArrowPosition m_emStartPosition;
	ZEMArrowPosition m_emEndPosition;
    ZEMArrowPosition m_emMidPosition;

	bool     m_bUseMiddlePosition;
	float    m_nArrowLineWidth;
    COLORREF m_crArrowColor;
    Gdiplus::DashStyle m_arrowLineDashStype;

	float    m_nArrowCapWidthSize;
    float    m_nArrowCapHeightSize;
    BOOL     m_isCapFilled;
	

	CPoint getPositionPoint(CRect& rt, ZEMArrowPosition emPosition);

public:
	afx_msg void OnPaint();
};


