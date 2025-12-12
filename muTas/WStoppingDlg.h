#pragma once


// KWStoppingDlg 대화 상자입니다.

class KWStoppingDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KWStoppingDlg)

public:
	KWStoppingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KWStoppingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6936_WStoppingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	
	inline int TAIteration(void)
	{
		return m_nTAIteration;
	}

	inline void TAIteration(int a_nTAIteration)
	{
		m_nTAIteration = a_nTAIteration;
	}

	inline int UEIteration(void)
	{
		return m_nUEIteration;
	}

	inline void UEIteration(int a_nUEIteration)
	{
		m_nUEIteration = a_nUEIteration;
	}

	inline double RelationGap(void)
	{
		return m_dRelativeGap;
	}

	inline void RelationGap(double a_dRelationGap)
	{
		m_dRelativeGap = a_dRelationGap;
	}

	inline double ScaleFactor(void)
	{
		return m_dScaleFactor;
	}

	inline void ScaleFactor(double a_dScaleFactor)
	{
		m_dScaleFactor = a_dScaleFactor;
	}

	inline int ScaleFactorCursor(void)
	{
		return m_nScaleFactorCursor;
	}

	inline void ScaleFactorCursor(int a_nScaleFacorCursor)
	{
		m_nScaleFactorCursor = a_nScaleFacorCursor;
	}

	inline int RelatevGapExp (void)
	{
		return m_nHighwayRelativeGapExp;
	}

	inline void RelatevGapExp(int a_nRelatevExp)
	{
		m_nHighwayRelativeGapExp = a_nRelatevExp;
	}

	inline double L2Norm(void)
	{
		return m_dL2Norm;
	}

	inline void	L2Norm(double a_dL2Norm)
	{
		m_dL2Norm = a_dL2Norm;
	}


protected:

	ZNumberEdit m_edtRelativeGap;
	ZNumberEdit m_edtScaleFactor;
	ZNumberEdit m_edtL2Norm;

	int			m_nTAIteration;
	int			m_nUEIteration;

	double		m_dRelativeGap;
	double		m_dScaleFactor;
	double		m_dL2Norm;

	int         m_nHighwayRelativeGapExp;
	int			m_nScaleFactorCursor;

	CComboBox m_cboRelativeGapExp;
	CComboBox m_cboScaleFactor;

protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboRelativeGap();
	afx_msg void OnSelchangeComboScaleFactor();
};
