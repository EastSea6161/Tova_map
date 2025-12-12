#pragma once

class KTarget;
class KIOColumns;
// KDefVDFFactorDlg 대화 상자입니다.

//^#include "ResizeDialogEx.h"

using namespace std;

class KDefVDFFactorDlg : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KDefVDFFactorDlg)

public:
	KDefVDFFactorDlg(KTarget* a_pTarget, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KDefVDFFactorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4169_DefVDFFactorDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	inline void SetUserColumns(KIOColumns* a_pColumns)
	{
		m_pUserDefineColumns = a_pColumns;
	}

	inline void FnID(std::set<int> a_setFnID)
	{
		m_setFnID = a_setFnID;
	}

private:
	void   InitColumnInfo(void);

	void   InitReportHeader(void);
	void   UpdateReportData(void);

	double GetDefaultValue(CString strColumnName);

public:
	void   Apply();

private:
	KTarget*    m_pTarget;
	KIOColumns* m_pColumns;
	KIOColumns* m_pUserDefineColumns;

private:
	set<int> m_setFnID;
	bool     m_bOK;

private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReportCtrl;
};
