#pragma once
#include "resource.h"
#include "QBicPictureEx.h"

class QBicSimpleProgressParameter;

typedef unsigned (__stdcall *ThreadCallBackFunction)(void*);
/**
*@brief 심플 프로그래스
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.1
*@remark -2019.06.11 : v1.0.1 언어설정 기능 추가 
*/
class AFX_EXT_CLASS QBicSimpleProgress : public CDialog
{    
	DECLARE_DYNAMIC(QBicSimpleProgress)

private:
	QBicSimpleProgress(ThreadCallBackFunction a_fnPtr, void* pParameter, CWnd* pParent = NULL, int nLang = 1);   
public:
	virtual ~QBicSimpleProgress();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_QW_0510_ProgressDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	/**
	    * @brief Progress Dialog가 수행 할 Thread function을 설정한다.
	    * @param[in] func - Thread function 
	    */
	void SetThreadCallBackFunction(unsigned (__stdcall *func)(void* pParam));

	/**
	    * @brief Thread function의 parameter를 설정한다.
	    * @param[in] pParam - Thread function의 parameter
	    */
	void SetThreadParam(void* pParam);

	void SetEnableStop(bool a_bEnable=true)
	{
		m_bEnabledStop = a_bEnable;
	}

public :	
	/**
	    * @brief Progress Dialog의 상태 메시지를 설정한다.
	    * @param[in] strMsg - 상태 메시지
	    */
	void SetStatus(LPCTSTR strMsg);

	/**
	    * @brief 사용자가 작업을 취소했는지 여부를 반환한다.
	    * @return bool - true:사용자가 작업을 취소, false:사용자가 작업을 취소하지 않음
	    */
	bool IsUserStopped(void);

    /**
	    * @brief 작업을 종료한다.
	    */
	void Stop(void);	

protected :
	CStatic       m_stcStatus;
	QBicPictureEx m_stcAnimate;
    
    CStatic    m_stcStartTime;
    CStatic    m_stcProgessTime;

protected:
	bool       m_bStop;
	bool       m_bUserKilled;
	bool       m_bEnabledStop;
    CString    m_strCaption;
    CString    m_strStatus;

    HANDLE   m_hThread;
    ThreadCallBackFunction CallBackFunction;
	void* m_pParam;
    QBicSimpleProgressParameter* m_pSimpleProgressParameter;
	
	CRITICAL_SECTION m_cs;
    CTime  m_timeStart;
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
    
    friend class QBicSimpleProgressThread;	

private:
	int m_nLang;	// 언어설정, 0=한국어, 1=영어

public:
	virtual INT_PTR DoModal();
};


typedef std::shared_ptr<QBicSimpleProgress> QBicSimpleProgressPtr;
/**
*@brief 심플 프로그레스 스레드
*@details 
*@author 큐빅웨어
*@date 2016/05/17
*@version 1.0.1
*@remark -2019.06.11 : v1.0.1 언어설정 기능 추가 
*/
class AFX_EXT_CLASS QBicSimpleProgressThread
{
private:
    QBicSimpleProgressThread()
    {
    }
            
public:
    ~QBicSimpleProgressThread()
    {
    }

private:
    static QBicSimpleProgressPtr m_spProgress;
public:
	/**
	*@brief 스레드 제거
	*@param ThreadCallBackFunction a_fnPtr	해당 스레드
	*@param void* a_pParameter				파라미터
	*@param bool bEnableStop				정지 여부
	*@param nLang : progress 다이얼로그 언어설정 값(0=한국어, 1=영어)
	*@return 
	*/
    static INT_PTR ExecuteThread(ThreadCallBackFunction a_fnPtr, void* a_pParameter, bool bEnableStop = false, 
		int nLang = 1);
	/**
	*@brief 심플 프로그레스
	*@return 프로그레스
	*/
    static QBicSimpleProgressPtr SimpleProgress();    
	/**
	*@brief 유저 중지 상태 확인
	*@return 값 반환
	*/
	static bool IsUserStopped();
};


