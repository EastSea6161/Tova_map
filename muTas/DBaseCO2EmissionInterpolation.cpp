#include "StdAfx.h"
#include "DBaseCO2EmissionInterpolation.h"

#include "Target.h"

KDBaseCO2EmissionInterpolation::KDBaseCO2EmissionInterpolation(void)
{
}


KDBaseCO2EmissionInterpolation::~KDBaseCO2EmissionInterpolation(void)
{
}

bool KDBaseCO2EmissionInterpolation::IsEmptyAssignResult( KTarget* a_pTarget )
{
	CString strQuery;
	bool    bResult = true;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM %s"), TABLE_ASSIGN_MODEL);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 존재하지 않습니다."));

		if (spResultSet->GetValueInt(0) != 0)
			bResult = false;
		
		return bResult;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::GetTravelTime( KTarget* a_pTarget, std::map<int, CString>& a_mapTravelTime )
{
	a_mapTravelTime.clear();

	CString strQuery;

	try
	{
		strQuery =            _T("SELECT run_seq, ( ");
		strQuery.Append(      _T("  SELECT display_name FROM column_description "));
		strQuery.AppendFormat(_T("  WHERE lower(table_name) = '%s' AND lower(column_name) = lower(average_time)) as avg_time "), TABLE_LINK);
		strQuery.AppendFormat(_T("FROM %s WHERE average_time is not null AND trim(average_time) != '' ORDER BY run_seq"), TABLE_ASSIGN_MODEL);


		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int     nRunSeq   = spResultSet->GetValueInt(0);
			CString strColumn = spResultSet->GetValueString(1);

			a_mapTravelTime.insert(std::make_pair(nRunSeq, strColumn));
		}

		if (a_mapTravelTime.size() == 0)
			ThrowException(_T("데이터가 존재하지 않습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::GetModeVolumeColumn( KTarget* a_pTarget, int a_nRunSeq, std::vector<CString>& a_vecModeVolColumn )
{
	a_vecModeVolColumn.clear();

	CString strQuery, strValue;

	try
	{
		strQuery.Format(_T("SELECT mode_volume FROM %s WHERE run_seq = %d"), TABLE_ASSIGN_MODEL, a_nRunSeq);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		strValue = spResultSet->GetValueString(0);
		::ParseStringArr(strValue, a_vecModeVolColumn);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::GetLinkSpeed( KTarget* a_pTarget, int a_nIdxTravelTime, std::map<Integer, double>& a_mapLinkSpeed )
{
	a_mapLinkSpeed.clear();

	CString strQuery, strColTravelTime;

	try
	{
		//TravelTime의 컬럼 정보를 Get
		strQuery.Format(_T("select average_time from assign_model WHERE run_seq = %d"), a_nIdxTravelTime);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("Assign 결과 정보가 없습니다."));

		strColTravelTime = spResultSet->GetValueString(0);

		//각 링크별 속도 정보를 DB를 이용하여 Get
		strQuery.Format(_T("SELECT link_id, ifnull(length/%s * 60, 0) as spd FROM LINK"), strColTravelTime);

		spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapLinkSpeed.insert(std::make_pair(spResultSet->GetValueInt64(0), spResultSet->GetValueDouble(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::GetLinkSpeed( KTarget* a_pTarget, KIOColumn* a_pLinkIOColumn, std::map<Integer, double>& a_mapLinkSpeed )
{
	a_mapLinkSpeed.clear();

	CString strQuery, strColTravelTime;

	try
	{
		strColTravelTime = a_pLinkIOColumn->Name();

		//TravelTime의 컬럼 정보를 Get
		//각 링크별 속도 정보를 DB를 이용하여 Get
		strQuery.Format(_T("SELECT link_id, ifnull(length/%s * 60, 0) as spd FROM LINK"), strColTravelTime);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapLinkSpeed.insert(std::make_pair(spResultSet->GetValueInt64(0), spResultSet->GetValueDouble(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::GetLinkVolume( KTarget* a_pTarget, CString a_strColumnName, std::map<Integer, double>& a_mapLinkVolume )
{
	a_mapLinkVolume.clear();

	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT link_id, %s FROM LINK ORDER BY link_id"), a_strColumnName);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapLinkVolume.insert(std::make_pair(spResultSet->GetValueInt64(0), spResultSet->GetValueDouble(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;	
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseCO2EmissionInterpolation::UpdateResult( KTarget* a_pTarget, CString a_strColumnName, std::map<Integer, double> a_mapResult )
{
	CString strQuery;
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		strQuery.Format(_T("update link set %s = ? WHERE link_id = ?"), a_strColumnName);

		spDBaseConnection->BeginTransaction();
		
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strQuery);

		std::map<Integer, double>::iterator bIter = a_mapResult.begin();

		while (bIter != a_mapResult.end())
		{
			spPreparedStatement->BindDouble(1, bIter->second);
			spPreparedStatement->BindInt64(2, bIter->first);
			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();

			++bIter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;	
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw 1;
	}
}



void KDBaseCO2EmissionInterpolation::GetLinkLength(KTarget* a_pTarget, std::map<Integer, double>& a_mapLinkLength)
{
	a_mapLinkLength.clear();

	CString strQuery, strColTravelTime;

	try
	{
		strQuery = _T("SELECT link_id, ifnull(length, 0.0) as spd FROM LINK");

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			a_mapLinkLength.insert(std::make_pair(spResultSet->GetValueInt64(0), spResultSet->GetValueDouble(1)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}