// PrepareChartPAColumnDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PrepareChartPAColumnDlg.h"
#include "afxdialogex.h"

#include "Target.h"
//^#include "DBaseConnector.h"
#include "KExRecordItem.h"
#include "KmzSystem.h"
#include "DBaseChart.h"

enum
{
	_0_COLUMN_SELECT     = 0,
	_1_COLUMN_FIELDNAME  = 1,
	_2_COLUMN_STARTDATE  = 2,
	_3_COLUMN_ENDDATE    = 3
};

// KPrepareChartPAColumnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KPrepareChartPAColumnDlg, KResizeDialogEx)

KPrepareChartPAColumnDlg::KPrepareChartPAColumnDlg(KTarget* a_pTarget, KEMPAType a_emPAType, KIOColumns* a_pSelectedColumns, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPrepareChartPAColumnDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_emPAType(a_emPAType)
	, m_pSelectedCoumns(a_pSelectedColumns)
{

}

KPrepareChartPAColumnDlg::~KPrepareChartPAColumnDlg()
{
}

void KPrepareChartPAColumnDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KPrepareChartPAColumnDlg, KResizeDialogEx)
		ON_BN_CLICKED(IDC_CHECK_ALL,            &KPrepareChartPAColumnDlg::OnBnClickedCheckAll)
		ON_NOTIFY(NM_CLICK, IDC_REPORT,  &KPrepareChartPAColumnDlg::OnReportItemClick)
		ON_BN_CLICKED(IDOK, &KPrepareChartPAColumnDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDCANCEL, &KPrepareChartPAColumnDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KPrepareChartPAColumnDlg 메시지 처리기입니다.
BOOL KPrepareChartPAColumnDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	
	CString strTitle(_T("Production Column :"));
	if( KEMPATypeAttraction == m_emPAType )
	{
		CString strAttraction = (KmzSystem::IsPassengerSystem() == true) ? ATTRACTION_STRING : CONSUMPTION_STRING;
		strTitle.Format(_T("%s Column :"), strAttraction);
	}
	SetDlgItemText(IDC_STATIC_TITLE, strTitle);

	std::vector<TGenerationModelDB> vecGenerationModelDB;
	KDBaseChart::LoadPAColumns(m_pTarget, m_emPAType, vecGenerationModelDB);

	KReportCtrlSetting::DefaultC(m_wndReportCtrl);
	InitReportHeader();
	InitReportData(vecGenerationModelDB);
	m_wndReportCtrl.Populate();

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPrepareChartPAColumnDlg::LoadPAColumns(std::vector<TGenerationModelDB> &a_vecGenerationModelDB)
{
	a_vecGenerationModelDB.clear();

	CString strSQL(_T(""));
	CString strFlagPA(_T(""));

	if(KEMPATypeProduction == m_emPAType)
		strFlagPA = _T("p");
	else
		strFlagPA = _T("a");

	strSQL.Append      (_T(" SELECT "));
	strSQL.Append      (_T(" Zone_Column_Name, Start_Date, End_Date "));
	strSQL.AppendFormat(_T(" FROM %s "), TABLE_GENERATION_MODEL);
	strSQL.AppendFormat(_T(" WHERE PA_Flag = '%s' "), strFlagPA);
	strSQL.Append      (_T(" ORDER BY Generation_run_seq "));

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{        
		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			TGenerationModelDB oTGenerationModelDB;
			oTGenerationModelDB.strColumnName   = spResultSet->GetValueString(0);
			oTGenerationModelDB.strStartDate    = spResultSet->GetValueString(1);
			oTGenerationModelDB.strEndDate      = spResultSet->GetValueString(2);

			a_vecGenerationModelDB.push_back(oTGenerationModelDB);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(_T("컬럼 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		AfxMessageBox(_T("컬럼 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
}


void KPrepareChartPAColumnDlg::InitReportHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,       _T("선택"),        60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_FIELDNAME,    _T("Field"),       30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_STARTDATE,    _T("start_date"),  35));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_ENDDATE,      _T("end_date"),    35));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
}


void KPrepareChartPAColumnDlg::InitReportData(std::vector<TGenerationModelDB> &a_vecGenerationModelDB)
{
	m_wndReportCtrl.ResetContent();

	try
	{
		KIOTables*  pIOTables  = m_pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_ZONE);
		const 
        KIOColumns* pIOColumns = pIOTable->Columns();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		size_t nCheckedCount = 0;
		for (size_t i= 0; i< a_vecGenerationModelDB.size(); i++)
		{
			TGenerationModelDB &oTGenerationModelDB = a_vecGenerationModelDB[i];
			
			KIOColumn* pIOColumn = pIOColumns->GetColumn(oTGenerationModelDB.strColumnName);
			if( nullptr == pIOColumn )
			{
				continue;
			}

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new KExRecordItemCheck());
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
			if( nullptr == m_pSelectedCoumns->GetColumn(oTGenerationModelDB.strColumnName) )
			{
				pItem->SetChecked(FALSE);
			}
			else
			{
				pItem->SetChecked(TRUE);
				nCheckedCount++;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pIOColumn->Caption()));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetItemData((DWORD_PTR)pIOColumn);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTGenerationModelDB.strStartDate));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(oTGenerationModelDB.strEndDate));
			pItem->SetAlignment(xtpColumnTextCenter);
		}

		if( nCheckedCount == a_vecGenerationModelDB.size() )
		{
			CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		return;
	}
	catch (...)
	{
		TxLogDebugException();	
		return;
	} 
}


void KPrepareChartPAColumnDlg::ResizeComponent()
{
	SetResize(IDC_REPORT,       			  SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR4,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,						      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KPrepareChartPAColumnDlg::OnBnClickedCheckAll()
{
	try
	{
		BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ALL);    

		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nCount = pRecords->GetCount();

		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		return;
	}
}


void KPrepareChartPAColumnDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_COLUMN_SELECT == nIndex)
	{
		CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
		int nTotalCount = pRecords->GetCount();

		int nCheckedCount = 0;
		for (int i=0; i<nTotalCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_COLUMN_SELECT);
			
			if (pItem->IsChecked() == TRUE)
				nCheckedCount++;
		}

		if(nCheckedCount == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
		if(nCheckedCount < nTotalCount || 0 == nTotalCount)
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
	}
}


void KPrepareChartPAColumnDlg::OnBnClickedOk()
{
	try
	{
		KIOColumns oSelectedColumns;

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount       = pRecords->GetCount();
		int nSelectedCnt = 0;
		for (int i= 0; i<nCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_SELECT);

			if( !pItem->IsChecked())
				continue;

			pItem   = pRecord->GetItem(_1_COLUMN_FIELDNAME);
			KIOColumn* pIOCoumn = (KIOColumn*)pItem->GetItemData();
			oSelectedColumns.AddNewColumn(pIOCoumn);

			++nSelectedCnt;
		}

		*m_pSelectedCoumns = oSelectedColumns;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		return;
	}

	KResizeDialogEx::OnOK();
}


void KPrepareChartPAColumnDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
