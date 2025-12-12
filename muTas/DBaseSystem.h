#pragma once

#include "Target.h"

class KDBaseSystem
{
public:
	KDBaseSystem(void);
	~KDBaseSystem(void);

	static void GetSystemType(KTarget* a_pTarget, int& ar_nSystemType);

};

