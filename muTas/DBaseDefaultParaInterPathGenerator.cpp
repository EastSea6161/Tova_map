#include "StdAfx.h"
#include "DBaseDefaultParaInterPathGenerator.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanPathInfo(std::map<int, std::vector<TUrbanPathInfo>>& a_mapUrbanPathInfo)
{
	TxLogDebugStartMsg();

	a_mapUrbanPathInfo.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, name, no_path From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_PATH_INFO);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TUrbanPathInfo oTUrbanPathInfo = {};
			int nMasterCode			   = spResultSet->GetValueInt    (0);
			oTUrbanPathInfo.nMode	   = spResultSet->GetValueInt	 (1);
			oTUrbanPathInfo.strMode    = spResultSet->GetValueString (2);
			oTUrbanPathInfo.nMaxofPath = spResultSet->GetValueInt	 (3);

			AutoType iter = a_mapUrbanPathInfo.find(nMasterCode);
			AutoType end  = a_mapUrbanPathInfo.end();

			if (iter == end)
			{
				std::vector<TUrbanPathInfo> vecTUrbanPathInfo;
				vecTUrbanPathInfo.push_back(oTUrbanPathInfo);
				a_mapUrbanPathInfo.insert(std::make_pair(nMasterCode, vecTUrbanPathInfo));
			}
			else
			{
				std::vector<TUrbanPathInfo>& vecTUrbanPathInfo = iter->second;
				vecTUrbanPathInfo.push_back(oTUrbanPathInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanAutoCost(std::map<int, std::vector<TInterAutoCost>>& a_mapUrbanAutoCost)
{
	TxLogDebugStartMsg();

	a_mapUrbanAutoCost.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, auto, price, ratio, fuel From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_AUTO_COST);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TInterAutoCost oTUrbanAutoCost = {};
			int nMasterCode			= spResultSet->GetValueInt    (0);
			oTUrbanAutoCost.nType   = spResultSet->GetValueInt    (1);
			oTUrbanAutoCost.strName	= spResultSet->GetValueString (2);
			oTUrbanAutoCost.dPrice	= spResultSet->GetValueDouble (3);
			oTUrbanAutoCost.dRation	= spResultSet->GetValueDouble (4);
			oTUrbanAutoCost.dfule	= spResultSet->GetValueDouble (5); 

			AutoType iter = a_mapUrbanAutoCost.find(nMasterCode);
			AutoType end  = a_mapUrbanAutoCost.end();

			if (iter == end)
			{
				std::vector<TInterAutoCost> vecUrbanAutoCost;
				vecUrbanAutoCost.push_back(oTUrbanAutoCost);
				a_mapUrbanAutoCost.insert(std::make_pair(nMasterCode, vecUrbanAutoCost));
			}
			else
			{
				std::vector<TInterAutoCost>& vecUrbanAutoCost = iter->second;
				vecUrbanAutoCost.push_back(oTUrbanAutoCost);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanTransitCost(std::map<int, std::vector<TInterTaxiCost>>& a_mapUrbanTransitCost)
{
	TxLogDebugStartMsg();

	a_mapUrbanTransitCost.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, taxi, base_fare, base_dist, dist_base_fare, unit_dist From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_URBAN_TRANSIT_COST);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TInterTaxiCost oTUrbanTaxiCost = {};
			int nMasterCode	          = spResultSet->GetValueInt    (0);
			oTUrbanTaxiCost.nType     = spResultSet->GetValueInt    (1);
			oTUrbanTaxiCost.strName   = spResultSet->GetValueString (2);
			oTUrbanTaxiCost.dBasefare = spResultSet->GetValueDouble (3);
			oTUrbanTaxiCost.dBaseDist = spResultSet->GetValueDouble (4);
			oTUrbanTaxiCost.dAddFare  = spResultSet->GetValueDouble (5);
			oTUrbanTaxiCost.dAddDist  = spResultSet->GetValueDouble (6);

			AutoType iter = a_mapUrbanTransitCost.find(nMasterCode);
			AutoType end  = a_mapUrbanTransitCost.end();

			if (iter == end)
			{
				std::vector<TInterTaxiCost> vecUrbanTaxiCost;
				vecUrbanTaxiCost.push_back(oTUrbanTaxiCost);
				a_mapUrbanTransitCost.insert(std::make_pair(nMasterCode, vecUrbanTaxiCost));
			}
			else
			{
				std::vector<TInterTaxiCost>& vecUrbanTaxiCost = iter->second;
				vecUrbanTaxiCost.push_back(oTUrbanTaxiCost);;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathInfo(std::map<int, std::vector<TRegionPathInfo>>& a_mapRegionalPathInfo)
{
	TxLogDebugStartMsg();

	a_mapRegionalPathInfo.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, name, no_path From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_INFO);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TRegionPathInfo oTRegionlPathInfo = {};
			int nMasterCode				 = spResultSet->GetValueInt    (0);
			oTRegionlPathInfo.nMode		 = spResultSet->GetValueInt	   (1);
			oTRegionlPathInfo.strMode    = spResultSet->GetValueString (2);
			oTRegionlPathInfo.nMaxofPath = spResultSet->GetValueInt	   (3);

			AutoType iter = a_mapRegionalPathInfo.find(nMasterCode);
			AutoType end  = a_mapRegionalPathInfo.end();

			if (iter == end)
			{
				std::vector<TRegionPathInfo> vecTRegionlPathInfo;
				vecTRegionlPathInfo.push_back(oTRegionlPathInfo);
				a_mapRegionalPathInfo.insert(std::make_pair(nMasterCode, vecTRegionlPathInfo));
			}
			else
			{
				std::vector<TRegionPathInfo>& vecTRegionlPathInfo = iter->second;
				vecTRegionlPathInfo.push_back(oTRegionlPathInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalAutoCost(std::map<int, std::vector<TInterAutoCost>>& a_mapRegionalAutoCost)
{
	TxLogDebugStartMsg();

	a_mapRegionalAutoCost.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, auto, price, ratio, fuel From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_AUTO_COST);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TInterAutoCost oTRegionalAutoCost = {};
			int nMasterCode			= spResultSet->GetValueInt    (0);
			oTRegionalAutoCost.nType   = spResultSet->GetValueInt    (1);
			oTRegionalAutoCost.strName	= spResultSet->GetValueString (2);
			oTRegionalAutoCost.dPrice	= spResultSet->GetValueDouble (3);
			oTRegionalAutoCost.dRation	= spResultSet->GetValueDouble (4);
			oTRegionalAutoCost.dfule	= spResultSet->GetValueDouble (5); 

			AutoType iter = a_mapRegionalAutoCost.find(nMasterCode);
			AutoType end  = a_mapRegionalAutoCost.end();

			if (iter == end)
			{
				std::vector<TInterAutoCost> vecRegionalAutoCost;
				vecRegionalAutoCost.push_back(oTRegionalAutoCost);
				a_mapRegionalAutoCost.insert(std::make_pair(nMasterCode, vecRegionalAutoCost));
			}
			else
			{
				std::vector<TInterAutoCost>& vecRegionalAutoCost = iter->second;
				vecRegionalAutoCost.push_back(oTRegionalAutoCost);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalTerminalTime(std::map<int, std::vector<TTerminalProcessTime>>& a_mapRegionalTerminalTime)
{
	TxLogDebugStartMsg();

	a_mapRegionalTerminalTime.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, id, name, access_time, egress_time, mode_transfer From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_TERMINAL_TIME);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			TTerminalProcessTime oTTerminalProcessTime = {};
			int nMasterCode						= spResultSet->GetValueInt    (0);
			oTTerminalProcessTime.nModeID		= spResultSet->GetValueInt    (1);
			oTTerminalProcessTime.strModeName	= spResultSet->GetValueString (2);
			oTTerminalProcessTime.dAccess		= spResultSet->GetValueDouble (3);
			oTTerminalProcessTime.dEgress		= spResultSet->GetValueDouble (4);
			oTTerminalProcessTime.nModeTransfer	= spResultSet->GetValueInt	  (5);

			AutoType iter = a_mapRegionalTerminalTime.find(nMasterCode);
			AutoType end  = a_mapRegionalTerminalTime.end();

			if (iter == end)
			{
				std::vector<TTerminalProcessTime> vecTTerminalProcessTime;
				vecTTerminalProcessTime.push_back(oTTerminalProcessTime);
				a_mapRegionalTerminalTime.insert(std::make_pair(nMasterCode, vecTTerminalProcessTime));
			}
			else
			{
				std::vector<TTerminalProcessTime>& vecTTerminalProcessTime = iter->second;
				vecTTerminalProcessTime.push_back(oTTerminalProcessTime);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathSetting(std::map<int, KPathTimeSetting> & a_mapRegionalPathTimeSetting)
{
	TxLogDebugStartMsg();

	a_mapRegionalPathTimeSetting.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" Select master_code, taxi_time, highway_time, rail_time, inter_vot From %s"), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_REGIONAL_PATH_SETTING);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			KPathTimeSetting oPathTimeSetting;
			int nMasterCode				         = spResultSet->GetValueInt    (0);
			oPathTimeSetting.dTaxiWaitingTime    = spResultSet->GetValueDouble (1);
			oPathTimeSetting.dHighwayTransitTime = spResultSet->GetValueDouble (2);
			oPathTimeSetting.dRailTransitTime    = spResultSet->GetValueDouble (3);
			oPathTimeSetting.dInterValueofTime   = spResultSet->GetValueDouble (4);

			a_mapRegionalPathTimeSetting.insert(std::make_pair(nMasterCode, oPathTimeSetting));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::SetUrbanModeData(std::map<int, TUrbanPathInfo>& a_mapUrbanPathInfo)
{
	TxLogDebugStartMsg();

	a_mapUrbanPathInfo.clear();
	
	try
	{
		TUrbanPathInfo oBusInfo = {};
		oBusInfo.nMode			= P_MODE_BUS;
		oBusInfo.strMode		= _T("Bus_Transit");
        if (KmzSystem::GetLanguage() == KEMKorea) {
            oBusInfo.strMode = _T("버스");
        }

		oBusInfo.nMaxofPath	= 10;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_BUS, oBusInfo));

		TUrbanPathInfo oSubwayInfo = {};
		oSubwayInfo.nMode			= P_MODE_RAIL;
		oSubwayInfo.strMode			= _T("Rail_Transit");
        if (KmzSystem::GetLanguage() == KEMKorea) {
            oSubwayInfo.strMode	= _T("도시철도");
        }

		oSubwayInfo.nMaxofPath		= 10;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_RAIL, oSubwayInfo));

		TUrbanPathInfo oBSInfo = {};
		oBSInfo.nMode			= P_MODE_BUS_RAIL;
		oBSInfo.strMode			= _T("Composit_Mode");
        if (KmzSystem::GetLanguage() == KEMKorea) {
            oBSInfo.strMode	= _T("버스+도시철도");
        }

		oBSInfo.nMaxofPath		= 10;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_BUS_RAIL, oBSInfo));

		TUrbanPathInfo oPRInfo = {};
		oPRInfo.nMode			= P_MODE_PARK_RIDE;
		oPRInfo.strMode			= _T("Park&Ride");
		oPRInfo.nMaxofPath		= 5;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_PARK_RIDE, oPRInfo));

		TUrbanPathInfo oKRInfo = {};
		oKRInfo.nMode			= P_MODE_KISS_RIDE;
		oKRInfo.strMode			= _T("Kiss&Ride");
		oKRInfo.nMaxofPath		= 5;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_KISS_RIDE, oKRInfo));

		TUrbanPathInfo oTTInfo = {};
		oTTInfo.nMode			= P_MODE_TAXI_TRANSIT;
		oTTInfo.strMode			= _T("Taxi&Transit");
        if (KmzSystem::GetLanguage() == KEMKorea) {
            oTTInfo.strMode	= _T("택시 & 대중교통");
        }
		oTTInfo.nMaxofPath		= 5;
		a_mapUrbanPathInfo.insert(std::make_pair(P_MODE_TAXI_TRANSIT, oTTInfo));
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::SetAutoCost(std::map<int, TInterAutoCost>& a_mapAutoCost)
{
	TxLogDebugStartMsg();

	a_mapAutoCost.clear();

	try
	{
		TInterAutoCost oGasoline = {};
		oGasoline.nType	         = P_FUEL_GASOLINE;
		oGasoline.strName        = P_GASOLINE_NAME;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oGasoline.strName		= P_GASOLINE_NAME_K;
		}
		oGasoline.dPrice         = 2000.0;
		oGasoline.dRation        = 0.50;
		oGasoline.dfule	         = 8.0;
		a_mapAutoCost.insert(std::make_pair(P_FUEL_GASOLINE, oGasoline));

		TInterAutoCost oDiesel = {};
		oDiesel.nType	       = P_FUEL_DIESEL;
		oDiesel.strName        = P_DIESEL_NAME;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oDiesel.strName		= P_DIESEL_NAME_K;
		}
		oDiesel.dPrice         = 1700.0;
		oDiesel.dRation        = 0.35;
		oDiesel.dfule	       = 12.0;
		a_mapAutoCost.insert(std::make_pair(P_FUEL_DIESEL, oDiesel));

		TInterAutoCost oLPG    = {};
		oLPG.nType	           = P_FUEL_LPG;
		oLPG.strName           = P_LPG_NAME;
		oLPG.dPrice            = 1200.0;
		oLPG.dRation           = 0.15;
		oLPG.dfule	           = 6.0;
		a_mapAutoCost.insert(std::make_pair(P_FUEL_LPG, oLPG));
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::SetRegionalModeInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapRegionalModeInfo)
{
	TxLogDebugStartMsg();

	a_mapRegionalModeInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		int nCompareKCode = 121;

		CString strSQL(_T(""));
		strSQL.Format (_T(" Select kcode_group_key, kcode_key, kcode_display From %s"), TABLE_KCODE_DEFINE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			int nKCodeGroupKey	= spResultSet->GetValueInt    (0);
			int nKCodeKey       = spResultSet->GetValueInt    (1);
			CString KCodeName   = spResultSet->GetValueString (2);

			if(nKCodeGroupKey == nCompareKCode)
			{
				a_mapRegionalModeInfo.insert(std::make_pair(nKCodeKey, KCodeName));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDBaseDefaultParaInterPathGenerator::SetRegionalTerminalTime(KTarget* a_pTarget, std::map<int, TTerminalProcessTime>& a_mapRegionalTerminalTime)
{
	TxLogDebugStartMsg();

	a_mapRegionalTerminalTime.clear();

	try
	{
		std::map<int, CString> mapRegionalModeInfo;
		KDBaseDefaultParaInterPathGenerator::SetRegionalModeInfo(a_pTarget, mapRegionalModeInfo);

        AutoType iterRegionModeInfo = mapRegionalModeInfo.begin();
		AutoType endRegionModeInfo  = mapRegionalModeInfo.end();
		if(iterRegionModeInfo != endRegionModeInfo)
		{
			TTerminalProcessTime oTTerminalProcessTime = {};

			std::map<int, int> mapTerminalMatching;
			KDBaseInterModal::GetInterRegionModeDefine(a_pTarget, mapTerminalMatching);

			size_t nModeCount = mapRegionalModeInfo.size();
			for(size_t i = 0; i < nModeCount; i++)
			{
				int nCodeKey        = iterRegionModeInfo->first;
				oTTerminalProcessTime.strModeName = iterRegionModeInfo->second;

				AutoType iterFindCode = mapTerminalMatching.find(nCodeKey);
				AutoType endFindCode  = mapTerminalMatching.end();
				if(iterFindCode != endFindCode)
				{
					if( iterFindCode->second == BUS_TRMINAL)
					{
						oTTerminalProcessTime.dAccess = 15;
						oTTerminalProcessTime.dEgress = 5;
					}
					else
					{
						oTTerminalProcessTime.dAccess = 20;
						oTTerminalProcessTime.dEgress = 10;
					}
				}
				else
				{
					oTTerminalProcessTime.dAccess = 20;
					oTTerminalProcessTime.dEgress = 10;
				}
				a_mapRegionalTerminalTime.insert(std::make_pair(nCodeKey, oTTerminalProcessTime));

				iterRegionModeInfo++;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}