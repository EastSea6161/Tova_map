#pragma once

#include "InterFunctionBase.h"

/* Forward declarations */
class KIOColumns;
class KInterFunctionUserEXP;


class KInterFunctionUser : public KInterFunctionBase
{
public:
	KInterFunctionUser(void);
	KInterFunctionUser(const KInterFunctionUser& r);
	~KInterFunctionUser(void);

public:
	KInterFunctionUser& operator=(const KInterFunctionUser& r);

	KInterFunctionUserEXP* Function(int nCode);

	void Function(int nCode, KInterFunctionUserEXP* pFunction);

	void GetArgumentInfo(CStringArray& strOutArray, KIOColumns& fixedLinkColumns, KIOColumns& userAddLinkColumns);

protected :
	std::map<int, KInterFunctionUserEXP*> m_FunctionMap;

	void clearFunctionMap(void);
};

