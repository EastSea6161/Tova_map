#include "StdAfx.h"
#include "DBaseTerminalAEModeViewer.h"

#include "model_common.h"
#include "Target.h"

KDBaseTerminalAEModeViewer::KDBaseTerminalAEModeViewer(void)
{
}


KDBaseTerminalAEModeViewer::~KDBaseTerminalAEModeViewer(void)
{
}

bool KDBaseTerminalAEModeViewer::IsEmptryTerminalResult( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	bool bResult = true;

	CString strQuery;
	strQuery.Format(_T("SELECT COUNT(*) FROM %s"), TABLE_TERMINAL_RESULT);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("데이터가 없습니다."));

		int nRecord = spResultSet->GetValueInt(0);

		if (nRecord != 0)
			bResult = false;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
	return bResult;
}


void KDBaseTerminalAEModeViewer::GetTerminalResultFieldName(KTarget* a_pTarget, std::vector<CString> &a_vecResultFieldName)
{
	try
	{
		a_vecResultFieldName.clear();

		CString strSQL(_T(""));
		strSQL.Append(_T(" SELECT "));
		strSQL.Append(_T(" tr_ac_auto, tr_ac_taxi, tr_ac_bus, tr_ac_rail, tr_ac_transit, "));
		strSQL.Append(_T(" tr_er_auto, tr_er_taxi, tr_er_bus, tr_er_rail, tr_er_transit, "));
		strSQL.Append(_T(" tr_total "));
		strSQL.AppendFormat(_T(" from %s where type = %d "), TABLE_ASSIGN_MODEL, KEMAssignmentModelInterModal);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
			
		if (spResultSet->Next())
		{
			for (int i= 0; i< 11; i++)
			{
				CString strFieldName = spResultSet->GetValueString(i);
				if (strFieldName.IsEmpty())
				{
					a_vecResultFieldName.clear();
					return;
				}
				a_vecResultFieldName.push_back(strFieldName);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecResultFieldName.clear();
	}
}


void KDBaseTerminalAEModeViewer::GetTerminalResultModeVolume(KTarget* a_pTarget, std::map<Integer, TTerminalResultInfo> &a_mapTerminalResultInfo)
{
	try
	{
		a_mapTerminalResultInfo.clear();

		std::vector<CString> vecResultFieldName;
		GetTerminalResultFieldName(a_pTarget, vecResultFieldName);

		if (vecResultFieldName.size() < 1)
		{
			return;
		}

		CString strTermialSystemCodes(_T(""));
        strTermialSystemCodes.AppendFormat(_T(" %d, "), TRANSITCOMPLEX);
		strTermialSystemCodes.AppendFormat(_T(" %d, "), INTER_REGIONAL_RAILSTATION);
		strTermialSystemCodes.AppendFormat(_T(" %d, "), BUS_TRMINAL);
		strTermialSystemCodes.AppendFormat(_T(" %d, "), INTER_REGIONAL_BUS_TRANSFER);
		strTermialSystemCodes.AppendFormat(_T(" %d, "), AIR_TERMIANL);
		strTermialSystemCodes.AppendFormat(_T(" %d  "), MARIN_PORT);

		CString strSQL(_T(""));
		strSQL.Append(_T(" SELECT TBL1.terminal_id, TBL2.name "));
		for (size_t i= 0; i< vecResultFieldName.size(); i++)
		{
			strSQL.AppendFormat(_T(" ,%s "), vecResultFieldName[i]);
		}
		strSQL.Append(_T(" FROM "));
		strSQL.Append(_T(" (select * from terminal_result)TBL1, "));
		strSQL.Append(_T(" (select node_id, name from node "));
		strSQL.Append(_T("		where node_type in( select user_code_key from define_system_code "));
		strSQL.AppendFormat(_T("		where user_code_group = %d and system_code_key in(%s)))TBL2 "), USER_NODECODE_GROUP_KEY, strTermialSystemCodes);
		strSQL.Append(_T(" WHERE TBL1.terminal_id =  TBL2.node_id "));

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			TTerminalResultInfo oTTerminalResultInfo;

			oTTerminalResultInfo.nxTerminalID     = spResultSet->GetValueInt64(0);
			oTTerminalResultInfo.strTerminalName  = spResultSet->GetValueString(1);

			oTTerminalResultInfo.dTotal           = spResultSet->GetValueDouble(2);
			
			oTTerminalResultInfo.dAccAuto         = spResultSet->GetValueDouble(3);
			oTTerminalResultInfo.dAccTaxi         = spResultSet->GetValueDouble(4);
			oTTerminalResultInfo.dAccBus          = spResultSet->GetValueDouble(5);
			oTTerminalResultInfo.dAccRail         = spResultSet->GetValueDouble(6);
			oTTerminalResultInfo.dAccTransit      = spResultSet->GetValueDouble(7);

			oTTerminalResultInfo.dEgrAuto         = spResultSet->GetValueDouble(8);
			oTTerminalResultInfo.dEgrTaxi         = spResultSet->GetValueDouble(9);
			oTTerminalResultInfo.dEgrBus          = spResultSet->GetValueDouble(10);
			oTTerminalResultInfo.dEgrRail         = spResultSet->GetValueDouble(11);
			oTTerminalResultInfo.dEgrTransit      = spResultSet->GetValueDouble(12);

			a_mapTerminalResultInfo.insert(std::make_pair(oTTerminalResultInfo.nxTerminalID, oTTerminalResultInfo));
		}
	}
	catch (...)
	{
		a_mapTerminalResultInfo.clear();
		TxLogDebugException();	
	}
}