/**
 * @file DistributionInfo.h
 * @brief KDistributionInfo 선언 파일
 * @author 
 * @date 2011.05.24
 * @remark
 */


#pragma once


#include "model_common.h"


/* Forward declarations */
class KDistributionFunctionBase;
class KIOColumn;
class KScenario;
class KTarget;


/**
 * @brief 분포 모형의 수식 등의 정보를 저장하는 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.24
 */
class KDistributionInfo
{
public:
	KDistributionInfo(void);
	~KDistributionInfo(void);

public :

	/**
	* @brief 분포모형 정보가 구동을 위해 선택되었는지 여부를 반환한다.
	* @return bool - 선택 여부
	*/
	bool Selected(void);

	/**
	* @brief 분포모형 정보의 선택을 설정한다.
	* @param[in] bSelected - 선택 여부
	*/
	void Selected(bool bSelected);

	/**
	* @brief 분포모형의 Group ID를 반환한다.
	* @return int - Group ID
	*/
	int PurposeGroupID(void);

	/**
	* @brief 분포모형의 Group ID를 설정한다.
	* @param[in] nGroupID - Group ID
	*/
	void PurposeGroupID(int nGroupID);

	/**
	* @brief 분포모형 정보의 ID를 반환한다.
	* @return int - 분포모형 정보의 ID
	*/
	int PurposeID(void);

	/**
	* @brief 분포모형 정보의 ID를 설정한다.
	* @param[in] nID - 분포모형 정보의 ID
	*/
	void PurposeID(int nID);

	/**
	* @brief 분포모형 정보의 명칭을 반환한다.
	* @return LPCTSTR - 분포모형 정보의 명칭
	*/
	LPCTSTR PurposeName(void);

	/**
	* @brief 분포모형 정보의 명칭을 설정한다.
	* @param[in] strName - 분포모형 정보의 명칭
	*/
	void PurposeName(LPCTSTR strName);

	/**
	* @brief 분포모형의 종류를 반환한다.
	* @return KEMDistributionModel - 분포모형의 종류
	*/
	KEMDistributionModel ModelType(void);

	/**
	* @brief 분포모형의 종류를 설정한다.
	* @param[in] emModel - 분포모형의 종류
	*/
	void ModelType(KEMDistributionModel emModel);

	/**
	* @brief 분포모형의 Impedance function 종류를 설정하고 생성하다.
	* @param[in] emFunction - Impedance function의 종류
	* @return KDistributionFunctionBase* - 생성된 Impedance function
	*/
	KDistributionFunctionBase* SetImpedanceFunctionType(KEMDistributionFunction emFunction);

	/**
	* @brief 분포모형의 Impedance function을 반환한다.
	* @return KDistributionFunctionBase* - Impedance function
	*/
	KDistributionFunctionBase* ImpedanceFunction(void);

	/**
	* @brief Base OD의 Scenario 정보를 반환한다.
	* @return KScenario* - Scenario 정보
	*/
	KScenario* BaseODScenario(void);

	/**
	* @brief Base OD의 Scenario 정보를 설정한다.
	* @param[in] pScenario - Scenario 정보
	*/
	void BaseODScenario(KScenario* pScenario);

	/**
	* @brief Base OD의 Target year 정보를 반환한다.
	* @return KTarget* - Target year 정보
	*/
	KTarget* BaseODTarget(void);

	/**
	* @brief Base OD의 Target year 정보를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void BaseODTarget(KTarget* pTarget);

	/**
	* @brief Base OD의 column 정보를 반환한다.
	* @return KIOColumn* - Column 정보
	*/
	KIOColumn* BaseODColumn(void);

	/**
	* @brief Base OD의 column 정보를 설정한다.
	* @param[in] pColumn - Column 정보
	*/
	void BaseODColumn(KIOColumn* pColumn);

	/**
	* @brief Production 입력 Column의 명칭을 반환한다.
	* @return LPCTSTR - Production 입력 Column의 명칭
	*/
	LPCTSTR ZoneProductionColumnName(void);

	/**
	* @brief Production 입력 Column의 명칭을 설정한다.
	* @param[in] strName - Production 입력 Column의 명칭
	*/
	void ZoneProductionColumnName(LPCTSTR strName);

	/**
	* @brief Production 입력 Column의 표출 명칭을 반환한다.
	* @return LPCTSTR - Production 입력 Column의 표출 명칭
	*/
	LPCTSTR ZoneProductionColumnCaption(void);

	/**
	* @brief Production 입력 Column의 표출 명칭을 설정한다.
	* @param[in] strCaption - Production 입력 Column의 표출 명칭
	*/
	void ZoneProductionColumnCaption(LPCTSTR strCaption);

	/**
	* @brief Attraction 입력 Column의 명칭을 반환한다.
	* @return LPCTSTR - Attraction 입력 Column의 명칭
	*/
	LPCTSTR ZoneAttractionColumnName(void);

	/**
	* @brief Attraction 입력 Column의 명칭을 설정한다.
	* @param[in] strName - Attraction 입력 Column의 명칭
	*/
	void ZoneAttractionColumnName(LPCTSTR strName);

	/**
	* @brief Attraction 입력 Column의 표출 명칭을 반환한다.
	* @return LPCTSTR - Attraction 입력 Column의 표출 명칭
	*/
	LPCTSTR ZoneAttractionColumnCaption(void);

	/**
	* @brief Attraction 입력 Column의 표출 명칭을 설정한다.
	* @param[in] strCaption - Attraction 입력 Column의 표출 명칭
	*/
	void ZoneAttractionColumnCaption(LPCTSTR strCaption);

	/**
	* @brief 분포모형 결과의 저장 Column 명칭을 반환한다.
	* @return LPCTSTR - Column 명칭
	*/
	LPCTSTR OutputPurposeODColumnName(void);

	/**
	* @brief 분포모형 결과의 저장 Column 명칭을 설정한다.
	* @param[in] strName - Column 명칭
	*/
	void OutputPurposeODColumnName(LPCTSTR strName);

	/**
	* @brief 분포모형 결과의 저장 Column 표출 명칭을 반환한다.
	* @return LPCTSTR - Column 표출 명칭
	*/
	LPCTSTR OutputPurposeODColumnCaption(void);

	/**
	* @brief 분포모형 결과의 저장 Column 표출 명칭을 설정한다.
	* @param[in] strCaption - Column의 표출 명칭
	*/
	void OutputPurposeODColumnCaption(LPCTSTR strCaption);

    void StartTime(LPCTSTR a_strStartTime);
    LPCTSTR StartTime();

    void EndTime(LPCTSTR a_strEndTime);
    LPCTSTR EndTime();

protected :
	bool m_bSelected;
	int m_nInfoGroupID;
	int m_nInfoID;
	CString m_strInfoName;
	KEMDistributionModel m_emModel;
	KDistributionFunctionBase* m_pFunction;

	// BaseOD(Fratar model일 경우)
	KScenario* m_pScenario;
	KTarget*   m_pTarget;
	KIOColumn* m_pColumn;

	// Input
	CString m_strPInputName;
	CString m_strPInputCaption;
	CString m_strAInputName;
	CString m_strAinputCaption;

	// Output
	CString m_strOutputColumnName;
	CString m_strOutputColumnCaption;

    // 수행시간 정보
    CString m_strStartTime;
    CString m_strEndTime;
};

