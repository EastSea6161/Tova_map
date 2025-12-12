#pragma once


// KWOStoppingDlg 대화 상자입니다.

class KWOStoppingDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KWOStoppingDlg)

public:
	KWOStoppingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KWOStoppingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6936_WOStoppingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
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
	double L2Norm(void);

	/**
	* @brief Gap을 설정한다.
	* @param[in] dGap - Gap
	*/
	void L2Norm(double dL2Norm);

protected:

	int		m_nIteration;
	double	m_dL2Norm;

	CEdit m_edtIteration;
	ZNumberEdit m_edtGap;

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
