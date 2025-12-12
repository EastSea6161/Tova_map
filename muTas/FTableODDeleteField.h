#pragma once

class KTarget;

class KFTableODDeleteField
{
public:
	KFTableODDeleteField(void);
	~KFTableODDeleteField(void);

public:
	static void DeleteModeODFieldLogic(KTarget* a_pTarget, CString a_strFieldName);
	static void DeleteChainODFieldLogic(KTarget* a_pTarget, CString a_strFieldName);
};

