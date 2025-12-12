// DevPlanImportDirectInputStep2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DevPlanImportDirectInputStep2Dlg.h"
#include "afxdialogex.h"

//#include "StdioFileEx.h"

class KColumnReportRecordItem : public CXTPReportRecordItem
{
public :
	KColumnReportRecordItem( int nIndex ) : m_nIndex( nIndex )
	{
	}

	CString GetCaption(CXTPReportColumn* pColumn)
	{
		CXTPReportRecordItemConstraint* pConstraint = 
			pColumn->GetEditOptions()->FindConstraint( m_nIndex );
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
			m_nIndex = pConstraint->m_dwData;
	}
	int m_nIndex;
};
// KDevPlanImportDirectInputStep2Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDevPlanImportDirectInputStep2Dlg, KDialogEx)

KDevPlanImportDirectInputStep2Dlg::KDevPlanImportDirectInputStep2Dlg(
	BOOL a_bContainHeader, KEMCSVEncoding a_emCSVEncoding, TCHAR a_cSeparator,
	CString a_strFileName, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDevPlanImportDirectInputStep2Dlg::IDD, pParent)
	, m_emCSVEncoding(a_emCSVEncoding)
	, m_bContainHeader(a_bContainHeader)
	, m_strFileName(a_strFileName)
	, m_cSeparator(a_cSeparator)
	, m_bGoBack(false)
	, m_bRunOK(false)
{
}

KDevPlanImportDirectInputStep2Dlg::~KDevPlanImportDirectInputStep2Dlg()
{
}

void KDevPlanImportDirectInputStep2Dlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndDefineReport);
	DDX_Control(pDX, IDC_REPORT1, m_wndPreviewReport);
}


BEGIN_MESSAGE_MAP(KDevPlanImportDirectInputStep2Dlg, KDialogEx)
	ON_BN_CLICKED(ID_BACK, &KDevPlanImportDirectInputStep2Dlg::OnBnClickedBack)
	ON_BN_CLICKED(IDOK, &KDevPlanImportDirectInputStep2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KDevPlanImportDirectInputStep2Dlg 메시지 처리기입니다.

bool KDevPlanImportDirectInputStep2Dlg::GoBack()
{
	return m_bGoBack;
}

BOOL KDevPlanImportDirectInputStep2Dlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	bool bResult;
	bResult = GenerateHeaderColumn();

	if(!bResult)
	{
		AfxMessageBox(_T("파일 로딩에 실패하였습니다."));
		OnBnClickedBack();
	}
	bResult = PreviewDataLoad();

	if(!bResult)
	{
		AfxMessageBox(_T("파일 로딩에 실패하였습니다."));
		OnBnClickedBack();
	}

	InitDefineReport();
	UpdateDefineReport();

	InitPreviewReport();
	UpdatePreviewReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool KDevPlanImportDirectInputStep2Dlg::GenerateHeaderColumn( void )
{
	TxLogDebugStartMsg();
	bool bResult = true;
	m_vecColumn.clear();

	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	CStdioFileEx csvFile;

	if( KEMCSVEncodingUTF8 == m_emCSVEncoding )
	{
		nCodePage = CP_UTF8;
	}
	else if( KEMCSVEncodingUnknown == m_emCSVEncoding )
	{
		bResult = false;
	}

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strFileName, nOpenMode ) == FALSE )
	{
		bResult = false;
	}

	CString strReadLine(_T(""));
	csvFile.ReadString( strReadLine );
	ParseCSVLineString( strReadLine, m_vecColumn );

	if (!m_bContainHeader)
	{
		int nColumnIndex(0);
		int nColumnCount = m_vecColumn.size();

		m_vecColumn.clear();

		for (int i = 0; i < nColumnCount; ++i)
		{
			CString strColumnName(_T(""));
			strColumnName.Format(_T("Column %d"), ++nColumnIndex);
			m_vecColumn.push_back( strColumnName );
		}
	}
	csvFile.Close();
	TxLogDebugEndMsg();
	return bResult;
}

void KDevPlanImportDirectInputStep2Dlg::ParseCSVLineString( CString& strLine, std::vector<CString>& row )
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

void KDevPlanImportDirectInputStep2Dlg::InitDefineReport( void )
{
	m_wndDefineReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndDefineReport.GetReportHeader()->AllowColumnReorder(FALSE);
	//m_wndDefineReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
	m_wndDefineReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndDefineReport.SetGridStyle(TRUE, xtpReportGridSmallDots);
	m_wndDefineReport.AllowEdit( TRUE );

	CXTPReportColumn* pCXTPColumn = NULL;

	pCXTPColumn = m_wndDefineReport.AddColumn( new CXTPReportColumn( 0, _T("Field"), 1));
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->SetAlignment(DT_CENTER);

	int nComboIndex(0);
	pCXTPColumn = m_wndDefineReport.AddColumn( new CXTPReportColumn( 1, _T("Import"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	std::vector<CString>::iterator iter = m_vecColumn.begin();
	std::vector<CString>::iterator end	= m_vecColumn.end();

	while (iter != end)
	{
		pCXTPColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
		++iter;
	}
	pCXTPColumn->SetAlignment(DT_CENTER);
	pCXTPColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pCXTPColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pCXTPColumn->GetEditOptions()->AddComboButton(TRUE);
}

void KDevPlanImportDirectInputStep2Dlg::UpdateDefineReport( void )
{
	CXTPReportRecord*		pCXTPRecord = NULL;
	CXTPReportRecordItem*	pCXTPItem	= NULL;
	
	//PlanZoneID
	pCXTPRecord = m_wndDefineReport.AddRecord(new CXTPReportRecord);
	pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("Plan Zone ID")));
	
	if ((int)m_vecColumn.size() < 1)
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
	}
	else
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( 1 ));
	}
	
	//From Zone ID
	pCXTPRecord = m_wndDefineReport.AddRecord(new CXTPReportRecord);
	pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("From Zone ID")));

	if ((int)m_vecColumn.size() < 2)
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
	}
	else
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( 2 ));
	}

	//Migration Rate(%)
	pCXTPRecord = m_wndDefineReport.AddRecord(new CXTPReportRecord);
	pCXTPItem   = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("Rate(%)")));

	if ((int)m_vecColumn.size() < 3)
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
	}
	else
	{
		pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( 3 ));
	}

	
	m_wndDefineReport.GetReportHeader()->AllowColumnRemove( FALSE );
	m_wndDefineReport.FocusSubItems( TRUE );
	m_wndDefineReport.Populate();
}

void KDevPlanImportDirectInputStep2Dlg::OnBnClickedBack()
{
	m_bGoBack = true;
	KDialogEx::OnOK();
}

void KDevPlanImportDirectInputStep2Dlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkey(this, _T("Import Data..."), TRUE, FALSE, FALSE, nLang);
		WriteBinaryFile(&oDonkey);
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

void KDevPlanImportDirectInputStep2Dlg::InitPreviewReport( void )
{
	m_wndPreviewReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndPreviewReport.GetReportHeader()->AllowColumnRemove(FALSE);
	//m_wndPreviewReport.GetReportHeader()->SetAutoColumnSizing(FALSE);
	m_wndPreviewReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndPreviewReport.SetGridStyle(TRUE, xtpReportGridSmallDots);
	m_wndPreviewReport.AllowEdit(TRUE);

	std::vector<CString>::iterator iter = m_vecColumn.begin();
	std::vector<CString>::iterator end	= m_vecColumn.end();

	CXTPReportColumn* pCXTPColumn = NULL;
	int nColumnIndex(0);

	while (iter != end)
	{
		pCXTPColumn = m_wndPreviewReport.AddColumn(new CXTPReportColumn(nColumnIndex++, *iter, 1));
		pCXTPColumn->SetHeaderAlignment(DT_CENTER);
		pCXTPColumn->SetAlignment(DT_RIGHT);
		
		++iter;
	}
}

void KDevPlanImportDirectInputStep2Dlg::UpdatePreviewReport( void )
{
	std::vector<std::vector<CString>>::iterator iter = m_vecPreviewVecData.begin();
	std::vector<std::vector<CString>>::iterator end	= m_vecPreviewVecData.end();

	while (iter != end)
	{
		CXTPReportRecord*		pCXTPRecord		= m_wndPreviewReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pCXTPItem		= NULL;
		std::vector<CString>	vecPreviewData	= *iter;
		
		std::vector<CString>::iterator	innerIter	= vecPreviewData.begin();
		std::vector<CString>::iterator	innerEnd	= vecPreviewData.end();

		while (innerIter != innerEnd)
		{
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(*innerIter));
			++innerIter;
		}
		++iter;
	}
	m_wndPreviewReport.Populate();
}

bool KDevPlanImportDirectInputStep2Dlg::PreviewDataLoad( void )
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
		bResult = false;
	}

	csvFile.SetCodePage( nCodePage );

	if( csvFile.Open( m_strFileName, nOpenMode ) == FALSE )
	{
		bResult = false;
	}

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
	return bResult;
}

void KDevPlanImportDirectInputStep2Dlg::WriteBinaryFile( QBicDonkeyProgress* pDonkey )
{
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
		
	CString strReadLine(_T(""));
	int		nCurrentLine(0);

	//remove((CStringA)_T(""));
	std::ofstream ofs(_T("ZoneParameter.dat"), std::ios::binary);
	
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

			if (nCurrentLine % 10000 == 0 && pDonkey)
			{
				pDonkey->PeekAndPump();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		CString strErrorMsg;
		strErrorMsg.Format(_T("%d Line Error"), nCurrentLine);
		//remove((CStringA)_T(""));

		ThrowException(strErrorMsg);
	}
		
	csvFile.Close();
	ofs.close();
	TxLogDebugEndMsg();
}

void KDevPlanImportDirectInputStep2Dlg::LineData2Binary( std::vector<CString> a_vecLineData, std::ofstream& ofs )
{
	CXTPReportRecords*	pCXTPRecords = m_wndDefineReport.GetRecords();
	int					nRecordCount = pCXTPRecords->GetCount();

	__int64 nxValue;
	double	dValue;

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*           pCXTPRecord  = pCXTPRecords->GetAt(i);
		KColumnReportRecordItem*	pKColumnItem = (KColumnReportRecordItem*)pCXTPRecord->GetItem(1);
		CString						strValue     = a_vecLineData[pKColumnItem->getColumnIndex() - 1];

		if (0 == i)
		{
			nxValue = _ttoi64(strValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(__int64));
		}
		else if (1 == i)
		{
			nxValue = _ttoi64(strValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(__int64));
		}
		else if (2 == i)
		{
			dValue	= _ttof(strValue);
			ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
		}
	}
}
