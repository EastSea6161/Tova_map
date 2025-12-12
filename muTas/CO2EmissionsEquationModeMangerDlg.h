#pragma once


// KCO2EmissionsEquationModeMangerDlg 대화 상자입니다.

class KCO2EmissionsEquationModeMangerDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KCO2EmissionsEquationModeMangerDlg)

public:
	KCO2EmissionsEquationModeMangerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KCO2EmissionsEquationModeMangerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_8542_KCO2EmissionsEquationModeMangerDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline void SetModeMap(std::map<int, CString> a_mapMode)
	{
		m_mapMode = a_mapMode;
	}

	inline void SetUsedMode(std::set<int> a_setUsedModeID)
	{
		m_setUsedModeID = a_setUsedModeID;
	}

	inline std::map<int, CString> GetModeMap(void)
	{
		return m_mapMode;
	}

private:
	void InitComboCtrl(void);

private:
	std::map<int, CString> m_mapMode;
	std::set<int> m_setUsedModeID;

	int m_nNextModeID;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedAdd();

private:
	CComboBox m_cboMode;
};
