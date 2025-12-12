#include "stdafx.h"
#include "QBicTime.h"

QBicTime::QBicTime() : CTime(CTime::GetCurrentTime())
{  
}

QBicTime::QBicTime( CTime& oTime ) : CTime(oTime)
{     
}

QBicTime::QBicTime( int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSec ) : CTime(CTime(nYear, nMonth, nDay, nHour, nMinute, nSec))
{   
}

QBicTime::~QBicTime()
{
}

__int64 QBicTime::DayInterval( const QBicTime& right ) const
{
    CTimeSpan oTimeSpan = *this - right;
    return oTimeSpan.GetDays();
}

__int64 QBicTime::SecInterval( const QBicTime& right ) const
{
    CTimeSpan oTimeSpan = *this - right;
    return oTimeSpan.GetTotalSeconds();
}
