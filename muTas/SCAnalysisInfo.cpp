#include "StdAfx.h"
#include "SCAnalysisInfo.h"


KSCAnalysisInfo::KSCAnalysisInfo(void)
	: m_bSelected(false)
	, m_dOccupancy(1.0f)
	, m_dVolumnPCU(1.0f)
	, m_dODPCU(1.0f)
	, m_pMode(NULL)
	, m_pInputColumn(NULL)
	, m_pSurveyColumn(NULL)
	, m_pOutputColumn(NULL)
	, m_strOutputColumnCaption(_T(""))
	, m_strColumnName(_T(""))
{
}

KSCAnalysisInfo::KSCAnalysisInfo( const KSCAnalysisInfo& r )
	: m_bSelected(r.m_bSelected)
	, m_dOccupancy(r.m_dOccupancy)
	, m_dVolumnPCU(r.m_dVolumnPCU)
	, m_dODPCU(r.m_dODPCU)
	, m_pMode(r.m_pMode)
	, m_pInputColumn(r.m_pInputColumn)
	, m_pSurveyColumn(r.m_pSurveyColumn)
	, m_pOutputColumn(r.m_pOutputColumn)
	, m_strOutputColumnCaption(r.m_strOutputColumnCaption)
	, m_strColumnName(r.m_strColumnName)
{
}


KSCAnalysisInfo::~KSCAnalysisInfo(void)
{
}

KSCAnalysisInfo& KSCAnalysisInfo::operator=( const KSCAnalysisInfo& r )
{
	m_bSelected					= r.m_bSelected;
	m_dOccupancy				= r.m_dOccupancy;
	m_dVolumnPCU				= r.m_dVolumnPCU;
	m_dODPCU					= r.m_dODPCU;
	m_pMode						= r.m_pMode;
	m_pInputColumn				= r.m_pInputColumn;
	m_pSurveyColumn				= r.m_pSurveyColumn;
	m_pOutputColumn				= r.m_pOutputColumn;
	m_strOutputColumnCaption	= r.m_strOutputColumnCaption;
	m_strColumnName				= r.m_strColumnName;

	return *this;
}

bool KSCAnalysisInfo::Selected( void )
{
	return m_bSelected;
}

void KSCAnalysisInfo::Selected( bool a_bSelected )
{
	m_bSelected	= a_bSelected;
}

double KSCAnalysisInfo::Occupancy( void )
{
	return m_dOccupancy;
}

void KSCAnalysisInfo::Occupancy( double a_dOccupancy )
{
	m_dOccupancy = a_dOccupancy;
}

double KSCAnalysisInfo::ODPCU( void )
{
	return m_dODPCU;
}

void KSCAnalysisInfo::ODPCU( double a_dODPCU )
{
	m_dODPCU = a_dODPCU;
}

double KSCAnalysisInfo::VolumePCU( void )
{
	return m_dVolumnPCU;
}

void KSCAnalysisInfo::VolumePCU( double a_dVolumePCU )
{
	m_dVolumnPCU = a_dVolumePCU;
}

KMode* KSCAnalysisInfo::Mode( void )
{
	return m_pMode;
}

void KSCAnalysisInfo::Mode( KMode* a_pMode )
{
	m_pMode = a_pMode;
}

KIOColumn* KSCAnalysisInfo::InputColumn( void )
{
	return m_pInputColumn;
}

void KSCAnalysisInfo::InputColumn( KIOColumn* a_pInputColumn )
{
	m_pInputColumn = a_pInputColumn;
}

KIOColumn* KSCAnalysisInfo::SurveyColumn( void )
{
	return m_pSurveyColumn;
}

void KSCAnalysisInfo::SurveyColumn( KIOColumn* a_pSurveyColumn )
{
	m_pSurveyColumn = a_pSurveyColumn;
}

KIOColumn* KSCAnalysisInfo::OutputColumn( void )
{
	return m_pOutputColumn;
}

void KSCAnalysisInfo::OutputColumn( KIOColumn* a_pOutputColumn )
{
	m_pOutputColumn = a_pOutputColumn;
}

CString KSCAnalysisInfo::OutputColumnName( void )
{
	return m_strOutputColumnCaption;
}

void KSCAnalysisInfo::OutputColumnName( CString a_strOutputColumnCaption )
{
	m_strOutputColumnCaption = a_strOutputColumnCaption;
}

CString KSCAnalysisInfo::ColumnName( void )
{
	return m_strColumnName;
}

void KSCAnalysisInfo::ColumnName( CString a_strColumnName )
{
	m_strColumnName = a_strColumnName;
}
