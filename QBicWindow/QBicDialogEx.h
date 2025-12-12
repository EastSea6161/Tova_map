#pragma once
/**
*@brief Modeless Feedback
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS ImChampModelessFeedback
{
public:
    ImChampModelessFeedback()         {}
    virtual ~ImChampModelessFeedback(){}
public:
	/**
	*@brief 모달리스 피드백 OK
	*@param UINT nDialogID	다이얼로그 ID
	*/
    virtual void ModelessOKFeedback(UINT nDialogID) = 0;
	/**
	*@brief 모달리스 피드백 Cancel
	*@param UINT nDialogID	다이얼로그 ID
	*/
	virtual void ModelessCancleFeedback(UINT nDialogID) = 0;
};
/**
*@brief ResizeDialog 설정
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KResizeDialogEx : public CDialog, public CXTPResize
{
	/// Constructor and Destructor
public:
	KResizeDialogEx();
	KResizeDialogEx(const UINT nID, CWnd* pParent = 0, const UINT nFlags = 0); // <combine CXTPResizeDialog::CXTPResizeDialog>
	KResizeDialogEx(LPCTSTR lpszTemplateName, CWnd* pParent = 0, const UINT nFlags = 0); // <combine CXTPResizeDialog::CXTPResizeDialog>

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	UINT m_nDialogID; // Contains the ID number of a dialog box template resource
private:
    CToolTipCtrl m_tooltipCtrl;
protected:
    void AddImChampTooltip(UINT nID, CString strTooltipMsg);
	void UpdateImChampTooltip(UINT nID, CString strTooltipMsg);
    // 키보드 Cancel/Enter 사용여부
private:
    bool m_bUseEscape;
    bool m_bUseReturn;
public:
    void UseKeyEscEnter(bool UseEscape=true, bool bUseReturn=true)
    {
        m_bUseEscape = UseEscape;
        m_bUseReturn = bUseReturn;
    }

// 사용자가 다이얼로그를 Close(Cancel, OK) 했는지 여부 : 스마트 포인터, 모들리스 다이얼로그에서의 처리 문제
private:
	bool m_bUserClosed;
public:
	bool IsUserClosed();
protected:
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

    void SetNativeLanguageMode (UINT nID);
    void SetEnglishLanguageMode(UINT nID);
private:
    ImChampModelessFeedback* m_pModelessReceiver;
public:
    void SetModelessReceiver(ImChampModelessFeedback* a_pReceiver);
protected:
    virtual void UserCommand(int a_nCommand=0) {}

public:
	virtual void ActivatedFrameWndNotifyProcess(){}
	virtual void DeActivatedFrameWndNotifyProcess(){}
};

namespace KDialogFeedback
{
    enum KEMActionFeedback
    {
        KEMActionFeedbackActivate, 
        KEMActionFeedbackDeActivate     
    };
}

//---------------------------------------------------------------------------------------------------------
/**
*@brief 다이얼로그 설정
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KDialogEx : public CDialogEx, public CXTPResize
{
    /// Constructor and Destructor
public:
    KDialogEx();
    KDialogEx(const UINT nID, CWnd* pParent = 0, const UINT nFlags = 0); 
    KDialogEx(LPCTSTR lpszTemplateName, CWnd* pParent = 0, const UINT nFlags = 0); 

protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnDestroy();
protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    UINT m_nDialogID; 
private:
    CToolTipCtrl m_tooltipCtrl;
protected:
    void AddImChampTooltip(UINT nID, CString strTooltipMsg);
	void UpdateImChampTooltip(UINT nID, CString strTooltipMsg);

    // 키보드 Cancel/Enter 사용여부
private:
    bool m_bUseEscape;
    bool m_bUseReturn;
public:
    void UseKeyEscEnter(bool UseEscape=true, bool bUseReturn=true)
    {
        m_bUseEscape = UseEscape;
        m_bUseReturn = bUseReturn;
    }

    // 사용자가 다이얼로그를 Close(Cancel, OK) 했는지 여부 : 스마트 포인터, 모들리스 다이얼로그에서의 처리 문제
private:
    bool m_bUserClosed;
public:
    bool IsUserClosed();
protected:
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    void SetNativeLanguageMode (UINT nID);
    void SetEnglishLanguageMode(UINT nID);

public:
    virtual void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode){}
private:
    ImChampModelessFeedback* m_pModelessReceiver;
public:
    void SetModelessReceiver(ImChampModelessFeedback* a_pReceiver);
protected:
    virtual void UserCommand(int a_nCommand=0) {}

public:
    virtual void ActivatedFrameWndNotifyProcess(){}
    virtual void DeActivatedFrameWndNotifyProcess(){}
};



//-----------------------------------------------------------------------------------------------------------
/**
*@brief 흰색 다이얼로그(?) 설정
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.0
*/
class AFX_EXT_CLASS KWhiteDlgEx : public CDialogEx, public CXTPResize
{
    /// Constructor and Destructor
public:
    KWhiteDlgEx();
    KWhiteDlgEx(const UINT nID, CWnd* pParent = 0, const UINT nFlags = 0); 
    KWhiteDlgEx(LPCTSTR lpszTemplateName, CWnd* pParent = 0, const UINT nFlags = 0); 

protected:
    DECLARE_MESSAGE_MAP()
    virtual BOOL OnInitDialog();
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnDestroy();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    UINT m_nDialogID; 
private:
    CToolTipCtrl m_tooltipCtrl;
protected:
    void AddImChampTooltip(UINT nID, CString strTooltipMsg);
	void UpdateImChampTooltip(UINT nID, CString strTooltipMsg);

    // 키보드 Cancel/Enter 사용여부
private:
    bool m_bUseEscape;
    bool m_bUseReturn;
public:
    void UseKeyEscEnter(bool UseEscape=true, bool bUseReturn=true)
    {
        m_bUseEscape = UseEscape;
        m_bUseReturn = bUseReturn;
    }

    // 사용자가 다이얼로그를 Close(Cancel, OK) 했는지 여부 : 스마트 포인터, 모들리스 다이얼로그에서의 처리 문제
private:
    bool m_bUserClosed;
public:
    bool IsUserClosed();
protected:
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    void SetNativeLanguageMode (UINT nID);
    void SetEnglishLanguageMode(UINT nID);

public:
    virtual void ActionFeedback(KDialogFeedback::KEMActionFeedback emActionCode){}
private:
    ImChampModelessFeedback* m_pModelessReceiver;
public:
    void SetModelessReceiver(ImChampModelessFeedback* a_pReceiver);
protected:
    virtual void UserCommand(int a_nCommand=0) {}

public:
    virtual void ActivatedFrameWndNotifyProcess(){}
    virtual void DeActivatedFrameWndNotifyProcess(){}
};