/**
 * @file NumberEdit.h
 * @brief ZNumberEdit 선언 파일
 * @author 
 * @date 2011.05.18
 * @remark
 */


#pragma once




/**
 * @brief 숫자(실수 포함)만 입력받을 수 있는 Edit control class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.18
 */
class AFX_EXT_CLASS ZNumberEdit : public CEdit
{
	DECLARE_DYNAMIC(ZNumberEdit)

public:
	ZNumberEdit();
	virtual ~ZNumberEdit();

protected:
	DECLARE_MESSAGE_MAP()


public :
	void EnableRealNumber(bool bReal);
    void EnableMinMax(double a_dMinValue, double a_dMaxValue);
    
protected :
	bool m_bReal;
    bool m_bRange;

    double m_dMinValue;
    double m_dMaxValue;
	//bool m_bComma;

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


