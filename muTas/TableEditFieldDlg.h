#pragma once

class KIOTable;
class KIOColumn;
class KExistNameChecker;
// KTableEditFieldDlg 대화 상자입니다.

class KTableEditFieldDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KTableEditFieldDlg)

public:
	KTableEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTableEditFieldDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4156_EditFieldDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool         InvalidateInput();

private:
	KIOTable*    m_pIOTable;
	KIOColumn*   m_pIOColumnEdit;
	KExistNameChecker* m_pNameChecker;

private:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* a_pMsg);


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
