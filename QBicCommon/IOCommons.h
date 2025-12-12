/**
 * @file IOCommons.h
 * @brief IO와 관련된 공통 선언 
 * @author 
 * @date 2011.03.23
 * @remark
 */

#pragma once

enum KEMDbmsType
{
    SQLite,
    MSSQL2012
};

/** Recordset type */
enum KEMIORecordsetType
{
	KEMIORecordsetTypeDynamic = 1,	/** Data를 DB에서 가져온다. */
	KEMIORecordsetTypeStatic = 2	/** Data를 메모리로 보관한다. */
};

/* Data types */
#define DATATYPESTRING_INTEGER		_T("Integer")
#define DATATYPESTRING_DOUBLE		_T("Double")
#define DATATYPESTRING_STRING		_T("String")

#define TSQLiteDataTypeInteger		_T("INTEGER")
#define TSQLiteDataTypeDouble		_T("DOUBLE")
#define TSQLiteDataTypeVarchar2		_T("VARCHAR2")

/** Data types */
enum KEMIODataType
{
	KEMIODataTypeInteger	    = 1,	/** Integer type */
	KEMIODataTypeDouble		    = 2,	/** Double type */
	KEMIODataTypeString		    = 3		/** String type */
};


/** Table types */
enum KEMIOTableType
{
	KEMIOTableTypeManager		= -9,	/** Manager table */
	KEMIOTableTypeSystem		= -1,	/** System table */
	KEMIOTableTypeUser			= 1,	/** User table */
	KEMIOTableTypeODData		= 2		/*OD Table*/
};

/** Table Display types */
enum KEMIOTableDisplayType 
{
    // 주의 사항(2015-05-01, nombara) : 
    // 여러 시스템에서 사용하고 있기 때문에
    // 수정에 주의 할 것

    KEMIOTableDisplayTypeSystemManager	= -1,
    KEMIOTableDisplayTypeNetwork		= 0,
    // ImTas - Start
    KEMIOTableDisplayTypeMatrix		    = 1,
    KEMIOTableDisplayTypeParameter      = 2,
    KEMIOTableDisplayTypeSED			= 3,
    // ImTas - Start

	KEMIOTableDisplayTypeTerminalInfo   = 1,
	KEMIOTableDisplayTypeRouteInfo		= 2,
	KEMIOTableDisplayTypeTrafficVolume  = 4, 
    KEMIOTableDisplayTypeEtc	        = 9,

	KEMIOTableDisplayTypeInterCityBus   = 1,
	KEMIOTableDisplayTypeRailway		= 2,
	KEMIOTableDisplayTypeRailway_them	= 3,
	KEMIOTableDisplayTypeCoastPassenger	= 4,
	KEMIOTableDisplayTypeFlight			= 5,

    // 장래
    KEMIOTableDisplayTypeFutureDB       = 2,
    KEMIOTableDisplayTypeFutureRailDB   = 4
};

/** Column types */
enum KEMIOColumnType
{
	KEMIOColumnTypeSystem		= -1,	/** System column */
	KEMIOColumnTypeFixed		= 0,	/** Fixed column */
	KEMIOColumnTypeUserDefine	= 1		/** User define column */
};

/** Column types */
enum KEMIOCodeType
{
    KEMIOCodeTypeIsNothing		= -1,	/** 데이터 컬럼 */
    KEMIOCodeTypeIsSingleCode	= 0,	/** 코드컬럼(3) */
    KEMIOCodeTypeIsMultiCode	= 1		/** 멀티 코드컬럼(1,2) */
};


