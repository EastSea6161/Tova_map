#pragma once
#include "afxwin.h"

class CSplashDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CSplashDlg)

public:
    CSplashDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
    virtual ~CSplashDlg();

    // 대화 상자 데이터입니다.
    enum { IDD = IDD_SPLASH };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    
    DECLARE_MESSAGE_MAP()
protected:
    virtual BOOL OnInitDialog();    
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
    void ShowSplashScreen(CWnd* pParentWnd /*= NULL*/);
    void HideSplashScreen();
};

static CSplashDlg* c_pSplashDlg;