#pragma once
#include "DefineNamesDefaultPara.h"
class KDBaseVersion_P018
{
public:
	KDBaseVersion_P018(void);
	~KDBaseVersion_P018(void);

	static void ProjectDBUpgrade(KDBaseConPtr spDBaseConnection);
		static void TableDefaultIntermodalPCEMode(KDBaseConPtr spDBaseConnection);

};

