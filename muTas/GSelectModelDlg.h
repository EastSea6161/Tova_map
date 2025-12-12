/**
 * @file GSelectModelDlg.h
 * @brief KGSelectModelDlg 선언 파일
 * @author 
 * @date 2011.04.21
 * @remark
 */


#pragma once


#include "GenerationInfoList.h"
#include "model_common.h"


/**
 * @brief 발생모형의 수행 모델과 Zone group 선택 다이얼로그
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.04.21
 */
class KGSelectModelDlg : public CXTResizeDialog, public KGenerationInfoList
{
	DECLARE_DYNAMIC(KGSelectModelDlg)

public:
	KGSelectModelDlg(KEMGHaveDBMode a_KEMGUSeDBMode, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGSelectModelDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_6010_GSelectModelDlg };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;


protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();

	/**
    * @brief Report 컬럼 정보 초기화 (컬럼 정보 쓰기)
    */
	void initReportColumn(void);

    /**
    * @brief Report 데이터 초기화( 데이터 정보쓰기_인구와 SED 데이터 모두 있을 경우)
    */
	void initReportRecordAll(void);	

	/**
    * @brief Report 데이터 초기화( 데이터 정보쓰기_인구 데이터만 있을 경우 )
    */
	void initReportRecordSedPop(void);

	/** 
    *@brief Report 데이터 초기화( 데이터 정보쓰기_SED 데이터만 있을경우 )
    */
	void initReportRecordSedVector(void);

	KEMGHaveDBMode m_KEMGUSeDBMode;


protected:
	afx_msg void OnReportItemClick(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBnClickedCancel();

	
};
