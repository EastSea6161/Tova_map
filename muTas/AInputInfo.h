/**
 * @file AInputInfo.h
 * @brief KAInputInfo 선언 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#pragma once


/* Forward declarations */
class KIOColumn;
class KMode;


/**
 * @brief 배정 모형의 Input 정보 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.17
 */
class KAInputInfo
{
public:
	KAInputInfo(void);
	KAInputInfo(const KAInputInfo& r);
	~KAInputInfo(void);


public :
	KAInputInfo& operator=(const KAInputInfo& r);

	
	/**
	* @brief 입력정보의 선택 여부를 반환한다.
	* @return bool - 선택 여부
	*/
	bool Selected(void);

	/**
	* @brief 입력정보의 선택 여부를 설정한다.
	* @param[in] bSelected - 선택 여부
	*/
	void Selected(bool bSelected);

	/**
	* @brief Network 종류를 반환한다.
	* @return int - Network 종류(1 : Highway, 2 : Transit, 3 : Rail)
	*/
    int NetworkType();

	/**
	* @brief Network 종류를 설정한다.
	* @param[in] nType - Network 종류(1 : Highway, 2 : Transit, 3 : Rail)
	*/
    void NetworkType(int nType);

	// deprecated : m_bHighway, m_bTransit

	/**
	* @deprecated : 
	* @brief Highway Network 여부를 반환한다.
	* @return bool - Highway Network 여부
	*/
    bool Highway(void);

	/**
	* @deprecated : 
	* @brief Highway Network 여부를 설정한다.
	* @param[in] bHighway - Highway Network 여부
	*/
	void Highway(bool bHighway);

	/**
	* @deprecated : 
	* @brief Transit Network 여부를 반환한다.
	* @return bool - Transit Network 여부
	*/
	bool Transit(void);
	
	/**
	* @deprecated : 
	* @brief Transit Network 여부를 설정한다.
	* @param[in] bTransit - Transit Network 여부
	*/
	void Transit(bool bTransit);

	/**
	* @brief PreLoad 적용 여부를 반환한다.
	* @return bool - PreLoad 적용 여부
	*/
	bool PreLoad(void);

	/**
	* @brief PreLoad 적용 여부를 설정한다.
	* @param[in] bPreLoad - PreLoad 적용 여부
	*/
	void PreLoad(bool bPreLoad);

	/**
	* @brief 점유율을 반환한다.
	* @return double - 점유율
	*/
	double Occupancy(void);

	/**
	* @brief 점유율을 설정한다.
	* @param[in] dOccupancy - 점유율
	*/
	void Occupancy(double dOccupancy);

	/**
	* @brief PCU를 반환한다.
	* @return double - PCU
	*/
	double PCU(void);

	/**
	* @brief PCU를 설정한다.
	* @param[in] dPCU - PCU
	*/
	void PCU(double dPCU);

	/**
	* @brief 수단을 반환한다.
	* @return KMode* - 수단 정보
	*/
	KMode* Mode(void);

	/**
	* @brief 수단을 설정한다.
	* @param[in] pMode - 수단 정보
	*/
	void Mode(KMode* pMode);

	/**
	* @brief 입력 Column을 반환한다.
	* @return KIOColumn* - Column 정보
	*/
	KIOColumn* InputColumn(void);

	/**
	* @brief 입력 Column을 설정한다.
	* @param[in] pColumn - Column 정보
	*/
	void InputColumn(KIOColumn* pColumn);
        
    /**
    * @brief Argument String Get
    */
    CString GetArgumentInfo(int index);

	CString GetOBArgumentInfo(int index, std::map<int, int> a_mapVDF, int nVDFCount, int nModeID);

protected :
	bool m_bSelected;
    // deprecated : m_bHighway, m_bTransit
    int  m_nNetworkType;
	bool m_bHighway;
	bool m_bTransit;
	bool m_bPreLoad;
	double m_dOccupancy;
	double m_dPCU;
	KMode* m_pMode;
	KIOColumn* m_pInputColumn;
};

