// DefVDFFactorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefVDFFactorDlg.h"
#include "afxdialogex.h"

#include "Target.h"

#include "ReportCtrlSetting.h"
//^#include "DBaseConnector.h"

// KDefVDFFactorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefVDFFactorDlg, KResizeDialogEx)

KDefVDFFactorDlg::KDefVDFFactorDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefVDFFactorDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pColumns(NULL)
	, m_bOK(false)
{

}

KDefVDFFactorDlg::~KDefVDFFactorDlg()
{
	if (m_pColumns != NULL)
	{
		delete m_pColumns;
		m_pColumns = NULL;
	}
}

void KDefVDFFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KDefVDFFactorDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KDefVDFFactorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KDefVDFFactorDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KDefVDFFactorDlg 메시지 처리기입니다.


BOOL KDefVDFFactorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitColumnInfo();

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrl);

	InitReportHeader();
	UpdateReportData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefVDFFactorDlg::InitColumnInfo( void )
{
	if (m_pColumns != NULL)
	{
		delete m_pColumns;
		m_pColumns = NULL;
	}

	m_pColumns = new KIOColumns;

	KIOColumn* pColumn = nullptr;

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_SPEED) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_SPEED);
	}

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_CAPA) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_CAPA);
	}	

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_ALPHA) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_ALPHA);
	}	

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_BETA) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_BETA);
	}	

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_A) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_A);
	}	

	if (m_pUserDefineColumns->GetColumn(COLUMN_LINK_B) == nullptr)
	{
		pColumn = m_pColumns->AddNewColumn();
		pColumn->Name(COLUMN_LINK_B);
	}
}

void KDefVDFFactorDlg::InitReportHeader( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIdx(0);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(nColumnIdx++, _T("Func No"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);


	int nColumns = m_pColumns->ColumnCount();

	for (int i = 0; i < nColumns; ++i)
	{
		KIOColumn* pIOColumn = m_pColumns->GetColumn(i);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(nColumnIdx++, pIOColumn->Name(), 1));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(DT_CENTER);
	}

	m_wndReportCtrl.Populate();
}

void KDefVDFFactorDlg::UpdateReportData( void )
{
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	set<int>::iterator beginIter, endIter = m_setFnID.end();

	int        nVDFNo;
	int        nColumns;
	double     dValue;
	KIOColumn* pColumn = nullptr;

	for (beginIter = m_setFnID.begin(); beginIter != endIter; ++beginIter)
	{
		nVDFNo = *beginIter;
		
		pRecord  = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
		pItem    = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nVDFNo, _T("%.0f")));
		
		nColumns = m_pColumns->ColumnCount();

		for (int i = 0; i < nColumns; ++i)
		{
			pColumn = m_pColumns->GetColumn(i);
			dValue  = GetDefaultValue(pColumn->Name());
			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%.4f")));
		}
	}
	m_wndReportCtrl.Populate();
}

double KDefVDFFactorDlg::GetDefaultValue( CString a_strColumnName )
{
	double dDefaultValue;

	if (a_strColumnName.CompareNoCase(COLUMN_LINK_SPEED) == 0)
	{
		dDefaultValue = 100.0;
	}
	else if (a_strColumnName.CompareNoCase(COLUMN_LINK_CAPA) == 0)
	{
		dDefaultValue = 2000.0;
	}
	else if (a_strColumnName.CompareNoCase(COLUMN_LINK_ALPHA) == 0)
	{
		dDefaultValue = 0.15;
	}
	else if (a_strColumnName.CompareNoCase(COLUMN_LINK_BETA) == 0)
	{
		dDefaultValue = 4.0;
	}
	else if (a_strColumnName.CompareNoCase(COLUMN_LINK_A) == 0)
	{
		dDefaultValue = 0.0;
	}
	else if (a_strColumnName.CompareNoCase(COLUMN_LINK_B) == 0)
	{
		dDefaultValue = 0.0;
	}

	return dDefaultValue;
}

void KDefVDFFactorDlg::OnBnClickedOk()
{
	try
	{
		m_bOK = true;
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}
	//AfxMessageBox(_T("적용되었습니다."));
	KResizeDialogEx::OnOK();
}

void KDefVDFFactorDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KDefVDFFactorDlg::Apply()
{
	if (!m_bOK)
		return;

	//Query 생성
	CString strQuery = _T("Update link Set ");

	int        nColumns = m_pColumns->ColumnCount();
	KIOColumn* pColumn  = nullptr;

	for (int i = 0; i < nColumns; ++i)
	{
		pColumn = m_pColumns->GetColumn(i);

		strQuery.AppendFormat(_T("%s = ?, "), pColumn->Name());
	}

	strQuery.Truncate(strQuery.GetLength() - 2);
	strQuery.AppendFormat(_T(" Where %s = ?"), COLUMN_LINK_FN_ID);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		KPreparedStatementPtr       spPreparedStatement = spDBaseConnection->PrepareStatement(strQuery);

		CXTPReportRecords*          pRecords = m_wndReportCtrl.GetRecords();
		int                         nRecords = pRecords->GetCount();
		int                         nColumns = m_wndReportCtrl.GetColumns()->GetCount();


		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemNumber* pNumber  = nullptr;

		int nFunctionID;
		
		for (int i = 0; i < nRecords; ++i)
		{
			int nBindCount(1);
			pRecord = pRecords->GetAt(i);

			//Func No
			pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(0);
			nFunctionID = (int)pNumber->GetValue();

			//0번 컬럼은 위에서 Get
			for (int j = 1; j < nColumns; ++j)
			{
				pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(j);
				spPreparedStatement->BindDouble(nBindCount++, pNumber->GetValue());
			}
			
			spPreparedStatement->BindInt(nBindCount++, nFunctionID);

			spPreparedStatement->ExecuteUpdate();
			spPreparedStatement->Reset();
		}

		spDBaseConnection->Commit();

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_LINK);
		 
		pTable->Notify();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		throw ex;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		throw 1;
	}
}
