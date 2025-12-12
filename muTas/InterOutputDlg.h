#pragma once

//forward declare
class KTarget;

// KInterOutputDlg 대화 상자입니다.

class KInterOutputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterOutputDlg)

public:
	KInterOutputDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterOutputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6980_InterOutputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitReportCtrl(void);
	void UpdateReportCtrl(void);

private:
	KTarget* m_pTarget;

private:
	virtual BOOL OnInitDialog();

private:
	CXTPReportControl m_wndReportCtrl;
};
