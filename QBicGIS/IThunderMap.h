#pragma once
#include "IThunderMouseEvent.h"
#include "TxMapLayerManager.h"
#include "ThunderMapDrawProcess.h"
#include "GDIStart.h"
#include "TxProcessInfo.h"
/**
*@brief ¸Ê
*@details ¸ÊÃÊ±âÈ­
*@author Å¥ºò¿þ¾î
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS IThunderMap : public IThunderMouseEvent
{
private:
    GDIStart oGDIStart;
protected:
    IThunderMap() : IThunderMouseEvent() {};
    virtual ~IThunderMap(){};
public:
    virtual HWND GetHwnd() = 0;
public:
    virtual TxMapLayerManagerPtr LayerManager() = 0;

public:
    virtual ThunderMapdrawProcessPtr GetThunderDxDraw() = 0;

public:
    virtual ThunderMapDispPtr GetThunderMapDisp() = 0;

public:
    virtual void SetMapExtent(const TxEnvelope& _oEnvelope) = 0;

public:
    virtual void   Refresh()   = 0;
    virtual double PixelDist() = 0;
    virtual double PointDist() = 0;
public:
    ULONG GetProcessID()
    {
        HWND hwnd = GetHwnd();
        if (hwnd == nullptr)
            return 0;

        return TxProcessInfo::ProcessIDFromHWnd(hwnd);
    }
};