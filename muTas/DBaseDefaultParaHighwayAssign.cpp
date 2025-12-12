#include "StdAfx.h"
#include "DBaseDefaultParaHighwayAssign.h"
#include "DBaseConnector.h"
#include "DBaseAssignment.h"
#include "Mode.h"
#include "ModeManager.h"
#include "DefineNamesDefaultPara.h"

void KDBaseDefaultParaHighwayAssign::LoadDefaultOccupancyPCEMode(std::map<int, std::vector<KOccupancyPCEMode>>& a_mapDefaultOccupancyPCEMode)
{
	TxLogDebugStartMsg();

	a_mapDefaultOccupancyPCEMode.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		        strSQL.Format (_T(" Select master_code, detail_mode_id, mode_name, occupancy, pce From %s"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_PCE_MODE);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);		
		while( spResultSet->Next() )
		{
			KOccupancyPCEMode oOccupancyPCEMode;
				int nMasterCode					= spResultSet->GetValueInt    (0);
				oOccupancyPCEMode.nModeID		= spResultSet->GetValueInt	  (1);
				oOccupancyPCEMode.strModeName	= spResultSet->GetValueString (2);
				oOccupancyPCEMode.dOccupancy	= spResultSet->GetValueDouble (3); 
				oOccupancyPCEMode.dPCE			= spResultSet->GetValueDouble (4);  

			AutoType iter = a_mapDefaultOccupancyPCEMode.find(nMasterCode);
		    AutoType end  = a_mapDefaultOccupancyPCEMode.end();

			if (iter == end)
			{
				std::vector<KOccupancyPCEMode> vecOccupancyPCEMode;
				vecOccupancyPCEMode.push_back(oOccupancyPCEMode);
				a_mapDefaultOccupancyPCEMode.insert(std::make_pair(nMasterCode, vecOccupancyPCEMode));
			}
			else
			{
				std::vector<KOccupancyPCEMode>& vecOccupancyPCEMode = iter->second;
				vecOccupancyPCEMode.push_back(oOccupancyPCEMode);
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

void KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(std::map<int, KODTime>& a_mapDefaultODTime)
{
	TxLogDebugStartMsg();

	a_mapDefaultODTime.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
	        	strSQL.Format (_T(" SELECT Master_Code, Time_Duration, Travel_Time FROM %s"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_OD_TIME);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KODTime oODTime;

		while( pResult->Next() )
		{
			int nMasterCode				= pResult->GetValueInt    (0);
			oODTime.dTimeDuration		= pResult->GetValueDouble (1);
			oODTime.dTravelTime			= pResult->GetValueDouble (2);

			a_mapDefaultODTime.insert(std::make_pair(nMasterCode, oODTime));
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

void KDBaseDefaultParaHighwayAssign::LoadDefaultVDFPara(std::map<int, KVDFPara>& a_mapDefaultVDFPara)
{
	TxLogDebugStartMsg();

	a_mapDefaultVDFPara.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
				strSQL.AppendFormat(_T(" SELECT Master_Code, AllLink_Alpha, AllLink_Beta, "));
				strSQL.AppendFormat(_T(" UseFunction_ID, LinkColumn_Alpha, LinkColumn_Beta, LinkColumn_a, LinkColumn_b, VDFChoice "));
				strSQL.AppendFormat(_T("FROM %s"), KDefaultParaTable::TABLE_DEFAULT_HIGHWAY_ASSIGN_VDF_PARA);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KVDFPara oVDFPara;

		while( pResult->Next() )
		{
			int nMasterCode				= pResult->GetValueInt    (0);
			oVDFPara.dAllLinkAlpha      = pResult->GetValueDouble (1);
			oVDFPara.dAllLinkBeta		= pResult->GetValueDouble (2);
			oVDFPara.strUseFunctionID   = pResult->GetValueString (3);
			oVDFPara.strLinkColumnAlpha = pResult->GetValueString (4);
			oVDFPara.strLinkColumnBeta	= pResult->GetValueString (5);
			oVDFPara.strLinkColumnA		= pResult->GetValueString (6);
			oVDFPara.strLinkColumnB		= pResult->GetValueString (7);
			oVDFPara.nVDFChoice			= pResult->GetValueInt    (8);

			a_mapDefaultVDFPara.insert(std::make_pair(nMasterCode, oVDFPara));
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

void KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntion(std::map<int, std::vector<KVDFFunction>>& a_mapDefaultVDFFuntion)
{
	TxLogDebugStartMsg();

	a_mapDefaultVDFFuntion.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
				strSQL.Format (_T(" SELECT Function_Group_ID, Function_ID, alpha, beta, a, b FROM %s"), KDefaultParaTable::TABLE_DEFAULT_FUNCTION);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KVDFFunction oVDFFunction;

		while( pResult->Next() )
		{
			int nFunctionGroup	= pResult->GetValueInt   (0);
			oVDFFunction.nFn	= pResult->GetValueInt   (1);
			oVDFFunction.dAlpha = pResult->GetValueDouble(2);
			oVDFFunction.dBeta  = pResult->GetValueDouble(3);
			oVDFFunction.dA     = pResult->GetValueDouble(4);
			oVDFFunction.dB     = pResult->GetValueDouble(5);

			AutoType find = a_mapDefaultVDFFuntion.find(nFunctionGroup);
			AutoType end  = a_mapDefaultVDFFuntion.end();

			if (find == end)
			{
				std::vector<KVDFFunction> vecVDFFunction;
				vecVDFFunction.push_back(oVDFFunction);
				a_mapDefaultVDFFuntion.insert(std::make_pair(nFunctionGroup, vecVDFFunction));
			}
			else
			{
				std::vector<KVDFFunction> & vecVDFFunction = find->second;
				vecVDFFunction.push_back(oVDFFunction);
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


void KDBaseDefaultParaHighwayAssign::LoadDefaultVDFFuntionGroup(std::map<CString, int>& a_mapDefaultVDFFuntionGroup)
{
	TxLogDebugStartMsg();

	a_mapDefaultVDFFuntionGroup.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
				strSQL.Format (_T(" SELECT Function_Group_ID, Function_Group_Name FROM %s"), KDefaultParaTable::TABLE_DEFAULT_FUNCTION_GROUP);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			int     nGroupID      = pResult->GetValueInt    (0);
			CString strGroupName  = pResult->GetValueString (1);
			
			a_mapDefaultVDFFuntionGroup.insert(std::make_pair(strGroupName, nGroupID));
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

void KDBaseDefaultParaHighwayAssign::GetModeInfo(KTarget* a_pTarget, std::map<int, CString>& a_mapGetModeinfo)
{
	try
	{
		a_mapGetModeinfo.clear();

		double dOccupancy(1.0);
		double dPCE(1.0);

		std::vector<KOccupancyPCEMode> vecOccupancyPCEMode;
		KOccupancyPCEMode oOccupancyPCEMode;

		KModeManager* pModeManager = a_pTarget->ModeManager();
		int nModeCount = pModeManager->GetModeCount();
		for(int i = 0; i < nModeCount; i++)
		{
			KMode* pManagerMode = pModeManager->GetMode(i);
			int nModeID = pManagerMode->ModeID();
			CString strModeName = pManagerMode->ModeName();

			a_mapGetModeinfo.insert(std::make_pair(nModeID, strModeName));
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

void KDBaseDefaultParaHighwayAssign::GetFModeInfo( KTarget* a_pTarget, std::map<int, CString>& a_mapGetModeInfo )
{
	CString strSQL(_T(""));
	a_mapGetModeInfo.clear();
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select fmode_id, fmode_name From %s Order by fmode_id"), TABLE_DETAIL_FMODE);

		KResultSetPtr spResultptr = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultptr->Next())
		{
			
			int		nFmode_id		= spResultptr->GetValueInt(0);
			CString strFomdeName	= spResultptr->GetValueString(1);

			a_mapGetModeInfo.insert(std::make_pair(nFmode_id, strFomdeName));
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