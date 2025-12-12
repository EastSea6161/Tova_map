#pragma once

class KTransitStationSeqKey
{
public:
    Integer TransitID;
    Integer StationID;
    Integer Seq;
public:
    KTransitStationSeqKey(Integer a_nxTransitID =0, Integer a_nxStationID = 0, Integer a_nxSeq = 0)
    {
        TransitID = a_nxTransitID;
        StationID = a_nxStationID;
        Seq       = a_nxSeq;
    }
    ~KTransitStationSeqKey(void);
public:
    bool operator <  (const KTransitStationSeqKey& right) const
    {
        if ( TransitID == right.TransitID ) 
        {
            if ( StationID == right.StationID ) 
            {
                return Seq < right.Seq;
            }

            return StationID < right.StationID;
        }
        else 
        {
            return TransitID < right.TransitID;
        }
    }
    bool operator == (const KTransitStationSeqKey& right) const
    {
        return (TransitID == right.TransitID && StationID == right.StationID && Seq == right.Seq);
    }
};
