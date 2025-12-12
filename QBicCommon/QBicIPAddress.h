#pragma once
/**
*@brief IP 번호 확인
*@details 주소관련 번호 확인
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicIPAddress
{
public:
    QBicIPAddress() {};
    virtual ~QBicIPAddress(){};
public:
	/**
	*@brief 물리적 주소 확인
	*@details MAC 물리적주소 확인(12글자)
	*@param CStringArray& ar_strArrMacAddress 물리적 주소
	*@return 정확하지 않다면 false를, 정확하다면 ture를 반환
	*/
    bool GetEthernetMacAddress(CStringArray& ar_strArrMacAddress);
};