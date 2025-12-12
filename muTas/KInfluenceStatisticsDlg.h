/**
 * @file KInfluenceStatisticsDlg.h
 * @brief 영향권 설정 정보(통계)
 * @author nomplus2@qbicware.com
 * @date 2011.08.28
 * @remark 
 */

#pragma once


/**
* @brief 영향권 설정 정보(통계)
* @version 1.0
* @author nomplus2@qbicware.com
* @date 2011.08.28
*/
class KInfluenceStatisticsDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(KInfluenceStatisticsDlg)

public:
	KInfluenceStatisticsDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInfluenceStatisticsDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5410_InfluenceAreaStatisticsDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
    /**
    * @brief Report Control Header 초기화
    */
    void InitReportHeader();

    /**
    * @brief 영향권 설정 통계 정보 디스플레이
    */
    void InitReportData();

protected:
    CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

    /**
    * @brief 다이얼로그 초기화
    */
    virtual BOOL OnInitDialog();

private:
    KTarget* m_pTarget;
public:
    /**
    * @brief Dialog 초기화(목표 년도) 설정
    * @param[in] pTarget - 목표 년도
    */
    void SetTarget(KTarget* pTarget) { m_pTarget = pTarget; }
};
