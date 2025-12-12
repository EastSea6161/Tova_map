#pragma once

class TTransitPassengerInfo
{
public:
    TTransitPassengerInfo() {};
    ~TTransitPassengerInfo(){};

public:
    Integer  TLinkID;

    bool     TDrawFromNode;
    Integer  TFromNodeID;    
    COLORREF TFromColor;
    double   TFromSize;
    CString  TFromLabel;
	double   TFromTrip;

    bool     TDrawToNode;
    Integer  TToNodeID;
    COLORREF TToColor;
    double   TToSize;
    CString  TToLabel;
	double   TToTrip;
};