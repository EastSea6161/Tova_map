#pragma once
using namespace Gdiplus;
/**
*@brief GDIStart
*@details 
*@author Å¥ºò¿þ¾î
*@date 2016/05/17
*@version 1.0.0
*/
class GDIStart
{
private:
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
public:
    GDIStart()
    {
        if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Ok)
        {
            throw 1;
        }
    }
    ~GDIStart()
    {
        GdiplusShutdown(gdiplusToken);
    }
};