// SimpleProgress.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QBicSimpleProgress.h"
#include "QBicProgressParameter.h"

#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include "QBicPictureEx.h"

IMPLEMENT_DYNAMIC(QBicSimpleProgress, CDialog)

QBicSimpleProgress::QBicSimpleProgress(ThreadCallBackFunction a_fnPtr, void* pParameter, CWnd* pParent /*=NULL*/, 
	int nLang /*=1*/)
	: CDialog(QBicSimpleProgress::IDD, pParent)
{
    m_bStop        = true;
	m_bUserKilled  = false;
	m_bEnabledStop = false;
    m_hThread   = NULL;
	m_nLang = nLang;

	if (0 == m_nLang)
	{
		m_strStatus = _T("데이터 처리 중입니다...\n잠시만 기다려 주세요...");
	}
	else if(1 == m_nLang)
	{
		m_strStatus = _T("Please wait... data processing...");
	}

    CallBackFunction = a_fnPtr;
    m_pParam         = pParameter;

    m_pSimpleProgressParameter = new QBicSimpleProgressParameter(this, pParameter);
	::InitializeCriticalSection(&m_cs);
}

QBicSimpleProgress::~QBicSimpleProgress()
{
    if (m_hThread)
    {
        CloseHandle(m_hThread);
    }

    if (m_pSimpleProgressParameter)
    {
        delete m_pSimpleProgressParameter;
    }

	::DeleteCriticalSection(&m_cs);
}

void QBicSimpleProgress::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STCSTATUS, m_stcStatus);
    DDX_Control(pDX, IDC_STCANIMATE, m_stcAnimate);
    DDX_Control(pDX, IDC_STATIC_START_TIME, m_stcStartTime);
    DDX_Control(pDX, IDC_STATIC_PROGRESS_TIME, m_stcProgessTime);
}


BEGIN_MESSAGE_MAP(QBicSimpleProgress, CDialog)
    ON_WM_TIMER()
    //ON_BN_CLICKED(IDCANCEL, &KSimpleProgress::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_STOP, &QBicSimpleProgress::OnBnClickedStop)
END_MESSAGE_MAP()


void QBicSimpleProgress::SetThreadCallBackFunction(unsigned (__stdcall *func)(void* pParam))
{
    CallBackFunction = func;
}


void QBicSimpleProgress::SetThreadParam(void* pParam)
{
    m_pParam = pParam;
}


BOOL QBicSimpleProgress::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL QBicSimpleProgress::OnInitDialog()
{
    CDialog::OnInitDialog();

    if(m_strCaption.IsEmpty() == TRUE)
    {
		if (0 == m_nLang)
		{
			m_strCaption = _T("실행 중");
		}
		else if (1 == m_nLang)
		{
			m_strCaption = _T("Progress");
		}
    }

    SetWindowText(m_strCaption);
	m_stcStatus.SetWindowText(m_strStatus);

    m_stcAnimate.Load(_T(".\\images\\Ix01\\donkey.gif"));
    m_stcAnimate.Draw();

	if (0 == m_nLang) {
		
	}
	else {
		SetDlgItemText(IDC_STATIC_START_CAPTION, _T("Start time :"));
		SetDlgItemText(IDC_STATIC_PROGRESS_CAPTION, _T("Total processing time :"));
	}

	CButton* pStopButton = (CButton*)GetDlgItem(IDC_STOP);
	if (m_bEnabledStop)
	{
        GetDlgItem(IDC_STATIC_HOR)->ShowWindow(SW_SHOW);
		pStopButton->EnableWindow(TRUE);
        pStopButton->ShowWindow(SW_SHOW);
	}
	else
	{
        //GetDlgItem(IDC_STATIC_HOR)->ShowWindow(SW_HIDE);
		pStopButton->EnableWindow(FALSE);
        pStopButton->ShowWindow(SW_HIDE);
	}

    if(NULL != CallBackFunction)
    {
        //InitializeCriticalSection(&m_cs);
        m_bStop = false;
        m_timeStart = CTime::GetCurrentTime();
        SetDlgItemText(IDC_STATIC_START_TIME, m_timeStart.Format(_T("%H:%M:%S")));
        
        unsigned int dwThreadID;
        m_hThread = (HANDLE)_beginthreadex(NULL, 0, CallBackFunction, (void*)m_pSimpleProgressParameter, 0, &dwThreadID);

        SetTimer(1001, 500, NULL);
    }

	try
	{
		std::remove("stop.bin");
	}
	catch(...)
	{
	}
	
    return TRUE;  
}


void QBicSimpleProgress::OnTimer(UINT_PTR nIDEvent)
{
    EnterCriticalSection(&m_cs);
    m_stcStatus.SetWindowText(m_strStatus);   
    CTime     timeEnd   = CTime::GetCurrentTime();
    CTimeSpan timeSpan = timeEnd - m_timeStart;
    SetDlgItemText(IDC_STATIC_PROGRESS_TIME, timeSpan.Format(_T("%H:%M:%S")));
    LeaveCriticalSection(&m_cs);
    // Thread 종료 확인.
    DWORD dwResult = 0;
    if (m_hThread)
    {
        GetExitCodeThread(m_hThread, &dwResult);
        if ( dwResult != STILL_ACTIVE)
        {   
			// 정상적인 종료 영역	
            m_bStop       = true;
        }
    }
    else
    {
		m_bUserKilled = true;
        m_bStop       = true;
    }    
        
    if(true == m_bStop)
    {
        KillTimer(1001);
        CloseHandle(m_hThread);
        m_hThread = NULL;

		if (m_bUserKilled)
		{
			OnCancel();
		}
		else
		{
			OnOK();
		}
    }	

    CDialog::OnTimer(nIDEvent);
}

void QBicSimpleProgress::SetStatus(LPCTSTR strMsg)
{
    EnterCriticalSection(&m_cs);
    m_strStatus = strMsg;    
    LeaveCriticalSection(&m_cs);
}

bool QBicSimpleProgress::IsUserStopped(void)
{
    return m_bUserKilled;
}

void QBicSimpleProgress::Stop(void)
{
    EnterCriticalSection(&m_cs);

	try
	{
		std::remove("stop.bin");
		std::ofstream ofs(_T("stop.bin"), std::ios::binary);
		ofs.close();

		m_bUserKilled    = true;
		CButton* pButton = (CButton*)GetDlgItem(IDC_STOP);

		pButton->SetWindowText(_T("Stopping..."));
		pButton->EnableWindow(FALSE);
	}
	catch (...)
	{
	}

    LeaveCriticalSection(&m_cs);
}

void QBicSimpleProgress::OnBnClickedStop()
{	
	Stop();
}

INT_PTR QBicSimpleProgress::DoModal()
{
	// 2019.06.11 다이얼로그 폰트 설정
	int nResult;
	if (m_nLang != 1)
	{
		CDialogTemplate dlt;
		// load dialog template
		if (!dlt.Load(MAKEINTRESOURCE(QBicSimpleProgress::IDD)))
		{
			return -1;
		}

		// set your own font
		if (m_nLang == 0)
		{
			dlt.SetFont(_T("나눔고딕"), 9);
		}

		// get pointer to the modified dialog template
		LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);

		// let MFC know that you are using your own template
		m_lpszTemplateName = NULL;
		InitModalIndirect(pdata);

		// display dialog box
		nResult = CDialog::DoModal();

		// unlock memory object
		GlobalUnlock(dlt.m_hTemplate);
	}
	else
	{
		nResult = CDialog::DoModal();
	}
	return nResult;
}


QBicSimpleProgressPtr QBicSimpleProgressThread::m_spProgress;


INT_PTR QBicSimpleProgressThread::ExecuteThread( ThreadCallBackFunction a_fnPtr, void* a_pParameter, bool bEnableStop /*= false*/, 
	int nLang /*=1*/)
{
    QBicSimpleProgressPtr spProgress(new QBicSimpleProgress(a_fnPtr, a_pParameter, NULL, nLang));   
    m_spProgress = spProgress;

    m_spProgress->SetEnableStop(bEnableStop);
    INT_PTR nResult = spProgress->DoModal();
    //m_spProgress.reset();
    return nResult;
}

QBicSimpleProgressPtr QBicSimpleProgressThread::SimpleProgress()
{
    return m_spProgress;
}

bool QBicSimpleProgressThread::IsUserStopped()
{
    if (m_spProgress)
    {
        return m_spProgress->IsUserStopped();
    }

    return true;
}



