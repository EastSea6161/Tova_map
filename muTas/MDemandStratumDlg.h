/**
 * @file MDemandStratumDlg.h
 * @brief KMDemandStratumDlg 선언 파일
 * @author 
 * @date 2011.06.01
 * @remark
 */


#pragma once


#include "model_common.h"
#include "Purpose.h"

/* Forward declarations */
class KIOColumn;
class KPurpose;
class KTarget;




/**
 * @brief 수단 선택 모형에서 demand stratum 설정 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.01
 */
class KMDemandStratumDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMDemandStratumDlg)

public:
	KMDemandStratumDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMDemandStratumDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6500_MSelectDemandDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보 
	*/
	void SetTarget(KTarget* pTarget);

	/**
	* @brief 목적 set을 설정한다.
	* @param[in] purposeSet
	*/
	void PurposeSet(std::set<KPurpose*> purposeSet);

	void PurposeList(std::list<KPurpose*> a_lstPurpose);

	/**
	* @brief 선택된 목적 set을 반환한다.
	* @return std::set<KPurpose*>& - 선택된 목적 set
	* @remark 
	* @warning 
	*/
	std::set<KPurpose*, TPurposeComparator>& SelectedPurpose(void);

	/**
	* @brief 선택된 목적 set을 설정한다.
	* @param[in] purposeSet - 선택된 목적 set
	*/
	void SelectedPurpose(std::set<KPurpose*, TPurposeComparator>& purposeSet);

	/**
	* @brief 입력되는 목적 - Column의 map을 반환한다.
	* @return std::map<KPurpose*, KIOColumn*>& - 목적 - Column map
	*/
	std::map<KPurpose*, KIOColumn*, TPurposeComparator>& InputMap(void);

	/**
	* @brief 입력되는 목적-Column의 map을 설정한다.
	* @param[in] inputMap - 목적-Column map
	*/
	void InputMap(std::map<KPurpose*, KIOColumn*, TPurposeComparator>& inputMap);


protected :
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndInput;
	KTarget* m_pTarget;
	std::set<KPurpose*> m_PurposeSet;
	std::list<KPurpose*> m_lstPurpose;
	std::set<KPurpose*, TPurposeComparator> m_SelectedPurposeSet;
	std::map<KPurpose*, KIOColumn*, TPurposeComparator> m_InputMap;

    /**
    * @brief Report Control 초기화
    */
	void initReportControl(void);
	
    /**
    * @brief Report Data(Purpose) Update
    */
	void updatePurposeRecord(void);

    /**
    * @brief Report Data(Input) 초기화
    */
	void initInputReportControl(void);

    /**
    * @brief Report Data(Input) Update
    */
	void updateInputRecord(void);

    /**
    * @brief Report Data(Purpose) Update
    */
	void updateInputPurposeRecord(CXTPReportRecord* pParent, KPurpose* pPurpose);

    /**
    * @brief 사용자 선택 목적 정보 체크
    */
	bool applySelectedPurpose(void);

    /**
    * @brief 사용자 선택 컬럼 정보 체크
    */
	bool applyInputColumn(void);

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnInputCheckItem(NMHDR* pNotifyStruct, LRESULT* result);
};
