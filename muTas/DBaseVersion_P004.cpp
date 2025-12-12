#include "stdafx.h"
#include "DBaseVersion_P004.h"
//^#include "DBaseConnector.h"

void KDBaseVersion_P004::Upgrade( KDBaseConPtr spDBaseConnection )
{
    try
    {
        NetworkEditLog (spDBaseConnection);
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

void KDBaseVersion_P004::NetworkEditLog( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE NetworkEditLog "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE [NetworkEditLog] (   "));
        strSQLCreate.Append(_T( "   [LAYER] INT NOT NULL DEFAULT 0, "));
        strSQLCreate.Append(_T( "   [KEY_VALUE] INTEGER NOT NULL DEFAULT 0, "));
        strSQLCreate.Append(_T( "   [EDIT_TYPE] INT NOT NULL DEFAULT 0,   "));    
        strSQLCreate.Append(_T( "   [EDIT_TIME] DATETIME DEFAULT (datetime('now', 'localtime')), "));
        strSQLCreate.Append(_T( "   CONSTRAINT [] PRIMARY KEY ([LAYER], [KEY_VALUE])) "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        throw 41;
    }    
}
