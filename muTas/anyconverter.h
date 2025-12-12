/**
 * @file anyconverter.h
 * @brief boost::any 와 CString간 변환을 위한 Utility 클래스들
 * @author 
 * @date 2010.09.15
 * @remark
 */


#pragma once



#include "IncludeBoost.h"



/**
* @brief boost::any 타입을 CString 문자열로 변환
* @remark
* @warning
* @version 1.0
* @author 
* @date 2010.09.15
*/
class KAnyToString
{
public :
	void operator() ( const boost::any& aValue, CString& strValue )
	{
		if( aValue.empty() == true )
		{
			strValue = _T("Null");
		}
		else if( aValue.type() == typeid( int ) )
		{
			strValue.Format( _T("%d"), boost::any_cast< int >( aValue ) );
		}
		else if( aValue.type() == typeid( __int64 ) )
		{
			strValue.Format( _T("%I64d"), boost::any_cast< __int64 >( aValue ) );
		}
		else if( aValue.type() == typeid( double ) )
		{
			strValue.Format( _T("%f"), boost::any_cast< double >( aValue ) );
		}
		else if( aValue.type() == typeid( CString ) )
		{
			strValue = boost::any_cast< CString >( aValue );
		}
	}
};




/**
 * @brief CString 문자열을 주어진 타입의 boost::any로 변환
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.09.15
 */
class KStringToAny
{
public :
	void operator() ( const CString& strValue, const type_info& aType, boost::any& aValue )
	{
		if( typeid( int ) == aType )
		{
			aValue = _ttoi( strValue );
		}
		else if( typeid( __int64 ) == aType )
		{
			aValue = _ttoi64( strValue );
		}
		else if( typeid( double ) == aType )
		{
			aValue = _tstof( strValue );
		}
		else if( typeid( CString ) == aType )
		{
			aValue = strValue;
		}
	}
};


