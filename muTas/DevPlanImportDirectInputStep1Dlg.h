#pragma once


// KDevPlanImportDirectInputStep1Dlg 대화 상자입니다.

class KDevPlanImportDirectInputStep1Dlg : public KDialogEx
{
	DECLARE_DYNAMIC(KDevPlanImportDirectInputStep1Dlg)

public:
	KDevPlanImportDirectInputStep1Dlg(
		BOOL a_bContainHeader, int a_nSeparatorType, int a_nEncodingType,
		CString a_strFileName, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDevPlanImportDirectInputStep1Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5611_DevPlanImportStep1Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void	ContainHeader(BOOL a_bContainHeader);
	BOOL	ContainHeader(void);

	void	SeparatorType(int a_nSeparatorType);
	int		SeparatorType(void);

	void	EncodingType(int a_nEncodingType);
	int		EncodingType(void);

	void	FileName(CString a_strFileName);
	CString FileName(void);

private:
	BOOL	m_bContainHeader;
	int		m_nSeparatorType;
	int		m_nEncodingType;
	CString	m_strFileName;

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

private:
	CXTBrowseEdit m_edtFile;
};
