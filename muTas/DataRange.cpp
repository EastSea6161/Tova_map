#include "StdAfx.h"
#include "DataRange.h"

KDataRange::KDataRange()
{
    dZ = 1.28;
}

KDataRange::~KDataRange( void )
{

}

std::vector<TRangeRecord> KDataRange::GetRange( std::map<KODKey, double> a_mapValues, int a_nDevideCount, double dBasicRange )
{
    size_t nDataCount= a_mapValues.size();

    std::map<KODKey, double>::iterator iter = a_mapValues.begin();
    std::map<KODKey, double>::iterator end  = a_mapValues.end();

    double dSum = 0.0;
    double dMinValue = iter->second;
    double dMaxValue = iter->second;

    while (iter != end)
    {
        double& dValue = iter->second;
        dSum += dValue;

        if (dMinValue > dValue)
        {
            dMinValue = dValue;
        }

        if (dMaxValue < dValue)
        {
            dMaxValue = dValue;
        }

        ++iter;
    }

    double dAverage = dSum / nDataCount;

    double dSumDeviationSquare = 0.0;
    iter = a_mapValues.begin();
    while (iter != end)
    {
        dSumDeviationSquare += pow((iter->second - dAverage), 2.0);
        ++iter;
    }

    // 표준 편차
    double dStandardDeviation = pow(dSumDeviationSquare / nDataCount, 0.5);

    double dMinRange = dAverage - dZ * dStandardDeviation / pow(nDataCount, 0.5);
    double dMaxRange = dAverage + dZ * dStandardDeviation / pow(nDataCount, 0.5);
    
    std::vector<TRangeRecord> vecRangeRecord;

    return vecRangeRecord;
}