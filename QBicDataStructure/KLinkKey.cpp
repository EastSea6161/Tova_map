#include "KLinkKey.h"


KLinkKey::KLinkKey(__int64 a_LinkID, __int64 a_FromNodeID , __int64 a_ToNodeID)
{
    LinkID      = a_LinkID;
    FromNodeID  = a_FromNodeID;
    ToNodeID    = a_ToNodeID;
}


KLinkKey::~KLinkKey(void)
{
}


bool KLinkKey::operator<(const KLinkKey &right) const 
{
    if ( LinkID == right.LinkID ) 
    {
        if ( FromNodeID == right.FromNodeID ) 
        {
            return ToNodeID < right.ToNodeID;
        }
        
        return FromNodeID < right.FromNodeID;
    }
    else 
    {
        return LinkID < right.LinkID;
    }
}

bool KLinkKey::operator==(const KLinkKey &right) const 
{
    return (LinkID == right.LinkID && FromNodeID == right.FromNodeID && ToNodeID == right.ToNodeID);
}