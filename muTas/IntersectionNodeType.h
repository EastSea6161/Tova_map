#pragma once

class KIntersectionNodeType
{
public:
    KIntersectionNodeType()
    {
        TNodeID         = 0;
        TDirectionCount = 0;
    }
    KIntersectionNodeType(Integer a_nxNodeID, int a_nDirectionCount)
    {
        TNodeID         = a_nxNodeID;
        TDirectionCount = a_nDirectionCount;
    }

    ~KIntersectionNodeType()
    {

    }
public:
    Integer TNodeID;
    int     TDirectionCount;
};

class KLinkIDGeometry
{
public:
    KLinkIDGeometry(){};
    KLinkIDGeometry(Integer a_nxLinkID, ITxGeometryPtr a_spGeometry)
    {
        TLinkID   = a_nxLinkID;
        TGeometry = a_spGeometry;
    }
    ~KLinkIDGeometry(){};

public:
    Integer      TLinkID;
    ITxGeometryPtr TGeometry;
    double       TAngle;

    CString      ToString()
    {
        CString  strOut;
        strOut.Format(_T("%I64d[%.2f]"), TLinkID, TAngle);

        return strOut;
    }
};