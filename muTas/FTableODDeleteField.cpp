#include "StdAfx.h"
#include "FTableODDeleteField.h"

#include "Target.h"


KFTableODDeleteField::KFTableODDeleteField(void)
{
}


KFTableODDeleteField::~KFTableODDeleteField(void)
{
}

void KFTableODDeleteField::DeleteModeODFieldLogic( KTarget* a_pTarget, CString a_strFieldName )
{
	TxLogDebugStartMsg();

	if (a_strFieldName.IsEmpty() || a_pTarget == nullptr)
	{
		TxLogDebugException();
		return;
	}

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	KResultSetPtr       spResultSet;
	KIOTable*           pTable = nullptr;
    pTable = a_pTarget->Tables()->FindTable(TABLE_MODE_OD);

	int     nRunSeq(0), nRecords(0);
	
	CString strQuery;
	std::vector<CString> vecRemoveColumn;
	
    /************************************************************************/
    /* 2017.07.27 - jyk
       소스를 분석한 결과 itemlogit_model 테이블은 화물과 관련된 테이블로 추정된다.
       현재 db상에 존재도 하지 않는다.
       그래서 itemlogit_model 테이블과 관련되어
       TABLE_MODE_OD의 컬럼이 삭제되는 과정에 문제를 일으키기 때문에,
       itemlogit_model 테이블과 관련된 소스를 주석처리 하겠다.
    */
    /************************************************************************/
// 	try
// 	{
// 		strQuery.Format(_T("SELECT run_seq, direct_road, road, rail FROM itemlogit_model WHERE direct_road = '%s' OR road = '%s' OR rail = '%s'"),
// 			a_strFieldName, a_strFieldName, a_strFieldName);
// 
// 		spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
// 
// 		while (spResultSet->Next())
// 		{
// 			nRunSeq      = spResultSet->GetValueInt(0);
// 
// 			if (a_strFieldName.CompareNoCase(spResultSet->GetValueString(1)) != 0)
// 				vecRemoveColumn.push_back(spResultSet->GetValueString(1));
// 			
// 			if (a_strFieldName.CompareNoCase(spResultSet->GetValueString(2)) != 0)
// 				vecRemoveColumn.push_back(spResultSet->GetValueString(2));
// 			
// 			if (a_strFieldName.CompareNoCase(spResultSet->GetValueString(3)) != 0)
// 				vecRemoveColumn.push_back(spResultSet->GetValueString(3));
// 
// 			++nRecords;
// 		}
// 
// 		if (nRecords != 1)
// 		{
// 			TxLogDebugException();
// 			return;
// 		}
// 
// 		int nSize = vecRemoveColumn.size();
// 
// 		pTable->BeginTransaction();
// 
// 		for (int i = 0; i < nSize; ++i)
// 		{
// 			pTable->RemoveColumn(vecRemoveColumn[i]);
// 		}
// 
// 		//Delete ItemLogit_model
// 		strQuery.Format(_T("DELETE FROM %s WHERE run_seq = %d"), TABLE_ITEMLOGIT_MODEL, nRunSeq);
// 		spDBaseConnection->ExecuteUpdate(strQuery);
// 		
// 		pTable->Commit();
// 	}
//     catch (KExceptionPtr ex)
//     {
//         TxExceptionPrint(ex);
//         TxLogDebugException();
//         pTable->Rollback();
//     }
// 	catch (...)
// 	{
// 		TxLogDebugException();
// 		pTable->Rollback();
// 		return;
// 	}

	TxLogDebugEndMsg();
}

void KFTableODDeleteField::DeleteChainODFieldLogic( KTarget* a_pTarget, CString a_strFieldName )
{
	TxLogDebugStartMsg();

	if (a_strFieldName.IsEmpty() || a_pTarget == nullptr)
	{
		TxLogDebugException();
		return;
	}

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	KResultSetPtr       spResultSet;
	KIOTable*           pTable = nullptr;
    pTable = a_pTarget->Tables()->FindTable(TABLE_CHAIN_OD);

	int     nRunSeq(0), nRecords(0);

	CString strQuery;
	std::vector<CString> vecRemoveColumn;

	try
	{
		strQuery.Format(_T("SELECT run_seq, road, rail FROM chain_model WHERE road = '%s' OR rail = '%s'"),
			a_strFieldName, a_strFieldName, a_strFieldName);

		spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			nRunSeq      = spResultSet->GetValueInt(0);

			if (a_strFieldName.CompareNoCase(spResultSet->GetValueString(1)) != 0)
				vecRemoveColumn.push_back(spResultSet->GetValueString(1));

			if (a_strFieldName.CompareNoCase(spResultSet->GetValueString(2)) != 0)
				vecRemoveColumn.push_back(spResultSet->GetValueString(2));

			++nRecords;
		}

		if (nRecords != 1)
		{
			TxLogDebugException();
			return;
		}

		int nSize = vecRemoveColumn.size();

		pTable->BeginTransaction();

		for (int i = 0; i < nSize; ++i)
		{
			pTable->RemoveColumn(vecRemoveColumn[i]);
		}

		//Delete ItemLogit_model
		strQuery.Format(_T("DELETE FROM %s WHERE run_seq = %d"), TABLE_CHAIN_MODEL, nRunSeq);
		spDBaseConnection->ExecuteUpdate(strQuery);

		pTable->Commit();
	}
    catch (KExceptionPtr ex) 
    {
        TxExceptionPrint(ex);
        TxLogDebugException();
        pTable->Rollback();
    }
	catch (...)
	{
		TxLogDebugException();
		pTable->Rollback();
		return;
	}

	TxLogDebugEndMsg();
}
