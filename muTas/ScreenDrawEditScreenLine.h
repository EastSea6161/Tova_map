#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"

enum KEMScreenLineEditStatus
{
    KEMScreenLineEditStatusNothing           = 0, 
    KEMScreenLineEditStatusNameInput         = 1,
    KEMScreenLineEditStatusNameInputed       = 2,
    KEMScreenLineEditStatusFirstPoint        = 3,
    KEMScreenLineEditStatusFirstPointInputed = 4, 
    KEMScreenLineEditStatusAdd               = 6,
    KEMScreenLineEditStatusComplete          = 9
};

class KScreenDrawEditScreenLine : public IScreenDrawEdit
{
public:
    KScreenDrawEditScreenLine(IMapView* a_mapRequest) ;
    virtual ~KScreenDrawEditScreenLine(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
public:
	void MapSelectionFeedback(Integer a_nxID){};

    virtual ThemeLayerDrawEdit GetType()
    {
        return ThemeLayerDrawEditScreenLine;
    }
    
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep() {}
private:
    void FirstPointCommandMsg();
    void AddPointCommandMsg();
private:
    KEMScreenLineEditStatus m_emScreenLineEditStatus;
private:
    CString m_strName;

private:
    std::vector<TxPoint> m_vecPt;
private:
    ITxSymbolPtr  m_spSymbolLine;
protected:
    virtual void Draw();
protected:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
};