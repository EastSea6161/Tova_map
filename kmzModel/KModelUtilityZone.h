#pragma once

//#include "Klog.h"
#include "KModelDefine.h"
#include "KEnumInModel.h"
#include "KConstModel.h"
#include "KMultiID.h"
#include <algorithm>
#include <map>
#include <numeric>

class KModelUtilityZone
{
public:
    KModelUtilityZone(void);
    ~KModelUtilityZone(void);

    static Double GetSumOfData ( const CAtlZnMap&  _cmData );
    static Double GetSumOfData ( const std::map<Integer, Double>& data );
    static void   ScaleData    ( std::map<Integer, Double>& data, Double scaleValue );

    static bool GetRefZoneValue( CAtlZnMap& _cmZone, 
                                 Integer    _zoneID, 
                                 Double&    rValue
                               );

    static bool GetRefZoneValue( const std::map<Integer, Double>& data, 
                                 Integer    _zoneID, 
                                 Double&    rValue
                                );
};
