
#pragma once
class KTarget;
class KGenerationModelInfo;

// KGenerationSelectModelDlg 대화 상자입니다.

class KGenerationSelectModelDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KGenerationSelectModelDlg)

public:
	KGenerationSelectModelDlg(KTarget* a_pTarget, std::map<int, KGenerationModelInfo>& a_mapGenerationModelInfo, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KGenerationSelectModelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6001_GenerationSelectModelDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
	KTarget*			m_pTarget;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

protected:
	std::map<int, KGenerationModelInfo>& m_mapGenerationModelInfo;


protected:
	void InitReportHeader(void);
	void UpdateReportData(void);

protected:
	bool CheckUseData(void);


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
