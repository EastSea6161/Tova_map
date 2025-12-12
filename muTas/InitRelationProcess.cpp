#include "StdAfx.h"
#include "InitRelationProcess.h"
#include "Target.h"
#include "KmzSystem.h"

KInitRelationProcess::KInitRelationProcess(void)
{
}


KInitRelationProcess::~KInitRelationProcess(void)
{
}


void KInitRelationProcess::DBRecordNotify(KTarget* a_pTarget, CString a_strTableName)
{
	return;
	try
	{
		if (KmzSystem::IsPassengerSystem())
		{
			if (a_strTableName.CompareNoCase(TABLE_NODE) == 0)
			{
#pragma region 노드테이블 관련
				HighwayAssignment      (a_pTarget);
				AccessNodeGenerator    (a_pTarget);
				TransitPathGenerator   (a_pTarget);
				MultiModalAssign       (a_pTarget);
				TargetRegionSet        (a_pTarget);
				TargetRegionGlobalTerminalSet(a_pTarget);
				AccessTerminalGenerator(a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
#pragma endregion 노드테이블 관련
			}
			else if (a_strTableName.CompareNoCase(TABLE_LINK) == 0)
			{
#pragma region 링크테이블 관련
				HighwayAssignment      (a_pTarget);
				AccessNodeGenerator    (a_pTarget);
				TransitPathGenerator   (a_pTarget);
				MultiModalAssign       (a_pTarget);
				TargetRegionSet        (a_pTarget);

				AccessTerminalGenerator(a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
#pragma endregion 링크테이블 관련
			}
			else if (a_strTableName.CompareNoCase(TABLE_ZONE) == 0)
			{
#pragma region 존 테이블 관련
				HighwayAssignment      (a_pTarget);
				AccessNodeGenerator    (a_pTarget);
				TransitPathGenerator   (a_pTarget);
				MultiModalAssign       (a_pTarget);
				TargetRegionSet        (a_pTarget);
				TargetRegionGlobalTerminalSet(a_pTarget);
				AccessTerminalGenerator(a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
#pragma endregion 존 테이블 관련
			}
			else if (a_strTableName.CompareNoCase(TABLE_TURN) == 0)
			{
#pragma region 턴 테이블 관련
				HighwayAssignment      (a_pTarget);
				MultiModalAssign       (a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
#pragma endregion 턴 테이블 관련
			}
			else if (a_strTableName.CompareNoCase(TABLE_TRANSIT) == 0)
			{
#pragma region Tranist 테이블 관련
				HighwayAssignment      (a_pTarget);
				AccessNodeGenerator    (a_pTarget);
				TransitPathGenerator   (a_pTarget);
				MultiModalAssign       (a_pTarget);
				TargetRegionSet        (a_pTarget);

				AccessTerminalGenerator(a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
#pragma endregion Tranist 테이블 관련
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::DBFieldNotify(KTarget* a_pTarget, CString a_strTableName)
{
	return;
	try
	{
		if (KmzSystem::IsPassengerSystem())
		{
			if (a_strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0)
			{
				AccessTerminalGenerator(a_pTarget);
				InterModalPathGenerator(a_pTarget);
				InterModalAssign       (a_pTarget);
			}
			else if (a_strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
			{
				HighwayAssignment      (a_pTarget);
				TransitPathGenerator   (a_pTarget);
				MultiModalAssign       (a_pTarget);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::NetworkTypeDefine(KTarget* a_pTarget)
{
	return;
	try
	{
		DBRecordNotify(a_pTarget, TABLE_NODE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::UrbanTransitModeDefine(KTarget* a_pTarget)
{
	return;
	try
	{
		AccessNodeGenerator    (a_pTarget);
		TransitPathGenerator   (a_pTarget);
		MultiModalAssign       (a_pTarget);
		TargetRegionSet        (a_pTarget);
		AccessTerminalGenerator(a_pTarget);
		InterModalPathGenerator(a_pTarget);
		InterModalAssign       (a_pTarget);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::RegionTransitModeDefine(KTarget* a_pTarget)
{
	return;
	try
	{
		TargetRegionGlobalTerminalSet(a_pTarget);
		AccessTerminalGenerator      (a_pTarget);
		InterModalPathGenerator      (a_pTarget);
		InterModalAssign             (a_pTarget);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::HighwayAssignment(KTarget* a_pTarget)
{
	try
	{
		// 관련 파일
		CString strFilePath(_T(""));

		strFilePath.Format(_T("%s\\%s"), a_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME);
		DeletePath(strFilePath);

		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ASSIGN_PATH_INPUT);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ASSIGN_PATH_MODEL);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::AccessNodeGenerator(KTarget* a_pTarget)
{
	try
	{
		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ACCESS_CLASS);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ACCESS_STOP_NODE);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ACCESS_STOP_NODE_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_ACCESS_LINE_STATION);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		KIOTables*       pIOTables = a_pTarget->Tables();
		KIOTable*        pIOTable  = nullptr;
		pIOTable = pIOTables->FindTable(TABLE_ACCESS_STOP_NODE);
		if (pIOTable) pIOTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::TransitPathGenerator(KTarget* a_pTarget)
{
	try
	{
		// 관련 파일
		CString strFilePath(_T(""));

		strFilePath.Format(_T("%s%s%s%s"), a_pTarget->GetLocation(), DLL_READ_WRITE_PATH, MULTIMODAL_GENERATOR_PATH, GENERATE_TRANSIT_RESULT_PATH);
		DeletePath(strFilePath);

		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_BUS_PATH_COEFFICIENT);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_BUS_PATH_COST);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_BUS_PATH_INPUT);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_BUS_PATH_MATRIX);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::MultiModalAssign(KTarget* a_pTarget)
{
	try
	{
		// 관련 파일
		CString strFilePath(_T(""));

		strFilePath.Format(_T("%s%s%s\\%s"), a_pTarget->GetLocation(), DLL_READ_WRITE_PATH, DLL_MULTIMODAL_FOLDER_NAME, MULTIMODAL_RESULT_PATH);
		DeletePath(strFilePath);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::TargetRegionSet(KTarget* a_pTarget)
{
	try
	{
		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLUpdate(_T(""));

		strSQLUpdate.Format(_T(" UPDATE %s SET target_region = '0' "), TABLE_NODE);
		spDBaseConnection->ExecuteUpdate(strSQLUpdate);

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" DELETE FROM %s where KCode_Group_Key = %d AND KCode_Key != 0 "), TABLE_KCODE_DEFINE, TARGET_REGION_CODE_GROUP_KEY);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		KIOTables*       pIOTables = a_pTarget->Tables();
		KIOTable*        pIOTable  = nullptr;
		pIOTable = pIOTables->FindTable(TABLE_NODE);
		if (pIOTable) pIOTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::TargetRegionGlobalTerminalSet(KTarget* a_pTarget)
{
	try
	{
		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_REGION_GLOBAL_TERMINAL);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::AccessTerminalGenerator(KTarget* a_pTarget)
{
	try
	{
		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_REGION_GLOBAL_TERMINAL);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::InterModalPathGenerator(KTarget* a_pTarget)
{
	try
	{
		// 관련 파일
		CString strFilePath(_T(""));

		strFilePath.Format(_T("%s%s%s%s"), a_pTarget->GetLocation(), DLL_READ_WRITE_PATH,INTERMODAL_GENERATOR_PATH, INTERMODAL_GEN_RESULT_PATH);
		DeletePath(strFilePath);

		// 관련 테이블
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_PURPOSE_OD_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_URBAN_PATH_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_REGION_PATH_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_AUTO_COST_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_TAXI_COST_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_TRANSIT_COST);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_TIME_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);

		strSQLDelete.Format(_T(" Delete From %s "), TABLE_INTER_TERMINAL_TIME_INFO);
		spDBaseConnection->ExecuteUpdate(strSQLDelete);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::InterModalAssign(KTarget* a_pTarget)
{
	try
	{
		// 관련 파일
// 		CString strFilePath(_T(""));
// 
// 		strFilePath.Format(_T("%s%s%s\\%s"), a_pTarget->GetLocation(), DLL_READ_WRITE_PATH, INTERMODAL_FOLDER_NAME, 결과 path);
// 		DeletePath(strFilePath);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInitRelationProcess::DeletePath( CString strPath )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(strPath.Right(1) != _T("\\"))
		strPath += _T("\\");

	strPath += _T("*.*");
	bContinue = finder.FindFile(strPath);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeletePath(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}

	finder.Close();

	strPath = strPath.Left(strPath.ReverseFind('\\'));  
	::RemoveDirectory((LPCTSTR)strPath);

}


void KInitRelationProcess::DelGroupGeneration(KTarget* a_pTarget, CString a_strTableName, CString a_strColumnName)
{
	try
	{
		if (KmzSystem::IsPassengerSystem())
		{
// 			if (a_strTableName.CompareNoCase(TABLE_ZONE) == 0)
// 			{
// 				// 관련 테이블
// 				KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
// 
// 				CString strSQLDelete(_T(""));
// 
// 				strSQLDelete.Format(_T(" Delete from Group_Generation_Zone where Output_Column = '%s' "), a_strColumnName);
// 				spDBaseConnection->ExecuteUpdate(strSQLDelete);
// 
// 			}
			if (a_strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0)
			{
				// 관련 테이블
				KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

				CString strSQLDelete(_T(""));

				strSQLDelete.Format(_T(" Delete From Group_Generation_OD where Output_Column = '%s' "), a_strColumnName);
				spDBaseConnection->ExecuteUpdate(strSQLDelete);

			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}