// InterProgressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "InterProgressDlg.h"
#include "afxdialogex.h"

#define TIMER_ID		1001

// KInterProgressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterProgressDlg, CDialogEx)

KInterProgressDlg::KInterProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KInterProgressDlg::IDD, pParent)
{
	ThreadFunction = NULL;
	m_pParam = NULL;
	::InitializeCriticalSection(&m_cs);
}

KInterProgressDlg::~KInterProgressDlg()
{
	::DeleteCriticalSection(&m_cs);
}

void KInterProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCANIMATE, m_stcAnimate);
	DDX_Control(pDX, IDC_LIST2, m_lstStatus);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_STCSTATUS, m_stcStatus);
}


BEGIN_MESSAGE_MAP(KInterProgressDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// KInterProgressDlg 메시지 처리기입니다.

void KInterProgressDlg::SetThreadFunction(void (*func)(void* pParam))
{
	ThreadFunction = func;
}


void KInterProgressDlg::SetThreadParam(void* pParam)
{
	m_pParam = pParam;
}


void KInterProgressDlg::SetStatus(LPCTSTR strMsg)
{
	::EnterCriticalSection(&m_cs);
	m_StatusQueue.push(strMsg);
	::LeaveCriticalSection(&m_cs);
}


bool KInterProgressDlg::IsCancled(void)
{
	return m_bCancel;
}


void KInterProgressDlg::Stop(void)
{
	m_bCancel = true;
}



// KAProgressDlg 메시지 처리기입니다.
BOOL KInterProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_stcAnimate.Load(_T(".\\images\\Ix01\\donkey.gif"));
	m_stcAnimate.Load(MAKEINTRESOURCE(IDR_PROGRESSIMAGE), _T("GIF"));
	m_stcAnimate.Draw();

	if(NULL != ThreadFunction)
	{
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		m_bCancel = false;
		_beginthread(ThreadFunction, 0, m_pParam);
		SetTimer(TIMER_ID, 200, NULL);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterProgressDlg::OnTimer(UINT_PTR nIDEvent)
{
	EnterCriticalSection(&m_cs);
	while(m_StatusQueue.empty() == false)
	{
		CString strStatus = m_StatusQueue.front();
		m_StatusQueue.pop();
		m_stcStatus.SetWindowText(strStatus);
		//m_lstStatus.AddString(strStatus);
		int nIndex = m_lstStatus.InsertString(-1, strStatus);
		m_lstStatus.SetCurSel(nIndex);

	}
	LeaveCriticalSection(&m_cs);

	if(true == m_bCancel)
	{
		m_stcStatus.SetWindowText(_T("InterModal 모형을 종료했습니다."));
		int nIndex = m_lstStatus.InsertString(-1, _T("종료"));
		m_lstStatus.SetCurSel(nIndex);
		//m_stcAnimate.ShowWindow(SW_HIDE);
		m_stcAnimate.Stop();
		KillTimer(TIMER_ID);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	}
	CDialog::OnTimer(nIDEvent);
}
