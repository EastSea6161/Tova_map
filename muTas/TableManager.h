
#pragma once

#include "stdafx.h"

class KScenario;
class KTarget;
class KIOTable;
class KIOColumn;

using namespace std;

struct TColumnInfo
{
	KIOColumn* pIOColumn;
	CString    strDisplayName;
	CString    strCaptionName;
};

struct TTargetTable
{
	KScenario* pScenario;
	KTarget*   pTarget;
	KIOTable*  pIOTable;
	vector<TColumnInfo> vecColumnInfo;
};


struct TUserColumnInfo
{
	CString    strUserFieldName;
	bool       bFunction;
	CString    strCopyFieldName;
	KIOColumn  oIOColumnUpdate;
	BOOL       bNew;
};