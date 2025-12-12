#include "stdafx.h"
#include "QBicDevideData.h"
#include "QBicFormat.h"

QBicDevideData::QBicDevideData()
{
}

QBicDevideData::~QBicDevideData()
{
}

QBicDevideInfo QBicDevideData::Proportion( int nGrade, int nDigit )
{      
    QBicDevideInfo oInfo;
    if (m_mapValue.size() == 0)
        return oInfo;

    std::map<double, int> mapValue; 
    for (auto iter = m_mapValue.begin(); iter != m_mapValue.end(); ++iter) 
    {// 전체 데이터를 루핑하면서 신규 급간을 만든다.
        // 버림연산을 통해 왼쪽으로..     
        double dValue = iter->first;
        double dConv(0); {
            dConv = QBicFormat::UnitMinNum(dValue, nDigit);
        }

        mapValue.insert(std::make_pair(dConv, 1)); 
        //Debug : mapValue[dConv] += iter->second;
    }

    size_t nxCnt = mapValue.size();  // 전체 급간
    size_t nxRangeCnt(0), nx1thCnt(0); 
    {
        nxRangeCnt = nxCnt / nGrade; // 한개 급간에 포함될 수 있는 카운트
        if (nxRangeCnt <= 1) {
            return Devide(nGrade, nDigit);
        } else {
            int nReserve = (int)nxCnt - (int)(nxRangeCnt * nGrade);
            if (nReserve <= 0) {
                nx1thCnt = nxRangeCnt;
            } else {
                nx1thCnt = nxRangeCnt + (int)(nReserve * 0.5);
            }
        }
    }

    std::vector<double> vecDevide;
    int nLoop(1), 
        nFindGrade(0);

    bool bFirst(true);

    for (auto iter = mapValue.begin(); iter != mapValue.end(); ++iter) 
    {  
        double dValue = iter->first;         
        if (nLoop == 1) {// 급간의 시작(왼쪽)
            nFindGrade ++;
            if (vecDevide.size() != 0 && nFindGrade > nGrade) {
                break;
            }
            /*★*/vecDevide.push_back(dValue);       
        }

        nLoop++;

        if (nLoop > (int)nxRangeCnt) { //★초기화
            if (bFirst == true) {
                if (nLoop > (int)nx1thCnt) {
                    nLoop  = 1; 
                    bFirst = false;
                }
            } 
            else {
                nLoop = 1; 
            }            
        }
    }

    double dMin(0); {
        auto iter = m_mapValue.begin();
        dMin = iter->first;
    }

    double dMax(0); {
        auto iter = m_mapValue.rbegin(); 
        dMax = iter->first;
    }

    nxCnt = vecDevide.size();
    double dValue = QBicFormat::UnitMaxNum(dMax, nDigit); 
    double dTemp  = vecDevide[nxCnt-1];
    if (dValue > dTemp) {
        vecDevide.push_back(dValue);
    }    

    oInfo.m_dMin = dMin;
    oInfo.m_dMax = dMax;
    oInfo.m_vecDevide = vecDevide;

    return oInfo;
}

QBicDevideInfo QBicDevideData::Devide( int nGrade, int nDigit )
{
    QBicDevideInfo oInfo;
    if (m_mapValue.size() == 0)
        return oInfo;

    double dMin(0); {
        auto iter = m_mapValue.begin();
        dMin = iter->first;
    }

    double dMax(0); {
        auto iter = m_mapValue.rbegin(); 
        dMax = iter->first;
    }

    double dVMin = QBicFormat::UnitMinNum(dMin, nDigit);
    double dVMax = QBicFormat::UnitMaxNum(dMax, nDigit);

    double dIntv = (dVMax - dVMin) / nGrade; {
        dIntv = QBicFormat::UnitMaxNum(dIntv, nDigit);
    }

    std::vector<double> vecDevide;

    for (int i = 0; i < nGrade; i++) {
        double dValue = dVMin + dIntv * i;

        if (dValue < dVMax)
            vecDevide.push_back(dValue);
    }

    vecDevide.push_back(dVMax);

    oInfo.m_dMin = dMin;
    oInfo.m_dMax = dMax;
    oInfo.m_vecDevide = vecDevide;

    return oInfo;
}
