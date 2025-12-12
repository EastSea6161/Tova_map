#pragma once

#include "KMapEvents.h"
#include "IMapView.h"
#include "ImChampCommand.h"
#include "IShapeEditOwner.h"
#include "CommandWnd.h"

class IShapeEditController;

class IShapeEdit : public KMapEvents, public ImChampCommandReceiver, public IShapeEditOwner
{
protected:
    IMapView*             m_pIMapView;
    IShapeEditController* m_pShapeEditController;
    IShapeEditOwner*      m_pShapeEditOwner;
    bool                  m_bEditing;
public:
    IShapeEdit(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) 
    { 
        m_pIMapView				= a_mapRequest;
        m_pShapeEditController  = a_pShapeEditController;
        m_pShapeEditOwner       = a_pShapeEditOwner;
        m_bEditing              = false;
        m_preMapMode            = m_pIMapView->GetMapMode();
        
        TxLogDebugVisitor();
    }
    virtual ~IShapeEdit(void) 
    {
        m_pIMapView        = NULL;
        m_bEditing         = false;
        TxLogDebugVisitor();
    }
protected:
    KEMMapMode m_preMapMode;

public:
    virtual void CommandFeedback(CString a_strResult) = 0;
    virtual void Canceled() = 0;

    virtual KEMMapShapeEdit   GetType()            = 0;

public:
public:
    virtual void StartEditing    () = 0;
    virtual void StopEditing     () = 0;
	virtual void Sleep           () = 0;
	virtual WORD GetCursorID     () = 0;
	virtual void OnShapeEditPopupCommand(UINT nID) {};
	virtual bool GetShapeEditPopupInfo(UINT&  nMenuID, int& nPos)
	{
		return false;
	}

public:
    virtual bool IsEditingStatus () 
    {
        return m_bEditing;
    }
    virtual void SendLogMsg(CString strLog)
    {
        KCommandWnd*    pCommandWindow = m_pIMapView->GetCommandView();
		if (pCommandWindow)
			pCommandWindow->CommandLogMsg(strLog);
    }
public:
	virtual void SetEditID(Integer a_nxID){};

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
};

typedef std::shared_ptr<IShapeEdit> KShapeEditPtr;
