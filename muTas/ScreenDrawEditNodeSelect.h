#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "NodeSelectionInfo.h"
#include "ScreenDrawFlashNode.h"

class KScreenDrawEditNodeSelect : public IScreenDrawEdit
{
public:
	KScreenDrawEditNodeSelect(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver, std::vector<TNodeSelectionInfo>& ar_vecTNodeStation,bool a_bLabel = true, double a_dDrawScale = 100000.0) ;
	virtual ~KScreenDrawEditNodeSelect(void) ;

public:
	virtual void CommandFeedback(CString a_strResult);
	virtual void Canceled       (                   );

	virtual ThemeLayerDrawEdit GetType()
	{
		return ThemeLayerDrawEditScreenIdentify;
	}

public:
	virtual void StartEditing();
	virtual void SaveEditing();
	virtual void StopEditing();
	virtual void Sleep() {};
private:
	void SelectCommandMsg();

	KScreenDrawFlashNodePtr m_spFlashNode;
public:
	void MapSelectionFeedback(Integer a_nxID);
	void MapSelectionNoneFeedback();
private:
	ImChampMapSelectionReceiver* m_pSlaveMapSelectionReceiver;

public:
	virtual void ActivatedFrameWndNotifyProcess()  ;  //향후, virtual
	virtual void DeActivatedFrameWndNotifyProcess();  //향후, virtual

private:
	std::vector<TNodeSelectionInfo> m_vecNodeSelectionInfo;
	std::vector<Integer>            m_vecSelectableNodeID;

private:
private:
    bool   m_bDrawLabel;
	double m_dDrawScale;

protected:
    virtual void Draw();
protected:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};