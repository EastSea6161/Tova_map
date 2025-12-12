/**
 * @file DSelectModelDlg.h
 * @brief KDSelectModelDlg 선언 파일
 * @author 
 * @date 2011.05.19
 * @remark
 */


#pragma once


#include "model_common.h"


/* Forward declarations */
class KDistributionInfo;
class KPurpose;


/**
 * @brief 분포모형에서 모델을 선택하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.19
 */
class KDSelectModelDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDSelectModelDlg)

public:
	KDSelectModelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDSelectModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6210_DSelectModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :
	/**
	 * @brief 분포 모형에서 설정 할 목적/수단 정보를 추가한다.
	 * @param[in] emODType - 정보의 OD Type
	 * @param[in] pInfo - 분포 모델의 목적/수단 정보
	 */
	void AddDistributionInfo(KEMDODType emODType, KDistributionInfo* pInfo);

	/**
	 * @brief 분포 모형의 목적/수단 정보를 주어진 list로 설정한다.
	 * @param[in] infoList
	 */
	void SetDistributionInfo(KEMDODType emODType, std::list<KDistributionInfo*>& infoList);

	/**
	* @brief 분포모형 OD의 종류를 반환한다.
	* @return KEMDODType - 분포모형 OD의 종류
	*/
	KEMDODType DistributionODType(void);

	/**
	* @brief 분포모형 OD의 종류를 설정한다.
	* @param[in] emODType - 분포모형 OD의 종류
	*/
	void DistributionODType(KEMDODType emODType);


protected :
	int m_nODType;
	KEMDODType m_emODType;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	std::list<KDistributionInfo*> m_PurposeInfoList;
	std::list<KDistributionInfo*> m_ModeInfoList;

    /**
    * @brief Report Column  초기화
    */
	void initReportColumn(void);

    /**
    * @brief Report Data Update
    */
	void updateReportRecords(void);


public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioClicked(UINT nID);
	afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
//    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckSelectAll();
			void UpdateReportSelectStatus(BOOL a_bCheck);
};
