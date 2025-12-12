#include "KModelUtilityZone.h"

KModelUtilityZone::KModelUtilityZone(void)
{
}

KModelUtilityZone::~KModelUtilityZone(void)
{
}

Double KModelUtilityZone::GetSumOfData(const CAtlZnMap& _cmData)
{    
    CAtlString tLogMsg;
    
    size_t nCount = _cmData.GetCount();

    Double rSum = 0.0;
    POSITION pos = _cmData.GetStartPosition();

    while (NULL != pos)
    {
        rSum += _cmData.GetNextValue(pos);
    }

    return rSum;
}

Double KModelUtilityZone::GetSumOfData(const std::map<Integer, Double>& data)
{
    Double rValue = 0.0;
    std::map<Integer, Double>::const_iterator iter = data.begin();
    std::map<Integer, Double>::const_iterator end  = data.end();

    for (; iter != end; ++iter)
    {
        rValue += iter->second;
    }

    return rValue;
}

void KModelUtilityZone::ScaleData( std::map<Integer, Double>& data, Double scaleValue )
{
    std::map<Integer, Double>::iterator iter = data.begin();
    std::map<Integer, Double>::iterator end  = data.end();

    while(iter != end)
    {
        iter->second = iter->second * scaleValue;
        ++iter;
    }
}

bool KModelUtilityZone::GetRefZoneValue( CAtlZnMap& _cmZone, 
                                         Integer    _zoneID, 
                                         Double&    rValue
                                        )
{
    CAtlZnMap::CPair* pair = _cmZone.Lookup(_zoneID);

    if ( NULL != pair )
    {
        rValue = pair->m_value;
    }
    else
    {
        return false;
    }

    return true;
}

bool KModelUtilityZone::GetRefZoneValue( const std::map<Integer, Double>& data, 
                      Integer    _zoneID, 
                      Double&    rValue
                    )
{
    std::map<Integer, Double>::const_iterator iter = data.find(_zoneID);
    if (iter == data.end())
    {
        return false;
    }

    rValue = iter->second;

    return true;
}