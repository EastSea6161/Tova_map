// DevPlanDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanModelDlg.h"
#include "afxdialogex.h"

#include "KUIDefDevPlan.h"
#include "DevPlanProgressDlg.h"

//^^ #include "IOTables.h"
#include "Target.h"
#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOTableView.h"
#include "IOView.h"

#include "IOTableController.h"

using namespace UIDevPlan;
// KDevPlanModelDlg 대화 상자입니다.

KDevPlanProgressDlg* KDevPlanModelDlg::m_pProgressDlg = NULL;

IMPLEMENT_DYNAMIC(KDevPlanModelDlg, KDialogEx)

KDevPlanModelDlg::KDevPlanModelDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanModelDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_devPlanDlg(m_pTarget)
	, m_devPlanGenerationDlg(m_pTarget)
	, m_devPlanDistributionDlg(m_pTarget)
	, m_nSelPlanID(0)
{
}

KDevPlanModelDlg::~KDevPlanModelDlg()
{
}

void KDevPlanModelDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ARROW1, m_uiArrow1);
	DDX_Control(pDX, IDC_STATIC_ARROW2, m_uiArrow2);
	DDX_Control(pDX, IDC_STATIC_ARROW3, m_uiArrow3);
	DDX_Control(pDX, IDC_STATIC_ARROW4, m_uiArrow4);
}


BEGIN_MESSAGE_MAP(KDevPlanModelDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KDevPlanModelDlg::OnBnClickedDevPlan)
	ON_BN_CLICKED(IDC_BUTTON2, &KDevPlanModelDlg::OnBnClickedGeneration)
	ON_BN_CLICKED(IDC_BUTTON3, &KDevPlanModelDlg::OnBnClickedDistribution)
	ON_BN_CLICKED(IDC_BUTTON4, &KDevPlanModelDlg::OnBnClickedModalSplit)
	ON_BN_CLICKED(IDC_BUTTON5, &KDevPlanModelDlg::OnBnClickedOutput)
	ON_BN_CLICKED(IDOK, &KDevPlanModelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON6, &KDevPlanModelDlg::OnBnClickedViewTable)
END_MESSAGE_MAP()


// KDevPlanModelDlg 메시지 처리기입니다.


void KDevPlanModelDlg::InitDialogUI( void )
{
	m_uiArrow1.ArrowLineColor(RGB(0, 0, 0));
	m_uiArrow1.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
	
	m_uiArrow2.ArrowLineColor(RGB(0, 0, 0));
	m_uiArrow2.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

	m_uiArrow3.ArrowLineColor(RGB(0, 0, 0));
	m_uiArrow3.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);

	m_uiArrow4.ArrowLineColor(RGB(0, 0, 0));
	m_uiArrow4.SetPosition(ZEMArrowPositionLC, ZEMArrowPositionRC);
}


BOOL KDevPlanModelDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	InitDialogUI();


	CWnd* pWnd = GetDlgItem(IDC_FRAME);
	CRect rect;
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_devPlanDlg.Create(IDD_5601_DevPlanDlg, this);
	m_devPlanDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_devPlanDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	
	m_devPlanGenerationDlg.Create(IDD_5602_DevPlanGDlg, this);
	m_devPlanGenerationDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_devPlanGenerationDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_devPlanDistributionDlg.Create(IDD_5603_DevPlanDDlg, this);
	m_devPlanDistributionDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_devPlanDistributionDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_devplanModalSplitDlg.SetTarget(m_pTarget);
	m_devplanModalSplitDlg.Create(IDD_5604_DevPlanMDlg, this);
	m_devplanModalSplitDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_devplanModalSplitDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	m_devplanOutputDlg.Create(IDD_5605_DevPlanODlg, this);
	m_devplanOutputDlg.ShowWindow(WS_VISIBLE | WS_CHILD);
	m_devplanOutputDlg.SetWindowPos(
		NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_HIDEWINDOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDevPlanModelDlg::OnBnClickedDevPlan()
{
	HideFrameDialog();
	m_devPlanDlg.ShowWindow(SW_SHOW);
}


void KDevPlanModelDlg::OnBnClickedGeneration()
{
	int nPlanID;

	m_setPlanID = m_devPlanDlg.PlanList();

	std::set<int>::iterator iter, end = m_setPlanID.end();

	for (iter = m_setPlanID.begin(); iter != end; ++iter)
	{
		nPlanID = *iter;
	}
		
	HideFrameDialog();
	
	m_devPlanGenerationDlg.ShowWindow(SW_SHOW);

	m_devPlanGenerationDlg.OnInitDialog();
}


void KDevPlanModelDlg::OnBnClickedDistribution()
{
	HideFrameDialog();
	m_devPlanDistributionDlg.ShowWindow(SW_SHOW);
}


void KDevPlanModelDlg::OnBnClickedModalSplit()
{
	HideFrameDialog();
	m_devplanModalSplitDlg.ShowWindow(SW_SHOW);
}


void KDevPlanModelDlg::OnBnClickedOutput()
{
	HideFrameDialog();
	m_devplanOutputDlg.ShowWindow(SW_SHOW);
}

void KDevPlanModelDlg::HideFrameDialog( void )
{
	m_devPlanDlg.ShowWindow(SW_HIDE);
	m_devPlanGenerationDlg.ShowWindow(SW_HIDE);
	m_devPlanDistributionDlg.ShowWindow(SW_HIDE);
	m_devplanModalSplitDlg.ShowWindow(SW_HIDE);
	m_devplanOutputDlg.ShowWindow(SW_HIDE);
}

void KDevPlanModelDlg::OnBnClickedViewTable()
{
	TxLogDebugStartMsg();
	KIOTables* pIOTables = m_pTarget->Tables();
	{
		KIOTable* pZoneTable = pIOTables->FindTable(TABLE_ZONE);
		ASSERT(NULL != pZoneTable);

		CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
		pChildFrame->ShowIOView( true );
		KIOView* pIOView = pChildFrame->GetIOView();

		if(NULL != pIOView)
		{
			KIOTableView* pTableView = pIOView->FindTableView(pZoneTable->Name());
			if(NULL == pTableView)
			{
				pTableView = pIOView->CreateNewTableView(pZoneTable);
				KIOTableController* pController = new KIOTableController(m_pTarget);
				pController->Attach(pZoneTable, pTableView);
			}
			pIOView->SetActiveTableView(pZoneTable->Name());
		}
	}
	{
		KIOTable* pTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
		ASSERT(NULL != pTable);

		CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
		pChildFrame->ShowIOView( true );
		KIOView* pIOView = pChildFrame->GetIOView();

		if(NULL != pIOView)
		{
			KIOTableView* pTableView = pIOView->FindTableView(pTable->Name());
			if(NULL == pTableView)
			{
				pTableView = pIOView->CreateNewTableView(pTable);
				KIOTableController* pController = new KIOTableController(m_pTarget);
				pController->Attach(pTable, pTableView);
			}
			pIOView->SetActiveTableView(pTable->Name());
		}
	}
	{
		KIOTable* pTable = pIOTables->FindTable(TABLE_MODE_OD);
		ASSERT(NULL != pTable);

		CChildFrame* pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();
		pChildFrame->ShowIOView( true );
		KIOView* pIOView = pChildFrame->GetIOView();

		if(NULL != pIOView)
		{
			KIOTableView* pTableView = pIOView->FindTableView(pTable->Name());
			if(NULL == pTableView)
			{
				pTableView = pIOView->CreateNewTableView(pTable);
				KIOTableController* pController = new KIOTableController(m_pTarget);
				pController->Attach(pTable, pTableView);
			}
			pIOView->SetActiveTableView(pTable->Name());
		}
	}
	TxLogDebugEndMsg();
}

void KDevPlanModelDlg::ReceiveStatusMessageFromDll( void* pParam1, int nStatus, char* strMsg )
{
	CString strDllMsg(strMsg);

	if(NULL != m_pProgressDlg)
	{
		m_pProgressDlg->SetStatus(strDllMsg);
	}
}

void KDevPlanModelDlg::OnBnClickedOk()
{
	m_pProgressDlg = new KDevPlanProgressDlg;
	m_pProgressDlg->SetThreadParam(this);
	m_pProgressDlg->SetThreadFunction(DevPlanModelThreadProc);
	m_pProgressDlg->DoModal();

	KDevPlanProgressDlg* pTemp = m_pProgressDlg;
	delete pTemp;

	m_pProgressDlg       = NULL;
}

void KDevPlanModelDlg::DevPlanModelThreadProc( void* pParam )
{
	if(NULL != pParam)
	{
		KDevPlanModelDlg* pDlg = (KDevPlanModelDlg*)pParam;
		pDlg->RunModel();
	}

	if(NULL != m_pProgressDlg)
	{
		TxLogDebugStartMsg();

		m_pProgressDlg->Stop();

		TxLogDebugEndMsg();
	}
}

void KDevPlanModelDlg::RunModel( void )
{
	AddStatusMessage(_T("Create Argument..."));
	Sleep(2000);
	AddStatusMessage(_T("Create Argument End."));
	
	AddStatusMessage(_T("> Development Plan Start ..."));

	if(NULL != m_hDevelopPlan)
		::FreeLibrary(m_hDevelopPlan);

	m_hDevelopPlan = ::LoadLibrary(_T("DevelopPlan"));

	if (NULL == m_hDevelopPlan)
	{
		AfxMessageBox(_T("DLL이 없습니다."));
		return;
	}

	DevelopPlanProcPtr = (DEVELOPPLAN)GetProcAddress(m_hDevelopPlan, "DEVELOPPLAN");

	if (NULL == DevelopPlanProcPtr)
	{
		AfxMessageBox(_T("DEVELOPPLAN 함수가 없습니다."));
		::FreeLibrary(m_hDevelopPlan);
		return;
	}

	int nResult = DevelopPlanProcPtr((void*)this, (void*)ReceiveStatusMessageFromDll);

	AddStatusMessage(_T("> Development Plan End ..."));

	AddStatusMessage(_T("> Save Results."));

	Sleep(5000);

	AddStatusMessage(_T("> Complete"));
}

void KDevPlanModelDlg::AddStatusMessage( CString strMsg )
{
	if(NULL != m_pProgressDlg)
	{
		m_pProgressDlg->SetStatus(strMsg);
	}
}
