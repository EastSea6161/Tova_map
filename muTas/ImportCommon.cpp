#include "StdAfx.h"
#include "ImportCommon.h"
#include "TableRelationInfo.h"
#include "InitRelationProcess.h"
#include "Target.h"

void KImportCommon::DeleteNodeRelation(KDBaseConPtr spDBase)
{
	CString strQuery(_T(""));

	try 
	{
		strQuery.Format(_T("delete from %s"), TABLE_NODE);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXIDX_NODE);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXSHP_NODE);
		spDBase->ExecuteUpdate(strQuery);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	std::vector<CString> vecRelationTables;
	KTableRelationInfo::RelationTablesNode( vecRelationTables );

	for (size_t i= 0; i< vecRelationTables.size(); i++) {
		try 
		{
			strQuery.Format(_T("delete from %s"), vecRelationTables[i]);
			spDBase->ExecuteUpdate(strQuery);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}

	DeleteZoneRelation(spDBase);
	DeleteLinkRelation(spDBase);
	DeleteIntersectionRelation(spDBase);
}


void KImportCommon::DeleteZoneRelation(KDBaseConPtr spDBase)
{
	CString strQuery(_T(""));

	try 
	{
		strQuery.Format(_T("delete from %s"), TABLE_ZONE);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXIDX_ZONE);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXSHP_ZONE);
		spDBase->ExecuteUpdate(strQuery);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	std::vector<CString> vecRelationTables;
	KTableRelationInfo::RelationTablesZone( vecRelationTables );
	for (size_t i= 0; i< vecRelationTables.size(); i++) {
		try 
		{
			strQuery.Format(_T("delete from %s"), vecRelationTables[i]);
			spDBase->ExecuteUpdate(strQuery);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}
	
	try 
	{
		strQuery.Format(_T("delete from %s"), TABLE_ACCESS_LINE_STATION);
		spDBase->ExecuteUpdate(strQuery);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}


void KImportCommon::DeleteLinkRelation(KDBaseConPtr spDBase)
{
	CString strQuery(_T(""));

	try 
	{
		strQuery.Format(_T("delete from %s"), TABLE_LINK);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXIDX_LINK);
		spDBase->ExecuteUpdate(strQuery);

		strQuery.Format(_T("delete from %s"), TABLE_TXSHP_LINK);
		spDBase->ExecuteUpdate(strQuery);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	std::vector<CString> vecRelationTables;
	KTableRelationInfo::RelationTablesLink( vecRelationTables );
	for (size_t i= 0; i< vecRelationTables.size(); i++) {
		try 
		{
			strQuery.Format(_T("delete from %s"), vecRelationTables[i]);
			spDBase->ExecuteUpdate(strQuery);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}
}


void KImportCommon::DeleteIntersectionRelation(KDBaseConPtr spDBase)
{
	CString strQuery(_T(""));

	std::vector<CString> vecRelationTables;
	KTableRelationInfo::RelationTablesIntersecion( vecRelationTables );

	for (size_t i= 0; i< vecRelationTables.size(); i++) {
		try 
		{
			strQuery.Format(_T("delete from %s"), vecRelationTables[i]);
			spDBase->ExecuteUpdate(strQuery);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}
}


void KImportCommon::DeleteTransitInfoRelation(KDBaseConPtr spDBase)
{
	CString strQuery(_T(""));

	std::vector<CString> vecRelationTables;
	KTableRelationInfo::RelationTablesTransitInfo( vecRelationTables );

	for (size_t i= 0; i< vecRelationTables.size(); i++) {
		try 
		{
			strQuery.Format(_T("delete from %s"), vecRelationTables[i]);
			spDBase->ExecuteUpdate(strQuery);
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}
}


void KImportCommon::NotifyNodeRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();
	try
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_NODE);
		if (nullptr != pTable) {
			pTable->Notify();
		}

		std::vector<CString> vecRelationTables;
		KTableRelationInfo::RelationTablesNode( vecRelationTables );

		for (size_t i= 0; i< vecRelationTables.size(); i++)	{
			pTable = pIOTables->FindTable(vecRelationTables[i]);
			if (nullptr == pTable)
				continue;

			pTable->Notify();
		}

		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_NODE);

		NotifyZoneRelation(a_pTarget);
		NotifyLinkRelation(a_pTarget);
		NotifyIntersectionRelation(a_pTarget);

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KImportCommon::NotifyTurnRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();
	try
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_TURN);
		if (nullptr != pTable) {
			pTable->Notify();
		}

		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_TURN);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
	TxLogDebugEndMsg();
}

void KImportCommon::NotifyZoneRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();
	try
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_ZONE);
		if (nullptr != pTable) {
			pTable->Notify();
		}

		std::vector<CString> vecRelationTables;
		KTableRelationInfo::RelationTablesZone( vecRelationTables );
		for (size_t i= 0; i< vecRelationTables.size(); i++) {

			pTable = pIOTables->FindTable(vecRelationTables[i]);
			if (nullptr != pTable) {
				pTable->Notify();
			}
		}

		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_ZONE);

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KImportCommon::NotifyLinkRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();
	try
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_LINK);
		if (nullptr != pTable) {
			pTable->Notify();
		}

		std::vector<CString> vecRelationTables;
		KTableRelationInfo::RelationTablesLink( vecRelationTables );

		for (size_t i= 0; i< vecRelationTables.size(); i++) {
			pTable = pIOTables->FindTable(vecRelationTables[i]);
			if (nullptr != pTable)
				pTable->Notify();
		}

		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_LINK);
		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_TURN);

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KImportCommon::NotifyIntersectionRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();
	
	try
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = nullptr;

		std::vector<CString> vecRelationTables;
		KTableRelationInfo::RelationTablesIntersecion( vecRelationTables );

		for (size_t i= 0; i< vecRelationTables.size(); i++)	{
			pTable = pIOTables->FindTable(vecRelationTables[i]);
			if (nullptr != pTable)
				pTable->Notify();
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KImportCommon::NotifyTransitInfoRelation(KTarget* a_pTarget)
{
	TxLogDebugStartMsg();

	try 
	{
		KIOTables* pIOTables = a_pTarget->Tables();
		KIOTable*  pTable    = nullptr;

		pTable = pIOTables->FindTable(TABLE_TRANSIT);
		if (nullptr != pTable)
			pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_TRANSITNODES);
		if (nullptr != pTable)
			pTable->Notify();

		KInitRelationProcess::DBRecordNotify(a_pTarget, TABLE_TRANSIT);

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

bool KImportCommon::IsNodeRelationColumn(CString a_strColunmName)
{
	if( a_strColunmName.CompareNoCase(COLUMN_FNODE_ID)    == 0 || 
		a_strColunmName.CompareNoCase(COLUMN_TNODE_ID)    == 0 ||
		a_strColunmName.CompareNoCase(COLUMN_TERMINAL_SCHEDULE_O_TERMINAL) == 0 ||
		a_strColunmName.CompareNoCase(COLUMN_TERMINAL_SCHEDULE_D_TERMINAL) == 0 ||
		a_strColunmName.CompareNoCase(COLUMN_NODE_ID) == 0) {
		return true;
	}

	return false;
}

bool KImportCommon::IsZoneRelationColumn(CString a_strColunmName)
{
	if( a_strColunmName.CompareNoCase(COLUMN_OZONE)    == 0 || 
		a_strColunmName.CompareNoCase(COLUMN_DZONE)    == 0 || 
		a_strColunmName.CompareNoCase(COLUMN_ZONE_ID)  == 0 ||
		a_strColunmName.CompareNoCase(COLUMN_PZONE_ID) == 0 || 
		a_strColunmName.CompareNoCase(COLUMN_CZONE_ID) == 0  ) {
		return true;
	}

	return false;
}

Integer KImportCommon::MaxLinkID( KDBaseConPtr spDBase, Integer a_nxStartID )
{
	CString strSQL(_T(""));
	Integer nxMaxLinkID = a_nxStartID-1;
	if (nxMaxLinkID < 0)
		nxMaxLinkID = 0;

	strSQL.Format(_T(" Select ifnull(max(link_id), '%I64d') as MaxID From Link Where link_id >= '%I64d' "), nxMaxLinkID, nxMaxLinkID);

	KResultSetPtr spResultSet = spDBase->ExecuteQuery(strSQL);
	if ( spResultSet->Next() ) {
		nxMaxLinkID = spResultSet->GetValueInt64(0);
	}

	return nxMaxLinkID;
}

bool KImportCommon::IsEmptyTable(KTarget* a_pTarget, CString a_strTableName)
{
	try 
	{
		KDBaseConPtr spDBase = a_pTarget->GetDBaseConnection();

		CString strQuery(_T(""));
		strQuery.Format(_T("select count(1) from %s"), a_strTableName);

		KResultSetPtr spResult = spDBase->ExecuteQuery(strQuery);
		Integer       nxDataCount(0);

		if(spResult->Next())
			nxDataCount = spResult->GetValueInt64(0);

		if(nxDataCount == 0)
			return true;

		return false;
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		CString strMsg(_T(""));
		strMsg.Format(_T("%s 테이블 Empty 체크 실패"), a_strTableName);
		ThrowException(strMsg);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		ThrowException(ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();

		CString strMsg(_T(""));
		strMsg.Format(_T("%s 테이블 Empty 체크 실패"), a_strTableName);
		ThrowException(strMsg);
	}

	return false;
}