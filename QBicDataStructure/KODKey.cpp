#include "KODKey.h"

KODKey::KODKey(__int64 _originID, __int64 _destinationID )
{
    OriginID      = _originID;
    DestinationID = _destinationID;
}

KODKey::~KODKey(void)
{
}

bool KODKey::operator<(const KODKey &right) const 
{
    if ( OriginID == right.OriginID ) 
    {
        return DestinationID < right.DestinationID;
    }
    else 
    {
        return OriginID < right.OriginID;
    }
}

bool KODKey::operator==(const KODKey &right) const 
{
    return (OriginID == right.OriginID && DestinationID == right.DestinationID);
}

void KODKey::WriteBytes( std::ofstream& ofs )
{
    ofs.write( reinterpret_cast<char*>( &OriginID      ), sizeof(__int64) );   
    ofs.write( reinterpret_cast<char*>( &DestinationID ), sizeof(__int64) );  
}

void KODKey::ReadBytes( std::ifstream& ifs )
{
    ifs.read( reinterpret_cast<char*>( &OriginID      ), sizeof(__int64) );
    ifs.read( reinterpret_cast<char*>( &DestinationID ), sizeof(__int64) );
}


KIntKey::KIntKey(int first, int second )
{
    First  = first;
    Second = second;
}

KIntKey::~KIntKey(void)
{
}

bool KIntKey::operator<(const KIntKey &right) const 
{
    if ( First == right.First ) 
    {
        return Second < right.Second;
    }
    else 
    {
        return First < right.First;
    }
}

bool KIntKey::operator==(const KIntKey &right) const 
{
    return (First == right.First && Second == right.Second);
}
