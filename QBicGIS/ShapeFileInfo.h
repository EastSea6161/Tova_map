#pragma once
/**
*@brief Shape파일
*@details Shape파일 정보
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ShapeFileInfo
{
public:
    int	   ShapeType; /*0:Null Shape, 1:Point, 3:Polyline, 5:Polygon*/
    int    Entities;
    double BoundMin[4];
    double BoundMax[4];
public:
    ShapeFileInfo()
    {
        ShapeType = 0;
        Entities  = 0;

        BoundMin[0] = 0.0;
        BoundMin[1] = 0.0;
        BoundMin[2] = 0.0;
        BoundMin[3] = 0.0;

        BoundMax[0] = 0.0;
        BoundMax[1] = 0.0;
        BoundMax[2] = 0.0;
        BoundMax[3] = 0.0;
    }
    ~ShapeFileInfo(){}
};
/**
*@brief Shape파일주제 [비어있음]
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ShapeObject
{
public:
    ShapeObject();
    ~ShapeObject();
};
/**
*@brief 데이터베이스 필드 정보
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS DbfFieldInfo
{
public:
    DbfFieldInfo()
    {
        m_strFieldName = _T("");
        m_nFiledType   = 0;
        m_nWidth       = 20;
        m_nDecimals    = 0;
    }
    ~DbfFieldInfo()
    {
    }
public:
    CString m_strFieldName;
    int     m_nFiledType; /*0:String, 1:Integer, 2:Double*/
    int     m_nWidth;
    int     m_nDecimals;
};
/**
*@brief 데이터베이스 필드 값
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS DbfFieldValue
{
public:
    DbfFieldValue() : m_strValue(_T("")), m_nFieldIndex(-1)
    {
    }
public:
    ~DbfFieldValue()
    {
    }
public:
    int     m_nFieldIndex;
    CString m_strValue;
};