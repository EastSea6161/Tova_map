#include "StdAfx.h"
#include "DBaseDefPolicyContainer.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"
#include "Target.h"

KDBaseDefPolicyContainer::KDBaseDefPolicyContainer(void)
{
}


KDBaseDefPolicyContainer::~KDBaseDefPolicyContainer(void)
{
}

void KDBaseDefPolicyContainer::GetDefaultPolicyContainerInfo( std::map<Integer, KPolicyContainerInfo>& ar_mapPContainerInfo )
{
	ar_mapPContainerInfo.clear();
	CString strSQL(_T(""));

	std::map<Integer, KPolicyContainerInfo> mapSystemInfo;
	std::map<Integer, KPolicyContainerInfo> mapUserInfo;

	try
	{
		strSQL.Format(_T(" Select master_code, zone_id, port_name, production, consumption From %s "), KDefaultParaTable::TABLE_DEFAULT_POLICY_CONTANIER);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			KPolicyContainerInfo oInfo;
			oInfo.TnMasterCode = spResultset->GetValueInt(0);
			oInfo.TnxZoneID	   = spResultset->GetValueInt64(1);
			oInfo.TstrPortName = spResultset->GetValueString(2);
			oInfo.TdProduction = spResultset->GetValueDouble(3);
			oInfo.TdConsumption= spResultset->GetValueDouble(4);

			if (oInfo.TnMasterCode == KDefaultParameterMasterCode::SYSTEM_MASTER_CODE)
			{
				mapSystemInfo.insert(std::make_pair(oInfo.TnxZoneID, oInfo));
			}
			else if (oInfo.TnMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
			{
				mapUserInfo.insert(std::make_pair(oInfo.TnxZoneID, oInfo));
			}
		}

		if (mapUserInfo.size() > 1)
		{
			ar_mapPContainerInfo = mapUserInfo;
		}
		else if (mapSystemInfo.size() > 1)
		{
			ar_mapPContainerInfo = mapSystemInfo;
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

void KDBaseDefPolicyContainer::GetAllZoneInfo( KTarget* a_pTarget, std::map<Integer, CString>& ar_mapZoneInfo )
{
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select zone_id, name From %s "),TABLE_ZONE );

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nxZoneID		= spResultset->GetValueInt64(0);
			CString strZoneName	= spResultset->GetValueString(1);

			ar_mapZoneInfo.insert(std::make_pair(nxZoneID, strZoneName));
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

bool KDBaseDefPolicyContainer::SaveContainerInfo( std::map<Integer, KPolicyContainerInfo> a_mapContainerInfo )
{
	CString strPrepareSQL(_T(""));
	CString strDelSQL(_T(""));

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	spDBaseConnection->BeginTransaction();

	try
	{
		strDelSQL.Format(_T("Delete From %s where master_code = %d "), KDefaultParaTable::TABLE_DEFAULT_POLICY_CONTANIER, KDefaultParameterMasterCode::USER_MASETER_CODE);

		strPrepareSQL.Format(_T(" Insert Into %s (master_code, zone_id, port_name, production, consumption) Values ( ?, ?, ?, ?, ?) "), 
			KDefaultParaTable::TABLE_DEFAULT_POLICY_CONTANIER);

		spDBaseConnection->ExecuteUpdate(strDelSQL);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPrepareSQL);

		AutoType iter = a_mapContainerInfo.begin();
		AutoType end  = a_mapContainerInfo.end();
		while(iter != end)
		{
			KPolicyContainerInfo oInfo = iter->second;

			spPrepareStatement->BindInt(1, KDefaultParameterMasterCode::USER_MASETER_CODE);
			spPrepareStatement->BindInt64(2, oInfo.TnxZoneID);
			spPrepareStatement->BindText(3, oInfo.TstrPortName);
			spPrepareStatement->BindDouble(4, oInfo.TdProduction);
			spPrepareStatement->BindDouble(5, oInfo.TdConsumption);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		return false;
	}

	return true;
}
