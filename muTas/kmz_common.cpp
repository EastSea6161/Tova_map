/**
 * @file kmz_common.cpp
 * @brief 공통 모듈 구현
 * @author 
 * @date 2010.10.05
 * @remark
 */


#include "stdafx.h"
#include "kmz_common.h"

int ParseTargetYear(CString a_strYears, std::vector<int>& a_vecTargetYears)
{
	return ParseIntegerArr(a_strYears, a_vecTargetYears);
}


int ParsePurposeName(CString a_strPurposes, std::vector<CString>& a_vecPurposes)
{
	return ParseStringArr(a_strPurposes, a_vecPurposes);
}


int ParsePurposeId(CString a_strPurposeId, std::vector<int>& a_vecPurposeId)
{
	return ParseIntegerArr(a_strPurposeId, a_vecPurposeId);
}


int ParseModeName(CString a_strModes, std::vector<CString>& a_vecModes)
{
	return ParseStringArr(a_strModes, a_vecModes);
}


int ParseModeId(CString a_strModeId, std::vector<int>& a_vecModeId)
{
	return ParseIntegerArr(a_strModeId, a_vecModeId);
}


int ParseIntegerArr(CString a_strIn, std::vector<int>& a_vecOut)
{
	CString strTemp, strFind = _T(", ");
	int nCount = 0;	
	int nIndex = a_strIn.Find(strFind);
	while( nIndex != -1 )
	{
		strTemp = a_strIn.Left(nIndex);
		a_strIn = a_strIn.Right(a_strIn.GetLength() - nIndex - 2);
		nIndex = a_strIn.Find(strFind);
		a_vecOut.push_back( _ttoi(strTemp) );
		nCount += 1;
	}

	if( a_strIn.IsEmpty() == false )
	{
		a_vecOut.push_back( _ttoi(a_strIn) );
		nCount += 1;
	}

	return nCount;
}


int ParseStringArr(CString a_strIn, std::vector<CString>& a_vecOut)
{
	CString strTemp, strFind = _T(", ");
	int nCount = 0;
	int nIndex = a_strIn.Find(strFind);
	while( nIndex != -1)
	{
		strTemp = a_strIn.Left(nIndex);
		a_strIn = a_strIn.Right( a_strIn.GetLength() - nIndex - 2 );

		nIndex = a_strIn.Find(strFind);

		a_vecOut.push_back( strTemp );
		nCount++;
	}

	if( a_strIn.IsEmpty() == false)
	{
		a_vecOut.push_back( a_strIn );
		nCount++;
	}

	return nCount;

}
