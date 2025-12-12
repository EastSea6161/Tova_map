#include "StdAfx.h"
#include "DBaseFieldCalculator.h"
#include "Target.h"
#include "MapView.h"
#include "ImChampFrameWindow.h"

KDBaseFieldCalculator::KDBaseFieldCalculator(void)
{
}

KDBaseFieldCalculator::~KDBaseFieldCalculator(void)
{
}

void KDBaseFieldCalculator::GetRecordSet( 
	KIOTable* a_pTable, vector<CString> a_vecCalcColumn, CString a_strFilter, 
	map<__int64, KSDoubleRecord*>& ar_mapResultRecord )
{
	ar_mapResultRecord.clear();

	CString strQuery = _T("Select RowID");

	int nSize = a_vecCalcColumn.size();

	for (int i = 0; i < nSize; ++i)
	{
		CString strColumn = a_vecCalcColumn[i];

		strQuery.AppendFormat(_T(", %s"), strColumn);
	}

	strQuery.AppendFormat(_T(" From %s"), a_pTable->Name());

	a_strFilter.Trim();

	if (!a_strFilter.IsEmpty())
		strQuery.AppendFormat(_T(" Where %s"), a_strFilter);

	KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();

	try
	{
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		while (oResultSetPtr->Next())
		{
			KSDoubleRecord* pRecord = nullptr;
			__int64         nxRowID = oResultSetPtr->GetValueInt64(0);

			map<__int64, KSDoubleRecord*>::iterator findIter = ar_mapResultRecord.find(nxRowID);
			map<__int64, KSDoubleRecord*>::iterator endIter  = ar_mapResultRecord.end();

			if (findIter == endIter)
			{
				pRecord = new KSDoubleRecord(nSize);
				ar_mapResultRecord.insert(make_pair(nxRowID, pRecord));
			}
			else
			{
				pRecord = findIter->second;
			}

			for (int i = 0; i < nSize; ++i)
			{
				pRecord->SetAt(i, oResultSetPtr->GetValueDouble(i + 1));
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		throw ex;
	}
	catch (...)
	{
		throw 1;
	}
}

void KDBaseFieldCalculator::ApplyCalcResult( 
	KIOTable* a_pTable, CString a_strTargetColumn, int a_nTargetIndex, 
	map<__int64, KSDoubleRecord*>& ar_mapResultRecord )
{
	CString strQuery;
	strQuery.Format(_T("Update %s Set %s = ? Where RowID = ?"), a_pTable->Name(), a_strTargetColumn);

	const KIOColumns* pColumns = a_pTable->Columns();
	int   nColumns = pColumns->ColumnCount();

	KEMIODataType emDataType;

	for (int i = 0; i < nColumns; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		if (a_strTargetColumn.CompareNoCase(pColumn->Name()) == 0)
			emDataType = pColumn->DataType();
	}

	KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr oPreparedStatementPtr = spDBaseConnection->PrepareStatement(strQuery);

		map<__int64, KSDoubleRecord*>::iterator beginIter, endIter = ar_mapResultRecord.end();

		for (beginIter = ar_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
		{
			int nBindCount(1);

			Integer         nxRowID = beginIter->first;
			KSDoubleRecord* pRecord = beginIter->second;

			if (emDataType == KEMIODataTypeInteger)
			{
				oPreparedStatementPtr->BindInt(nBindCount++, (int)pRecord->GetAt(a_nTargetIndex));
			}
			else
			{
				oPreparedStatementPtr->BindDouble(nBindCount++, pRecord->GetAt(a_nTargetIndex));
			}

			oPreparedStatementPtr->BindInt64(nBindCount++, nxRowID);

			oPreparedStatementPtr->ExecuteUpdate();
			oPreparedStatementPtr->Reset();
		}
		
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		throw 1;
	}
}
