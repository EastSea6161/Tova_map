#pragma once

#include <string>
#include <tchar.h>
#include <atlstr.h>
//typedef std::basic_string<TCHAR> tstring;
namespace std 
{
	typedef std::basic_string<TCHAR> tstring;
}

#define I_ZONEID     int
#define D_VALUE      double
#define CAtlODMap    CAtlMap  <KODKEY, D_VALUE, KODKeyTraits>
#define CAtlZnMap    CAtlMap  <I_ZONEID, D_VALUE>
#define CAtlZnArray  CAtlArray<I_ZONEID>



#include "Klog.h"
#include "KEnumInModel.h"
#include "KODKey.h"
#include "KConstModel.h"
#include "KDistributionData.h"
#include "KModelUtility.h"
#include "KParseVariableInfo.h"
#include "KDoubleRecord.h"

