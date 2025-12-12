#pragma once
class QBicPoint;
/**
*@brief 범위 지정
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicEnvelope
{
public:
    QBicEnvelope();
    QBicEnvelope(double dMinX, double dMinY);
    QBicEnvelope(double dMinX, double dMinY, double dMaxX, double dMaxY);
    QBicEnvelope(QBicEnvelope& right);
    ~QBicEnvelope();

private:
    double m_dMinX;
    double m_dMinY;
    double m_dMaxX;
    double m_dMaxY;

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
	*@brief 지정된 범위값 추출
	*@details 최소 X, Y  / 최대 X, Y
	*@param double&  dMinX 최소 X
	*@param double&  dMinY 최소 Y
	*@param double&  dMaxX 최대 X
	*@param double&  dMaxY 최대 Y
	*/
    void GetEnvelope(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY);

public:
	/**
	*@brief 지정된 범위에서 가운데 좌표 확인
	*@param double& dX X좌표
	*@param double& dY Y좌표
	*/
    void GetCenter(double& dX, double& dY);
public:
	/**
	*@brief 간격조정
	*@param double dOffset 범위설정
	*/
    void Offset(double dOffset);
public:
	/**
	*@brief 가로길이 확인
	*@details 최대 - 최소
	*@return 차이값 반환
	*/
    double GetWidth();
	/**
	*@brief 세로길이 확인
	*@details 최대 - 최소
	*@return 차이값 반환
	*/
    double GetHeight();

public:
	/**
	*@brief 정상값 확인
	*@details 최대값 / 최소값 범위 확인
	*@param QBicEnvelope& rOther 
	*@return 정상적인 값이면 ture, 비정상적이면 false를 반환
	*/
    bool IsIntersect(QBicEnvelope& rOther);
	/**
	*@brief 정상값 확인
	*@param QBicEnvelope& rOther 
	*@return bX, bY를 반환함
	*/
    bool IsContained(QBicEnvelope& rOther);
};