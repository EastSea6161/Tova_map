#pragma once


// KWStoopingTransitDlg 대화 상자입니다.

class KWStoopingTransitDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KWStoopingTransitDlg)

public:
	KWStoopingTransitDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KWStoopingTransitDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6936_WStoopingTransitDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:

	inline int OutterIteration(void)
	{
		return m_nOutterIteration;
	}

	inline void OutterIteration(int a_nOutterIteration)
	{
		m_nOutterIteration = a_nOutterIteration;
	}

	inline int InnerIteration(void)
	{
		return m_nInnerIteration;
	}

	inline void InnerIteration(int a_nInnerIteration)
	{
		m_nInnerIteration = a_nInnerIteration;
	}

	inline double OutterL2Norm(void)
	{
		return m_dOutterL2Norm;
	}

	inline void OutterL2Norm(double a_dOutterL2Norm)
	{
		m_dOutterL2Norm = a_dOutterL2Norm;
	}

	inline double InnerL2Norm(void)
	{
		return m_dInnerL2Norm;
	}

	inline void InnerL2Norm(double a_dInnerL2Norm)
	{
		m_dInnerL2Norm = a_dInnerL2Norm;
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

	int			m_nOutterIteration;
	int			m_nInnerIteration;

	double		m_dOutterL2Norm;
	double		m_dInnerL2Norm;

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
