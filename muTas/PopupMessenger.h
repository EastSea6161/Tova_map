#pragma once

#include "Singleton.h"

class KPopupMessenger
{
public:
    KPopupMessenger();
    virtual ~KPopupMessenger();
    
public:
    void Show(CString strMsg, CWnd* pMainWindow);
};

typedef ZSingleton<KPopupMessenger> KPopupMessengerConnector;