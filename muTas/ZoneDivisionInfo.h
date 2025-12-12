#pragma once

class KZoneDivisionInfo
{
public:
    KZoneDivisionInfo()
    {
    }

	KZoneDivisionInfo(double a_dRate,  CString a_strZoneName = _T(""))
	{
		TRate     = a_dRate;
		TZoneName = a_strZoneName;
	}

    ~KZoneDivisionInfo()
    {

    }
public:
    CString      TZoneName;
    double       TRate;
    Integer      TZoneID;
    TxPointPtr   TNodeGeometry;
    TxPolygonPtr TDemarcationGeometry;
    CString      TIntegrationZoneList;
};

typedef std::shared_ptr<KZoneDivisionInfo> KZoneDivisionInfoPtr;

class KZoningOriginZoneInfo
{
public:
    KZoningOriginZoneInfo()
    {
        TDivisionCount = 2;
        TvecZoneDivisionInfo.push_back(KZoneDivisionInfoPtr(new KZoneDivisionInfo(50.0)));
        TvecZoneDivisionInfo.push_back(KZoneDivisionInfoPtr(new KZoneDivisionInfo(50.0)));
    }

    ~KZoningOriginZoneInfo()
    {

    }

public:
    size_t TDivisionCount;
    std::vector<KZoneDivisionInfoPtr> TvecZoneDivisionInfo;
};

typedef std::shared_ptr<KZoningOriginZoneInfo> KZoningOriginZoneInfoPtr;