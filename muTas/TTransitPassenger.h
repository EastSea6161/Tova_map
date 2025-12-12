#pragma once

class TTransitPassengerMinMax
{
public:
    TTransitPassengerMinMax() {
        strFieldName = _T("");
        dMinVol = 0;
        dMaxVol = 0;
        bPassenger  = false;
    }
public:
	CString strFieldName;
	double dMinVol;
	double dMaxVol;
    bool   bPassenger;
};