#include "StdAfx.h"
#include "DBaseDefaultParaInterUrbanModeChoice.h"
#include "DBaseConnector.h"
#include "KDefInterUrbanModeChoiceVariableSettingDlg.h"


KDBaseDefaultParaInterUrbanModeChoice::KDBaseDefaultParaInterUrbanModeChoice(void)
{
}


KDBaseDefaultParaInterUrbanModeChoice::~KDBaseDefaultParaInterUrbanModeChoice(void)
{
}


int KDBaseDefaultParaInterUrbanModeChoice::MaxMasterCodeModeChlcePara()
{
	try
	{
		CString strSQL(_T(""));	
		strSQL.Format(_T(" select MAX(Master_Code), count(*) from Default_Intermodal_Urban_ModeChoice_Para_v1001 "));

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResult->Next())
		{
			int nMaxMasterCode = spResult->GetValueInt(0);
			int nRecordCnt     = spResult->GetValueInt(1);

			if (nRecordCnt > 0)
			{
				return nMaxMasterCode;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return -1;
}


int KDBaseDefaultParaInterUrbanModeChoice::MaxMasterCodeModeChlceMEU()
{
	try
	{
		CString strSQL(_T(""));	
		strSQL.Format(_T(" select MAX(Master_Code), count(*) from Default_Intermodal_Urban_ModeChoice_MEU "));

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		if (spResult->Next())
		{
			int nMaxMasterCode = spResult->GetValueInt(0);
			int nRecordCnt     = spResult->GetValueInt(1);

			if (nRecordCnt > 0)
			{
				return nMaxMasterCode;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return -1;
}


void KDBaseDefaultParaInterUrbanModeChoice::LoadDefaultModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara)
{
	try
	{
		a_mapDefaultPara.clear();

		int nMaxMasterCode = MaxMasterCodeModeChlcePara();
		if (nMaxMasterCode < 0)
		{
			return;
		}

		CString strSQL(_T(""));
#pragma region 쿼리 조합
		strSQL.Append(_T(" SELECT "));
		strSQL.Append(_T(" Master_Code, "));
		strSQL.Append(_T(" Object_ID, "));
		strSQL.Append(_T(" Cofficient_ID, "));
		strSQL.Append(_T(" Auto,  "));
		strSQL.Append(_T(" Taxi, "));
		strSQL.Append(_T(" Highway_Transit,  "));
		strSQL.Append(_T(" Rail_Transit, "));
		strSQL.Append(_T(" Composite_Mode,  "));
		strSQL.Append(_T(" Park_Ride, "));
		strSQL.Append(_T(" Kiss_Ride, "));
		// 2019.04.04 수정
		strSQL.Append(_T(" Taxi_Transit, "));
		strSQL.Append(_T(" ObjName, "));
		strSQL.Append(_T(" ObjNameEng "));
		// 2019.04.04 수정 끝
		strSQL.AppendFormat(_T(" FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 where Master_Code = %d "), nMaxMasterCode);
#pragma endregion 쿼리 조합

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			TUrbanChoiceValues oTUrbanChoiceValues;

			int nMasterCode				         = spResult->GetValueInt    (0);
			int nObjectID                        = spResult->GetValueInt    (1);
			int nCoefficientID                   = spResult->GetValueInt    (2);

			oTUrbanChoiceValues.nCofficientCode  = nCoefficientID;
			oTUrbanChoiceValues.dAuto            = spResult->GetValueDouble (3);
			oTUrbanChoiceValues.dTaxi            = spResult->GetValueDouble (4);
			oTUrbanChoiceValues.dHighwayTransit  = spResult->GetValueDouble (5);
			oTUrbanChoiceValues.dRailTransit     = spResult->GetValueDouble (6);
			oTUrbanChoiceValues.dCompositMode    = spResult->GetValueDouble (7);
			oTUrbanChoiceValues.dParkAndRide     = spResult->GetValueDouble (8);
			oTUrbanChoiceValues.dKissAndRide     = spResult->GetValueDouble (9);
			oTUrbanChoiceValues.dTaxiAndTransit  = spResult->GetValueDouble (10);
			// 2019.04.04 수정 
			oTUrbanChoiceValues.strObjName = spResult->GetValueString(11);
			oTUrbanChoiceValues.strObjNameEng = spResult->GetValueString(12);
			// 2019.04.04 수정 끝


			AutoType iter = a_mapDefaultPara.find(nObjectID);
			if (iter != a_mapDefaultPara.end())
			{
				std::map<int, TUrbanChoiceValues> &mapUrbanChoiceValues = iter->second;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
			}
			else
			{
				std::map<int, TUrbanChoiceValues> mapUrbanChoiceValues;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));

				a_mapDefaultPara.insert    (std::make_pair(nObjectID, mapUrbanChoiceValues));
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapDefaultPara.clear();
	}
}


void KDBaseDefaultParaInterUrbanModeChoice::LoadDefaultModeChoiceParaByObject(int a_nObjectID, std::map<int, TUrbanChoiceValues> &a_mapUrbanChoiceValues)
{
	try
	{
		a_mapUrbanChoiceValues.clear();

		int nMaxMasterCode = MaxMasterCodeModeChlcePara();
		if (nMaxMasterCode < 0)
		{
			return;
		}

		CString strSQL(_T(""));
#pragma region 쿼리 조합
		strSQL.Append(_T(" SELECT "));
		strSQL.Append(_T(" Master_Code, "));
		strSQL.Append(_T(" Object_ID, "));
		strSQL.Append(_T(" Cofficient_ID, "));
		strSQL.Append(_T(" Auto,  "));
		strSQL.Append(_T(" Taxi, "));
		strSQL.Append(_T(" Highway_Transit,  "));
		strSQL.Append(_T(" Rail_Transit, "));
		strSQL.Append(_T(" Composite_Mode,  "));
		strSQL.Append(_T(" Park_Ride, "));
		strSQL.Append(_T(" Kiss_Ride, "));
		strSQL.Append(_T(" Taxi_Transit, "));
		// 2019.04.04 수정
		strSQL.Append(_T(" ObjName, "));
		strSQL.Append(_T(" ObjNameEng "));
		// 2019.04.04 수정 끝
		strSQL.AppendFormat(_T(" FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 where Master_Code = %d AND  Object_ID = %d "), nMaxMasterCode, a_nObjectID);
#pragma endregion 쿼리 조합

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			TUrbanChoiceValues oTUrbanChoiceValues;

			int nMasterCode				         = spResult->GetValueInt    (0);
			int nObjectID                        = spResult->GetValueInt    (1);
			int nCoefficientID                   = spResult->GetValueInt    (2);

			oTUrbanChoiceValues.nCofficientCode  = nCoefficientID;
			oTUrbanChoiceValues.dAuto            = spResult->GetValueDouble (3);
			oTUrbanChoiceValues.dTaxi            = spResult->GetValueDouble (4);
			oTUrbanChoiceValues.dHighwayTransit  = spResult->GetValueDouble (5);
			oTUrbanChoiceValues.dRailTransit     = spResult->GetValueDouble (6);
			oTUrbanChoiceValues.dCompositMode    = spResult->GetValueDouble (7);
			oTUrbanChoiceValues.dParkAndRide     = spResult->GetValueDouble (8);
			oTUrbanChoiceValues.dKissAndRide     = spResult->GetValueDouble (9);
			oTUrbanChoiceValues.dTaxiAndTransit  = spResult->GetValueDouble (10);

			// 2019.04.04 수정
			oTUrbanChoiceValues.strObjName = spResult->GetValueString(11);
			oTUrbanChoiceValues.strObjNameEng = spResult->GetValueString(12);
			// 2019.04.04 수정 끝

			a_mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapUrbanChoiceValues.clear();
	}
}


void KDBaseDefaultParaInterUrbanModeChoice::DeleteInsertModeChoicePara( int a_nMasterCode, std::map<int, std::map<int, TUrbanChoiceValues>> &a_mapDefaultPara )
{
	KDBaseConPtr   spDBaseConnection  = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQLDel(_T(""));
		strSQLDel.AppendFormat(_T(" DELETE FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 WHERE Master_Code = %d "), a_nMasterCode);

		CString strSQL(_T(""));
#pragma region 쿼리 조합
		strSQL.Append(_T(" Insert Into Default_Intermodal_Urban_ModeChoice_Para_v1001 "));
		strSQL.Append(_T(" ( Master_Code, "));
		strSQL.Append(_T(" Object_ID, "));
		strSQL.Append(_T(" Cofficient_ID, "));
		strSQL.Append(_T(" Auto,  "));
		strSQL.Append(_T(" Taxi, "));
		strSQL.Append(_T(" Highway_Transit,  "));
		strSQL.Append(_T(" Rail_Transit, "));
		strSQL.Append(_T(" Composite_Mode,  "));
		strSQL.Append(_T(" Park_Ride, "));
		strSQL.Append(_T(" Kiss_Ride, "));
		// 2019.04.04 수정
		strSQL.Append(_T(" Taxi_Transit, "));
		strSQL.Append(_T(" ObjName, "));
		strSQL.Append(_T(" ObjNameEng ) "));
		strSQL.Append(_T(" Values ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? ) "));
		// 2019.04.04 수정 끝
#pragma endregion 쿼리 조합

		KDBaseConPtr   spDBaseConnection  = KDBase::GetProjectDBConnection();

		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQLDel);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		AutoType iter  = a_mapDefaultPara.begin();
		AutoType itEnd = a_mapDefaultPara.end();

		while (iter != itEnd)
		{
			int nObjectID   = iter->first;
			std::map<int, TUrbanChoiceValues> &mapUrbanChoiceValues = iter->second;

			AutoType iterB  = mapUrbanChoiceValues.begin();
			AutoType itEndB = mapUrbanChoiceValues.end();

			while (iterB != itEndB)
			{ 
				int                nCoefficientID       = iterB->first;
				TUrbanChoiceValues &oTUrbanChoiceValues = iterB->second;

				spPrepareStatement->BindInt   (1, a_nMasterCode);
				spPrepareStatement->BindInt   (2, nObjectID);
				spPrepareStatement->BindInt   (3, nCoefficientID);
				spPrepareStatement->BindDouble(4, oTUrbanChoiceValues.dAuto);
				spPrepareStatement->BindDouble(5, oTUrbanChoiceValues.dTaxi);
				spPrepareStatement->BindDouble(6, oTUrbanChoiceValues.dHighwayTransit);
				spPrepareStatement->BindDouble(7, oTUrbanChoiceValues.dRailTransit);
				spPrepareStatement->BindDouble(8, oTUrbanChoiceValues.dCompositMode);
				spPrepareStatement->BindDouble(9, oTUrbanChoiceValues.dParkAndRide);
				spPrepareStatement->BindDouble(10, oTUrbanChoiceValues.dKissAndRide);
				spPrepareStatement->BindDouble(11, oTUrbanChoiceValues.dTaxiAndTransit);
				// 2019.04.04 수정
				spPrepareStatement->BindText(12, oTUrbanChoiceValues.strObjName);
				spPrepareStatement->BindText(13, oTUrbanChoiceValues.strObjNameEng);
				// 2019.04.04 수정 끝

				spPrepareStatement->ExecuteUpdate();
				spPrepareStatement->Reset();

				++iterB;
			}

			++iter;
		}

		spDBaseConnection->Commit();
	}
	catch (...)
	{
		TxLogDebugException();	
		spDBaseConnection->RollBack();
	}
}


void KDBaseDefaultParaInterUrbanModeChoice::LoadDefaultModeChoiceMEU( double &a_dMeuParameter )
{
	try
	{
		int nMaxMasterCode = MaxMasterCodeModeChlceMEU();
		if (nMaxMasterCode < 0)
		{
			return;
		}

		CString strSQL(_T(""));
#pragma region 쿼리 조합
		strSQL.Append(_T(" SELECT "));
		strSQL.Append(_T(" Master_Code, "));
		strSQL.Append(_T(" MEU_Para "));
		strSQL.AppendFormat(_T(" FROM Default_Intermodal_Urban_ModeChoice_MEU where Master_Code = %d "), nMaxMasterCode);
#pragma endregion 쿼리 조합

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		while( spResult->Next() )
		{
			int nMasterCode	                   = spResult->GetValueInt    (0);
			a_dMeuParameter                    = spResult->GetValueDouble (1);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDBaseDefaultParaInterUrbanModeChoice::DeleteInsertModeChoiceMEU( int a_nMasterCode, double &a_dMeuParameter )
{
	KDBaseConPtr   spDBaseConnection  = KDBase::GetProjectDBConnection();

	try
	{
		CString strSQLDel(_T(""));
		strSQLDel.AppendFormat(_T(" DELETE FROM Default_Intermodal_Urban_ModeChoice_MEU WHERE Master_Code = %d "), a_nMasterCode);

		CString strSQL(_T(""));
		strSQL.Append(_T(" Insert Into Default_Intermodal_Urban_ModeChoice_MEU "));
		strSQL.Append(_T(" ( Master_Code, "));
		strSQL.Append(_T(" MEU_Para ) "));
		strSQL.AppendFormat(_T(" Values ( %d, %f ) "), a_nMasterCode, a_dMeuParameter);

		KDBaseConPtr   spDBaseConnection  = KDBase::GetProjectDBConnection();

		spDBaseConnection->BeginTransaction();
		spDBaseConnection->ExecuteUpdate(strSQLDel);
		spDBaseConnection->ExecuteUpdate(strSQL);

		spDBaseConnection->Commit();
	}
	catch (...)
	{
		TxLogDebugException();	
		spDBaseConnection->RollBack();
	}
}




void KDBaseDefaultParaInterUrbanModeChoice::ReadVariableInfo(std::map<int, TDescriptionVariableInfo>& mapVariableInfo)
{
	mapVariableInfo.clear();

	try
	{
		KDBaseConPtr spDBConnection = KDBase::GetProjectDBConnection();
		if (nullptr == spDBConnection)
		{
			return;
		}

		CString strSQL = _T("SELECT ID, VarType, Selected,  VarTypeName, VarName, VarUnit, VarDescription, "
			"		VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, InitSelect"
			" FROM UrbanModeChoiceVariable "
			" ORDER BY VarType, ID;");

		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
		if (nullptr == spResultset)
		{
			return;
		}

		while (spResultset->Next())
		{
			TDescriptionVariableInfo info;
			info.nId = spResultset->GetValueInt(0);
			info.nType = spResultset->GetValueInt(1);
			info.bSelected = spResultset->GetValueInt(2) == 1 ? true : false;
			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				info.strTypeName = spResultset->GetValueString(3);
				info.strVariableName = spResultset->GetValueString(4);
				info.strVariableUnit = spResultset->GetValueString(5);
				info.strDescript = spResultset->GetValueString(6);
			}
			else
			{
				info.strTypeName = spResultset->GetValueString(7);
				info.strVariableName = spResultset->GetValueString(8);
				info.strVariableUnit = spResultset->GetValueString(9);
				info.strDescript = spResultset->GetValueString(10);
			}
			info.bDefault = spResultset->GetValueInt(11) == 1 ? true : false;

			mapVariableInfo.insert({ info.nId, info });

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
}


void KDBaseDefaultParaInterUrbanModeChoice::ReadZoneVariableInfo(std::map<int, TZoneVariableInfo>& mapZoneVariableInfo)
{
	mapZoneVariableInfo.clear();

	try
	{
		KDBaseConPtr spDBConnection = KDBase::GetProjectDBConnection();
		if (nullptr == spDBConnection)
		{
			return;
		}

		CString strSQL = _T("SELECT ID, VarType, Selected, VarTypeName, VarName, VarTypeNameEng, VarNameEng, "
			"		ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription "
			" FROM UrbanModeChoiceVariableZone "
			" ORDER BY VarType, ID;");
		//CString strSQL = _T("SELECT ID, VarType, Selected, VarTypeName, VarName, VarTypeNameEng, VarNameEng, "
		//	"		ZoneColDispName, ZoneColDataType "
		//	" FROM UrbanModeChoiceVariableZone "
		//	" ORDER BY VarType, ID;");

		KResultSetPtr spResultset = spDBConnection->ExecuteQuery(strSQL);
		if (nullptr == spResultset)
		{
			return;
		}

		while (spResultset->Next())
		{
			TZoneVariableInfo info;
			info.nId = spResultset->GetValueInt(0);
			info.nType = spResultset->GetValueInt(1);
			info.bSelected = spResultset->GetValueInt(2) == 1 ? true : false;

			if (KmzSystem::GetLanguage() == KEMKorea)
			{
				info.strTypeName = spResultset->GetValueString(3);
				info.strVariableName = spResultset->GetValueString(4);
			}
			else
			{
				info.strTypeName = spResultset->GetValueString(5);
				info.strVariableName = spResultset->GetValueString(6);
			}

			info.strZoneColName = spResultset->GetValueString(7);
			info.strZoneColDispName = spResultset->GetValueString(8);
			if (info.strZoneColDispName.IsEmpty())
			{
				info.strZoneColDispName = _T("-");
			}
			info.nZoneColDataType = spResultset->GetValueInt(9);
			info.nZoneColCodeType = spResultset->GetValueInt(10);
			info.strZoneColDesc = spResultset->GetValueString(11);
			//info.strZoneColDispName = spResultset->GetValueString(7);
			//info.nZoneColDataType = spResultset->GetValueInt(8);

			mapZoneVariableInfo.insert({ info.nId, info });

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
}


bool KDBaseDefaultParaInterUrbanModeChoice::UpdateZoneVariableInfoUnselected(KDBaseConPtr spDBConn, std::vector<int>& arrId)
{
	if (arrId.empty())
	{
		return true;
	}

	if (nullptr == spDBConn)
	{
		return false;
	}

	CString strIds;
	for (int nId : arrId)
	{
		CString strTemp;
		strTemp.Format(_T("%d, "), nId);
		strIds += strTemp;
	}

	if (strIds.GetLength() > 2)
	{
		strIds.Delete(strIds.GetLength() - 2, 2);
	}

	CString strSQL = _T("UPDATE UrbanModeChoiceVariableZone "
		" SET Selected = 0, ZoneColName = '', ZoneColDispName = '', ZoneColDataType = 0, ZoneColCodeType = -1, ZoneColDescription = '-' "
		" WHERE ID IN (") + strIds + _T(")");
	//CString strSQL = _T("UPDATE UrbanModeChoiceVariableZone "
	//	" SET Selected = 0, ZoneColDispName = '', ZoneColDataType = 0 "
	//	" WHERE ID IN (") + strIds + _T(")");

	try
	{
		spDBConn->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KDBaseDefaultParaInterUrbanModeChoice::DeleteUrbanModeChoicePara(KDBaseConPtr spDBConn, std::vector<int>& arrId)
{
	if (arrId.empty())
	{
		return true;
	}

	if (nullptr == spDBConn)
	{
		return false;
	}

	CString strIds;
	for (int nId : arrId)
	{
		CString strTemp;
		strTemp.Format(_T("%d, "), nId);
		strIds += strTemp;
	}

	if (strIds.GetLength() > 2)
	{
		strIds.Delete(strIds.GetLength() - 2, 2);
	}

	CString strSQL = _T("DELETE FROM  Default_Intermodal_Urban_ModeChoice_Para_v1001 "
		" WHERE Cofficient_ID IN (") + strIds + _T(")");

	try
	{
		spDBConn->ExecuteUpdate(strSQL);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


bool KDBaseDefaultParaInterUrbanModeChoice::UpdateZoneVariableInfo(KDBaseConPtr spDBConn, std::vector<TZoneVariableInfo>& arrZoneVar)
{

	if (arrZoneVar.empty())
	{
		return true;
	}

	if (nullptr == spDBConn)
	{
		return false;
	}

	
	CString strSQLUpdate = _T("UPDATE UrbanModeChoiceVariableZone "
		" SET Selected = ?, ZoneColName = ?, ZoneColDispName = ?, ZoneColDataType = ?, ZoneColCodeType = ?, ZoneColDescription = ? "
		" WHERE ID = ?");
	//CString strSQLUpdate = _T("UPDATE UrbanModeChoiceVariableZone "
	//	" SET Selected = ?, ZoneColDispName = ?, ZoneColDataType = ? "
	//	" WHERE ID = ?");

	try
	{
		KPreparedStatementPtr spPrepared = spDBConn->PrepareStatement(strSQLUpdate);

		for (TZoneVariableInfo& zv : arrZoneVar)
		{
			spPrepared->BindInt(1, (zv.bSelected ? 1 : 0));
			spPrepared->BindText(2, zv.strZoneColName);
			spPrepared->BindText(3, zv.strZoneColDispName);
			spPrepared->BindInt(4, zv.nZoneColDataType);
			spPrepared->BindInt(5, zv.nZoneColCodeType);
			spPrepared->BindText(6, zv.strZoneColDesc);
			spPrepared->BindInt(7, zv.nId);
			//spPrepared->BindInt(1, (zv.bSelected ? 1 : 0));
			//spPrepared->BindText(2, zv.strZoneColDispName);
			//spPrepared->BindInt(3, zv.nZoneColDataType);
			//spPrepared->BindInt(4, zv.nId);

			spPrepared->ExecuteUpdate();
			spPrepared->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		return false;
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}


void KDBaseDefaultParaInterUrbanModeChoice::ReadDefaultModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &mapPara)
{
	mapPara.clear();
	try
	{
		CString strSQL = _T(
			" SELECT Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, "
			"		 Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, "
			"		 ObjName, ObjNameEng "		
			" FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 "
			" WHERE  Master_Code = 0 "
			" AND Cofficient_ID IN(SELECT id FROM UrbanModeChoiceVariable WHERE InitSelect = 1)"
		);

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			TUrbanChoiceValues oTUrbanChoiceValues;

			int nMasterCode = spResult->GetValueInt(0);
			int nObjectID = spResult->GetValueInt(1);
			int nCoefficientID = spResult->GetValueInt(2);

			oTUrbanChoiceValues.nCofficientCode = nCoefficientID;
			oTUrbanChoiceValues.dAuto = spResult->GetValueDouble(3);
			oTUrbanChoiceValues.dTaxi = spResult->GetValueDouble(4);
			oTUrbanChoiceValues.dHighwayTransit = spResult->GetValueDouble(5);
			oTUrbanChoiceValues.dRailTransit = spResult->GetValueDouble(6);
			oTUrbanChoiceValues.dCompositMode = spResult->GetValueDouble(7);
			oTUrbanChoiceValues.dParkAndRide = spResult->GetValueDouble(8);
			oTUrbanChoiceValues.dKissAndRide = spResult->GetValueDouble(9);
			oTUrbanChoiceValues.dTaxiAndTransit = spResult->GetValueDouble(10);
			oTUrbanChoiceValues.strObjName = spResult->GetValueString(11);
			oTUrbanChoiceValues.strObjNameEng = spResult->GetValueString(12);

			AutoType iter = mapPara.find(nObjectID);
			if (iter != mapPara.end())
			{
				std::map<int, TUrbanChoiceValues> &mapUrbanChoiceValues = iter->second;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
			}
			else
			{
				std::map<int, TUrbanChoiceValues> mapUrbanChoiceValues;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));

				mapPara.insert(std::make_pair(nObjectID, mapUrbanChoiceValues));
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		mapPara.clear();
	}
}


void KDBaseDefaultParaInterUrbanModeChoice::ReadCurrentModeChoicePara(std::map<int, std::map<int, TUrbanChoiceValues>> &mapPara)
{
	mapPara.clear();

	try
	{
		int nMaxMasterCode = MaxMasterCodeModeChlcePara();
		if (nMaxMasterCode < 0)
		{
			return;
		}

		CString strSQL;
		strSQL.Format(_T(
			" SELECT Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, "
			"		 Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, "
			"		 ObjName, ObjNameEng "
			" FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 "
			" WHERE  Master_Code = %d"
		), nMaxMasterCode);

		KDBaseConPtr  spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr        spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			TUrbanChoiceValues oTUrbanChoiceValues;

			int nMasterCode = spResult->GetValueInt(0);
			int nObjectID = spResult->GetValueInt(1);
			int nCoefficientID = spResult->GetValueInt(2);

			oTUrbanChoiceValues.nCofficientCode = nCoefficientID;
			oTUrbanChoiceValues.dAuto = spResult->GetValueDouble(3);
			oTUrbanChoiceValues.dTaxi = spResult->GetValueDouble(4);
			oTUrbanChoiceValues.dHighwayTransit = spResult->GetValueDouble(5);
			oTUrbanChoiceValues.dRailTransit = spResult->GetValueDouble(6);
			oTUrbanChoiceValues.dCompositMode = spResult->GetValueDouble(7);
			oTUrbanChoiceValues.dParkAndRide = spResult->GetValueDouble(8);
			oTUrbanChoiceValues.dKissAndRide = spResult->GetValueDouble(9);
			oTUrbanChoiceValues.dTaxiAndTransit = spResult->GetValueDouble(10);
			oTUrbanChoiceValues.strObjName = spResult->GetValueString(11);
			oTUrbanChoiceValues.strObjNameEng = spResult->GetValueString(12);

			AutoType iter = mapPara.find(nObjectID);
			if (iter != mapPara.end())
			{
				std::map<int, TUrbanChoiceValues> &mapUrbanChoiceValues = iter->second;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));
			}
			else
			{
				std::map<int, TUrbanChoiceValues> mapUrbanChoiceValues;
				mapUrbanChoiceValues.insert(std::make_pair(nCoefficientID, oTUrbanChoiceValues));

				mapPara.insert(std::make_pair(nObjectID, mapUrbanChoiceValues));
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		mapPara.clear();
	}
}