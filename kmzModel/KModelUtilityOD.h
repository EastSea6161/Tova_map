#pragma once

//#include "Klog.h"
#include "KModelDefine.h"
#include "KEnumInModel.h"
#include "KConstModel.h"
#include "KMultiID.h"

class KModelUtilityOD
{
public:
    KModelUtilityOD(void);
    ~KModelUtilityOD(void);

    static void CopyODData(CAtlODValueMap& _cmOriginOD, CAtlODValueMap& _cmDestinationOD);
    static void   SetODValue   ( CAtlODValueMap& _cmROD, 
                                 Integer        _originID, 
                                 Integer        _destinationID, 
                                 Double         _value
                               );	

    static void   SetODValue   ( std::map<KODKey, Double>& _smROD, 
                                 Integer        _originID, 
                                 Integer        _destinationID, 
                                 Double         _value
                               );	

    static bool   GetRefODValue( CAtlODValueMap& _cmOD, 
                                 Integer        _originID, 
                                 Integer        _destinationID, 
                                 Double&        rValue
                               );
   

    static Double GetODValue (CAtlODValueMap& _cmOD, KMultiID _key);
    static Double GetODValue (std::map<KODKey, Double>& _smOD, KODKey _key);
};
