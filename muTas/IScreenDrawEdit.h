#pragma once

#include "IMapView.h"
#include "ImChampCommand.h"
#include "ImChampMapSelectionReceiver.h"
#include "CommandWnd.h"

enum ThemeLayerDrawEdit
{
    ThemeLayerDrawEditCordonLine    = 120,
	ThemeLayerDrawEditScreenLine    ,
	ThemeLayerDrawEditScreenIdentify,      
	KEMMapLinkPathSelect              ,
    KEMScreenDrawEditSearchPolygon    ,
    KEMScreenDrawEditSearchRect
};

class IScreenDrawEdit : public ImChampCommandReceiver, public ImChampMapSelectionReceiver
{
protected:
    IMapView* m_pIMapView;
    bool      m_bScreenDrawEditing;
public:
    IScreenDrawEdit(IMapView* pMapView) { 
        m_pIMapView          = pMapView;
        m_bScreenDrawEditing = false;

        m_preMapMode         = m_pIMapView->GetMapMode();
        TxLogDebugVisitor();
    }

    virtual ~IScreenDrawEdit(void) {
        m_pIMapView          = NULL;
        m_bScreenDrawEditing = false;
        TxLogDebugVisitor();
    }

protected:
    KEMMapMode m_preMapMode;

public:
    virtual void CommandFeedback(CString a_strResult) = 0;
    virtual void Canceled       (                   ) = 0;
public:
    virtual ThemeLayerDrawEdit GetType() = 0;

public:
public:
    virtual void Draw() {};
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

public:
    virtual void StartEditing   () = 0;
    virtual bool IsEditingStatus() {
        return m_bScreenDrawEditing;
    }
	virtual void Sleep          () {}
public:
	virtual void ActivatedFrameWndNotifyProcess()  {};  //향후, 순수 virtual
	virtual void DeActivatedFrameWndNotifyProcess(){};  //향후, 순수 virtual
protected:
	virtual void SendLogMsg(CString strLog) {
		try
		{
			KCommandWnd*    pCommandWindow = m_pIMapView->GetCommandView();
			if (pCommandWindow)
				pCommandWindow->CommandLogMsg(strLog);
		}
		catch (...)
		{
		}		
	}

	virtual void SendModelessCommandMsg(CString strMsg) {
		try
		{
			KCommandWnd*  pCommandWindow = m_pIMapView->GetCommandView();
			ImChampModalCommandPtr spCommand(new ImChampCommand(strMsg, this));
			if (pCommandWindow)
				pCommandWindow->Command(spCommand, KEMModeless);
		}
		catch(...)
		{
		}
	}
};

typedef std::shared_ptr<IScreenDrawEdit> KScreenDrawEditPtr;