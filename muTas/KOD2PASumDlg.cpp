// KOD2PASumDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KOD2PASumDlg.h"
#include "afxdialogex.h"

#include "model_common.h"
//^^ #include "IOTables.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

// KOD2PASumDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KOD2PASumDlg, CXTPResizeDialog)

KOD2PASumDlg::KOD2PASumDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KOD2PASumDlg::IDD, pParent)
{
}

KOD2PASumDlg::~KOD2PASumDlg()
{
}

void KOD2PASumDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KOD2PASumDlg, CXTPResizeDialog)
    ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KOD2PASumDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &KOD2PASumDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KOD2PASumDlg 메시지 처리기입니다.
void KOD2PASumDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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

BOOL KOD2PASumDlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::DefaultF(m_wndReportCtrl, FALSE, TRUE);
    Initial();
    m_wndReportCtrl.Populate();
 
	SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KOD2PASumDlg::PostNcDestroy()
{
    CXTPResizeDialog::PostNcDestroy();    
    delete this;
}

void KOD2PASumDlg::Initial()
{
    KIOTables* pTables = m_pTarget->Tables();
    KIOTable* pTable = pTables->FindTable(TABLE_PURPOSE_OD); 

    if(NULL == pTable)
    {
        AfxMessageBox(_T("기종점 통행 데이터 테이블을 찾을 수 없습니다."));
        return;
    }

    CWaitCursor wc;

    //KIOColumnCollection  purposeColumns;
    const 
    KIOColumns* pColumnCollection = pTable->Columns();

    int nColumnCount = pColumnCollection->ColumnCount();
    CString strColumns = _T("");

    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = pColumnCollection->GetColumn(i);
        // TODO : 일단은 급한게... / 목적 컬럼인지 구별 필요
        if (pColumn->DataType() == KEMIODataTypeDouble)
        {
            m_purposeColumns.AddNewColumn(pColumn);

            if (strColumns.GetLength() > 0)
            {
                strColumns += _T(", ") + CString(pColumn->Caption());
            }
            else
            {
                strColumns = pColumn->Caption();
            }
        }
    }

    CString strMsg;
    strMsg.Format(_T("%s : %s [%s]"), m_pTarget->GetName(), pTable->Caption(), strColumns);

    SetDlgItemText(IDC_STATIC_INFO, strMsg);
    InitReportHeader();
}

void KOD2PASumDlg::InitReportHeader()
{
    CXTPReportColumn* pReportColumn = NULL;

    pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Zone"), 10));
    pReportColumn->SetHeaderAlignment(DT_CENTER);

    int nColumnCount = m_purposeColumns.ColumnCount();
    for (int i=0; i<nColumnCount; i++)
    {
        KIOColumn* pColumn = m_purposeColumns.GetColumn(i);
        CString    strColumnName = pColumn->Caption();
        pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(i*2+1, strColumnName + _T("[P]"), 10));
        pReportColumn->SetHeaderAlignment(DT_CENTER);
        pReportColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(i*2+2, strColumnName + _T("[A]"), 10));
        pReportColumn->SetHeaderAlignment(DT_CENTER);
    }    
}

void KOD2PASumDlg::InitReportData()
{
    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;
    KIOTables*      pTables = m_pTarget->Tables();
    KIOTable*             pTable  = pTables->FindTable(TABLE_PURPOSE_OD); 

    if(NULL == pTable)
    {
        return;
    }

    int nColumnCount = m_purposeColumns.ColumnCount();
    KIDKeyDoubleRecords productionRecords(nColumnCount);
    KIDKeyDoubleRecords attractionRecords(nColumnCount);

    KDBaseOD2PASum::OD2Production(m_pTarget, pTable, m_purposeColumns, productionRecords);
    KDBaseOD2PASum::OD2Attraction(m_pTarget, pTable, m_purposeColumns, attractionRecords);

    std::map<Integer, KSDoubleRecord*>::iterator iter = productionRecords.begin();
    std::map<Integer, KSDoubleRecord*>::iterator end  = productionRecords.end();

    while (iter != end)
    {
        Integer zoneID = iter->first;
        KSDoubleRecord* pRecordP = iter->second;
        KSDoubleRecord* pRecordA = attractionRecords.GetRecord(zoneID);

        if (pRecordA == NULL)
        {
            ++iter;
            continue;
        }

        pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

        /*CString strOut;
        strOut.Format(_T("%I64d"), zoneID);*/

        pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber((double)zoneID));
        pItem->SetAlignment(DT_CENTER);

        for (int i=0; i<nColumnCount; i++)
        {
            pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(pRecordP->GetAt(i), _T("%.2f")));
            pItem->SetAlignment(DT_RIGHT);
            pItem->SetBackgroundColor(RGB(196, 215, 155));
            pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(pRecordA->GetAt(i), _T("%.2f")));
            pItem->SetAlignment(DT_RIGHT);
        }       

        ++iter;
    }

    m_wndReportCtrl.Populate();
}


void KOD2PASumDlg::OnBnClickedOk()
{
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(OD2PASumThreadCaller, this, false, nLang);

}

void KOD2PASumDlg::OnBnClickedCancel()
{
    DestroyWindow();	
}

unsigned __stdcall KOD2PASumDlg::OD2PASumThreadCaller( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();
	pProgressWindow->SetStatus(_T("OD to PA Sum 구동 .... "));

	KOD2PASumDlg* pDlg = (KOD2PASumDlg*)pParameter->GetParameter();;

	pDlg->InitReportData();

	return 0;
}

void KOD2PASumDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}
