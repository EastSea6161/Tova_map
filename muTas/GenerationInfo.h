/**
 * @file GenerationInfo.h
 * @brief KGenerationInfo 선언 파일
 * @author 
 * @date 2011.05.27
 * @remark
 */


#pragma once


#include "model_common.h"


/* Forward declarations */
class KGBaseExpressionContainer;
class KIOColumn;
class KPAPurpose;


/**
 * @brief 발생모형의 수식정보 등을 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.27
 */
class KGenerationInfo
{
public:
	KGenerationInfo(void);
	~KGenerationInfo(void);

public :
	/**
	* @brief 발생모형 구동시 사용 여부를 반환한다. 
	* @return bool - 발생모형 사용 여부
	*/

	bool UseGeneration(void);
	/**
	* @brief 발생모형 구동시 사용 여부를 설정한다. 
	* @param[in] bUse - 발생모형 사용 여부
	* @param[out] 
	* @return 
	* @remark 
	* @warning 
	*/
	void UseGeneration(bool a_bUse);

	//KPurpose* Purpose(void);
	//void Purpose(KPurpose* pPurpose);

	/**
	* @brief 설정된 PA 목적을 반환한다.
	* @return KPAPurpose* - 설정된 목적
	*/
	KPAPurpose* Purpose(void);

	/**
	* @brief PA 목적을 설정한다.
	* @param[in] pPurpose - PA 목적 
	*/
	void Purpose(KPAPurpose* a_pPurpose);

	/**
	* @brief 발생 모형 정보의 명칭을 반환한다.
	* @return LPCTSTR - 발생모형 정보의 명칭
	*/
	LPCTSTR GetName(void);

	/**
	* @brief PA의 종류를 반환한다.
	* @return KEMPAType - PA 종류
	*/
	KEMPAType GetPAType(void);

	/**
	* @brief 목적 그룹의 ID를 반환한다.
	* @return KEMPurposeGroup - 목적 그룹
	*/
	KEMPurposeGroup GetObjectGroupID(void);

	/**
	* @brief 목적의 ID를 반환한다.
	* @return int - 목적 ID
	*/
	int GetObjectID(void);

	/**
	* @brief Zone group 여부를 반환한다.
	* @return bool - Zone group 여부
	*/
	bool ZoneGroup(void);

	/**
	* @brief Zone group 여부를 설정한다.
	* @param[in] bGroup - Zone group 여부
	*/
	void ZoneGroup(bool a_bGroup);

	/**
	* @brief 발생모형의　종류를 반환한다.
	* @return KEMGenerationModel - 발생모형의 종류
	*/
	KEMGenerationModel GenerationModel(void);

	/**
	* @brief 발생모형의 종류를 설정한다.
	* @param[in] emGModel - 발행 모형의 종류
	*/
	void GenerationModel(KEMGenerationModel a_emGModel);


	/**
	* @brief 발생모형의 종류의 이름을 반환한다.
	* @return CString - 발생모형 종류 명칭
	*/
	CString GenerationModelName(void);


	/**
	* @brief 모형 결과를 저장 할 Column 명칭을 반환한다. 
	* @return LPCTSTR - Column의 명칭
	*/
	LPCTSTR OutputColumnName(void);

	/**
	* @brief 모형 결과를 저장 할 Column의 명칭을 설정한다.
	* @param[in] strColumnName - Column의 명칭
	*/
	void OutputColumnName(LPCTSTR a_strColumnName);

	/**
	* @brief 모형 결과를 저장 할 Column의 표출 이름을 반환한다.
	* @return LPCTSTR - Column의 표출 이름
	*/
	LPCTSTR OutputColumnCaption(void);

	/**
	* @brief 모형 결과를 저장 할 Column의 표출 이름을 설정한다.
	* @param[in] strColumnCaption - Column의 표출 이름
	*/
	void OutputColumnCaption(LPCTSTR a_strColumnCaption);

	/**
	* @brief 모형 구동에 사용 할 수식 정보를 반환한다.
	* @return KGBaseExpressionContainer - 수식 정보
	*/
	KGBaseExpressionContainer* ExpressionContainer(void);

	/**
	* @brief 모형 구동에 사용 할 수식 정보를 설정한다.
	* @param[in] pContainer - 수식 정보
	*/
	void ExpressionContainer(KGBaseExpressionContainer* a_pContainer);

	/**
	* @brief 새로운 수식 정보를 생성한다.
	* @return KGBaseExpressionContainer - 생성된 수식 정보
	*/
	KGBaseExpressionContainer* CreateExpression(void);

	/**
	* @brief 설정된 수식 정보를 삭제 한다.
	*/
	void DeleteExpression(void);

    /**
	* @brief IROZoneColumn(Production Column) Get
	* @return KIOColumn
	*/
    KIOColumn* IROProductColumn();

    /**
	* @brief IROZoneColumn(Production Column) Set
	* @return KIOColumn
	*/
    void IROProductColumn(KIOColumn* a_pColumn);

    /**
	* @brief  투입 비율 Column Get
	* @return KIOColumn
	*/
    KIOColumn* IROInputRatioColumn();

    /**
	* @brief 투입 비율 Column Set
	* @return KIOColumn
	*/
    void IROInputRatioColumn(KIOColumn* a_pColumn);

	 /**
	* @brief 권역 비율 Column Set
	* @return KIOColumn
	*/
	KIOColumn* IRORegionRatioColumn();

	 /**
	* @brief 권역 비율 Column Set
	* @return KIOColumn
	*/
	void IRORegionRatioColumn(KIOColumn* a_pColumn);

protected :
	bool m_bUse;
	bool m_bZoneGroup;
	KPAPurpose* m_pPurpose;
	KEMGenerationModel m_emGModel;
	KGBaseExpressionContainer* m_pExpressionContainer;
	CString m_strOutputColumnName;
	CString m_strOutputColumnCaption;

    KIOColumn* m_pIROProductColumn;
    KIOColumn* m_pIROInputRatioColumn;
	KIOColumn* m_pIRORegionRatioColumn;

};

