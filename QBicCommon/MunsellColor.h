#pragma once
/**
*@brief 먼셀 표색
*@details 먼셀 표색
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KMunsellColor
{
private:
    std::vector<COLORREF> m_vecMunSell20Color;
    int  m_nStartColorIndex;
    bool m_bComplementary;

public:
    KMunsellColor(int nStartColorIndex = 0, bool bComplementary = false);
    ~KMunsellColor(void);

public:
	/**
	*@brief 색 입력
	*@return 인덱스 값에 따른 MunSell 반환
	*@param int a_nNoIndex		Nol인덱스
	*/
    COLORREF GetColor(int a_nNoIndex);
};

