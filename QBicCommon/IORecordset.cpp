/**
 * @file IORecordset.cpp
 * @brief KIORecordset 구현 파일
 * @author 
 * @date 2011.03.25
 * @remark
 */
#include "StdAfx.h"
#include "IORecordset.h"


KIORecordset::KIORecordset(KIOTable* pTable)
	:	m_nxRowCount(0),
		m_pTable(pTable),
        m_enDBType(SQLite)
{
}

KIORecordset::~KIORecordset(void)
{
}

size_t KIORecordset::GetRowCount(void)
{
	return m_nxRowCount;
}

const KIOColumns* KIORecordset::GetColumns(void)
{
	return m_pColumns;
}

void KIORecordset::SetDBType( KEMDbmsType enDBType )
{
    m_enDBType = enDBType;
}

KEMDbmsType KIORecordset::GetDBType()
{
    return m_enDBType;
}
