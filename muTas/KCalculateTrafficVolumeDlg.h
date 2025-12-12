/**
 * @file KCalculateTrafficVolumeDlg.h
 * @brief 조사 교통량과 분석 교통량 비교
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */


#pragma once

#include "KBulkType.h"
//^^ #include "IOColumns.h"



/**
* @brief 조사 교통량과 분석 교통량 비교
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KCalculateTrafficVolumeDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KCalculateTrafficVolumeDlg)

public:
	KCalculateTrafficVolumeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculateTrafficVolumeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5310_CalcuateTrafficVolumeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
    UINT m_nRadioButton;

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief 조사 교통량과 분석 교통량 비교 결과 Display
    */
    void InitReportData();

    /**
    * @brief 링크 기본 정보 로드
    */
    void Initial();

protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
    std::map<Integer, TFixedLinkDB> m_linkInfoRecords;
    KIOColumns             m_volColumns;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();
    virtual void PostNcDestroy();
    afx_msg void OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult);
    
    afx_msg void OnBnClickedRadio1();
    afx_msg void OnBnClickedRadio2();
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

	static unsigned __stdcall CalculateTrafficVolThreadCaller( void* pParam );
	static void AddStatusMessage( CString a_strMsg );
};
