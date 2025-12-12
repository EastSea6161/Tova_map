#include "StdAfx.h"
#include "DBaseDefaultInterRunOption.h"
#include "Target.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseConnector.h"
#include "ModeManager.h"
#include "Mode.h"


KDBaseDefaultInterRunOption::KDBaseDefaultInterRunOption(void)
{
}


KDBaseDefaultInterRunOption::~KDBaseDefaultInterRunOption(void)
{
}

void KDBaseDefaultInterRunOption::LoadInterOccupancyPCE( std::map<int, std::map<int, KInterOccupancePCEMode>>& a_mapOccupancyPCEmode )
{
	a_mapOccupancyPCEmode.clear();
	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

		strSQL.Format(_T(" Select master_code, use, detail_mode_id, occupancy, pce From %s "), KDefaultParaTable::TABLE_DEFAULT_INTERMODAL_PCE_MODE);

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			KInterOccupancePCEMode oInfo;
			int nMasterCode		= spResultset->GetValueInt(0);
			oInfo.nUse			= spResultset->GetValueInt(1);
			oInfo.nModeID		= spResultset->GetValueInt(2);
			oInfo.dOccupancy	= spResultset->GetValueDouble(3);
			oInfo.dPCE			= spResultset->GetValueDouble(4);

			AutoType find = a_mapOccupancyPCEmode.find(nMasterCode);
			AutoType fend = a_mapOccupancyPCEmode.end();
			if (find == fend)
			{
				std::map<int, KInterOccupancePCEMode> mapOPMode;
				mapOPMode.insert(std::make_pair(oInfo.nModeID, oInfo));
				a_mapOccupancyPCEmode.insert(std::make_pair(nMasterCode, mapOPMode));
			}
			else
			{
				std::map<int, KInterOccupancePCEMode>& mapOPMode = find->second;
				mapOPMode.insert(std::make_pair(oInfo.nModeID, oInfo));
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

void KDBaseDefaultInterRunOption::GetModeInfo( KTarget* a_pTarget, std::map<int, KInterOccupancePCEMode>& a_mapOccupancyPCE )
{
	a_mapOccupancyPCE.clear();
	
	try
	{
		std::map<int, std::map<int, KInterOccupancePCEMode>> mapAllOccupancyPCE;
		LoadInterOccupancyPCE(mapAllOccupancyPCE);

		std::map<int, KInterOccupancePCEMode> mapSelOccupancyPCE;
		AutoType fUser		= mapAllOccupancyPCE.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType fSystem	= mapAllOccupancyPCE.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		AutoType fend		= mapAllOccupancyPCE.end();

		if (fUser != fend)
		{
			mapSelOccupancyPCE = fUser->second;
		}
		else if (fSystem != fend)
		{
			mapSelOccupancyPCE = fSystem->second;
		}

		KModeManager* pModeManager = a_pTarget->ModeManager();
		int nModeCount = pModeManager->GetModeCount();
		for (int i = 0 ; i < nModeCount; i++)
		{
			KInterOccupancePCEMode oInfo;
			KMode* pMode = pModeManager->GetMode(i);
			
			AutoType findOP = mapSelOccupancyPCE.find(pMode->ModeID());
			AutoType endOP	= mapSelOccupancyPCE.end();
			if (findOP != endOP)
			{
				oInfo = findOP->second;
				oInfo.pMode = pMode;
			}
			else
			{
				oInfo.nModeID = pMode->ModeID();
				oInfo.pMode   = pMode;
			}
			a_mapOccupancyPCE.insert(std::make_pair(oInfo.nModeID, oInfo));
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

void KDBaseDefaultInterRunOption::LoadInterUrbanOccupancyPCE( KTarget* a_pTarget, std::vector<TInterModeDefine>& ar_vecInterUrbanMode )
{
	ar_vecInterUrbanMode.clear();

	std::vector<TInterModeDefine>	vecInterModeDefine;
	std::map<int, TInterModeDefine> mapSystemIntermodeDefine;
	std::map<int, TInterModeDefine> mapUserIntermodeDefine;
	
	TInterModeDefine oAutoMode = {};
	oAutoMode.nModeID		= -1;
	oAutoMode.strModeName	= _T("PassengerCar");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oAutoMode.strModeName = _T("Auto");
	}
	oAutoMode.dOccupancy	= 1.2;
	oAutoMode.dPCE			= 1;
	vecInterModeDefine.push_back(oAutoMode);

	TInterModeDefine oTaxiMode = {};
	oTaxiMode.nModeID		= -2;
	oTaxiMode.strModeName	= _T("Taxi");
	oTaxiMode.dOccupancy	= 1.6;
	oTaxiMode.dPCE			= 1;
	vecInterModeDefine.push_back(oTaxiMode);

	TInterModeDefine oBusMode = {};
	oBusMode.nModeID		= -3;
	oBusMode.strModeName	= _T("Bus");
	oBusMode.dOccupancy		= 0;
	oBusMode.dPCE			= 2;
	vecInterModeDefine.push_back(oBusMode);

	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		strSQL.Format(_T("Select Master_Code, detail_mode_id, mode_name, Occupancy, PCE From Default_Intermodal_Highway_Para_Urban Where Master_code = %d "), KDefaultParameterMasterCode::SYSTEM_MASTER_CODE );

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			int nMasterCode = spResultset->GetValueInt(0);

			TInterModeDefine oInfo = {};
			oInfo.nModeID		= spResultset->GetValueInt(1);
			oInfo.strModeName	= spResultset->GetValueString(2);
			oInfo.dOccupancy	= spResultset->GetValueDouble(3);
			oInfo.dPCE			= spResultset->GetValueDouble(4);

			mapSystemIntermodeDefine.insert(std::make_pair(oInfo.nModeID, oInfo));
		}

		for (size_t i = 0; i < vecInterModeDefine.size(); i++)
		{
			TInterModeDefine& oInfo = vecInterModeDefine[i];
			AutoType find = mapSystemIntermodeDefine.find(oInfo.nModeID);
			AutoType fend = mapSystemIntermodeDefine.end();
			if (find != fend)
			{
				oInfo = find->second;
			}
		}

		ar_vecInterUrbanMode = vecInterModeDefine;

		strSQL.Format(_T("Select Master_Code, detail_mode_id, mode_name, Occupancy, PCE From Default_Intermodal_Highway_Para_Urban Where Master_code = %d "), KDefaultParameterMasterCode::USER_MASETER_CODE);

		KResultSetPtr spResultsetUser = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultsetUser->Next())
		{
			int nMasterCode = spResultset->GetValueInt(0);

			TInterModeDefine oInfo = {};
			oInfo.nModeID		= spResultsetUser->GetValueInt(1);
			oInfo.strModeName	= spResultsetUser->GetValueString(2);
			oInfo.dOccupancy	= spResultsetUser->GetValueDouble(3);
			oInfo.dPCE			= spResultsetUser->GetValueDouble(4);

			mapUserIntermodeDefine.insert(std::make_pair(oInfo.nModeID, oInfo));
		}

		if (mapUserIntermodeDefine.size() > 0)
		{
			for (size_t i = 0; i < vecInterModeDefine.size(); i++)
			{
				TInterModeDefine& oInfo = vecInterModeDefine[i];
				AutoType find = mapUserIntermodeDefine.find(oInfo.nModeID);
				AutoType fend = mapUserIntermodeDefine.end();
				if (find != fend)
				{
					oInfo = find->second;
				}
			}
			ar_vecInterUrbanMode = vecInterModeDefine;
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

void KDBaseDefaultInterRunOption::LoadInterRegionalOccupancyPCE( KTarget* a_pTarget, std::vector<TInterModeDefine>& ar_vecInterUrRegionalMode  )
{
	ar_vecInterUrRegionalMode.clear();

	try
	{
		std::vector<TInterModeDefine> vecInterModeDefine;
		std::map<int, TInterModeDefine> mapSystemModeDefine;
		std::map<int, TInterModeDefine> mapUserModeDefine;

		TInterModeDefine oRAutoDefine;
		oRAutoDefine.nModeID	 = -1;
		oRAutoDefine.strModeName = _T("PassengerCar");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oRAutoDefine.strModeName = _T("Auto");
		}
		oRAutoDefine.dOccupancy	 =	1.80;
		oRAutoDefine.dPCE		 =  1.00;
		vecInterModeDefine.push_back(oRAutoDefine);

		std::map<int, int> mapRegionModeCode;
		KDBaseInterModal::GetInterRegionModeDefine(a_pTarget, mapRegionModeCode);
		
		AutoType iter = mapRegionModeCode.begin();
		AutoType end  = mapRegionModeCode.end();
		while(iter != end)
		{
			int nModeCode		= iter->first;
			int nTerminalCode	= iter->second;

			if (nTerminalCode == BUS_TRMINAL)
			{
				KCodeManager* pCodeMgr   = a_pTarget->CodeManager();
				KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
				CString strCodeName = pCodeGroup->SingleCodeValue(nModeCode);

				TInterModeDefine oRBusDefine;
				oRBusDefine.nModeID		= nModeCode;
				oRBusDefine.strModeName = strCodeName;
				oRBusDefine.dOccupancy  = 0.0;
				oRBusDefine.dPCE		= 2.0;

				vecInterModeDefine.push_back(oRBusDefine);
			}
			++iter;
		}

		CString strSQL(_T(""));
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		strSQL.Format(_T("Select Master_Code, detail_mode_id, mode_name, Occupancy, PCE From Default_Intermodal_Highway_Para_Regional Where Master_code = %d "), KDefaultParameterMasterCode::SYSTEM_MASTER_CODE );

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			int nMasterCode = spResultset->GetValueInt(0);

			TInterModeDefine oInfo = {};
			oInfo.nModeID		= spResultset->GetValueInt(1);
			oInfo.strModeName	= spResultset->GetValueString(2);
			oInfo.dOccupancy	= spResultset->GetValueDouble(3);
			oInfo.dPCE			= spResultset->GetValueDouble(4);

			mapSystemModeDefine.insert(std::make_pair(oInfo.nModeID, oInfo));
		}

		for (size_t i = 0; i < vecInterModeDefine.size(); i++)
		{
			TInterModeDefine& oInfo = vecInterModeDefine[i];
			AutoType find = mapSystemModeDefine.find(oInfo.nModeID);
			AutoType fend = mapSystemModeDefine.end();
			if (find != fend)
			{
				TInterModeDefine oGetInfo = find->second;
				oInfo.dOccupancy = oGetInfo.dOccupancy;
				oInfo.dPCE		 = oGetInfo.dPCE;
			}
		}

		ar_vecInterUrRegionalMode = vecInterModeDefine;

		strSQL.Format(_T("Select Master_Code, detail_mode_id, mode_name, Occupancy, PCE From Default_Intermodal_Highway_Para_Regional Where Master_code = %d "), KDefaultParameterMasterCode::USER_MASETER_CODE);

		KResultSetPtr spResultsetUser = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultsetUser->Next())
		{
			int nMasterCode = spResultsetUser->GetValueInt(0);

			TInterModeDefine oInfo = {};
			oInfo.nModeID		= spResultsetUser->GetValueInt(1);
			oInfo.strModeName	= spResultsetUser->GetValueString(2);
			oInfo.dOccupancy	= spResultsetUser->GetValueDouble(3);
			oInfo.dPCE			= spResultsetUser->GetValueDouble(4);

			mapUserModeDefine.insert(std::make_pair(oInfo.nModeID, oInfo));
		}

		if (mapUserModeDefine.size() > 0)
		{
			for (size_t i = 0; i < vecInterModeDefine.size(); i++)
			{
				TInterModeDefine& oInfo = vecInterModeDefine[i];
				AutoType find = mapUserModeDefine.find(oInfo.nModeID);
				AutoType fend = mapUserModeDefine.end();
				if (find != fend)
				{
					TInterModeDefine oGetInfo = find->second;
					oInfo.dOccupancy = oGetInfo.dOccupancy;
					oInfo.dPCE		 = oGetInfo.dPCE;
				}
			}
			ar_vecInterUrRegionalMode = vecInterModeDefine;
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

void KDBaseDefaultInterRunOption::LoadInterODDuration( KTarget* a_pTarget, double& ar_dDuration )
{
	bool   bUseUser(false);
	double dSystemDuation(14.0);
	double dUserDuation(14.0);

	try
	{
		CString strSQL(_T(""));
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		strSQL.Format(_T("Select Master_Code, Time_Duration  From Default_Intermodal_Highway_Para_Duration "));

		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultset->Next())
		{
			int		nMasterCode = spResultset->GetValueInt(0);
			double  dDuation	= spResultset->GetValueDouble(1); 
			if (nMasterCode == KDefaultParameterMasterCode::SYSTEM_MASTER_CODE )
			{
				dSystemDuation = dDuation;
			}
			else if (nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
			{
				dUserDuation = dDuation;
				bUseUser = true;
			}
		}

		if (bUseUser == true)
		{
			ar_dDuration = dUserDuation;
		}
		else
		{
			ar_dDuration = dSystemDuation;
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
