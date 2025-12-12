/**
 * @file KDBaseInspection.h
 * @brief SED 오류 검사 Base Class
 * @author nomplus2@qbicware.com
 * @date 2011.08.26
 * @remark 
 */

#pragma once
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumn.h" 
//^^ #include "IOColumns.h" 



/**
* @brief SED 오류 검사 결과 정보
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class InspectionResultSED
{
public:
    InspectionResultSED() {};
    ~InspectionResultSED() {};
public:
    CString Zone_ID;
    CString Year;
    CString ErrorType;
    CString ColumnName;
    CString Desc;
};

/**
* @brief Zone/Link/Node/Turn 오류 검사 결과 정보
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class InspectionResultOneKey
{
public:
    InspectionResultOneKey() {};
    ~InspectionResultOneKey() {};
public:
    CString ID;
    CString ErrorType;
    CString ColumnName;
    CString Desc;
};


/**
* @brief Transit 오류 검사 결과 정보
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class Transit_InspectionResult
{
public:
    Transit_InspectionResult() {};
    ~Transit_InspectionResult() {};
public:
    CString ID;
    CString Seq;
    CString LinkID;
    CString Desc;
};

/**
* @brief Insepcetion 조회 클래스
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.26
*/
class KDBaseInspection
{
public:
    KDBaseInspection(void);
    ~KDBaseInspection(void);
    // SED
private:
    /**
    * @brief SED 데이터 오류 검증(minus, null)
    * @param[in] pSEDVector - SED(Vector) Table
    * @param[in] pColumn - SED(Vector) Column
    * @param[out] ar_vecResult - SED 오류 결과 Set
    * @return true / false
    */
    static bool CheckSEDMinus(KIOTable* a_pSEDVector, KIOColumn* a_pColumn, std::vector<InspectionResultSED>& ar_vecResult);
public:
    /**
    * @brief SED 데이터 오류 검증(minus, null)
    * @param[in] pSEDVector - SED(Vector) Table
    * @param[out] ar_vecResult - SED 오류 결과 Set
    * @return true / false
    */
    static bool CheckSEDMinus(KIOTable* pSEDVector, std::vector<InspectionResultSED>& ar_vecResult);

private:
    /**
    * @brief SED 코드 데이터 오류 검증(minus, null)
    * @param[in] pSEDVector - SED(Vector) Table
    * @param[in] pColumn - SED(Vector) 코드 Column
    * @param[out] ar_vecResult - SED 코드 오류 결과 Set
    * @return true / false
    */
    static bool CheckSEDCode(KIOTable* pSEDVector, KIOColumn* pColumn, std::vector<InspectionResultSED>& ar_vecResult);
public:
    /**
    * @brief SED 코드 데이터 오류 검증(minus, null)
    * @param[in] pSEDVector - SED(Vector) Table
    * @param[in] pColumn - SED(Vector) 코드 Column
    * @param[out] ar_vecResult - SED 코드 오류 결과 Set
    * @return true / false
    */
    static bool CheckSEDCode(KIOTable* pSEDVector, std::vector<InspectionResultSED>& ar_vecResult);

    // Node
private:
    /**
    * @brief 노드 데이터 오류 검증(null)
    * @param[in] pTable - 노드 Table
    * @param[in] pColumn - 노드 Table Column
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 노드 데이터 오류 검증(null)
    * @param[in] pTable - 노드 Table
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

private:
    /**
    * @brief 노드 데이터 코드 오류 검증
    * @param[in] pTable - 노드 Table
    * @param[in] pColumn - 노드 Table Column
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 노드 데이터 코드 오류 검증
    * @param[in] pTable - 노드 Table
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

public:
    /**
    * @brief 노드 데이터 오류(존 매칭) 검증
    * @param[in] pTable - 노드 Table
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeZoneMatching(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

public:
    /**
    * @brief 노드 데이터 오류(Isolated 노드) 검증
    * @param[in] pTable - 노드 Table
    * @param[out] ar_vecResult - 노드 오류 결과 Set
    * @return true / false
    */
    static bool CheckNodeIsolated(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

    // Link
private:
    /**
    * @brief 링크 데이터 코드 오류(Null, <= 0) 검증
    * @param[in] pTable - 링크 Table
    * @param[in] pColumn - 링크 Table Column
    * @param[out] ar_vecResult - 링크 오류 결과 Set
    * @return true / false
    */
    static bool CheckLinkNullData(KIOTable* a_pTable, KIOColumn* a_pColumn, std::vector<InspectionResultOneKey>& vecResult);
public:
    /**
    * @brief 링크 데이터 코드 오류(Null, <= 0) 검증
    * @param[in] pTable - 링크 Table
    * @param[out] ar_vecResult - 링크 오류 결과 Set
    * @return true / false
    */
    static bool CheckLinkNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

public:
    /**
    * @brief 링크 데이터 코드 오류(링크 연결성(F/T node)) 검증
    * @param[in] pTable - 링크 Table
    * @param[out] ar_vecResult - 링크 오류 결과 Set
    * @return true / false
    */
    static bool CheckLinkFTNode(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

private:
    /**
    * @brief 링크 데이터 코드 오류(코드오류) 검증
    * @param[in] pTable - 링크 Table
    * @param[in] pColumn - 링크 Column
    * @param[out] ar_vecResult - 링크 오류 결과 Set
    * @return true / false
    */
    static bool CheckLinkCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 링크 데이터 코드 오류(코드오류) 검증
    * @param[in] pTable - 링크 Table
    * @param[out] ar_vecResult - 링크 오류 결과 Set
    * @return true / false
    */
    static bool CheckLinkCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

    // Zone
private:
    /**
    * @brief 존 데이터 오류(Null) 검증
    * @param[in] pTable - 존 Table
    * @param[in] pColumn - 존 Column
    * @param[out] ar_vecResult - 존 오류 결과 Set
    * @return true / false
    */
    static bool CheckZoneNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 존 데이터 오류(Null) 검증
    * @param[in] pTable - 존 Table
    * @param[out] ar_vecResult - 존 오류 결과 Set
    * @return true / false
    */
    static bool CheckZoneNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

private:
    /**
    * @brief 존 데이터 코드 오류(Null) 검증
    * @param[in] pTable - 존 Table
    * @param[in] pColumn - 존 Column
    * @param[out] ar_vecResult - 존 오류 결과 Set
    * @return true / false
    */
    static bool CheckZoneCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 존 데이터 코드 오류(Null) 검증
    * @param[in] pTable - 존 Table
    * @param[in] pColumn - 존 Column
    * @param[out] ar_vecResult - 존 오류 결과 Set
    * @return true / false
    */
    static bool CheckZoneCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

    // Turn
private:
    /**
    * @brief 회전정보 데이터 오류(Null) 검증
    * @param[in] pTable - 회전정보 Table
    * @param[in] pColumn - 회전정보 Column
    * @param[out] ar_vecResult - 회전정보 오류 결과 Set
    * @return true / false
    */
    static bool CheckTurnNullData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 회전정보 데이터 오류(Null) 검증
    * @param[in] pTable - 회전정보 Table
    * @param[out] ar_vecResult - 회전정보 오류 결과 Set
    * @return true / false
    */
    static bool CheckTurnNullData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);

private:
    /**
    * @brief 회전정보 코드 오류 검증
    * @param[in] pTable - 회전정보 Table
    * @param[in] pColumn - 회전정보 Column
    * @param[out] ar_vecResult - 회전정보 오류 결과 Set
    * @return true / false
    */
    static bool CheckTurnCodeData(KIOTable* pTable, KIOColumn* pColumn, std::vector<InspectionResultOneKey>& ar_vecResult);
public:
    /**
    * @brief 회전정보 코드 오류 검증
    * @param[in] pTable - 회전정보 Table
    * @param[out] ar_vecResult - 회전정보 오류 결과 Set
    * @return true / false
    */
    static bool CheckTurnCodeData(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);
    /**
    * @brief 회전정보 연결성 오류 검증
    * @param[in] pTable - 회전정보 Table
    * @param[out] ar_vecResult - 회전정보 오류 결과 Set
    * @return true / false
    */
    static bool CheckTurnLogic(KIOTable* pTable, std::vector<InspectionResultOneKey>& ar_vecResult);
};

