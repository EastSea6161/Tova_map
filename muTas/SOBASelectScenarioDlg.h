/**
* @file SOBASelectScenarioDlg.h
* @brief MultiAssignModelSOBA 다이얼로그에서 시나리오를 추가 실행시 대상 시나리오를 선택하는 다이얼로그
* @author
* @date 2019.04.22
* @remark
*/


#pragma once

#include "ARunningOptionInfo.h"



class KTarget;
class KScenario;
class KMultiAssignInfoSOBA;


// KSOBASelectScenarioDlg 대화 상자입니다.

class KSOBASelectScenarioDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KSOBASelectScenarioDlg)

public:
	KSOBASelectScenarioDlg(KTarget* pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSOBASelectScenarioDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_6650_SOBA_SelectScenarioDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	void SetMultiAssignInfoArray(std::vector<KMultiAssignInfoSOBA*>& arr);
	KMultiAssignInfoSOBA* GetSelectedAssignInfo();


protected:
	std::map<KScenario*, CXTPReportRecord*> m_mapScenarioRecord;
	std::vector<KMultiAssignInfoSOBA*> m_arrMultiAssingInfo;
	KMultiAssignInfoSOBA* m_pSelectedInfo;
	std::set<KTarget*> m_setWormStartSource;
	KTarget* m_pSelWSTarget;
	KScenario* m_pSelWSScenario;

	void InitTreeReport(void);
	void UpdateTreeData(void);

	/**
	* 대상 Target이 분석 가능한지 확인. 확인은 네트워크 데이터 이상여부, 수단 데이터 이상여부를 확인
	* @param pTarget 확인 대상 Target
	* @return bool 분석 가능하면 true, 그렇지 않으면 false를 반환
	*/
	bool CheckTargetRunnable(KTarget* pTarget, CString& strStatus);
	std::set<KTarget*> GetCheckedTarget();

	bool m_bTurnRestriction;
	bool m_bIncludeTurnVolume;
	bool m_bBusInitialVolume;
	bool m_bPathAnalysis;
	bool m_bIntersectionDelay;


protected:
	CXTPOfficeBorder<CXTPReportControl, false> m_wndTree;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnReportCheckItemTarget(NMHDR* a_pNotifyStruct, LRESULT* a_pResult);
};
