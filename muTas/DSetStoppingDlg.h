/**
 * @file DSetStoppingDlg.h
 * @brief KDSetStoppingDlg 선언 파일
 * @author 
 * @date 2011.05.23
 * @remark
 */


#pragma once
#include "afxwin.h"


/**
 * @brief 분포 모형에서 Stopping Criteria를 설정하는 Dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.05.23
 */
class KDSetStoppingDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDSetStoppingDlg)

public:
	KDSetStoppingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDSetStoppingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6330_DStoppingCriteriaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Fratar 모델의 Iteration을 반환한다.
	* @return int - Iteration
	*/
	int FratarIteration(void);

	/**
	* @brief Fratar 모델의 Iteration을 설정한다.
	* @param[in] nIteration - Iteration
	*/
	void FratarIteration(int nIteration);

	/**
	* @brief Fratar 모델의 Gap을 반환한다.
	* @return double - Gap
	*/
	double FratarGap(void);

	/**
	* @brief Fratar 모델의 Gap을 설정한다.
	* @param[in] dGap - Gap
	*/
	void FratarGap(double dGap);

	/**
	* @brief Gravity 모델의 Iteration을 반환한다.
	* @return int - Iteration
	*/
	int GravityIteration(void);

	/**
	* @brief Gravity 모델의 Iteration을 설정한다.
	* @param[in] nIteration - Iteration
	*/
	void GravityIteration(int nIteration);

	/**
	* @brief Gravity 모델의 Error율을 반환한다.
	* @return double - Error율
	*/
	double GravityError(void);

	/**
	* @brief Gravity 모델의 Error율을 설정한다.
	* @param[in] nError - Error율
	*/
	void GravityError(double nError);


protected :
	int m_nFratarIteration;
	double m_dFratarGap;
	int m_nGravityIteration;
	double m_dGravityError;
	CEdit m_edtFratarIteration;
	ZNumberEdit m_edtFratarGap;
	CEdit m_edtGravityIteration;
	CEdit m_edtGravityError;

public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
    afx_msg void OnBnClickedCancel();
};
