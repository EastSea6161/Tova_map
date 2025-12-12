#pragma once
/**
*@brief ITxOBject [비어있음]
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxObject
{
public:
    ITxObject(__int64 _nxTxObjectID = 0) : TxObjectID(_nxTxObjectID) {};
    virtual ~ITxObject() {};
public:
    __int64 TxObjectID;
};