// TableImportEmme2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableImpEmmeNodeSubDlg.h"
#include "afxdialogex.h"


//#include "StdioFileEx.h"

// KTableImpEmmeNodeSubDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableImpEmmeNodeSubDlg, KResizeDialogEx)

KTableImpEmmeNodeSubDlg::KTableImpEmmeNodeSubDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableImpEmmeNodeSubDlg::IDD, pParent)
	, m_strFile(_T(""))
	, m_bLoadCSV(false)
	, m_cSeparator(_T(' '))
	, m_pColumns(NULL)
{
	m_nNodeCount = 0;
}

KTableImpEmmeNodeSubDlg::~KTableImpEmmeNodeSubDlg()
{
	if (m_pColumns != NULL)
	{
		delete m_pColumns;
		m_pColumns = NULL;
	}
}

void KTableImpEmmeNodeSubDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboUser1);
	DDX_Control(pDX, IDC_COMBO2, m_cboUser2);
	DDX_Control(pDX, IDC_COMBO3, m_cboUser3);
	DDX_Control(pDX, IDC_REPORT_PREVIEW, m_ctrlReportPreview);
}


BEGIN_MESSAGE_MAP(KTableImpEmmeNodeSubDlg, KResizeDialogEx)
END_MESSAGE_MAP()


// KTableImpEmmeNodeSubDlg 메시지 처리기입니다.

void KTableImpEmmeNodeSubDlg::SetImportFile( CString a_strFile )
{
	m_strFile = a_strFile;	
}

void KTableImpEmmeNodeSubDlg::SetTable( KIOTable* a_pTable )
{
	m_pTable = a_pTable;

	InitComboCtrl();
	PreviewLoadEmme();

	if (m_bLoadCSV)
	{
		InitializePreview();
	}
}

BOOL KTableImpEmmeNodeSubDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	QBicReportCtrlSetting::Default(m_ctrlReportPreview, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::AutoColumnSizeFalse(m_ctrlReportPreview);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTableImpEmmeNodeSubDlg::InitComboCtrl( void )
{
	m_cboUser1.ResetContent();
	m_cboUser2.ResetContent();
	m_cboUser3.ResetContent();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_cboUser1.InsertString(-1, _T("--------------선택--------------"));
		m_cboUser2.InsertString(-1, _T("--------------선택--------------"));
		m_cboUser3.InsertString(-1, _T("--------------선택--------------"));
	}
	else
	{
		m_cboUser1.InsertString(-1, _T("---------Select---------"));
		m_cboUser2.InsertString(-1, _T("---------Select---------"));
		m_cboUser3.InsertString(-1, _T("---------Select---------"));
	}

	const KIOColumns* pColumns = m_pTable->Columns();

	int nColumnCnt = pColumns->ColumnCount();
	int nIndex(0);

	for (int i = 0; i < nColumnCnt; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		if (pColumn->DataType() == KEMIODataTypeString)
			continue;

		CString strColumnName = pColumn->Name();

		if (strColumnName.CompareNoCase(COLUMN_NODE_ID) == 0 
			|| strColumnName.CompareNoCase(COLUMN_NODE_X) == 0 
			|| strColumnName.CompareNoCase(COLUMN_NODE_Y) == 0
			|| strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
			continue;

		nIndex = m_cboUser1.InsertString(-1, pColumn->Caption());
		m_cboUser1.SetItemData(nIndex, (DWORD_PTR)pColumn);

		nIndex = m_cboUser2.InsertString(-1, pColumn->Caption());
		m_cboUser2.SetItemData(nIndex, (DWORD_PTR)pColumn);

		nIndex = m_cboUser3.InsertString(-1, pColumn->Caption());
		m_cboUser3.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cboUser1.SetCurSel(0);
	m_cboUser2.SetCurSel(0);
	m_cboUser3.SetCurSel(0);
}

void KTableImpEmmeNodeSubDlg::PreviewLoadEmme( void )
{
	TxLogDebugStartMsg();
	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	
	CStdioFileEx importFile;
	importFile.SetCodePage(nCodePage);

	if (importFile.Open(m_strFile, nOpenMode) == FALSE)
	{
		m_bLoadCSV = false;
		importFile.Close();
		return;
	}
	

	//Define Header

	m_ColumnRow.clear();

	m_ColumnRow.push_back(_T("ID"));
	m_ColumnRow.push_back(_T("X"));
	m_ColumnRow.push_back(_T("Y"));
	m_ColumnRow.push_back(_T("ui1"));
	m_ColumnRow.push_back(_T("ui2"));
	m_ColumnRow.push_back(_T("ui3"));
	m_ColumnRow.push_back(_T("Label"));

	CString strLine;

	int  nLoopCount = 0;
	bool bIsData    = false;

	// 2019.05.28 Free 버전 관련 수정
	m_nNodeCount = 0;

	while(importFile.ReadString( strLine ) && nLoopCount < 5000)
	{
		if ((strLine.Trim()).CompareNoCase(_T("")) == 0)
			continue;

		if (!bIsData)
		{
			 if ((strLine.Trim()).CompareNoCase(_T("t nodes init")) == 0)
				 bIsData = true;

			 continue;
		}

		++nLoopCount;
		++m_nNodeCount;	// 한 행이 하나의 노드 데이터, 노드 갯수를 증가시킨다.

		//link 정보가 시작되면 break;
		if ((strLine.Trim()).CompareNoCase(_T("t links init")) == 0)
			break;

		RemoveDoubleSpace(strLine);
		
		if (nLoopCount % 5 == 0)
		{
			CSVRow row;
			ParseCSVLineString( strLine, row );
			m_Rows.push_back( row );
		}		
	}

	importFile.Close();
	m_bLoadCSV = true;
	TxLogDebugEndMsg();
}

void KTableImpEmmeNodeSubDlg::InitializePreview( void )
{
	TxLogDebugStartMsg();

	CXTPReportColumn* pColumn = nullptr;
	int nColumnIndex = 0;
	std::vector< CString >::iterator itColumnArray = m_ColumnRow.begin();
	
	while( m_ColumnRow.end() != itColumnArray )
	{
		pColumn = m_ctrlReportPreview.AddColumn(new CXTPReportColumn (nColumnIndex, *itColumnArray, 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

		++nColumnIndex;
		++itColumnArray;
	}

	m_ctrlReportPreview.Populate();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	//add Data
	int nRow = 0;
	std::vector<CSVRow>::iterator itRows, itRowsEnd = m_Rows.end();
	for(itRows = m_Rows.begin(); itRows != itRowsEnd; ++itRows)
	{
		pRecord = m_ctrlReportPreview.AddRecord(new CXTPReportRecord());

		CSVRow& row = (*itRows);
		CSVRow::iterator itrow, itrowEnd = row.end();
		int nInsertIndex = 0;
		for(itrow = ++row.begin(); itrow != itrowEnd; ++itrow)
		{
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(*itrow));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable(FALSE);

			++nInsertIndex;
		}

		++nRow;
	}

	m_ctrlReportPreview.Populate();

	CXTPReportHeader*  pHeader        = m_ctrlReportPreview.GetReportHeader();
	CXTPReportColumns* pReportColumns = m_ctrlReportPreview.GetColumns();
	int                nReportColumns = pReportColumns->GetCount();
	for (int i= 0; i< nReportColumns; i++)
	{
		CXTPReportColumn* pReportColumn = pReportColumns->GetAt(i);
		pHeader->BestFit(pReportColumn);
	}

	TxLogDebugEndMsg();
}

void KTableImpEmmeNodeSubDlg::ParseCSVLineString( CString& strLine, CSVRow& row )
{
	bool bQuote = false;
	int nSub = 0;
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, m_cSeparator ) == TRUE )
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
				row.push_back(strRow);
			}
		}
		else
		{
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE )
			{
				row.push_back( strRow );
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
					row.push_back(strRow);
				}
			}
			else
			{
				chFirst = strRow.GetAt( 0 );
				chLast = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) )
				{
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				}
				else if( chFirst == _T('"') )
				{
					bQuote = true;
					strQuote = strRow;
				}
				else
				{
					row.push_back( strRow );
				}
			}
		}
	}
}

void KTableImpEmmeNodeSubDlg::RemoveDoubleSpace( CString& strLine )
{
	try
	{
		CString strTempLine = strLine;
		int     nResult(1);
		while (nResult > 0)
		{
			nResult = strTempLine.Replace(_T("  "), _T(" ")); // '  ' -> ' ' 변환
		}

		strLine = strTempLine;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


// 2019.05.28 : Free 버전 관련 수정
int KTableImpEmmeNodeSubDlg::GetNodeCount()
{
	return m_nNodeCount;
}