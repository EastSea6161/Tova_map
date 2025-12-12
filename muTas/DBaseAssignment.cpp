#include "StdAfx.h"
#include "DBaseAssignment.h"
#include "Mode.h"
#include "Target.h"
//^^#include "SEDData.h"
#include "AInputInfo.h"
#include "KBulkDBase.h"
//^^ #include "IOTables.h"
#include "ModeManager.h"

using namespace std;

#define CODE_GROUP_KEY_TRANSIT_DETAIL 114


KDBaseAssignment::KDBaseAssignment(void)
{
}


KDBaseAssignment::~KDBaseAssignment(void)
{
}


bool KDBaseAssignment::ModeChoiceExist( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	CString strQuery = _T("select count(*) CNT from modechoice_model");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	int nRecordCount(0);
	bool bExist = false;
	
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		
		if (spResultSet->Next())
			nRecordCount = spResultSet->GetValueInt(0);
		
		if( 0 != nRecordCount )
			bExist = true;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();
	return bExist;
}


bool KDBaseAssignment::BPRFnNameExist( KDBaseConPtr spDBaseConnection, CString a_strFnName)
{
	TxLogDebugStartMsg();
	bool bExist = false;
	CString strQuery;
	int nCount(0);
	
	try
	{
		strQuery.Format(_T("select count(*) from Function_Group where Function_Group_name = '%s'"), a_strFnName);
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if(spResultSet->Next())
			nCount = spResultSet->GetValueInt(0);

		if( 0 != nCount )
			bExist = true;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();
	return bExist;
}


int KDBaseAssignment::GetNewAssignRunSeq( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	int nRunSeq(0);
	CString strQuery = _T("Select ifnull(max(run_seq)+1, 1) as run_seq From assign_model");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if(spResultSet->Next())
			nRunSeq = spResultSet->GetValueInt(0);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
	return nRunSeq;
}


void KDBaseAssignment::UpdateAssignRunSeq( KDBaseConPtr spDBaseConnection, int runSeq, TAssignResultColumnName& resultLinkColumnName, 
											CString strStartDate, CString strEndDate, CString strDescription, int nType )
{
	TxLogDebugStartMsg();

	CString strModeVolumn;
	std::vector<CString> vecModeVolume = resultLinkColumnName.ModeVolumn;
	int nCount = vecModeVolume.size();

	if (nCount == 1)
	{
		strModeVolumn = vecModeVolume[0];
	}
	else if (nCount > 1)
	{
		strModeVolumn = vecModeVolume[0];
		for (int i = 1; i <nCount; i++)
		{
			strModeVolumn = strModeVolumn + _T(", ");
			CString strVolume = vecModeVolume[i];
			strModeVolumn = strModeVolumn + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strModeVolumn = _T("");
	}

	CString strQuery;
	strQuery.Format(_T("Insert Or Replace Into assign_model (run_seq, start_date, end_date, type, average_time, vc, sum_volume, trip, mode_Volume, Description, od_average_time, bus_initial_volume ) Values ( %d, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )"), 
		runSeq,  strStartDate, strEndDate, nType, resultLinkColumnName.AverageTime, resultLinkColumnName.VC, resultLinkColumnName.SumVolume , resultLinkColumnName.Trip, strModeVolumn, strDescription,
		resultLinkColumnName.ODAvgTime, resultLinkColumnName.BusInitialVolumn);

	TxLogDebug(strQuery.AllocSysString());

	spDBaseConnection->ExecuteUpdate(strQuery);
	TxLogDebugEndMsg();
}


void KDBaseAssignment::UpdateOBSingleARunSeq( KDBaseConPtr spDBaseConnection, int runSeq, TAssignResultColumnName& resultLinkColumnName, 
												CString strStartDate, CString strEndDate, CString strDescription, int nType )
{
	CString strModeVolumn;
	std::vector<CString> vecModeVolume = resultLinkColumnName.ModeVolumn;
	int nCount = vecModeVolume.size();

	if (nCount == 1)
	{
		strModeVolumn = vecModeVolume[0];
	}
	else if (nCount > 1)
	{
		strModeVolumn = vecModeVolume[0];
		for (int i = 1; i <nCount; i++)
		{
			strModeVolumn = strModeVolumn + _T(", ");
			CString strVolume = vecModeVolume[i];
			strModeVolumn = strModeVolumn + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strModeVolumn = _T("");
	}

	CString strQuery;
	strQuery.Format(_T("Insert Or Replace Into assign_model (run_seq, start_date, end_date, type, average_time, vc, sum_volume, mode_Volume, ave_time_toll, bus_initial_volume, od_average_time, Description ) Values ( %d, '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )"), 
		runSeq,  strStartDate, strEndDate, nType, resultLinkColumnName.AverageTime, resultLinkColumnName.VC, resultLinkColumnName.SumVolume, strModeVolumn, 
		resultLinkColumnName.AvgTimeToll, resultLinkColumnName.BusInitialVolumn, resultLinkColumnName.ODAvgTime, strDescription);

	spDBaseConnection->ExecuteUpdate(strQuery);
	TxLogDebugEndMsg();
}


void KDBaseAssignment::UpdateOBMultiARunSeq( KDBaseConPtr spDBaseConnection, int runSeq, TAssignResultColumnName& resultLinkColumnName, CString strStartDate, CString strEndDate, CString strDescription, int nType )
{
	//ModeVolume;
	CString strModeVolumn;
	std::vector<CString> vecModeVolum = resultLinkColumnName.ModeVolumn;
	int nCount = vecModeVolum.size();
	if (nCount == 1)
	{
		strModeVolumn = vecModeVolum[0];
	}
	else if (nCount > 1)
	{
		strModeVolumn = vecModeVolum[0];
		for (int i = 1; i <nCount; i++)
		{
			strModeVolumn = strModeVolumn + _T(", ");
			CString strVolume = vecModeVolum[i];
			strModeVolumn = strModeVolumn + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strModeVolumn = _T("");
	}

	//VDFTime;
	CString strVDFTime;
	std::vector<CString> vecVDFTime = resultLinkColumnName.VDFTime;
	nCount = vecVDFTime.size();
	if (nCount == 1)
	{
		strVDFTime = vecVDFTime[0];
	}
	else if (nCount > 1)
	{
		strVDFTime = vecVDFTime[0];
		for (int i = 1; i <nCount; i++)
		{
			strVDFTime = strVDFTime + _T(", ");
			CString strVolume = vecVDFTime[i];
			strVDFTime = strVDFTime + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strVDFTime = _T("");
	}

	//VDFTollTime;
	CString strVDFTollTime;
	std::vector<CString> vecVDFTollTime = resultLinkColumnName.VDFTollTime;
	nCount = vecVDFTollTime.size();
	if (nCount == 1)
	{
		strVDFTollTime = vecVDFTollTime[0];
	}
	else if (nCount > 1)
	{
		strVDFTollTime = vecVDFTollTime[0];
		for (int i = 1; i <nCount; i++)
		{
			strVDFTollTime = strVDFTollTime + _T(", ");
			CString strVolume = vecVDFTollTime[i];
			strVDFTollTime = strVDFTollTime + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strVDFTollTime = _T("");
	}

	//VDFODTime
	CString strODVDFTime;
	std::vector<CString> vecODVDFTime = resultLinkColumnName.ODVDFTime;
	nCount = vecODVDFTime.size();
	if (nCount == 1)
	{
		strODVDFTime = vecODVDFTime[0];
	}
	else if (nCount > 1)
	{
		strODVDFTime = vecODVDFTime[0];
		for (int i = 1; i <nCount; i++)
		{
			strODVDFTime = strODVDFTime + _T(", ");
			CString strVolume = vecODVDFTime[i];
			strODVDFTime = strODVDFTime + strVolume;
		}

	}
	else if (nCount = 0)
	{
		strODVDFTime = _T("");
	}
	
	CString strQuery;
	strQuery.Format(_T("Insert Or Replace Into assign_model (run_seq, start_date, end_date, type, average_time, vc, sum_volume, mode_Volume, ave_time_toll, bus_initial_volume, od_average_time, Description, vdf_time, vdf_toll_time, od_vdf_time ) Values ( %d, '%s', '%s', %d,'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s' )"), 
		runSeq,  strStartDate, strEndDate, nType, resultLinkColumnName.AverageTime, resultLinkColumnName.VC, resultLinkColumnName.SumVolume, strModeVolumn, 
		resultLinkColumnName.AvgTimeToll, resultLinkColumnName.BusInitialVolumn, resultLinkColumnName.ODAvgTime, strDescription, strVDFTime, strVDFTollTime, strODVDFTime);

	spDBaseConnection->ExecuteUpdate(strQuery);
	TxLogDebugEndMsg();
}


bool KDBaseAssignment::GetSumVolumeColumns(KTarget* pTarget, CStringArray& columnsNameArray)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		CString strSQL  = _T(" select sum_volume from assign_model group by (sum_volume) order by run_seq desc");    

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResult->Next())
		{
			columnsNameArray.Add(spResult->GetValueString(0));       
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KDBaseAssignment::TransitDataExist( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	bool bResult = false;
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		int nRecordCount(0);
		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) AS CNT FROM %s"), TABLE_TRANSIT);
		
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		if (oResultSetPtr->Next())
			nRecordCount = oResultSetPtr->GetValueInt(0);
	
		if (nRecordCount > 0)
			bResult = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
	TxLogDebugEndMsg();
	return bResult;
}


void KDBaseAssignment::GetRunHighwayAssignInfo( KTarget* a_pTarget, std::vector<TAssignResultInfo>& ar_vecAssignResultInfo )
{
	TxLogDebugStartMsg();
	ar_vecAssignResultInfo.clear();

	CString strSQL;
	strSQL =  _T(" Select run_seq, start_date, end_date, description, ") ;
	strSQL += _T(  " vc, average_time, vdf_time, sum_volume, mode_volume, od_average_time, od_vdf_time, ave_time_toll,vdf_toll_time, trip "); 
	strSQL += _T(" From assign_model Where type < 5 "); 
	strSQL += _T(" Order by run_seq ");

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			TAssignResultInfo oAssignResultInfo;

            oAssignResultInfo.TnRunSeq			= spResultSet->GetValueInt   (0);
            oAssignResultInfo.TstrStartDate		= spResultSet->GetValueString(1);
            oAssignResultInfo.TstrEndDate		= spResultSet->GetValueString(2);
            oAssignResultInfo.TstrDescription	= spResultSet->GetValueString(3);
            oAssignResultInfo.TstrVC			= spResultSet->GetValueString(4);
            oAssignResultInfo.TstrAverageTime	= spResultSet->GetValueString(5);
            oAssignResultInfo.TstrVDFTime		= spResultSet->GetValueString(6);
            oAssignResultInfo.TstrSumVolumn		= spResultSet->GetValueString(7);
            oAssignResultInfo.TstrModeVolume	= spResultSet->GetValueString(8);
            oAssignResultInfo.TstrODAveTime	    = spResultSet->GetValueString(9);
            oAssignResultInfo.TstrODVDFTime		= spResultSet->GetValueString(10);
            oAssignResultInfo.TstrAVETimeToll   = spResultSet->GetValueString(11);
            oAssignResultInfo.TstrVDFTimeToll	= spResultSet->GetValueString(12);
            oAssignResultInfo.TstrTrip			= spResultSet->GetValueString(13);

			ar_vecAssignResultInfo.push_back(oAssignResultInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 

	TxLogDebugEndMsg();
}


void KDBaseAssignment::UpdateAssignPathmodel( KDBaseConPtr spDBaseConnection, int runSeq, int nRunOption, int nZoneThrow, CString strAveTime, CString strStartDate, CString strEndDate )
{
	TxLogDebugStartMsg();
	CString strDeleteQurery;
	strDeleteQurery.Format(_T("Delete from assign_path_model "));

	CString strQuery;
	strQuery.Format(_T("Insert Or Replace Into assign_path_model(start_date, end_date, run_option, avrtime_column, seq, zone_through) Values ('%s', '%s', %d, '%s', %d, %d)"),
		strStartDate, strEndDate, nRunOption, strAveTime, runSeq, nZoneThrow);
	
	spDBaseConnection->ExecuteUpdate(strDeleteQurery);
	spDBaseConnection->ExecuteUpdate(strQuery);
	TxLogDebugEndMsg();
}


void KDBaseAssignment::UpdateAssignpathInfo( KDBaseConPtr spDBaseConnection, std::list<KAInputInfo*> a_lstAInfo, TAssignResultColumnName& resultLinkColumnName )
{
	TxLogDebugStartMsg();
	CString strDeleteQurery;
	strDeleteQurery.Format(_T("Delete from assign_path_input"));
	spDBaseConnection->ExecuteUpdate(strDeleteQurery);

	int nIndex = 0;
	std::list<KAInputInfo*>::iterator itInput,itEnd = a_lstAInfo.end();
	for(itInput = a_lstAInfo.begin(); itInput != itEnd; ++itInput)
	{
		KAInputInfo* pInput = *itInput;
		if (pInput->Selected() == true)
		{
			KMode* pMode = pInput->Mode();
			int nModeID = pMode->ModeID();
			CString strInputColumnName = pInput->InputColumn()->Name();
			double dOccupancy = pInput->Occupancy();
			double dpcu		  = pInput->PCU();
			int nNetworkType  = pInput->NetworkType();
			int nPreLoad = 0;
			if (pInput->PreLoad() == true)
			{
				nPreLoad = 1;
			} 
			
			CString strModeColumn =resultLinkColumnName.ModeVolumn[nIndex];
			nIndex++;

			CString strQurery;
			strQurery.Format(_T("Insert Or Replace Into assign_path_input (mode_id, occupancy, pcu, network_type, preload, Column_name, InputColumnName ) Values (%d, %f, %f, %d, %d, '%s', '%s')"),
				nModeID, dOccupancy, dpcu, nNetworkType, nPreLoad, strModeColumn, strInputColumnName);
			spDBaseConnection->ExecuteUpdate(strQurery);
		}
		
	}
	TxLogDebugEndMsg();
}


void KDBaseAssignment::DeleteAssignPathInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strDeleteQurery;
		strDeleteQurery.Format(_T("Delete from assign_path_input"));
		spDBaseConnection->ExecuteUpdate(strDeleteQurery);
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


void KDBaseAssignment::UpdateAssignTurnVolumeInfo( KDBaseConPtr spDBaseConnection, std::list<KAInputInfo*> a_lstAinfo, KARunningOptionInfo a_oRuningOption )
{
	try
	{
		CString strDeleteQurery;
		strDeleteQurery.Format(_T("Delete from %s"), TABLE_ASSIGN_TURN_VOLUME_INPUT);
		spDBaseConnection->ExecuteUpdate(strDeleteQurery);

		if (a_oRuningOption.IncludeTurnVolume() == false)
		{
			return;
		}

		int nIndex(0);
		CString strMode(_T(""));
		std::list<KAInputInfo*>::iterator itInput,itEnd = a_lstAinfo.end();
		for(itInput = a_lstAinfo.begin(); itInput != itEnd; ++itInput)
		{
			KAInputInfo* pInput = *itInput;
			if (pInput->Selected() == true)
			{
				KMode* pMode			= pInput->Mode();
				int nModeID				= pMode->ModeID();
				if (nIndex == 0)
				{
					strMode.Format(_T("%d"), nModeID);
				}
				else
				{
					strMode.AppendFormat(_T(",%d"), nModeID);
				}
				nIndex++;
			}
		}

		int nBusVolume(0);
		if (a_oRuningOption.IncludeBusInitialVolume() == true)
		{
			nBusVolume = 1;
		}

		CString strQurery;
		strQurery.Format(_T("Insert Into %s ( mode, busvolume ) Values ( '%s', '%d' )"), TABLE_ASSIGN_TURN_VOLUME_INPUT, strMode, nBusVolume);
		spDBaseConnection->ExecuteUpdate(strQurery);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KDBaseAssignment::DeleteAssignTurnVolumnInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strDeleteQurery;
		strDeleteQurery.Format(_T("Delete from %s"), TABLE_ASSIGN_TURN_VOLUME_INPUT);
		spDBaseConnection->ExecuteUpdate(strDeleteQurery);
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



void KDBaseAssignment::GetModeChoiceModelinfo( KTarget* a_pTarget, std::map<CString, int>& a_mapModeChoice )
{
	TxLogDebugStartMsg();

	a_mapModeChoice.clear();

	CString strQuery = _T("select detail_mode_id, mode_od_column_name from modechoice_model");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{
			int nModeId			  = spResultSet->GetValueInt(0);
			CString strColumnName = spResultSet->GetValueString(1);
			a_mapModeChoice.insert(std::make_pair(strColumnName, nModeId));
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


void KDBaseAssignment::GetLinkTravelTimeField( KTarget* a_pTarget, std::set<CString>& a_lstTraveTimeField )
{
	a_lstTraveTimeField.clear();

	CString strQuery = _T("select average_time from assign_model WHERE average_time is not null AND trim(average_time) != '' ");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{
			CString strColumnName = spResultSet->GetValueString(0);
			a_lstTraveTimeField.insert(strColumnName);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}


bool KDBaseAssignment::TravelTimeExist( KTarget* a_pTarget )
{
	TxLogDebugStartMsg();
	CString strQuery = _T("select count(*) CNT from assign_model");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	int nRecordCount(0);
	bool bExist = false;

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		if (spResultSet->Next())
			nRecordCount = spResultSet->GetValueInt(0);

		if( 0 != nRecordCount )
			bExist = true;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebugEndMsg();
	return bExist;
}


void KDBaseAssignment::GetMulitimodalAssignResultInfo( KTarget* a_pTarget, std::vector<TAssignResultInfo>& a_vecMAResultInfo )
{
	TxLogDebugStartMsg();
	a_vecMAResultInfo.clear();

	CString strSQL;
	strSQL =	_T("select run_seq, start_date, end_date, type, description, average_time, "); // 6
	strSQL +=	_T(" vc, sum_volume, trip, mode_volume, vdf_time, ave_time_toll, vdf_toll_time, "); //7
	strSQL +=	_T(" bus_initial_volume, od_average_time, od_vdf_time, od_average_bus, od_average_subway, od_average_bussubway, "); //6
	strSQL +=	_T(" od_bus_direct, od_bus_one_transfer, od_bus_two_transfer, linfo_num_passengers, "); //4
	strSQL +=   _T(" linfo_total_length, linfo_avg_length, linfo_max_passengers, linfo_avg_passengers, "); //4
	strSQL +=   _T(" linfo_max_second, linfo_avg_second, linfo_max_standing, linfo_avg_standing, ");  //4
	strSQL +=   _T(" linfo_max_congestion, linfo_avg_congestion, linfo_km_passengers, linfo_once_passengers, linfo_km_once_passengers, "); //5
	strSQL +=	_T(" od_bus, od_rail, od_busrail,od_inter_mode, "); //4
	strSQL +=   _T(" line_board_net, line_board_transfer, line_board_total, "); //3
	strSQL +=   _T(" line_alight_net, line_alight_transfer, line_alight_total, line_passenger, "); //4
	strSQL +=   _T(" node_bus_net_board, node_bus_transfer_board, node_bus_sum_board, "); //3
	strSQL +=   _T(" node_bus_net_alight, node_bus_transfer_alight, node_bus_sum_alight,"); //3
	strSQL +=   _T(" pr_auto, pr_bus, pr_rail, pr_transit, "); //4
	strSQL +=   _T(" terminal_trip, "); //1
	strSQL +=   _T(" tr_ac_auto, tr_ac_taxi, tr_ac_bus, tr_ac_rail, tr_ac_transit, "); //5
	strSQL +=   _T(" tr_er_auto, tr_er_taxi, tr_er_bus, tr_er_rail, tr_er_transit, tr_total "); //6
	strSQL +=   _T("From assign_model where type = 5");

	TxLogDebug(strSQL.AllocSysString());

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			TAssignResultInfo oMAssignResultInfo;
			oMAssignResultInfo.TnRunSeq				 = spResultSet->GetValueInt(0);
			oMAssignResultInfo.TstrStartDate		 = spResultSet->GetValueString(1);
			oMAssignResultInfo.TstrEndDate			 = spResultSet->GetValueString(2);
			oMAssignResultInfo.TnType				 = spResultSet->GetValueInt(3);
			oMAssignResultInfo.TstrDescription		 = spResultSet->GetValueString(4);
			oMAssignResultInfo.TstrAverageTime		 = spResultSet->GetValueString(5);
			oMAssignResultInfo.TstrVC				 = spResultSet->GetValueString(6);
			oMAssignResultInfo.TstrSumVolumn		 = spResultSet->GetValueString(7);
			oMAssignResultInfo.TstrTrip				 = spResultSet->GetValueString(8);
			oMAssignResultInfo.TstrModeVolume		 = spResultSet->GetValueString(9);
			oMAssignResultInfo.TstrVDFTime			 = spResultSet->GetValueString(10);
			oMAssignResultInfo.TstrAVETimeToll		 = spResultSet->GetValueString(11);
			oMAssignResultInfo.TstrVDFTimeToll		 = spResultSet->GetValueString(12);
			oMAssignResultInfo.TstrBusInitVolume	 = spResultSet->GetValueString(13);
			oMAssignResultInfo.TstrODAveTime		 = spResultSet->GetValueString(14);
			oMAssignResultInfo.TstrODVDFTime		 = spResultSet->GetValueString(15);
			oMAssignResultInfo.TstrODAveBus			 = spResultSet->GetValueString(16);
			oMAssignResultInfo.TstrODAveSubway		 = spResultSet->GetValueString(17);
			oMAssignResultInfo.TstrODAveBusSubway	 = spResultSet->GetValueString(18);
			oMAssignResultInfo.TstrODBusDirect		 = spResultSet->GetValueString(19);
			oMAssignResultInfo.TstrODBusOneTrans	 = spResultSet->GetValueString(20);
			oMAssignResultInfo.TstrODBusTwoTrans	 = spResultSet->GetValueString(21);
			oMAssignResultInfo.TstrLInfoPassengernum = spResultSet->GetValueString(22);
			oMAssignResultInfo.TstrLinfoTotalLength  = spResultSet->GetValueString(23);
			oMAssignResultInfo.TstrLinfoAvgLength  = spResultSet->GetValueString(24);
			oMAssignResultInfo.TstrLinfoMaxPassenger  = spResultSet->GetValueString(25);
			oMAssignResultInfo.LinfoAvePassenger  = spResultSet->GetValueString(26);
			oMAssignResultInfo.TstrLinfoMaxSecond  = spResultSet->GetValueString(27);
			oMAssignResultInfo.TstrLinfoAvgSecond  = spResultSet->GetValueString(28);
			oMAssignResultInfo.TstrLinfoMaxStanding  = spResultSet->GetValueString(29);
			oMAssignResultInfo.TstrLinfoAvgStanding  = spResultSet->GetValueString(30);
			oMAssignResultInfo.TstrLinfoMaxCongestion  = spResultSet->GetValueString(31);
			oMAssignResultInfo.TstrLinfoAvgCongestion  = spResultSet->GetValueString(32);
			oMAssignResultInfo.TstrLinfoKMPassenger  = spResultSet->GetValueString(33);
			oMAssignResultInfo.TstrLinfoOncePassenger  = spResultSet->GetValueString(34);
			oMAssignResultInfo.TstrLinfoKMOncePassenger  = spResultSet->GetValueString(35);
			oMAssignResultInfo.TstrODMBus				= spResultSet->GetValueString(36);
			oMAssignResultInfo.TstrODMRail				= spResultSet->GetValueString(37);
			oMAssignResultInfo.TstrODMBusRail			= spResultSet->GetValueString(38);
			oMAssignResultInfo.TstrODMInterMode			= spResultSet->GetValueString(39);
			oMAssignResultInfo.TstrLineBoradNet			= spResultSet->GetValueString(40);
			oMAssignResultInfo.TstrLineBoradTransfer	= spResultSet->GetValueString(41);
			oMAssignResultInfo.TstrLineBoradTotal		= spResultSet->GetValueString(42);
			oMAssignResultInfo.TstrLineAlightNet		= spResultSet->GetValueString(43);
			oMAssignResultInfo.TstrLineAlightTransfer	= spResultSet->GetValueString(44);
			oMAssignResultInfo.TstrLineAlightTotal		 = spResultSet->GetValueString(45);
			oMAssignResultInfo.TstrLinePassenger		= spResultSet->GetValueString(46);
			oMAssignResultInfo.TstrNodeBusNetBorad		= spResultSet->GetValueString(47);
			oMAssignResultInfo.TstrNodeBusTransferBoard = spResultSet->GetValueString(48);
			oMAssignResultInfo.TstrNodeBusSumBoard		= spResultSet->GetValueString(49);
			oMAssignResultInfo.TstrNodeBusNetAlight		= spResultSet->GetValueString(50);
			oMAssignResultInfo.TstrNodeBusTransferAlight= spResultSet->GetValueString(51);
			oMAssignResultInfo.TstrNodeBusSumAlight		= spResultSet->GetValueString(52);
			oMAssignResultInfo.TstrNodePRAuto			= spResultSet->GetValueString(53);
			oMAssignResultInfo.TstrNodePRBus			= spResultSet->GetValueString(54);
			oMAssignResultInfo.TstrNodePRRail			= spResultSet->GetValueString(55);
			oMAssignResultInfo.TstrNodePRTransit		= spResultSet->GetValueString(56);
			oMAssignResultInfo.TstrTerminalTrip			= spResultSet->GetValueString(57);
			oMAssignResultInfo.TstrTR_AC_Auto			= spResultSet->GetValueString(58);
			oMAssignResultInfo.TstrTR_AC_Taxi			= spResultSet->GetValueString(59);
			oMAssignResultInfo.TstrTR_AC_Bus			= spResultSet->GetValueString(60);
			oMAssignResultInfo.TstrTR_AC_Subway			= spResultSet->GetValueString(61);
			oMAssignResultInfo.TstrTR_AC_Transit		= spResultSet->GetValueString(62);
			oMAssignResultInfo.TstrTR_ER_Auto			= spResultSet->GetValueString(63);
			oMAssignResultInfo.TstrTR_ER_Taxi			= spResultSet->GetValueString(64);
			oMAssignResultInfo.TstrTR_ER_Bus			= spResultSet->GetValueString(65);
			oMAssignResultInfo.TstrTR_ER_Subway			= spResultSet->GetValueString(66);
			oMAssignResultInfo.TstrTR_ER_Transit		= spResultSet->GetValueString(67);
			oMAssignResultInfo.TstrTR_Total				= spResultSet->GetValueString(68);

			a_vecMAResultInfo.push_back(oMAssignResultInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}


void KDBaseAssignment::GetNoClass( KTarget* a_pTarget, std::map<CString, double>& a_mapNoClass )
{
	a_mapNoClass.clear();

	CString strQuery = _T("select class_name,time_min from access_class ");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{
			CString strClassName = spResultSet->GetValueString(0);
			double	dTimemin	 = spResultSet->GetValueDouble(1);
			a_mapNoClass.insert(std::make_pair(strClassName, dTimemin));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDBaseAssignment::GetUtilitFnCoffcient( KTarget* a_pTarget, std::vector<TMAFnCoefficient>& a_mapFnCoeffcient )
{
	a_mapFnCoeffcient.clear();

	CString strQuery = _T("select Transit_Type, In_Vehicle_Time, Wait_Time, Transfer_Time, Access_Walk_Time, Egress_Walk_Time, Num_Transfers, Cost, Num_Stop from bus_path_coefficient");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	TMAFnCoefficient oTFnCoffInfo;
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{
			oTFnCoffInfo.TnMode				= spResultSet->GetValueInt(0);
			oTFnCoffInfo.TdInVehcle			= spResultSet->GetValueDouble(1);
			oTFnCoffInfo.TdWaitingTime		= spResultSet->GetValueDouble(2);
			oTFnCoffInfo.TdTransferTime		= spResultSet->GetValueDouble(3);
			oTFnCoffInfo.TdAccessWalkTime	= spResultSet->GetValueDouble(4);
			oTFnCoffInfo.TdEgressWalkTime	= spResultSet->GetValueDouble(5);
			oTFnCoffInfo.TdNoOfTransfer		= spResultSet->GetValueDouble(6);
			oTFnCoffInfo.TdCost				= spResultSet->GetValueDouble(7);
			oTFnCoffInfo.TdNoOfStop			= spResultSet->GetValueDouble(8);

			a_mapFnCoeffcient.push_back(oTFnCoffInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDBaseAssignment::GetOptionCost( KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost )
{
    a_mapTransitCost.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	TMATransitCostInfo oTCostInfo;
	try 
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT Transit_Type, Transit_Detail_Type, TBL2.KCode_Display AS Detail_Display,  Base_Cost, Base_Length, Add_Cost, Add_Length, Cost_Method ")
			_T(" FROM ")
			_T(" (SELECT Transit_Type, Transit_Detail_Type, Base_Cost, Base_Length, Add_Cost, Add_Length, Cost_Method FROM %s)TBL1 ")
			_T(" LEFT OUTER JOIN ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL2  ")
			_T(" ON TBL2.KCode_Key = TBL1.Transit_Detail_Type  "), TABLE_BUS_PATH_COST, CODE_GROUP_KEY_TRANSIT_DETAIL);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			TMATransitCostInfo oTCostInfo;
			oTCostInfo.TnMode			= spResult->GetValueInt   (0);
			oTCostInfo.TnType			= spResult->GetValueInt   (1);
			oTCostInfo.TstrTypeName     = spResult->GetValueString(2);
			oTCostInfo.TdBasicRate      = spResult->GetValueDouble(3);
			oTCostInfo.TdBasicDistance  = spResult->GetValueDouble(4);
			oTCostInfo.TdAddRate        = spResult->GetValueDouble(5);
			oTCostInfo.TdAddDistance    = spResult->GetValueDouble(6);
			oTCostInfo.TnMergeRate      = spResult->GetValueInt   (7);

			a_mapTransitCost.push_back(oTCostInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapTransitCost.clear();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		a_mapTransitCost.clear();
		TxLogDebugException();
		throw 1;
	}
}


void KDBaseAssignment::GetInitBusPathValue( KTarget* a_pTarget,TMATransitOptionsBaseInfo& a_oBaseInfo )
{
	CString strQuery = _T("select Max_Routes, Time_Board, Wait_Vdf_Alpha, Wait_Vdf_Beta, UseBusSubway, DwellTime, path_time, invehicle_time, waiting_time, transfer_time, ae_time, path_length from bus_path_input");
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);
		while (spResultSet->Next())
		{

			a_oBaseInfo.TnMaxofNo					= spResultSet->GetValueInt(0);
			a_oBaseInfo.TdDWTimeInBus	= spResultSet->GetValueDouble(1);
			a_oBaseInfo.TdAlpha						= spResultSet->GetValueDouble(2);
			a_oBaseInfo.TdBeta						= spResultSet->GetValueDouble(3);
			a_oBaseInfo.TnUsingBusSubway			= spResultSet->GetValueInt(4);
			a_oBaseInfo.TdDWTimeInRail					= spResultSet->GetValueDouble(5);
			a_oBaseInfo.TdPathTime					= spResultSet->GetValueDouble(6);
			a_oBaseInfo.TnInvehicleTime				= spResultSet->GetValueInt(7);
			a_oBaseInfo.TnWatingTime				= spResultSet->GetValueInt(8);
			a_oBaseInfo.TnTransferTime				= spResultSet->GetValueInt(9);
			a_oBaseInfo.TnAETime					= spResultSet->GetValueInt(10);
			a_oBaseInfo.TdPathLength				= spResultSet->GetValueDouble(11);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


bool KDBaseAssignment::ExportDB2NodeFileExcludeNodeOff( KDBaseConPtr a_spDBaseConnection, CString strFileName/*=_T("node.dat")*/ )
{
    ofstream ofs(strFileName, ios::binary);
	if (! ofs) {
		return false;
	}

	//network_off read
	CString strNodeOffResult(_T(""));
	CString strNodeOffSQL = _T("select node_off from network_off");

	KResultSetPtr spResultSet;

	try
	{
		spResultSet = a_spDBaseConnection->ExecuteQuery(strNodeOffSQL);

		if(spResultSet->Next())
			strNodeOffResult = spResultSet->GetValueString(0);
	}
	catch (...)
	{
		ofs.close();
		return false;
	}

	CString strTok(_T(""));
	int nLoop = 0;
	std::vector<CString> vecOffType;

	while (AfxExtractSubString(strTok, strNodeOffResult, nLoop++, ','))
	{
		if (strTok.CompareNoCase(_T("")) == 0)
			break;

		vecOffType.push_back(strTok);
	}
	//export node to file
	CString strSQL = _T(" Select node_id, node_type, x, y, zone_id From Node");

	if (vecOffType.size() != 0)
	{
		strSQL.Append(_T(" where node_type not in ("));
	}

	int nSize = vecOffType.size();

	for (int i = 0; i < nSize; ++i)
	{
		if (0 != i)
			strSQL.Append(_T(","));

		strSQL.Append(_T(" ") + vecOffType[i]);

		if (nSize - 1 == i)
			strSQL.Append(_T(" )"));
	}
	strSQL.Append(_T(" Order By node_id "));

	try
	{
		spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

		TFixedNode node;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			node.node_id   = spResultSet->GetValueInt64 (0);
			node.node_type = spResultSet->GetValueInt   (1);
			node.x         = spResultSet->GetValueDouble(2);
			node.y         = spResultSet->GetValueDouble(3);
			node.zone_id   = spResultSet->GetValueInt64 (4);

			ofs.write(reinterpret_cast<char*>( &node   ), sizeof(TFixedNode) );  
		}
	}
	catch (...)
	{
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}


bool KDBaseAssignment::ExportDB2ODFile( KDBaseConPtr a_spDBaseConnection, CString strTableName, KIOColumns& userColumnCollection, CString strFileName )
{
	CString strSQL = _T(" Select ozone_id, dzone_id ") ;

	KIOColumn* pColumn = NULL;
	int nColumnCount = userColumnCollection.ColumnCount();
	for (int i=0; i<nColumnCount; i++)
	{
		pColumn = userColumnCollection.GetColumn(i);
		strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
	}

	strSQL = strSQL + _T(" From ") + strTableName;
	strSQL = strSQL + _T(" Order By ozone_id, dzone_id");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

		TFixedOD fixedOD;
		while(spResultSet->Next())
		{
			//ofs.write(reinterpret_cast<char*>)
			fixedOD.ozone_id  = spResultSet->GetValueInt64(0);
			fixedOD.dzone_id  = spResultSet->GetValueInt64(1);

			ofs.write(reinterpret_cast<char*>( &fixedOD ), sizeof(TFixedOD) );  

			for (int i=0; i<nColumnCount; i++)
			{
				double dValue = spResultSet->GetValueDouble(2+i);
				ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
			}
		}
	}
	catch (...)
	{
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}


bool KDBaseAssignment::ExportDB2LinkFileWithOutHighway( KDBaseConPtr a_spDBaseConnection, int nUseLinkTime, KIOColumn* a_pColumn, CString strFileName /*= _T("link.dat")*/ )
{
	CString strSQL = _T("select link_id, fnode_id, tnode_id, link_type, length, speed, ");
	if (nUseLinkTime == 1)
	{
		strSQL = strSQL + _T("0");
	}
	else
	{
		CString strColumnName = a_pColumn->Name();
		strSQL = strSQL + strColumnName;
	}

	strSQL = strSQL + _T(" From Link Order By link_id");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs) {
		return false;
	}

	try
	{
		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

		TMALInkInfoSpeed TlinkInfo;
		while(spResultSet->Next())
		{
			TlinkInfo.TnxLink_ID	= spResultSet->GetValueInt64 (0);
			TlinkInfo.TnxFNode_ID	= spResultSet->GetValueInt64 (1);
			TlinkInfo.TnxTNode_ID	= spResultSet->GetValueInt64 (2);
			TlinkInfo.TnLinkType	= spResultSet->GetValueInt   (3);
			TlinkInfo.TdLength		= spResultSet->GetValueDouble(4);
			TlinkInfo.TdSpeed		= spResultSet->GetValueDouble(5);
			TlinkInfo.TdAddField	= spResultSet->GetValueDouble(6);

			ofs.write(reinterpret_cast<char*>( &TlinkInfo ), sizeof(TMALInkInfoSpeed));
		}
	}
	catch (...)
	{
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}


void KDBaseAssignment::UpdateMuliModalRunSeq( KDBaseConPtr spDBaseConnection, int nRunSeq, TAssignResultColumnName& resultColumnName, CString strStartDate, CString strEndDate, CString strDescription, int nModeType )
{
	//Mode Volum;
	/*TxLogDebugStartMsg();*/
	try
	{
		CString strModeVolumn;
		std::vector<CString> vecModeVolum = resultColumnName.ModeVolumn;
		int nCount = vecModeVolum.size();
		if (nCount == 1) {
			strModeVolumn = vecModeVolum[0];
		}
		else if (nCount > 1) {
			strModeVolumn = vecModeVolum[0];
			for (int i = 1; i <nCount; i++)
			{
				strModeVolumn = strModeVolumn + _T(", ");
				CString strVolume = vecModeVolum[i];
				strModeVolumn = strModeVolumn + strVolume;
			}
		}
		else if (nCount = 0) {
			strModeVolumn = _T("");
		}

		CString strInterModeVolumn(_T(""));
		std::vector<CString> vecIntermode = resultColumnName.InterMode;
		if (vecIntermode.size() > 1) {
			for (size_t i = 0; i < vecIntermode.size(); i++) {
				if (i == 0) {
					strInterModeVolumn = vecIntermode[i];
				}
				else {
					strInterModeVolumn.AppendFormat(_T(", %s"), vecIntermode[i]);
				}
			}
		}

		CString strQuery;
		strQuery.Append			(_T(" Insert Or Replace Into assign_model"));
		strQuery.Append			(_T(" (run_seq, start_date, end_date, type, Description, "));
		strQuery.Append			(_T(" average_time, vc, sum_volume, trip, mode_volume, bus_Initial_volume, "));
		strQuery.Append			(_T(" od_average_time, od_average_bus, od_average_subway, od_average_bussubway, ")); 
		strQuery.Append			(_T(" od_bus_direct, od_bus_one_transfer,od_bus_two_transfer, "));
		strQuery.Append			(_T(" od_bus, od_rail, od_busrail, od_inter_mode, "));
		strQuery.Append			(_T(" linfo_num_passengers, "));
		strQuery.Append			(_T(" linfo_total_length, linfo_avg_length, linfo_max_passengers, linfo_avg_passengers, "));
		strQuery.Append			(_T(" linfo_max_second, linfo_avg_second, linfo_max_standing, linfo_avg_standing, "));
		strQuery.Append			(_T(" linfo_max_congestion, linfo_avg_congestion, linfo_km_passengers, linfo_once_passengers, linfo_km_once_passengers, "));
		strQuery.Append			(_T(" line_board_net, line_board_Transfer, line_board_Total, "));
		strQuery.Append			(_T(" line_Alight_net, line_Alight_Transfer, line_Alight_Total, "));
		strQuery.Append			(_T(" line_passenger, ")); 
		strQuery.Append			(_T(" node_bus_net_board, node_bus_transfer_board, node_bus_sum_board, "));
		strQuery.Append			(_T(" node_bus_net_alight, node_bus_transfer_alight, node_bus_sum_alight, "));
		strQuery.Append			(_T(" pr_auto, pr_bus, pr_rail, pr_transit, "));
		strQuery.Append			(_T(" terminal_trip,  "));
		strQuery.Append			(_T(" tr_ac_auto, tr_ac_taxi, tr_ac_bus, tr_ac_rail, tr_ac_transit, "));
		strQuery.Append			(_T(" tr_er_auto, tr_er_taxi, tr_er_bus, tr_er_rail, tr_er_transit, tr_total )"));
		strQuery.Append			(_T(" Values ( "));

		strQuery.AppendFormat	(_T(" %d, '%s', '%s', %d, '%s', "), nRunSeq, strStartDate, strEndDate, nModeType, strDescription);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', '%s', "), resultColumnName.AverageTime, resultColumnName.VC, 
			                                                                  resultColumnName.SumVolume, resultColumnName.Trip, strModeVolumn, resultColumnName.BusInitialVolumn);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.ODAvgTime, resultColumnName.ODAvgBusTime,
			                                                      resultColumnName.ODAvgSubwayTime, resultColumnName.ODAVgBusSubwayTime);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.ODBusDirect, resultColumnName.ODBusOneTransfer, resultColumnName.ODBustwoTransfer);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.ODMBus, resultColumnName.ODMRail, resultColumnName.ODMBusRail, strInterModeVolumn );
		strQuery.AppendFormat	(_T(" '%s', "), resultColumnName.LinfoPassenger);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.LinfoTotalLength, resultColumnName.LinfoAvgLength, resultColumnName.LinfoMaxPassenger, resultColumnName.LinfoAvePassenger );
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.LinfoMaxSecond, resultColumnName.LinfoAvgSecond, resultColumnName.LinfoMaxStanding, resultColumnName.LinfoAvgStanding );
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', "), resultColumnName.LinfoMaxCongestion, resultColumnName.LinfoAvgCongestion, resultColumnName.LinfoKMPassenger, resultColumnName.LinfoOncePassenger, resultColumnName.LinfoKMOncePassenger );

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.LineBoardNet, resultColumnName.LineBoardTransfer, resultColumnName.LineBoardTotal);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.LineAlightNet, resultColumnName.LineAlightTransfer, resultColumnName.LineAlightTotal);
		strQuery.AppendFormat	(_T(" '%s',"), resultColumnName.LinePassenger);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.NodeBusNetBorad, resultColumnName.NodeBusTransferBoard, resultColumnName.NodeBusSumBoard);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.NodeBusNetAlight, resultColumnName.NodeBusTransferAlight, resultColumnName.NodeBusSumAlight);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.NodePRAuto , resultColumnName.NodePRBus, resultColumnName.NodePRRail, resultColumnName.NodePRTransit);
		strQuery.AppendFormat	(_T(" '%s', "), resultColumnName.TerminalTrip);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', "), resultColumnName.TR_AC_Auto, resultColumnName.TR_AC_Taxi, resultColumnName.TR_AC_Bus, resultColumnName.TR_AC_Sub, resultColumnName.TR_AC_Transit);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', '%s' ) "), resultColumnName.TR_EG_Auto, resultColumnName.TR_EG_Taxi, resultColumnName.TR_EG_Bus, resultColumnName.TR_EG_Sub, resultColumnName.TR_EG_Transit, resultColumnName.TR_Total);

		spDBaseConnection->ExecuteUpdate(strQuery);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
	TxLogDebugEndMsg();
}


bool KDBaseAssignment::GetHighwayTypeData( KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	a_setIncludeLinkType.clear();
	a_setIncludeNodeType.clear();

	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key != %d "), USER_NODECODE_GROUP_KEY, SUBSATION_CODE_KEY);

		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while (spResultSet->Next())
		{
			int nCode = spResultSet->GetValueInt(0);
			a_setIncludeNodeType.insert(nCode);
		}

		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key != %d "), USER_LINKCODE_GROUP_KEY, RAIIL_CODE_KEY);

		KResultSetPtr  spResultLinkSet  = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while (spResultLinkSet->Next())
		{
			int nCode = spResultLinkSet->GetValueInt(0);
			a_setIncludeLinkType.insert(nCode);
		}
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
		return false;
	}

	return true;
}


bool KDBaseAssignment::GetAssignmentTypeData( KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	a_setIncludeLinkType.clear();
	a_setIncludeNodeType.clear();

	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d )"), USER_NODECODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_NODE_CODE_KEY );

		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while (spResultSet->Next())
		{
			int nCode = spResultSet->GetValueInt(0);
			a_setIncludeNodeType.insert(nCode);
		}

		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d, %d )"), USER_LINKCODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_CODE_KEY, EXCLUSIVE_HOV_LANE_KEY);

		KResultSetPtr  spResultLinkSet  = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while (spResultLinkSet->Next())
		{
			int nCode = spResultLinkSet->GetValueInt(0);
			a_setIncludeLinkType.insert(nCode);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
		return false;
	}

	return true;
}


bool KDBaseAssignment::GetHighwayAssignmentTypeData( KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	a_setIncludeLinkType.clear();
	a_setIncludeNodeType.clear();

	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d )"), USER_NODECODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_NODE_CODE_KEY );

		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while (spResultSet->Next())
		{
			int nCode = spResultSet->GetValueInt(0);
			a_setIncludeNodeType.insert(nCode);
		}

		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d )"), USER_LINKCODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_CODE_KEY);

		KResultSetPtr  spResultLinkSet  = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while (spResultLinkSet->Next())
		{
			int nCode = spResultLinkSet->GetValueInt(0);
			a_setIncludeLinkType.insert(nCode);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
		return false;
	}

	return true;
}

bool KDBaseAssignment::GetAOBATypeData( KTarget* a_pTarget, std::set<int>& a_setIncludeNodeType, std::set<int>& a_setIncludeLinkType )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	a_setIncludeLinkType.clear();
	a_setIncludeNodeType.clear();

	try
	{
		CString strNodeSQL(_T(""));
		strNodeSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d )"), USER_NODECODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_NODE_CODE_KEY );

		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strNodeSQL);

		while (spResultSet->Next())
		{
			int nCode = spResultSet->GetValueInt(0);
			a_setIncludeNodeType.insert(nCode);
		}

		CString strLinkSQL(_T(""));
		strLinkSQL.Format(_T("Select user_code_key From define_system_code where user_code_group = %d and system_code_key in ( %d , %d, %d ) "), USER_LINKCODE_GROUP_KEY, ZONE_CODE_KEY, HIGHWAY_CODE_KEY, EXCLUSIVE_HOV_LANE_KEY);

		KResultSetPtr  spResultLinkSet  = spDBaseConnection->ExecuteQuery(strLinkSQL);

		while (spResultLinkSet->Next())
		{
			int nCode = spResultLinkSet->GetValueInt(0);
			a_setIncludeLinkType.insert(nCode);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
		return false;
	}

	return true;
}


bool KDBaseAssignment::GetTransitGeneratorMode( KTarget* a_pTarget, std::list<TGenerationPathInfo>& a_lstGenerationPathInfo )
{
	a_lstGenerationPathInfo.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL;
		strSQL.Format(_T("Select name, use_bus, use_subway, use_bussubway, use_exclusive, mode_id From  %s"), TABLE_BUS_PATH_MATRIX);

		KResultSetPtr  spResultLinkSet  = spDBaseConnection->ExecuteQuery(strSQL);
		
		while(spResultLinkSet->Next())
		{
			CString strColumnName = spResultLinkSet->GetValueString(0);
			int nUseBus			  = spResultLinkSet->GetValueInt(1);
			int nUseSubway		  = spResultLinkSet->GetValueInt(2);
			int nUseBusSubway	  = spResultLinkSet->GetValueInt(3);
			int nUseExclusive	  = spResultLinkSet->GetValueInt(4);
			int nModeID			  = spResultLinkSet->GetValueInt(5);

			KMode* pMode		  = nullptr;
			KIOColumn* pColumn	  = nullptr;

			KModeManager* pModeManager = a_pTarget->ModeManager();
			int nModeCount = pModeManager->GetModeCount();
			for(int i = 0; i < nModeCount; ++i)
			{
				KMode* pManagerMode = pModeManager->GetMode(i);
				if (pManagerMode->ModeID() == nModeID)
				{
					pMode = pManagerMode;
				}
			}

			KIOTables*   pIOTables = a_pTarget->Tables();
			KIOTable*		   pTable		 = pIOTables->FindTable(TABLE_MODE_OD);
			const KIOColumns* pIOColumns = pTable->Columns();
			int nColumnCount = pIOColumns->ColumnCount();
			for (int i =0; i <nColumnCount; i++)
			{
				KIOColumn* pFindColumn = pIOColumns->GetColumn(i);
				if (pFindColumn->Name() == strColumnName)
				{
					pColumn = pFindColumn;
				}
			}

			TGenerationPathInfo oPathinfo;
			oPathinfo.nBus			= nUseBus;
			oPathinfo.nSubway		= nUseSubway;
			oPathinfo.nExclusive	= nUseExclusive;
			oPathinfo.pColumn		= pColumn;
			oPathinfo.pMode			= pMode;
			oPathinfo.strModeName	= strColumnName;

			a_lstGenerationPathInfo.push_back(oPathinfo);
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


bool KDBaseAssignment::DeleteOverWriteColumn( KTarget* a_pTarget, TAssignResultInfo& a_AssignResultInfo )
{
	try
	{
		KIOTable* pLinkTable			= a_pTarget->Tables()->FindTable(TABLE_LINK);
		KIOTable* pModeTable			= a_pTarget->Tables()->FindTable(TABLE_MODE_OD);
		KIOTable* pTransitinfo			= a_pTarget->Tables()->FindTable(TABLE_TRANSIT);
		KIOTable* pTransitLine			= a_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);
		KIOTable* pNodeTable			= a_pTarget->Tables()->FindTable(TABLE_NODE);
		KIOTable* pParameterTable		= a_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);	
		KIOTable* pTerminalTalbe		= a_pTarget->Tables()->FindTable(TABLE_TERMINAL_SCHEDULE);
		KIOTable* pTerminalResultTable  = a_pTarget->Tables()->FindTable(TABLE_TERMINAL_RESULT);

		std::vector<CString> vecParse;
		int nCount = 0;

		TxLogDebug(_T("Delete 시작"));

		//link
		CString strAveTime = a_AssignResultInfo.TstrAverageTime;
		if (strAveTime != _T(""))
		{
			pLinkTable->RemoveColumn(strAveTime);
		}

		CString strVC = a_AssignResultInfo.TstrVC;
		if (strVC != _T(""))
		{
			pLinkTable->RemoveColumn(strVC);
		}

		CString strSumVolumn = a_AssignResultInfo.TstrSumVolumn;
		if (strSumVolumn != _T(""))
		{
			pLinkTable->RemoveColumn(strSumVolumn);
		}

		CString strTrip = a_AssignResultInfo.TstrTrip;
		if (strTrip != _T(""))
		{
			pLinkTable->RemoveColumn(strTrip);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo.TstrModeVolume, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo.TstrVDFTime, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);

		}

		CString strAveTimeToll = a_AssignResultInfo.TstrAVETimeToll;
		if (strAveTimeToll != _T(""))
		{
			pLinkTable->RemoveColumn(strAveTimeToll);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo.TstrVDFTimeToll, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);

		}

		CString strBusInit = a_AssignResultInfo.TstrBusInitVolume;
		if (strBusInit != _T(""))
		{
			pLinkTable->RemoveColumn(strBusInit);
		}


		//OD Paramter
		CString strODAverageTime = a_AssignResultInfo.TstrODAveTime;
		if (strODAverageTime != _T(""))
		{
			pParameterTable->RemoveColumn(strODAverageTime);
		} 

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo.TstrODVDFTime, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pParameterTable->RemoveColumn(strColumnName);
		}

		CString strODAveBus = a_AssignResultInfo.TstrODAveBus;
		if (strODAveBus != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveBus);
		}

		CString strODAveSubway = a_AssignResultInfo.TstrODAveSubway;
		if (strODAveSubway != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveSubway);
		}

		CString strODAveBusSubway = a_AssignResultInfo.TstrODAveBusSubway;
		if (strODAveSubway != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveBusSubway);
		}

		CString strODBusDir = a_AssignResultInfo.TstrODBusDirect;
		if (strODBusDir != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusDir);
		}

		CString strODBusOne = a_AssignResultInfo.TstrODBusOneTrans;
		if (strODBusOne != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusOne);
		}

		CString strODBusTwo = a_AssignResultInfo.TstrODBusTwoTrans;
		if (strODBusTwo != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusTwo);
		}

		//Mode OD 
		CString strODMBus = a_AssignResultInfo.TstrODMBus;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMBus);
		}

		CString strODMRail = a_AssignResultInfo.TstrODMRail;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMRail);
		}

		CString strODMBusRail = a_AssignResultInfo.TstrODMBusRail;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMBusRail);
		}
		
		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo.TstrODMInterMode, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pModeTable->RemoveColumn(strColumnName);
		}

		//line info
		CString strLinfoPassenger = a_AssignResultInfo.TstrLInfoPassengernum;
		if (strLinfoPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(strLinfoPassenger);
		}

        if (a_AssignResultInfo.TstrLinfoTotalLength != _T(""))
        {
            pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoTotalLength);
        }

		if (a_AssignResultInfo.TstrLinfoAvgLength != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoAvgLength);
		}

		if (a_AssignResultInfo.TstrLinfoMaxPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoMaxPassenger);
		}

		if (a_AssignResultInfo.LinfoAvePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.LinfoAvePassenger);
		}

		if (a_AssignResultInfo.TstrLinfoMaxSecond != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoMaxSecond);
		}

		if (a_AssignResultInfo.TstrLinfoAvgSecond != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoAvgSecond);
		}

		if (a_AssignResultInfo.TstrLinfoMaxStanding != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoMaxStanding);
		}

		if (a_AssignResultInfo.TstrLinfoAvgStanding != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoAvgStanding);
		}

		if (a_AssignResultInfo.TstrLinfoMaxCongestion != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoMaxCongestion);
		}

		if (a_AssignResultInfo.TstrLinfoAvgCongestion != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoAvgCongestion);
		}

		if (a_AssignResultInfo.TstrLinfoKMPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoKMPassenger);
		}

		if (a_AssignResultInfo.TstrLinfoOncePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoOncePassenger);
		}

		if (a_AssignResultInfo.TstrLinfoKMOncePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo.TstrLinfoKMOncePassenger);
		}

		//line
		CString strLineBorad = a_AssignResultInfo.TstrLineBoradNet;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBorad);
		}

		CString strLineBoradTransfer = a_AssignResultInfo.TstrLineBoradTransfer;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBoradTransfer);
		}

		CString strLineBoradTotal = a_AssignResultInfo.TstrLineBoradTotal;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBoradTotal);
		}

		CString strLineAlight = a_AssignResultInfo.TstrLineAlightNet;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlight);
		}

		CString strLineAlightTransfer = a_AssignResultInfo.TstrLineAlightTransfer;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlightTransfer);
		}

		CString strLineAlightTotal = a_AssignResultInfo.TstrLineAlightTotal;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlightTotal);
		}

		CString strLineSecond = a_AssignResultInfo.TstrLinePassenger;
		if (strLineSecond != _T(""))
		{
			pTransitLine->RemoveColumn(strLineSecond);
		}

		//node
		CString strNodeBusNetBoard = a_AssignResultInfo.TstrNodeBusNetBorad;
		if (strNodeBusNetBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusNetBoard);
		}

		CString strNodeBusTransferBoard = a_AssignResultInfo.TstrNodeBusTransferBoard;
		if (strNodeBusTransferBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusTransferBoard);
		}

		CString strNodeBusSumBoard = a_AssignResultInfo.TstrNodeBusSumBoard;
		if (strNodeBusSumBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusSumBoard);
		}

		CString strNodeBusNetAlight = a_AssignResultInfo.TstrNodeBusNetAlight;
		if (strNodeBusNetAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusNetAlight);
		}

		CString strNodeBusTransferAlight = a_AssignResultInfo.TstrNodeBusTransferAlight;
		if (strNodeBusTransferAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusTransferAlight);
		}

		CString strNodeBusSumAlight = a_AssignResultInfo.TstrNodeBusSumAlight;
		if (strNodeBusSumAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusSumAlight);
		}

		CString strNodePRAuto	= a_AssignResultInfo.TstrNodePRAuto;
		if (strNodePRAuto != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRAuto);
		}

		CString strNodePRBus	= a_AssignResultInfo.TstrNodePRBus;
		if (strNodePRBus != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRBus);
		}

		CString strNodePRRail	= a_AssignResultInfo.TstrNodePRRail;
		if (strNodePRRail != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRRail);
		}

		CString strNodePRTransit	= a_AssignResultInfo.TstrNodePRTransit;
		if (strNodePRTransit != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRTransit);
		}

		//Terminal Schedule

		CString strTerminalTrip = a_AssignResultInfo.TstrTerminalTrip;
		if (strTerminalTrip != _T(""))
		{
			pTerminalTalbe->RemoveColumn(strTerminalTrip);
		}

		//Termianl Result
		CString strTR_AC_Auto = a_AssignResultInfo.TstrTR_AC_Auto;
		if (strTR_AC_Auto != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Auto);
		}

		CString strTR_AC_Taxi = a_AssignResultInfo.TstrTR_AC_Taxi;
		if (strTR_AC_Taxi != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Taxi);
		}

		CString strTR_AC_Bus = a_AssignResultInfo.TstrTR_AC_Bus;
		if (strTR_AC_Bus != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Bus);
		}

		CString strTR_AC_Subway = a_AssignResultInfo.TstrTR_AC_Subway;
		if (strTR_AC_Subway != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Subway);
		}

		CString strTR_AC_Transit = a_AssignResultInfo.TstrTR_AC_Transit;
		if (strTR_AC_Transit != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Transit);
		}


		CString strTR_ER_Auto = a_AssignResultInfo.TstrTR_ER_Auto;
		if (strTR_ER_Auto != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Auto);
		}

		CString strTR_ER_Taxi = a_AssignResultInfo.TstrTR_ER_Taxi;
		if (strTR_ER_Taxi != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Taxi);
		}

		CString strTR_ER_Bus = a_AssignResultInfo.TstrTR_ER_Bus;
		if (strTR_ER_Bus != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Bus);
		}

		CString strTR_ER_Subway = a_AssignResultInfo.TstrTR_ER_Subway;
		if (strTR_ER_Subway != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Subway);
		}

		CString strTR_ER_Transit = a_AssignResultInfo.TstrTR_ER_Transit;
		if (strTR_ER_Transit != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Transit);
		}

		CString strTR_Total = a_AssignResultInfo.TstrTR_Total;
		if (strTR_Total != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_Total);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebug(_T("Delete 완료"));

	return true;
}

bool KDBaseAssignment::DeleteOverWriteColumn( KTarget* a_pTarget, TAssignResultInfo* a_AssignResultInfo )
{
	TxLogDebug(_T("Delete OverWrite Column"));

	try
	{
		KIOTable* pLinkTable			= a_pTarget->Tables()->FindTable(TABLE_LINK);
		KIOTable* pModeTable			= a_pTarget->Tables()->FindTable(TABLE_MODE_OD);
		KIOTable* pTransitinfo			= a_pTarget->Tables()->FindTable(TABLE_TRANSIT);
		KIOTable* pTransitLine			= a_pTarget->Tables()->FindTable(TABLE_TRANSITNODES);
		KIOTable* pNodeTable			= a_pTarget->Tables()->FindTable(TABLE_NODE);
		KIOTable* pParameterTable		= a_pTarget->Tables()->FindTable(TABLE_PARAMETER_OD);	
		KIOTable* pTerminalTalbe		= a_pTarget->Tables()->FindTable(TABLE_TERMINAL_SCHEDULE);
		KIOTable* pTerminalResultTable  = a_pTarget->Tables()->FindTable(TABLE_TERMINAL_RESULT);

		std::vector<CString> vecParse;
		int nCount = 0;

		TxLogDebug(_T("Delete 시작"));

		//link
		CString strAveTime = a_AssignResultInfo->TstrAverageTime;
		if (strAveTime != _T(""))
		{
			pLinkTable->RemoveColumn(strAveTime);
		}

		CString strVC = a_AssignResultInfo->TstrVC;
		if (strVC != _T(""))
		{
			pLinkTable->RemoveColumn(strVC);
		}

		CString strSumVolumn = a_AssignResultInfo->TstrSumVolumn;
		if (strSumVolumn != _T(""))
		{
			pLinkTable->RemoveColumn(strSumVolumn);
		}

		CString strTrip = a_AssignResultInfo->TstrTrip;
		if (strTrip != _T(""))
		{
			pLinkTable->RemoveColumn(strTrip);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo->TstrModeVolume, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo->TstrVDFTime, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);

		}

		CString strAveTimeToll = a_AssignResultInfo->TstrAVETimeToll;
		if (strAveTimeToll != _T(""))
		{
			pLinkTable->RemoveColumn(strAveTimeToll);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo->TstrVDFTimeToll, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pLinkTable->RemoveColumn(strColumnName);

		}

		CString strBusInit = a_AssignResultInfo->TstrBusInitVolume;
		if (strBusInit != _T(""))
		{
			pLinkTable->RemoveColumn(strBusInit);
		}
        
		//OD Paramter
		CString strODAverageTime = a_AssignResultInfo->TstrODAveTime;
		if (strODAverageTime != _T(""))
		{
			pParameterTable->RemoveColumn(strODAverageTime);
		} 

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo->TstrODVDFTime, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pParameterTable->RemoveColumn(strColumnName);
		}

		CString strODAveBus = a_AssignResultInfo->TstrODAveBus;
		if (strODAveBus != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveBus);
		}

		CString strODAveSubway = a_AssignResultInfo->TstrODAveSubway;
		if (strODAveSubway != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveSubway);
		}

		CString strODAveBusSubway = a_AssignResultInfo->TstrODAveBusSubway;
		if (strODAveSubway != _T(""))
		{
			pParameterTable->RemoveColumn(strODAveBusSubway);
		}

		CString strODBusDir = a_AssignResultInfo->TstrODBusDirect;
		if (strODBusDir != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusDir);
		}

		CString strODBusOne = a_AssignResultInfo->TstrODBusOneTrans;
		if (strODBusOne != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusOne);
		}

		CString strODBusTwo = a_AssignResultInfo->TstrODBusTwoTrans;
		if (strODBusTwo != _T(""))
		{
			pParameterTable->RemoveColumn(strODBusTwo);
		}

		//Mode OD 
		CString strODMBus = a_AssignResultInfo->TstrODMBus;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMBus);
		}

		CString strODMRail = a_AssignResultInfo->TstrODMRail;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMRail);
		}

		CString strODMBusRail = a_AssignResultInfo->TstrODMBusRail;
		if (strODBusTwo != _T(""))
		{
			pModeTable->RemoveColumn(strODMBusRail);
		}

		vecParse.clear();
		nCount = ParseStringArr(a_AssignResultInfo->TstrODMInterMode, vecParse);
		for (int i = 0; i < nCount; i++)
		{
			CString strColumnName = vecParse[i];
			pModeTable->RemoveColumn(strColumnName);
		}

		//line info
		CString strLinfoPassenger = a_AssignResultInfo->TstrLInfoPassengernum;
		if (strLinfoPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(strLinfoPassenger);
		}

        if (a_AssignResultInfo->TstrLinfoTotalLength != _T(""))
        {
            pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoTotalLength);
        }

		if (a_AssignResultInfo->TstrLinfoAvgLength != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoAvgLength);
		}

		if (a_AssignResultInfo->TstrLinfoMaxPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoMaxPassenger);
		}

		if (a_AssignResultInfo->LinfoAvePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->LinfoAvePassenger);
		}

		if (a_AssignResultInfo->TstrLinfoMaxSecond != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoMaxSecond);
		}

		if (a_AssignResultInfo->TstrLinfoAvgSecond != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoAvgSecond);
		}

		if (a_AssignResultInfo->TstrLinfoMaxStanding != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoMaxStanding);
		}

		if (a_AssignResultInfo->TstrLinfoAvgStanding != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoAvgStanding);
		}

		if (a_AssignResultInfo->TstrLinfoMaxCongestion != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoMaxCongestion);
		}

		if (a_AssignResultInfo->TstrLinfoAvgCongestion != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoAvgCongestion);
		}

		if (a_AssignResultInfo->TstrLinfoKMPassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoKMPassenger);
		}

		if (a_AssignResultInfo->TstrLinfoOncePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoOncePassenger);
		}

		if (a_AssignResultInfo->TstrLinfoKMOncePassenger != _T(""))
		{
			pTransitinfo->RemoveColumn(a_AssignResultInfo->TstrLinfoKMOncePassenger);
		}

		//line
		CString strLineBorad = a_AssignResultInfo->TstrLineBoradNet;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBorad);
		}

		CString strLineBoradTransfer = a_AssignResultInfo->TstrLineBoradTransfer;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBoradTransfer);
		}

		CString strLineBoradTotal = a_AssignResultInfo->TstrLineBoradTotal;
		if (strLineBorad != _T(""))
		{
			pTransitLine->RemoveColumn(strLineBoradTotal);
		}

		CString strLineAlight = a_AssignResultInfo->TstrLineAlightNet;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlight);
		}

		CString strLineAlightTransfer = a_AssignResultInfo->TstrLineAlightTransfer;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlightTransfer);
		}

		CString strLineAlightTotal = a_AssignResultInfo->TstrLineAlightTotal;
		if (strLineAlight != _T(""))
		{
			pTransitLine->RemoveColumn(strLineAlightTotal);
		}

		CString strLineSecond = a_AssignResultInfo->TstrLinePassenger;
		if (strLineSecond != _T(""))
		{
			pTransitLine->RemoveColumn(strLineSecond);
		}

		//node
		CString strNodeBusNetBoard = a_AssignResultInfo->TstrNodeBusNetBorad;
		if (strNodeBusNetBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusNetBoard);
		}

		CString strNodeBusTransferBoard = a_AssignResultInfo->TstrNodeBusTransferBoard;
		if (strNodeBusTransferBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusTransferBoard);
		}

		CString strNodeBusSumBoard = a_AssignResultInfo->TstrNodeBusSumBoard;
		if (strNodeBusSumBoard != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusSumBoard);
		}

		CString strNodeBusNetAlight = a_AssignResultInfo->TstrNodeBusNetAlight;
		if (strNodeBusNetAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusNetAlight);
		}

		CString strNodeBusTransferAlight = a_AssignResultInfo->TstrNodeBusTransferAlight;
		if (strNodeBusTransferAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusTransferAlight);
		}

		CString strNodeBusSumAlight = a_AssignResultInfo->TstrNodeBusSumAlight;
		if (strNodeBusSumAlight != _T(""))
		{
			pNodeTable->RemoveColumn(strNodeBusSumAlight);
		}

		CString strNodePRAuto	= a_AssignResultInfo->TstrNodePRAuto;
		if (strNodePRAuto != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRAuto);
		}

		CString strNodePRBus	= a_AssignResultInfo->TstrNodePRBus;
		if (strNodePRBus != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRBus);
		}

		CString strNodePRRail	= a_AssignResultInfo->TstrNodePRRail;
		if (strNodePRRail != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRRail);
		}

		CString strNodePRTransit	= a_AssignResultInfo->TstrNodePRTransit;
		if (strNodePRTransit != _T(""))
		{
			pNodeTable->RemoveColumn(strNodePRTransit);
		}

		//Terminal Schedule
		CString strTerminalTrip = a_AssignResultInfo->TstrTerminalTrip;
		if (strTerminalTrip != _T(""))
		{
			pTerminalTalbe->RemoveColumn(strTerminalTrip);
		}

		//Termianl Result
		CString strTR_AC_Auto = a_AssignResultInfo->TstrTR_AC_Auto;
		if (strTR_AC_Auto != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Auto);
		}

		CString strTR_AC_Taxi = a_AssignResultInfo->TstrTR_AC_Taxi;
		if (strTR_AC_Taxi != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Taxi);
		}

		CString strTR_AC_Bus = a_AssignResultInfo->TstrTR_AC_Bus;
		if (strTR_AC_Bus != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Bus);
		}

		CString strTR_AC_Subway = a_AssignResultInfo->TstrTR_AC_Subway;
		if (strTR_AC_Subway != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Subway);
		}

		CString strTR_AC_Transit = a_AssignResultInfo->TstrTR_AC_Transit;
		if (strTR_AC_Transit != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_AC_Transit);
		}


		CString strTR_ER_Auto = a_AssignResultInfo->TstrTR_ER_Auto;
		if (strTR_ER_Auto != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Auto);
		}

		CString strTR_ER_Taxi = a_AssignResultInfo->TstrTR_ER_Taxi;
		if (strTR_ER_Taxi != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Taxi);
		}

		CString strTR_ER_Bus = a_AssignResultInfo->TstrTR_ER_Bus;
		if (strTR_ER_Bus != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Bus);
		}

		CString strTR_ER_Subway = a_AssignResultInfo->TstrTR_ER_Subway;
		if (strTR_ER_Subway != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Subway);
		}

		CString strTR_ER_Transit = a_AssignResultInfo->TstrTR_ER_Transit;
		if (strTR_ER_Transit != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_ER_Transit);
		}

		CString strTR_Total = a_AssignResultInfo->TstrTR_Total;
		if (strTR_Total != _T(""))
		{
			pTerminalResultTable->RemoveColumn(strTR_Total);
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	TxLogDebug(_T("Delete 완료"));

	return true;
}



void KDBaseAssignment::DeleteUpdateInterModalRunSeq( KDBaseConPtr spDBaseConnection, int nRunSeq, TAssignResultColumnName& resultColumnName, CString strStartDate, CString strEndDate, CString strDescription, int nModeType )
{
	//Mode Volum;
	TxLogDebugStartMsg();
	try
	{
		CString strModeVolumn;
		std::vector<CString> vecModeVolum = resultColumnName.ModeVolumn;
		int nCount = vecModeVolum.size();
		if (nCount == 1)
		{
			strModeVolumn = vecModeVolum[0];
		}
		else if (nCount > 1)
		{
			strModeVolumn = vecModeVolum[0];
			for (int i = 1; i <nCount; i++)
			{
				strModeVolumn = strModeVolumn + _T(", ");
				CString strVolume = vecModeVolum[i];
				strModeVolumn = strModeVolumn + strVolume;
			}

		}
		else if (nCount = 0)
		{
			strModeVolumn = _T("");
		}

		CString strInterModeVolumn(_T(""));
		std::vector<CString> vecIntermode = resultColumnName.InterMode;
		if (vecIntermode.size() > 1)
		{
			for (size_t i = 0; i < vecIntermode.size(); i++)
			{
				if (i == 0)
				{
					strInterModeVolumn = vecIntermode[i];
				}
				else
				{
					strInterModeVolumn.AppendFormat(_T(", %s"), vecIntermode[i]);
				}

			}
		}


		CString strQuery;

		strQuery.Append			(_T(" Insert Or Replace Into assign_model"));
		strQuery.Append			(_T(" (run_seq, start_date, end_date, type, Description, "));
		strQuery.Append			(_T(" average_time, vc, sum_volume, trip, mode_volume, bus_Initial_volume, "));
		strQuery.Append			(_T(" od_average_time, od_average_bus, od_average_subway, od_average_bussubway, ")); 
		strQuery.Append			(_T(" od_bus_direct, od_bus_one_transfer,od_bus_two_transfer, "));
		strQuery.Append			(_T(" od_bus, od_rail, od_busrail, od_inter_mode, "));
		strQuery.Append			(_T(" linfo_num_passengers, "));
		strQuery.Append			(_T(" linfo_total_length, linfo_avg_length, linfo_max_passengers, linfo_avg_passengers, "));
		strQuery.Append			(_T(" linfo_max_second, linfo_avg_second, linfo_max_standing, linfo_avg_standing, "));
		strQuery.Append			(_T(" linfo_max_congestion, linfo_avg_congestion, linfo_km_passengers, linfo_once_passengers, linfo_km_once_passengers, "));
		strQuery.Append			(_T(" line_board_net, line_board_Transfer, line_board_Total, "));
		strQuery.Append			(_T(" line_Alight_net, line_Alight_Transfer, line_Alight_Total, "));
		strQuery.Append			(_T(" line_passenger, "));
		strQuery.Append			(_T(" node_bus_net_board, node_bus_transfer_board, node_bus_sum_board, "));
		strQuery.Append			(_T(" node_bus_net_alight, node_bus_transfer_alight, node_bus_sum_alight, "));
		strQuery.Append			(_T(" pr_auto, pr_bus, pr_rail, pr_transit, "));
		strQuery.Append			(_T(" terminal_trip,  "));
		strQuery.Append			(_T(" tr_ac_auto, tr_ac_taxi, tr_ac_bus, tr_ac_rail, tr_ac_transit, "));
		strQuery.Append			(_T(" tr_er_auto, tr_er_taxi, tr_er_bus, tr_er_rail, tr_er_transit, tr_total )"));

		strQuery.Append			(_T(" Values ( "));

		strQuery.AppendFormat	(_T(" %d, '%s', '%s', %d, '%s', "), nRunSeq, strStartDate, strEndDate, nModeType, strDescription);

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', '%s', "), resultColumnName.AverageTime, resultColumnName.VC, 
			resultColumnName.SumVolume, resultColumnName.Trip, strModeVolumn, resultColumnName.BusInitialVolumn);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.ODAvgTime, resultColumnName.ODAvgBusTime,
			resultColumnName.ODAvgSubwayTime, resultColumnName.ODAVgBusSubwayTime);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.ODBusDirect, resultColumnName.ODBusOneTransfer, resultColumnName.ODBustwoTransfer);

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.ODMBus, resultColumnName.ODMRail, resultColumnName.ODMBusRail, strInterModeVolumn);

		strQuery.AppendFormat	(_T(" '%s', "), resultColumnName.LinfoPassenger);

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.LinfoTotalLength, resultColumnName.LinfoAvgLength, resultColumnName.LinfoMaxPassenger, resultColumnName.LinfoAvePassenger );
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.LinfoMaxSecond, resultColumnName.LinfoAvgSecond, resultColumnName.LinfoMaxStanding, resultColumnName.LinfoAvgStanding );
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', "), resultColumnName.LinfoMaxCongestion, resultColumnName.LinfoAvgCongestion, resultColumnName.LinfoKMPassenger, resultColumnName.LinfoOncePassenger, resultColumnName.LinfoKMOncePassenger );

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.LineBoardNet, resultColumnName.LineBoardTransfer, resultColumnName.LineBoardTotal);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.LineAlightNet, resultColumnName.LineAlightTransfer, resultColumnName.LineAlightTotal);

		strQuery.AppendFormat	(_T(" '%s',"), resultColumnName.LinePassenger);

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.NodeBusNetBorad, resultColumnName.NodeBusTransferBoard, resultColumnName.NodeBusSumBoard);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', "), resultColumnName.NodeBusNetAlight, resultColumnName.NodeBusTransferAlight, resultColumnName.NodeBusSumAlight);

		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', "), resultColumnName.NodePRAuto , resultColumnName.NodePRBus, resultColumnName.NodePRRail, resultColumnName.NodePRTransit);
		strQuery.AppendFormat	(_T(" '%s', "), resultColumnName.TerminalTrip);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s', "), resultColumnName.TR_AC_Auto, resultColumnName.TR_AC_Taxi, resultColumnName.TR_AC_Bus, resultColumnName.TR_AC_Sub, resultColumnName.TR_AC_Transit);
		strQuery.AppendFormat	(_T(" '%s', '%s', '%s', '%s', '%s' ) "), resultColumnName.TR_EG_Auto, resultColumnName.TR_EG_Taxi, resultColumnName.TR_EG_Bus, resultColumnName.TR_EG_Sub, resultColumnName.TR_EG_Transit, resultColumnName.TR_Total);

		TxLogDebug((LPCTSTR)strQuery);

		spDBaseConnection->ExecuteUpdate(strQuery);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KDBaseAssignment::GetInterModalResultInfo( KTarget* a_pTarget, TAssignResultInfo& a_oInterResultInfo )
{
	CString strSQL;
	strSQL =	_T("select run_seq, start_date, end_date, type, description, average_time, ");
	strSQL +=	_T(" vc, sum_volume, trip, mode_volume, vdf_time, ave_time_toll, vdf_toll_time, ");
	strSQL +=	_T(" bus_initial_volume, od_average_time, od_vdf_time, od_average_bus, od_average_subway, od_average_bussubway, ");
	strSQL +=	_T(" od_bus_direct, od_bus_one_transfer, od_bus_two_transfer, linfo_num_passengers, ");
	strSQL +=   _T(" linfo_total_length, linfo_avg_length, linfo_max_passengers, linfo_avg_passengers, ");
	strSQL +=   _T(" linfo_max_second, linfo_avg_second, linfo_max_standing, linfo_avg_standing, ");
	strSQL +=   _T(" linfo_max_congestion, linfo_avg_congestion, linfo_km_passengers, linfo_once_passengers, linfo_km_once_passengers, ");
	strSQL +=	_T(" od_bus, od_rail, od_busrail, od_inter_mode, ");
	strSQL +=   _T(" line_board_net, line_board_transfer, line_board_total, ");
	strSQL +=   _T(" line_alight_net, line_alight_transfer, line_alight_total, line_passenger, ");
	strSQL +=   _T(" node_bus_net_board, node_bus_transfer_board, node_bus_sum_board, ");
	strSQL +=   _T(" node_bus_net_alight, node_bus_transfer_alight, node_bus_sum_alight, ");
	strSQL +=   _T(" pr_auto, pr_bus, pr_rail, pr_transit, ");
	strSQL +=   _T(" terminal_trip, ");
	strSQL +=   _T(" tr_ac_auto, tr_ac_taxi, tr_ac_bus, tr_ac_rail, tr_ac_transit, ");
	strSQL +=   _T(" tr_er_auto, tr_er_taxi, tr_er_bus, tr_er_rail, tr_er_transit, tr_total ");
	strSQL +=   _T("From assign_model where type = 6");
    	
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			a_oInterResultInfo.TnRunSeq				 = spResultSet->GetValueInt(0);
			a_oInterResultInfo.TstrStartDate		 = spResultSet->GetValueString(1);
			a_oInterResultInfo.TstrEndDate			 = spResultSet->GetValueString(2);
			a_oInterResultInfo.TnType				 = spResultSet->GetValueInt(3);
			a_oInterResultInfo.TstrDescription		 = spResultSet->GetValueString(4);
			a_oInterResultInfo.TstrAverageTime		 = spResultSet->GetValueString(5);
			a_oInterResultInfo.TstrVC				 = spResultSet->GetValueString(6);
			a_oInterResultInfo.TstrSumVolumn		 = spResultSet->GetValueString(7);
			a_oInterResultInfo.TstrTrip				 = spResultSet->GetValueString(8);
			a_oInterResultInfo.TstrModeVolume		 = spResultSet->GetValueString(9);
			a_oInterResultInfo.TstrVDFTime			 = spResultSet->GetValueString(10);
			a_oInterResultInfo.TstrAVETimeToll		 = spResultSet->GetValueString(11);
			a_oInterResultInfo.TstrVDFTimeToll		 = spResultSet->GetValueString(12);
			a_oInterResultInfo.TstrBusInitVolume	 = spResultSet->GetValueString(13);
			a_oInterResultInfo.TstrODAveTime		 = spResultSet->GetValueString(14);
			a_oInterResultInfo.TstrODVDFTime		 = spResultSet->GetValueString(15);
			a_oInterResultInfo.TstrODAveBus			 = spResultSet->GetValueString(16);
			a_oInterResultInfo.TstrODAveSubway		 = spResultSet->GetValueString(17);
			a_oInterResultInfo.TstrODAveBusSubway	 = spResultSet->GetValueString(18);
			a_oInterResultInfo.TstrODBusDirect		 = spResultSet->GetValueString(19);
			a_oInterResultInfo.TstrODBusOneTrans	 = spResultSet->GetValueString(20);
			a_oInterResultInfo.TstrODBusTwoTrans	 = spResultSet->GetValueString(21);
			a_oInterResultInfo.TstrLInfoPassengernum = spResultSet->GetValueString(22);
			a_oInterResultInfo.TstrLinfoTotalLength  = spResultSet->GetValueString(23);
			a_oInterResultInfo.TstrLinfoAvgLength  = spResultSet->GetValueString(24);
			a_oInterResultInfo.TstrLinfoMaxPassenger  = spResultSet->GetValueString(25);
			a_oInterResultInfo.LinfoAvePassenger  = spResultSet->GetValueString(26);
			a_oInterResultInfo.TstrLinfoMaxSecond  = spResultSet->GetValueString(27);
			a_oInterResultInfo.TstrLinfoAvgSecond  = spResultSet->GetValueString(28);
			a_oInterResultInfo.TstrLinfoMaxStanding  = spResultSet->GetValueString(29);
			a_oInterResultInfo.TstrLinfoAvgStanding  = spResultSet->GetValueString(30);
			a_oInterResultInfo.TstrLinfoMaxCongestion  = spResultSet->GetValueString(31);
			a_oInterResultInfo.TstrLinfoAvgCongestion  = spResultSet->GetValueString(32);
			a_oInterResultInfo.TstrLinfoKMPassenger  = spResultSet->GetValueString(33);
			a_oInterResultInfo.TstrLinfoOncePassenger  = spResultSet->GetValueString(34);
			a_oInterResultInfo.TstrLinfoKMOncePassenger  = spResultSet->GetValueString(35);
			a_oInterResultInfo.TstrODMBus			 = spResultSet->GetValueString(36);
			a_oInterResultInfo.TstrODMRail			 = spResultSet->GetValueString(37);
			a_oInterResultInfo.TstrODMBusRail		 = spResultSet->GetValueString(38);
			a_oInterResultInfo.TstrODMInterMode		 = spResultSet->GetValueString(39);
			a_oInterResultInfo.TstrLineBoradNet		 = spResultSet->GetValueString(40);
			a_oInterResultInfo.TstrLineBoradTransfer = spResultSet->GetValueString(41);
			a_oInterResultInfo.TstrLineBoradTotal	 = spResultSet->GetValueString(42);
			a_oInterResultInfo.TstrLineAlightNet	 = spResultSet->GetValueString(43);
			a_oInterResultInfo.TstrLineAlightTransfer= spResultSet->GetValueString(44);
			a_oInterResultInfo.TstrLineAlightTotal	 = spResultSet->GetValueString(45);
			a_oInterResultInfo.TstrLinePassenger	 = spResultSet->GetValueString(46);
			a_oInterResultInfo.TstrNodeBusNetBorad	 = spResultSet->GetValueString(47);
			a_oInterResultInfo.TstrNodeBusTransferBoard = spResultSet->GetValueString(48);
			a_oInterResultInfo.TstrNodeBusSumBoard		= spResultSet->GetValueString(49);
			a_oInterResultInfo.TstrNodeBusNetAlight		= spResultSet->GetValueString(50);
			a_oInterResultInfo.TstrNodeBusTransferAlight= spResultSet->GetValueString(51);
			a_oInterResultInfo.TstrNodeBusSumAlight		= spResultSet->GetValueString(52);
			a_oInterResultInfo.TstrNodePRAuto			= spResultSet->GetValueString(53);
			a_oInterResultInfo.TstrNodePRBus			= spResultSet->GetValueString(54);
			a_oInterResultInfo.TstrNodePRRail			= spResultSet->GetValueString(55);
			a_oInterResultInfo.TstrNodePRTransit		= spResultSet->GetValueString(56);
			a_oInterResultInfo.TstrTerminalTrip			= spResultSet->GetValueString(57);
			a_oInterResultInfo.TstrTR_AC_Auto			= spResultSet->GetValueString(58);
			a_oInterResultInfo.TstrTR_AC_Taxi			= spResultSet->GetValueString(59);
			a_oInterResultInfo.TstrTR_AC_Bus			= spResultSet->GetValueString(60);
			a_oInterResultInfo.TstrTR_AC_Subway			= spResultSet->GetValueString(61);
			a_oInterResultInfo.TstrTR_AC_Transit		= spResultSet->GetValueString(62);
			a_oInterResultInfo.TstrTR_ER_Auto			= spResultSet->GetValueString(63);
			a_oInterResultInfo.TstrTR_ER_Taxi			= spResultSet->GetValueString(64);
			a_oInterResultInfo.TstrTR_ER_Bus			= spResultSet->GetValueString(65);
			a_oInterResultInfo.TstrTR_ER_Subway			= spResultSet->GetValueString(66);
			a_oInterResultInfo.TstrTR_ER_Transit		= spResultSet->GetValueString(67);
			a_oInterResultInfo.TstrTR_Total				= spResultSet->GetValueString(68);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}

void KDBaseAssignment::GetAssignPathInputInfo(KTarget* a_pTarget, std::vector<TAssignPathInput>& ar_vecAssignPathInputInfo )
{
	TxLogDebugStartMsg();

	ar_vecAssignPathInputInfo.clear();

	CString strQuery = _T("Select mode_id, occupancy, pcu, network_type, preload, column_name, inputcolumnname From assign_path_input");

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			TAssignPathInput oAssignPathInput = {};

			oAssignPathInput.nModeID              = spResultSet->GetValueInt   (0);
			oAssignPathInput.dOccupancy           = spResultSet->GetValueDouble(1);
			oAssignPathInput.dPCU                 = spResultSet->GetValueDouble(2);
			oAssignPathInput.nNetworkType         = spResultSet->GetValueInt   (3);
			oAssignPathInput.nPreload             = spResultSet->GetValueInt   (4);
			oAssignPathInput.strOutputColumnName  = spResultSet->GetValueString(5);
			oAssignPathInput.strInputColumnName   = spResultSet->GetValueString(6);

			ar_vecAssignPathInputInfo.push_back(oAssignPathInput);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		ar_vecAssignPathInputInfo.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		ar_vecAssignPathInputInfo.clear();
	}

	TxLogDebugEndMsg();
}


bool KDBaseAssignment::InterSectionTypeDB2File( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T(" Select node_id, intersection_type, cross_type From %s order by node_id"), TABLE_INTERSECTION);

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			TInterSectionType oInfo;
			oInfo.nxNodeID = spResult->GetValueInt64(0);
			oInfo.nType	   = spResult->GetValueInt(1);
			oInfo.nTopoloy = spResult->GetValueInt(2);
			ofs.write(reinterpret_cast<char*>( &oInfo   ), sizeof(TInterSectionType));  
		}

		ofs.close();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ofs.close();
	}
	catch(...)
	{
		TxLogDebugException();
		ofs.close();
	}

	return true;
}

bool KDBaseAssignment::InterSectionMovementDB2File( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	ofstream ofs(strFileName, ios::binary);
	if (!ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		std::vector<TInterSectionMovement> vecIntersectionInfo;
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, intersection_type, first_bound_node_id, second_bound_node_id, third_bound_node_id, fourth_bound_node_id, cross_type From %s Order by node_id "), TABLE_INTERSECTION);
		
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			Integer nxNodeID			= spResult->GetValueInt64(0);
			int		nType				= spResult->GetValueInt(1);
			Integer nxfirstBoundnode	= spResult->GetValueInt64(2);
			Integer nxsecondBoundnode	= spResult->GetValueInt64(3);
			Integer nxthirdBoundnode	= spResult->GetValueInt64(4);
			Integer nxfourthBoundnode	= spResult->GetValueInt64(5);
			int		nCrossType			= spResult->GetValueInt(6);

			std::vector<Integer> vecBoundRotation;
			vecBoundRotation.push_back(nxfirstBoundnode);
			vecBoundRotation.push_back(nxsecondBoundnode);
			vecBoundRotation.push_back(nxthirdBoundnode);
			vecBoundRotation.push_back(nxfirstBoundnode);
			vecBoundRotation.push_back(nxsecondBoundnode);

			int nIndex = 0;
			if (nxfourthBoundnode == nxfirstBoundnode)
				nIndex = 0;
			if (nxfourthBoundnode == nxsecondBoundnode)
				nIndex = 1;
			if (nxfourthBoundnode == nxthirdBoundnode)
				nIndex = 2;

			Integer nxMain		= vecBoundRotation[nIndex];
			Integer nxNext		= vecBoundRotation[nIndex+ 1];
			Integer nxBefore	= vecBoundRotation[nIndex + 2];

			if ( 0 == nCrossType )
			{
				TInterSectionMovement oInfo1 = {};
				oInfo1.nxFromNode = nxfirstBoundnode;
				oInfo1.nxAtNode	  = nxNodeID;
				oInfo1.nxToNode	  = nxsecondBoundnode;
				oInfo1.nTurnIndex = 1;
				vecIntersectionInfo.push_back(oInfo1);

				TInterSectionMovement oInfo2 = {};
				oInfo2.nxFromNode = nxfirstBoundnode;
				oInfo2.nxAtNode	  = nxNodeID;
				oInfo2.nxToNode	  = nxthirdBoundnode;
				oInfo2.nTurnIndex = 2;
				vecIntersectionInfo.push_back(oInfo2);

				TInterSectionMovement oInfo3 = {};
				oInfo3.nxFromNode = nxfirstBoundnode;
				oInfo3.nxAtNode	  = nxNodeID;
				oInfo3.nxToNode	  = nxfourthBoundnode;
				oInfo3.nTurnIndex = 3;
				vecIntersectionInfo.push_back(oInfo3);

				TInterSectionMovement oInfo4 = {};
				oInfo4.nxFromNode = nxthirdBoundnode;
				oInfo4.nxAtNode	  = nxNodeID;
				oInfo4.nxToNode	  = nxfourthBoundnode;
				oInfo4.nTurnIndex = 4;
				vecIntersectionInfo.push_back(oInfo4);

				TInterSectionMovement oInfo5 = {};
				oInfo5.nxFromNode = nxthirdBoundnode;
				oInfo5.nxAtNode	  = nxNodeID;
				oInfo5.nxToNode	  = nxfirstBoundnode;
				oInfo5.nTurnIndex = 5;
				vecIntersectionInfo.push_back(oInfo5);

				TInterSectionMovement oInfo6 = {};
				oInfo6.nxFromNode = nxthirdBoundnode;
				oInfo6.nxAtNode	  = nxNodeID;
				oInfo6.nxToNode	  = nxsecondBoundnode;
				oInfo6.nTurnIndex = 6;
				vecIntersectionInfo.push_back(oInfo6);

				TInterSectionMovement oInfo7 = {};
				oInfo7.nxFromNode = nxfourthBoundnode;
				oInfo7.nxAtNode	  = nxNodeID;
				oInfo7.nxToNode	  = nxfirstBoundnode;
				oInfo7.nTurnIndex = 7;
				vecIntersectionInfo.push_back(oInfo7);

				TInterSectionMovement oInfo8 = {};
				oInfo8.nxFromNode = nxfourthBoundnode;
				oInfo8.nxAtNode	  = nxNodeID;
				oInfo8.nxToNode	  = nxsecondBoundnode;
				oInfo8.nTurnIndex = 8;
				vecIntersectionInfo.push_back(oInfo8);

				TInterSectionMovement oInfo9 = {};
				oInfo9.nxFromNode = nxfourthBoundnode;
				oInfo9.nxAtNode	  = nxNodeID;
				oInfo9.nxToNode	  = nxthirdBoundnode;
				oInfo9.nTurnIndex = 9;
				vecIntersectionInfo.push_back(oInfo9);

				TInterSectionMovement oInfo10 = {};
				oInfo10.nxFromNode	  = nxsecondBoundnode;
				oInfo10.nxAtNode	  = nxNodeID;
				oInfo10.nxToNode	  = nxthirdBoundnode;
				oInfo10.nTurnIndex	  = 10;
				vecIntersectionInfo.push_back(oInfo10);

				TInterSectionMovement oInfo11 = {};
				oInfo11.nxFromNode	  = nxsecondBoundnode;
				oInfo11.nxAtNode	  = nxNodeID;
				oInfo11.nxToNode	  = nxfourthBoundnode;
				oInfo11.nTurnIndex	  = 11;
				vecIntersectionInfo.push_back(oInfo11);

				TInterSectionMovement oInfo12 = {};
				oInfo12.nxFromNode	  = nxsecondBoundnode;
				oInfo12.nxAtNode	  = nxNodeID;
				oInfo12.nxToNode	  = nxfirstBoundnode;
				oInfo12.nTurnIndex	  = 12;
				vecIntersectionInfo.push_back(oInfo12);
			}
			else if (1 == nCrossType) //ㅏ
			{
				TInterSectionMovement oInfo4 = {};
				oInfo4.nxFromNode = nxMain;
				oInfo4.nxAtNode	  = nxNodeID;
				oInfo4.nxToNode	  = nxNext;
				oInfo4.nTurnIndex = 4;
				vecIntersectionInfo.push_back(oInfo4);

				TInterSectionMovement oInfo6 = {};
				oInfo6.nxFromNode = nxMain;
				oInfo6.nxAtNode	  = nxNodeID;
				oInfo6.nxToNode	  = nxBefore;
				oInfo6.nTurnIndex = 6;
				vecIntersectionInfo.push_back(oInfo6);

				TInterSectionMovement oInfo8 = {};
				oInfo8.nxFromNode = nxNext;
				oInfo8.nxAtNode	  = nxNodeID;
				oInfo8.nxToNode	  = nxBefore;
				oInfo8.nTurnIndex = 8;
				vecIntersectionInfo.push_back(oInfo8);

				TInterSectionMovement oInfo9 = {};
				oInfo9.nxFromNode = nxNext;
				oInfo9.nxAtNode	  = nxNodeID;
				oInfo9.nxToNode	  = nxMain;
				oInfo9.nTurnIndex = 9;
				vecIntersectionInfo.push_back(oInfo9);

				TInterSectionMovement oInfo10 = {};
				oInfo10.nxFromNode	  = nxBefore;
				oInfo10.nxAtNode	  = nxNodeID;
				oInfo10.nxToNode	  = nxMain;
				oInfo10.nTurnIndex	  = 10;
				vecIntersectionInfo.push_back(oInfo10);

				TInterSectionMovement oInfo11 = {};
				oInfo11.nxFromNode	  = nxBefore;
				oInfo11.nxAtNode	  = nxNodeID;
				oInfo11.nxToNode	  = nxNext;
				oInfo11.nTurnIndex	  = 11;
				vecIntersectionInfo.push_back(oInfo11);

			}
			else if (2 == nCrossType) //ㅓ
			{
				TInterSectionMovement oInfo1 = {};
				oInfo1.nxFromNode = nxMain;
				oInfo1.nxAtNode	  = nxNodeID;
				oInfo1.nxToNode	  = nxNext;
				oInfo1.nTurnIndex = 1;
				vecIntersectionInfo.push_back(oInfo1);

				TInterSectionMovement oInfo3 = {};
				oInfo3.nxFromNode = nxMain;
				oInfo3.nxAtNode	  = nxNodeID;
				oInfo3.nxToNode	  = nxBefore;
				oInfo3.nTurnIndex = 3;
				vecIntersectionInfo.push_back(oInfo3);

				TInterSectionMovement oInfo11 = {};
				oInfo11.nxFromNode	  = nxNext;
				oInfo11.nxAtNode	  = nxNodeID;
				oInfo11.nxToNode	  = nxBefore;
				oInfo11.nTurnIndex	  = 11;
				vecIntersectionInfo.push_back(oInfo11);

				TInterSectionMovement oInfo12 = {};
				oInfo12.nxFromNode		= nxNext;
				oInfo12.nxAtNode		= nxNodeID;
				oInfo12.nxToNode		= nxMain;
				oInfo12.nTurnIndex		= 12;
				vecIntersectionInfo.push_back(oInfo12);

				TInterSectionMovement oInfo7 = {};
				oInfo7.nxFromNode		= nxBefore;
				oInfo7.nxAtNode			= nxNodeID;
				oInfo7.nxToNode			= nxMain;
				oInfo7.nTurnIndex		= 7;
				vecIntersectionInfo.push_back(oInfo7);

				TInterSectionMovement oInfo8 = {};
				oInfo8.nxFromNode		= nxBefore;
				oInfo8.nxAtNode			= nxNodeID;
				oInfo8.nxToNode			= nxNext;
				oInfo8.nTurnIndex		= 8;
				vecIntersectionInfo.push_back(oInfo8);
			}
			else if (3 == nCrossType) //ㅗ
			{
				TInterSectionMovement oInfo10 = {};
				oInfo10.nxFromNode    = nxMain;
				oInfo10.nxAtNode	  = nxNodeID;
				oInfo10.nxToNode	  = nxNext;
				oInfo10.nTurnIndex	  = 10;
				vecIntersectionInfo.push_back(oInfo10);

				TInterSectionMovement oInfo12 = {};
				oInfo12.nxFromNode		= nxMain;
				oInfo12.nxAtNode		= nxNodeID;
				oInfo12.nxToNode		= nxBefore;
				oInfo12.nTurnIndex		= 12;
				vecIntersectionInfo.push_back(oInfo12);

				TInterSectionMovement oInfo5 = {};
				oInfo5.nxFromNode = nxNext;
				oInfo5.nxAtNode	  = nxNodeID;
				oInfo5.nxToNode	  = nxBefore;
				oInfo5.nTurnIndex = 5;
				vecIntersectionInfo.push_back(oInfo5);

				TInterSectionMovement oInfo6 = {};
				oInfo6.nxFromNode = nxNext;
				oInfo6.nxAtNode	  = nxNodeID;
				oInfo6.nxToNode	  = nxMain;
				oInfo6.nTurnIndex = 6;
				vecIntersectionInfo.push_back(oInfo6);

				TInterSectionMovement oInfo1 = {};
				oInfo1.nxFromNode		= nxBefore;
				oInfo1.nxAtNode			= nxNodeID;
				oInfo1.nxToNode			= nxMain;
				oInfo1.nTurnIndex		= 1;
				vecIntersectionInfo.push_back(oInfo10);

				TInterSectionMovement oInfo2 = {};
				oInfo2.nxFromNode		= nxBefore;
				oInfo2.nxAtNode			= nxNodeID;
				oInfo2.nxToNode			= nxNext;
				oInfo2.nTurnIndex		= 2;
				vecIntersectionInfo.push_back(oInfo2);
			}
			else if (4 == nCrossType) //ㅜ
			{
				TInterSectionMovement oInfo7 = {};
				oInfo7.nxFromNode = nxMain;
				oInfo7.nxAtNode	  = nxNodeID;
				oInfo7.nxToNode	  = nxNext;
				oInfo7.nTurnIndex = 7;
				vecIntersectionInfo.push_back(oInfo7);

				TInterSectionMovement oInfo9 = {};
				oInfo9.nxFromNode = nxMain;
				oInfo9.nxAtNode	  = nxNodeID;
				oInfo9.nxToNode	  = nxBefore;
				oInfo9.nTurnIndex = 9;
				vecIntersectionInfo.push_back(oInfo9);

				TInterSectionMovement oInfo2 = {};
				oInfo2.nxFromNode = nxNext;
				oInfo2.nxAtNode	  = nxNodeID;
				oInfo2.nxToNode	  = nxBefore;
				oInfo2.nTurnIndex = 2;
				vecIntersectionInfo.push_back(oInfo2);

				TInterSectionMovement oInfo3 = {};
				oInfo3.nxFromNode = nxNext;
				oInfo3.nxAtNode	  = nxNodeID;
				oInfo3.nxToNode	  = nxMain;
				oInfo3.nTurnIndex = 3;
				vecIntersectionInfo.push_back(oInfo3);

				TInterSectionMovement oInfo4 = {};
				oInfo4.nxFromNode	= nxBefore;
				oInfo4.nxAtNode		= nxNodeID;
				oInfo4.nxToNode		= nxMain;
				oInfo4.nTurnIndex	= 4;
				vecIntersectionInfo.push_back(oInfo4);

				TInterSectionMovement oInfo5 = {};
				oInfo5.nxFromNode		= nxBefore;
				oInfo5.nxAtNode			= nxNodeID;
				oInfo5.nxToNode			= nxNext;
				oInfo5.nTurnIndex		= 5;
				vecIntersectionInfo.push_back(oInfo5);
			}	 
		}

		size_t nCount = vecIntersectionInfo.size();
		for (size_t i = 0; i < nCount; i++ )
		{
			TInterSectionMovement oInfo = vecIntersectionInfo[i];
			ofs.write(reinterpret_cast<char*>( &oInfo), sizeof(TInterSectionMovement));  
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ofs.close();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}

bool KDBaseAssignment::InterSectionBoundTurnTypeDB2File( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, bound_direction, lt_type, rt_type, signal_type from %s order by node_id, bound_direction "), TABLE_INTERSECTION_INFO);

		std::map<Integer, TInterSectionBoundTurnType> mapBoundType;

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{

			Integer nxNodeID		= spResult->GetValueInt64(0);
			int		nBoundDirection = spResult->GetValueInt(1);
			int		nlt				= spResult->GetValueInt(2);
			int		nrt				= spResult->GetValueInt(3);
			int		nSigtype		= spResult->GetValueInt(4);


			AutoType find = mapBoundType.find(nxNodeID);
			AutoType findend  = mapBoundType.end();
			if (find == findend)
			{
				TInterSectionBoundTurnType oInfo = {};
				oInfo.nxNodeID = nxNodeID;
				if (1 == nBoundDirection )
				{
					oInfo.n1Lt		= nlt;
					oInfo.n1Rt		= nrt;
					oInfo.n1SigType = nSigtype;
				}
				else if ( 2 == nBoundDirection)
				{
					oInfo.n2Lt		= nlt;
					oInfo.n2Rt		= nrt;
					oInfo.n2SigType = nSigtype;
				}
				else if ( 3 == nBoundDirection)
				{
					oInfo.n3Lt		= nlt;
					oInfo.n3Rt		= nrt;
					oInfo.n3SigType = nSigtype;
				}
				else if ( 4 == nBoundDirection)
				{
					oInfo.n4Lt		= nlt;
					oInfo.n4Rt		= nrt;
					oInfo.n4SigType = nSigtype;
				}
				mapBoundType.insert(std::make_pair(nxNodeID, oInfo));
			}
			else
			{
				TInterSectionBoundTurnType& oInfo = find->second;
				if (1 == nBoundDirection )
				{
					oInfo.n1Lt		= nlt;
					oInfo.n1Rt		= nrt;
					oInfo.n1SigType = nSigtype;
				}
				else if ( 2 == nBoundDirection)
				{
					oInfo.n2Lt		= nlt;
					oInfo.n2Rt		= nrt;
					oInfo.n2SigType = nSigtype;
				}
				else if ( 3 == nBoundDirection)
				{
					oInfo.n3Lt		= nlt;
					oInfo.n3Rt		= nrt;
					oInfo.n3SigType = nSigtype;
				}
				else if ( 4 == nBoundDirection)
				{
					oInfo.n4Lt		= nlt;
					oInfo.n4Rt		= nrt;
					oInfo.n4SigType = nSigtype;
				}
			}
		}

		AutoType iter = mapBoundType.begin();
		AutoType end  = mapBoundType.end();
		while(iter != end)
		{
			TInterSectionBoundTurnType oInfo = iter->second;
			ofs.write(reinterpret_cast<char*>( &oInfo), sizeof(TInterSectionBoundTurnType));  
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ofs.close();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		ofs.close();
		return false;

	}

	ofs.close();
	return true;
}

bool KDBaseAssignment::InterSectionPhaseOperDB2File( KDBaseConPtr a_spDBaseConnection, CString strFileName )
{
	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		TxLogDebugException();
		return false;
	}

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select node_id, signal_seq, signal_lamp_type ,green_lamp_duration from %s order by node_id, signal_seq "), TABLE_INTERSECTION_SIGNAL_INDICATION);

		std::map<Integer, TInterSectionPhaseOper> mapPhaseType;

		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResult->Next())
		{
			Integer nxNodeID		= spResult->GetValueInt64(0);
			int		nSeq			= spResult->GetValueInt(1);
			int		nlamptype		= spResult->GetValueInt(2);
			int		nlampduation	= spResult->GetValueInt(3);

			AutoType find = mapPhaseType.find(nxNodeID);
			AutoType findend  = mapPhaseType.end();
			if (find == findend)
			{
				TInterSectionPhaseOper oInfo = {};
				oInfo.nxNodeID = nxNodeID;
				if (1 == nSeq )
				{
					oInfo.nPhaseType1 = nlamptype;
					oInfo.nPhaseTime1 = nlampduation;
				}
				else if ( 2 == nSeq)
				{
					oInfo.nPhaseType2 = nlamptype;
					oInfo.nPhaseTime2 = nlampduation;
				}
				else if ( 3 == nSeq)
				{
					oInfo.nPhaseType3 = nlamptype;
					oInfo.nPhaseTime3 = nlampduation;
				}
				else if ( 4 == nSeq)
				{
					oInfo.nPhaseType4 = nlamptype;
					oInfo.nPhaseTime4 = nlampduation;
				}
				else if ( 5 == nSeq)
				{
					oInfo.nPhaseType5 = nlamptype;
					oInfo.nPhaseTime5 = nlampduation;
				}
				else if ( 6 == nSeq)
				{
					oInfo.nPhaseType6 = nlamptype;
					oInfo.nPhaseTime6 = nlampduation;
				}
				mapPhaseType.insert(std::make_pair(nxNodeID, oInfo));
			}
			else
			{
				TInterSectionPhaseOper& oInfo = find->second;
				oInfo.nxNodeID = nxNodeID;
				if (1 == nSeq )
				{
					oInfo.nPhaseType1 = nlamptype;
					oInfo.nPhaseTime1 = nlampduation;
				}
				else if ( 2 == nSeq)
				{
					oInfo.nPhaseType2 = nlamptype;
					oInfo.nPhaseTime2 = nlampduation;
				}
				else if ( 3 == nSeq)
				{
					oInfo.nPhaseType3 = nlamptype;
					oInfo.nPhaseTime3 = nlampduation;
				}
				else if ( 4 == nSeq)
				{
					oInfo.nPhaseType4 = nlamptype;
					oInfo.nPhaseTime4 = nlampduation;
				}
				else if ( 5 == nSeq)
				{
					oInfo.nPhaseType5 = nlamptype;
					oInfo.nPhaseTime5 = nlampduation;
				}
				else if ( 6 == nSeq)
				{
					oInfo.nPhaseType6 = nlamptype;
					oInfo.nPhaseTime6 = nlampduation;
				}
			}
		}

		AutoType iter = mapPhaseType.begin();
		AutoType end  = mapPhaseType.end();
		while(iter != end)
		{
			TInterSectionPhaseOper oInfo = iter->second;

			CString strSQLNo(_T(""));
			strSQLNo.Format(_T("Select node_id, max(signal_seq)from %s where node_id = %I64d"), TABLE_INTERSECTION_SIGNAL_INDICATION, oInfo.nxNodeID);
			KResultSetPtr spResultNo = a_spDBaseConnection->ExecuteQuery(strSQLNo);
			while(spResultNo->Next())
			{
				int nNo = spResultNo->GetValueInt(1);
				oInfo.nPhaseNo = nNo;
			}
			ofs.write(reinterpret_cast<char*>(&oInfo), sizeof(TInterSectionPhaseOper));
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		ofs.close();
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}

void KDBaseAssignment::GetModeChoiceColumn( KTarget* a_pTarget, KMode* a_pMode, std::vector<TModeChoiceColumnInfo>& ar_vecModeColumnInfo )
{
	ar_vecModeColumnInfo.clear();
	try
	{
		KDBaseConPtr spDBaseConnetion = a_pTarget->GetDBaseConnection();
		
		CString strSQL(_T(""));
		strSQL.Format(_T("Select mode_od_column_name, start_date, end_date From %s "), TABLE_MODECHOICE_MODEL);
		strSQL.AppendFormat(_T(" Where mode_group_code = %d and detail_mode_id = %d "), a_pMode->ModeGroup(), a_pMode->ModeID());
		strSQL.AppendFormat(_T(" Order by end_date DESC"));

		KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			TModeChoiceColumnInfo oInfo = {};
			oInfo.TnModeGroupCode = a_pMode->ModeGroup();
			oInfo.TnDetailmodeID  = a_pMode->ModeID();
			oInfo.TstrColumnName  = spResultset->GetValueString(0);
			oInfo.TstrStartDate	  = spResultset->GetValueString(1);
			oInfo.TstrEndDate	  = spResultset->GetValueString(2);

			ar_vecModeColumnInfo.push_back(oInfo);
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

void KDBaseAssignment::GetModeChoiceODAtGroupingMode( KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapModeColumnInfo )
{
	ar_mapModeColumnInfo.clear();
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnetion = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select detail_mode_id, mode_od_column_name From %s order by detail_mode_id "), TABLE_MODECHOICE_MODEL);

		KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			int nModeID				= spResultset->GetValueInt(0);
			CString strColumnName	= spResultset->GetValueString(1);

			AutoType find = ar_mapModeColumnInfo.find(nModeID);
			AutoType end  = ar_mapModeColumnInfo.end();
			if (find != end)
			{
				std::vector<CString>& vecColumns = find->second;
				vecColumns.push_back(strColumnName);
			}
			else
			{
				std::vector<CString> vecColumns;
				vecColumns.push_back(strColumnName);
				ar_mapModeColumnInfo.insert(std::make_pair(nModeID, vecColumns));
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


void KDBaseAssignment::GetInterDefineCost( KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost )
{
	TxLogDebugStartMsg();

	a_mapTransitCost.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	TMATransitCostInfo oTCostInfo;
	try 
	{
		CString strSQL(_T(""));
		strSQL.Format(
			_T(" SELECT Transit_Type, Transit_Detail_Type, TBL2.KCode_Display AS Detail_Display,  Base_Cost, Base_Length, Add_Cost, Add_Length, Cost_Method ")
			_T(" FROM ")
			_T(" (SELECT Transit_Type, Transit_Detail_Type, Base_Cost, Base_Length, Add_Cost, Add_Length, Cost_Method FROM %s)TBL1 ")
			_T(" LEFT OUTER JOIN ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL2  ")
			_T(" ON TBL2.KCode_Key = TBL1.Transit_Detail_Type  "), TABLE_INTER_TRANSIT_COST, CODE_GROUP_KEY_TRANSIT_DETAIL);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			TMATransitCostInfo oTCostInfo;
			oTCostInfo.TnMode			= spResult->GetValueInt   (0);
			oTCostInfo.TnType			= spResult->GetValueInt   (1);
			oTCostInfo.TstrTypeName     = spResult->GetValueString(2);
			oTCostInfo.TdBasicRate      = spResult->GetValueDouble(3);
			oTCostInfo.TdBasicDistance  = spResult->GetValueDouble(4);
			oTCostInfo.TdAddRate        = spResult->GetValueDouble(5);
			oTCostInfo.TdAddDistance    = spResult->GetValueDouble(6);
			oTCostInfo.TnMergeRate      = spResult->GetValueInt   (7);

			a_mapTransitCost.push_back(oTCostInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		a_mapTransitCost.clear();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		a_mapTransitCost.clear();
		TxLogDebugException();
		throw 1;
	}

	TxLogDebugEndMsg();
}

void KDBaseAssignment::GetFModeChoiceODAtGroupingMode( KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapModeColumnInfo )
{
	ar_mapModeColumnInfo.clear();
	CString strSelModeSQL(_T(""));
	CString strSelColuSQL(_T(""));
	try
	{
		std::vector<int> vecFmodeCode;

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		strSelModeSQL.Format(_T("Select fmode_id from %s Order by fmode_id"), TABLE_DETAIL_FMODE);
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSelModeSQL);
		while(spResultset->Next())
		{
			int nCode = spResultset->GetValueInt(0);
			vecFmodeCode.push_back(nCode);
		}

		size_t nCount = vecFmodeCode.size();
		for (size_t i = 0; i < nCount; i++)
		{
			int nCode = vecFmodeCode[i];

			strSelColuSQL.Format(_T("Select column_name From %s where fmode_id = %d "), TABLE_FMODE_MODEL, nCode);
			KResultSetPtr spColumnResult = spDBaseConnection->ExecuteQuery(strSelColuSQL);

			std::vector<CString> vecColumnName;
			while(spColumnResult->Next())
			{
				CString strColumnName = spColumnResult->GetValueString(0);
				vecColumnName.push_back(strColumnName);
			}

			ar_mapModeColumnInfo.insert(std::make_pair(nCode, vecColumnName));
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

void KDBaseAssignment::GetZoneID( KTarget* a_pTarget, std::set<Integer>& ar_setZoneID )
{
	try
	{
		ar_setZoneID.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select zone_id from Zone"));
		KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			Integer nxZoneID = spResultset->GetValueInt64(0);
			ar_setZoneID.insert(nxZoneID);
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

void KDBaseAssignment::GetDistributionODAtGroupingPurpose( KTarget* a_pTarget, std::map<int,std::vector<CString>>& ar_mapPurposeColumnInfo )
{
	ar_mapPurposeColumnInfo.clear();
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnetion = a_pTarget->GetDBaseConnection();
		strSQL.Format(_T("Select detail_object_id, purpose_od_column_name From %s order by detail_object_id "), TABLE_DISTRIBUTION_MODEL);

		KResultSetPtr spResultset = spDBaseConnetion->ExecuteQuery(strSQL);
		while(spResultset->Next())
		{
			int nModeID				= spResultset->GetValueInt(0);
			CString strColumnName	= spResultset->GetValueString(1);

			AutoType find = ar_mapPurposeColumnInfo.find(nModeID);
			AutoType end  = ar_mapPurposeColumnInfo.end();
			if (find != end)
			{
				std::vector<CString>& vecColumns = find->second;
				vecColumns.push_back(strColumnName);
			}
			else
			{
				std::vector<CString> vecColumns;
				vecColumns.push_back(strColumnName);
				ar_mapPurposeColumnInfo.insert(std::make_pair(nModeID, vecColumns));
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

void KDBaseAssignment::UpdateInterModeODInfo( KDBaseConPtr a_spDBaseConnection, std::vector<TInterModeODInfo> a_vecModeInfo )
{
	try
	{
		CString strDelSQL(_T(""));
		strDelSQL.Format(_T("Delete From %s"), TABLE_ASSIGN_INTER_MODE_INFO);
		a_spDBaseConnection->ExecuteUpdate(strDelSQL);
		
		CString strInsertSQL(_T(""));
		strInsertSQL.Format(_T("Insert Into %s(id, name, column) "), TABLE_ASSIGN_INTER_MODE_INFO);
		strInsertSQL.Append(_T(" Values "));
		strInsertSQL.Append(_T(" (?, ?, ?)"));
		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strInsertSQL);
		for (size_t i = 0; i < a_vecModeInfo.size(); i++)
		{
			TInterModeODInfo oInfo = a_vecModeInfo[i];
			spPrepareStatement->BindInt(1, oInfo.nModeID);
			spPrepareStatement->BindText(2, oInfo.strModeName);
			spPrepareStatement->BindText(3, oInfo.strModeColumn);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
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
