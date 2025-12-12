#pragma once
#include "afxcolorbutton.h"
#include "afxcmn.h"
#include "afxwin.h"


// KMapBackgroundColorDlg 대화 상자입니다.

class KMapBackgroundColorDlg : public KDialogEx
{
	DECLARE_DYNAMIC(KMapBackgroundColorDlg)

public:
	KMapBackgroundColorDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KMapBackgroundColorDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_2201_MAP_BACKGROUND };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	Gdiplus::Color m_crBackground;

	DECLARE_MESSAGE_MAP()

public:
	int GetMapBackgroundColorMode();
	void SetMapBackgroundColorMode(int mode);
	Gdiplus::Color GetMapBackgroundColor();
	void SetMapBackgroundColor(Gdiplus::Color cr);

public:
	int m_nColorType;
	CXTColorPicker  m_btnColorPick;
	CSliderCtrl m_sldTransparent;
	ZNumberEdit m_edtTransparent;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedRadio(UINT id);
};
