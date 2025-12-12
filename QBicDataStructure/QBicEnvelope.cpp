#include "QBicEnvelope.h"
#include "QBicPoint.h"

QBicEnvelope::QBicEnvelope()
{
    m_dMinX = 0.0;
    m_dMinY = 0.0;
    m_dMaxX = 0.0;
    m_dMaxY = 0.0;
}

QBicEnvelope::QBicEnvelope( double dMinX, double dMinY )
{
    m_dMinX = dMinX;
    m_dMaxX = dMinX;

    m_dMinY = dMinY;
    m_dMaxY = dMinY;
}


QBicEnvelope::QBicEnvelope( double dMinX, double dMinY, double dMaxX, double dMaxY )
{
    if (dMinX < dMaxX)
    {
        m_dMinX = dMinX;
        m_dMaxX = dMaxX;
    }
    else
    {
        m_dMinX = dMaxX;
        m_dMaxX = dMinX;
    }

    if (dMinY < dMaxY)
    {
        m_dMinY = dMinY;
        m_dMaxY = dMaxY;
    }
    else
    {
        m_dMinY = dMaxY;
        m_dMaxY = dMinY;
    }
}

QBicEnvelope::QBicEnvelope( QBicEnvelope& right )
{
    m_dMinX = right.m_dMinX;
    m_dMaxX = right.m_dMaxX;

    m_dMinY = right.m_dMinY;
    m_dMaxY = right.m_dMaxY;
}


QBicEnvelope::~QBicEnvelope()
{

}

void QBicEnvelope::SetEnvelope( double dMinX, double dMinY, double dMaxX, double dMaxY )
{
    if (dMinX < dMaxX)
    {
        m_dMinX = dMinX;
        m_dMaxX = dMaxX;
    }
    else
    {
        m_dMinX = dMaxX;
        m_dMaxX = dMinX;
    }

    if (dMinY < dMaxY)
    {
        m_dMinY = dMinY;
        m_dMaxY = dMaxY;
    }
    else
    {
        m_dMinY = dMaxY;
        m_dMaxY = dMinY;
    }
}

void QBicEnvelope::GetEnvelope( double& dMinX, double& dMinY, double& dMaxX, double& dMaxY )
{
    dMinX = m_dMinX;
    dMinY = m_dMinY;

    dMaxX = m_dMaxX;
    dMaxY = m_dMaxY;
}


void QBicEnvelope::GetCenter( double& dX, double& dY )
{
    dX = (m_dMaxX + m_dMinX) * 0.5;
    dY = (m_dMaxY + m_dMinY) * 0.5;
}

double QBicEnvelope::GetWidth()
{
    return m_dMaxX - m_dMinX;
}

double QBicEnvelope::GetHeight()
{
    return m_dMaxY - m_dMinY;
}

void QBicEnvelope::Offset( double dOffset )
{
    if (dOffset >= 0)
    {
        m_dMinX -= dOffset;
        m_dMaxX += dOffset;

        m_dMinY -= dOffset;
        m_dMaxY += dOffset;
    }
    else
    {
        double dWidth  = m_dMaxX - m_dMinX;
        double dHeight = m_dMaxY - m_dMinY;
        double dCx = m_dMinX + dWidth  * 0.5;
        double dCy = m_dMinY + dHeight * 0.5;
        
        // Shrink (-)
        dOffset = -1.0 * dOffset;

        if (dWidth * 0.5 >= dOffset)
        {
            m_dMinX += dOffset;
            m_dMaxX -= dOffset;            
        }
        else
        {
            double dTemp = m_dMinX;
            m_dMinX = m_dMaxX - dOffset;
            m_dMaxX = dTemp   + dOffset;
        }

        if (dHeight * 0.5 >= dOffset)
        {
            m_dMinY += dOffset;
            m_dMaxY -= dOffset;
        }
        else
        {
            double dTemp = m_dMinY;
            m_dMinY = m_dMaxY - dOffset;
            m_dMaxY = dTemp   + dOffset;
        }
    }
}

bool QBicEnvelope::IsIntersect( QBicEnvelope& rOther )
{
    if (rOther.m_dMaxX < m_dMinX)
        return false;

    if (rOther.m_dMinX > m_dMaxX)
        return false;

    if (rOther.m_dMaxY < m_dMinY)
        return false;

    if (rOther.m_dMinY > m_dMaxY)
        return false;

    return true;
}

bool QBicEnvelope::IsContained( QBicEnvelope& rOther )
{
    bool bX = (m_dMinX <= rOther.m_dMinX) && (m_dMaxX >= rOther.m_dMaxX);
    bool bY = (m_dMinY <= rOther.m_dMinY) && (m_dMaxY >= rOther.m_dMaxY);

    return bX && bY;
}



