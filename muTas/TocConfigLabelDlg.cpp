// TocConfigLabelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TocConfigLabelDlg.h"
#include "afxdialogex.h"

#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

// KTocConfigLabelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTocConfigLabelDlg, KResizeDialogEx)

KTocConfigLabelDlg::KTocConfigLabelDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTocConfigLabelDlg::IDD, pParent)
{
	m_pTarget = NULL;
}

KTocConfigLabelDlg::~KTocConfigLabelDlg()
{
}

void KTocConfigLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KTocConfigLabelDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KTocConfigLabelDlg::OnBnClickedOk)
    ON_NOTIFY(NM_CLICK,  IDC_REPORT, &KTocConfigLabelDlg::OnReportItemClick)
END_MESSAGE_MAP()


// KTocConfigLabelDlg 메시지 처리기입니다.
BOOL KTocConfigLabelDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
	InitReportHeader();
	m_wndReport.ResetContent(); 
	InitReportData();
	m_wndReport.Populate();

	SetResize(IDC_REPORT,        SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDOK,              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	UpdateData(FALSE);

	return TRUE;  
}


void KTocConfigLabelDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("선택"), 50, FALSE));
	}
	else
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(0, _T("Select"), 50, FALSE));
	}
	pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("컬럼명"), 80));
	}
	else
	{
		pColumn = m_wndReport.AddColumn(new CXTPReportColumn(1, _T("Field"), 80));
	}
	pColumn->SetHeaderAlignment(DT_CENTER);
}


void KTocConfigLabelDlg::InitReportData()
{
	if (m_pTarget == NULL)
	{
		TxLogDebugException();
		return;
	}
	
	const KIOColumns* pIOColumns;
	if (m_emLayerType == NODE)
	{
		pIOColumns = m_pTarget->Tables()->FindTable(TABLE_NODE)->Columns();
	}

	if (m_emLayerType == LINK)
	{
		pIOColumns = m_pTarget->Tables()->FindTable(TABLE_LINK)->Columns();
	}
		
	m_wndReport.ResetContent(TRUE);
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	CString strTok;
	std::set<CString> setSelectedColumnName;
	
	int nTokenPos = 0; 
	while (AfxExtractSubString(strTok, m_strSelectColumns, nTokenPos++, ',')) {
		setSelectedColumnName.insert(strTok.MakeLower());

        if (nTokenPos > 0) {//★ 2016-03-09 : 1개만 선택되게 꼼수, 복수 컬럼 라벨 적용할 수 없도록
            break;
        }
	}

	std::set<CString>::iterator iter = setSelectedColumnName.begin();
	std::set<CString>::iterator end  = setSelectedColumnName.end();

	int nColumnCount = pIOColumns->ColumnCount();
	for(int i= 0; i< nColumnCount; i++)
	{
		pRecord = m_wndReport.AddRecord(new CXTPReportRecord());

		KIOColumn* pColumn        = pIOColumns->GetColumn(i);
		CString    strDisplayName = pColumn->Caption();
		CString    strColumnName  = pColumn->Name();
		
		// - 선택
		pItem = pRecord->AddItem(new CXTPReportRecordItemText);
		pItem->HasCheckbox(TRUE);		
		pItem->SetAlignment(xtpColumnIconCenter);

		iter = setSelectedColumnName.find(strColumnName.MakeLower());
		if (iter != end)
		{
			pItem->SetChecked(TRUE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}
		// - name
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strDisplayName ));
		pItem->SetAlignment(xtpColumnTextLeft);
		pItem->SetItemData((DWORD_PTR) pColumn);
	}

	m_wndReport.Populate();
}

void KTocConfigLabelDlg::OnBnClickedOk()
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItem       = NULL;

	pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	
	std::vector<CString> vecSelectedColumns;
	BOOL bCheck;

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(0);
		bCheck  = pItem->IsChecked();

		pItem   = pRecord->GetItem(1);
		KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();

		if( TRUE == bCheck )
		{
			vecSelectedColumns.push_back(pColumn->Name());
		}
	}

	m_strSelectColumns = _T("");

	size_t nxCount = vecSelectedColumns.size();
	for (size_t i=0; i<nxCount; i++)
	{
		if (i == nxCount -1)
		{
			m_strSelectColumns = m_strSelectColumns + vecSelectedColumns[i];
		}
		else
		{
			m_strSelectColumns = m_strSelectColumns + vecSelectedColumns[i] + _T(",");
		}		
	}
		
	KResizeDialogEx::OnOK();
}

void KTocConfigLabelDlg::OnReportItemClick( NMHDR* pNMHDR, LRESULT* pResult )
{//★ 복수 라벨 선택 제거
    try
    {
        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

        if (!pClickedItem->pRow || !pClickedItem->pColumn)
            return;

        int nRowIndex    = pClickedItem->pRow->GetIndex();
        int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

        CXTPReportRecords*	  pRecords = NULL;
        CXTPReportRecord*     pRecord  = NULL;
        CXTPReportRecordItem* pItem    = NULL;
        pRecords = m_wndReport.GetRecords();

        if( 0 == nColumnIndex ) {
            int nRecordCount = pRecords->GetCount();
            for (int i=0; i<nRecordCount; i++)
            {
                pRecord    = pRecords->GetAt(i);
                pItem      = pRecord->GetItem(0);
                pItem->SetChecked(FALSE);
            }

            pRecord = pClickedItem->pRow->GetRecord();
            pItem   = pRecord->GetItem(0);
            pItem->SetChecked(TRUE);
        }
    }
    catch(...)
    {
        TxLogDebugException();
    }    
}
