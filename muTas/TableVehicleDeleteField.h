#pragma once

class KTarget;

class KTableVehicleDeleteField
{
public:
	KTableVehicleDeleteField(void);
	~KTableVehicleDeleteField(void);

public:
	static void DeleteVehicleFieldLogic(KTarget* a_pTarget, CString a_strName);


};

