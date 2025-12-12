//^^ #include "IOColumn.h"
#include "Mode.h"

struct TGenerationPathInfo
{
	KIOColumn* pColumn;
	KMode*	   pMode;
	CString    strModeName;
	int        nBus;		// 0 : NO , 1:Yes
	int        nSubway;		// 0 : NO , 1:Yes
	int        nExclusive;  // 0 : NO , 1:Yes
};


#pragma once
class KGenerationBusPathInfo
{
public:
	KGenerationBusPathInfo(void);
	~KGenerationBusPathInfo(void);

public:

};

