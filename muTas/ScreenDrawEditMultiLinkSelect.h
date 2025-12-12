#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"
#include "ScreenDrawFlashLink.h"
#include "ScreenDrawFlashLinkPath.h"
#include "KThemeLayerUnion.h"
#include "ImChampMapSelectionReceiver.h"

#pragma once

class KScreenDrawEditMultiLinkSelect : public IScreenDrawEdit
{
private:
    ImChampMapSelectionReceiver* m_pMapSelectionReceiver;
public:
    KScreenDrawEditMultiLinkSelect(IMapView* a_mapRequest, ImChampMapSelectionReceiver* a_pMapSelectionReceiver=nullptr);
    ~KScreenDrawEditMultiLinkSelect();
public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
    virtual ThemeLayerDrawEdit GetType();

public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);
public:
    virtual void StartEditing   ();
    virtual bool IsEditingStatus() 
    {
        return m_bScreenDrawEditing;
    }
    virtual void Sleep          (){};
private:
    void LinkSelectCommandMsg();
private:
    void StopEditing();
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();

private:
    TUserLinkPathSelectMode::KEMInsert m_emStatus;    
    KScreenDrawFlashLinkPtr     m_spScreenDrawFlashLink;
    KScreenDrawFlashLinkPathPtr m_spScreenDrawFlashLinkPath;

private:
    std::vector<Integer>        m_vecSelectedLink;
    KScreenDrawCompositePtr     m_spScreenDrawComposite;

    void AddDrawCompositeLink(std::vector<Integer> a_vecSelectedLink);
};