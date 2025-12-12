#include "stdafx.h"
#include "DBaseVersion_P003.h"
//^#include "DBaseConnector.h"

void KDBaseVersion_P003::Upgrade( KDBaseConPtr spDBaseConnection )
{
    try
    {
        TransitCost (spDBaseConnection);
        RegionalMode(spDBaseConnection);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 1;
    }    
}

void KDBaseVersion_P003::TransitCost( KDBaseConPtr spDBaseConnection )
{
    try
    {
        CString strSQLAlter (_T(" ALTER TABLE transit_cost ADD COLUMN mode INTEGER DEFAULT 3 "));
        spDBaseConnection->ExecuteUpdate(strSQLAlter);
                
        try
        {
            CString strSQLUpdate(_T(""));
            strSQLUpdate = _T(" Update transit_cost set mode = ifnull( (select max(type) from transit Tb Where Tb.detail_type = transit_cost.type group by type, detail_type), 3) ");
            spDBaseConnection->ExecuteUpdate(strSQLUpdate);
        }
        catch (...)
        {
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

void KDBaseVersion_P003::RegionalMode( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE define_regional_mode "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLInsert(_T(""));

        strSQLInsert = _T(" INSERT INTO KCode_Group VALUES ('121', '지역간대중교통유형', 'Y') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO KCode_Define VALUES ('121', '1', 'Express_Bus') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO KCode_Define VALUES ('121', '2', '새마을호') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO KCode_Define VALUES ('121', '3', 'KTX') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO KCode_Define VALUES ('121', '4', 'Air') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO KCode_Define VALUES ('121', '5', 'Marine') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        CString strSQLCreate(_T(" CREATE TABLE define_regional_mode (regional_mode INTEGER PRIMARY KEY NOT NULL, Terminal_type INTEGER) " ));
        spDBaseConnection->ExecuteUpdate(strSQLCreate);
                
        strSQLInsert = _T(" INSERT INTO define_regional_mode VALUES ('1', '6') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO define_regional_mode VALUES ('2', '5') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO define_regional_mode VALUES ('3', '5') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO define_regional_mode VALUES ('4', '8') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);

        strSQLInsert = _T(" INSERT INTO define_regional_mode VALUES ('5', '9') ");
        spDBaseConnection->ExecuteUpdate(strSQLInsert);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 3;
    }  
}
