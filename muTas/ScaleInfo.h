/**
 * @file ScaleInfo.h
 * @brief KScaleInfo 선언
 * @author 
 * @date 2010.08.24
 * @remark
 */

#pragma once


#include "Singleton.h"


/* Map Scale */
struct KSTMapScale
{
	KSTMapScale( int n, CString str )
	{
		nScale = n;
		strScale = str;
	}
	int nScale;
	CString strScale;
};


/* Type definitions */
typedef std::vector< KSTMapScale > KTDScaleArray;


/**
 * @brief Map의 기본 스케일 정보
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.24
 */
class KScaleInfo 
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public :
	KScaleInfo( void );
	~KScaleInfo( void );


	/**************************************
	* Accessors                           *
	**************************************/
public :
	inline const KTDScaleArray& getScaleArray( void ) const
	{
		return m_ScaleArray;
	}


	/**************************************
	* Operations                          *
	**************************************/


	/**************************************
	* Properties / Variables              *
	**************************************/
protected :
	KTDScaleArray m_ScaleArray;
	
};

typedef ZSingleton< KScaleInfo > KTDScaleInfo;