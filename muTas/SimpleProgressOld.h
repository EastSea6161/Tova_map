#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "PictureEx.h"
#include "resource.h"


/**
* @brief 작업의 진행상태를 표시하는 Dialog class 
* @remark 
* @warning 
* @version 1.0
* @author 
* @date 2011.08.23
*/


class KSimpleProgressOld : public CDialog
{
	DECLARE_DYNAMIC(KSimpleProgressOld)

public:
	KSimpleProgressOld(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KSimpleProgressOld();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_0500_ProgressDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	
public :

	/**
	 * @brief Progress Dialog에서 표출 할 제목을 설정한다.
	 * @param[in] strCaption - Progress Dialog 의 제목
	 */
	void Caption(LPCTSTR strCaption);

	/**
	 * @brief Progress Dialog가 수행 할 Thread function을 설정한다.
	 * @param[in] func - Thread function 
	 */
	void SetThreadCallBackFunction(void (*func)(void* pParam));

	/**
	 * @brief Thread function의 parameter를 설정한다.
	 * @param[in] pParam - Thread function의 parameter
	 */
	void SetThreadParam(void* pParam);
	
	/**
	 * @brief Progress Dialog의 상태 메시지를 설정한다.
	 * @param[in] strMsg - 상태 메시지
	 */
	void SetStatus(LPCTSTR strMsg);

	/**
	 * @brief 사용자가 작업을 취소했는지 여부를 반환한다.
	 * @return bool - true:사용자가 작업을 취소, false:사용자가 작업을 취소하지 않음
	 */
	bool IsCancled(void);

	/**
	 * @brief 작업을 종료한다.
	 */
	void Stop(void);

protected :
	CStatic    m_stcStatus;
	CPictureEx m_stcAnimate;

protected:
	bool    m_bCancel;
    CString m_strCaption;
    CString m_strStatus;

	void (*ThreadCallBackFunction)(void* pParam);
	void* m_pParam;
	
	CRITICAL_SECTION m_cs;
    HANDLE m_hThread;
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
};

extern KSimpleProgressOld* g_pProgressDlg;



