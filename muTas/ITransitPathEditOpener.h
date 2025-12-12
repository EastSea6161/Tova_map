#pragma once

class ITransitPathEditOpener
{
public:
    ITransitPathEditOpener() {}
    virtual ~ ITransitPathEditOpener () {}
public:
    virtual void TransitPathEditFeedback() = 0;
    virtual void TransitPathStationEditFeedback() = 0;
    virtual void TransitSaveFeedback(__int64 nxID = 0) = 0;

    virtual void TransitPathCommand(int nCommand) 
    {   
    }
};
