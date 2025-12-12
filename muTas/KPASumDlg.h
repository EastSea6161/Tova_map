/**
 * @file KPASumDlg.h
 * @brief 발생/도착량 집계
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once
/* Forward declarations */
class KDistributionInfo;
class KScenario;
class KTarget;
#include "KPurposeKey.h"



/**
* @brief 발생/도착량 집계
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KPASumDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KPASumDlg)
private:
    std::map<KPurposeKey, CString> m_AllPurposeMap;
public:
	KPASumDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPASumDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5510_ViewResultPADlg };
    
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
    
protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportTree;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportData;

    void initTreeReportControl(void);
    void updateTargetTree(void);
    void addTargetRecord(CXTPReportRecord* pParnet, KScenario* pScenario);

    /**
    * @brief Report Control Header 초기화
    */
    void initDataReportHeader();

    /**
    * @brief Report Control Data 초기화
    */
    void initDataReport();

    /**
    * @brief 시나리오 정보 Set
    * @param[in] pScenario - 시나리오
    */
    void initDataReportTarget(KScenario* pScenario );

    /**
    * @brief 목적 정보 로드
    */
    void GetPurposeInfo();

    afx_msg void OnReportItemClick(NMHDR * pNotifyStruct, LRESULT * result);
    afx_msg void OnBnClickedOk();

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
};
