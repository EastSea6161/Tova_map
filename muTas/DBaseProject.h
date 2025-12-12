#pragma once

class KTarget;
class KPurpose;
class KMode;

class KDBaseProject
{
public:
	KDBaseProject(void);
	~KDBaseProject(void);

	static void ModifyPurposeInfo      (KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose);
	static void GetDisusedPurposes     (KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose, std::vector<int>& ar_vecDisusedPurposeID);
	static void DelDisusedPurposeDefine(KDBaseConPtr a_spDBaseConnection, std::vector<int> a_vecDisusedPurposeID);
	static void SetModifiedPurpose     (KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> a_vecPurpose);

	static void ModifyModeInfo         (KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode);
	static void GetDisusedModes        (KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode, std::vector<int>& ar_vecDisusedModeID);
	static void DelDisusedModeDefine   (KDBaseConPtr a_spDBaseConnection, std::vector<int> a_vecDisusedModeID);
	static void SetModifiedMode        (KDBaseConPtr a_spDBaseConnection, std::vector<KMode*> a_vecMode);
};

