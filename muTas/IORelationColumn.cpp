/**
 * @file IORelationColumn.cpp
 * @brief KIORelationColumn 구현 파일
 * @author jyk@qbicware.com
 * @date 2011.04.06
 * @remark
 */

#include "StdAfx.h"
#include "IORelationColumn.h"
#include "KmzSystem.h"


KIORelationColumn::KIORelationColumn(LPCTSTR strTableName)
{
	m_strTableName = strTableName;

	if ( KmzSystem::IsPassengerSystem() == true )
	{
		InitPassengerRelationInfo();
	}
	else
	{
		InitFreightRelationInfo();
	}
}


KIORelationColumn::~KIORelationColumn(void)
{

}


std::vector<TRelationColumnInfo> KIORelationColumn::GetRelationInfo()
{
	m_strTableName = m_strTableName.MakeUpper();

	std::map< CString, std::vector<TRelationColumnInfo> >::iterator iter = m_mapRelationColumnInfo.find(m_strTableName);

	std::vector<TRelationColumnInfo> vecRelationColumnInfo;

	if( iter != m_mapRelationColumnInfo.end())
	{
		vecRelationColumnInfo = iter->second;
	}

	return vecRelationColumnInfo;
}


void KIORelationColumn::InitPassengerRelationInfo()
{
	m_mapRelationColumnInfo.clear();
	 
	RegisterRelationInfo(TABLE_ZONE,        TABLE_GENERATION_MODEL,   COLUMN_ZONE_COLUMN_NAME);
	RegisterRelationInfo(TABLE_MODE_OD,     TABLE_MODECHOICE_MODEL,   COLUMN_MODE_OD_COLUMN_NAME);
	RegisterRelationInfo(TABLE_PURPOSE_OD,  TABLE_DISTRIBUTION_MODEL, COLUMN_PURPOSE_OD_COLUMN_NAME);
}


void KIORelationColumn::InitFreightRelationInfo()
{
	m_mapRelationColumnInfo.clear();

	RegisterRelationInfo(TABLE_ZONE,        TABLE_GENERATION_MODEL,   COLUMN_ZONE_COLUMN_NAME);
	RegisterRelationInfo(TABLE_MODE_OD,     TABLE_MODECHOICE_MODEL,   COLUMN_MODE_OD_COLUMN_NAME);

	RegisterRelationInfo(TABLE_MODE_OD,     TABLE_CHAIN_MODEL,   COLUMN_CHAINMODEL_ROAD);
	RegisterRelationInfo(TABLE_MODE_OD,     TABLE_CHAIN_MODEL,   COLUMN_CHAINMODEL_ROAD);

	RegisterRelationInfo(TABLE_VEHICLE_MODE_OD,     TABLE_VEHICLECONV_MODEL,   COLUMN_VEHICLECONV_MODEL_SMALL);
	RegisterRelationInfo(TABLE_VEHICLE_MODE_OD,     TABLE_VEHICLECONV_MODEL,   COLUMN_VEHICLECONV_MODEL_MEDIUM);
	RegisterRelationInfo(TABLE_VEHICLE_MODE_OD,     TABLE_VEHICLECONV_MODEL,   COLUMN_VEHICLECONV_MODEL_LARGE);
    RegisterRelationInfo(TABLE_VEHICLE_MODE_OD,     TABLE_VEHICLECONV_MODEL,   COLUMN_VEHICLECONV_MODEL_SUM);

	RegisterRelationInfo(TABLE_PURPOSE_OD,  TABLE_DISTRIBUTION_MODEL, COLUMN_PURPOSE_OD_COLUMN_NAME);
}


void KIORelationColumn::RegisterRelationInfo( CString a_strTableName, CString a_strRelationTableName, CString a_strRelationcolumnName )
{
	TRelationColumnInfo              oTRelationColumnInfo;

	oTRelationColumnInfo.strTableName  = a_strRelationTableName;
	oTRelationColumnInfo.strColumnName = a_strRelationcolumnName;

	a_strTableName = a_strTableName.MakeUpper();
	std::map< CString, std::vector<TRelationColumnInfo> >::iterator iter = m_mapRelationColumnInfo.find(a_strTableName);

	if(iter != m_mapRelationColumnInfo.end())	// 기존에 추가
	{
		std::vector<TRelationColumnInfo> &vecRelationColumnInfo = iter->second;
		vecRelationColumnInfo.push_back(oTRelationColumnInfo);
	}
	else										// 새로 생성
	{
		std::vector<TRelationColumnInfo> vecRelationColumnInfo;
		vecRelationColumnInfo.push_back(oTRelationColumnInfo);
		
		m_mapRelationColumnInfo.insert(std::make_pair(a_strTableName, vecRelationColumnInfo));
	}
}