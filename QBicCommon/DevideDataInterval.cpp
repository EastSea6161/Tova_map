#include "StdAfx.h"
#include "DevideDataInterval.h"
#include "ExceptionDefine.h"

KDevideDataInterval::KDevideDataInterval()
{
    InitialData();
}

KDevideDataInterval::~KDevideDataInterval( void )
{

}

void KDevideDataInterval::InitialData()
{
    m_vecData.clear();

    m_dSum         = 0.0;
    m_dAverage     = 0.0;
    m_dMinValue    = 0.0;
    m_dMaxValue    = 0.0;
    m_dStdDev      = 0.0;
    m_dVariance    = 0.0;
    m_dStdErr      = 0.0;

    m_dMinConfidenceInterval = 0.0;
    m_dMaxConfidenceInterval = 0.0;

    m_nLastDeviedCount = -1;
}

void KDevideDataInterval::AddData( double a_dValue )
{
    m_vecData.push_back(a_dValue);
}

void KDevideDataInterval::SetDataCalculate( std::vector<double>& a_vecData )
{   
    m_vecData.clear();
    size_t nxCount = a_vecData.size();

    m_vecData.reserve(nxCount);
    m_vecData.assign(a_vecData.begin(), a_vecData.end());

    Calculate();
}

void KDevideDataInterval::Calculate()
{
    size_t nxCount = m_vecData.size();

    if (nxCount < 1)
    {
        InitialData();
		return;
    }

    double dMin = 1.0E100;
    double dMax = dMin * -1.0;
    double dSum = 0.0;

    // Step 1. Min, Max, Sum
    for (size_t i=0; i<nxCount; i++)
    {
        double dValue = m_vecData[i];

        if (dValue < dMin)
        {
            dMin = dValue;
        }

        if (dValue > dMax)
        {
            dMax = dValue;
        }

        dSum += dValue;
    }

    // Step 2. 평균
    double dAverage  = 0.0;
    dAverage  = dSum / nxCount;

    // Step 3. 표준 편차, 분산 계산
    double dStdDev   = 0.0;
    double dVariance = 0.0;
    double dTempDeviationSquare = 0.0;
    for (size_t i=0; i<nxCount; i++)
    {
        double dValue = m_vecData[i];

        dTempDeviationSquare += pow(dValue - dAverage, 2.0);
    }

    dVariance = dTempDeviationSquare / nxCount; 
    dStdDev   = sqrt(dVariance);

    // Step 4. 98%(2.33), 95%(1.96) 신뢰도 범위 계산
    //double dMinConfidenceInterval = dAverage - 1.28 * ( dStdDev / sqrt((double)nxCount));
    //double dMaxConfidenceInterval = dAverage + 1.28 * ( dStdDev / sqrt((double)nxCount));
    double dMinConfidenceInterval = dAverage - 2.33 * dStdDev;
    double dMaxConfidenceInterval = dAverage + 2.33 * dStdDev;

    // Step 5. 표준 오차
    double dStdErr = 0.0;
    dStdErr = dStdDev/sqrt( (double)nxCount );

    m_dSum         = dSum;
    m_dAverage     = dAverage;
    m_dMinValue    = dMin;
    m_dMaxValue    = dMax;
    m_dStdDev      = dStdDev;
    m_dVariance    = dVariance;
    m_dStdErr      = dStdErr;

    if (dMinConfidenceInterval < dMin) {
        m_dMinConfidenceInterval = dMin;
    } else {
        m_dMinConfidenceInterval = dMinConfidenceInterval;
    }

    if (dMaxConfidenceInterval > dMax) {
        m_dMaxConfidenceInterval = dMax;
    } else {
        m_dMaxConfidenceInterval = dMaxConfidenceInterval;
    }    
}

size_t KDevideDataInterval::GetDataCount()
{
    return m_vecData.size();
}

double KDevideDataInterval::GetSum()
{
    return m_dSum;
}

double KDevideDataInterval::GetAverage()
{
    return m_dAverage;
}

double KDevideDataInterval::GetMinValue()
{
    return m_dMinValue;
}

double KDevideDataInterval::GetMaxValue()
{
    return m_dMaxValue;
}

double KDevideDataInterval::GetStdDev()
{
    return m_dStdDev;
}

double KDevideDataInterval::GetVariance()
{
    return m_dVariance;
}

double KDevideDataInterval::GetStdErr()
{
    return m_dStdErr;
}

std::vector<TDevideDataInterval> KDevideDataInterval::GetDevide( int a_nDevideCount, int a_nDigit )
{
    const double Epsilon = 1.0E-7; // 1.0E-4
    std::vector<TDevideDataInterval> vecOut;

    int nDigit = a_nDigit;

    if (a_nDevideCount < 1)
    {
        ThrowException(_T("Devide by 0"));
    }

    if (a_nDevideCount == 1)
    {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);
        m_nLastDeviedCount = -7;
        return vecOut;
    }

    double dInterval = (m_dMaxValue - m_dMinValue) / (a_nDevideCount);

    if (dInterval < Epsilon) {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);

        m_nLastDeviedCount = -7;
        return vecOut;
    }

    // 최소 간격 
    double dIntervalDigit = pow(10.0, nDigit);

    while (dInterval < dIntervalDigit)
    {
        nDigit--;
        dIntervalDigit = pow(10.0, nDigit);
    }

    int nShare = (int) (dInterval / dIntervalDigit);
    dInterval  = nShare * dIntervalDigit;

    double  dPreMaxValue = m_dMinValue;

    // 가정) 2개 이상으로 분할 : 1개로 분할은 앞에서 해결
    for (int i=0; i<a_nDevideCount; i++)
    {
        TDevideDataInterval oDevideDataInterval;

        if (i==0)
        {
            oDevideDataInterval.TMinValue = m_dMinValue;
            double dRemainder             = fmod(m_dMinValue, pow(10.0, nDigit));            
            dPreMaxValue                  = m_dMinValue + dInterval - fabs(dRemainder);
            oDevideDataInterval.TMaxValue = dPreMaxValue;            
        }
        else
        {            
            double dTMinValue = dPreMaxValue /*pow(10.0, nDigit-1)*/;
            double dTMaxValue = dPreMaxValue + dInterval;

            oDevideDataInterval.TMinValue = dTMinValue;
            if (dTMaxValue >= m_dMaxValue)
            {                
                oDevideDataInterval.TMaxValue = m_dMaxValue;                 
            }
            else
            {
                oDevideDataInterval.TMaxValue = dTMaxValue;
            }

            //dPreMaxValue = dTMaxValue;
			dPreMaxValue = oDevideDataInterval.TMaxValue;
        }

        if (i == a_nDevideCount -1)
        {
            oDevideDataInterval.TMaxValue = m_dMaxValue; 
        }

        vecOut.push_back(oDevideDataInterval);
    }

    m_nLastDeviedCount = nDigit;

    return vecOut;
}

std::vector<TDevideDataInterval> KDevideDataInterval::GetOptimizeDevide( int a_nDevideCount, int a_nDigit )
{
    const double Epsilon = 1.0E-7; // 1.0E-4
    std::vector<TDevideDataInterval> vecOut;

    int nDigit = a_nDigit;

    if (a_nDevideCount < 1) {
        ThrowException(_T("Devide by 0"));
    }

    if (a_nDevideCount == 1) {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);
        m_nLastDeviedCount = -7;
        return vecOut;
    }

    double dInterval = (m_dMaxValue - m_dMinValue) / (a_nDevideCount);

    if (dInterval < Epsilon) {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);
        m_nLastDeviedCount = -7;
        return vecOut;
    }

    if (a_nDevideCount < 3) {
        return GetDevide(a_nDevideCount, a_nDigit);
    }

    // 3개 이상의 경우
    // 최소 간격 
    bool bFxFirst     = false;
    bool bFxLast      = false;
    int  nSubtraction = 2;

    if (fabs(m_dMinConfidenceInterval - m_dMinValue) < Epsilon) {
        nSubtraction--;
        bFxFirst = true;
    }

    if (fabs(m_dMaxConfidenceInterval - m_dMaxValue) < Epsilon) {
        nSubtraction--;
        bFxLast = true;
    }

    dInterval = (m_dMaxConfidenceInterval - m_dMinConfidenceInterval) / (a_nDevideCount-nSubtraction);
    double dIntervalDigit = pow(10.0, nDigit);

    while (dInterval < dIntervalDigit) {
        nDigit--;
        dIntervalDigit = pow(10.0, nDigit);
    }
	
    int nShare = (int) (dInterval / dIntervalDigit);
    dInterval  = nShare * dIntervalDigit;

    double  dPreMaxValue = m_dMinValue;

    for (int i=0; i<a_nDevideCount; i++)
    {
        TDevideDataInterval oDevideDataInterval;

        if (i==0) {
            oDevideDataInterval.TMinValue = m_dMinValue;
            double dRemainder = 0.0;
            if (bFxFirst) {
                dRemainder   = fmod(m_dMinValue + dInterval, pow(10.0, nDigit));
                dPreMaxValue = m_dMinValue + dInterval - fabs(dRemainder);
            } else {
                dRemainder   = fmod(m_dMinConfidenceInterval, pow(10.0, nDigit)); 
                dPreMaxValue = m_dMinConfidenceInterval - fabs(dRemainder);
            }
            oDevideDataInterval.TMaxValue = dPreMaxValue;            

        } else {            
            double dTMinValue = dPreMaxValue /*pow(10.0, nDigit-1)*/;
            double dTMaxValue = dPreMaxValue + dInterval;

            oDevideDataInterval.TMinValue = dTMinValue;
            if (dTMaxValue >= m_dMaxValue) {                
                oDevideDataInterval.TMaxValue = m_dMaxValue;                 
				dPreMaxValue = m_dMaxValue;
            } else {
                oDevideDataInterval.TMaxValue = dTMaxValue;
				dPreMaxValue = dTMaxValue;
            }
            //dPreMaxValue = dTMaxValue;
        }

        if (i == a_nDevideCount -1) {
            oDevideDataInterval.TMaxValue = m_dMaxValue; 
        }

        vecOut.push_back(oDevideDataInterval);
    }

    m_nLastDeviedCount = nDigit;
    return vecOut;
}

std::vector<TDevideDataInterval> KDevideDataInterval::GetOptimizeCntDevide(const int a_nDevideCount, int a_nDigit)
{
    const double Epsilon = 1.0E-7; // 1.0E-4
    std::vector<TDevideDataInterval> vecOut;

    int nDigit = a_nDigit;

    if (a_nDevideCount < 1) {
        ThrowException(_T("Devide by 0"));
    }

    if (a_nDevideCount == 1) {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);
        m_nLastDeviedCount = -7;
        return vecOut;
    }

    double dInterval = (m_dMaxValue - m_dMinValue) / (a_nDevideCount);

    if (dInterval < Epsilon) {
        TDevideDataInterval oDevideDataInterval;
        oDevideDataInterval.TMinValue = m_dMinValue;
        oDevideDataInterval.TMaxValue = m_dMaxValue;
        vecOut.push_back(oDevideDataInterval);
        m_nLastDeviedCount = -7;
        return vecOut;
    }

    if (a_nDevideCount < 3) {
        return GetDevide(a_nDevideCount, a_nDigit);
    }

    if (m_vecData.size() < 1) {
        return vecOut;
    }

    if (m_vecData.size() <= a_nDevideCount) {
        return GetDevide(a_nDevideCount, a_nDigit);
    }

    // 3개 이상의 경우
    // 최소 간격 
    // 최소 간격 
    std::sort(m_vecData.begin(), m_vecData.end(), Ascending_BaseData); /* SetDataCalculate 에서 한번 태울지 고민이다. */

    int nSizeVal = (int)m_vecData.size()-1;
    double dST(0);
    double dND(0);
    int    nST_Cls(0);
    int    nND_Cls(0);
    double dPre_ST(0);

    std::map<int, TDevideDataInterval> mapClass;

    /************************************************************************/
    /* 이 모듈의 주의 할 점은
       샘플데이터가 a_nDevideCount 보다 커야 한다.  */
    /************************************************************************/

    for (int i = a_nDevideCount; i >= 1; i--)
    {
        TDevideDataInterval oDevideDataInterval;

        if (1 == i) { // 첫 등급
            nND_Cls = (int)((double)nSizeVal / (double)a_nDevideCount);

            dST = m_dMinValue;
            //dND = m_vecData[nd_cls];
            dND = dPre_ST;

            oDevideDataInterval.TMinValue = dST;
            oDevideDataInterval.TMaxValue = dND;

            mapClass.insert(std::make_pair(i, oDevideDataInterval));
        }
        else if (a_nDevideCount == i) { // 맨 마지막 등급
            nST_Cls = nSizeVal - (int)((double)nSizeVal / (double)i);
            dST = m_vecData[nST_Cls];
            dND = m_dMaxValue + 0.5;

            if (true) {
                int nDigit = a_nDigit;
                double dIntervalDigit = pow(10.0, nDigit);

                while (dST < dIntervalDigit)
                {
                    nDigit--;
                    dIntervalDigit = pow(10.0, nDigit);
                }

                int nShare = (int)(dST / dIntervalDigit);
                dST = nShare * dIntervalDigit;
            }

            oDevideDataInterval.TMinValue = dST;
            oDevideDataInterval.TMaxValue = dND;

            mapClass.insert(std::make_pair(i, oDevideDataInterval));
        }
        else {
            nST_Cls = (int)((double)nSizeVal / ((double)a_nDevideCount / (double)(i - 1)));
            nND_Cls = (int)((double)nSizeVal / ((double)a_nDevideCount / (double)i));

            dST = m_vecData[nST_Cls];
            //dND = m_vecData[nd_cls];
            dND = dPre_ST;

            if (true) {
                int nDigit = a_nDigit;
                double dIntervalDigit = pow(10.0, nDigit);

                while (dST < dIntervalDigit)
                {
                    nDigit--;
                    dIntervalDigit = pow(10.0, nDigit);
                }

                int nShare = (int)(dST / dIntervalDigit);
                dST = nShare * dIntervalDigit;
            }

            oDevideDataInterval.TMinValue = dST;
            oDevideDataInterval.TMaxValue = dND;

            mapClass.insert(std::make_pair(i, oDevideDataInterval));
        }

        dPre_ST = dST;

        dST = 0.0;
        dND = 0.0;
        nST_Cls = 0;
        nND_Cls = 0;
    }

    // 정렬의 역할
    auto iter = mapClass.begin();
    auto itEd = mapClass.end();
    for (; iter != itEd; ++iter)
    {
        vecOut.push_back(iter->second);
    }

    m_nLastDeviedCount = nDigit;
    return vecOut;
}

std::vector<TDevideDataInterval> KDevideDataInterval::GetOptimizeCntDevideMedian(const int a_nDevideCount /*= 5*/, int a_nDigit /*= 2*/)
{
	const double Epsilon = 1.0E-7; // 1.0E-4
	std::vector<TDevideDataInterval> vecOut;

	if (m_vecData.size() < 1) {
		return vecOut;
	}

	int nDigit = a_nDigit; //자리수 (음수이면 소수점 단위)

	if (a_nDevideCount < 1) {
		ThrowException(_T("Devide by 0"));
	}

	//한개이면 그냥 가기!!!
	if (a_nDevideCount == 1) {
		TDevideDataInterval oDevideDataInterval;
		oDevideDataInterval.TMinValue = m_dMinValue;
		oDevideDataInterval.TMaxValue = m_dMaxValue;
		vecOut.push_back(oDevideDataInterval);
		m_nLastDeviedCount = -7;
		return vecOut;
	}

	//사이값이 너무 작은경우 
	double dInterval = (m_dMaxValue - m_dMinValue) / (a_nDevideCount);

	if (dInterval < Epsilon) {
		TDevideDataInterval oDevideDataInterval;
		oDevideDataInterval.TMinValue = m_dMinValue;
		oDevideDataInterval.TMaxValue = m_dMaxValue;
		vecOut.push_back(oDevideDataInterval);
		m_nLastDeviedCount = -7;
		return vecOut;
	}

	std::map<double,int> mapValueCnt;

	for (size_t i = 0; i < m_vecData.size(); i++)
	{
		double dValue = m_vecData[i];
		
		//반올림
		dValue = dValue + pow(10.0, nDigit) / 2;

		if (nDigit == 0)
			dValue = floor(nDigit);
		else if (nDigit < 0)
			dValue = floor((dValue * pow(10.0, abs(nDigit)))) / pow(10.0, abs(nDigit));
		else
			dValue = floor((dValue / pow(10.0, abs(nDigit)))) * pow(10.0, abs(nDigit));

		auto findValue = mapValueCnt.find(dValue);
		if (findValue == mapValueCnt.end())
		{
			mapValueCnt.insert(std::make_pair(dValue, 1));
		}
		else
		{
			findValue->second++;
		}
	}

	if (mapValueCnt.size() < a_nDevideCount)
	{
		return vecOut;
	}

	// 두개 이상일경우 나누기.... 흠냐 Loop를 돌아야 될거 같긴한다... 표준편차와 값들을 이용한 분할....

	int nOptinamCnt = (int)m_vecData.size()/a_nDevideCount;












	//두개면 가운데 값을 기준으로 반땅하기
	if (a_nDevideCount < 3) {
		return GetDevide(a_nDevideCount, a_nDigit);
	}

	
	if (m_vecData.size() <= a_nDevideCount) {
		return GetDevide(a_nDevideCount, a_nDigit);
	}

	// 3개 이상의 경우
	// 최소 간격 
	// 최소 간격 
	std::sort(m_vecData.begin(), m_vecData.end(), Ascending_BaseData); /* SetDataCalculate 에서 한번 태울지 고민이다. */

	int nSizeVal = (int)m_vecData.size() - 1;
	double dST(0);
	double dND(0);
	int    nST_Cls(0);
	int    nND_Cls(0);
	double dPre_ST(0);

	std::map<int, TDevideDataInterval> mapClass;

	return vecOut;
}

size_t KGetGrade::GetGrade( std::vector<TDevideDataInterval>& a_vecDevideDataInterval, double a_dValue )
{
    size_t nxCount = a_vecDevideDataInterval.size();
    if (nxCount == 0)
    {
        return 0;
    }

    size_t nxIndex = (nxCount - 1) / 3;
    return GetGrade(a_vecDevideDataInterval, a_dValue, nxIndex);
}

size_t KGetGrade::GetGrade( std::vector<TDevideDataInterval>& a_vecDevideDataInterval, double a_dValue, size_t a_nxIndex )
{
    size_t nxCount = a_vecDevideDataInterval.size();
    size_t nxMaxIndex = nxCount - 1;

    TDevideDataInterval& oDevideDataInterval = a_vecDevideDataInterval[a_nxIndex];

    if (a_nxIndex == 0)
    {
        if (oDevideDataInterval.TMinValue >= a_dValue)
        {
            return 0;
        }
    }

    if (a_nxIndex == nxMaxIndex)
    {
        if (oDevideDataInterval.TMaxValue <= a_dValue)
        {
            return nxMaxIndex;
        }
    }

    size_t nxIndex;
    // Forward - 2개씩 증가
    if (oDevideDataInterval.TMaxValue < a_dValue)
    {
        nxIndex = a_nxIndex + 2;
        if (nxIndex > nxMaxIndex)
        {
            nxIndex = nxMaxIndex;
        }

        return GetGrade(a_vecDevideDataInterval, a_dValue, nxIndex);
    }
    else
    {
        // Bingo
        if (oDevideDataInterval.TMinValue <= a_dValue)
        {
            return a_nxIndex;
        }
        else
        {
            nxIndex = a_nxIndex -1;
            if (nxIndex < 0)
            {
                nxIndex = 0;
            }

            return GetGrade(a_vecDevideDataInterval, a_dValue, nxIndex);
        }
    }

    return 0;
}
// size_t KGetGrade::GetGrade( std::vector<TDevideDataInterval>& a_vecDevideDataInterval, double a_dValue, size_t a_nxIndex, size_t a_nxPreIndex )
// {
//     size_t nxCount = a_vecDevideDataInterval.size();
//     if (a_nxIndex >= nxCount -1)
//         return nxCount -1;
// 
//     TDevideDataInterval& oDevideDataInterval = a_vecDevideDataInterval[a_nxIndex];
//     // Forward
//     if (oDevideDataInterval.TMinValue <= a_dValue)
//     {
//         if (oDevideDataInterval.TMaxValue >= a_dValue)
//         {
//             return a_nxIndex;
//         }
//         else
//         {
//             size_t nxIndex;
//             if (a_nxPreIndex > a_nxIndex)
//             {
//                 nxIndex = (a_nxPreIndex - a_nxIndex) / 2 + a_nxIndex;
//             }
//             else
//             {
//                 nxIndex = (nxCount - a_nxIndex) / 2 + a_nxIndex;
//             }
//             
//             return GetGrade(a_vecDevideDataInterval, a_dValue, nxIndex, a_nxIndex);
//         }
//     }
//     else //BackWard
//     {
//         size_t nxIndex = a_nxIndex / 2;
//         if (nxIndex == 0)
//             return 0;
//         return GetGrade(a_vecDevideDataInterval, a_dValue, nxIndex, a_nxIndex);
//     }
// 
//     return 0;
// }