#include "stdafx.h"
#include "DefineNamesDefaultPara.h"

namespace KDefaultParaTable
{
	const TCHAR* TABLE_DEFAULT_ACCESS_CLASSINFO						= _T("Default_Access_ClassInfo");
	const TCHAR* TABLE_DEFAULT_ACCESS_CLASSINFO_SETTING				= _T("Default_Access_Classinfo_Setting");
	const TCHAR* TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT				= _T("Default_Path_Generate_ConstraintConditions");
	const TCHAR* TABLE_DEFAULT_PATH_GENERATE_INVEHICLE				= _T("Default_Path_Generate_inVehicleCongestion");
	const TCHAR* TABLE_DEFAULT_MULTIMODAL_PATH_GENERATION			= _T("Default_Multimodal_PathGeneration");
	const TCHAR* TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA			= _T("Default_Multimodal_ModeChoice_para");
	const TCHAR* TABLE_DEFAULT_MULTIMODAL_MODECHOICE_SETTING		= _T("Default_Multimodal_ModeChoice_Setting");
	const TCHAR* TABLE_DEFAULT_TRANSIT_ROUTECHOICE_PARA				= _T("Default_Transit_RouteChoice_para");
	const TCHAR* TABLE_DEFAULT_TRANSIT_ROUTECHOICE_SETTING			= _T("Default_Transit_RouteChoice_Setting");
	const TCHAR* TABLE_DEFAULT_HIGHWAY_ASSIGN_PCE_MODE				= _T("Default_Highway_Assign_PCE_Mode");
	const TCHAR* TABLE_DEFAULT_HIGHWAY_ASSIGN_OD_TIME				= _T("Default_Highway_Assign_OD_Time");
	const TCHAR* TABLE_DEFAULT_HIGHWAY_ASSIGN_TRAVELTIME			= _T("Default_Highway_Assign_TravelTime");
	const TCHAR* TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA				= _T("Default_Highway_Assign_VDF_Para");
	const TCHAR* TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA_SET			= _T("Default_Highway_Assign_VDF_Para_Set");
	const TCHAR* TABLE_DEFAULT_FUNCTION  							= _T("Function");
	const TCHAR* TABLE_DEFAULT_FUNCTION_GROUP						= _T("function_Group");
	const TCHAR* TABLE_DEFAULT_ACCESS_TERMINAL_SETTING				= _T("Default_Access_Terminal_Setting");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_URBAN_PATH_INFO			= _T("Default_Intermodal_Urban_Path_Info");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_URBAN_AUTO_COST			= _T("Default_Intermodal_Urban_Auto_Cost");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_URBAN_TRANSIT_COST		= _T("Default_Intermodal_Urban_Transit_Cost");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_INFO		= _T("Default_Intermodal_Regional_Path_Info");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_REGIONAL_AUTO_COST		= _T("Default_Intermodal_Regional_Auto_Cost");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME	= _T("Default_Intermodal_Regional_Terminal_Time");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_SETTING		= _T("Default_Intermodal_Regional_Path_Setting");
	const TCHAR* TABLE_DEFAULT_INTERMODAL_PCE_MODE					= _T("Default_Intermodal_PCE_Mode");
	const TCHAR* TABLE_DEFAULT_REGIONAL_MODECHOICE_MEU				= _T("Default_Regional_ModeChoice_MEU");
	const TCHAR* TABLE_DEFAULT_REGIONAL_MODECHOICE_PARA				= _T("Default_Regional_ModeChoice_Para");
	const TCHAR* TABLE_DEFAULT_TRIP_DESTRIBUTION					= _T("Default_Trip_Distribution");

	//화물
	const TCHAR* TABLE_DEFAULT_FMODECHOICE_VARIABLE	= _T("Default_FModeChoice_Variable");
	const TCHAR* TABLE_DEFAULT_FGENERATIONPARA		= _T("Default_FGenerationPara");
	const TCHAR* TABLE_DEFAULT_LOGISTICPARA			= _T("Default_LogisticPara");
	const TCHAR* TABLE_DEFAULT_POLICY_CONTANIER		= _T("Default_FPolicy_Contanier");

	const TCHAR* TABLE_DEFAULT_AVGCAGOVOLUMNE		= _T("Default_AvgCargoVolume");
	const TCHAR* TABLE_DEFAULT_FORMULACAGOVOLUMNE	= _T("Default_FormulaCagoVolume");	
	const TCHAR* TABLE_DEFAULT_AVGCAGOCAPA			= _T("Default_AvgCargoCapa");
	const TCHAR* TABLE_DEFAULT_AVGEMPTYVEHICLE		= _T("Default_AvgEmptyVehicle");
	const TCHAR* TABLE_DEFAULT_CALTIMECOST			= _T("Default_CalTimeCost");
	const TCHAR* TABLE_DEFAUlT_VEHICEL_CONTAINER	= _T("Default_Vehicle_Container");

	const TCHAR* TABLE_DEFAULT_MODECHOICE_DEFINE_VALUE	 = _T("Default_ModeChoice_Define_Value");
	const TCHAR* TABLE_DEFAULT_MODECHOICE_DEFINEVAL_INFO = _T("Default_ModeChoice_DefineVal_Info");		
}

namespace KAccessStationDefine
{
	const int ACCESS_DEFAULT_DISTANCE		= 0;
	const int ACCESS_ALL_CLASS_COUNT		= 10;
	const int ACCESS_INCREASE_TIME			= 3;
	const int ACCESS_INIT_TIME				= 3;
	const TCHAR ACCESS_CLASS_INIT_CHAR		= 'A';
}

namespace KVariableNameDefine
{
	const int V_INVEHICLE_TIME				= 1;
	const int V_WAITING_TIME				= 2;
	const int V_TRANSFER_TIME				= 3;
	const int V_ACCESSWALK_TIME				= 4;
	const int V_EGRESSWALK_TIME				= 5;
	const int V_NO_TRANSFER					= 6;
	const int V_COST						= 7;
	const int V_NO_STOP_ZONE				= 8;
	const int V_DUMMY						= 9;

	const TCHAR* V_INVEHICLE_TIME_NAME		= (_T("In-Vehicle Time"));
	const TCHAR* V_WAITING_TIME_NAME		= (_T("Waiting Time"));
	const TCHAR* V_TRANSFER_TIME_NAME		= (_T("Transfer Time"));
	const TCHAR* V_ACCESSWALK_TIME_NAME		= (_T("Access Walk Time"));
	const TCHAR* V_EGRESSWALK_TIME_NAME		= (_T("Egress Walk Time"));
	const TCHAR* V_NO_TRANSFER_NAME			= (_T("No.of Transfers"));
	const TCHAR* V_COST_NAME				= (_T("Cost"));
	const TCHAR* V_NO_STOP_ZONE_NAME		= (_T("No.of Stop At Zone"));
	const TCHAR* V_DUMMY_NAME				= (_T("Dummy"));

	const TCHAR* V_INVEHICLE_TIME_NAME_K	= (_T("차내통행시간"));
	const TCHAR* V_WAITING_TIME_NAME_K		= (_T("대기시간"));
	const TCHAR* V_TRANSFER_TIME_NAME_K		= (_T("환승시간"));
	const TCHAR* V_ACCESSWALK_TIME_NAME_K	= (_T("출발 접근시간"));
	const TCHAR* V_EGRESSWALK_TIME_NAME_K	= (_T("도착 접근시간"));
	const TCHAR* V_NO_TRANSFER_NAME_K		= (_T("환승횟수"));
	const TCHAR* V_COST_NAME_K				= (_T("통행비용"));
	const TCHAR* V_NO_STOP_ZONE_NAME_K		= (_T("역/정류장 개수"));
	const TCHAR* V_DUMMY_NAME_K				= (_T("더미"));
}

namespace KDefaultParameterMasterCode
{
	const int NEW_MASTER_CODE		= -1;//시스템과 유저코드 둘다 없을경우
	const int SYSTEM_MASTER_CODE	= 0;
	const int USER_MASETER_CODE		= 1;
}

namespace KDefaultParaPCEOccupancy
{
	const double DEFAULT_OCCUPANCY_AUTO   = 1.33;
	const double DEFAULT_OCCUPANCY_TAXI   = 1.47;
	const double DEFAULT_OCCUPANCY_BUS	  = 19.27;
	const double DEFAULT_OCCUPANCY_OTHER  = 1;

	const double DEFAULT_PCE_AUTO	= 1.0;
	const double DEFAULT_PCE_TAXI	= 1.0;
	const double DEFAULT_PCE_BUS	= 2.0;
	const double DEFAULT_PCE_OTHER	= 1.0;

	const int DEFAULT_AUTO = 1;
	const int DEFAULT_TAXI = 3;
	const int DEFAULT_BUS  = 4;
}


namespace KDefaultVehicleFormula
{
	const int SERVISE_BUSINESS_CODE			= 0;
	const int SERVISE_NONBUSINESS_CODE		= 1;

	const TCHAR* SERVISE_BUSINESS_NAME		= _T("사업용");
	const TCHAR* SERVISE_NONBUSINESS_NAME	= _T("비사업용");

	const int VEHICLE_SMALL_CODE			= 0;
	const int VEHICLE_MIDDLE_CODE			= 1;
	const int VEHICLE_LARGE_CODE			= 2;

	const TCHAR* VEHICLE_SMALL_NAME			= _T("소형");
	const TCHAR* VEHICLE_MIDDLE_NAME		= _T("중형");
	const TCHAR* VEHICEL_LARGE_NAME			= _T("대형");
}

namespace KDefaultParaFolder
{
	const TCHAR* FOLDER_PARAMETER		= _T("Parameter");
}

namespace KDefaultParaFile
{
	const TCHAR* FILE_GENERATION		= _T("GenerationDef.xml");
	const TCHAR* FILE_GENERATION_TEMP	= _T("GenerationDef_temp.xml");
	const TCHAR* FILE_MODECHOICE_VAR    = _T("ModeChoiceVarDef.xml");
}