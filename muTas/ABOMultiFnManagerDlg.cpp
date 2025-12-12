

#include "stdafx.h"
#include "ABOMultiFnManagerDlg.h"
#include "afxdialogex.h"

//#include "StdioFileEx.h"
#include "DBaseAssignment.h"
//^#include "DBaseConnector.h"
#include "Target.h"

IMPLEMENT_DYNAMIC(KABOMultiFnManagerDlg, KDialogEx)

KABOMultiFnManagerDlg::KABOMultiFnManagerDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KABOMultiFnManagerDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KABOMultiFnManagerDlg::~KABOMultiFnManagerDlg()
{
}

void KABOMultiFnManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KABOMultiFnManagerDlg, KDialogEx)
	ON_BN_CLICKED(ID_BTN_IMPORT, &KABOMultiFnManagerDlg::OnBnClickedBtnImport)
END_MESSAGE_MAP()



void KABOMultiFnManagerDlg::SetColumnInfo( std::vector<CString> vecColumnInfo )
{
	m_vecColumnInfo = vecColumnInfo;
}



BOOL KABOMultiFnManagerDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitReportCtrl();

	GetLinkFunctionID();

	if (m_vecRows.size() == 0)
	{
		UPdateInitReportCtrl();
	}
	else
	{
		UpdateReportImportCtrl();
	}
	

	return TRUE; 
}


void KABOMultiFnManagerDlg::InitReportCtrl()
{
	CXTPReportColumn* pReportColumn = NULL;

	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
	
	pReportColumn = pReportColumn = new CXTPReportColumn(0, _T("Function No."), 80);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);
	
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pReportColumn->SetCaption(_T("함수번호"));
    }

	int nColumnItemIndex(1);
	size_t nCount = m_vecColumnInfo.size();

	for (size_t i = 0; i < nCount; i ++)
	{
		CString strColumnName = m_vecColumnInfo[i];
		pReportColumn = pReportColumn = new CXTPReportColumn(nColumnItemIndex++, strColumnName, 80);
		pReportColumn = m_wndReport.AddColumn(pReportColumn);
		pReportColumn->SetHeaderAlignment(DT_CENTER);
		pReportColumn->SetAlignment(DT_RIGHT);
	}

	m_wndReport.AllowEdit(TRUE);
	m_wndReport.Populate();
}


void KABOMultiFnManagerDlg::OnBnClickedBtnImport()
{
	TCHAR szFilter[] = _T("(*.txt)|*.txt|");

	CFileDialog fileDlg(true, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);

	if( fileDlg.DoModal() != IDOK )
		return;

	m_strImportFileName = fileDlg.GetPathName();

	ReadBPRTextFile();

	UpdateReportImportCtrl();
}


void KABOMultiFnManagerDlg::ReadBPRTextFile( void )
{
	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	CStdioFileEx csvFile;
	try
	{
		m_vecRows.clear();

		CString strReadLine;
		csvFile.SetCodePage(nCodePage);

		if(csvFile.Open(m_strImportFileName, nOpenMode) != TRUE)
			ThrowException(_T("File Open Error"));

		while (csvFile.ReadString(strReadLine))
		{
			CString strRow, strQuote;
			int nSub(0);
			std::vector<CString> vecRow;
			bool bQuote = false;
			while ( AfxExtractSubString(strRow, strReadLine, nSub++, _T('\t')))
			{
				if( true == bQuote )
				{
					strQuote += strRow;
					if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') )
					{
						strQuote.Remove( _T('"') );
						strRow = strQuote;
						bQuote = false;
						strQuote = _T("");
						vecRow.push_back(strRow);
					}
				}
				else
				{
					TCHAR chFirst, chLast;
					if( strRow.IsEmpty() == TRUE )
					{
						vecRow.push_back( strRow );
					}
					else if( strRow.GetLength() == 1 )
					{
						chFirst = strRow.GetAt( 0 );
						if( chFirst == _T('"') )
						{
							bQuote = true;
						}
						else
						{
							vecRow.push_back(strRow);
						}
					}
					else
					{
						chFirst = strRow.GetAt( 0 );
						chLast = strRow.GetAt( strRow.GetLength() - 1 );

						if( (chFirst == _T('"')) && (chLast == _T('"')) )
						{
							strRow.Remove( _T('"') );
							vecRow.push_back( strRow );
						}
						else if( chFirst == _T('"') )
						{
							bQuote = true;
							strQuote = strRow;
						}
						else
						{
							vecRow.push_back( strRow );
						}
					}
				}
			}

			int ImportRowCount = vecRow.size();
			int ColumnCount	  = m_vecColumnInfo.size() + 1; // Function No 컬럼 추가

			if (ImportRowCount != ColumnCount)
			{
				AfxMessageBox(_T("선택한 컬럼과 파일에 데이터가 일치하지 않습니다."));
				csvFile.Close();
				return;
			}

			m_vecRows.push_back(vecRow);
		}
		csvFile.Close();
	}
	catch(KExceptionPtr ex)
	{
		csvFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		csvFile.Close();
		TxLogDebugException();
		return;
	}
}


void KABOMultiFnManagerDlg::UpdateReportImportCtrl()
{
	m_wndReport.ResetContent();

	int nRecordCount = m_vecRows.size();

	for (int i = 0; i < nRecordCount; ++i)
	{
		std::vector<CString> row = m_vecRows[i];
		int nColumnCount = row.size();

		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		__int64 nxNo = _ttoi64(row[0]);
		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNo));
		pRecordItem->SetEditable(FALSE);

		for (int nIndex = 1; nIndex < nColumnCount; nIndex++)
		{
			double dValue = _ttof(row[nIndex]);
			pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dValue, _T("%g")));
		}
	}
	m_wndReport.Populate();
}


void KABOMultiFnManagerDlg::GetImportData( std::vector<std::vector<CString>>& vecRows )
{

	m_vecRows.clear();

	CXTPReportRecords* pReportRecords= m_wndReport.GetRecords();
	int nReportCount = pReportRecords->GetCount();
	CString strValue;

	for (int i = 0; i < nReportCount; ++i)
	{
		CXTPReportRecord* pReportRecord = pReportRecords->GetAt(i);
		CXTPReportRecordItemNumber*	pRecordItemNumber = NULL;

		std::vector<CString> vecRow;

		int nColumnCount = m_vecColumnInfo.size();
		
		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(0);
		strValue.Format(_T("%d"), (int)pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		for (int nIndex = 0; nIndex < nColumnCount; nIndex++)
		{
			pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(nIndex + 1);
			strValue.Format(_T("%f"), (double)pRecordItemNumber->GetValue());
			vecRow.push_back(strValue);
		}
		m_vecRows.push_back(vecRow);
	}

	vecRows = m_vecRows;
	return;
}


void KABOMultiFnManagerDlg::UPdateInitReportCtrl()
{

	m_wndReport.ResetContent();

	int nCount = m_vecLinkFunction.size();
	for (int i = 0; i < nCount ; i++)
	{
		CString strFuntionID = m_vecLinkFunction[i];
		int nFunctionID		 = _ttoi(strFuntionID);

		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nFunctionID));
		pRecordItem->SetEditable(FALSE);

		int nColumnCount = m_vecColumnInfo.size();
		
		for (int i = 0; i < nColumnCount; i++)
		{
			CString strColumnName = m_vecColumnInfo[i];

			CString strCmma = strColumnName.Right(1);

			if (strCmma == _T("α"))
			{
				pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.15, _T("%g")));
			}
			else if (strCmma == _T("β"))
			{
				pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(4.0, _T("%g")));
			}
			else 
			{
				pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%g")));
			}
		}
	}
	m_wndReport.Populate();
}

bool KABOMultiFnManagerDlg::GetLinkFunctionID()
{
	m_vecLinkFunction.clear();

	CString strSQL(_T("Select function_id From link Group By function_id "));

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{	
			int nFunctionID = spResult->GetValueInt(0);
			
			CString strFunctionID(_T(""));
			strFunctionID.Format(_T("%d"), nFunctionID);

			m_vecLinkFunction.push_back(strFunctionID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		m_vecLinkFunction.clear();
		return false;
	}

	return true;
}

void KABOMultiFnManagerDlg::SetFnData( std::vector<std::vector<CString>>& vecRows )
{
	m_vecRows = vecRows;
}

