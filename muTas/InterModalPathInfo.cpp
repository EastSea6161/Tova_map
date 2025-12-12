#include "StdAfx.h"
#include "InterModalPathInfo.h"
#include "MultiModalResultInfo.h"

KInterModalPathInfo::KInterModalPathInfo(void)
{
}

KInterModalPathInfo::~KInterModalPathInfo(void)
{
}

void KInterModalPathInfo::GetStatoinIDFromSEQ( KTarget* pTarget, int a_nSeq, Integer a_nxLineID, Integer& a_nxStationID )
{
	KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

	try
	{
		int nSEQ = a_nSeq - 1;

		CString strSQL (_T(""));
		if (nSEQ == 0 ) {
			strSQL.Format(_T(" select tB.fnode_id from %s tA, %s tB where tA.transit_id = %I64d and tA.seq = 1 and tA.link_id = tB.link_id "), TABLE_TRANSITLINKS, TABLE_LINK, a_nxLineID); //★ 필요
		}
		else {
			strSQL.Format(_T(" select tB.tnode_id from %s tA, %s tB where tA.transit_id = %I64d and tA.seq = %d and tA.link_id = tB.link_id "), TABLE_TRANSITLINKS, TABLE_LINK, a_nxLineID, nSEQ); //★ 필요
		}

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResult->Next()) {
			a_nxStationID = spResult->GetValueInt64(0);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KInterModalPathInfo::Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint )
{
	KMultiModalResultInfo::Node2XYCoordinate(pTarget, oODKey, a_vecMapPoint);
}

void KInterModalPathInfo::NewGetStatoinIDFromSEQ( std::map<Integer, std::vector<Integer>> &a_mapLineSeq, std::map<Integer, TLinkFTNodeID> &a_mapLinkFTNode, int a_nSeq, Integer a_nxTransitID, Integer& a_nxStationID )
{
	try
	{
		AutoType iter  = a_mapLineSeq.find(a_nxTransitID);
		AutoType itEnd = a_mapLineSeq.end();

		int nSEQ = a_nSeq - 1;

		if(iter != itEnd)
		{
			std::vector<Integer> &vecTransitLinks = iter->second;
			int nSize = (int)vecTransitLinks.size();

			int nConvertSeq(0);
			if (nSEQ == 0 )
			{
				nConvertSeq = 1;
				int nArrIdx = nConvertSeq-1;

				if(nArrIdx >= nSize)
					throw 1;

				Integer &nxLinkID = vecTransitLinks[nArrIdx];

				AutoType iterLink  = a_mapLinkFTNode.find(nxLinkID);
				AutoType itLinkEnd = a_mapLinkFTNode.end();

				if(iterLink != itLinkEnd)
				{
					TLinkFTNodeID &oTLinkFTNodeID = iterLink->second;
					a_nxStationID = oTLinkFTNodeID.TFromNodeID;
				}
			}
			else
			{
				nConvertSeq = nSEQ;
				int nArrIdx = nConvertSeq-1;

				if(nArrIdx >= nSize)
					throw 1;

				Integer &nxLinkID = vecTransitLinks[nArrIdx];

				AutoType iterLink  = a_mapLinkFTNode.find(nxLinkID);
				AutoType itLinkEnd = a_mapLinkFTNode.end();

				if(iterLink != itLinkEnd)
				{
					TLinkFTNodeID &oTLinkFTNodeID = iterLink->second;
					a_nxStationID = oTLinkFTNodeID.TToNodeID;
				}
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
		TxLogDebugException();
		throw 1;
	}
}

void KInterModalPathInfo::CreateRegionalMainTrip(std::vector<TMainPathInfo> &a_vecMainPathInfo, 
										OUT std::set<Integer> &a_setOriginZoneID, std::set<Integer> &a_setDestinationZoneID, std::set<Integer> &a_setTerminalID, std::set<Integer> &a_setTransTerminalID,
										OUT std::map<int, std::map<KODKey, double>> &a_mapRegionalModeMainTrip)
{
	size_t nxMainCnt = a_vecMainPathInfo.size();
	for (size_t i= 0; i< nxMainCnt; i++)
	{
		TMainPathInfo &oTMainPath = a_vecMainPathInfo[i];
		int nMode    = oTMainPath.nMode;

		if (REGION_MODE_CODE_AUTO == nMode) // Auto 일때 메인은 ozone 과 dzone
		{
			KODKey oODKey(oTMainPath.nxMasterOZoneID, oTMainPath.nxMasterDZoneID);

			a_setOriginZoneID.insert(oTMainPath.nxMasterOZoneID);
			a_setDestinationZoneID.insert(oTMainPath.nxMasterDZoneID);

			RegionalMainTrip(oODKey, oTMainPath.dTrip, nMode, a_mapRegionalModeMainTrip);

			continue;
		}

		Integer nxOrgTermID   = oTMainPath.nxOrgTermID;
		Integer nxDesTermID   = oTMainPath.nxDesTermID;
		Integer nxTransTermID = oTMainPath.nxTransTermID;

		a_setTerminalID.insert(nxOrgTermID);
		a_setTerminalID.insert(nxDesTermID);

		if (nxTransTermID > 0) // 환승 터미널 존재
		{
			a_setTransTerminalID.insert(nxTransTermID);

			KODKey oODKeyA(nxOrgTermID, nxTransTermID);

			RegionalMainTrip(oODKeyA, oTMainPath.dTrip, nMode, a_mapRegionalModeMainTrip);

			KODKey oODKeyB(nxTransTermID, nxDesTermID);

			RegionalMainTrip(oODKeyB, oTMainPath.dTrip, nMode, a_mapRegionalModeMainTrip);
		}
		else
		{
			KODKey oODKey(nxOrgTermID, nxDesTermID);

			RegionalMainTrip(oODKey, oTMainPath.dTrip, nMode, a_mapRegionalModeMainTrip);
		}
	}
}

void KInterModalPathInfo::ResultTransitPathDrawInfo3(KTarget* a_pTarget, std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue,  
													std::set<DWORD_PTR> &a_setInterAssignInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo,
													TInResultSettingInfo &a_oSettingInfo, OUT TInMapResultValue& a_oMapResult)
{
	try
	{
		COLORREF clrBus         = RGB (   0, 128,   0 );
		COLORREF clrRail        = RGB (   0,   0, 255 );
		COLORREF clrWalk        = RGB ( 102, 102, 153 );
		COLORREF clrAutoTaxi    = RGB ( 255,   0,   0 );

		std::map<int, TInterPathColor> &mapUrbanColor    = a_oSettingInfo.mapUrbanColor;
		std::map<int, TInterPathColor> &mapRegionalColor = a_oSettingInfo.mapRegionalColor;

#pragma region 색상 설정
		AutoType iterColor  = mapUrbanColor.begin();
		AutoType itEndColor = mapUrbanColor.end();

		while(iterColor != itEndColor)
		{
			int nMode = iterColor->first;
			TInterPathColor oTInterPathColor = iterColor->second;

			if (URBAN_MODE_AUTO == nMode)
			{
				clrAutoTaxi = oTInterPathColor.clrPath;
			} 
			else if (URBAN_MODE_BUS == nMode)
			{
				clrBus = oTInterPathColor.clrPath;
			}
			else if (URBAN_MODE_SUBWAY == nMode)
			{
				clrRail = oTInterPathColor.clrPath;
			}
			else if (URBAN_MODE_WALK == nMode)
			{
				clrWalk = oTInterPathColor.clrPath;
			}
			++iterColor;
		}
#pragma endregion 색상 설정

#pragma region 저장소
		std::map<KODKey, double>	mapWalkingTrip;
		std::map<KODKey, double>    mapAutoTrip;
		std::map<KODKey, double>	mapTransferWalkingB2BTrip;
		std::map<KODKey, double>	mapTransferWalkingB2STrip;
		std::map<KODKey, double>	mapTransferWalkingS2STrip;
		std::map<KODKey, double>	mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
		std::map<KODKey, double>	mapTerminalConnetWalkingTrip;
		std::map<int/*regionalmode*/, std::map<KODKey, double>> mapRegionalModeMainTrip;

		std::map<Integer, double>	mapBusLinkTrip;
		std::map<Integer, double>	mapSubwayLinkTrip;

		std::set<Integer>			setOriginZoneID;
		std::set<Integer>			setDestinationZoneID;
		std::set<Integer>			setStaionID;
		std::set<Integer>			setBusSationID;
		std::set<Integer>			setSubwayStaionID;
		std::set<Integer>			setTerminalID;
		std::set<Integer>			setTransTerminalID;
		std::set<Integer>           setAccessNodeStation;

		std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
		std::vector<TDrawNodeInfo>  vecNodeInfo;
		std::vector<TDrawLinkInfo>  vecLinkInfo;
		std::vector<TUserLineInfo>	vecUserLineInfo;
#pragma endregion 저장소

		// regional main 라인 정보 담기 (mapRegionalModeMainTrip)
#pragma region insert mapRegionalModeMainTrip

		CreateRegionalMainTrip(a_vecMainPathInfo, OUT setOriginZoneID, setDestinationZoneID, setTerminalID, setTransTerminalID, mapRegionalModeMainTrip);

#pragma endregion insert mapRegionalModeMainTrip

		auto iterKey  = a_setInterAssignInfo.begin();
		auto itKeyEnd = a_setInterAssignInfo.end();
		for (; iterKey != itKeyEnd; ++iterKey)
		{
			DWORD_PTR dwPtr = *iterKey;

			auto itFind = a_mapDestResultInputValue.find(dwPtr);
			if (itFind == a_mapDestResultInputValue.end()) {
				continue;
			}

			TInMapResultInputValue &oInMapResultInputValue = itFind->second;	

			mapWalkingTrip.insert(oInMapResultInputValue.mapWalkingTrip.begin(), oInMapResultInputValue.mapWalkingTrip.end());
	
			// 2018.08.01 명지대 요청사항 수정
			updateODTripMap(oInMapResultInputValue.mapAutoTrip, mapAutoTrip);
			updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2BTrip, mapTransferWalkingB2BTrip);
			updateODTripMap(oInMapResultInputValue.mapTransferWalkingB2STrip, mapTransferWalkingB2STrip);
			updateODTripMap(oInMapResultInputValue.mapTransferWalkingS2STrip, mapTransferWalkingS2STrip);
			updateODTripMap(oInMapResultInputValue.mapTransferWalkingA2TTrip, mapTransferWalkingA2TTrip);
			updateODTripMap(oInMapResultInputValue.mapTerminalConnetWalkingTrip, mapTerminalConnetWalkingTrip);

			updateLinkTripMap(oInMapResultInputValue.mapBusLinkTrip, mapBusLinkTrip);
			updateLinkTripMap(oInMapResultInputValue.mapSubwayLinkTrip, mapSubwayLinkTrip);

			//mapAutoTrip.insert(oInMapResultInputValue.mapAutoTrip.begin(), oInMapResultInputValue.mapAutoTrip.end());
			//mapTransferWalkingB2BTrip.insert(oInMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oInMapResultInputValue.mapTransferWalkingB2BTrip.end());
			//mapTransferWalkingB2STrip.insert(oInMapResultInputValue.mapTransferWalkingB2STrip.begin(), oInMapResultInputValue.mapTransferWalkingB2STrip.end());
			//mapTransferWalkingS2STrip.insert(oInMapResultInputValue.mapTransferWalkingS2STrip.begin(), oInMapResultInputValue.mapTransferWalkingS2STrip.end());
			//mapTransferWalkingA2TTrip.insert(oInMapResultInputValue.mapTransferWalkingA2TTrip.begin(), oInMapResultInputValue.mapTransferWalkingA2TTrip.end());
			//mapTerminalConnetWalkingTrip.insert(oInMapResultInputValue.mapTerminalConnetWalkingTrip.begin(), oInMapResultInputValue.mapTerminalConnetWalkingTrip.end());

			//mapBusLinkTrip.insert(oInMapResultInputValue.mapBusLinkTrip.begin(), oInMapResultInputValue.mapBusLinkTrip.end());
			//mapSubwayLinkTrip.insert(oInMapResultInputValue.mapSubwayLinkTrip.begin(), oInMapResultInputValue.mapSubwayLinkTrip.end());
			// 2018.08.01 명지대 요청사항 수정 끝

			setOriginZoneID.insert(oInMapResultInputValue.setOriginZoneID.begin(), oInMapResultInputValue.setOriginZoneID.end());
			setDestinationZoneID.insert(oInMapResultInputValue.setDestinationZoneID.begin(), oInMapResultInputValue.setDestinationZoneID.end());
			setStaionID.insert(oInMapResultInputValue.setStaionID.begin(), oInMapResultInputValue.setStaionID.end());
			setBusSationID.insert(oInMapResultInputValue.setBusSationID.begin(), oInMapResultInputValue.setBusSationID.end());
			setSubwayStaionID.insert(oInMapResultInputValue.setSubwayStaionID.begin(), oInMapResultInputValue.setSubwayStaionID.end());
			setTerminalID.insert(oInMapResultInputValue.setTerminalID.begin(), oInMapResultInputValue.setTerminalID.end());
			setTransTerminalID.insert(oInMapResultInputValue.setTransTerminalID.begin(), oInMapResultInputValue.setTransTerminalID.end());
			setAccessNodeStation.insert(oInMapResultInputValue.setAccessNodeStation.begin(), oInMapResultInputValue.setAccessNodeStation.end());
		}

		//Auto
		AutoType iterAuto = mapAutoTrip.begin();
		AutoType endAuto  = mapAutoTrip.end();
#pragma region mapAutoTrip - vecUserLineInfo
		while(iterAuto != endAuto)
		{
			KODKey oODKey = iterAuto->first;
			double dTrip  = iterAuto->second;

			TUserLineInfo oAutoLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterAuto;
					continue;
				}
				oAutoLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oAutoLine.TWidth    = a_oSettingInfo.dMinSize; 
				oAutoLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterAuto;
					continue;
				}
				oAutoLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oAutoLine.TWidth    = a_oSettingInfo.dMinSize;
				oAutoLine.TLabel    = _T("");
			}
			else
			{
				oAutoLine.TColorRGB = clrAutoTaxi;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oAutoLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oAutoLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oAutoLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oAutoLine);
			++iterAuto;
		}
#pragma endregion mapAutoTrip - vecUserLineInfo

		//Bus
		AutoType iterBus = mapBusLinkTrip.begin();
		AutoType endBus  = mapBusLinkTrip.end();
#pragma region mapBusLinkTrip - vecLinkInfo
		while(iterBus != endBus)
		{
			TDrawLinkInfo oLinkInfo;
			oLinkInfo.TLinkID = iterBus->first;
			double dTrip      = iterBus->second;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterBus;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterBus;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else
			{
				oLinkInfo.TColorRGB = clrBus;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip );
				oLinkInfo.TLabel = strTrip;
			}

			vecLinkInfo.push_back(oLinkInfo);
			++iterBus;
		}
#pragma endregion mapBusLinkTrip - vecLinkInfo

		//Rail
		AutoType iterRail = mapSubwayLinkTrip.begin();
		AutoType endRail  = mapSubwayLinkTrip.end();
#pragma region mapSubwayLinkTrip - vecLinkInfo
		while(iterRail != endRail)
		{
			TDrawLinkInfo oLinkInfo;
			oLinkInfo.TLinkID    = iterRail->first;
			double dTrip         = iterRail->second;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterRail;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterRail;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else
			{
				oLinkInfo.TColorRGB = clrRail;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), iterRail->second);
				oLinkInfo.TLabel = strTrip;
			}

			vecLinkInfo.push_back(oLinkInfo);
			++iterRail;
		}
#pragma endregion mapSubwayLinkTrip - vecLinkInfo

		//O존
		AutoType iterOZone = setOriginZoneID.begin();
		AutoType endOZone  = setOriginZoneID.end();
		while(iterOZone != endOZone)
		{
			SetOriginZoneNodeInfo(*iterOZone, vecZoneNodeInfo, a_oSettingInfo.clrOrgZone);

			++iterOZone;
		}

		//D존
		AutoType iterDZone = setDestinationZoneID.begin();
		AutoType endDZone  = setDestinationZoneID.end();
		while(iterDZone != endDZone)
		{
			SetDestinationZoneNodeInfo(*iterDZone, vecZoneNodeInfo, a_oSettingInfo.clrDestZone);

			++iterDZone;
		}

		// 존 접근 가능 정류장
		AutoType iterAccessNodeStation = setAccessNodeStation.begin();
		AutoType endAccessNodeStation  = setAccessNodeStation.end();
#pragma region setAccessNodeStation - vecNodeInfo
		while(iterAccessNodeStation != endAccessNodeStation)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterAccessNodeStation;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterAccessNodeStation);
			oNodeInfo.TLabel = strNodeLable;

			//oNodeInfo.TColorRGB = RGB(196, 189, 151);
			oNodeInfo.TColorRGB = RGB(225, 123, 182);
			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 48;
			vecNodeInfo.push_back(oNodeInfo);

			++iterAccessNodeStation;
		}
#pragma endregion setAccessNodeStation - vecNodeInfo

		//정류장 
		AutoType iterStation = setStaionID.begin();
		AutoType endStation  = setStaionID.end();
#pragma region setStation - vecNodeInfo
		while(iterStation != endStation)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterStation;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterStation);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(255,165,0);

			//임시 사이즈
			oNodeInfo.TSize = 11.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterStation;
		}
#pragma endregion setStation - vecNodeInfo

		//터미널 
		AutoType iterTerminal = setTerminalID.begin();
		AutoType endTerminal  = setTerminalID.end();
#pragma region setTerminalID - vecNodeInfo
		while(iterTerminal != endTerminal)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterTerminal;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterTerminal);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(128,0,192);

			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterTerminal;
		}
#pragma endregion setTerminalID - vecNodeInfo

		// 환승 터미널 
		AutoType iterTransTerminal = setTransTerminalID.begin();
		AutoType endTransTerminal  = setTransTerminalID.end();
#pragma region setTransTerminalID - vecNodeInfo
		while(iterTransTerminal != endTransTerminal)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterTransTerminal;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterTransTerminal);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(255,0,255);

			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterTransTerminal;
		}
#pragma endregion setTransTerminalID - vecNodeInfo

		//버스에서 버스로
		AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
		AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
#pragma region mapTransferWalkingB2BTrip - vecUserLineInfo
		while(iterB2BWalking != endB2BWalking)
		{
			KODKey oODKey = iterB2BWalking->first;
			double dTrip  = iterB2BWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterB2BWalking;
		}
#pragma endregion mapTransferWalkingB2BTrip - vecUserLineInfo

		// Auto 에서 Transit
		AutoType iterA2TWalking = mapTransferWalkingA2TTrip.begin();
		AutoType endA2TWalking  = mapTransferWalkingA2TTrip.end();
#pragma region mapTransferWalkingA2TTrip - vecUserLineInfo
		while(iterA2TWalking != endA2TWalking)
		{
			KODKey oODKey = iterA2TWalking->first;
			double dTrip  = iterA2TWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterA2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterA2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterA2TWalking;
		}
#pragma endregion mapTransferWalkingA2TTrip - vecUserLineInfo

		// 버스에서 전철로 환승 도보
		AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
		AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
#pragma region mapTransferWalkingB2STrip - vecUserLineInfo
		while(iterB2SWalking != endB2SWalking)
		{
			KODKey oODKey = iterB2SWalking->first;
			double dTrip  = iterB2SWalking->second;

			TUserLineInfo oWalkingLine;
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);


			++iterB2SWalking;
		}
#pragma endregion mapTransferWalkingB2STrip - vecUserLineInfo

		//전철에서 전철로
		AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
		AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
#pragma region mapTransferWalkingS2STrip - vecUserLineInfo
		while(iterS2SWalking != endS2SWalking)
		{
			KODKey oODKey = iterS2SWalking->first;
			double dTrip  = iterS2SWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterS2SWalking;
		}
#pragma endregion mapTransferWalkingS2STrip - vecUserLineInfo

		// access or egress 에서 터미널
		AutoType iterS2TWalking = mapTerminalConnetWalkingTrip.begin();
		AutoType endS2TWalking  = mapTerminalConnetWalkingTrip.end();
#pragma region mapTerminalConnetWalkingTrip - vecUserLineInfo
		while(iterS2TWalking != endS2TWalking)
		{
			KODKey oODKey = iterS2TWalking->first;
			double dTrip  = iterS2TWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterS2TWalking;
		}
#pragma endregion mapTerminalConnetWalkingTrip - vecUserLineInfo

		// Regional Mode의 Main Trip
		AutoType iterRegionalModeMainTrip = mapRegionalModeMainTrip.begin();
		AutoType endRegionalModeMainTrip  = mapRegionalModeMainTrip.end();
#pragma region mapRegionalModeMainTrip - vecUserLineInfo
		while (iterRegionalModeMainTrip != endRegionalModeMainTrip)
		{
			int nRegionalMode                     = iterRegionalModeMainTrip->first;
			COLORREF clrMain = RGB(255, 255, 255);
			if (REGION_MODE_CODE_AUTO == nRegionalMode)
			{
				clrMain = clrAutoTaxi;
			}
			else
			{
				AutoType itFindColor = mapRegionalColor.find(nRegionalMode);
				if (itFindColor != mapRegionalColor.end())
				{
					const TInterPathColor &oTInterPathColor = itFindColor->second;
					clrMain = oTInterPathColor.clrPath;
				}
			}

			std::map<KODKey, double> &mapMainTrip = iterRegionalModeMainTrip->second;

			// regional main
			AutoType iterMain = mapMainTrip.begin();
			AutoType endMain  = mapMainTrip.end();

			while(iterMain != endMain)
			{
				KODKey oODKey = iterMain->first;
				double dTrip  = iterMain->second;

				TUserLineInfo oRegionalMainLine;

				if (dTrip > a_oSettingInfo.dMaxTrip)
				{
					if (false == a_oSettingInfo.bUseOutRange)
					{
						++iterMain;
						continue;
					}
					oRegionalMainLine.TColorRGB = a_oSettingInfo.clrOutRange;
					oRegionalMainLine.TWidth    = a_oSettingInfo.dMinSize; 
					oRegionalMainLine.TLabel    = _T("");
				}
				else if (dTrip < a_oSettingInfo.dMinTrip)
				{
					if (false == a_oSettingInfo.bUseOutRange)
					{
						++iterMain;
						continue;
					}
					oRegionalMainLine.TColorRGB = a_oSettingInfo.clrOutRange;
					oRegionalMainLine.TWidth    = a_oSettingInfo.dMinSize;
					oRegionalMainLine.TLabel    = _T("");
				}
				else
				{
					oRegionalMainLine.TColorRGB = clrMain;
					double dWidth(0.0);
					CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
					oRegionalMainLine.TWidth = dWidth;

					CString strTrip(_T(""));
					strTrip.Format(_T("%.2f"), dTrip);
					oRegionalMainLine.TLabel = strTrip;
				}

				std::vector<TMapPoint> vecPoint;
				Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
				oRegionalMainLine.TPoints = vecPoint;
				vecUserLineInfo.push_back(oRegionalMainLine);

				++iterMain;
			}

			++iterRegionalModeMainTrip;
		}
#pragma endregion mapRegionalModeMainTrip - vecUserLineInfo

		a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
		a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
		a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
		a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KInterModalPathInfo::ResultTransitTripInfo2(KTarget* pTarget, std::set<DWORD_PTR> &a_setInterAssignInfo, std::vector<TMainPathInfo> &a_vecMainPathInfo, 
												OUT std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue, std::map<int, std::map<KODKey, double>> &a_mapRegionalModeMainTrip)
{
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

#pragma region 라인정보담기
	auto iterMain  = a_setInterAssignInfo.begin();
	auto itEndMain = a_setInterAssignInfo.end();
	for (; iterMain != itEndMain; ++iterMain)
	{
		DWORD_PTR dwPtr = *iterMain;
		KInterPathEditInfo* pBusAssignInfo = (KInterPathEditInfo*)dwPtr;
		KInterPathEditInfo &oBusAssignInfo = *pBusAssignInfo;

		TInMapResultInputValue oInMapResultInputValue;
#pragma region 저장소
		std::map<KODKey, double>	&mapWalkingTrip                 = oInMapResultInputValue.mapWalkingTrip;
		std::map<KODKey, double>    &mapAutoTrip					= oInMapResultInputValue.mapAutoTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2BTrip		= oInMapResultInputValue.mapTransferWalkingB2BTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2STrip		= oInMapResultInputValue.mapTransferWalkingB2STrip;
		std::map<KODKey, double>	&mapTransferWalkingS2STrip		= oInMapResultInputValue.mapTransferWalkingS2STrip;
		std::map<KODKey, double>	&mapTransferWalkingA2TTrip		= oInMapResultInputValue.mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
		std::map<KODKey, double>	&mapTerminalConnetWalkingTrip	= oInMapResultInputValue.mapTerminalConnetWalkingTrip;

		std::map<Integer, double>	&mapBusLinkTrip					= oInMapResultInputValue.mapBusLinkTrip;
		std::map<Integer, double>	&mapSubwayLinkTrip				= oInMapResultInputValue.mapSubwayLinkTrip;
		std::set<Integer>			&setOriginZoneID				= oInMapResultInputValue.setOriginZoneID;
		std::set<Integer>			&setDestinationZoneID			= oInMapResultInputValue.setDestinationZoneID;
		std::set<Integer>			&setStaionID					= oInMapResultInputValue.setStaionID;
		std::set<Integer>			&setBusSationID					= oInMapResultInputValue.setBusSationID;
		std::set<Integer>			&setSubwayStaionID				= oInMapResultInputValue.setSubwayStaionID;
		std::set<Integer>			&setTerminalID					= oInMapResultInputValue.setTerminalID; 
		std::set<Integer>			&setTransTerminalID				= oInMapResultInputValue.setTransTerminalID;
		std::set<Integer>           &setAccessNodeStation			= oInMapResultInputValue.setAccessNodeStation;
#pragma endregion 저장소

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMInPathType	emLastPathType = KEMInPathTypeNull;

		Integer			nxOZone        = oBusAssignInfo.nxOrginID;
		Integer			nxDZone        = oBusAssignInfo.nxDestinationID;

		Integer			nxAutoStart    = oBusAssignInfo.nxAutoStartNode;
		Integer			nxAutoEnd      = oBusAssignInfo.nxAutoEndNode;

		if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Acccess 일때는  DZone이 의미 하는 바는 OrginTerminal
				setOriginZoneID.insert(nxOZone);
				setTerminalID.insert(nxDZone);
			}
			else if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Egress 일때는   OZone이 의미 하는 바는 DestinaionTerminal
				setDestinationZoneID.insert(nxDZone);
				setTerminalID.insert(nxOZone);
			}
		}
		else
		{
			setOriginZoneID.insert(nxOZone);
			setDestinationZoneID.insert(nxDZone);
		}

		if (KEMInterModeAutoTaxi == oBusAssignInfo.emInterMode) 
		{
			KODKey oAutoTaxi(nxOZone, nxDZone);

			double dTrip(0.0);
			if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
			{
				dTrip = oBusAssignInfo.oTRegAutoSubData.dTrip;
			}
			else
			{
				dTrip = oBusAssignInfo.oTAutoSubData.dTrip;
			}

#pragma region mapAutoTrip - 얻기
			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();

			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, dTrip));
			}
			else
			{
				double &dSumValue = iterAutoReg->second;
				dSumValue += dTrip;
			}
#pragma endregion mapAutoTrip - 얻기

			a_mapDestResultInputValue.insert(std::make_pair(dwPtr, oInMapResultInputValue));

			continue;
		}

		if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
			KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
			KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
		{
			KODKey oAutoTaxi(nxAutoStart, nxAutoEnd);
			double dTrip = oBusAssignInfo.oTMultiSubData.dTrip;

#pragma region mapAutoTrip - 얻기
			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();

			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, dTrip));
			}
			else
			{
				double &dSumValue = iterAutoReg->second;
				dSumValue += dTrip;
			}
#pragma endregion mapAutoTrip - 얻기
		}

		std::vector<TInIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResulTransitInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{
			std::vector<TInResultTransitInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TInResultTransitInfo &oTMResultBusInfo = vecTResult[nIndex]; 
				//oTMResultBusInfo.emPathType
				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
						KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
						KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
					{
						KODKey oTransWalk(nxAutoEnd, nxStartID); // 차에서 내려서 부터 첫번째 정류장까지의 Walk
						double dTrip = oBusAssignInfo.oTMultiSubData.dTrip;

#pragma region mapTransferWalkingA2TTrip - 얻기
						AutoType iterFind  = mapTransferWalkingA2TTrip.find(oTransWalk);
						AutoType iterEnd   = mapTransferWalkingA2TTrip.end();
						if (iterFind == iterEnd)
						{
							mapTransferWalkingA2TTrip.insert(std::make_pair(oTransWalk, dTrip));
						}
						else
						{
							double &dSumValue = iterFind->second;
							dSumValue += dTrip;
						}
#pragma endregion mapTransferWalkingA2TTrip - 얻기
					}
					else
					{
						//OZone 부터 첫번째 정류장까지의 Walk
						Integer nxStationID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);

						if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
						{
							setAccessNodeStation.insert(nxStationID);
// 							mapWalkingTrip은 명지대 요청으로 Draw 에서 제외되어, 데이터 수집 제외
// 							해당 주석 부분은 Urban의 Bus, Rail, Bus+Rail 첫번째 정류장까지의 Walk;
// 							KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);
// 							double dAssignTrip(0.0);
// 						
// 							if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 							else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
// 							else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 
// 							AutoType iterfind = mapWalkingTrip.find(oFirstWalk); 
// 							AutoType iterend  = mapWalkingTrip.end();
// 							if (iterfind == iterend)
// 							{
// 								mapWalkingTrip.insert(std::make_pair(oFirstWalk, dAssignTrip));
// 							}
// 							else
// 							{
// 								double &dSumValue = iterfind->second;
// 								dSumValue += dAssignTrip;
// 							}
						}
						else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
						{
							if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
							{
								Integer nxDestionationTerminal = nxOZone;
								KODKey oLastWalk(nxDestionationTerminal, nxStationID);
								double dAssignTrip(0.0);

								if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
								{
									dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
								}
								else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
								{
									dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
								}

								AutoType iterfind = mapTerminalConnetWalkingTrip.find(oLastWalk);
								AutoType iterend  = mapTerminalConnetWalkingTrip.end();
								if (iterfind == iterend)
								{
									mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
								}
								else
								{
									double &dSumValue = iterfind->second;
									dSumValue += dAssignTrip;
								}
							}
						}
					}
				}
				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus && emLastPathType == KEMInPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					double dAssignTrip(0.0);

					if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
					{
						if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
					}
					else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
					{
						if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						}
					}

					AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2BTrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + dAssignTrip;
						iterfind->second = dValue;
					}
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMInPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					double dAssignTrip(0.0);

					if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
					{
						if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
					}
					else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
					{
						if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						}
					}

					AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2STrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + dAssignTrip;
						iterfind->second = dValue;
					}
				}
				else if(oTMResultBusInfo.emPathType == KEMInPathTypeRail && emLastPathType == KEMInPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						double dAssignTrip(0.0);

						if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
						{
							if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
							else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
							else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
						{
							if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
							{
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
							}
							else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
							{
								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
							}
						}

						AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingS2STrip.end();
						if (iterfind == iterend)
						{
							mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, dAssignTrip));
						}
						else
						{
							double dValue = iterfind->second;
							dValue = dValue + dAssignTrip;
							iterfind->second = dValue;
						}
					}
				}

				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus) { 
					//★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					//★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
						// GetReverseTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		Integer nxStationID(0);
		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);

		if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				Integer nxOriginTerminal = nxDZone;

				KODKey oLastWalk(nxStationID, nxOriginTerminal);

				double dAssignTrip(0.0);
				if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
				{
					dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
				}
				else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
				{
					dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
				}

				AutoType iterfind = mapTerminalConnetWalkingTrip.find(oLastWalk);
				AutoType iterend  = mapTerminalConnetWalkingTrip.end();
				if (iterfind == iterend)
				{
					mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
				}
				else
				{
					double &dSumValue = iterfind->second;
					dSumValue += dAssignTrip;
				}
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값 // mapWalkingTrip은 그릴때 사용 되지 않을 것임
		//해당 주석 부분은 Urban의 Bus, Rail, Bus+Rail 마지막 정류장 부터 DZone까지의 Walk값
		if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
		{
// 			Integer nxStationID(0);
// 			NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
			setAccessNodeStation.insert(nxStationID);

// 			KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
// 			double dAssignTrip(0.0);
// 						
// 			if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 			else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
// 			else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 
// 			AutoType iterfind = mapWalkingTrip.find(oLastWalk); 
// 			AutoType iterend  = mapWalkingTrip.end();
// 			if (iterfind == iterend)
// 			{
// 				mapWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
// 			}
// 			else
// 			{
// 				double &dSumValue = iterfind->second;
// 				dSumValue += dAssignTrip;
// 			}
		}

		a_mapDestResultInputValue.insert(std::make_pair(dwPtr, oInMapResultInputValue));
	}
#pragma endregion 라인정보담기

	// regional main 라인 정보 담기
	// 여기서의 호출은 오직 a_mapRegionalModeMainTrip만을 수집하기 위한
	std::set<Integer>			setTempOriginZoneID;
	std::set<Integer>			setTempDestinationZoneID;
	std::set<Integer>			setTempTerminalID;
	std::set<Integer>			setTempTransTerminalID;
	CreateRegionalMainTrip(a_vecMainPathInfo, OUT setTempOriginZoneID, setTempDestinationZoneID, setTempTerminalID, setTempTransTerminalID, a_mapRegionalModeMainTrip);
}


void KInterModalPathInfo::ResultTransitPathDrawInfo(KTarget* a_pTarget, TInMapResultInputValue &a_oInMapResultTripInfo, 
													TInResultSettingInfo &a_oSettingInfo, TInMapResultValue& a_oMapResult)
{
	try
	{
		COLORREF clrBus         = RGB (   0, 128,   0 );
		COLORREF clrRail        = RGB (   0,   0, 255 );
		COLORREF clrWalk        = RGB ( 102, 102, 153 );
		COLORREF clrAutoTaxi    = RGB ( 255,   0,   0 );

		std::map<int, TInterPathColor> &mapUrbanColor    = a_oSettingInfo.mapUrbanColor;
		std::map<int, TInterPathColor> &mapRegionalColor = a_oSettingInfo.mapRegionalColor;

#pragma region 색상 설정
		AutoType iterColor  = mapUrbanColor.begin();
		AutoType itEndColor = mapUrbanColor.end();

		while(iterColor != itEndColor)
		{
			int nMode = iterColor->first;
			TInterPathColor oTInterPathColor = iterColor->second;

			if (URBAN_MODE_AUTO == nMode)
			{
				clrAutoTaxi = oTInterPathColor.clrPath;
			} 
			else if (URBAN_MODE_BUS == nMode)
			{
				clrBus = oTInterPathColor.clrPath;
			}
			else if (URBAN_MODE_SUBWAY == nMode)
			{
				clrRail = oTInterPathColor.clrPath;
			}
			else if (URBAN_MODE_WALK == nMode)
			{
				clrWalk = oTInterPathColor.clrPath;
			}
			++iterColor;
		}
#pragma endregion 색상 설정

#pragma region 저장소
		std::map<KODKey, double>	&mapWalkingTrip				= a_oInMapResultTripInfo.mapWalkingTrip;
		std::map<KODKey, double>    &mapAutoTrip					= a_oInMapResultTripInfo.mapAutoTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2BTrip	= a_oInMapResultTripInfo.mapTransferWalkingB2BTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2STrip	= a_oInMapResultTripInfo.mapTransferWalkingB2STrip;
		std::map<KODKey, double>	&mapTransferWalkingS2STrip	= a_oInMapResultTripInfo.mapTransferWalkingS2STrip;
		std::map<KODKey, double>	&mapTransferWalkingA2TTrip	= a_oInMapResultTripInfo.mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
		std::map<KODKey, double>	&mapTerminalConnetWalkingTrip= a_oInMapResultTripInfo.mapTerminalConnetWalkingTrip;
		std::map<int/*regionalmode*/, std::map<KODKey, double>> &mapRegionalModeMainTrip = a_oInMapResultTripInfo.mapRegionalModeMainTrip;

		std::map<Integer, double>	mapBusLinkTrip				= a_oInMapResultTripInfo.mapBusLinkTrip;
		std::map<Integer, double>	mapSubwayLinkTrip			= a_oInMapResultTripInfo.mapSubwayLinkTrip;

		std::set<Integer>			setOriginZoneID				= a_oInMapResultTripInfo.setOriginZoneID;
		std::set<Integer>			setDestinationZoneID		= a_oInMapResultTripInfo.setDestinationZoneID;
		std::set<Integer>			setStaionID					= a_oInMapResultTripInfo.setStaionID;
		std::set<Integer>			setBusSationID				= a_oInMapResultTripInfo.setBusSationID;
		std::set<Integer>			setSubwayStaionID			= a_oInMapResultTripInfo.setSubwayStaionID;
		std::set<Integer>			setTerminalID				= a_oInMapResultTripInfo.setTerminalID;
		std::set<Integer>			setTransTerminalID			= a_oInMapResultTripInfo.setTransTerminalID;
		std::set<Integer>           setAccessNodeStation		= a_oInMapResultTripInfo.setAccessNodeStation;

		std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
		std::vector<TDrawNodeInfo>  vecNodeInfo;
		std::vector<TDrawLinkInfo>  vecLinkInfo;
		std::vector<TUserLineInfo>	vecUserLineInfo;
#pragma endregion 저장소
	
		//Auto
		AutoType iterAuto = mapAutoTrip.begin();
		AutoType endAuto  = mapAutoTrip.end();
#pragma region mapAutoTrip - vecUserLineInfo
		while(iterAuto != endAuto)
		{
			KODKey oODKey = iterAuto->first;
			double dTrip  = iterAuto->second;

			TUserLineInfo oAutoLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterAuto;
					continue;
				}
				oAutoLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oAutoLine.TWidth    = a_oSettingInfo.dMinSize; 
				oAutoLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterAuto;
					continue;
				}
				oAutoLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oAutoLine.TWidth    = a_oSettingInfo.dMinSize;
				oAutoLine.TLabel    = _T("");
			}
			else
			{
				oAutoLine.TColorRGB = clrAutoTaxi;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oAutoLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oAutoLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oAutoLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oAutoLine);
			++iterAuto;
		}
#pragma endregion mapAutoTrip - vecUserLineInfo

		//Bus
		AutoType iterBus = mapBusLinkTrip.begin();
		AutoType endBus  = mapBusLinkTrip.end();
#pragma region mapBusLinkTrip - vecLinkInfo
		while(iterBus != endBus)
		{
			TDrawLinkInfo oLinkInfo;
			oLinkInfo.TLinkID = iterBus->first;
			double dTrip      = iterBus->second;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterBus;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterBus;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else
			{
				oLinkInfo.TColorRGB = clrBus;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip );
				oLinkInfo.TLabel = strTrip;
			}

			vecLinkInfo.push_back(oLinkInfo);
			++iterBus;
		}
#pragma endregion mapBusLinkTrip - vecLinkInfo

		//Rail
		AutoType iterRail = mapSubwayLinkTrip.begin();
		AutoType endRail  = mapSubwayLinkTrip.end();
#pragma region mapSubwayLinkTrip - vecLinkInfo
		while(iterRail != endRail)
		{
			TDrawLinkInfo oLinkInfo;
			oLinkInfo.TLinkID    = iterRail->first;
			double dTrip         = iterRail->second;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterRail;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterRail;
					continue;
				}
				oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
				oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
				oLinkInfo.TLabel    = _T("");
			}
			else
			{
				oLinkInfo.TColorRGB = clrRail;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oLinkInfo.TWidth = dWidth;
				
				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), iterRail->second);
				oLinkInfo.TLabel = strTrip;
			}

			vecLinkInfo.push_back(oLinkInfo);
			++iterRail;
		}
#pragma endregion mapSubwayLinkTrip - vecLinkInfo

		//O존
		AutoType iterOZone = setOriginZoneID.begin();
		AutoType endOZone  = setOriginZoneID.end();
		while(iterOZone != endOZone)
		{
			SetOriginZoneNodeInfo(*iterOZone, vecZoneNodeInfo, a_oSettingInfo.clrOrgZone);

			++iterOZone;
		}

		//D존
		AutoType iterDZone = setDestinationZoneID.begin();
		AutoType endDZone  = setDestinationZoneID.end();
		while(iterDZone != endDZone)
		{
			SetDestinationZoneNodeInfo(*iterDZone, vecZoneNodeInfo, a_oSettingInfo.clrDestZone);

			++iterDZone;
		}

		// 존 접근 가능 정류장
		AutoType iterAccessNodeStation = setAccessNodeStation.begin();
		AutoType endAccessNodeStation  = setAccessNodeStation.end();
#pragma region setAccessNodeStation - vecNodeInfo
		while(iterAccessNodeStation != endAccessNodeStation)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterAccessNodeStation;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterAccessNodeStation);
			oNodeInfo.TLabel = strNodeLable;

			//oNodeInfo.TColorRGB = RGB(196, 189, 151);
			oNodeInfo.TColorRGB = RGB(225, 123, 182);
			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 48;
			vecNodeInfo.push_back(oNodeInfo);

			++iterAccessNodeStation;
		}
#pragma endregion setAccessNodeStation - vecNodeInfo

		//정류장 
		AutoType iterStation = setStaionID.begin();
		AutoType endStation  = setStaionID.end();
#pragma region setStation - vecNodeInfo
		while(iterStation != endStation)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterStation;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterStation);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(255,165,0);

			//임시 사이즈
			oNodeInfo.TSize = 11.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterStation;
		}
#pragma endregion setStation - vecNodeInfo

		//터미널 
		AutoType iterTerminal = setTerminalID.begin();
		AutoType endTerminal  = setTerminalID.end();
#pragma region setTerminalID - vecNodeInfo
		while(iterTerminal != endTerminal)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterTerminal;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterTerminal);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(128,0,192);

			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterTerminal;
		}
#pragma endregion setTerminalID - vecNodeInfo

		// 환승 터미널 
		AutoType iterTransTerminal = setTransTerminalID.begin();
		AutoType endTransTerminal  = setTransTerminalID.end();
#pragma region setTransTerminalID - vecNodeInfo
		while(iterTransTerminal != endTransTerminal)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterTransTerminal;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterTransTerminal);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = RGB(255,0,255);

			//임시 사이즈
			oNodeInfo.TSize = 15.0;

			//임시 모양
			oNodeInfo.TSymbol = 72;

			vecNodeInfo.push_back(oNodeInfo);
			++iterTransTerminal;
		}
#pragma endregion setTransTerminalID - vecNodeInfo

		//버스에서 버스로
		AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
		AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
#pragma region mapTransferWalkingB2BTrip - vecUserLineInfo
		while(iterB2BWalking != endB2BWalking)
		{
			KODKey oODKey = iterB2BWalking->first;
			double dTrip  = iterB2BWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterB2BWalking;
		}
#pragma endregion mapTransferWalkingB2BTrip - vecUserLineInfo

		// Auto 에서 Transit
		AutoType iterA2TWalking = mapTransferWalkingA2TTrip.begin();
		AutoType endA2TWalking  = mapTransferWalkingA2TTrip.end();
#pragma region mapTransferWalkingA2TTrip - vecUserLineInfo
		while(iterA2TWalking != endA2TWalking)
		{
			KODKey oODKey = iterA2TWalking->first;
			double dTrip  = iterA2TWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterA2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterA2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterA2TWalking;
		}
#pragma endregion mapTransferWalkingA2TTrip - vecUserLineInfo

		// 버스에서 전철로 환승 도보
		AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
		AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
#pragma region mapTransferWalkingB2STrip - vecUserLineInfo
		while(iterB2SWalking != endB2SWalking)
		{
			KODKey oODKey = iterB2SWalking->first;
			double dTrip  = iterB2SWalking->second;

			TUserLineInfo oWalkingLine;
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
				
				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);


			++iterB2SWalking;
		}
#pragma endregion mapTransferWalkingB2STrip - vecUserLineInfo

		//전철에서 전철로
		AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
		AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
#pragma region mapTransferWalkingS2STrip - vecUserLineInfo
		while(iterS2SWalking != endS2SWalking)
		{
			KODKey oODKey = iterS2SWalking->first;
			double dTrip  = iterS2SWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterS2SWalking;
		}
#pragma endregion mapTransferWalkingS2STrip - vecUserLineInfo

		// access or egress 에서 터미널
		AutoType iterS2TWalking = mapTerminalConnetWalkingTrip.begin();
		AutoType endS2TWalking  = mapTerminalConnetWalkingTrip.end();
#pragma region mapTerminalConnetWalkingTrip - vecUserLineInfo
		while(iterS2TWalking != endS2TWalking)
		{
			KODKey oODKey = iterS2TWalking->first;
			double dTrip  = iterS2TWalking->second;

			TUserLineInfo oWalkingLine;

			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2TWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
				oWalkingLine.TLabel    = _T("");
			}
			else
			{
				oWalkingLine.TColorRGB = clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;

				CString strTrip(_T(""));
				strTrip.Format(_T("%.2f"), dTrip);
				oWalkingLine.TLabel = strTrip;
			}

			std::vector<TMapPoint> vecPoint;
			Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
			oWalkingLine.TPoints = vecPoint;
			vecUserLineInfo.push_back(oWalkingLine);

			++iterS2TWalking;
		}
#pragma endregion mapTerminalConnetWalkingTrip - vecUserLineInfo

		// Regional Mode의 Main Trip
		AutoType iterRegionalModeMainTrip = mapRegionalModeMainTrip.begin();
		AutoType endRegionalModeMainTrip  = mapRegionalModeMainTrip.end();
#pragma region mapRegionalModeMainTrip - vecUserLineInfo
		while (iterRegionalModeMainTrip != endRegionalModeMainTrip)
		{
			int nRegionalMode                     = iterRegionalModeMainTrip->first;
			COLORREF clrMain = RGB(255, 255, 255);
			if (REGION_MODE_CODE_AUTO == nRegionalMode)
			{
				clrMain = clrAutoTaxi;
			}
			else
			{
				AutoType itFindColor = mapRegionalColor.find(nRegionalMode);
				if (itFindColor != mapRegionalColor.end())
				{
					const TInterPathColor &oTInterPathColor = itFindColor->second;
					clrMain = oTInterPathColor.clrPath;
				}
			}

			std::map<KODKey, double> &mapMainTrip = iterRegionalModeMainTrip->second;

			// regional main
			AutoType iterMain = mapMainTrip.begin();
			AutoType endMain  = mapMainTrip.end();

			while(iterMain != endMain)
			{
				KODKey oODKey = iterMain->first;
				double dTrip  = iterMain->second;

				TUserLineInfo oRegionalMainLine;

				if (dTrip > a_oSettingInfo.dMaxTrip)
				{
					if (false == a_oSettingInfo.bUseOutRange)
					{
						++iterMain;
						continue;
					}
					oRegionalMainLine.TColorRGB = a_oSettingInfo.clrOutRange;
					oRegionalMainLine.TWidth    = a_oSettingInfo.dMinSize; 
					oRegionalMainLine.TLabel    = _T("");
				}
				else if (dTrip < a_oSettingInfo.dMinTrip)
				{
					if (false == a_oSettingInfo.bUseOutRange)
					{
						++iterMain;
						continue;
					}
					oRegionalMainLine.TColorRGB = a_oSettingInfo.clrOutRange;
					oRegionalMainLine.TWidth    = a_oSettingInfo.dMinSize;
					oRegionalMainLine.TLabel    = _T("");
				}
				else
				{
					oRegionalMainLine.TColorRGB = clrMain;
					double dWidth(0.0);
					CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
					oRegionalMainLine.TWidth = dWidth;

					CString strTrip(_T(""));
					strTrip.Format(_T("%.2f"), dTrip);
					oRegionalMainLine.TLabel = strTrip;
				}

				std::vector<TMapPoint> vecPoint;
				Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
				oRegionalMainLine.TPoints = vecPoint;
				vecUserLineInfo.push_back(oRegionalMainLine);

				++iterMain;
			}

			++iterRegionalModeMainTrip;
		}
#pragma endregion mapRegionalModeMainTrip - vecUserLineInfo

		a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
		a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
		a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
		a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalPathInfo::ResultTransitTripInfo(KTarget* pTarget, std::vector<KInterPathEditInfo> &a_vecTransitEditInfo, 
												std::vector<TMainPathInfo> &a_vecMainPathInfo, TInMapResultInputValue &a_oInMapResultInputValue)
{
#pragma region 저장소
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	std::map<KODKey, double>	&mapWalkingTrip                 = a_oInMapResultInputValue.mapWalkingTrip;
	std::map<KODKey, double>    &mapAutoTrip					= a_oInMapResultInputValue.mapAutoTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2BTrip		= a_oInMapResultInputValue.mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2STrip		= a_oInMapResultInputValue.mapTransferWalkingB2STrip;
	std::map<KODKey, double>	&mapTransferWalkingS2STrip		= a_oInMapResultInputValue.mapTransferWalkingS2STrip;
	std::map<KODKey, double>	&mapTransferWalkingA2TTrip		= a_oInMapResultInputValue.mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
	std::map<KODKey, double>	&mapTerminalConnetWalkingTrip	= a_oInMapResultInputValue.mapTerminalConnetWalkingTrip;
	std::map<int, std::map<KODKey, double>> &mapRegionalModeMainTrip = a_oInMapResultInputValue.mapRegionalModeMainTrip;

	std::map<Integer, double>	&mapBusLinkTrip					= a_oInMapResultInputValue.mapBusLinkTrip;
	std::map<Integer, double>	&mapSubwayLinkTrip				= a_oInMapResultInputValue.mapSubwayLinkTrip;
	std::set<Integer>			&setOriginZoneID				= a_oInMapResultInputValue.setOriginZoneID;
	std::set<Integer>			&setDestinationZoneID			= a_oInMapResultInputValue.setDestinationZoneID;
	std::set<Integer>			&setStaionID					= a_oInMapResultInputValue.setStaionID;
	std::set<Integer>			&setBusSationID					= a_oInMapResultInputValue.setBusSationID;
	std::set<Integer>			&setSubwayStaionID				= a_oInMapResultInputValue.setSubwayStaionID;
	std::set<Integer>			&setTerminalID					= a_oInMapResultInputValue.setTerminalID; 
	std::set<Integer>			&setTransTerminalID				= a_oInMapResultInputValue.setTransTerminalID;
	std::set<Integer>           &setAccessNodeStation			= a_oInMapResultInputValue.setAccessNodeStation;
#pragma endregion 저장소

#pragma region 라인정보담기
	size_t nAssignCount = a_vecTransitEditInfo.size();
	for (size_t i = 0; i < nAssignCount; i++)
	{
		KInterPathEditInfo &oBusAssignInfo = a_vecTransitEditInfo[i];

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMInPathType	emLastPathType = KEMInPathTypeNull;

		Integer			nxOZone        = oBusAssignInfo.nxOrginID;
		Integer			nxDZone        = oBusAssignInfo.nxDestinationID;

		Integer			nxAutoStart    = oBusAssignInfo.nxAutoStartNode;
		Integer			nxAutoEnd      = oBusAssignInfo.nxAutoEndNode;

		if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Acccess 일때는  DZone이 의미 하는 바는 OrginTerminal
				setOriginZoneID.insert(nxOZone);
				setTerminalID.insert(nxDZone);
			}
			else if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Egress 일때는   OZone이 의미 하는 바는 DestinaionTerminal
				setDestinationZoneID.insert(nxDZone);
				setTerminalID.insert(nxOZone);
			}
		}
		else
		{
			setOriginZoneID.insert(nxOZone);
			setDestinationZoneID.insert(nxDZone);
		}

		if (KEMInterModeAutoTaxi == oBusAssignInfo.emInterMode) 
		{
			KODKey oAutoTaxi(nxOZone, nxDZone);

			double dTrip(0.0);
			if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
			{
				dTrip = oBusAssignInfo.oTRegAutoSubData.dTrip;
			}
			else
			{
				dTrip = oBusAssignInfo.oTAutoSubData.dTrip;
			}

#pragma region mapAutoTrip - 얻기
			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();

			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, dTrip));
			}
			else
			{
				double &dSumValue = iterAutoReg->second;
				dSumValue += dTrip;
			}
#pragma endregion mapAutoTrip - 얻기

			continue;
		}

		if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
			KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
			KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
		{
			KODKey oAutoTaxi(nxAutoStart, nxAutoEnd);
			double dTrip = oBusAssignInfo.oTMultiSubData.dTrip;

#pragma region mapAutoTrip - 얻기
			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();

			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, dTrip));
			}
			else
			{
				double &dSumValue = iterAutoReg->second;
				dSumValue += dTrip;
			}
#pragma endregion mapAutoTrip - 얻기
		}

		std::vector<TInIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResulTransitInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{
			std::vector<TInResultTransitInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TInResultTransitInfo &oTMResultBusInfo = vecTResult[nIndex]; 
				//oTMResultBusInfo.emPathType
				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
						KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
						KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
					{
						KODKey oTransWalk(nxAutoEnd, nxStartID); // 차에서 내려서 부터 첫번째 정류장까지의 Walk
						double dTrip = oBusAssignInfo.oTMultiSubData.dTrip;

#pragma region mapTransferWalkingA2TTrip - 얻기
						AutoType iterFind  = mapTransferWalkingA2TTrip.find(oTransWalk);
						AutoType iterEnd   = mapTransferWalkingA2TTrip.end();
						if (iterFind == iterEnd)
						{
							mapTransferWalkingA2TTrip.insert(std::make_pair(oTransWalk, dTrip));
						}
						else
						{
							double &dSumValue = iterFind->second;
							dSumValue += dTrip;
						}
#pragma endregion mapTransferWalkingA2TTrip - 얻기
					}
					else
					{
						//OZone 부터 첫번째 정류장까지의 Walk
						Integer nxStationID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);

						if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
						{
							setAccessNodeStation.insert(nxStationID);
// 							mapWalkingTrip은 명지대 요청으로 Draw 에서 제외되어, 데이터 수집 제외
// 							해당 주석 부분은 Urban의 Bus, Rail, Bus+Rail 첫번째 정류장까지의 Walk;
// 							KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);
// 							double dAssignTrip(0.0);
// 						
// 							if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 							else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
// 							else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
// 								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 
// 							AutoType iterfind = mapWalkingTrip.find(oFirstWalk); 
// 							AutoType iterend  = mapWalkingTrip.end();
// 							if (iterfind == iterend)
// 							{
// 								mapWalkingTrip.insert(std::make_pair(oFirstWalk, dAssignTrip));
// 							}
// 							else
// 							{
// 								double &dSumValue = iterfind->second;
// 								dSumValue += dAssignTrip;
// 							}
						}
						else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
						{
							if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
							{
								Integer nxDestionationTerminal = nxOZone;
								KODKey oLastWalk(nxDestionationTerminal, nxStationID);
								double dAssignTrip(0.0);

								if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
								{
									dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
								}
								else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
								{
									dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
								}

								AutoType iterfind = mapTerminalConnetWalkingTrip.find(oLastWalk);
								AutoType iterend  = mapTerminalConnetWalkingTrip.end();
								if (iterfind == iterend)
								{
									mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
								}
								else
								{
									double &dSumValue = iterfind->second;
									dSumValue += dAssignTrip;
								}
							}
						}
					}
				}
				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus && emLastPathType == KEMInPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					double dAssignTrip(0.0);

					if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
					{
						if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
					}
					else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
					{
						if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						}
					}

					AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2BTrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + dAssignTrip;
						iterfind->second = dValue;
					}
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMInPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					double dAssignTrip(0.0);

					if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
					{
						if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
					}
					else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
					{
						if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
						{
							dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
						}
					}

					AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2STrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + dAssignTrip;
						iterfind->second = dValue;
					}
				}
				else if(oTMResultBusInfo.emPathType == KEMInPathTypeRail && emLastPathType == KEMInPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						double dAssignTrip(0.0);

						if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
						{
							if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
							else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
							else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
						}
						else if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
						{
							if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
							{
								dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
							}
							else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
							{
								dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
							}
						}

						AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingS2STrip.end();
						if (iterfind == iterend)
						{
							mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, dAssignTrip));
						}
						else
						{
							double dValue = iterfind->second;
							dValue = dValue + dAssignTrip;
							iterfind->second = dValue;
						}
					}
				}

				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus) { 
                    //★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
                    //★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
                        // GetReverseTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		Integer nxStationID(0);
		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);

		if (KEMInterModalPathModeRegional == oBusAssignInfo.emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				Integer nxOriginTerminal = nxDZone;

				KODKey oLastWalk(nxStationID, nxOriginTerminal);

				double dAssignTrip(0.0);
				if (KEMInterRegDetailModeBus == oBusAssignInfo.emInterRegDetailMode || KEMInterRegDetailModeBusRail == oBusAssignInfo.emInterRegDetailMode)
				{
					dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
				}
				else if (KEMInterRegDetailModeRail == oBusAssignInfo.emInterRegDetailMode)
				{
					dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
				}

				AutoType iterfind = mapTerminalConnetWalkingTrip.find(oLastWalk);
				AutoType iterend  = mapTerminalConnetWalkingTrip.end();
				if (iterfind == iterend)
				{
					mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
				}
				else
				{
					double &dSumValue = iterfind->second;
					dSumValue += dAssignTrip;
				}
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값 // mapWalkingTrip은 그릴때 사용 되지 않을 것임
		//해당 주석 부분은 Urban의 Bus, Rail, Bus+Rail 마지막 정류장 부터 DZone까지의 Walk값
		if (KEMInterModalPathModeUrban == oBusAssignInfo.emInterModalPathMode)
		{
// 			Integer nxStationID(0);
// 			NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
			setAccessNodeStation.insert(nxStationID);

// 			KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
// 			double dAssignTrip(0.0);
// 						
// 			if ( KEMInterModeBus == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 			else if( KEMInterModeRail == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTRailSubData.dTrip;
// 			else if( KEMInterModeBusRail == oBusAssignInfo.emInterMode )
// 				dAssignTrip = oBusAssignInfo.oTBusSubData.dTrip;
// 
// 			AutoType iterfind = mapWalkingTrip.find(oLastWalk); 
// 			AutoType iterend  = mapWalkingTrip.end();
// 			if (iterfind == iterend)
// 			{
// 				mapWalkingTrip.insert(std::make_pair(oLastWalk, dAssignTrip));
// 			}
// 			else
// 			{
// 				double &dSumValue = iterfind->second;
// 				dSumValue += dAssignTrip;
// 			}
		}
	}
#pragma endregion 라인정보담기

	// regional main 라인 정보 담기
	CreateRegionalMainTrip(a_vecMainPathInfo, OUT setOriginZoneID, setDestinationZoneID, setTerminalID, setTransTerminalID, mapRegionalModeMainTrip);
}


void KInterModalPathInfo::RegionalMainTrip(KODKey oODKey, double a_dTrip, int a_nRegionalMode, std::map<int, std::map<KODKey, double>> &a_mapTestTrip)
{
	try
	{
		AutoType itFind = a_mapTestTrip.find(a_nRegionalMode);
		if (itFind == a_mapTestTrip.end())
		{
			std::map<KODKey, double> mapRegionalMainTrip;
			mapRegionalMainTrip.insert(std::make_pair(oODKey, a_dTrip));

			a_mapTestTrip.insert(std::make_pair(a_nRegionalMode, mapRegionalMainTrip));
		}
		else
		{
			std::map<KODKey, double> &mapRegionalMainTrip = itFind->second;

			AutoType itFindB = mapRegionalMainTrip.find(oODKey);
			if (itFindB == mapRegionalMainTrip.end())
			{
				mapRegionalMainTrip.insert(std::make_pair(oODKey, a_dTrip));
			}
			else
			{
				double &dSumVolume = itFindB->second;
				dSumVolume += a_dTrip;
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalPathInfo::ResultTransitEditInfo(KTarget* pTarget, std::vector<KInterPathEditInfo> &a_vecTransitEditInfo, 
	KEMInterModalPathMode a_emInterModalPathMode, std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo,
	TInMapResultValue& a_oMapResult, std::map<int, TInterPathColor> &a_mapUrbanColor)
{
	COLORREF clrBus         = RGB (   0, 128,   0 );
	COLORREF clrRail        = RGB (   0,   0, 255 );
	COLORREF clrWalk        = RGB ( 102, 102, 153 );
	COLORREF clrAutoTaxi    = RGB ( 255,   0,   0 );

	AutoType iterColor  = a_mapUrbanColor.begin();
	AutoType itEndColor = a_mapUrbanColor.end();

	while(iterColor != itEndColor)
	{
		int nMode = iterColor->first;
		TInterPathColor oTInterPathColor = iterColor->second;

		if (URBAN_MODE_AUTO == nMode)
		{
			clrAutoTaxi = oTInterPathColor.clrPath;
		} 
		else if (URBAN_MODE_BUS == nMode)
		{
			clrBus = oTInterPathColor.clrPath;
		}
		else if (URBAN_MODE_SUBWAY == nMode)
		{
			clrRail = oTInterPathColor.clrPath;
		}
		else if (URBAN_MODE_WALK == nMode)
		{
			clrWalk = oTInterPathColor.clrPath;
		}
		++iterColor;
	}

	double   dWidth     = 3.0;

#pragma region 저장소
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>    mapAutoTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<KODKey, double>	mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
	std::map<KODKey, double>	mapTerminalConnetWalkingTrip;

	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::set<Integer>			setTerminalID; // Draw와 중복
	std::set<Integer>			setTransTerminalID; // Draw와 중복
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;

	std::set<Integer>           setAccessNodeStation;
#pragma endregion 저장소

#pragma region 라인정보담기
	size_t nAssignCount = a_vecTransitEditInfo.size();
	for (size_t i = 0; i < nAssignCount; i++)
	{
		KInterPathEditInfo &oBusAssignInfo = a_vecTransitEditInfo[i];
		
		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMInPathType	emLastPathType = KEMInPathTypeNull;

		Integer			nxOZone = oBusAssignInfo.nxOrginID;
		Integer			nxDZone = oBusAssignInfo.nxDestinationID;

		Integer			nxAutoStart = oBusAssignInfo.nxAutoStartNode;
		Integer			nxAutoEnd   = oBusAssignInfo.nxAutoEndNode;

		if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Acccess 일때는  DZone이 의미 하는 바는 OrginTerminal
				setOriginZoneID.insert(nxOZone);
				setTerminalID.insert(nxDZone);
			}
			else if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Egress 일때는   OZone이 의미 하는 바는 DestinaionTerminal
				setDestinationZoneID.insert(nxDZone);
				setTerminalID.insert(nxOZone);
			}
		}
		else
		{
			setOriginZoneID.insert(nxOZone);
			setDestinationZoneID.insert(nxDZone);
		}

		if (KEMInterModeAutoTaxi == oBusAssignInfo.emInterMode)
		{
			KODKey oAutoTaxi(nxOZone, nxDZone);

			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();
			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, 1));
			}

			continue;
		}

		if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
			KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
			KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
		{
			KODKey oAutoTaxi(nxAutoStart, nxAutoEnd);

			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();
			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, 1));
			}
		}


		std::vector<TInIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResulTransitInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{

			std::vector<TInResultTransitInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TInResultTransitInfo &oTMResultBusInfo = vecTResult[nIndex]; 

				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
						KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
						KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
					{
						KODKey oTransWalk(nxAutoEnd, nxStartID);
						mapTransferWalkingA2TTrip.insert(std::make_pair(oTransWalk, 1));
					}
					else
					{
						//OZone 부터 첫번째 정류장까지의 Walk
						Integer nxStationID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
						setAccessNodeStation.insert(nxStationID);

						if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
						{
							if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
							{
								Integer nxDestionationTerminal = nxOZone;

								KODKey oLastWalk(nxDestionationTerminal, nxStationID);

								mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, 1));
							}
						}

					}
				}
				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus && emLastPathType == KEMInPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMInPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if(oTMResultBusInfo.emPathType == KEMInPathTypeRail && emLastPathType == KEMInPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, 1));
					}
				}

				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus) {
                    //★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
                        //★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
                        //★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				Integer nxStationID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);

				Integer nxOriginTerminal = nxDZone;

				KODKey oLastWalk(nxStationID, nxOriginTerminal);

				mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, 1));
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값
		Integer nxStationID(0);

		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
		KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
		AutoType iterfind = mapWalkingTrip.find(oLastWalk);
		AutoType iterend  = mapWalkingTrip.end();

		mapWalkingTrip.insert(std::make_pair(oLastWalk, 1));

		setAccessNodeStation.insert(nxStationID);
	}
#pragma endregion 라인정보담기


	
	if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
	{
		AutoType itMainPath    =  a_mapMainPathInfo.begin();
		AutoType itEndMainPath =  a_mapMainPathInfo.end();
		while (itMainPath != itEndMainPath)
		{
			int           nMode          = itMainPath->first;
			std::vector<TMainPathInfo> &vecMainPathInfo = itMainPath->second;

			for (size_t i= 0; i< vecMainPathInfo.size(); i++)
			{
				TMainPathInfo &oTMainPath = vecMainPathInfo[i];

				if (REGION_MODE_CODE_AUTO == nMode) // Auto 일때 메인은 ozone 과 dzone
				{
					KODKey oODKey(oTMainPath.nxMasterOZoneID, oTMainPath.nxMasterDZoneID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");

					std::vector<TMapPoint> vecPoint;
					Node2XYCoordinate(pTarget, oODKey, vecPoint);
					oAutoLine.TPoints = vecPoint;
					vecUserLineInfo.push_back(oAutoLine);

					continue;
				}

				Integer nxOrgTermID   = oTMainPath.nxOrgTermID;
				Integer nxDesTermID   = oTMainPath.nxDesTermID;
				Integer nxTransTermID = oTMainPath.nxTransTermID;

				setTerminalID.insert(nxOrgTermID);
				setTerminalID.insert(nxDesTermID);

				if (nxTransTermID > 0) // 환승 터미널 존재
				{
					setTransTerminalID.insert(nxTransTermID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");
					
					KODKey oODKeyA(nxOrgTermID, nxTransTermID);
					std::vector<TMapPoint> vecPointA;
					Node2XYCoordinate(pTarget, oODKeyA, vecPointA);
					oAutoLine.TPoints = vecPointA;
					vecUserLineInfo.push_back(oAutoLine);

					KODKey oODKeyB(nxTransTermID, nxDesTermID);
					std::vector<TMapPoint> vecPointB;
					Node2XYCoordinate(pTarget, oODKeyB, vecPointB);
					oAutoLine.TPoints = vecPointB;
					vecUserLineInfo.push_back(oAutoLine);
				}
				else
				{
					KODKey oODKey(nxOrgTermID, nxDesTermID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");

					std::vector<TMapPoint> vecPoint;
					Node2XYCoordinate(pTarget, oODKey, vecPoint);
					oAutoLine.TPoints = vecPoint;
					vecUserLineInfo.push_back(oAutoLine);
				}
			}

			++itMainPath;
		}
	}


#pragma region Map에 맞게 데이터 정리

	//Auto
	AutoType iterAuto = mapAutoTrip.begin();
	AutoType endAuto  = mapAutoTrip.end();
	while(iterAuto != endAuto)
	{
		KODKey oODKey = iterAuto->first;

		TUserLineInfo oAutoLine;
		oAutoLine.TColorRGB = clrAutoTaxi;
		oAutoLine.TWidth = dWidth;
		oAutoLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oAutoLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oAutoLine);
		++iterAuto;
	}


	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID   = iterBus->first;
		oLinkInfo.TLabel    = _T("");
		oLinkInfo.TColorRGB = clrBus;
		oLinkInfo.TWidth    = dWidth;

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID    = iterRail->first;
		oLinkInfo.TLabel     = _T("");
		oLinkInfo.TColorRGB  = clrRail;
		oLinkInfo.TWidth     = dWidth;

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}

	//O존
	AutoType iterOZone = setOriginZoneID.begin();
	AutoType endOZone  = setOriginZoneID.end();
	while(iterOZone != endOZone)
	{
		SetOriginZoneNodeInfo(*iterOZone, vecZoneNodeInfo);

		++iterOZone;
	}

	//D존
	AutoType iterDZone = setDestinationZoneID.begin();
	AutoType endDZone  = setDestinationZoneID.end();
	while(iterDZone != endDZone)
	{
		SetDestinationZoneNodeInfo(*iterDZone, vecZoneNodeInfo);

		++iterDZone;
	}

	// 존 접근 가능 정류장
	AutoType iterAccessNodeStation = setAccessNodeStation.begin();
	AutoType endAccessNodeStation  = setAccessNodeStation.end();
	while(iterAccessNodeStation != endAccessNodeStation)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterAccessNodeStation;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterAccessNodeStation);
		oNodeInfo.TLabel = strNodeLable;

		//oNodeInfo.TColorRGB = RGB(196, 189, 151);
		oNodeInfo.TColorRGB = RGB(225, 123, 182);
		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 48;
		vecNodeInfo.push_back(oNodeInfo);

		++iterAccessNodeStation;
	}

	//정류장 
	AutoType iterStation = setStaionID.begin();
	AutoType endStation  = setStaionID.end();
	while(iterStation != endStation)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterStation;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterStation);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(255,165,0);

		//임시 사이즈
		oNodeInfo.TSize = 11.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterStation;
	}

	//터미널 
	AutoType iterTerminal = setTerminalID.begin();
	AutoType endTerminal  = setTerminalID.end();
	while(iterTerminal != endTerminal)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterTerminal;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterTerminal);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(128,0,192);

		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterTerminal;
	}

	//터미널 
	AutoType iterTransTerminal = setTransTerminalID.begin();
	AutoType endTransTerminal  = setTransTerminalID.end();
	while(iterTransTerminal != endTransTerminal)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterTransTerminal;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterTransTerminal);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(255,0,255);

		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterTransTerminal;
	}

	//버스에서 버스로
	AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
	AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
	while(iterB2BWalking != endB2BWalking)
	{
		KODKey oODKey = iterB2BWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2BWalking;
	}

	// Auto 에서 Transit
	AutoType iterA2TWalking = mapTransferWalkingA2TTrip.begin();
	AutoType endA2TWalking  = mapTransferWalkingA2TTrip.end();
	while(iterA2TWalking != endA2TWalking)
	{
		KODKey oODKey = iterA2TWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterA2TWalking;
	}

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey oODKey = iterB2SWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2SWalking;
	}

	//전철에서 전철로
	AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
	AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
	while(iterS2SWalking != endS2SWalking)
	{
		KODKey oODKey = iterS2SWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth    = dWidth;
		oWalkingLine.TLabel     = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2SWalking;
	}

	// access or egress 에서 터미널
	AutoType iterS2TWalking = mapTerminalConnetWalkingTrip.begin();
	AutoType endS2TWalking  = mapTerminalConnetWalkingTrip.end();
	while(iterS2TWalking != endS2TWalking)
	{
		KODKey oODKey = iterS2TWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth    = dWidth;
		oWalkingLine.TLabel     = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2TWalking;
	}

#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}


void KInterModalPathInfo::ResultTransitEditInfo2(KTarget* pTarget, std::set<DWORD_PTR> &a_setMBusAssignInfo, 
	KEMInterModalPathMode a_emInterModalPathMode, std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue)
{
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

#pragma region 라인정보담기
	auto iterMain  = a_setMBusAssignInfo.begin();
	auto itEndMain = a_setMBusAssignInfo.end();
	for (; iterMain != itEndMain; ++iterMain)
	{
		DWORD_PTR dwPtr = *iterMain;
		KInterPathEditInfo* pBusAssignInfo = (KInterPathEditInfo*)dwPtr;
		KInterPathEditInfo &oBusAssignInfo = *pBusAssignInfo;

		TInMapResultInputValue oInMapResultInputValue;
#pragma region 저장소
		std::map<KODKey, double>	&mapWalkingTrip					= oInMapResultInputValue.mapWalkingTrip;
		std::map<KODKey, double>    &mapAutoTrip					= oInMapResultInputValue.mapAutoTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2BTrip		= oInMapResultInputValue.mapTransferWalkingB2BTrip;
		std::map<KODKey, double>	&mapTransferWalkingB2STrip		= oInMapResultInputValue.mapTransferWalkingB2STrip;
		std::map<KODKey, double>	&mapTransferWalkingS2STrip		= oInMapResultInputValue.mapTransferWalkingS2STrip;
		std::map<KODKey, double>	&mapTransferWalkingA2TTrip		= oInMapResultInputValue.mapTransferWalkingA2TTrip;
		std::map<KODKey, double>	&mapTerminalConnetWalkingTrip	= oInMapResultInputValue.mapTerminalConnetWalkingTrip;

		std::map<Integer, double>	&mapBusLinkTrip					= oInMapResultInputValue.mapBusLinkTrip;
		std::map<Integer, double>	&mapSubwayLinkTrip				= oInMapResultInputValue.mapSubwayLinkTrip;
		std::set<Integer>			&setOriginZoneID				= oInMapResultInputValue.setOriginZoneID;
		std::set<Integer>			&setDestinationZoneID			= oInMapResultInputValue.setDestinationZoneID;
		std::set<Integer>			&setStaionID					= oInMapResultInputValue.setStaionID;
		std::set<Integer>			&setBusSationID					= oInMapResultInputValue.setBusSationID;
		std::set<Integer>			&setSubwayStaionID				= oInMapResultInputValue.setSubwayStaionID;
		std::set<Integer>			&setTerminalID					= oInMapResultInputValue.setTerminalID;
		std::set<Integer>			&setTransTerminalID				= oInMapResultInputValue.setTransTerminalID;
		std::set<Integer>           &setAccessNodeStation			= oInMapResultInputValue.setAccessNodeStation;
#pragma endregion 저장소

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMInPathType	emLastPathType = KEMInPathTypeNull;

		Integer			nxOZone = oBusAssignInfo.nxOrginID;
		Integer			nxDZone = oBusAssignInfo.nxDestinationID;

		Integer			nxAutoStart = oBusAssignInfo.nxAutoStartNode;
		Integer			nxAutoEnd   = oBusAssignInfo.nxAutoEndNode;

		if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Acccess 일때는  DZone이 의미 하는 바는 OrginTerminal
				setOriginZoneID.insert(nxOZone);
				setTerminalID.insert(nxDZone);
			}
			else if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
			{
				//프로토콜 상에서 Egress 일때는   OZone이 의미 하는 바는 DestinaionTerminal
				setDestinationZoneID.insert(nxDZone);
				setTerminalID.insert(nxOZone);
			}
		}
		else
		{
			setOriginZoneID.insert(nxOZone);
			setDestinationZoneID.insert(nxDZone);
		}

		if (KEMInterModeAutoTaxi == oBusAssignInfo.emInterMode)
		{
			KODKey oAutoTaxi(nxOZone, nxDZone);

			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();
			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, 1));
			}

			//continue;
		}

		if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
			KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
			KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
		{
			KODKey oAutoTaxi(nxAutoStart, nxAutoEnd);

			AutoType iterAutoReg  = mapAutoTrip.find(oAutoTaxi);
			AutoType itEndAutoReg = mapAutoTrip.end();
			if (iterAutoReg == itEndAutoReg)
			{
				mapAutoTrip.insert(std::make_pair(oAutoTaxi, 1));
			}
		}


		std::vector<TInIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResulTransitInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{

			std::vector<TInResultTransitInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TInResultTransitInfo &oTMResultBusInfo = vecTResult[nIndex]; 

				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation,   oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					if (KEMInterModeParkRide    == oBusAssignInfo.emInterMode || 
						KEMInterModeKissRide    == oBusAssignInfo.emInterMode ||
						KEMInterModeTaxiTransit == oBusAssignInfo.emInterMode )
					{
						KODKey oTransWalk(nxAutoEnd, nxStartID);
						mapTransferWalkingA2TTrip.insert(std::make_pair(oTransWalk, 1));
					}
					else
					{
						//OZone 부터 첫번째 정류장까지의 Walk
						Integer nxStationID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
						setAccessNodeStation.insert(nxStationID);

						if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
						{
							if (KEMInterAccEgrTypeEgress == oBusAssignInfo.emInterAccEgrType)
							{
								Integer nxDestionationTerminal = nxOZone;

								KODKey oLastWalk(nxDestionationTerminal, nxStationID);

								mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, 1));
							}
						}

					}
				}
				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus && emLastPathType == KEMInPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMInPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation,                    nxLastTransitID,              nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if(oTMResultBusInfo.emPathType == KEMInPathTypeRail && emLastPathType == KEMInPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, 1));
					}
				}

				if (oTMResultBusInfo.emPathType == KEMInPathTypeBus) {
					//★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
					DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
						//★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
						//★ NewGetTransitInfo -> DllTransitSeqToLinkTrip 함수로 통합 
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
		{
			if (KEMInterAccEgrTypeAccess == oBusAssignInfo.emInterAccEgrType)
			{
				Integer nxStationID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);

				Integer nxOriginTerminal = nxDZone;

				KODKey oLastWalk(nxStationID, nxOriginTerminal);

				mapTerminalConnetWalkingTrip.insert(std::make_pair(oLastWalk, 1));
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값
		Integer nxStationID(0);

		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
		KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
		AutoType iterfind = mapWalkingTrip.find(oLastWalk);
		AutoType iterend  = mapWalkingTrip.end();

		mapWalkingTrip.insert(std::make_pair(oLastWalk, 1));

		setAccessNodeStation.insert(nxStationID);

		a_mapDestResultInputValue.insert(std::make_pair(dwPtr, oInMapResultInputValue));
	}
#pragma endregion 라인정보담기
}

void KInterModalPathInfo::ResultTransitEditInfo3(KTarget* pTarget, std::map<DWORD_PTR, TInMapResultInputValue> &a_mapDestResultInputValue, std::set<DWORD_PTR> &a_setMBusAssignInfo,
	KEMInterModalPathMode a_emInterModalPathMode, std::map<int, std::vector<TMainPathInfo>> &a_mapMainPathInfo,
	TInMapResultValue& a_oMapResult, std::map<int, TInterPathColor> &a_mapUrbanColor)
{
	COLORREF clrBus         = RGB (   0, 128,   0 );
	COLORREF clrRail        = RGB (   0,   0, 255 );
	COLORREF clrWalk        = RGB ( 102, 102, 153 );
	COLORREF clrAutoTaxi    = RGB ( 255,   0,   0 );

	AutoType iterColor  = a_mapUrbanColor.begin();
	AutoType itEndColor = a_mapUrbanColor.end();

	while(iterColor != itEndColor)
	{
		int nMode = iterColor->first;
		TInterPathColor oTInterPathColor = iterColor->second;

		if (URBAN_MODE_AUTO == nMode)
		{
			clrAutoTaxi = oTInterPathColor.clrPath;
		} 
		else if (URBAN_MODE_BUS == nMode)
		{
			clrBus = oTInterPathColor.clrPath;
		}
		else if (URBAN_MODE_SUBWAY == nMode)
		{
			clrRail = oTInterPathColor.clrPath;
		}
		else if (URBAN_MODE_WALK == nMode)
		{
			clrWalk = oTInterPathColor.clrPath;
		}
		++iterColor;
	}

	double   dWidth     = 3.0;

#pragma region 저장소

	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>    mapAutoTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<KODKey, double>	mapTransferWalkingA2TTrip; // Auto 에서 Transit : kiss&ride 등...
	std::map<KODKey, double>	mapTerminalConnetWalkingTrip;

	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::set<Integer>			setTerminalID; // Draw와 중복
	std::set<Integer>			setTransTerminalID; // Draw와 중복
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;

	std::set<Integer>           setAccessNodeStation;
#pragma endregion 저장소

	if (KEMInterModalPathModeRegional == a_emInterModalPathMode)
	{
		AutoType itMainPath    =  a_mapMainPathInfo.begin();
		AutoType itEndMainPath =  a_mapMainPathInfo.end();
		while (itMainPath != itEndMainPath)
		{
			int           nMode          = itMainPath->first;
			std::vector<TMainPathInfo> &vecMainPathInfo = itMainPath->second;

			for (size_t i= 0; i< vecMainPathInfo.size(); i++)
			{
				TMainPathInfo &oTMainPath = vecMainPathInfo[i];

				if (REGION_MODE_CODE_AUTO == nMode) // Auto 일때 메인은 ozone 과 dzone
				{
					KODKey oODKey(oTMainPath.nxMasterOZoneID, oTMainPath.nxMasterDZoneID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");

					std::vector<TMapPoint> vecPoint;
					Node2XYCoordinate(pTarget, oODKey, vecPoint);
					oAutoLine.TPoints = vecPoint;
					vecUserLineInfo.push_back(oAutoLine);

					continue;
				}

				Integer nxOrgTermID   = oTMainPath.nxOrgTermID;
				Integer nxDesTermID   = oTMainPath.nxDesTermID;
				Integer nxTransTermID = oTMainPath.nxTransTermID;

				setTerminalID.insert(nxOrgTermID);
				setTerminalID.insert(nxDesTermID);

				if (nxTransTermID > 0) // 환승 터미널 존재
				{
					setTransTerminalID.insert(nxTransTermID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");

					KODKey oODKeyA(nxOrgTermID, nxTransTermID);
					std::vector<TMapPoint> vecPointA;
					Node2XYCoordinate(pTarget, oODKeyA, vecPointA);
					oAutoLine.TPoints = vecPointA;
					vecUserLineInfo.push_back(oAutoLine);

					KODKey oODKeyB(nxTransTermID, nxDesTermID);
					std::vector<TMapPoint> vecPointB;
					Node2XYCoordinate(pTarget, oODKeyB, vecPointB);
					oAutoLine.TPoints = vecPointB;
					vecUserLineInfo.push_back(oAutoLine);
				}
				else
				{
					KODKey oODKey(nxOrgTermID, nxDesTermID);

					TUserLineInfo oAutoLine;
					oAutoLine.TColorRGB = oTMainPath.clrMode;
					oAutoLine.TWidth = dWidth;
					oAutoLine.TLabel = _T("");

					std::vector<TMapPoint> vecPoint;
					Node2XYCoordinate(pTarget, oODKey, vecPoint);
					oAutoLine.TPoints = vecPoint;
					vecUserLineInfo.push_back(oAutoLine);
				}
			}

			++itMainPath;
		}
	}

	auto iterKey  = a_setMBusAssignInfo.begin();
	auto itKeyEnd = a_setMBusAssignInfo.end();
	for (; iterKey != itKeyEnd; ++iterKey)
	{
		DWORD_PTR dwPtr = *iterKey;

		auto itFind = a_mapDestResultInputValue.find(dwPtr);
		if (itFind == a_mapDestResultInputValue.end()) {
			continue;
		}

		TInMapResultInputValue &oInMapResultInputValue = itFind->second;

		mapWalkingTrip.insert(oInMapResultInputValue.mapWalkingTrip.begin(), oInMapResultInputValue.mapWalkingTrip.end());
		mapAutoTrip.insert(oInMapResultInputValue.mapAutoTrip.begin(), oInMapResultInputValue.mapAutoTrip.end());
		mapTransferWalkingB2BTrip.insert(oInMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oInMapResultInputValue.mapTransferWalkingB2BTrip.end());
		mapTransferWalkingB2STrip.insert(oInMapResultInputValue.mapTransferWalkingB2STrip.begin(), oInMapResultInputValue.mapTransferWalkingB2STrip.end());
		mapTransferWalkingS2STrip.insert(oInMapResultInputValue.mapTransferWalkingS2STrip.begin(), oInMapResultInputValue.mapTransferWalkingS2STrip.end());
		mapTransferWalkingA2TTrip.insert(oInMapResultInputValue.mapTransferWalkingA2TTrip.begin(), oInMapResultInputValue.mapTransferWalkingA2TTrip.end());
		mapTerminalConnetWalkingTrip.insert(oInMapResultInputValue.mapTerminalConnetWalkingTrip.begin(), oInMapResultInputValue.mapTerminalConnetWalkingTrip.end());

		mapBusLinkTrip.insert(oInMapResultInputValue.mapBusLinkTrip.begin(), oInMapResultInputValue.mapBusLinkTrip.end());
		mapSubwayLinkTrip.insert(oInMapResultInputValue.mapSubwayLinkTrip.begin(), oInMapResultInputValue.mapSubwayLinkTrip.end());
		setOriginZoneID.insert(oInMapResultInputValue.setOriginZoneID.begin(), oInMapResultInputValue.setOriginZoneID.end());
		setDestinationZoneID.insert(oInMapResultInputValue.setDestinationZoneID.begin(), oInMapResultInputValue.setDestinationZoneID.end());
		setStaionID.insert(oInMapResultInputValue.setStaionID.begin(), oInMapResultInputValue.setStaionID.end());
		setBusSationID.insert(oInMapResultInputValue.setBusSationID.begin(), oInMapResultInputValue.setBusSationID.end());
		setSubwayStaionID.insert(oInMapResultInputValue.setSubwayStaionID.begin(), oInMapResultInputValue.setSubwayStaionID.end());
		setTerminalID.insert(oInMapResultInputValue.setTerminalID.begin(), oInMapResultInputValue.setTerminalID.end());
		setTransTerminalID.insert(oInMapResultInputValue.setTransTerminalID.begin(), oInMapResultInputValue.setTransTerminalID.end());
		setAccessNodeStation.insert(oInMapResultInputValue.setAccessNodeStation.begin(), oInMapResultInputValue.setAccessNodeStation.end());
	}

#pragma region Map에 맞게 데이터 정리

	//Auto
	AutoType iterAuto = mapAutoTrip.begin();
	AutoType endAuto  = mapAutoTrip.end();
	while(iterAuto != endAuto)
	{
		KODKey oODKey = iterAuto->first;

		TUserLineInfo oAutoLine;
		oAutoLine.TColorRGB = clrAutoTaxi;
		oAutoLine.TWidth = dWidth;
		oAutoLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oAutoLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oAutoLine);
		++iterAuto;
	}


	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID   = iterBus->first;
		oLinkInfo.TLabel    = _T("");
		oLinkInfo.TColorRGB = clrBus;
		oLinkInfo.TWidth    = dWidth;

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID    = iterRail->first;
		oLinkInfo.TLabel     = _T("");
		oLinkInfo.TColorRGB  = clrRail;
		oLinkInfo.TWidth     = dWidth;

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}

	//O존
	AutoType iterOZone = setOriginZoneID.begin();
	AutoType endOZone  = setOriginZoneID.end();
	while(iterOZone != endOZone)
	{
		SetOriginZoneNodeInfo(*iterOZone, vecZoneNodeInfo);

		++iterOZone;
	}

	//D존
	AutoType iterDZone = setDestinationZoneID.begin();
	AutoType endDZone  = setDestinationZoneID.end();
	while(iterDZone != endDZone)
	{
		SetDestinationZoneNodeInfo(*iterDZone, vecZoneNodeInfo);

		++iterDZone;
	}

	// 존 접근 가능 정류장
	AutoType iterAccessNodeStation = setAccessNodeStation.begin();
	AutoType endAccessNodeStation  = setAccessNodeStation.end();
	while(iterAccessNodeStation != endAccessNodeStation)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterAccessNodeStation;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterAccessNodeStation);
		oNodeInfo.TLabel = strNodeLable;

		//oNodeInfo.TColorRGB = RGB(196, 189, 151);
		oNodeInfo.TColorRGB = RGB(225, 123, 182);
		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 48;
		vecNodeInfo.push_back(oNodeInfo);

		++iterAccessNodeStation;
	}

	//정류장 
	AutoType iterStation = setStaionID.begin();
	AutoType endStation  = setStaionID.end();
	while(iterStation != endStation)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterStation;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterStation);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(255,165,0);

		//임시 사이즈
		oNodeInfo.TSize = 11.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterStation;
	}

	//터미널 
	AutoType iterTerminal = setTerminalID.begin();
	AutoType endTerminal  = setTerminalID.end();
	while(iterTerminal != endTerminal)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterTerminal;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterTerminal);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(128,0,192);

		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterTerminal;
	}

	//터미널 
	AutoType iterTransTerminal = setTransTerminalID.begin();
	AutoType endTransTerminal  = setTransTerminalID.end();
	while(iterTransTerminal != endTransTerminal)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterTransTerminal;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterTransTerminal);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = RGB(255,0,255);

		//임시 사이즈
		oNodeInfo.TSize = 15.0;

		//임시 모양
		oNodeInfo.TSymbol = 72;

		vecNodeInfo.push_back(oNodeInfo);
		++iterTransTerminal;
	}

	//버스에서 버스로
	AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
	AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
	while(iterB2BWalking != endB2BWalking)
	{
		KODKey oODKey = iterB2BWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2BWalking;
	}

	// Auto 에서 Transit
	AutoType iterA2TWalking = mapTransferWalkingA2TTrip.begin();
	AutoType endA2TWalking  = mapTransferWalkingA2TTrip.end();
	while(iterA2TWalking != endA2TWalking)
	{
		KODKey oODKey = iterA2TWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterA2TWalking;
	}

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey oODKey = iterB2SWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth = dWidth;
		oWalkingLine.TLabel = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2SWalking;
	}

	//전철에서 전철로
	AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
	AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
	while(iterS2SWalking != endS2SWalking)
	{
		KODKey oODKey = iterS2SWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth    = dWidth;
		oWalkingLine.TLabel     = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2SWalking;
	}

	// access or egress 에서 터미널
	AutoType iterS2TWalking = mapTerminalConnetWalkingTrip.begin();
	AutoType endS2TWalking  = mapTerminalConnetWalkingTrip.end();
	while(iterS2TWalking != endS2TWalking)
	{
		KODKey oODKey = iterS2TWalking->first;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		oWalkingLine.TWidth    = dWidth;
		oWalkingLine.TLabel     = _T("");

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2TWalking;
	}

#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}


void KInterModalPathInfo::SetOriginZoneNodeInfo(Integer a_nxZoneID , std::vector<TDrawNodeInfo> &a_vecZoneNodeInfo, COLORREF a_clrOrigin)
{
	try
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = a_nxZoneID;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), a_nxZoneID);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = a_clrOrigin;
		//임시 모양
		oNodeInfo.TSymbol = 48;

		a_vecZoneNodeInfo.push_back(oNodeInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalPathInfo::SetDestinationZoneNodeInfo(Integer a_nxZoneID , std::vector<TDrawNodeInfo> &a_vecZoneNodeInfo, COLORREF a_clrDestiantion)

{
	try
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = a_nxZoneID;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), a_nxZoneID);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = a_clrDestiantion;
		//임시 모양
		oNodeInfo.TSymbol = 48;

		a_vecZoneNodeInfo.push_back(oNodeInfo);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}



void KInterModalPathInfo::CheckParentItem( CXTPReportRecord* pRecord )
{
	CXTPReportRecord*      pParentRecord = pRecord->GetParentRecord();
	if (pParentRecord == nullptr)
	{
		return;
	}
	CXTPReportRecordItemText* pParentItem = (CXTPReportRecordItemText*)pParentRecord->GetItem(0);
	pParentItem->SetChecked(TRUE);
	CheckParentItem(pParentRecord);
}


void KInterModalPathInfo::UnCheckParentItem( CXTPReportRecord* pRecord )
{

	CXTPReportRecord*      pParentRecord = pRecord->GetParentRecord();
	if (pParentRecord == nullptr)
	{
		return;
	}

	bool bChecked = false;

	CXTPReportRecords* pParentRecords = pRecord->GetRecords();
	int nSiblingCount = pParentRecords->GetCount();
	for (int i = 0; i < nSiblingCount; i++)
	{
		CXTPReportRecord*    pSiblingRecord = pParentRecords->GetAt(i);
		CXTPReportRecordItemText*  pChdItem   = (CXTPReportRecordItemText*)pSiblingRecord->GetItem(0);
		CString strCaption = pChdItem->GetCaption(nullptr);
		if (pChdItem->IsChecked())
		{
			bChecked = true;
		}
		if (pChdItem->GetHasCheckbox() == FALSE )
		{
			bChecked = true;
			continue;
		}
	}

	if (bChecked == true)
	{
		return;
	}
	else
	{
		CXTPReportRecordItemText* pParentItem = (CXTPReportRecordItemText*)pParentRecord->GetItem(0);
		pParentItem->SetChecked(FALSE);
		UnCheckParentItem(pParentRecord);
	}
}


void KInterModalPathInfo::CheckChildItem( CXTPReportRecord* pRecord, BOOL a_BChecked )
{
	if (TRUE == pRecord->HasChildren())
	{
		CXTPReportRecords* pChildRecords = pRecord->GetChilds();
		int nCount = pChildRecords->GetCount();
		for (int i = 0; i < nCount; i++)
		{
			CXTPReportRecord*    pSiblingRecord = pChildRecords->GetAt(i);
			CXTPReportRecordItemText*  pChdItem   = (CXTPReportRecordItemText*)pSiblingRecord->GetItem(0);
			pChdItem->SetChecked(a_BChecked);
			CheckChildItem(pSiblingRecord, a_BChecked);
		}
	}
}


void KInterModalPathInfo::ResultAreaModeDrawInfo(KTarget* a_pTarget, TInModePathResultInputValue &a_oTInModePathResultInputValue, TInModePathResultSettingInfo &a_oTInResltSettingInfo, 
												TInMapResultValue& a_oMapResult, bool a_bRatio)
{
	try
	{
		std::set<Integer>			&setOriginZoneID           = a_oTInModePathResultInputValue.setOriginZoneID;
		std::set<Integer>			&setDestinationZoneID      = a_oTInModePathResultInputValue.setDestinationZoneID;
		std::map<int, double>       &mapModeSumVolume          = a_oTInModePathResultInputValue.mapModeSumVolume;
		std::vector<TMapPoint>      &vecAvgSelectedPoints      = a_oTInModePathResultInputValue.vecAvgSelectedPoints;

		std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
		std::vector<TUserLineInfo>	vecUserLineInfo;
		std::vector<TUserPointInfo> vecUserPointInfo;

		//O존
		AutoType iterOZone = setOriginZoneID.begin();
		AutoType endOZone  = setOriginZoneID.end();
#pragma region Zone Origin
		while(iterOZone != endOZone)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterOZone;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterOZone);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = a_oTInResltSettingInfo.clrZone;

			//임시 사이즈
			oNodeInfo.TSize = 18.0;

			oNodeInfo.TColorRGB = RGB(255,0,0);
			//임시 모양
			oNodeInfo.TSymbol = 48;

			vecZoneNodeInfo.push_back(oNodeInfo);
			++iterOZone;
		}
#pragma endregion Zone Origin

		//D존
		AutoType iterDZone = setDestinationZoneID.begin();
		AutoType endDZone  = setDestinationZoneID.end();
#pragma region Zone Destination
		while(iterDZone != endDZone)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterDZone;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterDZone);
			oNodeInfo.TLabel = strNodeLable;

			oNodeInfo.TColorRGB = a_oTInResltSettingInfo.clrZone;

			//임시 사이즈
			oNodeInfo.TSize = 18.0;

			oNodeInfo.TColorRGB = RGB(0,0,205);
			//임시 모양
			oNodeInfo.TSymbol = 48;

			vecZoneNodeInfo.push_back(oNodeInfo);

			++iterDZone;
		}
#pragma endregion Zone Destination

		AutoType iterVol  = mapModeSumVolume.begin();
		AutoType itVolEnd = mapModeSumVolume.end();

		CString strUserPointLabel(_T(""));
		double  dSumWidth(0.0);
		while (iterVol != itVolEnd)
		{
		  	int    nModeID  = iterVol->first;
			double &dTrip   = iterVol->second;

			TUserLineInfo oAreaToLine;

			if (dTrip > a_oTInResltSettingInfo.dMaxTrip)
			{
				++iterVol;
				continue;
			}
			else if (dTrip < a_oTInResltSettingInfo.dMinTrip)
			{
				++iterVol;
				continue;
			}

			double dWidth(0.0);
			CalculateValue2Width(dTrip, a_oTInResltSettingInfo, dWidth);
			dSumWidth += dWidth;

			oAreaToLine.TWidth = dSumWidth;

			CString strTrip(_T(""));
			if (KEMInterPathAreaModeAuto == nModeID)
			{
				//strTrip.Format(_T("Auto: "));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strTrip.Format(_T("Auto: "));
				}
				else
				{
					strTrip.Format(_T("PassengerCar: "));
				}
				oAreaToLine.TColorRGB = a_oTInResltSettingInfo.clrAuto;
			}
			else if (KEMInterPathAreaModeTaxi == nModeID)
			{
				strTrip.Format(_T("Taxi: "));
				oAreaToLine.TColorRGB = a_oTInResltSettingInfo.clrTaxi;
			}
			else if (KEMInterPathAreaModeBus == nModeID)
			{
				strTrip.Format(_T("Bus: "));
				oAreaToLine.TColorRGB = a_oTInResltSettingInfo.clrBus;
			}
			else if (KEMInterPathAreaModeRail == nModeID)
			{
				strTrip.Format(_T("Rail: "));
				oAreaToLine.TColorRGB = a_oTInResltSettingInfo.clrRail;
			}

			strTrip.AppendFormat(_T("%.2f"), dTrip);
			oAreaToLine.TLabel = strTrip;

			strUserPointLabel.AppendFormat(_T("%s\n"), strTrip);

			oAreaToLine.TPoints = vecAvgSelectedPoints;
			vecUserLineInfo.push_back(oAreaToLine);

			++iterVol;
		}
		
		TUserPointInfo oUserPointInfo;
		oUserPointInfo.Tx     = vecAvgSelectedPoints[1].Tx; // Destination Info
		oUserPointInfo.Ty     = vecAvgSelectedPoints[1].Ty; 
		oUserPointInfo.TLabel = strUserPointLabel;
		oUserPointInfo.TSize  = 0.1;
		oUserPointInfo.TColorRGB = vecUserLineInfo[0].TColorRGB;
		vecUserPointInfo.push_back(oUserPointInfo);

		std::sort(vecUserLineInfo.begin(), vecUserLineInfo.end(), PathSumVolDescending);

		a_oMapResult.vecDrawZoneNodeInfo = vecZoneNodeInfo;
		a_oMapResult.vecUserLineInfo     = vecUserLineInfo;
		a_oMapResult.vecUserPointInfo    = vecUserPointInfo;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterModalPathInfo::TerminalAccEgrTripInfo(KTarget* a_pTarget, Integer a_nxTerminalID, std::map<int, std::map<KODKey, std::map<int, double>>> &a_mapTypeODModeTrip, 
	TInTerminalAccEgrSettingInfo &a_oTResltSettingInfo, TInMapResultValue& a_oMapResult)
{
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;

	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;
	std::vector<TUserPointInfo> vecUserPointInfo;
	try
	{
		AutoType iter  = a_mapTypeODModeTrip.begin();
		AutoType itEnd = a_mapTypeODModeTrip.end();
		while (iter != itEnd)
		{
			int nAccEgrType = iter->first;
			std::map<KODKey, std::map<int, double>> &mapODModeTrip = iter->second;

			AutoType iterB  = mapODModeTrip.begin();
			AutoType itEndB = mapODModeTrip.end();
			while (iterB != itEndB)
			{
				KODKey oODKey                      = iterB->first;
				std::map<int, double> &mapModeTrip = iterB->second;

				if (KEMInterAccEgrTypeAccess == nAccEgrType)
				{
					setOriginZoneID.insert(oODKey.OriginID);
				}
				else
				{
					setDestinationZoneID.insert(oODKey.DestinationID);
				}

				if (mapModeTrip.size() < 1)
				{
					++iterB;
					continue;
				}

				AutoType iterVol  = mapModeTrip.begin();
				AutoType itVolEnd = mapModeTrip.end();

				std::vector<TUserLineInfo>	vecSubUserLineInfo;
				CString strUserPointLabel(_T(""));
				double  dSumWidth(0.0);
				while (iterVol != itVolEnd)
				{
					int    nModeID  = iterVol->first;
					double &dTrip   = iterVol->second;

					TUserLineInfo oAreaToLine;

					if (dTrip > a_oTResltSettingInfo.dMaxTrip)
					{
						++iterVol;
						continue;
					}
					else if (dTrip < a_oTResltSettingInfo.dMinTrip)
					{
						++iterVol;
						continue;
					}

					double dWidth(0.0);
					CalculateValue2Width(dTrip, a_oTResltSettingInfo, dWidth);
					dSumWidth += dWidth;

					oAreaToLine.TWidth = dSumWidth;

					CString strTrip(_T(""));

					std::map<int, TInterPathColor> &mapModeColor = a_oTResltSettingInfo.mapModeColor;
					AutoType itFindColor = mapModeColor.find(nModeID);
					if (itFindColor != mapModeColor.end())
					{
						TInterPathColor &oTInterPathColor = itFindColor->second;

						strTrip.Format(_T("%s: "), oTInterPathColor.strName);
						oAreaToLine.TColorRGB = oTInterPathColor.clrPath;
					}
					else
					{
						oAreaToLine.TColorRGB = RGB(255, 0, 0);
					}

					strTrip.AppendFormat(_T("%.2f"), dTrip);
					oAreaToLine.TLabel = strTrip;

					strUserPointLabel.AppendFormat(_T("%s\n"), strTrip);

					std::vector<TMapPoint> vecPoint;
					Node2XYCoordinate(a_pTarget, oODKey, vecPoint);
					oAreaToLine.TPoints = vecPoint;

					vecSubUserLineInfo.push_back(oAreaToLine);

					++iterVol;
				}

				if (vecSubUserLineInfo.size() > 0)
				{
					std::vector<TMapPoint> vecPoint = vecSubUserLineInfo[0].TPoints;

					TUserPointInfo oUserPointInfo;
					oUserPointInfo.Tx     = vecPoint[1].Tx; 
					oUserPointInfo.Ty     = vecPoint[1].Ty; 
					oUserPointInfo.TLabel = strUserPointLabel;
					oUserPointInfo.TSize  = 0.1;
					oUserPointInfo.TColorRGB = vecSubUserLineInfo[0].TColorRGB;
					vecUserPointInfo.push_back(oUserPointInfo);
				}

				std::sort(vecSubUserLineInfo.begin(), vecSubUserLineInfo.end(), PathSumVolDescending);

// 				for (size_t i= 0; i< vecSubUserLineInfo.size(); i++) {
// 					TUserLineInfo &oUserLineInfo = vecSubUserLineInfo[i];
// 					vecUserLineInfo.push_back(oUserLineInfo);
// 				}

				int nPartBeginIdx(0);
				int nPartEndIdx(0);

				CString strUserLineLabel(_T(""));
				int nPartLineCount = (int)vecSubUserLineInfo.size();
				for (int i= 0; i< nPartLineCount; i++) {
					TUserLineInfo &oSubUserLineInfo = vecSubUserLineInfo[i];
					
					CString strPartLabel = oSubUserLineInfo.TLabel;
					oSubUserLineInfo.TLabel = _T("");

					if (i == nPartLineCount-1) { // 마지막 인덱스
						strUserLineLabel.AppendFormat(_T("%s"), strPartLabel);
					}
					else {
						strUserLineLabel.AppendFormat(_T("%s\r\n"), strPartLabel);
					}

					vecUserLineInfo.push_back(oSubUserLineInfo);

					if (0 == i) {
						nPartBeginIdx = vecUserLineInfo.size() -1;
					}
					else if (nPartLineCount-1 == i) {
						nPartEndIdx = vecUserLineInfo.size() -1;
					}
				}

				// 가장두꺼운 Partline에 병합 레이블 삽입
				if (nPartLineCount > 0) {
					//vecUserLineInfo[nPartEndIdx].TLabel = strUserLineLabel;
					vecUserLineInfo[nPartBeginIdx].TLabel = strUserLineLabel;
				}

				++iterB;
			}

			++iter;
		}

		//터미널
#pragma region 터미널
// 		TDrawNodeInfo oNodeInfo;
// 		oNodeInfo.TNodeID = a_nxTerminalID;
// 
// 		CString strNodeLable(_T(""));
// 		strNodeLable.Format(_T("%I64d"), a_nxTerminalID);
// 		oNodeInfo.TLabel = strNodeLable;
// 
// 		oNodeInfo.TColorRGB = RGB(128,0,192);
// 
// 		//임시 사이즈
// 		oNodeInfo.TSize = 15.0;
// 
// 		//임시 모양
// 		oNodeInfo.TCharacterIndex = 72;
// 
// 		vecNodeInfo.push_back(oNodeInfo);
#pragma endregion 터미널

		//O존
		AutoType iterOZone = setOriginZoneID.begin();
		AutoType endOZone  = setOriginZoneID.end();
#pragma region Zone Origin
		while(iterOZone != endOZone)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterOZone;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterOZone);
			oNodeInfo.TLabel = strNodeLable;

			//임시 사이즈
			oNodeInfo.TSize = 18.0;

			oNodeInfo.TColorRGB = a_oTResltSettingInfo.clrOrgZone;
			//임시 모양
			oNodeInfo.TSymbol = 48;

			vecZoneNodeInfo.push_back(oNodeInfo);
			++iterOZone;
		}
#pragma endregion Zone Origin

		//D존
		AutoType iterDZone = setDestinationZoneID.begin();
		AutoType endDZone  = setDestinationZoneID.end();
#pragma region Zone Destination
		while(iterDZone != endDZone)
		{
			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID = *iterDZone;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), *iterDZone);
			oNodeInfo.TLabel = strNodeLable;

			//임시 사이즈
			oNodeInfo.TSize = 18.0;

			oNodeInfo.TColorRGB = a_oTResltSettingInfo.clrDestZone;
			//임시 모양
			oNodeInfo.TSymbol = 48;

			vecZoneNodeInfo.push_back(oNodeInfo);

			++iterDZone;
		}
#pragma endregion Zone Destination
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	a_oMapResult.vecDrawNodeInfo     = vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo = vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo     = vecUserLineInfo;
	a_oMapResult.vecUserPointInfo    = vecUserPointInfo;
}


bool KInterModalPathInfo::PathSumVolDescending( TUserLineInfo left, TUserLineInfo right )
{
	return left.TWidth > right.TWidth;
}


void KInterModalPathInfo::CalculateValue2Width(double a_dTrip, TInModePathResultSettingInfo a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


void KInterModalPathInfo::CalculateValue2Width(double a_dTrip, TInResultSettingInfo a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


void KInterModalPathInfo::CalculateValue2Width(double a_dTrip, TInTerminalAccEgrSettingInfo a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}

void KInterModalPathInfo::DllTransitSeqToLinkTrip( std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TInResultTransitInfo oResultInfo, std::map<Integer, double>& mapLinkTrip )
{
    __int64 nxLineID = oResultInfo.nxTransitID;    
    auto iter = mapTransitLinkSeq.find(nxLineID);
    if (iter == mapTransitLinkSeq.end())
        return;

    std::vector<__int64>& vecTransitLink = iter->second;

    //★ 명지대 모듈은 1번을 시작 인덱스로 한다.
    int nIdxFromSeq = oResultInfo.nStartStation; {
        nIdxFromSeq--;
    }
    int nIdxToSeq = oResultInfo.nEndStation; {
        nIdxToSeq--;
    }

    //★ 동일한 Seq는 올 수 없다.
    if (nIdxFromSeq == nIdxToSeq)
        return;

    //★ 0보다 작은 인덱스는 명지대 오류이다.
    if (nIdxFromSeq < 0 || nIdxToSeq < 0) {
        return;
    }

    //★ 앞에서 -1을 했기 때문에 이러한 경우는 명지대 오류이다.
    if (nIdxFromSeq >= (int)vecTransitLink.size() || nIdxToSeq > (int)vecTransitLink.size()) {
        return;
    }

    //★ 두가지 케이스가 있다.
    if (nIdxFromSeq < nIdxToSeq) 
    {
        for (int i = nIdxFromSeq; i < nIdxToSeq; i++) {
            __int64 nxLinkID = vecTransitLink[i];
            double& dTrip = mapLinkTrip[nxLinkID]; {
                dTrip += oResultInfo.dTrip;
            }
        }

        return; //★ 종료한다.
    }

    for (int i = nIdxFromSeq; i < (int)vecTransitLink.size(); i++) {
        __int64 nxLinkID = vecTransitLink[i];
        double& dTrip = mapLinkTrip[nxLinkID]; {
            dTrip += oResultInfo.dTrip;
        }
    }

    for (int i = 0; i < nIdxToSeq; i++) {
        __int64 nxLinkID = vecTransitLink[i];
        double& dTrip = mapLinkTrip[nxLinkID]; {
            dTrip += oResultInfo.dTrip;
        }
    }
}

/*
void KInterModalPathInfo::NewGetTransitInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TInResultTransitInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip )
{
    try
	{
		bool bUseSEQ = false; 
		
		AutoType iter  = a_mapLineSeq.find(a_oResultInfo.nxTransitID);
		AutoType itEnd = a_mapLineSeq.end();

		if(iter == itEnd)
			return;

		std::vector<Integer> &vecTransitLinks = iter->second;
		size_t nSize = vecTransitLinks.size();
		for (size_t i= 0; i< nSize; i++)
		{
			int     nSeq		= (i+1);
			Integer nxLinkID	= vecTransitLinks[i];

			int     nDLLSEQ     = nSeq + 1;

			if (nDLLSEQ == a_oResultInfo.nStartStation )
			{
				bUseSEQ = true;
			}
			else if (a_oResultInfo.nStartStation == 1 && nSeq == 1)
			{
				bUseSEQ = true;
				AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
				AutoType iterend  = a_mapLinkTrip.end();
				if (iterfind == iterend)
				{
					a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
				}
				else
				{
					double dExitValue = iterfind->second;
					dExitValue = dExitValue + a_oResultInfo.dTrip;
					iterfind->second = dExitValue;
				}
				if (nDLLSEQ == a_oResultInfo.nEndStation)
				{
					bUseSEQ = false;
				}
			}
			else if (true == bUseSEQ)
			{
				AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
				AutoType iterend  = a_mapLinkTrip.end();
				if (iterfind == iterend)
				{
					a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
				}
				else
				{
					double dExitValue = iterfind->second;
					dExitValue = dExitValue + a_oResultInfo.dTrip;
					iterfind->second = dExitValue;
				}

				if (nDLLSEQ == a_oResultInfo.nEndStation)
				{
					bUseSEQ = false;
				}
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}    
}
*/

/*
void KInterModalPathInfo::GetReverseTransitInfo( std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TInResultTransitInfo a_oResultInfo, std::map<Integer, double>& a_mapLinkTrip, std::map<Integer, TLinkFTNodeID> a_mapFTNode)
{
	bool bUseSEQ = false; 
	try
	{
		std::map<int, Integer> mapBeforeLine;
		std::map<int, Integer> mapAfterLine;

		AutoType iter = a_mapLineSeq.find(a_oResultInfo.nxTransitID);
		AutoType end  = a_mapLineSeq.end();
		if (iter == end)
		{
			return;
		}

		bool bDuplication(false);
		Integer nxStartNodeID(0);
		int nMiddleSEQ(0);
		int nDuplicationCount(0);
		int nLastSEQ(0);

		std::vector<Integer>& vecTransitLins = iter->second;
		int nSize = vecTransitLins.size();
		for (int i = 0; i < nSize; i++)
		{
			int nSEQ = i + 1;

			Integer nxLinkID = vecTransitLins[i];

			if (i == 0)
			{
				AutoType findSFNode = a_mapFTNode.find(nxLinkID);
				AutoType endSFNode	= a_mapFTNode.end();
				if (findSFNode != endSFNode)
				{
					TLinkFTNodeID oNodeID = findSFNode->second;
					nxStartNodeID = oNodeID.TFromNodeID;
				}
			}

			Integer nxNestNodeID(0);
			AutoType findTNode = a_mapFTNode.find(nxLinkID);
			AutoType endTNode = a_mapFTNode.end();
			if (findTNode != endTNode )
			{
				TLinkFTNodeID oNodeID = findTNode->second;
				nxNestNodeID = oNodeID.TToNodeID; 
			}

			if (false == bDuplication)
			{
				mapBeforeLine.insert(std::make_pair(nSEQ, nxLinkID));
			}
			else
			{
				mapAfterLine.insert(std::make_pair(nSEQ,nxLinkID));
			}

			if (nxStartNodeID == nxNestNodeID)
			{
				nDuplicationCount++;
				if (false == bDuplication)
				{
					nMiddleSEQ = nSEQ;
				}
				bDuplication = true;
			}


			nLastSEQ = nSEQ;
		}

		if (2 == nDuplicationCount)
		{
			int nStartSEQ = a_oResultInfo.nStartStation;
			int nEndSEQ	  = a_oResultInfo.nEndStation;
			//시작 정류장이 중간정류장보다 작을경우 
			if (nStartSEQ <= nMiddleSEQ)
			{
				// 시작 부터 Middle 
				AutoType iterbefore = mapBeforeLine.begin();
				AutoType endbefore	= mapBeforeLine.end();
				while(iterbefore != endbefore)
				{
					int nSeq = iterbefore->first;
					Integer nxLinkID = iterbefore->second;
					if (nSeq == nStartSEQ)
					{
						bUseSEQ = true;
					}

					if (true == bUseSEQ)
					{
						AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
						AutoType iterend  = a_mapLinkTrip.end();
						if (iterfind == iterend)
						{
							a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + a_oResultInfo.dTrip;
							iterfind->second = dExitValue;
						}
					}
					iterbefore++;
				}

				if (nStartSEQ == nLastSEQ)
				{
					bUseSEQ = true;
				}

				//Middle 부터 EndSEQ까지
				AutoType iterAfter = mapBeforeLine.begin();
				AutoType endAfter  = mapBeforeLine.end();
				while(iterAfter != endAfter)
				{
					int nSeq = iterAfter->first;
					Integer nxLinkID = iterAfter->second;

					if (nEndSEQ == nSeq)
					{
						bUseSEQ = false;
					}

					if(true == bUseSEQ)
					{
						AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
						AutoType iterend  = a_mapLinkTrip.end();
						if (iterfind == iterend)
						{
							a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + a_oResultInfo.dTrip;
							iterfind->second = dExitValue;
						}
					}


					iterAfter++;
				}
			}
			// 시작 정류장이 중간정류장보다 클경우
			else
			{
				AutoType iterbefore = mapAfterLine.begin();
				AutoType endbefore	= mapAfterLine.end();
				while(iterbefore != endbefore)
				{
					int nSeq = iterbefore->first;
					Integer nxLinkID = iterbefore->second;
					if (nSeq == nStartSEQ)
					{
						bUseSEQ = true;
					}

					if (true == bUseSEQ)
					{
						AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
						AutoType iterend  = a_mapLinkTrip.end();
						if (iterfind == iterend)
						{
							a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + a_oResultInfo.dTrip;
							iterfind->second = dExitValue;
						}
					}
					iterbefore++;
				}

				bUseSEQ = true;

				AutoType iterAfter = mapAfterLine.begin();
				AutoType endAfter  = mapAfterLine.end();
				while(iterAfter != endAfter)
				{

					int nSeq = iterAfter->first;
					Integer nxLinkID = iterAfter->second;

					if (nEndSEQ == nSeq)
					{
						bUseSEQ = false;
					}

					if(true == bUseSEQ)
					{
						AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
						AutoType iterend  = a_mapLinkTrip.end();
						if (iterfind == iterend)
						{
							a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + a_oResultInfo.dTrip;
							iterfind->second = dExitValue;
						}
					}


					iterAfter++;
				}
			}
		}
		else
		{
			AutoType iter  = a_mapLineSeq.find(a_oResultInfo.nxTransitID);
			AutoType itEnd = a_mapLineSeq.end();

			if(iter == itEnd)
				return;

			std::vector<Integer> &vecTransitLinks = iter->second;
			size_t nSize = vecTransitLinks.size();
			for (size_t i= 0; i< nSize; i++)
			{
				int     nSeq		= (i+1);
				Integer nxLinkID	= vecTransitLinks[i];

				int     nDLLSEQ     = nSeq + 1;

				if (nDLLSEQ == a_oResultInfo.nStartStation )
				{
					bUseSEQ = true;
				}
				else if (a_oResultInfo.nStartStation == 1 && nSeq == 1)
				{
					bUseSEQ = true;
					AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
					AutoType iterend  = a_mapLinkTrip.end();
					if (iterfind == iterend)
					{
						a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + a_oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}
					if (nDLLSEQ == a_oResultInfo.nEndStation)
					{
						bUseSEQ = false;
					}
				}
				else if (true == bUseSEQ)
				{
					AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
					AutoType iterend  = a_mapLinkTrip.end();
					if (iterfind == iterend)
					{
						a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + a_oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}

					if (nDLLSEQ == a_oResultInfo.nEndStation)
					{
						bUseSEQ = false;
					}
				}
			}

			for (size_t i= 0; i< nSize; i++)
			{
				int     nSeq		= (i+1);
				Integer nxLinkID	= vecTransitLinks[i];
				int     nDLLSEQ     = nSeq + 1;

				if (true == bUseSEQ)
				{
					AutoType iterfind = a_mapLinkTrip.find(nxLinkID);
					AutoType iterend  = a_mapLinkTrip.end();
					if (iterfind == iterend)
					{
						a_mapLinkTrip.insert(std::make_pair(nxLinkID,a_oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + a_oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}

					if (nDLLSEQ == a_oResultInfo.nEndStation)
					{
						bUseSEQ = false;
					}
				}
			}
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
*/


// 2018.08.01 명지대 요청사항 수정
void KInterModalPathInfo::updateODTripMap(std::map<KODKey, double>& src, std::map<KODKey, double>& dest)
{
	for (std::map<KODKey, double>::iterator it = src.begin(); it != src.end(); ++it)
	{
		std::map<KODKey, double>::iterator itFind = dest.find(it->first);
		if (dest.end() == itFind)
		{
			dest.insert({ it->first, it->second });
		}
		else
		{
			itFind->second += it->second;
		}
	}
}


void KInterModalPathInfo::updateLinkTripMap(std::map<Integer, double>& src, std::map<Integer, double>& dest)
{
	for (std::map<Integer, double>::iterator it = src.begin(); it != src.end(); ++it)
	{
		std::map<Integer, double>::iterator itFind = dest.find(it->first);
		if (dest.end() == itFind)
		{
			dest.insert({ it->first, it->second });
		}
		else
		{
			itFind->second += it->second;
		}
	}
}
// 2018.08.01 명지대 요청사항 수정 끝