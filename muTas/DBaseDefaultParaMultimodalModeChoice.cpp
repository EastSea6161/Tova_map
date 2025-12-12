#include "StdAfx.h"
#include "DBaseDefaultParaMultimodalModeChoice.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

void KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoicePara(std::map<int, std::vector<KModeChoicePara>>& a_mapDefaultModeChoicePara)
{
	TxLogDebugStartMsg();

	a_mapDefaultModeChoicePara.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, Variable_Code, Highway_Transit, Rail_Transit, Composite_Mode FROM %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		KModeChoicePara oModeChoicePara;

		while( spResult->Next() )
		{
			int nMasterCode					= spResult->GetValueInt    (0);
			oModeChoicePara.nVariableCode	= spResult->GetValueInt    (1);
			oModeChoicePara.dHighwayTransit = spResult->GetValueDouble (2);
			oModeChoicePara.dRailTransit    = spResult->GetValueDouble (3);
			oModeChoicePara.dCompositeMode  = spResult->GetValueDouble (4);

			AutoType find = a_mapDefaultModeChoicePara.find(nMasterCode);
			AutoType end  = a_mapDefaultModeChoicePara.end();

			if (end == find)
			{
				std::vector<KModeChoicePara> vecModeChoicePara;
				vecModeChoicePara.push_back(oModeChoicePara);
				a_mapDefaultModeChoicePara.insert(std::make_pair(nMasterCode, vecModeChoicePara));
			}
			else
			{
				std::vector<KModeChoicePara> & vecModeChoicePara = find->second;
				vecModeChoicePara.push_back(oModeChoicePara);
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

void KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoiceSetting(std::map<int, KModeChoiceSetting>& a_mapDefaultModeChoiceSetting)
{
	TxLogDebugStartMsg();

	a_mapDefaultModeChoiceSetting.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, MEU, BoardingTime, DwellTime FROM %s"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_SETTING);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		KModeChoiceSetting oModeChoiceSetting;

		while( spResult->Next() )
		{
			int nMasterCode						= spResult->GetValueInt    (0);
			oModeChoiceSetting.dMEU				= spResult->GetValueDouble (1);
			oModeChoiceSetting.dBoardingTime	= spResult->GetValueDouble (2);
			oModeChoiceSetting.dDwellTime		= spResult->GetValueDouble (3);

			a_mapDefaultModeChoiceSetting.insert(std::make_pair(nMasterCode, oModeChoiceSetting));
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

void KDBaseDefaultParaMultimodalModeChoice::GetVariableCodeName(std::map<int, CString>& a_mapVariableCodeName)
{
	TxLogDebugStartMsg();

	a_mapVariableCodeName.clear();

	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_INVEHICLE_TIME,	KVariableNameDefine::V_INVEHICLE_TIME_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_WAITING_TIME,  	KVariableNameDefine::V_WAITING_TIME_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_TRANSFER_TIME,	KVariableNameDefine::V_TRANSFER_TIME_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_ACCESSWALK_TIME, KVariableNameDefine::V_ACCESSWALK_TIME_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_EGRESSWALK_TIME, KVariableNameDefine::V_EGRESSWALK_TIME_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_NO_TRANSFER,		KVariableNameDefine::V_NO_TRANSFER_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_COST,			KVariableNameDefine::V_COST_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_NO_STOP_ZONE,	KVariableNameDefine::V_NO_STOP_ZONE_NAME_K));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_DUMMY,			KVariableNameDefine::V_DUMMY_NAME_K));
		}
		else
		{
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_INVEHICLE_TIME,	KVariableNameDefine::V_INVEHICLE_TIME_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_WAITING_TIME,  	KVariableNameDefine::V_WAITING_TIME_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_TRANSFER_TIME,	KVariableNameDefine::V_TRANSFER_TIME_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_ACCESSWALK_TIME, KVariableNameDefine::V_ACCESSWALK_TIME_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_EGRESSWALK_TIME, KVariableNameDefine::V_EGRESSWALK_TIME_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_NO_TRANSFER,		KVariableNameDefine::V_NO_TRANSFER_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_COST,			KVariableNameDefine::V_COST_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_NO_STOP_ZONE,	KVariableNameDefine::V_NO_STOP_ZONE_NAME));
			a_mapVariableCodeName.insert(std::make_pair(KVariableNameDefine::V_DUMMY,			KVariableNameDefine::V_DUMMY_NAME));
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

void KDBaseDefaultParaMultimodalModeChoice::LoadDefaultTransitRouteChoiceParaBus(std::map<int, TMAFnCoefficient>& a_mapBusCofficient )
{
	a_mapBusCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, highway_transit from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);
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
				SaveCofficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCofficient(nVariableCode, dValue, oCofficient);
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

void KDBaseDefaultParaMultimodalModeChoice::LoadDefaultTransitRouteChoiceParaRail(std::map<int, TMAFnCoefficient>& a_mapSubwayCofficient )
{
	a_mapSubwayCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, rail_transit from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);
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
				SaveCofficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCofficient(nVariableCode, dValue, oCofficient);
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

void KDBaseDefaultParaMultimodalModeChoice::LoadDefaultTransitRouteChoiceParaComposite(std::map<int, TMAFnCoefficient>& a_mapCompositCofficient )
{
	a_mapCompositCofficient.clear();
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("select master_code, variable_code, Composite_Mode from %s order by master_code, variable_code"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);
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
				SaveCofficient(nVariableCode, dValue, oCofficient);
			}
			else
			{
				TMAFnCoefficient oCofficient = {};
				SaveCofficient(nVariableCode, dValue, oCofficient);
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

void KDBaseDefaultParaMultimodalModeChoice::SaveCofficient( int a_nVariableCode, double a_dValue, TMAFnCoefficient& a_oFnCofficient )
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
