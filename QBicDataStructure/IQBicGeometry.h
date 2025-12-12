#pragma once
#include "QBicEnvelope.h"
/**
*@brief 기하학
*@details 큐빅웨어 사용 기하학
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class IQBicGeometry
{
public:
    IQBicGeometry() {}
    virtual ~IQBicGeometry() {}

public:
    virtual QBicEnvelope GetEnvelope() = 0;
};