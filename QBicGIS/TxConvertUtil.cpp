#include "stdafx.h"
#include "TxConvertUtil.h"

Gdiplus::RectF TxConvertUtil::ToRectF( const CPoint& ptFrom, const CPoint& ptTo )
{
    long minX = 999999999;
    long maxX = -99999999;

    long minY = minX;
    long maxY = maxX;

    if (ptFrom.x < ptTo.x)
    {
        minX = ptFrom.x;
        maxX = ptTo.x;
    }
    else
    {
        maxX = ptFrom.x;
        minX = ptTo.x;
    }

    if (ptFrom.y < ptTo.y)
    {
        minY = ptFrom.y;
        maxY = ptTo.y;
    }
    else
    {
        maxY = ptFrom.y;
        minY = ptTo.y;
    }

    float left   = (float) minX;
    float top    = (float) minY;
    float width  = (float)(maxX-minX);
    float height = (float)(maxY-minY);

    return Gdiplus::RectF(left, top, width, height);
}

CRect TxConvertUtil::ToCRect( const CPoint& ptFrom, const CPoint& ptTo )
{
    int minX = 999999999;
    int maxX = -99999999;

    int minY = minX;
    int maxY = maxX;

    if (ptFrom.x < ptTo.x)
    {
        minX = ptFrom.x;
        maxX = ptTo.x;
    }
    else
    {
        maxX = ptFrom.x;
        minX = ptTo.x;
    }

    if (ptFrom.y < ptTo.y)
    {
        minY = ptFrom.y;
        maxY = ptTo.y;
    }
    else
    {
        maxY = ptFrom.y;
        minY = ptTo.y;
    }

    return CRect(minX, minY, maxX, maxY);
}
