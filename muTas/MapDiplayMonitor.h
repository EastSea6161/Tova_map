#pragma once

#include <list>
class IMapDisplayObserver;

class KMapDisplayMonitor
{
private:
    KMapDisplayMonitor(void);
public:
    ~KMapDisplayMonitor(void);

public:
    static KMapDisplayMonitor* Instance();

    static void Register  (IMapDisplayObserver* a_pMapDisplayReporter);
    static void UnRegister(IMapDisplayObserver* a_pMapDisplayReporter);

    static void ChangeMapDisplay (IMapDisplayObserver* a_pMapDisplayReporter);

private:
    static 
        KMapDisplayMonitor*              m_pKMapDisplayMonitor;
    static 
        std::list<IMapDisplayObserver*> m_lstIMapDisplayReporter;
};

