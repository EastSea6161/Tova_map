#pragma once
#include "IQBicGeometry.h"

class QBicEnvelope;
/**
*@brief 포인트
*@details 큐빅웨어 포인트
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class QBicPoint : public IQBicGeometry
{
public:
    QBicPoint();
    QBicPoint(double dX, double dY);
    QBicPoint(int nX, int nY);
    QBicPoint(const QBicPoint& right);

    virtual ~QBicPoint();

public:
    bool operator == (const QBicPoint& right) const;
    QBicPoint& operator = (const QBicPoint& right);
public:
    double X;
    double Y;

public:
	/**
	*@brief 포인트
	*@return 점 좌표 반환
	*/
    virtual QBicEnvelope GetEnvelope();
};
