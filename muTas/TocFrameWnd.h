#pragma once
#include "TocWindow.h"
#include "TocFormView.h"
/* Forward declaration */
class KMapView;

class KTocFrameWnd : public CXTPFrameWnd
{
    DECLARE_DYNCREATE(KTocFrameWnd)
    DECLARE_MESSAGE_MAP()

protected:
    // 동적 만들기에 사용되는 protected 생성자입니다.
    KTocFrameWnd();           
    virtual ~KTocFrameWnd();
public :
    inline KTocFormView* GetTocFormView()
    {
        try
        {
            if (m_wndTocFormView.GetSafeHwnd() == NULL)
                return NULL;
        }
        catch(...)
        {
            return NULL;
        }

        return &m_wndTocFormView;
    }
private:
    KMapView*  m_pMapView;	
public:
    void SetBuddyMapView(KMapView* a_pMapView)
    {
        m_pMapView = a_pMapView;
        m_wndTocFormView.SetBuddyMapView(a_pMapView);
    }

protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);    
protected:	
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);    
    afx_msg void OnSize(UINT nType, int cx, int cy);

protected:	
    CXTPDockingPaneManager  m_paneManager;
    KTocFormView            m_wndTocFormView;
};
