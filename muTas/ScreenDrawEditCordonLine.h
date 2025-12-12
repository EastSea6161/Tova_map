#pragma once

#include "IMapView.h"
#include "IScreenDrawEdit.h"

enum KEMCordonLineEditStatus
{
    KEMCordonLineEditStatusNothing           = 0, 
    KEMCordonLineEditStatusNameInput         = 1,
    KEMCordonLineEditStatusNameInputed       = 2,
    KEMCordonLineEditStatusFirstPoint        = 3,
    KEMCordonLineEditStatusFirstPointInputed = 4, 
    KEMCordonLineEditStatusAdd               = 6,
    KEMCordonLineEditStatusComplete          = 9
};

class KScreenDrawEditCordonLine : public IScreenDrawEdit
{
public:
    KScreenDrawEditCordonLine(IMapView* a_mapRequest) ;
    virtual ~KScreenDrawEditCordonLine(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );
public:
	void MapSelectionFeedback(Integer a_nxID){};
    virtual ThemeLayerDrawEdit GetType()
    {
        return ThemeLayerDrawEditCordonLine;
    }
    
public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
	virtual void Sleep();
private:
    void FirstPointCommandMsg();
    void AddPointCommandMsg();
private:
    KEMCordonLineEditStatus m_emCordonLineEditStatus;
private:
    CString  m_strName;
private:
    ITxSymbolPtr m_spSymbolLine;
    ITxSymbolPtr m_spSymbolPoly;
private:
    ITxGeometryPtr m_spGeoDraw;    
private:
    std::vector<TxPoint> m_vecPt;

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