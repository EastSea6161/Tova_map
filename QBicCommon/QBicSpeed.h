#pragma once
/**
*@brief 속도
*@details 시간별 속도
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS QBicSpeed
{
public:
    QBicSpeed() {}
    ~QBicSpeed(){}

public:
	/**
	*@brief 속도
	*@details 초속계산
	*@param double dLengthKm	길이Km
	*@param double dSpeedKmHour	속도/h
	*@return 초속 값
	*/
    static double TravelTimeSec(double dLengthKm, double dSpeedKmHour)
    {
        double dTravelTime(0.0);
        dTravelTime = dLengthKm / dSpeedKmHour;
        dTravelTime = dTravelTime / 3600.0;
        return dTravelTime;
    }
};