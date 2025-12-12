#include "StdAfx.h"
#include "DBaseTableMgr.h"


#include "TableManager.h"

//^#include "DBaseConnector.h"

KDBaseTableMgr::KDBaseTableMgr(void)
{
}


KDBaseTableMgr::~KDBaseTableMgr(void)
{
}

void KDBaseTableMgr::NetworkResultView( int a_nSizeDoubleRecord,
	vector<TTargetTable> a_vecTargetTable, map<__int64, KSDoubleRecord*>& a_mapResultRecord )
{
	TxLogDebugStartMsg();

	int nCntChekedItem = a_vecTargetTable.size();
	int nIdxRecordArray(0);

	for (int i = 0; i < nCntChekedItem; ++i)
	{
		TTargetTable oTargetTable   = a_vecTargetTable[i];
		KIOTable*    pTable         = oTargetTable.pIOTable;
		CString      strTableName   = pTable->Name();
		CString      strQueryHeader(_T("")); 
		CString      strQuery(_T(""));

		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

		int nCntColumn = vecColumnInfo.size();

		for (int j = 0; j < nCntColumn; ++j)
		{
			if (strTableName.CompareNoCase(TABLE_NODE) == 0)
			{
				strQueryHeader = _T("select node_id ");
			}
			else if (strTableName.CompareNoCase(TABLE_LINK) == 0)
			{
				strQueryHeader = _T("select link_id ");
			}
			else if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
			{
				strQueryHeader = _T("select zone_id ");
			}
			else if (strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0)
			{
				strQueryHeader = _T("select zone_id ");
			}
			else
			{
				//정상적인 케이스가 아님.
				return;
			}
			TColumnInfo oColumnInfo = vecColumnInfo[j];

			KIOColumn* pColumn = oColumnInfo.pIOColumn;

			strQuery.Format(_T("%s, %s from %s"), strQueryHeader, pColumn->Name(), strTableName);
						
			try
			{				
                KDBaseConPtr spDBaseConnection = pTable->GetDBaseConnection();
				KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

				while (spResultSet->Next())
				{
					__int64 nxKeyID = spResultSet->GetValueInt64(0);

					map<__int64, KSDoubleRecord*>::iterator findIter = a_mapResultRecord.find(nxKeyID);
					map<__int64, KSDoubleRecord*>::iterator endIter  = a_mapResultRecord.end();

					KSDoubleRecord* pRecord = nullptr;

					if (findIter == endIter)
					{
						pRecord = new KSDoubleRecord(a_nSizeDoubleRecord);
						a_mapResultRecord.insert(make_pair(nxKeyID, pRecord));
					}
					else
					{
						pRecord = findIter->second;
					}
					pRecord->SetAt(nIdxRecordArray, spResultSet->GetValueDouble(1));
				}
			}
			catch (...)
			{
				AfxMessageBox(_T("DB Error"));
			}
			nIdxRecordArray++;
		}
	}
	TxLogDebugEndMsg();
}

void KDBaseTableMgr::MatrixResultView( 
	int a_nCntDoubleRecord, vector<TTargetTable> a_vecTargetTable, map<KODKey, KSDoubleRecord*>& a_mapResultRecord )
{
	int nCntTargetTable = a_vecTargetTable.size();
	int nIdxRecordArray(0);

	for (int i = 0; i < nCntTargetTable; ++i)
	{
		TTargetTable oTargetTable   = a_vecTargetTable[i];
		KIOTable*    pTable         = oTargetTable.pIOTable;
		CString      strTableName   = pTable->Name();
		CString      strQuery       = _T("");

		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;
		int nCntColumnInfo = vecColumnInfo.size();

		for (int j = 0; j < nCntColumnInfo; ++j)
		{
			//Table이 OD가 아닐경우에 대한 예외처리 추가진행하여야 함.
			TColumnInfo oColumnInfo = vecColumnInfo[j];
			KIOColumn*  pColumn     = oColumnInfo.pIOColumn;

			strQuery.Format(_T("select ozone_id, dzone_id, %s from %s"), pColumn->Name(), strTableName);

			try
			{				
                KDBaseConPtr spDBaseConnection = pTable->GetDBaseConnection();
				KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

				while (spResultSet->Next())
				{
					KODKey oODKey(spResultSet->GetValueInt64(0), spResultSet->GetValueInt64(1));

					map<KODKey, KSDoubleRecord*>::iterator findIter = a_mapResultRecord.find(oODKey);
					map<KODKey, KSDoubleRecord*>::iterator endIter  = a_mapResultRecord.end();

					KSDoubleRecord* pRecord = nullptr;

					if (findIter == endIter)
					{
						pRecord = new KSDoubleRecord(a_nCntDoubleRecord);
						a_mapResultRecord.insert(make_pair(oODKey, pRecord));
					}
					else
					{
						pRecord = findIter->second;
					}
					pRecord->SetAt(nIdxRecordArray, spResultSet->GetValueDouble(2));
				}
			}
			catch (...)
			{
				AfxMessageBox(_T("DB Error"));
			}
			nIdxRecordArray++;
		}
	}
}


void KDBaseTableMgr::GetMatrix2VectorValue( TColumnInfo oColumnInfo, KIOTable* a_pTable,KIDKeyDoubleRecords& a_records )
{
	try
	{
		a_records.RemoveAll();

		CString strSQLProduction(_T(""));
		strSQLProduction.Format(_T(" Select ozone_id, "));
		strSQLProduction.AppendFormat(_T("sum(%s) "), oColumnInfo.pIOColumn->Name());
		strSQLProduction.AppendFormat(_T(" From %s group by ozone_id"), a_pTable->Name());

		//KIDKeyDoubleRecords records(2);
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr       spPResultSet      = spDBaseConnection->ExecuteQuery(strSQLProduction);
		while (spPResultSet->Next())
		{
			Integer nxZoneID = spPResultSet->GetValueInt64(0);
			double  dValue	 = spPResultSet->GetValueDouble(1);

			KSDoubleRecord* pRecord = a_records.AddRecord(nxZoneID);
			pRecord->SetAt(0, dValue);
		}

		CString strSQLAttraction(_T(""));
		strSQLAttraction.Format(_T(" Select dzone_id, "));
		strSQLAttraction.AppendFormat(_T("sum(%s) "), oColumnInfo.pIOColumn->Name());
		strSQLAttraction.AppendFormat(_T(" From %s group by dzone_id"), a_pTable->Name());

		KResultSetPtr    spAResultSet = spDBaseConnection->ExecuteQuery(strSQLAttraction);
		while (spAResultSet->Next())
		{
			Integer nxZoneID = spAResultSet->GetValueInt64(0);
			double  dValue	 = spAResultSet->GetValueDouble(1);

			KSDoubleRecord* pRecord = a_records.AddRecord(nxZoneID);
			pRecord->SetAt(1, dValue);
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
