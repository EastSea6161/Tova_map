// KInfluenceStatisticsDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KInfluenceStatisticsDlg.h"
#include "afxdialogex.h"
#include "KDBaseStatistics.h"
//^^ #include "IOTables.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"

// KInfluenceStatisticsDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInfluenceStatisticsDlg, CXTPResizeDialog)

KInfluenceStatisticsDlg::KInfluenceStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KInfluenceStatisticsDlg::IDD, pParent)
{

}

KInfluenceStatisticsDlg::~KInfluenceStatisticsDlg()
{
}

void KInfluenceStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KInfluenceStatisticsDlg, CXTPResizeDialog)
END_MESSAGE_MAP()


// KInfluenceStatisticsDlg 메시지 처리기입니다.
void KInfluenceStatisticsDlg::InitReportHeader()
{
    CXTPReportColumn* pColumn = NULL;

    //m_wndReportCtrl.FocusSubItems(FALSE);
    m_wndReportCtrl.SelectionEnable(FALSE);
    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("No"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Type"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Count"), 10));
    pColumn->SetHeaderAlignment(DT_CENTER);

    pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Length(Sum)"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

    CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
    pHeader->AllowColumnRemove(FALSE);
    pHeader->AllowColumnReorder(FALSE);
    pHeader->AllowColumnSort(FALSE);
}

void KInfluenceStatisticsDlg::InitReportData()
{
    KIOTables*  pIOTables = m_pTarget->Tables();
    KIOTable*         pTable        = pIOTables->FindTable(TABLE_LINK);
    ASSERT(NULL != pTable);

    const 
    KIOColumns* pColumns = pTable->Columns();
    KIOColumn*           pColumn  = pColumns->GetColumn(COLUMN_LINK_TYPE);
    ASSERT(NULL != pColumn);

    KCodeManager* pCodeManager = m_pTarget->CodeManager();
    KCodeGroup*   pCodeData    = pCodeManager->FindCodeGroup(pColumn->CodeGroup());
        
    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;

    std::vector<TLinkInfluence> results;
    KDBaseStatistics::StatisticsLinkInfluence(m_pTarget->GetDBaseConnection(), results);
    std::vector<TLinkInfluence>::iterator iter = results.begin();
    std::vector<TLinkInfluence>::iterator end  = results.end();

    TLinkInfluence sumRecords;
    sumRecords.count  = 0;
    sumRecords.length = 0;

    int nNo = 0;
    while (iter != end)
    {
        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(++nNo));
        pItem->SetAlignment(DT_CENTER);

        int    nLinkType = iter->link_type;
        int    nCount    = iter->count;
        double dLength   = iter->length;
                
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(pCodeData->SingleCodeValue(nLinkType, 1)));    
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(nCount));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dLength, _T("%.2f")));
        pItem->SetAlignment(DT_CENTER);
        
        sumRecords.count  += nCount;
        sumRecords.length += dLength;

        ++iter;
    }  

    if (nNo > 0)
    {
        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("Sum")));    
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));    
        pItem->SetAlignment(DT_CENTER);

        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(sumRecords.count));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(sumRecords.length, _T("%.2f")));
        pItem->SetAlignment(DT_CENTER);
    }

    m_wndReportCtrl.Populate();
}

BOOL KInfluenceStatisticsDlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    CWaitCursor wc;

    InitReportHeader();
    InitReportData();
    m_wndReportCtrl.Populate();

    m_wndReportCtrl.CanCopy();
    m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
    SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    //SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}