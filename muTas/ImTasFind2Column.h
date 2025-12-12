#pragma once


// KImTasFind2Column 대화 상자입니다.

class KImTasFind2Column : public CDialogEx
{
	DECLARE_DYNAMIC(KImTasFind2Column)

public:
	KImTasFind2Column(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KImTasFind2Column();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_3013_FIND_ID2COLUMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
