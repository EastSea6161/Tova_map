#include "stdafx.h"
#include "TxTables.h"


TxItem::TxItem() : m_strColumnName(_T(""))
{
}

TxItem::TxItem( const TxItem& right )
{
    m_strColumnName = right.m_strColumnName;
    m_anyItemValue  = right.m_anyItemValue;
}

TxItem::TxItem( CString strColumnName ) : m_strColumnName(strColumnName)
{
}

TxItem::TxItem( CString strColumnName, int nValue ) : m_strColumnName(strColumnName)
{
    m_anyItemValue = nValue;
}

TxItem::TxItem(CString strColumnName, __int64  nxValue) : m_strColumnName(strColumnName)
{
	m_anyItemValue = nxValue;
}

TxItem::TxItem( CString strColumnName, float fValue )  : m_strColumnName(strColumnName)
{
	m_anyItemValue = fValue;
}

TxItem::TxItem( CString strColumnName, double dValue ) : m_strColumnName(strColumnName)
{
	m_anyItemValue = dValue;
}

TxItem::TxItem( CString strColumnName, CString strValue )  : m_strColumnName(strColumnName)
{
	m_anyItemValue = strValue;
}

TxItem::~TxItem()
{
}


TxItem& TxItem::operator=( const TxItem& right )
{
    if (&right == this)
        return *this;

    m_strColumnName = right.m_strColumnName;
    m_anyItemValue  = right.m_anyItemValue;

    return *this;
}


TxDataType TxItem::ItemType()
{
    if (m_anyItemValue.empty() == true)
        return EnNull;

    if (typeid(int) == m_anyItemValue.type()) {
        return EnInt32;
    } else if (typeid(__int64) == m_anyItemValue.type()){
        return EnInt64;
    } else if (typeid(float) == m_anyItemValue.type()) {
        return EnFloat;
    } else if (typeid(double) == m_anyItemValue.type()) {
        return EnDouble;
    } else if (typeid(CString) == m_anyItemValue.type()) {
        return EnString;
    }

    return EnNull;
}


CString TxItem::ItemName()
{
    return m_strColumnName;
}

void TxItem::ItemName( CString strFieldName )
{
    m_strColumnName = strFieldName;
}

boost::any TxItem::Value()
{
    return m_anyItemValue;
}

void TxItem::Value( int nValue )
{
    m_anyItemValue = nValue;
}

void TxItem::Value( float fValue )
{
    m_anyItemValue = fValue;
}

void TxItem::Value( double dValue )
{
    m_anyItemValue = dValue;
}

void TxItem::Value( CString strValue )
{
    m_anyItemValue = strValue;
}

void TxItem::Value( __int64 nxValue )
{
	m_anyItemValue = nxValue;
}


CString TxItem::ToString()
{
    CString strOut(_T(""));
    
    if (typeid(int) == m_anyItemValue.type()) {
        strOut.Format(_T("%d"), boost::any_cast<int>(m_anyItemValue));
    } 
    else if (typeid(__int64) == m_anyItemValue.type()){
        strOut.Format(_T("%I64d"), boost::any_cast<__int64>(m_anyItemValue));
    } 
    else if (typeid(float) == m_anyItemValue.type()) {
        strOut.Format(_T("%f"), boost::any_cast<float>(m_anyItemValue));
    } 
    else if (typeid(double) == m_anyItemValue.type()) {
        strOut.Format(_T("%f"), boost::any_cast<double>(m_anyItemValue));
    } 
    else if (typeid(CString) == m_anyItemValue.type()) {
        strOut.Format(_T("%s"), boost::any_cast<CString>(m_anyItemValue));
    }

    return strOut;
}

// ########################################################

TxRecord::TxRecord()
{
}

TxRecord::TxRecord( TxRecord& right )
{
    std::vector<TxItem*> vecItem = right.m_vecItem;
    for (int i=0; i<(int)right.ItemCount(); i++) {
        TxItem* pRight = right.GetItem(i);
        TxItem* pItem  = new TxItem(*pRight);

        m_vecItem.push_back(pItem);
    }
}

TxRecord::~TxRecord()
{
    Clear();
}


TxRecord& TxRecord::operator=( TxRecord& right )
{
    if (this == &right)
        return *this;

    Clear();

    std::vector<TxItem*> vecItem = right.m_vecItem;
    for (int i=0; i<(int)right.ItemCount(); i++) {
        TxItem* pRight = right.GetItem(i);
        TxItem* pItem  = new TxItem(*pRight);

        m_vecItem.push_back(pItem);
    }

    return *this;
}

TxItem* TxRecord::AddItem( CString strColumnName )
{
    TxItem* pItem = new TxItem(strColumnName);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::AddItem( CString strColumnName, int nValue )
{
    TxItem* pItem = new TxItem(strColumnName, nValue);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::AddItem( CString strColumnName, float fValue )
{
    TxItem* pItem = new TxItem(strColumnName, fValue);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::AddItem( CString strColumnName, double dValue )
{
    TxItem* pItem = new TxItem(strColumnName, dValue);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::AddItem( CString strColumnName, __int64 nxValue )
{
    TxItem* pItem = new TxItem(strColumnName, nxValue);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::AddItem( CString strColumnName, CString strValue )
{
    TxItem* pItem = new TxItem(strColumnName, strValue);
    m_vecItem.push_back(pItem);

    return  pItem;
}

TxItem* TxRecord::GetItem( size_t i )
{
    size_t nxCount = m_vecItem.size();
    if (nxCount <= i)
        return nullptr;

    return m_vecItem[i];
}

void TxRecord::RemoveItem( size_t i )
{
    size_t nxCount = m_vecItem.size();
    if (nxCount <= i)
        return;

    auto iter = m_vecItem.begin();
    auto end  = m_vecItem.end();
    
    iter = iter + i;

    TxItem* pItem = *iter;
    delete  pItem;

    m_vecItem.erase(iter);
}


size_t TxRecord::ItemCount() const
{
    return m_vecItem.size();
}


void TxRecord::Clear()
{
    for (size_t i=0; i<m_vecItem.size(); i++) {
        TxItem* pItem = m_vecItem[i];
        delete  pItem;
    }

    m_vecItem.clear();
}

//###############################################################
TxRecords::TxRecords()
{
}

TxRecords::TxRecords( TxRecords& right )
{
    std::vector<TxRecord*>& vecRecord = right.m_vecRecord;
    for(size_t i=0; i<vecRecord.size(); i++) {
        TxRecord* pRight = vecRecord[i];

        TxRecord* pRecord = new TxRecord(*pRight);
        m_vecRecord.push_back(pRecord);
    }
}

TxRecords::~TxRecords()
{
    Clear();
}


TxRecords& TxRecords::operator=( TxRecords& right )
{
    if (this == &right)
        return *this;

    std::vector<TxRecord*>& vecRecord = right.m_vecRecord;
    for(size_t i=0; i<vecRecord.size(); i++) {
        TxRecord* pRight = vecRecord[i];

        TxRecord* pRecord = new TxRecord(*pRight);
        m_vecRecord.push_back(pRecord);
    }

    return *this;
}

TxRecord* TxRecords::AddRecord()
{
    TxRecord* pRecord = new TxRecord();
    m_vecRecord.push_back(pRecord);
    return pRecord;
}

TxRecord* TxRecords::GetRecord( size_t i )
{
    size_t nxCount = m_vecRecord.size();
    if (nxCount <= i)
        return nullptr;

    return m_vecRecord[i];
}

void TxRecords::RemoveRecord( size_t i )
{
    size_t nxCount = m_vecRecord.size();
    if (nxCount <= i)
        return;

    auto iter = m_vecRecord.begin();
    auto end  = m_vecRecord.end();

    iter = iter + i;

    TxRecord* pRecord = *iter;
    delete    pRecord;

    m_vecRecord.erase(iter);
}


size_t TxRecords::RecordCount()
{
    return m_vecRecord.size();
}


void TxRecords::Clear()
{
    for (size_t i=0; i<m_vecRecord.size(); i++) {
        TxRecord* pRecord = m_vecRecord[i];
        delete    pRecord;
    }

    m_vecRecord.clear();
}

//###########################################################################

TxColumn::TxColumn(CString strColumnName, TxDataType enType) : m_strColumnName(strColumnName), m_enColumnType(enType)
{
    m_bsFlag[EnumFlagPK]   = false;
    m_bsFlag[EnumFlagTxID] = false;
}

TxColumn::TxColumn( TxColumn& right )
{
    m_strColumnName    = right.m_strColumnName;
    m_strColumnCaption = right.m_strColumnCaption;

    m_bsFlag = right.m_bsFlag;
}

TxColumn::~TxColumn()
{    
}


TxColumn& TxColumn::operator=( TxColumn& right )
{
    if (this == &right)
        return *this;

    m_strColumnName    = right.m_strColumnName;
    m_strColumnCaption = right.m_strColumnCaption;

    m_bsFlag = right.m_bsFlag;

    return *this;
}

void TxColumn::Name( CString strName )
{
    m_strColumnName = strName;
}

CString TxColumn::Name()
{
    return m_strColumnName;
}

void TxColumn::Caption( CString strCaption )
{
    m_strColumnCaption = strCaption;
}

CString TxColumn::Caption()
{
    return m_strColumnCaption;
}

void TxColumn::ColumnType( TxDataType enType )
{    
    m_enColumnType = enType;
}

TxDataType TxColumn::ColumnType()
{
    return m_enColumnType;
}

void TxColumn::PrimaryKey( bool bPK/*=true*/ )
{
    m_bsFlag[EnumFlagPK] = bPK;
}

bool TxColumn::IsPrimaryKey()
{
    return m_bsFlag[EnumFlagPK];
}

void TxColumn::TxIDField( bool bFlag )
{
    m_bsFlag[EnumFlagTxID] = bFlag;
}

bool TxColumn::IsTxIDField()
{
    return m_bsFlag[EnumFlagTxID];
}

//#############################################################

TxColumns::TxColumns()
{
}

TxColumns::TxColumns( TxColumns& right )
{
    std::vector<TxColumn*>& vecColumn = right.m_vecColumn;
    for (size_t i=0; i<vecColumn.size(); i++) {
        TxColumn* pRight = vecColumn[i];
        TxColumn* pColumn = new TxColumn(*pRight);
        m_vecColumn.push_back(pColumn);
    }
}

TxColumns::~TxColumns()
{
    Clear();
}


TxColumns& TxColumns::operator=( TxColumns& right )
{
    if (this == &right)
        return *this;

    Clear();
    
    std::vector<TxColumn*>& vecColumn = right.m_vecColumn;
    for (size_t i=0; i<vecColumn.size(); i++) {
        TxColumn* pRight = vecColumn[i];
        TxColumn* pColumn = new TxColumn(*pRight);
        m_vecColumn.push_back(pColumn);
    }

    return *this;
}

TxColumn* TxColumns::AddColumn( CString strColumnName, TxDataType enType )
{
    TxColumn* pColumn = new TxColumn(strColumnName, enType);
    m_vecColumn.push_back(pColumn);
    return pColumn;
}

TxColumn* TxColumns::GetColumn( size_t i )
{
    size_t nxCount = m_vecColumn.size();
    if (nxCount <= i)
        return nullptr;

    return m_vecColumn[i];
}

void TxColumns::RemoveColumn( size_t i )
{
    size_t nxCount = m_vecColumn.size();
    if (nxCount <= i)
        return;

    auto iter = m_vecColumn.begin();
    auto end  = m_vecColumn.end();

    iter = iter + i;

    TxColumn* pColumn = *iter;
    delete    pColumn;

    m_vecColumn.erase(iter);
}


size_t TxColumns::ColumnCount()
{
    return m_vecColumn.size();
}

void TxColumns::Clear()
{
    for (size_t i=0; i<m_vecColumn.size(); i++) {
        TxColumn* pColumn = m_vecColumn[i];
        delete    pColumn;
    }

    m_vecColumn.clear();
}
