#pragma once
#ifdef DEBUG
	#pragma comment( lib, "log4cxxD.lib" )
#else
	#pragma comment( lib, "log4cxx.lib"  )
#endif

#pragma warning ( disable: 4231 ) 
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

#include "kmzModelLib.h"

class KLogPropertyConfigurator
{
public:
	KLogPropertyConfigurator(void);
	~KLogPropertyConfigurator(void);

	static void initialize();
};
