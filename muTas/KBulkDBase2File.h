/**
 * @file KBulkDBase2File.h
 * @brief DB -> 파일 생성 클래스
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */


#pragma once

#include <sys/stat.h>
#include <fstream>
#include <iterator>
#include <iostream>
#include "KBulkType.h"
#include "DefineNames.h"


/**
* @brief DB자료를 Binary 파일로 변환
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KBulkDBase2File
{
public:
    KBulkDBase2File(void);
    ~KBulkDBase2File(void);
    
public:
    /**
    * @brief 노드 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] strFileName - File Name(Default : node.dat)
    * @return true / false
    */
    static bool ExportDB2NodeFile(KDBaseConPtr a_spDBaseConnection, CString strFileName=_T("node.dat"));

	static bool ExportDB2NodeFileExcludeType(KDBaseConPtr a_spDBaseConnection,std::set<Integer> a_setInCludeNode, CString strFileName =_T("node.dat"));

    static bool ReadNodeFile2Log (CString strFileName=_T("node.dat"));
    
    /**
    * @brief 링크 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name(Default : link.dat)
    * @return true / false
    */
    static bool ExportDB2LinkFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName=_T("link.dat"));

	static bool ExportDB2LinkFileExcludeType(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, std::set<Integer> a_setIncldueLink, CString strFileName=_T("link.dat"));
    
	static bool ExportDB2LinkfileExcludeTypeByPathFind(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, std::set<Integer> a_setIncldueLink, CString strFileName=_T("link.dat"));
	/**
    * @brief 링크 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name(Default : link.dat)
    * @return true / false
    */
    static bool ExportDB2LinkFileB(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName=_T("link.dat"));

	/**
    * @brief 링크 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name(Default : link.dat)
    * @return true / false
    */
    static bool ExportDB2LinkFileC(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName=_T("link.dat"));
	static bool ExportDB2LinkFileForShortestPathTransit(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName=_T("link.dat"));

	static bool ExportDB2LinkFileGeneratorPath(KDBaseConPtr a_spDBaseConnection, KIOColumn* a_pColumn, CString strFileName=_T("link.dat"));

	static bool EXportDB2LinkFileInterGeneratorPath(KDBaseConPtr a_spDBaseConnection, KIOColumn* a_pColumn, CString strFileName=_T("link.dat"));

	static bool ExportDB2LinkFileD(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName=_T("link.dat"));
    /**
    * @brief 링크의 고정 컬럼 정보 조회
    * @return KIOColumnCollection - Column Collection
    */
    static KIOColumns  GetFixedLinkColumns();


    /**
    * @brief OD 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] strTableName - Table Name
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name
    * @return true / false
    */
    static bool ExportDB2ODFile(KDBaseConPtr a_spDBaseConnection, CString strTableName, KIOColumns& userColumnCollection, CString strFileName);

    /**
    * @brief Turn 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] strFileName - File Name(Default : turn.dat)
    * @return true / false
    */
    static bool ExportDB2TurnFile(KDBaseConPtr a_spDBaseConnection, CString strFileName=_T("turn.dat"));

	static bool ExportDB2TurnFileExcludeType(KDBaseConPtr a_spDBaseConnection, std::set<Integer> a_setIncludeTurn,  CString strFileName=_T("turn.dat"));

    /**
    * @brief Transit Line 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] strLineInfoFileName - 노선 File Name(Default : line_info.dat)
    * @param[in] strLineFileName - 노선 상세 File Name(Default : line.dat)
    * @return true / false
    */
    static bool ExportDB2LineFile(KDBaseConPtr a_spDBaseConnection, CString strLineInfoFileName=_T("line_info.dat"), CString strLineFileName=_T("line.dat"));

    /**
    * @brief Transit Line 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] ofs - File
    * @param[in] transitID - 노선 ID
    * @param[in] linkRecords - Link의 시작노드 종료노드 정보
    * @return true / false
    */
    static int  ExportLineDataFile(KDBaseConPtr a_spDBaseConnection, std::ofstream& ofs, Integer transitID, std::map<Integer, TFTNode>& linkRecords);

    /**
    * @brief Transit Line 배정 결과(노선별 이용객수) 파일 Read
    * @param[out] transitRecords - 파일 Read 결과 Set
    * @param[in] strFileName - File
    * @return true / false
    */
    static bool ReadLineOutFile    (std::vector<TFixedTransitFileRead>&     transitRecords,     CString strFileName=_T("line_info_out.dat"));

    /**
    * @brief Transit Line 배정 결과(승차, 하차, 재차) 파일 Read
    * @param[out] transitRecords - 파일 Read 결과 Set
    * @param[in] strFileName - File
    * @return true / false
    */
    static bool ReadLineDataOutFile(std::vector<TFixedTransitLinkFileRead>& transitLineRecords, CString strFileName=_T("line_out.dat"));


	/**
    * @brief SED Vector 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name
    * @return true / false
    */
    static bool ExportDB2SedVecFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName);


	/**
    * @brief zone 파일 생성
    * @param[in] a_spDBaseConnection - DB Connection
    * @param[in] userColumnCollection - Export Column Collection
    * @param[in] strFileName - File Name
    * @return true / false
    */
    static bool ExportDB2ZoneFile(KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString strFileName);

	static bool ExportDB2AccessLineStationFile(KDBaseConPtr a_spDBaseConnection, CString strFileName=_T("AccBusSubwayStopAtZone.dat"));

	static bool ExportDB2AccessStopNodeFile(KDBaseConPtr a_spDBaseConnection, CString strFileName=_T("AccBusSubwayStopAtZone.dat"));

	static bool ExportDB2ZoneIDFile(KDBaseConPtr a_spDBaseConnection,CString strFileName = _T("ZoneID.dat"));
};

