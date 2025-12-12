#pragma once
#include "afxwin.h"

class KAInputInfo;
class KTarget;
class KScenario;
class KMultiAssignInfoSOBA;


// KAWarmStartDlg 대화 상자입니다.

class KAWarmStartDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KAWarmStartDlg)

public:
	KAWarmStartDlg(KTarget* a_pTarget, std::list<KAInputInfo*> a_lstInputInfo, KTarget* a_pSelTarget, KScenario* a_pSelScnario, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAWarmStartDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6710_AWARMSTART };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget* m_pTarget;
	std::list<KAInputInfo*> m_lstInputInfo;

	KTarget*		m_pSelTarget;
	KScenario*		m_pSelScenario;

	// 2018.08.02 명지대 요청사항 수정
	const CString m_argFileName = _T("\\MODEL\\HighwayAssign\\assignment.arg");
	bool m_bTurnRestriction;		// 회전제약 적용
	bool m_bIncludeTurnVolume;		// 회전교통량 분석
	bool m_bBusInitialVolume;	// 버스 노선 운행차량 교통량 포함
	bool m_bPathAnalysis;		// 경로 교통량 분석
	bool m_bIntersectionDelay;	// 교차로 지체 분석/적용

public:
	bool isTurnRestriction();
	bool isIncludeTurnVolume();
	bool isBusInitialVolume();
	bool isPathAnalysis();
	bool isIntersectionDelay();

protected: 
	void readRunOption(CString& argFileName);
	// 2018.08.02 명지대 요청사항 수정 끝

	// 2019.04.19 수정 : 여러 시나리오 반복실행에서 Bush 파일이 없지만 모형을 돌리는 분석년도(대상)은 선택이 가능하게 한다.
	std::set<KTarget*> m_setCheckedTarget;
	//std::set<KTarget*> m_setWormStartSource;
	//std::map<KTarget*, KARunningOptionInfo> m_mapRunningOption;

public:
	void SetCheckedTarget(std::set<KTarget*>& setCheckedTarget);
	//void SetRunningOptionMap(std::map<KTarget*, KARunningOptionInfo>& mapRunningOption)

public:
	void GetWarmData(KTarget* a_pTarget, KScenario* a_pScanrio);
	KTarget* GetWarmTarget();
	KScenario* GetWarmScnario();

protected:

	void InitReportHeader();
	void InitReportData();
	void AddTreeRecord( CXTPReportRecord* a_pParent, KScenario* a_pScenario );

protected:
	CComboBox m_cboScnario;
	CComboBox m_cboTarget;

protected:
	bool CheckTargetBushFile(KTarget* a_pTarget);
	bool CheckMode(KTarget* a_pTarget);
	void GetExistModeID(KTarget* a_pTarget, std::set<int>& a_setModeInfo);
	CString GetExistModeName(KTarget* a_pTarget);
	CString GetEndDate(KTarget* a_pTarget);

	void ResizeComponet(void);

	// 2019.04.23 추가 : 해당 시나리오를 웜스타트 초기치로 선택 가능 여부를 판단
public:
	void SetRunningOptionPathAnalysis(bool bPathAnalysis);
protected:
	bool m_bROPathAnalysis;

	/**
	* 주어진 KTarget이 웜스타트의 초기치로 선택 가능여부를 반환
	* @param pTarget 검사할 KTarget
	* @param strStatus(out) 검사 결과를 반환 할 문자열
	* @param strStatusEng(out) 검사 결과를 영문으로 반환 할 문자열
	* @return bool 웜스타트 초기치 선택 가능 시 true, 그렇지 않으면 false를 반환
	*/
	bool CheckTargetSelectable(KTarget* pTarget, CString& strStatus, CString& strStatusEng);

	/**
	* 주어진 KTarget의 통행배정 이력이 있는지 확인
	* @param pTarget 검사할 KTarget
	* @return bool 통행배정 이력이 존재하면 true, 그렇지 않으면 false를 반환
	*/
	bool CheckAssignHistoryExists(KTarget* pTarget);


	// 2019.04.24 수정 : SOBA의 여러시나리오 웜스타트 설정시 분석예정 시나리오의 수단정보 추출을 위해 추가
public:
	void SetMultiAssingInfo(std::vector<KMultiAssignInfoSOBA*>& arr);
protected:
	std::map<KTarget*, KMultiAssignInfoSOBA*> m_mapMultiAssignInfo;
	CString GetTargetModeName(KTarget* pTarget);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* result);
	
};
