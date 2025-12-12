#include "StdAfx.h"
#include "DBaseDefaultLogistic.h"
#include "Target.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"

KDBaseDefaultLogistic::KDBaseDefaultLogistic(void)
{
}


KDBaseDefaultLogistic::~KDBaseDefaultLogistic(void)
{
}

void KDBaseDefaultLogistic::GetDefaultLogisticParameter( std::map<int, std::map<int, KLogisticParaInfo>>& ar_mapFLogisticPara )
{
	ar_mapFLogisticPara.clear();
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select master_code, object_id, ratio From %s Order by master_code, object_id"), KDefaultParaTable::TABLE_DEFAULT_LOGISTICPARA);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			KLogisticParaInfo oInfo;
			oInfo.TnMasterCode	 = spResultset->GetValueInt(0);
			oInfo.TnPurposeID	 = spResultset->GetValueInt(1);
			oInfo.TdRatio		 = spResultset->GetValueDouble(2);

			AutoType ofind = ar_mapFLogisticPara.find(oInfo.TnPurposeID);
			AutoType oend  = ar_mapFLogisticPara.end();
			if (ofind == oend)
			{
				std::map<int, KLogisticParaInfo> mapPara;
				mapPara.insert(std::make_pair(oInfo.TnMasterCode, oInfo));

				ar_mapFLogisticPara.insert(std::make_pair(oInfo.TnPurposeID, mapPara));
			}
			else
			{
				std::map<int, KLogisticParaInfo>& mapPara = ofind->second;

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

void KDBaseDefaultLogistic::SetDefaultLogisticParameter( std::vector<KLogisticParaInfo> a_vecLogisticPara )
{
	CString strDelSQL(_T(""));
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();
		strDelSQL.Format(_T(" Delete From %s Where master_code = %d"), KDefaultParaTable::TABLE_DEFAULT_LOGISTICPARA, KDefaultParameterMasterCode::USER_MASETER_CODE );
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		for ( size_t i = 0; i < a_vecLogisticPara.size(); i++)
		{
			KLogisticParaInfo oInfo = a_vecLogisticPara[i];
			
			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s (master_code, object_id, ratio ) Values "),KDefaultParaTable::TABLE_DEFAULT_LOGISTICPARA );

			CString strUserSQL(_T(""));
			strUserSQL.AppendFormat(_T(" %s ('%d', '%d', '%f' ) "), strSQL
				, KDefaultParameterMasterCode::USER_MASETER_CODE, oInfo.TnPurposeID, oInfo.TdRatio);
			spDBaseConnection->ExecuteUpdate(strUserSQL);

			if (oInfo.TnMasterCode == KDefaultParameterMasterCode::NEW_MASTER_CODE)
			{
				CString strSystemSQL(_T(""));
				strSystemSQL.AppendFormat(_T(" %s ('%d', '%d', '%f' ) "), strSQL
					, KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, oInfo.TnPurposeID, oInfo.TdRatio);
				spDBaseConnection->ExecuteUpdate(strSystemSQL);
			}
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw -1;
	}

	spDBaseConnection->Commit();
}
