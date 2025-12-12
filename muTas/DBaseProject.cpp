#include "StdAfx.h"
#include "DBaseProject.h"

#include "Target.h"
#include "Purpose.h"
#include "Mode.h"

KDBaseProject::KDBaseProject(void)
{
}


KDBaseProject::~KDBaseProject(void)
{
}

void KDBaseProject::ModifyPurposeInfo( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose )
{
	std::vector<int>    vecDisusedPurposeID;

	try
	{
		GetDisusedPurposes     (a_spDBaseConnection, a_vecPurpose, vecDisusedPurposeID);
		DelDisusedPurposeDefine(a_spDBaseConnection, vecDisusedPurposeID);
		SetModifiedPurpose     (a_spDBaseConnection, a_vecPurpose);
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

void KDBaseProject::GetDisusedPurposes( 
	KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose, std::vector<int>& ar_vecDisusedPurposeID )
{
	ar_vecDisusedPurposeID.clear();
	CString strQuery(_T("")), strWhere(_T(""));

	try
	{
		int nModifiedPurpose = a_vecPurpose.size();

		if (nModifiedPurpose != 0)
		{
			strWhere.AppendFormat(_T("Where %s Not In ("), COLUMN_DETAIL_OBJECT_ID);

			for (int i = 0; i < nModifiedPurpose; ++i)
			{
				KPurpose* pPurpose = a_vecPurpose[i];

				strWhere.AppendFormat(_T("%d, "), pPurpose->PurposeID());
			}

			strWhere.Truncate(strWhere.GetLength() - 2);
			strWhere.Append(_T(")"));
		}

		strQuery.Format(_T("Select %s From %s %s"),
			COLUMN_DETAIL_OBJECT_ID, TABLE_DETAIL_OBJECT, strWhere);

		TxLogDebug(strQuery.AllocSysString());

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			ar_vecDisusedPurposeID.push_back(spResultSet->GetValueInt(0));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseProject::DelDisusedPurposeDefine( KDBaseConPtr a_spDBaseConnection, std::vector<int> a_vecDisusedPurposeID )
{
	CString strQuery(_T(""));

	try
	{
		//TABLE_GENERATION_MODEL
		strQuery.Format(_T("Delete From %s Where %s = ?"), TABLE_GENERATION_MODEL, COLUMN_DETAIL_OBJECT_ID);
		KPreparedStatementPtr spPreparedStatement = a_spDBaseConnection->PrepareStatement(strQuery);

		int nUndefinedPurpose = a_vecDisusedPurposeID.size();

		for (int i = 0; i < nUndefinedPurpose; ++i)
		{
			spPreparedStatement->BindInt(1, a_vecDisusedPurposeID[i]);
			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}
		//TABLE_DISTRIBUTION_MODEL
		strQuery.Format(_T("Delete From %s Where %s = ?"), TABLE_DISTRIBUTION_MODEL, COLUMN_DETAIL_OBJECT_ID);
		spPreparedStatement = a_spDBaseConnection->PrepareStatement(strQuery);

		for (int i = 0; i < nUndefinedPurpose; ++i)
		{
			spPreparedStatement->BindInt(1, a_vecDisusedPurposeID[i]);
			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseProject::SetModifiedPurpose( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose )
{
	CString strQuery(_T(""));

	try
	{
		strQuery.Format(_T("Delete From %s"), TABLE_DETAIL_OBJECT);
		a_spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("Insert Into %s(%s, %s, %s) Values(?, ?, ?)"), 
			TABLE_DETAIL_OBJECT, COLUMN_OBJECT_GROUP_CODE, COLUMN_DETAIL_OBJECT_ID, COLUMN_DETAIL_OBJECT_NAME);
		KPreparedStatementPtr spPreparedStatement = a_spDBaseConnection->PrepareStatement(strQuery);

		int nPurpose = a_vecPurpose.size();

		for (int i = 0; i < nPurpose; ++i)
		{
			KPurpose* pPurpose = a_vecPurpose[i];

			spPreparedStatement->BindInt (1, pPurpose->PurposeGroup());
			spPreparedStatement->BindInt (2, pPurpose->PurposeID());
			spPreparedStatement->BindText(3, pPurpose->PurposeName());

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseProject::ModifyModeInfo( KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode )
{
	std::vector<int> vecDisusedModeID;

	try
	{
		GetDisusedModes     (a_spDBaseConnection, a_vecMode, vecDisusedModeID);
		DelDisusedModeDefine(a_spDBaseConnection, vecDisusedModeID);
		SetModifiedMode     (a_spDBaseConnection, a_vecMode);
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

void KDBaseProject::GetDisusedModes( KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode, std::vector<int>& ar_vecDisusedModeID )
{
	ar_vecDisusedModeID.clear();
	CString strQuery(_T("")), strWhere(_T(""));

	try
	{
		int nModes = a_vecMode.size();

		//Add where clause
		if (nModes != 0)
		{
			strWhere.AppendFormat(_T("Where %s Not In ("), COLUMN_DETAIL_MODE_ID);

			for (int i = 0; i < nModes; ++i)
			{
				KMode* pMode = a_vecMode[i];
				strWhere.AppendFormat(_T("%d, "), pMode->ModeID());
			}

			strWhere.Truncate(strWhere.GetLength() - 2);
			strWhere.Append(_T(")"));
		}

		strQuery.Format(_T("Select %s From %s %s"),
			COLUMN_DETAIL_MODE_ID, TABLE_DETAIL_MODE, strWhere);

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			ar_vecDisusedModeID.push_back(spResultSet->GetValueInt(0));
		}
	}
	catch (KExceptionPtr ex)
	{
	
	}
	catch (...)
	{
		
	}
}

void KDBaseProject::DelDisusedModeDefine( KDBaseConPtr a_spDBaseConnection, std::vector<int> a_vecDisusedModeID )
{
	CString strQuery(_T(""));

	try
	{
		//TABLE_MODECHOICE_MODEL
		strQuery.Format(_T("Delete From %s Where %s = ?"), TABLE_MODECHOICE_MODEL, COLUMN_DETAIL_MODE_ID);
		KPreparedStatementPtr spPreparedStatement = a_spDBaseConnection->PrepareStatement(strQuery);

		int nUndefinedPurpose = a_vecDisusedModeID.size();

		for (int i = 0; i < nUndefinedPurpose; ++i)
		{
			spPreparedStatement->BindInt(1, a_vecDisusedModeID[i]);
			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDBaseProject::SetModifiedMode( KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode )
{
	CString strQuery(_T(""));

	try
	{
		strQuery.Format(_T("Delete From %s"), TABLE_DETAIL_MODE);
		a_spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("Insert Into %s(%s, %s, %s) Values(?, ?, ?)"), 
			TABLE_DETAIL_MODE, COLUMN_MODE_GROUP_CODE, COLUMN_DETAIL_MODE_ID, COLUMN_DETAIL_MODE_NAME);
		KPreparedStatementPtr spPreparedStatement = a_spDBaseConnection->PrepareStatement(strQuery);

		int nPurpose = a_vecMode.size();

		for (int i = 0; i < nPurpose; ++i)
		{
			KMode* pMode = a_vecMode[i];

			spPreparedStatement->BindInt (1, pMode->ModeGroup());
			spPreparedStatement->BindInt (2, pMode->ModeID());
			spPreparedStatement->BindText(3, pMode->ModeName());

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}
