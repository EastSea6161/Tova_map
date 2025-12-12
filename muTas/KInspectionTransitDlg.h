/**
 * @file KInspectionTransitDlg.h
 * @brief Transit 오류 검증 다이얼로그
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once

#include "KDBaseInspection.h"
#include "KBulkDBase.h"
//^#include "ResizeDialogEx.h"


/**
* @brief Transit 오류 검증 다이얼로그
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KInspectionTransitDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInspectionTransitDlg)
private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief Report Control Data 초기화
    * @param[in] results - 오류 데이터
    */
    void InitReportData(std::vector<Transit_InspectionResult>& results);

    /**
    * @brief Transit 데이터 오류 검증
    * @param[out] results - 오류 데이터
    */
    void CheckTransit(std::vector<Transit_InspectionResult>& results);
    
    /**
    * @brief Transit 데이터 오류 검증
    * @param[in] linkRecords - 링크ID, 시작노드, 종료 노드 Collection
    * @param[in] oneTransit - 노선 기본 정보
    * @param[out] results - 오류 데이터
    */
    void CheckTransitLink(std::map<Integer, TFTNode>& linkRecords, TFixedTransitDB& oneTransit, std::vector<Transit_InspectionResult>& results);
public:
	KInspectionTransitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInspectionTransitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5160_InspectionTransitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
    BOOL m_chkDataInspection;
    BOOL m_chkCodeInspection;
    BOOL m_chkIsolatedLink;
protected:

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);    
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    
private:
    KTarget* m_pTarget;
public:
    /**
    * @brief Dialog 초기화(목표 년도) 설정
    * @param[in] pTarget - 목표 년도
    */
    void SetTarget(KTarget* pTarget) { m_pTarget = pTarget; }
};

typedef std::shared_ptr<KInspectionTransitDlg> KInspectionTransitDlgPtr;