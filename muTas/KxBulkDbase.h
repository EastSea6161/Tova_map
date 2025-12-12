#pragma once

#include "KBulkType.h"
//^#include "DBaseConnector.h"

/*forward declare*/
class KIOTable;
class KIOColumns;

class KxBulkDbase
{
public:
    KxBulkDbase(KDBaseConPtr spDBaseConnection, KIOTable* a_pIOTable, KIOColumns* a_pUpsertColumns);
    ~KxBulkDbase(void);

    void	ImportData( Integer a_nxOZoneID, Integer a_nxDZoneID, KSDoubleRecord* a_pRecord);	
            // OD Table에 코드 데이터 추가-정수호
            void ImportData( KODKey a_odKey, int a_nValue);	
    void	ExecuteFullOuterJoin( void );

    void	DeprecatedFnExecute( void );
private:
    //ODUpsert에 사용되는, RenameTable, ImportTable을 수행이전에 삭제함
    void    RemoveWorkTable( void );

private:
    void	OnInitUpsert( void );
    void	CreateImportTable( void );
    void	InitPrepared( void );

private:
    KDBaseConPtr		m_spDBaseConnection;
    KIOTable*				m_pTable;
    KIOColumns*	m_pUpsertColumns;

    CString					m_strOriginTable;
    CString					m_strImportTable;
    CString					m_strRenameTable;

    int						m_nUpsertColumnCount;
    KPreparedStatementPtr	m_spPreparedStatement;
};

