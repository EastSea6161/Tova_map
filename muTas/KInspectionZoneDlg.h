/**
 * @file KInspectionZoneDlg.h
 * @brief 존 데이터 검증 다이얼로그
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */


#pragma once
#include "KDBaseInspection.h"
//^#include "ResizeDialogEx.h"

/**
* @brief 존 데이터 검증 다이얼로그
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KInspectionZoneDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInspectionZoneDlg)
private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief Report Control Data 초기화
    * @param[in] results - 오류 데이터
    */
    void InitReportData(std::vector<InspectionResultOneKey>& results);

public:
	KInspectionZoneDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInspectionZoneDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5150_InspectionZoneDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
   
protected:

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
protected:    
    BOOL m_chkDataInspection;
    BOOL m_chkCodeInspection;
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
private:
    KTarget* m_pTarget;
public:
    /**
    * @brief Dialog 초기화(목표 년도) 설정
    * @param[in] pTarget - 목표 년도
    */
    void SetTarget(KTarget* pTarget) { m_pTarget = pTarget; }    
};

typedef std::shared_ptr<KInspectionZoneDlg> KInspectionZoneDlgPtr;