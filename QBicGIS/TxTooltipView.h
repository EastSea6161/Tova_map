#pragma once
#include "Resource.h"

// TxTooltipView 대화 상자입니다.
/**
*@brief 툴팁 보기
*@details 툴팁 보기 다이얼로
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class TxTooltipView : public CDialogEx
{
	DECLARE_DYNAMIC(TxTooltipView)

public:
	TxTooltipView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~TxTooltipView();
	enum { IDD = IDD_TX99_TOOLTIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();
protected:
    afx_msg void OnBnClickedOk();
    afx_msg void OnPaint();
private:
    CString m_strText;
public:
	/**
	*@brief 간단한 툴팁
	*@param CString strTooltip
	*@param CPoint point
	*/
    void SetSimpleTooltip(CString strTooltip, CPoint point);    
private:
    int m_nTextWidth;
    int m_nTextHeight;
    int m_nMargin;
private:
	/**
	*@brief 윈도우 계산기
	*/
    void CalculateWindowPos();
private:
    CBrush m_brush;
    CPen   m_pen;
    CFont  m_font;

private:
    CPoint m_mousePt;
};

typedef std::shared_ptr<TxTooltipView> TxTooltipViewPtr;