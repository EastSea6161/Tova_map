/**
 * @file GOutputDesignDlg.h
 * @brief KGOutputDesignDlg 선언 파일
 * @author 
 * @date 2011.05.17
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "GenerationInfoList.h"


/* Forward declarations */
class KIOTable;
class KTarget;


/**
 * @brief 발생모형의 Output을 정의 하기 위한 다이얼로그
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.17
 */
class KGOutputDesignDlg : public CXTResizeDialog, public  KGenerationInfoList
{
	DECLARE_DYNAMIC(KGOutputDesignDlg)

public:
	KGOutputDesignDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6190_GOutPutDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	 * @brief 발생모형 Output 디자인을 실행 하는 Target 정보
	 * @param[in] pTarget - Target 정보
	 */
	void SetTarget(KTarget* a_pTarget);

	/**
	 * @brief 발생모형 Output 디자인에서 설정 할 Zone table을 설정한다.
	 * @param[in] pTable - Zone table 정보
	 */
	void SetZoneTable(KIOTable* a_pTable);




protected:

    /**
    * @brief ReportControl 초기화
    */
	void initReportControl(void);

    /**
    * @brief ReportControl Update
    */
	void updateReportControl(void);

    /**
    * @brief 목적별 발생, 도착 정보 추가
    * @param[in] pParent - Parent Record
    * @param[in] pGInfo - 발생(P Or A) 정보
    */
	void addPARecord(CXTPReportRecord* a_pParent, KGenerationInfo* a_pGInfo);

    /**
    * @brief Output Columns 추가
    * @param[in] pParent - Parent Record
    * @param[in] pGInfo - 발생(P Or A) 정보
    */
	void addOutputColumns(CXTPReportRecord* a_pParent, KGenerationInfo* a_pGInfo);

    /**
    * @brief name check
    * @param[in] pParent - Parent Record
    * @param[in] strName - 이름
    */
	bool isNameExists(CXTPReportRecord* a_pParent, LPCTSTR a_strName);

    /**
    * @brief 사용자 선택 정보 설정
    * @param[in] pRecord - Record
    */
	void setPurposeOutput(CXTPReportRecord* a_pRecord);


protected :
	KTarget* m_pTarget;
	KIOTable* m_pZone;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnReportItemClick(NMHDR * a_pNotifyStruct, LRESULT * a_result);
	afx_msg void OnItemButtonClick(NMHDR * a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg LRESULT OnExpressionEdit(WPARAM a_wParam, LPARAM a_lParam);
	afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};
