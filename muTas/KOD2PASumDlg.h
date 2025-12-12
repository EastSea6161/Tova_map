/**
 * @file KOD2PASumDlg.h
 * @brief 발생 도착량 집계
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once
#include "KDBaseOD2PASum.h"
class KIDKeyDoubleRecords;



/**
* @brief 발생 도착량 집계
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KOD2PASumDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KOD2PASumDlg)

public:
	KOD2PASumDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KOD2PASumDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5210_AggregateODtoPADlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief 목적 별 Production / Attraction 합
    */
    void InitReportData();

    /**
    * @brief 다이얼로그 초기 로드 설정
    */
    void Initial();
protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
    KIOColumns  m_purposeColumns;
protected:
    
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    virtual void PostNcDestroy();
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

	static unsigned __stdcall OD2PASumThreadCaller( void* pParam );
	static void AddStatusMessage( CString a_strMsg );
};
