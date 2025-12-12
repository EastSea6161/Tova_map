/**
 * @file ColorRangeGen.h
 * @brief KColorRangeGen 선언
 * @author 
 * @date 2011.02.21
 * @remark
 */

#pragma once


/**
 * @brief 주어진색으로 부터 Color 값을 반환하는 클래스
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.02.21
 */
class AFX_EXT_CLASS KColorRangeGen
{
public:
	KColorRangeGen(void);
	virtual ~KColorRangeGen(void);

public :

	/**
	* @brief Color range의 시작 색상을 설정한다.
	* @param[in] clrStart - 색상
	*/
	void setStartColor(COLORREF clrStart);

	/**
	* @brief Color range의 종료 색상을 설정한다.
	* @param[in] clrStart - 색상
	*/
	void setEndColor(COLORREF clrEnd);

	/**
	* @brief Color range의 크기를 설정한다.
	* @param[in] nSize - 크기
	*/
	void setSize(int nSize);

	/**
	* @brief Color range를 생성한다.
	* @return bool - 수행 결과
	*/
	bool createColorRange(void);

	/**
	* @brief 다음 색상을 반환한다.
	* @return COLORREF - 색상
	*/
	COLORREF getNextColor(void);


protected :
	//IAlgorithmicColorRampPtr m_spColorRamp;
	COLORREF m_clrStart;
	COLORREF m_clrEnd;
	int m_nSize;
	int m_nCurrentIndex;

	std::vector<COLORREF> m_vecColor;
};
