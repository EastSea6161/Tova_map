/**
 * @file Scenario.h
 * @brief KScenario 선언
 * @author 
 * @date 2010.06.21
 * @remark
 */


#pragma once


//#include "KmzApp.h"
#include "Object.h"
#include "kmz_common.h"
#include "KBulkType.h"
#include "KBulkTypeKTDB.h"

/* forward declarations */
class KProject;
class KTarget;
class KPurpose;
class KMode;
class KODKeyDoubleRecords;

struct TBasicTableData
{
	std::map<Integer/*linkid*/,    TKTDBLinkDB*>	mapLinkRecords;
	std::map<Integer/*nodeid*/,    TKTDBNodeDB*>	mapNodeRecords;
	std::map<Integer/*zoneid*/,    TKTDBZoneDB*>	mapZoneRecords;
	std::map<Integer/*turnid*/,    TKTDBTurnDB>	    mapTurnRecords;
	std::map<Integer/*transitid*/, TKTDBTransitDB > mapTransitRecords;
	std::vector<TKTDBTransitLinkDB>                 vecTransitLinkRecords;
};

struct TBasicPassengerTableData
{
	std::vector<TKTDBColumnDescriptionDB> vecSedVectorColumnDesDB;
	std::vector<TKTDBSedVectorDB>         vecSedVectorRecords;
	std::vector<TKTDBColumnDescriptionDB> vecParaODColumnDesDB;
	std::vector<TKTDBODDB>                vecParameterRecords;
	std::vector<TKTDBParameterODInfoDB>   vecParameterODInfoRecords;
	std::vector<TKTDBSedPopDB>            vecSedPopRecords;
	std::vector<TKTDBCategoryAgeGroupDB>  vecCategoryAgeGroupRecords;
	std::vector<TKTDBCategoryClassDB>     vecCategoryClassRecords;
	std::vector<TKTDBIntersectionDB>      vecIntersectionRecords;
	std::vector<TKTDBDetailTypeDB>        vecDetailTypeRecords; 
	std::vector<TKTDBKCodeGroupDB>        vecKCodeGroupRecords;
	std::vector<TKTDBKCodeDefineDB>       vecKCodeDefineRecords;
	std::vector<TKTDBTerminalScheduleDB>  vecTerminalScheduleRecords;
	std::vector<TKTDBDefineSystemCodeDB>  vecDefineSystemCodeRecords;
	std::vector<TKTDBRegionalModeDB>      vecRegionalModeRecords;
	std::vector<TKTDBTransitCostDB>       vecTransitCostReords;
	std::vector<TKTDBXBusRestDB>          vecXBusRestRecords;
};

struct TBasicFreightTableData
{
	std::vector<TKTDBColumnDescriptionDB> vecSedVectorColumnDesDB;		/*Sed_Vector DB*/
	std::vector<TKTDBSedVectorDB>         vecSedVectorRecords;
	std::vector<TKTDBColumnDescriptionDB> vecParaODColumnDesDB;			/*Parameter_OD DB*/
	std::vector<TKTDBParameterODDB>       vecParameterRecords;
	std::vector<TKTDBParameterODInfoDB>   vecParameterODInfoRecords;
	std::vector<TKTDBColumnDescriptionDB> vecAvrLoadODColumnDesDB;		/*Average_loaddage_OD DB*/
	std::vector<TKTDBODDB>                vecAvrLoadODRecords;
	std::vector<TKTDBColumnDescriptionDB> vecGoodsTransODColumnDesDB;	/*Goods_Transported_OD DB*/
	std::vector<TKTDBODDB>                vecGoodsTransODRecords;
	std::vector<TKTDBColumnDescriptionDB> vecVehicleModeODColumnDesDB;	/*Vehicle_Mode_OD DB*/
	std::vector<TKTDBODDB>                vecVehicleModeODRecords;
	std::vector<TKTDBColumnDescriptionDB> vecEmptyVehicleODColumnDesDB;	/*Empty_vehicle_OD DB*/
	std::vector<TKTDBODDB>                vecEmptyVehicleODRecords;
	std::vector<TKTDBMatchingVehicleConvDB> vecMatchingVehicleConvRecords; 
	std::vector<TKTDBDetailTypeDB>        vecDetailTypeRecords;
	std::vector<TKTDBKCodeGroupDB>        vecKCodeGroupRecords;
	std::vector<TKTDBKCodeDefineDB>       vecKCodeDefineRecords;
	std::vector<TKTDBDefineSystemCodeDB>  vecDefineSystemCodeRecords;
};

/* type definiations */
typedef std::map< unsigned int, KTarget* >                   KTDTargetMap;
typedef std::map< CString     , KODKeyDoubleRecords* >       KTDODKeyDoubleRecordsMap;
typedef std::map< CString     , TBasicTableData* >           KTDBasicTableDataMap;
typedef std::map< CString     , TBasicPassengerTableData* >  KTDBasicPassengerTableDataMap;
typedef std::map< CString     , TBasicFreightTableData* >    KTDBasicFreightTableDataMap;
/**
 * @brief Target 정보를 포함하는 Scenario class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.06.21
 */
class KScenario : public KObject
{
	/**************************************
	* Constructors / Destructor           *
	**************************************/
public:
	KScenario(void);
	~KScenario(void);


	/**************************************
	* Accessors                           *
	**************************************/
public :

	/**
	* @brief Target year 항목을 반환한다.
	* @return KTDTargetMap& - Target year 항목
	*/
	inline const KTDTargetMap& GetTargetMap( void ) const 
	{
		return m_mapTarget;
	}


	/**************************************
	* Operations                          *
	**************************************/
public :

	/**
	* @brief 객체를 초기화 한다.
	* @return bool - 수행 결과
	*/
	bool CreateInitialObject(void);

	/**
	* @brief 새로운 Target year를 생성한다.
	* @param[in] nTargetYear - Target year
	* @param[in] strTargetName - Target year의 명칭 
	* @return KTarget* - Target year
	*/
	KTarget* CreateTarget(int nTargetYear, CString strTargetName);

	/**
	* @brief empty data를 import 한다.
	* @return bool - 수행 결과
	*/
	bool ImportBLANK( void );

	bool ImportSingleTargetBLANK( KTarget* a_pTarget );

	/**
	* @brief 주어진 Scenario로부터 Data를 복사한다.
	* @param[in] pSource - Scenario
	* @param[in] bOverwrite - 덮어쓰기 여부 
	* @return bool - 수행 결과
	*/
	bool CopyFrom( const KScenario* pSource, bool bOverwrite, bool bCopyModelResults = false);

	/**
	* @brief Target year를 삭제하다.
	* @param[in] pTarge - 삭제 할 Target year
	*/
	void removeTarget( KTarget* pTarget );

	/**
	* @brief Target year 항목을 초기화 한다.
	*/
	void ClearTarget( void );

	/**
	* @brief 새로운 Target year를 생성한다.
	* @return bool - 수행 결과
	*/
	bool addNewTarget( void );

	/**
	* @brief Target year를 검색한다.
	* @param[in] strName - Target year 명칭
	* @return KTarget* - Target year
	*/
	KTarget* findTarget( CString strName );

	/**
	* @brief Scenario파일을 읽어온다.
	* @return bool - 수행 결과
	*/
	bool OpenScenario( void );

	/**
	* @brief Scenario를 저장한다.
	*/
	void saveScenario( void );

	/**
	* @brief Scenario의 활성 여부를 반환한다.
	* @return bool - 활성 여부
	*/
	bool IsOpened(void);

	/**
	* @brief 현재 Scenarior가 BASE 인지 여부를 반환 한다. 
	* @return bool - BASE 시나리오 여부 
	*/
	bool IsBaseScenario(void);

	/**
	* @brief Purpose를 설정한다.
	* @param[in] a_vecPurpose - Purpose 배열
	*/
// 	inline void SetPurpose(std::vector<KPurpose*> a_vecPurpose)
// 	{
// 		m_vecPurpose = a_vecPurpose;
// 	}

	/**
	* @brief Mode를 설정한다.
	* @param[in] a_vecMode - Mode 배열
	*/
	inline void SetMode(std::vector<KMode*> a_vecMode)
	{
		m_vecMode = a_vecMode;
	}

protected :
	bool OpenTarget( const CString& strFile );

	bool GetPurposeODColumnName  ( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> &a_vecPurpose,  std::list<CString> &a_lstOldPurposeODColunName,  std::list<CString> &a_lstAddPurposeODColunName );

	bool GetModeODColumnName     ( KDBaseConPtr a_spDBaseConnection, std::vector<KMode*>    &a_vecMode,     std::list<CString> &a_lstOldModeODColunName,     std::list<CString> &a_lstAddModeODColunName );

	bool GetCoefficientColumnName( KDBaseConPtr a_spDBaseConnection, std::vector<KPurpose*> &a_vecPurpose,  std::list<CString> &lstOldCoefficientColunName,  std::list<CString> &a_lstAddCoefficientColunName);

	/**
	* @brief 체크한 년도에 대한 목적, 수단 OD 데이터를 생성 반환 한다. 
	* @param[in] a_strAnalysisArea			- 권역 정보
	* @param[in] a_vecTotalSelectedYear		- 보간을 위한 선택한 모든 '년도' 배열
	* @param[in] a_vecPurpose				- 목적 name 배열
	* @param[in] a_vecMode				    - 수단 name 배열
	* @param[out] a_mapYearsPurposeODData	- 년도 와 PurposeODData 맵핑
	* @param[out] a_mapYearsModeODData	    - 년도 와 ModeODData 맵핑
	*/
	bool CreateEmptyTargetData( CString&	a_strNetworkDataFile, CString& a_strIODataFile, KTarget* pTarget );

	void GetVectorDataYear    ( std::map<int, int> &a_mapDataYear, std::vector<int> &a_vecDataYear );

	bool CopyNetworkData      ( CString &a_strSysBaseDomain, CString &a_strBaseCopyYear, CString &a_strNetworkDataFile);

	bool InsertNewUserPurposeOD( CString& a_strIODataFile, std::vector<KPurpose*>&	a_vecPurpose );

	bool InsertNewUserModeOD   ( CString& a_strIODataFile, std::vector<KMode*>&	a_vecMode    );

	bool InsertNewODColumn( KDBaseConPtr a_spDBaseConnection, CString a_strTableName, CString& a_strColumnName );

	bool InsertNewColumnDescription( KDBaseConPtr a_spDBaseConnection, CString a_strTableName, std::list<CString>& a_lstODColumnName, std::list<CString>& a_lstODDisplayName );

	bool InsertNewDistributionModel( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstPurposeODColumnName,  std::vector<KPurpose*>& a_vecPurpose );

	bool InsertNewModeChoiceModel  ( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstModeODColumnName,     std::vector<KMode*>&    a_vecMode );

	bool InsertNewCoefficientModel( KDBaseConPtr a_spDBaseConnection, std::list<CString>& a_lstCoefficientColumnName, std::vector<KPurpose*>& a_vecPurpose );
	
	bool DeletePurposeODData( CString a_strFileName );

	bool DefinePurposeAndMode( CString& a_strIODataFile );

	bool DefinePurpose( KDBaseConPtr a_spDBaseConnection ); 

	bool DefineMode( KDBaseConPtr a_spDBaseConnection );

	Double GetTargetRecord( Double a_dFirstYear, Double a_dFirstRecord, Double a_dSecondYear, Double a_dSecondRecord, Double a_dTargetYear );

	void ResetScenario(CString a_strFile);

	/**************************************
	* Properties / Variables              *
	**************************************/
protected :
	bool					m_bOpen;
	FILE				   *m_hKSX;
	KTDTargetMap			m_mapTarget;
	//std::vector<KPurpose*>  m_vecPurpose;
	std::vector<KMode*>		m_vecMode;
	std::map<int,int>	    m_mapDeleteYear;

public:
    void Vaccum();
	void CompactGISDB();
};

