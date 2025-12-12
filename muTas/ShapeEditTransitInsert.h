#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "TransitPathInsertDlg.h"
#include "ScreenDrawFlashLink.h"
#include "ScreenDrawFlashLinkPath.h"
#include "KEditForeDrawTransitPath.h"
#include "ImChampMapSelectionReceiver.h"


class KShapeEditTransitInsert : public IShapeEdit, public ImChampMapSelectionReceiver, public ITransitPathEditOpener
{
public:
    KShapeEditTransitInsert(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
    virtual ~KShapeEditTransitInsert(void) ;

public:
    virtual void CommandFeedback(CString a_strResult);
    virtual void Canceled       (                   );

    virtual KEMMapShapeEdit GetType()
    {
        return KEMMapShapeEditLinkUpdate;
    }
    
public:
    virtual void Draw();
public:
    virtual void OnMapLButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapLButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}
    virtual void OnMapLButtonDblClk (UINT nFlags, long X, long Y, double mapX, double mapY) {}

    virtual void OnMapMouseMove     (UINT nFlags, long X, long Y, double mapX, double mapY);

    virtual void OnMapRButtonDown   (UINT nFlags, long X, long Y, double mapX, double mapY);
    virtual void OnMapRButtonUp     (UINT nFlags, long X, long Y, double mapX, double mapY) {}

public:
    virtual void StartEditing();
    virtual void SaveEditing();
    virtual void StopEditing();
    virtual void Sleep       (){};
    virtual WORD GetCursorID ();

public:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();
private:
    KForeDrawSingleTransitPathPtr m_spForeDrawTransitPath;
private:
    KScreenDrawFlashLinkPtr     m_spScreenDrawFlashLink;
    KScreenDrawFlashLinkPathPtr m_spScreenDrawFlashLinkPath;

public:
    virtual void SetEditID(Integer a_nxID);
private:
    Integer m_nxEditTransitID;
    KTransitPathInsertDlgPtr m_spTransitPathEditDlg;

private:
    TUserLinkPathSelectMode::KEMInsert m_emStatus;
private:
    void TransitPathPopulate();
private:
    virtual void TransitPathEditFeedback();
    virtual void TransitPathStationEditFeedback();
    virtual void TransitSaveFeedback(__int64 nxID = 0);
    virtual void TransitPathCommand(int nCommand);
private:
    void RedrawUserSelectedPath();

private:
    UINT m_nMouseFlag;
};