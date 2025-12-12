#pragma once

#include "ARunningOptionInfo.h"
#include "model_common.h"

class KTarget;

// KFARunOptionDlg 대화 상자입니다.

class KFARunOptionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KFARunOptionDlg)

public:
	KFARunOptionDlg(KTarget* a_pTarget, KEMAssignmentModel a_emAModel, int a_nVDFCount, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KFARunOptionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6710_FARunOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;	
	int						m_nVDFCount;
	KEMAssignmentModel		m_emAModel;
	KARunningOptionInfo		m_RunningOption;

public:

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

protected:

	
	CButton m_chkTurn;
	CStatic m_stcTurn;




protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();

};
