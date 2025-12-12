#include "StdAfx.h"
#include "MultiModalResultInfo.h"
//^#include "DBaseConnector.h"



KMultiModalResultInfo::KMultiModalResultInfo(void)
{
}


KMultiModalResultInfo::~KMultiModalResultInfo(void)
{
}


void KMultiModalResultInfo::TransitPathFind( KTarget* pTarget, std::vector<KMBusAssignInfo> & a_vecTransitInfo, TMMapResultValue& a_oMapResult )
{

	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;

	std::map<KODKey, COLORREF>  mapWalkingOD;
	std::map<KODKey, COLORREF>	mapTransferWalkingB2BTrip;
	std::map<KODKey, COLORREF>	mapTransferWalkingB2STrip;
	std::map<KODKey, COLORREF>	mapTransferWalkingS2STrip;

	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	size_t nAssignCount = a_vecTransitInfo.size();
	for (size_t i = 0; i < nAssignCount; i++)
	{
		KMBusAssignInfo &oBusAssignInfo  = a_vecTransitInfo[i];

		int				nLastStation		= 0;
		Integer			nxLastTransitID		= 0;
		KEMPathType		emLastPathType		= KEMPathTypeNull;

		Integer			nxOZoneID			= oBusAssignInfo.nxOrginID;
		Integer			nxDZoneID			= oBusAssignInfo.nxDestinationID;

		setOriginZoneID.insert(nxOZoneID);
		setDestinationZoneID.insert(nxDZoneID);

		std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
		int nPathCount = vecTMResultBusInfo.size();

		for (int nPath = 0; nPath < nPathCount; nPath++ )
		{

			std::vector<TMResultBusInfo>& vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			int nSectionCount = vecTResult.size();

			for (int nSection = 0; nSection < nSectionCount; nSection++)
			{
				TMResultBusInfo &oTMResultBusInfo = vecTResult[nSection];
				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nSection)
				{
					KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStartID); 
					COLORREF clrWalk = RGB(0,255,0); // 임시색 다시 확인
					mapWalkingOD.insert(std::make_pair(oFirstWalk,clrWalk));
				}
				if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nSection)
				{
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastStation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					COLORREF clrWalk = RGB(0,255,0); // 임시색 다시 확인
					mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk,clrWalk));

				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nSection)
				{
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastStation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					COLORREF clrWalk = RGB(0,255,0); // 임시색 다시 확인
					mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk,clrWalk));

				}
				else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nSection )
				{
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastStation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					COLORREF clrWalk = RGB(0,255,0); // 임시색 다시 확인
					mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk,clrWalk));
				}
			}
		}
	}
}

void KMultiModalResultInfo::ResultBusPathDrawInfo3(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue, std::map<Integer, std::map<int, std::set<int>>> &a_mapZoneModePIndex, TMResultSettingInfo &a_oSettingInfo, TMMapResultValue& a_oMapResult, bool a_bLinkRatio)
{
#pragma region 저장소
	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::set<Integer>           setAccessNodeStation;

	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;

#pragma endregion 저장소

	auto iterMain  = a_mapZoneModePIndex.begin();
	auto itEndMain = a_mapZoneModePIndex.end();
	for (; iterMain != itEndMain; ++iterMain) {
		const Integer                &nxDZoneID     = iterMain->first;
		std::map<int, std::set<int>> &mapModePIndex = iterMain->second;

		auto itFindZone = a_mapDestResultInputValue.find(nxDZoneID);
		if (itFindZone == a_mapDestResultInputValue.end()) {
			continue;
		}
		std::map<int, std::vector<TMMapResultInputValue>> &mapModeInputValue = itFindZone->second;

		auto iterMode  = mapModePIndex.begin();
		auto itEndMode = mapModePIndex.end();
		for (; iterMode != itEndMode; ++iterMode) {
			const int     &nMode     = iterMode->first;
			std::set<int> &setPIndex = iterMode->second;

			auto itFind = mapModeInputValue.find(nMode);
			if (itFind == mapModeInputValue.end()) {
				continue;
			}

			std::vector<TMMapResultInputValue> &vecInputValue = itFind->second;
			for (int i= 0; i< (int)vecInputValue.size(); i++) {
				auto itFind = setPIndex.find(i);
				if (itFind == setPIndex.end()) {
					continue;
				}

				TMMapResultInputValue &oTMMapResultInputValue = vecInputValue[i];

				// 2018.07.30 명지대 요청 사항 수정
				for (std::map<Integer, double>::iterator it = oTMMapResultInputValue.mapBusLinkTrip.begin();
					it != oTMMapResultInputValue.mapBusLinkTrip.end(); ++it)
				{
					std::map<Integer, double>::iterator itTemp = mapBusLinkTrip.find(it->first);
					if (mapBusLinkTrip.end() == itTemp)
					{
						mapBusLinkTrip.insert({ it->first, it->second });
					}
					else
					{
						itTemp->second += it->second;
					}
				}
				//mapBusLinkTrip.insert(oTMMapResultInputValue.mapBusLinkTrip.begin(), oTMMapResultInputValue.mapBusLinkTrip.end());

				for (std::map<Integer, double>::iterator it = oTMMapResultInputValue.mapSubwayLinkTrip.begin();
					it != oTMMapResultInputValue.mapSubwayLinkTrip.end(); ++it)
				{
					std::map<Integer, double>::iterator itTemp = mapSubwayLinkTrip.find(it->first);
					if (mapSubwayLinkTrip.end() == itTemp)
					{
						mapSubwayLinkTrip.insert({ it->first, it->second });
					}
					else
					{
						itTemp->second += it->second;
					}
				}
				//mapSubwayLinkTrip.insert(oTMMapResultInputValue.mapSubwayLinkTrip.begin(), oTMMapResultInputValue.mapSubwayLinkTrip.end());
				// 2018.07.30 명지대 요청 사항 수정 끝

				setOriginZoneID.insert(oTMMapResultInputValue.setOriginZoneID.begin(), oTMMapResultInputValue.setOriginZoneID.end());
				setDestinationZoneID.insert(oTMMapResultInputValue.setDestinationZoneID.begin(), oTMMapResultInputValue.setDestinationZoneID.end());
				setAccessNodeStation.insert(oTMMapResultInputValue.setAccessNodeStation.begin(), oTMMapResultInputValue.setAccessNodeStation.end());
				setStaionID.insert(oTMMapResultInputValue.setStaionID.begin(), oTMMapResultInputValue.setStaionID.end());

				// 2018.07.30 명지대 요청 사항 수정
				for (std::map<KODKey, double>::iterator it = oTMMapResultInputValue.mapTransferWalkingB2BTrip.begin();
					it != oTMMapResultInputValue.mapTransferWalkingB2BTrip.end(); ++it)
				{
					std::map<KODKey, double>::iterator itTemp = mapTransferWalkingB2BTrip.find(it->first);
					if (mapTransferWalkingB2BTrip.end() == itTemp)
					{
						mapTransferWalkingB2BTrip.insert({ it->first, it->second });
					}
					else
					{
						itTemp->second += it->second;
					}
				}
				//mapTransferWalkingB2BTrip.insert(oTMMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2BTrip.end());

				for (std::map<KODKey, double>::iterator it = oTMMapResultInputValue.mapTransferWalkingB2STrip.begin();
					it != oTMMapResultInputValue.mapTransferWalkingB2STrip.end(); ++it)
				{
					std::map<KODKey, double>::iterator itTemp = mapTransferWalkingB2STrip.find(it->first);
					if (mapTransferWalkingB2STrip.end() == itTemp)
					{
						mapTransferWalkingB2STrip.insert({ it->first, it->second });
					}
					else
					{
						itTemp->second += it->second;
					}
				}
				//mapTransferWalkingB2STrip.insert(oTMMapResultInputValue.mapTransferWalkingB2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2STrip.end());

				for (std::map<KODKey, double>::iterator it = oTMMapResultInputValue.mapTransferWalkingS2STrip.begin();
					it != oTMMapResultInputValue.mapTransferWalkingS2STrip.end(); ++it)
				{
					std::map<KODKey, double>::iterator itTemp = mapTransferWalkingS2STrip.find(it->first);
					if (mapTransferWalkingS2STrip.end() == itTemp)
					{
						mapTransferWalkingS2STrip.insert({ it->first, it->second });
					}
					else
					{
						itTemp->second += it->second;
					}
				}
				//mapTransferWalkingS2STrip.insert(oTMMapResultInputValue.mapTransferWalkingS2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingS2STrip.end());
				// 2018.07.30 명지대 요청 사항 수정 끝
			}
		}
	}

#pragma region Map에 맞게 데이터 정리

	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
#pragma region Bus Trip
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterBus->first;

		CString strTrip(_T(""));
		double  dTrip   = iterBus->second;
		strTrip.Format(_T("%.2f"),iterBus->second );
		oLinkInfo.TLabel = strTrip;

		if (dTrip > a_oSettingInfo.dMaxTrip)
		{
			if (false == a_oSettingInfo.bUseOutRange)
			{
				++iterBus;
				continue;
			}
			oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
			oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
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
		}
		else
		{
			oLinkInfo.TColorRGB = a_oSettingInfo.clrBus;
			double dWidth(0.0);
			CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
			oLinkInfo.TWidth = dWidth;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}
#pragma endregion Bus Trip

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
#pragma region Rail Trip
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterRail->first;

		CString strTrip(_T(""));
		double  dTrip   = iterRail->second;
		strTrip.Format(_T("%.2f"),iterRail->second );
		oLinkInfo.TLabel = strTrip;

		if (dTrip > a_oSettingInfo.dMaxTrip)
		{
			if (false == a_oSettingInfo.bUseOutRange)
			{
				++iterRail;
				continue;
			}
			oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
			oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
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
		}
		else
		{
			oLinkInfo.TColorRGB = a_oSettingInfo.clrRail;
			double dWidth(0.0);
			CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
			oLinkInfo.TWidth = dWidth;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}
#pragma endregion Rail Trip

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

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

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

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(0,0,205);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
		++iterDZone;
	}
#pragma endregion Zone Destination

    // 존 접근 가능 정류장
    AutoType iterAccessNodeStation = setAccessNodeStation.begin();
    AutoType endAccessNodeStation  = setAccessNodeStation.end();
#pragma region 존 접근 가능 정류장
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
#pragma endregion 존 접근 가능 정류장

	//정류장 
	AutoType iterStation = setStaionID.begin();
	AutoType endStation  = setStaionID.end();
#pragma region 정류장
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
#pragma endregion 정류장

	//존부터 정류장까지의 환승 - 명지대 제외 요청
 #pragma region 존부터 정류장까지의 환승 - 명지대 제외 요청
  /* 
	AutoType iterWalking = mapWalkingTrip.begin();
	AutoType endWalking  = mapWalkingTrip.end();
	while(iterWalking != endWalking)
	{
		KODKey oODKey = iterWalking->first;
		double dTrip  = iterWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterWalking;
	}
    */
#pragma endregion 존부터 정류장까지의 환승 - 명지대 제외 요청


	// 버스에서 버스로 환승 도보
	AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
	AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
#pragma region 버스에서 버스로 환승 도보
	while(iterB2BWalking != endB2BWalking)
	{
		KODKey oODKey = iterB2BWalking->first;
		double dTrip  = iterB2BWalking->second;


		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2BWalking;
	}
#pragma endregion 버스에서 버스로 환승 도보

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
#pragma region 버스에서 전철로 환승 도보
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey oODKey = iterB2SWalking->first;
		double dTrip  = iterB2SWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}


		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2SWalking;
	}
#pragma endregion 버스에서 전철로 환승 도보

	//전철에서 전철로
	AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
	AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
#pragma region 전철에서 전철로
	while(iterS2SWalking != endS2SWalking)
	{
		KODKey oODKey = iterS2SWalking->first;
		double dTrip  = iterS2SWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2SWalking;
	}
#pragma endregion 전철에서 전철로
#pragma endregion Map에 맞게 데이터 정리


	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}

void KMultiModalResultInfo::ResultBusPathTripInfo2(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<KMBusAssignInfo> *>> &a_mapDestMAssignInfo, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue)
{
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	auto iterMain  = a_mapDestMAssignInfo.begin();
	auto itEndMain = a_mapDestMAssignInfo.end();

	for (; iterMain != itEndMain; ++iterMain) {
		const Integer                                     &nxDZoneID          = iterMain->first;
		std::map<int, std::vector<KMBusAssignInfo>*>      &mapModeMAssignInfo = iterMain->second;
		// 결과 insert 
		std::map<int, std::vector<TMMapResultInputValue>> &mapModeResult      = a_mapDestResultInputValue[nxDZoneID];

		auto iterMode  = mapModeMAssignInfo.begin();
		auto itEndMode = mapModeMAssignInfo.end();

		for (; iterMode != itEndMode; ++iterMode) {
			const int                          &nMode               = iterMode->first;
			std::vector<KMBusAssignInfo>*      pvecAssignInfo       = iterMode->second;
			// 결과 insert 
			std::vector<TMMapResultInputValue> &vecResultInputValue = mapModeResult[nMode];

#pragma region 라인정보담기
			size_t nAssignCount = (*pvecAssignInfo).size();
			for (size_t i = 0; i < nAssignCount; i++)
			{
				KMBusAssignInfo &oBusAssignInfo = (*pvecAssignInfo)[i];

				TMMapResultInputValue oTMMapResultInputValue;
#pragma region 저장소
				std::map<KODKey, double>	&mapWalkingTrip            = oTMMapResultInputValue.mapWalkingTrip;
				std::map<KODKey, double>	&mapTransferWalkingB2BTrip = oTMMapResultInputValue.mapTransferWalkingB2BTrip;
				std::map<KODKey, double>	&mapTransferWalkingB2STrip = oTMMapResultInputValue.mapTransferWalkingB2STrip;
				std::map<KODKey, double>	&mapTransferWalkingS2STrip = oTMMapResultInputValue.mapTransferWalkingS2STrip;
				std::map<Integer, double>	&mapBusLinkTrip            = oTMMapResultInputValue.mapBusLinkTrip;
				std::map<Integer, double>	&mapSubwayLinkTrip         = oTMMapResultInputValue.mapSubwayLinkTrip;
				std::set<Integer>			&setOriginZoneID           = oTMMapResultInputValue.setOriginZoneID;
				std::set<Integer>			&setDestinationZoneID      = oTMMapResultInputValue.setDestinationZoneID;
				std::set<Integer>			&setStaionID               = oTMMapResultInputValue.setStaionID;
				std::set<Integer>			&setBusSationID            = oTMMapResultInputValue.setBusSationID;
				std::set<Integer>			&setSubwayStaionID         = oTMMapResultInputValue.setSubwayStaionID;
				std::set<Integer>           &setAccessNodeStation      = oTMMapResultInputValue.setAccessNodeStation;
#pragma endregion 저장소

				int				nLastSation	    = 0;
				Integer			nxLastTransitID = 0;
				KEMPathType		emLastPathType = KEMPathTypeNull;

				Integer			nxOZone = oBusAssignInfo.nxOrginID;
				Integer			nxDZone = oBusAssignInfo.nxDestinationID;

				setOriginZoneID.insert(nxOZone);
				setDestinationZoneID.insert(nxDZone);

				std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
				size_t nPathCount = vecTMResultBusInfo.size();
				for (size_t nPath = 0; nPath < nPathCount; nPath++)
				{

					std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
					size_t nCount = vecTResult.size();
					for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
					{
						TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

						Integer nxStartID(0);
						Integer nxEndID(0);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
						setStaionID.insert(nxStartID);
						setStaionID.insert(nxEndID);

						if (0 == nPath && 0 == nIndex )
						{
							//OZone 부터 첫번째 정류장까지의 Walk
							Integer nxStationID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
							KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);
							double dAssignTrip(0.0);
							if (KEMPathTypeBus == oTMResultBusInfo.emPathType )
							{
								dAssignTrip = oBusAssignInfo.oTBusSubData.dAssignTrip;
							}
							else
							{
								dAssignTrip = oBusAssignInfo.oTRailSubData.dAssignTrip;
							}

							AutoType iterfind = mapWalkingTrip.find(oFirstWalk);
							AutoType iterend  = mapWalkingTrip.end();
							if (iterfind == iterend)
							{
								mapWalkingTrip.insert(std::make_pair(oFirstWalk, dAssignTrip));
							}
							else
							{
								double dValue = iterfind->second;
								dValue = dValue + dAssignTrip;
								iterfind->second = dValue;
							}

							setAccessNodeStation.insert(nxStationID);
						}
						if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
						{
							//버스에서 버스로 환승시
							Integer nxFirstID;
							Integer nxLastID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
							KODKey oTransWalk(nxLastID, nxFirstID);
							AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
							AutoType iterend  = mapTransferWalkingB2BTrip.end();
							if (iterfind == iterend)
							{
								mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTBusSubData.dAssignTrip));
							}
							else
							{
								double dValue = iterfind->second;
								dValue = dValue + oTMResultBusInfo.dTrip;
								iterfind->second = dValue;
							}
						}
						else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
						{
							//버스에서 전철 또는 전철 에서 버스
							Integer nxFirstID;
							Integer nxLastID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
							KODKey oTransWalk(nxLastID, nxFirstID);
							AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
							AutoType iterend  = mapTransferWalkingB2STrip.end();
							if (iterfind == iterend)
							{
								mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTBusSubData.dAssignTrip));
							}
							else
							{
								double dValue = iterfind->second;
								dValue = dValue + oTMResultBusInfo.dTrip;
								iterfind->second = dValue;
							}
						}
						else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
						{
							//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
							if (false == oTMResultBusInfo.bDuplicationRail)
							{
								Integer nxFirstID;
								Integer nxLastID;
								NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
								NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
								KODKey oTransWalk(nxLastID, nxFirstID);
								AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
								AutoType iterend  = mapTransferWalkingS2STrip.end();
								if (iterfind == iterend)
								{
									mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTRailSubData.dAssignTrip));
								}
								else
								{
									double dValue = iterfind->second;
									dValue = dValue + oBusAssignInfo.oTRailSubData.dAssignTrip;
									iterfind->second = dValue;
								}
							}

						}

						if (oTMResultBusInfo.emPathType == KEMPathTypeBus) {					
							//★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
							DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
						}
						else
						{
							if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
								//★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
								DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
							}
							else {
								//★ GetReverseTransitInfo로 통합
								DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
							}					
						}

						emLastPathType	= oTMResultBusInfo.emPathType;
						nLastSation	    = oTMResultBusInfo.nEndStation;
						nxLastTransitID = oTMResultBusInfo.nxTransitID;
					}
				}

				//마지막 정류장 부터 DZone까지의 Walk값
				Integer nxStationID(0);

				/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
				KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
				AutoType iterfind = mapWalkingTrip.find(oLastWalk);
				AutoType iterend  = mapWalkingTrip.end();

				double dLAssignTrip(0.0);
				if (KEMPathTypeBus == emLastPathType )
				{
					dLAssignTrip = oBusAssignInfo.oTBusSubData.dAssignTrip;
				}
				else
				{
					dLAssignTrip = oBusAssignInfo.oTRailSubData.dAssignTrip;
				}

				if (iterfind == iterend)
				{
					mapWalkingTrip.insert(std::make_pair(oLastWalk, dLAssignTrip));
				}
				else
				{
					double dValue = iterfind->second;
					dValue = dValue + dLAssignTrip;
					iterfind->second = dValue;
				}
				setAccessNodeStation.insert(nxStationID);

				vecResultInputValue.push_back(oTMMapResultInputValue);
			}
#pragma endregion 라인정보담기
		}
	}
}


void KMultiModalResultInfo::ResultBusPathDrawInfo(KTarget* pTarget, TMMapResultInputValue& a_oMMapResultInput, TMResultSettingInfo &a_oSettingInfo, TMMapResultValue& a_oMapResult, bool a_bLinkRatio)
{
#pragma region 저장소
	std::map<KODKey, double>	&mapWalkingTrip            = a_oMMapResultInput.mapWalkingTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2BTrip = a_oMMapResultInput.mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2STrip = a_oMMapResultInput.mapTransferWalkingB2STrip;
	std::map<KODKey, double>	&mapTransferWalkingS2STrip = a_oMMapResultInput.mapTransferWalkingS2STrip;
	std::map<Integer, double>	&mapBusLinkTrip            = a_oMMapResultInput.mapBusLinkTrip;
	std::map<Integer, double>	&mapSubwayLinkTrip         = a_oMMapResultInput.mapSubwayLinkTrip;
	std::set<Integer>			&setOriginZoneID           = a_oMMapResultInput.setOriginZoneID;
	std::set<Integer>			&setDestinationZoneID      = a_oMMapResultInput.setDestinationZoneID;
	std::set<Integer>			&setStaionID               = a_oMMapResultInput.setStaionID;
	std::set<Integer>			&setBusSationID            = a_oMMapResultInput.setBusSationID;
	std::set<Integer>			&setSubwayStaionID         = a_oMMapResultInput.setSubwayStaionID;
	std::set<Integer>           &setAccessNodeStation      = a_oMMapResultInput.setAccessNodeStation;

	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;
#pragma endregion 저장소


#pragma region Map에 맞게 데이터 정리

	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
#pragma region Bus Trip
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterBus->first;

		CString strTrip(_T(""));
		double  dTrip   = iterBus->second;
		strTrip.Format(_T("%.2f"),iterBus->second );
		oLinkInfo.TLabel = strTrip;

		if (dTrip > a_oSettingInfo.dMaxTrip)
		{
			if (false == a_oSettingInfo.bUseOutRange)
			{
				++iterBus;
				continue;
			}
			oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
			oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
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
		}
		else
		{
			oLinkInfo.TColorRGB = a_oSettingInfo.clrBus;
			double dWidth(0.0);
			CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
			oLinkInfo.TWidth = dWidth;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}
#pragma endregion Bus Trip

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
#pragma region Rail Trip
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterRail->first;

		CString strTrip(_T(""));
		double  dTrip   = iterRail->second;
		strTrip.Format(_T("%.2f"),iterRail->second );
		oLinkInfo.TLabel = strTrip;

		if (dTrip > a_oSettingInfo.dMaxTrip)
		{
			if (false == a_oSettingInfo.bUseOutRange)
			{
				++iterRail;
				continue;
			}
			oLinkInfo.TColorRGB = a_oSettingInfo.clrOutRange;
			oLinkInfo.TWidth    = a_oSettingInfo.dMinSize;
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
		}
		else
		{
			oLinkInfo.TColorRGB = a_oSettingInfo.clrRail;
			double dWidth(0.0);
			CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
			oLinkInfo.TWidth = dWidth;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}
#pragma endregion Rail Trip

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

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

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

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(0,0,205);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
		++iterDZone;
	}
#pragma endregion Zone Destination

    // 존 접근 가능 정류장
    AutoType iterAccessNodeStation = setAccessNodeStation.begin();
    AutoType endAccessNodeStation  = setAccessNodeStation.end();
#pragma region 존 접근 가능 정류장
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
#pragma endregion 존 접근 가능 정류장

	//정류장 
	AutoType iterStation = setStaionID.begin();
	AutoType endStation  = setStaionID.end();
#pragma region 정류장
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
#pragma endregion 정류장

	//존부터 정류장까지의 환승 - 명지대 제외 요청
 #pragma region 존부터 정류장까지의 환승 - 명지대 제외 요청
  /* 
	AutoType iterWalking = mapWalkingTrip.begin();
	AutoType endWalking  = mapWalkingTrip.end();
	while(iterWalking != endWalking)
	{
		KODKey oODKey = iterWalking->first;
		double dTrip  = iterWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
			}
			else if (dTrip < a_oSettingInfo.dMinTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterWalking;
	}
    */
#pragma endregion 존부터 정류장까지의 환승 - 명지대 제외 요청


	// 버스에서 버스로 환승 도보
	AutoType iterB2BWalking = mapTransferWalkingB2BTrip.begin();
	AutoType endB2BWalking  = mapTransferWalkingB2BTrip.end();
#pragma region 버스에서 버스로 환승 도보
	while(iterB2BWalking != endB2BWalking)
	{
		KODKey oODKey = iterB2BWalking->first;
		double dTrip  = iterB2BWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2BWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2BWalking;
	}
#pragma endregion 버스에서 버스로 환승 도보

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
#pragma region 버스에서 전철로 환승 도보
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey oODKey = iterB2SWalking->first;
		double dTrip  = iterB2SWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterB2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize; 
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}


		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterB2SWalking;
	}
#pragma endregion 버스에서 전철로 환승 도보

	//전철에서 전철로
	AutoType iterS2SWalking = mapTransferWalkingS2STrip.begin();
	AutoType endS2SWalking  = mapTransferWalkingS2STrip.end();
#pragma region 전철에서 전철로
	while(iterS2SWalking != endS2SWalking)
	{
		KODKey oODKey = iterS2SWalking->first;
		double dTrip  = iterS2SWalking->second;

		TUserLineInfo oWalkingLine;

		if (a_bLinkRatio)
		{
			oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
			oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
			oWalkingLine.TLabel    = _T("");
		}
		else
		{
			if (dTrip > a_oSettingInfo.dMaxTrip)
			{
				if (false == a_oSettingInfo.bUseOutRange)
				{
					++iterS2SWalking;
					continue;
				}
				oWalkingLine.TColorRGB = a_oSettingInfo.clrOutRange;
				oWalkingLine.TWidth    = a_oSettingInfo.dMinSize;
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
			}
			else
			{
				oWalkingLine.TColorRGB = a_oSettingInfo.clrWalk;
				double dWidth(0.0);
				CalculateValue2Width(dTrip, a_oSettingInfo, dWidth);
				oWalkingLine.TWidth = dWidth;
			}

			CString strTrip(_T(""));
			strTrip.Format(_T("%.2f"), dTrip);
			oWalkingLine.TLabel = strTrip;
		}

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		++iterS2SWalking;
	}
#pragma endregion 전철에서 전철로
#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}

void KMultiModalResultInfo::ResultBusPathTripInfo(KTarget* pTarget, std::vector<KMBusAssignInfo> &a_vecBusAssinInfo, TMMapResultInputValue& a_oMapResultInput)
{

#pragma region 저장소
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	std::map<KODKey, double>	&mapWalkingTrip            = a_oMapResultInput.mapWalkingTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2BTrip = a_oMapResultInput.mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	&mapTransferWalkingB2STrip = a_oMapResultInput.mapTransferWalkingB2STrip;
	std::map<KODKey, double>	&mapTransferWalkingS2STrip = a_oMapResultInput.mapTransferWalkingS2STrip;
	std::map<Integer, double>	&mapBusLinkTrip            = a_oMapResultInput.mapBusLinkTrip;
	std::map<Integer, double>	&mapSubwayLinkTrip         = a_oMapResultInput.mapSubwayLinkTrip;
	std::set<Integer>			&setOriginZoneID           = a_oMapResultInput.setOriginZoneID;
	std::set<Integer>			&setDestinationZoneID      = a_oMapResultInput.setDestinationZoneID;
	std::set<Integer>			&setStaionID               = a_oMapResultInput.setStaionID;
	std::set<Integer>			&setBusSationID            = a_oMapResultInput.setBusSationID;
	std::set<Integer>			&setSubwayStaionID         = a_oMapResultInput.setSubwayStaionID;
    std::set<Integer>           &setAccessNodeStation      = a_oMapResultInput.setAccessNodeStation;
#pragma endregion 저장소

#pragma region 라인정보담기
	size_t nAssignCount = a_vecBusAssinInfo.size();
	for (size_t i = 0; i < nAssignCount; i++)
	{
		KMBusAssignInfo &oBusAssignInfo = a_vecBusAssinInfo[i];

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMPathType		emLastPathType = KEMPathTypeNull;

		Integer			nxOZone = oBusAssignInfo.nxOrginID;
		Integer			nxDZone = oBusAssignInfo.nxDestinationID;

		setOriginZoneID.insert(nxOZone);
		setDestinationZoneID.insert(nxDZone);

		std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{

			std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					//OZone 부터 첫번째 정류장까지의 Walk
					Integer nxStationID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
					KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);
					double dAssignTrip(0.0);
					if (KEMPathTypeBus == oTMResultBusInfo.emPathType )
					{
						dAssignTrip = oBusAssignInfo.oTBusSubData.dAssignTrip;
					}
					else
					{
						dAssignTrip = oBusAssignInfo.oTRailSubData.dAssignTrip;
					}

					AutoType iterfind = mapWalkingTrip.find(oFirstWalk);
					AutoType iterend  = mapWalkingTrip.end();
					if (iterfind == iterend)
					{
						mapWalkingTrip.insert(std::make_pair(oFirstWalk, dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + dAssignTrip;
						iterfind->second = dValue;
					}
					
                    setAccessNodeStation.insert(nxStationID);
				}
				if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2BTrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTBusSubData.dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + oTMResultBusInfo.dTrip;
						iterfind->second = dValue;
					}
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2STrip.end();
					if (iterfind == iterend)
					{
						mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTBusSubData.dAssignTrip));
					}
					else
					{
						double dValue = iterfind->second;
						dValue = dValue + oTMResultBusInfo.dTrip;
						iterfind->second = dValue;
					}
				}
				else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingS2STrip.end();
						if (iterfind == iterend)
						{
							mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, oBusAssignInfo.oTRailSubData.dAssignTrip));
						}
						else
						{
							double dValue = iterfind->second;
							dValue = dValue + oBusAssignInfo.oTRailSubData.dAssignTrip;
							iterfind->second = dValue;
						}
					}
					
				}

				if (oTMResultBusInfo.emPathType == KEMPathTypeBus) {					
					//★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
                    DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation) {
                        //★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
                        //★ GetReverseTransitInfo로 통합
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}					
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값
		Integer nxStationID(0);

		/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
		KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
		AutoType iterfind = mapWalkingTrip.find(oLastWalk);
		AutoType iterend  = mapWalkingTrip.end();

		double dLAssignTrip(0.0);
		if (KEMPathTypeBus == emLastPathType )
		{
			dLAssignTrip = oBusAssignInfo.oTBusSubData.dAssignTrip;
		}
		else
		{
			dLAssignTrip = oBusAssignInfo.oTRailSubData.dAssignTrip;
		}

		if (iterfind == iterend)
		{
			mapWalkingTrip.insert(std::make_pair(oLastWalk, dLAssignTrip));
		}
		else
		{
			double dValue = iterfind->second;
			dValue = dValue + dLAssignTrip;
			iterfind->second = dValue;
		}
        setAccessNodeStation.insert(nxStationID);
	}
#pragma endregion 라인정보담기
}


void KMultiModalResultInfo::ResultTransitShortestInfo(KTarget* pTarget, TMResultSettingInfo &a_oSettingInfo,  std::vector<KMBusAssignInfo> &a_vecBusAssinInfo, TMMapResultValue& a_oMapResult, COLORREF a_clrWalk)
{
	double   dFixWidth = 3.0;
	COLORREF &clrWalk  = a_clrWalk;

	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;
	std::set<Integer>                       setZoneNodeID;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);
	KBulkDBaseNode::ZoneNodeSetQuery(pTarget, setZoneNodeID);


	std::map<KODKey, TPathColor>	mapWalkingTrip;
	std::map<KODKey, TPathColor>	mapTransferWalkingB2BTrip;
	std::map<KODKey, TPathColor>	mapTransferWalkingB2STrip;
	std::map<KODKey, TPathColor>	mapTransferWalkingS2STrip;
	std::map<Integer, TPathColor>	mapBusLinkTrip;
	std::map<Integer, TPathColor>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginNodeID;
	std::set<Integer>			setDestinationNodeID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;

#pragma region 라인정보담기
	size_t nAssignCount = a_vecBusAssinInfo.size();
	for (size_t i = 0; i < nAssignCount; i++)
	{
		KMBusAssignInfo &oBusAssignInfo = a_vecBusAssinInfo[i];

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMPathType		emLastPathType = KEMPathTypeNull;

		Integer			nxONode = oBusAssignInfo.nxOrginID;
		Integer			nxDNode = oBusAssignInfo.nxDestinationID;

		setOriginNodeID.insert(nxONode);
		setDestinationNodeID.insert(nxDNode);

		std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{

			std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					//OZone 부터 첫번째 정류장까지의 Walk
					Integer nxStationID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
					KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);

					AutoType iterfind = mapWalkingTrip.find(oFirstWalk);
					AutoType iterend  = mapWalkingTrip.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = oTMResultBusInfo.clrSect;

						if( setZoneNodeID.find(oBusAssignInfo.nxOrginID) == setZoneNodeID.end())
						{
							mapWalkingTrip.insert(std::make_pair(oFirstWalk, oTCostData));
						}
					}
				}
				if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);

					AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2BTrip.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = oTMResultBusInfo.clrSect;

						mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, oTCostData));
					}
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
					AutoType iterend  = mapTransferWalkingB2STrip.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = oTMResultBusInfo.clrSect;

						mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, oTCostData));
					}
				}
				else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
				{
					//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
					if (false == oTMResultBusInfo.bDuplicationRail)
					{
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingS2STrip.end();
						if (iterfind == iterend)
						{
							TPathColor oTCostData;
							oTCostData.clrSect = oTMResultBusInfo.clrSect;

							mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, oTCostData));
						}
					}
				}

				if (oTMResultBusInfo.emPathType == KEMPathTypeBus) {
                    //★ GetTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합                    
					DllTransitSeqToLinkClr(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation)
					{
                        //★ GetTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합    
						DllTransitSeqToLinkClr(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else
					{
                        //★ GetReverseTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합    
						DllTransitSeqToLinkClr(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값
		Integer nxStationID(0);

		/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
		KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
		AutoType iterfind = mapWalkingTrip.find(oLastWalk);
		AutoType iterend  = mapWalkingTrip.end();

		if (iterfind == iterend)
		{
			TPathColor oTCostData;
			oTCostData.clrSect = clrWalk;

			if( setZoneNodeID.find(oBusAssignInfo.nxDestinationID) == setZoneNodeID.end())
			{
				mapWalkingTrip.insert(std::make_pair(oLastWalk, oTCostData));
			}
		}
	}
#pragma endregion 라인정보담기

#pragma region Map에 맞게 데이터 정리

	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterBus->first;

		CString strTrip(_T(""));
		oLinkInfo.TLabel = strTrip;
		oLinkInfo.TWidth = dFixWidth;

		TPathColor &oTCostData = iterBus->second;
		oLinkInfo.TColorRGB = oTCostData.clrSect;

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterRail->first;

		CString strTrip(_T(""));
		oLinkInfo.TLabel = strTrip;
		oLinkInfo.TWidth = dFixWidth;

		TPathColor &oTCostData = iterRail->second;
		oLinkInfo.TColorRGB = oTCostData.clrSect;

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}

	//Origin Node
	AutoType iterONode = setOriginNodeID.begin();
	AutoType endONode  = setOriginNodeID.end();
	while(iterONode != endONode)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterONode;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterONode);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(255,0,0);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecNodeInfo.push_back(oNodeInfo);

		if(setZoneNodeID.find(oNodeInfo.TNodeID) != setZoneNodeID.end())
		{
			vecZoneNodeInfo.push_back(oNodeInfo);
		}

		++iterONode;
	}

	//Destination Node
	AutoType iterDNode = setDestinationNodeID.begin();
	AutoType endDNode  = setDestinationNodeID.end();
	while(iterDNode != endDNode)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterDNode;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterDNode);
		oNodeInfo.TLabel = strNodeLable;

		oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(0,0,205);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecNodeInfo.push_back(oNodeInfo);

		if(setZoneNodeID.find(oNodeInfo.TNodeID) != setZoneNodeID.end())
		{
			vecZoneNodeInfo.push_back(oNodeInfo);
		}

		++iterDNode;
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

	
	//존부터 정류장까지의 도보-> 일반노드 일때 도보 표출, 존노드 일때 도보 미표출 - 명지대 요청
 	AutoType iterWalking = mapWalkingTrip.begin();
	AutoType endWalking  = mapWalkingTrip.end();
	while(iterWalking != endWalking)
	{
		KODKey    oODKey       = iterWalking->first;
		TPathColor &oTCostData  = iterWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;
		
		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip;
		oWalkingLine.TWidth = dFixWidth;
		
		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);
		
		++iterWalking;
	}

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey    oODKey       = iterB2SWalking->first;
		TPathColor &oTCostData  = iterB2SWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;

		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip;
		oWalkingLine.TWidth = dFixWidth;

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
		KODKey    oODKey       = iterS2SWalking->first;
		TPathColor &oTCostData  = iterS2SWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;

		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip;
		oWalkingLine.TWidth = dFixWidth;


		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);

		++iterS2SWalking;
	}
#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	 = vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	 = vecNodeInfo;
	a_oMapResult.vecUserLineInfo	 = vecUserLineInfo;
	a_oMapResult.vecDrawZoneNodeInfo = vecZoneNodeInfo;
}


void KMultiModalResultInfo::ResultTransitShortestInfo2(KTarget* pTarget, std::map<int, std::vector<KMBusAssignInfo>*> &a_mapModeMAssignInfo, std::map<int, std::vector<TTransitResultInputValue>> &a_mapModeResultInputValue)
{
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;
	std::set<Integer>                       setZoneNodeID;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);
	KBulkDBaseNode::ZoneNodeSetQuery(pTarget, setZoneNodeID);

	auto iterMode  = a_mapModeMAssignInfo.begin();
	auto itEndMode = a_mapModeMAssignInfo.end();

	for (; iterMode != itEndMode; ++iterMode) {
		const int                          &nMode               = iterMode->first;
		std::vector<KMBusAssignInfo>*      pvecAssignInfo       = iterMode->second;
		// 결과 insert 
		std::vector<TTransitResultInputValue> &vecResultInputValue = a_mapModeResultInputValue[nMode];

#pragma region 라인정보담기
		size_t nAssignCount = (*pvecAssignInfo).size();
		for (size_t i = 0; i < nAssignCount; i++)
		{
			KMBusAssignInfo &oBusAssignInfo = (*pvecAssignInfo)[i];

			TTransitResultInputValue oTMMapResultInputValue;
#pragma region 저장소
			std::map<KODKey, TMResultBusInfo*>	&mapWalkingTrip				= oTMMapResultInputValue.mapWalkingTrip;
			std::map<KODKey, TMResultBusInfo*>	&mapTransferWalkingB2BTrip	= oTMMapResultInputValue.mapTransferWalkingB2BTrip;
			std::map<KODKey, TMResultBusInfo*>	&mapTransferWalkingB2STrip	= oTMMapResultInputValue.mapTransferWalkingB2STrip;
			std::map<KODKey, TMResultBusInfo*>	&mapTransferWalkingS2STrip	= oTMMapResultInputValue.mapTransferWalkingS2STrip;
			std::map<Integer, TMResultBusInfo*>	&mapBusLinkTrip				= oTMMapResultInputValue.mapBusLinkTrip;
			std::map<Integer, TMResultBusInfo*>	&mapSubwayLinkTrip			= oTMMapResultInputValue.mapSubwayLinkTrip;
			std::set<Integer>			    &setOriginNodeID			= oTMMapResultInputValue.setOriginNodeID;
			std::set<Integer>			    &setDestinationNodeID		= oTMMapResultInputValue.setDestinationNodeID;
			std::set<Integer>			    &setStaionID				= oTMMapResultInputValue.setStaionID;		
			std::set<Integer>			    &setBusSationID				= oTMMapResultInputValue.setBusSationID;
			std::set<Integer>			    &setSubwayStaionID			= oTMMapResultInputValue.setSubwayStaionID;
#pragma endregion 저장소

			int				nLastSation	    = 0;
			Integer			nxLastTransitID = 0;
			KEMPathType		emLastPathType = KEMPathTypeNull;

			Integer			nxONode = oBusAssignInfo.nxOrginID;
			Integer			nxDNode = oBusAssignInfo.nxDestinationID;

			setOriginNodeID.insert(nxONode);
			setDestinationNodeID.insert(nxDNode);

			std::vector<TIndexResultTransitInfo> &vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
			size_t nPathCount = vecTMResultBusInfo.size();
			for (size_t nPath = 0; nPath < nPathCount; nPath++)
			{

				std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
				size_t nCount = vecTResult.size();
				for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
				{
					TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

					Integer nxStartID(0);
					Integer nxEndID(0);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
					setStaionID.insert(nxStartID);
					setStaionID.insert(nxEndID);

					if (0 == nPath && 0 == nIndex )
					{
						//OZone 부터 첫번째 정류장까지의 Walk
						Integer nxStationID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
						KODKey oFirstWalk(oBusAssignInfo.nxOrginID, nxStationID);

						AutoType iterfind = mapWalkingTrip.find(oFirstWalk);
						AutoType iterend  = mapWalkingTrip.end();
						if (iterfind == iterend)
						{
							//TPathColor oTCostData;
							//oTCostData.clrSect = oTMResultBusInfo.clrSect;

							if( setZoneNodeID.find(oBusAssignInfo.nxOrginID) == setZoneNodeID.end())
							{
								mapWalkingTrip.insert(std::make_pair(oFirstWalk, &oTMResultBusInfo));
								//oTMResultBusInfo 의 색상을 사용하겠다.
							}
						}
					}
					if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
					{
						//버스에서 버스로 환승시
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);

						AutoType iterfind = mapTransferWalkingB2BTrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingB2BTrip.end();
						if (iterfind == iterend)
						{
							//TPathColor oTCostData;
							//oTCostData.clrSect = oTMResultBusInfo.clrSect;
							mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, &oTMResultBusInfo));
							//oTMResultBusInfo 의 색상을 사용하겠다.
						}
					}
					else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
					{
						//버스에서 전철 또는 전철 에서 버스
						Integer nxFirstID;
						Integer nxLastID;
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
						KODKey oTransWalk(nxLastID, nxFirstID);
						AutoType iterfind = mapTransferWalkingB2STrip.find(oTransWalk);
						AutoType iterend  = mapTransferWalkingB2STrip.end();
						if (iterfind == iterend)
						{
							//TPathColor oTCostData;
							//oTCostData.clrSect = oTMResultBusInfo.clrSect;
							mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, &oTMResultBusInfo));
							//oTMResultBusInfo 의 색상을 사용하겠다.
						}
					}
					else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
					{
						//전철에서 전철로 환승시 // 두번째 세번째 전철은 제외
						if (false == oTMResultBusInfo.bDuplicationRail)
						{
							Integer nxFirstID;
							Integer nxLastID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
							KODKey oTransWalk(nxLastID, nxFirstID);
							AutoType iterfind = mapTransferWalkingS2STrip.find(oTransWalk);
							AutoType iterend  = mapTransferWalkingS2STrip.end();
							if (iterfind == iterend)
							{
								//TPathColor oTCostData;
								//oTCostData.clrSect = oTMResultBusInfo.clrSect;
								mapTransferWalkingS2STrip.insert(std::make_pair(oTransWalk, &oTMResultBusInfo));
								//oTMResultBusInfo 의 색상을 사용하겠다.
							}
						}
					}

					if (oTMResultBusInfo.emPathType == KEMPathTypeBus) {
						//★ GetTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합                    
						DllTransitSeqToLinkBusInfoPtr(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
					}
					else
					{
						if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation)
						{
							//★ GetTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합    
							DllTransitSeqToLinkBusInfoPtr(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
						}
						else
						{
							//★ GetReverseTransitShortestInfo -> DllTransitSeqToLinkClr 함수로 통합    
							DllTransitSeqToLinkBusInfoPtr(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
						}
					}

					emLastPathType	= oTMResultBusInfo.emPathType;
					nLastSation	    = oTMResultBusInfo.nEndStation;
					nxLastTransitID = oTMResultBusInfo.nxTransitID;
				}
			}

			//마지막 정류장 부터 DZone까지의 Walk값
			Integer nxStationID(0);

			/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
			NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
			KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
			AutoType iterfind = mapWalkingTrip.find(oLastWalk);
			AutoType iterend  = mapWalkingTrip.end();

			if (iterfind == iterend)
			{
				//TPathColor oTCostData;
				//oTCostData.clrSect = clrWalk; // Draw에서 할당 받는다.

				if( setZoneNodeID.find(oBusAssignInfo.nxDestinationID) == setZoneNodeID.end())
				{
					mapWalkingTrip.insert(std::make_pair(oLastWalk, nullptr));
				}
			}

			vecResultInputValue.push_back(oTMMapResultInputValue);
		}
#pragma endregion 라인정보담기
	}
}


void KMultiModalResultInfo::ResultTransitShortestInfo3(KTarget* pTarget, std::map<int, std::vector<TTransitResultInputValue>> &a_mapModeResultInputValue, std::map<int, std::set<int>> &a_mapModePIndex, TMResultSettingInfo &a_oSettingInfo, TMMapResultValue& a_oMapResult, COLORREF a_clrWalk)
{
	double   dFixWidth = 3.0;
	COLORREF &clrWalk  = a_clrWalk;
#pragma region 저장소
	std::map<KODKey, TMResultBusInfo*>	mapWalkingTrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingB2BTrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingB2STrip;
	std::map<KODKey, TMResultBusInfo*>	mapTransferWalkingS2STrip;
	std::map<Integer, TMResultBusInfo*>	mapBusLinkTrip;
	std::map<Integer, TMResultBusInfo*>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginNodeID;
	std::set<Integer>			setDestinationNodeID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
#pragma endregion 저장소

	auto iterMode  = a_mapModePIndex.begin();
	auto itEndMode = a_mapModePIndex.end();
	for (; iterMode != itEndMode; ++iterMode) {
		const int     &nMode     = iterMode->first;
		std::set<int> &setPIndex = iterMode->second;

		auto itFind = a_mapModeResultInputValue.find(nMode);
		if (itFind == a_mapModeResultInputValue.end()) {
			continue;
		}

		std::vector<TTransitResultInputValue> &vecInputValue = itFind->second;
		for (int i= 0; i< (int)vecInputValue.size(); i++) {
			auto itFind = setPIndex.find(i);
			if (itFind == setPIndex.end()) {
				continue;
			}

			TTransitResultInputValue &oTMMapResultInputValue = vecInputValue[i];

			mapWalkingTrip.insert(oTMMapResultInputValue.mapWalkingTrip.begin(), oTMMapResultInputValue.mapWalkingTrip.end());
			mapTransferWalkingB2BTrip.insert(oTMMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2BTrip.end());
			mapTransferWalkingB2STrip.insert(oTMMapResultInputValue.mapTransferWalkingB2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2STrip.end());
			mapTransferWalkingS2STrip.insert(oTMMapResultInputValue.mapTransferWalkingS2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingS2STrip.end());
			mapBusLinkTrip.insert(oTMMapResultInputValue.mapBusLinkTrip.begin(), oTMMapResultInputValue.mapBusLinkTrip.end());
			mapSubwayLinkTrip.insert(oTMMapResultInputValue.mapSubwayLinkTrip.begin(), oTMMapResultInputValue.mapSubwayLinkTrip.end());
			setOriginNodeID.insert(oTMMapResultInputValue.setOriginNodeID.begin(), oTMMapResultInputValue.setOriginNodeID.end());
			setDestinationNodeID.insert(oTMMapResultInputValue.setDestinationNodeID.begin(), oTMMapResultInputValue.setDestinationNodeID.end());
			setStaionID.insert(oTMMapResultInputValue.setStaionID.begin(), oTMMapResultInputValue.setStaionID.end());
			setBusSationID.insert(oTMMapResultInputValue.setBusSationID.begin(), oTMMapResultInputValue.setBusSationID.end());
			setSubwayStaionID.insert(oTMMapResultInputValue.setSubwayStaionID.begin(), oTMMapResultInputValue.setSubwayStaionID.end());
		}
	}

#pragma region Map에 맞게 데이터 정리

	//Bus
	AutoType iterBus = mapBusLinkTrip.begin();
	AutoType endBus  = mapBusLinkTrip.end();
	while(iterBus != endBus)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterBus->first;

		CString strTrip(_T(""));
		oLinkInfo.TLabel = strTrip;// 빈값
		oLinkInfo.TWidth = dFixWidth;

		TMResultBusInfo* pInfo = iterBus->second;
		if (nullptr != pInfo) {
			oLinkInfo.TColorRGB = pInfo->clrSect;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterBus;
	}

	//Rail
	AutoType iterRail = mapSubwayLinkTrip.begin();
	AutoType endRail  = mapSubwayLinkTrip.end();
	while(iterRail != endRail)
	{
		TDrawLinkInfo oLinkInfo;
		oLinkInfo.TLinkID = iterRail->first;

		CString strTrip(_T(""));
		oLinkInfo.TLabel = strTrip;// 빈값
		oLinkInfo.TWidth = dFixWidth;

		TMResultBusInfo* pInfo = iterRail->second;
		if (nullptr != pInfo) {
			oLinkInfo.TColorRGB = pInfo->clrSect;
		}

		vecLinkInfo.push_back(oLinkInfo);
		++iterRail;
	}

	//Origin Node
	{ // 외부에서 정의됨
// 		AutoType iterONode = setOriginNodeID.begin();
// 		AutoType endONode  = setOriginNodeID.end();
// 		while(iterONode != endONode)
// 		{
// 			TDrawNodeInfo oNodeInfo;
// 			oNodeInfo.TNodeID = *iterONode;
// 
// 			CString strNodeLable(_T(""));
// 			strNodeLable.Format(_T("%I64d"), *iterONode);
// 			oNodeInfo.TLabel = strNodeLable;
// 
// 			oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;
// 
// 			//임시 사이즈
// 			oNodeInfo.TSize = 18.0;
// 
// 			oNodeInfo.TColorRGB = RGB(255,0,0);
// 			//임시 모양
// 			oNodeInfo.TSymbol = 48;
// 
// 			vecNodeInfo.push_back(oNodeInfo);
// 
// 			if(setZoneNodeID.find(oNodeInfo.TNodeID) != setZoneNodeID.end())
// 			{
// 				vecZoneNodeInfo.push_back(oNodeInfo);
// 			}
// 
// 			++iterONode;
// 		}
	}


	//Destination Node
	{ // 외부에서 정의됨
// 		AutoType iterDNode = setDestinationNodeID.begin();
// 		AutoType endDNode  = setDestinationNodeID.end();
// 		while(iterDNode != endDNode)
// 		{
// 			TDrawNodeInfo oNodeInfo;
// 			oNodeInfo.TNodeID = *iterDNode;
// 
// 			CString strNodeLable(_T(""));
// 			strNodeLable.Format(_T("%I64d"), *iterDNode);
// 			oNodeInfo.TLabel = strNodeLable;
// 
// 			oNodeInfo.TColorRGB = a_oSettingInfo.clrZone;
// 
// 			//임시 사이즈
// 			oNodeInfo.TSize = 18.0;
// 
// 			oNodeInfo.TColorRGB = RGB(0,0,205);
// 			//임시 모양
// 			oNodeInfo.TSymbol = 48;
// 
// 			vecNodeInfo.push_back(oNodeInfo);
// 
// 			if(setZoneNodeID.find(oNodeInfo.TNodeID) != setZoneNodeID.end())
// 			{
// 				vecZoneNodeInfo.push_back(oNodeInfo);
// 			}
// 
// 			++iterDNode;
// 		}
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


	//존부터 정류장까지의 도보-> 일반노드 일때 도보 표출, 존노드 일때 도보 미표출 - 명지대 요청
	AutoType iterWalking = mapWalkingTrip.begin();
	AutoType endWalking  = mapWalkingTrip.end();
	while(iterWalking != endWalking)
	{
		KODKey           oODKey = iterWalking->first;
		TMResultBusInfo* pInfo  = iterWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;

		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip; // 빈값
		oWalkingLine.TWidth = dFixWidth;

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);

		++iterWalking;
	}

	// 버스에서 전철로 환승 도보
	AutoType iterB2SWalking = mapTransferWalkingB2STrip.begin();
	AutoType endB2SWalking  = mapTransferWalkingB2STrip.end();
	while(iterB2SWalking != endB2SWalking)
	{
		KODKey           oODKey = iterB2SWalking->first;
		TMResultBusInfo* pInfo  = iterB2SWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;

		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip; // 빈값
		oWalkingLine.TWidth = dFixWidth;

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
		KODKey           oODKey = iterS2SWalking->first;
		TMResultBusInfo* pInfo  = iterS2SWalking->second;

		TUserLineInfo oWalkingLine;
		oWalkingLine.TColorRGB = clrWalk;

		CString strTrip(_T(""));
		oWalkingLine.TLabel = strTrip; // 빈값
		oWalkingLine.TWidth = dFixWidth;

		std::vector<TMapPoint> vecPoint;
		Node2XYCoordinate(pTarget, oODKey, vecPoint);
		oWalkingLine.TPoints = vecPoint;
		vecUserLineInfo.push_back(oWalkingLine);

		++iterS2SWalking;
	}
#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	 = vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	 = vecNodeInfo;
	a_oMapResult.vecUserLineInfo	 = vecUserLineInfo;
	a_oMapResult.vecDrawZoneNodeInfo = vecZoneNodeInfo;
}


void KMultiModalResultInfo::CalculateValue2Width(double a_dTrip, TMResultSettingInfo a_oSettingInfo, double& dWidth )
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


void KMultiModalResultInfo::Node2XYCoordinate(KTarget* pTarget, KODKey oODKey, std::vector<TMapPoint>& a_vecMapPoint )
{
	Integer nxStartID = oODKey.OriginID;
	Integer nxEndID   = oODKey.DestinationID;

	KDBaseConPtr spDBase = pTarget->GetDBaseConnection();

    CString strSQL (_T("")); {
        strSQL.Format(_T(" Select node_id, x, y From %s Where node_id In ('%I64d', '%I64d') "), TABLE_NODE, nxStartID, nxEndID);
    }

	try
	{
        std::map<__int64, TMapPoint> mapInfo;

        KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
        while (spResult->Next())
        {
            __int64 nxID = spResult->GetValueInt(0);
            double dx = spResult->GetValueDouble(1);
            double dy = spResult->GetValueDouble(2);

            TMapPoint& oPoint = mapInfo[nxID]; {
                oPoint.Tx = dx;
                oPoint.Ty = dy;
            }
        }

        auto iFind = mapInfo.find(nxStartID);
        if (iFind != mapInfo.end()) {
            a_vecMapPoint.push_back(iFind->second);
        }

        iFind = mapInfo.find(nxEndID);
        if (iFind != mapInfo.end()) {
            a_vecMapPoint.push_back(iFind->second);
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


void KMultiModalResultInfo::DllTransitSeqToLinkTrip( std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, double>& mapLinkTrip )
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

/* DllTransitSeqToLinkTrip로 통합....
void KMultiModalResultInfo::GetReverseTransitInfo( std::map<Integer, std::vector<Integer>> &mapTransitLinkSeq, 
                                                   TMResultBusInfo   oResultInfo, 
                                                   std::map<Integer, double>& mapLinkTrip, 
                                                   std::map<Integer, TLinkFTNodeID> mapLinkFTNode)
{
	bool bUseSEQ = false; 
	try
	{
		std::map<int, Integer> mapBeforeLine;
		std::map<int, Integer> mapAfterLine;

        //★ 노선 경유 링크 정보
		auto iter = mapTransitLinkSeq.find(oResultInfo.nxTransitID);
		if (iter == mapTransitLinkSeq.end()) {
			return;
		}

		bool bDuplication(false);
		Integer nxStartNodeID(0);
		int nMiddleSEQ(0);
		int nDuplicationCount(0);
		int nLastSEQ(0);

		std::vector<Integer>& vecTransitLins = iter->second;
		int nSize = (int)vecTransitLins.size();
		
        for (int i = 0; i < nSize; i++)
		{
			int nSEQ = i + 1;

			Integer nxLinkID = vecTransitLins[i];
			
			if (i == 0) {
				AutoType findSFNode = mapLinkFTNode.find(nxLinkID);
				AutoType endSFNode	= mapLinkFTNode.end();
				if (findSFNode != endSFNode)
				{
					TLinkFTNodeID oNodeID = findSFNode->second;
					nxStartNodeID = oNodeID.TFromNodeID;
				}
			}

			Integer nxNestNodeID(0);
			AutoType findTNode = mapLinkFTNode.find(nxLinkID);
			AutoType endTNode = mapLinkFTNode.end();
			if (findTNode != endTNode )
			{
				TLinkFTNodeID oNodeID = findTNode->second;
				nxNestNodeID = oNodeID.TToNodeID; 
			}

			if (false == bDuplication) 	{
				mapBeforeLine.insert(std::make_pair(nSEQ, nxLinkID));
			}
			else {
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
			int nStartSEQ = oResultInfo.nStartStation;
			int nEndSEQ	  = oResultInfo.nEndStation;
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
						AutoType iterfind = mapLinkTrip.find(nxLinkID);
						AutoType iterend  = mapLinkTrip.end();
						if (iterfind == iterend)
						{
							mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + oResultInfo.dTrip;
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
						AutoType iterfind = mapLinkTrip.find(nxLinkID);
						AutoType iterend  = mapLinkTrip.end();
						if (iterfind == iterend)
						{
							mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + oResultInfo.dTrip;
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
						AutoType iterfind = mapLinkTrip.find(nxLinkID);
						AutoType iterend  = mapLinkTrip.end();
						if (iterfind == iterend)
						{
							mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + oResultInfo.dTrip;
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
						AutoType iterfind = mapLinkTrip.find(nxLinkID);
						AutoType iterend  = mapLinkTrip.end();
						if (iterfind == iterend)
						{
							mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
						}
						else
						{
							double dExitValue = iterfind->second;
							dExitValue = dExitValue + oResultInfo.dTrip;
							iterfind->second = dExitValue;
						}
					}					

					iterAfter++;
				}
			}
		}
		else
		{
			AutoType iter  = mapTransitLinkSeq.find(oResultInfo.nxTransitID);
			AutoType itEnd = mapTransitLinkSeq.end();

			if(iter == itEnd)
				return;

			std::vector<Integer> &vecTransitLinks = iter->second;
			size_t nSize = vecTransitLinks.size();
			for (size_t i= 0; i< nSize; i++)
			{
				int     nSeq		= (i+1);
				Integer nxLinkID	= vecTransitLinks[i];

				int     nDLLSEQ     = nSeq + 1;

				if (nDLLSEQ == oResultInfo.nStartStation )
				{
					bUseSEQ = true;
				}
				else if (oResultInfo.nStartStation == 1 && nSeq == 1)
				{
					bUseSEQ = true;
					AutoType iterfind = mapLinkTrip.find(nxLinkID);
					AutoType iterend  = mapLinkTrip.end();
					if (iterfind == iterend)
					{
						mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}
					if (nDLLSEQ == oResultInfo.nEndStation)
					{
						bUseSEQ = false;
					}
				}
				else if (true == bUseSEQ)
				{
					AutoType iterfind = mapLinkTrip.find(nxLinkID);
					AutoType iterend  = mapLinkTrip.end();
					if (iterfind == iterend)
					{
						mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}

					if (nDLLSEQ == oResultInfo.nEndStation)
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
					AutoType iterfind = mapLinkTrip.find(nxLinkID);
					AutoType iterend  = mapLinkTrip.end();
					if (iterfind == iterend)
					{
						mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
					}
					else
					{
						double dExitValue = iterfind->second;
						dExitValue = dExitValue + oResultInfo.dTrip;
						iterfind->second = dExitValue;
					}

					if (nDLLSEQ == oResultInfo.nEndStation)
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

/* DllTransitSeqToLinkTrip로 통합....
void KMultiModalResultInfo::NewGetTransitInfo(std::map<Integer, std::vector<Integer>> &mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, double>& mapLinkTrip )
{
    try
    {
        bool bUseSEQ = false; 

        auto iter  = mapTransitLinkSeq.find(oResultInfo.nxTransitID);
        if(iter == mapTransitLinkSeq.end())
            return;

        //★ Transit을 구성하는 링크 정보
        std::vector<Integer>& vecTransitLinks = iter->second;

        size_t nSize = vecTransitLinks.size();
        for (size_t i= 0; i< nSize; i++)
        {
            int     nSeq		= (i+1);
            Integer nxLinkID	= vecTransitLinks[i];

            int     nDLLSEQ     = nSeq + 1;

            if (nDLLSEQ == oResultInfo.nStartStation ) {
                bUseSEQ = true;
            }
            else if (oResultInfo.nStartStation == 1 && nSeq == 1)
            {
                bUseSEQ = true;
                AutoType iterfind = mapLinkTrip.find(nxLinkID);
                AutoType iterend  = mapLinkTrip.end();
                if (iterfind == iterend)
                {
                    mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
                }
                else
                {
                    double dExitValue = iterfind->second;
                    dExitValue = dExitValue + oResultInfo.dTrip;
                    iterfind->second = dExitValue;
                }
                if (nDLLSEQ == oResultInfo.nEndStation)
                {
                    bUseSEQ = false;
                }
            }
            else if (true == bUseSEQ)
            {
                AutoType iterfind = mapLinkTrip.find(nxLinkID);
                AutoType iterend  = mapLinkTrip.end();
                if (iterfind == iterend)
                {
                    mapLinkTrip.insert(std::make_pair(nxLinkID,oResultInfo.dTrip));
                }
                else
                {
                    double dExitValue = iterfind->second;
                    dExitValue = dExitValue + oResultInfo.dTrip;
                    iterfind->second = dExitValue;
                }

                if (nDLLSEQ == oResultInfo.nEndStation)
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

void KMultiModalResultInfo::GetStatoinIDFromSEQ( KTarget* pTarget, int a_nSeq, Integer a_nxLineID, Integer& a_nxStationID )
{
	KDBaseConPtr spDBaseConnection = pTarget->GetDBaseConnection();

	try
	{
		int nSEQ = a_nSeq - 1;
		
		CString strSQL (_T(""));
		if (nSEQ == 0 )
		{
			strSQL.Format(_T(" select tB.fnode_id from %s tA, %s tB where tA.transit_id = %I64d and tA.seq = 1 and tA.link_id = tB.link_id "), TABLE_TRANSITLINKS, TABLE_LINK, a_nxLineID); //★ 필요
		}
		else
		{
			strSQL.Format(_T(" select tB.tnode_id from %s tA, %s tB where tA.transit_id = %I64d and tA.seq = %d and tA.link_id = tB.link_id "), TABLE_TRANSITLINKS, TABLE_LINK, a_nxLineID, nSEQ); //★ 필요
		}
		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
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


void KMultiModalResultInfo::NewGetStatoinIDFromSEQ( std::map<Integer, std::vector<Integer>> &a_mapLineSeq, std::map<Integer, TLinkFTNodeID> &a_mapLinkFTNode, int a_nSeq, Integer a_nxTransitID, Integer& a_nxStationID )
{
	try
	{
		auto iter  = a_mapLineSeq.find(a_nxTransitID);
        if (iter == a_mapLineSeq.end())
            return;

		int nSEQ = a_nSeq - 1;
        std::vector<Integer> &vecTransitLinks = iter->second;
        int nSize = (int)vecTransitLinks.size();

        if (nSEQ == 0 ) {
            int nArrIdx = 0;
            if(nArrIdx >= nSize)
                throw 1;

            Integer &nxLinkID = vecTransitLinks[nArrIdx];

            auto iterLink  = a_mapLinkFTNode.find(nxLinkID);
            if(iterLink != a_mapLinkFTNode.end()) {
                TLinkFTNodeID &oTLinkFTNodeID = iterLink->second; {
                    //★ TFromNodeID
                    a_nxStationID = oTLinkFTNodeID.TFromNodeID;
                }
                
            }
        }
        else
        {
            int nArrIdx = nSEQ-1;

            if(nArrIdx >= nSize)
                throw 1;

            Integer& nxLinkID = vecTransitLinks[nArrIdx];

            auto iterLink  = a_mapLinkFTNode.find(nxLinkID);
            if(iterLink != a_mapLinkFTNode.end())
            {
                TLinkFTNodeID &oTLinkFTNodeID = iterLink->second; {
                    //★ TToNodeID
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


void KMultiModalResultInfo::ResultTransitEditInfo(KTarget* pTarget, std::vector<KMTransitEditInfo> &a_vecTransitEditInfo, TMMapResultValue& a_oMapResult)
{
	COLORREF clrBus     = RGB (   0, 128,   0 );
	COLORREF clrRail    = RGB (   0,   0, 255 );
	COLORREF clrWalk    = RGB ( 102, 102, 153 );
	double   dWidth     = 3.0;

#pragma region 저장소
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;

	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
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
		KMTransitEditInfo &oBusAssignInfo = a_vecTransitEditInfo[i];

		int				nLastSation	    = 0;
		Integer			nxLastTransitID = 0;
		KEMPathType		emLastPathType = KEMPathTypeNull;

		Integer			nxOZone = oBusAssignInfo.nxOrginID;
		Integer			nxDZone = oBusAssignInfo.nxDestinationID;

		setOriginZoneID.insert(nxOZone);
		setDestinationZoneID.insert(nxDZone);

		std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
		size_t nPathCount = vecTMResultBusInfo.size();
		for (size_t nPath = 0; nPath < nPathCount; nPath++)
		{

			std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
			size_t nCount = vecTResult.size();
			for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
			{
				TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

				Integer nxStartID(0);
				Integer nxEndID(0);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
				setStaionID.insert(nxStartID);
				setStaionID.insert(nxEndID);

				if (0 == nPath && 0 == nIndex )
				{
					//OZone 부터 첫번째 정류장까지의 Walk
					Integer nxStationID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
					setAccessNodeStation.insert(nxStationID);
				}
				if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
				{
					//버스에서 버스로 환승시
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
				{
					//버스에서 전철 또는 전철 에서 버스
					Integer nxFirstID;
					Integer nxLastID;
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
					NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
					KODKey oTransWalk(nxLastID, nxFirstID);
					mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, 1));
				}
				else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
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

				if (oTMResultBusInfo.emPathType == KEMPathTypeBus)
				{
                    //★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
					DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
				}
				else
				{
					if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation)
					{
                        //★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
						DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
					}
					else {
                        //★ GetReverseTransitInfo 함수를 변경
                        DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
					}
				}

				emLastPathType	= oTMResultBusInfo.emPathType;
				nLastSation	    = oTMResultBusInfo.nEndStation;
				nxLastTransitID = oTMResultBusInfo.nxTransitID;
			}
		}

		//마지막 정류장 부터 DZone까지의 Walk값
		Integer nxStationID(0);

		/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
		NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
		KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
		AutoType iterfind = mapWalkingTrip.find(oLastWalk);
		AutoType iterend  = mapWalkingTrip.end();

		mapWalkingTrip.insert(std::make_pair(oLastWalk, 1));

		setAccessNodeStation.insert(nxStationID);
	}
#pragma endregion 라인정보담기

#pragma region Map에 맞게 데이터 정리

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
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterOZone;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterOZone);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(255,0,0);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
		++iterOZone;
	}

	//D존
	AutoType iterDZone = setDestinationZoneID.begin();
	AutoType endDZone  = setDestinationZoneID.end();
	while(iterDZone != endDZone)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterDZone;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterDZone);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(0,0,205);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
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

	// 버스에서 버스로 환승 도보
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
#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}


void KMultiModalResultInfo::ResultTransitEditInfo2(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<KMTransitEditInfo>*>> &a_mapDestMAssignInfo, OUT std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue)
{
	std::map<Integer, std::vector<Integer>> mapLineSeq;
	std::map<Integer, TLinkFTNodeID>        mapLinkFTNode;
	mapLineSeq.clear();
	KBulkDBaseLink::AllTrasitLineSeq(pTarget, mapLineSeq );
	mapLinkFTNode.clear();
	KBulkDBaseLink::AllLinkFTNode   (pTarget, mapLinkFTNode);

	auto iterMain  = a_mapDestMAssignInfo.begin();
	auto itEndMain = a_mapDestMAssignInfo.end();
	
	for (; iterMain != itEndMain; ++iterMain) {
		const Integer                                     &nxDZoneID          = iterMain->first;
		std::map<int, std::vector<KMTransitEditInfo>*>    &mapModeMAssignInfo = iterMain->second;
		// 결과 insert 
		std::map<int, std::vector<TMMapResultInputValue>> &mapModeResult      = a_mapDestResultInputValue[nxDZoneID];

		auto iterMode  = mapModeMAssignInfo.begin();
		auto itEndMode = mapModeMAssignInfo.end();

		for (; iterMode != itEndMode; ++iterMode) {
			const int                          &nMode               = iterMode->first;
			std::vector<KMTransitEditInfo>*    pvecAssignInfo       = iterMode->second;
			// 결과 insert 
			std::vector<TMMapResultInputValue> &vecResultInputValue = mapModeResult[nMode];
#pragma region 라인정보담기
			size_t nAssignCount = (*pvecAssignInfo).size();
			for (size_t i = 0; i < nAssignCount; i++)
			{
				KMTransitEditInfo &oBusAssignInfo = (*pvecAssignInfo)[i];
				
				TMMapResultInputValue oTMMapResultInputValue;
#pragma region 저장소
				std::map<KODKey, double>	&mapWalkingTrip            = oTMMapResultInputValue.mapWalkingTrip;
				std::map<KODKey, double>	&mapTransferWalkingB2BTrip = oTMMapResultInputValue.mapTransferWalkingB2BTrip;
				std::map<KODKey, double>	&mapTransferWalkingB2STrip = oTMMapResultInputValue.mapTransferWalkingB2STrip;
				std::map<KODKey, double>	&mapTransferWalkingS2STrip = oTMMapResultInputValue.mapTransferWalkingS2STrip;
				std::map<Integer, double>	&mapBusLinkTrip            = oTMMapResultInputValue.mapBusLinkTrip;
				std::map<Integer, double>	&mapSubwayLinkTrip         = oTMMapResultInputValue.mapSubwayLinkTrip;
				std::set<Integer>			&setOriginZoneID           = oTMMapResultInputValue.setOriginZoneID;
				std::set<Integer>			&setDestinationZoneID      = oTMMapResultInputValue.setDestinationZoneID;
				std::set<Integer>			&setStaionID               = oTMMapResultInputValue.setStaionID;
				std::set<Integer>           &setAccessNodeStation      = oTMMapResultInputValue.setAccessNodeStation;
#pragma endregion 저장소

				int				nLastSation	    = 0;
				Integer			nxLastTransitID = 0;
				KEMPathType		emLastPathType = KEMPathTypeNull;

				Integer			nxOZone = oBusAssignInfo.nxOrginID;
				Integer			nxDZone = oBusAssignInfo.nxDestinationID;

				setOriginZoneID.insert(nxOZone);
				setDestinationZoneID.insert(nxDZone);

				std::vector<TIndexResultTransitInfo> vecTMResultBusInfo = oBusAssignInfo.vecResultBusInfo;
				size_t nPathCount = vecTMResultBusInfo.size();
				for (size_t nPath = 0; nPath < nPathCount; nPath++)
				{
					std::vector<TMResultBusInfo> &vecTResult = vecTMResultBusInfo[nPath].vecLowResultTransitInfo;
					size_t nCount = vecTResult.size();
					for (size_t nIndex = 0; nIndex < nCount; nIndex++ )
					{
						TMResultBusInfo &oTMResultBusInfo = vecTResult[nIndex]; 

						Integer nxStartID(0);
						Integer nxEndID(0);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStartID);
						NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nEndStation, oTMResultBusInfo.nxTransitID, nxEndID);
						setStaionID.insert(nxStartID);
						setStaionID.insert(nxEndID);

						if (0 == nPath && 0 == nIndex )
						{
							//OZone 부터 첫번째 정류장까지의 Walk
							Integer nxStationID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxStationID);
							setAccessNodeStation.insert(nxStationID);
						}
						if (oTMResultBusInfo.emPathType == KEMPathTypeBus && emLastPathType == KEMPathTypeBus && 0 == nIndex)
						{
							//버스에서 버스로 환승시
							Integer nxFirstID;
							Integer nxLastID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
							KODKey oTransWalk(nxLastID, nxFirstID);
							mapTransferWalkingB2BTrip.insert(std::make_pair(oTransWalk, 1));
						}
						else if (oTMResultBusInfo.emPathType != emLastPathType && emLastPathType != KEMPathTypeNull && 0 == nIndex)
						{
							//버스에서 전철 또는 전철 에서 버스
							Integer nxFirstID;
							Integer nxLastID;
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, oTMResultBusInfo.nStartStation, oTMResultBusInfo.nxTransitID, nxFirstID);
							NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxLastID);
							KODKey oTransWalk(nxLastID, nxFirstID);
							mapTransferWalkingB2STrip.insert(std::make_pair(oTransWalk, 1));
						}
						else if(oTMResultBusInfo.emPathType == KEMPathTypeRail && emLastPathType == KEMPathTypeRail && 0 == nIndex )
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

						if (oTMResultBusInfo.emPathType == KEMPathTypeBus)
						{
                            //★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
							DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapBusLinkTrip);
						}
						else
						{
							if (oTMResultBusInfo.nStartStation < oTMResultBusInfo.nEndStation)
							{
                                //★ NewGetTransitInfo를 DllTransitSeqToLinkTrip로 통합
								DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip);
							}
							else
							{
                                //★ GetReverseTransitInfo 함수를 변경
								DllTransitSeqToLinkTrip(mapLineSeq, oTMResultBusInfo, mapSubwayLinkTrip/*, mapLinkFTNode*/);
							}
						}

						emLastPathType	= oTMResultBusInfo.emPathType;
						nLastSation	    = oTMResultBusInfo.nEndStation;
						nxLastTransitID = oTMResultBusInfo.nxTransitID;
					}
				}

				//마지막 정류장 부터 DZone까지의 Walk값
				Integer nxStationID(0);

				/*GetStatoinIDFromSEQ(pTarget, nLastSation, nxLastTransitID, nxStationID);*/
				NewGetStatoinIDFromSEQ(mapLineSeq, mapLinkFTNode, nLastSation, nxLastTransitID, nxStationID);
				KODKey oLastWalk(nxStationID, oBusAssignInfo.nxDestinationID);
				AutoType iterfind = mapWalkingTrip.find(oLastWalk);
				AutoType iterend  = mapWalkingTrip.end();

				mapWalkingTrip.insert(std::make_pair(oLastWalk, 1));

				setAccessNodeStation.insert(nxStationID);

				vecResultInputValue.push_back(oTMMapResultInputValue);
			}
#pragma endregion 라인정보담기
		}
	}
}


void KMultiModalResultInfo::ResultTransitEditInfo3(KTarget* pTarget, std::map<Integer, std::map<int, std::vector<TMMapResultInputValue>>> &a_mapDestResultInputValue, std::map<Integer, std::map<int, std::set<int>>> &a_mapZoneModePIndex, OUT TMMapResultValue& a_oMapResult)
{
	COLORREF clrBus     = RGB (   0, 128,   0 );
	COLORREF clrRail    = RGB (   0,   0, 255 );
	COLORREF clrWalk    = RGB ( 102, 102, 153 );
	double   dWidth     = 3.0;

#pragma region 저장소
	std::map<KODKey, double>	mapWalkingTrip;
	std::map<KODKey, double>	mapTransferWalkingB2BTrip;
	std::map<KODKey, double>	mapTransferWalkingB2STrip;
	std::map<KODKey, double>	mapTransferWalkingS2STrip;
	std::map<Integer, double>	mapBusLinkTrip;
	std::map<Integer, double>	mapSubwayLinkTrip;
	std::set<Integer>			setOriginZoneID;
	std::set<Integer>			setDestinationZoneID;
	std::set<Integer>			setStaionID;
	std::set<Integer>			setBusSationID;
	std::set<Integer>			setSubwayStaionID;
	std::vector<TDrawNodeInfo>  vecZoneNodeInfo;
	std::vector<TDrawNodeInfo>  vecNodeInfo;
	std::vector<TDrawLinkInfo>  vecLinkInfo;
	std::vector<TUserLineInfo>	vecUserLineInfo;

	std::set<Integer>           setAccessNodeStation;
#pragma endregion 저장소

	auto iterMain  = a_mapZoneModePIndex.begin();
	auto itEndMain = a_mapZoneModePIndex.end();
	for (; iterMain != itEndMain; ++iterMain) {
		const Integer                &nxDZoneID     = iterMain->first;
		std::map<int, std::set<int>> &mapModePIndex = iterMain->second;

		auto itFindZone = a_mapDestResultInputValue.find(nxDZoneID);
		if (itFindZone == a_mapDestResultInputValue.end()) {
			continue;
		}
		std::map<int, std::vector<TMMapResultInputValue>> &mapModeInputValue = itFindZone->second;

		auto iterMode  = mapModePIndex.begin();
		auto itEndMode = mapModePIndex.end();
		for (; iterMode != itEndMode; ++iterMode) {
			const int     &nMode     = iterMode->first;
			std::set<int> &setPIndex = iterMode->second;

			auto itFind = mapModeInputValue.find(nMode);
			if (itFind == mapModeInputValue.end()) {
				continue;
			}

			std::vector<TMMapResultInputValue> &vecInputValue = itFind->second;
			for (int i= 0; i< (int)vecInputValue.size(); i++) {
				auto itFind = setPIndex.find(i);
				if (itFind == setPIndex.end()) {
					continue;
				}

				TMMapResultInputValue &oTMMapResultInputValue = vecInputValue[i];

				mapBusLinkTrip.insert(oTMMapResultInputValue.mapBusLinkTrip.begin(), oTMMapResultInputValue.mapBusLinkTrip.end());
				mapSubwayLinkTrip.insert(oTMMapResultInputValue.mapSubwayLinkTrip.begin(), oTMMapResultInputValue.mapSubwayLinkTrip.end());
				setOriginZoneID.insert(oTMMapResultInputValue.setOriginZoneID.begin(), oTMMapResultInputValue.setOriginZoneID.end());
				setDestinationZoneID.insert(oTMMapResultInputValue.setDestinationZoneID.begin(), oTMMapResultInputValue.setDestinationZoneID.end());
				setAccessNodeStation.insert(oTMMapResultInputValue.setAccessNodeStation.begin(), oTMMapResultInputValue.setAccessNodeStation.end());
				setStaionID.insert(oTMMapResultInputValue.setStaionID.begin(), oTMMapResultInputValue.setStaionID.end());
				mapTransferWalkingB2BTrip.insert(oTMMapResultInputValue.mapTransferWalkingB2BTrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2BTrip.end());
				mapTransferWalkingB2STrip.insert(oTMMapResultInputValue.mapTransferWalkingB2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingB2STrip.end());
				mapTransferWalkingS2STrip.insert(oTMMapResultInputValue.mapTransferWalkingS2STrip.begin(), oTMMapResultInputValue.mapTransferWalkingS2STrip.end());
			}
		}
	}


#pragma region Map에 맞게 데이터 정리

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
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterOZone;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterOZone);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(255,0,0);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
		++iterOZone;
	}

	//D존
	AutoType iterDZone = setDestinationZoneID.begin();
	AutoType endDZone  = setDestinationZoneID.end();
	while(iterDZone != endDZone)
	{
		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID = *iterDZone;

		CString strNodeLable(_T(""));
		strNodeLable.Format(_T("%I64d"), *iterDZone);
		oNodeInfo.TLabel = strNodeLable;

		//임시 사이즈
		oNodeInfo.TSize = 18.0;

		oNodeInfo.TColorRGB = RGB(0,0,205);
		//임시 모양
		oNodeInfo.TSymbol = 48;

		vecZoneNodeInfo.push_back(oNodeInfo);
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

	// 버스에서 버스로 환승 도보
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
#pragma endregion Map에 맞게 데이터 정리

	a_oMapResult.vecDrawLinkInfo	= vecLinkInfo;
	a_oMapResult.vecDrawNodeInfo	= vecNodeInfo;
	a_oMapResult.vecDrawZoneNodeInfo= vecZoneNodeInfo;
	a_oMapResult.vecUserLineInfo	= vecUserLineInfo;
}

void KMultiModalResultInfo::GetStationODSQLData( KTarget* a_pTarget, Integer a_nxTransitID, std::map<int, TStationODInfo>& a_mapStationODData )
{
	a_mapStationODData.clear();

	try
	{
		std::vector<TTransitLinkInfo> vecTransitLinkInfo;
		KBulkDBaseTransit::TransitLinkInfo(a_pTarget, a_nxTransitID, vecTransitLinkInfo);

		for (size_t i = 0; i < vecTransitLinkInfo.size(); i++)
		{
			TTransitLinkInfo oTransitInfo = vecTransitLinkInfo[i];

			CString strSQL(_T(""));
			strSQL.Format(_T("select fnode_id, tnode_id, (select name from %s where node_id = fnode_id), (select name from %s where node_id = tnode_id) from %s where link_id = %I64d")
				, TABLE_NODE, TABLE_NODE, TABLE_LINK, oTransitInfo.nxLinkID);

			KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();

			KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
			if (spResultset->Next())
			{
				Integer nxFnode			= spResultset->GetValueInt64(0);
				Integer nxTNode			= spResultset->GetValueInt64(1);
				CString	strFnodeName	= spResultset->GetValueString(2);
				CString strTnodeName	= spResultset->GetValueString(3);

				if (oTransitInfo.nSeq == 1)
				{
					TStationODInfo oFInfo;
					oFInfo.TNodeID		= nxFnode;
					oFInfo.TNodeName	= strFnodeName;
					oFInfo.TStationYN	= 1;
					
					a_mapStationODData.insert(std::make_pair(1, oFInfo));
				}

				TStationODInfo oInfo; {
                    oInfo.TNodeID	 = nxTNode;
                    oInfo.TNodeName	 = strTnodeName;
                    oInfo.TStationYN = oTransitInfo.nStaionYN;
                }				

				int nSeq = oTransitInfo.nSeq + 1;
				a_mapStationODData.insert(std::make_pair(nSeq, oInfo));
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

void KMultiModalResultInfo::DllTransitSeqToLinkClr( std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr )
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
            TPathColor& oInfo = mapLinkPathClr[nxLinkID]; {
                oInfo.clrSect = oResultInfo.clrSect;
            }
        }

        return; //★ 종료한다.
    }

    for (int i = nIdxFromSeq; i < (int)vecTransitLink.size(); i++) {
        __int64 nxLinkID = vecTransitLink[i];
        TPathColor& oInfo = mapLinkPathClr[nxLinkID]; {
            oInfo.clrSect = oResultInfo.clrSect;
        }
    }

    for (int i = 0; i < nIdxToSeq; i++) {
        __int64 nxLinkID = vecTransitLink[i];
        TPathColor& oInfo = mapLinkPathClr[nxLinkID]; {
            oInfo.clrSect = oResultInfo.clrSect;
        }
    }
}



void KMultiModalResultInfo::DllTransitSeqToLinkBusInfoPtr( std::map<Integer, std::vector<Integer>>& mapTransitLinkSeq, TMResultBusInfo &oResultInfo, std::map<Integer, TMResultBusInfo*>& mapLinkPathClr )
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

			mapLinkPathClr.insert(std::make_pair(nxLinkID, &oResultInfo));
		}

		return; //★ 종료한다.
	}

	for (int i = nIdxFromSeq; i < (int)vecTransitLink.size(); i++) {
		__int64 nxLinkID = vecTransitLink[i];

		mapLinkPathClr.insert(std::make_pair(nxLinkID, &oResultInfo));
	}

	for (int i = 0; i < nIdxToSeq; i++) {
		__int64 nxLinkID = vecTransitLink[i];
		mapLinkPathClr.insert(std::make_pair(nxLinkID, &oResultInfo));
	}
}


/*
void KMultiModalResultInfo::GetTransitShortestInfo(std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TMResultBusInfo a_oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr )
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
			else if (1 == a_oResultInfo.nStartStation && 1 == nSeq)
			{
				bUseSEQ = true;
				AutoType iterfind = mapLinkPathClr.find(nxLinkID);
				AutoType iterend  = mapLinkPathClr.end();
				if (iterfind == iterend)
				{
					TPathColor oTCostData;
					oTCostData.clrSect = a_oResultInfo.clrSect;

					mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
				}
			}
			else if (true == bUseSEQ)
			{
				AutoType iterfind = mapLinkPathClr.find(nxLinkID);
				AutoType iterend  = mapLinkPathClr.end();
				if (iterfind == iterend)
				{
					TPathColor oTCostData;
					oTCostData.clrSect = a_oResultInfo.clrSect;

					mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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

void KMultiModalResultInfo::GetReverseTransitShortestInfo( std::map<Integer, std::vector<Integer>> &a_mapLineSeq, TMResultBusInfo a_oResultInfo, std::map<Integer, TPathColor>& mapLinkPathClr, std::map<Integer, TLinkFTNodeID> a_mapFTNode)
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
						AutoType iterfind = mapLinkPathClr.find(nxLinkID);
						AutoType iterend  = mapLinkPathClr.end();
						if (iterfind == iterend)
						{
							TPathColor oTCostData;
							oTCostData.clrSect = a_oResultInfo.clrSect;

							mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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
						AutoType iterfind = mapLinkPathClr.find(nxLinkID);
						AutoType iterend  = mapLinkPathClr.end();
						if (iterfind == iterend)
						{
							TPathColor oTCostData;
							oTCostData.clrSect = a_oResultInfo.clrSect;

							mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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
						AutoType iterfind = mapLinkPathClr.find(nxLinkID);
						AutoType iterend  = mapLinkPathClr.end();
						if (iterfind == iterend)
						{
							TPathColor oTCostData;
							oTCostData.clrSect = a_oResultInfo.clrSect;

							mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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
						AutoType iterfind = mapLinkPathClr.find(nxLinkID);
						AutoType iterend  = mapLinkPathClr.end();
						if (iterfind == iterend)
						{
							TPathColor oTCostData;
							oTCostData.clrSect = a_oResultInfo.clrSect;

							mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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
					AutoType iterfind = mapLinkPathClr.find(nxLinkID);
					AutoType iterend  = mapLinkPathClr.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = a_oResultInfo.clrSect;

						mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
					}

					if (nDLLSEQ == a_oResultInfo.nEndStation)
					{
						bUseSEQ = false;
					}
				}
				else if (true == bUseSEQ)
				{
					AutoType iterfind = mapLinkPathClr.find(nxLinkID);
					AutoType iterend  = mapLinkPathClr.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = a_oResultInfo.clrSect;

						mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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
					AutoType iterfind = mapLinkPathClr.find(nxLinkID);
					AutoType iterend  = mapLinkPathClr.end();
					if (iterfind == iterend)
					{
						TPathColor oTCostData;
						oTCostData.clrSect = a_oResultInfo.clrSect;

						mapLinkPathClr.insert(std::make_pair(nxLinkID, oTCostData));
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