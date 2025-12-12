#include "stdafx.h"
#include "TxGridTileIndex.h"

TxGridTileIndex::TxGridTileIndex(double dGridSize) : m_dGridSize(dGridSize)
{
    // 0,0을 바꾸지 마세요
    m_dGridMinX = 0.0;
    m_dGridMinY = 0.0;   
}

TxGridTileIndex::~TxGridTileIndex()
{
}

void TxGridTileIndex::GetGeoGridID( double x, double y, __int64& Gx, __int64& Gy )
{    
    double dTx = (x - m_dGridMinX) / m_dGridSize + 0.5;
    double dTy = (y - m_dGridMinY) / m_dGridSize + 0.5;

    Gx = (__int64)dTx;
    Gy = (__int64)dTy;
}

std::set<TxGridID> TxGridTileIndex::GetGeoGridID( double dMinX, double dMinY, double dMaxX, double dMaxY )
{
    if (dMinX > dMaxX) {
        double dTemp = dMaxX;
        dMaxX = dMinX;
        dMinX = dTemp;
    }

    if (dMinY > dMaxY) {
        double dTemp = dMaxY;
        dMaxY = dMinY;
        dMinY = dTemp;
    }

    __int64 MinGx(0), MinGy(0);
    GetGeoGridID(dMinX, dMinY, MinGx, MinGy);

    __int64 MaxGx(0), MaxGy(0);
    GetGeoGridID(dMaxX, dMaxY, MaxGx, MaxGy);

    std::set<TxGridID> setGridID;

    for (__int64 i=MinGx; i<=MaxGx; i++) 
    {
        for (__int64 j=MinGy; j<=MaxGy; j++) 
        {
            setGridID.insert(TxGridID(i, j));
        }
    }

    return setGridID;
}
