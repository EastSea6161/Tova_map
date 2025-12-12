#include "StdAfx.h"
#include "DBaseDefaultParaAccessTerminalSetting.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

void KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(std::map<int, std::vector<KAccessTerminal>>& a_mapDefaultAccessTerminal)
{
	TxLogDebugStartMsg();

	a_mapDefaultAccessTerminal.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT Master_Code, Type, Terminal_Name, Boundary_Dist From %s "), KDefaultParaTable::TABLE_DEFAULT_ACCESS_TERMINAL_SETTING);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);


		while( spResult->Next() )
		{
			KAccessTerminal oAccessTerminal;

			int nMasterCode					= spResult->GetValueInt    (0);
			oAccessTerminal.nType			= spResult->GetValueInt	   (1);
			oAccessTerminal.strCodeName		= spResult->GetValueString (2); 
			oAccessTerminal.dBoundaryDist	= spResult->GetValueDouble (3);  

			AutoType find = a_mapDefaultAccessTerminal.find(nMasterCode);
			AutoType end  = a_mapDefaultAccessTerminal.end();

			if (find == end)
			{
				std::vector<KAccessTerminal> vecAccessTerminal;
				vecAccessTerminal.push_back(oAccessTerminal);
				a_mapDefaultAccessTerminal.insert(std::make_pair(nMasterCode, vecAccessTerminal));
			}
			else
			{
				std::vector<KAccessTerminal>& vecAccessTerminal = find->second;
				vecAccessTerminal.push_back(oAccessTerminal);
			}
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

void KDBaseDefaultParaAccessTerminalSetting::MakeTerminalNode(std::map<int, double>& a_mapTerminalNode)
{
	try
	{
		a_mapTerminalNode.clear();

		a_mapTerminalNode.insert(std::make_pair(TRANSITCOMPLEX, 10));
		a_mapTerminalNode.insert(std::make_pair(INTER_REGIONAL_RAILSTATION, 20));
		a_mapTerminalNode.insert(std::make_pair(BUS_TRMINAL, 20));
		a_mapTerminalNode.insert(std::make_pair(AIR_TERMIANL, 40));
		a_mapTerminalNode.insert(std::make_pair(MARIN_PORT, 40));
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

void KDBaseDefaultParaAccessTerminalSetting::GetTableCode(int a_nMasterCode, std::set<int>& a_setAccessTerminalTableCode)
{
	try
	{
		std::map<int, std::vector<KAccessTerminal>> mapDefaultAccessTerminal;
		KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(mapDefaultAccessTerminal);

		AutoType iterDefault = mapDefaultAccessTerminal.find(a_nMasterCode);
		AutoType endDefault  = mapDefaultAccessTerminal.end();

		if(iterDefault != endDefault)
		{
			std::vector<KAccessTerminal>& vecAccessTerminal = iterDefault->second;
			size_t nDefaultCount = vecAccessTerminal.size();

			for(size_t i = 0; i < nDefaultCount; i++)
			{
				KAccessTerminal oAccessTerminal = vecAccessTerminal[i];
				int				nType			= oAccessTerminal.nType;

				a_setAccessTerminalTableCode.insert(nType);
			}
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

void KDBaseDefaultParaAccessTerminalSetting::NodeCodeMatching(KTarget* a_pTarget, std::set<int>& a_setNodeSystemUserCodeMatching)
{
	try
	{
		std::vector<int>vecNodeType;
		std::map<int, int> mapSystemCode;
		KDBaseInterModal::GetNetworkTypePairData(a_pTarget, mapSystemCode);
		KDBaseInterModal::GetUsingNodeType(a_pTarget, vecNodeType);

		size_t nCount = vecNodeType.size();

		for(size_t i = 0; i < nCount; i++)
		{
			int nUsingCode = vecNodeType[i];

			AutoType iterMathing = mapSystemCode.find(nUsingCode);
			AutoType endMathing  = mapSystemCode.end();

			if(iterMathing != endMathing)
			{
				int nSystemCode = iterMathing->second;

				a_setNodeSystemUserCodeMatching.insert(nSystemCode);
			}
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

void KDBaseDefaultParaAccessTerminalSetting::GetAccessTerminalTable(int a_nMasterCode, std::map<int, KAccessTerminal>& a_mapGetAccessTerminalTable)
{
	try
	{
		std::map<int, std::vector<KAccessTerminal>> mapDefaultAccessTerminal;
		KDBaseDefaultParaAccessTerminalSetting::LoadDefaultAccessTerminal(mapDefaultAccessTerminal);

		AutoType iterDefault = mapDefaultAccessTerminal.find(a_nMasterCode);
		AutoType endDefault  = mapDefaultAccessTerminal.end();

		if(iterDefault != endDefault)
		{
			std::vector<KAccessTerminal>& vecAccessTerminal = iterDefault->second;
			size_t nDefaultCount = vecAccessTerminal.size();

			for(size_t i = 0; i < nDefaultCount; i++)
			{
				KAccessTerminal oAccessTerminal = vecAccessTerminal[i];
				int				nType			= oAccessTerminal.nType;

				a_mapGetAccessTerminalTable.insert(std::make_pair(nType, oAccessTerminal));
			}
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

void KDBaseDefaultParaAccessTerminalSetting::GetTerminalInfo(KTarget* a_pTarget, std::map<int, std::vector<KAccessTerminal>>& a_mapSetAccessTerminalInfo)
{
	try
	{
		a_mapSetAccessTerminalInfo.clear();

		KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(SYSTEM_NODECODE_GROUP_KEY);

		std::vector<int>vecNodeType;
		std::map<int, int> mapSystemCode;
		std::map<int, double> mapTerminalNode;

		KDBaseInterModal::GetNetworkTypePairData(a_pTarget, mapSystemCode);
		KDBaseInterModal::GetUsingNodeType(a_pTarget, vecNodeType);
		KDBaseDefaultParaAccessTerminalSetting::MakeTerminalNode(mapTerminalNode);

		size_t nCount = vecNodeType.size();

		for (size_t i = 0; i < nCount; i++)
		{
			int nUsingCode = vecNodeType[i];

			AutoType iterMathing = mapSystemCode.find(nUsingCode);
			AutoType endMathing  = mapSystemCode.end();

			if(iterMathing != endMathing)
			{
				int nSystemCode = iterMathing->second;

				AutoType iterTerminalNode = mapTerminalNode.find(nSystemCode);
				AutoType endTerminalNode  = mapTerminalNode.end();

				if(iterTerminalNode != endTerminalNode)
				{
					std::vector<KAccessTerminal>vecAccessTerminal;
					KAccessTerminal oAccessTerminal;
					int  nType		               = nSystemCode;
					oAccessTerminal.strCodeName    = pCodeGroup->SingleCodeValue(nSystemCode);
					oAccessTerminal.dBoundaryDist  = iterTerminalNode->second;
					vecAccessTerminal.push_back(oAccessTerminal);
					a_mapSetAccessTerminalInfo.insert(std::make_pair(nType, vecAccessTerminal));
				}
			}
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