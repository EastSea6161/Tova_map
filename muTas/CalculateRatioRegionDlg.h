#pragma once
#include "afxwin.h"
#include <list>

class KTarget;
class KRatioRegionInfo;


// KCalculateRatioRegion 대화 상자입니다.

class KCalculateRatioRegionDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KCalculateRatioRegionDlg)

public:
	KCalculateRatioRegionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCalculateRatioRegionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5530_CalculateRatioRegionDlg};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:

	KTarget*						m_pTarget;
	std::list<CString>				m_lstColumnName;
	std::vector<KRatioRegionInfo* > m_vecpRatioInfo;
	int								m_nSEDYear;


public:

	void SetTarget(KTarget* a_pTarget);

protected:

	
	 /**
    * @brief 화면 초기화
    * @return 초기화 결과
    */
	virtual BOOL OnInitDialog();
	 /**
    * @brief SED의 연도 데이터 가져와서 콤보 박스에 넣기 
    */
	void GetSEDYearData();
	 /**
    * @brief SED의 컬럼 리스트 저장하기
    */
	void InitSEDColumnList();
	 /**
    * @brief 품목 데이터 저장하기
    */
	void InitCommodityList(void);
	 /**
    * @brief 표의 헤더 파일 만들기
    */
	void InitCommoditReportColumn();
	 /**
    * @brief 표의 내용 업데이트 하기  
    */
	void InitUpdateColumn();
	 /**
    * @brief SED의 코드 찾기 
    */
	bool FindSEDCode(CString a_strSEDName, CString& a_strSEDCode);
	 /**
    * @brief DB의 초기화
    */
	void DelExistiongDB();
	 /**
    * @brief 컬럼명과 품목을 Mathging 테이블에 입력하기 
    */
	void WriteMatchingTable();
	 /**
    * @brief 매칭되지 않은RegionInfo Class 삭제
    */
	void DeleteRegionInfo();
	 /**
    * @brief Ratio_Region DB의 기본 정보 입력
    */
	void SetRatioRegionBaseDB();
	 /**
    * @brief 프로그램 계산후 DB에 저장하는 작업
    */
	void RunRatioRegion();
	 /**
    * @brief Category별 데이터 연산 작업
    */
	void CalculateSumCategory(KRatioRegionInfo* a_pRatioInfo);
	 /**
    * @brief 품목에 맞는 컬럼 생성
    */
	bool AddRatioRegionColumn(KRatioRegionInfo* a_pRatioInfo);
	 /**
    * @brief SED Tree view 초기화
    */
	void RefreshTreeWnd();
	 /**
    * @brief Ratio_regeion 테이블 초기화
    */
	void ViewRatioRegionTable();
	

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_reportCommodity;
	int					m_nTargetYear;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeCombo1();
	
};
