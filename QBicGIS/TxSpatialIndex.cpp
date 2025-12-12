#include "stdafx.h"
#include "TxSpatialIndex.h"

void TxMemorySpatialIndex::Insert( __int64 nxID, const TxEnvelope& oEnvelope )
{
    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);
    oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    BoostEnvelope oBoostEnvelope(BoostPoint(dMinX, dMinY), BoostPoint(dMaxX, dMaxY));
    auto oEnvelopeID = std::make_pair(oBoostEnvelope, nxID);
    m_oRTree.insert(oEnvelopeID);        
}

void TxMemorySpatialIndex::Remove( __int64 nxID )
{
    BoostEnvelope oBoostEnvelope = m_oRTree.bounds();
    auto iter = m_oRTree.qbegin(BoostGeometry::index::intersects(oBoostEnvelope));
    auto end  = m_oRTree.qend();

    for (/**/; iter != end; ++iter)
    {
        TreeValue oNode = *iter;
        if (oNode.second == nxID)
        {
            m_oRTree.remove(oNode);
            break;
        }
    }
}

void TxMemorySpatialIndex::Clear()
{
    m_oRTree.clear(); 
}

std::vector<__int64> TxMemorySpatialIndex::Intersect( const TxEnvelope& oEnvelope )
{
    std::vector<__int64> vecResult;
    if (m_oRTree.size() < 1)
        return vecResult;

    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);
    oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    BoostEnvelope oBoostEnvelope(BoostPoint(dMinX, dMinY), BoostPoint(dMaxX, dMaxY));

    std::vector<TreeValue> vecTreeValue;
    m_oRTree.query(BoostGeometry::index::intersects(oBoostEnvelope), std::back_inserter(vecTreeValue));

    for (size_t i=0; i<vecTreeValue.size(); i++)
    {
        TreeValue& oNode = vecTreeValue[i];
        vecResult.push_back(oNode.second);
    }
        
    return vecResult;
}

TxEnvelope TxMemorySpatialIndex::GetExtent()
{
    BoostEnvelope oEnvelope = m_oRTree.bounds();
    return TxEnvelope(oEnvelope);
}

__int64 TxMemorySpatialIndex::EntityCount()
{
    return m_oRTree.size();
}

/*
std::vector<__int64> TMemorySpatialIndex::IntersectMapDisp( const TxEnvelope& oEnvelope )
{
    double dX(0.0), dY(0.0);
    oEnvelope.GetCenter(dX, dY);
    double dOffset = oEnvelope.GetWidth();

    if (dOffset > oEnvelope.GetHeight())
        dOffset = oEnvelope.GetHeight();

    dOffset *= 0.4f;
    
    std::vector<__int64> vecResult;
    if (m_oRTree.size() < 1)
        return vecResult;

    double dMinX(0.0), dMinY(0.0), dMaxX(0.0), dMaxY(0.0);
    oEnvelope.GetEnvelope(dMinX, dMinY, dMaxX, dMaxY);

    std::set<__int64> setFirstDraw;
    {
        BoostEnvelope oBoostEnvelope(BoostPoint(dX-dOffset, dY-dOffset), BoostPoint(dX+dOffset, dY+dOffset));
        std::vector<TreeValue> vecTreeValue;
        m_oRTree.query(BoostGeometry::index::intersects(oBoostEnvelope), std::back_inserter(vecTreeValue));

        for (size_t i=0; i<vecTreeValue.size(); i++)
        {
            TreeValue& oNode = vecTreeValue[i];
            setFirstDraw.insert(oNode.second);
        }
    }

    BoostEnvelope oBoostEnvelope(BoostPoint(dMinX, dMinY), BoostPoint(dMaxX, dMaxY));
    std::vector<TreeValue> vecTreeValue;
    m_oRTree.query(BoostGeometry::index::intersects(oBoostEnvelope), std::back_inserter(vecTreeValue));

    // Sorting 속도가 너무 느리다
    // XTMemorySpatialIndex::m_oSortPoint.set<0>(dX);
    // XTMemorySpatialIndex::m_oSortPoint.set<1>(dY);
    // std::sort(vecTreeValue.begin(), vecTreeValue.end(), SortDist);
    

    auto iter = setFirstDraw.begin();
    auto end  = setFirstDraw.end();
    while (iter != end)
    {
        vecResult.push_back(*iter);
        ++iter;
    }

    for (size_t i=0; i<vecTreeValue.size(); i++)
    {
        TreeValue& oNode = vecTreeValue[i];
        __int64 nxID = oNode.second;
        iter = setFirstDraw.find(nxID);
        if (iter == end)
            vecResult.push_back(nxID);
    }

    return vecResult;
}
*/

