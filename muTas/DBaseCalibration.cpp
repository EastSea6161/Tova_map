#include "StdAfx.h"
#include "DBaseCalibration.h"

#include "Target.h"
//^#include "DBaseConnector.h"

KDBaseCalibration::KDBaseCalibration(void)
{
}


KDBaseCalibration::~KDBaseCalibration(void)
{
}

int KDBaseCalibration::SelectZoneCount( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	int nTotalZoneCount(0);
	CString strQuery(_T(""));
	strQuery.Format(_T("SELECT COUNT(*) FROM %s"), TABLE_ZONE);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nTotalZoneCount = spResultSet->GetValueInt(0);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
		return 0;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return 0;
	}
	TxLogDebugEndMsg();

	return nTotalZoneCount;
}


