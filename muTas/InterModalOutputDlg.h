#pragma once
#include "afxwin.h"

class TOutputName
{
public:
    TOutputName() {
        TName    = _T("");
        TCaption = _T("");
    }

    TOutputName(CString strName, CString strCaption) {
        TName    = strName;
        TCaption = strCaption;
    }
    ~TOutputName(){}
public:
    CString TName;
    CString TCaption;
};

class KTarget;

class KInterModalOutputDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KInterModalOutputDlg)

public:
	KInterModalOutputDlg(KTarget* a_pTarget, CString strDescription, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterModalOutputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6876_InterModalOuputDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();

protected:
	CString				m_strDescription;
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;

	KTarget* m_pTarget;

private:
	CString   m_strNodeGroupName;
	CString   m_strLinkGroupName;
	CString   m_strLineGroupName;
	CString   m_strTransitGroupName;
	CString   m_strODGroupName;
	CString   m_strTerminalGroupName;
    
protected:
	void InitReportHeader();
	void UpdateReportData();

protected:	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
    
public:
    void GetDescription(CString& a_strDescription);
private:
    std::vector<TOutputName> m_vecMode;
public:
    void SetModeOD(std::vector<TOutputName>& vecName) {
        m_vecMode = vecName;
    }
};
