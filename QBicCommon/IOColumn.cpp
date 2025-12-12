#include "StdAfx.h"
#include "IOColumn.h"


KIOColumn::KIOColumn(void)
{
    m_strName		= _T("");
    m_strCaption	= _T("");
    m_strColumnInfo	= _T("");

    m_emColumnType	= KEMIOColumnTypeUserDefine;
    m_enumCodeType  = KEMIOCodeTypeIsNothing;
    m_emDataType	= KEMIODataTypeDouble;    
    
    m_bPK           = false;
    m_bFreeze       = false;
    m_bDisplay      = true;

    m_nDisplayOrder = -1;
    m_nBaseID		= -1;
    m_nCodeGroup	= -1;
	m_nColumnGroup  = 1;
}


KIOColumn::KIOColumn(const KIOColumn& column)
{
    m_strName		= column.m_strName;
    m_strCaption	= column.m_strCaption;
    m_strColumnInfo	= column.m_strColumnInfo;

    m_emColumnType	= column.m_emColumnType;
    m_enumCodeType	= column.m_enumCodeType;
    m_emDataType	= column.m_emDataType;
	
    m_bPK			= column.m_bPK;
    m_bDisplay		= column.m_bDisplay;
	m_bFreeze		= column.m_bFreeze;
	
	m_nDisplayOrder = column.m_nDisplayOrder;
	m_nBaseID		= column.m_nBaseID;

	m_nCodeGroup	= column.m_nCodeGroup;
	m_nColumnGroup  = column.m_nColumnGroup;
}


KIOColumn::~KIOColumn(void)
{
}


KIOColumn& KIOColumn::operator=(const KIOColumn& column)
{
    if (this == &column)
        return *this;

    m_strName		= column.m_strName;
    m_strCaption	= column.m_strCaption;
    m_strColumnInfo	= column.m_strColumnInfo;

    m_emColumnType	= column.m_emColumnType;
    m_enumCodeType	= column.m_enumCodeType;
    m_emDataType	= column.m_emDataType;

    m_bPK			= column.m_bPK;
    m_bDisplay		= column.m_bDisplay;
    m_bFreeze		= column.m_bFreeze;

    m_nDisplayOrder = column.m_nDisplayOrder;
    m_nBaseID		= column.m_nBaseID;

    m_nCodeGroup	= column.m_nCodeGroup;
	m_nColumnGroup  = column.m_nColumnGroup;

	return *this;
}


KEMIOCodeType KIOColumn::CodeType(void) const
{
	return m_enumCodeType;
}


void KIOColumn::CodeType(KEMIOCodeType a_nCodeType)
{
	m_enumCodeType = a_nCodeType;
}


bool KIOColumn::Visible(void) const
{
	return m_bDisplay;
}


void KIOColumn::Visible(bool bVisible)
{
	m_bDisplay = bVisible;
}


bool KIOColumn::FreezeColumn(void) const
{
	return m_bFreeze;
}


void KIOColumn::FreezeColumn(bool bFreeze)
{
	m_bFreeze = bFreeze;
}


bool KIOColumn::PrimaryKey(void) const
{
	return m_bPK;
}


void KIOColumn::PrimaryKey(bool bPK)
{
	m_bPK = bPK;
}


int KIOColumn::DisplayOrder(void) const
{
	return m_nDisplayOrder;
}


void KIOColumn::DisplayOrder(int nOrder)
{
	m_nDisplayOrder = nOrder;
}


int KIOColumn::BaseID(void)
{
	return m_nBaseID;
}


void KIOColumn::BaseID(int nBaseID)
{
	m_nBaseID = nBaseID;
}


int KIOColumn::IncreseBaseID(void)
{
	return m_nBaseID++;
}


int KIOColumn::CodeGroup(void) const
{
	return m_nCodeGroup;
}


void KIOColumn::CodeGroup(int nCodeGroup)
{
	m_nCodeGroup = nCodeGroup;
}


int KIOColumn::ColumnGroup(void) const
{
	return m_nColumnGroup;
}


void KIOColumn::ColumnGroup(int nColumnGroup)
{
	m_nColumnGroup = nColumnGroup;
}


KEMIOColumnType KIOColumn::ColumnType(void) const
{
	return m_emColumnType;
}


void KIOColumn::ColumnType(KEMIOColumnType emColumnType)
{
	m_emColumnType = emColumnType;
}


KEMIODataType KIOColumn::DataType(void) const
{
	return m_emDataType;
}


void KIOColumn::DataType(KEMIODataType emDataType)
{
	m_emDataType = emDataType;
}


LPCTSTR KIOColumn::Name(void) const
{
	return m_strName;
}


void KIOColumn::Name(LPCTSTR strName)
{
	m_strName = strName;
}


LPCTSTR KIOColumn::Caption(void) const
{
	return m_strCaption;
}


void KIOColumn::Caption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
}


LPCTSTR KIOColumn::ColumnInfo(void) const
{
	return m_strColumnInfo;
}


void KIOColumn::ColumnInfo(LPCTSTR strColumnInfo)
{
	m_strColumnInfo = strColumnInfo;
}

