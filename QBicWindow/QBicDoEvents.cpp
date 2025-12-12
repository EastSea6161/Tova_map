#include "stdafx.h"
#include "QBicDoEvents.h"


QBicDoEvents::QBicDoEvents()
{

}

QBicDoEvents::~QBicDoEvents()
{

}

void QBicDoEvents::DoEvents()
{
    MSG msg;

    while ( PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE ) )
    {
        if ( GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            break;
    }
}


