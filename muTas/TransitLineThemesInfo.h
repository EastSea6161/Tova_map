#pragma once

#include "TDrawLinkInfo.h"
#include "TTransitPassengerInfo.h"

class KTarget;

struct TTransitLineThemesInfo
{
	std::vector<Integer> vecTransitID;

	CString   strPassenger;
	CString   strBoard;
	CString   strAlight;

	double    dMinVolumeP;
	double    dMaxVolumeP;
	double    dMinSizeP;
	double    dMaxSizeP;
	COLORREF  clrLineP;
	bool      bOutRangeP;
	COLORREF  clrOutRangeP;

	double    dMinVolumeBA;
	double    dMaxVolumeBA;
	double    dMinSizeBA;
	double    dMaxSizeBA;
	COLORREF  clrLineBA;
	bool      bOutRangeBA;
	COLORREF  clrOutRangeBA;

	double	  dCapaRatio;
	COLORREF  clrSeatCapa;

    int TPassengerAlpha;
    int TSeatCapaAlpha;
};


class KTransitLineThemesInfo
{
public:
	KTransitLineThemesInfo(void);
	~KTransitLineThemesInfo(void);

public:	
	static void GetTransitLineInfo(KTarget* a_pTarget, TTransitLineThemesInfo a_oLineThemesInfo);
	static void GetPassengerInfo(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo);
	static void GetBoardingInfo(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TTransitPassengerInfo>& a_vecPassengerInfo);
	static void GetAlightingInfo(KTarget* a_pTarget, std::map<KODKey, __int64>& mapFTNodeLinkID, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TTransitPassengerInfo>& a_vecPassengerInfo);
	
	static void GetSeatInfo(KTarget* a_pTarget, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo);
	static void GetCapacityInfo(KTarget* a_pTarget, TTransitLineThemesInfo a_oLineThemesInfo, std::vector<TDrawLinkInfo>& a_vecDrawLinkInfo);

private:

	static void CalculateValue2Width(double a_dValue, double a_dMaxValue, double a_dMinValue, double a_dMaxSize, double a_dMinsize, double& a_dOutWidth);
	static void CalculateValue2WidthBySeatLine(double a_dValue, double a_dMaxValue, double a_dMinValue, double a_dMaxSize, double a_dMinsize, double& a_dOutWidth);
};

