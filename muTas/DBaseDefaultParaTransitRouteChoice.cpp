#include "StdAfx.h"
#include "DBaseDefaultParaTransitRouteChoice.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"


void KDBaseDefaultParaTransitRouteChoice::LoadDefaultTransitRouteChoicePara(std::map<int, std::vector<KTransitRouteChoicePara>>& a_mapDefaultTransitRouteChoicePara)
{
	TxLogDebugStartMsg();

	a_mapDefaultTransitRouteChoicePara.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, Variable_Code, Highway_Transit, Rail_Transit, Composite_Mode FROM %s "), KDefaultParaTable::TABLE_DEFAULT_TRANSIT_ROUTECHOICE_PARA);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KTransitRouteChoicePara oTransitRouteChoicePara;

		while( pResult->Next() )
		{
			int nMasterCode							= pResult->GetValueInt    (0);
			oTransitRouteChoicePara.nVariableCode	= pResult->GetValueInt    (1);
			oTransitRouteChoicePara.dHighwayTransit = pResult->GetValueDouble (2);
			oTransitRouteChoicePara.dRailTransit    = pResult->GetValueDouble (3);
			oTransitRouteChoicePara.dCompositeMode  = pResult->GetValueDouble (4);

			AutoType find = a_mapDefaultTransitRouteChoicePara.find(nMasterCode);
			AutoType end  = a_mapDefaultTransitRouteChoicePara.end();

			if (end == find)
			{
				std::vector<KTransitRouteChoicePara> vecModeChoicePara;
				vecModeChoicePara.push_back(oTransitRouteChoicePara);
				a_mapDefaultTransitRouteChoicePara.insert(std::make_pair(nMasterCode, vecModeChoicePara));
			}
			else
			{
				std::vector<KTransitRouteChoicePara> & vecModeChoicePara = find->second;
				vecModeChoicePara.push_back(oTransitRouteChoicePara);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseDefaultParaTransitRouteChoice::LoadDefaultTransitRouteChoiceSetting(std::map<int, KTransitRouteChoiceSetting>& a_mapDefaultTransitRouteChoiceSetting)
{
	TxLogDebugStartMsg();

	a_mapDefaultTransitRouteChoiceSetting.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, BoardingTime, DwellTime FROM %s"), KDefaultParaTable::TABLE_DEFAULT_TRANSIT_ROUTECHOICE_SETTING);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KTransitRouteChoiceSetting oTransitRouteChoiceSetting;

		while( pResult->Next() )
		{
			int nMasterCode								= pResult->GetValueInt    (0);
			oTransitRouteChoiceSetting.dBoardingTime	= pResult->GetValueDouble (1);
			oTransitRouteChoiceSetting.dDwellTime		= pResult->GetValueDouble (2);

			a_mapDefaultTransitRouteChoiceSetting.insert(std::make_pair(nMasterCode, oTransitRouteChoiceSetting));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseDefaultParaTransitRouteChoice::SaveCoefficient( int a_nVariableCode, double a_dValue, TMAFnCoefficient& a_oFnCofficient )
{
	try
	{
		if (KVariableNameDefine::V_INVEHICLE_TIME == a_nVariableCode)
		{
			a_oFnCofficient.TdInVehcle		= a_dValue;
		}
		else if (KVariableNameDefine::V_WAITING_TIME == a_nVariableCode)
		{
			a_oFnCofficient.TdWaitingTime	= a_dValue;
		}
		else if (KVariableNameDefine::V_TRANSFER_TIME == a_nVariableCode)
		{
			a_oFnCofficient.TdTransferTime	= a_dValue;
		}
		else if (KVariableNameDefine::V_ACCESSWALK_TIME == a_nVariableCode)
		{
			a_oFnCofficient.TdAccessWalkTime	= a_dValue;
		}
		else if (KVariableNameDefine::V_EGRESSWALK_TIME == a_nVariableCode)
		{
			a_oFnCofficient.TdEgressWalkTime	= a_dValue;
		}
		else if (KVariableNameDefine::V_NO_TRANSFER == a_nVariableCode)
		{
			a_oFnCofficient.TdNoOfTransfer	= a_dValue;
		}
		else if (KVariableNameDefine::V_COST == a_nVariableCode)
		{
			a_oFnCofficient.TdCost	= a_dValue;
		}
		else if (KVariableNameDefine::V_NO_STOP_ZONE == a_nVariableCode)
		{
			a_oFnCofficient.TdNoOfStop	= a_dValue;
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

void KDBaseDefaultParaTransitRouteChoice::LoadDefaultTransitRouteChoiceParaBus(std::map<int, TMAFnCoefficient>& a_mapBusCofficient )
{
	a_mapBusCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, highway_transit from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_TRANSIT_ROUTECHOICE_PARA);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			int nMasterCode		= spResult->GetValueInt(0);
			int nVariableCode	= spResult->GetValueInt(1);
			double dValue		= spResult->GetValueDouble(2);

			AutoType find = a_mapBusCofficient.find(nMasterCode);
			AutoType end  = a_mapBusCofficient.end();

			if (find != end)
			{
				TMAFnCoefficient& oCofficient = find->second;
				SaveCoefficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCoefficient(nVariableCode, dValue, oCofficient);
				a_mapBusCofficient.insert(std::make_pair(nMasterCode, oCofficient));
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

void KDBaseDefaultParaTransitRouteChoice::LoadDefaultTransitRouteChoiceParaRail(std::map<int, TMAFnCoefficient>& a_mapSubwayCofficient )
{
	a_mapSubwayCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, rail_transit from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_TRANSIT_ROUTECHOICE_PARA);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			int nMasterCode		= spResult->GetValueInt(0);
			int nVariableCode	= spResult->GetValueInt(1);
			double dValue		= spResult->GetValueDouble(2);

			AutoType find = a_mapSubwayCofficient.find(nMasterCode);
			AutoType end  = a_mapSubwayCofficient.end();

			if (find != end)
			{
				TMAFnCoefficient& oCofficient = find->second;
				SaveCoefficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCoefficient(nVariableCode, dValue, oCofficient);
				a_mapSubwayCofficient.insert(std::make_pair(nMasterCode, oCofficient));
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

void KDBaseDefaultParaTransitRouteChoice::LoadDefaultTransitRouteChoiceParaComposite(std::map<int, TMAFnCoefficient>& a_mapCompositCofficient )
{
	a_mapCompositCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, Composite_Mode from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_TRANSIT_ROUTECHOICE_PARA);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			int nMasterCode		= spResult->GetValueInt(0);
			int nVariableCode	= spResult->GetValueInt(1);
			double dValue		= spResult->GetValueDouble(2);

			AutoType find = a_mapCompositCofficient.find(nMasterCode);
			AutoType end  = a_mapCompositCofficient.end();

			if (find != end)
			{
				TMAFnCoefficient& oCofficient = find->second;
				SaveCoefficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCoefficient(nVariableCode, dValue, oCofficient);
				a_mapCompositCofficient.insert(std::make_pair(nMasterCode, oCofficient));
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

