#pragma once
#include "ImChampMapSelectionReceiver.h"
class KMapView;

enum MapViewFeedbackType
{
    MapViewFeedbackTypeNone = 0,
    MapViewFeedbackTypeNodeSelect,
    MapViewFeedbackTypeLinkSelect,
    MapViewFeedbackTypeTransitLinkSelect,
    MapViewFeedbackTypeTransitNodeSelect
};

class KMapViewSelectionFeedback : public ImChampMapSelectionReceiver
{
private:
    KMapView* m_pMapView;
private:
    MapViewFeedbackType m_emFeedbackType;
            
public:
    KMapViewSelectionFeedback(KMapView* a_pMapView);
    ~KMapViewSelectionFeedback();

private:
    virtual void MapSelectionFeedback(Integer);
    virtual void MapSelectionFeedback(std::vector<Integer> a_vecSelection);
    virtual void MapSelectionNoneFeedback();
public:
    void SetFeedbackType(MapViewFeedbackType a_emFeedbackType)
    {
        m_emFeedbackType = a_emFeedbackType;
    }

private:
    void TransitLinkSelect (Integer a_nxLinkID);
    void TransitNodeSelect (Integer a_nxLinkID);

    void LinkSelect(std::vector<Integer> a_vecLinkID);
    void NodeSelect(std::vector<Integer> a_vecNodeID);
};

typedef std::shared_ptr<KMapViewSelectionFeedback> KMapViewSelectionFeedbackPtr;