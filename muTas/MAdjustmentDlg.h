/**
 * @file MAdjustmentDlg.h
 * @brief KMAdjustmentDlg 선언 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#pragma once


/* Forward declarations */
class KMAdjustmentDummy;
class KModeChoiceInfo;
class KTarget;


/**
 * @brief 수단 선택 모형에서 Acjustment 설정 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.02
 */
class KMAdjustmentDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMAdjustmentDlg)

public:
	KMAdjustmentDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMAdjustmentDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6530_MAdjustmentDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief 수단선택 모형에서 구동할 모형정보를 추가한다.
	* @param[in] pInfo - 수단선택 모형 정보
	*/
	void AddModeChoiceInfo(KModeChoiceInfo* pInfo);

	/**
	* @brief Dummy 적용 여부를 반환한다.
	* @return bool - Dummy 적용 여부
	*/
	bool ApplyDummy(void);

	/**
	* @brief Dummy 적용 여부를 설정한다.
	* @param[in] bApply - Dummy 적용 여부
	*/
	void ApplyDummy(bool bApply);
	
	/**
	* @brief 수단선택 모형이 구동 될 Target year 정보를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void Target(const KTarget* pTarget);

protected :
	bool m_bApplyDummy;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	const KTarget* m_pTarget;
	std::map<KModeChoiceInfo*, KMAdjustmentDummy*> m_DummyMap;

    /**
    * @brief Report Control 초기화
    */
	void initReportControl(void);

    /**
    * @brief Dummy 변수 초기화
    */
	void initDummyConstraint(void);

    /**
    * @brief Report Data Update
    */
	void updateReportRecord(void);

    /**
    * @brief Report Mode Data Update
    */
	void updateReportRecordMode(CXTPReportRecord* pParent, KMAdjustmentDummy* pDummy);

    /**
    * @brief Dummy 적용 여부
    */
	void applyAdjustmentDummy(CXTPReportRecord* pRoot);

    /**
    * @brief Field Caption Return
    * @param[in] strName - Column Name
    * @param[out] CString - Field Caption
    */
	CString getFieldCaption(LPCTSTR strName);

public:

    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnReportCheckItem(NMHDR*  pNotifyStruct, LRESULT* result);
};
