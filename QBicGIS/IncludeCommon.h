#pragma once

#include "../QBicCommon/IncludeQBicCommon.h"

//declspec( __dllimport ) extern void MyClass::myStaticVariable;

#ifndef BoostDynamicCast
#define BoostDynamicCast std::dynamic_pointer_cast
#endif
