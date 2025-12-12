#include "StdAfx.h"
#include "DBaseMAssignment.h"
#include "Mode.h"
#include "Target.h"
#include "KBulkDBase.h"
using namespace std;

#define CODE_GROUP_KEY_TRANSIT_DETAIL 114

KDBaseMAssignment::KDBaseMAssignment(void)
{
}


KDBaseMAssignment::~KDBaseMAssignment(void)
{
}

bool KDBaseMAssignment::ModeChoiceExist( KTarget* a_pTarget )
{
	KLogDebugStartMsg();
	CString strQuery = _T("select count(*) CNT from modechoice_model");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	int nRecordCount(0);
	bool bExist = false;

	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);

		if (resultSetPtr->Next())
			nRecordCount = resultSetPtr->GetValueInt(0);

		if( 0 != nRecordCount )
			bExist = true;
	}
	catch(...)
	{
		KLogDebugException();
		return false;
	}
	KLogDebugEndMsg();
	return bExist;
}

void KDBaseMAssignment::GetModeChoiceModelinfo( KTarget* a_pTarget, std::map<CString, int>& a_mapModeChoice )
{
	KLogDebugStartMsg();

	a_mapModeChoice.clear();

	CString strQuery = _T("select detail_mode_id, mode_od_column_name from modechoice_model");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
		while (resultSetPtr->Next())
		{
			int nModeId			  = resultSetPtr->GetValueInt(0);
			CString strColumnName = resultSetPtr->GetValueString(1);
			a_mapModeChoice.insert(std::make_pair(strColumnName, nModeId));
		}

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		throw 1;
	}

	KLogDebugEndMsg();
}

void KDBaseMAssignment::GetLinkTravelTimeField( KTarget* a_pTarget, std::set<CString>& a_lstTraveTimeField )
{
	a_lstTraveTimeField.clear();

	CString strQuery = _T("select average_time from assign_model ");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
		while (resultSetPtr->Next())
		{
			CString strColumnName = resultSetPtr->GetValueString(0);
			a_lstTraveTimeField.insert(strColumnName);
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		throw 1;
	}

	KLogDebugEndMsg();
}

bool KDBaseMAssignment::TravelTimeExist( KTarget* a_pTarget )
{
	KLogDebugStartMsg();
	CString strQuery = _T("select count(*) CNT from assign_model");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	int nRecordCount(0);
	bool bExist = false;

	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);

		if (resultSetPtr->Next())
			nRecordCount = resultSetPtr->GetValueInt(0);

		if( 0 != nRecordCount )
			bExist = true;
	}
	catch(...)
	{
		KLogDebugException();
		return false;
	}
	KLogDebugEndMsg();
	return bExist;
}

int KDBaseMAssignment::GetNewAssignRunSeq( KTarget* a_pTarget )
{
	KLogDebugStartMsg();
	int nRunSeq(0);
	CString strQuery = _T("Select ifnull(max(run_seq)+1, 1) as run_seq From assign_model");
	KDBaseConnection oDBaseConnection( a_pTarget->GetDB() );

	try
	{
		KResultSetPtr oResultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);

		if(oResultSetPtr->Next())
			nRunSeq = oResultSetPtr->GetValueInt(0);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}
	KLogDebugEndMsg();
	return nRunSeq;
}
//
//void KDBaseMAssignment::GetAssignResultInfo( KTarget* a_pTarget, std::vector<TAssignResultInfo>& a_vecMAResultInfo )
//{
//	KLogDebugStartMsg();
//	a_vecMAResultInfo.clear();
//
//	CString strSQL;
//	strSQL =	_T("select run_seq, start_date, end_date, type, description, average_time,");
//	strSQL +=	_T(" vc, sum_volume, trip, mode_volume, vdf_time, ave_time_toll, vdf_toll_time,");
//	strSQL +=	_T(" bus_initial_volume, od_average_time, od_vdf_time, od_average_bus, od_average_subway, ");
//	strSQL +=	_T(" od_bus_direct, od_bus_one_transfer, od_bus_two_transfer, linfo_num_passengers, ");
//	strSQL +=   _T(" line_boarding, line_alighting, line_second_ride from assign_model");
//	
//	KDBaseConnection oDBaseConnection (a_pTarget->GetDB());
//	try
//	{        
//		KResultSetPtr  spResultSet  = oDBaseConnection.ExecuteQuery(strSQL);
//
//		while ( spResultSet->Next() )
//		{
//			TAssignResultInfo oMAssignResultInfo;
//			oMAssignResultInfo.TnRunSeq				 = spResultSet->GetValueInt(0);
//			oMAssignResultInfo.TstrStartDate		 = spResultSet->GetValueString(1);
//			oMAssignResultInfo.TstrEndDate			 = spResultSet->GetValueString(2);
//			oMAssignResultInfo.TnType				 = spResultSet->GetValueInt(3);
//			oMAssignResultInfo.TstrDescription		 = spResultSet->GetValueString(4);
//			oMAssignResultInfo.TstrAverageTime		 = spResultSet->GetValueString(5);
//			oMAssignResultInfo.TstrVC				 = spResultSet->GetValueString(6);
//			oMAssignResultInfo.TstrSumVolumn		 = spResultSet->GetValueString(7);
//			oMAssignResultInfo.TstrTrip				 = spResultSet->GetValueString(8);
//			oMAssignResultInfo.TstrModeVolume		 = spResultSet->GetValueString(9);
//			oMAssignResultInfo.TstrVDFTime			 = spResultSet->GetValueString(10);
//			oMAssignResultInfo.TstrAVETimeToll		 = spResultSet->GetValueString(11);
//			oMAssignResultInfo.TstrVDFTimeToll		 = spResultSet->GetValueString(12);
//			oMAssignResultInfo.TstrBusInitVolume	 = spResultSet->GetValueString(13);
//			oMAssignResultInfo.TstrODAveTime		 = spResultSet->GetValueString(14);
//			oMAssignResultInfo.TstrODVDFTime		 = spResultSet->GetValueString(15);
//			oMAssignResultInfo.TstrODAveBus			 = spResultSet->GetValueString(16);
//			oMAssignResultInfo.TstrODAveSubway		 = spResultSet->GetValueString(17);
//			oMAssignResultInfo.TstrODBusDirect		 = spResultSet->GetValueString(18);
//			oMAssignResultInfo.TstrODBusOneTrans	 = spResultSet->GetValueString(19);
//			oMAssignResultInfo.TstrODBusTwoTrans	 = spResultSet->GetValueString(20);
//			oMAssignResultInfo.TstrLInfoPassengernum = spResultSet->GetValueString(21);
//			oMAssignResultInfo.TstrLineBorading		 = spResultSet->GetValueString(22);
//			oMAssignResultInfo.TstrLineAlighting	 = spResultSet->GetValueString(23);
//			oMAssignResultInfo.TstrLInfoPassengernum = spResultSet->GetValueString(24);
//			
//			a_vecMAResultInfo.push_back(oMAssignResultInfo);
//		}
//	}
//	catch (KExceptionPtr ex)
//	{
//		Log4cxxExceptionPrint(ex);
//	}
//	catch (...)
//	{
//		KLogDebugWarning();
//	} 
//
//}

void KDBaseMAssignment::GetNoClass( KTarget* a_pTarget, std::map<CString, double>& a_mapNoClass )
{
	KLogDebugStartMsg();
	a_mapNoClass.clear();

	CString strQuery = _T("select class_name,time_min from access_class ");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
		while (resultSetPtr->Next())
		{
			CString strClassName = resultSetPtr->GetValueString(0);
			double	dTimemin	 = resultSetPtr->GetValueDouble(1);
			a_mapNoClass.insert(std::make_pair(strClassName, dTimemin));
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		throw 1;
	}

	KLogDebugEndMsg();
}


void KDBaseMAssignment::GetUtilitFnCoffcient( KTarget* a_pTarget, std::vector<TMAFnCofficient>& a_mapFnCoeffcient )
{
	KLogDebugStartMsg();

	a_mapFnCoeffcient.clear();

	CString strQuery = _T("select Transit_Type, In_Vehicle_Time, Wait_Time, Transfer_Time, Access_Walk_Time, Egress_Walk_Time, Num_Transfers, Cost, Num_Stop from bus_path_coefficient");
	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
	TMAFnCofficient oTFnCoffInfo;
	try
	{
		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
		while (resultSetPtr->Next())
		{
			oTFnCoffInfo.TnMode				= resultSetPtr->GetValueInt(0);
			oTFnCoffInfo.TdInVehcle			= resultSetPtr->GetValueDouble(1);
			oTFnCoffInfo.TdWaitingTime		= resultSetPtr->GetValueDouble(2);
			oTFnCoffInfo.TdTransferTime		= resultSetPtr->GetValueDouble(3);
			oTFnCoffInfo.TdAccessWalkTime	= resultSetPtr->GetValueDouble(4);
			oTFnCoffInfo.TdEqressWalkTime	= resultSetPtr->GetValueDouble(5);
			oTFnCoffInfo.TdNoofTransfer		= resultSetPtr->GetValueDouble(6);
			oTFnCoffInfo.TdCost				= resultSetPtr->GetValueDouble(7);
			oTFnCoffInfo.TdNoofStop			= resultSetPtr->GetValueDouble(8);

			a_mapFnCoeffcient.push_back(oTFnCoffInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		throw 1;
	}

	KLogDebugEndMsg();
}


void KDBaseMAssignment::GetOptionCost( KTarget* a_pTarget, std::vector<TMATransitCostInfo>& a_mapTransitCost )
{
	KLogDebugStartMsg();

	a_mapTransitCost.clear();

	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
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

		KResultSetPtr pResult = oDBaseConnection.ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TMATransitCostInfo oTCostInfo;
			oTCostInfo.TnMode			= pResult->GetValueInt   (0);
			oTCostInfo.TnType			= pResult->GetValueInt   (1);
			oTCostInfo.TstrTypeName     = pResult->GetValueString(2);
			oTCostInfo.TdBasicRate      = pResult->GetValueDouble(3);
			oTCostInfo.TdBasicDistance  = pResult->GetValueDouble(4);
			oTCostInfo.TdAddRate        = pResult->GetValueDouble(5);
			oTCostInfo.TdAddDistance    = pResult->GetValueDouble(6);
			oTCostInfo.TnMergeRate      = pResult->GetValueInt   (7);

			a_mapTransitCost.push_back(oTCostInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		KLogDebugException();
		throw 1;
	}

	KLogDebugEndMsg();
}


//
//void KDBaseMAssignment::GetInitBusPathValue( KTarget* a_pTarget,TMATransitOptionsBaseInfo& a_oBaseInfo )
//{
//	KLogDebugStartMsg();
//
//	CString strQuery = _T("select Max_Routes, Time_Board, Wait_Vdf_Alpha, Wait_Vdf_Beta, Rail_Station_Node_Type, Rail_Link_Type, Rail_Connect_Link_Type from bus_path_input");
//	KDBaseConnection oDBaseConnection(a_pTarget->GetDB());
//	try
//	{
//		KResultSetPtr resultSetPtr = oDBaseConnection.ExecuteQuery(strQuery);
//		while (resultSetPtr->Next())
//		{
//			a_oBaseInfo.TnMaxofNo					= resultSetPtr->GetValueInt(0);
//			a_oBaseInfo.TnTimeBorardingAlighting	= resultSetPtr->GetValueInt(1);
//			a_oBaseInfo.TdAlpha						= resultSetPtr->GetValueDouble(2);
//			a_oBaseInfo.TdBeta						= resultSetPtr->GetValueDouble(3);
//			a_oBaseInfo.TnSationNodeType			= resultSetPtr->GetValueInt(4);
//			a_oBaseInfo.TnSubwayLinkType			= resultSetPtr->GetValueInt(5);
//			a_oBaseInfo.TnSubwayConnectLinkType		= resultSetPtr->GetValueInt(6);
//		}
//	}
//	catch (KExceptionPtr ex)
//	{
//		KLogDebug(ex->GetErrorMessage());
//		throw ex;
//	}
//	catch (...)
//	{
//		KLogDebugException();
//		throw 1;
//	}
//
//	KLogDebugEndMsg();
//}
//



bool KDBaseMAssignment::ExportDB2NodeFileExcludeNodeOff( sqlite3* pDB, CString strFileName/*=_T("node.dat")*/ )
{
	KLogDebugStartMsg();

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		KLogDebugException();
		return false;
	}

	sqlite3_stmt* pstmt = NULL;
	SqliteTail tail;
	int nDBState = SQLITE_ERROR;    

	sqlite3_exec(pDB, "PRAGMA synchronous  = OFF",    NULL, NULL, NULL); 
	sqlite3_exec(pDB, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);

	//network_off read
	CString strNodeOffResult(_T(""));
	CString strNodeOffSQL = _T("select node_off from network_off");

	nDBState = SqlitePrepareV2(pDB, strNodeOffSQL, -1, &pstmt, &tail);

	if(SQLITE_ERROR == nDBState)
	{
		KLogDebug(sqlite3_errmsg(pDB));
		SqliteFinalize(pstmt);
		ofs.close();
		return false;
	}
	nDBState = SqliteStep(pstmt);

	if(SQLITE_ROW == nDBState)
		strNodeOffResult = SqliteColumnText(pstmt, 0);

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

	nDBState = SqlitePrepareV2(pDB, strSQL, -1, &pstmt, &tail);

	if(SQLITE_ERROR == nDBState)
	{
		KLogDebug(sqlite3_errmsg(pDB));
		SqliteFinalize(pstmt);
		ofs.close();
		return false;
	}

	nDBState = SqliteStep(pstmt);
	TFixedNode node;
	while(SQLITE_ROW == nDBState)
	{
		//ofs.write(reinterpret_cast<char*>)
		node.node_id   = SqliteColumnInt64 (pstmt, 0);  
		node.node_type = SqliteColumnInt   (pstmt, 1);
		node.x         = SqliteColumnDouble(pstmt, 2);
		node.y         = SqliteColumnDouble(pstmt, 3);
		node.zone_id   = SqliteColumnInt64 (pstmt, 4);  

		ofs.write(reinterpret_cast<char*>( &node   ), sizeof(TFixedNode) );  
		nDBState = SqliteStep(pstmt);
	}

	SqliteFinalize(pstmt);

	ofs.close();

	KLogDebugEndMsg();
	return true;
}

bool KDBaseMAssignment::ExportDB2ODFile( sqlite3* pDB, CString strTableName, KIOColumnCollection& userColumnCollection, CString strFileName )
{
	KLogDebugStartMsg();

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
		KLogDebugException();
		return false;
	}

	sqlite3_stmt* pstmt = NULL;
	SqliteTail tail;
	int nDBState = SQLITE_ERROR;    

	sqlite3_exec(pDB, "PRAGMA synchronous  = OFF",    NULL, NULL, NULL); 
	sqlite3_exec(pDB, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);  

	nDBState = SqlitePrepareV2(pDB, strSQL, -1, &pstmt, &tail);
	if(SQLITE_ERROR == nDBState)
	{
		KLogDebug(sqlite3_errmsg(pDB));
		KLogDebug(strSQL.AllocSysString());
		SqliteFinalize(pstmt);
		ofs.close();
		return false;
	}

	nDBState = SqliteStep(pstmt);
	TFixedOD fixedOD;
	while(SQLITE_ROW == nDBState)
	{
		//ofs.write(reinterpret_cast<char*>)
		fixedOD.ozone_id  = SqliteColumnInt64 (pstmt, 0);  
		fixedOD.dzone_id  = SqliteColumnInt64 (pstmt, 1);

		ofs.write(reinterpret_cast<char*>( &fixedOD ), sizeof(TFixedOD) );  

		for (int i=0; i<nColumnCount; i++)
		{
			double dValue = SqliteColumnDouble(pstmt, 2+i);
			ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
		}

		nDBState = SqliteStep(pstmt);
	}

	SqliteFinalize(pstmt);
	ofs.close();

	KLogDebugEndMsg();
	return true;
}

bool KDBaseMAssignment::ExportDB2LinkFileWithOutHighway( sqlite3* pDB, int nUseLinkTime, KIOColumn* a_pColumn,CString strFileName /*= _T("link.dat")*/ )
{
	KLogDebugStartMsg();

	CString strSQL = _T("select link_id, fnode_id, tnode_id, link_type, length, ");
	if (nUseLinkTime == 0)
	{
		strSQL = strSQL + _T("speed");
	}
	else
	{
		CString strColumnName = a_pColumn->Name();
		strSQL = strSQL + strColumnName;
	}

	strSQL = strSQL + _T(" From Link Order By link_id");

	ofstream ofs(strFileName, ios::binary);
	if (! ofs)
	{
		KLogDebugException();
		return false;
	}

	sqlite3_stmt* pstmt = NULL;
	SqliteTail tail;
	int nDBState = SQLITE_ERROR;    

	sqlite3_exec(pDB, "PRAGMA synchronous  = OFF",    NULL, NULL, NULL); 
	sqlite3_exec(pDB, "PRAGMA journal_mode = MEMORY", NULL, NULL, NULL);  
	nDBState = SqlitePrepareV2(pDB, strSQL, -1, &pstmt, &tail);
	if(SQLITE_ERROR == nDBState)
	{
		KLogDebug(sqlite3_errmsg(pDB));
		SqliteFinalize(pstmt);
		ofs.close();
		return false;
	}

	nDBState = SqliteStep(pstmt);

	TMALinkInfo TlinkInfo;
	while(SQLITE_ROW == nDBState)
	{
		TlinkInfo.TnxLink_ID	= SqliteColumnInt64 (pstmt, 0);  
		TlinkInfo.TnxFNode_ID	= SqliteColumnInt64 (pstmt, 1);
		TlinkInfo.TnxTNode_ID	= SqliteColumnInt64 (pstmt, 2);
		TlinkInfo.TnLinkType	= SqliteColumnInt   (pstmt, 3);
		TlinkInfo.TdLength		= SqliteColumnDouble(pstmt, 4);
		TlinkInfo.TdAddField	= SqliteColumnDouble(pstmt, 5);

		ofs.write(reinterpret_cast<char*>( &TlinkInfo ), sizeof(TMALinkInfo));

		nDBState = SqliteStep(pstmt);
	}
	SqliteFinalize(pstmt);
	ofs.close();

	KLogDebugEndMsg();
	return true;
}
