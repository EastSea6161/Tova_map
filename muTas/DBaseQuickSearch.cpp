#include "StdAfx.h"
#include "DBaseQuickSearch.h"


KDBaseQuickSearch::KDBaseQuickSearch(void)
{
}


KDBaseQuickSearch::~KDBaseQuickSearch(void)
{
}

void KDBaseQuickSearch::GetReportData( KIOTable* a_pTable, KIOColumns* a_pColumns, std::map<Integer, std::vector<CString>>& a_mapData )
{
	try
	{
		a_mapData.clear();
		CString strQuery, strValue;
		
		//Map의 Key Column으로 사용하기 위해 rowid를 조회함
		strQuery.Append(_T("Select rowid, "));
		
		int nColumn = a_pColumns->ColumnCount();

		for (int i = 0; i < nColumn; ++i)
		{
			KIOColumn* pColumn = a_pColumns->GetColumn(i);
			strQuery.AppendFormat(_T("%s, "), pColumn->Name());
		}

		strQuery.Truncate(strQuery.GetLength() - 2);
		strQuery.AppendFormat(_T(" From %s"), a_pTable->Name());

		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);
		Integer             nxRowID;

		while (spResultSet->Next())
		{
			nxRowID = spResultSet->GetValueInt64(0);

			std::vector<CString> vecRow;

			for (int i = 0; i < nColumn; ++i)
			{
				strValue = spResultSet->GetValueString(i + 1);
				vecRow.push_back(strValue);
			}

			a_mapData.insert(std::make_pair(nxRowID, vecRow));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebug(_T("unexepcted error - KDBaseQuickSearch::GetReportData"));
		throw 1;
	}
}
