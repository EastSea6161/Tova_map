#include "StdAfx.h"
#include "DefineNamesModels.h"

namespace KInterUrbanPara
{

	const int  AUTO_CODE			= 1;	
	const int  TAXI_CODE			= 2;	
	const int  HIGHWAYTRANSIT_CODE	= 3;	
	const int  RAILTRANSIT_CODE		= 4;	
	const int  COMPOSITMODE_CODE	= 5;	
	const int  PARK_RADE_CODE		= 6;
	const int  KISS_RADE_CODE		= 7;
	const int  TAXI_TRANSIT_CODE	= 8;


	const TCHAR* AUTO_NAME					= _T("Auto");
	const TCHAR* TAXI_NAME					= _T("Taxi");
	const TCHAR* HIGHWAYTRANSIT_NAME		= _T("Bus Transit");
	const TCHAR* RAILTRANSIT_NAME			= _T("Rail Transit");
	const TCHAR* COMPOSITMODE_NAME			= _T("Composite Mode");
	const TCHAR* PARK_RADE_NAME				= _T("Park&Ride");
	const TCHAR* KISS_RADE_NAME				= _T("Kiss&Ride");
	const TCHAR* TAXI_TRANSIT_NAME			= _T("Taxi&Tranist");
}



namespace KInterCofficientPara
{
	const int INVEHICLE_TIME_CODE			= 1;
	const int TRANSIT_WAITING_TIME_CODE		= 2;
	const int TRANSIT_TRANSFER_TIME_CODE	= 3;
	const int COST_CODE						= 4;
	const int PARKING_FEE_CODE				= 5;
	const int AE_WALKING_TIME_CODE			= 6;
	const int AE_INVEHICLE_TIME_CODE		= 7;
	const int AE_INVEHICLE_COST_CODE		= 8;
	const int TRANSIT_TRANSFER_NUM_CODE		= 9;
	const int DUMMY_CODE					= 10;

	/*주의 이름에 '/' 입력 하면 안됨(포트란에서 읽을때 나눗셈 처리를 한다고 함)  */
	//내부 Data 용 - 띄워쓰기가 있으면 안됨
	const TCHAR* INVEHICLE_TIME_NAME			= _T("Invehicle_Time");
	const TCHAR* TRANSIT_WAITING_TIME_NAME		= _T("Transit_Waiting_Time");
	const TCHAR* TRANSIT_TRANSFER_TIME_NAME		= _T("Transit_Transfer_Time");
	const TCHAR* COST_NAME						= _T("Cost");
	const TCHAR* PARKING_FEE_NAME				= _T("Parking_Fee");
	const TCHAR* AE_WALKING_TIME_NAME			= _T("A_E_Walking_Time");
	const TCHAR* AE_INVEHICLE_TIME_NAME			= _T("A_E_InVehicle_Time");
	const TCHAR* AE_INVEHICLE_COST_NAME			= _T("A_E_InVehicle_Cost");
	const TCHAR* TRANSIT_TRANSFER_NUM_NAME		= _T("Transit_Transfer_Num");
	const TCHAR* DUMMY_NAME						= _T("Dummy");

	//View 단에 Display용 
	const TCHAR* INVEHICLE_TIME_DISPLAY			    = _T("Invehicle Time");
	const TCHAR* TRANSIT_WAITING_TIME_DISPLAY		= _T("Transit Waiting Time");
	const TCHAR* TRANSIT_TRANSFER_TIME_DISPLAY		= _T("Transit Transfer Time");
	const TCHAR* COST_DISPLAY						= _T("Cost");
	const TCHAR* PARKING_FEE_DISPLAY				= _T("Parking Fee");
	const TCHAR* AE_WALKING_TIME_DISPLAY			= _T("A/E Walking Time");
	const TCHAR* AE_INVEHICLE_TIME_DISPLAY			= _T("A/E InVehicle Time");
	const TCHAR* AE_INVEHICLE_COST_DISPLAY			= _T("A/E InVehicle Cost");
	const TCHAR* TRANSIT_TRANSFER_NUM_DISPLAY		= _T("Transit Transfer Num");
	const TCHAR* DUMMY_DISPLAY						= _T("Dummy");

	const TCHAR* INVEHICLE_TIME_DISPLAY_K			= _T("평균 차내통행시간");
	const TCHAR* TRANSIT_WAITING_TIME_DISPLAY_K		= _T("평균 대기시간");
	const TCHAR* TRANSIT_TRANSFER_TIME_DISPLAY_K	= _T("평균 환승시간");
	const TCHAR* COST_DISPLAY_K						= _T("평균 통행비용");
	const TCHAR* PARKING_FEE_DISPLAY_K				= _T("주차비용");
	const TCHAR* AE_WALKING_TIME_DISPLAY_K			= _T("평균 도보접근시간");
	const TCHAR* AE_INVEHICLE_TIME_DISPLAY_K		= _T("평균 차내접근시간");
	const TCHAR* AE_INVEHICLE_COST_DISPLAY_K		= _T("평균 접근비용");
	const TCHAR* TRANSIT_TRANSFER_NUM_DISPLAY_K		= _T("환승횟수");
	const TCHAR* DUMMY_DISPLAY_K					= _T("더미");
}


namespace KTransitMode
{
	const int HIGHWAY_TRANSIT_CODE = 3;
	const int RAIL_TRANSIT_CODE	   = 4;
	const int COMPOSIT_MODE_CODE   = 5;
	const int UPPER_LEVEL		   = 9;

}


namespace KInterRegionModeID
{
	const int AUTO_CODE		= -1;
	const int ACC_EGR_CODE	= 999;
	const TCHAR* AUTO_CODE_NAME = _T("Auto");
	const TCHAR* ACC_EGR_CODE_NAME = _T("AccEgrPath");
}



namespace KDistrbutionFunction
{
	const int Function1_Code = 1;
	const int Function2_Code = 2;
	const int Function3_Code = 3;

	const TCHAR* Function1_Name = _T("EXP(-β*Rij)");
	const TCHAR* Function2_Name = _T("α*EXP(-β*Rij)");
	const TCHAR* Function3_Name =  _T("α*Rij-β*EXP(-θ*Rij)");

}