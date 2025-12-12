#pragma once
/**
*@brief 급간 정보
*@details 최소값, 최대값 초기화
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicDevideInfo
{
public:
    QBicDevideInfo() {
        m_dMin = 0;
        m_dMax = 0;
    }
    ~QBicDevideInfo(){}

public:
    std::vector<double> m_vecDevide;
public:
    double m_dMin;
    double m_dMax;
};
/**
*@brief 급간 정보
*@details 편집기능	
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicDevideData 
{
public:
    QBicDevideData();
    ~QBicDevideData();

private:
    std::map<double, int> m_mapValue;

public:
	/**
	*@brief 데이터 추가
	*@param double dValue	벨류값
	*/
    void AddData(double dValue) {
        int& nCnt = m_mapValue[dValue]; {
            nCnt++;
        }
    }

public:
	/**
	*@brief 급간 분배
	*@param int nGrade	등급
	*@param int nDigit	숫자
	*@return 최소값, 최대값, 벡터를 포함한 정보 반환
	*/
    QBicDevideInfo Proportion(int nGrade, int nDigit);
	/**
	*@brief 
	*@param int nGrade	등급
	*@param int nDigit	숫자
	*@return 최소값, 최대값, 벡터를 포함한 정보 반환
	*/
    QBicDevideInfo Devide(int nGrade, int nDigit);
};