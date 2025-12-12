/**
 * @file timeod_common.h
 * @brief timeod의 공통 type 정의 파일
 * @author jyk@qbicware.com
 * @date 2012.05.14
 * @remark
 */
//^^ #include "IOColumn.h"

#pragma once

enum KEMTimeODGeneration
{
	KEMTimeODHourlyTotal = 1,
	KEMTimeODHourly      = 2
};

typedef std::set< int > KTDSurveyCodeSet;
	
/*********************************
* Fixed Mode 정보
*********************************/
#define MODEDATA_COUNT 4

enum KEMTimeODDefaultMode
{
	KEMInputModeAuto = 1,
	KEMInputModeTaxi    ,
	KEMInputModeBus     ,
	KEMInputModeRail = 4
};

#define MODENAME_AUTO _T("Auto")
#define MODENAME_AUTO_EN _T("PassengerCar")
#define MODENAME_TAXI _T("Taxi")
#define MODENAME_BUS  _T("Bus")
#define MODENAME_RAIL _T("Rail")
/*********************************/

struct TModeBase
{
	KEMTimeODDefaultMode emMode;
	CString              strModeName;
};

struct TInputModeInfo
{
	BOOL			     bSelected;
	TModeBase            oTMode;

	KIOColumn*           pModeODColumn;
	KIOColumn*           pParaODColumn;
};

struct TSurveyMode
{
	TModeBase            oTMode;
	std::set<int>        setModeSurveyCode;
};

struct TOutputModeInfo
{
	TModeBase            oTMode;
	KIOColumn*           pModeODColumn;
	int                  nModeGroup;
	int                  nDetailModeID;
	CString              strOutputName;
};

/*********************************
* Fixed Purpose 정보
*********************************/
#define PURPOSEDATA_COUNT 7

enum KEMTimeODDefaultPurpose
{
	KEMTimeODDefaultPurposeOffice    = 1,
	KEMTimeODDefaultPurposeSchool       ,    
	KEMTimeODDefaultPurposeInstitute    ,
	KEMTimeODDefaultPurposeShop         ,      
	KEMTimeODDefaultPurposeHome         ,      
	KEMTimeODDefaultPurposeBusiness     ,  
	KEMTimeODDefaultPurposeEtc       = 7       
};

#define PURPOSE_NAME_OFFICE    _T("출근")
#define PURPOSE_NAME_SCHOOL    _T("등교")
#define PURPOSE_NAME_INSTITUTE _T("학원")
#define PURPOSE_NAME_SHOP      _T("쇼핑")
#define PURPOSE_NAME_HOME      _T("귀가")
#define PURPOSE_NAME_BUSINESS  _T("업무")
#define PURPOSE_NAME_ETC       _T("기타")
/*********************************/

struct TPurposeBase
{
	KEMTimeODDefaultPurpose emPurpose;
	CString                 strPurposeName;
};

struct TSurveyPurpose
{
	TPurposeBase  oTPurpose;
	std::set<int> setPurposeSurveyCode;
};

#define ZONE_CHARACTER_COUNT 5

enum KEMZoneCharacterClassNum
{
	KEMClassStartNum = 2,
	KEMClassEndNum   = 3
};

struct TZoneCharacterInfo
{
	BOOL			   bSelected;
	KIOColumn*         pSedColumn;
	int                nClassNum;
};

enum KEMSurveyDataType
{
	KEMSurveyDataTypePurpose = 0,
	KEMSurveyDataTypeMode    = 1
};

enum KEMDefaultColumnIndex
{
	KEMDefaultColumnIndex_PersonalID = 2 ,
	KEMDefaultColumnIndex_Oringin    = 10,
	KEMDefaultColumnIndex_Dest       = 16,
	KEMDefaultColumnIndex_Purpose    = 12,
	KEMDefaultColumnIndex_Mode       = 19,
	KEMDefaultColumnIndex_StartHour  = 13,
	KEMDefaultColumnIndex_StartMin   = 14,
	KEMDefaultColumnIndex_EndHour    = 17,
	KEMDefaultColumnIndex_EndMin     = 18
};
