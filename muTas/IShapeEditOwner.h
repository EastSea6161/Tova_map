#pragma once

enum KEMSlaveEndFeedback
{
    KEMSlaveEndFeedbackCancel = 0, 
    KEMSlaveEndFeedbackOK     = 1
};

enum KEMMapShapeEdit
{
    KEMMapShapeEditNodeInsert        = 1,
    KEMMapShapeEditNodeUpdate        = 2,
    KEMMapShapeEditLinkInsert        = 5,
    KEMMapShapeEditLinkUpdate        = 6,
    KEMMapShapeEditDemarcationInsert = 9,
    KEMMapShapeEditDemarcationVertex = 10,
    KEMMapShapeEditOneWayLinkVertex,
    KEMMapShapeEditTwoWayLinkVertex,
    KEMMapShapeEditTurnInsert
};

class IShapeEditOwner
{
public:
    IShapeEditOwner ()         {};
    virtual ~IShapeEditOwner() {};
public:
    
};