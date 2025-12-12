#include "stdafx.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

void KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(std::map<int, KPathGenerateConstraint>& a_mapDefaultPathGenerateConstraint)
{
	TxLogDebugStartMsg();

	a_mapDefaultPathGenerateConstraint.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" SELECT "));
		strSQL.AppendFormat(_T(" Master_Code, PathTime, InvehicleTime, WaitingTime, TransferTime, AETime, PathLength, PathCost "));
		strSQL.AppendFormat(_T(" FROM %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KPathGenerateConstraint oPathGenerateConstraint;

		while(pResult->Next())
		{
			int nMasterCode							= pResult->GetValueInt    (0);
			oPathGenerateConstraint.dPathTime		= pResult->GetValueDouble (1);
			oPathGenerateConstraint.nInvehicleTime	= pResult->GetValueInt	  (2);
			oPathGenerateConstraint.nWatingTime		= pResult->GetValueInt	  (3);
			oPathGenerateConstraint.nTransferTime	= pResult->GetValueInt    (4);
			oPathGenerateConstraint.nAETime			= pResult->GetValueInt	  (5);
			oPathGenerateConstraint.dPathLength		= pResult->GetValueDouble (6);
			oPathGenerateConstraint.dPathCost		= pResult->GetValueDouble (7);


			AutoType find = a_mapDefaultPathGenerateConstraint.find(nMasterCode);
			AutoType end  = a_mapDefaultPathGenerateConstraint.end();

			if ( find == end)
			{

			}

			a_mapDefaultPathGenerateConstraint.insert(std::make_pair(nMasterCode, oPathGenerateConstraint));
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

void KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateInvehicle(std::map<int, KPathGenerateInvehicle>& a_mapDefaultPathGenerateInvehicle)
{
	TxLogDebugStartMsg();

	a_mapDefaultPathGenerateInvehicle.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" SELECT "));
		strSQL.AppendFormat(_T(" Master_Code, Alpha, Beta "));
		strSQL.AppendFormat(_T(" FROM %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_INVEHICLE);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KPathGenerateInvehicle oPathGenerateInvehicle;

		while(pResult->Next())
		{
			int nMasterCode						= pResult->GetValueInt    (0);
			oPathGenerateInvehicle.dAlpha		= pResult->GetValueDouble (1);
			oPathGenerateInvehicle.dBeta		= pResult->GetValueDouble (2);

			a_mapDefaultPathGenerateInvehicle.insert(std::make_pair(nMasterCode, oPathGenerateInvehicle));
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

bool KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateCheck()
{
	int nCount(0);

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select Count(*) From %s"), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
		  nCount = pResult->GetValueInt(0);
		}
		if (0 ==nCount)
		{
			return false;
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
	return true;
}