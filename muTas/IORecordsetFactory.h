#pragma once

//^^ #include "IOCommons.h"

/* Forward declarations */
class KIORecordset;
class KIOTable;

class KIORecordsetFactory
{
public:
	KIORecordsetFactory(void);
	~KIORecordsetFactory(void);

public :
	static KIORecordset* CreateRecordset(KIOTable* pTable, KEMIORecordsetType emRecordsetType);
};

