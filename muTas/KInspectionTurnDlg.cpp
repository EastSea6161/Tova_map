// KInspectionTurnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KInspectionTurnDlg.h"
#include "afxdialogex.h"
//^^ #include "IOTables.h"

// KInspectionTurnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInspectionTurnDlg, KResizeDialogEx)

KInspectionTurnDlg::KInspectionTurnDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInspectionTurnDlg::IDD, pParent)
    , m_chkDataInspection(TRUE)
    , m_chkCodeInspection(TRUE)
{

}

KInspectionTurnDlg::~KInspectionTurnDlg()
{
}

void KInspectionTurnDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
    DDX_Check(pDX, IDC_CHECK1, m_chkDataInspection);
    DDX_Check(pDX, IDC_CHECK2, m_chkCodeInspection);
}


BEGIN_MESSAGE_MAP(KInspectionTurnDlg, KResizeDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KInspectionTurnDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KInspectionTurnDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInspectionTurnDlg 메시지 처리기입니다.
void KInspectionTurnDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

    if (!pItemNotify->pRow || !pItemNotify->pColumn)
        return;

    int nIndex = pItemNotify->pColumn->GetItemIndex();
    /*if( _3_COLUMN_COLOR == nIndex )
    {
        CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor()) ;
        if ( IDOK == dlg.DoModal() )
        {
            pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());            
        }
    }*/
}

void KInspectionTurnDlg::InitReportHeader()
{
    CXTPReportColumn* pColumn = NULL;

    //m_wndReportCtrl.FocusSubItems(FALSE);
    m_wndReportCtrl.SelectionEnable(FALSE);
    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("No"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("ID"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Column"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Type"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("Description"), 40));
    pColumn->SetHeaderAlignment(DT_CENTER);

    CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
    pHeader->AllowColumnRemove(FALSE);
    pHeader->AllowColumnReorder(FALSE);
    pHeader->AllowColumnSort(FALSE);
}

void KInspectionTurnDlg::InitReportData(std::vector<InspectionResultOneKey>& results)
{
    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;

    std::vector<InspectionResultOneKey>::iterator iter = results.begin();
    std::vector<InspectionResultOneKey>::iterator end  = results.end();

    int i = 0;

    while (iter != end)
    {
        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(++i));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->ID));    
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->ColumnName));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->ErrorType));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->Desc));
        ++iter;
    }  
    m_wndReportCtrl.Populate();
}

BOOL KInspectionTurnDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    KReportCtrlSetting::Default(m_wndReportCtrl);
    KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

    InitReportHeader();
    m_wndReportCtrl.Populate();
        
    SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInspectionTurnDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);
    m_wndReportCtrl.GetRecords()->RemoveAll();
        
    KIOTables* pTables = m_pTarget->Tables();
    KIOTable* pTable = pTables->FindTable(_T("turn"));
    if(NULL == pTable)
    {
        AfxMessageBox(_T("회전제약 정보를 찾을 수 없습니다."));
        return;
    }

    CWaitCursor wc;

    std::vector<InspectionResultOneKey> results;
    if (m_chkDataInspection)
    {
        KDBaseInspection::CheckTurnNullData(pTable, results);
        KDBaseInspection::CheckTurnLogic(pTable, results);
    }

    if (m_chkCodeInspection)
    {
        KDBaseInspection::CheckTurnCodeData(pTable, results);
    }

    if (results.size() > 0)
    {
        InitReportData(results);
    }
    else
    {
        m_wndReportCtrl.Populate();
        AfxMessageBox(_T("오류 정보가 존재하지 않습니다."));
    }
}

void KInspectionTurnDlg::OnBnClickedCancel()
{
    KResizeDialogEx::OnCancel();
}