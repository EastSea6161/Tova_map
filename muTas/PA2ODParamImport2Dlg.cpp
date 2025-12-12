#include "StdAfx.h"
#include "afxdialogex.h"
#include "PA2ODParamImport2Dlg.h"

//#include "StdioFileEx.h"
#include "PA2ODInfo.h"
#include "Purpose.h"
#include "ImChampDir.h"

/**
 * @brief CXTPReportControl에서 Combobox 표출
 * @remark
 * @warning
 * @version 1.0
 * @author 
 * @date 2010.08.23
 */


class KColumnReportRecordItem : public CXTPReportRecordItem
{
public :
	KColumnReportRecordItem( int nIndex ) : m_nIndex( nIndex )
	{

	}

	CString GetCaption(CXTPReportColumn* pColumn)
	{
		CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint( m_nIndex );
		ASSERT(pConstraint);
		return pConstraint->m_strConstraint;
	}

	int getColumnIndex( void )
	{
		return m_nIndex;
	}

	DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
	{
		return m_nIndex;
	}

	virtual void OnConstraintChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, 
		CXTPReportRecordItemConstraint* pConstraint )
	{
		if( pConstraint )
		{
			m_nIndex = pConstraint->m_dwData;
		}
	}
	int m_nIndex;
};

IMPLEMENT_DYNAMIC(KPA2ODParamImport2Dlg, KDialogEx)

KPA2ODParamImport2Dlg::KPA2ODParamImport2Dlg(
	BOOL a_bContainHeader, KEMCSVEncoding a_emCSVEncoding, TCHAR a_cSeparator,
	CString a_strFileName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KPA2ODParamImport2Dlg::IDD, pParent)
	, m_bContainHeader(a_bContainHeader)
	, m_emCSVEncoding(a_emCSVEncoding)
	, m_cSeparator(a_cSeparator)
	, m_strFileName(a_strFileName)
	, m_nCurrentHeader(0)
	, m_bGoBack(false)
{
}


KPA2ODParamImport2Dlg::~KPA2ODParamImport2Dlg(void)
{
}

void KPA2ODParamImport2Dlg::DoDataExchange( CDataExchange* pDX )
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndZoneReport);
	DDX_Control(pDX, IDC_REPORT1, m_wndHBReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndNHBReport);
	DDX_Control(pDX, IDC_REPORT4, m_wndPreviewReport);
}

BEGIN_MESSAGE_MAP(KPA2ODParamImport2Dlg, KDialogEx)
	ON_BN_CLICKED(ID_BACK, &KPA2ODParamImport2Dlg::OnBnClickedBack)
	ON_BN_CLICKED(IDOK, &KPA2ODParamImport2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()

bool KPA2ODParamImport2Dlg::GoBack( void )
{
	return m_bGoBack;
}

BOOL KPA2ODParamImport2Dlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		GenerateHeaderColumn();

		PreviewDataLoad();

		KReportCtrlSetting::Default(m_wndZoneReport, TRUE, FALSE);
		KReportCtrlSetting::AutoColumnSizeFalse(m_wndZoneReport);
		KReportCtrlSetting::Default(m_wndHBReport, TRUE, FALSE);
		KReportCtrlSetting::Default(m_wndNHBReport, TRUE, FALSE);
		KReportCtrlSetting::Default(m_wndPreviewReport, FALSE, FALSE, FALSE);
		KReportCtrlSetting::AutoColumnSizeFalse(m_wndPreviewReport);

		InitPreviewReport();
		UpdatePreviewReport();

		InitZoneReport();
		UpdateZoneReport();

		InitHBReport();
		UpdateHBReport();

		InitNHBReport();
		UpdateNHBReport();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPA2ODParamImport2Dlg::OnBnClickedBack()
{
	m_bGoBack = true;
	KDialogEx::OnOK();
}

void KPA2ODParamImport2Dlg::GenerateHeaderColumn( void )
{
	TxLogDebugStartMsg();
	m_vecColumnHeader.clear();
	CString strReadLine;

	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	CStdioFileEx csvFile;

	if( KEMCSVEncodingUTF8 == m_emCSVEncoding )
	{
		nCodePage = CP_UTF8;
	}
	else if( KEMCSVEncodingUnknown == m_emCSVEncoding )
	{
		ThrowException(_T("unknown encoding type..."));
	}

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strFileName, nOpenMode ) == FALSE )
		ThrowException(_T("Cannot open file..."));

	csvFile.ReadString(strReadLine);
	ParseCSVLineString(strReadLine, m_vecColumnHeader);

	if (!m_bContainHeader)
	{
		int nColumnIndex = 0;
		int nColumnCount = m_vecColumnHeader.size();
		m_vecColumnHeader.clear();

		for (int i = 0; i < nColumnCount; ++i)
		{
			CString strColumnName(_T(""));
			strColumnName.Format(_T("Column %d"), ++nColumnIndex);
			m_vecColumnHeader.push_back( strColumnName );
		}
	}
	csvFile.Close();
	TxLogDebugEndMsg();
}

void KPA2ODParamImport2Dlg::ParseCSVLineString( CString& strLine, std::vector<CString>& row )
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

void KPA2ODParamImport2Dlg::HomeBasedInfo( std::list<KPA2ODInfo*> a_lstHomeBasedPA2ODInfo )
{
	m_lstHomeBasedPA2ODInfo = a_lstHomeBasedPA2ODInfo;
}


void KPA2ODParamImport2Dlg::NotHBasedInfo( std::list<KPA2ODInfo*> a_lstNotHBasedPA2ODInfo )
{
	m_lstNotHBasedPA2ODInfo = a_lstNotHBasedPA2ODInfo;
}

void KPA2ODParamImport2Dlg::InitPreviewReport( void )
{
	std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	CXTPReportColumn* pCXTPColumn  = NULL;
	int 			  nColumnIndex = 0;

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn = m_wndPreviewReport.AddColumn(new CXTPReportColumn(nColumnIndex++, *iter, 70));
		pCXTPColumn->SetHeaderAlignment(DT_CENTER);
		pCXTPColumn->SetAlignment(DT_RIGHT);
	}
}

void KPA2ODParamImport2Dlg::UpdatePreviewReport( void )
{
	std::vector<std::vector<CString>>::iterator iter, end = m_vecPreviewVecData.end();

	for (iter = m_vecPreviewVecData.begin(); iter != end; ++iter)
	{
		CXTPReportRecord*     pCXTPRecord	 = m_wndPreviewReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem		 = NULL;
		std::vector<CString>  vecPreviewData = *iter;

		std::vector<CString>::iterator	innerIter, innerEnd = vecPreviewData.end();

		for (innerIter = vecPreviewData.begin(); innerIter != innerEnd; ++innerIter)
		{
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(*innerIter));
		}
	}
	m_wndPreviewReport.Populate();
}

void KPA2ODParamImport2Dlg::PreviewDataLoad( void )
{
	TxLogDebugStartMsg();
	bool bResult = true;

	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	CStdioFileEx csvFile;

	if( KEMCSVEncodingUTF8 == m_emCSVEncoding )
	{
		nCodePage = CP_UTF8;
	}
	else if( KEMCSVEncodingUnknown == m_emCSVEncoding )
	{
		ThrowException(_T("unknown encoding type..."));
	}

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strFileName, nOpenMode ) == FALSE )
		ThrowException(_T("Cannot open file..."));

	CString strReadLine(_T(""));
	m_vecPreviewVecData.clear();

	if (m_bContainHeader)
		csvFile.ReadString(strReadLine);
	int nloopCount(0);

	while (csvFile.ReadString(strReadLine) && nloopCount++ < 100)
	{
		std::vector<CString> vecPreviewData;
		ParseCSVLineString(strReadLine, vecPreviewData);

		m_vecPreviewVecData.push_back(vecPreviewData);
	}
	csvFile.Close();
	TxLogDebugEndMsg();
}

void KPA2ODParamImport2Dlg::InitZoneReport( void )
{
	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = m_wndZoneReport.AddColumn( new CXTPReportColumn( 0, _T("Field"), 120) );
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	int nComboIndex = 0;

	pCXTPColumn = m_wndZoneReport.AddColumn( new CXTPReportColumn( 1, _T("Import"), 120) );
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);
}

void KPA2ODParamImport2Dlg::UpdateZoneReport( void )
{
	std::vector<CString> vecZone;

	vecZone.push_back(_T("Orgin"));
	vecZone.push_back(_T("Dest"));

	int nColumnCount = vecZone.size();
	int nCSVIndex    = 0;

	for (int i = 0; i < nColumnCount; ++i)
	{
		CString strColumnName = vecZone[i];

		CXTPReportRecord* 	  pCXTPRecord = m_wndZoneReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem	  = NULL;

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strColumnName));

		if( m_nCurrentHeader < (int)m_vecColumnHeader.size() )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( ++m_nCurrentHeader ));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
	}

	m_wndZoneReport.Populate();
}

void KPA2ODParamImport2Dlg::InitHBReport( void )
{
	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = m_wndHBReport.AddColumn( new CXTPReportColumn( 0, _T("Field"), 34) );
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	int nComboIndex = 0;

	pCXTPColumn = m_wndHBReport.AddColumn( new CXTPReportColumn( 1, _T("Go"), 33) );
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);

	nComboIndex = 0;

	pCXTPColumn = m_wndHBReport.AddColumn( new CXTPReportColumn( 2, _T("Return"), 33) );
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	//std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);

}

void KPA2ODParamImport2Dlg::UpdateHBReport( void )
{
	std::list<KPA2ODInfo*>::iterator iter, end = m_lstHomeBasedPA2ODInfo.end();

	for (iter = m_lstHomeBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pPurpose 		= pPA2ODInfo->Purpose();
		CString   strColumnName = pPurpose->PurposeName();

		CXTPReportRecord* 	  pCXTPRecord = m_wndHBReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem	  = NULL;

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strColumnName));

		//Go
		if( m_nCurrentHeader < (int)m_vecColumnHeader.size() )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( ++m_nCurrentHeader ));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
		//Return
		if( m_nCurrentHeader < (int)m_vecColumnHeader.size() )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( ++m_nCurrentHeader ));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
	}
	m_wndHBReport.Populate();
}

void KPA2ODParamImport2Dlg::InitNHBReport( void )
{
	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = m_wndNHBReport.AddColumn( new CXTPReportColumn( 0, _T("Field"), 34) );
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	int nComboIndex = 0;

	pCXTPColumn = m_wndNHBReport.AddColumn( new CXTPReportColumn( 1, _T("Go"), 33) );
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);

	nComboIndex = 0;

	pCXTPColumn = m_wndNHBReport.AddColumn( new CXTPReportColumn( 2, _T("Return"), 33) );
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	//std::vector<CString>::iterator iter, end = m_vecColumnHeader.end();

	for (iter = m_vecColumnHeader.begin(); iter != end; ++iter)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);
}

void KPA2ODParamImport2Dlg::UpdateNHBReport( void )
{
	std::list<KPA2ODInfo*>::iterator iter, end = m_lstNotHBasedPA2ODInfo.end();

	for (iter = m_lstNotHBasedPA2ODInfo.begin(); iter != end; ++iter)
	{
		KPA2ODInfo* pPA2ODInfo = *iter;

		if (!pPA2ODInfo->Selected())
			continue;

		KPurpose* pPurpose 		= pPA2ODInfo->Purpose();
		CString   strColumnName = pPurpose->PurposeName();

		CXTPReportRecord* 	  pCXTPRecord = m_wndNHBReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pCXTPItem	  = NULL;

		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strColumnName));
		//Go
		if( m_nCurrentHeader < (int)m_vecColumnHeader.size() )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( ++m_nCurrentHeader ));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
		//Return
		if( m_nCurrentHeader < (int)m_vecColumnHeader.size() )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( ++m_nCurrentHeader ));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
	}
	m_wndNHBReport.Populate();
}

void KPA2ODParamImport2Dlg::OnBnClickedOk()
{
	try
	{
		CWaitCursor cw;
		WriteBinaryFile();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
		return;
	}

	KDialogEx::OnOK();
}

void KPA2ODParamImport2Dlg::WriteBinaryFile( void )
{
	ImChampDir::SetCurrentDirectoryToApp();

	CString strPAInputFile = _T("Parameter.dat");

	TxLogDebugStartMsg();
	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;

	if( KEMCSVEncodingUTF8 == m_emCSVEncoding )
	{
		nCodePage = CP_UTF8;
	}
	else if( KEMCSVEncodingUnknown == m_emCSVEncoding )
	{
		ThrowException(_T("Unknown encoding"));
	}
	remove((CStringA)strPAInputFile);
	std::ofstream ofs(strPAInputFile, std::ios::binary);

	CString strReadLine(_T(""));
	int		nCurrentLine(0);

	CStdioFileEx csvFile;
	csvFile.SetCodePage(nCodePage);

	if (!csvFile.Open(m_strFileName, nOpenMode))
		ThrowException(_T("File open failed"));

	try
	{
		if (m_bContainHeader)
		{
			++nCurrentLine;
			csvFile.ReadString(strReadLine);
		}

		while (csvFile.ReadString(strReadLine))
		{
			++nCurrentLine;

			std::vector<CString> vecLineData;
			ParseCSVLineString(strReadLine, vecLineData);
			LineData2Binary(vecLineData, ofs);
		}
		ofs.close();
	}
	catch (KExceptionPtr ex)
	{
		ofs.close();
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch (...)
	{
		ofs.close();
		TxLogDebugException();
		AfxMessageBox(_T("데이터 불러오기에 실패하였습니다."));
		return;
	}
	TxLogDebugEndMsg();
}

void KPA2ODParamImport2Dlg::LineData2Binary( std::vector<CString> a_vecLineData, std::ofstream& ofs )
{
	__int64 nxValue;
	double  dValue;
	//WriteZone
	CXTPReportRecords*	pCXTPZoneRecords = m_wndZoneReport.GetRecords();
	int					nRecordCount     = pCXTPZoneRecords->GetCount();
	CString             strValue;

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*        pCXTPRecord  = pCXTPZoneRecords->GetAt(i);
		KColumnReportRecordItem* pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(1);

		if ( 0 != pKColumnItem->getColumnIndex())
		{
			strValue = a_vecLineData[pKColumnItem->getColumnIndex() - 1];
		}
		else
		{
			strValue = _T("0");
		}

		nxValue = _ttoi64(strValue);
		ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(__int64));
	}

	//WriteHomeBased
	CXTPReportRecords* pCXTPHomeRecords = m_wndHBReport.GetRecords();
	nRecordCount = pCXTPHomeRecords->GetCount();
	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*        pCXTPRecord  = pCXTPHomeRecords->GetAt(i);
		KColumnReportRecordItem* pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(1);

		if ( 0 != pKColumnItem->getColumnIndex())
		{
			strValue = a_vecLineData[pKColumnItem->getColumnIndex() - 1];
		}
		else
		{
			strValue = _T("0");
		}

		dValue = _ttof(strValue);
		ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));

		pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(2);

		if ( 0 != pKColumnItem->getColumnIndex())
		{
			strValue = a_vecLineData[pKColumnItem->getColumnIndex() - 1];
		}
		else
		{
			strValue = _T("0");
		}

		dValue = _ttof(strValue);
		ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
	}
	//WriteNotHBased
	CXTPReportRecords* pCXTPNotHRecords = m_wndNHBReport.GetRecords();
	nRecordCount = pCXTPNotHRecords->GetCount();
	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*        pCXTPRecord  = pCXTPNotHRecords->GetAt(i);
		KColumnReportRecordItem* pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(1);

		if ( 0 != pKColumnItem->getColumnIndex())
		{
			strValue = a_vecLineData[pKColumnItem->getColumnIndex() - 1];
		}
		else
		{
			strValue = _T("0");
		}

		dValue = _ttof(strValue);
		ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));

		pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(2);

		if ( 0 != pKColumnItem->getColumnIndex())
		{
			strValue = a_vecLineData[pKColumnItem->getColumnIndex() - 1];
		}
		else
		{
			strValue = _T("0");
		}

		dValue = _ttof(strValue);
		ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
	}
}
