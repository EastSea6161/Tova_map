#pragma once

#include "resource.h"

// KInterDefineCostDlg 대화 상자입니다.

class KInterDefineCostDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KInterDefineCostDlg)

public:
	KInterDefineCostDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KInterDefineCostDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_6910_InterDefineCostDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
