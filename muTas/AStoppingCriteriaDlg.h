/**
 * @file AStoppingCriteriaDlg.h
 * @brief KAStoppingCriteriaDlg 선언 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "model_common.h"


/**
 * @brief 배정 모형에서 Transit과 User Equilibrium모델의 Stopping Criteria를 설정하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.17
 */
class KAStoppingCriteriaDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAStoppingCriteriaDlg)

public:
	KAStoppingCriteriaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAStoppingCriteriaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6770_AStoppingCriteriaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public :

	/**
	* @brief 배정 모델의 종류를 설정한다.
	* @param[in] emModel - 배정 모델의 종류
	*/
	void AssignmentMode(KEMAssignmentModel emModel);

	/**
	* @brief Iteration을 반환한다.
	* @return int - Iteration
	*/
	int NumberOfIteration(void);

	/**
	* @brief Iteration을 설정한다.
	* @param[in] nIteration - Iteration
	*/
	void NumberOfIteration(int nIteration);

	/**
	* @brief Gap을 반환한다.
	* @return double - Gap
	*/
	double Gap(void);

	/**
	* @brief Gap을 설정한다.
	* @param[in] dGap - Gap
	*/
	void Gap(double dGap);


protected :
	CEdit m_edtIteration;
	ZNumberEdit m_edtGap;
	CStatic m_stcAssign;
	CStatic m_stcGap;
	KEMAssignmentModel m_emModel;
	int m_nIteration;
	double m_dGap;

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
    afx_msg void OnBnClickedCancel();
};
