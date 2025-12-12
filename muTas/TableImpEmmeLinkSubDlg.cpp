// TableImportEmmeLinkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableImpEmmeLinkSubDlg.h"
#include "afxdialogex.h"


//#include "StdioFileEx.h"

// KTableImpEmmeLinkSubDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableImpEmmeLinkSubDlg, KResizeDialogEx)

KTableImpEmmeLinkSubDlg::KTableImpEmmeLinkSubDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableImpEmmeLinkSubDlg::IDD, pParent)
	, m_nStartLinkID(1)
	, m_strFile(_T(""))
	, m_cSeparator(_T(' '))
	, m_pColumns(NULL)
	, m_bAutoID(TRUE)
	, m_bLoadCSV(false)
{

}

KTableImpEmmeLinkSubDlg::~KTableImpEmmeLinkSubDlg()
{
	if (m_pColumns != NULL)
	{
		delete m_pColumns;
		m_pColumns = NULL;
	}
}

void KTableImpEmmeLinkSubDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Check  (pDX, IDC_CHECK3, m_bAutoID);
	DDX_Control(pDX, IDC_COMBO1, m_cboUser1);
	DDX_Control(pDX, IDC_COMBO2, m_cboUser2);
	DDX_Control(pDX, IDC_COMBO3, m_cboUser3);
	DDX_Control(pDX, IDC_REPORT_PREVIEW, m_ctrlReportPreview);
}


BEGIN_MESSAGE_MAP(KTableImpEmmeLinkSubDlg, KResizeDialogEx)
END_MESSAGE_MAP()


// KTableImpEmmeLinkSubDlg 메시지 처리기입니다.

void KTableImpEmmeLinkSubDlg::SetImportFile( CString a_strFile )
{
	m_strFile = a_strFile;
}

void KTableImpEmmeLinkSubDlg::SetTable( KIOTable* a_pTable )
{
	m_pTable = a_pTable;

	InitComboCtrl();
	PreviewLoadEmme();

	if (m_bLoadCSV)
	{
		InitializePreview();
	}
}

int KTableImpEmmeLinkSubDlg::GetStartLinkID( void )
{
	m_nStartLinkID = GetDlgItemInt(IDC_EDIT1);
	return m_nStartLinkID;
}


BOOL KTableImpEmmeLinkSubDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemInt(IDC_EDIT1, m_nStartLinkID);

	QBicReportCtrlSetting::Default(m_ctrlReportPreview, FALSE, FALSE, FALSE);
	QBicReportCtrlSetting::AutoColumnSizeFalse(m_ctrlReportPreview);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableImpEmmeLinkSubDlg::InitComboCtrl( void )
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

		if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0
			|| strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0
			|| strColumnName.CompareNoCase(COLUMN_FNODE_ID) == 0 
			|| strColumnName.CompareNoCase(COLUMN_TNODE_ID) == 0 
			|| strColumnName.CompareNoCase(COLUMN_LINK_LENGTH) == 0
			|| strColumnName.CompareNoCase(COLUMN_LINK_LANE) == 0
			|| strColumnName.CompareNoCase(COLUMN_LINK_FN_ID) == 0)
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

void KTableImpEmmeLinkSubDlg::PreviewLoadEmme( void )
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

	//define Header
	m_ColumnRow.clear();

	m_ColumnRow.push_back(_T("F_Node"));
	m_ColumnRow.push_back(_T("T_Node"));
	m_ColumnRow.push_back(_T("Length"));
	m_ColumnRow.push_back(_T("Modes"));
	m_ColumnRow.push_back(_T("Type"));
	m_ColumnRow.push_back(_T("Lane"));
	m_ColumnRow.push_back(_T("Fn_ID"));
	m_ColumnRow.push_back(_T("ul1"));
	m_ColumnRow.push_back(_T("ul2"));
	m_ColumnRow.push_back(_T("ul3"));

	CString strLine;

	int  nLoopCount = 0;
	bool bIsData    = false;

	while(importFile.ReadString( strLine ) && nLoopCount < 5000)
	{		
		if ((strLine.Trim()).CompareNoCase(_T("")) == 0)
			continue;

		if (!bIsData)
		{
			if ((strLine.Trim()).CompareNoCase(_T("t links init")) == 0)
				bIsData = true;

			continue;
		}

		++nLoopCount;

		RemoveDoubleSpace(strLine);
		
		if (nLoopCount % 5 == 0)
		{
			CSVRow row;
			ParseCSVLineString( strLine, row );
			m_Rows.push_back( row );
		}		
	}

	if ( m_Rows.size() == 0)
	{
		m_bLoadCSV = false;
	}
	else
	{
		m_bLoadCSV = true;
	}

	importFile.Close();
	TxLogDebugEndMsg();
}

void KTableImpEmmeLinkSubDlg::InitializePreview( void )
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

void KTableImpEmmeLinkSubDlg::ParseCSVLineString( CString& strLine, CSVRow& row )
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

void KTableImpEmmeLinkSubDlg::RemoveDoubleSpace( CString& strLine )
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
