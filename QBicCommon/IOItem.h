#pragma once
/**
*@brief KIO아이템
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KIOItem
{
public:
	KIOItem(void);
	KIOItem(const KIOItem& item);
	~KIOItem(void);
    
	KIOItem& operator=(const KIOItem& item);

public :
	/**
	*@brief 결과값을 INT형으로
	*@return int형으로 변환된 값 추출
	*/
	int     GetValueToInt() const;
	/**
	*@brief 결과값을 __int64형으로
	*@return __int64형으로 변환된 값 추출
	*/
	__int64 GetValueAsInteger64() const;
		/**
	*@brief 결과값을 double형으로
	*@return double형으로 변환된 값 추출
	*/
	double  GetValueAsDouble(void) const;
		/**
	*@brief 결과값을 LPCTSTR형으로
	*@return LPCTSTR형으로 변환된 값 추출
	*/
	LPCTSTR GetValueAsString(void) const;

	/**
	 * @brief Item의 값을 설정한다.
	 * @param[in] nValue - Integer형의 value
	 */
	void SetValue(int nValue);
	/**
	 * @brief Item의 값을 설정한다.
	 * @param[in] nValue - 64bit Integerg형의 value
	 */
	void SetValue(__int64 nValue);
	/**
	 * @brief Item의 값을 설정한다.
	 * @param[in] dValue - Double형의 Value
	 */
	void SetValue(double dValue);
	/**
	 * @brief Item의 값을 설정한다.
	 * @param[in] strValue - 문자열 형의 Value
	 */
	void SetValue(LPCTSTR strValue);
	/**
	 * @brief Item의 값을 설정한다.
	 * @param[in] dValue - Double형의 Value(15자리까지)
	 */
	void SetValueDetails(double dValue);


private :
	CString m_strValue;
};
