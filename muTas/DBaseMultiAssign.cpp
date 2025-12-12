#include "StdAfx.h"
#include "DBaseMultiAssign.h"

#include "DBaseConnector.h"

KDBaseMultiAssign::KDBaseMultiAssign(void)
{
}


KDBaseMultiAssign::~KDBaseMultiAssign(void)
{
}

bool KDBaseMultiAssign::IsExistName( CString a_strName )
{
	bool    bResult = false;
	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM multi_assign_info WHERE name = '%s'"), a_strName);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

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

bool KDBaseMultiAssign::IsExistNameExcludeCurrent( int a_nID, CString a_strName )
{
	bool    bResult = false;
	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM multi_assign_info WHERE name = '%s' AND ID != %d"), a_strName, a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

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

int KDBaseMultiAssign::InsertMutliInfo( CString a_strName, CString a_strModes )
{
	CString strQuery;
	int     nID(0);

	try
	{
		strQuery = _T("SELECT IFNULL(MAX(ID) + 1, 1) FROM multi_assign_info");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nID = spResultSet->GetValueInt(0);

		strQuery.Format(_T("INSERT INTO multi_assign_info(id, name, modes) VALUES(%d, '%s', '%s')"),
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

void KDBaseMultiAssign::UpdateMultiInfo( int a_nID, CString a_strName, CString a_strModes )
{
	CString strQuery;
	
	try
	{

		strQuery.Format(_T("UPDATE multi_assign_info set name = '%s', modes = '%s' WHERE id = %d"),
			a_strName, a_strModes, a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		
		spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("DELETE FROM multi_assign_data WHERE id = %d"), a_nID);

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



void KDBaseMultiAssign::InsertMultiData( int a_nID, CString a_strScenario, CString a_strTarget, CString a_strData )
{
	CString strQuery;

	try
	{
		strQuery.Format(_T("INSERT INTO multi_assign_data(id, scenario, target, data) VALUES(%d, '%s', '%s', '%s')"),
			a_nID, a_strScenario, a_strTarget, a_strData);

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

int KDBaseMultiAssign::NextMultiInfoID( void )
{
	CString strQuery;
	int     nID(0);

	try
	{
		strQuery = _T("SELECT IFNULL(MAX(ID) + 1, 1) FROM multi_assign_info");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

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

void KDBaseMultiAssign::SelectMultiInfos( CString a_strModes, std::map<int, CString>& a_mapInfo )
{
	a_mapInfo.clear();

	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT id, name FROM multi_assign_info WHERE modes = '%s'"), a_strModes);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

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

void KDBaseMultiAssign::SelectMultiDatas( int a_nID, std::vector<TMultiAssignData>& a_vecData )
{
	a_vecData.clear();

	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT scenario, target, data FROM multi_assign_data WHERE id = %d"), a_nID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TMultiAssignData oData = {};
			oData.strScenario = spResultSet->GetValueString(0);
			oData.strTarget   = spResultSet->GetValueString(1);
			oData.strData     = spResultSet->GetValueString(2);

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

void KDBaseMultiAssign::DeleteMultiAssign( int a_nID )
{
	CString strQuery;
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		
		spDBaseConnection->BeginTransaction();

		strQuery.Format(_T("DELETE FROM multi_assign_info WHERE id = %d"), a_nID);
		spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("DELETE FROM multi_assign_data WHERE id = %d"), a_nID);
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
