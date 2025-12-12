#pragma once

//^#include "DBaseConnector.h"
/* Forward declarations */

enum KEMAddFieldType
{
	KEMAddFieldNormal    = 0,
	KEMAddFieldMode      = 1,
	KEMAddFieldPurpose   = 2,
	KEMAddFieldZone      = 3
};

enum KEMIODataType;
class KExistNameChecker;
class KIOTable;
class KIOColumn;
class KMode;
class KPurpose;
class KTarget;

const int COLUMN_MODE_NORMAL = 0;
const int COLUMN_MODE_CODE = 1;

class KTableAddFieldInfo
{
public:
	KTableAddFieldInfo(void);
	~KTableAddFieldInfo(void);

public:
 	static bool AddField       (KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, bool bDefault = false);
	static bool AddModeField   (KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KMode* a_pMode, bool bDefault = false);
	static bool AddPurposeField(KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KPurpose* a_pPurpose, bool bDefault = false);
	static bool AddZoneField   (KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KPurpose* a_pPurpose, CString a_strPAFlag, bool bDefault = false);

	static void UpdateVehicleModel	   (KTarget* a_pTarget, CString a_strColumnName, int a_nModeID);

	static bool AddModeField (KIOTable* a_pIOTable, KIOColumns& a_oAddColumns, KPurpose* a_pPurpose, bool bDefault = false);
	static bool AddChainField(KIOTable* a_pIOTable, KIOColumns& a_oAddColumns, KPurpose* a_pPurpose, bool bDefault = false);

private:
	static void UpdateModeChoiceModel  (KDBaseConPtr spDBaseConnection, KMode* a_pMode, KIOColumn& a_oIOColumn);
	static void UpdateDistributionModel(KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumn& a_oIOColumn);
	static void UpdateGenerationModel  (KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, CString &a_strPAFlag, KIOColumn& a_oIOColumn);
	
	static void UpdateItemLogitModel   (KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumns& a_oIOColumns);	
	static void UpdateChainModel       (KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumns& a_oIOColumns);
};

