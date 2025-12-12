#include "StdAfx.h"
#include "DBaseDefineTransitCost.h"
//^#include "DBaseConnector.h"
#include "Target.h"

KDBaseDefineTransitCost::KDBaseDefineTransitCost(void)
{
}


KDBaseDefineTransitCost::~KDBaseDefineTransitCost(void)
{
}

void KDBaseDefineTransitCost::GetTransitCostValue2DB( KTarget* a_pTarget, std::map<int, TTransitCost>& a_mapTransitCost )
{

	a_mapTransitCost.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL(_T(""));

		strSQL.Format(_T("Select type, mode, basic_rate, basic_distance, add_rate, add_distance, merge_rete From %s"), TABLE_TRANSIT_COST);

		KResultSetPtr pResultCost = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResultCost->Next())
		{
			TTransitCost oTransitCost;
			oTransitCost.nTType				= pResultCost->GetValueInt(0);
			oTransitCost.nMode				= pResultCost->GetValueInt(1);
			oTransitCost.dTbasicRate		= pResultCost->GetValueDouble(2);
			oTransitCost.dTbasicDistance	= pResultCost->GetValueDouble(3);
			oTransitCost.dTAddRate			= pResultCost->GetValueDouble(4);
			oTransitCost.dTAddDistance		= pResultCost->GetValueDouble(5);
			oTransitCost.nTMergeRate		= pResultCost->GetValueInt(6);

			a_mapTransitCost.insert(std::make_pair(oTransitCost.nTType, oTransitCost));
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

void KDBaseDefineTransitCost::GetTransitType2DB( KTarget* a_pTarget, std::vector<int>& a_vecTransitType )
{

	a_vecTransitType.clear();

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{

		CString strSQL(_T(""));
		strSQL.Format(_T("Select detail_type From %s group by detail_type"), TABLE_TRANSIT);

		KResultSetPtr pResultCost = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResultCost->Next())
		{
			int nDType = pResultCost->GetValueInt(0);
			a_vecTransitType.push_back(nDType);
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

void KDBaseDefineTransitCost::UpdateTransitCost( KTarget* a_pTarget, std::vector<TTransitCost> a_vecTransitCost )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{
		CString strDeleteSQL = _T("");
		strDeleteSQL.Format(_T("delete from %s"), TABLE_TRANSIT_COST);
		spDBaseConnection->ExecuteUpdate(strDeleteSQL);

		int nCount = a_vecTransitCost.size();
		for (int i = 0; i < nCount; i++)
		{
			TTransitCost oCost = a_vecTransitCost[i];
			CString strSQL(_T(""));
			strSQL.Format(_T("Insert Into %s(type, mode, basic_rate, basic_distance, add_rate, add_distance, merge_rete) Values  (%d, %d, %f, %f, %f, %f, %d)"), 
				TABLE_TRANSIT_COST, oCost.nTType, oCost.nMode
				, oCost.dTbasicRate, oCost.dTbasicDistance, oCost.dTAddRate, oCost.dTAddDistance, oCost.nTMergeRate);
			spDBaseConnection->ExecuteUpdate(strSQL);
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

void KDBaseDefineTransitCost::GetTransitModeCostValue2DB( KTarget* a_pTarget, std::vector<TTransitCost>& a_vecTransitCost )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		a_vecTransitCost.clear();

		CString strSQL(_T(""));
// 		strSQL.Append(_T(" Select tA.type, tA.detail_type, tB.basic_rate, tB.basic_distance, tB.add_rate, tB.add_distance, tB.merge_rete "));
// 		strSQL.AppendFormat(_T(" From %s tA, %s tB Where tA.detail_type = tB.type Group by tA.detail_type "), TABLE_TRANSIT, TABLE_TRANSIT_COST);

		strSQL.Append(_T(" SELECT TBL1.type, TBL1.detail_type, TBL2.basic_rate, TBL2.basic_distance, TBL2.add_rate, TBL2.add_distance, TBL2.merge_rete  "));
		strSQL.Append(_T(" FROM  "));
		strSQL.Append(_T(" (SELECT type, detail_type FROM transit GROUP BY detail_Type)TBL1,  "));
		strSQL.Append(_T(" transit_cost TBL2  "));
		strSQL.Append(_T(" WHERE TBL1.detail_type = TBL2.type ORDER BY TBL1.type  "));

		KResultSetPtr pResultCost = spDBaseConnection->ExecuteQuery(strSQL);

		while(pResultCost->Next())
		{
			TTransitCost oTransitCost;
			oTransitCost.nMode			= pResultCost->GetValueInt(0);
			oTransitCost.nTType			= pResultCost->GetValueInt(1);
			oTransitCost.dTbasicRate	= pResultCost->GetValueDouble(2);
			oTransitCost.dTbasicDistance= pResultCost->GetValueDouble(3);
			oTransitCost.dTAddRate		= pResultCost->GetValueDouble(4);
			oTransitCost.dTAddDistance	= pResultCost->GetValueDouble(5);
			oTransitCost.nTMergeRate	= pResultCost->GetValueInt(6);
			a_vecTransitCost.push_back(oTransitCost);
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


bool KDBaseDefineTransitCost::WriteDefineCostArgument(CStdioFileEx& of, std::vector<TTransitCost>& a_vecTransitCost)
{
	CString strOut = _T("");

	try
	{
		const int nBusMode(3);
		const int nRailMode(4);

		std::vector<TTransitCost> vecBusCost;
		std::vector<TTransitCost> vecRailCost;

		for (size_t i= 0; i< a_vecTransitCost.size(); i++)
		{
			TTransitCost &oTTransitCost = a_vecTransitCost[i];
			int &nMode = a_vecTransitCost[i].nMode;

			if (nMode == nBusMode)
			{
				vecBusCost.push_back(oTTransitCost);
			}
			else if (nMode == nRailMode)
			{
				vecRailCost.push_back(oTTransitCost);
			}
		}

		strOut.Format(_T("%s\t%d\t%d\r\n"),  _T("detailedLineType"), (int)vecBusCost.size(), (int)vecRailCost.size());
		of.WriteString(strOut);

		for (size_t i= 0; i< vecBusCost.size(); i++)
		{
			TTransitCost &oTBusCost = vecBusCost[i];
			strOut.Format(_T("buscost\t%d\t%.0f\t%.0f\t%.0f\t%.0f\t%d\r\n"), oTBusCost.nTType, oTBusCost.dTbasicRate, oTBusCost.dTbasicDistance, oTBusCost.dTAddRate, oTBusCost.dTAddDistance, oTBusCost.nTMergeRate);
			of.WriteString(strOut);
		}

		for (size_t i= 0; i< vecRailCost.size(); i++)
		{
			TTransitCost &oTRailCost = vecRailCost[i];
			strOut.Format(_T("railcost\t%d\t%.0f\t%.0f\t%.0f\t%.0f\t%d\r\n"), oTRailCost.nTType, oTRailCost.dTbasicRate, oTRailCost.dTbasicDistance, oTRailCost.dTAddRate, oTRailCost.dTAddDistance, oTRailCost.nTMergeRate);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

void KDBaseDefineTransitCost::UpdateTransitInfoMode( KTarget* a_pTarget, std::vector<TTransitCost> a_vecTransitCost )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{
		int nBus(0);
		int nRail(0);

		CString strBus(_T(""));
		CString strRail(_T(""));

		size_t nCount = a_vecTransitCost.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TTransitCost oTransitCost = a_vecTransitCost[i];
			if (TRANSIT_MODE_BUS == oTransitCost.nMode)
			{
				if (0 == nBus)
				{
					strBus.AppendFormat(_T(" %d "), oTransitCost.nTType);
				}
				else
				{
					strBus.AppendFormat(_T(", %d "), oTransitCost.nTType);
				}
				nBus++;
			}
			else if (TRANSIT_MODE_RAIL == oTransitCost.nMode)
			{
				if (0 == nRail)
				{
					strRail.AppendFormat(_T(" %d "), oTransitCost.nTType);
				}
				else
				{
					strRail.AppendFormat(_T(", %d "), oTransitCost.nTType);
				}
				nRail++;
			}
			
		}

		if (nBus != 0)
		{
			CString strBusSQL = _T("");
			strBusSQL.Format(_T("Update %s Set type = %d where detail_type In (%s) "), TABLE_TRANSIT, TRANSIT_MODE_BUS, strBus);
			spDBaseConnection->ExecuteUpdate(strBusSQL);
		}
		
		if (nRail != 0)
		{
			CString strRailSQL = _T("");
			strRailSQL.Format(_T("Update %s Set type = %d where detail_type In (%s) "), TABLE_TRANSIT, TRANSIT_MODE_RAIL, strRail);
			spDBaseConnection->ExecuteUpdate(strRailSQL);
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

void KDBaseDefineTransitCost::GetGeneratorParameter( KTarget* a_pTarget, TAssignRule& a_oUrbanInfo, TAssignRule& a_oRegionalInfo )
{
	CString strSQL(_T(""));
	try
	{
		KDBaseConPtr spDBaseConnetion = a_pTarget->GetDBaseConnection();

		strSQL.Format(_T(" Select taxi_time, bus_time, subway_time, constaint_time, constaint_length, invehicle_time,  "));
		strSQL.AppendFormat(_T(" waiting_time, transfer_time, ae_time, use_turn, inter_vot From %s where type = %d "), TABLE_INTER_TIME_INFO, INTER_URBAN_CODE);
		KResultSetPtr spResultSet = spDBaseConnetion->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			a_oUrbanInfo.dDWTimeTaxi		= spResultSet->GetValueDouble(0);
			a_oUrbanInfo.dDWTimeBusstop		= spResultSet->GetValueDouble(1);
			a_oUrbanInfo.dDwTimeRailStation = spResultSet->GetValueDouble(2);
			a_oUrbanInfo.dFirstPathTime		= spResultSet->GetValueDouble(3);
			a_oUrbanInfo.dSecondePathLength = spResultSet->GetValueDouble(4);
			a_oUrbanInfo.nInvehicle			= spResultSet->GetValueInt(5);
			a_oUrbanInfo.nWating			= spResultSet->GetValueInt(6);
			a_oUrbanInfo.nTransfer			= spResultSet->GetValueInt(7);
			a_oUrbanInfo.nAETime			= spResultSet->GetValueInt(8);
			a_oUrbanInfo.nUseTurn			= spResultSet->GetValueInt(9);
			a_oUrbanInfo.dInterValueofTime	= spResultSet->GetValueDouble(10);
		}

		strSQL.Format(_T(" Select constaint_time, constaint_length, invehicle_time, waiting_time, transfer_time, ae_time From %s Where type = %d "), 
						TABLE_INTER_TIME_INFO, INTER_REGIONAL_CODE);
		spResultSet = spDBaseConnetion->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			a_oRegionalInfo.dFirstPathTime		= spResultSet->GetValueDouble(0);
			a_oRegionalInfo.dSecondePathLength	= spResultSet->GetValueDouble(1);
			a_oRegionalInfo.nInvehicle			= spResultSet->GetValueInt(2);
			a_oRegionalInfo.nWating				= spResultSet->GetValueInt(3);
			a_oRegionalInfo.nTransfer			= spResultSet->GetValueInt(4);
			a_oRegionalInfo.nAETime				= spResultSet->GetValueInt(5);
			a_oRegionalInfo.nUseTurn			= spResultSet->GetValueInt(6);
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
