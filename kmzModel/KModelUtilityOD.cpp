#include "KModelUtilityOD.h"
#include <numeric>

KModelUtilityOD::KModelUtilityOD(void)
{
}

KModelUtilityOD::~KModelUtilityOD(void)
{
}

void KModelUtilityOD::CopyODData( CAtlODValueMap& _cmOriginOD, 
                                  CAtlODValueMap& _cmDestinationOD
                                )
{
    //^TxLogDebug( _T("start...^^"));

    _cmDestinationOD.RemoveAll();
    POSITION pos = _cmOriginOD.GetStartPosition();	
    CAtlODValueMap::CPair* pair = NULL;

    while (NULL != pos)
    {
        pair = _cmOriginOD.GetNext(pos);
        _cmDestinationOD.SetAt(pair->m_key, pair->m_value);
    }

    //^TxLogDebug( _T("end...^^"));
}

void KModelUtilityOD::SetODValue( CAtlODValueMap& _cmROD, 
                                  Integer         _originID, 
                                  Integer         _destinationID, 
                                  Double          _value
                               )
{
    KMultiID odKey(_originID, _destinationID);

    CAtlODValueMap::CPair* pairOD = NULL;
    pairOD = _cmROD.Lookup(odKey);

    if (NULL != pairOD)
    {
        pairOD->m_value = _value;
    }
}

void KModelUtilityOD::SetODValue( std::map<KODKey, Double>& _cmROD, 
                                  Integer         _originID, 
                                  Integer         _destinationID, 
                                  Double          _value
                                )
{
    KODKey odKey(_originID, _destinationID);

    std::map<KODKey, Double>::iterator iter = _cmROD.find(odKey);
    if (iter != _cmROD.end())
    {
        iter->second = _value;
    }
}


Double KModelUtilityOD::GetODValue( CAtlODValueMap& _cmOD, KMultiID _key )
{
    CAtlODValueMap::CPair* pairOD = NULL;
    pairOD = _cmOD.Lookup(_key);

    if (NULL != pairOD)
    {
        return pairOD->m_value;
    }
    else
    {
        return 0.0;
    }
}

Double KModelUtilityOD::GetODValue (std::map<KODKey, Double>& _smOD, KODKey _key)
{
    std::map<KODKey, Double>::iterator iter = _smOD.find(_key);
    if (iter == _smOD.end())
        return 0.0;

    return iter->second;
}



bool KModelUtilityOD::GetRefODValue( CAtlODValueMap& _cmOD, 
                             Integer        _originID, 
                             Integer        _destinationID, 
                             Double&        rValue
                           )
{
    CAtlODValueMap::CPair* pairOD = NULL;
    pairOD = _cmOD.Lookup(KMultiID(_originID, _destinationID));

    if (NULL != pairOD)
    {
        rValue =  pairOD->m_value;
        return true;
    }
    
    return false;
}