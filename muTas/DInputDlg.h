/**
 * @file DInputDlg.h
 * @brief KDInputDlg 선언 파일
 * @author 
 * @date 2011.05.19
 * @remark
 */


#pragma once


#include "DistributionInfoList.h"


/* Forward declarations */
class KDistributionInfo;
class KPAPurpose;
//class KPurpose;
class KIOTable;
class KTarget;


/**
 * @brief 분포 모형의 Input 설정 다이얼로그 class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.19
 */
class KDInputDlg : public KDialogEx, public KDistributionInfoList
{
	DECLARE_DYNAMIC(KDInputDlg)

public:
	KDInputDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6300_DInputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	///**
	// * @brief 발생 모형에서 설정 할 목적을 추가한다.
	// * @param[in] pPurpose - 목적 정보
	// */
	//void AddModelPurpose(KPurpose* pPurpose);
	/**
	 * @brief 발생 모형에서 설정 할 목적을 추가한다.
	 * @param[in] pPurpose - 목적 정보
	 */
	void AddModelPurpose(KPAPurpose* pPurpose);

	///**
	// * @brief 목적 정보를 삭제한다.
	// * @param[in] pPurpose - 삭제 할 목적 정보
	// */
	//void RemoveModelPurpose(KPurpose* pPurpose);
	/**
	 * @brief 목적 정보를 삭제한다.
	 * @param[in] pPurpose - 삭제 할 목적 정보
	 */
	void RemoveModelPurpose(KPAPurpose* pPurpose);

	/**
	 * @brief 모든 목적 정보를 삭제 한다.
	 */
	void RemoveAllModelPurpose(void);

	///**
	// * @brief 목적 정보를 주어진 list로 설정한다.
	// * @param[in] purposeList
	// */
	//void SetModelPurposeList(std::list<KPurpose*>& purposeList);
	/**
	 * @brief 목적 정보를 주어진 list로 설정한다.
	 * @param[in] purposeList
	 */
	void SetModelPurposeList(std::list<KPAPurpose*>& purposeList);

	/**
	* @brief Target을 설정 
	* @param[in] pTarget - Target
	*/
	void SetTarget(KTarget* pTarget);


protected :
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget* m_pTarget;
	KIOTable* m_pZone;
	//std::list<KPurpose*> m_PurposeList;
	std::list<KPAPurpose*> m_PurposeList;

    /**
    * @brief Report Column 초기화
    */
	void initReportColumn(void);

    /**
    * @brief Report Data Update
    */
	void updateReportRecord(void);
	//void updatePARecord(CXTPReportRecord* pPARecord, KPurpose* pPurpose);

    /**
    * @brief Report Data Update
    */
	void updatePARecord(CXTPReportRecord* pPARecord, KPAPurpose* pPurpose);

 
	bool setInputData(CXTPReportRecord* pRecord);

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
};
