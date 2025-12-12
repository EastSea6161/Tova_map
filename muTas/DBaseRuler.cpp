#include "StdAfx.h"
#include "DBaseRuler.h"
#include "Target.h"


KDBaseRuler::KDBaseRuler(void)
{
}


KDBaseRuler::~KDBaseRuler(void)
{
}

void KDBaseRuler::GetRulerInfo( KTarget* a_pTarget, KIOColumn* a_pColumn, std::vector<TRulerInfo>& ar_vecRulerInfo )
{
	try
	{
		CString strQuery;
		strQuery.Format(_T("Select link_id, %s From link where link_id = ?"), a_pColumn->Name());

		KDBaseConPtr   spDBaseConnection   = a_pTarget->GetDBaseConnection();
		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strQuery);

		int nSize = ar_vecRulerInfo.size();

		for (int i = 0; i < nSize; ++i)
		{
			TRulerInfo& oRulerInfo = ar_vecRulerInfo[i];

			spPreparedStatement->BindInt64(1, oRulerInfo.nxLinkID);
			KResultSetPtr spResultSet = spPreparedStatement->ExecuteQuery();

			if (spResultSet->Next())
			{
				oRulerInfo.dValue = spResultSet->GetValueDouble(1);	
			}
			else
			{
				ThrowException(_T("조회 결과가 없습니다."));
			}

			spPreparedStatement->Reset();
		}

	}
	catch (KExceptionPtr ex)
	{
	
	}
	catch (...)
	{
		
	}
}

void KDBaseRuler::GetRulerInfo( KTarget* a_pTarget, KIOColumn* a_pColumn, TRulerInfo& ar_oRulerInfo )
{
	try
	{
		CString strQuery;
		strQuery.Format(_T("Select link_id, %s From link where link_id = %I64d"), 
			a_pColumn->Name(), ar_oRulerInfo.nxLinkID);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			ar_oRulerInfo.dValue = spResultSet->GetValueDouble(1);
		}
		else
		{
			ThrowException(_T("조회 결과가 없습니다."));
		}
	}
	catch (KExceptionPtr ex)
	{

	}
	catch (...)
	{

	}
}
