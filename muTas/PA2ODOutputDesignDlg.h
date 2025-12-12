#pragma once

//forward  declare
class KTarget;
class KPA2ODInfo;

// KPA2ODOutputDesignDlg 대화 상자입니다.

class KPA2ODOutputDesignDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KPA2ODOutputDesignDlg)

public:
	KPA2ODOutputDesignDlg(KTarget* a_pTarget,
		std::list<KPA2ODInfo*> a_lstOutput, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPA2ODOutputDesignDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_5840_PA2ODOutputDesignDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitReportCtrl(void);
	void UpdateReportCtrl(void);

	void AddOverwriteOutputColumns(
		CXTPReportRecord* a_pParent, KPA2ODInfo* a_pSCInfo, int& a_nOverwriteCount);

private:
	KTarget* m_pTarget;
	std::list<KPA2ODInfo*> m_lstOutput;

private:
	virtual BOOL OnInitDialog();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndOutputReport;

};
