#include "StdAfx.h"
#include "KPurposeKey.h"


KPurposeKey::KPurposeKey(int groucode, int objectid, int paKey) : GroupCode(groucode), ObjectID(objectid), PAKey(paKey)
{
}


KPurposeKey::~KPurposeKey(void)
{
}

bool KPurposeKey::operator<(const KPurposeKey &right) const 
{
    if ( GroupCode == right.GroupCode ) 
    {
        if (ObjectID == right.ObjectID)
        {
            return PAKey < right.PAKey;
        }
        else
        {
            return ObjectID < right.ObjectID;
        }        
    }
    else 
    {
        return GroupCode < right.GroupCode;
    }
}

bool KPurposeKey::operator==(const KPurposeKey &right) const 
{
    return (GroupCode == right.GroupCode && ObjectID == right.ObjectID && PAKey == right.PAKey);
}