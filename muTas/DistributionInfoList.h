/**
 * @file DistributionInfoList.h
 * @brief KDistributionInfoList 선언 파일
 * @author 
 * @date 2011.05.31
 * @remark
 */


#pragma once


/* Forward declarations */
class KDistributionInfo;


/**
 * @brief 분포모형을 설정하는 다이얼로그에서 DistributionInfo를 저장하기 위한 Interface class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.31
 */
class KDistributionInfoList
{
public:
	KDistributionInfoList(void);
	~KDistributionInfoList(void);

public :
	/**
	 * @brief 분포 모형에서 Output을 설정 할 목적/수단 정보를 추가한다.
	 * @param[in] pInfo - 분포 모델의 목적/수단 정보
	 */
	void AddDistributionInfo(KDistributionInfo* pInfo);

	/**
	 * @brief 분포 모형의 목적/수단 정보를 주어진 list로 설정한다.
	 * @param[in] infoList - 분포 모형의 목적/수단 정보 list
	 */
	void SetDistributionInfo(std::list<KDistributionInfo*>& infoList);

protected :
	std::list<KDistributionInfo*> m_DInfoList;
};

