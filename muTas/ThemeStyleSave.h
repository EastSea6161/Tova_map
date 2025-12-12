#pragma once
//^#include "ResizeDialogEx.h"

// KThemeStyleSave 대화 상자입니다.

class KThemeStyleSave : public KResizeDialogEx
{
	DECLARE_DYNAMIC(KThemeStyleSave)

public:
	KThemeStyleSave(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KThemeStyleSave();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TOC_STYLE_SAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
private:
	CString m_strExistStyleName;
	CString m_strNewStyleName;
	bool    m_bOverwrite;
public:
	void SetExistStyleName(CString a_strExistStyleName);
	bool IsOverWrite()
	{
		return m_bOverwrite;
	}

	CString GetNewStyleName()
	{
		return m_strNewStyleName;
	}
	afx_msg void OnEnSetfocusEditStyleName();
};
