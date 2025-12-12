// KTabMainSample.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TabMainSample.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"

// KTabMainSample 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTabMainSample, KResizeDialogEx)

KTabMainSample::KTabMainSample(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTabMainSample::IDD, pParent)
{
    m_nTabAbovePixcel       = 120;
    m_pCurrentShowClientDlg = NULL;
}

KTabMainSample::~KTabMainSample()
{
    m_pCurrentShowClientDlg = NULL;
}

void KTabMainSample::DoDataExchange(CDataExchange* pDX)
{
    KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KTabMainSample, KResizeDialogEx)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KTabMainSample::OnTcnSelchangeTab1)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON1, &KTabMainSample::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &KTabMainSample::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &KTabMainSample::OnBnClickedButton3)
END_MESSAGE_MAP()


// KTabMainSample 메시지 처리기입니다.

void KTabMainSample::OnSize(UINT nType, int cx, int cy)
{
    KResizeDialogEx::OnSize(nType, cx, cy);
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    if (m_pCurrentShowClientDlg != nullptr)
    {        
        CRect oTabRect;
        m_tabCtrl.GetClientRect(&oTabRect);	
        m_dlgTabSubSample.SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
        
        // 추가...
        //m_dlgTabSubSample2.SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
    }
}


BOOL KTabMainSample::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    m_tabCtrl.InsertItem(0, _T("Node"));
    m_tabCtrl.InsertItem(1, _T("Link"));
    m_tabCtrl.SetCurSel(0);

    CRect oTabRect;
    m_tabCtrl.GetClientRect(&oTabRect);	
    m_dlgTabSubSample.Create(IDD_TAB_SUB, &m_tabCtrl);
    m_dlgTabSubSample.SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

    SetResize(IDC_TAB1, SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    DisplayTabControl(0);

    KReportCtrlSetting::Default(m_wndReportCtrl, TRUE, FALSE, FALSE);
    InitReportHeader();
    InitReportData();

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTabMainSample::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    DisplayTabControl(m_tabCtrl.GetCurSel());
    *pResult = 0;
}

void KTabMainSample::DisplayTabControl( int a_nTabIndex )
{
    int nIndex = a_nTabIndex;

    if(m_pCurrentShowClientDlg != NULL)
    {        
        m_pCurrentShowClientDlg->ShowWindow(SW_HIDE);
        m_pCurrentShowClientDlg = NULL;
    }

    switch(nIndex)
    {
    case 0:
        m_dlgTabSubSample.ShowWindow(SW_SHOW);
        m_pCurrentShowClientDlg = &m_dlgTabSubSample;
        break;
    case 1:
        m_dlgTabSubSample.ShowWindow(SW_SHOW);
        m_pCurrentShowClientDlg = &m_dlgTabSubSample;
        break;
    case 2:
    default:
        m_dlgTabSubSample.ShowWindow(SW_SHOW);
        m_pCurrentShowClientDlg = &m_dlgTabSubSample;	
        break;
    }
}

void KTabMainSample::InitReportHeader()
{
    CXTPReportColumn* pColumn = NULL;
    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Path"), 200));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("승객"), 50));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("기본요금"), 50));
    pColumn->SetHeaderAlignment(DT_CENTER);
}

void KTabMainSample::InitReportData()
{
    CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
    CXTPReportRecord*  pRecord  = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

    CXTPReportRecordItem*pItem  = NULL;
    CXTPReportRecordItemEditOptions* pEditOptions = nullptr;

    CXTPReportRecordItemComboEx* pItemCombo;
    int nIndex(0);

    pItem   = pRecord->AddItem(new CXTPReportRecordItem);
    pItem->SetEditable(TRUE);

    pItem   = pRecord->AddItem(new CXTPReportRecordItem);
    pItem->SetCaption(_T("nnn"));
       
    pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());    
    nIndex     = pItemCombo->AddString(_T("1"));
    pItemCombo->SetItemData(nIndex, 1000);
    nIndex     = pItemCombo->AddString(_T("2"), 11);
    nIndex     = pItemCombo->AddString(_T("3"), 12);

    pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

    pItem   = pRecord->AddItem(new CXTPReportRecordItem);
    pItem->SetEditable(TRUE);

    pItem   = pRecord->AddItem(new CXTPReportRecordItem);
    pItem->SetCaption(_T("888"));

    pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());    
    nIndex     = pItemCombo->AddString(_T("771"), 90);
    nIndex     = pItemCombo->AddString(_T("772"), 91);
    nIndex     = pItemCombo->AddString(_T("773"), 92);
    pItemCombo->SetCurSel(2);

    m_wndReportCtrl.Populate();
}


void KTabMainSample::OnBnClickedButton1()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
    CXTPReportRecord*  pRecord  = pRecords->GetAt(0);

    CXTPReportRecordItemComboEx* pItem = (CXTPReportRecordItemComboEx*)pRecord->GetItem(2);
    pItem->SetCurSel(0);
    m_wndReportCtrl.RedrawControl();
}


void KTabMainSample::OnBnClickedButton2()
{
    CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
    CXTPReportRecord*  pRecord  = pRecords->GetAt(1);

    CXTPReportRecordItemComboEx* pItem = (CXTPReportRecordItemComboEx*)pRecord->GetItem(2);
    int nIndex = pItem->GetCurSel();
    if (nIndex < 0)
        return;

    int nValue = pItem->GetItemData(nIndex);

    CString strMsg;
    strMsg.Format(_T("%d"), nValue);

    AfxMessageBox(strMsg);
}


void KTabMainSample::OnBnClickedButton3()
{
    CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
    CXTPReportRecord*  pRecord  = pRecords->GetAt(1);

    CXTPReportRecordItemComboEx* pItem = (CXTPReportRecordItemComboEx*)pRecord->GetItem(2);
    pItem->ResetContent();
    int nIndex;
    nIndex     = pItem->AddString(_T("771"), 771);
    nIndex     = pItem->AddString(_T("772"), 772);
    nIndex     = pItem->AddString(_T("773"), 773);
    pItem->SetCurSel(0);
    m_wndReportCtrl.RedrawControl();
}
