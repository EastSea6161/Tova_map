#pragma once






#pragma once

#include "TDrawLinkInfo.h"
#include "TPathAnalysisInfo.h"

class KTarget;

struct TPathVolumeSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	COLORREF clrPath;
	COLORREF clrOutRange;
};


class KPathVolumeInfo
{
public:
	KPathVolumeInfo(void);
	~KPathVolumeInfo(void);
	
public:
	static void GetDrawBushVolumeInfo(std::map<Integer/*DZone*/, std::vector<TPathVolumeInfo>> &a_mapPathVolumeInfo, 
								      TPathVolumeSettingInfo     &a_oSettingInfo, 
								      std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo,
									  std::vector<TDrawLinkInfo> &a_vecDrawLinkInfoNoLabel);

	static void GetDrawGogumaVolumeInfo(std::vector<TPathVolumeInfo> &a_vecPathVolumeInfo, 
		                                TPathVolumeSettingInfo       &a_oSettingInfo, 
		                                std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfo,
										std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfoNoLabel);

	static void GetDrawPathVolumeInfo(std::vector<TPathVolumeInfo> &a_vecPathVolumeInfo, 
		                              std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfo,
									  std::vector<TDrawLinkInfo>   &a_vecDrawLinkInfoNoLabel);

	static void GetDrawVolumeAreaInfo(std::map<Integer, double>  &a_mapLinkVolume, 
		                              TPathVolumeSettingInfo     &a_oSettingInfo, 
									  std::vector<TDrawLinkInfo> &a_vecDrawLinkInfo, 
									  std::vector<TDrawLinkInfo> &a_vecDrawLinkInfoNoLabel);

private:
	static void CalculateValue2Width(double a_dTrip, TPathVolumeSettingInfo &a_oSettingInfo, double& dWidth);
	
};