#pragma once
#include "ShapeFileInfo.h"
#include <boost/any.hpp>

#include "../QBicExtStaticCLib/ShapeFileLib/shapefil.h"
#include "../QBicCommon/QBicBinary.h"

/**
*@brief Shape파일 리더
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

class TxPoint;

class AFX_EXT_CLASS ShapeFileReader
{
public:
    ShapeFileReader();
    ~ShapeFileReader();
public:
	/**
	*@brief 파일 열기
	*@param CString strPath 파일 경로
	*@param CString strName 파일 명
	*@return 성공시 true, 실패시 false 반환
	*/
    int Open(CString strPath, CString strName/*const char* pszShapeFile*/);
	/**
	*@brief 파일 닫기
	*/
    void Close();
private:
    ShapeFileInfo m_oShpInfo;
    SHPHandle     m_hShp;
private:
    DBFHandle	  m_hDBF;
private:
	int           m_nEncoding;

private:
	/**
	*@brief Shape파일 정보
	*@param const char * pszShapeFile 
	*@param ShapeFileInfo& oShapeFileInfo
	*@return 
	*/
    int  ShapeInfo(const char * pszShapeFile, ShapeFileInfo& oShapeFileInfo);
	/**
	*@brief Shape 읽어오기
	*@param const char * pszShapeFile 
	*@param ShapeFileInfo& oShapeFileInfo
	*/
    void ReadShape(const char * pszShapeFile, ShapeFileInfo& oShapeFileInfo);
private:

public:
	/**
	*@brief 인코딩 셋팅
	*@parma int a_nEncoding	인코딩
	*@details 받아온 인코딩을 멤버변수 인코딩에 입력
	*/
	void SetEncoding(int a_nEncoding);

public:
	/**
	*@brief 객체 확인
	*@return 객체 수 반환
	*/
    int EntityCount();
	/**
	*@brief Shape 읽어오기
	*@param int nIndex
	*@param QBicBinary& oBinary
	*@param int nBinaryType
	*/
    int ReadShape(int nIndex, QBicBinary& oBinary, int nBinaryType=0);
private:
	/**
	*@brief 버텍스 정보
	*@param SHPObject* psShape
	*@param QBicBinary& oBinary
	*@param int nBinaryType
	*/
    void ReadPoint   (SHPObject* psShape, QBicBinary& oBinary, int nBinaryType);
	/**
	*@brief 폴리라인 정보
	*@param SHPObject* psShape
	*@param QBicBinary& oBinary
	*@param int nBinaryType
	*/
    void ReadPolyline(SHPObject* psShape, QBicBinary& oBinary, int nBinaryType);
	/**
	*@brief 폴리곤 정보
	*@param SHPObject* psShape
	*@param QBicBinary& oBinary
	*@param int nBinaryType
	*/
    void ReadPolygon (SHPObject* psShape, QBicBinary& oBinary, int nBinaryType);

public:
	/**
	*@brief Shape 형식
	*@return  형식 반환
	*/
    int ShapeType() {return m_oShpInfo.ShapeType;}
	/**
	*@brief 레코드 수
	*@return 레코드 수 반환
	*/
    int RecordCount(); // DBF Record Count
	/**
	*@brief 필드 수
	*@return 필드 수 반환
	*/
    int FieldCount (); // DBF Field Count
    std::vector<DbfFieldInfo> FieldInfo();
    std::vector<DbfFieldValue> ReadRecord(int nRecord);

private:
    void OutLineInfo(std::map<int, std::vector<TxPoint>>& mapPartPt, int& nStart, int& nEnd);
    int  NeighborLine(TxPoint& oPt, std::map<int, std::vector<TxPoint>>&, double& dDist, int& nFromToFlag);
    void AppendLine(TxPoint& oFrom, TxPoint& oTo, std::map<int, std::vector<TxPoint>>& mapPartPt, std::vector<TxPoint>& vecPoint);
    void ConvertClosewiseRing(std::map<int, std::vector<TxPoint>>& mapPartPt);

// 알고자 하는 정보는 무엇인가 ? 
// Geometry Type
// Geometry 정보/DBF 정보(컬럼들에 대한 정보 / 각레코드 정보)
/* 변환프로그램 요구사항
1) 파일 선택(기본적인 검증 - Shape Type, 레코드 갯수 정보 등)
2) 테이블명을 무었으로 할 것인가 ?
   - 부가적인 정보는 엔진에서는 신경쓰지 않는다. 
   - 오직 테이블에 ID와 TxGeometry 컬럼이 있는가 여부만 체크한다.
   - 기본적으로 단순화 하자 : ID, TxGeometry
3) 다른 비지니스는 그들이 알아서 하도록 하자.. -> 일단은
*/
};

// 포인트 Shape, 링크, 존을 읽어보자...