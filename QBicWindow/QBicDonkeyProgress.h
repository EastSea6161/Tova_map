#pragma once
#include "QBicPictureEx.h"
/**
*@brief 당나귀프로세스???
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.1
*@remark -2019.06.11 : v1.0.1 언어설정 기능 추가 
*/
class AFX_EXT_CLASS QBicDonkeyProgress : public CWnd
{
    // Construction/Destruction
public:
    QBicDonkeyProgress(int nLang = 1);
    QBicDonkeyProgress(CWnd* pParent, LPCTSTR pszTitle=_T("Progress"), BOOL bModal = TRUE, BOOL bProgress = FALSE, BOOL bSmooth = FALSE, int nLang = 1);
    virtual ~QBicDonkeyProgress();

    BOOL Create(CWnd* pParent, LPCTSTR pszTitle, BOOL bModal = TRUE, BOOL bProgress = FALSE, BOOL bSmooth = FALSE);
    BOOL GoModal(LPCTSTR pszTitle =_T("Progress"), BOOL bSmooth = FALSE);
    void Close();
protected:
    void CommonConstruct();

    // Operations
public:
    void SetRange(int nLower, int nUpper, int nStep = 1);
    // Set range and step size
    int OffsetPos(int nPos);                        // Same as CProgressCtrl
    int StepIt();                                   //    "
    int SetStep(int nStep);                         //    "
    int SetPos(int nPos);                           //    "

    //void SetText(LPCTSTR fmt, ...);                 // Set text in text area
    void SendTextEvent(const CString& strMsg);
    void SetText(const CString& strMsg);
    void Clear();                                   // Clear text, reset bar
    void Hide();                                    // Hide window
    void Show();                                    // Show window

    BOOL Cancelled() { return m_bCancelled; }       // Was "Cancel" hit?

    void SetWindowSize(int nNumTextLines, int nWindowWidth = 390, BOOL bProgress = FALSE);

    void PeekAndPump(BOOL bCancelOnESCkey = TRUE);  // Message pumping for modal operation   

    // create by nombara
    void SlowPeekAndPump(const CString& strMsg, BOOL bCancelOnESCkey = TRUE);

    // Implementation
    int m_nSlowPeekRequestNo;
protected:
    void GetPreviousSettings();
    void SaveCurrentSettings();

protected:
    CWnd * m_wRenenableWnd;

    BOOL m_bCancelled;
    BOOL m_bModal;
    BOOL m_bPersistantPosition;
    int  m_nPrevPos, m_nPrevPercent;
    int  m_nStep;
    int  m_nMaxValue, m_nMinValue;
    int  m_nNumTextLines;

    CStatic       m_Text;
    CProgressCtrl m_wndProgress;
    CButton       m_CancelButton;
    CString       m_strTitle, m_strCancelLabel;
    CFont         m_font;
    QBicPictureEx m_Picture;
    CStatic       m_Devider;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(KDonkeyProgress)
public:
    virtual BOOL DestroyWindow();
    //}}AFX_VIRTUAL

    // Generated message map functions
protected:
    //{{AFX_MSG(KDonkeyProgress)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    afx_msg void OnCancel();
    DECLARE_MESSAGE_MAP()

	// 2019.06.11 언어설정 관련 수정
private:
	int m_nLang;	// 언어설정, 0=한국어, 1=영어
public: 
	static const CString DEFAULT_TITLE;
};