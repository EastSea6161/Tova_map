#pragma once

#include "IMapView.h"
#include "IShapeEdit.h"
#include "TransitPathEditDlg.h"
#include "KEditForeDrawTransitPath.h"
#include "ImChampMapSelectionReceiver.h"
#include "ScreenDrawFlashLink.h"
#include "ScreenDrawFlashLinkPath.h"
#include "BulkDBaseLink.h"
#include "ImChampMapSelectionReceiver.h"

class KShapeEditTransitEdit : public IShapeEdit, public ImChampMapSelectionReceiver, public ITransitPathEditOpener
{
public:
	KShapeEditTransitEdit(IMapView* a_mapRequest, IShapeEditController* a_pShapeEditController, IShapeEditOwner* a_pShapeEditOwner = NULL) ;
	virtual ~KShapeEditTransitEdit(void) ;

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
private:
    void ReStartEditing();
public:
private:
    Integer m_nxEditTransitID;
public:
	virtual void SetEditID(Integer a_nxID);
private:
	KTransitPathEditDlgPtr m_spTransitPathEditDlg;
private:
    KForeDrawSingleTransitPathPtr m_spForeDrawTransitPath; /*현재 사용자가 수정하고 있는 노선 그리기*/
private:
    KScreenDrawFlashLinkPtr     m_spScreenDrawFlashLink;
    KScreenDrawFlashLinkPathPtr m_spScreenDrawFlashLinkPath;
public:
    virtual void MapSelectionFeedback(Integer a_nxID);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();
private:
    virtual void TransitPathEditFeedback();
    virtual void TransitPathStationEditFeedback();
    virtual void TransitSaveFeedback(__int64 nxID = 0);

    int m_nCommandType;
    virtual void TransitPathCommand(int nCommand);
private:
    void TransitPathPopulate();
    void RedrawUserSelectedPath();

private:
    TUserLinkPathSelectMode::KEMInsert m_emStatus;
private:
    std::map<Integer, TLinkFTNodeID> m_mapLinkFTNode;
    int m_nFirstSeq;

    // 사용자가 선택한 정보
    std::vector<TTransitPath> m_vecNewPath;
    // 사용자가 선택한 Path Draw -> 작업 종료 체크
    void EditPathAppendDraw(std::vector<Integer> vecLinkID, bool a_bStation=true);

    void AppendTransitFirst();
    void AppendTransitLast();
    void InsertTransit(int a_nFirst, int a_nLast);

    int  IsInTransitLink(Integer a_nxLinkID);
    int  FindTransitPathSeqByFromNode(int a_nFirstSeq, Integer a_nxID, bool a_bUserQuestion = false, bool a_bEnablePass = true);
    int  FindTransitPathSeqByToNode  (int a_nFirstSeq, Integer a_nxID, bool a_bUserQuestion = true,  bool a_bEnablePass = true);
private:
    std::vector<Integer> FindSelectableLink();
private:
    long   m_lMouseDownX;
    long   m_lMouseDownY;
    double m_dMouseDownMapX;
    double m_dMouseDownMapY;

private:
    UINT m_nMouseFlag;
};