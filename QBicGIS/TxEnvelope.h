#pragma once
#include "IncludeBoost.h"
class TxPoint;
/**
*@brief 범위
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS TxEnvelope
{
public:
    TxEnvelope();
    TxEnvelope(double dMinX, double dMinY);
    TxEnvelope(double dCx,   double dCY,   double dSquareWidth);
    TxEnvelope(double dMinX, double dMinY, double dMaxX, double dMaxY);
    TxEnvelope(const TxEnvelope& right);
    TxEnvelope(BoostEnvelope& right);
    TxEnvelope(RECT& rc);
public:
    ~TxEnvelope();
private:
    double m_dMinX;
    double m_dMinY;
    double m_dMaxX;
    double m_dMaxY;

public:
    TxEnvelope& operator =  (const TxEnvelope& right);
    TxEnvelope  operator +  (const TxEnvelope& right);
    TxEnvelope& operator += (const TxEnvelope& right);
    bool Equal(const TxEnvelope& right);
public:
    void Reset();
public:
    void Union(const TxEnvelope& oEnvelope);
public:
    operator BoostEnvelope() {
        return BoostEnvelope(BoostPoint(m_dMinX, m_dMinY), BoostPoint(m_dMaxX, m_dMaxY));
    }

public:
	/**
	*@brief 최소, 최대범위 지정
	*@details 최소 X, Y  / 최대 X, Y
	*@param double  dMinX 최소 X
	*@param double  dMinY 최소 Y
	*@param double  dMaxX 최대 X
	*@param double  dMaxY 최대 Y
	*/
    void SetEnvelope(double  dMinX, double  dMinY, double  dMaxX, double  dMaxY);
	/**
	*@brief 최소, 최대범위 지정
	*@details 최소 X, Y  / 최대 X, Y
	*@param double  dMinX 최소 X
	*@param double  dMinY 최소 Y
	*@param double  dMaxX 최대 X
	*@param double  dMaxY 최대 Y
	*/
    void GetEnvelope(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY) const;

public:
		/**
	*@brief 지정된 범위에서 가운데 좌표 확인
	*@param double& dX X좌표
	*@param double& dY Y좌표
	*/
    void GetCenter(double& dX, double& dY) const;
public:
	/**
	*@brief 이동
	*@param double dNewX X좌표
	*@param double dNewY Y좌표
	*/
    void Move(double dNewX, double dNewY);
public:
	/**
	*@brief 넓이
	*/
    double Area() const;
public:
	/**
	*@brief 버텍스 추출
	*/
    std::vector<TxPoint> GetPoints() const;
public:
	/**
	*@brief 간격조정
	*@param double dOffset 범위설정
	*/
    void Offset(double dOffset);
    void Expansion(double dRate);
    void Shift(double dX, double dY);
public:
	/**
	*@brief 가로길이 확인
	*@details 최대 - 최소
	*@return 차이값 반환
	*/
    double GetWidth() const;
	/**
	*@brief 세로길이 확인
	*@details 최대 - 최소
	*@return 차이값 반환
	*/
    double GetHeight()const;
public:
    double GetMinX() const {return m_dMinX;}
    double GetMinY() const {return m_dMinY;}
    double GetMaxX() const {return m_dMaxX;}
    double GetMaxY() const {return m_dMaxY;}
public:
	/**
	*@brief 정상값 확인
	*@details 최대값 / 최소값 범위 확인
	*@param const TxEnvelope& rOther 
	*@return 정상적인 값이면 ture, 비정상적이면 false를 반환
	*/
    bool Intersect (const TxEnvelope& rOther) const;
	/**
	*@brief 내부에 있는지 확인
	*@param const TxEnvelope& rOther 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
    bool Contains  (const TxEnvelope& rOther) const;
	/**
	*@brief 내부에 있는지 확인(버텍스)
	*@param const TxPoint& _oPt 가지고 있는 좌표
	*@return 정상적이면 ture, 맞지않으면 false 반환
	*/
    bool IsInPoint (const TxPoint& _oPt) const;
public:
	/**
	*@brief 표출
	*@return 최소값, 최대값 표출
	*/
    CString ToString() const;
};

typedef std::shared_ptr<TxEnvelope> TxEnvelopePtr;