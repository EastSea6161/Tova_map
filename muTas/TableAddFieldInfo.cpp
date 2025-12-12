#include "StdAfx.h"
#include "TableAddFieldInfo.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumn.h"
#include "Mode.h"
#include "Purpose.h"
#include "Target.h"

KTableAddFieldInfo::KTableAddFieldInfo(void)
{
}


KTableAddFieldInfo::~KTableAddFieldInfo(void)
{
}


bool KTableAddFieldInfo::AddField(KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, bool bDefault)
{
	try
	{
		CWaitCursor cw;

		a_pIOTable->BeginTransaction();
		if(a_pIOTable->AddColumn(a_oAddColumn, bDefault) != NULL)
		{
			a_pIOTable->Commit();
		}
		else
		{
			throw 1;
		}
	}
	catch (...)
	{
		a_pIOTable->Rollback();

		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTableAddFieldInfo::AddModeField(KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KMode* a_pMode, bool bDefault)
{
	KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();

	try
	{
		CWaitCursor cw;

		a_pIOTable->BeginTransaction();
		if(a_pIOTable->AddColumn(a_oAddColumn, bDefault) != NULL)
		{
			if (nullptr != a_pMode)
			{
				UpdateModeChoiceModel(spDBaseConnection, a_pMode, a_oAddColumn);
			}
			a_pIOTable->Commit();
		}
		else
		{
			throw 1;
		}
	}
	catch (...)
	{
		a_pIOTable->Rollback();

		TxLogDebugException();
		return false;
	}

	return true;
}

bool KTableAddFieldInfo::AddModeField( KIOTable* a_pIOTable, KIOColumns& a_oAddColumns, KPurpose* a_pPurpose, bool bDefault /*= false*/ )
{
	KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();

	try
	{
		CWaitCursor cw;
		a_pIOTable->BeginTransaction();

		int nSize = a_oAddColumns.ColumnCount();
		
		if (nSize != 3)
			throw 1;

		for (int i = 0; i < nSize; ++i)
		{
			KIOColumn* pColumn = a_oAddColumns.GetColumn(i);

			if (a_pIOTable->AddColumn(*pColumn, bDefault) == nullptr)
			{
				TxLogDebugException();
				throw 1;
			}			
		}
		
		UpdateItemLogitModel(spDBaseConnection, a_pPurpose, a_oAddColumns);

		a_pIOTable->Commit();
	}
	catch (...)
	{
		a_pIOTable->Rollback();
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KTableAddFieldInfo::AddChainField( KIOTable* a_pIOTable, KIOColumns& a_oAddColumns, KPurpose* a_pPurpose, bool bDefault /*= false*/ )
{
	KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();

	try
	{
		CWaitCursor cw;
		a_pIOTable->BeginTransaction();

		int nSize = a_oAddColumns.ColumnCount();

		if (nSize != 2)
			throw 1;

		for (int i = 0; i < nSize; ++i)
		{
			KIOColumn* pColumn = a_oAddColumns.GetColumn(i);

			if (a_pIOTable->AddColumn(*pColumn, bDefault) == nullptr)
			{
				TxLogDebugException();
				throw 1;
			}			
		}

		UpdateChainModel(spDBaseConnection, a_pPurpose, a_oAddColumns);

		a_pIOTable->Commit();
	}
	catch (...)
	{
		a_pIOTable->Rollback();
		TxLogDebugException();
		return false;
	}

	return true;
}


void KTableAddFieldInfo::UpdateModeChoiceModel(KDBaseConPtr spDBaseConnection, KMode* a_pMode, KIOColumn& a_oIOColumn)
{
	TxLogDebugStartMsg();

	try
	{
		CTime   time    = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strInsertSQL(_T(""));

		strInsertSQL.Format(_T(" Insert into modechoice_model(mode_group_code, detail_mode_id, mode_od_column_name, mode_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
			a_pMode->ModeGroup(),  a_pMode->ModeID(), a_oIOColumn.Name(), _T(" (select ifnull(max(mode_run_seq)+1, 1) from modechoice_model) " ),  strTime, strTime  );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


bool KTableAddFieldInfo::AddPurposeField(KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KPurpose* a_pPurpose, bool bDefault)
{
	KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();

	try
	{
		CWaitCursor cw;

		a_pIOTable->BeginTransaction();
		if(a_pIOTable->AddColumn(a_oAddColumn, bDefault) != NULL)
		{
			if (nullptr != a_pPurpose)
			{
				UpdateDistributionModel(spDBaseConnection, a_pPurpose, a_oAddColumn);
			}
			a_pIOTable->Commit();
		}
		else
		{
			throw 1;
		}
	}
	catch (...)
	{
		a_pIOTable->Rollback();

		TxLogDebugException();
		return false;
	}

	return true;
}


void KTableAddFieldInfo::UpdateDistributionModel(KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumn& a_oIOColumn)
{
	TxLogDebugStartMsg();

	try
	{
		CTime   time    = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strInsertSQL(_T(""));

		strInsertSQL.Format(_T(" Insert into Distribution_Model(object_group_code, detail_object_id, purpose_od_column_name, distribution_run_seq, start_date, end_date)  Values ( %d, %d, '%s', %s, '%s', '%s' ) "), 
			a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(),  a_oIOColumn.Name(), _T(" (select ifnull(max(distribution_run_seq)+1, 1) from Distribution_Model) " ),  strTime, strTime  );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}


bool KTableAddFieldInfo::AddZoneField(KIOTable* a_pIOTable, KIOColumn &a_oAddColumn, KPurpose* a_pPurpose,  CString a_strPAFlag, bool bDefault)
{
	KDBaseConPtr spDBaseConnection = a_pIOTable->GetDBaseConnection();
    
	try
	{
		CWaitCursor cw;

		a_pIOTable->BeginTransaction();
		if(a_pIOTable->AddColumn(a_oAddColumn, bDefault) != NULL)
		{
			if (nullptr != a_pPurpose)
			{
				UpdateGenerationModel(spDBaseConnection, a_pPurpose, a_strPAFlag, a_oAddColumn);
			}
			a_pIOTable->Commit();
		}
		else
		{
			throw 1;
		}
	}
	catch (...)
	{
		a_pIOTable->Rollback();

		TxLogDebugException();
		return false;
	}

	return true;
}


void KTableAddFieldInfo::UpdateGenerationModel(KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, CString &a_strPAFlag, KIOColumn& a_oIOColumn)
{
	TxLogDebugStartMsg();

	try
	{
		CTime   time    = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strInsertSQL(_T(""));

		strInsertSQL.Format(_T(" Insert into Generation_Model(object_group_code, detail_object_id, pa_flag, zone_column_name, generation_run_seq, start_date, end_date)  Values ( %d, %d, '%s', '%s', %s, '%s', '%s' ) "), 
			a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(), a_strPAFlag, a_oIOColumn.Name(), _T(" (select ifnull(max(generation_run_seq)+1, 1) from Generation_Model) " ),  strTime, strTime  );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}

void KTableAddFieldInfo::UpdateVehicleModel( KTarget* a_pTarget, CString a_strColumnName, int a_nModeID )
{
	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Insert Into %s ( fmode_id, column_name ) Values ( '%d', '%s' )"), TABLE_FMODE_MODEL, a_nModeID, a_strColumnName);
		spDBaseConnection->ExecuteUpdate(strSQL);
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

void KTableAddFieldInfo::UpdateItemLogitModel( KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumns& a_oIOColumns )
{
	TxLogDebugStartMsg();

	try
	{
		CTime time = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strInsertSQL(_T(""));

		strInsertSQL.Format(_T("INSERT INTO itemlogit_model(mode_group_code, detail_mode_id, run_seq, start_date, end_date, direct_road, road, rail) Values ( %d, %d, %s, '%s', '%s', '%s', '%s', '%s' ) "), 
			a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(), _T(" (select ifnull(max(run_seq)+1, 1) from itemlogit_model) " ),  strTime, strTime, 
			a_oIOColumns.GetColumn(0)->Name(), a_oIOColumns.GetColumn(1)->Name(), a_oIOColumns.GetColumn(2)->Name()  );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KTableAddFieldInfo::UpdateChainModel( KDBaseConPtr spDBaseConnection, KPurpose* a_pPurpose, KIOColumns& a_oIOColumns )
{
	TxLogDebugStartMsg();

	try
	{
		CTime time = CTime::GetCurrentTime();
		CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strInsertSQL(_T(""));

		strInsertSQL.Format(_T("INSERT INTO chain_model(mode_group_code, detail_mode_id, run_seq, start_date, end_date, road, rail) Values ( %d, %d, %s, '%s', '%s', '%s', '%s' ) "), 
			a_pPurpose->PurposeGroup(),  a_pPurpose->PurposeID(), _T(" (select ifnull(max(run_seq)+1, 1) from chain_model) " ),  strTime, strTime, 
			a_oIOColumns.GetColumn(0)->Name(), a_oIOColumns.GetColumn(1)->Name() );

		spDBaseConnection->ExecuteUpdate(strInsertSQL);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}
