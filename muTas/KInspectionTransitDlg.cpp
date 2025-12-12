// KInspectionTransitDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KmzSystem.h"
#include "KInspectionTransitDlg.h"
#include "afxdialogex.h"
//^^ #include "IOTables.h"

// KInspectionTransitDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInspectionTransitDlg, KResizeDialogEx)

KInspectionTransitDlg::KInspectionTransitDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInspectionTransitDlg::IDD, pParent)
    , m_chkDataInspection(TRUE)
    , m_chkCodeInspection(FALSE)
{

}

KInspectionTransitDlg::~KInspectionTransitDlg()
{
}

void KInspectionTransitDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
    DDX_Check(pDX, IDC_CHECK1, m_chkDataInspection);
    DDX_Check(pDX, IDC_CHECK2, m_chkCodeInspection);
}


BEGIN_MESSAGE_MAP(KInspectionTransitDlg, KResizeDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KInspectionTransitDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KInspectionTransitDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInspectionTransitDlg 메시지 처리기입니다.
void KInspectionTransitDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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

void KInspectionTransitDlg::InitReportHeader()
{
    CXTPReportColumn* pColumn = NULL;

    //m_wndReportCtrl.FocusSubItems(FALSE);
    m_wndReportCtrl.SelectionEnable(FALSE);
    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("No"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Route"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Seq"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Link"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("Description"), 40));
    pColumn->SetHeaderAlignment(DT_CENTER);

    CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
    pHeader->AllowColumnRemove(FALSE);
    pHeader->AllowColumnReorder(FALSE);
    pHeader->AllowColumnSort(FALSE);
}

void KInspectionTransitDlg::InitReportData(std::vector<Transit_InspectionResult>& results)
{
    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;

    std::vector<Transit_InspectionResult>::iterator iter = results.begin();
    std::vector<Transit_InspectionResult>::iterator end  = results.end();

    int i = 0;

    while (iter != end)
    {
        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(++i));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->ID));     
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->Seq));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->LinkID));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(iter->Desc));
        ++iter;
    }  
    m_wndReportCtrl.Populate();
}

BOOL KInspectionTransitDlg::OnInitDialog()
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

void KInspectionTransitDlg::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);
    m_wndReportCtrl.GetRecords()->RemoveAll();        
    
    CWaitCursor wc;

    std::vector<Transit_InspectionResult> results;
    if (m_chkDataInspection)
    {
        CheckTransit(results);
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

void KInspectionTransitDlg::OnBnClickedCancel()
{
    KResizeDialogEx::OnCancel();	
}


void KInspectionTransitDlg::CheckTransit(std::vector<Transit_InspectionResult>& results)
{
    KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	std::vector<TFixedTransitDB> transitRecords;
	KBulkDBase::BulkTransitSelect(spDBaseConnection, transitRecords);

	std::map<Integer, TFTNode> linkRecords;
	KBulkDBase::BulkLinkFTNodeSelect(spDBaseConnection, linkRecords);

    size_t nTransitCount = transitRecords.size();

    for ( size_t i=0; i<nTransitCount; i++ )
    {
        CheckTransitLink(linkRecords, transitRecords[i], results);
    }
}


void KInspectionTransitDlg::CheckTransitLink(std::map<Integer, TFTNode>& linkRecords, TFixedTransitDB& oneTransit, std::vector<Transit_InspectionResult>& results)
{
    std::vector<TFixedTransitLink> transitLinkRecords;
    KBulkDBase::BulkTransitLinksSelect(m_pTarget->GetDBaseConnection(), transitLinkRecords, oneTransit.transit_id);

    std::map<Integer, TFTNode>::iterator iter;
    std::map<Integer, TFTNode>::iterator end  = linkRecords.end();

    // 오류 유형 : 시퀀스, 존재하지 않는 링크...

    size_t nTransitLinksCount = transitLinkRecords.size();
    Integer nextFromNodeID = -99999999;
    int     chkSeq;
    Transit_InspectionResult result;

    for ( size_t i=0; i<nTransitLinksCount; i++ )
    {
        TFixedTransitLink& transitLink = transitLinkRecords[i];
        Integer link_id = transitLink.link_id;

        iter = linkRecords.find(link_id);

        if (iter == end)
        {
            // 존재하지 않는 링크(노선ID, Seq, 링크ID)  
            result.ID.Format(_T("%s [%I64d]"), oneTransit.name, oneTransit.transit_id);
            result.Seq.Format(_T("%d"), transitLink.seq);
            result.LinkID.Format(_T("%I64d"), transitLink.link_id);
            result.Desc = _T("존재하지 않는 링크입니다.");

            results.push_back(result);
        }
        else
        {
            TFTNode& tFTNode = iter->second;
            Integer fnode_id = tFTNode.fnode_id;
            Integer tnode_id = tFTNode.tnode_id;

            if (nextFromNodeID > 0)
            {                
                if (nextFromNodeID != fnode_id)
                {
                    // 연결정보 오류
                    // 이전 링크와의 연결성 오류 (노선ID, Seq, 링크ID)  
                    result.ID.Format(_T("%s [%I64d]"), oneTransit.name, oneTransit.transit_id);
                    result.Seq.Format(_T("%d"), transitLink.seq);
                    result.LinkID.Format(_T("%I64d"), transitLink.link_id);
                    result.Desc = _T("이전 링크와의 연결성 오류");
                    results.push_back(result);
                }

                if (chkSeq + 1 != transitLink.seq)
                {
                    // 시퀀스 오류
                    result.ID.Format(_T("%s [%I64d]"), oneTransit.name, oneTransit.transit_id);
                    result.Seq.Format(_T("%d"), transitLink.seq);
                    result.LinkID.Format(_T("%I64d"), transitLink.link_id);
                    result.Desc = _T("시퀀스 오류");
                    results.push_back(result);
                }
            }

            nextFromNodeID = tnode_id;
            chkSeq = transitLink.seq;
        }
    }
}