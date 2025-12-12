#include "stdafx.h"
#include "DBaseVersion_P005.h"
//^#include "DBaseConnector.h"

void KDBaseVersion_P005::Upgrade( KDBaseConPtr spDBaseConnection )
{
    try
    {
        Target_Region_Vertex (spDBaseConnection);
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

void KDBaseVersion_P005::Target_Region_Vertex( KDBaseConPtr spDBaseConnection )
{
    try
    {
        try
        {
            CString strSQLDrop (_T(" DROP TABLE Target_Region_Vertex "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE [Target_Region_Vertex] (   "));
        strSQLCreate.Append(_T( "   [ID] INT NOT NULL DEFAULT 0, "));
        strSQLCreate.Append(_T( "   [Seq] INTEGER NOT NULL DEFAULT 0, "));
        strSQLCreate.Append(_T( "   [X] DOUBLE NOT NULL DEFAULT 0,   "));   
        strSQLCreate.Append(_T( "   [Y] DOUBLE NOT NULL DEFAULT 0,   ")); 
        strSQLCreate.Append(_T( "   CONSTRAINT [] PRIMARY KEY ([ID], [Seq])) "));

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
