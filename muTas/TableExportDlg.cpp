/**
 * @file TableExportDlg.cpp
 * @brief KTableExportDlg 구현
 * @author 
 * @date 2010.08.16
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "TableExportDlg.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOItem.h"
//^^ #include "IORecordset.h"
//^^ #include "IORow.h"
//^^ #include "IOTable.h"
//#include "StdioFileEx.h"
//^#include "DBaseConnector.h"

#include "Project.h"
#include "Scenario.h"
#include "Target.h"
#include "ImportTableInfo.h"




/******************************************************************************
* KTableExportDlg                                                             *
******************************************************************************/
IMPLEMENT_DYNAMIC(KTableExportDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KTableExportDlg::KTableExportDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableExportDlg::IDD, pParent)
	, m_pTable(NULL)
	, m_pTarget(NULL)
	, m_bExportColumn(TRUE)
	, m_nFieldSeparator(3)
	, m_nEncoding(0)
	, m_bSetDataType(false)
{
	m_pExportColumns = new KIOColumns;
}


KTableExportDlg::~KTableExportDlg()
{
	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject  = (KProject*)pScenario->GetParentObject();

	CString aa = pProject->GetLocation();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());

	if(NULL != m_pExportColumns)
	{
		delete m_pExportColumns;
	}
}


/**************************************
* Overrides                           *
**************************************/
BOOL KTableExportDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ASSERT(m_pTarget != NULL);

	m_lstColumnList.SetExtendedStyle( LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT );
	m_lstColumnList.InsertColumn(  0, _T("Column Name") );
	m_lstColumnList.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	m_lstColumnList.InsertColumn(  1, _T("Column Data Type") );
	m_lstColumnList.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	m_lstColumnList.InsertColumn(  2, _T("Column Information") );
	m_lstColumnList.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);

	if( NULL != m_pTable )
	{
		TCHAR szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
		m_editFile.SetDlgOpenFile( FALSE );
		m_editFile.SetReadOnly( TRUE );
		m_editFile.SetDlgTitle( _T("Select file") );
		m_editFile.SetDlgFilter( szFilter );
		m_editFile.SetDlgStyle(OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);

		KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
		KProject*  pProject  = (KProject*)pScenario->GetParentObject();

		m_editFile.SetDlgInitialDir(pProject->GetLocation());
		
		//node-based, link-based Select;
		CString strTableName = m_pTable->Name();

		if (strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0)
		{
			m_bSetDataType = true;

			GetDlgItem(IDC_STATIC_DATATYPE)->ShowWindow(TRUE);
			GetDlgItem(IDC_COMBO_DATATYPE)->ShowWindow(TRUE);

			InitComboBaseDataType();
		}

		UpdateExportColumnInfo();
	}

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTableExportDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Check  (pDX, IDC_CHECK1,         m_bExportColumn);
	DDX_Radio  (pDX, IDC_RADIO5,         m_nEncoding);
	DDX_Control(pDX, IDC_LIST1,          m_lstColumnList);
	DDX_Control(pDX, IDC_FILENAME,       m_editFile );
	DDX_Radio  (pDX, IDC_RADIO8,         m_nFieldSeparator);
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_cboDataType);
}

BEGIN_MESSAGE_MAP(KTableExportDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTableExportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK3, &KTableExportDlg::OnBnClickedCheckSelectAll)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &KTableExportDlg::OnLvnItemchangedList1)
	ON_CBN_SELCHANGE(IDC_COMBO_DATATYPE, &KTableExportDlg::OnSelchangeComboBaseDataType)
END_MESSAGE_MAP()

void KTableExportDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	CString strFile;
	m_editFile.GetWindowText( strFile );
	if( strFile.IsEmpty() == true )
	{
		AfxMessageBox( _T("파일명이 비어있습니다.") );
		return;
	}

	switch( m_nEncoding )
	{
	case 0 : m_emCSVEncoding = KEMCSVEncodingANSI; break;
	case 1 : m_emCSVEncoding = KEMCSVEncodingUTF8; break;
	case 2 : m_emCSVEncoding = KEMCSVEncodingUTF16LE; break;
	default : m_emCSVEncoding = KEMCSVEncodingUnknown; break;
	}

	switch( m_nFieldSeparator )
	{
	case 0 : m_strSeparator = _T(","); break;
	case 1 : m_strSeparator = _T(";"); break;
	case 2 : m_strSeparator = _T("|"); break;
	case 3 : m_strSeparator = _T("\t"); break;
	case 4 : m_strSeparator = _T(" "); break;
	}

	int nColumnCount = m_lstColumnList.GetItemCount();
	m_pExportColumns->Clear();

	for( int i = 0; i < nColumnCount; ++i )
	{
		if( m_lstColumnList.GetCheck( i ) == TRUE )
		{
			KIOColumn* pColumn = (KIOColumn*)(m_lstColumnList.GetItemData(i));
			m_pExportColumns->AddNewColumn(pColumn);
		}
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(TableExportThreadCaller, this, false, nLang);
	OnOK();
}

unsigned __stdcall KTableExportDlg::TableExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 내보내는 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Exporting Table Data..."));
	}

	KTableExportDlg* pDlg = (KTableExportDlg*)pParameter->GetParameter();
	pDlg->ExportTable();

	return 0;
}

bool KTableExportDlg::ExportTable( void )
{
	TxLogDebugStartMsg();

	bool bExportResult = false;

	CString strFileName;
	m_editFile.GetWindowText( strFileName );

	UINT nWriteFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	UINT nCodePage = 949;

	CStdioFileEx csvFile;

	if( KEMCSVEncodingUTF8 == m_emCSVEncoding )
	{
		nCodePage = CP_UTF8;
	}
	else if( KEMCSVEncodingUTF16LE == m_emCSVEncoding)
	{
		nWriteFlags |= CStdioFileEx::modeWriteUnicode;
	}

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( strFileName, nWriteFlags ) == FALSE )
	{
		return false;
	}

	
	int nColumnCount = m_pExportColumns->ColumnCount();

	//transit export시, Node 선택시의 로직
	if (m_bSetDataType && m_cboDataType.GetItemData(m_cboDataType.GetCurSel()) == KEMBaseNode)
	{
		//write column header
		if(TRUE == m_bExportColumn)
		{
			CString strColumnLine = _T("");
			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pColumn = m_pExportColumns->GetColumn(i);
				if(0 < i)
				{
					strColumnLine += m_strSeparator;
				}
				strColumnLine += _T("\"");

				CString strColumnName = pColumn->Name();

				if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0)
				{
					strColumnLine += COLUMN_NODE_ID;
				}
				else
				{
					strColumnLine += pColumn->Caption();
				}
				
				strColumnLine += _T("\"");
			}

			strColumnLine += _T("\r\n");
			csvFile.WriteString(strColumnLine);
		}
		//write data
		ExportTransit2TxtTypeNode(csvFile);
	}
	else
	{
		//write Column header
		if(TRUE == m_bExportColumn)
		{
			CString strColumnLine = _T("");
			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pColumn = m_pExportColumns->GetColumn(i);
				if(0 < i)
				{
					strColumnLine += m_strSeparator;
				}
				strColumnLine += _T("\"");
				strColumnLine += pColumn->Caption();
				strColumnLine += _T("\"");
			}

			strColumnLine += _T("\r\n");
			csvFile.WriteString(strColumnLine);
		}

		ExportDB2Txt( csvFile );
	}

	csvFile.Close();

	TxLogDebugEndMsg();
	return true;
}


void KTableExportDlg::ExportDB2Txt( CStdioFileEx& of )
{
	//Generate Query
	CString strQuery;
	strQuery.Append(_T("Select "));

	int nColumnCount = m_pExportColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = m_pExportColumns->GetColumn(i);
		
		if(i != 0)
		{
			strQuery.Append(_T("||'"));
			strQuery.Append(m_strSeparator);
			strQuery.Append(_T("'||"));
		}

		if(KEMIODataTypeString == pColumn->DataType())
		{
			strQuery.Append(_T("'\"'||"));
			strQuery.Append(_T("IFNULL("));
			strQuery.Append(pColumn->Name());
			strQuery.Append(_T(", '')"));
			strQuery.Append(_T("||'\"'"));
		}
		else
		{
			strQuery.Append(_T("IFNULL("));
			strQuery.Append(pColumn->Name());
			strQuery.Append(_T(", '0')"));
		}
	}
	strQuery.Append(_T(" From "));
	strQuery.Append(m_pTable->Name());

	if (m_strFilter.Compare(_T("")) != 0)
	{
		strQuery.AppendFormat(_T(" Where %s "), m_strFilter);
	}

	TxLogDebug(strQuery.AllocSysString());

	//Write Text	
    KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();
	KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResultSet->Next())
	{
		CString strLine = spResultSet->GetValueString(0);
		strLine.Append(_T("\r\n"));
		of.WriteString(strLine);
	}
}


void KTableExportDlg::ExportTransit2TxtTypeNode( CStdioFileEx& of )
{
	CString strQuery(_T(""));
	std::map<Integer, TFTNode> mapLinkNodeInfo;

	strQuery = _T(" select link_id, fnode_id, tnode_id from link ");

	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();
	KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResultSet->Next())
	{
		TFTNode oFTNode = {};

		Integer nxLinkdID = spResultSet->GetValueInt64(0);
		oFTNode.fnode_id  = spResultSet->GetValueInt64(1);
		oFTNode.tnode_id  = spResultSet->GetValueInt64(2);

		mapLinkNodeInfo.insert(std::make_pair(nxLinkdID, oFTNode));
	}

	//쿼리 생성
	strQuery = _T(" SELECT ");
#pragma region 쿼리 생성
	int nColumnCount = m_pExportColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		if (i > 0)
		{
			strQuery.Append(_T(", "));
		}
		KIOColumn* pColumn = m_pExportColumns->GetColumn(i);
		CString strColName = pColumn->Name();

		strQuery.AppendFormat(_T("%s "), strColName);
	}
	strQuery.AppendFormat(_T(" FROM %s "), TABLE_TRANSITLINKS);

	if (m_strFilter.CompareNoCase(_T("")) != 0)
		strQuery.AppendFormat(_T(" Where %s"), m_strFilter);

	strQuery.Append(_T(" Order By transit_id, seq"));
#pragma endregion 쿼리 생성

	spResultSet = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResultSet->Next())
	{
		int nSeq(0);
		CString strWriteLine(_T(""));
		CString strWriteLineSeq1(_T(""));

		for (int i= 0; i< nColumnCount; i++)
		{
			if (0 != i)
			{
				strWriteLine.Append(m_strSeparator);
				strWriteLineSeq1.Append(m_strSeparator);
			}

			KIOColumn* pColumn     = m_pExportColumns->GetColumn(i);
			CString    strColName  = pColumn->Name();

			CString strData(_T(""));

			if (pColumn->DataType() == KEMIODataTypeDouble)
			{
				strData.Format(_T("%f"), spResultSet->GetValueDouble(i));
			}
			else if (pColumn->DataType() == KEMIODataTypeString)
			{
				strData.Format(_T("%s"), spResultSet->GetValueString(i));
			}
			else if (pColumn->DataType() == KEMIODataTypeInteger)
			{
				strData.Format(_T("%I64d"), spResultSet->GetValueInt64(i));
			}

			if (strColName.CompareNoCase(COLUMN_LINK_ID) == 0)
			{
				Integer nxLinkID = spResultSet->GetValueInt64(i);
				AutoType itFind  = mapLinkNodeInfo.find(nxLinkID);

				TFTNode oFTNode = {};
				if (itFind != mapLinkNodeInfo.end())
					oFTNode = itFind->second;

				CString strFNode(_T(""));
				CString strTNode(_T(""));

				strFNode.Format(_T("%I64d"), oFTNode.fnode_id);
				strTNode.Format(_T("%I64d"), oFTNode.tnode_id);

				strWriteLine.Append(strTNode);
				strWriteLineSeq1.Append(strFNode);
			}
			else if (strColName.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) == 0)
			{
				nSeq = spResultSet->GetValueInt(i);

				CString strDataNormal(_T(""));
				CString strDataSeq1(_T(""));
				if (1 == nSeq)
				{
					strDataSeq1.Format(_T("%d"), 1);
					strWriteLineSeq1.Append(strDataSeq1);
				}

				strDataNormal.Format(_T("%d"), (nSeq+1));
				strWriteLine.Append(strDataNormal);
			}
			else if (strColName.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) == 0)
			{
				int nStationYN = spResultSet->GetValueInt(i);

				if (nSeq == 1)
				{
					strWriteLineSeq1.AppendFormat(_T("%d"), 1);
					strWriteLine.AppendFormat(_T("%d"), nStationYN);
				}
				else
				{
					strWriteLine.AppendFormat(_T("%d"), nStationYN);
				}
			}
			else
			{
				strWriteLine.Append(strData);
				strWriteLineSeq1.Append(strData);
			}

// 			if (strColName.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) == 0)
// 			{
// 				nSeq = spResultSet->GetValueInt(i);
// 			}
		}
		
		if (1 == nSeq)
		{
			strWriteLineSeq1.Append(_T("\r\n"));
			of.WriteString(strWriteLineSeq1);
		}
		strWriteLine.Append(_T("\r\n"));
		of.WriteString(strWriteLine);
	}
}


void KTableExportDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		int nRecordCount = m_lstColumnList.GetItemCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			m_lstColumnList.SetCheck(i, bSelectAll);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTableExportDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV->uChanged & LVIF_STATE) // item state has been changed
	{
		switch(pNMLV->uNewState & LVIS_STATEIMAGEMASK)
		{
		case INDEXTOSTATEIMAGEMASK(BST_CHECKED + 1): // new state: checked
			;
			break;
		case INDEXTOSTATEIMAGEMASK(BST_UNCHECKED + 1): // new state: unchecked
			CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
			break; 
		}
	}

	*pResult = 0;
}

void KTableExportDlg::InitComboBaseDataType( void )
{
	m_cboDataType.ResetContent();

	int nIndex = m_cboDataType.InsertString(-1, _T("NodeBase"));
	m_cboDataType.SetItemData(nIndex, KEMBaseNode);

	nIndex = m_cboDataType.AddString(_T("LinkBase"));
	m_cboDataType.SetItemData(nIndex, KEMBaseLink);

	m_cboDataType.SetCurSel(0);
}

void KTableExportDlg::UpdateExportColumnInfo( void )
{
	m_lstColumnList.DeleteAllItems();

	BOOL bSelectAll(TRUE);

	const KIOColumns* pColumns = m_pTable->Columns();
	int nColumnCount = pColumns->ColumnCount();

	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn       = pColumns->GetColumn(i);
		CString    strColumnName = pColumn->Caption();
		CString    strColumnInfo = pColumn->ColumnInfo();
		CString    strDataType   = _T("");

		//Export Based를 Node, Link 따로 변경하는 경우에만 해당 로직을 탐
		int nIndex = m_cboDataType.GetCurSel();

		if (strColumnName.CompareNoCase(COLUMN_LINK_ID) == 0 && m_bSetDataType)
		{
			if (KEMBaseNode == m_cboDataType.GetItemData(nIndex))
			{
				strColumnName = _T("Node_ID");
				strColumnInfo = _T("Node_ID");
			}
		}

		if (KEMBaseNode == m_cboDataType.GetItemData(nIndex))
		{
			if (_tcsicmp(m_pTable->Name(), TABLE_TRANSITLINKS) == 0)
			{
				CString strColumn = pColumn->Name();
				if (strColumn.CompareNoCase(COLUMN_TRANSIT_ID) != 0 && strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_ID) != 0 
					&& strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_SEQ) != 0 && strColumn.CompareNoCase(COLUMN_TRANSIT_LINK_STATION_YN) != 0)
				{
					continue;
				}
			}
		}
		
		switch(pColumn->DataType())
		{
		case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
		case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE; break;
		case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING; break;
		}

		BOOL bCheck(FALSE);
		if (KEMIOColumnTypeSystem == pColumn->ColumnType())
		{
			bCheck = TRUE;
		}
		else
		{
			bCheck     = FALSE;
			bSelectAll = FALSE;
		}

		m_lstColumnList.InsertItem (i, strColumnName);
		m_lstColumnList.SetItemText(i, 1, strDataType);
		m_lstColumnList.SetItemText(i, 2, strColumnInfo);
		m_lstColumnList.SetCheck   (i, bCheck);
		m_lstColumnList.SetItemData(i, (DWORD_PTR)pColumn);
	}

	if (TRUE == bSelectAll)
	{
		CheckDlgButton(IDC_CHECK3, BST_CHECKED);
	}
	else
	{
		CheckDlgButton(IDC_CHECK3, BST_UNCHECKED);
	}

	for( int i = 0; i < 4; ++i )
	{
		m_lstColumnList.SetColumnWidth( i, LVSCW_AUTOSIZE );
		int nColumnWidth = m_lstColumnList.GetColumnWidth(i);
		m_lstColumnList.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = m_lstColumnList.GetColumnWidth(i); 
		m_lstColumnList.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
	}
}

void KTableExportDlg::OnSelchangeComboBaseDataType()
{
	UpdateExportColumnInfo();
}