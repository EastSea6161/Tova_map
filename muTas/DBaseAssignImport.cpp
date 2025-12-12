#include "StdAfx.h"
#include "DBaseAssignImport.h"
#include "Target.h"
#include "Mode.h"


KDBaseAssignImport::KDBaseAssignImport(void)
{
}


KDBaseAssignImport::~KDBaseAssignImport(void)
{
}

bool KDBaseAssignImport::GetSystemCode( KTarget* a_pTarget, int& ar_nSystemCode )
{
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select system from %s"), TABLE_IMTASVERSION);
		KDBaseConPtr spDBaseConnection	= a_pTarget->GetDBaseConnection();
		KResultSetPtr spReusltset				= spDBaseConnection->ExecuteQuery(strSQL);

		if (spReusltset->Next())
		{
			ar_nSystemCode = spReusltset->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KDBaseAssignImport::GetZoneInfo( KTarget* a_pTarget, std::set<Integer>& ar_setZoneInfo )
{
	CString strSQL(_T(""));
	ar_setZoneInfo.clear();

	try
	{
		strSQL.Format(_T("Select zone_id from %s Order by zone_id"), TABLE_ZONE);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr		spResultset		  = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			Integer nxZoneID = spResultset->GetValueInt64(0);
			ar_setZoneInfo.insert(nxZoneID);
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
	return true;
}

void KDBaseAssignImport::SetDetailFmodeTable( KDBaseConPtr& spDBaseConnetion, std::vector<KAImportInfo> a_vecImportInfo )
{
	CString strDelFmode(_T(""));
	CString strInsertSQL(_T(""));

	try
	{

// 		strDelFmode.Format(_T("Delete From %s where system_type = %d"), TABLE_DETAIL_FMODE, KEMPASSENGER_SYSTEM);
// 		spDBaseConnetion->ExecuteUpdate(strDelFmode);

		strInsertSQL.Format(_T(" Insert Into %s "), TABLE_DETAIL_FMODE);
		strInsertSQL.AppendFormat(_T(" ( fmode_id, fmode_name, system_type )"));
		strInsertSQL.AppendFormat(_T(" Values "));
		strInsertSQL.AppendFormat(_T(" ( ?, ?, ? )"));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strInsertSQL);

		size_t nCount = a_vecImportInfo.size();
		for (size_t i = 0; i < nCount; i++)
		{
			KAImportInfo oInfo = a_vecImportInfo[i];

			if (oInfo.bIsNewMode == true)
			{
				spPrepareStatement->BindInt(1, ( oInfo.nModeID ) );
				spPrepareStatement->BindText(2, oInfo.strModeName);
				spPrepareStatement->BindInt(3, KEMPASSENGER_SYSTEM);

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();
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

void KDBaseAssignImport::SetFModeModelTable( KDBaseConPtr& spDBaseConnetion, std::vector<KAImportInfo> a_vecImportInfo, std::vector<CString>& ar_vecDelColumn )
{

	ar_vecDelColumn.clear();

	CString strDelSQL(_T(""));
	CString strInsertSQL(_T(""));
	CString strDelSelSQL(_T(""));

	try
	{
		//기존 여객 정보 컬럼명가져오기 (삭제위한)
// 		strDelSelSQL.Format(_T("Select Column_name From %s Where fmode_id In (select fmode_id from %s where system_type = %d )"),
// 			TABLE_FMODE_MODEL, TABLE_DETAIL_FMODE,KEMPASSENGER_SYSTEM );
// 
// 		KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strDelSelSQL);
// 		while(spResultset->Next())
// 		{
// 			CString strColumnName = spResultset->GetValueString(0);
// 			ar_vecDelColumn.push_back(strColumnName);
// 		}
// 
// 		// 기존 여객 정보 삭제
// 		strDelSQL.Format(_T("Delete From %s Where fmode_id In (Select fmode_id From %s Where system_type = %d )"), 
// 			TABLE_FMODE_MODEL, TABLE_DETAIL_FMODE, KEMPASSENGER_SYSTEM);
// 		spDBaseConnetion->ExecuteUpdate(strDelSQL);

		//새로운 여객 정보 추가
		strInsertSQL.Format(_T(" Insert Into %s "), TABLE_FMODE_MODEL);
		strInsertSQL.AppendFormat(_T(" ( fmode_id, column_name)"));
		strInsertSQL.AppendFormat(_T(" Values "));
		strInsertSQL.AppendFormat(_T(" ( ?, ?)"));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strInsertSQL);

		size_t nCount = a_vecImportInfo.size();
		for (size_t i = 0; i < nCount; i++)
		{
			KAImportInfo oInfo = a_vecImportInfo[i];

			spPrepareStatement->BindInt(1, ( oInfo.nModeID) );
			spPrepareStatement->BindText(2, oInfo.pCurrentColumn->Name());

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
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



void KDBaseAssignImport::InsertImportData( KDBaseConPtr& spCurrentConnection, KTarget* a_pImportTarget, std::vector<KAImportInfo> a_vecImportInfo )
{
	CString strAttachSQL(_T(""));
	CString strUpdateSQL(_T(""));
	CString strADBName(_T("a"));

	try
	{

		CString strSetSQL(_T(""));
		size_t nCount = a_vecImportInfo.size();
		for (size_t i = 0; i < nCount; i++)
		{
			KAImportInfo oInfo = a_vecImportInfo[i];
			CString strMemeberSQL(_T(""));

			CString strCurrentColumn = oInfo.pCurrentColumn->Name();
			CString strImportColumn  = oInfo.pImportColumn->Name();

			strMemeberSQL.AppendFormat(_T(" %s = (Select %s From %s.%s Where %s.%s.ozone_id = %s.ozone_id and %s.%s.dzone_id = %s.dzone_id) "),
				strCurrentColumn, strImportColumn, strADBName,TABLE_MODE_OD,strADBName,TABLE_MODE_OD,TABLE_VEHICLE_MODE_OD, strADBName,TABLE_MODE_OD,TABLE_VEHICLE_MODE_OD);

			if (i == 0 )
			{
				strSetSQL.AppendFormat(_T(" %s "), strMemeberSQL);
			}
			else
			{ 
				strSetSQL.AppendFormat(_T(", %s "), strMemeberSQL);
			}
		}

		strUpdateSQL.Format(_T(" Update %s set %s "), TABLE_VEHICLE_MODE_OD, strSetSQL);
		spCurrentConnection->ExecuteUpdate(strUpdateSQL);

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

void KDBaseAssignImport::GetFmodeInfo( KTarget* a_pTarget, std::vector<KFAModeInfo>& ar_vecFAmodeInfo )
{

	ar_vecFAmodeInfo.clear();

	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select fmode_id, fmode_name, system_type From %s Order by fmode_id"), TABLE_DETAIL_FMODE);
		
		KResultSetPtr spResultptr = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultptr->Next())
		{
			KFAModeInfo oInfo;
			oInfo.nFmode_id		= spResultptr->GetValueInt(0);
			oInfo.strFomdeName	= spResultptr->GetValueString(1);
			oInfo.nSystemType	= spResultptr->GetValueInt(2);

			ar_vecFAmodeInfo.push_back(oInfo);
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

void KDBaseAssignImport::GetFmodeInfo( KTarget* a_pTarget, std::map<int, KFAModeInfo>& ar_mapFAmodeInfo )
{

	ar_mapFAmodeInfo.clear();

	CString strSQL(_T(""));
	
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select fmode_id, fmode_name, system_type From %s Order by fmode_id"), TABLE_DETAIL_FMODE);

		KResultSetPtr spResultptr = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultptr->Next())
		{
			KFAModeInfo oInfo;
			oInfo.nFmode_id		= spResultptr->GetValueInt(0);
			oInfo.strFomdeName	= spResultptr->GetValueString(1);
			oInfo.nSystemType	= spResultptr->GetValueInt(2);

			ar_mapFAmodeInfo.insert(std::make_pair(oInfo.nFmode_id, oInfo));
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

void KDBaseAssignImport::InsertFModeCode( KTarget* a_pTarget, std::map<int,KFAModeInfo> a_mapFmodeInfo )
{
	CString strDelFmode(_T(""));
	CString strInsertSQL(_T(""));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		strDelFmode.Format(_T("Delete From %s where system_type = %d"), TABLE_DETAIL_FMODE, KEMPASSENGER_SYSTEM);
		spDBaseConnection->ExecuteUpdate(strDelFmode);

		strInsertSQL.Format(_T(" Insert Into %s "), TABLE_DETAIL_FMODE);
		strInsertSQL.AppendFormat(_T(" ( fmode_id, fmode_name, system_type )"));
		strInsertSQL.AppendFormat(_T(" Values "));
		strInsertSQL.AppendFormat(_T(" ( ?, ?, ? )"));


		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strInsertSQL);

		AutoType iter = a_mapFmodeInfo.begin();
		AutoType end  = a_mapFmodeInfo.end();
		while(iter != end)
		{
			KFAModeInfo oInfo = iter->second;

			spPrepareStatement->BindInt(1, (oInfo.nFmode_id) );
			spPrepareStatement->BindText(2, oInfo.strFomdeName);
			spPrepareStatement->BindInt(3, KEMPASSENGER_SYSTEM);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++iter;
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


void KDBaseAssignImport::DelFmodeModelData( KTarget* a_pTarget)
{
	CString strSQL(_T(""));
	try
	{
		std::vector<KFAModeInfo> vecModeInfo;
		GetFmodeInfo(a_pTarget, vecModeInfo);

		CString strFindSQL(_T(""));
		size_t nCount = vecModeInfo.size();
		for (size_t i = 0; i <nCount; i++)
		{
			KFAModeInfo oInfo = vecModeInfo[i];
			if (i == 0)
			{
				strFindSQL.AppendFormat(_T(" '%d' "), oInfo.nFmode_id);
			}
			else
			{
				strFindSQL.AppendFormat(_T(" , '%d' "), oInfo.nFmode_id);
			}
		}
		
		strSQL.Format(_T(" Delete From %s Where fmode_id Not In ( %s )"), TABLE_FMODE_MODEL, strFindSQL);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
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
