#pragma once
#include "TabSubSample.h"

// KTabSubSample 대화 상자입니다.

class KTabSubSample : public KDialogEx
{
	DECLARE_DYNAMIC(KTabSubSample)

public:
	KTabSubSample(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTabSubSample();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TAB_SUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual BOOL OnInitDialog();
};
