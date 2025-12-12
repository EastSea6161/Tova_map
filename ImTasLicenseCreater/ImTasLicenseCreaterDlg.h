
// ImTasLicenseCreaterDlg.h : 헤더 파일
//

#pragma once


// CImTasLicenseCreaterDlg 대화 상자
class CImTasLicenseCreaterDlg : public CDialogEx
{
// 생성입니다.
public:
	CImTasLicenseCreaterDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMTASLICENSECREATER_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;
    DECLARE_MESSAGE_MAP()
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();    
	afx_msg HCURSOR OnQueryDragIcon();
protected:	
    afx_msg void OnBnClickedOk(); 
    afx_msg void OnBnClickedButton1();
private:
    bool InputDataCheck(CString& ar_strMacAddress, int& ar_nLicenseFileID, CString& ar_strUserName, 
                        CString& ar_strOrganization, CTime& ar_timeStart, CTime& ar_timeEnd);
private:
    CString GetAppPath();    
    KDBaseConnectionPtr CImTasLicenseCreaterDlg::GetLicenseDBConnection();
    void WriteDBInfo(CString ar_strMacAddress, int ar_nLicenseFileID, CString ar_strUserName, CString ar_strOrganization, CTime& ar_timeStart, CTime& ar_timeEnd, int ar_version=1);
    int  GetLastLicenseFileID();
};
