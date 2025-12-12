#pragma once
class KTarget;

struct TNetworkEditLog
{
	int     TLayer;
	Integer TID;
	int     TEditType;
	int		TCheckData;
};


class KNetworkEditLog
{
public:
	KNetworkEditLog(void);
	~KNetworkEditLog(void);

public:
	static void WriteNetworkEditLog(KTarget* a_pTarget, int a_nLayer, Integer a_nxID, int a_nEditType);
	static void NetworkEditLog(KTarget* a_pTarget, std::vector<TNetworkEditLog>& a_vecNetworkEditLog);
	static void NetworkEditLog(KTarget* a_pTarget, int a_nLayer, std::vector<TNetworkEditLog>& a_vecNetworkEditLog);

	static void SetImportNetworkLog(KTarget* a_pTarget);
	static void GetLastNetWorkLog(KTarget* a_pTarget, int& a_nCheckData);
	static void SetNetworkLogUpdateInspection(KTarget* a_pTarget);
public:
    static CTime GetLastEditTime(KTarget* pTarget);
};

