/**
 * @file AStoppingCriteriaMMDlg.h
 * @brief KAStoppingCriteriaMMDlg 선언 파일
 * @author 
 * @date 2011.06.17
 * @remark
 */


#pragma once
#include "afxwin.h"


/**
 * @brief 배정 모형에서 Multi-Modal 모델의 Stopping Criteria를 설정하는 dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.17
 */
class KAStoppingCriteriaMMDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KAStoppingCriteriaMMDlg)

public:
	KAStoppingCriteriaMMDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAStoppingCriteriaMMDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6780_AStoppingMulitymodalDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Highway의 Iteration을 반환한다.
	* @return int - Highway Iteration
	*/
	int HighwayIteration(void);
	
	/**
	* @brief Highway의 Iteration을 설정한다.
	* @return nIteration - Highway Iteration
	*/
	void HighwayIteration(int nIteration);

	/**
	* @brief Relative Gap을 반환한다.
	* @return double - Relative Gap
	*/
	double RelativeGap(void);
	
	/**
	* @brief Relative Gap을 설정한다.
	* @return dGap - Relative Gap
	*/
	void RelativeGap(double dGap);

	/**
	* @brief Transit Iteration을 반환한다.
	* @return int - Transit Iteration
	*/
	int TransitIteration(void);
	
	/**
	* @brief Transit Iteration을 설정한다.
	* @return nIteration - Transit Iteration
	*/
	void TransitIteration(int nIteration);

	/**
	* @brief Normalize Gap을 반환한다.
	* @return double - Normalize Gap
	*/
	double NormalizedGap(void);
	
	/**
	* @brief Normalize Gap을 설정한다.
	* @return dGap - Normalize Gap
	*/
	void NormalizedGap(double dGap);

protected :
	ZNumberEdit m_edtHGap;
	ZNumberEdit m_edtTGap;
	int m_nHIteration;
	double m_dHGap;
	int m_nTIteration;
	double m_dTGap;


public:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
