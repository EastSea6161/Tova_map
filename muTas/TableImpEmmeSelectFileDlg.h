#pragma once

class KTarget;

// KTableImpEmmeSelectFileDlg 대화 상자입니다.

class KTableImpEmmeSelectFileDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableImpEmmeSelectFileDlg)

public:
	KTableImpEmmeSelectFileDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableImpEmmeSelectFileDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4166_ImportTableEmme1Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CString  m_strFile;

private:
	KTarget* m_pTarget;
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

private:
	CXTBrowseEdit m_editFile;

};
