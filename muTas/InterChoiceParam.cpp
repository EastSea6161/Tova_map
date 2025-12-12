#include "StdAfx.h"
#include "InterChoiceParam.h"


KInterChoiceParam::KInterChoiceParam(void)
{
}

KInterChoiceParam::KInterChoiceParam(CString a_strAttr)
	:	m_strAttr(a_strAttr),
		m_dblAuto(0.0),
		m_dblBus(0.0),
		m_dblRail(0.0),
		m_dblTaxi(0.0)
{
}


KInterChoiceParam::~KInterChoiceParam(void)
{
}

CString KInterChoiceParam::Attribute(void)
{
	return m_strAttr;
}

void KInterChoiceParam::Attribute(CString a_strAttr)
{
	m_strAttr = a_strAttr;
}

double KInterChoiceParam::Auto(void)
{
	return m_dblAuto;
}

void KInterChoiceParam::Auto(double a_dblAuto)
{
	m_dblAuto = a_dblAuto;
}


double KInterChoiceParam::Taxi(void)
{
	return m_dblTaxi;
}

void KInterChoiceParam::Taxi(double a_dblTaxi)
{
	m_dblTaxi = a_dblTaxi;
}

double KInterChoiceParam::Bus(void)
{
	return m_dblBus;
}

void KInterChoiceParam::Bus(double a_dblBus)
{
	m_dblBus = a_dblBus;
}

double KInterChoiceParam::Rail(void)
{
	return m_dblRail;
}

void KInterChoiceParam::Rail(double a_dblRail)
{
	m_dblRail = a_dblRail;
}

void KInterChoiceParam::InitParamData(void)
{
	m_dblAuto = 0.0;
	m_dblTaxi = 0.0;
	m_dblBus  = 0.0;
	m_dblRail = 0.0;
}