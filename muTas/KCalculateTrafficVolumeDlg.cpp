// KCalculateTrafficVolumeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KCalculateTrafficVolumeDlg.h"
#include "afxdialogex.h"

#include "DBaseAssignment.h"
//^^ #include "IOTables.h"
#include "model_common.h"
#include "KBulkDBase.h"
// KCalculateTrafficVolumeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KCalculateTrafficVolumeDlg, CXTPResizeDialog)

KCalculateTrafficVolumeDlg::KCalculateTrafficVolumeDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KCalculateTrafficVolumeDlg::IDD, pParent)
    , m_nRadioButton(0)
{

}

KCalculateTrafficVolumeDlg::~KCalculateTrafficVolumeDlg()
{
}

void KCalculateTrafficVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
    DDX_Radio(pDX, IDC_RADIO1, (int&)m_nRadioButton);   
}


BEGIN_MESSAGE_MAP(KCalculateTrafficVolumeDlg, CXTPResizeDialog)
    ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KCalculateTrafficVolumeDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KCalculateTrafficVolumeDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_RADIO1, &KCalculateTrafficVolumeDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &KCalculateTrafficVolumeDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// KCalculateTrafficVolumeDlg 메시지 처리기입니다.
void KCalculateTrafficVolumeDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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

BOOL KCalculateTrafficVolumeDlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    Initial();
    m_wndReportCtrl.SetFreezeColumnsCount(5);
    m_wndReportCtrl.CanCopy();
    m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);    
    
    m_wndReportCtrl.Populate();
    SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KCalculateTrafficVolumeDlg::PostNcDestroy()
{
    CXTPResizeDialog::PostNcDestroy();    
    delete this;
}

void KCalculateTrafficVolumeDlg::Initial()
{
    KIOTables* pTables = m_pTarget->Tables();
    KIOTable* pTable = pTables->FindTable(TABLE_LINK); 

    if(NULL == pTable)
    {
        AfxMessageBox(_T("링크 테이블 정보가 존재하지 않습니다."));
        return;
    }

    CWaitCursor wc;
    //m_linkInfoRecords
    KBulkDBase::BulkLinkSelect(pTable, m_linkInfoRecords);
    InitReportHeader();
}

void KCalculateTrafficVolumeDlg::InitReportHeader()
{
    KIOTables* pTables = m_pTarget->Tables();
    KIOTable* pTable = pTables->FindTable(TABLE_LINK); 

    if(NULL == pTable)
    {
        AfxMessageBox(_T("링크 테이블 정보가 존재하지 않습니다."));
        return;
    }

    const 
    KIOColumns* pColumnCollection = pTable->Columns();

   /* m_wndReportCtrl.GetRecords()->RemoveAll();
    m_wndReportCtrl.GetColumns()->Clear();
    m_wndReportCtrl.Populate();*/

    CXTPReportColumn* pReportColumn = NULL;

    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Link"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);    
    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("FNode"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);
    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("TNode"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);
    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("차선수"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);
    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(4, _T("길이"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);

    //pReportColumn->SetFiltrable(TRUE);
    

    CStringArray columnsNameArray;
    columnsNameArray.Add(_T("survey_volume"));
    KDBaseAssignment::GetSumVolumeColumns(m_pTarget, columnsNameArray);

    int nColumnCount = columnsNameArray.GetCount();   

    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = pColumnCollection->GetColumn(columnsNameArray.GetAt(i));
        if (pColumn)
        {
            m_volColumns.AddNewColumn(pColumn);
        }
    }

    // 조사 교통량
    KIOColumn* pColumn = m_volColumns.GetColumn(0);
    CString    strColumnName = pColumn->Caption();
    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(5, strColumnName, 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);

    nColumnCount = m_volColumns.ColumnCount();
    for (int i=1; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = m_volColumns.GetColumn(i);
        CString    strColumnName = pColumn->Caption();
        pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(i*2+4, strColumnName, 10));
        pReportColumn->SetHeaderAlignment(DT_CENTER);
        pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(i*2+5, _T("정산률"), 10));
        pReportColumn->SetHeaderAlignment(DT_CENTER);
    }    

     m_wndReportCtrl.Populate();
}

void KCalculateTrafficVolumeDlg::InitReportData()
{
   
    //UpdateData(TRUE);
    CString strWhere;

    switch(m_nRadioButton)   
    {   
    case 0:   
        strWhere = _T(" Influence = 1 And survey_volume > 0 ");
        break;   
    case 1:   
        strWhere = _T(" survey_volume > 0 ");
        break;   
    }   

    KIOTables* pTables = m_pTarget->Tables();
    KIOTable* pTable = pTables->FindTable(TABLE_LINK); 

    if(NULL == pTable)
    {
        AfxMessageBox(_T("링크 테이블 정보가 존재하지 않습니다."));
        return;
    }

    // 첫번째..컬럼은 조사 교통량
    int nCount = m_volColumns.ColumnCount();
    KIDKeyDoubleRecords records(nCount);
    KBulkDBase::BulkLinkSelect(pTable, m_volColumns, records, strWhere);

    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;
    

    std::map<Integer, KSDoubleRecord*>::iterator iter = records.begin();
    std::map<Integer, KSDoubleRecord*>::iterator end  = records.end();

    std::map<Integer, TFixedLinkDB>::iterator    iterL;
    std::map<Integer, TFixedLinkDB>::iterator    iterLEnd = m_linkInfoRecords.end();

    while (iter != end)
    {
        Integer linkID = iter->first;
        KSDoubleRecord* pDataRecord = iter->second;

        iterL = m_linkInfoRecords.find(linkID);
        if (iterL == iterLEnd)
        {
            ++iter;
            continue;
        }

        TFixedLinkDB& linkInfo = iterL->second;

        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)linkInfo.link_id, _T("%.f")));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)linkInfo.fnode_id, _T("%.f")));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)linkInfo.tnode_id, _T("%.f")));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)linkInfo.lane,   _T("%.f")));
        pItem->SetAlignment(DT_CENTER);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(linkInfo.length, _T("%f")));
        pItem->SetAlignment(DT_RIGHT);

        int nDataCount = pDataRecord->GetCount();
        double dSurveyVolume = pDataRecord->GetAt(0);
        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dSurveyVolume, _T("%f")));
        pItem->SetAlignment(DT_RIGHT);

        for (int i=1; i<nDataCount; i++)
        {
            double dValue = pDataRecord->GetAt(i);
            pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%f")));
            pItem->SetAlignment(DT_RIGHT);
            pItem->SetBackgroundColor(RGB(255, 255, byte(100* i)));
            double dCalcValue = (dValue - dSurveyVolume) / dSurveyVolume * 100.0;
            pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dCalcValue, _T("%f")));
            pItem->SetAlignment(DT_RIGHT);
            pItem->SetBackgroundColor(RGB(196, 255, 155));
        }

        ++iter;
    }

    records.RemoveAll();
    m_wndReportCtrl.Populate();
}


void KCalculateTrafficVolumeDlg::OnBnClickedOk()
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(CalculateTrafficVolThreadCaller, this, false, nLang);

}
	
void KCalculateTrafficVolumeDlg::OnBnClickedCancel()
{
    DestroyWindow();
}


void KCalculateTrafficVolumeDlg::OnBnClickedRadio1()
{
    UpdateData(TRUE);
}


void KCalculateTrafficVolumeDlg::OnBnClickedRadio2()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    UpdateData(TRUE);
}

unsigned __stdcall KCalculateTrafficVolumeDlg::CalculateTrafficVolThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("Traffic Volume 구동 .... "));

	KCalculateTrafficVolumeDlg* pDlg = (KCalculateTrafficVolumeDlg*)pParameter->GetParameter();;

	pDlg->InitReportData();

	return 0;
}

void KCalculateTrafficVolumeDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}
