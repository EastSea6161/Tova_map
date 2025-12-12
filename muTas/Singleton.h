/**
 * @file Singleton.h
 * @brief ZSingleton ¼±¾ð
 * @author 
 * @date 2010.08.19
 * @remark
 */


#pragma once


#include "IncludeBoost.h"


template< typename T >
class ZSingleton
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
private :
	ZSingleton( void ) { }
	~ZSingleton( void ) { }


	/**************************************
	* Accessors                           *
	**************************************/
public :
	static T* GetInstnce( void )
	{
		if( NULL == m_pInstance )
		{
			m_pInstance.reset( new T );
		}
		return m_pInstance.get();
	}

    static void Reset()
    {
        if (m_pInstance != NULL)
            m_pInstance.reset();
    }

	/**************************************
	* Properties / Variables              *
	**************************************/
private :
	static std::shared_ptr<T> m_pInstance;
};


template< typename T >
std::shared_ptr< T > ZSingleton< T >::m_pInstance;

