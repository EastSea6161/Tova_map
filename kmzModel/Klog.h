#pragma once

#ifdef _DEBUG
#ifdef _UNICODE
#pragma comment( lib, "log4cxxD.lib"  )
#else
#pragma comment( lib, "log4cxxD.lib"   )
#endif
#else
#ifdef _UNICODE
#pragma comment( lib, "log4cxx.lib"   )
#else
#pragma comment( lib, "log4cxx.lib"    )
#endif
#endif

#pragma warning ( disable: 4231 ) 
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

class KLog
{
private:
	KLog(void);
public:
	~KLog(void);

private:
	static bool bIsRootLoggerInitialed;
	static LoggerPtr rootLogger;
public:
	static LoggerPtr GetRootLogger();
	static bool isDebugEnabled();

private:
	static bool bIsNetworkLoggerInitialed;
	static LoggerPtr networkLogger;
public:
	static LoggerPtr GetNetworkLogger();
};

#ifndef KLogNetwork
#define KLogNetwork(MSG)             LOG4CXX_DEBUG(KLog::GetNetworkLogger(), MSG)
#endif

#ifndef KLogDebug
#define KLogDebug(MSG)             LOG4CXX_DEBUG(KLog::GetRootLogger(), MSG)
#endif

#ifndef KLogDebugStartMsg
#define KLogDebugStartMsg()        LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("Start..."))
#endif

#ifndef KLogDebugEndMsg
#define KLogDebugEndMsg()          LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("End...^^"))
#endif

#ifndef KLogDebugReturnMsg
#define KLogDebugReturnMsg()          LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("Return..."))
#endif

#ifndef KLogDebugCommandStartMsg
#define KLogDebugCommandStartMsg() LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("------------------------ Start..."))
#endif

#ifndef KLogDebugCommandEndMsg
#define KLogDebugCommandEndMsg()   LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("^^^^^^^^^^^^^^^^^^^^^^^^ End...^^"))
#endif

#ifndef KLogDebugVisitor
#define KLogDebugVisitor()         LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("--------Visitor--------"))
#endif

#ifndef KLogDebugException
#define KLogDebugException()       LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("Exception.. !!!"))
#endif

#ifndef KLogDebugWarning
#define KLogDebugWarning()         LOG4CXX_DEBUG(KLog::GetRootLogger(), _T("Warning.. !!!"))
#endif