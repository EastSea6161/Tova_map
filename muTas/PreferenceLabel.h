#pragma once
//^#include "ResizeDialogEx.h"
#include "afxfontcombobox.h"

// KPreferenceLabel 대화 상자입니다.

class KPreferenceLabel : public KDialogEx
{
	DECLARE_DYNAMIC(KPreferenceLabel)

public:
	KPreferenceLabel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KPreferenceLabel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0100_Preference_Label };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnInitDialog();
protected:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
    CXTPLogFont         m_lf;
    CXTPFontComboBox	m_wndComboFontNode;
    CXTPFontComboBox	m_wndComboFontLink;
private:
    afx_msg void OnSelEndOKFontNode();
    afx_msg void OnSelEndOKFontLink();

private:
    CString m_strNodeFontName;
    CString m_strLinkFontName;

private:
    CXTColorPicker  m_btnNodeColorPicker;
    CXTColorPicker  m_btnLinkColorPicker;

private:
    COLORREF m_rgbNodeColor;
    COLORREF m_rgbLinkColor;
private:
    afx_msg void OnSelEndOKColor();

private:
    double m_dNodeFontSize;
    double m_dLinkFontSize;

public:
    virtual void UserCommand(int a_nCommand=0);
};
