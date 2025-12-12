#pragma once


// KWoStoppingTransitDlg 대화 상자입니다.

class KWoStoppingTransitDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KWoStoppingTransitDlg)

public:
	KWoStoppingTransitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KWoStoppingTransitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6936_WoStoppingTransitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	/**
	* @brief Iteration을 반환한다.
	* @return int - Iteration
	*/
	int OutterNumberOfIteration(void);
	/**
	* @brief Iteration을 설정한다.
	* @param[in] nIteration - Iteration
	*/
	void OutterNumberOfIteration(int nIteration);

	int InnerNumberOfIteration(void);

	void InnerNumberOfIteration(int nIteration);

	/**
	* @brief Gap을 반환한다.
	* @return double - Gap
	*/
	double OutterL2Norm(void);

	/**
	* @brief Gap을 설정한다.
	* @param[in] dGap - Gap
	*/
	void OutterL2Norm(double dL2Norm);

	double InnerL2Norm(void);

	void InnerL2Norm(double dL2Norm);

protected:

	int m_nOuterIteration;
	int m_nInnerIteration;

	double m_dOuterL2Norm;
	double m_dInnerL2Norm;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
