#include "stdafx.h"
#include "DBaseVersion_P1001.h"
#include "Project.h"
#include "Purpose.h"


const TCHAR* TABLE_NAME_URBANMODECHOICE = _T("UrbanModeChoiceVariable");
const TCHAR* TABLE_NAME_URBANMODECHOICE_ZONE = _T("UrbanModeChoiceVariableZone");

const TCHAR* TABLE_NAME_MULTI_ASSIGN_INFO_SOBA = _T("Multi_Assign_Info_Soba");
const TCHAR* TABLE_NAME_MULTI_ASSIGN_DATA_SOBA = _T("Multi_Assign_Data_Soba");




void KDBaseVersion_P1001::ProjectDBUpgrade(KProject* pProject, KDBaseConPtr spDBConn)
{
	try
	{
		if (!CheckTableExists(spDBConn, CString(TABLE_NAME_URBANMODECHOICE))
			|| !CheckTableExists(spDBConn, CString(TABLE_NAME_URBANMODECHOICE_ZONE)))
		{
			CreateTableUrbanModeChoiceVariable(spDBConn);
			CreateTableUrbanModeChoiceVariableZone(spDBConn);
			UpdateDefaultIntermodalUrbanModeChoicePara(pProject, spDBConn);
			UpdateSelectedVariable(spDBConn);
		}

		if (!CheckTableExists(spDBConn, CString(TABLE_NAME_MULTI_ASSIGN_INFO_SOBA))
			|| !CheckTableExists(spDBConn, CString(TABLE_NAME_MULTI_ASSIGN_DATA_SOBA)))
		{
			CreateTableMultiAssignInfoSoba(spDBConn);
			CreateTableMultiassignDataSoba(spDBConn);
		}
	
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

bool KDBaseVersion_P1001::CheckTableExists(KDBaseConPtr spDBConn, CString& strTableName)
{
	bool bExist = false;
	CString strSQL;
	strSQL.Format(_T(
		" SELECT name FROM sqlite_master "
		" WHERE type = 'table' "
		" AND name = '%s' "), strTableName);

	try
	{
		int nCount = 0;
		KResultSetPtr spResult = spDBConn->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
			bExist = true;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}

	return bExist;
}


void KDBaseVersion_P1001::CreateTableUrbanModeChoiceVariable(KDBaseConPtr spDBConn)
{
	try
	{
		CString strSQLDrop;
		strSQLDrop.Format(_T("DROP TABLE IF EXISTS %s "), TABLE_NAME_URBANMODECHOICE);
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		CString strSQLCreate;
		strSQLCreate.Format(_T(
			" CREATE TABLE `%s` ( "
			"	`ID`	INTEGER, "
			"	`VarType`	INTEGER, "
			"	`Selected`	INTEGER DEFAULT 0, "
			"	`VarTypeName`	TEXT, "
			"	`VarName`	TEXT, "
			"	`VarUnit`	TEXT, "
			"	`VarDescription`	TEXT, "
			"	`VarTypeNameEng`	TEXT, "
			"	`VarNameEng`	TEXT, "
			"	`VarUnitEng`	TEXT, "
			"	`VarDescriptionEng`	TEXT, "
			"	`InitSelect`	INTEGER, "
			"	PRIMARY KEY(`ID`) "
			"	) "), 
			TABLE_NAME_URBANMODECHOICE
		);

		spDBConn->ExecuteUpdate(strSQLCreate);

		// 초기 데이터 입력
		std::vector<CString> arrSQLInsert {
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(1, 1, '통행시간관련 수단특성변수', '총통행시간a', '분', '총통행시간a = 차내통행시간+환승시간+대기시간', 'Time-Related', 'Total-Travel-Time1', 'Minute', 'Total-Travel-Time1=Invehicle+Transfer+Waiting',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(2, 1, '통행시간관련 수단특성변수', '총통행시간b', '분', '총통행시간b = 차내통행시간+환승시간+대기시간+출발 도보접근시간+도착 도보접근시간', 'Time-Related', 'Total-Travel-Time2', 'Minute', 'Total-Travel-Time2=Invehicle+Transfer+Waiting+Access+Egress',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(3, 1, '통행시간관련 수단특성변수', '차내통행시간', '분', '차량 순 탑승시간', 'Time-Related', 'Invehicle Time', 'Minute', 'Invehicle Time',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(4, 1, '통행시간관련 수단특성변수', '환승시간', '분', '(대중교통 환승 도보시간) + (환승역에서의 대기시간)의 합', 'Time-Related', 'Transfer Time', 'Minute', 'Transfer Time',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(5, 1, '통행시간관련 수단특성변수', '대기시간', '분', '출발지 정류장(역)에서의 대중교통 대기시간', 'Time-Related', 'Waiting Time', 'Minute', 'Waiting Time',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(6, 1, '통행시간관련 수단특성변수', '출발 도보접근시간', '분', '출발지로부터 최초 탑승정류장(역)까지의 도보접근시간', 'Time-Related', 'Access Walking Time', 'Minute', 'Access Walking Time',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(7, 1, '통행시간관련 수단특성변수', '도착 도보접근시간', '분', '도착지 정류장(역)에서 최종 도착지까지의 도보접근시간', 'Time-Related', 'Egress Walking Time', 'Minute', 'Egress Walking Time',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(8, 1, '통행시간관련 수단특성변수', '차내접근시간', '분', '주수단을 이용하기 위해 기타수단으로 접근하는 시간', 'Time-Related', 'Access Egress Invehicle Time', 'Minute', 'Access Egress Invehicle Time',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(9, 1, '통행시간관련 수단특성변수', 'ln(총통행시간a)', '분', '총통행시간a의 log값', 'Time-Related', 'ln(Total-Travel-Time1)', 'Minute', 'ln(Total-Travel-Time1)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(10, 1, '통행시간관련 수단특성변수', 'ln(총통행시간b)', '분', '총통행시간b의 log값', 'Time-Related', 'ln(Total-Travel-Time2)', 'Minute', 'ln(Total-Travel-Time2)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(11, 1, '통행시간관련 수단특성변수', 'ln(차내통행시간)', '분', '차내통행시간의 log값', 'Time-Related', 'ln(Invehicle Time)', 'Minute', 'ln(Invehicle Time)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(12, 1, '통행시간관련 수단특성변수', 'ln(차외통행시간a)', '분', '(환승시간+대기시간)의 log값', 'Time-Related', 'ln(Out-of-Vehicle Time1)', 'Minute', 'ln(Out-of-Vehicle-Time1=Transfer+Waiting)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(13, 1, '통행시간관련 수단특성변수', 'ln(차외통행시간b)', '분', '(환승시간+대기시간+출발 도보접근시간+도착 도보접근시간)의 log값', 'Time-Related', 'ln(Out-of-Vehicle Time2)', 'Minute', 'ln(Out-of-Vehicle-Time2=Transfer+Waiting+Access+Egress)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(14, 1, '통행시간관련 수단특성변수', '차내통행시간 비율', '%', '총통행시간에 대한 차내통행시간의 비율 : (차내통행시간 ÷ 총통행시간b)*100', 'Time-Related', 'Invehicle Time Ratio', '%', '(Invehicle Time)/(Total Travel Time)*100',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(15, 1, '통행시간관련 수단특성변수', '환승시간 비율', '%', '총통행시간에 대한 환승시간의 비율 : (환승시간 ÷ 총통행시간b)*100', 'Time-Related', 'Transfer Time Ratio', '%', '(Transfer Time)/(Total Travel Time)*100',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(16, 1, '통행시간관련 수단특성변수', '대기시간 비율', '%', '총통행시간에 대한 대기시간의 비율 : (대기시간 ÷ 총통행시간b)*100', 'Time-Related', 'Waiting Time Ratio', '%', '(Waiting Time)/(Total Travel Time)*100',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(17, 1, '통행시간관련 수단특성변수', '출발 도보접근시간 비율', '%', '총통행시간에 대한 출발 도보접근시간의 비율 : (출발 도보접근시간 ÷ 총통행시간b)*100', 'Time-Related', 'Access Walking Time Ratio', '%', '(Access Walking Time)/(Total Travel Time)*100',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(18, 1, '통행시간관련 수단특성변수', '도착 도보접근시간 비율', '%', '총통행시간에 대한 도착 도보접근시간의 비율 : (도착 도보접근시간 ÷ 총통행시간b)*100', 'Time-Related', 'Egress Walking Time Ratio', '%', '(Egress Walking Time)/(Total Travel Time)*100',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(19, 2, '통행비용관련 수단특성변수', '통행비용', '100원', '통행비용 = 승용차/택시비용+통행료', 'Cost-Related', 'Out-of-Pocket Cost', '100KRW', 'Out-of-Pocket Cost=PassengerCar/Taxi Cost + Toll Fee',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(20, 2, '통행비용관련 수단특성변수', '승용차/택시비용', '100원', '승용차 연료비용/택시 요금', 'Cost-Related', 'PassengerCar/Taxi Cost', '100KRW', 'Fuel Cost/Taxi Fare',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(21, 2, '통행비용관련 수단특성변수', '통행료', '100원', '유료도로 통행료', 'Cost-Related', 'Toll Fee', '100KRW', 'Toll Fee',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(22, 2, '통행비용관련 수단특성변수', 'PR주차비용', '100원', 'Park&Ride의 주차비용', 'Cost-Related', 'Parking Cost', '100KRW', 'Park&Ride Parking Cost',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(23, 2, '통행비용관련 수단특성변수', '접근비용', '100원', '택시 및 대중교통 접근비용', 'Cost-Related', 'Access Egress Cost', '100KRW', 'Access Egress Cost',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(24, 2, '통행비용관련 수단특성변수', '대중교통 요금', '100원', '대중교통 요금', 'Cost-Related', 'Transit Fare', '100KRW', 'Transit Fare',1, 1)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(25, 2, '통행비용관련 수단특성변수', 'ln(통행비용)', '100원', '통행비용의 log값', 'Cost-Related', 'ln(Out-of-Pocket Cost)', '100KRW', 'ln(Out-of-Pocket Cost)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(26, 2, '통행비용관련 수단특성변수', 'ln(대중교통 요금)', '100원', '대중교통 요금의 log값', 'Cost-Related', 'ln(Transit Fare)', '100KRW', 'ln(Transit Fare)',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(27, 3, '기타 수단특성변수', '환승횟수', '-', '환승한 횟수', 'Others', 'No-of-Transfers', '-', 'Number of Transfers',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(28, 3, '기타 수단특성변수', '운행횟수', '-', '출발지 대중교통 노선 운행횟수', 'Others', 'Freq', '-', 'Operating Frequency of Urban Transit Line',0, 0)"),
			_T("INSERT INTO UrbanModeChoiceVariable(ID, VarType, VarTypeName, VarName, VarUnit, VarDescription, VarTypeNameEng, VarNameEng, VarUnitEng, VarDescriptionEng, Selected, InitSelect) VALUES(29, 3, '기타 수단특성변수', '더미', '-', '수단 특성 상수', 'Others', 'Dummy', '-', 'Alternative-specific constant',1, 1)"),
		};

		for (CString& strSQLInsert : arrSQLInsert)
		{
			spDBConn->ExecuteUpdate(strSQLInsert);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}
}


void KDBaseVersion_P1001::CreateTableUrbanModeChoiceVariableZone(KDBaseConPtr spDBConn)
{
	try
	{
		// 기존 테이블 삭제
		CString strSQLDrop;
		strSQLDrop.Format(_T("DROP TABLE IF EXISTS %s "), TABLE_NAME_URBANMODECHOICE_ZONE);
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		CString strSQLCreate;
		strSQLCreate.Format(_T(
			" CREATE TABLE `%s` ( "
			"	`ID`	INTEGER, "
			"	`VarType`	INTEGER, "
			"	`Selected`	INTEGER DEFAULT 0, "
			"	`VarTypeName`	TEXT, "
			"	`VarName`	TEXT, "
			"	`VarTypeNameEng`	TEXT, "
			"	`VarNameEng`	TEXT, "
			"	`ZoneColName`	TEXT, "
			"	`ZoneColDispName`	TEXT, "
			"	`ZoneColDataType`	INTEGER DEFAULT 0, "
			"	`ZoneColCodeType`	INTEGER DEFAULT - 1, "
			"	`ZoneColDescription`	TEXT, "
			"	PRIMARY KEY(`ID`) "
			"	) "), 
			TABLE_NAME_URBANMODECHOICE_ZONE
		);

		spDBConn->ExecuteUpdate(strSQLCreate);

		// 초기 데이터 입력
		std::vector<CString> arrSQLInsert{
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(101, 101, '존특성변수', 'z1', 'Zone-Specific', 'z1', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(102, 101, '존특성변수', 'z2', 'Zone-Specific', 'z2', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(103, 101, '존특성변수', 'z3', 'Zone-Specific', 'z3', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(104, 101, '존특성변수', 'z4', 'Zone-Specific', 'z4', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(105, 101, '존특성변수', 'z5', 'Zone-Specific', 'z5', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(106, 101, '존특성변수', 'z6', 'Zone-Specific', 'z6', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(107, 101, '존특성변수', 'z7', 'Zone-Specific', 'z7', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(108, 101, '존특성변수', 'z8', 'Zone-Specific', 'z8', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(109, 101, '존특성변수', 'z9', 'Zone-Specific', 'z9', '', '', 0,-1, '-',0)"),
			_T("INSERT INTO UrbanModeChoiceVariableZone(ID, VarType, VarTypeName, VarName, VarTypeNameEng, VarNameEng, ZoneColName, ZoneColDispName, ZoneColDataType, ZoneColCodeType, ZoneColDescription, Selected) VALUES(110, 101, '존특성변수', 'z10', 'Zone-Specific', 'z10', '', '', 0,-1, '-',0)")
		};

		for (CString& strSQLInsert : arrSQLInsert)
		{
			spDBConn->ExecuteUpdate(strSQLInsert);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}
}



void KDBaseVersion_P1001::UpdateDefaultIntermodalUrbanModeChoicePara(KProject* pProject, KDBaseConPtr spDBConn)
{
	// 데이터 갱신에 사용할 임시 자료형 
	struct STDIUMCP_TEMP
	{
		int nObjId;		// 목적ID
		int nCofId;		// 변수ID
		double dAuto;	// 승용차
		double dTaxi;	// 택시
		double dHTransit;	// 버스
		double dRTransit;	// 도시철도
		double dComposite;	// 버스+도시철도
		double dParkRide;	// Park & Ride
		double dKissRide;	// Kiss & Ride
		double dTTransit;	// 택시 & 대중교통
		CString strObjName;	// 목적 명
		CString strObjNameEng;	// 목적 영문명
		STDIUMCP_TEMP() : nObjId(0), nCofId(0), dAuto(0), dTaxi(0), dHTransit(0), dRTransit(0),
			dComposite(0), dParkRide(0), dKissRide(0), dTTransit(0) {}
	};

	// 이전 시나리오의 마지막 변수와 값을 가져온다
	CString strSQL =
		_T(" SELECT Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, "
		   "		Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit "
		   " FROM Default_Intermodal_Urban_ModeChoice_Para "
		   " WHERE Master_Code = (SELECT MAX(Master_code) FROM Default_Intermodal_Urban_ModeChoice_Para)");

	try
	{

		std::vector<KPurpose*> arrPurpose = pProject->GetPurpose();
		std::map<int, CString> mapPurpose;
		for (KPurpose* pPurpose : arrPurpose)
		{
			mapPurpose.insert({ pPurpose->PurposeID(), pPurpose->PurposeName() });
		}

		std::vector<STDIUMCP_TEMP> arrOldPara;
		KResultSetPtr        spResult = spDBConn->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			STDIUMCP_TEMP t;
			int nMasterCode = spResult->GetValueInt(0);
			t.nObjId = spResult->GetValueInt(1);
			t.nCofId = spResult->GetValueInt(2);
			t.dAuto = spResult->GetValueDouble(3);
			t.dTaxi = spResult->GetValueDouble(4);
			t.dHTransit = spResult->GetValueDouble(5);
			t.dRTransit = spResult->GetValueDouble(6);
			t.dComposite = spResult->GetValueDouble(7);
			t.dParkRide = spResult->GetValueDouble(8);
			t.dKissRide = spResult->GetValueDouble(9);
			t.dTTransit = spResult->GetValueDouble(10);

			std::map<int, CString>::iterator itObjName = mapPurpose.find(t.nObjId);
			if (mapPurpose.end() != itObjName)
			{
				t.strObjName = itObjName->second;
				t.strObjNameEng = t.strObjName;
			}

			arrOldPara.push_back(t);
		}

		// 기존 값을 새로운 값으로 변환
		std::vector<STDIUMCP_TEMP> arrNewPara;
		for (STDIUMCP_TEMP& para : arrOldPara)
		{
			switch (para.nCofId)
			{
			case 1: {
				para.nCofId = 3;
				arrNewPara.push_back(para);
				break;
			}
			case 2: {
				para.nCofId = 5;
				arrNewPara.push_back(para);
				break;
			}
			case 3: {
				para.nCofId = 4;
				arrNewPara.push_back(para);
				break;
			}
			case 4: {
				// 평균 통행비용은 통행비용a와 대중교통요금으로 분리하여 저장
				// 승용차/택시
				STDIUMCP_TEMP t1 = para;
				t1.nCofId = 19;
				t1.dHTransit = 0;
				t1.dRTransit = 0;
				t1.dComposite = 0;
				arrNewPara.push_back(t1);

				// 버스/도시철도/버스+도시철도
				STDIUMCP_TEMP t2 = para;
				t2.nCofId = 24;
				t2.dAuto = 0;
				t2.dTaxi = 0;
				arrNewPara.push_back(t2);
				break;
			}
			case 5: {
				para.nCofId = 22;
				arrNewPara.push_back(para);
				break;
			}
			case 6: {
				// 평균 도보접근시간은 출발 도보접근시간과 도착 도보접근시간으로 분리하여 저장
				// 출발 도보접근시간
				arrNewPara.push_back(para);
				// 도착 도보접근시간
				STDIUMCP_TEMP t = para;
				t.nCofId = 7;
				arrNewPara.push_back(t);
				break;
			}
			case 7: {
				para.nCofId = 8;
				arrNewPara.push_back(para);
				break;
			}
			case 8: {
				para.nCofId = 23;
				arrNewPara.push_back(para);
				break;
			}
			case 9: {
				para.nCofId = 27;
				arrNewPara.push_back(para);
				break;
			}
			case 10: {
				para.nCofId = 29;
				arrNewPara.push_back(para);
				break;
			}
			}
		}

		CString strSQLCreate = _T(
			" CREATE TABLE `Default_Intermodal_Urban_ModeChoice_Para_v1001` ( "
			" 	`Master_Code`	INTEGER NOT NULL, "
			" 	`Object_ID`	INTEGER NOT NULL, "
			" 	`Cofficient_ID`	INTEGER NOT NULL, "
			" 	`Auto`	DOUBLE, "
			" 	`Taxi`	DOUBLE, "
			" 	`Highway_Transit`	DOUBLE, "
			" 	`Rail_Transit`	DOUBLE, "
			" 	`Composite_Mode`	DOUBLE, "
			" 	`Park_Ride`	DOUBLE, "
			" 	`Kiss_Ride`	DOUBLE, "
			" 	`Taxi_Transit`	DOUBLE, "
			"	`ObjName`	TEXT, "
			"	`ObjNameEng`	TEXT, "
			" 	CONSTRAINT `` PRIMARY KEY(`Master_Code`,`Object_ID`,`Cofficient_ID`) "
			" 	) ");

		spDBConn->ExecuteUpdate(strSQLCreate);


		//// 기존 저장된 값 삭제
		//CString strSQLDelete(_T(" Delete From Default_Intermodal_Urban_ModeChoice_Para_v1001"));
		//spDBConn->ExecuteUpdate(strSQLDelete);

		 
		// 초기값 입력
		std::vector<CString> arrSQLInsert{
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 3, -0.04398242, -0.04398242, -0.05492378, -0.05492378, -0.05492378, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 16, 0, 0, -0.03795691, -0.03795691, -0.03795691, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 17, 0, 0, -0.04309381, -0.04309381, -0.04309381, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 18, 0, 0, -0.03070491, -0.03070491, -0.03070491, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 19, -0.00983075, -0.00983075, 0, 0, 0, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 24, 0, 0, -0.07258449, -0.07258449, -0.07258449, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 29, 0, -3.28507865, 4.64596241, 3.58247181, 5.18975777, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 3, -0.11442524, -0.02750862, -0.07058808, -0.07058808, -0.07058808, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 15, 0, 0, -0.17843119, -0.17843119, -0.17843119, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 16, 0, 0, -0.01577147, -0.01577147, -0.01577147, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 17, 0, 0, -0.0198006, -0.0198006, -0.0198006, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 18, 0, 0, -0.0198006, -0.0198006, -0.0198006, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 19, -0.02921729, -0.02921729, 0, 0, 0, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 24, 0, 0, -0.02921729, -0.02921729, -0.02921729, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 2, 29, 0, -2.66457731, 3.23899809, 2.16368299, 3.67675698, 0, 0, 0, '가정기반등하교', 'HomeBasedSchool')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 3, -0.069128, -0.069128, -0.06183746, -0.06183746, -0.06183746, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 15, 0, 0, -0.29566145, -0.29566145, -0.29566145, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 16, 0, 0, -0.05126481, -0.05126481, -0.05126481, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 17, 0, 0, -0.04497242, -0.04497242, -0.04497242, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 18, 0, 0, -0.03457749, -0.03457749, -0.03457749, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 19, -0.00710563, -0.00710563, 0, 0, 0, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 24, 0, 0, -0.0838996, -0.0838996, -0.0838996, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 3, 29, 0, -0.88603661, 5.81551422, 4.13271153, 6.50822822, 0, 0, 0, '가정기반기타', 'HomeBasedOthers')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 3, -0.05221118, -0.05221118, -0.04281299, -0.04281299, -0.04281299, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 15, 0, 0, -0.25509439, -0.25509439, -0.25509439, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 16, 0, 0, -0.05953648, -0.05953648, -0.05953648, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 17, 0, 0, -0.03664146, -0.03664146, -0.03664146, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 18, 0, 0, -0.03893268, -0.03893268, -0.03893268, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 1, 15, 0, 0, -0.2343167, -0.2343167, -0.2343167, 0, 0, 0, '가정기반출퇴근', 'HomeBasedWork')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 19, -0.00916851, -0.00916851, 0, 0, 0, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 24, 0, 0, -0.13254463, -0.13254463, -0.13254463, 0, 0, 0, '비가정기반', 'NonHomeBased')"),
			_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) VALUES(0, 4, 29, 0, -1.50141681, 4.53620173, 3.91165914, 5.22267498, 0, 0, 0, '비가정기반', 'NonHomeBased')")
		};
	
		for (CString& strSQLInsert : arrSQLInsert)
		{
			spDBConn->ExecuteUpdate(strSQLInsert);
		}
		

		// 갱신된 변수 값 입력
		for (STDIUMCP_TEMP& p : arrNewPara)
		{
			CString strSQLInsert;
			strSQLInsert.Format(_T("INSERT INTO Default_Intermodal_Urban_ModeChoice_Para_v1001(Master_Code, Object_ID, Cofficient_ID, "
				"Auto, Taxi, Highway_Transit, Rail_Transit, Composite_Mode, Park_Ride, Kiss_Ride, Taxi_Transit, ObjName, ObjNameEng) "
				" VALUES(1, %d, %d, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, %.8f, '%s', '%s')"),
				p.nObjId, p.nCofId, p.dAuto, p.dTaxi, p.dHTransit, p.dRTransit, p.dComposite, p.dParkRide, p.dKissRide, p.dTTransit, 
				p.strObjName, p.strObjNameEng);

			spDBConn->ExecuteUpdate(strSQLInsert);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 2;
	}
}



void KDBaseVersion_P1001::UpdateSelectedVariable(KDBaseConPtr spDBConn)
{
	// 갱신된 변수의 마지막 변수의 ID를 가져온다
	CString strSQL =
		_T(" SELECT Master_Code, Cofficient_ID "
			" FROM Default_Intermodal_Urban_ModeChoice_Para_v1001 "
			" WHERE Master_Code = (SELECT MAX(Master_code) FROM Default_Intermodal_Urban_ModeChoice_Para_v1001)"
			" GROUP BY Cofficient_ID");
	try
	{

		std::vector<int> arrCoffId;
		KResultSetPtr        spResult = spDBConn->ExecuteQuery(strSQL);

		while (spResult->Next())
		{
			int nMasterCode = spResult->GetValueInt(0);
			int nCoffId = spResult->GetValueInt(1);
			arrCoffId.push_back(nCoffId);
		}

		// 이전 선택된 값을 초기화
		CString strSQLUpdate = _T("UPDATE UrbanModeChoiceVariable SET Selected = 0");
		spDBConn->ExecuteUpdate(strSQLUpdate);


		// 선택된 변수 항목을 선택됨으로 갱신
		strSQLUpdate = _T("UPDATE UrbanModeChoiceVariable SET Selected = 1 WHERE ID IN (");
		for (int nId : arrCoffId)
		{
			CString strId;
			strId.Format(_T("%d,"), nId);
			strSQLUpdate.Append(strId);
		}

		strSQLUpdate.Delete(strSQLUpdate.GetLength() - 1, 1);
		strSQLUpdate.Append(_T(")"));
		
		spDBConn->ExecuteUpdate(strSQLUpdate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 2;
	}
}



void KDBaseVersion_P1001::CreateTableMultiAssignInfoSoba(KDBaseConPtr spDBConn)
{
	try
	{
		CString strSQLDrop;
		strSQLDrop.Format(_T("DROP TABLE IF EXISTS %s "), TABLE_NAME_MULTI_ASSIGN_INFO_SOBA);
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		CString strSQLCreate;
		strSQLCreate.Format(_T(
			" CREATE TABLE `%s` ( "
			"	`ID`	INTEGER, "
			"	`NAME`	VARCHAR2, "
			"	`MODES`	VARCHAR2, "
			"	PRIMARY KEY(`ID`) "
			"	) "),
			TABLE_NAME_MULTI_ASSIGN_INFO_SOBA
		);

		spDBConn->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}
}

void KDBaseVersion_P1001::CreateTableMultiassignDataSoba(KDBaseConPtr spDBConn)
{
	try
	{
		CString strSQLDrop;
		strSQLDrop.Format(_T("DROP TABLE IF EXISTS %s "), TABLE_NAME_MULTI_ASSIGN_DATA_SOBA);
		spDBConn->ExecuteUpdate(strSQLDrop);

		// 테이블 생성
		CString strSQLCreate;
		strSQLCreate.Format(_T(
			" CREATE TABLE `%s` ( "
			"	`ID`	INTEGER, "
			"	'NO'	INTEGER, "
			"	`Scenario`	VARCHAR2, "
			"	`Target`	VARCHAR2, "
			"	`Data`	VARCHAR2, "
			"	`WS_Scenario`	VARCHAR2, "
			"	`WS_Target`	VARCHAR2, "
			"	PRIMARY KEY(`ID`, 'NO') "
			"	) "),
			TABLE_NAME_MULTI_ASSIGN_DATA_SOBA
		);

		spDBConn->ExecuteUpdate(strSQLCreate);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		throw 8;
	}
}