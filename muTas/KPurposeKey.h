/**
 * @file KPurposeKey.h
 * @brief KPurposeKey
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once



/**
* @brief KPurposeKey
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KPurposeKey
{
public:
    KPurposeKey(int groucode, int objectid, int paKey);
    ~KPurposeKey(void);
public:
    int GroupCode;
    int ObjectID;
    int PAKey;
public:
    bool operator <  (const KPurposeKey& right) const;
    bool operator == (const KPurposeKey& right) const;
};

