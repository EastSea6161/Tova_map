#include "StdAfx.h"
#include "MunsellColor.h"


KMunsellColor::KMunsellColor(int nStartColorIndex, bool bComplementary)
{
    /*00*/m_vecMunSell20Color.push_back(RGB(255,   0,   0));
    /*01*/m_vecMunSell20Color.push_back(RGB(252,  71,   0));
    /*02*/m_vecMunSell20Color.push_back(RGB(255, 102,   0));
    /*03*/m_vecMunSell20Color.push_back(RGB(255, 204,   0));
    /*04*/m_vecMunSell20Color.push_back(RGB(255, 255,   0));
    /*05*/m_vecMunSell20Color.push_back(RGB(204, 255, 102));
    /*06*/m_vecMunSell20Color.push_back(RGB(125, 205,   0));
    /*07*/m_vecMunSell20Color.push_back(RGB( 54, 183,   0));
    /*08*/m_vecMunSell20Color.push_back(RGB(  0, 153,   0));
    /*09*/m_vecMunSell20Color.push_back(RGB(  0, 128, 128));
    /*10*/m_vecMunSell20Color.push_back(RGB(  0, 102, 102));
    /*11*/m_vecMunSell20Color.push_back(RGB(  0, 146, 151));
    /*12*/m_vecMunSell20Color.push_back(RGB(  0, 102, 255));
    /*13*/m_vecMunSell20Color.push_back(RGB(  6,  17, 242));
    /*14*/m_vecMunSell20Color.push_back(RGB(  0,  58, 154));
    /*00*//*m_vecMunSell20Color.push_back(RGB(  0,   0, 126));*/
    /*15*/m_vecMunSell20Color.push_back(RGB( 77,   0, 154));
    /*16*/m_vecMunSell20Color.push_back(RGB(128,   0, 127));
    /*17*/m_vecMunSell20Color.push_back(RGB(119,   0,  85));
    /*18*/m_vecMunSell20Color.push_back(RGB(232,   0, 116));
    /*19*/m_vecMunSell20Color.push_back(RGB(255,   0, 102));

    m_nStartColorIndex = nStartColorIndex;
    m_bComplementary   = bComplementary;
}


KMunsellColor::~KMunsellColor(void)
{
}

COLORREF KMunsellColor::GetColor( int a_nNoIndex )
{
    int nIndex = m_nStartColorIndex + a_nNoIndex;
    if (m_bComplementary)
    {
        if (a_nNoIndex%2 != 0)
        {
            nIndex += 10; //º¸»ö
        }
    }

    int nTimes = nIndex / 20;
    if (nTimes != 0)
    {
        nIndex = nIndex - 20 * nTimes;
        COLORREF colorRefValue = m_vecMunSell20Color[nIndex];
        BYTE r =  GetRValue(colorRefValue) + (255- GetRValue(colorRefValue)) / (++nIndex);
        BYTE g =  GetGValue(colorRefValue);// + (255- GetGValue(colorRefValue)) / (++nIndex);
        BYTE b =  GetBValue(colorRefValue);// + (255- GetBValue(colorRefValue)) / (++nIndex);

        return RGB(r, g, b);
    }

    return m_vecMunSell20Color[nIndex];
}