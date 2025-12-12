#pragma once
#include "afxwin.h"


// KTransitPathDirectionSelect 대화 상자입니다.

class KTransitPathDirectionSelect : public KDialogEx
{
	DECLARE_DYNAMIC(KTransitPathDirectionSelect)

public:
	KTransitPathDirectionSelect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KTransitPathDirectionSelect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_4100_TRANSITPATH_DIRECTION_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
private:
    CPoint m_windowPosition;
public:
    void SetWindowPosition(CPoint a_point)
    {
        m_windowPosition = a_point;
    }

private:
    afx_msg void OnCbnSelchangeCombo1();
public:
    void SetSelect(CString a_strCaption, std::vector<CString> a_vecSelect, int a_nInitIndex=0);
    void SetEnablePass(bool a_bEnablePass = true);
    int  GetSelectedIndex();
private:
    CComboBox m_cboSelect;
    CString   m_strCaption;
    std::vector<CString> m_vecSelect;
    int       m_nSelectedIndex;
    bool      m_bEnablePass;
    int       m_nInitIndex;
private:
    afx_msg void OnBnClickedOk();
};
