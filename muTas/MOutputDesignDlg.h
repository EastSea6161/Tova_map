/**
 * @file MOutputDesignDlg.h
 * @brief KMOutputDesignDlg 선언 파일
 * @author 
 * @date 2011.06.02
 * @remark
 */


#pragma once


/* Forward declarations */
class KMode;
class KMOutputInfo;
class KTarget;


/**
 * @brief 수단 선택 모형에서 모형 결과 저장을 설정하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.02
 */
class KMOutputDesignDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KMOutputDesignDlg)

public:
	KMOutputDesignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6590_MOutPutDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Target year를 설정한다.
	* @param[in] pTarget - Target year 정보
	*/
	void Target(KTarget* pTarget);

	/**
	* @brief 수단의 Output 정보를 추가한다.
	* @param[in] pMode - 수단 정보
	* @param[in] pInfo - Output 정보 
	*/
	void AddOutptuInfo(KMode* pMode, KMOutputInfo* pInfo);

protected :

    /**
    * @brief Report Control  초기화
    */
	void initReportControl(void);

    /**
    * @brief Output 정보 Update
    */
	void updateOutputRecord(void);

    /**
    * @brief Output 컬럼 정보 추가
    */
	void addOutputColumns(CXTPReportRecord* pParent, KMode* pMode);

    /**
    * @brief 이름 체크
    */
	bool isNameExists(CXTPReportRecord* pParent, LPCTSTR strName);

    /**
    * @brief 사용자 선택한 Out Field 정보 적용
    */
	bool applyOutputField(CXTPReportRecord* pModeRecord);

protected:

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget* m_pTarget;
	std::map<KMode*, KMOutputInfo*> m_OutputMap;


protected:

    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
};
