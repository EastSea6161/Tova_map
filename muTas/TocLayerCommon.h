#pragma once


// KTocLayerCommon 대화 상자입니다.

class KTocLayerCommon : public KDialogEx
{
	DECLARE_DYNAMIC(KTocLayerCommon)

public:
	KTocLayerCommon(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTocLayerCommon();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_LAYER_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
