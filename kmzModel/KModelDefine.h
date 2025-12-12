#pragma once

#include <tchar.h>
#pragma once

#include <string>
#include <boost/algorithm/string.hpp> 
#include <atlcoll.h>
#include <atlstr.h>

namespace std 
{
    typedef std::basic_string<TCHAR> tstring;
}

#include "../QBicDataStructure/QBicDataStructureDefine.h"

#define LinkIDType     Integer
#define NodeIDType     Integer
#define ZoneIDType     Integer

#define CAtlODValueMap CAtlMap   <KMultiID,  Double,  KMutiIDTraits>
#define CAtlODTypeMap  CAtlMap   <KMultiID,  Integer, KMutiIDTraits>

#define CAtlZnMap      CAtlMap   <Integer, Double>
#define CAtlZnArray    CAtlArray <Integer>

#define CAtlODValuesMap CAtlMap  <KMultiID, KDoubleRecord*, KMutiIDTraits>

#define CAtlDestinationNodeArray  CAtlArray <Integer>

#include "../QBicDataStructure/KODKey.h"
#include "../QBicDataStructure/KODKeyDoubleRecords.h"
#include "../QBicDataStructure/KValueList.h"
#include "../QBicDataStructure/KSDoubleRecordArray.h"
#include "../QBicDataStructure/KIDKeyDoubleRecords.h"


#include "KMultiID.h"


