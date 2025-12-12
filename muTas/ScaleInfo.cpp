/**
 * @file ScaleInfo.cpp
 * @brief KScaleInfo ±¸Çö
 * @author 
 * @date 2010.08.24
 * @remark
 */

#include "stdafx.h"
#include "ScaleInfo.h"


/******************************************************************************
* KScaleInfo                                                                  *
******************************************************************************/

/**************************************
* Constructors / Destructor           *
**************************************/
KScaleInfo::KScaleInfo( void )
{
	KSTMapScale scaleNone( 0, _T("None") );
	m_ScaleArray.push_back( scaleNone );

	KSTMapScale scale1000( 1000, _T("1/1,000") );
	m_ScaleArray.push_back( scale1000 );

	KSTMapScale scale5000( 5000, _T("1/5,000") );
	m_ScaleArray.push_back( scale5000 );

	KSTMapScale scale10000( 10000, _T("1/10,000") );
	m_ScaleArray.push_back( scale10000 );

	KSTMapScale scale50000( 50000, _T("1/50,000") );
	m_ScaleArray.push_back( scale50000 );

	KSTMapScale scale100000( 100000, _T("1/100,000") );
	m_ScaleArray.push_back( scale100000 );

    KSTMapScale scale150000( 150000, _T("1/150,000") );
    m_ScaleArray.push_back( scale150000 );

    KSTMapScale scale200000( 200000, _T("1/200,000") );
    m_ScaleArray.push_back( scale200000 );

    KSTMapScale scale250000( 250000, _T("1/250,000") );
    m_ScaleArray.push_back( scale250000 );

    KSTMapScale scale300000( 300000, _T("1/300,000") );
    m_ScaleArray.push_back( scale300000 );
}


KScaleInfo::~KScaleInfo( void )
{

}
