#include "StdAfx.h"
#include "DBaseDevPlan.h"

#include "Target.h"
//^#include "DBaseConnector.h"
//^^ #include "IOColumn.h"

KDBaseDevPlan::KDBaseDevPlan(void)
{
}


KDBaseDevPlan::~KDBaseDevPlan(void)
{
}

void KDBaseDevPlan::LstPlanInfo( KTarget* a_pTarget, std::map<int, CString>& a_mapPlanInfo )
{
	TxLogDebugStartMsg();
	a_mapPlanInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strQuery = _T("select plan_id, plan_name from dev_plan order by plan_id");
	
	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);
	
	while (spResult->Next())
	{
		a_mapPlanInfo.insert(
			std::make_pair(spResult->GetValueInt(0), spResult->GetValueString(1)));
	}
	TxLogDebugEndMsg();
}

double KDBaseDevPlan::CalcBasicUnit( KTarget* a_pTarget, KIOColumn* a_pPAColumn, KIOColumn* a_pSEDColumn )
{
	double dBasicUnit, dSumPA, dSumSED;

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	CString strQuery;
	strQuery.Format(_T("select sum(%s) from %s"), a_pPAColumn->Name(), TABLE_ZONE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strQuery);

	if (spResult->Next())
		dSumPA = spResult->GetValueDouble(0);

	strQuery.Format(_T("select sum(%s) from %s"), a_pSEDColumn->Name(), TABLE_SED_VECTOR);
	spResult = spDBaseConnection->ExecuteQuery(strQuery);

	if (spResult->Next())
		dSumSED = spResult->GetValueDouble(0);

	dBasicUnit = dSumPA / dSumSED;

	return dBasicUnit;
}
