#pragma once

#include <boost/functional/hash.hpp>
#include "KModelDefine.h"


//struct KMultiID
//{
//    union
//    {
//        struct KEY
//        {
//            Integer  FirstID;
//            Integer  SecondID;
//        };
//
//        KEY Key;
//        unsigned __int64  nValue;
//    };
//
//    //ODKEY() : nValue(0) {}
//    //ODKEY(INT64 _value) : nValue(_value) {}
//    KMultiID(Integer _firstID, Integer _secondID)
//    {
//        Key.FirstID   = _firstID;
//        Key.SecondID  = _secondID;
//
//        //nValue = ULONG(_firstID * _secondID);
//    }
//
//    static ULONG gid;
//    // utility function
//    Integer GetOriginID() const      { return Key.FirstID;  }
//    Integer GetDestinationID() const { return Key.SecondID; }
//
//    Integer GetFirstID() const     { return Key.FirstID;  }
//    Integer GetSecondID()const     { return Key.SecondID; }
//};
//
//class KMutiIDTraits : public CElementTraits <KMultiID>
//{
//
//public:
//    static ULONG Hash(const KMultiID& element) throw()
//    {
//        boost::hash<INT64> hasher;        
//        return (ULONG)hasher(element.nValue);
//        //return element.nValue;
//        //return (ULONG)(element.GetFirstID() * element.GetSecondID());
//
//       
//    }
//
//    static bool CompareElements(const KMultiID& a, const KMultiID& b)
//    {
//        return (a.Key.FirstID == b.Key.FirstID && a.Key.SecondID == b.Key.SecondID) ? true : false;
//    };
//};

class KMultiID
{
public:
    Integer  FirstID;
    Integer  SecondID;
    //ULONG    internalKey;
public:
    KMultiID(Integer _firstID, Integer _secondID)
    {
        FirstID   = _firstID;
        SecondID  = _secondID;

        //internalKey = ULONG(_firstID * _secondID);
    }

    KMultiID::KMultiID(const KMultiID& r)
    {
        FirstID  = r.FirstID;
        SecondID = r.SecondID;
    }

    KMultiID& operator=(const KMultiID& r)
    {
        if (&r == this)
        {
            return *this;
        }

        FirstID  = r.FirstID;
        SecondID = r.SecondID;
    }
    Integer GetOriginID() const      { return FirstID;  }
    Integer GetDestinationID() const { return SecondID; }

    Integer GetFirstID() const     { return FirstID;  }
    Integer GetSecondID()const     { return SecondID; }
};

class KMutiIDTraits : public CElementTraits <KMultiID>
{

public:
    static ULONG Hash(const KMultiID& element) throw()
    {
        //return (ULONG)(element.FirstID * element.SecondID);
        return ULONG(element.FirstID * element.SecondID);
    }

    static bool CompareElements(const KMultiID& a, const KMultiID& b)
    {
        return (a.GetFirstID() == b.GetFirstID() && a.GetSecondID() == b.GetSecondID()) ? true : false;
    };
};

