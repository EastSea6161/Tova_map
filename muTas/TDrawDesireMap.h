#pragma once

class TDesireOrigin
{
public:
	TDesireOrigin()
	{
		int m = 0;
	}
	~TDesireOrigin()
	{
	}
public:
	Integer TID;
	CString TName;
	double  TMapX;
	double  TMapY;
};

typedef std::shared_ptr<TDesireOrigin> TDesireOriginPtr;

class TDesireDest
{
public:
	Integer  TID;
	CString  TName;
	double   TMapX;
	double   TMapY;
	double   TValue;
	double   TWidth;
	COLORREF TRGB;
};

typedef std::shared_ptr<TDesireDest> TDesireDestPtr;

class TDesireMapData
{
public:
	TDesireOrigin TOrigin;
	std::vector<TDesireDest> TVecDestination;
};

class TDesireODInfo
{
public:
	CString  strTableName;
	CString  strFieldName;
};


struct TDesireSettingInfo
{
	bool   bUseOutRange;

	double dMaxTrip;
	double dMinTrip;
	double dMinSize;
	double dMaxSize;

	COLORREF clrPath;
	COLORREF clrOutRange;
};
