// DBaseDefaultParameter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DBaseDefaultParaAccessSetting.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

namespace DefaultAccessStationSetting
{
	const int	  ALL_CLASS_COUNT =	10;
	const TCHAR   CLASS_INIT_CHAR = 'A';
	const int	  PLUS_CLASS_TIME = 5;
	const int	  INITTIME		  = 5;
};
using namespace DefaultAccessStationSetting;

void KDBaseDefaultParameter::LoadDefaultAccessClassInfo(std::map<int, std::vector<TAccessClassInfo>>& a_mapDefaultAccessClassInfo)
{
	TxLogDebugStartMsg();

	a_mapDefaultAccessClassInfo.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, ID, Class, Time FROM %s ORDER BY Master_Code, ID "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		TAccessClassInfo oTAccessClassInfo;

		while( pResult->Next() )
		{
			int nMasterCode					= pResult->GetValueInt    (0);
			int nID							= pResult->GetValueInt	  (1);
			oTAccessClassInfo.strClassName	= pResult->GetValueString (2);
			oTAccessClassInfo.nTimeMin 		= pResult->GetValueInt	  (3);

			AutoType find = a_mapDefaultAccessClassInfo.find(nMasterCode);
			AutoType end  = a_mapDefaultAccessClassInfo.end();

			if (end == find)
			{
				std::vector<TAccessClassInfo> vecDefaultAccessClassinfo;
				vecDefaultAccessClassinfo.push_back(oTAccessClassInfo);
				a_mapDefaultAccessClassInfo.insert(std::make_pair(nMasterCode, vecDefaultAccessClassinfo));
			}
			else
			{
				std::vector<TAccessClassInfo> & vecDefaultAccessClassinfo = find->second;
				vecDefaultAccessClassinfo.push_back(oTAccessClassInfo);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		//throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		//throw -1;
	}
}

void KDBaseDefaultParameter::LoadDefaultAccessSetting(std::map<int, std::vector<KAccessStationSetting>>& a_mapDefaultAccessSetting)
{
	TxLogDebugStartMsg();

	a_mapDefaultAccessSetting.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, Distance, Region_Code, Class_Number FROM %s"), KDefaultParaTable::TABLE_DEFAULT_ACCESS_CLASSINFO_SETTING);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		KAccessStationSetting oDefaultAccessSetting;

		while( pResult->Next() )
		{
			int nMasterCode						= pResult->GetValueInt (0);
			oDefaultAccessSetting.dDistance	    = pResult->GetValueDouble(1);
			oDefaultAccessSetting.nRegionCode	= pResult->GetValueInt (2);
			oDefaultAccessSetting.nClassNumber	= pResult->GetValueInt (3);

			std::vector<KAccessStationSetting> vecDefaultAccessSetting;
			vecDefaultAccessSetting.push_back(oDefaultAccessSetting);
			a_mapDefaultAccessSetting.insert(std::make_pair(nMasterCode, vecDefaultAccessSetting));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		//throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		//throw -1;
	}
}

void KDBaseDefaultParameter::LoadAccessClassInfo(std::vector<TAccessClassInfo>& a_vecLoadAccessClassInfo)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT class_id, class_name, time_min  FROM %s ORDER BY class_id "), TABLE_ACCESS_CLASS);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TAccessClassInfo oTAccessClassInfo;

			int nClassID                   = pResult->GetValueInt   (0);
			oTAccessClassInfo.strClassName = pResult->GetValueString(1);
			oTAccessClassInfo.nTimeMin	   = pResult->GetValueInt   (2);

			a_vecLoadAccessClassInfo.push_back(oTAccessClassInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		//throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		//throw -1;
	}	
}

void KDBaseDefaultParameter::SetAccessibility(std::map<int, CString>& a_mapAccessibility)
{
	try
	{
		TCHAR cFirst    = KAccessStationDefine::ACCESS_CLASS_INIT_CHAR;
		int   nClass    = 1;
		for (int i= 0; i< ALL_CLASS_COUNT; i++)
		{
			CString strClassName = (CString)cFirst++;
			a_mapAccessibility.insert(std::make_pair(nClass, strClassName));
			nClass++;
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