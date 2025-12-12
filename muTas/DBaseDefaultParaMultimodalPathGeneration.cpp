#include "stdafx.h"
#include "DBaseDefaultParaMultimodalPathGeneration.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

void KDBaseDefaultParaMultimodalPathGeneration::LoadDefaultMultimodalPathInfo(std::map<int, KMultimodalPathGenerationInfo>& a_mapDefaultMultimodalPathInfo)
{
	TxLogDebugStartMsg();

	a_mapDefaultMultimodalPathInfo.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" SELECT "));
		strSQL.AppendFormat(_T(" Master_Code, Max_Routes, DwellTime_Bus, DwellTime_Rail, WalkTime"));
		strSQL.AppendFormat(_T(" FROM %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_PATH_GENERATION);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KMultimodalPathGenerationInfo oMultimodalPathGenerationInfo;

		while(pResult->Next())
		{
			int nMasterCode									= pResult->GetValueInt    (0);
			oMultimodalPathGenerationInfo.nMax_Routes		= pResult->GetValueInt    (1);
			oMultimodalPathGenerationInfo.dDwellTimeBus		= pResult->GetValueDouble (2);
			oMultimodalPathGenerationInfo.dDwellTimeRail	= pResult->GetValueDouble (3);
			oMultimodalPathGenerationInfo.nWalkTime			= pResult->GetValueInt    (4);

			a_mapDefaultMultimodalPathInfo.insert(std::make_pair(nMasterCode, oMultimodalPathGenerationInfo));
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