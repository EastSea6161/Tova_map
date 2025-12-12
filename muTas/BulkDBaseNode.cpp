#include "StdAfx.h"
#include "BulkDBaseNode.h"

//^#include "DBaseConnector.h"
#include "Target.h"

//#include "CodeGroup.h"
//#include "CodeManager.h"

KBulkDBaseNode::KBulkDBaseNode(void)
{
}


KBulkDBaseNode::~KBulkDBaseNode(void)
{
}

void KBulkDBaseNode::TotalNodeCount( KTarget* a_pTarget, int& a_nTotalNodeCount )
{
	TxLogDebugStartMsg();
	a_nTotalNodeCount = 0;

	CString strSQL(_T("Select count(*) From Node"));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr   spResult          = spDBaseConnection->ExecuteQuery(strSQL);
		
		if ( spResult->Next() )
			a_nTotalNodeCount = spResult->GetValueInt(0);
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

void KBulkDBaseNode::ZoneNodeQuery(KTarget* a_pTarget, std::vector<Integer>& vecID)
{
    TxLogDebugStartMsg();

    vecID.clear();

    CString strSQL(_T("Select node_id From Node Where node_type = '0'"));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            vecID.push_back(spResultSet->GetValueInt64(0));
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

void KBulkDBaseNode::ZoneNodeSetQuery(KTarget* a_pTarget, std::set<Integer>& a_setID)
{
	TxLogDebugStartMsg();

	a_setID.clear();

	CString strSQL(_T("Select node_id From Node Where node_type = '0'"));
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

	try
	{        
		KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			a_setID.insert(spResultSet->GetValueInt64(0));
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

void KBulkDBaseNode::NodeIDLabelQuery( KTarget* a_pTarget, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo )
{
    TxLogDebugStartMsg();

    a_vecDrawLabelInfo.clear();

    CString strSQL(_T("Select node_id From Node "));
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

    try
    {        
        KResultSetPtr    spResultSet     = spDBaseConnection->ExecuteQuery(strSQL);
        TDrawLabelInfo   oDrawLabelInfo;
        while ( spResultSet->Next() )
        {
            oDrawLabelInfo.TFeatureID    = spResultSet->GetValueInt64(0);
            oDrawLabelInfo.TLabel.Format(_T("%I64d"), spResultSet->GetValueInt64(0));

            a_vecDrawLabelInfo.push_back(oDrawLabelInfo);
        }

        TxLogDebugEndMsg();
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

void KBulkDBaseNode::NodeLabelQuery( KTarget* a_pTarget, KIOColumns& a_columnCollection, std::vector<TDrawLabelInfo>& a_vecDrawLabelInfo, CString a_strWhere /*=_T("") */ )
{
    TxLogDebugStartMsg();

    a_vecDrawLabelInfo.clear();

    KCodeManager* pCodeMgr     = a_pTarget->CodeManager();
    int           nColumnCount = a_columnCollection.ColumnCount();

    CString strSQL(_T(""));

    #pragma region strSQL
    strSQL  = _T(" Select node_id ");

    for (int i=0; i<nColumnCount; i++)
    {
        strSQL =   strSQL +  _T(", ") + a_columnCollection.GetColumn(i)->Name() + _T(" ");
    }

    strSQL += _T(" From Node ");

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

Integer KBulkDBaseNode::MaxNodeID( KDBaseConPtr a_spDBaseConnection, Integer a_nxStartID )
{
    CString strSQL(_T(""));

    Integer nxMaxNodeID = a_nxStartID - 1;
    if (nxMaxNodeID < 0)
        nxMaxNodeID = 0;
    
	try
	{
		strSQL.Format(_T(" Select ifnull(max(node_id), '%I64d') as MaxID From Node Where node_id >= '%I64d' "), nxMaxNodeID, nxMaxNodeID);
		KResultSetPtr  spResultSet     = a_spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			nxMaxNodeID = spResultSet->GetValueInt64(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

    return nxMaxNodeID;
}

Integer KBulkDBaseNode::MaxNodeID( KTarget* a_pTarget, Integer a_nxStartID )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    Integer nxMaxNodeID =  MaxNodeID(spDBaseConnection, a_nxStartID);
        
    return nxMaxNodeID;
}
void KBulkDBaseNode::Insert( KDBaseConPtr a_spDBaseConnection, TFixedNode a_oTFixedNode )
{
    // 호출하는 곳에서 try, catch...
    CString strInsert = _T("");
    strInsert.Format(_T("Insert Into Node  (node_id, node_type, x, y ) Values(%I64d, %d, %f, %f) "),
                                           a_oTFixedNode.node_id, 
                                           a_oTFixedNode.node_type, 
                                           a_oTFixedNode.x, 
                                           a_oTFixedNode.y);
	try
	{
		a_spDBaseConnection->ExecuteUpdate(strInsert);	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseNode::Insert(KDBaseConPtr a_spDBaseConnection, std::vector<TNodeKeyValue>& ar_vecKeyValue)
{
	size_t nxCount = ar_vecKeyValue.size();
	if (nxCount == 0)
		return;

	CString strColumns;
	CString strValues;
	for (size_t i=0; i<nxCount; i++)
	{
		TNodeKeyValue& oNodeKeyValue = ar_vecKeyValue[i];
		strColumns.Append(oNodeKeyValue.TKeyName);

		strValues.Append (_T("'"));
		strValues.Append (oNodeKeyValue.TValue);
		strValues.Append (_T("'"));

		if (i != nxCount-1)
		{
			strColumns.Append(_T(", "));
			strValues.Append (_T(", "));
		}
	}

	CString strInsert;
	strInsert.Append(_T(" Insert Into Node ( " ));
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

bool KBulkDBaseNode::ExistNode( KTarget* a_pTarget, Integer a_nxNodeID )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    
    int nCount = 0;

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select count(*) as Cnt From Node Where node_id = %I64d "), a_nxNodeID);

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

bool KBulkDBaseNode::ExistNode( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, int& a_nType )
{
    a_nType = -1;

	try
	{
		CString strSQL = _T("");
		strSQL.Format(_T("Select node_type From Node Where node_id = %I64d "), a_nxNodeID);

		KResultSetPtr    spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			a_nType = spResultSet->GetValueInt(0);
			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

    return false;
}


bool KBulkDBaseNode::UsedZoneIDNode( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID )
{
    try
    {
        CString strSQL = _T("");
        strSQL.Format(_T("Select count(*) From %s Where %s = '%I64d' And %s != '%I64d'"), TABLE_NODE, COLUMN_ZONE_ID, a_nxNodeID, COLUMN_NODE_ID, a_nxNodeID);

        KResultSetPtr    spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            if (spResultSet->GetValueInt64(0) > 0)
                return true;
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

    return false;
}

void KBulkDBaseNode::UpdateZoneIDZero(KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID)
{
    try
    {
        CString strSQL = _T("");
        strSQL.Format(_T("Update %s Set %s = '0' Where %s = '%I64d' "), TABLE_NODE, COLUMN_ZONE_ID, COLUMN_ZONE_ID, a_nxNodeID);
        a_spDBaseConnection->ExecuteUpdate(strSQL);
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

void KBulkDBaseNode::ReplaceNodeID( KDBaseConPtr a_spDBaseConnection, Integer a_nxOriginNodeID, Integer a_nxReplaceID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Update Node Set node_id = '%I64d' Where node_id ='%I64d' "), a_nxReplaceID, a_nxOriginNodeID);

		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KBulkDBaseNode::DeleteNode( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T("Delete From Node Where node_id ='%I64d' "), a_nxNodeID);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KBulkDBaseNode::FindFTNode    (KTarget* a_pTarget, Integer a_nxLinkID, Integer &a_nxFNodeID, Integer &a_nxTNodeID)
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
	int nCount = 0;

	CString strSQL = _T("");
	strSQL.Format(_T(" Select fnode_id, tnode_id From link Where link_id = %I64d "), a_nxLinkID);

	try
	{
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			a_nxFNodeID = spResultSet->GetValueInt64(0);
			a_nxTNodeID = spResultSet->GetValueInt64(1);

			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return false;
}

void KBulkDBaseNode::Nodes( KTarget* a_pTarget, std::vector<Integer>& ar_vecNode )
{
	try
	{
		ar_vecNode.clear();

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		int nCount = 0;

		CString strSQL = _T("Select node_id From Node");

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			ar_vecNode.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch(...)
	{
		ar_vecNode.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseNode::Nodes( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID, int a_nType )
{
    try
    {
        ar_vecNodeID.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T("Select %s From %s Where %s = '%d' "), COLUMN_NODE_ID, TABLE_NODE, COLUMN_NODE_TYPE, a_nType);

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            ar_vecNodeID.push_back(spResultSet->GetValueInt64(0));
        }
    }
    catch(...)
    {
		ar_vecNodeID.clear();
        TxLogDebugException();
    }
}

void KBulkDBaseNode::FreightFacilityNodes( KTarget* a_pTarget, std::vector<TFreightNode>& ar_vecFreightNodeInfo )
{
    try
    {
        ar_vecFreightNodeInfo.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL(_T("Select node_id, node_type, available_item, traffic_mean From Node Where node_type = '2' and Length(Traffic_Mean) > 0 "));
        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TFreightNode oFreightNode;
              oFreightNode.TnxNodeID         = spResultSet->GetValueInt64 (0);
              oFreightNode.TnNodeType        = spResultSet->GetValueInt   (1);
              oFreightNode.TstrAvailableItem = spResultSet->GetValueString(2);
              oFreightNode.TstrTrafficMean   = spResultSet->GetValueString(3);
            
            ar_vecFreightNodeInfo.push_back(oFreightNode);
        }
    }
    catch(...)
    {
		ar_vecFreightNodeInfo.clear();
        TxLogDebugException();
    }
}

void KBulkDBaseNode::Nodes( KTarget* a_pTarget,std::map<Integer, TCompareNodeField>& ar_mapNodes )
{
    try
    {
    	ar_mapNodes.clear();
        CString strSQL(_T(""));
        strSQL.Format(_T("Select %s, %s, %s, %s, %s, %s, target_region, parking_fee From %s "), COLUMN_NODE_ID, COLUMN_NODE_TYPE, COLUMN_NODE_X, COLUMN_NODE_Y, COLUMN_ZONE_ID, COLUMN_NODE_NAME, TABLE_NODE);
    
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            Integer nxNodeID = spResultSet->GetValueInt64(0);
            TCompareNodeField oNodeSystemField;
              oNodeSystemField.TnxNodeID      = nxNodeID;
              oNodeSystemField.TnNodeType     = spResultSet->GetValueInt   (1);
              oNodeSystemField.TdX            = spResultSet->GetValueDouble(2);
              oNodeSystemField.TdY            = spResultSet->GetValueDouble(3);
              oNodeSystemField.TnxZoneID      = spResultSet->GetValueInt64 (4);
              oNodeSystemField.TstrName       = spResultSet->GetValueString(5);
			  oNodeSystemField.TnTargetRegion = spResultSet->GetValueInt(6);
			  oNodeSystemField.TdParkingFee   = spResultSet->GetValueDouble(7);

            ar_mapNodes.insert(std::make_pair(nxNodeID, oNodeSystemField));
        }
    }
    catch (KExceptionPtr ex)
    {
		ar_mapNodes.clear();
        TxExceptionPrint(ex);
    }
    catch (...)
    {
		ar_mapNodes.clear();
    	TxLogDebugException();
    }
}

void KBulkDBaseNode::Nodes( KTarget* a_pTarget, std::map<Integer, TFCompareNodeField>& ar_mapNodes )
{
	try
	{
		ar_mapNodes.clear();
		CString strSQL(_T(""));
		strSQL.Format(_T("Select %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s From %s "), COLUMN_NODE_ID, COLUMN_NODE_TYPE, COLUMN_NODE_X, COLUMN_NODE_Y
			, COLUMN_ZONE_ID, COLUMN_NODE_NAME, COLUMN_NODE_FACILITY_TYPE, COLUMN_NODE_FACILITY_FEE, COLUMN_NODE_LOADING_FEE, COLUMN_NODE_UNLOADING_FEE
			, COLUMN_NODE_AVAILABLE_ITEM, COLUMN_NODE_TRAFFIC_MEAN, COLUMN_NODE_OCCURRENCE_VIA, COLUMN_NODE_CAPA ,TABLE_NODE);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);
			TFCompareNodeField oNodeSystemField;
			oNodeSystemField.TnxNodeID			= nxNodeID;
			oNodeSystemField.TnNodeType			= spResultSet->GetValueInt   (1);
			oNodeSystemField.TdX				= spResultSet->GetValueDouble(2);
			oNodeSystemField.TdY				= spResultSet->GetValueDouble(3);
			oNodeSystemField.TnxZoneID			= spResultSet->GetValueInt64 (4);
			oNodeSystemField.TstrName			= spResultSet->GetValueString(5);
			oNodeSystemField.TnFacility_type	= spResultSet->GetValueInt(6);
			oNodeSystemField.TdFacility_Fee		= spResultSet->GetValueDouble(7);
			oNodeSystemField.TdLoading_Fee		= spResultSet->GetValueDouble(8);
			oNodeSystemField.TdUnLoading_Fee	= spResultSet->GetValueDouble(9);
			oNodeSystemField.TstrAvailialbe_item= spResultSet->GetValueString(10);
			oNodeSystemField.TstrTraffic_mean	= spResultSet->GetValueString(11);
			oNodeSystemField.TnOccurrence_via	= spResultSet->GetValueInt(12);
			oNodeSystemField.TdCapa				= spResultSet->GetValueDouble(13);

			ar_mapNodes.insert(std::make_pair(nxNodeID, oNodeSystemField));
		}
	}
	catch (KExceptionPtr ex)
	{
		ar_mapNodes.clear();
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		ar_mapNodes.clear();
		TxLogDebugException();
	}
}


void KBulkDBaseNode::Nodes( KTarget* a_pTarget, std::vector<TNodeIDZoneID>& ar_vecNodes )
{
    try
    {
        ar_vecNodes.clear();
        CString strSQL(_T(""));
        strSQL.Format(_T("Select %s, %s From %s "), COLUMN_NODE_ID, COLUMN_ZONE_ID, TABLE_NODE);

        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            TNodeIDZoneID oNodeIDZoneID;
            Integer nxNodeID = spResultSet->GetValueInt64(0);
            Integer nxZoneID = spResultSet->GetValueInt64(1);

            oNodeIDZoneID.NodeID = nxNodeID;
            oNodeIDZoneID.ZoneID = nxZoneID;

            ar_vecNodes.push_back(oNodeIDZoneID);
        }
    }
    catch (KExceptionPtr ex)
    {
		ar_vecNodes.clear();
        TxExceptionPrint(ex);
    }
    catch (...)
    {
		ar_vecNodes.clear();
        TxLogDebugException();
    }
}

void KBulkDBaseNode::Nodes( KTarget* a_pTarget, std::map<Integer, CString> &a_mapNodeName )
{
	try
	{
		a_mapNodeName.clear();

		CString strSQL(_T(""));
		strSQL.Format(_T(" SELECT node_id, name FROM %s "), TABLE_NODE);

		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);
			CString strName  = spResultSet->GetValueString(1);

			a_mapNodeName.insert(std::make_pair(nxNodeID, strName));
		}
	}
	catch (...)
	{
		a_mapNodeName.clear();
		TxLogDebugException();	
	}
}



void KBulkDBaseNode::NodesExceptType( KTarget* a_pTarget, std::vector<Integer>& ar_vecNodeID, int a_nType )
{
	try
	{
		ar_vecNodeID.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From %s Where %s != '%d' "), COLUMN_NODE_ID, TABLE_NODE, COLUMN_NODE_TYPE, a_nType);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			ar_vecNodeID.push_back(spResultSet->GetValueInt64(0));
		}
	}
	catch(...)
	{
		ar_vecNodeID.clear();	
		TxLogDebugException();
	}
}

void KBulkDBaseNode::AllNodeValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From Node"), a_strColumnName);

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

void KBulkDBaseNode::AllNodeValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TNodeIDValue>& ar_vecValue )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select node_id, %s From Node Order by %s Desc"), a_strColumnName, a_strColumnName);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeIDValue oTNodeIDValue;
			oTNodeIDValue.TNodeID = spResultSet->GetValueInt64(0);
			oTNodeIDValue.TValue  = spResultSet->GetValueDouble(1);
			ar_vecValue.push_back(oTNodeIDValue);
		}
	}
	catch(...)
	{
		ar_vecValue.clear();	
		TxLogDebugException();
	}
}


void KBulkDBaseNode::SrchNodeValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<double>& ar_vecValue, CString a_strFilter )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From Node %s"), a_strColumnName, a_strFilter);

		if (!a_strFilter.IsEmpty())
			strSQL.Append(_T(" And"));
		else
			strSQL.Append(_T(" Where"));

		strSQL.AppendFormat(_T(" %s IS NULL or %s < 9e999"), a_strColumnName, a_strColumnName); // "< 9e999" 조건만 하면 null 값은 나오지 않는다.
		
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

void KBulkDBaseNode::SrchNodeValue( KTarget* a_pTarget, CString a_strColumnName, std::vector<TNodeIDValue>& ar_vecValue, CString a_strFilter )
{
	try
	{
		ar_vecValue.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select node_id, %s From Node %s Order by %s Desc"), a_strColumnName, a_strFilter, a_strColumnName);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TNodeIDValue oTNodeIDValue;
			oTNodeIDValue.TNodeID = spResultSet->GetValueInt64(0);
			oTNodeIDValue.TValue  = spResultSet->GetValueDouble(1);
			ar_vecValue.push_back(oTNodeIDValue);
		}
	}
	catch(...)
	{
		ar_vecValue.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseNode::UpdateNodeXY( KDBaseConPtr a_spDBaseConnection, Integer a_nxNodeID, double a_dX, double a_dY )
{
	try
	{
		CString strSQL;
		strSQL.Format(_T(" Update %s Set %s = '%f', %s = '%f' Where %s ='%I64d' "), TABLE_NODE, COLUMN_NODE_X, a_dX, COLUMN_NODE_Y, a_dY, COLUMN_NODE_ID, a_nxNodeID);
		a_spDBaseConnection->ExecuteUpdate(strSQL);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

bool KBulkDBaseNode::GetNodeXY( KTarget* a_pTarget, Integer a_nxNodeID, double& ar_dX, double& ar_dY )
{
    KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    int nCount = 0;

    CString strSQL = _T("");
    strSQL.Format(_T(" Select x, y From node Where node_id = %I64d "), a_nxNodeID);

	try
	{
		KResultSetPtr   spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			ar_dX = spResultSet->GetValueDouble(0);
			ar_dY = spResultSet->GetValueDouble(1);

			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

    return false;
}

Integer KBulkDBaseNode::GetMinMaxXY( KTarget* a_pTarget, double& dMinX, double& dMinY, double& dMaxX, double& dMaxY )
{
	KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
    Integer nxCount(0);

    CString strSQL = _T(" Select min(x), min(y), max(x), max(y), count(x) From node ");
    
	try
	{
		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		if ( spResultSet->Next() )
		{
			dMinX   = spResultSet->GetValueDouble(0);
			dMinY   = spResultSet->GetValueDouble(1);

			dMaxX   = spResultSet->GetValueDouble(2);
			dMaxY   = spResultSet->GetValueDouble(3);
			nxCount = spResultSet->GetValueInt64 (4);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

    return nxCount;
}

void KBulkDBaseNode::NodeType( KTarget* a_pTarget, std::map<Integer, int>& ar_mapNodeType )
{
    try
    {
        ar_mapNodeType.clear();		
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T("Select %s, %s From %s "), COLUMN_NODE_ID, COLUMN_NODE_TYPE, TABLE_NODE);

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            Integer nxNodeID = spResultSet->GetValueInt64(0);
            int     nType    = spResultSet->GetValueInt  (1);
            ar_mapNodeType.insert(std::make_pair(nxNodeID, nType));
        }
    }
    catch(...)
    {
		ar_mapNodeType.clear();		
        TxLogDebugException();
    }
}

bool KBulkDBaseNode::ExistZone( KTarget* a_pTarget, Integer a_nxID )
{
    try
    {
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T(" Select count(*) From %s Where %s ='0' and %s = '%I64d' "), TABLE_NODE, COLUMN_NODE_TYPE, COLUMN_NODE_ID, a_nxID);

        KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            if (spResultSet->GetValueInt64(0) > 0)
            {
                return true;
            }
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

    return false;
}

bool KBulkDBaseNode::ExistZone( KDBaseConPtr a_spDBaseConnection, Integer a_nxID )
{
    try
    {   
        CString strSQL;
        strSQL.Format(_T(" Select count(*) From %s Where %s ='0' and %s = '%I64d' "), TABLE_NODE, COLUMN_NODE_TYPE, COLUMN_NODE_ID, a_nxID);

        KResultSetPtr    spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            if (spResultSet->GetValueInt64(0) > 0)
            {
                return true;
            }
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

    return false;
}

Integer KBulkDBaseNode::MaxZoneID( KTarget* a_pTarget )
{
    Integer nxMaxZoneID = -1;

    try
    {        
        KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
        CString strSQL;
        strSQL.Format(_T(" Select ifnull(max(%s), -1) From node Where %s ='0' "), COLUMN_NODE_ID, COLUMN_NODE_TYPE);

        KResultSetPtr       spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        if ( spResultSet->Next() )
        {
            nxMaxZoneID = spResultSet->GetValueInt64(0);
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

    return nxMaxZoneID;
}

void KBulkDBaseNode::AllNodeGroupByType( KTarget* a_pTarget, std::vector<int>& a_vecNodeType )
{
	a_vecNodeType.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

		CString strQuery = _T("select node_type from node group by node_type order by node_type");

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			int nTypeGroup = spResultSet->GetValueInt(0);
			a_vecNodeType.push_back(nTypeGroup);
		}
	}
	catch (...)
	{
		a_vecNodeType.clear();
		TxLogDebugException();
	}
}

void KBulkDBaseNode::AllNodeFixedValue( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TNodeFixedColumn>& ar_mapNodeFixedValue )
{
	ar_mapNodeFixedValue.clear();

	CString strQuery;
	strQuery.Format(_T("Select %s, %s, %s, %s, %s, %s, %s, %s from %s")
		,COLUMN_NODE_ID, COLUMN_NODE_NAME, COLUMN_NODE_TYPE, COLUMN_NODE_X, COLUMN_NODE_Y
		,_T("zone_id"), _T("target_region"), _T("parking_fee"), TABLE_NODE);

	try
	{
		KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strQuery);

		while (spResult->Next())
		{
			Integer nxNodeID = spResult->GetValueInt64(0);

			TNodeFixedColumn oNodeFixedColumn = {};

			oNodeFixedColumn.nxNodeID      = spResult->GetValueInt64(0);
			oNodeFixedColumn.strName       = spResult->GetValueString(1);
			oNodeFixedColumn.nType         = spResult->GetValueInt(2);
			oNodeFixedColumn.dX            = spResult->GetValueDouble(3);
			oNodeFixedColumn.dY            = spResult->GetValueDouble(4);
			oNodeFixedColumn.nZodeID       = spResult->GetValueInt(5);
			oNodeFixedColumn.nTargetRegion = spResult->GetValueInt(6);
			oNodeFixedColumn.dParkingFee   = spResult->GetValueDouble(7);

			ar_mapNodeFixedValue.insert(std::make_pair(nxNodeID, oNodeFixedColumn));
		}
	}
	catch (...)
	{
		ar_mapNodeFixedValue.clear();
		TxLogDebugException();	
	}
}


void KBulkDBaseNode::AllNodeID( KTarget* a_pTarget, std::set<Integer> &a_setNodeID )
{
	try
	{
		a_setNodeID.clear();		
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		CString strSQL;
		strSQL.Format(_T("Select %s From %s "), COLUMN_NODE_ID, TABLE_NODE);

		KResultSetPtr    spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			Integer nxNodeID = spResultSet->GetValueInt64(0);

			a_setNodeID.insert(nxNodeID);
		}
	}
	catch(...)
	{
		a_setNodeID.clear();		
		TxLogDebugException();
	}
}

void KBulkDBaseNode::AllNodeCoords( KTarget* a_pTarget, std::map<Integer, TNodeCoords>& a_mapAllNodeCoords )
{
	a_mapAllNodeCoords.clear();

	CString strQuery = _T("SELECT node_id, x, y FROM NODE ORDER BY node_id");

	try
	{
		KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		while (spResultSet->Next())
		{
			Integer     nxNodeID    = spResultSet->GetValueInt64(0);
			TNodeCoords oNodeCoords = {};

			oNodeCoords.TdX = spResultSet->GetValueDouble(1);
			oNodeCoords.TdY = spResultSet->GetValueDouble(2);

			a_mapAllNodeCoords.insert(std::make_pair(nxNodeID, oNodeCoords));
		}
	}
	catch (...)
	{
		TxLogDebugException();
		a_mapAllNodeCoords.clear();	
	}
}
