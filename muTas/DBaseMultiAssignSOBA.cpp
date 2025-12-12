#include "StdAfx.h"
#include "DBaseMultiAssignSOBA.h"

#include "DBaseConnector.h"



KDBaseMultiAssignSOBA::KDBaseMultiAssignSOBA(void)
{
}


KDBaseMultiAssignSOBA::~KDBaseMultiAssignSOBA(void)
{
}


bool KDBaseMultiAssignSOBA::CheckMultiAssignSOBATableExists()
{
	bool bExist = false;
	CString strSQL;
	strSQL = _T(
		" SELECT name FROM sqlite_master "
		" WHERE type = 'table' "
		" AND name IN('Multi_Assign_Info_Soba', 'Multi_Assign_Data_Soba')");

	try
	{
		KDBaseConPtr spDBConn = KDBase::GetProjectDBConnection();
		int nCount = 0;
		KResultSetPtr spResult = spDBConn->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
			CString strName = spResult->GetValueString(0);
			if ((strName.CompareNoCase(_T("Multi_Assign_Info_Soba")) == 0)
				|| (strName.CompareNoCase(_T("Multi_Assign_Data_Soba")) == 0))
			{
				++nCount;
			}
		}

		if (nCount == 2)
		{
			bExist = true;
		}
		
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}

	return bExist;
}


bool KDBaseMultiAssignSOBA::CreateMultiAssignSOBATable()
{
	try
	{
		KDBaseConPtr spDBConn = KDBase::GetProjectDBConnection();

		// Multi_Assign_Info_Soba 테이블 생성
		CString strSQLDrop;
		strSQLDrop = _T("DROP TABLE IF EXISTS Multi_Assign_Info_Soba");
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		CString strSQLCreate;
		strSQLCreate = _T(
			" CREATE TABLE `Multi_Assign_Info_Soba` ( "
			"	`ID`	INTEGER, "
			"	`NAME`	VARCHAR2, "
			"	`MODES`	VARCHAR2, "
			"	PRIMARY KEY(`ID`) "
			"	) ");

		spDBConn->ExecuteUpdate(strSQLCreate);

		// Multi_Assign_Data_Soba 테이블 생성
		strSQLDrop = _T("DROP TABLE IF EXISTS Multi_Assign_Data_Soba ");
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		strSQLCreate = _T(
			" CREATE TABLE `Multi_Assign_Data_Soba` ( "
			"	`ID`	INTEGER, "
			"	'NO'	INTEGER, "
			"	`Scenario`	VARCHAR2, "
			"	`Target`	VARCHAR2, "
			"	`Data`	VARCHAR2, "
			"	`WS_Scenario`	VARCHAR2, "
			"	`WS_Target`	VARCHAR2, "
			"	PRIMARY KEY(`ID`, 'NO') "
			"	) ");

		spDBConn->ExecuteUpdate(strSQLCreate);

	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}
	return true;
}



bool KDBaseMultiAssignSOBA::IsExistName(CString a_strName)
{
	bool    bResult = false;
	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM Multi_Assign_Info_Soba WHERE NAME = '%s'"), a_strName);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			if (spResultSet->GetValueInt(0) > 0)
				bResult = true;
		}

		return bResult;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

bool KDBaseMultiAssignSOBA::IsExistNameExcludeCurrent(int a_nID, CString a_strName)
{
	bool    bResult = false;
	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM Multi_Assign_Info_Soba WHERE NAME = '%s' AND ID != %d"), a_strName, a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			if (spResultSet->GetValueInt(0) > 0)
				bResult = true;
		}

		return bResult;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

int KDBaseMultiAssignSOBA::InsertMutliInfo(CString a_strName, CString a_strModes)
{
	CString strQuery;
	int     nID(0);

	try
	{
		strQuery = _T("SELECT IFNULL(MAX(ID) + 1, 1) FROM Multi_Assign_Info_Soba");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nID = spResultSet->GetValueInt(0);

		strQuery.Format(_T("INSERT INTO Multi_Assign_Info_Soba(ID, NAME, MODES) VALUES(%d, '%s', '%s')"),
			nID, a_strName, a_strModes);

		spDBaseConnection->ExecuteUpdate(strQuery);

		return nID;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

void KDBaseMultiAssignSOBA::UpdateMultiInfo(int a_nID, CString a_strName, CString a_strModes)
{
	CString strQuery;

	try
	{

		strQuery.Format(_T("UPDATE Multi_Assign_Info_Soba set NAME = '%s', MODES = '%s' WHERE ID = %d"),
			a_strName, a_strModes, a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("DELETE FROM Multi_Assign_Data_Soba WHERE ID = %d"), a_nID);

		spDBaseConnection->ExecuteUpdate(strQuery);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}



void KDBaseMultiAssignSOBA::InsertMultiData(int a_nID, int nNo, CString a_strScenario, CString a_strTarget, CString a_strData, 
	CString strWSScenario, CString strWSTarget)
{
	CString strQuery;

	try
	{
		strQuery.Format(_T("INSERT INTO Multi_Assign_Data_Soba(ID, NO, Scenario, Target, Data, WS_Scenario, WS_Target) "
			" VALUES(%d, %d, '%s', '%s', '%s', '%s', '%s')"),
			a_nID, nNo, a_strScenario, a_strTarget, a_strData, strWSScenario, strWSTarget );

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		spDBaseConnection->ExecuteUpdate(strQuery);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

int KDBaseMultiAssignSOBA::NextMultiInfoID(void)
{
	CString strQuery;
	int     nID(0);

	try
	{
		strQuery = _T("SELECT IFNULL(MAX(ID) + 1, 1) FROM Multi_Assign_Info_Soba");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nID = spResultSet->GetValueInt(0);

		return nID;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

void KDBaseMultiAssignSOBA::SelectMultiInfos(CString a_strModes, std::map<int, CString>& a_mapInfo)
{
	a_mapInfo.clear();

	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT ID, NAME FROM Multi_Assign_Info_Soba WHERE MODES = '%s'"), a_strModes);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapInfo.insert(std::make_pair(spResultSet->GetValueInt(0), spResultSet->GetValueString(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

void KDBaseMultiAssignSOBA::SelectMultiDatas(int a_nID, std::vector<TMultiAssignDataSOBA>& a_vecData)
{
	a_vecData.clear();

	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT NO, Scenario, Target, Data, WS_Scenario, WS_Target FROM Multi_Assign_Data_Soba WHERE ID = %d ORDER BY NO"), a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TMultiAssignDataSOBA oData = {};
			int nNo = spResultSet->GetValueInt(0);
			oData.strScenario = spResultSet->GetValueString(1);
			oData.strTarget = spResultSet->GetValueString(2);
			oData.strData = spResultSet->GetValueString(3);
			oData.strWSScenario = spResultSet->GetValueString(4);
			oData.strWSTarget = spResultSet->GetValueString(5);

			a_vecData.push_back(oData);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}

void KDBaseMultiAssignSOBA::DeleteMultiAssign(int a_nID)
{
	CString strQuery;
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{

		spDBaseConnection->BeginTransaction();

		strQuery.Format(_T("DELETE FROM Multi_Assign_Info_Soba WHERE ID = %d"), a_nID);
		spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("DELETE FROM Multi_Assign_Data_Soba WHERE ID = %d"), a_nID);
		spDBaseConnection->ExecuteUpdate(strQuery);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		AfxMessageBox(_T("DB 조회중 오류가 발생했습니다."));
		throw 1;
	}
}
