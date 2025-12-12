#include "StdAfx.h"
#include "BulkDBaseTransit.h"

#include "Target.h"
//^#include "DBaseConnector.h"
//^#include "MunsellColorCircle.h"
//^#include "DevideDataInterval.h"
#include "TChartTransit.h"

KBulkDBaseTransit::KBulkDBaseTransit( void )
{

}

KBulkDBaseTransit::~KBulkDBaseTransit( void )
{

}

void KBulkDBaseTransit::TransitLinkPathQuery( KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo )
{
    TxLogDebugStartMsg();

    a_vecTransitLinkInfo.clear();

    CString strSQL;
    strSQL.Format(_T("Select Link_ID, Seq, Station_YN, 0, 0, 0 From Transit_Links Where Transit_ID = '%I64d' Order By Seq"), a_nxTransitID); //★ 필요

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        
        TDrawTransitLinkInfo   oDrawTransitLinkInfo;
        while ( pResultSet->Next() )
        {
            oDrawTransitLinkInfo.TTransitID  = a_nxTransitID;
            oDrawTransitLinkInfo.TLinkID     = pResultSet->GetValueInt64 (0);
            oDrawTransitLinkInfo.TSeq        = pResultSet->GetValueInt   (1);
            oDrawTransitLinkInfo.TStationYN  = pResultSet->GetValueInt   (2);
            oDrawTransitLinkInfo.TOnPersons  = pResultSet->GetValueDouble(3);
            oDrawTransitLinkInfo.TOffPersons = pResultSet->GetValueDouble(4);
            oDrawTransitLinkInfo.TAtPersons  = pResultSet->GetValueDouble(5);            

            a_vecTransitLinkInfo.push_back(oDrawTransitLinkInfo);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch (...)
    {
        TxLogDebugWarning();
        throw 1;
    } 

    TxLogDebugEndMsg();
}

void KBulkDBaseTransit::TransitLinkPathQuery( KTarget* a_pTarget, std::vector<TDevideDataInterval>& a_vecDevideDataInterval, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo )
{
    TxLogDebugStartMsg();

    a_vecTransitLinkInfo.clear();

    CString strSQL;
    strSQL.Format(_T("Select Link_ID, Seq, Station_YN, 0, 0, 0 From Transit_Links Where Transit_ID = '%I64d' Order By Seq"), a_nxTransitID); //★ 필요

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    TDrawTransitLinkInfo   oDrawTransitLinkInfo;

    try
    {        
        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        
        while ( pResultSet->Next() )
        {
            oDrawTransitLinkInfo.TTransitID  = a_nxTransitID;

            oDrawTransitLinkInfo.TLinkID     = pResultSet->GetValueInt64 (0);
            oDrawTransitLinkInfo.TSeq        = pResultSet->GetValueInt   (1);
            oDrawTransitLinkInfo.TStationYN  = pResultSet->GetValueInt   (2);
            oDrawTransitLinkInfo.TOnPersons  = pResultSet->GetValueDouble(3);
            oDrawTransitLinkInfo.TOffPersons = pResultSet->GetValueDouble(4);
            oDrawTransitLinkInfo.TAtPersons  = pResultSet->GetValueDouble(5);            

            oDrawTransitLinkInfo.TGrade      = KGetGrade::GetGrade(a_vecDevideDataInterval, oDrawTransitLinkInfo.TAtPersons);

            a_vecTransitLinkInfo.push_back(oDrawTransitLinkInfo);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch (...)
    {
        CString strException;
        strException.Format(_T("Exception : TransitID(%I64d), LinkID(%I64d), Seq(%d) %.f"), 
                                a_nxTransitID, oDrawTransitLinkInfo.TLinkID, oDrawTransitLinkInfo.TSeq, 
                                oDrawTransitLinkInfo.TOnPersons);
        TxLogDebug((LPCTSTR)strException);
        throw 1;
    } 

    TxLogDebugEndMsg();
}

bool KBulkDBaseTransit::TransitInfoQuery( KTarget* a_pTarget, Integer a_nxTransitID, TTransitInfo& a_TransitInfo, double a_dWidth)
{
    TxLogDebugStartMsg();

    CString strSQL;
    strSQL.Format(_T("Select Transit_ID, Type, Name From Transit Where Transit_ID = '%I64d'"), a_nxTransitID);

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        KMunsellColor    oMunsellColor(0, false);

        if ( pResultSet->Next() )
        {
            a_TransitInfo.TTransitID   = a_nxTransitID;
            a_TransitInfo.TType        = pResultSet->GetValueInt(1);
            a_TransitInfo.TTransitName = pResultSet->GetValueString(2);
            a_TransitInfo.TRGBColor    = oMunsellColor.GetColor((int)a_nxTransitID);      
            
            a_TransitInfo.dWidth  = a_dWidth; // 디폴트 값
            //a_TransitInfo.dOffset = 0.0; // 디폴트 값

            return true;
        }

        return false;
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch (...)
    {
        TxLogDebugWarning();
        throw 1;
    } 

    return false;
    TxLogDebugEndMsg();
}

void KBulkDBaseTransit::TransitInfoQuery(KTarget* a_pTarget, Integer a_nxTransitID, std::map<CString, CString>& a_mapKeyValue)
{
    try
    {
        a_mapKeyValue.clear();
        const KIOColumns* pIOColumns = a_pTarget->Tables()->FindTable(TABLE_TRANSIT)->Columns();
        int nCount = pIOColumns->ColumnCount();
        if (nCount == 0)
            return;

        CString strSQL(_T("Select "));

        for (int i=0; i<nCount; i++)
        {
            if (i != nCount-1)
            {
                strSQL.AppendFormat(_T(" %s, "), pIOColumns->GetColumn(i)->Name());
            }
            else
            {
                strSQL.AppendFormat(_T(" %s "), pIOColumns->GetColumn(i)->Name());
            }
        }

        strSQL.AppendFormat(_T(" From %s Where transit_id = '%I64d' "), TABLE_TRANSIT, a_nxTransitID);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        if (pResultSet->Next())
        {
            for (int i=0; i<nCount; i++)
            {
                CString strColumnName(pIOColumns->GetColumn(i)->Name());
                CString strValue(pResultSet->GetValueString(i));

                a_mapKeyValue.insert(std::make_pair(strColumnName, strValue));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch(...)
    {
        throw 1;
    }
}

void KBulkDBaseTransit::DevideLinkAfterTransitAction( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink, __int64 nxFirstID, __int64 nxNodeID, __int64 nxLastID )
{
    std::vector<Integer> vecLinkViaTransit;
    LinkViaTransit(a_spDBaseConnection, a_nxLinkID, vecLinkViaTransit);

    size_t nxCount = vecLinkViaTransit.size();
    for (size_t i=0; i<nxCount; i++) {
        DevideTransitLink(a_spDBaseConnection, vecLinkViaTransit[i], a_nxLinkID, a_nxFirstLink, a_nxSecondLink, nxFirstID, nxNodeID, nxLastID);
    }
}

void KBulkDBaseTransit::LinkViaTransit( KDBaseConPtr a_spDBaseConnection, Integer nxLinkID, std::vector<Integer>& a_vecLinkViaTransit )
{
    a_vecLinkViaTransit.clear();

    CString strSQL = _T(""); {
        strSQL.Format(_T("Select Transit_ID From Transit_Links Where Link_ID = '%I64d' Group By Transit_ID "), nxLinkID); //★ 필요
    }    

    KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
    while (spResultSet->Next())
    {
        a_vecLinkViaTransit.push_back(spResultSet->GetValueInt64(0));
    }
}

void KBulkDBaseTransit::DevideTransitLink( KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID, Integer a_nxLinkID, Integer a_nxFirstLink, Integer a_nxSecondLink, __int64 nxFirstID, __int64 nxNodeID, __int64 nxLastID )
{     
    //★ Path 정보 조회(링크)
    std::vector<TDrawTransitLinkInfo> vecLineLinkInfo;   
    TransitLinkPathQuery (a_spDBaseConnection, a_nxTransitID, vecLineLinkInfo);

    //★ Path 정보 조회(노드)
    std::vector<TTransitNodeInfo> vecLineNodeInfo;
    TransitNodePathQuery (a_spDBaseConnection, a_nxTransitID, vecLineNodeInfo);

    //★ Transit Links, Transit Nodes 삭제
    DeleteTransitLinks(a_spDBaseConnection, a_nxTransitID);

    if (vecLineNodeInfo.size() <= 1) {
        return;
    }

    //★ Transit Links 신규 추가 프로세스
    if (vecLineLinkInfo.size() > 0) {
        CString strSQL(_T(" Insert Into Transit_Links(Transit_ID, Link_ID, Seq, Station_YN) Values(?, ?, ?, ?) "));
        KPreparedStatementPtr spPreparedStatement = a_spDBaseConnection->PrepareStatement(strSQL);

        size_t nxCount = vecLineLinkInfo.size();
        size_t nxSeq   = 0;
        for (size_t i=0; i<nxCount; i++) {
            TDrawTransitLinkInfo& oTransitLink = vecLineLinkInfo[i];

            if ( oTransitLink.TLinkID == a_nxLinkID ) {
                nxSeq ++;
                spPreparedStatement->BindInt64(1, oTransitLink.TTransitID);
                spPreparedStatement->BindInt64(2, a_nxFirstLink);
                spPreparedStatement->BindInt64(3, nxSeq);
                spPreparedStatement->BindInt  (4, 0);
                spPreparedStatement->ExecuteUpdate();
                spPreparedStatement->Reset();

                nxSeq ++;
                spPreparedStatement->BindInt64(1, oTransitLink.TTransitID);
                spPreparedStatement->BindInt64(2, a_nxSecondLink);
                spPreparedStatement->BindInt64(3, nxSeq);
                spPreparedStatement->BindInt  (4, oTransitLink.TStationYN);
                spPreparedStatement->ExecuteUpdate();
                spPreparedStatement->Reset();
            }
            else
            {
                nxSeq ++;
                spPreparedStatement->BindInt64(1, oTransitLink.TTransitID);
                spPreparedStatement->BindInt64(2, oTransitLink.TLinkID);
                spPreparedStatement->BindInt64(3, nxSeq);
                spPreparedStatement->BindInt64(4, oTransitLink.TStationYN);
                spPreparedStatement->ExecuteUpdate();
                spPreparedStatement->Reset();
            }        
        }
    }

    //★ Transit Links 신규 추가 프로세스
    CString strSQL(_T(" Insert Into Transit_Nodes(Transit_ID, Node_ID, Seq, Station_YN) Values(?, ?, ?, ?) "));
    KPreparedPtr spPrepared = a_spDBaseConnection->PrepareStatement(strSQL); 
    
    if (true) { //★ 첫번째 정류장
        TTransitNodeInfo& oInfo = vecLineNodeInfo[0];
        spPrepared->BindInt64(1, a_nxTransitID);
        spPrepared->BindInt64(2, oInfo.TNodeID);
        spPrepared->BindInt64(3, 1);
        spPrepared->BindInt64(4, 1);
        spPrepared->ExecuteUpdate();
        spPrepared->Reset();
    }

    size_t nxSeq(1);
    for (size_t i=0; i < vecLineNodeInfo.size()-1; i++) {
        TTransitNodeInfo& oInfo    = vecLineNodeInfo[i];
        TTransitNodeInfo& oInfoNxt = vecLineNodeInfo[i+1];

        if (oInfo.TNodeID == nxFirstID && oInfoNxt.TNodeID == nxLastID) {
            nxSeq++; {
                spPrepared->BindInt64(1, a_nxTransitID);
                spPrepared->BindInt64(2, nxNodeID);
                spPrepared->BindInt64(3, nxSeq);
                spPrepared->BindInt64(4, 0);
                spPrepared->ExecuteUpdate();
                spPrepared->Reset();
            }
        } 

        nxSeq++;
        spPrepared->BindInt64(1, a_nxTransitID);
        spPrepared->BindInt64(2, oInfoNxt.TNodeID);
        spPrepared->BindInt64(3, nxSeq);
        spPrepared->BindInt64(4, oInfoNxt.TStationYN);
        spPrepared->ExecuteUpdate();
        spPrepared->Reset();
    }
}


void KBulkDBaseTransit::TransitLinkPathQuery( KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID, std::vector<TDrawTransitLinkInfo>& a_vecTransitLinkInfo )
{
    a_vecTransitLinkInfo.clear();

    CString strSQL;
    strSQL.Format(_T("Select Link_ID, Seq, Station_YN, 0, 0, 0 From Transit_Links Where Transit_ID = '%I64d' Order By Seq"), a_nxTransitID);

    KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

    TDrawTransitLinkInfo   oDrawTransitLinkInfo;
    while ( spResultSet->Next() )
    {
        oDrawTransitLinkInfo.TTransitID  = a_nxTransitID;
        oDrawTransitLinkInfo.TLinkID     = spResultSet->GetValueInt64 (0);
        oDrawTransitLinkInfo.TSeq        = spResultSet->GetValueInt   (1);
        oDrawTransitLinkInfo.TStationYN  = spResultSet->GetValueInt   (2);
        oDrawTransitLinkInfo.TOnPersons  = spResultSet->GetValueDouble(3);
        oDrawTransitLinkInfo.TOffPersons = spResultSet->GetValueDouble(4);
        oDrawTransitLinkInfo.TAtPersons  = spResultSet->GetValueDouble(5);            

        a_vecTransitLinkInfo.push_back(oDrawTransitLinkInfo);
    }
}

void KBulkDBaseTransit::TransitPath( KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TTransitPath>& vecTransitPathInfo )
{
	vecTransitPathInfo.clear();

	CString strSQL(_T("")); {
        strSQL.Format(_T("Select Link_ID, Seq, Station_YN From Transit_Links Where Transit_ID = '%I64d' Order By Seq"), a_nxTransitID);
    }

	KDBaseConPtr spDBase = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

		while ( spResult->Next() )
		{
            TTransitPath  oInfo;
			oInfo.TTransitID  = a_nxTransitID;
			oInfo.TLinkID     = spResult->GetValueInt64 (0);
			oInfo.TSeq        = spResult->GetValueInt   (1);
			oInfo.TStationYN  = spResult->GetValueInt   (2);     

			vecTransitPathInfo.push_back(oInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 
}

void KBulkDBaseTransit::TransitNodePathQuery( KDBaseConPtr spDBase, Integer nxLineID, std::vector<TTransitNodeInfo>& vecLineNodeInfo )
{
    vecLineNodeInfo.clear();
    CString strSQL(_T("")); {
        strSQL.Format(_T("Select Node_ID, Seq, Station_YN From Transit_Nodes Where Transit_ID = '%I64d' Order By Seq"), nxLineID);
    }

    try
    {        
        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

        while ( spResult->Next() )
        {
            TTransitNodeInfo  oInfo;
            oInfo.TNodeID     = spResult->GetValueInt64 (0);
            oInfo.TSeq        = spResult->GetValueInt   (1);
            oInfo.TStationYN  = spResult->GetValueInt   (2);     

            vecLineNodeInfo.push_back(oInfo);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebug(ex->GetErrorMessage());
        throw ex;
    }
    catch (...)
    {
        TxLogDebugWarning();
        throw 1;
    } 
}


void KBulkDBaseTransit::DeleteTransit( KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID )
{
    try
    {
        CString strSQLDelTransit(_T(""));
                strSQLDelTransit.Format(_T(" DELETE FROM transit WHERE transit_id = '%I64d' "), a_nxTransitID);
    	a_spDBaseConnection->ExecuteUpdate(strSQLDelTransit);

        DeleteTransitLinks(a_spDBaseConnection, a_nxTransitID);
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At ")); 
    }
}

void KBulkDBaseTransit::DeleteTransitLinks( KDBaseConPtr a_spDBaseConnection, Integer a_nxTransitID )
{
    CString strSQL(_T("")); 

    if (true) {
        strSQL.Format(_T("Delete From Transit_Links Where Transit_ID = '%I64d' "), a_nxTransitID);
        a_spDBaseConnection->ExecuteUpdate(strSQL);
    }
    
    if (true) {
        strSQL.Format(_T("Delete From Transit_Nodes Where Transit_ID = '%I64d' "), a_nxTransitID);
        a_spDBaseConnection->ExecuteUpdate(strSQL);
    }
}

void KBulkDBaseTransit::LinkIDChangeAfterTransitAction( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginLinkID, Integer a_nxNewLinkID )
{
    CString strSQL;
    strSQL.Format(_T(" Update Transit_Links Set Link_id = '%I64d' Where Link_id = '%I64d' "), a_nxNewLinkID, a_nxOriginLinkID);
    a_spDBaseConnection->ExecuteUpdate(strSQL);
}

Integer KBulkDBaseTransit::TransitLinksFindLinkID( KTarget* a_pTarget, Integer a_nxTransitID, Integer a_nxLinkID )
{
    CString strSQL;
    strSQL.Format(_T("Select Link_ID from transit_links Where Transit_ID = '%I64d' And Link_ID = '%I64d'"), a_nxTransitID, a_nxLinkID); //★ 필요

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    Integer nxLinkID = -1;

    try
    {        
        KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        if ( pResultSet->Next() )
            nxLinkID = pResultSet->GetValueInt(0);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 

    return nxLinkID;
}

void KBulkDBaseTransit::TransitName( KTarget* a_pTarget, std::map<int, CString>& a_mapTransitName )
{
	a_mapTransitName.clear();
	CString strSQL(_T(" Select transit_id, name From Transit "));

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( pResultSet->Next() )
		{
			int  nTransitID = pResultSet->GetValueInt(0);
			CString strName = pResultSet->GetValueString(1);

			a_mapTransitName.insert(std::make_pair(nTransitID, strName));
		}
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}

void KBulkDBaseTransit::TransitName( KTarget* a_pTarget, std::map<Integer, CString>& a_mapTransitName )
{
    a_mapTransitName.clear();
    CString strSQL(_T(" Select transit_id, name From Transit "));

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( pResultSet->Next() )
        {
            Integer nxTransitID = pResultSet->GetValueInt64(0);
            CString strName = pResultSet->GetValueString(1);

            a_mapTransitName.insert(std::make_pair(nxTransitID, strName));
        }
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTransit::TransitMainInfo(KTarget* a_pTarget, std::map<Integer, TTransitMainInfo> &a_mapTransitMainInfo)
{
    a_mapTransitMainInfo.clear();
    CString strSQL(_T(" Select transit_id, type, detail_type, name From Transit "));

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {
        KResultSetPtr  pResultSet = spDBaseConnection->ExecuteQuery(strSQL);

        Integer nxTransitID(0);
        int     nType(0);
        int     nDetailType(0);
        CString strName(_T(""));
        TTransitMainInfo oTTransitMainInfo = {};

        while (pResultSet->Next())
        {
            nxTransitID                   = pResultSet->GetValueInt64(0);
            oTTransitMainInfo.TType       = pResultSet->GetValueInt(1);
            oTTransitMainInfo.TDetailType = pResultSet->GetValueInt(2);
            oTTransitMainInfo.TName       = pResultSet->GetValueString(3);

            a_mapTransitMainInfo.insert(std::make_pair(nxTransitID, oTTransitMainInfo));
        }
    }
    catch (...)
    {
        TxLogDebugWarning();
    }
}

Integer KBulkDBaseTransit::MaxTransitID( KTarget* a_pTarget )
{
    Integer nxMaxTransitID = 0;

    try
    {
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        CString strSQL;
        strSQL.Format(_T("Select ifnull(max(%s), 0) as MaxID From %s "), COLUMN_TRANSIT_ID, TABLE_TRANSIT);
        

        KResultSetPtr    pResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        if ( pResultSet->Next() )
        {
            nxMaxTransitID = pResultSet->GetValueInt64(0);
        }

        return nxMaxTransitID;
    }
    catch (KExceptionPtr ex)
    {
        throw ex;
    }
    catch (...)
    {
    	ThrowException(_T("MaxTransitID"));
    }
    
    return nxMaxTransitID;
}

void KBulkDBaseTransit::Insert(KDBaseConPtr spDBaseConnection, std::vector<TTransitKeyValue>& ar_vecKeyValue)
{
    size_t nxCount = ar_vecKeyValue.size();
    if (nxCount == 0)
        return;

    CString strColumns;
    CString strValues;
    for (size_t i=0; i<nxCount; i++)
    {
        TTransitKeyValue& oKeyValue = ar_vecKeyValue[i];
        strColumns.Append(oKeyValue.TKeyName);

        strValues.Append (_T("'"));
        strValues.Append (oKeyValue.TValue);
        strValues.Append (_T("'"));

        if (i != nxCount-1)
        {
            strColumns.Append(_T(", "));
            strValues.Append (_T(", "));
        }
    }

    CString strInsert;
    strInsert.AppendFormat(_T(" Insert Into %s ( %s ) " ), TABLE_TRANSIT, strColumns);
           strInsert.AppendFormat(_T(" Values  ( %s ) " ), strValues);

    try
    {
        spDBaseConnection->ExecuteUpdate(strInsert);
    }
    catch (KExceptionPtr ex)
    {
        TxLogDebugException();
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        ThrowException(_T("Unknown Exception"));
    }	
}

void KBulkDBaseTransit::UsedLinkInTransit( KTarget* a_pTarget, std::set<KODKey>& ar_setLinkTransitID, std::set<Integer>& ar_setTransitID )
{
    ar_setLinkTransitID.clear();
    ar_setTransitID.clear();

    CString strSQL(_T(""));
    strSQL.Format(_T("Select %s, %s From %s Group By %s, %s"), COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_ID, TABLE_TRANSITLINKS, COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_ID); //★ 필요

    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    try
    {        
        KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            Integer nxLinkID    = spResultSet->GetValueInt64(0);
            Integer nxTransitID = spResultSet->GetValueInt64(1);

            KODKey oKey(nxLinkID, nxTransitID);
            ar_setLinkTransitID.insert(oKey);

            ar_setTransitID.insert(nxTransitID);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugWarning();
    } 
}

void KBulkDBaseTransit::UsedLinkInTransit( KDBaseConPtr a_spDBaseConnection, std::map<Integer, std::set<Integer>>& ar_mapLinkTransitID )
{
    ar_mapLinkTransitID.clear();

    try
    {
    	CString strSQL(_T(""));
        strSQL.Format(_T(" Select %s, %s From %s Group By %s, %s "), COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_ID, TABLE_TRANSITLINKS, COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_ID); //★ 필요
    
        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            Integer nxLinkID    = spResultSet->GetValueInt64(0);
            Integer nxTransitID = spResultSet->GetValueInt64(1);

            std::map<Integer, std::set<Integer>>::iterator iter = ar_mapLinkTransitID.find(nxLinkID);
            std::map<Integer, std::set<Integer>>::iterator end  = ar_mapLinkTransitID.end();

            if (iter != end)
            {
                std::set<Integer>& setTransitID = iter->second;
                setTransitID.insert(nxTransitID);
            }
            else
            {
                std::set<Integer> setTransitID;
                setTransitID.insert(nxTransitID);
                ar_mapLinkTransitID.insert(std::make_pair(nxLinkID, setTransitID));
            }
        }
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At UsedLinkInTransit")); 
    }
}

int KBulkDBaseTransit::TransitExtent( KTarget* pTarget, __int64 nxID, TxEnvelope& oEnv )
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  min(Tx2.x), min(Tx2.y), max(Tx2.x), max(Tx2.y) "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  ( "));
        strSQL.Append(_T("   Select "));
        strSQL.Append(_T("    T2.fnode_id fnode_id, T2.tnode_id tnode_id "));
        strSQL.Append(_T("   From ")); 
        strSQL.Append(_T("    Transit_Links T1, Link T2 "));
        strSQL.Append(_T("   Where ")); 
        if(true) {
            strSQL.AppendFormat(_T("    T1.Transit_ID = '%I64d' And "), nxID);
        }
        strSQL.Append(_T("    T1.Link_ID = T2.Link_ID "));
        strSQL.Append(_T("  ) Tx1, Node Tx2 "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T("  Tx1.fnode_id = Tx2.node_id Or ")); 
        strSQL.Append(_T("  Tx1.tnode_id = Tx2.node_id "));
    }

    KDBaseConPtr spDBase  = pTarget->GetDBaseConnection();
    KResultSetPtr       spResult = spDBase->ExecuteQuery(strSQL);
    if (spResult->Next()) 
    {
        double dMinX = spResult->GetValueDbl(0);
        double dMinY = spResult->GetValueDbl(1);
        double dMaxX = spResult->GetValueDbl(2);
        double dMaxY = spResult->GetValueDbl(3);

        oEnv.SetEnvelope(dMinX, dMinY, dMaxX, dMaxY);
        return 1;
    }
    return -1;
}

void KBulkDBaseTransit::TransitLinksPerson( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, double& ar_dMinValue, double& ar_dMaxValue )
{
    std::map<__int64, double> mapLinkPerson;
    TransitLinksPerson(a_pTarget, mapFTNodeLinkID, a_strColumnName, a_vecTransitID, ar_dMinValue, ar_dMaxValue, mapLinkPerson);
}

void KBulkDBaseTransit::TransitLinksPerson( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, std::map<__int64, double>& mapLinkPerson )
{
    double ar_dMinValue(0), ar_dMaxValue(0);
    TransitLinksPerson(a_pTarget, mapFTNodeLinkID, a_strColumnName, a_vecTransitID, ar_dMinValue, ar_dMaxValue, mapLinkPerson);
}

void KBulkDBaseTransit::TransitLinksPerson( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString a_strColumnName, std::vector<Integer>& a_vecTransitID, double& ar_dSumMinValue, double& ar_dSumMaxValue, std::map<__int64, double>& mapLinkPerson)
{
    mapLinkPerson.clear();
	ar_dSumMinValue = 0.0;
	ar_dSumMaxValue = 0.0;

    size_t nxCount = a_vecTransitID.size();
    if (nxCount < 1)
        return;
        
	try
	{
		CString strSQL(_T("")); {
            strSQL.Format(_T(" Select %s, %s, %s, %s From %s Where %s In "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_ID, COLUMN_TRANSIT_NODE_SEQ, a_strColumnName, TABLE_TRANSITNODES, COLUMN_TRANSIT_ID);
            strSQL.Append(_T(" ( "));
            for(size_t i=0; i<nxCount; i++) {
                if (i == 0) {
                    strSQL.AppendFormat(_T(" '%I64d' "), a_vecTransitID[i]);
                }
                else {
                    strSQL.AppendFormat(_T(", '%I64d' "), a_vecTransitID[i]);
                }            
            }
            strSQL.Append(_T(" ) "));
            strSQL.AppendFormat(_T(" Order By %s, %s "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_SEQ);
        }
               
        //★ 첫번째 정류장에 재차가 있는 경우에 대한 처리
        std::map<__int64, std::vector<TTemp>> mapLinePerson;
		KDBaseConPtr  spDBase  = a_pTarget->GetDBaseConnection();
		KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);

        while (spResult->Next()) {
            __int64 nxLineID = spResult->GetValueI64(0);
            __int64 nxNodeID = spResult->GetValueI64(1);
            int     nSeq     = spResult->GetValueInt(2);
            double  dValue   = spResult->GetValueDbl(3);

            TTemp oInfo; {
                oInfo.TSeq    = nSeq;
                oInfo.TNodeID = nxNodeID;
                oInfo.TValue  = dValue;
            }

            std::vector<TTemp>& vecInfo = mapLinePerson[nxLineID]; {
                vecInfo.push_back(oInfo);
            }
        }

        for (auto iter = mapLinePerson.begin(); iter != mapLinePerson.end(); ++iter) {
            std::vector<TTemp>& vecInfo = iter->second; {
                if (vecInfo.size() < 2)
                    continue;
            }     

            __int64 nxFirstID(0);

            bool bProcess(false); {
                TTemp& oInfo = vecInfo[0];

                if (oInfo.TValue > 0) {
                    bProcess = true;
                }

                nxFirstID = oInfo.TNodeID;
            }

            if (bProcess == false)
                continue;

            std::vector<TTemp> vecOverlap;
            for (size_t i = 0; bProcess == true && i < vecInfo.size(); i++) {
                TTemp& oInfo = vecInfo[i];

                if (oInfo.TNodeID == nxFirstID) {
                    vecOverlap.push_back(oInfo);
                }
            }

            if (vecOverlap.size() <= 1)
                continue;

            //★ 명지대학교 한혁 요청사항(주제도 그릴때 특이사항)
            for (size_t i = vecOverlap.size() - 1; i > 0; i--) {
                TTemp& oInfoPre = vecOverlap[i-1];
                TTemp& oInfoCur = vecOverlap[i];

                oInfoCur.TValue = oInfoPre.TValue;
            }

            TTemp& oFirst = vecOverlap[0]; {
                oFirst.TValue = 0;
            }

            for (size_t i = 0; i < vecOverlap.size(); i++) {
                TTemp& oInfo = vecOverlap[i];
                
                int nIdx = oInfo.TSeq - 1; {
                    TTemp& oChange = vecInfo[nIdx];
                    oChange.TValue = oInfo.TValue;
                }
            }
        }
        
        //★ 링크베이스로 재차를 저장한다.
        for (auto iter = mapLinePerson.begin(); iter != mapLinePerson.end(); ++iter) {
            std::vector<TTemp>& vecInfo = iter->second; {
                if (vecInfo.size() <= 1)
                    continue;
            }     

            for (size_t i = 1; i < vecInfo.size(); i++) {
                TTemp& oInfo    = vecInfo[i];
                TTemp& oInfoPre = vecInfo[i-1]; //★ 0번은 통과했다.
                auto iFind = mapFTNodeLinkID.find(KODKey(oInfoPre.TNodeID, oInfo.TNodeID));
                if (iFind == mapFTNodeLinkID.end())
                    continue; 

                __int64 nxLinkID = iFind->second;
                double& dValue = mapLinkPerson[nxLinkID]; {
                    dValue += oInfo.TValue;
                }
            }
        }

        if (mapLinkPerson.size() == 0)
            return;

        //★ 링크 중심으로 재차인원을 구했다. Min/Max 계산을 진행
        double dMinValue(99999999999); 
        double dMaxValue(0);
        for ( auto iter = mapLinkPerson.begin(); iter != mapLinkPerson.end(); ++iter) {
            double dValue = iter->second;

            if (dMinValue > dValue) {
                dMinValue = dValue;
            }

            if (dMaxValue < dValue) {
                dMaxValue = dValue;
            }
        }		

        ar_dSumMinValue = dMinValue;
        ar_dSumMaxValue = dMaxValue;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KBulkDBaseTransit::TransitLinkBoard( KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, std::map<__int64, double>& mapLinkPerson)
{
    mapLinkPerson.clear();

    if (vecTransitID.size() == 0)
        return;

    class TTemp {
    public:
        TTemp() {
            TNodeID = 0;
            TValue  = 0;
        }
    public:
        __int64 TNodeID;
        double  TValue;
    };

    CString strSQL(_T("")); {
        strSQL.Format(_T(" Select %s, %s, %s, %s From %s Where %s In "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_ID, COLUMN_TRANSIT_NODE_SEQ, strColummn, TABLE_TRANSITNODES, COLUMN_TRANSIT_ID);
        strSQL.Append(_T(" ( "));
        for(size_t i = 0; i < vecTransitID.size(); i++) {
            if (i == 0) {
                strSQL.AppendFormat(_T(" '%I64d' "), vecTransitID[i]);
            }
            else {
                strSQL.AppendFormat(_T(", '%I64d' "), vecTransitID[i]);
            }            
        }
        strSQL.Append(_T(" ) "));
        strSQL.AppendFormat(_T(" Order By %s, %s "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_SEQ);
    }

    std::map<__int64, std::vector<TTemp>> mapLinePerson;

    KDBaseConPtr  spDBae   = pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBae->ExecuteQuery(strSQL);

    while(spResult->Next()) {
        __int64 nxLineID = spResult->GetValueI64(0);
        __int64 nxNodeID = spResult->GetValueI64(1);
        int     nSeq     = spResult->GetValueInt(2);
        double  dValue   = spResult->GetValueDbl(3);

        TTemp oInfo; {
            oInfo.TNodeID = nxNodeID;
            oInfo.TValue  = dValue;
        }

        std::vector<TTemp>& vecInfo = mapLinePerson[nxLineID]; {
            vecInfo.push_back(oInfo);
        }
    }

    //★ 링크베이스로 승차정보를 저장한다.
    for (auto iter = mapLinePerson.begin(); iter != mapLinePerson.end(); ++iter) {
        std::vector<TTemp>& vecInfo = iter->second; {
            if (vecInfo.size() < 2)
                continue;
        }            

        //★ 두개짜리(링크1개) 처리.. 일반적으로 3개 이상의 노드 구성으로 되어있다.
        if (vecInfo.size() == 2) {
            TTemp& oFrom = vecInfo[0];
            TTemp& oTo   = vecInfo[1];

            auto iFind = mapFTNodeLinkID.find(KODKey(oFrom.TNodeID, oTo.TNodeID));
            if (iFind == mapFTNodeLinkID.end())
                continue;

            __int64 nxLinkID = iFind->second;
            double& dValue = mapLinkPerson[nxLinkID]; {
                dValue += oFrom.TValue;
                dValue += oTo.TValue;
            }

            continue;
        }
        
        //★ 승차량 계산
        for (size_t i = 0; i < vecInfo.size()-1; i++) {
            TTemp& oInfo    = vecInfo[i];
            TTemp& oInfoNxt = vecInfo[i+1]; 

            auto iFind = mapFTNodeLinkID.find(KODKey(oInfo.TNodeID, oInfoNxt.TNodeID));
            if (iFind == mapFTNodeLinkID.end())
                continue; 

            __int64 nxLinkID = iFind->second;
            double& dValue = mapLinkPerson[nxLinkID]; {
                dValue += oInfo.TValue;
            }
        }

        //★ 마지막 정류장에 승차가 있으면 -> 이경우는 없지만 안전을 위해서
        TTemp& oInfoLast = vecInfo[vecInfo.size() - 1]; 
        if (oInfoLast.TValue > 0) {
            TTemp& oInfo = vecInfo[0]; 
            if (oInfoLast.TNodeID == oInfo.TNodeID) {
                TTemp& oInfoNxt = vecInfo[1]; 
                auto iFind = mapFTNodeLinkID.find(KODKey(oInfo.TNodeID, oInfoNxt.TNodeID));
                if (iFind == mapFTNodeLinkID.end())
                    continue; 

                __int64 nxLinkID = iFind->second;
                double& dValue = mapLinkPerson[nxLinkID]; {
                    dValue += oInfo.TValue;
                }
            }
        }
    }
}

void KBulkDBaseTransit::TransitLinkAlight( KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, std::map<__int64, double>& mapLinkPerson )
{
    mapLinkPerson.clear();

    if (vecTransitID.size() == 0)
        return;

    class TTemp {
    public:
        TTemp() {
            TNodeID = 0;
            TValue  = 0;
        }
    public:
        __int64 TNodeID;
        double  TValue;
    };

    CString strSQL(_T("")); {
        strSQL.Format(_T(" Select %s, %s, %s, %s From %s Where %s In "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_ID, COLUMN_TRANSIT_NODE_SEQ, strColummn, TABLE_TRANSITNODES, COLUMN_TRANSIT_ID);
        strSQL.Append(_T(" ( "));
        for(size_t i = 0; i < vecTransitID.size(); i++) {
            if (i == 0) {
                strSQL.AppendFormat(_T(" '%I64d' "), vecTransitID[i]);
            }
            else {
                strSQL.AppendFormat(_T(", '%I64d' "), vecTransitID[i]);
            }            
        }
        strSQL.Append(_T(" ) "));
        strSQL.AppendFormat(_T(" Order By %s, %s "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_NODE_SEQ);
    }

    std::map<__int64, std::vector<TTemp>> mapLinePerson;

    KDBaseConPtr  spDBae   = pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBae->ExecuteQuery(strSQL);

    while(spResult->Next()) {
        __int64 nxLineID = spResult->GetValueI64(0);
        __int64 nxNodeID = spResult->GetValueI64(1);
        int     nSeq     = spResult->GetValueInt(2);
        double  dValue   = spResult->GetValueDbl(3);

        TTemp oInfo; {
            oInfo.TNodeID = nxNodeID;
            oInfo.TValue  = dValue;
        }

        std::vector<TTemp>& vecInfo = mapLinePerson[nxLineID]; {
            vecInfo.push_back(oInfo);
        }
    }

    //★ 링크베이스로 하차정보를 저장한다.
    for (auto iter = mapLinePerson.begin(); iter != mapLinePerson.end(); ++iter) {
        std::vector<TTemp>& vecInfo = iter->second; {
            if (vecInfo.size() < 2)
                continue;
        }            

        //★ 두개짜리(링크1개) 처리.. 일반적으로 3개 이상의 노드 구성으로 되어있다.
        if (vecInfo.size() == 2) {
            TTemp& oFrom = vecInfo[0];
            TTemp& oTo   = vecInfo[1];

            auto iFind = mapFTNodeLinkID.find(KODKey(oFrom.TNodeID, oTo.TNodeID));
            if (iFind == mapFTNodeLinkID.end())
                continue;

            __int64 nxLinkID = iFind->second;
            double& dValue = mapLinkPerson[nxLinkID]; {
                dValue += oFrom.TValue;
                dValue += oTo.TValue;
            }

            continue;
        }

        //★ 하차량 계산
        for (size_t i = 1; i < vecInfo.size(); i++) {
            TTemp& oInfoPre = vecInfo[i-1]; 
            TTemp& oInfo    = vecInfo[i];
            
            auto iFind = mapFTNodeLinkID.find(KODKey(oInfoPre.TNodeID, oInfo.TNodeID));
            if (iFind == mapFTNodeLinkID.end())
                continue; 

            __int64 nxLinkID = iFind->second;
            double& dValue = mapLinkPerson[nxLinkID]; {
                dValue += oInfo.TValue;
            }
        }

        //★ 첫번째 정류장에 하차가 있으면 -> 이경우는 없지만 안전을 위해서
        TTemp& oInfoLast = vecInfo[vecInfo.size() - 1]; 
        if (oInfoLast.TValue > 0) {
            TTemp& oInfo = vecInfo[0]; 
            if (oInfoLast.TNodeID == oInfo.TNodeID) {
                TTemp& oInfoPre2 = vecInfo[vecInfo.size() - 1]; 
                auto iFind = mapFTNodeLinkID.find(KODKey(oInfoPre2.TNodeID, oInfo.TNodeID));
                if (iFind == mapFTNodeLinkID.end())
                    continue; 

                __int64 nxLinkID = iFind->second;
                double& dValue = mapLinkPerson[nxLinkID]; {
                    dValue += oInfo.TValue;
                }
            }
        }
    }
}

void KBulkDBaseTransit::TransitBoardAlightMinMax( KTarget* pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, CString strColummn, std::vector<Integer>& vecTransitID, int nFlag, double& ar_dMinValue, double& ar_dMaxValue )
{
    ar_dMinValue = 0;
    ar_dMaxValue = 0;

    //★ nFlag - 0 : Board, 1 : Alight
    std::map<__int64, double> mapLinkPerson;
    if (nFlag == 0) {
        TransitLinkBoard(pTarget, mapFTNodeLinkID, strColummn, vecTransitID, mapLinkPerson);
    }
    else {
        TransitLinkAlight(pTarget, mapFTNodeLinkID, strColummn, vecTransitID, mapLinkPerson);
    }

    double dMinValue(99999999999);
    double dMaxValue(0);
    
    for (auto iter = mapLinkPerson.begin(); iter != mapLinkPerson.end(); ++iter) {
        double dValue = iter->second;

        if (dMinValue > dValue) {
            dMinValue = dValue;
        }

        if (dMaxValue < dValue) {
            dMaxValue = dValue;
        }
    }

    ar_dMinValue = dMinValue;
    ar_dMaxValue = dMaxValue;
}

int KBulkDBaseTransit::IsTransitInLink( KTarget* a_pTarget, Integer a_nxTransitID, Integer a_nxLinkID )
{
    int nMinSeq(-1);

    try
    {        
        CString strSQL;
        strSQL.Format(_T(" Select ifnull(min(%s), -1) as MinSeq From %s Where %s = '%I64d' and %s = '%I64d' "), COLUMN_TRANSIT_LINK_SEQ, TABLE_TRANSITLINKS, COLUMN_TRANSIT_ID, a_nxTransitID, COLUMN_TRANSIT_LINK_ID, a_nxLinkID); //★ 필요
        
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr    pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

        if ( pResultSet->Next() )
        {
            nMinSeq = pResultSet->GetValueInt(0);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return nMinSeq;
}

int KBulkDBaseTransit::GetTransitMode( KTarget* a_pTarget, CString a_strDetailType )
{
    int nResult(TRANSIT_MODE_BUS);
    try
    {
        CString strSQL;
        strSQL.Format(_T(" Select mode From %s Where type = '%s' "), TABLE_TRANSIT_COST, a_strDetailType);
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr    pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
        if ( pResultSet->Next() )
        {
            nResult = pResultSet->GetValueInt(0);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

    return nResult;
}


void KBulkDBaseTransit::TotalTransitCount( KTarget* a_pTarget, int& a_nTotalTransitCount )
{
	TxLogDebugStartMsg();
	a_nTotalTransitCount = 0;

	CString strSQL(_T(""));
	strSQL.Format(_T("Select count(*) From %s"), TABLE_TRANSIT);
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);

		if ( pResultSet->Next() )
			a_nTotalTransitCount = pResultSet->GetValueInt(0);
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
}


void KBulkDBaseTransit::TotalTransitLineCount( KTarget* a_pTarget, int& a_nTotalLineCount )  
{
	TxLogDebugStartMsg();
	a_nTotalLineCount = 0;

	CString strSQL(_T(""));
	strSQL.Format(_T("Select count(*) From %s"), TABLE_TRANSITLINKS); //★ 필요
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);

		if ( pResultSet->Next() )
			a_nTotalLineCount = pResultSet->GetValueInt(0);
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
}


void KBulkDBaseTransit::UsedTransitBusType(KTarget* a_pTarget, std::vector<TTransitTypeName> &a_vecBusType)
{
	a_vecBusType.clear();

	CString strSQL;
	strSQL.Append(      _T(" SELECT TBL1.detail_type, TBL2.KCode_Display "));
	strSQL.Append(      _T(" FROM "));
	strSQL.AppendFormat(_T(" (SELECT detail_type FROM %s WHERE type = %d GROUP BY detail_type)TBL1 "), TABLE_TRANSIT, TRANSIT_MODE_BUS);
	strSQL.Append(      _T(" LEFT OUTER JOIN "));
	strSQL.AppendFormat(_T(" (SELECT KCode_Key, KCode_Display FROM %s WHERE KCode_Group_Key = %d) TBL2 "), TABLE_KCODE_DEFINE, TRANSIT_TYPE_CODE_KEY);
	strSQL.Append(      _T(" ON TBL1.detail_type = TBL2.KCode_Key "));
	strSQL.Append(      _T(" ORDER BY TBL1.detail_type "));

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  pResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( pResultSet->Next() )
		{
			TTransitTypeName oTTransitTypeName;

			oTTransitTypeName.nType   = pResultSet->GetValueInt   (0);
			oTTransitTypeName.strName = pResultSet->GetValueString(1);

			a_vecBusType.push_back(oTTransitTypeName);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugWarning();
	} 
}


void KBulkDBaseTransit::TransitAllIdCaption( KTarget* a_pTarget, std::vector<KIDCaption> &a_vecLineIDCaption, std::map<Integer, CString> &a_mapLineData )
{
	TxLogDebugStartMsg();

	a_vecLineIDCaption.clear();
	a_mapLineData.clear();

	CString strSQL(_T(""));
	strSQL.Format(_T(" SELECT transit_id, name FROM %s "), TABLE_TRANSIT);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	Integer nxLineID(0);
	CString strLineName(_T(""));

	try
	{        
		KResultSetPtr  spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLineID(_T(""));
		while ( spResultSet->Next() )
		{
			nxLineID    = spResultSet->GetValueInt64 (0);
			strLineName = spResultSet->GetValueString(1);

			KIDCaption oIDCaption;
			strLineID.Format(_T("%I64d"), nxLineID);
			oIDCaption.TID      = strLineID;
			oIDCaption.TCaption = strLineName;

			a_vecLineIDCaption.push_back(oIDCaption);

			a_mapLineData.insert(std::make_pair(nxLineID, strLineName));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_vecLineIDCaption.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_vecLineIDCaption.clear();
	} 

	TxLogDebugEndMsg();
}

bool KBulkDBaseTransit::ExistTransit( KDBaseConPtr a_spDBaseConnection )
{
    try
    {
        int nCount(0);
        CString strSQL(_T(""));
        strSQL.Format(_T(" SELECT count(*) as cnt, name FROM %s "), TABLE_TRANSIT);

        KResultSetPtr  spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        if (spResultSet->Next())
            nCount = spResultSet->GetValueInt(0);

        if (nCount > 0)
            return true;

        return false;
    }
    catch (KExceptionPtr ex)
    {
    	TxExceptionPrint(ex);
    	throw ex;
    }
    catch (...)
    {
    	TxLogDebugException();
    	ThrowException(_T("At ExistTransit")); 
    }
}

void KBulkDBaseTransit::TransitAllIdCaptionType( KTarget* a_pTarget, std::vector<KIDCaption> &a_vecLineIDCaption, std::map<Integer, TTransitModeName> &a_mapLineData )
{
	TxLogDebugStartMsg();

	a_vecLineIDCaption.clear();
	a_mapLineData.clear();

	CString strSQL(_T(""));
	strSQL.Format(_T(" SELECT transit_id, type, detail_type, name FROM %s "), TABLE_TRANSIT);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	Integer nxLineID(0);
	CString strLineName(_T(""));
	int		nType;
	int		nMode;

	try
	{        
		KResultSetPtr  spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLineID(_T(""));
		while ( spResultSet->Next() )
		{
			nxLineID    = spResultSet->GetValueInt64 (0);
			nMode		= spResultSet->GetValueInt(1);
			nType		= spResultSet->GetValueInt(2);
			strLineName = spResultSet->GetValueString(3);

			KIDCaption oIDCaption;
			strLineID.Format(_T("%I64d"), nxLineID);
			oIDCaption.TID      = strLineID;
			oIDCaption.TCaption = strLineName;

			a_vecLineIDCaption.push_back(oIDCaption);

			TTransitModeName oInfo = {};
			oInfo.nMode = nMode;
			oInfo.nType = nType;
			oInfo.strName = strLineName;

			a_mapLineData.insert(std::make_pair(nxLineID, oInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_vecLineIDCaption.clear();
	}
	catch (...)
	{
		TxLogDebugWarning();
		a_vecLineIDCaption.clear();
	} 

	TxLogDebugEndMsg();
}

void KBulkDBaseTransit::TransitLinkInfo( KTarget* a_pTarget, Integer a_nxTransitID, std::vector<TTransitLinkInfo>& a_vecTransitLinkInfo )
{
	TxLogDebugStartMsg();

	a_vecTransitLinkInfo.clear();

	CString strSQL;
	strSQL.Format(_T("Select Link_ID, Seq, Station_YN From Transit_Links Where Transit_ID = '%I64d' Order By Seq"), a_nxTransitID);

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr    pResultSet     = spDBaseConnection->ExecuteQuery(strSQL);

		TTransitLinkInfo   oTransitLinkInfo;
		while ( pResultSet->Next() )
		{
			oTransitLinkInfo.nxTransitID	= a_nxTransitID;
			oTransitLinkInfo.nxLinkID		= pResultSet->GetValueInt64 (0);
			oTransitLinkInfo.nSeq			= pResultSet->GetValueInt   (1);
			oTransitLinkInfo.nStaionYN		= pResultSet->GetValueInt   (2);       

			a_vecTransitLinkInfo.push_back(oTransitLinkInfo);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugWarning();
		throw 1;
	} 

	TxLogDebugEndMsg();
}


