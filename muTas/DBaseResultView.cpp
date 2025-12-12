#include "StdAfx.h"
#include "DBaseResultView.h"

//^#include "DBaseConnector.h"


#include "ViewResultInfo.h"
#include "Target.h"


KDBaseResultView::KDBaseResultView(void)
{
}


KDBaseResultView::~KDBaseResultView(void)
{
}


bool KDBaseResultView::AssignResultView( std::vector<KViewResultInfo> a_vecTreeResultInfo, std::map<KLinkKey, KSDoubleRecord*>& a_mapResultRecord )
{
	TxLogDebugStartMsg();
	int nSelectedItemCount = a_vecTreeResultInfo.size();

	for (int i = 0; i < nSelectedItemCount; ++i)
	{
		KViewResultInfo oViewResultInfo = a_vecTreeResultInfo[i];
		KIOColumn* pColumn = oViewResultInfo.GetColumnInfo();

		if( pColumn == NULL || pColumn->PrimaryKey() || pColumn->DataType() != KEMIODataTypeDouble)
			continue;

		CString strQuery;
		strQuery.Format(_T("select link_id, fnode_id, tnode_id, %s from %s"), pColumn->Name(), TABLE_LINK);

		KTarget*   pTarget   = oViewResultInfo.GetTargetInfo();
		KIOTables* pIOTables = pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_LINK);

		try
		{
			KDBaseConPtr spDBaseConnection = pTable->GetDBaseConnection();
			KResultSetPtr		resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			if (NULL == resultSetPtr)
			{
				return false;
			}

			while (resultSetPtr->Next())
			{
				KLinkKey oLinkKey(resultSetPtr->GetValueInt64(0), resultSetPtr->GetValueInt64(1), resultSetPtr->GetValueInt64(2));

				std::map<KLinkKey, KSDoubleRecord*>::iterator findIter	= a_mapResultRecord.find(oLinkKey);
				std::map<KLinkKey, KSDoubleRecord*>::iterator end		= a_mapResultRecord.end();
				KSDoubleRecord* pRecord = NULL;

				if(findIter == end)
				{
					pRecord = new KSDoubleRecord(nSelectedItemCount);
					a_mapResultRecord.insert(std::make_pair(oLinkKey, pRecord));
				}
				else
				{
					pRecord = findIter->second;
				}
				pRecord->SetAt(i, resultSetPtr->GetValueDouble(3));
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
	}
	TxLogDebugEndMsg();
	return true;
}


bool KDBaseResultView::ODResultView(CString a_TableName, std::vector<KViewResultInfo> a_vecTreeResultInfo, std::map<KODKey, KSDoubleRecord*>& a_mapResultRecord)
{
	TxLogDebugStartMsg();

	int nSelectedItemCount = a_vecTreeResultInfo.size();

	for (int i = 0; i < nSelectedItemCount; ++i)
	{
		KViewResultInfo oViewResultInfo = a_vecTreeResultInfo[i];
		KIOColumn* pColumn = oViewResultInfo.GetColumnInfo();

		if( pColumn == NULL || pColumn->PrimaryKey() || pColumn->DataType() != KEMIODataTypeDouble)
			continue;

		CString strQuery;
		strQuery.Format(_T("select ozone_id, dzone_id, %s from %s"), pColumn->Name(), a_TableName);

		KTarget*   pTarget   = oViewResultInfo.GetTargetInfo();
		KIOTables* pIOTables = pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(a_TableName);

		try
		{
			KDBaseConPtr spDBaseConnection = pTable->GetDBaseConnection();
			KResultSetPtr		resultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			if (NULL == resultSetPtr)
			{
				return false;
			}

			while (resultSetPtr->Next())
			{
				KODKey oODKey(resultSetPtr->GetValueInt64(0), resultSetPtr->GetValueInt64(1));

				std::map<KODKey, KSDoubleRecord*>::iterator findIter	= a_mapResultRecord.find(oODKey);
				std::map<KODKey, KSDoubleRecord*>::iterator end			= a_mapResultRecord.end();
				KSDoubleRecord* pRecord = NULL;

				if(findIter == end)
				{
					pRecord = new KSDoubleRecord(nSelectedItemCount);
					a_mapResultRecord.insert(std::make_pair(oODKey, pRecord));
				}
				else
				{
					pRecord = findIter->second;
				}
				pRecord->SetAt(i, resultSetPtr->GetValueDouble(2));
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
	}
	TxLogDebugEndMsg();
	return true;
}
