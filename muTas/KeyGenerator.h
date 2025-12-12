/**
 * @file KeyGenerator.h
 * @brief ZKeyGenerator 선언
 * @author 
 * @date 2010.08.19
 * @remark
 */

#pragma once

/**
 * @brief Project / Scenario / Target의 ID를 생성한다.
 * @remark 
 * @warning key의 타입은 K++연산자를 재정의 필요
 * @version 1.0
 * @author 
 * @date 2010.08.19
 */

template< typename T >
class ZKeyGenerator
{

public:
	ZKeyGenerator(void) : m_key(0) { }
	virtual ~ZKeyGenerator(void) {};

public :
	T GenerateUniqueKey( void )
	{
		return m_key++;
	}

private :
	T m_key;
};


