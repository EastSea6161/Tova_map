#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "ScreenDrawFlashLink.h"
#include "ImChampMapSelectionReceiver.h"

class KScreenDrawEditSingleLinkSelect : public IScreenDrawEdit
{
private:
    ImChampMapSelectionReceiver* m_pMapSelectionReceiver;
public:
    KScreenDrawEditSingleLinkSelect(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver=nullptr);
    ~KScreenDrawEditSingleLinkSelect();
public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual ThemeLayerDrawEdit GetType();

public:
    virtual void Draw() {};
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

public:
    virtual void StartEditing   ();
    virtual bool IsEditingStatus() {
        return m_bScreenDrawEditing;
    }
    virtual void Sleep          (){};

private:
    void StopEditing();
private:
    KScreenDrawFlashLinkPtr m_spScreenDrawFlashLink;
private:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();
};