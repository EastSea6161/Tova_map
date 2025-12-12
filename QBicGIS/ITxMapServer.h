#pragma once

#include "ITxGisServer.h"
#include "TxMapEnumType.h"
#include "TxLayerInfo.h"
#include "ITxGeometry.h"

class TxFeatureLayer;
class ThunderMapdrawProcess;
class TxColumns;
class TxRecord;
class TxItem;
/**
*@brief TxMapServer
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ITxMapServer : public ITxGisServer
{
public:
    ITxMapServer(EnumTxMapDataServerType enMapServerType, CString strConnectionString) 
    : m_enMapServerType(enMapServerType), m_strConnectionString(strConnectionString)
    {
    }
    
    virtual ~ITxMapServer(){}

protected:
    EnumTxMapDataServerType m_enMapServerType;
public:
	/**
	*@brief 서버 타입
	*@return 서버 타입 반환
	*/
    EnumTxMapDataServerType ServerType() {
        return m_enMapServerType;
    }

protected:
    CString m_strConnectionString;

public:
	/**
	*@brief 관련 문자
	*@param CString strConnection
	*@return 
	*/
    void ConnectionString(CString strConnection)
    {
        m_strConnectionString = strConnection;
    }
	/**
	*@brief 관련 문자
	*@return 
	*/
    CString ConnectionString()
    {
        return m_strConnectionString;
    }
public:
    virtual KDBaseConnectionPtr Connection() = 0;
public:
    virtual int Connect() = 0;
    virtual int Connect(KDBaseConnectionPtr spDBase) = 0;
    virtual std::vector<TxLayerInfoPtr> LayerInfo() = 0;

public:
    virtual void LoadServerData(TxLayerInfoPtr spLayerInfo, TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& ) = 0;
    virtual int  LoadServerData(TxLayerInfoPtr spLayerInfo, std::map<__int64, ITxGeometryPtr>& ) = 0;
protected:
    virtual void LoadServerData(TxLayerInfoPtr spLayerInfo, TxEnvelope oEnvelope, double dMinExtent, std::map<__int64, ITxGeometryPtr>&, ThunderMapdrawProcess* ) = 0;
public:
    virtual ITxGeometryPtr LoadGeometry(TxLayerInfoPtr spLayerInfo, __int64 nxID) = 0;

public:
    virtual void BeginTransaction() = 0;
    virtual void Commit() = 0;
    virtual void Rollback() = 0;

private:
    virtual void Insert(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry) = 0;
public:
    virtual void Insert(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord) = 0;
    virtual void Delete(TxLayerInfoPtr _spLayerInfo, __int64 nxID) = 0;
    virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry) = 0;
    virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, TxRecord& oRecord) = 0;
    virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord) = 0;    
    virtual void RemoveAll(TxLayerInfoPtr _spLayerInfo)=0;
public:
    virtual void RemoveLayer(CString strLayerName)=0;
public:
    virtual TxLayerInfoPtr AddLayer(EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns)=0;

public:
    virtual TxEnvelope GetExtent  (TxLayerInfoPtr spLayerInfo) = 0;
    virtual __int64    EntityCount(TxLayerInfoPtr spLayerInfo) = 0;

public:
    virtual void RebuildIndex(TxLayerInfoPtr _spLayerInfo, double a_dExGridSize = 0.0) = 0;
friend class TxFeatureLayer;
};

typedef std::shared_ptr<ITxMapServer> ITxMapServerPtr;



