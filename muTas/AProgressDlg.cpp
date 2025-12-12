// AProgressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "AProgressDlg.h"
#include "afxdialogex.h"


#define TIMER_ID		1001

// KAProgressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAProgressDlg, CXTResizeDialog)

KAProgressDlg::KAProgressDlg(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KAProgressDlg::IDD, pParent)
{
	ThreadFunction = NULL;
	m_pParam = NULL;
	::InitializeCriticalSection(&m_cs);
}

KAProgressDlg::~KAProgressDlg()
{
	::DeleteCriticalSection(&m_cs);
}

void KAProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STCANIMATE, m_stcAnimate);
	DDX_Control(pDX, IDC_LIST2, m_lstStatus);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	DDX_Control(pDX, IDC_STCSTATUS, m_stcStatus);
}


BEGIN_MESSAGE_MAP(KAProgressDlg, CXTResizeDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &KAProgressDlg::OnBnClickedCancel)
END_MESSAGE_MAP()



void KAProgressDlg::SetThreadFunction(void (*func)(void* pParam))
{
	ThreadFunction = func;
}


void KAProgressDlg::SetThreadParam(void* pParam)
{
	m_pParam = pParam;
}


void KAProgressDlg::SetStatus(LPCTSTR strMsg)
{
	::EnterCriticalSection(&m_cs);
	m_StatusQueue.push(strMsg);
	::LeaveCriticalSection(&m_cs);
}


bool KAProgressDlg::IsCancled(void)
{
	return m_bCancel;
}


void KAProgressDlg::Stop(void)
{
	m_bCancel = true;
}


BOOL KAProgressDlg::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}


// KAProgressDlg 메시지 처리기입니다.
BOOL KAProgressDlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	//SetResize(IDC_LIST2, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	//IDC_STCANIMATE
	SetResize(IDC_LIST2, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STCANIMATE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STCSTATUS, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	//IDCANCEL
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

	//다이어로그 로드 시 창 크기 확대.
	CRect rect;
	GetWindowRect(rect);
	MoveWindow(rect.left, rect.top, rect.Width()+300, rect.Height()+200);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAProgressDlg::OnTimer(UINT_PTR nIDEvent)
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
		m_stcStatus.SetWindowText(_T("Assignment 모형을 종료했습니다."));
		int nIndex = m_lstStatus.InsertString(-1, _T("종료"));
		m_lstStatus.SetCurSel(nIndex);
		//m_stcAnimate.ShowWindow(SW_HIDE);
		m_stcAnimate.Stop();
		KillTimer(TIMER_ID);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
	}
	CXTResizeDialog::OnTimer(nIDEvent);
}


void KAProgressDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bCancel)
	{
		CXTResizeDialog::OnCancel();
	}
}

