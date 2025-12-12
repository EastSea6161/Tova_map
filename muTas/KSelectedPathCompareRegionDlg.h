#pragma once

#include "KSelectedPathCompareTypes.h"




// KSelectedPathCompareRegionDlg 대화 상자입니다.

class KSelectedPathCompareRegionDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KSelectedPathCompareRegionDlg)

public:
	KSelectedPathCompareRegionDlg(bool bPathEdit, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSelectedPathCompareRegionDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_6929_KSelectedPathCompareRegion };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	
public:
	void SetInterPathRegionInfos(std::vector<KInterPathRegionInfoWrapper>& arr);


protected: 
	bool m_bPathEdit;
	std::vector<KInterPathRegionInfoWrapper> m_arrRegionPathInfo;
	CXTPOfficeBorder<CXTPReportControl, false> m_wndReport;

	void InitReportHeader();
	void UpdateReportData();

	struct STRegionPathCostTime
	{
		KRegionModePathInfo* pPathInfo;
		KRegionPathDetail* pPathDetail;
		CString strDest;	// 종점
		CString strMode;	// 주수단
		CString strPathName;	// 경로구분
		CString strFromTerminal;	// 출발 터미널
		CString strToTerminal;		// 도착 터미널
		double dTotalTime;	// 기종점간 통행시간
		double dTotalCost;	// 기종점간 통행비용
		double dMainInVehTime;	// 주수단 차내통행시간(승용차의 경우 없음)
		double dMainWaitTime;	// 주수단 대기시간(승용차의 경우 없음)
		double dMainTransTime;	// 주수단 환승시간(승용차의 경우 없음)
		double dMainProcTime;	// 주수단 탑승수속시간(승용차의 경우 없음)
		double dMainTotalTime;	// 주수단 총 통행시간
		double dMainTotalCost;	// 주수단 통행비용
		double dAvgAccessTime;	// 출발 접근수단 평균 통행시간
		double dAvgAccessCost;	// 출발 접근수단 평균 통행비용
		double dAvgEgressTime;	// 도착 접근수단 평균 통행시간
		double dAvgEgressCost;	// 도착 접근수단 평균 통행비용
		double dTrip;			// 통행량(인)
		STRegionPathCostTime() : pPathInfo(NULL), pPathDetail(NULL), dTotalTime(0), dTotalCost(0),
			dMainInVehTime(0), dMainWaitTime(0), dMainTransTime(0), dMainProcTime(0), dMainTotalTime(0), dMainTotalCost(0),
			dAvgAccessTime(0), dAvgAccessCost(0), dAvgEgressTime(0), dAvgEgressCost(0), dTrip(0) {}
	};

	std::vector<STRegionPathCostTime> m_arrPathCostTime;
	void AggregateRetionPath(std::vector<STRegionPathCostTime>& arrPathCostTime);
	void CalcRegionPathTimeAndCost(STRegionPathCostTime& rpct);
	void AddAutoModeRecord(CXTPReportRecord* pRecord, STRegionPathCostTime& rpct);
	void AddOtherModeRecord(CXTPReportRecord* pRecord, STRegionPathCostTime& rpct);
	void GetInterPathTimeAndCost(std::vector<KInterPathEditInfo>& arrInfo, double& dTotalTime, double& dTotalCost, double& dTotalTrip);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

typedef std::shared_ptr<KSelectedPathCompareRegionDlg> KSelectedPathCompareRegionDlgPtr;