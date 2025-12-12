#include "StdAfx.h"
#include "EditAccessLineStation.h"
#include "Target.h"
//^#include "DBaseConnector.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"

KEditAccessLineStation::KEditAccessLineStation(KTarget* a_pTarget, Integer a_nxZoneID, Integer a_nxLineID)
	: m_pTarget(a_pTarget)
	, m_nxZoneID(a_nxZoneID)
	, m_nxLineID(a_nxLineID)
{
}


KEditAccessLineStation::~KEditAccessLineStation(void)
{
}


void KEditAccessLineStation::DeleteAccessLineStation()
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	CString          strQuery  (_T(""));
	try
	{
		strQuery.Format(
			_T(" DELETE FROM %s WHERE zone_id = %I64d AND line_id = %I64d "), TABLE_ACCESS_LINE_STATION, m_nxZoneID, m_nxLineID);

		spDBaseConnection->ExecuteUpdate(strQuery);

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ACCESS_LINE_STATION);
		pIOTable->Notify();

		DeleteRelationFile();
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KEditAccessLineStation::DeleteRelationFile()
{
	int nResult = -1;

	nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_R_BUSPATH));
	nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE));
}


CString KEditAccessLineStation::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFileName);

	return strSavePath;
}
