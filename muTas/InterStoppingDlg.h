#pragma once


// KInterStoppingDlg 대화 상자입니다.

class KInterStoppingDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterStoppingDlg)

public:
	KInterStoppingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterStoppingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6875_InterStoppingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	inline int InterModalIteration(void)
	{
		return m_nInterModalIteration;
	}

	inline void InterModalIteration(int a_nOutterIteration)
	{
		m_nInterModalIteration = a_nOutterIteration;
	}

	inline int Multimodaliteration(void)
	{
		return m_nMultimodalIteration;
	}

	inline void Multimodaliteration(int a_nInnerIteration)
	{
		m_nMultimodalIteration = a_nInnerIteration;
	}

	inline double InterModalL2Norm(void)
	{
		return m_dInterModalL2Norm;
	}

	inline void InterModalL2Norm(double a_dOutterL2Norm)
	{
		m_dInterModalL2Norm = a_dOutterL2Norm;
	}

	inline double MultiModalL2Norm(void)
	{
		return m_dMultimodalL2Norm;
	}

	inline void MultiModalL2Norm(double a_dInnerL2Norm)
	{
		m_dMultimodalL2Norm = a_dInnerL2Norm;
	}

	inline int UEIteration(void)
	{
		return m_nUEIteration;
	}

	inline void UEIteration(int a_nUEIteration)
	{
		m_nUEIteration = a_nUEIteration;
	}

	inline int RelatevGapExp (void)
	{
		return m_nHighwayRelativeGapExp;
	}

	inline void RelatevGapExp(int a_nRelatevExp)
	{
		m_nHighwayRelativeGapExp = a_nRelatevExp;
	}

	inline int ScaleFactorCursor(void)
	{
		return m_nScaleFactorCursor;
	}

	inline void ScaleFactorCursor(int a_nScaleFacorCursor)
	{
		m_nScaleFactorCursor = a_nScaleFacorCursor;
	}

	inline double ScaleFactor(void)
	{
		return m_dScaleFactor;
	}

	inline void ScaleFactor(double a_dScaleFactor)
	{
		m_dScaleFactor = a_dScaleFactor;
	}

	inline double RelationGap(void)
	{
		return m_dRelativeGap;
	}

	inline void RelationGap(double a_dRelationGap)
	{
		m_dRelativeGap = a_dRelationGap;
	}

protected:

	ZNumberEdit m_edtRelativeGap;
	ZNumberEdit m_edtScaleFactor;
	ZNumberEdit m_edtOutL2Norm;
	ZNumberEdit m_edtInL2Norm;

	int			m_nInterModalIteration;
	int			m_nMultimodalIteration;

	double		m_dInterModalL2Norm;
	double		m_dMultimodalL2Norm;

	int			m_nUEIteration;

	int         m_nHighwayRelativeGapExp;
	int			m_nScaleFactorCursor;

	double		m_dRelativeGap;
	double		m_dScaleFactor;

	CComboBox m_cboRelativeGapExp;
	CComboBox m_cboScaleFactor;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboRelativeGap();
	afx_msg void OnSelchangeComboScaleFactor();
    afx_msg void OnBnClickedOk();
};
