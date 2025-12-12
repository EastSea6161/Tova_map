#pragma once
#include "ITxObject.h"
#include "TxMapEnumType.h"
#include <memory>

/**
*@brief 레이어 정보
*@details 레이어 정보
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxLayerInfo
{
public:
    TxLayerInfo(){}
    TxLayerInfo(EnumTxGeoType enGeoType, CString strLayerName, CString strLayerCaption=_T(""))
    {
        m_enGeoType       = enGeoType;
        m_strLayerName    = strLayerName;
        m_strLayerCaption = strLayerCaption;
    }

    virtual ~TxLayerInfo() {}

private:
    CString m_strLayerName;
    CString m_strLayerCaption;
public:
	/**
	*@brief 레이어 명칭
	*@return 레이어 명칭
	*/
    CString Name()    {return m_strLayerName;   }
	/**
	*@brief 레이어 주석
	*@return 레이어 주석
	*/
    CString Caption() {return m_strLayerCaption;}
private:
    EnumTxGeoType m_enGeoType;
public:
	/**
	*@brief 기하학 타입
	*@return 기하학 타입
	*/
    EnumTxGeoType GeoType() {return m_enGeoType;}   
	/**
	*@brief 기하하 타입 
	*@param EnumTxGeoType enGeoType 기하학 타입
	*/
    void GeoType(EnumTxGeoType enGeoType) {m_enGeoType = enGeoType;}
};

typedef std::shared_ptr<TxLayerInfo> TxLayerInfoPtr;
/**
*@brief 레이어 정보
*@details 레이어 정보(SQL)
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxLayerInfoSQL: public TxLayerInfo
{
public:
    TxLayerInfoSQL(EnumTxGeoType enGeoType, CString strLayerName, CString strTxIDField, CString strLayerCaption=_T("")) 
                  : TxLayerInfo(enGeoType, strLayerName, strLayerCaption)
    {
        m_dGridSize = 1.0;
        m_dMinX = 0.0;
        m_dMinY = 0.0;
        m_dMaxX = 0.0;
        m_dMaxY = 0.0;

        m_strTxIDField = strTxIDField;
    }
    virtual ~TxLayerInfoSQL() {}

private:
    double m_dMinX;
    double m_dMinY;
    double m_dMaxX;
    double m_dMaxY;
public:
	/**
	*@brief 최소값 X
	*@return 최소값 X
	*/
    double GetMinX() {return m_dMinX;}
	/**
	*@brief 최소값 Y
	*@return 최소값 Y
	*/
    double GetMinY() {return m_dMinY;}
	/**
	*@brief 최대값 X
	*@return 최대값 X
	*/
    double GetMaxX() {return m_dMaxX;}
	/**
	*@brief 최대값 Y
	*@return 최대값 Y
	*/
    double GetMaxY() {return m_dMaxY;}
public:
	/**
	*@brief 규모 설정
	*@param double dMinX 최소값 X
	*@param double dMinY 최소값 Y
	*@param double dMaxX 최대값 X
	*@param double dMaxY 최대값 Y
	*/
    void SetExtent(double dMinX, double dMinY, double dMaxX, double dMaxY){
        if (dMinX < dMaxX) {
            m_dMinX = dMinX;
            m_dMaxX = dMaxX;
        }else {
            m_dMinX = dMaxX;
            m_dMaxX = dMinX;
        }

        if (dMinY < dMaxY) {
            m_dMinY = dMinY;
            m_dMaxY = dMaxY;
        }else {
            m_dMinY = dMaxY;
            m_dMaxY = dMinY;
        }
    }
private:
    double m_dGridSize;
public:
	/**
	*@brief 그리드 크기 설정
	*@param double dGridSize 적용할 그리드 크기
	*/
    void   SetGridSize(double dGridSize) {
        m_dGridSize = dGridSize;
    }
	/**
	*@brief 그리드 크기 추출
	*@return 그리드 크기 반환
	*/
    double GetGridSize() {return m_dGridSize;}

private:
    CString m_strTxIDField;
public:
	/**
	*@brief 필드 ID
	*@return  필드 ID 반환
	*/
    CString TxIDField() {
        return m_strTxIDField;
    }
};

typedef std::shared_ptr<TxLayerInfoSQL> TxLayerInfoSQLPtr;