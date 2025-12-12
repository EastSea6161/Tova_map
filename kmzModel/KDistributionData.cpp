#include "KDistributionData.h"

KDistributionZoneDataCollection::KDistributionZoneDataCollection(void)
{
    m_records.reserve(2000);
}

KDistributionZoneDataCollection::~KDistributionZoneDataCollection(void)
{
    RemoveAll();
}

KDistributionZoneData* KDistributionZoneDataCollection::AddRecord()
{
    KDistributionZoneData* pRecord = new KDistributionZoneData();
    m_records.push_back(pRecord);

    return pRecord;
}

KDistributionZoneData* KDistributionZoneDataCollection::AddRecord(Integer _ZoneID, Double _Production, Double _Attraction)
{
    KDistributionZoneData* pRecord = new KDistributionZoneData(_ZoneID, _Production, _Attraction);
    m_records.push_back(pRecord);

    return pRecord;
}

void KDistributionZoneDataCollection::RemoveAll()
{
    std::vector<KDistributionZoneData*>::iterator iter = m_records.begin();
    std::vector<KDistributionZoneData*>::iterator end  = m_records.end();

    while( iter != end )
    {
        KDistributionZoneData* pRecord = *iter;
        delete pRecord;
        ++iter;
    }

    m_records.clear();
}

std::vector<KDistributionZoneData*>::iterator KDistributionZoneDataCollection::begin()
{
    return m_records.begin();
}

std::vector<KDistributionZoneData*>::iterator KDistributionZoneDataCollection::end()
{
    return m_records.end();
}