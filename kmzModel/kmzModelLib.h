#pragma once

#include <string>
#include <log4cxx\log4cxx.h>
#include <muParser\muParser.h>

#ifdef DEBUG
	#ifdef UNICODE
		#pragma comment( lib, "muParserUD.lib"  )
	#else
		#pragma comment( lib, "muParserD.lib"   )
	#endif
#else
	#ifdef UNICODE
		#pragma comment( lib, "muParserU.lib"   )
	#else
		#pragma comment( lib, "muParser.lib"    )
	#endif
#endif


#include <tchar.h>
typedef std::basic_string<TCHAR> tstring;

#include "KParser.h"
#include "KParsingCalculator.h"
#include "KLogPropertyConfigurator.h"
