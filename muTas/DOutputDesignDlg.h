/**
 * @file DOutputDesignDlg.h
 * @brief KDOutputDesignDlg 선언 파일
 * @author 
 * @date 2011.05.23
 * @remark
 */


#pragma once

#include "afxwin.h"
#include "model_common.h"
#include "DistributionInfoList.h"



/* Forward declarations */
class KDistributionInfo;
class KTarget;


/**
 * @brief 분포모형에서 Output을 설정하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.23
 */
class KDOutputDesignDlg : public CXTResizeDialog, public KDistributionInfoList
{
	DECLARE_DYNAMIC(KDOutputDesignDlg)

public:
	KDOutputDesignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6390_DOutPutDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Target year 정보를 지정한다.
	* @param[in] pTarget - Target year의 정보
	*/
	void SetTarget(KTarget* pTarget);

protected :
	KTarget* m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;

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
	void addOutputColumns(CXTPReportRecord* pParent, KDistributionInfo* pInfo);

    /**
    * @brief 이름 체크
    */
	bool isNameExists(CXTPReportRecord* pParent, LPCTSTR strName);

    /**
    * @brief Output 정보 Set
    */
	bool setDistributionOutput(CXTPReportRecord* pRecord);

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT * a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);

};