#include "StdAfx.h"
#include "RoadTimeCostInfo.h"
//^^ #include "IOColumn.h"


KRoadTimeCostInfo::KRoadTimeCostInfo(void)
{
}


KRoadTimeCostInfo::~KRoadTimeCostInfo(void)
{
}

void KRoadTimeCostInfo::UseFunction( bool a_bSelected )
{
	m_bSelected = a_bSelected;
}

bool KRoadTimeCostInfo::UseFunction( void )
{
	return m_bSelected;
}

void KRoadTimeCostInfo::UpLoadValue( double a_dUpLaodValue )
{
	m_dUpLoadValue = a_dUpLaodValue;
}

double KRoadTimeCostInfo::UpLoadValue( void )
{
	return m_dUpLoadValue;
}

void KRoadTimeCostInfo::DownLoadValue( double a_dDownlaodValue )
{
	m_dDownLoadValue = a_dDownlaodValue;
}

double KRoadTimeCostInfo::DownLoadValue( void )
{
	return m_dDownLoadValue;
}

void KRoadTimeCostInfo::TripValue( double a_dTripTime )
{
	m_dTripValue = a_dTripTime;
}

double KRoadTimeCostInfo::TripValue( void )
{
	return m_dTripValue;
}

void KRoadTimeCostInfo::OriginRoadColumn( KIOColumn* a_pKIoColumn )
{
	if (a_pKIoColumn == NULL)
	{
		return;
	}
	m_pOriginRoadColumn = a_pKIoColumn;
	m_strOriginRoadColumnName = a_pKIoColumn->Caption();
}

KIOColumn* KRoadTimeCostInfo::OriginRoadColumn( void )
{
	return m_pOriginRoadColumn;
}

void KRoadTimeCostInfo::DestinationRoadColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}
	m_pDestiantionRoadColumn = a_pKIOColumn;
	m_strDestinationRoadColumnName = m_pDestiantionRoadColumn->Caption();
}

KIOColumn* KRoadTimeCostInfo::DestinationRoadColumn( void )
{
	return m_pDestiantionRoadColumn;
}

void KRoadTimeCostInfo::OutPutColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}
	m_pOutPutColumn = a_pKIOColumn;
	m_strOutPutColumnName = m_pOutPutColumn->Caption();
}

KIOColumn* KRoadTimeCostInfo::OutPutColumn( void )
{
	return m_pOutPutColumn;
}

CString KRoadTimeCostInfo::OriginRoadColumnName()
{
	return m_strOriginRoadColumnName;
}

CString KRoadTimeCostInfo::DestinationRoadColumnName()
{
	return m_strDestinationRoadColumnName;
}

CString KRoadTimeCostInfo::OutPutColumnName()
{
	return m_strOutPutColumnName;
}

bool KRoadTimeCostInfo::CheckUsingColumn( CString& a_strUnitName )
{
	if (m_bSelected == false)
	{
		return true;
	}

	if (m_strOriginRoadColumnName == _T(""))
	{
		a_strUnitName = _T("Road Origin Dist");
		return false;
	}

	if (m_strDestinationRoadColumnName == _T(""))
	{
		a_strUnitName = _T("Road Destination Dist");
		return false;
	}

	if (m_strOutPutColumnName == _T(""))
	{
		a_strUnitName = _T("Road OutPut");
		return false;
	}
	return true;
}

void KRoadTimeCostInfo::Mode( int a_nModeName )
{
	 m_nMode = a_nModeName;
}

int KRoadTimeCostInfo::Mode( void )
{
	return m_nMode;
}

void KRoadTimeCostInfo::Type( int a_nType )
{
	m_nType = a_nType;
}

int KRoadTimeCostInfo::Type( void )
{
	return m_nType;
}








