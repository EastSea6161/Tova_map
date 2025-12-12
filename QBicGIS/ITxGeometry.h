#pragma once
#include "TxMapEnumType.h"
#include "TxEnvelope.h"
#include "../QBicCommon/QBicBinary.h"
/**
*@brief 기하학
*@details ITX 기반 기하학
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxGeometry 
{
public:
    ITxGeometry(EnumTxGeoType enGeoType = GeoShapeType) : m_enGeometryType(enGeoType)
    {
    }

    virtual ~ ITxGeometry() {}

protected:
    EnumTxGeoType m_enGeometryType;
public:
    EnumTxGeoType GeometryType() const
    {
        return m_enGeometryType;
    }

public:
    virtual TxEnvelope Envelope() const = 0;
    virtual bool Intersect(const TxEnvelope& _oEnvelope) const = 0;

public:
    virtual void GetBinary(QBicBinary& oBinary) = 0;

public:
    virtual std::shared_ptr<ITxGeometry> CopyGeometry() = 0;
};

typedef std::shared_ptr<ITxGeometry> ITxGeometryPtr;