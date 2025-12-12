#pragma once

//^#include "ResizeDialogEx.h"

class KTarget;
class CStdioFileEx;

// KTableExpEmmeMatrixDlg 대화 상자입니다.

class KTableExpEmmeMatrixDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KTableExpEmmeMatrixDlg)

public:
	KTableExpEmmeMatrixDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableExpEmmeMatrixDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4175_TableExpEmmeMatrixDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void InitMatrixComboCtrl(void);
	void OnCbnSelChangeMatrix(void);
	void InitColumnComboCtrl(void);

	static void               AddStatusMessage(CString a_strMsg);
	static unsigned __stdcall ExpEmmeMatrixThreadCaller(void* p);

	void Run(void);

	void WriteEmmeHeader(CStdioFileEx& ar_outFile);
	void WriteEmmeMatrix(CStdioFileEx& ar_outFile);

private:
	KTarget* m_pTarget;
	CString  m_strFileName;
	bool     m_bThreadOK;
	CString  m_strErrMsg;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CXTPBrowseEdit m_editFile;
	CComboBox      m_cboMatrix;
	CComboBox      m_cboColumn;
};
