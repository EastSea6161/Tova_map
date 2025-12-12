#pragma once

#include "ITxGisServer.h"
#include "TxMapEnumType.h"
#include "TxLayerInfo.h"
#include "ITxGeometry.h"
#include "ITxMapServer.h"
#include "TxTables.h"

class TxFeatureLayer;
class ThunderMapdrawProcess;

//####################################################################################################################
/**
*@brief SQL Map Server
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/

//★ 대량의 데이터 로딩을 할때 - SQLite(ReleaseMemory 함수)
//★ 지도를 핸들하는데 계속적으로 메모리가 증가되고 떨어지지 않을 경우

class AFX_EXT_CLASS TxSQLMapServer : public ITxMapServer
{
private:
    bool m_bConnected;

public:
    TxSQLMapServer(EnumTxMapDataServerType enServerType, CString strConnection) : ITxMapServer(enServerType, strConnection), m_bConnected(false) 
    {
		if (!(enServerType == TxMapDataServerSqlite || enServerType == TxMapDataServerSQL))
			throw 1;
	}

    virtual ~TxSQLMapServer() {}
private:
    KDBaseConnectionPtr m_spDBConnection;
public:	
	/**
	*@brief 연동
	*@return 성공시 1 , 실패시 0 
	*/
    virtual int Connect();
	/**
	*@brief 연동
	*@param KDBaseConnectinPtr spDBase
	*@return 성공시1, 실패시 0
	*/
    virtual int Connect(KDBaseConnectionPtr spDBase);
public:
    virtual KDBaseConnectionPtr Connection() {
        return m_spDBConnection;
    }
public:
	/**
	*@brief 레이어 정보
	*/
    virtual std::vector<TxLayerInfoPtr> LayerInfo();

public:
	/**
	*@brief 서버데이터 불러오기
	*@param TxLayerInfoPtr spLayerInfo
	*@param TxEnvelope oEnvelope
	*@param std::map<__int64, ITxGeometryPtr>&
	*/
    virtual void LoadServerData(TxLayerInfoPtr spLayerInfo, TxEnvelope oEnvelope, std::map<__int64, ITxGeometryPtr>& );
protected:
		/**
	*@brief 서버데이터 불러오기
	*@param TxLayerInfoPtr spLayerInfo
	*@param TxEnvelope oEnvelope
	*@param std::map<__int64, ITxGeometryPtr>&
	*@param std::map<__int64, ITxGeometryPtr>&, ThunderMapdrawProcess*
	*/
    virtual void LoadServerData(TxLayerInfoPtr spLayerInfo, TxEnvelope oEnvelope, double dMinExtent, std::map<__int64, ITxGeometryPtr>&, ThunderMapdrawProcess* );
public:
		/**
	*@brief 서버데이터 불러오기
	*@param TxLayerInfoPtr spLayerInfo
	*@param std::map<__int64, ITxGeometryPtr>&
	*@return 
	*/
    virtual int  LoadServerData(TxLayerInfoPtr spLayerInfo, std::map<__int64, ITxGeometryPtr>&);
private:
	/**
	*@brief 기하학 정보 불러오기
	*@param CString strLayerName				레이어 명칭
	*@param std::map<__int64, ITxGeometryPtr>&  기하학 종류
	*@return 
	*/
    int LoadGeometry(CString strLayerName, std::map<__int64, ITxGeometryPtr>&);

public:
	/**
	*@brief 기하학 정보 불러오기
	*@param TxLayerInfoPtr spLayerInfo
	*@param __int64 nxID				정보 ID
	*/
    virtual ITxGeometryPtr LoadGeometry(TxLayerInfoPtr spLayerInfo, __int64 nxID);

public:
	/**
	*@brief 시작
	*/
    virtual void BeginTransaction();
	/**
	*@brief 커밋
	*/
    virtual void Commit();
	/**
	*@brief 롤백
	*/
    virtual void Rollback();

private:
	/**
	*@brief 삽입
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*@param ITxGeometryPtr spGeometry
	*/
    virtual void Insert(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry);   
public:
	/**
	*@brief 삽입
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*@param ITxGeometryPtr spGeometry
	*/
    virtual void Insert(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);
    /**
	*@brief 삭제
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*/
	virtual void Delete(TxLayerInfoPtr _spLayerInfo, __int64 nxID);
    /**
	*@brief 수정
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*@param ITxGeometryPtr spGeometry
	*/
	virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry);
    /**
	*@brief 수정
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*@param TxRecord& oRecord
	*/
	virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, TxRecord& oRecord);
	/**
	*@brief 수정
	*@param TxLayerInfoPtr _spLayerInfo
	*@param __int64 nxID				자료 ID
	*@param ITxGeometryPtr spGeometry
	*@param TxRecord& oRecord
	*/
    virtual void Update(TxLayerInfoPtr _spLayerInfo, __int64 nxID, ITxGeometryPtr spGeometry, TxRecord& oRecord);  
	/**
	*@brief 전체삭제
	*@param TxLayerInfoPtr _spLayerInfo
	*/
    virtual void RemoveAll(TxLayerInfoPtr _spLayerInfo);
public:
	/**
	*@brief 레이어 삭제
	*@param CString strLayerName
	*/
    virtual void RemoveLayer(CString strLayerName);
public:
	/**
	*@brief 레이어 추가
	*@param EnumTxGeoType enGeoType
	*@param CString strLayerName	레이어명
	*@param CString strCaption		주석
	*@param CString strTxIDField	필드ID
	*@param TxColumns& oColumns		컬럼명
	*@return 레이어 정보
	*/
    virtual TxLayerInfoPtr AddLayer(EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns);
public:
	/**
	*@brief 컬럼 입력
	*@param	CString strLayerName 레이어 명칭
	*@return 컬럼 반환
	*/
    virtual TxColumns GetColumns(CString strLayerName);
private:
	/**
	*@brief 레이어 추가(Sqlite)
	*@param EnumTxGeoType enGeoType
	*@param CString strLayerName	레이어명
	*@param CString strCaption		주석
	*@param CString strTxIDField	필드ID
	*@param TxColumns& oColumns		컬럼명
	*@return 레이어 정보
	*/
    virtual TxLayerInfoPtr AddLayerSqlite(EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns);
	/**
	*@brief 레이어 추가(MSSql)
	*@param EnumTxGeoType enGeoType
	*@param CString strLayerName	레이어명
	*@param CString strCaption		주석
	*@param CString strTxIDField	필드ID
	*@param TxColumns& oColumns		컬럼명
	*@return 레이어 정보
	*/
    virtual TxLayerInfoPtr AddLayerMSSql (EnumTxGeoType enGeoType, CString strLayerName, CString strCaption, CString strTxIDField, TxColumns& oColumns);

public:
		/**
	*@brief 
	*@param TxLayerInfoPtr spLayerInfo
	*@return 
	*/
    virtual TxEnvelope GetExtent(TxLayerInfoPtr spLayerInfo);
	/**
	*@brief 엔티티 개수
	*@param TxLayerInfoPtr spLayerInfo
	*@return 개수 반환
	*/
    virtual __int64    EntityCount(TxLayerInfoPtr spLayerInfo);

public:
	/**
	*@brief 최대 ID
	*@param TxLayerInfoPtr spLayerInfo
	*@return 
	*/
    virtual __int64 MaxID(TxLayerInfoPtr spLayerInfo);

public:
    virtual void RebuildIndex(TxLayerInfoPtr _spLayerInfo, double a_dExGridSize = 0.0);

friend class TxFeatureLayer;
};

typedef std::shared_ptr<TxSQLMapServer> TxSQLMapServerPtr;