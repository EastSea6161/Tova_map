// ListSimpleProgress.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "QBicListSimpleProgress.h"
#include "QBicProgressParameter.h"

#include <sys/stat.h>
#include <fstream>
#include <iostream>

const int KLAUNCHER_USER_FORCE_STOP = 99909;

IMPLEMENT_DYNAMIC(QBicListSimpleProgress, CXTResizeDialog)

QBicListSimpleProgress::QBicListSimpleProgress(ThreadCallBackFunction a_fnPtr, void* pParameter, CWnd* pParent /*=NULL*/, 
	int nLang /*=1*/)
	: CXTResizeDialog(QBicListSimpleProgress::IDD, pParent)
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
	else if (1 == m_nLang)
	{
		m_strStatus = _T("Please wait... data processing...");
	}

	CallBackFunction = a_fnPtr;
	m_pParam         = pParameter;

	m_pSimpleProgressParameter = new QBicListSimpleProgressParameter(this, pParameter);
	InitializeCriticalSection(&m_cs);
}

QBicListSimpleProgress::~QBicListSimpleProgress()
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

void QBicListSimpleProgress::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCANIMATE, m_stcAnimate);
	DDX_Control(pDX, IDC_STATIC_START_TIME, m_stcStartTime);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_TIME, m_stcProgessTime);
	DDX_Control(pDX, IDC_QW_LIST, m_lstStatus);
}


BEGIN_MESSAGE_MAP(QBicListSimpleProgress, CXTResizeDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &QBicListSimpleProgress::OnBnClickedStop)
	ON_BN_CLICKED(IDCANCEL, &QBicListSimpleProgress::OnBnClickedCancel)
END_MESSAGE_MAP()


// KListSimpleProgress 메시지 처리기입니다.

void QBicListSimpleProgress::SetThreadCallBackFunction(unsigned (__stdcall *func)(void* pParam))
{
	CallBackFunction = func;
}


void QBicListSimpleProgress::SetThreadParam(void* pParam)
{
	m_pParam = pParam;
}


BOOL QBicListSimpleProgress::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}


BOOL QBicListSimpleProgress::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	SetResize(IDC_QW_LIST, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STCANIMATE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STOP, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);	
	//SetResize(IDC_STATIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	//IDC_STATIC_PROGRESS_CAPTION
	SetResize(IDC_STATIC_START_TIME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_PROGRESS_TIME, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_START_CAPTION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_PROGRESS_CAPTION, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	if(m_strCaption.IsEmpty() == TRUE)
	{
		if (0 == m_nLang)
		{
			m_strCaption = _T("실행 중");
		}
		else if (1 == m_nLang)
		{
			m_strCaption = _T("Processing");
		}
	}

	SetWindowText(m_strCaption);


	m_stcAnimate.Load(_T(".\\images\\Ix01\\donkey.gif"));
	m_stcAnimate.Draw();

	// 2020.02.14 - Han He : 영문 버전에서 사용
	if (0 == m_nLang) {

	}
	else {
		SetDlgItemText(IDC_STATIC_START_CAPTION, _T("Start time :"));
		SetDlgItemText(IDC_STATIC_PROGRESS_CAPTION, _T("Total processing time :"));
	}

	CButton* pStopButton = (CButton*)GetDlgItem(IDC_STOP);
	if (m_bEnabledStop)
	{
		pStopButton->EnableWindow(TRUE);
	}
	else
	{
		pStopButton->EnableWindow(FALSE);
	}

	CButton* pButton = (CButton*)GetDlgItem(IDCANCEL);
	pButton->EnableWindow(FALSE);

	if(NULL != CallBackFunction)
	{		
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

	CRect rect;
	GetWindowRect(rect);
	MoveWindow(rect.left, rect.top, 1200, 600);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void QBicListSimpleProgress::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&m_cs);
	 
	CTime     timeEnd   = CTime::GetCurrentTime();
	CTimeSpan timeSpan = timeEnd - m_timeStart;
	SetDlgItemText(IDC_STATIC_PROGRESS_TIME, timeSpan.Format(_T("%H:%M:%S")));

	while(m_StatusQueue.empty() == false)
	{
		CString strStatus = m_StatusQueue.front();
		m_StatusQueue.pop();
		int nIndex = m_lstStatus.InsertString(-1, strStatus);
		m_lstStatus.SetCurSel(nIndex);
	}

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

			m_stcAnimate.Stop();

			CButton* pButton = (CButton*)GetDlgItem(IDC_STOP);

			pButton->SetWindowText(_T("Stop"));
			pButton->EnableWindow(FALSE);

			pButton = (CButton*)GetDlgItem(IDCANCEL);
			pButton->EnableWindow(TRUE);
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
	}	

	CDialog::OnTimer(nIDEvent);
}

void QBicListSimpleProgress::SetStatus(LPCTSTR strMsg)
{
	EnterCriticalSection(&m_cs);
	m_StatusQueue.push(strMsg);
	LeaveCriticalSection(&m_cs);
}

bool QBicListSimpleProgress::IsUserStopped(void)
{
	return m_bUserKilled;
}

void QBicListSimpleProgress::Stop(void)
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

		//부모창에 SendMeessage
		TCHAR lpszString[BUFSIZ] = {0};

		COPYDATASTRUCT cds;
		cds.dwData = KLAUNCHER_USER_FORCE_STOP; /*결과정보*/ 
		cds.cbData = (DWORD) (sizeof(TCHAR) * (_tcslen(lpszString) + 1));
		cds.lpData = lpszString;

		CWnd* pWnd = this->GetParent();

		::SendMessage(pWnd->m_hWnd, WM_COPYDATA, 0/*(WPARAM)nResult*/, (LPARAM)(LPVOID)&cds);
	}
	catch (...)
	{
	}
	LeaveCriticalSection(&m_cs);
}

void QBicListSimpleProgress::OnBnClickedStop()
{	
	Stop();
}

void QBicListSimpleProgress::OnBnClickedCancel()
{
	if(true == m_bStop)
	{		
		if (m_bUserKilled)
		{
			OnCancel();
		}
		else
		{
			OnOK();
		}
	}	
}

QBicListSimpleProgressPtr QBicListSimpleProgressThread::m_spProgress;

INT_PTR QBicListSimpleProgressThread::ExecuteThread( ThreadCallBackFunction a_fnPtr, void* a_pParameter, bool bEnableStop /*= false*/,
	int nLang /*=1*/)
{
    QBicListSimpleProgressPtr spProgress(new QBicListSimpleProgress(a_fnPtr, a_pParameter, NULL, nLang));   
    m_spProgress = spProgress;

    m_spProgress->SetEnableStop(bEnableStop);
    INT_PTR nResult = spProgress->DoModal();
    //m_spProgress.reset();
    return nResult;
}

QBicListSimpleProgressPtr QBicListSimpleProgressThread::SimpleProgress()
{
    return m_spProgress;
}

bool QBicListSimpleProgressThread::IsUserStopped()
{
    if (m_spProgress)
    {
        return m_spProgress->IsUserStopped();
    }

    return true;
}
