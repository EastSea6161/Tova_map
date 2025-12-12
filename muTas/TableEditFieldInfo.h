#pragma once

enum KEMEditFieldType
{
	KEMEditFieldNormal    = 1,
	KEMEditFieldMode      = 2,
	KEMEditFieldPurpose   = 3,
	KEMEditFieldZone      = 4,
	KEMEditFieldCode      = 5,
	KEMEditFieldFVehicle  = 6
};

struct TDistributionModelKey 
{
	int     nObject_Group_Code;
	int     nDetail_Object_ID;
	CString strPurpose_OD_Column_Name;
};

struct TModeChoiceModelKey
{
	int     nMode_Group_Code;
	int     nDetail_Mode_ID;
	CString strMode_OD_Column_Name;
};

struct TGenerationModelKey
{
	int     nObject_Group_Code;
	int     nDetail_Object_ID;
	CString strZone_Column_Name;
	CString strPA_Flag;
};

struct TVehicleModelKey
{
	int		nFmode_id;
	CString strColumnName;
	int		nRunSeq;
};

class KTarget;
class KIOTable;
class KIOColumn;

class KTableEditFieldInfo
{
public:
	KTableEditFieldInfo(void);
	~KTableEditFieldInfo(void);

public:
	static void InitGroupCode(KTarget* a_pTarget, std::map<int, CString> &a_mapGroupCode);
	static void DefaultCodeFiledData(KIOTable* a_pIOTable, KIOColumn &a_oIOColumnUpdate);
	static KEMEditFieldType GetEditFieldType(KIOTable* a_pIOTable, KIOColumn a_oIOColumnUpdate);
	static void GetVehicleModeInfo(KDBaseConPtr spDBaseConnection, CString strColumnName, TVehicleModelKey& a_oVehicleInfo);

	static bool IsRegisterFieldDistiributionModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nObjectID);
	static bool IsRegisterFieldDistiributionModel(KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn);
	static bool IsRegisterFieldModeChoiceModel   (KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nModeID);
	static bool IsRegisterFieldModeChoiceModel   (KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn);
	static bool IsRegisterFieldGenerationModel   (KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn, int &a_nObjectID, CString &a_strPAFlag);
	static bool IsRegisterFieldGenerationModel   (KDBaseConPtr spDBaseConnection, KIOColumn &a_oIOColumn);
	static bool IsRegisterFieldVehicleConverModel(KDBaseConPtr spDbConeeciton, KIOColumn & a_oIOColumn);
	static bool IsRegisterFieldVehicleConverModel(KDBaseConPtr spDbConeeciton, KIOColumn & a_oIOColumn, int& a_nModeID);

	static bool UpdateDistributionModelObjectID  (KDBaseConPtr spDBaseConnection, TDistributionModelKey oTDistributionModelKey, int a_nNewObjectID);
	static bool UpdateModeChoiceModelModeID      (KDBaseConPtr spDBaseConnection, TModeChoiceModelKey   oTModeChoiceModelKey,   int a_nNewModeID);
	static bool UpdateGenerationModelObjectID    (KDBaseConPtr spDBaseConnection, TGenerationModelKey   oTGenerationModelKey,   int a_nNewObjectID, CString a_strNewPAFlag);
	static bool UpdateVehicleModelMode			 (KDBaseConPtr spDBaseConneciton, TVehicleModelKey oTVehicleInfo, int a_nModeID);
};

