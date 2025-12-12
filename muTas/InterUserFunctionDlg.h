#pragma once

#include "resource.h"

/* Forward declarations */
class KTarget;
class KCodeGroup;
class KInterFunctionUser;
class KExpressionChecker;
class KInterFunctionUserEXP;

// KInterUserFunctionDlg 대화 상자입니다.

class KInterUserFunctionDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KInterUserFunctionDlg)

public:
	KInterUserFunctionDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterUserFunctionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6877_InterUserFunctionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult);
	afx_msg LRESULT OnFunctionEdit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCancel();

public:
	KInterFunctionUser* GetFunction(void);

	void SetFunction(KInterFunctionUser* a_pFunction);

private:
	CXTPOfficeBorder<CXTPReportControl,false> m_wndReport;
	KTarget*			m_pTarget;
	KCodeGroup*			m_pCodeData;
	KInterFunctionUser* m_pFunctionUser;
	KExpressionChecker* m_pExpressionChecker;

	void initExpressionChecker(void);

	void initReportControl(void);

	void updateReportRecord(void);
	
	void generateUserFunction(LPCTSTR strFunction, KInterFunctionUserEXP& function);
	    
	CString getFieldName(LPCTSTR strCaption);

};
