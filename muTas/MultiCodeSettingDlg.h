#pragma once

class KTarget;
class KIOColumn;
class KCodeGroup;

// KMultiCodeSettingDlg 대화 상자입니다.

class KMultiCodeSettingDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KMultiCodeSettingDlg)

public:
	KMultiCodeSettingDlg(KTarget* a_pTarget, KIOColumn* a_pColumn, CString strCodeList, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMultiCodeSettingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_2200_MultiCodeSettingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

protected:
	KTarget*				m_pTarget;
	KIOColumn*				m_pColumn;
	CString					m_strCodeList;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportContrl;

protected:
	void InitReportHeader();
	void UpdaeteReportData();

	void SelectCodeConvertToSet(std::set<int>& a_setCode);

	void ResizeComponet();
protected:

	void SaveReusult();

public:
	CString GetResult();

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckAll();
};
