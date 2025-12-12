#include "StdAfx.h"
#include "Target.h"
#include "DBaseDefaultDistribution.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"


KDBaseDefaultDistribution::KDBaseDefaultDistribution(void)
{
}


KDBaseDefaultDistribution::~KDBaseDefaultDistribution(void)
{
}

void KDBaseDefaultDistribution::GetDefaultDistributionParameter(std::map<int, std::map<int, KDistributionParaInfo>>& ar_mapDestributionPara )
{
	ar_mapDestributionPara.clear();

	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Select master_code, object_id, function_type, alpha, beta, theta, rij From %s Order by master_code, object_id "), KDefaultParaTable::TABLE_DEFAULT_TRIP_DESTRIBUTION);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			KDistributionParaInfo oInfo;
			oInfo.TnMasterCode		= spResultset->GetValueInt(0);
			oInfo.TnPurposeID		= spResultset->GetValueInt(1);
			oInfo.TnFunctionType	= spResultset->GetValueInt(2);
			oInfo.TdParameterAlpa	= spResultset->GetValueDouble(3);
			oInfo.TdParameterBeta	= spResultset->GetValueDouble(4);
			oInfo.TdParameterTheta	= spResultset->GetValueDouble(5);
			oInfo.TstrVariable		= spResultset->GetValueString(6);

			AutoType ofind = ar_mapDestributionPara.find(oInfo.TnPurposeID);
			AutoType oend  = ar_mapDestributionPara.end();
			if (ofind == oend)
			{
				std::map<int, KDistributionParaInfo> mapPara;
				mapPara.insert(std::make_pair(oInfo.TnMasterCode, oInfo));

				ar_mapDestributionPara.insert(std::make_pair(oInfo.TnPurposeID, mapPara));
			}
			else
			{
				std::map<int, KDistributionParaInfo>& mapPara = ofind->second;

				AutoType mfind = mapPara.find(oInfo.TnMasterCode);
				AutoType mend  = mapPara.end();
				if (mfind == mend)
				{
					mapPara.insert(std::make_pair(oInfo.TnMasterCode, oInfo));
				}
				else
				{
					mapPara.erase(oInfo.TnMasterCode);
					mapPara.insert(std::make_pair(oInfo.TnMasterCode, oInfo));
				}
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

void KDBaseDefaultDistribution::SetDefaultDistributionParameter( std::vector<KDistributionParaInfo> a_vecResultPara )
{
	CString strDelSQL(_T(""));
	KDBaseConPtr spDBaseConnetion = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnetion->BeginTransaction();
		strDelSQL.Format(_T("Delete From %s Where master_code = %d"), KDefaultParaTable::TABLE_DEFAULT_TRIP_DESTRIBUTION, KDefaultParameterMasterCode::USER_MASETER_CODE );
		spDBaseConnetion->ExecuteUpdate(strDelSQL);

		for (size_t i = 0; i < a_vecResultPara.size(); i++)
		{
			KDistributionParaInfo oInfo = a_vecResultPara[i];

			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s (master_code, object_id, function_type, alpha, beta, theta, rij)"),KDefaultParaTable::TABLE_DEFAULT_TRIP_DESTRIBUTION );
			strSQL.AppendFormat(_T(" Values "));

			CString strUserSQL(_T(""));
			strUserSQL.AppendFormat(_T(" %s ( '%d', '%d', '%d', '%f', '%f', '%f', '%s' ) "), strSQL 
				, KDefaultParameterMasterCode::USER_MASETER_CODE, oInfo.TnPurposeID, oInfo.TnFunctionType, oInfo.TdParameterAlpa, oInfo.TdParameterBeta, oInfo.TdParameterTheta, oInfo.TstrVariable);
			spDBaseConnetion->ExecuteUpdate(strUserSQL);

			if (oInfo.TnMasterCode == KDefaultParameterMasterCode::NEW_MASTER_CODE)
			{
				CString strSystemSQL(_T(""));
				strSystemSQL.AppendFormat(_T(" %s ( '%d', '%d', '%d', '%f', '%f', '%f', '%s' ) "), strSQL 
					, KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, oInfo.TnPurposeID, oInfo.TnFunctionType, oInfo.TdParameterAlpa, oInfo.TdParameterBeta, oInfo.TdParameterTheta, oInfo.TstrVariable);
				spDBaseConnetion->ExecuteUpdate(strSystemSQL);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnetion->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnetion->RollBack();
	}
	spDBaseConnetion->Commit();
}
