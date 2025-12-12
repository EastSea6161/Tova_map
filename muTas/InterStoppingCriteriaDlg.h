#pragma once


// KInterStoppingCriteriaDlg 대화 상자입니다.

class KInterStoppingCriteriaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterStoppingCriteriaDlg)

public:
	KInterStoppingCriteriaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterStoppingCriteriaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6970_InterStoppingCriteriaDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	int    NumberOfIteration(void);	
	void   NumberOfIteration(int nIteration);

	double RelativeGap(void);	
	void   RelativeGap(double dGap);

    int RelativeGapExp()
    {
        return m_nHighwayRelativeGapExp;
    }

    void RelativeGapExp(int a_nRelativeGapExp)
    {
        m_nHighwayRelativeGapExp = a_nRelativeGapExp;
    }

	double ScaleFactor(void);	
	void   ScaleFactor(double dGap);

	int	   ScaleFactorCursor(void);
	void   ScaleFactorCursor(int a_nCursorIndex);

	void   IntermodalIteration(int a_nIntermodalIteration);
	int    IntermodalIteration(void);

	void   IntermodalL2Norm(double a_dL2Norm);
	double IntermodalL2Norm(void); 

protected :
	ZNumberEdit m_edtHighwayRelativeGap;
	ZNumberEdit m_edtUEOBaseScaleFactor;
	int         m_nHighwayIteration;
	double      m_dHighwayRelativeGap;
	double      m_dUEOBaseScaleFactor;
	int         m_nHighwayRelativeGapExp;
	int			m_nScaleFactorCursor;
	
	int         m_nIntermodalIteration;
	double      m_dL2Norm;

protected:
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL OnInitDialog();
    afx_msg void OnCbnSelchangeComboAdjustFactor();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedOk();

protected:
    CComboBox m_cboRelativeGapExp;
	CComboBox m_cboScaleFactor;

public:
    afx_msg void OnCbnSelchangeComboRelativeGap();
	afx_msg void OnSelchangeComboScaleFactor();
};