#pragma once

struct TRangeRecord
{
    double MinValue;
    double MaxValue;
};

class KDataRange
{
private:
    double dZ;
public:
    KDataRange();
    ~KDataRange(void);

public:
    std::vector<TRangeRecord> GetRange(std::map<KODKey, double> a_mapValues, int a_nDevideCount, double dBasicRange);

public:
};
