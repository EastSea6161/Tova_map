#include "StdAfx.h"
#include "KDBaseInspection.h"
//^#include "DBaseConnector.h"

KDBaseInspection::KDBaseInspection(void)
{
}


KDBaseInspection::~KDBaseInspection(void)
{
}

bool KDBaseInspection::CheckSEDMinus(KIOTable* a_pSEDVector, KIOColumn* a_pColumn, std::vector<InspectionResultSED>& ar_vecResult)
{
    TxLogDebugStartMsg();
    
    CString strSQL; 
    strSQL.Format (_T(" Select Zone_ID, Year, %s From %s Where %s < 0 Or %s is null Order by Zone_ID, Year "), 
                                              a_pColumn->Name(), 
                                                      a_pSEDVector->Name(), 
                                                               a_pColumn->Name(), 
                                                                         a_pColumn->Name());    
    
    InspectionResultSED result;
    CString             strOut;

    result.ErrorType  = _T("데이터 오류");
    result.ColumnName = a_pColumn->Caption();

    KDBaseConPtr spDBaseConnection = a_pSEDVector->GetDBaseConnection();

    try
    {        
        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        while ( pResultSet->Next() )
        {
            result.Zone_ID = pResultSet->GetValueString(0);
            result.Year    = pResultSet->GetValueString(1);
            strOut         = pResultSet->GetValueString(2);                       

            if (strOut.GetLength() > 0)
            {
                result.Desc.Format(_T("%s Data에 -값이 존재합니다.[%s]"), a_pColumn->Caption(), strOut);
            }
            else
            {
                result.Desc.Format(_T("%s Data에 Null 값이 존재합니다."), a_pColumn->Caption());
            }

            ar_vecResult.push_back(result);
        }

        TxLogDebugEndMsg();
        return true;
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
    }
    catch (...)
    {
        TxLogDebugWarning();
    }    

    TxLogDebugEndMsg();
    return false;
}

bool KDBaseInspection::CheckSEDMinus(KIOTable* pSEDVector, std::vector<InspectionResultSED>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pSEDVector->Columns();
    KIOColumn* pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();
    
    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
        {
            CheckSEDMinus(pSEDVector, pColumn, ar_vecResult);
        }
    }
    
    return true;
}

bool KDBaseInspection::CheckSEDCode(KIOTable* pSEDVector, KIOColumn* pColumn, std::vector<InspectionResultSED>& ar_vecResult)
{    
    CString strSQL; 
    strSQL.Format(_T("Select Zone_ID, Year, %s From %s Where %s not in ( select kcode_key from kcode_define where kcode_group_key = %d )  Order by Zone_ID, Year "), 
                                           pColumn->Name(), pSEDVector->Name(), pColumn->Name(), pColumn->CodeGroup());
        
    KDBaseConPtr spDBConnection = pSEDVector->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultSED result;

        result.ErrorType  = _T("코드 오류");
        result.ColumnName = pColumn->Caption();
        result.Zone_ID    = spResultSet->GetValueString(0);
        result.Year       = spResultSet->GetValueString(1);
        strOut            = spResultSet->GetValueString(2);

        if (strOut.GetLength() > 0)
        {
            result.Desc.Format(_T("%s [%s]값에 대한 코드 정보가 없습니다.[%s]"), pColumn->Caption(), strOut);
        }

        ar_vecResult.push_back(result);        
    }
        
    return true;
}

bool KDBaseInspection::CheckSEDCode(KIOTable* pSEDVector, std::vector<InspectionResultSED>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pSEDVector->Columns();
    KIOColumn*  pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            CheckSEDCode(pSEDVector, pColumn, ar_vecResult);
        }
    }

    return true;
}

bool KDBaseInspection::CheckNodeNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL; 
    strSQL.Format(_T("Select node_id, %s From %s Where %s is null "), pColumn->Name(), pTable->Name(), pColumn->Name());

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultOneKey result;

        result.ErrorType    = _T("null");
        result.ColumnName   = pColumn->Caption();
        result.ID           = spResultSet->GetValueString(0);
        result.Desc.Format(_T("%s Data에 Null 값이 존재합니다."), pColumn->Caption());

        ar_vecResult.push_back(result);
    }
    
    return true;
}

bool KDBaseInspection::CheckNodeNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn* pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
        {
            CheckNodeNullData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}

bool KDBaseInspection::CheckNodeCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL; 
    strSQL.Format(_T("Select Node_ID, %s From %s Where %s not in ( select kcode_key from kcode_define where kcode_group_key = %d ) "), 
                              pColumn->Name(), pTable->Name(), pColumn->Name(), pColumn->CodeGroup());

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultOneKey result;
        result.ErrorType    = _T("code");
        result.ColumnName   = pColumn->Caption();
        result.ID           = spResultSet->GetValueString(0);
        strOut              = spResultSet->GetValueString(1);

        if (strOut.GetLength() > 0)
        {
            result.Desc.Format(_T("%s [%s]값에 대한 코드 정보가 없습니다."), pColumn->Caption(), strOut);
        }

        ar_vecResult.push_back(result);
    }   

    return true;
}

bool KDBaseInspection::CheckNodeCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn* pColumn   = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            CheckNodeCodeData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}


bool KDBaseInspection::CheckNodeZoneMatching(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    try
    {
        CString strColumnName(pTable->Columns()->GetColumn(COLUMN_ZONE_ID)->Caption());
        CString strSQL = _T("select node_id, zone_id from node where zone_id not in(select zone_id from zone) ");

        KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
        KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            CString strOut;
            InspectionResultOneKey result;

            result.ErrorType  = _T("zone_id");
            result.ColumnName = strColumnName;
            result.ID         = spResultSet->GetValueString(0);
            strOut            = spResultSet->GetValueString(1);
            result.Desc.Format(_T("[%s]값에 대한 존 정보가 없습니다."), strOut);

            ar_vecResult.push_back(result);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        return false;
    }
    catch (...)
    {
    	TxLogDebugException();
        return false;
    }    

    return true;
}


bool KDBaseInspection::CheckNodeIsolated(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL = _T("select node_id from node where node_id not in (select fnode_id from link) AND node_id not in (select tnode_id from link)");

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultOneKey result;

        result.ErrorType  = _T("Isolated");
        result.ColumnName = _T("");
        result.ID         = spResultSet->GetValueString(0);

        result.Desc.Format(_T("노드[%s] : Isolated"), result.ID);
        ar_vecResult.push_back(result);
    }

    return true;
}


bool KDBaseInspection::CheckLinkNullData(KIOTable* a_pTable, KIOColumn* a_pColumn, std::vector<InspectionResultOneKey>& vecResult)
{
    TxLogDebugStartMsg();

    CString strSQL; 
    if (a_pColumn->DataType() == KEMIODataTypeDouble)
    {
        strSQL.Format(_T("Select link_id, %s From %s Where %s is null or %s < 0 "), 
                                        a_pColumn->Name(), 
                                                a_pTable->Name(), 
                                                        a_pColumn->Name(), a_pColumn->Name());
    }
    else
    {
        strSQL.Format(_T("Select link_id, %s From %s Where %s is null "), 
                                        a_pColumn->Name(), 
                                                a_pTable->Name(), 
                                                        a_pColumn->Name());
    }    
 
    InspectionResultOneKey result;
    CString                strOut;
 
    result.ErrorType  = _T("null");
    result.ColumnName = a_pColumn->Caption();
    
    try
    {
        KDBaseConPtr spDBConnection = a_pTable->GetDBaseConnection();
        KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);
        while( spResultSet->Next() )
        {          
            result.ID = spResultSet->GetValueString(0); 
            strOut    = spResultSet->GetValueString(1); 
            if (strOut.GetLength() < 1)
            {
                result.Desc.Format(_T("%s Data에 Null 값이 존재합니다."), a_pColumn->Caption());
            }
            else
            {
                result.Desc.Format(_T("%s Data에 0보다 작은 값[%s]이 존재합니다."), a_pColumn->Caption(), strOut);
            } 

            vecResult.push_back(result);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        return false;
    }
    catch (...)
    {
        return false;
    }    
    
    TxLogDebugEndMsg();
    return true;
}

bool KDBaseInspection::CheckLinkNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn* pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
        {
            CheckLinkNullData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}


bool KDBaseInspection::CheckLinkFTNode(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL = _T("select link_id, fnode_id, tnode_id from link where fnode_id = tnode_id");

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultOneKey result;
        
        result.ErrorType  = _T("From = To");
        result.ColumnName = _T("-");
        result.ID         = spResultSet->GetValueString(0); 

        result.Desc.Format(_T("링크[%s] : 시작노드와 종료노드가 동일합니다."), result.ID);
        ar_vecResult.push_back(result);
    }

    return true;
}

bool KDBaseInspection::CheckLinkCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{    
    CString strSQL; 
    strSQL.Format(_T("Select Link_ID, %s From %s Where %s not in ( select kcode_key from kcode_define where kcode_group_key = %d ) "), 
        pColumn->Name(), pTable->Name(), pColumn->Name(), pColumn->CodeGroup());

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        CString strOut;
        InspectionResultOneKey result;

        result.ErrorType    = _T("코드 오류");
        result.ColumnName   = pColumn->Caption();
        result.ID           = spResultSet->GetValueString(0); 
        strOut              = spResultSet->GetValueString(1); 

        if (strOut.GetLength() > 0)
        {
            result.Desc.Format(_T("%s [%s]값에 대한 코드 정보가 없습니다."), pColumn->Caption(), strOut);
        }

        ar_vecResult.push_back(result);
    }

    return true;
}

bool KDBaseInspection::CheckLinkCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn* pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            CheckLinkCodeData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}


bool KDBaseInspection::CheckZoneNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{    
    CString strSQL; 
    strSQL.Format(_T("Select zone_id, %s From %s Where %s is null "), pColumn->Name(), pTable->Name(), pColumn->Name()); 

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        InspectionResultOneKey result;
        CString strOut;

        result.ErrorType    = _T("null");
        result.ColumnName   = pColumn->Caption();
        result.ID           = spResultSet->GetValueString(0);
        strOut              = spResultSet->GetValueString(1);
        result.Desc.Format(_T("%s Data에 Null 값이 존재합니다."), result.ColumnName);

        ar_vecResult.push_back(result);
    }

    return true;
}

bool KDBaseInspection::CheckZoneNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn* pColumn = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
        {
            CheckZoneNullData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}

bool KDBaseInspection::CheckZoneCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{    
    CString strSQL; 
    strSQL.Format(_T("Select Zone_ID, %s From %s Where %s not in ( select kcode_key from kcode_define where kcode_group_key = %d ) "), 
        pColumn->Name(), pTable->Name(), pColumn->Name(), pColumn->CodeGroup());

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        InspectionResultOneKey result;
        CString strOut;

        result.ErrorType    = _T("코드 오류");
        result.ColumnName   = pColumn->Caption();
        result.ID           = spResultSet->GetValueString(0);
        strOut              = spResultSet->GetValueString(1); 

        if (strOut.GetLength() > 0)
        {
            result.Desc.Format(_T("%s [%s]값에 대한 코드 정보가 없습니다."), pColumn->Caption(), strOut);
        }

        ar_vecResult.push_back(result);
    }

    return true;
}

bool KDBaseInspection::CheckZoneCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn*  pColumn  = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            CheckZoneCodeData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}


bool KDBaseInspection::CheckTurnNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{    
    CString strSQL; 
    strSQL.Format(_T("Select turn_id, %s From %s Where %s is null "), pColumn->Name(), pTable->Name(), pColumn->Name()); 

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        InspectionResultOneKey result;
        CString strOut;

        result.ErrorType   = _T("null");
        result.ColumnName  = pColumn->Caption();
        result.ID          = spResultSet->GetValueString(0);
        strOut             = spResultSet->GetValueString(1); 

        result.Desc.Format(_T("%s Data에 Null 값이 존재합니다."), result.ColumnName);
        ar_vecResult.push_back(result);
    }
    
    return true;
}

bool KDBaseInspection::CheckTurnNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn*  pColumn  = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->DataType() == KEMIODataTypeDouble || pColumn->DataType() == KEMIODataTypeInteger)
        {
            CheckTurnNullData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}

bool KDBaseInspection::CheckTurnCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL; 
    strSQL.Format(_T("Select turn_ID, %s From %s Where %s not in ( select kcode_key from kcode_define where kcode_group_key = %d ) "), 
        pColumn->Name(), pTable->Name(), pColumn->Name(), pColumn->CodeGroup());

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        InspectionResultOneKey result;
        CString strOut;

        result.ErrorType  = _T("코드 오류");
        result.ColumnName = pColumn->Caption();
        result.ID         = spResultSet->GetValueString(0);
        strOut            = spResultSet->GetValueString(1);

        if (strOut.GetLength() > 0)
        {
            result.Desc.Format(_T("%s [%s]값에 대한 코드 정보가 없습니다."), pColumn->Caption(), strOut);
        }

        ar_vecResult.push_back(result);
    }
    
    return true;
}

bool KDBaseInspection::CheckTurnCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn*  pColumn  = NULL;

    int nColumnCount = pColumns->ColumnCount();

    for(int i=0; i<nColumnCount; i++)
    {
        pColumn = pColumns->GetColumn(i);
        if (pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
        {
            CheckTurnCodeData(pTable, pColumn, ar_vecResult);
        }
    }

    return true;
}

bool KDBaseInspection::CheckTurnLogic(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult)
{
    CString strSQL;
    strSQL = _T(" select * from ");    
    strSQL += _T(" (");
    strSQL += _T("    select ");
    strSQL += _T("     turn_id, inlink_id, node_id, outlink_id, ");
    strSQL += _T("     (select tnode_id from link where link.link_id = turn.inlink_id) as inEndNode, ");
    strSQL += _T("     (select fnode_id from link where link.link_id = turn.outlink_id) as outStartNode ");
    strSQL += _T("    from turn ");
    strSQL += _T(" ) Where node_id != inEndNode Or node_id != outStartNode ");

    KDBaseConPtr spDBConnection = pTable->GetDBaseConnection();
    KResultSetPtr       spResultSet    = spDBConnection->ExecuteQuery(strSQL);

    while(spResultSet->Next())
    {
        InspectionResultOneKey result;
        CString strOut;

        result.ErrorType  = _T("connectivity");
        result.ColumnName = _T("-");
        result.ID         = spResultSet->GetValueString(0); 

        result.Desc.Format(_T("Turn[%s] : 연속적인 링크가 아닙니다."), result.ID);
        ar_vecResult.push_back(result);
    }

    return true;
}