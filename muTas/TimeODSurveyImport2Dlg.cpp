// TimeODSurveyImport2Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TimeODSurveyImport2Dlg.h"
#include "timeod_common.h"
#include "afxdialogex.h"

//#include "StdioFileEx.h"


const CString BIN_FILE_NAME = _T("Persontrip.dat");

// KTimeODSurveyImport2Dlg 대화 상자입니다.
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

IMPLEMENT_DYNAMIC(KTimeODSurveyImport2Dlg, CXTResizeDialog)

KTimeODSurveyImport2Dlg::KTimeODSurveyImport2Dlg(
	BOOL a_bContainHeader, KEMCSVEncoding a_emCSVEncoding, TCHAR a_cSeparator,
	CString a_strFileName, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(KTimeODSurveyImport2Dlg::IDD, pParent)
	, m_emCSVEncoding(a_emCSVEncoding)
	, m_bContainHeader(a_bContainHeader)
	, m_strFileName(a_strFileName)
	, m_cSeparator(a_cSeparator)
	, m_bGoBack(false)
	, m_bRunOK(false)
{
}

KTimeODSurveyImport2Dlg::~KTimeODSurveyImport2Dlg()
{
}

void KTimeODSurveyImport2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndDefineReport);
	DDX_Control(pDX, IDC_REPORT1, m_wndPreviewReport);
}


BEGIN_MESSAGE_MAP(KTimeODSurveyImport2Dlg, CXTResizeDialog)
	ON_BN_CLICKED(ID_BACK, &KTimeODSurveyImport2Dlg::OnBnClickedBack)
	ON_BN_CLICKED(IDOK, &KTimeODSurveyImport2Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KTimeODSurveyImport2Dlg 메시지 처리기입니다.

bool KTimeODSurveyImport2Dlg::GoBack()
{
	return m_bGoBack;
}

BOOL KTimeODSurveyImport2Dlg::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetResize(IDC_REPORT,	SZ_TOP_LEFT,	SZ_TOP_RIGHT);
	SetResize(IDC_REPORT1,	SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);
	
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(ID_BACK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

	m_setPurpose.clear();
	m_setMode.clear();
	
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

	KReportCtrlSetting::Default(m_wndDefineReport, TRUE, FALSE, FALSE);
	InitDefineReport();
	UpdateDefineReport();

	KReportCtrlSetting::DefaultE(m_wndPreviewReport);
	InitPreviewReport();
	UpdatePreviewReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

bool KTimeODSurveyImport2Dlg::GenerateHeaderColumn( void )
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

void KTimeODSurveyImport2Dlg::ParseCSVLineString( CString& strLine, std::vector<CString>& row )
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

void KTimeODSurveyImport2Dlg::InitDefineReport( void )
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndDefineReport.AddColumn( new CXTPReportColumn( 0, _T("Field"), 120) );
	pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 컬럼"));
    }	

	pColumn = m_wndDefineReport.AddColumn( new CXTPReportColumn( 1, _T("Data Type"), 120) );
	pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("데이터 유형"));
    }	

	int nComboIndex(0);
	pColumn = m_wndDefineReport.AddColumn( new CXTPReportColumn( 2, _T("Import"), 120) );
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("파일 컬럼"));
    }	

	pColumn->GetEditOptions()->AddConstraint(_T("Null"), nComboIndex++);

	std::vector<CString>::iterator iter = m_vecColumn.begin();
	std::vector<CString>::iterator end	= m_vecColumn.end();

	while (iter != end)
	{
		pColumn->GetEditOptions()->AddConstraint(*iter, nComboIndex++);
		++iter;
	}
	pColumn->SetAlignment(DT_CENTER);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pColumn->GetEditOptions()->AddComboButton(TRUE);
}

void KTimeODSurveyImport2Dlg::UpdateDefineReport( void )
{
	std::vector<CString>::iterator iter = m_vecColumn.begin();
	std::vector<CString>::iterator end	= m_vecColumn.end();
	
	std::vector<int> vecDefaultIndex;
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_PersonalID);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_Oringin);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_Dest);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_Purpose);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_Mode);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_StartHour);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_StartMin);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_EndHour);
	vecDefaultIndex.push_back(KEMDefaultColumnIndex_EndMin);

	std::vector<CString> vecDefaultColumn;
	vecDefaultColumn.push_back(_T("Personal ID"));
	vecDefaultColumn.push_back(_T("Origin"));
	vecDefaultColumn.push_back(_T("Dest"));
	vecDefaultColumn.push_back(_T("Purpose"));
	vecDefaultColumn.push_back(_T("Mode"));
	vecDefaultColumn.push_back(_T("Start Hour"));
	vecDefaultColumn.push_back(_T("Start Min"));
	vecDefaultColumn.push_back(_T("End Hour"));
	vecDefaultColumn.push_back(_T("End Min"));

	std::vector<KEMSurveyImportDataType> vecDefaultType;
	vecDefaultType.push_back(KEMSurveyImportInt64);
	vecDefaultType.push_back(KEMSurveyImportInt64);
	vecDefaultType.push_back(KEMSurveyImportInt64);
	vecDefaultType.push_back(KEMSurveyImportInteger);
	vecDefaultType.push_back(KEMSurveyImportInteger);
	vecDefaultType.push_back(KEMSurveyImportInteger);
	vecDefaultType.push_back(KEMSurveyImportInteger);
	vecDefaultType.push_back(KEMSurveyImportInteger);
	vecDefaultType.push_back(KEMSurveyImportInteger);

	int nDefaultColumnCount = vecDefaultColumn.size();
	int nCSVIndex(0);

	for (int i = 0; i < nDefaultColumnCount; ++i)
	{
		CXTPReportRecord*		pCXTPRecord = m_wndDefineReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pCXTPItem	= NULL;
		
		KEMSurveyImportDataType emSurveyImportDataType = vecDefaultType[i];
		CString strFieldName = vecDefaultColumn[i];
		
		pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(strFieldName));
		pCXTPItem->SetItemData((DWORD_PTR)emSurveyImportDataType);

		if (emSurveyImportDataType == KEMSurveyImportInt64)
		{
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("Integer64")));
		}
		else if (emSurveyImportDataType == KEMSurveyImportDouble)
		{
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("Double")));
		}
		else if (emSurveyImportDataType == KEMSurveyImportInteger)
		{
			pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText(_T("Integer")));
		}
		else
		{
			ThrowException(_T("Error"));
		}
		
		/*if (iter != end)
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(++nCSVIndex));
			++iter;
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}*/

		if( (int)m_vecColumn.size() < vecDefaultIndex[i] )
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem(0));
		}
		else
		{
			pCXTPItem = pCXTPRecord->AddItem(new KColumnReportRecordItem( vecDefaultIndex[i] ));
		}
	}

	m_wndDefineReport.GetReportHeader()->AllowColumnRemove( FALSE );
	m_wndDefineReport.FocusSubItems( TRUE );
	m_wndDefineReport.Populate();
}

void KTimeODSurveyImport2Dlg::OnBnClickedBack()
{
	m_bGoBack = true;
	CXTResizeDialog::OnOK();
}

void KTimeODSurveyImport2Dlg::OnBnClickedOk()
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
	CXTResizeDialog::OnOK();
}

void KTimeODSurveyImport2Dlg::InitPreviewReport( void )
{
	std::vector<CString>::iterator iter = m_vecColumn.begin();
	std::vector<CString>::iterator end	= m_vecColumn.end();

	CXTPReportColumn* pCXTPColumn = NULL;
	int nColumnIndex(0);

	while (iter != end)
	{
		pCXTPColumn = m_wndPreviewReport.AddColumn(new CXTPReportColumn(nColumnIndex++, *iter, 70));
		pCXTPColumn->SetHeaderAlignment(DT_CENTER);
		pCXTPColumn->SetAlignment(DT_RIGHT);
		
		++iter;
	}
}

void KTimeODSurveyImport2Dlg::UpdatePreviewReport( void )
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

bool KTimeODSurveyImport2Dlg::PreviewDataLoad( void )
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

void KTimeODSurveyImport2Dlg::WriteBinaryFile( QBicDonkeyProgress* pDonkey )
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

	remove((CStringA)BIN_FILE_NAME);
	std::ofstream ofs(BIN_FILE_NAME, std::ios::binary);
	
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
		remove((CStringA)BIN_FILE_NAME);

		ThrowException(strErrorMsg);
	}
		
	csvFile.Close();
	ofs.close();
	TxLogDebugEndMsg();
}

void KTimeODSurveyImport2Dlg::LineData2Binary( std::vector<CString> a_vecLineData, std::ofstream& ofs )
{
	CXTPReportRecords*	pCXTPRecords = m_wndDefineReport.GetRecords();
	int					nRecordCount = pCXTPRecords->GetCount();

	__int64 nxValue;
	double	dValue;
	int		nValue;

	for (int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pCXTPRecord = pCXTPRecords->GetAt(i);
		CXTPReportRecordItemText* pCXTPItemText = (CXTPReportRecordItemText*)pCXTPRecord->GetItem(0);

		KEMSurveyImportDataType emSurveyImportDataType = 
			(KEMSurveyImportDataType)pCXTPItemText->GetItemData();

		KColumnReportRecordItem*	pKColumnItem	= (KColumnReportRecordItem*)pCXTPRecord->GetItem(2);
		CString						strValue		= a_vecLineData[pKColumnItem->getColumnIndex() - 1];

 		if(emSurveyImportDataType == KEMSurveyImportInt64)
		{
			nxValue = _ttoi64(strValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(__int64));
		}
		else if(emSurveyImportDataType == KEMSurveyImportInteger)
		{
			nValue	= _ttoi(strValue);
			ofs.write(reinterpret_cast<char*>(&nValue), sizeof(int));
		}
		else if(emSurveyImportDataType == KEMSurveyImportDouble)
		{
			dValue	= _ttof(strValue);
			ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
		}

		CString strCaption = pCXTPItemText->GetCaption(NULL);

		if(strCaption.CompareNoCase(_T("Purpose")) == 0)
			m_setPurpose.insert(nValue);

		if(strCaption.CompareNoCase(_T("Mode")) == 0)
			m_setMode.insert(nValue);
	}
}

std::set<int> KTimeODSurveyImport2Dlg::PurposeSet( void )
{
	return m_setPurpose;
}

std::set<int> KTimeODSurveyImport2Dlg::ModeSet( void )
{
	return m_setMode;
}
