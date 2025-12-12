#include "StdAfx.h"
#include "DBaseGeneration.h"
#include "DBaseConnector.h"

KDBaseGeneration::KDBaseGeneration(void)
{
}


KDBaseGeneration::~KDBaseGeneration(void)
{
}

void KDBaseGeneration::GetCategoryClass( KTarget* a_pTarget, std::map<int, TCategoryClassInto>& ar_mapCategoryInfo )
{
	CString strSQL(_T(""));
	ar_mapCategoryInfo.clear();
	try
	{
		strSQL.Format(_T(" Select "));
		strSQL.Append(_T(" category_id, name, sex_use, age_use"));
		strSQL.AppendFormat(_T(" From %s order by category_id"), TABLE_CATEGORY_CLASS );

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			TCategoryClassInto oInfo;
			oInfo.nCategoryID		= spResultSet->GetValueInt(0);
			oInfo.strCategoryName	= spResultSet->GetValueString(1);
			oInfo.nSexUse			= spResultSet->GetValueInt(2);
			oInfo.nAgeUse			= spResultSet->GetValueInt(3);

			std::map<Integer, KODKey> mapAgeGroup;
			GetCategoryAgeGroup(a_pTarget, oInfo.nCategoryID, mapAgeGroup);
			oInfo.mapAgeGroup = mapAgeGroup;

			std::map<KODKey, double> mapClassValue;
			GetCategoryClassValue(a_pTarget, oInfo.nCategoryID, mapClassValue);
			oInfo.mapClassValue = mapClassValue;

			ar_mapCategoryInfo.insert(std::make_pair(oInfo.nCategoryID,oInfo));
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

void KDBaseGeneration::GetCategoryAgeGroup( KTarget* a_pTarget, int nCategory_id, std::map<Integer, KODKey>& ar_mapAgeGroup )
{
	CString strSQL(_T(""));
	ar_mapAgeGroup.clear();
	try
	{
		strSQL.Format(_T("Select seq, min, max From %s where category_id = %d"), TABLE_CATEGORY_AGE_GROUP, nCategory_id);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nSeq	= spResultset->GetValueInt64(0);
			Integer nxMin	= spResultset->GetValueInt64(1);
			Integer nxMax	= spResultset->GetValueInt64(2);
			KODKey oODkey(nxMin, nxMax);
			ar_mapAgeGroup.insert(std::make_pair(nSeq, oODkey));
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

void KDBaseGeneration::GetCategoryClassValue( KTarget* a_pTarget, int nCategory_id, std::map<KODKey, double>& ar_mapClassValue )
{
	CString strSQL(_T(""));
	ar_mapClassValue.clear();
	try
	{
		strSQL.Format(_T("Select male_female, age_seq, value From %s where category_id = %d"), TABLE_CATEGORY_CLASS_VALUE, nCategory_id);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nMaleFemale	= spResultset->GetValueInt64(0);
			Integer nxAgeseq	= spResultset->GetValueInt64(1);
			double	dValue		= spResultset->GetValueDouble(2);
			KODKey oODkey(nMaleFemale, nxAgeseq);
			ar_mapClassValue.insert(std::make_pair(oODkey, dValue));
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


void KDBaseGeneration::GetCategoryClassName( KTarget* a_pTarget, int nCategory_id, CString& ar_strClassName )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T(" Select name From %s where category_id = %d "), TABLE_CATEGORY_CLASS, nCategory_id);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultset->Next())
		{
			ar_strClassName = spResultset->GetValueString(0);
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


void KDBaseGeneration::GetSEDPopMaxAge( KTarget* a_pTarget, int& a_nMaxAge )
{
	CString strSQL(_T(""));
	int nMaxAge(1);
	try
	{
		strSQL.Format(_T("Select max(age) From %s "), TABLE_SED_POP);
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			nMaxAge = spResultset->GetValueInt(0);
		}

		if (nMaxAge < 1 )
		{
			a_nMaxAge = 1;
		}
		else
		{
			a_nMaxAge = nMaxAge;
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

void KDBaseGeneration::DeleteCategoryClass( KTarget* a_pTarget, int nCategory_id )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Delete From %s Where category_id = %d"), TABLE_CATEGORY_AGE_GROUP, nCategory_id);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		spDBaseConnection->ExecuteUpdate(strSQL);

		strSQL.Format(_T("Delete From %s Where category_id = %d"), TABLE_CATEGORY_CLASS_VALUE, nCategory_id);
		spDBaseConnection->ExecuteUpdate(strSQL);

		strSQL.Format(_T("Delete From %s Where category_id = %d"), TABLE_CATEGORY_CLASS, nCategory_id);
		spDBaseConnection->ExecuteUpdate(strSQL);

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

void KDBaseGeneration::GetMaxCategoryID( KTarget* a_pTarget, int& nMaxCategory_id )
{
	CString strSQL(_T(""));
	int nMaxID(0);
	try
	{
		strSQL.Format(_T("Select Max(category_id) From %s"), TABLE_CATEGORY_CLASS);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr spResultptr = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultptr->Next())
		{
			nMaxID = spResultptr->GetValueInt(0);
		}

		nMaxCategory_id = nMaxID + 1;

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseGeneration::SaveCategroyAgeGroup( KTarget* a_pTarget, int a_nCategory_id, std::map<int, KODKey> a_mapCategroyAge )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Insert Into %s(category_id, seq, min, max) Values (?, ?, ?, ?)"), TABLE_CATEGORY_AGE_GROUP);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KPreparedStatementPtr spPreparedStatementPtr = spDBaseConnection->PrepareStatement(strSQL);

		AutoType iter = a_mapCategroyAge.begin();
		AutoType end  = a_mapCategroyAge.end();
		while(iter != end)
		{
			KODKey oODKey = iter->second;
			spPreparedStatementPtr->BindInt  (1, a_nCategory_id);
			spPreparedStatementPtr->BindInt  (2, iter->first);
			spPreparedStatementPtr->BindInt64(3, oODKey.OriginID);
			spPreparedStatementPtr->BindInt64(4, oODKey.DestinationID);

			spPreparedStatementPtr->ExecuteUpdate();
			spPreparedStatementPtr->Reset();

			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseGeneration::SaveCategoryClassValue( KTarget* a_pTarget, int a_nCategroy_id, std::map<KODKey, double> a_mapCategroyValue )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Insert Into %s(category_id, male_female, age_seq, value) VALUES (?, ?, ?, ?)"), TABLE_CATEGORY_CLASS_VALUE);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KPreparedStatementPtr spPreparedStatementPtr = spDBaseConnection->PrepareStatement(strSQL);

		AutoType iter = a_mapCategroyValue.begin();
		AutoType end  = a_mapCategroyValue.end();
		while(iter != end)
		{
			KODKey oODKey = iter->first;
			spPreparedStatementPtr->BindInt		(1, a_nCategroy_id);
			spPreparedStatementPtr->BindInt64	(2, oODKey.OriginID);
			spPreparedStatementPtr->BindInt64	(3, oODKey.DestinationID);
			spPreparedStatementPtr->BindDouble	(4, iter->second);

			spPreparedStatementPtr->ExecuteUpdate();
			spPreparedStatementPtr->Reset();

			++iter;
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

void KDBaseGeneration::DeleteCategoryValue( KTarget* a_pTarget, int a_nCategroy_id )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Delete From %s Where category_id = %d"), TABLE_CATEGORY_CLASS_VALUE, a_nCategroy_id);
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		spDBaseConnection->ExecuteUpdate(strSQL);
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

bool KDBaseGeneration::IsPopTableNotEmpty( KTarget* a_pTarget )
{
	CString strSQL(_T(""));
	try
	{
		strSQL.Format(_T("Select count(*) From %s"), TABLE_SED_POP);
		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			if (spResult->GetValueInt(0) < 1)
			{
				return false;
			}
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

	return true;
}


void KDBaseGeneration::GetSubCode(KTarget* a_pTarget, CString a_strColumnName, std::map<CString, int> a_mapPCodeInfo, std::set<int>& a_setCode )
{
	CString strSQL(_T(""));
	try
	{
		CString strWhere(_T(""));
		int nIndex(0);
		AutoType iter = a_mapPCodeInfo.begin();
		AutoType end  = a_mapPCodeInfo.end();
		while(iter != end)
		{
			if (nIndex == 0)
			{
				strWhere.Format(_T("Where %s = '%d' "), iter->first, iter->second);
			}
			else
			{
				strWhere.AppendFormat(_T(" And %s = '%d' "), iter->first, iter->second);
			}

			nIndex++;
			++iter;
		}

		strSQL.Format(_T("Select %s From %s  %s "), a_strColumnName, TABLE_ZONE, strWhere);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			int nCode = spResultset->GetValueInt(0);
			a_setCode.insert(nCode);
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
