#include "StdAfx.h"
#include "DBaseDefaultCo2EmissionsInterpolation.h"

#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

KDBaseDefaultCo2EmissionsInterpolation::KDBaseDefaultCo2EmissionsInterpolation(void)
{
}


KDBaseDefaultCo2EmissionsInterpolation::~KDBaseDefaultCo2EmissionsInterpolation(void)
{
}

void KDBaseDefaultCo2EmissionsInterpolation::SeperateBySemicolon( CString a_strValue, std::vector<double>& a_vecData )
{
	a_vecData.clear();

	try
	{
		CString strTok;
		int     nTokenPos = 0;

		while (AfxExtractSubString(strTok, a_strValue, nTokenPos++, _T(';')))
		{
			a_vecData.push_back(_ttof(strTok));
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

void KDBaseDefaultCo2EmissionsInterpolation::VectorConvertString( std::vector<double> a_vecData, CString& a_strValue )
{
	a_strValue = _T("");

	try
	{
		int nSize = a_vecData.size();

		for (int i = 0; i < nSize; ++i)
		{
			if (i != 0)
				a_strValue.Append(_T(";"));

			a_strValue.AppendFormat(_T("%f"), a_vecData[i]);
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

void KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationInfo( 
	std::map<int, KCO2InterpolationSetting>& a_mapCO2InterpolationInfo )
{
	a_mapCO2InterpolationInfo.clear();

	try
	{
		CString strQuery = _T("SELECT master_code, max_speed, class_interval FROM Default_CO2_Interpolation_Info");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		KCO2InterpolationSetting oCo2InterpolationSetting;

		while (spResultSet->Next())
		{
			int nMasterCD = spResultSet->GetValueInt(0);
			oCo2InterpolationSetting.nMaxSpeed = spResultSet->GetValueInt(1);
			oCo2InterpolationSetting.nInterval = spResultSet->GetValueInt(2);

			a_mapCO2InterpolationInfo.insert(std::make_pair(nMasterCD, oCo2InterpolationSetting));
		}

		if (a_mapCO2InterpolationInfo.size() == 0)
		{
			KCO2InterpolationSetting oSetting;

			oSetting.nMaxSpeed = 100;
			oSetting.nInterval = 10;

			a_mapCO2InterpolationInfo.insert(std::make_pair(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, oSetting));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		//TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			TxLogDebug(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		throw 1;
	}
}

void KDBaseDefaultCo2EmissionsInterpolation::GetDefaultCo2InterpolationData( 
	std::map<int, std::map<int, KCO2InterpolationData>>& a_mapCO2InterpolationData )
{
	a_mapCO2InterpolationData.clear();

	try
	{
		CString strQuery = _T("SELECT master_code, mode_seq, mode_name, pce, speed_value, is_selected FROM default_co2_interpolation_data ORDER BY master_code, mode_seq");

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		CString strValue;

		while (spResultSet->Next())
		{
			KCO2InterpolationData oData;

			oData.nMasterCode = spResultSet->GetValueInt(0);
			oData.nModeSeq    = spResultSet->GetValueInt(1);
			oData.strModeName = spResultSet->GetValueString(2);
			oData.dPCE        = spResultSet->GetValueDouble(3);
			strValue          = spResultSet->GetValueString(4);

			SeperateBySemicolon(strValue, oData.vecData);
			
			if (spResultSet->GetValueInt(5) == 0)
			{
				oData.bIsSelected = FALSE;
			}
			else
			{
				oData.bIsSelected = TRUE;
			}
			
			std::map<int, std::map<int, KCO2InterpolationData>>::iterator fIter = a_mapCO2InterpolationData.find(oData.nMasterCode);


			if (fIter == a_mapCO2InterpolationData.end())
			{
				std::map<int, KCO2InterpolationData> mapData;
				mapData.insert(std::make_pair(oData.nModeSeq, oData));

				a_mapCO2InterpolationData.insert(std::make_pair(oData.nMasterCode, mapData));
			}
			else
			{
				std::map<int, KCO2InterpolationData>& mapData = fIter->second;
				std::map<int, KCO2InterpolationData>::iterator fDataIter = mapData.find(oData.nModeSeq);

				if (fDataIter == mapData.end())
				{
					mapData.insert(std::make_pair(oData.nModeSeq, oData));
				}
				else
				{
					mapData.erase(oData.nModeSeq);
					mapData.insert(std::make_pair(oData.nModeSeq, oData));
				}
			}
		}

		if (a_mapCO2InterpolationData.size() == 0)
		{
			std::vector<double> vecData;
			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);

			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);
			vecData.push_back(0.0);

			vecData.push_back(0.0);

			std::map<int, KCO2InterpolationData> mapData;
			
			KCO2InterpolationData oData;

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 1;
			oData.strModeName = _T("PassengerCar");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("승용차");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 2;
			oData.strModeName = _T("SmallBus");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("소형버스");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 3;
			oData.strModeName = _T("MediumBus");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("중형버스");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 4;
			oData.strModeName = _T("LargeBus");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("대형버스");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 5;
			oData.strModeName = _T("SmallTruck");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("소형트럭");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 6;
			oData.strModeName = _T("MediumTruck");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("중형트럭");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));


			oData.nMasterCode = KDefaultParameterMasterCode::SYSTEM_MASTER_CODE;
			oData.nModeSeq    = 7;
			oData.strModeName = _T("LargeTruck");
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oData.strModeName = _T("대형트럭");
			}
			oData.dPCE        = 0.0;
			oData.bIsSelected = FALSE;
			oData.vecData     = vecData;

			mapData.insert(std::make_pair(oData.nModeSeq, oData));

			a_mapCO2InterpolationData.insert(std::make_pair(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, mapData));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		//TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			TxLogDebug(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		throw 1;
	}
}

void KDBaseDefaultCo2EmissionsInterpolation::SaveDefaultCo2Interpolation( 
	std::map<int, KCO2InterpolationSetting> a_mapCO2InterpolationInfo, std::map<int, std::map<int, KCO2InterpolationData>> a_mapCO2InterpolationData )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	CString strQuery, strValue;
	int     nValue;

	try
	{
		spDBaseConnection->BeginTransaction();
		
		//Insert Info
		std::map<int, KCO2InterpolationSetting>::iterator bInfoIter = a_mapCO2InterpolationInfo.begin();

		if (bInfoIter == a_mapCO2InterpolationInfo.end() || a_mapCO2InterpolationInfo.size() != 1)
			ThrowException(_T("Info 데이터 오류"));

		strQuery.Format(_T("DELETE FROM default_co2_interpolation_info WHERE master_code = %d"), bInfoIter->first);

		if (SQLITE_DONE != spDBaseConnection->ExecuteUpdate(strQuery))
			ThrowException(_T("데이터 삭제 오류-default_co2_interpolation_info"));

		strQuery.Format(_T("INSERT INTO default_co2_interpolation_info(master_code, max_speed, class_interval) VALUES(%d, %d, %d)"),
			bInfoIter->first, bInfoIter->second.nMaxSpeed, bInfoIter->second.nInterval);

		if (SQLITE_DONE != spDBaseConnection->ExecuteUpdate(strQuery))
			ThrowException(_T("데이터 입력 오류"));

		//Insert Data
		std::map<int, std::map<int, KCO2InterpolationData>>::iterator bDataIter = a_mapCO2InterpolationData.begin();

		if (bDataIter == a_mapCO2InterpolationData.end() || a_mapCO2InterpolationData.size() != 1)
			ThrowException(_T("Data 데이터 오류"));

		strQuery.Format(_T("DELETE FROM default_co2_interpolation_data WHERE master_code = %d"), bDataIter->first);

		if (SQLITE_DONE != spDBaseConnection->ExecuteUpdate(strQuery))
			ThrowException(_T("데이터 삭제 오류-default_co2_interpolation_data"));

		std::map<int, KCO2InterpolationData> mapData = bDataIter->second;
		std::map<int, KCO2InterpolationData>::iterator bMapIter = mapData.begin();

		while (bMapIter != mapData.end())
		{
			KCO2InterpolationData oData = bMapIter->second;
			VectorConvertString(oData.vecData, strValue);

			nValue = oData.bIsSelected ? 1 : 0;

			strQuery.Format(
				_T("INSERT INTO default_co2_interpolation_data(master_code, mode_seq, mode_name, pce, speed_value, is_selected) Values(%d, %d, '%s', %f, '%s', %d)"),
				bDataIter->first, bMapIter->first, oData.strModeName, oData.dPCE, strValue, 
				nValue);

			if (SQLITE_DONE != spDBaseConnection->ExecuteUpdate(strQuery))
				ThrowException(_T("데이터 입력 오류"));

			++bMapIter;
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
		//TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			TxLogDebug(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			TxLogDebug(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
		throw 1;
	}
}
