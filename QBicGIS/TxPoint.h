#pragma once
#include "ITxGeometry.h"

class AFX_EXT_CLASS TxPoint : public ITxGeometry
{
public:
	TxPoint();
	TxPoint(double dX, double dY);
	TxPoint(int    nX, int    nY);
	TxPoint(long   nX, long   nY);

	TxPoint(const TxPoint& right);
    TxPoint(std::shared_ptr<TxPoint> spPt);

	virtual ~TxPoint();
public:
	TxPoint(QBicBinary& oBinary, size_t nIndex=0);
public:
	TxPoint& operator =  (const TxPoint& right);
	bool     operator == (const TxPoint& right) const;
    bool     operator != (const TxPoint& right) const;

	operator BoostPoint()
	{
		return BoostPoint(X, Y);
	}

	/*operator D2D1_POINT_2F()
	{
	return D2D1::Point2F((float)X, (float)Y);
	}*/

	operator Gdiplus::PointF()
	{
		return Gdiplus::PointF((float)X, (float)Y);
	}
public:
	/**
	*@brief
	*@param double dWidth 길이
	*@param	double dHeight 높이
	*@return 최소 X, Y좌표 / 길이, 높이
	*/
	Gdiplus::RectF ToScreenGdiPlusRectF(double dWidth = 0.0, double dHeight = 0.0);

public:
	double X;
	double Y;
public:
    void PutCoords(double dX, double dY);
public:
	/**
	*@brief X,Y값 전달
	*@return X,Y값 전달
	*/
	virtual TxEnvelope Envelope() const;
public:
	/**
	*@brief 버텍스 위치
	*@return 버텍스 위치 반환
	*/
	virtual CString ToString();
public:
	/**
	*@brief 정보 삽입
	*@param QBicBinary& oBinary에 벡터형식 정보 입력
	*/
	virtual void GetBinary(QBicBinary& oBinary);
public:
	/**
	*@brief 조건 검사
	*@param const TxEnvelope& _oEnvelope
	*@return 조건 일치 ture, 미일치 false 반환
	*/
	virtual bool Intersect(const TxEnvelope& _oEnvelope) const;

public:
	/**
	*@brief 거리 차이
	*@param const TxPoiont& oPt 점 위치
	*@return 거리 차이 반환
	*/
	virtual double MinDist(const TxPoint& oPt);
public:
	virtual ITxGeometryPtr CopyGeometry() {
		return ITxGeometryPtr(new TxPoint(X, Y));
	}
};

typedef std::shared_ptr<TxPoint> TxPointPtr;
/**
*@brief 버텍스
*@details TxM 버텍스 위치
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxMPoint : public ITxGeometry
{
private:
	std::vector<TxPoint> m_vecPoint;
public:
	TxMPoint();
	TxMPoint(const TxMPoint& right);
	TxMPoint(QBicBinary& oBinary, size_t nIndex=0);
	virtual ~TxMPoint();
public:
	TxMPoint& operator =  (const TxMPoint& right);
public:
	/**
	*@brief X,Y값 전달
	*@return 버텍스의 크기 전달
	*/
	size_t Count() {return m_vecPoint.size();}

public:
	/**
	*@brief 버텍스
	*@return 좌표 지정
	*/
	virtual TxEnvelope Envelope() const;
public:
	/**
	*@brief 버텍스
	*@return 점 좌표 반환
	*/
	virtual void GetBinary(QBicBinary& oBinary);
public:
	/**
	*@brief 정상적인 폴리라인 확인
	*@param const TxEnvelope& _oEnvelope 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
	virtual bool Intersect(const TxEnvelope& _oEnvelope) const;
public:
	/**
	*@brief 최소거리
	*@param const TxPoint& oPt
	*@return 최소거리 반환
	*/
	virtual double MinDist(const TxPoint& oPt);
public:
	/** 
	*@brief 기하학 복사
	*@return 복사값 반환
	*/
	virtual ITxGeometryPtr CopyGeometry(){
		return ITxGeometryPtr(new TxMPoint(*this));
	}
};

typedef std::shared_ptr<TxMPoint> TxMPointPtr;