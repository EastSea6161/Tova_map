#pragma once

#include "model_common.h"

class KInterFunctionBase
{
public:
	KInterFunctionBase(KEMInterModalFunction emFunctionType);
	KInterFunctionBase(const KInterFunctionBase& r);
	virtual ~KInterFunctionBase(void) = 0;

public:
	KInterFunctionBase& operator=(const KInterFunctionBase& r);


	KEMInterModalFunction FunctionType(void);

protected:
	KEMInterModalFunction m_emFunctionType;
};