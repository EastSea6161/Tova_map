#pragma once

class KTarget;
class KIOColumns;

using namespace std;

struct TRunOptionInfo
{
	int     nRunOption;
	CString strAvgColumnName;
	int     nZoneThrough;
};

#pragma pack(push, 4) 

struct TSubNetworkNode
{
	Integer nxNodeID; 
	int     nNodeType;
	double  dX; 
	double  dY; 
	Integer nxZoneID; 
	int     nInSubField;
};

struct TSubNetworkLink
{
	Integer nxLinkID;
	Integer nxFNodeID;
	Integer nxTNodeID;
	int     nLinkType;
	double  dLength;
	int     nLane;
	double	dCapa;
	double  dSpeed;
	int     nFunctionID;
	double  dAvgTime;
	int     nInSubField;
};

#pragma pack(pop)

class KDBaseSubNetwork
{
public:
	KDBaseSubNetwork(void);
	~KDBaseSubNetwork(void);

public:
	static void GetDBModeInfo(
		KTarget* a_pTarget, map<int, CString>& ar_mapModeInfo);
	static void GetDBRunOptionInfo(
		KTarget* a_pTarget, TRunOptionInfo& oRunOptionInfo);
	static void ExportDB2NodeFile(
		KTarget* a_pTarget, set<Integer> a_setIncludeNode, map<Integer, int> a_mapNodeSubField, 
		CString a_strFileName=_T("node.dat"));
	static void ExportDB2LinkFile(
		KTarget* a_pTarget, set<Integer> a_setIncludeLink, map<Integer, int> a_mapLinkSubField,
		KIOColumns a_UserColumnCollection, CString a_strFileName=_T("link.dat"));
	
	static int  GetDBModeInfoTableRecords(KTarget* a_pTarget);
};

