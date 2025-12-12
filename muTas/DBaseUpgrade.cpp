#include "StdAfx.h"
#include "ProgramVersionInfo.h"
#include "DBaseUpgrade.h"
#include "Project.h"
#include "DBaseVersion_P001.h"
#include "DBaseVersion_P003.h"
#include "DBaseVersion_P004.h"
#include "DBaseVersion_P005.h"
#include "DBaseVersion_P010.h"
#include "DBaseVersion_P011.h"
#include "DBaseVersion_P012.h"
#include "DBaseVersion_P013.h"
#include "DBaseVersion_P014.h"
#include "DBaseVersion_P015.h"
#include "DBaseVersion_P016.h"
#include "DBaseVersion_P017.h"
#include "DBaseVersion_P018.h"
#include "DBaseVersion_P019.h"
#include "DBaseVersion_P020.h"
#include "DBaseVersion_P021.h"
#include "DBaseVersion_P022.h"
#include "DBaseVersion_P023.h"
#include "DBaseVersion_P1001.h"

KDBaseUpgrade::KDBaseUpgrade(void)
{
}


KDBaseUpgrade::~KDBaseUpgrade(void)
{
}

int KDBaseUpgrade::GetTargetImTasVersion( KDBaseConPtr spDBaseConnection )
{
	int nVersion(0);
	try
	{
		CString strSQL(_T("Select ImTasVersion, Date, Description From ImTasVersion Order By ImTasVersion Desc"));               
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);	
		// spResultSet이 nullptr인지 확인
		if (spResultSet == nullptr) {
			// 쿼리 실행 실패 처리
			return false;
		}

		if ( spResultSet->Next())
		{
			nVersion = spResultSet->GetValueInt(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return nVersion;
}

void KDBaseUpgrade::SetTargetImTasVersion( KDBaseConPtr spDBaseConnection, int a_nVersion )
{
	try
	{	
		CString strSQL(_T("Delete From ImTasVersion "));
		spDBaseConnection->ExecuteUpdate(strSQL);

		int nSystem(0);
		if (KmzSystem::IsFreightSystem())
		{
			nSystem = 1;
		}

		strSQL.Format(_T("Insert Or Replace Into ImTasVersion(ImTasVersion, Date, system) Values('%d', datetime('now', 'localtime'), '%d') "), a_nVersion, nSystem);                 
		spDBaseConnection->ExecuteUpdate(strSQL);

	}
	catch (KExceptionPtr ex)
	{        
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw;
	}
}

void KDBaseUpgrade::UpgradeIolocalProcess( KDBaseConPtr spDBaseConnection, QBicSimpleProgressPtr spProgressWindow )
{
	try
	{
		int TImtasVersion(GxnIoLocalVersion);

		const int nVersion = GetTargetImTasVersion(spDBaseConnection);
			
		if (nVersion == TImtasVersion)
		{
			return;
		}

		if (nVersion > TImtasVersion)
		{
			//CString strMsg;
			//strMsg.Format(_T("상위 버전에서 작성된 프로젝트 입니다.\n프로그램을 업데이트 한 후에 다시 실행하여 주세요."));
			//AfxMessageBox(strMsg);
			//CString strErr;
			//strErr.Format(_T("project version(%d) is higher then program version(%d)"), nVersion, TImtasVersion);
			//throw KExceptionPtr(new KException(10001, strErr));
			return;
		}

		if (nVersion < TImtasVersion)
		{
			//CString msg;
			//msg.Format(_T(
			//	"프로젝트 버전 : %d, 프로그램 버전 : %d\n"
			//	"프로젝트를 업데이트 해야 합니다.\n\n"
			//	"* 프로젝트를 업데이트 한 후에는 이전 버전의 프로그램에서 불러올 경우 프로그램 구동 중 문제가 발생 할 수 있습니다.\n\n"
			//	"계속 하시겠습니까?")
			//	, nVersion, TImtasVersion
			//);

			//if (AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL)
			//{
			//	CString strErr;
			//	strErr.Format(_T("user cancle the project upgrade, project version=%d, program version=%d"), nVersion, TImtasVersion);
			//	throw KExceptionPtr(new KException(10002, strErr));
			//}
			
		}

		spDBaseConnection->BeginTransaction();
#pragma region TOVA 이전 Tas의 버전
// 		if (nVersion == 0)
// 		{
// 			if (spProgressWindow)
// 			{
// 				spProgressWindow->SetStatus(_T("Upgrade DataBase"));
// 			}
// 
// 			if (spProgressWindow)
// 			{
// 				spProgressWindow->SetStatus(_T("Upgrade DataBase"));
// 			}
// 
// 
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P001::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P003::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P004::Upgrade(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 1)
// 		{
// 			if (spProgressWindow)
// 			{
// 				spProgressWindow->SetStatus(_T("Upgrade DataBase"));
// 			}
// 
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P003::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}         
// 
// 			KDBaseVersion_P004::Upgrade(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 2)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P003::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P004::Upgrade(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			
// 		}
// 		else if (nVersion == 3)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P004::Upgrade(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			
// 		}
// 		else if (nVersion == 4)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			
// 		}
// 		else if(nVersion == 5)
// 		{
// 			
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			CreateImTasTable(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			
// 		}
// 		else if (nVersion == 10)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			CreateImTasTable(spDBaseConnection);
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			
// 		}
// 		else if (nVersion == 11)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 12)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 13)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 14)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 15)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 16)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 17)
// 		{
// 
// 		}
// 		else if (nVersion == 18)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 19)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 20)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 21)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 22)
// 		{
// 
// 		}
// 		else
// 		{        
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P001::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P003::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P005::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P010::Upgrade(spDBaseConnection);
// 				KDBaseVersion_P014::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P017::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::IolocalDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P022::IolocalDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P012::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P004::Upgrade(spDBaseConnection);
// 			KDBaseVersion_P013::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P015::IolocalDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P019::IolocalDBUpgrade(spDBaseConnection);			
// 		}
		

#pragma endregion TOVA 이전 Tas의 버전

		KDBaseUpgrade::SetTargetImTasVersion(spDBaseConnection,TImtasVersion);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		spDBaseConnection->RollBack();
	}
	catch (...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}

void KDBaseUpgrade::UpgradeProjectProcess(KProject* pProject, KDBaseConPtr spDBaseConnection, QBicSimpleProgressPtr spProgressWindow  )
{
	try
	{
		int TImtasVersion(GxnProjectVersion);
		const int nVersion = GetTargetImTasVersion(spDBaseConnection);

		if (nVersion == TImtasVersion)
		{
			return;
		}

		if (nVersion > TImtasVersion)
		{
			//CString strMsg;
			//strMsg.Format(_T("상위 버전에서 작성된 프로젝트 입니다.\n프로그램을 업데이트 한 후에 다시 실행하여 주세요."));
			//AfxMessageBox(strMsg);
			//CString strErr;
			//strErr.Format(_T("project version(%d) is higher then program version(%d)"), nVersion, TImtasVersion);
			//throw KExceptionPtr(new KException(10001, strErr));
			return;
		}

		if (nVersion < TImtasVersion)
		{
			//CString msg;
			//msg.Format(_T(
			//	"프로젝트 버전 : %d, 프로그램 버전 : %d\n"
			//	"프로젝트를 업데이트 해야 합니다.\n\n"
			//	"* 프로젝트를 업데이트 한 후에는 이전 버전의 프로그램에서 불러올 경우 프로그램 구동 중 문제가 발생 할 수 있습니다.\n\n"
			//	"계속 하시겠습니까?")
			//	, nVersion, TImtasVersion
			//);

			//if (AfxMessageBox(msg, MB_OKCANCEL) == IDCANCEL)
			//{
			//	CString strErr;
			//	strErr.Format(_T("user cancle the project upgrade, project version=%d, program version=%d"), nVersion, TImtasVersion);
			//	throw KExceptionPtr(new KException(10002, strErr));
			//}
		}

		spDBaseConnection->BeginTransaction();

#pragma region TOVA 이전 Tas의 버전
// 		if (spProgressWindow)
// 		{
// 			spProgressWindow->SetStatus(_T("Upgrade DataBase"));
// 		}
// 
// 		if (nVersion == 10)
// 		{			
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P011::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P014::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 
// 			CreateImTasTable(spDBaseConnection);
// 			KDBaseVersion_P012::ProjectDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 11)
// 		{
// 
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 
// 			CreateImTasTable(spDBaseConnection);
// 			KDBaseVersion_P012::ProjectDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 12)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 13)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P014::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 14)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 15)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 16)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 		}
// 		else if (nVersion == 17)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 18)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 19)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 20)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 21)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else if (nVersion == 22)
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P023::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 		}
// 		else
// 		{
// 			if (KmzSystem::IsPassengerSystem())
// 			{
// 				KDBaseVersion_P011::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P014::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P016::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P018::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P019::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P020::ProjectDBUpgrade(spDBaseConnection);
// 				KDBaseVersion_P021::ProjectDBUpgrade(spDBaseConnection);
// 			}
// 
// 			KDBaseVersion_P012::ProjectDBUpgrade(spDBaseConnection);
// 			KDBaseVersion_P017::ProjectDBUpgrade(spDBaseConnection);
// 			CreateImTasTable(spDBaseConnection);
// 		}

		if (nVersion <= 1000)
		{
			KDBaseVersion_P1001::ProjectDBUpgrade(pProject, spDBaseConnection);			
		}

#pragma endregion TOVA 이전 Tas의 버전

		KDBaseUpgrade::SetTargetImTasVersion(spDBaseConnection, TImtasVersion);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
	}
}

void KDBaseUpgrade::CreateImTasTable( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strSQLDrop (_T("Drop Table ImTasVersion"));
		spDBaseConnection->ExecuteUpdate(strSQLDrop);
	}
	catch (...)
	{
	}


	CString strSQLCreate(_T(""));
	strSQLCreate.Append(_T( " CREATE TABLE [ImTasVersion] (  "));
	strSQLCreate.Append(_T( "   [ImTasVersion] VARCHAR2 PRIMARY KEY  NOT NULL, "));
	strSQLCreate.Append(_T( "   [Date] VARCHAR2,  "));
	strSQLCreate.Append(_T( "   [System] INTEGER,  "));
	strSQLCreate.Append(_T( "   [Description] VARCHAR2 ) "));

	spDBaseConnection->ExecuteUpdate(strSQLCreate);
}
