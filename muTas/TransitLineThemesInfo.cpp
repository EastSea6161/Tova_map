#include "StdAfx.h"
#include "TransitLineThemesInfo.h"
#include "Target.h"
#include "MultiModalResultInfo.h"
#include "BulkDBaseTransit.h"

KTransitLineThemesInfo::KTransitLineThemesInfo(void)
{
}


KTransitLineThemesInfo::~KTransitLineThemesInfo(void)
{
}

void KTransitLineThemesInfo::GetTransitLineInfo( KTarget* a_pTarget, TTransitLineThemesInfo a_oLineThemesInfo )
{

}
 
void KTransitLineThemesInfo::GetPassengerInfo( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo )
{
	a_vecDrawLinkInfo.clear();

	try
	{
		std::vector<Integer>& vecTransitID  = a_oLineThemesInfo.vecTransitID;
		size_t nxCount = vecTransitID.size();
		if (nxCount < 1)
			return;

        std::map<__int64, double> mapLinkPerson;
        KBulkDBaseTransit::TransitLinksPerson(a_pTarget, mapFTNodeLinkID, a_oLineThemesInfo.strPassenger, vecTransitID, mapLinkPerson);
		
        for (auto iter = mapLinkPerson.begin(); iter != mapLinkPerson.end(); ++iter)
		{
			TDrawLinkInfo oLinkInfo;
			Integer nxlinkid	= iter->first;
			double  dPassenger	= iter->second;

			CString strLabel(_T("")); {
                strLabel.Format(_T("%.3f"),dPassenger);
            }
			oLinkInfo.TLinkID = nxlinkid;
			oLinkInfo.TLabel  = strLabel;
            oLinkInfo.TAlpha  = a_oLineThemesInfo.TPassengerAlpha;

			if (dPassenger > a_oLineThemesInfo.dMaxVolumeP)
			{
				if (false == a_oLineThemesInfo.bOutRangeP) {
					continue;
				}
				oLinkInfo.TColorRGB = a_oLineThemesInfo.clrOutRangeP;
				oLinkInfo.TWidth	= a_oLineThemesInfo.dMinSizeP;
			}
			else if (dPassenger < a_oLineThemesInfo.dMinVolumeP)
			{
				if (false == a_oLineThemesInfo.bOutRangeP)
				{
					continue;
				}
				oLinkInfo.TColorRGB = a_oLineThemesInfo.clrOutRangeP;
				oLinkInfo.TWidth	= a_oLineThemesInfo.dMinSizeP;
			}
			else
			{
				oLinkInfo.TColorRGB = a_oLineThemesInfo.clrLineP;
				double dWidth(0.0);
				CalculateValue2Width(dPassenger, a_oLineThemesInfo.dMaxVolumeP, a_oLineThemesInfo.dMinVolumeP, a_oLineThemesInfo.dMaxSizeP, a_oLineThemesInfo.dMinSizeP, dWidth);
				oLinkInfo.TWidth = dWidth;
			}

			a_vecDrawLinkInfo.push_back(oLinkInfo);
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

void KTransitLineThemesInfo::CalculateValue2Width( double a_dValue, double a_dMaxValue, double a_dMinValue, double a_dMaxSize, double a_dMinsize, double& a_dOutWidth )
{
	if (a_dMaxValue == a_dMinValue) {
		a_dOutWidth = a_dMinsize;
	}
	else {
		a_dOutWidth = (a_dMaxSize - a_dMinsize) / (a_dMaxValue - a_dMinValue) * (a_dValue - a_dMinValue) + a_dMinsize;
	}

	if (a_dOutWidth > a_dMaxSize) {
		a_dOutWidth = a_dMaxSize;
	}
	
	if (a_dOutWidth < a_dMinsize) {
		a_dOutWidth = a_dMinsize;
	}
}


void KTransitLineThemesInfo::GetBoardingInfo( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TTransitPassengerInfo>& a_vecPassengerInfo )
{
    a_vecPassengerInfo.clear();
	std::map<Integer, TTransitPassengerInfo> mapPassengerInfo;

    try
    {
		AutoType &vecTransitID = a_oLineThemesInfo.vecTransitID;
		size_t    nxCount      = vecTransitID.size();
		if (nxCount < 1)
			return;

        std::map<Integer, TLinkFTNodeID> mapLinkFTNode; {
            KBulkDBaseLink::AllLinkFTNode(a_pTarget, mapLinkFTNode);
        }

        std::map<__int64, double> mapLinkBoard; {
            KBulkDBaseTransit::TransitLinkBoard(a_pTarget, mapFTNodeLinkID, a_oLineThemesInfo.strBoard, vecTransitID, mapLinkBoard);
        }        

        for (auto iter = mapLinkBoard.begin(); iter != mapLinkBoard.end(); ++iter) {
            __int64 nxID      = iter->first;
            double  dBoarding = iter->second;

            if (dBoarding <= 0)
                continue;

            TTransitPassengerInfo oInfo; {                
                auto iFind = mapLinkFTNode.find(nxID);
                if (iFind == mapLinkFTNode.end())
                    continue;

                TLinkFTNodeID& oLinkFTNode = iFind->second;

                oInfo.TLinkID        = nxID;
                oInfo.TDrawFromNode  = true;
                oInfo.TDrawToNode    = false;
                oInfo.TFromNodeID    = oLinkFTNode.TFromNodeID;
                oInfo.TFromTrip      = dBoarding;
                CString& strBorading = oInfo.TFromLabel; {
                    strBorading.Format(_T("%.2f"), oInfo.TFromTrip);
                }
            }            

            mapPassengerInfo.insert(std::make_pair(nxID, oInfo));
        }
        
		AutoType iter  = mapPassengerInfo.begin();
		AutoType itEnd = mapPassengerInfo.end();

		while (iter != itEnd)
		{
			TTransitPassengerInfo &oPassengerInfo = iter->second;
			double &dUseBoarding = oPassengerInfo.TFromTrip;

			if (dUseBoarding > a_oLineThemesInfo.dMaxVolumeBA)
			{
				if (false == a_oLineThemesInfo.bOutRangeBA)
				{
					++iter;
					continue;
				}
				oPassengerInfo.TFromColor	= a_oLineThemesInfo.clrOutRangeBA;
				oPassengerInfo.TFromSize	= a_oLineThemesInfo.dMinSizeBA;
			}
			else if (dUseBoarding < a_oLineThemesInfo.dMinVolumeBA)
			{
				if (false == a_oLineThemesInfo.bOutRangeBA)
				{
					++iter;
					continue;
				}
				oPassengerInfo.TFromColor	= a_oLineThemesInfo.clrOutRangeBA;
				oPassengerInfo.TFromSize	= a_oLineThemesInfo.dMinSizeBA;
			}
			else
			{
				oPassengerInfo.TFromColor = a_oLineThemesInfo.clrLineBA;
				double dWidth(0.0);
				CalculateValue2Width(dUseBoarding, a_oLineThemesInfo.dMaxVolumeBA, a_oLineThemesInfo.dMinVolumeBA, a_oLineThemesInfo.dMaxSizeBA, a_oLineThemesInfo.dMinSizeBA, dWidth);
				oPassengerInfo.TFromSize = dWidth;
			}

			a_vecPassengerInfo.push_back(oPassengerInfo);

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


void KTransitLineThemesInfo::GetAlightingInfo( KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TTransitPassengerInfo>& a_vecPassengerInfo )
{
	a_vecPassengerInfo.clear();
	std::map<Integer, TTransitPassengerInfo> mapPassengerInfo;

	try
	{
		AutoType &vecTransitID = a_oLineThemesInfo.vecTransitID;
		size_t    nxCount      = vecTransitID.size();
		if (nxCount < 1)
			return;

        std::map<Integer, TLinkFTNodeID> mapLinkFTNode; {
            KBulkDBaseLink::AllLinkFTNode(a_pTarget, mapLinkFTNode);
        }

        std::map<__int64, double> mapLinkAlight; {
            KBulkDBaseTransit::TransitLinkAlight(a_pTarget, mapFTNodeLinkID, a_oLineThemesInfo.strAlight, vecTransitID, mapLinkAlight);
        }        

        for (auto iter = mapLinkAlight.begin(); iter != mapLinkAlight.end(); ++iter) {
            __int64 nxID    = iter->first;
            double  dAlight = iter->second;

            if (dAlight <= 0)
                continue;

            TTransitPassengerInfo oInfo; {                
                auto iFind = mapLinkFTNode.find(nxID);
                if (iFind == mapLinkFTNode.end())
                    continue;

                TLinkFTNodeID& oLinkFTNode = iFind->second;

                oInfo.TLinkID        = nxID;
                oInfo.TDrawFromNode  = false;
                oInfo.TDrawToNode    = true;
                oInfo.TToNodeID      = oLinkFTNode.TToNodeID;
                oInfo.TToTrip        = dAlight;

                CString& strLabel = oInfo.TToLabel; {
                    strLabel.Format(_T("%.2f"), oInfo.TToTrip);
                }
            }            

            mapPassengerInfo.insert(std::make_pair(nxID, oInfo));
        }

		AutoType iter  = mapPassengerInfo.begin();
		AutoType itEnd = mapPassengerInfo.end();

		while (iter != itEnd)
		{
			TTransitPassengerInfo &oPassengerInfo = iter->second;
			double &dUseAlighting = oPassengerInfo.TToTrip;

			if (dUseAlighting > a_oLineThemesInfo.dMaxVolumeBA)
			{
				if (false == a_oLineThemesInfo.bOutRangeBA)
				{
					++iter;
					continue;
				}
				oPassengerInfo.TToColor	= a_oLineThemesInfo.clrOutRangeBA;
				oPassengerInfo.TToSize	= a_oLineThemesInfo.dMinSizeBA;
			}
			else if (dUseAlighting < a_oLineThemesInfo.dMinVolumeBA)
			{
				if (false == a_oLineThemesInfo.bOutRangeBA)
				{
					++iter;
					continue;
				}
				oPassengerInfo.TToColor	= a_oLineThemesInfo.clrOutRangeBA;
				oPassengerInfo.TToSize	= a_oLineThemesInfo.dMinSizeBA;
			}
			else
			{
				oPassengerInfo.TToColor = a_oLineThemesInfo.clrLineBA;
				double dWidth(0.0);
				CalculateValue2Width(dUseAlighting, a_oLineThemesInfo.dMaxVolumeBA, a_oLineThemesInfo.dMinVolumeBA, a_oLineThemesInfo.dMaxSizeBA, a_oLineThemesInfo.dMinSizeBA, dWidth);
				oPassengerInfo.TToSize = dWidth;
			}

			a_vecPassengerInfo.push_back(oPassengerInfo);

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

void KTransitLineThemesInfo::GetSeatInfo( KTarget* a_pTarget,TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo )
{
	CString strSQL(_T(""));
	CString strTLineSQL(_T(""));

	try
	{
        //¡Ú ¸µÅ©, ÃÑÁÂ¼®¼ö
        std::map<__int64, double> mapLinkSeat;

		std::vector<Integer> &vecTransitID  = a_oLineThemesInfo.vecTransitID;
		for (size_t i = 0; i < vecTransitID.size(); i++)
		{
			KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection(); {
                strSQL.Format(_T("Select seat_count, total_frequency From Transit Where transit_ID = '%I64d'"), vecTransitID[i]);
            }
			
            double dSeatCount(0);
            double dTotalFreq(0);
            double dTotalSeat(0);

			KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
            if (spResultset->Next()) {
				dSeatCount = spResultset->GetValueDouble(0);
				dTotalFreq = spResultset->GetValueDouble(1);
			}

            //¡Ú 
			dTotalSeat = dSeatCount * dTotalFreq;
			strTLineSQL.Format(_T("Select link_id From %s Where transit_id = '%I64d'"), TABLE_TRANSITLINKS, vecTransitID[i]);
			
			spResultset = spDBaseConnection->ExecuteQuery(strTLineSQL);
			while(spResultset->Next()) {
                Integer nxLinkID = spResultset->GetValueInt64(0);
                double& dSeat = mapLinkSeat[nxLinkID]; {
                    dSeat += dTotalSeat;
                }
			}
		}

        for (auto iter = mapLinkSeat.begin(); iter != mapLinkSeat.end(); ++iter) {
            __int64 nxLinkID   = iter->first;
            double  dTotalSeat = iter->second;

            TDrawLinkInfo oLinkInfo; {
                oLinkInfo.TLinkID	= nxLinkID;
                oLinkInfo.TColorRGB	= a_oLineThemesInfo.clrSeatCapa;
                double dSize(0); {
                    CalculateValue2WidthBySeatLine(dTotalSeat, a_oLineThemesInfo.dMaxVolumeP, a_oLineThemesInfo.dMinVolumeP, a_oLineThemesInfo.dMaxSizeP, a_oLineThemesInfo.dMinSizeP, dSize);
                }

                oLinkInfo.TWidth		= dSize;
                oLinkInfo.TLabel		= _T("");
                oLinkInfo.TAlpha = a_oLineThemesInfo.TSeatCapaAlpha;
            }
            a_vecDrawLinkInfo.push_back(oLinkInfo);        
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

void KTransitLineThemesInfo::CalculateValue2WidthBySeatLine( double a_dValue, double a_dMaxValue, double a_dMinValue, double a_dMaxSize, double a_dMinsize, double& a_dOutWidth )
{
	try
	{
		if (a_dMaxValue == a_dMinValue) {
			a_dOutWidth = a_dMinsize;
		}
		else {
			a_dOutWidth = (a_dMaxSize - a_dMinsize) / (a_dMaxValue - a_dMinValue) * (a_dValue - a_dMinValue) + a_dMinsize;
		}

		if (a_dOutWidth < a_dMinsize) {
			a_dOutWidth = a_dMinsize;
		}        
		else if (a_dOutWidth > a_dMaxSize * 2)
		{
			a_dOutWidth = a_dMaxSize * 2;
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

void KTransitLineThemesInfo::GetCapacityInfo( KTarget* a_pTarget, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo )
{
	CString strSQL(_T(""));
	CString strTLineSQL(_T(""));

	try
	{
        std::map<__int64, double> mapLinkCapacity;

		std::vector<Integer> &vecTransitID  = a_oLineThemesInfo.vecTransitID;
		for (size_t i = 0; i < vecTransitID.size(); i++)
		{
			double dCapacity(0);
			double dTotalFreq(0);
			double dTotalCapa(0);

			KDBaseConPtr spDBaseConnection = a_pTarget->GetDBaseConnection();
			strSQL.Format(_T("Select capacity, total_frequency From Transit Where transit_ID = '%I64d'"), vecTransitID[i]);
			KResultSetPtr spResultset = spDBaseConnection->ExecuteQuery(strSQL);
			if (spResultset->Next())
			{
				dCapacity  = spResultset->GetValueDouble(0);
				dTotalFreq = spResultset->GetValueDouble(1);
			}

			dTotalCapa = dCapacity * dTotalFreq * (a_oLineThemesInfo.dCapaRatio / 100);
			strTLineSQL.Format(_T("Select link_id From %s Where transit_id = '%I64d'"), TABLE_TRANSITLINKS, vecTransitID[i]);

			spResultset = spDBaseConnection->ExecuteQuery(strTLineSQL);
			while(spResultset->Next())
			{
                Integer nxLinkID = spResultset->GetValueInt64(0);
                double& dCapa = mapLinkCapacity[nxLinkID]; {
                    dCapa += dTotalCapa;
                }
			}
		}

        for (auto iter = mapLinkCapacity.begin(); iter != mapLinkCapacity.end(); ++iter) {
            __int64 nxLinkID   = iter->first;
            double  dTotalCapa = iter->second;
        
            TDrawLinkInfo oLinkInfo; {
                oLinkInfo.TLinkID	= nxLinkID;
                oLinkInfo.TColorRGB	= a_oLineThemesInfo.clrSeatCapa;
                double dSize(0);
                CalculateValue2WidthBySeatLine(dTotalCapa, a_oLineThemesInfo.dMaxVolumeP, a_oLineThemesInfo.dMinVolumeP, a_oLineThemesInfo.dMaxSizeP, a_oLineThemesInfo.dMinSizeP, dSize);
                oLinkInfo.TWidth		= dSize;
                oLinkInfo.TLabel		= _T("");
                oLinkInfo.TAlpha = a_oLineThemesInfo.TSeatCapaAlpha;
            }
            a_vecDrawLinkInfo.push_back(oLinkInfo);
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
