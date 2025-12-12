#include "stdafx.h"
#include "FreightBulkPathGenerate.h"

#include "Target.h"
//^#include "DBaseConnector.h"
#include "BulkDBaseNode.h"
//^#include "ParseString.h"
//^#include "Logger.h"
#include "ImChampDir.h"


void KFreightBulkPathGenerate::BulkPathGenerate( KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress )
{
    TxLogDebugStartMsg();

    KLinkCollection            oRoadLinkCollection;
    KLinkCollection            oRailLinkCollection;
    KCostLinkCollection        rCostLinkCollection;
    KNodeArrivedMinLink        rNodeArrivedMinLink;

    try
    {          
        if (spSimpleProgress)
        {
            spSimpleProgress->SetStatus(_T("Initial Reading Data"));
        }
        CString  strSQL;
        strSQL.Format(_T(" Select %s, %s, %s, %s, %s From %s "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_LENGTH, COLUMN_LINK_TYPE, TABLE_LINK);

        KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        while(spResultSet->Next())
        {
            Integer nxLinkID  = spResultSet->GetValueInt64 (0);
            Integer nxFNodeID = spResultSet->GetValueInt64 (1);
            Integer nxTNodeID = spResultSet->GetValueInt64 (2);
            double  dLength   = spResultSet->GetValueDouble(3);
            int     nLinkType = spResultSet->GetValueInt   (4);

            if (nLinkType == 9)
            {
                KLink* pLink = oRailLinkCollection.GetExistOrNew(nxLinkID);
                pLink->SetValues(nxFNodeID, nxTNodeID, dLength);
            }
            else
            {
                KLink* pLink = oRoadLinkCollection.GetExistOrNew(nxLinkID);
                pLink->SetValues(nxFNodeID, nxTNodeID, dLength);
            }            
        }
    }
    catch (KExceptionPtr e)
    {
        TxLogDebugException();
        throw e;
    }
    catch (...)
    {
        TxLogDebugException();
        throw;        
    }
    
    std::map<KODKey, TDistLinksInfo> mapO2TDKeyDist;
    std::map<KODKey, TDistLinksInfo> mapT2TDKeyDist;
    std::map<KODKey, TDistLinksInfo> mapR2RKeyDist;

    std::vector<Integer> vecOriginID;
    std::vector<Integer> vecZoneNodeID;    
    std::vector<Integer> vecFacilityNodeID;
    std::vector<Integer> vecRailStationNodeID;
    std::set<Integer>    setDestination;
    
    try
    {
        size_t nxCount(0);
               
        // 존 정보 검색
        KBulkDBaseNode::Nodes(pTarget, vecZoneNodeID,     0);

        std::vector<TFreightNode> vecFreightFacilityInfo;
        KBulkDBaseNode::FreightFacilityNodes(pTarget, vecFreightFacilityInfo);

        nxCount = vecFreightFacilityInfo.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TFreightNode& oFreightNode = vecFreightFacilityInfo[i];
            Integer nxNodeID = oFreightNode.TnxNodeID;

            CString strTrafficMean = oFreightNode.TstrTrafficMean;
            
            if (strTrafficMean.GetLength() == 0)
            {//Nothing
            }
            else if (strTrafficMean.GetLength() == 1)
            {
                if (strTrafficMean.CompareNoCase(_T("1")) == 0)
                {
                    vecFacilityNodeID.push_back(nxNodeID);
                }
                else if (strTrafficMean.CompareNoCase(_T("2")) == 0)
                {
                    vecFacilityNodeID.push_back(nxNodeID);
                    vecRailStationNodeID.push_back(nxNodeID);
                }
            }
            else
            {
                std::vector<CString> vecString = KParseString::ParseString(strTrafficMean, _T(','));
                bool bFindRoad(false), bFindRail(false);

                for (size_t j=0; j<vecString.size(); j++)
                {
                    CString strParseTrafficMean = vecString[j];

                    if (strParseTrafficMean.CompareNoCase(_T("1")) == 0)
                    {
                        if (!bFindRoad)
                        {
                            vecFacilityNodeID.push_back(nxNodeID);
                            bFindRoad = true;
                        }
                    }
                    else if (strParseTrafficMean.CompareNoCase(_T("2")) == 0)
                    {
                        if (!bFindRoad)
                        {
                            vecFacilityNodeID.push_back(nxNodeID);
                            bFindRoad = true;
                        }

                        if (!bFindRail)
                        {
                            vecRailStationNodeID.push_back(nxNodeID);
                            bFindRail = true;
                        }                        
                    }
                }
            }
        }
        
        // 링크타입이 9이 아닌것만 추출
        //Step1) 시점존에서      전체존+전체물류시설 : 시점(O)->물류시설(1), 철도시설(2) + Destination -> 시점에서물류시설, 시점->종점(직송)         
        //Step2) 전체물류시설에서 전체존            : 시점(물류시설(1), 철도시설(2))->종점(Destination) : 정보저장은 두군대로 분리하여 입력
        //Step3) 철송정보
                
        vecOriginID.clear();
        setDestination.clear();
        mapO2TDKeyDist.clear();

        nxCount = vecZoneNodeID.size();
        for(size_t i=0; i<nxCount; i++)
        {
            setDestination.insert(vecZoneNodeID[i]);
        }
        nxCount = vecFacilityNodeID.size();
        for(size_t i=0; i<nxCount; i++)
        {
            setDestination.insert(vecFacilityNodeID[i]);
        }

        nxCount = vecZoneNodeID.size();        
        for(size_t i=0; i<nxCount; i++)
        {
            if (spSimpleProgress /*&& i%5 == 0*/)
            {
                if (spSimpleProgress->IsUserStopped())
                    return;

                CString strMsg(_T(""));
                strMsg.Format(_T("Find Path #1(%d/%d) - %I64d"), i+1, nxCount, vecZoneNodeID[i]);
                spSimpleProgress->SetStatus(strMsg);
            }

            KShortestPath::MuLinkBaseExecute(vecZoneNodeID[i], setDestination, oRoadLinkCollection, rCostLinkCollection, rNodeArrivedMinLink);


            ExtractResult (vecZoneNodeID[i], setDestination, mapO2TDKeyDist, rCostLinkCollection, rNodeArrivedMinLink, spSimpleProgress);   
        }
        
        //Step2)
        TxLogDebugVisitor();

        vecOriginID.clear();        
        mapT2TDKeyDist.clear();        
        setDestination.clear();

        nxCount = vecZoneNodeID.size();        
        for(size_t i=0; i<nxCount; i++)
        {
            setDestination.insert(vecZoneNodeID[i]);
        }

        nxCount = vecFacilityNodeID.size();        
        for(size_t i=0; i<nxCount; i++)
        {
            if (spSimpleProgress /*&& i%5 == 0*/)
            {
                if (spSimpleProgress->IsUserStopped())
                    return;

                CString strMsg(_T(""));
                strMsg.Format(_T("Find Path #2(%d/%d) - %I64d"), i+1, nxCount, vecFacilityNodeID[i]);
                spSimpleProgress->SetStatus(strMsg);
            }

            KShortestPath::MuLinkBaseExecute(vecFacilityNodeID[i], setDestination, oRoadLinkCollection, rCostLinkCollection, rNodeArrivedMinLink);
            ExtractResult (vecFacilityNodeID[i], setDestination, mapT2TDKeyDist, rCostLinkCollection, rNodeArrivedMinLink, spSimpleProgress);            
        }

        TxLogDebugVisitor();
        //Step3)
        setDestination.clear();

        nxCount = vecRailStationNodeID.size();        
        for(size_t i=0; i<nxCount; i++)
        {
            setDestination.insert(vecRailStationNodeID[i]);
        }

        nxCount = vecRailStationNodeID.size();        
        for(size_t i=0; i<nxCount; i++)
        {
            if (spSimpleProgress /*&& i%5 == 0*/)
            {
                if (spSimpleProgress->IsUserStopped())
                    return;

                CString strMsg(_T(""));
                strMsg.Format(_T("Find Path #3(%d/%d) - %I64d"), i+1, nxCount, vecRailStationNodeID[i]);
                spSimpleProgress->SetStatus(strMsg);
            }

            KShortestPath::MuLinkBaseExecute(vecRailStationNodeID[i], setDestination, oRailLinkCollection, rCostLinkCollection, rNodeArrivedMinLink);
            ExtractResult (vecRailStationNodeID[i], setDestination, mapR2RKeyDist, rCostLinkCollection, rNodeArrivedMinLink, spSimpleProgress);            
        }

        TxLogDebugVisitor();
        if (spSimpleProgress)
        {
            if (spSimpleProgress->IsUserStopped())
                return;

            spSimpleProgress->SetStatus(_T("Processing.."));
        }

        // 정보저장
        InsertTTempDB(0, pTarget, mapO2TDKeyDist);
        InsertTTempDB(1, pTarget, mapT2TDKeyDist);
        InsertTTempDB(2, pTarget, mapR2RKeyDist);
    }
    catch (KExceptionPtr e)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }

    TxLogDebugEndMsg();
}

void KFreightBulkPathGenerate::ExtractResult( Integer nxOriginID,  std::set<Integer>& setDestination, std::map<KODKey, TDistLinksInfo>& mapKeyDist, 
                                              KCostLinkCollection& rCostLinkCollection, 
                                              KNodeArrivedMinLink& rNodeArrivedMinLink, 
                                              QBicSimpleProgressPtr   a_spSimpleProgress )
{
    try
    {
    	std::set<Integer>::iterator iter = setDestination.begin();
        std::set<Integer>::iterator end  = setDestination.end();

        while(iter != end)
        {
            Integer nxDesitionID = *iter;
            KODKey  oKey(nxOriginID, nxDesitionID);

			TDistLinksInfo oTDLInfo = {};

			KMinLinkInfo* pMinLink = rNodeArrivedMinLink.GetMinLink(nxDesitionID);
            if (nxOriginID == nxDesitionID)
            {
				oTDLInfo.dDist = 0.0;
            }
            else
            {
                if (pMinLink == nullptr)
                {
                    oTDLInfo.dDist = 99999999.0;
                }
                else
                {
                    double dCost = pMinLink->Cost;
                    oTDLInfo.dDist = dCost;
                }
            }

			CString strLinks(_T(""));
			if (pMinLink != nullptr)
			{
				Integer nxBackLinkID = pMinLink->LinkID;
				strLinks.Format(_T("%I64d"), nxBackLinkID);

				while(nxBackLinkID > 0)
				{
					KCostLink* pCostLink = rCostLinkCollection.GetLink(nxBackLinkID);
					if (nullptr == pCostLink)
					{
						strLinks = _T("");
						break;
					}
					else
					{
						nxBackLinkID = pCostLink->GetPreLinkID();
						if (0 != nxBackLinkID)
						{
							strLinks.AppendFormat(_T(",%I64d"), nxBackLinkID);
						}
					}
				}
			}

			oTDLInfo.strLinks = strLinks;
			mapKeyDist.insert(std::make_pair(oKey, oTDLInfo));
            ++iter;
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
}

void KFreightBulkPathGenerate::InsertTTempDB(int nType, KTarget* pTarget, std::map<KODKey, TDistLinksInfo>& mapKeyDist)
{
	KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

    try
    {
        spDBaseConnection->BeginTransaction();

        // 테이블 삭제...
        CString strSQLDelete(_T(""));
        CString strSQLInsert(_T(""));

        switch(nType)
        {
        case 0:
            strSQLDelete = (_T(" Delete From TTable_O2TD "));
            strSQLInsert = (_T(" Insert Into TTable_O2TD (OZone_ID, DZone_ID, Length, Time, Cost, Links) Values(?, ?, ?, 0, 0, ?) "));
            break;
        case 1:
            strSQLDelete = (_T(" Delete From TTable_T2TD "));
            strSQLInsert = (_T(" Insert Into TTable_T2TD (OZone_ID, DZone_ID, Length, Time, Cost, Links) Values(?, ?, ?, 0, 0, ?) "));
            break;
        case 2:
            strSQLDelete = (_T(" Delete From TTable_R2R "));
            strSQLInsert = (_T(" Insert Into TTable_R2R (OZone_ID, DZone_ID, Length, Time, Cost, Links) Values(?, ?, ?, 0, 0, ?) "));
            break;
        default:
            break;
        }
        
        spDBaseConnection->ExecuteUpdate(strSQLDelete);

        KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

        std::map<KODKey, TDistLinksInfo>::iterator iter = mapKeyDist.begin();
        std::map<KODKey, TDistLinksInfo>::iterator end  = mapKeyDist.end();
        while(iter != end)
        {
            KODKey  oKey			= iter->first;
            TDistLinksInfo  oInfo	= iter->second;

            spPreparedStatement->BindInt64 (1, oKey.OriginID);
            spPreparedStatement->BindInt64 (2, oKey.DestinationID);
            spPreparedStatement->BindDouble(3, oInfo.dDist);
			spPreparedStatement->BindText(4, oInfo.strLinks);
            spPreparedStatement->ExecuteUpdate();
            spPreparedStatement->Reset();

            ++iter;
        }

        spDBaseConnection->Commit();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        spDBaseConnection->RollBack();
    }
    catch (...)
    {
    	TxLogDebugException();
        spDBaseConnection->RollBack();
    }
}

//직송Time    pRecord[i*6 + 0] 
//직송Cost    pRecord[i*6 + 1]
//물류Time    pRecord[i*6 + 2] 
//물류Cost    pRecord[i*6 + 3] 
//철송Time    pRecord[i*6 + 4]
//철송Cost    pRecord[i*6 + 5] 
void KFreightBulkPathGenerate::TimeCost( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress )
{
    try
    {
        // 존 정보 검색
        std::vector<Integer> vecZoneNodeID;
        KBulkDBaseNode::Nodes(pTarget, vecZoneNodeID, 0);

        std::vector<TFreightNode> vecFreightFacilityInfo;
        KBulkDBaseNode::FreightFacilityNodes(pTarget, vecFreightFacilityInfo);

        std::map<Integer, std::set<int>> mapAvailableItem;
        std::map<Integer, std::set<int>> mapTrafficMean;

        size_t nxCount = vecFreightFacilityInfo.size();
        for (size_t i=0; i<nxCount; i++)
        {
            TFreightNode& oFreightNode = vecFreightFacilityInfo[i];
            Integer nxNodeID         = oFreightNode.TnxNodeID;
            CString strAvailableItem = oFreightNode.TstrAvailableItem;
            CString strTrafficMean   = oFreightNode.TstrTrafficMean;

            std::vector<CString> vecAIString = KParseString::ParseString(strAvailableItem, _T(','));
            std::vector<CString> vecTMString = KParseString::ParseString(strTrafficMean,   _T(','));

            std::set<int> setAItem;
            for (size_t i=0; i<vecAIString.size(); i++)
            {
                setAItem.insert(_ttoi(vecAIString[i]));
            }

            mapAvailableItem.insert(std::make_pair(nxNodeID, setAItem));

            std::set<int> setTMean;
            for (size_t i=0; i<vecTMString.size(); i++)
            {
                setTMean.insert(_ttoi(vecTMString[i]));
            }

            mapTrafficMean.insert(std::make_pair(nxNodeID, setTMean));
        }

        if (spSimpleProgress)
        {
            if (spSimpleProgress->IsUserStopped())
                return;

            spSimpleProgress->SetStatus(_T("직송 정보 계산.."));
        }
               
        //직송정보계산
        DirectTimeCost( pTarget, vecItemTimeCost, oODKeyDoubleRecords, spSimpleProgress, mapAvailableItem, mapTrafficMean );

        if (spSimpleProgress)
        {
            if (spSimpleProgress->IsUserStopped())
                return;

            spSimpleProgress->SetStatus(_T("육송 정보 계산.."));
        }

        size_t nxItemCount = vecItemTimeCost.size();
        std::vector<std::map<KODKey, TRoadViaPathInfo>> vecFindRoadViaPathItem;
        for (size_t i=0; i<nxItemCount; i++)
        {
            std::map<KODKey, TRoadViaPathInfo> mapFindPath;
            vecFindRoadViaPathItem.push_back(mapFindPath);
        }

        //육송정보계산
        nxCount = vecZoneNodeID.size();

        int nTotalCount = nxCount*nxCount;
        int nIndex(1);
        for (size_t i=0; i<nxCount; i++)
        {
            for (size_t j=0; j<nxCount; j++)
            {
                Integer nxOID = vecZoneNodeID[i];
                Integer nxDID = vecZoneNodeID[j];

                if (spSimpleProgress)
                {
                    if (spSimpleProgress->IsUserStopped())
                        return;

                    CString strTemp(_T(""));
                    strTemp.Format(_T("육송 정보 계산[%d/%d] : %I64d -> %I64d"), nIndex++, nTotalCount, nxOID, nxDID);
                    spSimpleProgress->SetStatus(strTemp);
                }

                if (nxOID != nxDID)
                    FDistributionTimeCost(pTarget, vecItemTimeCost, oODKeyDoubleRecords, spSimpleProgress, mapAvailableItem, mapTrafficMean, nxOID, nxDID, vecFindRoadViaPathItem);
            }
        }
        
        std::vector<std::map<KODKey, TRailPathInfo>> vecFindRailPathItem;
        for (size_t i=0; i<nxItemCount; i++)
        {
            std::map<KODKey, TRailPathInfo> mapFindPath;
            vecFindRailPathItem.push_back(mapFindPath);
        }

        //철송정보계산
        DistRailTimeCost(pTarget, vecItemTimeCost, oODKeyDoubleRecords, spSimpleProgress, mapAvailableItem, mapTrafficMean, vecFindRailPathItem);

		
		//기존 정보 가져오기
		std::map<int, std::map<KODKey, TViaTermianlInfo>> mapOldViaInfo;
		GetExistPathInfo(pTarget, mapOldViaInfo);


		//
		CString strSize(_T(""));
		int nSize1 = mapOldViaInfo.size();
		strSize.Format(_T("%d"), nSize1);
		TxLogDebug((LPCTSTR)strSize);

		AutoType itersize = mapOldViaInfo.begin();
		AutoType endsize  = mapOldViaInfo.end();
		while(itersize != endsize)
		{
			int nOldID = itersize->first;
			std::map<KODKey, TViaTermianlInfo> mapOldTerminalInfo = itersize->second;
			int nsize2 = mapOldTerminalInfo.size();

			strSize.Format(_T("%d_%d"), nOldID, nsize2);
			TxLogDebug((LPCTSTR)strSize);
			itersize++;
		}
		//

		//새로 저장될 정보 가져오기
		std::map<int, std::map<KODKey, TViaTermianlInfo>> mapNewViaInfo;
		GetNewPathInfo(vecItemTimeCost, vecZoneNodeID, vecFindRoadViaPathItem, vecFindRailPathItem, mapNewViaInfo);

		//기존 정보에 새로 저장될 정보 합치기
		AutoType iterOldViaInfo	= mapOldViaInfo.begin();
		AutoType endOldViaInfo	= mapOldViaInfo.end();
		while(iterOldViaInfo != endOldViaInfo)
		{
			int nOldID = iterOldViaInfo->first;
			std::map<KODKey, TViaTermianlInfo> mapOldTerminalInfo = iterOldViaInfo->second;

			AutoType findNewvia = mapNewViaInfo.find(nOldID);
			AutoType endNewvia  = mapNewViaInfo.end();
			if (findNewvia == endNewvia)
			{
				mapNewViaInfo.insert(std::make_pair(nOldID, mapOldTerminalInfo));
			}

			iterOldViaInfo++;
		}

        //Temporay 정보 저장
        //CreateTTablePathInfo(pTarget, vecItemTimeCost, spSimpleProgress);
        //InsertTTablePathInfo(pTarget, vecItemTimeCost, spSimpleProgress, vecZoneNodeID, vecFindRoadViaPathItem, vecFindRailPathItem);
        
		CreatePathInfoTable(pTarget,mapNewViaInfo, spSimpleProgress );
		InsertPathInfoTable(pTarget, vecItemTimeCost, spSimpleProgress, vecZoneNodeID, mapNewViaInfo);

        // 로그파일
        //WriteTTablePathInfo(pTarget, vecItemTimeCost, spSimpleProgress, vecZoneNodeID, vecFindRoadViaPathItem, vecFindRailPathItem);
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

void KFreightBulkPathGenerate::DirectTimeCost( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress, std::map<Integer, std::set<int>>& mapAvailableItem, std::map<Integer, std::set<int>>& mapTrafficMean )
{
    try
    {
    	CString strSQL(_T(""));

        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("    OZone_ID, DZone_ID, Length "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("    TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T("    Ta.DZone_ID = Tb.node_id "));

        size_t nxItemCount = vecItemTimeCost.size();
        
        KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            Integer nxOriginID = spResultSet->GetValueInt64 (0);
            Integer nxDestID   = spResultSet->GetValueInt64 (1);
            double  dLength    = spResultSet->GetValueDouble(2);

            for (size_t i=0; i<nxItemCount; i++)
            {
                TItemTimeCost& oItemTimeCost = vecItemTimeCost[i];
                KSDoubleRecord* pRecord = oODKeyDoubleRecords.GetRecord(KODKey(nxOriginID, nxDestID));
                if (pRecord == nullptr)
                {
                    pRecord = oODKeyDoubleRecords.AddRecord(KODKey(nxOriginID, nxDestID));  
                    for (size_t j=0; j<nxItemCount*6; j++)
                    {
                        if (nxOriginID == nxDestID)
                        {
                            pRecord->SetAt(j, 0.0);
                        }
                        else
                        {
                            pRecord->SetAt(j, 99999999.0);
                        }                        
                    }
                }

                double dTime(99999999.0);
                double dCost(99999999.0);
                if (dLength < 99999999.0)
                {
                    dTime = (dLength * oItemTimeCost.RoadTime_Trip) / 60.0;
                    dCost = (dLength * oItemTimeCost.RoadCost_Trip) / 1000.0;
                }                

                // 직송Time
                pRecord->SetAt(i*6 + 0 , dTime);
                // 직송Cost  
                pRecord->SetAt(i*6 + 1 , dCost);        
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
}

void KFreightBulkPathGenerate::FDistributionTimeCost( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress, 
                                                      std::map<Integer, std::set<int>>& mapAvailableItem, std::map<Integer, std::set<int>>& mapTrafficMean, 
                                                      Integer nxOriginID, Integer nxDestinationID, std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem )
{
    try
    {
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select Tx1.OZone_ID as OZone_ID, Tx2.OZone_ID as TerminalID, Tx2.DZone_ID as DZone_ID, Tx1.Length as LengthA, Tx2.Length as LengthB From "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("   Select OZone_ID, DZone_ID, Length From TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '2') Tb "));
        strSQL.Append(_T("   Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
        strSQL.Append(_T(" )Tx1, "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("   Select OZone_ID, DZone_ID, Length From TTable_T2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb "));
        strSQL.Append(_T("   Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
        strSQL.Append(_T(" )Tx2 "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T(" Tx1.DZone_ID  = Tx2.OZone_ID and ")); 
        strSQL.Append(_T(" Tx2.OZone_ID != Tx2.DZone_ID and ")); 
        strSQL.AppendFormat(_T(" Tx1.OZone_ID  = '%I64d' and Tx2.DZone_ID = '%I64d' "), nxOriginID, nxDestinationID);  	

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        size_t nxItemCount = vecItemTimeCost.size();
        while ( spResultSet->Next() )
        {
            Integer nxOriginID   = spResultSet->GetValueInt64 (0);
            Integer nxTerminalID = spResultSet->GetValueInt64 (1);
            Integer nxDestID     = spResultSet->GetValueInt64 (2);
            double  dLengthA     = spResultSet->GetValueDouble(3);
            double  dLengthB     = spResultSet->GetValueDouble(4);

            // 물류시설이용가능 수단 도로 체크
            std::map<Integer, std::set<int>>::iterator iterTMean = mapTrafficMean.find(nxTerminalID);
            std::map<Integer, std::set<int>>::iterator endTMean  = mapTrafficMean.end();

            if (iterTMean == endTMean)
            {
                continue;
            }
            else
            {
                std::set<int>& setTMean = iterTMean->second;
                if (setTMean.find(1) == setTMean.end())
                {
                    continue;
                }
            }

            // 물류시설이용 품목 체크
            std::map<Integer, std::set<int>>::iterator iterAItem = mapAvailableItem.find(nxTerminalID);
            std::map<Integer, std::set<int>>::iterator endAItem  = mapAvailableItem.end();

            if (iterAItem == endAItem)
            {
                continue;
            }            

            std::set<int>& setAvailableItem     = iterAItem->second;
            std::set<int>::iterator endSetAItem = setAvailableItem.end();

            for (size_t i=0; i<nxItemCount; i++)
            {
                TItemTimeCost& oItemTimeCost = vecItemTimeCost[i];

                std::set<int>::iterator iterSetAItem =  setAvailableItem.find(oItemTimeCost.CommodityType);
                if(iterSetAItem == endSetAItem)
                {
                    continue;
                }

                double dRoadTime = oItemTimeCost.RoadTime_LoadUp + oItemTimeCost.RoadTime_LoadDown + (dLengthA + dLengthB) * oItemTimeCost.RoadTime_Trip;
                double dRoadCost = oItemTimeCost.RoadCost_LoadUp + oItemTimeCost.RoadCost_LoadDown + (dLengthA + dLengthB) * oItemTimeCost.RoadCost_Trip;

                dRoadTime = dRoadTime / 60.0;
                dRoadCost = dRoadCost / 1000.0;

                KSDoubleRecord* pRecord = oODKeyDoubleRecords.GetRecord(KODKey(nxOriginID, nxDestID));
                if (pRecord == nullptr)
                {
                    pRecord = oODKeyDoubleRecords.AddRecord(KODKey(nxOriginID, nxDestID));  
                    for (size_t j=0; j<nxItemCount*6; j++)
                    {
                        if (nxOriginID == nxDestID)
                        {
                            pRecord->SetAt(j, 0.0);
                        }
                        else
                        {
                            pRecord->SetAt(j, 99999999.0);
                        }                        
                    }
                }

                double dTempCost = pRecord->GetAt(i*6 + 3);
                if (dRoadCost < dTempCost)
                {
                    pRecord->SetAt(i*6 + 2, dRoadTime);
                    pRecord->SetAt(i*6 + 3, dRoadCost);

                    // 여기서
                    std::map<KODKey, TRoadViaPathInfo>& mapRoadViaPathInfo = vecFindRoadViaPathItem[i];
                    std::map<KODKey, TRoadViaPathInfo>::iterator iterRPath = mapRoadViaPathInfo.find(KODKey(nxOriginID, nxDestinationID));

                    if (iterRPath == mapRoadViaPathInfo.end())
                    {
                        TRoadViaPathInfo oRoadViaPathInfo;
                        oRoadViaPathInfo.FacilityViaID = nxTerminalID;
                        oRoadViaPathInfo.LengthOA = dLengthA;
                        oRoadViaPathInfo.LengthAD = dLengthB;

                        mapRoadViaPathInfo.insert(std::make_pair(KODKey(nxOriginID, nxDestinationID), oRoadViaPathInfo));
                    }
                    else
                    {
                        TRoadViaPathInfo& oRoadViaPathInfo = iterRPath->second;
                        oRoadViaPathInfo.FacilityViaID = nxTerminalID;
                        oRoadViaPathInfo.LengthOA = dLengthA;
                        oRoadViaPathInfo.LengthAD = dLengthB;
                    }
                }
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
}


void KFreightBulkPathGenerate::RailTimeCost( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress, std::map<Integer, std::set<int>>& mapAvailableItem, std::map<Integer, std::set<int>>& mapTrafficMean, Integer nxOriginID, Integer nxDestinationID )
{
    try
    {
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select Tx1.OZone_ID as OZone_ID, Tx2.OZone_ID as TerminalA, Tx2.DZone_ID as TerminalB, Tx3.DZone_ID as DZone_ID, Tx1.Length, Tx2.Length, Tx3.Length From "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("   Select OZone_ID, DZone_ID, Length From TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '2' ) Tb "));
        strSQL.Append(_T("   Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
        strSQL.Append(_T(" )Tx1, ")); 
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  Select OZone_ID, DZone_ID, Length From TTable_R2R Ta "));
        strSQL.Append(_T("  Where Length < 99999999 "));
        strSQL.Append(_T(" )Tx2, "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  Select OZone_ID, DZone_ID, Length From TTable_T2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb "));
        strSQL.Append(_T("  Where Ta.DZone_ID = Tb.node_id  and Length < 99999999 "));
        strSQL.Append(_T(" )Tx3 "));
        strSQL.Append(_T(" Where "));
        strSQL.Append(_T(" Tx1.DZone_ID =  Tx2.OZone_ID and "));
        strSQL.Append(_T(" Tx2.DZone_ID =  Tx3.OZone_ID and "));
        strSQL.Append(_T(" Tx2.OZone_ID != Tx2.DZone_ID and "));  
        strSQL.Append(_T(" Tx1.OZone_ID != Tx3.DZone_ID and "));
        strSQL.AppendFormat(_T(" Tx1.OZone_ID = '%I64d' and Tx3.DZone_ID = %I64d "), nxOriginID, nxDestinationID);  
        strSQL.Append(_T(" Limit 100 "));

        KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        size_t nxItemCount = vecItemTimeCost.size();

        while ( spResultSet->Next() )
        {
            Integer nxOriginID    = spResultSet->GetValueInt64(0);
            Integer nxTerminalIDA = spResultSet->GetValueInt64(1);
            Integer nxTerminalIDB = spResultSet->GetValueInt64(2);
            Integer nxDestID      = spResultSet->GetValueInt64 (3);
            double  dLengthA      = spResultSet->GetValueDouble(4);
            double  dLengthB      = spResultSet->GetValueDouble(5);
            double  dLengthC      = spResultSet->GetValueDouble(6);

            // 철도시설이용가능 수단 철도 체크(양쪽)
            std::map<Integer, std::set<int>>::iterator iterTMean = mapTrafficMean.find(nxTerminalIDA);
            std::map<Integer, std::set<int>>::iterator endTMean  = mapTrafficMean.end();

            if (iterTMean == endTMean)
            {
                continue;
            }
            else
            {                
                std::set<int>& setTMeanA = iterTMean->second;
                if (setTMeanA.find(2) == setTMeanA.end())
                {
                    continue;
                }

                iterTMean = mapTrafficMean.find(nxTerminalIDB);
                if (iterTMean == endTMean)
                {
                    continue;
                }
                std::set<int>& setTMeanB = iterTMean->second;
                if (setTMeanB.find(2) == setTMeanB.end())
                {
                    continue;
                }
            }

            // 물류시설이용 품목 체크
            std::map<Integer, std::set<int>>::iterator iterAItem = mapAvailableItem.find(nxTerminalIDA);
            std::map<Integer, std::set<int>>::iterator endAItem  = mapAvailableItem.end();

            if (iterAItem == endAItem)
            {
                continue;
            }            

            std::map<Integer, std::set<int>>::iterator iterBItem = mapAvailableItem.find(nxTerminalIDB);
            std::map<Integer, std::set<int>>::iterator endBItem  = mapAvailableItem.end();

            if (iterBItem == endBItem)
            {
                continue;
            }    

            std::set<int>& setAvailableItemA     = iterAItem->second;
            std::set<int>::iterator endSetAItemA = setAvailableItemA.end();

            std::set<int>& setAvailableItemB     = iterBItem->second;
            std::set<int>::iterator endSetAItemB = setAvailableItemB.end();

            for (size_t i=0; i<nxItemCount; i++)
            {
                TItemTimeCost& oItemTimeCost = vecItemTimeCost[i];

                std::set<int>::iterator iterSetAItemA =  setAvailableItemA.find(oItemTimeCost.CommodityType);
                if(iterSetAItemA == endSetAItemA)
                {
                    continue;
                }

                std::set<int>::iterator iterSetAItemB =  setAvailableItemB.find(oItemTimeCost.CommodityType);
                if(iterSetAItemB == endSetAItemB)
                {
                    continue;
                }

                double dRailTime = 2.0 * (oItemTimeCost.RailTime_LoadUp + oItemTimeCost.RailTime_LoadDown) + 
                                   (dLengthA + dLengthC) * oItemTimeCost.RailTime_Trip                     + 
                                   dLengthB / oItemTimeCost.RailTime_Speed / 60.0;
                double dRailCost = 2.0 * (oItemTimeCost.RailCost_LoadUp + oItemTimeCost.RailCost_LoadDown) + 
                                   (dLengthA + dLengthC) * oItemTimeCost.RailCost_RoadTrip                 + 
                                   dLengthB * oItemTimeCost.RailCost_RailTrip                              + 
                                   oItemTimeCost.RailCost_FacilityCost;

                dRailTime = dRailTime / 60.0;
                dRailCost = dRailCost / 1000.0;

                KSDoubleRecord* pRecord = oODKeyDoubleRecords.GetRecord(KODKey(nxOriginID, nxDestID));
                if (pRecord == nullptr)
                {
                    pRecord = oODKeyDoubleRecords.AddRecord(KODKey(nxOriginID, nxDestID));  
                    for (size_t j=0; j<nxItemCount*6; j++)
                    {
                        if (nxOriginID == nxDestID)
                        {
                            pRecord->SetAt(j, 0.0);
                        }
                        else
                        {
                            pRecord->SetAt(j, 99999999.0);
                        }                        
                    }
                }

                double dTempCost = pRecord->GetAt(i*6 + 5);
                if (dRailCost < dTempCost)
                {
                    pRecord->SetAt(i*6 + 4, dRailTime);
                    pRecord->SetAt(i*6 + 5, dRailCost);
                }
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
}


void KFreightBulkPathGenerate::DistRailTimeCost( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, KODKeyDoubleRecords& oODKeyDoubleRecords, QBicSimpleProgressPtr spSimpleProgress, 
                                                 std::map<Integer, std::set<int>>& mapAvailableItem, std::map<Integer, std::set<int>>& mapTrafficMean,
                                                 std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem)
{
    try
    {
        TxLogDebugStartMsg();
        struct ODLength
        {
            Integer OriginID;
            Integer DestinationID;
            double  Length;
        };
                                
        std::map<KODKey, TRailPathInfo>::iterator iterFind, endFind;
        
        std::vector<ODLength> vecOriginToTerminalALength;
        std::vector<ODLength> vecTerminalBToDestLength;
        
        CString strSQL(_T(""));
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("   OZone_ID, DZone_ID, Length "));
        strSQL.Append(_T(" From  TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '2' and Traffic_Mean like '%2%') Tb "));
        strSQL.Append(_T(" Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));     
        strSQL.Append(_T(" Order By Length "));

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        while ( spResultSet->Next() )
        {
            Integer nxOriginID = spResultSet->GetValueInt64 (0);
            Integer nxDestID   = spResultSet->GetValueInt64 (1);
            double  dLength    = spResultSet->GetValueDouble(2);

            ODLength oODLength;
            oODLength.OriginID      = nxOriginID;
            oODLength.DestinationID = nxDestID;
            oODLength.Length        = dLength;

            vecOriginToTerminalALength.push_back(oODLength);
        }

        TxLogDebugVisitor();
        strSQL = _T("");
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("   OZone_ID, DZone_ID, Length "));
        strSQL.Append(_T(" From  TTable_T2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb, (Select node_id From Node Where Node_Type = '2' and Traffic_Mean like '%2%') Tc "));
        strSQL.Append(_T(" Where Ta.DZone_ID = Tb.node_id and Length < 99999999 and Ta.OZone_ID = Tc.node_id  "));     
        strSQL.Append(_T(" Order By Length "));

        spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {            
            Integer nxOriginID = spResultSet->GetValueInt64 (0);
            Integer nxDestID   = spResultSet->GetValueInt64 (1);
            double  dLength    = spResultSet->GetValueDouble(2);
            
            ODLength oODLength;
            oODLength.OriginID      = nxOriginID;
            oODLength.DestinationID = nxDestID;
            oODLength.Length        = dLength;

            vecTerminalBToDestLength.push_back(oODLength);
        }

        TxLogDebugVisitor();
        std::map<KODKey, double> mapTerminalToTerminal;
        strSQL = _T("");
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("   OZone_ID, DZone_ID, Length "));
        strSQL.Append(_T(" From TTable_R2R "));
        strSQL.Append(_T(" Where Length < 99999999 "));

        spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
        while ( spResultSet->Next() )
        {
            Integer nxOriginID = spResultSet->GetValueInt64 (0);
            Integer nxDestID   = spResultSet->GetValueInt64 (1);
            double  dLength    = spResultSet->GetValueDouble(2);

            mapTerminalToTerminal.insert(std::make_pair(KODKey(nxOriginID, nxDestID), dLength));
        }

        size_t nxOriginToTerACount = vecOriginToTerminalALength.size();
        size_t nxTerBToDesCount    = vecTerminalBToDestLength.size();
                
        std::vector<std::set<Integer>> vecItemSerachOrigin;
        size_t nxItemCount = vecItemTimeCost.size();
        for (size_t m=0; m<nxItemCount; m++)
        {
            std::set<Integer> setSearchOrigin;
            vecItemSerachOrigin.push_back(setSearchOrigin);
        }

        for (size_t i=0; i<nxOriginToTerACount; i++)
        {
            ODLength& oOriginToTerminalALength  = vecOriginToTerminalALength[i];
              Integer nxOriginID    = oOriginToTerminalALength.OriginID;
              Integer nxTerminalAID = oOriginToTerminalALength.DestinationID;
              double  dLengthOA     = oOriginToTerminalALength.Length;
            
            CString strTemp;
            strTemp.Format(_T("Rail Path Search[%d/%d] : %I64d"), i, nxOriginToTerACount, nxOriginID);
            if (spSimpleProgress)
            {
                if (spSimpleProgress->IsUserStopped())
                    return;

                spSimpleProgress->SetStatus(strTemp);
            }

            if (IsAvailableMode(nxTerminalAID, 2, mapTrafficMean) == false)
                continue;
                        
            size_t nxFindItemCount(0);
            for (size_t j=0; j<nxItemCount; j++)
            {
                TItemTimeCost& oItemTimeCost = vecItemTimeCost[j];
                if (IsAvailableItem(nxTerminalAID, oItemTimeCost.CommodityType, mapAvailableItem) == false)
                    continue;
                
                std::set<Integer>& setSearchOrigin = vecItemSerachOrigin[j];
                if (setSearchOrigin.find(nxOriginID) == setSearchOrigin.end())
                {      
                    bool bOriginFindPath(false);
                    for(size_t k=0; k<nxTerBToDesCount; k++)
                    {
                        ODLength& oTerminalBDestLength  = vecTerminalBToDestLength[k];
                        Integer nxTerminalBID = oTerminalBDestLength.OriginID;
                        Integer nxDestID      = oTerminalBDestLength.DestinationID;
                        double  dLenghtBD     = oTerminalBDestLength.Length;

                        if (nxTerminalAID == nxTerminalBID)
                            continue;

                        if (nxOriginID == nxDestID)
                            continue;

                        //기존 경로 검색 결과 검색 - 이미 최단으로 검색된 것이 있으면.. 더이상 검색 하지 않는다.
                        std::map<KODKey, TRailPathInfo>& mapRailPath = vecFindRailPathItem[j];
                        iterFind = mapRailPath.find(KODKey(nxOriginID, nxDestID));
                        endFind  = mapRailPath.end();

                        if (iterFind != endFind)
                            continue;

                        //철도 존재 체크
                        std::map<KODKey, double>::iterator iterTT = mapTerminalToTerminal.find(KODKey(nxTerminalAID, nxTerminalBID));
                        std::map<KODKey, double>::iterator endTT  = mapTerminalToTerminal.end();
                        if (iterTT == endTT)
                            continue;

                        if (IsAvailableMode(nxTerminalBID, 2, mapTrafficMean) == false)
                            continue;

                        if (IsAvailableItem(nxTerminalBID, oItemTimeCost.CommodityType, mapAvailableItem) == false)
                            continue;

                        TRailPathInfo oPath;
                        oPath.TerminalA = nxTerminalAID;
                        oPath.TerminalB = nxTerminalBID;
                        oPath.LengthOA  = dLengthOA;
                        oPath.LengthAB  = iterTT->second;
                        oPath.LenghtBD  = dLenghtBD;

                        mapRailPath.insert(std::make_pair(KODKey(nxOriginID, nxDestID), oPath));

                        // 비용계산                    
                        double dRailTime = 2.0 * (oItemTimeCost.RailTime_LoadUp + oItemTimeCost.RailTime_LoadDown) + 
                                           (oPath.LengthOA + oPath.LenghtBD)    * oItemTimeCost.RailTime_Trip      + 
                                           (oPath.LengthAB / oItemTimeCost.RailTime_Speed * 60.0);
                        double dRailCost = 2.0 * (oItemTimeCost.RailCost_LoadUp + oItemTimeCost.RailCost_LoadDown) + 
                                           (oPath.LengthOA + oPath.LenghtBD)    * oItemTimeCost.RailCost_RoadTrip  + 
                                            oPath.LengthAB * oItemTimeCost.RailCost_RailTrip                       + 
                                            oItemTimeCost.RailCost_FacilityCost * 2.0;

                        dRailTime = dRailTime / 60.0;
                        dRailCost = dRailCost / 1000.0;

                        KSDoubleRecord* pRecord = oODKeyDoubleRecords.GetRecord(KODKey(nxOriginID, nxDestID));
                        if (pRecord == nullptr)
                        {
                            pRecord = oODKeyDoubleRecords.AddRecord(KODKey(nxOriginID, nxDestID));  
                            for (size_t m=0; m<nxItemCount*6; m++)
                            {
                                if (nxOriginID == nxDestID)
                                {
                                    pRecord->SetAt(m, 0.0);
                                }
                                else
                                {
                                    pRecord->SetAt(m, 99999999.0);
                                }                                
                            }
                        }

                        double dTempCost = pRecord->GetAt(j*6 + 5);
                        if (dRailCost < dTempCost)
                        {
                            pRecord->SetAt(j*6 + 4, dRailTime);
                            pRecord->SetAt(j*6 + 5, dRailCost);
                        }

                        bOriginFindPath = true;
                        if (spSimpleProgress)
                        {
                            if (spSimpleProgress->IsUserStopped())
                                return;
                        }
                    }

                    if (bOriginFindPath)
                        setSearchOrigin.insert(nxOriginID);
                }                
            }
        }

        /*std::map<KODKey, RailPath>& mapRailPath = vecFindRailPathItem[0];
        CString strOut;
        strOut.Format(_T("Count : %d"), mapRailPath.size());*/

        //AfxMessageBox(strOut);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }
    TxLogDebugEndMsg();
}


bool KFreightBulkPathGenerate::IsAvailableMode( Integer nxNodeID, int nMode, std::map<Integer, std::set<int>>& mapTrafficMean )
{
    bool bResult(false);

    try
    {
        std::map<Integer, std::set<int>>::iterator iter = mapTrafficMean.find(nxNodeID);
        std::map<Integer, std::set<int>>::iterator end  = mapTrafficMean.end();

        if (iter == end)
            return false;

        std::set<int>& setItem = iter->second;
        std::set<int>::iterator iterSet = setItem.find(nMode);
        std::set<int>::iterator endSet  = setItem.end();

        if (iterSet == endSet)
            return false;

        return true;
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return bResult;
}

bool KFreightBulkPathGenerate::IsAvailableItem (Integer nxNodeID, int nItem, std::map<Integer, std::set<int>>& mapAvailableItem)
{
    bool bResult(false);

    try
    {
        std::map<Integer, std::set<int>>::iterator iter = mapAvailableItem.find(nxNodeID);
        std::map<Integer, std::set<int>>::iterator end  = mapAvailableItem.end();

        if (iter == end)
            return false;

        std::set<int>& setItem = iter->second;
        std::set<int>::iterator iterSet = setItem.find(nItem);
        std::set<int>::iterator endSet  = setItem.end();

        if (iterSet == endSet)
            return false;

        return true;
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }

    return bResult;
}



void KFreightBulkPathGenerate::QueryTime( KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress )
{
    TxLogDebugStartMsg();

    CString strSQL(_T(""));
    strSQL.Append(_T(" Select "));
    strSQL.Append(_T("     Tx1.OZone_ID as OZone_ID, Tx1.DZone_ID as TerminalID, Tx2.DZone_ID as DZone_ID, Tx1.Length as LengthA, Tx2.Length as LengthB "));
    strSQL.Append(_T("     From "));
    strSQL.Append(_T("     ( "));
    strSQL.Append(_T("     Select "));
    strSQL.Append(_T("     OZone_ID, DZone_ID, Length "));
    strSQL.Append(_T("     From TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '2') Tb "));
    strSQL.Append(_T("     Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
    strSQL.Append(_T("     ) Tx1, "));
    strSQL.Append(_T("     ( "));
    strSQL.Append(_T("     Select "));
    strSQL.Append(_T("     OZone_ID, DZone_ID, Length "));
    strSQL.Append(_T("     From TTable_T2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb "));
    strSQL.Append(_T("     Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
    strSQL.Append(_T("     ) Tx2 "));
    strSQL.Append(_T("     Where "));
    strSQL.Append(_T("     Tx1.DZone_ID  = Tx2.OZone_ID and  "));
    strSQL.Append(_T("     Tx1.OZone_ID != Tx2.DZone_ID ")); 

    KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

    Integer nxCount(1);
    while(spResultSet->Next())
    {
        if (spSimpleProgress)
        {
            CString strTemp;
            strTemp.Format(_T("QueryTime - %I64d"), nxCount);
            if (spSimpleProgress->IsUserStopped())
                return;

            spSimpleProgress->SetStatus(strTemp);
        }
        nxCount++;
    }

    CString strOut;
    strOut.Format(_T("Count : %I64d"), nxCount);

    TxLogDebug((LPCTSTR)strOut);
    TxLogDebugEndMsg();
}

void KFreightBulkPathGenerate::QueryTime2( KTarget* pTarget, QBicSimpleProgressPtr spSimpleProgress )
{
    TxLogDebugStartMsg();

    CString strSQL(_T(""));
    strSQL.Append(_T(" Select Tx1.OZone_ID, Tx2.OZone_ID, Tx2.DZone_ID, Tx3.DZone_ID, Tx1.Length, Tx2.Length, Tx3.Length From "));
    strSQL.Append(_T("     ( "));
    strSQL.Append(_T("     Select OZone_ID, DZone_ID, Length From TTable_O2TD Ta, (Select node_id From Node Where Node_Type = '2' and Traffic_Mean like '%2%') Tb "));
    strSQL.Append(_T("     Where Ta.DZone_ID = Tb.node_id and Length < 99999999 "));
    strSQL.Append(_T("     )Tx1, "));
    strSQL.Append(_T("     ( "));
    strSQL.Append(_T("     Select OZone_ID, DZone_ID, Length From TTable_R2R Ta "));
    strSQL.Append(_T("     Where Length < 99999999 "));
    strSQL.Append(_T("     )Tx2, "));
    strSQL.Append(_T("     ( "));
    strSQL.Append(_T("     Select OZone_ID, DZone_ID, Length From TTable_T2TD Ta, (Select node_id From Node Where Node_Type = '0') Tb, (Select node_id From Node Where Node_Type = '2' and Traffic_Mean like '%2%') Tc "));
    strSQL.Append(_T("     Where Ta.DZone_ID = Tb.node_id and Length < 99999999 and Ta.OZone_ID = Tc.node_id "));
    strSQL.Append(_T("     )Tx3 "));
    strSQL.Append(_T("     Where "));
    strSQL.Append(_T("     Tx1.DZone_ID =  Tx2.OZone_ID and "));
    strSQL.Append(_T("     Tx2.DZone_ID =  Tx3.OZone_ID and "));
    strSQL.Append(_T("     Tx2.OZone_ID != Tx2.DZone_ID and "));
    strSQL.Append(_T("     Tx1.OZone_ID != Tx3.DZone_ID and "));
    strSQL.Append(_T("     Tx1.OZone_ID = 1 "));

	KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
    KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

    Integer nxCount(1);
    while(spResultSet->Next())
    {
        if (spSimpleProgress)
        {
            CString strTemp;
            strTemp.Format(_T("QueryTime2 - %I64d"), nxCount);
            if (spSimpleProgress->IsUserStopped())
                return;

            spSimpleProgress->SetStatus(strTemp);
        }
        nxCount++;
    }

    CString strOut;
    strOut.Format(_T("Count : %I64d"), nxCount);

    TxLogDebug((LPCTSTR)strOut);
    TxLogDebugEndMsg();
}


void KFreightBulkPathGenerate::CreateTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress )
{
    try
    {
        size_t nxCount = vecItemTimeCost.size();
        if (nxCount == 0)
            return;

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
                
        try
        {
            CString strSQLDrop (_T(" DROP TABLE TTablePathInfo "));
            spDBaseConnection->ExecuteUpdate(strSQLDrop);
        }
        catch (...)
        {
        }

        CString strSQLCreate(_T(""));
        strSQLCreate.Append(_T( " CREATE TABLE TTablePathInfo "));
        strSQLCreate.Append(_T( "  (ozone_id INTEGER NOT NULL,  "));
        strSQLCreate.Append(_T( "   dzone_id INTEGER NOT NULL,  "));

        for (size_t i=0; i<nxCount; i++)
        {            
            strSQLCreate.AppendFormat(_T(" TerminalA_%d Integer, "), vecItemTimeCost[i].CommodityType);
            strSQLCreate.AppendFormat(_T(" TerminalB_%d Integer, "), vecItemTimeCost[i].CommodityType);
            strSQLCreate.AppendFormat(_T(" RoadVia_%d   Integer, "), vecItemTimeCost[i].CommodityType);
        }

        strSQLCreate.Append(_T( "   PRIMARY KEY (ozone_id, dzone_id)) "));

        spDBaseConnection->ExecuteUpdate(strSQLCreate);
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
        throw ex;
    }
    catch (...)
    {
        TxLogDebugException();
        throw;
    }    
}


void KFreightBulkPathGenerate::CreatePathInfoTable( KTarget* pTarget, std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo, QBicSimpleProgressPtr spSimpleProgress )
{
	try
	{
		int nCount = ar_mapPathInfo.size();
		if (nCount == 0)
			return;

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		try
		{
			CString strSQLDrop (_T(" DROP TABLE TTablePathInfo "));
			spDBaseConnection->ExecuteUpdate(strSQLDrop);
		}
		catch (...)
		{

		}

		CString strSQLCreate(_T(""));
		strSQLCreate.Append(_T( " CREATE TABLE TTablePathInfo "));
		strSQLCreate.Append(_T( "  ( [ozone_id] INTEGER NOT NULL,  "));
		strSQLCreate.Append(_T( "   [dzone_id] INTEGER NOT NULL,  "));

		std::vector<TViaColumnInfo> vecViaColumnInfo;

		AutoType iter = ar_mapPathInfo.begin();
		AutoType end  = ar_mapPathInfo.end();
		while(iter != end)
		{
			CString strTerminalA(_T(""));
			strTerminalA.Format(_T("TerminalA_%d"), iter->first);

			CString strTerminalB(_T(""));
			strTerminalB.Format(_T("TerminalB_%d"), iter->first);

			CString strRoadVia(_T(""));
			strRoadVia.Format(_T("RoadVia_%d"), iter->first);

			strSQLCreate.AppendFormat(_T(" [%s] Integer, "), strTerminalA);
			strSQLCreate.AppendFormat(_T(" [%s] Integer, "), strTerminalB);
			strSQLCreate.AppendFormat(_T(" [%s] Integer, "), strRoadVia);

			TViaColumnInfo oInfo = {};
			oInfo.nID			= iter->first;
			oInfo.strTerminalA	= strTerminalA;
			oInfo.strTerminalB	= strTerminalB;
			oInfo.strRoadVia	= strRoadVia;

			vecViaColumnInfo.push_back(oInfo);
			++iter;
		}
		
		strSQLCreate.Append(_T( " PRIMARY KEY ([ozone_id], [dzone_id])) "));

		spDBaseConnection->ExecuteUpdate(strSQLCreate);

		CString strDelColumnSQL(_T(""));
		strDelColumnSQL.Format(_T("Delete From %s"), TABLE_T_TERMINAL_PATHCOLUMN_INFO);
		spDBaseConnection->ExecuteUpdate(strDelColumnSQL);


		size_t nModeCount = vecViaColumnInfo.size();
		for (size_t i = 0; i < nModeCount; i++)
		{
			TViaColumnInfo oInfo = vecViaColumnInfo[i];
			CString strInsertSQL(_T(""));
			strInsertSQL.Format(_T("Insert Into %s( ID, TerminalA, TerminalB, RoadVia ) Values ( %d, '%s', '%s', '%s')"), TABLE_T_TERMINAL_PATHCOLUMN_INFO, 
									oInfo.nID, oInfo.strTerminalA, oInfo.strTerminalB, oInfo.strRoadVia);
			spDBaseConnection->ExecuteUpdate(strInsertSQL);
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
		throw;
	}    
}

void KFreightBulkPathGenerate::InsertTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, 
                                                     std::vector<Integer>& vecZoneNodeID, 
                                                     std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem, 
                                                     std::vector<std::map<KODKey, TRailPathInfo>>&    vecFindRailPathItem )
{
    try
    {
    	size_t nxZoneCount = vecZoneNodeID.size();
        if (nxZoneCount == 0)
            return;


        size_t nxItemCount = vecItemTimeCost.size();
        if (nxItemCount == 0)
            return;

        KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        CString strSQLInsert(_T(""));
        strSQLInsert.Append(_T(" Insert Into TTablePathInfo (OZone_ID, DZone_ID "));
        
        CString strValue(_T(""));
        for(size_t i=0; i<nxItemCount; i++)
        {
            int nCommodityType = vecItemTimeCost[i].CommodityType;
            strSQLInsert.AppendFormat(_T(" , TerminalA_%d, TerminalB_%d, RoadVia_%d "), nCommodityType, nCommodityType, nCommodityType);

            strValue.Append(_T(",?, ?, ? "));    
        }

        strSQLInsert.AppendFormat(_T(" ) Values(?, ? %s) "), strValue);
        KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

        int nIndexCount = (int)nxItemCount;
        int nIndex(0);


        for (size_t i=0; i<nxZoneCount; i++)
        {
            Integer nxOZoneID = vecZoneNodeID[i];
            Integer nxDZoneID(0);
            for (size_t j=0; j<nxZoneCount; j++)
            {
                nxDZoneID = vecZoneNodeID[j];
                KODKey oKey(nxOZoneID, nxDZoneID);
                spPreparedStatement->BindInt64 (1, oKey.OriginID);
                spPreparedStatement->BindInt64 (2, oKey.DestinationID);

                for(size_t nxItem=0; nxItem<nxItemCount; nxItem++)
                {
                    std::map<KODKey, TRoadViaPathInfo>& mapRoadViaPathItem = vecFindRoadViaPathItem[nxItem];
                    std::map<KODKey, TRailPathInfo>&    mapRailPathInfo    = vecFindRailPathItem[nxItem];
                    
                    nIndex = 3 * (int)nxItem;

                    AutoType iterRail = mapRailPathInfo.find(oKey);
                    if ( iterRail != mapRailPathInfo.end())
                    {
                        TRailPathInfo& oRailPathInfo = iterRail->second;

                        spPreparedStatement->BindInt64(3 + nIndex, oRailPathInfo.TerminalA); 
                        spPreparedStatement->BindInt64(4 + nIndex, oRailPathInfo.TerminalB); 
                    }
                    else
                    {
                        spPreparedStatement->BindInt64(3 + nIndex, 0); 
                        spPreparedStatement->BindInt64(4 + nIndex, 0); 
                    }
                    
                    AutoType iterRoad = mapRoadViaPathItem.find(oKey);
                    if ( iterRoad != mapRoadViaPathItem.end())
                    {
                        TRoadViaPathInfo& oRoadViaPathInfo = iterRoad->second;
                        spPreparedStatement->BindInt64(5 + nIndex, oRoadViaPathInfo.FacilityViaID); 
                    }
                    else
                    {
                        spPreparedStatement->BindInt64(5 + nIndex, 0); 
                    }
                }

                // Insert    
                spPreparedStatement->ExecuteUpdate();
                spPreparedStatement->Reset();

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
}


void KFreightBulkPathGenerate::InsertPathInfoTable( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, 
	std::vector<Integer>& vecZoneNodeID, 
	std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo )
{
	try
	{
		size_t nxZoneCount = vecZoneNodeID.size();
		if (nxZoneCount == 0)
			return;

		size_t nxItemCount = vecItemTimeCost.size();
		if (nxItemCount == 0)
			return;

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		CString strSQLInsert(_T(""));
		strSQLInsert.Append(_T(" Insert Into TTablePathInfo (ozone_id, dzone_id "));

		CString strValue(_T(""));
		AutoType iter = ar_mapPathInfo.begin();
		AutoType end  = ar_mapPathInfo.end();
		while(iter != end)
		{
			int nCommodityType = iter->first;

			strSQLInsert.AppendFormat(_T(" , TerminalA_%d, TerminalB_%d, RoadVia_%d "), nCommodityType, nCommodityType, nCommodityType);

			strValue.Append(_T(",?, ?, ? ")); 
			++iter;
		}

		strSQLInsert.AppendFormat(_T(" ) Values(?, ? %s) "), strValue);

		KPreparedStatementPtr spPreparedStatement = spDBaseConnection->PrepareStatement(strSQLInsert);

		int nIndexCount = (int)nxItemCount;

		for (size_t i=0; i<nxZoneCount; i++)
		{
			Integer nxOZoneID = vecZoneNodeID[i];
			Integer nxDZoneID(0);
			for (size_t j=0; j<nxZoneCount; j++)
			{
				nxDZoneID = vecZoneNodeID[j];
				KODKey oKey(nxOZoneID, nxDZoneID);
				spPreparedStatement->BindInt64 (1, oKey.OriginID);
				spPreparedStatement->BindInt64 (2, oKey.DestinationID);

				if (spSimpleProgress->IsUserStopped())
					return;

				CString strTemp(_T(""));
				strTemp.Format(_T(" 경로 저장 : %I64d -> %I64d"), nxOZoneID, nxDZoneID);
				spSimpleProgress->SetStatus(strTemp);

				int nIndex(0);

				AutoType iterPath = ar_mapPathInfo.begin();
				AutoType endPath  = ar_mapPathInfo.end();
				while(iterPath != endPath)
				{
					int nLIndex = nIndex * 3;
					std::map<KODKey, TViaTermianlInfo>& mapTermianlInfo = iterPath->second;

					AutoType findTerminal = mapTermianlInfo.find(oKey);
					AutoType endTermminal = mapTermianlInfo.end();
					if (findTerminal != endTermminal)
					{
						TViaTermianlInfo& oTerminalInfo = findTerminal->second;
						spPreparedStatement->BindInt64(3 + nLIndex, oTerminalInfo.TermianlA); 
						spPreparedStatement->BindInt64(4 + nLIndex, oTerminalInfo.TermianlB); 
						spPreparedStatement->BindInt64(5 + nLIndex, oTerminalInfo.RoadVia);
					}
					else
					{
						spPreparedStatement->BindInt64(3 + nLIndex, 0); 
						spPreparedStatement->BindInt64(4 + nLIndex, 0); 
						spPreparedStatement->BindInt64(5 + nLIndex, 0);
					}

					nIndex++;
					iterPath++;
				}

				spPreparedStatement->ExecuteUpdate();
				spPreparedStatement->Reset();
			}
		}       
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KFreightBulkPathGenerate::WriteTTablePathInfo( KTarget* pTarget, std::vector<TItemTimeCost>& vecItemTimeCost, QBicSimpleProgressPtr spSimpleProgress, std::vector<Integer>& vecZoneNodeID, std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem, std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem )
{
    try
    {
        size_t nxZoneCount = vecZoneNodeID.size();
        if (nxZoneCount == 0)
            return;

        size_t nxItemCount = vecItemTimeCost.size();
        if (nxItemCount == 0)
            return;
        
        int nIndexCount = (int)nxItemCount;
        int nIndex(0);

        QBicLoggerPtr spLogger = QBicLoggerPtr(new QBicLogger());	
        spLogger->LoggerOpen(pTarget->GetLocation(), _T("path_log"), _T("txt"));
        
        for (size_t i=0; i<nxZoneCount; i++)
        {
            Integer nxOZoneID = vecZoneNodeID[i];
            Integer nxDZoneID(0);
            for (size_t j=0; j<nxZoneCount; j++)
            {
                nxDZoneID = vecZoneNodeID[j];
                KODKey oKey(nxOZoneID, nxDZoneID);
                
                CString strLogOut(_T(""));
                strLogOut.AppendFormat(_T("%I64d, %I64d : "), nxOZoneID, nxDZoneID);

                for(size_t nxItem=0; nxItem<nxItemCount; nxItem++)
                {
                    std::map<KODKey, TRoadViaPathInfo>& mapRoadViaPathItem = vecFindRoadViaPathItem[nxItem];
                    std::map<KODKey, TRailPathInfo>&    mapRailPathInfo    = vecFindRailPathItem[nxItem];

                    nIndex = 3 * (int)nxItem;

                    AutoType iterRoad = mapRoadViaPathItem.find(oKey);
                    if ( iterRoad != mapRoadViaPathItem.end())
                    {
                        TRoadViaPathInfo& oRoadViaPathInfo = iterRoad->second;
                        strLogOut.AppendFormat(_T(",%I64d, %.2f, %I64d, %.2f, %I64d"), 
                                                   nxOZoneID, 
                                                   oRoadViaPathInfo.LengthOA, oRoadViaPathInfo.FacilityViaID, oRoadViaPathInfo.LengthAD, 
                                                   nxDZoneID);
                    }
                    else
                    {
                        strLogOut.Append(_T(",---, ---, ---, ---, ---"));
                    }

                    AutoType iterRail = mapRailPathInfo.find(oKey);
                    if ( iterRail != mapRailPathInfo.end())
                    {
                        TRailPathInfo& oRailPathInfo = iterRail->second;

                        strLogOut.AppendFormat(_T(" ,%I64d, %.2f, %I64d, %.2f, %I64d, %.2f, %I64d"), 
                                                    nxOZoneID, 
                                                    oRailPathInfo.LengthOA, oRailPathInfo.TerminalA, 
                                                    oRailPathInfo.LengthAB, oRailPathInfo.TerminalB, 
                                                    oRailPathInfo.LenghtBD, 
                                                    nxDZoneID);
                    }
                    else
                    {
                        strLogOut.Append(_T(" ,---, ---, ---, ---, ---, ---, ---"));
                    }                    
                }

                spLogger->WriteLog(strLogOut);
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
}


void KFreightBulkPathGenerate::FreightLinkCollection( KTarget* pTarget, KLinkCollection& oRoadLinkCollection, KLinkCollection& oRailLinkCollection, QBicSimpleProgressPtr spSimpleProgress )
{
    try
    {          
        oRoadLinkCollection.RemoveAll();
        oRailLinkCollection.RemoveAll();

        if (spSimpleProgress)
        {
            spSimpleProgress->SetStatus(_T("Initial Reading Data"));
        }

        CString  strSQL;
        strSQL.Format(_T(" Select %s, %s, %s, %s, %s From %s "), COLUMN_LINK_ID, COLUMN_FNODE_ID, COLUMN_TNODE_ID, COLUMN_LINK_LENGTH, COLUMN_LINK_TYPE, TABLE_LINK);

		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();
        KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

        while(spResultSet->Next())
        {
            Integer nxLinkID  = spResultSet->GetValueInt64 (0);
            Integer nxFNodeID = spResultSet->GetValueInt64 (1);
            Integer nxTNodeID = spResultSet->GetValueInt64 (2);
            double  dLength   = spResultSet->GetValueDouble(3);
            int     nLinkType = spResultSet->GetValueInt   (4);

            if (nLinkType == 9)
            {
                KLink* pLink = oRailLinkCollection.GetExistOrNew(nxLinkID);
                pLink->SetValues(nxFNodeID, nxTNodeID, dLength);
            }
            else
            {
                KLink* pLink = oRoadLinkCollection.GetExistOrNew(nxLinkID);
                pLink->SetValues(nxFNodeID, nxTNodeID, dLength);
            }            
        }
    }
    catch (KExceptionPtr e)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KFreightBulkPathGenerate::BulkPathLinks( KTarget* pTarget,  KLinkCollection& oLinkCollection, 
                                              Integer nxStartID, std::set<Integer>& setDestination, 
                                              std::set<Integer>& setAppendPathLinks, QBicSimpleProgressPtr spSimpleProgress )
{   
    KCostLinkCollection  oCostLinkCollection;
    KNodeArrivedMinLink  oNodeArrivedMinLink;
    
    try
    {
        KShortestPath::MuLinkBaseExecute(nxStartID, setDestination, oLinkCollection, oCostLinkCollection, oNodeArrivedMinLink);
        std::set<Integer>::iterator iter = setDestination.begin();
        std::set<Integer>::iterator end  = setDestination.end();
        while (iter != end)
        {
            Integer nxDestinationID = *iter;
            std::vector<Integer> vecPathLink;
            KShortestPath::ExtractPath(nxDestinationID, oCostLinkCollection, oNodeArrivedMinLink, vecPathLink);

            for (size_t i=0; i<vecPathLink.size(); i++)
            {
                setAppendPathLinks.insert(vecPathLink[i]);
            }
            ++iter;
        }
    }
    catch (KExceptionPtr e)
    {
        TxLogDebugException();
    }
    catch (...)
    {
        TxLogDebugException();
    }
}

void KFreightBulkPathGenerate::GetExistPathInfo( KTarget* pTarget, std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo )
{
	ar_mapPathInfo.clear();
	std::map<int, TViaColumnInfo> mapColumnInfo;
	CString strSQL(_T(""));

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		strSQL.Format(_T("Select ID, TerminalA, TerminalB, RoadVia from %s order by ID"), TABLE_T_TERMINAL_PATHCOLUMN_INFO);

		TxLogDebug((LPCTSTR)strSQL);

		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			TViaColumnInfo oInfo = {};
			oInfo.nID			= spResultSet->GetValueInt(0);
			oInfo.strTerminalA	= spResultSet->GetValueString(1);
			oInfo.strTerminalB	= spResultSet->GetValueString(2);
			oInfo.strRoadVia	= spResultSet->GetValueString(3);

			mapColumnInfo.insert(std::make_pair(oInfo.nID, oInfo));
		}

		if (mapColumnInfo.size() < 1)
		{
			return;
		}

		AutoType iter = mapColumnInfo.begin();
		AutoType end  = mapColumnInfo.end();
		while(iter != end)
		{
			CString strSelectSQL(_T(""));

			TViaColumnInfo oInfo = iter->second;

			strSelectSQL.Format(_T("Select ozone_id, dzone_id, %s , %s, %s From %s "), oInfo.strTerminalA, oInfo.strTerminalB, oInfo.strRoadVia, TABLE_F_TERMINAL_PATH_INFO);

			TxLogDebug((LPCTSTR)strSelectSQL);

			KResultSetPtr spSelResult = spDBaseConnection->ExecuteQuery(strSelectSQL);

			std::map<KODKey, TViaTermianlInfo> mapViaTerminalInfo;
			
			while(spSelResult->Next())
			{
				KODKey oODkey;
				oODkey.OriginID			= spSelResult->GetValueInt64(0);
				oODkey.DestinationID	= spSelResult->GetValueInt64(1);

				TViaTermianlInfo oViaInfo = {};
				oViaInfo.TermianlA		= spSelResult->GetValueInt64(2);
				oViaInfo.TermianlB		= spSelResult->GetValueInt64(3);
				oViaInfo.RoadVia		= spSelResult->GetValueInt64(4);

				mapViaTerminalInfo.insert(std::make_pair(oODkey, oViaInfo));
			}

			ar_mapPathInfo.insert(std::make_pair(oInfo.nID, mapViaTerminalInfo));

			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KFreightBulkPathGenerate::GetNewPathInfo(	std::vector<TItemTimeCost>& vecItemTimeCost, 
												std::vector<Integer>& vecZoneNodeID, 
												std::vector<std::map<KODKey, TRoadViaPathInfo>>& vecFindRoadViaPathItem, 
												std::vector<std::map<KODKey, TRailPathInfo>>& vecFindRailPathItem, 
												std::map<int, std::map<KODKey,TViaTermianlInfo>>& ar_mapPathInfo )
{
	try
	{
		ar_mapPathInfo.clear();

		size_t nxZoneCount = vecZoneNodeID.size();
		if (nxZoneCount == 0)
			return;

		size_t nxItemCount = vecItemTimeCost.size();
		if (nxItemCount == 0)
			return;

		for(size_t nxItem=0; nxItem<nxItemCount; nxItem++)
		{
			TItemTimeCost oTCInfo = vecItemTimeCost[nxItem];

			std::map<KODKey, TRoadViaPathInfo>& mapRoadViaPathItem = vecFindRoadViaPathItem[nxItem];
			std::map<KODKey, TRailPathInfo>&    mapRailPathInfo    = vecFindRailPathItem[nxItem];

			std::map<KODKey,TViaTermianlInfo> mapViaTerminalInfo;

			for (size_t i=0; i<nxZoneCount; i++)
			{
				Integer nxOZoneID = vecZoneNodeID[i];
				Integer nxDZoneID(0);
				for (size_t j=0; j<nxZoneCount; j++)
				{
					nxDZoneID = vecZoneNodeID[j];
					KODKey oKey(nxOZoneID, nxDZoneID);

					TViaTermianlInfo oTerminalInfo = {};

					AutoType findRoad = mapRoadViaPathItem.find(oKey);
					AutoType endRoad  = mapRoadViaPathItem.end();
					if (findRoad != endRoad)
					{
						oTerminalInfo.RoadVia = findRoad->second.FacilityViaID;
					}

					AutoType findRail = mapRailPathInfo.find(oKey);
					AutoType endRail  = mapRailPathInfo.end();
					if (findRail != endRail)
					{
						oTerminalInfo.TermianlA = findRail->second.TerminalA;
						oTerminalInfo.TermianlB = findRail->second.TerminalB;
					}

					mapViaTerminalInfo.insert(std::make_pair(oKey, oTerminalInfo));
				}
			}

			ar_mapPathInfo.insert(std::make_pair(oTCInfo.CommodityType, mapViaTerminalInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KFreightBulkPathGenerate::GetTableO2TD( KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress )
{

	CString strSQL(_T(""));
	//ar_setLinkID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		strSQL.Format(_T("Select Links From TTable_O2TD Where OZone_ID = %I64d and DZone_ID = %I64d "), a_nxOZoneID, a_nxDZoneID);

		KResultSetPtr spResultPtr = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLinks(_T(""));
		if (spResultPtr->Next())
		{
			strLinks = spResultPtr->GetValueString(0);
		}

		std::vector<CString> vecValues = KParseString::ParseString(strLinks, _T(','));
		size_t nCount = vecValues.size();
		for (size_t i = 0; i <nCount; i++)
		{
			CString strLink = vecValues[i];
			Integer nxLink = _ttoi64(strLink);
			ar_setLinkID.insert(nxLink);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KFreightBulkPathGenerate::GetTableR2R( KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress )
{
	CString strSQL(_T(""));
	//ar_setLinkID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		strSQL.Format(_T("Select Links From TTable_R2R Where OZone_ID = %I64d and DZone_ID = %I64d "), a_nxOZoneID, a_nxDZoneID);

		KResultSetPtr spResultPtr = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLinks(_T(""));
		if (spResultPtr->Next())
		{
			strLinks = spResultPtr->GetValueString(0);
		}

		std::vector<CString> vecValues = KParseString::ParseString(strLinks, _T(','));
		size_t nCount = vecValues.size();
		for (size_t i = 0; i <nCount; i++)
		{
			CString strLink = vecValues[i];
			Integer nxLink = _ttoi64(strLink);
			ar_setLinkID.insert(nxLink);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KFreightBulkPathGenerate::GetTableT2TD( KTarget* pTarget, Integer a_nxOZoneID, Integer a_nxDZoneID, std::set<Integer>& ar_setLinkID, QBicSimpleProgressPtr a_spSimpleProgress )
{
	CString strSQL(_T(""));
	//ar_setLinkID.clear();

	try
	{
		KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

		strSQL.Format(_T("Select Links From TTable_T2TD Where OZone_ID = %I64d and DZone_ID = %I64d "), a_nxOZoneID, a_nxDZoneID);

		KResultSetPtr spResultPtr = spDBaseConnection->ExecuteQuery(strSQL);

		CString strLinks(_T(""));
		if (spResultPtr->Next())
		{
			strLinks = spResultPtr->GetValueString(0);
		}

		std::vector<CString> vecValues = KParseString::ParseString(strLinks, _T(','));
		size_t nCount = vecValues.size();
		for (size_t i = 0; i <nCount; i++)
		{
			CString strLink = vecValues[i];
			Integer nxLink = _ttoi64(strLink);
			ar_setLinkID.insert(nxLink);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

