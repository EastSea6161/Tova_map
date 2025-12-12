#pragma once


// KThemeNodePropertiesMulti 대화 상자입니다.

class KThemeNodePropertiesMulti : public KDialogEx
{
	DECLARE_DYNAMIC(KThemeNodePropertiesMulti)

public:
	KThemeNodePropertiesMulti(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeNodePropertiesMulti();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_NODE_PROPERTIES_MULTI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
