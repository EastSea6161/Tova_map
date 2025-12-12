#include "stdafx.h"
#include "QBicFileIni.h"
#include "QBicFlashIni.h"
#include "SQLiteConnection.h"

QBicFileIni::QBicFileIni( CString strPath ) : m_strPath(strPath)
{
    Load();
}

QBicFileIni::~QBicFileIni()
{
    Save();
}

void QBicFileIni::Load()
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  TKey, TValue "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  TFileIni "));
    }

    KDBaseConnectionPtr spDBase = IniConnect(m_strPath);
    if (spDBase == nullptr)
        return;

    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL); 
    while (spResult->Next()) {        
        CString TKey   = spResult->GetValueString(0); {
            TKey.MakeLower();
        }
        CString TValue = spResult->GetValueString(1);
        m_mapKeyValue[TKey] = TValue;
    }
}

void QBicFileIni::Save()
{
    KDBaseConnectionPtr spDBase = IniConnect(m_strPath);
    if (spDBase == nullptr)
        return;

    if (m_mapKeyValue.size() == 0)
        return;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Insert Into TFileIni "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  TKey, TValue "));
        strSQL.Append(_T(" ) "));
        strSQL.Append(_T(" Values "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  ?, ? "));
        strSQL.Append(_T(" ) "));
    }

    try
    {
        spDBase->BeginTransaction();
        spDBase->ExecuteUpdate(_T("Delete From TFileIni"));

        KPreparedStatementPtr spPrepared =  spDBase->PrepareStatement(strSQL);

        for(auto iter = m_mapKeyValue.begin(); iter != m_mapKeyValue.end(); ++iter) {
            CString TKey = iter->first;
            CString TVal = iter->second;

            spPrepared->BindText(1, TKey);
            spPrepared->BindText(2, TVal);
            spPrepared->ExecuteUpdate();
            spPrepared->Reset();
        }

        spDBase->Commit();
    } catch (...) {
        spDBase->RollBack();
    }
}

KDBaseConnectionPtr QBicFileIni::IniConnect( CString strPath )
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" CREATE TABLE "));
        strSQL.Append(_T(" If Not Exists [TFileIni] "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  [TKey]   VARCHAR2, "));
        strSQL.Append(_T("  [TValue] VARCHAR2, "));
        strSQL.Append(_T(" CONSTRAINT [] PRIMARY KEY ([TKey])"));
        strSQL.Append(_T(" ) "));
    }

    KDBaseConnectionPtr spDBase = KDBaseConnectionPtr(new KSQLiteConnection());

    try {
        KConnectionString oCon; {
            oCon.m_strUrl = strPath;
        }

        if (spDBase->Connection(oCon)) {
            spDBase->ExecuteUpdate(strSQL);
            return spDBase;
        } 
        else {
            spDBase.reset();
        }
    }
    catch (...) {
        spDBase.reset();
    }

    return nullptr;
}


QBicFileGrpIni::QBicFileGrpIni( CString strPath ) : m_strPath(strPath)
{  
    Load();
}


QBicFileGrpIni::~QBicFileGrpIni()
{
    Save();
}

KDBaseConnectionPtr QBicFileGrpIni::IniConnect( CString strPath )
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" CREATE TABLE "));
        strSQL.Append(_T(" If Not Exists [TFileGrpIni] "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  [TType]  INT, "));
        strSQL.Append(_T("  [TKey]   VARCHAR2, "));
        strSQL.Append(_T("  [TValue] VARCHAR2, "));
        strSQL.Append(_T(" CONSTRAINT [] PRIMARY KEY ([TType], [TKey])"));
        strSQL.Append(_T(" ) "));
    }

    KDBaseConnectionPtr spDBase = KDBaseConnectionPtr(new KSQLiteConnection());

    try {
        KConnectionString oCon; {
            oCon.m_strUrl = strPath;
        }

        if (spDBase->Connection(oCon)) {
            spDBase->ExecuteUpdate(strSQL);
            return spDBase;
        } 
        else {
            spDBase.reset();
        }
    }
    catch (...) {
        spDBase.reset();
    }

    return nullptr;
}

void QBicFileGrpIni::Load()
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  TType, TKey, TValue "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  TFileGrpIni "));
    }

    KDBaseConnectionPtr spDBase = IniConnect(m_strPath);
    if (spDBase == nullptr)
        return;
    
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL); 
    while (spResult->Next()) {
        int     nType  = spResult->GetValueInt   (0);
        CString TKey   = spResult->GetValueString(1); {
            TKey.MakeLower();
        }
        CString TValue = spResult->GetValueString(2);

        std::map<CString, CString>& mapKeyVal = m_mapTypeKeyValue[nType];
        mapKeyVal[TKey] = TValue;
    }
}

void QBicFileGrpIni::Save()
{
    KDBaseConnectionPtr spDBase = IniConnect(m_strPath);
    if (spDBase == nullptr)
        return;

    if (m_mapTypeKeyValue.size() == 0)
        return;

    CString strSQL(_T("")); {
        strSQL.Append(_T(" Insert Into TFileGrpIni "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  TType, TKey, TValue "));
        strSQL.Append(_T(" ) "));
        strSQL.Append(_T(" Values "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  ?, ?, ? "));
        strSQL.Append(_T(" ) "));
    }

    try
    {
        spDBase->BeginTransaction();
        spDBase->ExecuteUpdate(_T("Delete From TFileGrpIni"));

        KPreparedStatementPtr spPrepared =  spDBase->PrepareStatement(strSQL);

        for (auto iter = m_mapTypeKeyValue.begin(); iter != m_mapTypeKeyValue.end(); ++iter) {
            int nType = iter->first;
            std::map<CString, CString>& mapKeyVal = iter->second;
            
            for(auto iter = mapKeyVal.begin(); iter != mapKeyVal.end(); ++iter) {
                CString TKey = iter->first;
                CString TVal = iter->second;

                spPrepared->BindInt (1, nType);
                spPrepared->BindText(2, TKey);
                spPrepared->BindText(3, TVal);
                spPrepared->ExecuteUpdate();
                spPrepared->Reset();
            }
        }

        spDBase->Commit();
    } catch (...) {
        spDBase->RollBack();
    }
}

