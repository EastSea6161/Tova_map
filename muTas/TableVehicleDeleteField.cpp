#include "StdAfx.h"
#include "TableVehicleDeleteField.h"
#include "Target.h"
#include "TableEditFieldInfo.h"

KTableVehicleDeleteField::KTableVehicleDeleteField(void)
{
}


KTableVehicleDeleteField::~KTableVehicleDeleteField(void)
{
}

void KTableVehicleDeleteField::DeleteVehicleFieldLogic( KTarget* a_pTarget, CString a_strName )
{
	CString strDelSQL(_T(""));
	CString strUpdateSQL(_T(""));
	CString strUpdateRunSQL(_T(""));

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	spDBaseConnection->BeginTransaction();
	try
	{
		
		TVehicleModelKey oInfo;
		KTableEditFieldInfo::GetVehicleModeInfo(spDBaseConnection , a_strName, oInfo);

		strDelSQL.Format(_T(" Delete From %s where column_name = '%s' "), TABLE_FMODE_MODEL, oInfo.strColumnName);
		spDBaseConnection->ExecuteUpdate(strDelSQL);

		strUpdateRunSQL.Format(_T("Update %s Set run_seq = 0 where run_seq = %d" ), TABLE_FMODE_MODEL, oInfo.nRunSeq);
		spDBaseConnection->ExecuteUpdate(strUpdateRunSQL);

		strUpdateSQL.Format(_T("Update %s Set small = '' , medium = '', large = '' where run_seq = '%d' "), TABLE_VEHICLECONV_MODEL, oInfo.nRunSeq);
		spDBaseConnection->ExecuteUpdate(strUpdateSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}
