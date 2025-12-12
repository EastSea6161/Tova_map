#pragma once



class IMapEditObserver
{
public:
	IMapEditObserver(void) {};
	virtual ~IMapEditObserver(void) {};

public:
	virtual void DrawingLink() = 0;
// public:
// 	virtual KEMMapDisplaySync GetMapSyncMode() = 0;
// 
// 	virtual void   MapPositionSyncMessage     (double a_dMapCenterPointX, double a_dMapCenterPointY) = 0;
// 	virtual void   MapPositionScaleSyncMessage(double a_dMapCenterPointX, double a_dMapCenterPointY, double a_dScale) = 0;
// 
// 	virtual double GetMapScale() = 0;
// 	virtual void   GetMapCenterPoint(double& a_dMapCenterPointX, double& a_dMapCenterPointY) = 0;
};

typedef std::shared_ptr<IMapEditObserver> IMapEditObserverPtr;