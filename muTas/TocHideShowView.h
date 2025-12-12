#pragma once



// KTocHideShowView 폼 뷰입니다.

class KTocHideShowView : public CFormView
{
	DECLARE_DYNCREATE(KTocHideShowView)

protected:
	KTocHideShowView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~KTocHideShowView();

public:
	enum { IDD = IDD_TOC_HIDE_SHOW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
protected:
    virtual void OnInitialUpdate();
protected:
    afx_msg void OnBnClickedButtonShow();
};


