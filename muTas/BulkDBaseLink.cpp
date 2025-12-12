#include "StdAfx.h"
#include "BulkDBaseLink.h"
//^#include "DBaseConnector.h"
#include "Target.h"

//#include "CodeGroup.h"
//#include "CodeManager.h"

KBulkDBaseLink::KBulkDBaseLink( void )
{

}

KBulkDBaseLink::~KBulkDBaseLink( void )
{

}

void KBulkDBaseLink::LinkIDLabelQuery( KTarget* a_pTarget, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo )
{
    TxLogDebugStartMsg();

    a_vecDrawLabelInfo.clear();

    CString strSQL(_T("Select link_id From Link "));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        TDrawLabelInfo   oDrawLabelInfo;
        while ( spResultSet->Next() )
        {
            oDrawLabelInfo.TFeatureID = spResultSet->GetValueInt64(0);
            oDrawLabelInfo.TLabel.Format(_T("%I64d"), oDrawLabelInfo.TFeatureID);

            a_vecDrawLabelInfo.push_back(oDrawLabelInfo);
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

void KBulkDBaseLink::LinkLabelQuery( KTarget* a_pTarget, KIOColumns& a_columnCollection, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo, CString  a_strWhere )
{
    TxLogDebugStartMsg();

    a_vecDrawLabelInfo.clear();
        
    KCodeManager* pCodeMgr     = a_pTarget->CodeManager();
    int           nColumnCount = a_columnCollection.ColumnCount();

    CString strSQL(_T(""));

    #pragma region strSQL
    strSQL  = _T(" Select link_id ");

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL =   strSQL +  _T(", ") + a_columnCollection.GetColumn(i)->Name() + _T(" ");
    }

    strSQL += _T(" From Link ");

    if ( a_strWhere.GetLength() > 0 )
    {
        strSQL = strSQL + _T(" Where ") + a_strWhere;
    }
    #pragma endregion strSQL
        
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        TDrawLabelInfo   oDrawLabelInfo;
        while ( spResultSet->Next() )
        {
            oDrawLabelInfo.TFeatureID = spResultSet->GetValueInt64(0);
            CString strLabel(_T(""));
            CString strValue(_T(""));

            for (int i=0; i<nColumnCount; i++)
            {
                KIOColumn*  pColumn = a_columnCollection.GetColumn(i);
                if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
		        {			        
			        KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( pColumn->CodeGroup() );
			        if( NULL == pCodeGroup )
                    {
				        strValue = spResultSet->GetValueString(i+1);
                    }
                    else
                    {
                        strValue = pCodeGroup->SingleCodeValue(spResultSet->GetValueInt(i+1), 1);
                    }       
		        }
		        else if(pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		        {
			        KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup( pColumn->CodeGroup() );

                    if( NULL == pCodeGroup )
                    {
                        strValue = spResultSet->GetValueString(i+1);
                    }
                    else
                    {
                        strValue = pCodeGroup->MultiCodeValue(spResultSet->GetValueString(i+1), 1);
                    }	        
		        }
		        else
		        {
			        if ( pColumn->DataType() == KEMIODataTypeDouble)
                    {
                        strValue.Format(_T("%.1f"), spResultSet->GetValueDouble(i+1));
                    }
                    else
                    {
                        strValue = spResultSet->GetValueString(i+1);
                    }
		        }    

                if (strLabel.GetLength() == 0)
                {
                    strLabel = strValue;
                }
                else
                {
                    strLabel = strLabel + _T("-") + strValue;
                }			
            }

            oDrawLabelInfo.TLabel = strLabel;
            a_vecDrawLabelInfo.push_back(oDrawLabelInfo);
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


Integer KBulkDBaseLink::MaxLinkID( KDBaseConPtr a_spDBaseConnection, Integer a_nxStartID )
{
    CString strSQL(_T(""));
    Integer nxMaxLinkID = a_nxStartID-1;
    if (nxMaxLinkID < 0)
        nxMaxLinkID = 0;
    
    strSQL.Format(_T(" Select ifnull(max(link_id), '%I64d') as MaxID From Link Where link_id >= '%I64d' "), nxMaxLinkID, nxMaxLinkID);

    KResultSetPtr    spResultSet     = a_spDBaseConnection->ExecuteQuery(strSQL);
    if ( spResultSet->Next() )
    {
        nxMaxLinkID = spResultSet->GetValueInt64(0);
    }

    return nxMaxLinkID;
}

Integer KBulkDBaseLink::MaxLinkID( KTarget* a_pTarget, Integer a_nxStartID )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    Integer nxMaxLinkID = MaxLinkID(spDBaseConnection, a_nxStartID);

    return nxMaxLinkID;
}
/*
*
*struct TFixedLink
{
Integer link_id;
Integer fnode_id;
Integer tnode_id;
int     link_type;
double  length;
int     lane;
double	capa;
double  speed;
};
 **/
void KBulkDBaseLink::Insert( KDBaseConPtr a_spDBaseConnection, TFixedLink a_oTFixedLink )
{
	try
	{
		CString strInsert = _T("");
		strInsert.Format(_T("Insert Into Link  (link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed ) Values(%I64d, %I64d, %I64d, %d, %f, %d, %f, %f) "),
			a_oTFixedLink.link_id,
			a_oTFixedLink.fnode_id,
			a_oTFixedLink.tnode_id,
			a_oTFixedLink.link_type,
			a_oTFixedLink.length,
			a_oTFixedLink.lane, 
			a_oTFixedLink.capa,
			a_oTFixedLink.speed);

		a_spDBaseConnection->ExecuteUpdate(strInsert);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseLink::Insert(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkKeyValue>& ar_vecKeyValue)
{
	size_t nxCount = ar_vecKeyValue.size();
	if (nxCount == 0)
		return;

	CString strColumns;
	CString strValues;
	for (size_t i=0; i<nxCount; i++)
	{
		TLinkKeyValue& oLinkKeyValue = ar_vecKeyValue[i];
		strColumns.Append(oLinkKeyValue.TKeyName);

		strValues.Append (_T("'"));
		strValues.Append (oLinkKeyValue.TValue);
		strValues.Append (_T("'"));

		if (i != nxCount-1)
		{
			strColumns.Append(_T(", "));
			strValues.Append (_T(", "));
		}
	}

	CString strInsert;
	strInsert.Append(_T(" Insert Into Link ( " ));
	strInsert.Append(              strColumns  ) ;
	strInsert.Append(_T(                  ")"  ));
	strInsert.Append(_T(" Values ( "           ));
	strInsert.Append(              strValues   ) ;
	strInsert.Append(_T(        ")"            ));

	try
	{
		a_spDBaseConnection->ExecuteUpdate(strInsert);
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

bool KBulkDBaseLink::GetLink( KDBaseConPtr a_spDBaseConnection, Integer nxLinkID, TFixedLink& a_oTFixedLink )
{
	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed From Link Where Link_ID = %I64d"), nxLinkID);

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
		if (spResultSet->Next())
		{
			a_oTFixedLink.link_id   = spResultSet->GetValueInt64 (0);
			a_oTFixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
			a_oTFixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
			a_oTFixedLink.link_type = spResultSet->GetValueInt   (3);
			a_oTFixedLink.length    = spResultSet->GetValueDouble(4);
			a_oTFixedLink.lane      = spResultSet->GetValueInt   (5);
			a_oTFixedLink.capa      = spResultSet->GetValueDouble(6);
			a_oTFixedLink.speed     = spResultSet->GetValueDouble(7);
		}
		else
		{
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

    return true;
}

void KBulkDBaseLink::DeleteLink( KDBaseConPtr a_spDBaseConnection, Integer nxLinkID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Delete From link Where link_id = '%I64d' "), nxLinkID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseLink::FindConnectLink( KDBaseConPtr spDBaseConnection, Integer nxNodeID, std::vector<Integer>& ar_vecConnectLink )
{
    ar_vecConnectLink.clear();

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select link_id From Link Where Fnode_id = %I64d or Tnode_id = %I64d"), nxNodeID, nxNodeID);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResultSet->Next())
		{
			Integer nxLinkID = spResultSet->GetValueInt64 (0);
			ar_vecConnectLink.push_back(nxLinkID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ar_vecConnectLink.clear();
	}
}

void KBulkDBaseLink::ReplaceNodeID( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T(" Update Link Set Fnode_id = '%I64d' Where Fnode_id ='%I64d' "), a_nxReplaceID, a_nxOriginNodeID);
		a_spDBaseConnection->ExecuteUpdate(strSQL);

		strSQL.Format(_T(" Update Link Set Tnode_id = '%I64d' Where Tnode_id ='%I64d' "), a_nxReplaceID, a_nxOriginNodeID);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

bool KBulkDBaseLink::ExistLink( KTarget* a_pTarget, Integer a_nxLinkID )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    int nCount = 0;

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select count(*) as Cnt From Link Where link_id = '%I64d' "), a_nxLinkID);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			nCount = spResultSet->GetValueInt(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

    if (nCount == 0)
    {
        return false;
    }

    return true;
}

bool KBulkDBaseLink::ExistLink( KDBaseConPtr a_spDBaseConnection )
{
    try
    {
        int nCount(0);
        CString strSQL(_T(""));
        strSQL.Format(_T(" SELECT count(*) as cnt, name FROM %s "), TABLE_LINK);

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
        ThrowException(_T("At ")); 
    }
}

__int64 KBulkDBaseLink::FindLink( KDBaseConPtr spDBase, __int64 nxFNode, __int64 nxTNode )
{
    try
    {
        CString strSQL(_T("")); {
            strSQL.AppendFormat(_T("Select link_id From Link Where fnode_id = '%I64d' And tnode_id = '%I64d'"), nxFNode, nxTNode);
        }

        KResultSetPtr  spResultSet = spDBase->ExecuteQuery(strSQL);
        if (spResultSet->Next()) {
            __int64 nxID = spResultSet->GetValueI64(0);
            return nxID;
        }
    }
    catch(...) {
        TxLogDebugException();
    }
    
    return -1;
}

void KBulkDBaseLink::InsertDefaultLink( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, Integer a_nxFromNodeID, Integer a_nxToNodeID, int a_nType, double a_dLength )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T(" Insert Into Link(link_id, fnode_id, tnode_id, link_type, length) Values ('%I64d', '%I64d', '%I64d', '%d', '%f')"), 
			a_nxLinkID, a_nxFromNodeID, a_nxToNodeID, a_nType, a_dLength);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseLink::SearchNextLinks( KTarget* a_pTarget, CString a_strLinkID, std::vector<Integer>& ar_vecNextLink )
{
	ar_vecNextLink.clear();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(_T("Select link_id From ") 
			_T("( ")
			_T("  Select fnode_id as FNode, tnode_id as TNode, link_id ")
			_T("  From Link ") 
			_T("  Where fnode_id = ( ")
			_T("  Select max(tnode_id) from link where link_id = '%s') ")
			_T(") ") 
			_T(" Where ")
			_T("  TNode not in ")
			_T("  ( select max(fnode_id) from link where link_id = '%s' ) ")
			, a_strLinkID, a_strLinkID
			);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			Integer    nxLinkID = spResultSet->GetValueInt64(0); 
			ar_vecNextLink.push_back(nxLinkID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		ar_vecNextLink.clear();
	}
}

void KBulkDBaseLink::SearchNextLinks( KTarget* a_pTarget, CString a_strLinkID, Integer& ar_nxIntersectionID, std::vector<Integer>& ar_vecNextLink )
{
    ar_vecNextLink.clear();
    ar_nxIntersectionID = -1;

	CString strSQL;
	strSQL.Format(_T("Select fnode_id, link_id From Link Where fnode_id = (select max(tnode_id) from link where link_id = '%s') Order By link_id "), a_strLinkID);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			ar_nxIntersectionID = spResultSet->GetValueInt64(0);
			Integer    nxLinkID = spResultSet->GetValueInt64(1);        
			ar_vecNextLink.push_back(nxLinkID);
		}
	}
	catch (...)
	{
		TxLogDebug((LPCTSTR) strSQL);
		ar_vecNextLink.clear();
	}
}


void KBulkDBaseLink::LinkFTNodeGeometry( KTarget* a_pTarget, std::map<Integer, TLinkFTNodeXY>& a_mapLinkFTNode )
{
    a_mapLinkFTNode.clear();

    CString strSQL;
    strSQL += _T("Select ");
    strSQL += _T(" link_id, fnode_id, ");
    strSQL += _T(" (select x from node where node_id = fnode_id) as FromX, "); 
    strSQL += _T(" (select y from node where node_id = fnode_id) as FromY, "); 
    strSQL += _T(" tnode_id, ");
    strSQL += _T(" (select x from node where node_id = tnode_id) as ToX  , ");
    strSQL += _T(" (select y from node where node_id = tnode_id) as ToY    ");    
    strSQL += _T("From link");

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{        
			Integer     nxLinkID    = spResultSet->GetValueInt64 (0);

			TLinkFTNodeXY oLinkFTNode;
			oLinkFTNode.TFromNodeID = spResultSet->GetValueInt64 (1);
			oLinkFTNode.TFromNodeX  = spResultSet->GetValueDouble(2);
			oLinkFTNode.TFromNodeY  = spResultSet->GetValueDouble(3);

			oLinkFTNode.TToNodeID   = spResultSet->GetValueInt64 (4);
			oLinkFTNode.TToNodeX    = spResultSet->GetValueDouble(5);
			oLinkFTNode.TToNodeY    = spResultSet->GetValueDouble(6);

			a_mapLinkFTNode.insert(std::make_pair(nxLinkID, oLinkFTNode));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapLinkFTNode.clear();
	}
}

bool KBulkDBaseLink::LinkFTNodeGeometry( KTarget* a_pTarget, Integer a_nxLinkID, TLinkFTNodeXY& ar_oLinkFTNode )
{
    bool bReturn(false);

    try
    {
        CString strSQL;
        strSQL += _T(" Select ");
        strSQL += _T("  link_id, fnode_id, ");
        strSQL += _T("  (select x from node where node_id = fnode_id) as FromX, "); 
        strSQL += _T("  (select y from node where node_id = fnode_id) as FromY, "); 
        strSQL += _T("  tnode_id, ");
        strSQL += _T("  (select x from node where node_id = tnode_id) as ToX  , ");
        strSQL += _T("  (select y from node where node_id = tnode_id) as ToY    ");    
        strSQL += _T(" From link ");
        strSQL.AppendFormat(_T(" Where link_id = '%I64d' "), a_nxLinkID);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        if ( spResultSet->Next() )
        {        
            Integer     nxLinkID       = spResultSet->GetValueInt64 (0);
            ar_oLinkFTNode.TFromNodeID = spResultSet->GetValueInt64 (1);
            ar_oLinkFTNode.TFromNodeX  = spResultSet->GetValueDouble(2);
            ar_oLinkFTNode.TFromNodeY  = spResultSet->GetValueDouble(3);

            ar_oLinkFTNode.TToNodeID   = spResultSet->GetValueInt64 (4);
            ar_oLinkFTNode.TToNodeX    = spResultSet->GetValueDouble(5);
            ar_oLinkFTNode.TToNodeY    = spResultSet->GetValueDouble(6);
            bReturn = true;
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

    return bReturn;
}

bool KBulkDBaseLink::GetToNodeID( KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxToNodeID )
{
	CString strSQL;
	strSQL.Format(_T("Select tnode_id From Link Where link_id = '%I64d' "), a_nxLinkID);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{        
			ar_nxToNodeID    = spResultSet->GetValueInt64 (0);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return false;
}

bool KBulkDBaseLink::GetFromNodeID( KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxFromNodeID )
{
	CString strSQL;
	strSQL.Format(_T("Select fnode_id From Link Where link_id = '%I64d' "), a_nxLinkID);
	
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{        
			ar_nxFromNodeID = spResultSet->GetValueInt64 (0);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return false;
}

bool KBulkDBaseLink::GetFTNodeID( KTarget* a_pTarget, Integer a_nxLinkID, Integer& ar_nxFromNodeID, Integer& ar_nxToNodeID )
{
    CString strSQL;
    strSQL.Format(_T("Select fnode_id, tnode_id From Link Where link_id = '%I64d' "), a_nxLinkID);

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{        
			ar_nxFromNodeID    = spResultSet->GetValueInt64 (0);
			ar_nxToNodeID      = spResultSet->GetValueInt64 (1);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

    return false;
}

void KBulkDBaseLink::AllLinkValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue )
{
	try
	{
		ar_vecValue.clear();		

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format(_T("Select %s From Link"), a_strColumnName);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			ar_vecValue.push_back(spResultSet->GetValueDouble(0));
		}
	}
	catch(...)
	{
		ar_vecValue.clear();	
		TxLogDebugException();
	}
}

void KBulkDBaseLink::AllLinkValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TLinkIDValue>& ar_vecValue )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select link_id, %s From Link Order by %s Desc"), a_strColumnName, a_strColumnName);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TLinkIDValue oTLinkIDValue;
			oTLinkIDValue.TLinkID = spResultSet->GetValueInt64(0);
			oTLinkIDValue.TValue  = spResultSet->GetValueDouble(1);
			ar_vecValue.push_back(oTLinkIDValue);
		}
	}
	catch(...)
	{
		ar_vecValue.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseLink::SrchLinkValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue, CString a_strFilter )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From Link %s"), a_strColumnName, a_strFilter);

		if (!a_strFilter.IsEmpty())
			strSQL.Append(_T(" And"));
		else
			strSQL.Append(_T(" Where"));

		strSQL.AppendFormat(_T(" %s IS NULL or %s < 9e999"), a_strColumnName, a_strColumnName);  // "< 9e999" 조건만 하면 null 값은 나오지 않는다.

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			ar_vecValue.push_back(spResultSet->GetValueDouble(0));
		}
	}
	catch(...)
	{
		ar_vecValue.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseLink::SrchLinkValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TLinkIDValue>& ar_vecValue, CString a_strFilter )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select link_id, %s From Link %s Order by %s Desc"), a_strColumnName, a_strFilter, a_strColumnName);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TLinkIDValue oTLinkIDValue;
			oTLinkIDValue.TLinkID = spResultSet->GetValueInt64(0);
			oTLinkIDValue.TValue  = spResultSet->GetValueDouble(1);
			ar_vecValue.push_back(oTLinkIDValue);
		}
	}
	catch(...)
	{
		ar_vecValue.clear();	
		TxLogDebugException();
	}
}

void KBulkDBaseLink::SearchLinksByToNode( KTarget* a_pTarget, Integer a_nxToNodeID, std::vector<Integer>& ar_vecLink )
{
	try
	{
		ar_vecLink.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T(" Select %s From %s Where %s = '%I64d' "), COLUMN_LINK_ID, TABLE_LINK, COLUMN_TNODE_ID, a_nxToNodeID);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			ar_vecLink.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch(...)
	{
		ar_vecLink.clear();		
		TxLogDebugException();
	}
}

void KBulkDBaseLink::SearchLinksByFromNode( KTarget* a_pTarget, Integer a_nxFromNodeID, std::vector<Integer>& ar_vecLink )
{
	try
	{
		ar_vecLink.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T(" Select %s From %s Where %s = '%I64d' "), COLUMN_LINK_ID, TABLE_LINK, COLUMN_FNODE_ID, a_nxFromNodeID);

		KResultSetPtr  spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
		{
			ar_vecLink.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch(...)
	{
		ar_vecLink.clear();		
		TxLogDebugException();
	}
}

void KBulkDBaseLink::UpdateLinkLength( KDBaseConPtr a_spDBaseConnection, Integer a_nxLinkID, double a_dLength )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T(" Update %s Set %s = '%f' Where %s ='%I64d' "), TABLE_LINK, COLUMN_LINK_LENGTH, a_dLength, COLUMN_LINK_ID, a_nxLinkID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseLink::AllLinkFTNode( KTarget* a_pTarget, std::map<Integer, TLinkFTNodeID>& a_mapLinkFTNode )
{
    try
    {
        a_mapLinkFTNode.clear();		
        CString strSQL;
        strSQL.Format(_T(" Select %s, %s, %s From %s "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, TABLE_LINK);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TLinkFTNodeID oTLinkFTNodeID;
            
            Integer nxLinkID(spResultSet->GetValueInt64(0));
            oTLinkFTNodeID.TLinkID     = nxLinkID;
            oTLinkFTNodeID.TFromNodeID = spResultSet->GetValueInt64(1);
            oTLinkFTNodeID.TToNodeID   = spResultSet->GetValueInt64(2);

            a_mapLinkFTNode.insert(std::make_pair(nxLinkID, oTLinkFTNodeID));
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KBulkDBaseLink::AllLinkFTNodeType( KTarget* a_pTarget, std::map<Integer, TLinkFTNodeType>& a_mapLinkFTNodeType )
{
    try
    {
        a_mapLinkFTNodeType.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T(" Select %s, %s, %s, %s From %s "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_TYPE, TABLE_LINK);

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TLinkFTNodeType oTLinkFTNodeType;

            Integer nxLinkID(spResultSet->GetValueInt64(0));
            
            oTLinkFTNodeType.TFromNodeID = spResultSet->GetValueInt64(1);
            oTLinkFTNodeType.TToNodeID   = spResultSet->GetValueInt64(2);
            oTLinkFTNodeType.TType       = spResultSet->GetValueInt  (3);

            a_mapLinkFTNodeType.insert(std::make_pair(nxLinkID, oTLinkFTNodeType));
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KBulkDBaseLink::FTLinkMap( KTarget* a_pTarget, std::map<KODKey, Integer>& ar_mapFTLink )
{
    try
    {
        ar_mapFTLink.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T(" Select %s, %s, %s From %s "), COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_ID, TABLE_LINK);

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {            
            Integer nxFromNodeID(spResultSet->GetValueInt64(0));
            Integer nxToNodeID  (spResultSet->GetValueInt64(1));
            Integer nxLinkID    (spResultSet->GetValueInt64(2));

            KODKey  oKey(nxFromNodeID, nxToNodeID);
            ar_mapFTLink.insert(std::make_pair(oKey, nxLinkID));
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KBulkDBaseLink::FTLinkMap( KTarget* a_pTarget, std::map<KODKey, std::set<Integer>>& ar_mapFTLink )
{
    try
    {
        ar_mapFTLink.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T(" Select %s, %s, %s From %s "), COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_ID, TABLE_LINK);
        
        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {            
            Integer nxFromNodeID(spResultSet->GetValueInt64(0));
            Integer nxToNodeID  (spResultSet->GetValueInt64(1));
            Integer nxLinkID    (spResultSet->GetValueInt64(2));

            KODKey  oKey(nxFromNodeID, nxToNodeID);

            std::map<KODKey, std::set<Integer>>::iterator iter = ar_mapFTLink.find(oKey);
            std::map<KODKey, std::set<Integer>>::iterator end  = ar_mapFTLink.end();

            if (iter == end)
            {
                std::set<Integer> setLink;
                setLink.insert(nxLinkID);
                ar_mapFTLink.insert(std::make_pair(oKey, setLink));
            }
            else
            {
                std::set<Integer>& setLink = iter->second;
                setLink.insert(nxLinkID);
            }            
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }
}

void KBulkDBaseLink::AllTrasitLineSeq( KTarget* a_pTarget, std::map<Integer, std::vector<Integer>> &a_mapLineSeq )
{
	try
	{
		a_mapLineSeq.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T(" SELECT %s, %s, %s FROM %s ORDER BY %s, %s "), COLUMN_TRANSIT_ID, COLUMN_TRANSIT_LINK_ID, COLUMN_TRANSIT_LINK_SEQ, TABLE_TRANSITLINKS, COLUMN_TRANSIT_ID, COLUMN_TRANSIT_LINK_SEQ); //★ 필요

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			Integer nxTransitID(spResultSet->GetValueInt64(0));
			Integer nxLinkID   (spResultSet->GetValueInt64(1));
			Integer nSeq       (spResultSet->GetValueInt(2));

			AutoType iter  = a_mapLineSeq.find(nxTransitID);
			AutoType itEnd = a_mapLineSeq.end();

			if(iter != itEnd)
			{
				std::vector<Integer> &vecLinks = iter->second;
				vecLinks.push_back(nxLinkID);
			}
			else
			{
				std::vector<Integer> vecLinks;
				vecLinks.push_back(nxLinkID);
				a_mapLineSeq.insert(std::make_pair(nxTransitID, vecLinks));
			}
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KBulkDBaseLink::AllLinkGroupByType( KTarget* a_pTarget, std::vector<int>& a_vecLinkType )
{
	a_vecLinkType.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strQuery = _T("select link_type from link group by link_type order by link_type");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int nTypeGroup = spResultSet->GetValueInt(0);
			a_vecLinkType.push_back(nTypeGroup);
		}
	}
	catch (...)
	{
		a_vecLinkType.clear();
		TxLogDebugException();
	}
}


void KBulkDBaseLink::Links( KTarget* a_pTarget, std::map<Integer, TFCompareLinkField>& ar_mapLinks )
{
	ar_mapLinks.clear();
	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Select %s, %s, %s, "), COLUMN_LINK_ID,    COLUMN_FNODE_ID,   COLUMN_TNODE_ID);
		strSQL.AppendFormat(_T("    %s, %s, %s, %s, "), COLUMN_LINK_NAME,  COLUMN_LINK_TYPE,  COLUMN_LINK_LENGTH, COLUMN_LINK_LANE);
		strSQL.AppendFormat(_T("    %s, %s, %s, %s, "), COLUMN_LINK_CAPA,  COLUMN_LINK_SPEED, COLUMN_LINK_SURVEYVOLUME, COLUMN_LINK_FN_ID);
		strSQL.AppendFormat(_T("    %s, %s, %s, %s  "), COLUMN_LINK_ALPHA, COLUMN_LINK_BETA,  COLUMN_LINK_A,      COLUMN_LINK_B);
		strSQL.AppendFormat(_T(" From %s "), TABLE_LINK);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			Integer nxLinkID = spResultSet->GetValueInt64(0);
			TFCompareLinkField oCompareLinkField;
			oCompareLinkField.TnxLinkID  = nxLinkID;
			oCompareLinkField.TnxFNodeID = spResultSet->GetValueInt64 (1);
			oCompareLinkField.TnxTNodeID = spResultSet->GetValueInt64 (2);
			oCompareLinkField.TstrName   = spResultSet->GetValueString(3);
			oCompareLinkField.TnLinkType = spResultSet->GetValueInt   (4);
			oCompareLinkField.TdLength   = spResultSet->GetValueDouble(5);
			oCompareLinkField.TdLane     = spResultSet->GetValueInt   (6);
			oCompareLinkField.TdCapa     = spResultSet->GetValueDouble(7);
			oCompareLinkField.TdSpeed    = spResultSet->GetValueDouble(8);
			oCompareLinkField.TdSurveyVolume = spResultSet->GetValueDouble(9);
			oCompareLinkField.TnFunctionID = spResultSet->GetValueInt (10);
			oCompareLinkField.TdAlpha    = spResultSet->GetValueDouble(11);
			oCompareLinkField.TdBeta     = spResultSet->GetValueDouble(12);
			oCompareLinkField.TdA        = spResultSet->GetValueDouble(13);
			oCompareLinkField.TdB        = spResultSet->GetValueDouble(14);

			ar_mapLinks.insert(std::make_pair(nxLinkID, oCompareLinkField));
		}
	}
	catch (...)
	{
		ar_mapLinks.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseLink::Links( KTarget* a_pTarget, std::map<Integer, TCompareLinkField>& ar_mapLinks )
{
    ar_mapLinks.clear();
    try
    {
        CString strSQL(_T(""));
        strSQL.AppendFormat(_T(" Select %s, %s, %s, "), COLUMN_LINK_ID,    COLUMN_FNODE_ID,   COLUMN_TNODE_ID);
        strSQL.AppendFormat(_T("    %s, %s, %s, %s, "), COLUMN_LINK_NAME,  COLUMN_LINK_TYPE,  COLUMN_LINK_LENGTH, COLUMN_LINK_LANE);
        strSQL.AppendFormat(_T("    %s, %s, %s, %s, "), COLUMN_LINK_CAPA,  COLUMN_LINK_SPEED, COLUMN_LINK_SURVEYVOLUME, COLUMN_LINK_FN_ID);
        strSQL.AppendFormat(_T("    %s, %s, %s, %s  "), COLUMN_LINK_ALPHA, COLUMN_LINK_BETA,  COLUMN_LINK_A,      COLUMN_LINK_B);
        strSQL.AppendFormat(_T(" From %s "), TABLE_LINK);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            Integer nxLinkID = spResultSet->GetValueInt64(0);
            TCompareLinkField oCompareLinkField;
            oCompareLinkField.TnxLinkID  = nxLinkID;
            oCompareLinkField.TnxFNodeID = spResultSet->GetValueInt64 (1);
            oCompareLinkField.TnxTNodeID = spResultSet->GetValueInt64 (2);
            oCompareLinkField.TstrName   = spResultSet->GetValueString(3);
            oCompareLinkField.TnLinkType = spResultSet->GetValueInt   (4);
            oCompareLinkField.TdLength   = spResultSet->GetValueDouble(5);
            oCompareLinkField.TdLane     = spResultSet->GetValueInt   (6);
            oCompareLinkField.TdCapa     = spResultSet->GetValueDouble(7);
            oCompareLinkField.TdSpeed    = spResultSet->GetValueDouble(8);
            oCompareLinkField.TdSurveyVolume = spResultSet->GetValueDouble(9);
            oCompareLinkField.TnFunctionID = spResultSet->GetValueInt (10);
            oCompareLinkField.TdAlpha    = spResultSet->GetValueDouble(11);
            oCompareLinkField.TdBeta     = spResultSet->GetValueDouble(12);
            oCompareLinkField.TdA        = spResultSet->GetValueDouble(13);
            oCompareLinkField.TdB        = spResultSet->GetValueDouble(14);

            ar_mapLinks.insert(std::make_pair(nxLinkID, oCompareLinkField));
        }
    }
    catch (...)
    {
		ar_mapLinks.clear();
        TxLogDebugException();
    }
}

void KBulkDBaseLink::Links( KTarget* a_pTarget, std::vector<Integer>& ar_vecLink )
{
	ar_vecLink.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strQuery = _T("Select link_id from Link");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			ar_vecLink.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch (...)
	{
		ar_vecLink.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseLink::Links( KTarget* a_pTarget, std::map<Integer, CString>& a_mapLinkName )
{
	try
	{
		a_mapLinkName.clear();

		CString strSQL;
		strSQL.Format(_T("Select %s, %s From %s"), COLUMN_LINK_ID, COLUMN_LINK_NAME, TABLE_LINK);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResultSet->Next())
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);
			CString strName  = spResultSet->GetValueString(1);

			a_mapLinkName.insert(std::make_pair(nxNodeID, strName));
		}
	}
	catch (...)
	{
		a_mapLinkName.clear();
		TxLogDebugException();
	}
}


void KBulkDBaseLink::UsedLinkInspectionType(KTarget* a_pTarget, std::vector<TLinkTypeName> &a_vecTypeName)
{
	a_vecTypeName.clear();

    //★ 2016-10-05 : 명지대 한혁 요구사항..
    //   HIGHWAY_CODE_KEY, EXCLUSIVE_HOV_LANE_KEY - 2개 타입만 표출
	CString strSQL(_T(""));
	strSQL.Append(      _T(" SELECT TBL1.link_type, TBL2.KCode_Display "));
	strSQL.Append(      _T(" FROM "));
	strSQL.AppendFormat(_T(" (SELECT link_type FROM %s GROUP BY link_type)TBL1, "), TABLE_LINK);
	strSQL.AppendFormat(_T(" (SELECT KCode_Key, KCode_Display FROM %s WHERE KCode_Group_Key = %d "), TABLE_KCODE_DEFINE, USER_LINKCODE_GROUP_KEY);
	strSQL.Append   											(_T(" AND KCode_Key IN "));
	strSQL.Append   											(_T(" (select user_code_key from  define_system_code "));
	strSQL.AppendFormat											(_T(" where user_code_group = %d "), USER_LINKCODE_GROUP_KEY);
	strSQL.AppendFormat											(_T(" and system_code_group = %d "), SYSTEM_LINKCODE_GROUP_KEY);
	strSQL.AppendFormat											(_T(" and system_code_key in (%d, %d)))TBL2 "), /*ZONE_CONNECTOR_KEY,*/ HIGHWAY_CODE_KEY, EXCLUSIVE_HOV_LANE_KEY/*, WALK_CONNECTOR_CODE_KEY*/);
	strSQL.Append(      _T(" WHERE TBL1.link_type = TBL2.KCode_Key "));
	strSQL.Append(      _T(" ORDER BY TBL1.link_type "));

	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr  spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);

		while ( spResultSet->Next() )
		{
			TLinkTypeName oTLinkTypeName;

			oTLinkTypeName.TType     = spResultSet->GetValueInt   (0);
			oTLinkTypeName.TTypeName = spResultSet->GetValueString(1);

			a_vecTypeName.push_back(oTLinkTypeName);
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

void KBulkDBaseLink::AllLinkFixedValue( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TLinkFixedColumn>& ar_mapLinkFixedValue )
{
	ar_mapLinkFixedValue.clear();

	CString strQuery(_T("Select "));
	strQuery.Append(_T("link_id, fnode_id, tnode_id, name, link_type, "));
	strQuery.Append(_T("length, lane, capa, speed, survey_volume, "));
	strQuery.Append(_T("influence, function_id, alpha, beta, a, "));
	strQuery.Append(_T("b from link"));

	try
	{
		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer nxLinkID = spResultSet->GetValueInt64(0);

			TLinkFixedColumn oLinkFixedColumn = {};

			oLinkFixedColumn.nxLinkID      = spResultSet->GetValueInt64(0);
			oLinkFixedColumn.nxFnodeID     = spResultSet->GetValueInt64(1);
			oLinkFixedColumn.nxTNodeID     = spResultSet->GetValueInt64(2);
			oLinkFixedColumn.strName       = spResultSet->GetValueString(3);
			oLinkFixedColumn.nLinkType     = spResultSet->GetValueInt(4);
			oLinkFixedColumn.dLength       = spResultSet->GetValueDouble(5);
			oLinkFixedColumn.nLane         = spResultSet->GetValueInt(6);
			oLinkFixedColumn.dCapa         = spResultSet->GetValueDouble(7);
			oLinkFixedColumn.dSpeed        = spResultSet->GetValueDouble(8);
			oLinkFixedColumn.dSurveyVolume = spResultSet->GetValueDouble(9);
			oLinkFixedColumn.nInfluence    = spResultSet->GetValueInt(10);
			oLinkFixedColumn.nFunctionID   = spResultSet->GetValueInt(11);
			oLinkFixedColumn.dAlpha        = spResultSet->GetValueDouble(12);
			oLinkFixedColumn.dBeta         = spResultSet->GetValueDouble(13);
			oLinkFixedColumn.dConstantCost = spResultSet->GetValueDouble(14);
			oLinkFixedColumn.dVariableCost = spResultSet->GetValueDouble(15);

			ar_mapLinkFixedValue.insert(std::make_pair(nxLinkID, oLinkFixedColumn));
		}
	}
	catch (...)
	{
		ar_mapLinkFixedValue.clear();
		TxLogDebugException();	
	}
}

void KBulkDBaseLink::UsedNodeInfo( KDBaseConPtr a_spDBaseConnection, std::map<Integer, std::set<Integer>>& ar_mapNodeLinkID )
{
    try
    {
        ar_mapNodeLinkID.clear();

    	CString strSQL(_T(""));
        strSQL.Format(_T(" Select %s, %s, %s From %s "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, TABLE_LINK);

        std::map<Integer, std::set<Integer>>::iterator iter;
        std::map<Integer, std::set<Integer>>::iterator end;

        KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        while(spResultSet->Next())
        {
            Integer nxLinkID     = spResultSet->GetValueInt64(0);
            Integer nxFromNodeID = spResultSet->GetValueInt64(1);
            Integer nxToNodeID   = spResultSet->GetValueInt64(2);

            iter = ar_mapNodeLinkID.find(nxFromNodeID);
            end  = ar_mapNodeLinkID.end();

            if (iter != end)
            {
                std::set<Integer>& setLinkID = iter->second;
                setLinkID.insert(nxLinkID);
            }
            else
            {
                std::set<Integer> setLinkID;
                setLinkID.insert(nxLinkID);
                ar_mapNodeLinkID.insert(std::make_pair(nxFromNodeID, setLinkID));
            }

            iter = ar_mapNodeLinkID.find(nxToNodeID);
            end  = ar_mapNodeLinkID.end();

            if (iter != end)
            {
                std::set<Integer>& setLinkID = iter->second;
                setLinkID.insert(nxLinkID);
            }
            else
            {
                std::set<Integer> setLinkID;
                setLinkID.insert(nxLinkID);
                ar_mapNodeLinkID.insert(std::make_pair(nxToNodeID, setLinkID));
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
    	ThrowException(_T("At UsedNodeInfo")); 
    }
}


void KBulkDBaseLink::AllLinkIdType( KDBaseConPtr a_spDBaseConnection, std::map<Integer, int> &a_mapLinkIdType )
{
	try
	{
		a_mapLinkIdType.clear();

		CString strSQL(_T(""));
		strSQL.Format(_T(" Select %s, %s From %s "), COLUMN_LINK_ID, COLUMN_LINK_TYPE, TABLE_LINK);

		KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			Integer nxLinkID     = spResultSet->GetValueInt64(0);
			int     nLinkType    = spResultSet->GetValueInt  (1);
			
			a_mapLinkIdType.insert(std::make_pair(nxLinkID, nLinkType));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		a_mapLinkIdType.clear();
	}
	catch (...)
	{
		TxLogDebugException();
		a_mapLinkIdType.clear();
	}
}


void KBulkDBaseLink::AllLinkId( KTarget* a_pTarget, std::set<Integer> &a_setLinkId )
{
	try
	{
		a_setLinkId.clear();

		CString strSQL(_T(""));
		strSQL.Format(_T(" Select %s From %s "), COLUMN_LINK_ID, TABLE_LINK);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while(spResultSet->Next())
		{
			Integer nxLinkID     = spResultSet->GetValueInt64(0);

			a_setLinkId.insert(nxLinkID);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		a_setLinkId.clear();
	}
	catch (...)
	{
		TxLogDebugException();
		a_setLinkId.clear();
	}
}
