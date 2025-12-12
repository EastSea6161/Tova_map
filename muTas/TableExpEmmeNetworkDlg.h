#pragma once

// KTableExpEmmeNetworkDlg 대화 상자입니다.

//^#include "ResizeDialogEx.h"
//#include "StdioFileEx.h"

class KTarget;

using namespace std;

class KTableExpEmmeNetworkDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableExpEmmeNetworkDlg)

public:
	KTableExpEmmeNetworkDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableExpEmmeNetworkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4171_TableExpEmmeNetworkDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitNodeComboCtrl(void);
	void InitLinkComboCtrl(void);

	void Run(void);

	void GenNodeExportQuery(CString& ar_strQuery);
	void GenLinkExportQuery(CString& ar_strQuery);

	void WriteEmmeHeader(CStdioFileEx& ar_outFile);
	void WriteEmmeNode(CStdioFileEx& ar_outFile);
	void WriteEmmeLink(CStdioFileEx& ar_outFile);

	static void               AddStatusMessage(CString a_strMsg);
	static unsigned __stdcall ExpEmmeNetworkThreadCaller(void* p);

private:
	bool     m_bDefined;
	bool     m_bRunOK;
	KTarget* m_pTarget;
	CString  m_strFileName;

	map<int, CString> m_mapDefineModes;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnDefineModes();

private:
	CXTBrowseEdit m_editFile;
	CComboBox     m_cboNodeUser1;
	CComboBox     m_cboNodeUser2;
	CComboBox     m_cboNodeUser3;
	CComboBox     m_cboLinkUser1;
	CComboBox     m_cboLinkUser2;
	CComboBox     m_cboLinkUser3;
	
};
