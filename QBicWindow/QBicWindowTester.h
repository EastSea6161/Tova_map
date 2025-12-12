#pragma once


class AFX_EXT_CLASS QBicWindowTester : public CDialogEx
{
	DECLARE_DYNAMIC(QBicWindowTester)

public:
	QBicWindowTester(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~QBicWindowTester();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
