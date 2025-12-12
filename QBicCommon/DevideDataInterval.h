#pragma once
/**
*@brief 데이타 간격
*@details 데이타의 간격 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TDevideDataInterval
{
public:
    TDevideDataInterval():TMinValue(0.0), TMaxValue(0.0)
    {   
    }
    ~TDevideDataInterval() {}
public:
    double TMinValue;
    double TMaxValue;
};
/**
*@brief 데이타 간격
*@details 데이타의 간격 설정
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KDevideDataInterval
{
private:
    std::vector<double> m_vecData;

public:
    KDevideDataInterval();
    ~KDevideDataInterval(void);
public:
	/**
	*@brief 초기 데이타 설정
	*/
    void InitialData();
	/**
	*@brief m_vecData에 a_dValue 추가
	*@param double a_dValue	double형 벨류
	*/
    void AddData(double a_dValue);
	/**
	*@brief 계산
	*@details 최소값, 최대값, 합 / 평균 / 표준 편차, 분산 계산 / 신뢰도 범위 계산 / 표준 오차
	*/
    void Calculate();
public:
	/**
	*@brief 계산하기 전 셋팅
	*@param std::vector<double>& a_vecData	벡터 데이타
	*@details 벡터 초기화 작업 및 시작, 끝 설정
	*/
    void SetDataCalculate(std::vector<double>& a_vecData);
	/**
	*@brief 분배 설정
	*@param int a_nDevideCount	최소 간격
	*@param int a_nDigit		분배할 수
	*@return 최소간격으로 설정 / 2개 이상 분할
	*/
    std::vector<TDevideDataInterval> GetDevide        (int a_nDevideCount = 5, int a_nDigit = 2);
	/**
	*@brief 분배 설정
	*@param int a_nDevideCount	최소 간격
	*@param int a_nDigit		분배할 수
	*@return 최소간격으로 설정 / 3개 이상인 경우
	*/
    std::vector<TDevideDataInterval> GetOptimizeDevide(int a_nDevideCount = 5, int a_nDigit = 2);
    std::vector<TDevideDataInterval> GetOptimizeCntDevide(const int a_nDevideCount = 5, int a_nDigit = 2);


	//시간날때마다 개발하기... 갯수로 인한 범위설정
	std::vector<TDevideDataInterval> GetOptimizeCntDevideMedian(const int a_nDevideCount = 5, int a_nDigit = 2);


    static bool Ascending_BaseData(double left, double right)
    {
        return left < right;
    }
private:
	int m_nLastDeviedCount;
public:
	/**
	*@brief 마지막 분배
	*/
	int LastDevideCount() { return m_nLastDeviedCount; } 

private:
    double m_dSum;
    double m_dAverage;
    double m_dMinValue;
    double m_dMaxValue;

    double m_dStdDev;
    double m_dVariance;
    double m_dStdErr;

    double m_dMinConfidenceInterval;
    double m_dMaxConfidenceInterval;

public:
	/**
	*@brief 데이터 개수
	*@return 데이터의 개수 반환
	*/
    size_t GetDataCount();
	/**
	*@brief 데이터 합
	*@return 데이터의 합 반환
	*/
    double GetSum();
	/**
	*@brief 데이터 평균
	*@return 데이터의 평균 반환
	*/
    double GetAverage();
	/**
	*@brief 데이터 최소값
	*@return 데이터의 최소값 반환
	*/
    double GetMinValue();
	/**
	*@brief 데이터 최대값
	*@return 데이터의 최대값 반환
	*/
    double GetMaxValue();
	/**
	*@brief 데이터 표준편차
	*@return 데이터의 표준편차 반환
	*/
    double GetStdDev();
	/**
	*@brief 데이터 분산
	*@return 데이터의 분산값 반환
	*/
    double GetVariance();
	/**
	*@brief 데이터 신뢰도 범위
	*@return 데이터 신뢰도 범위 반환
	*/
    double GetStdErr();

};
/**
*@brief 등급  추출
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KGetGrade
{
public:
    KGetGrade();
    ~KGetGrade();
public:
	/**
	*@brief 등급 추출
	*@param std::vector<TDevideDataInterval>& a_vecDevideDataInterval	
	*@param double a_dValue												더블형 벨류
	*@return 
	*/
    static size_t GetGrade( std::vector<TDevideDataInterval>& a_vecDevideDataInterval, double a_dValue );
private:
	/**
	*@brief 등급 추출
	*@param std::vector<TDevideDataInterval>& a_vecDevideDataInterval	
	*@param double a_dValue												더블형 벨류
	*@param size_t a_nxIndex											인덱스 크기
	*@return 
	*/
    static size_t GetGrade( std::vector<TDevideDataInterval>& a_vecDevideDataInterval, double a_dValue, size_t a_nxIndex );
};