#pragma once

class TMultiModalInfo
{
public:
	CString strDescript;
	CString strFieldBoard;
	CString strFieldAlight;
	CString strFieldReBoard;

	CString strFieldBoardNet;
	CString strFieldBoardTransfer;
	CString strFieldBoardTot;
	CString strFieldAlightNet;
	CString strFieldAlightTransfer;
	CString strFieldAlightTot;
	CString strFieldPassenger;
};

class TChartTransitInfo
{
public:
	Integer nxID;
	CString strName;
};

class TChartDetailLineInfo
{
public:
	Integer nxLinkID;
	Integer nxFNodeID;
	Integer nxTNodeID;
	int     nSeq;
	int     nStationYN;
};

struct TData 
{
	Integer nxLinkID;
	double  dBoard;
	double  dAlight;
	double  dReBoard;
    int     nSeq;
	double  dValue;
};

struct TMain
{
	CString TCaption;
	std::vector<TData> TvecData;
};

//¡Ú 
class TChartData {
public:
    TChartData() {
        TCaption = _T("");
    }
public:
    CString TCaption;
    std::vector<double> TData;
};

//¡Ú 
class TCharStnInfo {
public:
    TCharStnInfo() {
        TNodeID    = 0;
        TStationYN = 0;
    }
public:
    __int64 TNodeID;
    int     TStationYN;
};