// ProgressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "SimpleProgressOld.h"
#include "afxdialogex.h"


KSimpleProgressOld* g_pProgressDlg = NULL;

// ZProgressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSimpleProgressOld, CDialog)

KSimpleProgressOld::KSimpleProgressOld(CWnd* pParent /*=NULL*/)
	: CDialog(KSimpleProgressOld::IDD, pParent)
{
    m_bCancel = true;
    m_hThread = NULL;

    m_strStatus = _T("Please wait..processing data....");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_strStatus = _T("데이터 처리중입니다... 잠시만 기다려 주십시오...");
	}
    TxLogDebugVisitor();
}

KSimpleProgressOld::~KSimpleProgressOld()
{
    TxLogDebugVisitor();
}

void KSimpleProgressOld::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCSTATUS, m_stcStatus);
	DDX_Control(pDX, IDC_STCANIMATE, m_stcAnimate);
}


BEGIN_MESSAGE_MAP(KSimpleProgressOld, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &KSimpleProgressOld::OnBnClickedCancel)
END_MESSAGE_MAP()



void KSimpleProgressOld::Caption(LPCTSTR strCaption)
{
	m_strCaption = strCaption;
}

//void (*func_ptr)(int, int) = NULL
void KSimpleProgressOld::SetThreadCallBackFunction(void (*func)(void* pParam))
{
	ThreadCallBackFunction = func;
}


void KSimpleProgressOld::SetThreadParam(void* pParam)
{
	m_pParam = pParam;
}


void KSimpleProgressOld::SetStatus(LPCTSTR strMsg)
{
	EnterCriticalSection(&m_cs);
    m_strStatus = strMsg;    
	LeaveCriticalSection(&m_cs);
}


bool KSimpleProgressOld::IsCancled(void)
{
	return m_bCancel;
}

void KSimpleProgressOld::Stop(void)
{
	m_bCancel = true;
}

// ZProgressDlg 메시지 처리기입니다.


BOOL KSimpleProgressOld::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_strCaption.IsEmpty() == TRUE)
	{
		m_strCaption = _T("Progress");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strStatus = _T("실행중");
		}
	}

	SetWindowText(m_strCaption);

	m_stcAnimate.Load(_T(".\\images\\Ix01\\donkey.gif"));
	m_stcAnimate.Draw();
	
	if(NULL != ThreadCallBackFunction)
	{
		InitializeCriticalSection(&m_cs);
		m_bCancel = false;
		m_hThread = (HANDLE)_beginthread(ThreadCallBackFunction, 0, m_pParam);
		SetTimer(1001, 200, NULL);
		//GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
        GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSimpleProgressOld::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&m_cs);
    m_stcStatus.SetWindowText(m_strStatus);   
	LeaveCriticalSection(&m_cs);

    DWORD dwResult=0;
    
    if ( m_hThread != NULL && !GetExitCodeThread(m_hThread, &dwResult) )
    {   
        m_bCancel = true;
    }

	if(true == m_bCancel)
	{
		OnCancel();
	}
	CDialog::OnTimer(nIDEvent);
}

void KSimpleProgressOld::OnBnClickedCancel()
{
	m_bCancel = true;
	CDialog::OnCancel();
}

/*QBicSimpleProgressPtr KSimpleProgressFactory::m_spProgress;*/