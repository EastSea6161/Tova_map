/**
 * @file KBulkDBase.h
 * @brief DB 조회(대용량) Utility
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once
#include "stdafx.h"
#include "../kmzModel/KDistributionData.h"

#include "KBulkType.h"
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumn.h" 
//^^ #include "IOColumns.h" 

/**
* @brief DB 조회(대용량) Utility
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KBulkDBase
{
public:
    KBulkDBase();
    ~KBulkDBase(void);
public:

    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] columnCollection - Column Collection
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkOneKeySelect ( CString keyField, KIOTable* pIOTable, KIOColumns& columnCollection,       KIDKeyDoubleRecords& records, CString strWhere = _T("") );
    
    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] strColumnsArray - Column Name Array
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkOneKeySelect ( CString keyField, KIOTable* pIOTable, CStringArray& strColumnsArray,               KIDKeyDoubleRecords& records, CString strWhere = _T("") );
    
    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkOneKeySelect ( CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, double >& records, CString strWhere = _T(""));

    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return 
    */
    static bool BulkOneKeySelect ( CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, int    >& records, CString strWhere = _T(""));

    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkOneKeySelect ( CString keyField, KIOTable* pIOTable, CString fieldName, std::map<Integer, CString>& records, CString strWhere = _T(""));

    /**
    * @brief Primary Key가 1개인 데이터 조회
    * @param[in] keyField - PK Field Name
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkOneKeyUpdate ( CString keyField, KIOTable* pIOTable, KIOColumns& columnCollection,       KIDKeyDoubleRecords& records, bool bAutoCommit = true );

	static void BulkOneKeyUpdate ( KDBaseConPtr spDBaseConnection, 
			CString a_strKeyField, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& a_records);

public:
    // Zone, Node, Link 컬럼 정보에 Primary Key를 포함하지 않는다.

    /**
    * @brief 존 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] strProductionColumnName - Production Column Name
    * @param[in] strAttractionColumnName - Attraction Column Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkZoneSelect  ( KIOTable* pIOTable, 
                                  CString   strProductionColumnName, 
                                  CString   strAttractionColumnName, KDistributionZoneDataCollection& records );

    /**
    * @brief 존 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] KIOColumnCollection - Column Collection
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkZoneSelect  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords&        records, CString strWhere = _T("") );

    /**
    * @brief 존 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkZoneSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, double >& records, CString strWhere = _T("") );

    /**
    * @brief 존 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkZoneSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, int    >& records, CString strWhere = _T("") );

    /**
    * @brief 존 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkZoneSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, CString>& records, CString strWhere = _T("") );
    //static bool BulkZoneIDDataZeroSelect ();

    /**
    * @brief 노드 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] columnCollection - Column Collection
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkNodeSelect  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records );

    /**
    * @brief 노드 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkNodeSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, double >& records );


	static bool BulkNodeSelectAll (KTarget* a_pTarget, std::map<Integer,TFixedNodeInfo>& ar_mapNode);

    /**
    * @brief 노드 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkNodeSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, int    >& records );

    /**
    * @brief 노드 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkNodeSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, CString>& records );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] columnCollection - Column Collection
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere = _T("") );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] strColumnsArray - Column Name Array
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, CStringArray& strColumnsArray,         KIDKeyDoubleRecords& records, CString strWhere = _T("") );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, double >& records );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, int    >& records );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[in] fieldName - Field Name
    * @param[out] records - 조회 결과
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, CString              fieldName,        std::map<Integer, CString>& records );

    /**
    * @brief 링크 데이터 조회
    * @param[in] pIOTable - Table
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkLinkSelect  ( KIOTable* pIOTable, std::map<Integer, TFixedLinkDB>& records, CString strWhere = _T("") );

	/**
    * @brief 투입계수(화물IRO) 조회
    * @param[in] pIOTable - Table
    * @param[in] columnCollection - Column Collection
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkInputCoeff  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere = _T(""));
    
    /**
    * @brief 링크ID / FromNode, TNode 조회
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[out] records - 조회 결과
    * @param[in] strWhere - 조회 조건
    * @return true / false
    */
    static bool BulkLinkFTNodeSelect ( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TFTNode>& records );

    /**
    * @brief 존 정보 업데이트
    * @param[in] pIOTable - 존 Table
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 입력 데이터
    * @param[in] bAutoCommit - Commit 여부
    * @return true / false
    */
    static bool BulkZoneUpdate  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit = true );

    /**
    * @brief 존 정보 업데이트
    * @param[in] pIOTable - 존 Table
    * @param[in] pColumn - Column
    * @param[in] records - 입력 데이터
    * @param[in] bAutoCommit - Commit 여부
    * @return true / false
    */
    static bool BulkZoneUpdate  ( KIOTable* pIOTable, KIOColumn* pColumn, std::map<Integer/*zone_id*/, double>& records,   bool bAutoCommit = true );

    /**
    * @brief 노드 정보 업데이트
    * @param[in] pIOTable - 노드 Table
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 입력 데이터
    * @param[in] bAutoCommit - Commit 여부
    * @return true / false
    */
    static bool BulkNodeUpdate  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit = true );

	static void BulkTerminalResultUpdata( KDBaseConPtr a_spDBaseConnection, KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records );

	static void BulkNodeUpdata (KDBaseConPtr spDBaseConnection, 
		KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records);

    /**
    * @brief 링크 정보 업데이트
    * @param[in] pIOTable - 링크 Table
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 입력 데이터
    * @param[in] bAutoCommit - Commit 여부
    * @return true / false
    */
    static bool BulkLinkUpdate  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, bool bAutoCommit = true );

	static void BulkLinkUpdate	( KDBaseConPtr spDBaseConnection, 
			KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records);

	static void BulkLineinfoUpdate	( KDBaseConPtr spDBaseConnection, 
		KIOTable* a_pIOTable, KIOColumns& a_columnCollection, KIDKeyDoubleRecords& records);


    /**
    * @brief OD 정보 선택
    * @param[in] pIOTable - OD Table
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkODSelect ( KIOTable* pIOTable, KIOColumns& columnCollection, KODKeyDoubleRecords& records );

    /**
    * @brief OD 정보 선택
    * @param[in] pIOTable - OD Table
    * @param[in] strFilter - 조건 문
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkODSelect ( KIOTable* pIOTable, CString strFilter,     KIOColumns& columnCollection, KODKeyDoubleRecords& records );    

    /**
    * @brief OD 정보 선택
    * @param[in] pIOTable - OD Table
    * @param[in] strColumnName - 컬럼명
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkODSelect ( KIOTable* pIOTable, CString strColumnName, std::map<KODKey, double>& records );

    /**
    * @brief OD 정보 선택
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] strTableName - Table Name
    * @param[in] columnCollection - Column Collection
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkODSelect ( KDBaseConPtr a_spDBaseConnection,      CString strTableName,  KIOColumns& columnCollection, KODKeyDoubleRecords& records );

	static bool BulkRoadRailTransportODSelect (KDBaseConPtr a_spDBaseConnection, int a_nPurposeID, KODKeyIntegerRecords& records);
    
	static bool BulkODSelectByModeChoice( KIOTable* pIOTable, CString strFilter, KIOColumns& columnCollection, KODKeyDoubleRecords& records );

    /**
    * @brief OD 정보 Update And Insert
    * @param[in] pIOTable - OD Table
    * @param[in] upsertColumnCollection - Column Collection
    * @param[in] upsertRecords - 입력 데이터 Set
    * @param[in] bAutoCommit - Commit 여부
    * @return true / false
    */
    static bool BulkODUpsert ( KIOTable* pIOTable, KIOColumns& upsertColumnCollection, KODKeyDoubleRecords& upsertRecords, bool bAutoCommit = true ); 
    //static bool BulkODUpsertInputRemove ( KIOTable* a_pIOTable, KIOColumns& a_rUpsertColumnCollection, KODKeyDoubleRecords& a_rUpsertRecords, bool a_bAutoCommit = true ); 
 
    /**
    * @brief Taransit DB 조회
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkTransitSelect ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitDB>& records);

    /**
    * @brief Taransit 기본정보 DB 조회
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] records - 결과 데이터(File 생성) Set
    * @return true / false
    */
    static bool BulkTransitSelect ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitFile>& records);
    
    /**
    * @brief Transit_Links 정보 DB 조회
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] records - 결과 데이터 Set
    * @param[in] transit_id - 노선ID
    * @return true / false
    */
    static bool BulkTransitLinksSelect ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedTransitLink>& records, Integer transit_id );
    
	/**
	* @brief 회전 정보 DB 조회
	* @param[in] a_spDBaseConnection - DB Connection
	* @param[in] records - 입력 데이터 Set
	* @return true / false
	*/
	static bool BulkTurnSelect ( KDBaseConPtr a_spDBaseConnection, std::map<Integer, TFixedTurnDB>& records );


    /**
    * @brief Sed_Pop 정보 조회
    * @param[in] year - 년도
    * @param[in] records - 결과 데이터 Set
    * @return true / false
    */
    static bool BulkSedPopSelect  ( KDBaseConPtr a_spDBaseConnection, int year, std::vector<TFixedSEDPop>& records);

	static bool BulkSedPopSelect  ( KDBaseConPtr a_spDBaseConnection, std::vector<TFixedSEDPop>& records);
    
    /**
    * @brief 영향권 설정 Off
    * @param[in] a_spDBaseConnection - DB Connection
    * @return true / false
    */
    static bool BulkLinkInfluenceOff (KDBaseConPtr a_spDBaseConnection);

	static bool BulkSEDVectorSelect (KDBaseConPtr a_spDBaseConnection, std::vector<TFixedSEDVector>& records);

	static bool BulkSEDVectorSelect  ( KIOTable* pIOTable, KIOColumns& columnCollection, KIDKeyDoubleRecords& records, CString strWhere = _T("") );
	
};

