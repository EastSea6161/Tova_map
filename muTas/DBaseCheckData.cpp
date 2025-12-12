#include "StdAfx.h"
#include "DBaseCheckData.h"
#include "Target.h"
#include "Project.h"

#include "model_common.h"
#include "ImChampFrameWindow.h"
#include "NetworkEditLog.h"
#include "NetworkInspectionDlg.h"

#include "Purpose.h"
#include "PurposeInfoCollection.h"


KDBaseCheckData::KDBaseCheckData(void)
{
}


KDBaseCheckData::~KDBaseCheckData(void)
{
}


bool KDBaseCheckData::SimpleCheckDBData(KTarget* a_pTarget, CString a_strTableName)
{
	int nCount(0);
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery;
		strQuery.Format(_T("select Count(*) from %s"), a_strTableName);

		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{
			nCount = spResultSet->GetValueInt(0);
		}

		if (nCount == 0)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KDBaseCheckData::CheckParameterODLengthdata( KTarget* a_pTarget )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select Count(direct_dist) From %s"), TABLE_PARAMETER_OD);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		int nCount(0);
		while(spResultSet->Next())
		{
			nCount = spResultSet->GetValueInt(0);
		}

		if (nCount < 1)
		{
			AfxMessageBox(_T("경로_거리 계산을 먼저 실행해야 합니다."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDBaseCheckData::CheckParaemterODCostdata( KTarget* a_pTarget )
{
	try
	{
		KIOTable* pParameterTable = a_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);

		const KIOColumns* pColumnCollection = pParameterTable->Columns();
		int nCount = pColumnCollection->ColumnCount();
		int nUserDefineCount(0);
		for (int i=0; i<nCount; i++)
		{
			KIOColumn* pIOColumn    = pColumnCollection->GetColumn(i);
			if (KEMIOColumnTypeFixed == pIOColumn->ColumnType())
			{
				nUserDefineCount++;
			}
		}
		if (nUserDefineCount < 1)
		{
			AfxMessageBox(_T("시간_비용 계산을 먼저 실행해야 합니다."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KDBaseCheckData::GetCheckAccessStopCount( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_ACCESS_STOP_NODE);
}

bool KDBaseCheckData::GetCheckParameterData( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_PARAMETER_OD);
}

bool KDBaseCheckData::GetCheckTransitCostdata( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_TRANSIT_COST);
}

bool KDBaseCheckData::GetCheckTransitInfoData( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_TRANSIT);
}

bool KDBaseCheckData::GetCheckAssignReNewUserEquilibriumOB( KTarget* a_pTarget )
{
	int nCount(0);
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery(_T(""));

		strQuery.Append      (_T(" SELECT type, seq FROM "));
		strQuery.AppendFormat	(_T(" (SELECT seq FROM %s) TBL1, "), TABLE_ASSIGN_PATH_MODEL);
		strQuery.AppendFormat   (_T(" (SELECT MAX(run_seq) as run_seq, type FROM %s) TBL2 "), TABLE_ASSIGN_MODEL);
		strQuery.AppendFormat(_T(" WHERE TBL2.type = %d AND TBL1.seq = TBL2.run_seq "), KEMAssignmentModelUserEquilibriumOB);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		int nResult(-1);
		if(spResultSet->Next())
		{			
			nResult = spResultSet->GetValueInt(1);
		}

		if (nResult< 0)
		{
			AfxMessageBox(_T("도로 통행배정 관련 결과가 존재하지 않습니다.\r\n도로 통행배정 모형(SOBA)을 먼저 실행해 주세요."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("UserEquilibriumOB 결과 조회간 오류 발생"));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("UserEquilibriumOB 결과 조회간 오류 발생"));
		return false;
	}
	return true;
}


bool KDBaseCheckData::GetCheckAssignReNewMultiModal( KTarget* a_pTarget )
{
	int nCount(0);
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery(_T(""));

		strQuery.Append      (_T(" SELECT run_seq FROM "));
		strQuery.AppendFormat   (_T(" (SELECT MAX(run_seq) as run_seq, type FROM %s) TBL2 "), TABLE_ASSIGN_MODEL);
		strQuery.AppendFormat(_T(" WHERE TBL2.type = %d "), KEMAssignmentModelMultiModal);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		int nResult(-1);
		if(spResultSet->Next())
		{			
			nResult = spResultSet->GetValueInt(0);
		}

		if (nResult< 0)
		{
			AfxMessageBox(_T("대중교통 통행배정 관련 결과가 존재하지 않습니다.\r\n대중교통 통행배정 모형을 먼저 실행해 주세요."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("MultiModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("MultiModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	return true;
}


bool KDBaseCheckData::GetCheckAssignReNewInterModal( KTarget* a_pTarget )
{
	int nCount(0);
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery(_T(""));

		strQuery.Append      (_T(" SELECT run_seq FROM "));
		strQuery.AppendFormat   (_T(" (SELECT MAX(run_seq) as run_seq, type FROM %s) TBL2 "), TABLE_ASSIGN_MODEL);
		strQuery.AppendFormat(_T(" WHERE TBL2.type = %d "), KEMAssignmentModelInterModal);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		int nResult(-1);
		if(spResultSet->Next())
		{			
			nResult = spResultSet->GetValueInt(0);
		}

		if (nResult< 0)
		{
			AfxMessageBox(_T("복합수단 통행배정 결과가 존재하지 않습니다.\n모형 > 복합수단 통행배정 모형을 먼저 실행해 주세요."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("InterModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("InterModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	return true;
}


bool KDBaseCheckData::FCheckParameterDirectZone( KTarget* a_pTarget )
{
	int nCount = 0;

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE direct_dist > 0 or direct_time > 0"), TABLE_PARAMETER_OD);

		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nCount = spResultSet->GetValueInt(0);

		if (nCount == 0)
			return false;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KDBaseCheckData::GetCheckTerminaNodeData( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_ACCESS_TERMINAL_NODE);
}

bool KDBaseCheckData::GetCheckTargetRegionCode( KTarget* a_pTarget )
{
	int nCount(0);
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery;
		strQuery.Format(_T("select Count(*) from %s where target_region != 0"), TABLE_NODE);
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			nCount = spResultSet->GetValueInt(0);
		}

		if (nCount == 0)
		{
			return false;
		}

		KCodeManager* pCodeMgr     = a_pTarget->CodeManager();
		KCodeGroup*	  pCodeGroup   = pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		int nCodeCount = pCodeGroup->CodeCount();
		if (nCodeCount < 2)
		{
			return false;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KDBaseCheckData::GetCheckBusPathInputData( KTarget* a_pTarget )
{
	return SimpleCheckDBData(a_pTarget, TABLE_BUS_PATH_INPUT);
}


bool KDBaseCheckData::GetCheckDataInterTerminalTimeInfo( KTarget* a_pTarget )
{
    //★ 
	//2018-04-11 : return SimpleCheckDBData(a_pTarget, TABLE_INTER_TERMINAL_TIME_INFO);
    bool bChk01 = SimpleCheckDBData(a_pTarget, TABLE_INTER_URBAN_PATH_INFO);
    bool bChk02 = SimpleCheckDBData(a_pTarget, TABLE_INTER_TERMINAL_TIME_INFO);
    
    return (bChk01 || bChk02);
}

bool KDBaseCheckData::GetCheckInterModalPathFile( KTarget* a_pTarget )
{
	try
	{
		CString strPathFile(_T(""));
		strPathFile.Format(_T("%s%s%s%s%s"), a_pTarget->GetLocation(), DLL_READ_WRITE_PATH, 
			INTERMODAL_GENERATOR_PATH, INTERMODAL_GEN_RESULT_PATH, _T("*.dat"));

		CFileFind oCFileFinder;
		if (FALSE == oCFileFinder.FindFile(strPathFile))
		{
			return false;
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

bool KDBaseCheckData::GetCheckInterModalPahthGenerator( KTarget* a_pTarget )
{
	try
	{
		if (GetCheckDataInterTerminalTimeInfo(a_pTarget) == false || GetCheckInterModalPathFile(a_pTarget) == false)
		{
			return false;
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

void KDBaseCheckData::GetZoneCount( KTarget* a_pTarget, int& a_nCount)
{
	CString strSQL(_T(""));
	a_nCount = 0;
	try
	{
		strSQL.Format(_T("Select count(*) From %s"), TABLE_ZONE);
		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultset->Next())
		{
			a_nCount = spResultset->GetValueInt(0);
		}
		else
		{
			a_nCount = 0;
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


bool KDBaseCheckData::GetCheckAssignInterModalResult( KTarget* a_pTarget )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strQuery(_T(""));
		strQuery.Format(_T(" SELECT run_seq FROM %s WHERE type = %d "), TABLE_ASSIGN_MODEL, KEMAssignmentModelInterModal);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		int nResult(-1);
		if(spResultSet->Next())
		{			
			nResult = spResultSet->GetValueInt(0);
		}

		if (nResult< 0)
		{
			AfxMessageBox(_T("복합수단 통행배정 결과가 존재하지 않습니다.\n모형 > 복합수단 통행배정 모형을 먼저 실행해 주세요."));
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("InterModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("InterModal Assign 결과 조회간 오류 발생"));
		return false;
	}
	return true;
}

bool KDBaseCheckData::GetCheckInspectionErrorHighway( KTarget* a_pTarget )
{
	try
	{
		int nEditLog(1);
		KNetworkEditLog::GetLastNetWorkLog(a_pTarget, nEditLog);

		KProject* pProject  = ImChampFrameWindow::GetProject();
		KNetworkInspectionDlg oDlg(pProject, a_pTarget);

		if (1 != nEditLog)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("네트워크가 편집되었습니다.\r\n네트워크 데이터 검사를 다시 실행해 주세요."));
			AfxMessageBox(strMsg);
			oDlg.DoModal();

			if (oDlg.ExistErrorFile() == 1 || oDlg.ExistErrorFile() == 3 )
			{
				AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 실행해 주세요."));
				return false;
			}
			else if (oDlg.ExistErrorFile() == -1)
			{
				AfxMessageBox(_T("먼저 네트워크 데이터 검사를 실행하여 오류사항 수정 완료 후 모형을 실행할 수 있습니다."));
				return false;
			}
		}
		else
		{
			if (oDlg.CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_ERROR) == TRUE)
			{
				AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
				oDlg.DoModal();

				if (oDlg.ExistErrorFile() > 0 )
				{
					AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
					return false;
				}
				else if (oDlg.ExistErrorFile() == -1)
				{
					AfxMessageBox(_T("먼저 네트워크 데이터 검사를 실행하여 오류사항 수정 완료 후 모형을 실행할 수 있습니다."));
					return false;
				}
			}
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KDBaseCheckData::GetCheckInspectionErrorHighwayNoMB(KTarget* a_pTarget)
{
	try
	{
		int nEditLog(1);
		KNetworkEditLog::GetLastNetWorkLog(a_pTarget, nEditLog);

		KProject* pProject = ImChampFrameWindow::GetProject();
		KNetworkInspectionDlg oDlg(pProject, a_pTarget);

		if (1 != nEditLog)
		{
			return false;
		}
		else
		{
			if (oDlg.CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_ERROR) == TRUE)
			{
				return false;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDBaseCheckData::GetCheckInspectionErrorTransit( KTarget* a_pTarget )
{
	try
	{
		int nEditLog(1);
		KNetworkEditLog::GetLastNetWorkLog(a_pTarget, nEditLog);

		KProject* pProject  = ImChampFrameWindow::GetProject();
		KNetworkInspectionDlg oDlg(pProject, a_pTarget);

		if (1 != nEditLog)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("네트워크가 편집되었습니다.\r\n네트워크 데이터 검사를 다시 실행해 주세요."));
			AfxMessageBox(strMsg);
			oDlg.DoModal();

			if (oDlg.ExistErrorFile() > 0 )
			{
				AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
				return false;
			}
			else if (oDlg.ExistErrorFile() == -1)
			{
				AfxMessageBox(_T("먼저 네트워크 데이터 검사를 실행하여 오류사항 수정 완료 후 모형을 실행할 수 있습니다."));
				return false;
			}
		}
		else
		{
			if (oDlg.CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_ERROR) == TRUE)
			{
				AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
				oDlg.DoModal();

				if (oDlg.ExistErrorFile() > 0 )
				{
					AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
					return false;
				}
				else if (oDlg.ExistErrorFile() == -1)
				{
					AfxMessageBox(_T("먼저 네트워크 데이터 검사를 실행하여 오류사항 수정 완료 후 모형을 실행할 수 있습니다."));
					return false;
				}
			}
			else if (oDlg.CheckEmptyOutFile(T_DAT_OUT_TRANSIT_ERROR) == TRUE)
			{
				AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
				oDlg.DoModal();

				if (oDlg.ExistErrorFile() > 0 )
				{
					AfxMessageBox(_T("네트워크에 오류가 존재합니다.\r\n수정 완료 후 네트워크 데이터 검사를 다시 수행해 주세요."));
					return false;
				}
				else if (oDlg.ExistErrorFile() == -1)
				{
					AfxMessageBox(_T("먼저 네트워크 데이터 검사를 실행하여 오류사항 수정 완료 후 모형을 실행할 수 있습니다."));
					return false;
				}
			}
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDBaseCheckData::GetExistModeODData( KTarget* a_pTarget )
{
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Select count(*) From %s "), TABLE_MODE_OD);
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		int nCount(0);
		if (spResultset->Next())
		{
			nCount = spResultset->GetValueInt(0);
		}

		if (nCount < 1)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KDBaseCheckData::GetCheckAssignResult( KTarget* a_pTarget )
{
	bool bResult = false;

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM %s"), TABLE_ASSIGN_MODEL);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
		{
			if (spResultSet->GetValueInt(0) > 0)
				bResult = true;
		}

		if (!bResult)
		{
			AfxMessageBox(_T("관련 통행배정 결과가 존재하지 않습니다.\r\n관련 통행배정 모형을 먼저 실행해 주세요."));
			return bResult;
		}

		return bResult;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("Assignment 결과 조회간 오류 발생"));
		return false;	
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("Assignment 결과 조회간 오류 발생"));
		return false;
	}
}

bool KDBaseCheckData::CheckBuslaneCode( KTarget* a_pTarget )
{
	CString strSQL(_T(""));
	int nCode(0);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select user_code_key From %s Where system_code_key = 2 And user_code_group = 3 ") ,TABLE_DEFINE_SYSTEM_CODE);

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResultset->Next())
		{
			nCode = spResultset->GetValueInt(0);
		}

		if (nCode < 1)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}
