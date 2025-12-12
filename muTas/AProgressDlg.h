/**
 * @file AProgressDlg.h
 * @brief KAProgressDlg 선언 파일
 * @author 
 * @date 2011.06.24
 * @remark
 */


#pragma once


#include "afxwin.h"
#include "PictureEx.h"


/**
 * @brief Assignment 모형의 Progress dialog class
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2011.06.24
 */
class KAProgressDlg : public CXTResizeDialog
{
	DECLARE_DYNAMIC(KAProgressDlg)

public:
	KAProgressDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~KAProgressDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_9700_KAPROGRESSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public :

	/**
	* @brief Progress Dialog가 수행 할 Thread Function을 설정한다.
	* @param[in] func - Thread function
	*/
	void SetThreadFunction(void (*func)(void* pParam));

	/**
	* @brief Thread Function의 Parameter를 설정한다.
	* @param[in] pParam - Thread Function Parameter
	*/
	void SetThreadParam(void* pParam);

	/**
	* @brief Progress Dialog에서 표출 할 상태 메시지를 설정한다.
	* @param[in] strMsg - 상태 메시지
	*/
	void SetStatus(LPCTSTR strMsg);

	/**
	* @brief 사용자의 작업 취소 여부를 반환한다.
	* @return bool - 작업 취소 여부
	*/
	bool IsCancled(void);

	/**
	* @brief 현재 작업을 중지한다.
	*/
	void Stop(void);


protected :
	CPictureEx m_stcAnimate;
	CStatic m_stcStatus;
	CListBox m_lstStatus;
	CButton m_btnClose;
	bool m_bCancel;
	CString m_strCaption;
	CRITICAL_SECTION m_cs;
	void (*ThreadFunction)(void* pParam);
	void* m_pParam;
	std::queue<CString> m_StatusQueue;


public :
    /**
    * @brief 다이얼로그 초기화
    */
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
};
