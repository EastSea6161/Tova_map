#include "StdAfx.h"
#include "DBaseMatrixView.h"

#include "Target.h"

//^#include "DBaseConnector.h"

KDBaseMatrixView::KDBaseMatrixView(void)
{
}


KDBaseMatrixView::~KDBaseMatrixView(void)
{
}


bool KDBaseMatrixView::ODVectorRecord( KIOTable* a_pTable, CXTPReportColumns* a_pCXTPColumns, CString a_strStartUnit, CString a_strEndUnit, KSDoubleRecordArray& a_pRecordArray )
{
	TxLogDebugStartMsg();
	CString strQuery(_T("SELECT"));
	strQuery += _T(" B.") + a_strStartUnit;
	strQuery += _T(", C.") + a_strEndUnit;

	int nColumnCnt = a_pCXTPColumns->GetCount();
	for (int i = 2; i < nColumnCnt; i++)
	{
		CXTPReportColumn* pCXTPColumn = a_pCXTPColumns->GetAt(i);
		strQuery += _T(", SUM(") + pCXTPColumn->GetInternalName() + _T(")");
	}
	CString strTableName = a_pTable->Name();
	
	strQuery.Append(_T(" FROM ") + strTableName + _T(" A, ZONE B, ZONE C"));
	strQuery.Append(_T(" WHERE A.OZONE_ID = B.ZONE_ID AND A.DZONE_ID = C.ZONE_ID"));
	strQuery.Append(_T(" GROUP BY B.") + a_strStartUnit + _T(", C.") + a_strEndUnit);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();

		KResultSetPtr resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);
		if (NULL == resultSetPtr)
		{
			return false;
		}

		while( resultSetPtr->Next() )
		{
			KSDoubleRecord* pRecord = a_pRecordArray.AddNewRecord();
			for (int i = 0; i < nColumnCnt; ++i)
			{
				pRecord->SetAt(i, resultSetPtr->GetValueDouble(i));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();
	return true;
}


bool KDBaseMatrixView::GetListZoneGroup( KTarget* a_pTarget, CString a_strZoneGroup, std::vector<__int64>& a_vecZoneID )
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("SELECT %s FROM %s WHERE %s IS NOT NULL GROUP BY %s ORDER BY %s")
		, a_strZoneGroup, TABLE_ZONE, a_strZoneGroup, a_strZoneGroup, a_strZoneGroup);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while ( spResultSet->Next())
		{
			a_vecZoneID.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KDBaseMatrixView::ODMatrixRecord(KIOTable* a_pTable, CString a_strOZoneGroup, CString a_strDZoneGroup, CString a_strColumn, std::map<KODKey, double>& a_mapMatrixData)
{
	TxLogDebugStartMsg();
	CString strQuery(_T(""));
	strQuery.Format(_T("SELECT B.%s, C.%s, SUM(%s) FROM %s A, ZONE B, ZONE C WHERE A.OZONE_ID = B.ZONE_ID AND A.DZONE_ID = C.ZONE_ID GROUP BY B.%s, C.%s ORDER BY B.%s, C.%s")
		, a_strOZoneGroup, a_strDZoneGroup, a_strColumn, a_pTable->Name(), a_strOZoneGroup
		, a_strDZoneGroup, a_strOZoneGroup, a_strDZoneGroup);
	
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);
		if(NULL == resultSetPtr)
			return false;

		a_mapMatrixData.clear();

		while ( resultSetPtr->Next() )
		{
			KODKey matrixOD(resultSetPtr->GetValueInt(0), resultSetPtr->GetValueInt(1));
			a_mapMatrixData.insert(std::make_pair(matrixOD,  resultSetPtr->GetValueDouble(2)));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();
	return true;
}

void KDBaseMatrixView::GetZoneName( KTarget* a_pTarget, std::map<Integer, CString>& a_mapZoneName )
{
	TxLogDebugStartMsg();
	a_mapZoneName.clear();

	try
	{
		CString strQuery;
		strQuery.Format(_T("Select %s, %s From %s Order By %s"), COLUMN_ZONE_ID, COLUMN_ZONE_NAME, TABLE_ZONE, COLUMN_ZONE_ID);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		Integer nxZoneID;
		CString strZoneName;		

		while (spResultSet->Next())
		{
			nxZoneID    = spResultSet->GetValueInt64(0);
			strZoneName = spResultSet->GetValueString(1);

			if (strZoneName.IsEmpty())
				strZoneName = _T("(x)");

			a_mapZoneName.insert(std::make_pair(nxZoneID, strZoneName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	TxLogDebugEndMsg();
}
