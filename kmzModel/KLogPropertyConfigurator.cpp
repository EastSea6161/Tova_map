#include "KLogPropertyConfigurator.h"

KLogPropertyConfigurator::KLogPropertyConfigurator(void)
{
}

KLogPropertyConfigurator::~KLogPropertyConfigurator(void)
{
}

void KLogPropertyConfigurator::initialize()
{
	PropertyConfigurator::configure(log4cxx::File(_T("log4cxx.conf")));
	LoggerPtr rootlog = Logger::getRootLogger();
}