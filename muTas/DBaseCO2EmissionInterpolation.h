#pragma once

class KTarget;

class KDBaseCO2EmissionInterpolation
{
public:
	KDBaseCO2EmissionInterpolation(void);
	~KDBaseCO2EmissionInterpolation(void);

	static bool IsEmptyAssignResult(KTarget* a_pTarget);
	static void GetTravelTime(KTarget* a_pTarget, std::map<int, CString>& a_mapTravelTime);
	static void GetModeVolumeColumn(KTarget* a_pTarget, int a_nRunSeq, std::vector<CString>& a_vecModeVolColumn);

	static void GetLinkSpeed(KTarget* a_pTarget, int a_nIdxTravelTime, std::map<Integer, double>& a_mapLinkSpeed);
	static void GetLinkSpeed( KTarget* a_pTarget, KIOColumn* a_pLinkIOColumn, std::map<Integer, double>& a_mapLinkSpeed );
	static void GetLinkVolume(KTarget* a_pTarget, CString a_strColumnName, std::map<Integer, double>& a_mapLinkVolume);

	static void UpdateResult(KTarget* a_pTarget, CString a_strColumnName, std::map<Integer, double> a_mapResult);

	// 2019.01.17
	static void GetLinkLength(KTarget* a_pTarget, std::map<Integer, double>& a_mapLinkLength);
};

