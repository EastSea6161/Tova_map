/**
 * @file ARunningOptionInfo.h
 * @brief KARunningOptionInfo 선언 파일
 * @author 
 * @date 2011.06.21
 * @remark
 */


#pragma once


/* Forward declaraions */
class KIOTable;
class KTarget;
class KScenario;

/**
 * @brief 배정 모형의 Running Options 설정 정보 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.21
 */
class KARunningOptionInfo
{
public:
	KARunningOptionInfo(void);
	KARunningOptionInfo(const KARunningOptionInfo& r);
	~KARunningOptionInfo(void);

public :
	KARunningOptionInfo& operator=(const KARunningOptionInfo& r);

	/**
	* @brief 회전제약 정보 포함 여부를 반환한다.
	* @return bool - 회전제약 정보 포함 여부
	*/
	bool IncludeTurnRestriction(void);

	/**
	* @brief 회전제약 정보 포함 여부를 설정한다.
	* @param[in] bInclude - 회전제약 정보 포함 여부
	*/
	void IncludeTurnRestriction(bool bInclude);

	/**
	* @brief 회전제약 정보 Table을 반환한다.
	* @return KIOTable* - 회전제약 정보 Table
	*/
	//KIOTable* TurnRestrictionData(void);

	/**
	* @brief 회전제약 정보 Table을 설정한다.
	* @param[in] pTable - 회전제약 정보 Table
	*/
	//void TurnRestrictionData(KIOTable* pTable);

	/**
	* @brief Bus Initial Volume 데이터 포함 여부를 반환한다.
	* @return bool - Bus Initial Volume 포함 여부
	*/
	bool IncludeBusInitialVolume(void);

	/**
	* @brief Bus Initial Volume 포함 여부를 설정한다.
	* @param[in] bInclude - Bus Initial Volume 포함 여부
	*/
	void IncludeBusInitialVolume(bool bInclude);

	/**
	* @brief Bus Initial Volume Table을 반환한다.
	* @return KIOTable - Bus Initial Volume Table
	*/
	KIOTable* BusInitialVolumeData(void);

	/**
	* @brief Bus Initial Volume Table을 설정한다.
	* @param[in] pTable - Bus Initial Volume table
	*/
	void BusInitialVolumeData(KIOTable* pTable);

	/**
	* @brief Path Analysis 적용 여부를 반환한다.
	* @return bool - Path Analysis 적용 여부
	*/
	bool IncludePathAnalysis(void);

	/**
	* @brief Path Analysis 적용 여부를 설정한다.
	* @return bInclude - Path Analysis 적용 여부
	*/
	void IncludePathAnalysis(bool bInclude);

	/**
	* @brief Path Analysis Table을 반환한다.
	* @return KIOTable* - Path Analysis table
	*/
	//KIOTable* PathAnalysisData(void);
	
	/**
	* @brief Path Analysis Table을 설정한다.
	* @return pTable - Path Analysis table
	*/
	//void PathAnalysisData(KIOTable* pTable);

	/**
	* @brief Intersection Delay 적용 여부를 반환한다.
	* @return bool - Intersection Delay 적용 여부
	*/
	bool IncludeIntersectionDelay(void);
	
	/**
	* @brief Intersection Delay 적용 여부를 설정한다.
	* @return bInclude - Intersection Delay 적용 여부
	*/
	void IncludeIntersectionDelay(bool bInclude);
	
	/**
	* @brief Intersection Delay table을 반환한다.
	* @return KIOTable* - Intersection Delay table
	*/
	//KIOTable* IntersectionDelayData(void);
	
	/**
	* @brief Intersection Delay table을 설정한다.
	* @return pTable - Intersection Delay table
	*/
	//void IntersectionDelayData(KIOTable* pTable);

    /**
    * @brief 모형 argument Get
    */
    CString GetArgumentInfo();

	bool IncludeWarmStart(void);

	void IncludeWarmStart(bool bInclude);

	bool IncludeTurnVolume(void);

	void IncludeTurnVolume(bool bTurnVolume);

	bool IncludeBalancing(void);

	void IncludeBalancing(bool bBalancing);

public:
	inline void Target(KTarget* a_pTarget)
	{
		m_pTarget = a_pTarget;
	}

	inline KTarget* Target()
	{
		return m_pTarget;
	}

	inline void Scnario(KScenario* a_pScnario)
	{
		m_pScnario = a_pScnario;
	}

	inline KScenario* Scnario(void)
	{
		return m_pScnario;
	}

public:
	int		m_nIteration;
	double	m_dMaximumPathRate;

protected :
	bool m_bTurnRestriction;
	bool m_bBusInitialVolume;
	bool m_bPathAnalysis;
	bool m_bIntersectionDelay;
	bool m_bIncludeWarmStart;
	bool m_bIncludeTurnVolume;
	bool m_bIncludeBalancing;


	KTarget*	m_pTarget;
	KScenario* m_pScnario;
	/*KIOTable* m_pTurnRestriction;
	KIOTable* m_pBusInitialVolume;
	KIOTable* m_pPathAnalysis;
	KIOTable* m_pIntersectionDelay;*/
};

