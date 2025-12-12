/**
 * @file ARunOptionDlg.h
 * @brief KARunOptionDlg 선언 파일
 * @author 
 * @date 2011.06.16
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "ARunningOptionInfo.h"
#include "Target.h"
#include "model_common.h"
#include "AInputInfo.h"

/* Forward declarations */
class KIOTable;
class KScenario;


/**
 * @brief 배정 모형에서 Run option을 설정하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.16
 */
class KARunOptionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KARunOptionDlg)

public:
	KARunOptionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KARunOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6710_ARunOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief 배정모형 Running Option을 설정한다.
	* @param[in] info - Running Option
	*/
	void RunningOption(KARunningOptionInfo& info);

	/**
	* @brief 배정모형 Running Option을 반환한다.
	* @return KARunningOptionInfo - Running Option
	*/
	KARunningOptionInfo& RunningOption(void);


protected :
	int m_nVDFCount;
	KEMAssignmentModel m_emAModel;
	CButton m_chkTurn;
	CButton m_chkVolume;
	CButton m_chkPath;
	CButton m_chkIntersection;
	CButton m_chkWarm;
	CButton m_chkTurnVolume;
	CComboBox m_cboBalanceIter;

	CStatic m_stcTurn;
	CStatic m_stcVolume;
	CStatic m_stcPath;
	CStatic m_stcIntersection;
	KARunningOptionInfo m_RunningOption;

	std::list<KAInputInfo*> m_lstInputInfo;

protected:
	void InitComboBoxIter(void);

protected :
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedCancel();

public:
    void SetTarget(KTarget* pTarget) { m_pTarget = pTarget; }
	void SetAssignMode(KEMAssignmentModel emAModel);
	void SetVDFCount(int a_nVDFCount);
	void SetInputIinfo(std::list<KAInputInfo*> a_lstInputInfo);

	bool CheckTargetBushFile(KTarget* a_pTarget);

private:
    KTarget*	 m_pTarget;
	KTarget*	 m_pSelTarget;
	KScenario*	 m_pSelScnario;

public:
	afx_msg void OnBnClickedButtonWarm();
	afx_msg void OnBnClickedCheckTurnVolumn();
	afx_msg void OnBnClickedCheckTurn();
	afx_msg void OnBnClickedCheckWarm();
	
	afx_msg void OnCbnSelchangeComboIter();
    void ContorlComboIter();
};
