#include "StdAfx.h"
#include "DBaseDefaultCo2EmissionsEquation.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

KDBaseDefaultCo2EmissionsEquation::KDBaseDefaultCo2EmissionsEquation(void)
{
}


KDBaseDefaultCo2EmissionsEquation::~KDBaseDefaultCo2EmissionsEquation(void)
{
}

void KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationInfo( std::map<int, std::map<int, CString>>& a_mapDefCO2EquationInfo )
{
	a_mapDefCO2EquationInfo.clear();

	try
	{
		CString strQuery = _T("SELECT master_cd, mode_id, mode_name FROM Default_CO2_Equation_Info ORDER BY master_cd, mode_id");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		int     nMasterCD, nModeID;
		CString strModeName;

		while (spResultSet->Next())
		{
			nMasterCD   = spResultSet->GetValueInt(0);
			nModeID     = spResultSet->GetValueInt(1);
			strModeName = spResultSet->GetValueString(2);

			std::map<int, std::map<int, CString>>::iterator fIter = a_mapDefCO2EquationInfo.find(nMasterCD);

			if (fIter == a_mapDefCO2EquationInfo.end())
			{
				std::map<int, CString> mapData;
				mapData.insert(std::make_pair(nModeID, strModeName));

				a_mapDefCO2EquationInfo.insert(std::make_pair(nMasterCD, mapData));
			}
			else
			{
				std::map<int, CString>& mapData = fIter->second;
				std::map<int, CString>::iterator fDataIter = mapData.find(nModeID);

				if (fDataIter == mapData.end())
				{
					mapData.insert(std::make_pair(nModeID, strModeName));
				}
				else
				{
					mapData.erase(nModeID);
					mapData.insert(std::make_pair(nModeID, strModeName));
				}
			}
		}

		if (a_mapDefCO2EquationInfo.size() == 0)
		{
			std::map<int, CString> mapData;

			mapData.insert(std::make_pair(1, _T("승용차")));
			mapData.insert(std::make_pair(2, _T("택시")));
			mapData.insert(std::make_pair(3, _T("버스")));
			mapData.insert(std::make_pair(4, _T("화물")));

			a_mapDefCO2EquationInfo.insert(std::make_pair(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, mapData));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseDefaultCo2EmissionsEquation::GetDefCO2EquationData( std::map<int, std::vector<KCO2EquationData>>& a_mapDefCO2EquationData )
{
	a_mapDefCO2EquationData.clear();

	try
	{
		CString strQuery = _T("SELECT");
		strQuery.Append(_T(" master_cd, mode_id, vehicle_name, pce, fuel_name,"));
		strQuery.Append(_T(" ratio, spd_gap, is_selected, is_spd_gap_all_selected, exp_under,"));
		strQuery.Append(_T(" exp_andover"));
		strQuery.Append(_T(" FROM default_co2_equation_data ORDER BY master_cd, mode_id"));
		
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			KCO2EquationData oData;
			oData.nMasterCD   = spResultSet->GetValueInt(0);
			oData.nModeID     = spResultSet->GetValueInt(1);
			oData.strNameVeh  = spResultSet->GetValueString(2);
			oData.dPCE        = spResultSet->GetValueDouble(3);
			oData.strNameFuel = spResultSet->GetValueString(4);
			oData.dRatio      = spResultSet->GetValueDouble(5);
			oData.dSpdGap     = spResultSet->GetValueDouble(6);

			if (spResultSet->GetValueInt(7) == 1)
			{
				oData.bIsSelcted = TRUE;
			}
			else
			{
				oData.bIsSelcted = FALSE;
			}

			if (spResultSet->GetValueInt(8) == 1)
			{
				oData.bIsSpdGapAllSelected = TRUE;
			}
			else
			{
				oData.bIsSpdGapAllSelected = FALSE;
			}

			oData.strExpUnder   = spResultSet->GetValueString(9);
			oData.strExpAndOver = spResultSet->GetValueString(10);

			std::map<int, std::vector<KCO2EquationData>>::iterator fIter = a_mapDefCO2EquationData.find(oData.nMasterCD);

			if (fIter == a_mapDefCO2EquationData.end())
			{
				std::vector<KCO2EquationData> vecData;
				vecData.push_back(oData);

				a_mapDefCO2EquationData.insert(std::make_pair(oData.nMasterCD, vecData));
			}
			else
			{
				std::vector<KCO2EquationData>& vecData = fIter->second;
				vecData.push_back(oData);
			}
		}

		if (a_mapDefCO2EquationData.size() == 0)
		{
			std::vector<KCO2EquationData> vecData;

			KCO2EquationData oData;

			oData.nMasterCD     = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeID       = 1;
			oData.strNameVeh    = _T("");
			oData.dPCE          = 0.0;
			oData.strNameFuel   = _T("");
			oData.dRatio        = 0.0;
			oData.dSpdGap       = 0.0;
			oData.bIsSelcted    = FALSE;
			oData.bIsSpdGapAllSelected = FALSE;
			oData.strExpUnder   = _T("");
			oData.strExpAndOver = _T("");

			vecData.push_back(oData);
			vecData.push_back(oData);
			vecData.push_back(oData);
			vecData.push_back(oData);
			vecData.push_back(oData);

			vecData.push_back(oData);
			vecData.push_back(oData);
			vecData.push_back(oData);

			oData.nModeID       = 2;

			vecData.push_back(oData);
			vecData.push_back(oData);

			oData.nModeID       = 3;

			vecData.push_back(oData);
			vecData.push_back(oData);

			oData.nModeID       = 4;

			vecData.push_back(oData);
			vecData.push_back(oData);

			a_mapDefCO2EquationData.insert(std::make_pair(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, vecData));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseDefaultCo2EmissionsEquation::UpdateDefCO2Equation( 
	std::map<int, std::map<int, CString>>& a_mapDefCO2EquationInfo, std::map<int, std::vector<KCO2EquationData>>& a_mapDefCO2EquationData )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	CString strQuery;

	try
	{
		spDBaseConnection->BeginTransaction();

		std::map<int, std::map<int, CString>>::iterator bInfoIter = a_mapDefCO2EquationInfo.begin();

		//Update Info
		while (bInfoIter != a_mapDefCO2EquationInfo.end())
		{
			strQuery.Format(_T("DELETE FROM Default_CO2_Equation_Info WHERE master_cd = %d"), bInfoIter->first);
			spDBaseConnection->ExecuteUpdate(strQuery);

			std::map<int, CString>::iterator bModeIter = bInfoIter->second.begin();

			while (bModeIter != bInfoIter->second.end())
			{
				strQuery.Format(_T("INSERT INTO Default_CO2_Equation_Info(master_cd, mode_id, mode_name) VALUES(%d, %d, '%s')"),
					bInfoIter->first, bModeIter->first, bModeIter->second);

				spDBaseConnection->ExecuteUpdate(strQuery);

				++bModeIter;
			}
			++bInfoIter;
		}

		//Update Data
		std::map<int, std::vector<KCO2EquationData>>::iterator bDataIter = a_mapDefCO2EquationData.begin();

		while (bDataIter != a_mapDefCO2EquationData.end())
		{
			strQuery.Format(_T("DELETE FROM Default_CO2_Equation_DATA WHERE master_cd = %d"), bDataIter->first);
			spDBaseConnection->ExecuteUpdate(strQuery);

			std::vector<KCO2EquationData> vecData = bDataIter->second;

			int nSize = vecData.size();

			for (int i = 0; i < nSize; ++i)
			{
				KCO2EquationData oData = vecData[i];

				strQuery = _T("");
				strQuery.AppendFormat(_T("INSERT INTO Default_CO2_Equation_Data("));
				strQuery.AppendFormat(_T(" master_cd, mode_id, vehicle_name, pce, fuel_name,"));
				strQuery.AppendFormat(_T(" ratio, spd_gap, is_selected, is_spd_gap_all_selected, exp_under,"));
				strQuery.AppendFormat(_T(" exp_andover"));
				strQuery.AppendFormat(_T(" ) Values("));
				strQuery.AppendFormat(_T(" %d, %d, '%s', %f, '%s',"), bDataIter->first, oData.nModeID, oData.strNameVeh, oData.dPCE, oData.strNameFuel);
				strQuery.AppendFormat(_T(" %f, %f, %d, %d, '%s',"), oData.dRatio, oData.dSpdGap, oData.bIsSelcted, oData.bIsSpdGapAllSelected, oData.strExpUnder);
				strQuery.AppendFormat(_T(" '%s')"), oData.strExpAndOver);
				
				spDBaseConnection->ExecuteUpdate(strQuery);
			}

			++bDataIter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}
