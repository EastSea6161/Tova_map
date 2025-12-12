#include "StdAfx.h"
#include "DBaseChart.h"
//^#include "DBaseConnector.h"
#include "Target.h"

KDBaseChart::KDBaseChart(void)
{
}


KDBaseChart::~KDBaseChart(void)
{
}


void KDBaseChart::LoadPAColumns(KTarget* a_pTarget, KEMPAType a_emPAType, std::vector<TGenerationModelDB> &a_vecGenerationModelDB)
{
	TxLogDebugStartMsg();
	a_vecGenerationModelDB.clear();

	CString strSQL(_T(""));
	CString strFlagPA(_T(""));

	if(KEMPATypeProduction == a_emPAType)
		strFlagPA = _T("p");
	else
		strFlagPA = _T("a");

	strSQL.Append      (_T(" SELECT "));
	strSQL.Append      (_T(" Zone_Column_Name, Start_Date, End_Date "));
	strSQL.AppendFormat(_T(" FROM %s "), TABLE_GENERATION_MODEL);
	strSQL.AppendFormat(_T(" WHERE PA_Flag = '%s' "), strFlagPA);
	strSQL.Append      (_T(" ORDER BY Generation_run_seq "));

	try
	{        
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TGenerationModelDB oTGenerationModelDB;
			oTGenerationModelDB.strColumnName   = spResultSet->GetValueString(0);
			oTGenerationModelDB.strStartDate    = spResultSet->GetValueString(1);
			oTGenerationModelDB.strEndDate      = spResultSet->GetValueString(2);

			a_vecGenerationModelDB.push_back(oTGenerationModelDB);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(_T("컬럼 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		AfxMessageBox(_T("컬럼 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
	TxLogDebugEndMsg();
}