#include "StdAfx.h"
#include "RailTimeCostInfo.h"
//^^ #include "IOColumn.h"


KRailTimeCostInfo::KRailTimeCostInfo(void)
{
}


KRailTimeCostInfo::~KRailTimeCostInfo(void)
{
}


void KRailTimeCostInfo::UseFunction( bool a_bSelected )
{
	m_bSelected = a_bSelected;
}

bool KRailTimeCostInfo::UseFunction( void )
{
	return 	m_bSelected;
}

void KRailTimeCostInfo::UpLoadValue( double a_dUpLaodValue )
{
	m_dUpLoadValue = a_dUpLaodValue;
}

double KRailTimeCostInfo::UpLoadValue( void )
{
	return m_dUpLoadValue;
}

void KRailTimeCostInfo::DownLoadValue( double a_dDownLoadValue )
{
	m_dDownLoadValue = a_dDownLoadValue;
}

double KRailTimeCostInfo::DownLoadValue( void )
{
	return m_dDownLoadValue;
}

void KRailTimeCostInfo::RoadTripValue( double a_dRoadTripValue )
{
	m_dRoadTripValue = a_dRoadTripValue;
}

double KRailTimeCostInfo::RoadTripValue( void )
{
	return m_dRoadTripValue;
}

void KRailTimeCostInfo::RailTripValue( double a_dRailTripValue )
{
	m_dRailTripValue = a_dRailTripValue;
}

double KRailTimeCostInfo::RailTripValue( void )
{
	return m_dRailTripValue;
}

void KRailTimeCostInfo::FacilityValue( double a_dFacilityValue )
{
	m_dFacilityValue = a_dFacilityValue;
}

double KRailTimeCostInfo::FacilityValue( void )
{
	return m_dFacilityValue;
}

void KRailTimeCostInfo::OriginRoadColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}
	m_pOriginRoadColumn = a_pKIOColumn;

	m_strOriginRoadName = m_pOriginRoadColumn->Caption();

}

KIOColumn* KRailTimeCostInfo::OriginRoadColumn( void )
{
	return m_pOriginRoadColumn;
}

void KRailTimeCostInfo::DestinationRoadColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}
	m_pDestionationRoadColumn = a_pKIOColumn;

	m_strDestinationRoadName = m_pDestionationRoadColumn->Caption();

}

KIOColumn* KRailTimeCostInfo::DestinationRoadColumn( void )
{
	return m_pDestionationRoadColumn;
}

void KRailTimeCostInfo::RailColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}

	m_pRailColumn = a_pKIOColumn;
	m_strRailName = m_pRailColumn->Caption();

}

KIOColumn* KRailTimeCostInfo::RailColumn( void )
{
	return m_pRailColumn;
}

void KRailTimeCostInfo::OutPutColumn( KIOColumn* a_pKIOColumn )
{
	if (a_pKIOColumn == NULL)
	{
		return;
	}

	m_pOutputColumn = a_pKIOColumn;

	m_strOutPutName = m_pOutputColumn->Caption();
}

KIOColumn* KRailTimeCostInfo::OutPutColumn( void )
{
	return m_pOutputColumn;
}

CString KRailTimeCostInfo::OriginRoadColumnName()
{
	return m_strOriginRoadName;
}

CString KRailTimeCostInfo::DestinationRoadColumName()
{
	return m_strDestinationRoadName;
}

CString KRailTimeCostInfo::RailColumnName()
{
	return m_strRailName;
}

CString KRailTimeCostInfo::OutPutColumnName()
{
	return m_strOutPutName;
}

bool KRailTimeCostInfo::CheckUsingColumn(CString& a_strUnitName)
{
	if (m_bSelected == false)
	{
		return true;
	}

	if (m_strOriginRoadName == _T(""))
	{
		a_strUnitName = _T("Road Origin Dist");
		return false;
	}

	if (m_strDestinationRoadName == _T(""))
	{
		a_strUnitName = _T("Road Destination Dist");
		return false;
	}

	if (m_strRailName == _T(""))
	{
		a_strUnitName = _T("Rail Dist");
		return false;
	}

	if (m_strOutPutName == _T(""))
	{
		a_strUnitName = _T("Road OutPut");
		return false;
	}
	return true;
}

void KRailTimeCostInfo::Mode( int a_nModeName )
{
	m_nMode = a_nModeName;
}

int KRailTimeCostInfo::Mode( void )
{
	return m_nMode;
}

void KRailTimeCostInfo::Type( int a_nType )
{
	m_nType = a_nType;
}

int KRailTimeCostInfo::Type( void )
{
	return m_nType;
}











