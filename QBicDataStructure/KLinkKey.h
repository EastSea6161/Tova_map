#pragma once
#include "IncludeQBicDataStructure.h"

/**
*@brief 링크 관련 정보
*@details 링크ID, 시작노드ID, 종료노드ID
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class KLinkKey
{
public:
    __int64 LinkID;
    __int64 FromNodeID;
    __int64 ToNodeID;
public:
    KLinkKey(__int64 LinkID =0, __int64 FromNodeID = 0, __int64 ToNodeID = 0);
    ~KLinkKey(void);
public:
    bool operator <  (const KLinkKey& right) const;
    bool operator == (const KLinkKey& right) const;
};

