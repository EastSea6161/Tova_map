#include "KLog.h"

bool      KLog::bIsRootLoggerInitialed = false;
LoggerPtr KLog::rootLogger    = NULL;

bool      KLog::bIsNetworkLoggerInitialed = false;
LoggerPtr KLog::networkLogger    = NULL;

KLog::KLog(void)
{
	
}

KLog::~KLog(void)
{
}

LoggerPtr KLog::GetRootLogger()
{	
	if(!bIsRootLoggerInitialed)
	{
		bIsRootLoggerInitialed = true;
		PropertyConfigurator::configure(log4cxx::File("log4cxx.conf"));
		LoggerPtr rootlog = Logger::getRootLogger(); 
		
		rootLogger = Logger::getLogger("");
		
		setlocale(LC_ALL, "");
	}	
	
	return rootLogger;
}

bool KLog::isDebugEnabled()
{
	if ( NULL != rootLogger )
	{
		return rootLogger->isDebugEnabled();
	}

	return false;
}

LoggerPtr KLog::GetNetworkLogger()
{	
	if(!bIsNetworkLoggerInitialed)
	{
		bIsNetworkLoggerInitialed = true;
		
		networkLogger = Logger::getLogger("kmz.network");

		setlocale(LC_ALL, "");
	}	

	return networkLogger;
}
