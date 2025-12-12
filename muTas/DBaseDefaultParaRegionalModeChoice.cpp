#include "StdAfx.h"
#include "DBaseDefaultParaRegionalModeChoice.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"
#include "DBaseInterModal.h"

KDBaseDefaultParaRegionalModeChoice::KDBaseDefaultParaRegionalModeChoice(void)
{
}


KDBaseDefaultParaRegionalModeChoice::~KDBaseDefaultParaRegionalModeChoice(void)
{
}

void KDBaseDefaultParaRegionalModeChoice::LoadDefaultRegionalModeChoiceMEU( std::map<int, double>& ar_mapMEU )
{
	ar_mapMEU.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strQuery;
		strQuery.Format(_T("select Master_Code, MEU_Para from %s"),
			KDefaultParaTable::TABLE_DEFAULT_REGIONAL_MODECHOICE_MEU);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int    nMasterCD = spResultSet->GetValueInt(0);
			double dMEU      = spResultSet->GetValueDouble(1);

			ar_mapMEU.insert(std::make_pair(nMasterCD, dMEU));
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
		throw -1;
	}
}

void KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceMEU( int a_nMasterCD, double a_dMEU )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strQery;
		strQery.Format(_T("Insert or Replace Into %s(Master_Code, MEU_Para) Values(%d ,%f)"),
			KDefaultParaTable::TABLE_DEFAULT_REGIONAL_MODECHOICE_MEU, a_nMasterCD, a_dMEU);

		TxLogDebug(strQery.AllocSysString());

		spDBaseConnection->ExecuteUpdate(strQery);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}

void KDBaseDefaultParaRegionalModeChoice::LoadDefaultRegionalModeChoiceParam( int a_nPurPoseID, int a_nAlternativeID,
	std::map<int/*Master_ID*/, TRegionalParameter>& a_mapRegionalChoiceValue )
{
	a_mapRegionalChoiceValue.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		CString strQuery     (_T("Select Master_Code, InVehecle_Time, Waiting_Time, Transfer_Time, Process_Time, Frequency, Cost, Dummy"));
		strQuery.AppendFormat(_T(" From %s"), KDefaultParaTable::TABLE_DEFAULT_REGIONAL_MODECHOICE_PARA);
		strQuery.AppendFormat(_T(" Where Object_id = %d And Alternative_ID = %d Order by Master_Code, Alternative_ID"), a_nPurPoseID, a_nAlternativeID);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int nMasterID = spResultSet->GetValueInt(0);

			TRegionalParameter oRegionalParameter = {};
			oRegionalParameter.TdInvehicleTime  = spResultSet->GetValueDouble(1);
			oRegionalParameter.TdWaitingTime		= spResultSet->GetValueDouble(2);
			oRegionalParameter.TdTransferTime	= spResultSet->GetValueDouble(3);
			oRegionalParameter.TdProcessTime	= spResultSet->GetValueDouble(4);
			oRegionalParameter.TdFrequency		= spResultSet->GetValueDouble(5);
			oRegionalParameter.TdCost			= spResultSet->GetValueDouble(6);
			oRegionalParameter.TdDummy			= spResultSet->GetValueDouble(7);

			a_mapRegionalChoiceValue.insert(std::make_pair(nMasterID, oRegionalParameter));
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
		throw -1;
	}
}

void KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceParam( 
	int a_nMasterCD, KRegionalChoiceParameter& oRegionalChoiceParameter )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strQuery(_T(""));
		strQuery.AppendFormat(_T("Insert or Replace Into %s(Master_Code, Object_ID, Alternative_ID, InVehecle_Time, Waiting_Time, Transfer_Time, Process_Time, Frequency, Cost, Dummy)"), 
			KDefaultParaTable::TABLE_DEFAULT_REGIONAL_MODECHOICE_PARA);
		strQuery.AppendFormat(_T(" Values(?, ?, ?, ?, ?, ?, ?, ?, ? ,? )"));

		KPreparedStatementPtr spPreparedStatementPtr = spDBaseConnection->PrepareStatement(strQuery);
		
		std::map<int, TRegionalParameter> mapParam = oRegionalChoiceParameter.mapModeRegionalChoiceValue;
		KPurpose* pPurpose = oRegionalChoiceParameter.pPurpose;

		AutoType bIter = mapParam.begin();
		AutoType eIter = mapParam.end();

		while (bIter != eIter)
		{
			TRegionalParameter oRegionalParameter = bIter->second;
			
			spPreparedStatementPtr->BindInt   (1, a_nMasterCD);
			spPreparedStatementPtr->BindInt   (2, pPurpose->PurposeID());
			spPreparedStatementPtr->BindInt   (3, oRegionalParameter.TnModeCode);
			spPreparedStatementPtr->BindDouble(4, oRegionalParameter.TdInvehicleTime);
			spPreparedStatementPtr->BindDouble(5, oRegionalParameter.TdWaitingTime);
			spPreparedStatementPtr->BindDouble(6, oRegionalParameter.TdTransferTime);
			spPreparedStatementPtr->BindDouble(7, oRegionalParameter.TdProcessTime);
			spPreparedStatementPtr->BindDouble(8, oRegionalParameter.TdFrequency);
			spPreparedStatementPtr->BindDouble(9, oRegionalParameter.TdCost);
			spPreparedStatementPtr->BindDouble(10, oRegionalParameter.TdDummy);

			spPreparedStatementPtr->ExecuteUpdate();
			spPreparedStatementPtr->Reset();
			
			++bIter;
		}

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		throw -1;
	}
}
