#pragma once

#include "KModelDefine.h"
#include <vector>

class KDistributionZoneData
{
public:
    //KDistributionZoneData(void){};
	KDistributionZoneData(Integer _ZoneID=0, Double _Production=0.0, Double _Attraction=0.0)
	{
		ZoneID     = _ZoneID;
		Production = _Production;
		Attraction = _Attraction;
	};
    ~KDistributionZoneData(void){};
public:
	Integer    ZoneID;
	Double Production;
	Double Attraction; 
};

class KDistributionZoneDataCollection
{
public:
    KDistributionZoneDataCollection();
    ~KDistributionZoneDataCollection();
private:
    std::vector<KDistributionZoneData*> m_records;

public:
    KDistributionZoneData* AddRecord();
    KDistributionZoneData* AddRecord(Integer _ZoneID, Double _Production, Double _Attraction);
    void RemoveAll();

    std::vector<KDistributionZoneData*>::iterator begin();
    std::vector<KDistributionZoneData*>::iterator end();
};

class KDistributionODData
{
public:
	KDistributionODData(void);
	KDistributionODData(Integer _OriginID, Integer _DestinationID, Double _ODValue)
	{
		OriginID	  = _OriginID;
		DestinationID = _DestinationID;
		ODValue       = _ODValue;
	};

	~KDistributionODData(void);
public:
	Integer  OriginID;
	Integer  DestinationID;
	Double   ODValue; 
};

class KDistributionFactor
{
public:
	KDistributionFactor(void)
	{
		ProductionGrowthFactor = 0.0;
		AttractionGrowthFactor = 0.0;

		ProductionAdjustFactor = 0.0;
		AttractionAdjustFactor = 0.0;	
	};

	KDistributionFactor(Double _productionGrowthFactor, Double _attractionGrowthFactor)
	{
		ProductionGrowthFactor = _productionGrowthFactor;
		AttractionGrowthFactor = _attractionGrowthFactor;

		ProductionAdjustFactor = 0.0;
		AttractionAdjustFactor = 0.0;		
	};

	~KDistributionFactor(void);

public:
	Double ProductionGrowthFactor;
	Double AttractionGrowthFactor;

	Double ProductionAdjustFactor;
	Double AttractionAdjustFactor;

};