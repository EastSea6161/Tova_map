#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "TDrawLinkInfo.h"
#include "ScreenDrawFlashLink.h"
#include "ScreenDrawFlashNode.h"
#include "ScreenDrawFlashZone.h"
#include "KEMLayer.h"
#include "PropertiesDlg.h"


class KScreenDrawEditMapIdentify : public IScreenDrawEdit
{
private:
	KEMLayer m_emLayer;
public:
	void SetIdentifyLayer(KEMLayer a_enumLayer) {
		m_emLayer = a_enumLayer;
	}
public:
	KScreenDrawEditMapIdentify(IMapView* pMapView) ;
	virtual ~KScreenDrawEditMapIdentify(void) ;

public:
	virtual void CommandFeedback(CString a_strResult);
	virtual void Canceled       (                   );

	virtual ThemeLayerDrawEdit GetType()
	{
		return ThemeLayerDrawEditScreenIdentify;
	}
    
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY);

public:
	virtual void StartEditing();
	virtual void SaveEditing();
	virtual void StopEditing();
	virtual void Sleep() {};
private:
	void SelectCommandMsg();
	void UserSelectFirstLinkAfterProcess(Integer nxValue, int nHosun);

	KScreenDrawFlashLinkPtr m_spScreenDrawFlashLink;
	KScreenDrawFlashNodePtr m_spScreenDrawFlashNode;
    KScreenDrawFlashZonePtr m_spScreenDrawFlashZone; 
public:
	void MapSelectionFeedback(Integer a_nxID);

private:
	void SearchNode(double mapX, double mapY);

	KPropertiesDlgPtr m_spPropertiesDlg;
public:
	virtual void ActivatedFrameWndNotifyProcess()  ;  //향후, virtual
	virtual void DeActivatedFrameWndNotifyProcess();  //향후, virtual
};