#include "stdafx.h"
#include "AdoConnection.h"
#include "QBicComFunction.h"
#include "AdoResultSet.h"
#include "AdoPreparedStatement.h"

KAdoConnection::KAdoConnection()
{
    try
    {
        QBicComFunction::IsHResultError(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));
        QBicComFunction::IsHResultError(m_spAdoConnection.CreateInstance(CLSID_Connection));
    }
    catch(_com_error& e)
    {
        throw e;
    }
    catch(...)
    {
        ThrowException(_T("Connection Error"));
    }
}

KAdoConnection::KAdoConnection( LPCTSTR a_strFileName )
{
    try
    {
        QBicComFunction::IsHResultError(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED));        
        QBicComFunction::IsHResultError(m_spAdoConnection.CreateInstance(CLSID_Connection));

        if ( Connection(a_strFileName) == false)
        {
            m_spAdoConnection = NULL;
            throw 1;
        }
    }
    catch(_com_error& e)
    {
        throw e;
    }
    catch(...)
    {
        ThrowException(_T("Connection Error"));
    }
}

KAdoConnection::~KAdoConnection( void )
{
    if (m_spAdoConnection)
    {
        m_spAdoConnection->Close();
    }

    try
    {
        if (m_spAdoConnection)
        {
            CloseConnection();
        }  

        CoUninitialize();	
    }    
    catch (...)
    {    	
    }    
}

bool KAdoConnection::Connection( KConnectionString& ar_ConnectionString)
{
    CString strProvider   = _T("SQLOLEDB");
    CString strDataSource = _T("192.168.40.26");
    CString strDBName     = _T("BigData");
    CString strUID        = _T("sa");
    CString strPWD        = _T("w00rinara");
    
    CString strUrl(_T(""));
    strUrl.Format(_T("Provider=%s;Data Source=%s;Initial Catalog=%s;"), strProvider, strDataSource, strDBName);

    strUrl = ar_ConnectionString.m_strUrl;
    strUID = ar_ConnectionString.m_strUserID;
    strPWD = ar_ConnectionString.m_strPassword;

    CString strConnectionString = _T("");
    strConnectionString.Format(_T("%sUser ID=%s;Password=%s;"), strUrl, strUID, strPWD);

    return Connection(strConnectionString);
}

bool KAdoConnection::Connection( LPCTSTR a_strConnectionString )
{
    HRESULT hr = m_spAdoConnection->Open(_bstr_t(a_strConnectionString), _bstr_t(L""), _bstr_t(L""), -1);
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

void KAdoConnection::CloseConnection()
{
    if (!m_spAdoConnection)
    {        
        return;
    }

    if ( m_spAdoConnection->GetState() != adStateClosed)
        m_spAdoConnection->Close();
}

KResultSetPtr KAdoConnection::ExecuteQuery( LPCTSTR a_strSql )
{
    try
    {
        AdoRecordsetPtr spRecordSet = m_spAdoConnection->Execute(_bstr_t(a_strSql), NULL, ADODB::adCmdText);    
        KResultSetPtr   spResultSet = KResultSetPtr(new KAdoResultSet(spRecordSet));

        return spResultSet;
    }
    catch (...)
    {
        throw 1;
    }    
}

int KAdoConnection::ExecuteUpdate( LPCTSTR a_strSql )
{
    int nResult(ADO_OK);
    try
    {
    	m_spAdoConnection->Execute(_bstr_t(a_strSql), NULL, ADODB::adCmdText);
    }
    catch (...)
    {
        nResult = ADO_ERROR;
    }
    
    return nResult;
}

KPreparedStatementPtr KAdoConnection::PrepareStatement( LPCTSTR a_strSql )
{
    if (!m_spAdoConnection)
        ThrowException(_T("Not Connect to ..."));

    if ( m_spAdoConnection->GetState() == adStateClosed)
        ThrowException(_T("Closed Connection ..."));

    KPreparedStatementPtr spPreparedStatement;
    spPreparedStatement = KPreparedStatementPtr(new KAdoPreparedStatement(m_spAdoConnection, a_strSql));

    return spPreparedStatement;
}

void KAdoConnection::BeginTransaction()
{
    if (!m_spAdoConnection)
        ThrowException(_T("Not Connect to ..."));

    if ( m_spAdoConnection->GetState() == adStateClosed)
        ThrowException(_T("Closed Connection ..."));

    m_spAdoConnection->BeginTrans();
}

void KAdoConnection::Commit()
{
    if (!m_spAdoConnection)
        ThrowException(_T("Not Connect to ..."));

    if ( m_spAdoConnection->GetState() == adStateClosed)
        ThrowException(_T("Closed Connection ..."));

    m_spAdoConnection->CommitTrans();
}

void KAdoConnection::RollBack()
{
    if (!m_spAdoConnection)
        ThrowException(_T("Not Connect to ..."));

    if ( m_spAdoConnection->GetState() == adStateClosed)
        ThrowException(_T("Closed Connection ..."));

    m_spAdoConnection->RollbackTrans();
}
