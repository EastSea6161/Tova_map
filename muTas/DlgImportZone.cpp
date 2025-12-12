// DlgImportZone.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "DlgImportZone.h"
#include "afxdialogex.h"
#include "DefineNames.h"
#include "..\QBicCommon\QBicTxtReader.h"
#include "..\QBicCommon\QBicTxtWriter.h"
#include "DBaseImportTable.h"
//#include "ImportCommon.h"
#include "..\QBicGIS\TxPolygon.h"

#include "Target.h"
#include "ImChampFrameWindow.h"

enum
{
	_0_COLUMN_DEFFILED_NAME = 0,
	_1_COLUMN_DEFFILED_TYPE,
	//_2_COLUMN_DEFFILED_PK_YN,
	_2_COLUMN_IMPORTFIELD_NAME
};

// KDlgImportZone 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDlgImportZone, KWhiteDlgEx)

KDlgImportZone::KDlgImportZone(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KWhiteDlgEx(KDlgImportZone::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, m_strTableName(_T(""))
{
	if (nullptr != m_pTable) {
		m_strTableName = m_pTable->Name();
	}

	CString strTargetLocation = m_pTarget->GetLocation();

	m_strErrFile.Format   (_T("%s\\%s"), strTargetLocation, IMPORT_ERROR_FILE_NAME);
	m_strMiddleFile.Format(_T("%s\\%s"), strTargetLocation, IMPORT_FILE_NAME);

	m_mapSeparator.clear(); {
		TSeparator oTSeparator;
		oTSeparator.nCode = 0;
		oTSeparator.tcSeparator = _T(',');
		oTSeparator.strDispName = _T("Comma(,)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 1;
		oTSeparator.tcSeparator = _T(';');
		oTSeparator.strDispName = _T("Semicolon(;)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 2;
		oTSeparator.tcSeparator = _T('|');
		oTSeparator.strDispName = _T("Pipe(|)");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));

		oTSeparator.nCode = 3;
		oTSeparator.tcSeparator = _T('	');
		oTSeparator.strDispName = _T("Tab");
		m_mapSeparator.insert(std::make_pair(oTSeparator.nCode, oTSeparator));
	}
}

KDlgImportZone::~KDlgImportZone()
{
}

void KDlgImportZone::DoDataExchange(CDataExchange* pDX)
{
	KWhiteDlgEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1,    m_ctrlReportColumn);
	DDX_Control(pDX, IDC_REPORT2,    m_ctrlReportPreview);
	DDX_Control(pDX, IDC_REPORT7,    m_ctrlReportSampleColumn);
	DDX_Control(pDX, IDC_FILENAME2, m_editFile);
	DDX_Control(pDX, IDC_COMBO5, m_cboSeparator);
	DDX_Control(pDX, IDC_COMBO4, m_cboEncoding);
}


BEGIN_MESSAGE_MAP(KDlgImportZone, KWhiteDlgEx)
	ON_BN_CLICKED(IDOK,       &KDlgImportZone::OnBnClickedOk)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT1,     &KDlgImportZone::OnReportRecordsDropped)
	ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT7,     &KDlgImportZone::OnReportDropSampleColumn)
	ON_BN_CLICKED(IDC_CHECK4, &KDlgImportZone::OnBnClickedCheckDefault)
	ON_BN_CLICKED(IDCANCEL, &KDlgImportZone::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_FILENAME2, &KDlgImportZone::OnEnChangeFilename)
	ON_CBN_SELCHANGE(IDC_COMBO4, &KDlgImportZone::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &KDlgImportZone::OnCbnSelchangeCombo5)
	ON_BN_CLICKED(IDC_CHECK1, &KDlgImportZone::OnBnClickedCheck1)
END_MESSAGE_MAP()


// KDlgImportZone 메시지 처리기입니다.
void KDlgImportZone::ResizeComponent()
{
	try 
	{
		SetResize(IDC_STATIC1,		SZ_TOP_LEFT,	SZ_TOP_RIGHT);
		SetResize(IDC_FILENAME2,    SZ_TOP_LEFT,	SZ_TOP_RIGHT);
		SetResize(IDC_STATIC6,		SZ_TOP_LEFT,	SZ_TOP_RIGHT);

		SetResize(IDC_REPORT1,	    SZ_TOP_LEFT,	CXTPResizePoint(.8f, .5));
		SetResize(IDC_REPORT7,		CXTPResizePoint(.8f, 0),	SZ_MIDDLE_RIGHT);
		SetResize(IDC_STATIC5,		SZ_MIDDLE_LEFT,	SZ_MIDDLE_RIGHT);

		SetResize(IDC_STATIC3,		SZ_MIDDLE_LEFT,	SZ_MIDDLE_LEFT);

		SetResize(IDC_REPORT2,		SZ_MIDDLE_LEFT,	SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC2,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_SEP,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO5,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
		SetResize(IDC_STATIC_ENC,	SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO4,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);
		SetResize(IDC_CHECK1,		SZ_BOTTOM_LEFT,	SZ_BOTTOM_LEFT);

		SetResize(IDOK,				SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::InitBaseDataTypeUI()
{
	UpdateData(TRUE);

	BOOL bShow = FALSE;

	KEMImportType emImportType = GetSelectedImportType();
	if(KEMImportInsert == emImportType)
		bShow = FALSE;
	else if (KEMImportUpdate == emImportType)
		bShow = FALSE;
	else if (KEMImportAdd == emImportType)
		bShow = FALSE;
}


KEMImportType KDlgImportZone::GetSelectedImportType()
{
	return KEMImportUpdate;
}


BOOL KDlgImportZone::OnInitDialog()
{
	KWhiteDlgEx::OnInitDialog();
	KWhiteDlgEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	ResizeComponent();

	InitComboSeparator();
	InitComboEncoding();

	if (true) { // 입력창 설정
		TCHAR szFilter[] = _T("Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
		m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
		m_editFile.SetDlgOpenFile( TRUE );
		m_editFile.SetReadOnly( TRUE );
		m_editFile.SetDlgTitle( _T("Select file") );
		m_editFile.SetDlgFilter( szFilter );

		m_editFile.SetDlgInitialDir(m_pTarget->GetLocation());
	}

	InitBaseDataTypeUI();

	QBicReportCtrlSetting::Default(m_ctrlReportColumn, TRUE, FALSE, FALSE);
	QBicReportCtrlSetting::Default(m_ctrlReportSampleColumn, TRUE);
	QBicReportCtrlSetting::Default(m_ctrlReportPreview, FALSE, FALSE, FALSE);

	CheckDlgButton(IDC_CHECK4, BST_CHECKED);

	InitializeSampleColumnReport();
	InitializeColumnField();

	m_ctrlReportColumn.EnableDragDrop      (_T("TestDragDrop"), xtpReportAllowDrop, xtpReportDropSelect);
	m_ctrlReportSampleColumn.EnableDragDrop(_T("TestDragDrop"), xtpReportAllowDragCopy | xtpReportAllowDrop, xtpReportDropSelect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDlgImportZone::OnEnChangeFilename()
{
	UpdateData( TRUE );

	CString strFile(_T("")); {
		m_editFile.GetWindowText( strFile );
		if (strFile.IsEmpty())
			return;
	}

	// 디폴트 구분자 설정
	CheckDefaultSeperator(strFile);

	// 디폴트 인코딩 설정
	int nEncoding = CheckEncoding(strFile);
	if (KEMCSVEncodingUTF8 == nEncoding)
		m_cboEncoding.SetCurSel(1);
	else // 
		m_cboEncoding.SetCurSel(0);

	if (CheckImportHeader(strFile)) {
		CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	} else {
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}

	if( PriviewLoadCSV() ) {
		InitializeColumnField();
		UpdateColumnRecord();
		InitializePreview();

		ControlDefaultCheck();
	} 
}

void KDlgImportZone::CheckDefaultSeperator(CString a_strFile)
{
	try
	{
		QBicTxtReader oReader; {
			if (oReader.Open(a_strFile) == false) {
				throw 1;
			}
		}

		CString strLine(_T(""));
		oReader.ReadLine(strLine);

		int nSeparator(0);
		int nMax = -9; // 하나씩 파싱을 해보고 파싱된 결과가 가장 많은 개수인 구분자를 디폴트 구분자로.. 

		auto iter  = m_mapSeparator.begin();
		auto itEnd = m_mapSeparator.end();

		for (; iter != itEnd; ++iter) {
			const int  &nCode       = iter->first;
			TSeparator &oTSeparator = iter->second;

			std::vector<CString> vecSelect = QBicSplit::Split(strLine, oTSeparator.tcSeparator);

			int nSizeVec = (int)vecSelect.size();
			if (nSizeVec > nMax) {
				nMax = nSizeVec;
				nSeparator = nCode;
			}
		}

		m_cboSeparator.SetCurSel(nSeparator);

		oReader.Close();
	} catch (int& ex) {
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg = _T("Fail - Read TextFile");
		else
			strMsg.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strMsg);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	UpdateData(FALSE);
}

bool KDlgImportZone::CheckImportHeader(CString a_strFile)
{
	try
	{
		QBicTxtReader oReader; {
			if (oReader.Open(a_strFile) == false) {
				throw 1;
			}
		}

		TCHAR tcSeparator; {
			int nCurSel = m_cboSeparator.GetCurSel();
			int nCode   = (int)m_cboSeparator.GetItemData(nCurSel);

			auto itFind = m_mapSeparator.find(nCode);
			if (itFind != m_mapSeparator.end()) {
				TSeparator &oTSeparator = itFind->second;
				tcSeparator = oTSeparator.tcSeparator;
			}
		}

		std::vector<CString> vecSelect;
		oReader.ReadLine(tcSeparator, vecSelect);
		oReader.Close();

		bool bFirstColumnHeader(false);
		CString strSample = vecSelect[0]; // 한줄을 읽고, 맨앞 데이터 하나를 Sample로..
		if (QBicStringChecker::IsNumeric(strSample)) { // 숫자라면, 일반 데이터
			bFirstColumnHeader = false;
		} else { // 문자라면, Header일 것이다.
			bFirstColumnHeader = true;
		}

		return bFirstColumnHeader;

	} catch (int& ex) {
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg = _T("Fail - Read TextFile");
		else
			strMsg.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strMsg);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	UpdateData(FALSE);

	return false;
}

int KDlgImportZone::CheckEncoding(CString a_strFile, bool a_bAlarmErr)
{
	try
	{
		int nEncoding = KEMCSVEncodingUnknown; {
			QBicTxtReader oReader;
			nEncoding = oReader.GetTxtFileEncoding(a_strFile);

			if (nEncoding != KEMCSVEncodingUTF8 && nEncoding != KEMCSVEncodingANSI)	
				throw 9;
		}

		return nEncoding; // 성공
	} catch (int& ex) {
		CString strMsg(_T(""));
		if (1 == ex)
			strMsg = _T("인코딩 인식에 실패하였습니다.");
		else if (9 == ex)
			strMsg = _T("ANSI와 UTF-8 인코딩 파일만 인식 가능합니다.");
		else
			strMsg.Format(_T("오류 : %d"), ex);
		TxLogDebug((LPCTSTR)strMsg);
		if (a_bAlarmErr)
			AfxMessageBox(strMsg);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		if (a_bAlarmErr)
			AfxMessageBox(ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		if (a_bAlarmErr)
			AfxMessageBox(_T("인코딩 인식에 실패하였습니다."));
	}

	return KEMCSVEncodingUnknown;
}

void KDlgImportZone::InitComboSeparator()
{
	try
	{
		m_cboSeparator.ResetContent();

		auto iter  = m_mapSeparator.begin();
		auto itEnd = m_mapSeparator.end();

		for (; iter != itEnd; ++iter)
		{
			const int  &nCode       = iter->first;
			TSeparator &oTSeparator = iter->second;

			int nCur = m_cboSeparator.AddString(oTSeparator.strDispName);
			m_cboSeparator.SetItemData(nCur, (DWORD_PTR)oTSeparator.nCode);
		}

		m_cboSeparator.SetCurSel(3);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::InitComboEncoding()
{
	try
	{
		m_cboEncoding.ResetContent();
		int nCur = m_cboEncoding.AddString(_T("ANSI"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)1);

		nCur = m_cboEncoding.AddString(_T("UTF-8"));
		m_cboEncoding.SetItemData(nCur, (DWORD_PTR)2);

		m_cboEncoding.SetCurSel(0);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::InitializeColumnField( void )
{
	TxLogDebugStartMsg();

	try 
	{
		m_ctrlReportColumn.ResetContent();
		m_ctrlReportColumn.GetColumns()->Clear();
		m_ctrlReportColumn.Populate();

		// Field
		CXTPReportColumn* pColumn = m_ctrlReportColumn.AddColumn( new CXTPReportColumn( _0_COLUMN_DEFFILED_NAME, _T("System Column"), 30 ) );
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment( DT_CENTER );
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("시스템 컬럼"));
		}

		// type
		pColumn = m_ctrlReportColumn.AddColumn( new CXTPReportColumn( _1_COLUMN_DEFFILED_TYPE, _T("Type"), 16 ) );
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment( DT_CENTER );
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("데이터 유형"));
		}

		// Primary key
		// 		pColumn = m_ctrlReportColumn.AddColumn( new CXTPReportColumn( _2_COLUMN_DEFFILED_PK_YN, _T("Primary Key"), 25 ) );
		// 		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;
		// 		pColumn->SetHeaderAlignment(DT_CENTER);
		// 		pColumn->SetAlignment( DT_CENTER );

		// column
		CXTPReportColumn* pImport = m_ctrlReportColumn.AddColumn( new CXTPReportColumn( _2_COLUMN_IMPORTFIELD_NAME, _T("File Column"), 33 ) );
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pImport->SetCaption(_T("파일 컬럼"));
		}

		m_ctrlReportSampleColumn.ResetContent();

		/// csv 파일의 컬럼을 combo에 추가, 0 = NULL, 1~... = Column Name
		if (m_ColumnRow.size() > 0) {
			int nColumnIndex(0);

			CString strNull(_T("Null"));
			pImport->GetEditOptions()->AddConstraint( strNull, nColumnIndex);
			UpdateReportDataSampleColumn(strNull, nColumnIndex);
			nColumnIndex++; 

			auto itColumnArray  = m_ColumnRow.begin();
			auto itEnd          = m_ColumnRow.end();
			for (; itColumnArray != itEnd; ++itColumnArray, nColumnIndex++) {
				pImport->GetEditOptions()->AddConstraint( *itColumnArray, nColumnIndex );
				UpdateReportDataSampleColumn(*itColumnArray, nColumnIndex);
			}

			m_ctrlReportSampleColumn.Populate();
		}

		pImport->SetHeaderAlignment(DT_CENTER);
		pImport->SetAlignment( DT_CENTER );
		pImport->GetEditOptions()->m_bAllowEdit = TRUE;
		pImport->GetEditOptions()->m_bExpandOnSelect = TRUE;
		pImport->GetEditOptions()->AddComboButton( TRUE );	
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KDlgImportZone::UpdateReportDataSampleColumn(CString a_strColName, int a_nIndex)
{
	try 
	{
		CXTPReportRecord*     pRecord = m_ctrlReportSampleColumn.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = pRecord->AddItem(new CXTPReportRecordItemText(a_strColName));
		pItem->SetAlignment(DT_CENTER);
		pItem->SetItemData(a_nIndex);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::InitializeSampleColumnReport()
{
	try 
	{
		CXTPReportColumn* pColumn = m_ctrlReportSampleColumn.AddColumn( new CXTPReportColumn( 0, _T("Drag & Drop"), 100 ) );
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetSortable(FALSE);
		pColumn->SetEditable(FALSE);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("끌어 놓기"));
		}

		m_ctrlReportSampleColumn.Populate();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::UpdateColumnRecord( void )
{
	TxLogDebugStartMsg();
	UpdateData(TRUE);

	CheckDlgButton(IDC_CHECK4, BST_CHECKED);

	m_ctrlReportColumn.ResetContent();
	if (m_ColumnRow.size() < 1) {
		return;
	}

	try
	{
		int nCSVIndex(0);
		auto itCSVColumn = m_ColumnRow.begin();

		const KIOColumns* pColumns = m_pTable->Columns();
		int nColumnCount           = pColumns->ColumnCount();

		for (int i= 0; i< nColumnCount; i++) {
			KIOColumn*            pColumn  = pColumns->GetColumn(i);

			CString               strColNm = pColumn->Name();
			CXTPReportRecord*     pRecord  = m_ctrlReportColumn.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem    = nullptr;

            bool bRed(false);
            CString strField(_T("")); {
                if (strColNm.CompareNoCase(COLUMN_ZONE_ID)   == 0 ) {
                    strField.Format(_T("* %s"), pColumn->Caption());
                    bRed = true;
                }
                else {
                    strField.Format(_T("%s"), pColumn->Caption());
                }
            }

            pItem = pRecord->AddItem(new CXTPReportRecordItemText(strField)); {
                pItem->SetItemData((DWORD_PTR)pColumn);
                if (bRed) {
                    pItem->SetTextColor(RGB(255,0,0));
                }
            }	

			// type
			CString strDataType(_T(""));
			switch(pColumn->DataType())
			{
			case KEMIODataTypeInteger : strDataType = DATATYPESTRING_INTEGER; break;
			case KEMIODataTypeDouble  : strDataType = DATATYPESTRING_DOUBLE;  break;
			case KEMIODataTypeString  : strDataType = DATATYPESTRING_STRING;  break;
			}
			pRecord->AddItem(new CXTPReportRecordItemText(strDataType));

			// 			// Primary key
			// 			CString strPK = (pColumn->PrimaryKey() == true ? _T("Y") : _T(""));
			// 			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strPK));

			// column
			if(m_ColumnRow.end() == itCSVColumn) {
				pRecord->AddItem(new KColumnReportRecordItem(0));
			} else {
				pRecord->AddItem(new KColumnReportRecordItem(MatchingColumnIndex(pColumn->Caption())));
				++itCSVColumn;
			}
		}

		m_ctrlReportColumn.GetReportHeader()->AllowColumnRemove( FALSE );
		m_ctrlReportColumn.FocusSubItems( TRUE );
		m_ctrlReportColumn.Populate();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

int KDlgImportZone::MatchingColumnIndex(CString strColumnCaption)
{
	int nIndex = 0;

	for (int i = 0; i < m_ColumnRow.size(); i++)
	{
		if (strColumnCaption.CompareNoCase(m_ColumnRow[i]) == 0)
		{
			nIndex = i + 1; // 0은 Null로 처리
			break;
		}
	}

	return nIndex;
}

void KDlgImportZone::InitializePreview( void )
{
	TxLogDebugStartMsg();

	try
	{
		m_ctrlReportPreview.ResetContent();
		m_ctrlReportPreview.GetColumns()->Clear();
		m_ctrlReportPreview.Populate();

		bool bAutoSize(false); {
			if (m_ColumnRow.size() < 10)
				bAutoSize = true;
		}

		if (!bAutoSize) {
			QBicReportCtrlSetting::AutoColumnSizeFalse(m_ctrlReportPreview);
		} else {
			m_ctrlReportPreview.GetReportHeader()->SetAutoColumnSizing(TRUE);
		}

		CXTPReportColumn* pColumn = NULL;	

		int nColumnIndex(0);
		std::vector< CString >::iterator itColumnArray = m_ColumnRow.begin();
		while( m_ColumnRow.end() != itColumnArray )	{
			pColumn = m_ctrlReportPreview.AddColumn(new CXTPReportColumn(nColumnIndex,      (*itColumnArray), 100));
			pColumn->SetHeaderAlignment(DT_CENTER);
			if (!bAutoSize)
				pColumn->SetBestFitMode(xtpColumnBestFitModeAllData);

			++nColumnIndex;
			++itColumnArray;
		}

		CXTPReportRecord*      pRecord = nullptr;
		CXTPReportRecordItem*  pItem   = nullptr;

		int nRow(0);
		std::vector<CSVRow>::iterator itRows, itRowsEnd = m_Rows.end();
		for(itRows = m_Rows.begin(); itRows != itRowsEnd; ++itRows) {
			pRecord = m_ctrlReportPreview.AddRecord(new CXTPReportRecord());

			CSVRow& row = (*itRows);
			CSVRow::iterator itrow, itrowEnd = row.end();
			for(itrow = row.begin(); itrow != itrowEnd; ++itrow) {
				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(*itrow));
				pItem->SetAlignment(DT_CENTER);
			}

			int nSizeRowData = (int)row.size();
			if (nColumnIndex > nSizeRowData) {
				for (int i= 0; i< (nColumnIndex - nSizeRowData); i++) {
					pItem   = pRecord->AddItem(new CXTPReportRecordItem);
				}
			}

			++nRow;
		}

		m_ctrlReportPreview.Populate();

		CXTPReportHeader*  pHeader        = m_ctrlReportPreview.GetReportHeader();
		CXTPReportColumns* pReportColumns = m_ctrlReportPreview.GetColumns();
		int                nReportColumns = pReportColumns->GetCount();
		for (int i= 0; i< nReportColumns; i++) {
			CXTPReportColumn* pReportColumn = pReportColumns->GetAt(i);
			if (!bAutoSize)
				pHeader->BestFit(pReportColumn);
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

int KDlgImportZone::GetSelectedEncoding()
{
	int nEncoding; {
		if (m_cboEncoding.GetCurSel() == 1)
			nEncoding = KEMCSVEncodingUTF8;
		else 
			nEncoding = KEMCSVEncodingANSI;
	}

	return nEncoding;
}

TCHAR KDlgImportZone::GetSelectedSeparator()
{
	TCHAR tcSeparator; {
		int nCurSel = m_cboSeparator.GetCurSel();
		int nCode   = (int)m_cboSeparator.GetItemData(nCurSel);

		auto itFind = m_mapSeparator.find(nCode);
		if (itFind != m_mapSeparator.end()) {
			TSeparator &oTSeparator = itFind->second;
			tcSeparator = oTSeparator.tcSeparator;
		}
	}	

	return tcSeparator;
}

bool KDlgImportZone::PriviewLoadCSV( void )
{
	TxLogDebugStartMsg();
	m_Rows.clear();
	m_ColumnRow.clear();

	TCHAR tcSeparator = GetSelectedSeparator();
	int   nEncoding   = GetSelectedEncoding();
	bool  bContainsColumn(false); {
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bContainsColumn = true;
	}
	CString strFile(_T("")); {
		m_editFile.GetWindowText( strFile );
	}

	try
	{
		QBicTxtReader oReader; {
			oReader.SetEncoding(nEncoding);
			if (oReader.Open(strFile) == false) {
				throw 2;
			}
		}

		CString strLine(_T(""));
		oReader.ReadLine(strLine);
		if( true == bContainsColumn ) {
			ParseCSVLineString( strLine, tcSeparator, m_ColumnRow );
		} else {
			int nIndex(1);
			CSVRow row;

			ParseCSVLineString( strLine, tcSeparator, row );

			CString strColumnName(_T(""));
			CSVRow::iterator itRow = row.begin(); 
			while( row.end() != itRow )	{
				strColumnName.Format( _T("Column %d"), nIndex++ );
				m_ColumnRow.push_back( strColumnName );
				++itRow;
			}

			m_Rows.push_back( row );
		}

		int nLoopCount = 0;

		while(oReader.ReadLine(strLine) && nLoopCount < 5000) {
			++nLoopCount;

			if ((strLine.Trim()).CompareNoCase(_T("")) == 0)
				continue;

			if (nLoopCount < 100 || nLoopCount % 5 == 0) {
				CSVRow row;
				ParseCSVLineString( strLine, tcSeparator, row );
				m_Rows.push_back( row );
			}		
		}

		oReader.Close();
	} catch (int& ex) {
		CString strError(_T(""));
		if (ex == 2)
			strError.Format(_T("Error - Open File: %s"), strFile);
		else
			strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return false;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return false;
	} catch (...) {
		TxLogDebugException();
		return false;
	}

	return true;

	TxLogDebugEndMsg();
}

void KDlgImportZone::ParseCSVLineString( CString& strLine, TCHAR tcSeparator, CSVRow& row )
{
	bool bQuote(false);
	int  nSub(0);
	CString strRow, strQuote;

	while( AfxExtractSubString( strRow, strLine, nSub++, tcSeparator ) == TRUE )
	{
		if( true == bQuote ) {
			strQuote += strRow;
			if( strRow.GetAt( strRow.GetLength() - 1 ) == _T('"') ) {
				strQuote.Remove( _T('"') );
				strRow   = strQuote;
				bQuote   = false;
				strQuote = _T("");
				row.push_back(strRow);
			}
		} else {
			TCHAR chFirst, chLast;
			if( strRow.IsEmpty() == TRUE ) {
				row.push_back( strRow );
			} else if( strRow.GetLength() == 1 ) {
				chFirst = strRow.GetAt( 0 );
				if( chFirst == _T('"') ) {
					bQuote = true;
				} else {
					row.push_back(strRow);
				}
			} else {
				chFirst = strRow.GetAt( 0 );
				chLast  = strRow.GetAt( strRow.GetLength() - 1 );

				if( (chFirst == _T('"')) && (chLast == _T('"')) ) {
					strRow.Remove( _T('"') );
					row.push_back( strRow );
				} else if( chFirst == _T('"') ) {
					bQuote = true;
					strQuote = strRow;
				} else {
					row.push_back( strRow );
				}
			}
		}
	}
}

void KDlgImportZone::OnBnClickedCancel()
{
	KWhiteDlgEx::OnCancel();
}

void KDlgImportZone::OnReportRecordsDropped(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);
	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount = pRecords->GetCount();
		if (nCount < 1)	{
			return;
		}

		CXTPReportRecord*  pDropRecord    = pRecords->GetAt(0);
		CXTPReportRecords* pTargetRecords = m_ctrlReportColumn.GetRecords();

		if (nullptr != pDropRecord)	{
			int                   nRowIndex = pDropRecord->GetIndex(); // will be Drop Row's Index
			CXTPReportRecordItem* pItem2    = pDropRecord->GetItem(0);
			int nComboIndex                 = (int)pItem2->GetItemData();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_ctrlReportColumn.Populate();

			pTargetRecords = m_ctrlReportColumn.GetRecords();
			CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex); // pItemNotify->pTargetRecord 사용 불가

			if (nullptr != pTargetRecord) {
				KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pTargetRecord->GetItem(_2_COLUMN_IMPORTFIELD_NAME);
				pItem->m_nIndex = nComboIndex;
				//m_ctrlReportColumn.RedrawControl();

				CXTPReportRow* pRow = m_ctrlReportColumn.GetRows()->Find(pTargetRecord);
				m_ctrlReportColumn.SetFocusedRow(pRow);
			}
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::OnReportDropSampleColumn(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	try
	{
		XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount       = pRecords->GetCount();

		for (int i= 0; i< nCount; i++) {
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			m_ctrlReportSampleColumn.RemoveRecordEx(pRecord);
		}
		m_ctrlReportSampleColumn.Populate();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::OnBnClickedCheckDefault()
{
	ControlDefaultCheck();
}

void KDlgImportZone::ControlDefaultCheck()
{
	try 
	{
		if (IsDlgButtonChecked(IDC_CHECK4) == BST_CHECKED)
			UpdateColumnRecord();
		else
			AllNullCheck();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::AllNullCheck()
{
	try 
	{
		CXTPReportRecords* pRecords = m_ctrlReportColumn.GetRecords();
		int nRecordCount = pRecords->GetCount();

		for (int i = 0; i < nRecordCount; ++i) {
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			if (FALSE == pRecord->IsVisible())
				continue;

			KColumnReportRecordItem* pItem   = (KColumnReportRecordItem*)pRecord->GetItem(_2_COLUMN_IMPORTFIELD_NAME);
			pItem->m_nIndex = 0;
		}

		m_ctrlReportColumn.RedrawControl();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::OnBnClickedOk()
{
	UpdateData( TRUE );

	try
	{
		KEMImportType emImportType = GetSelectedImportType();

		if (KEMImportUpdate == emImportType)	{
			ValidateUpdateMode();
		}

		if( false == AlertTableChange() ) 
			return;

		ThreadPara oPara(this);

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		if (true) { // 데이터 검증 및 중간 바이너리 작성
			oPara.TBusiness = 0;
			oPara.TKeyInt32[0] = 0;      // 레코드 개수 저장
			oPara.TKeyInt32[1] = 0;      // Success 개수 저장
			oPara.TKeyInt32[2] = 0;      // 실패 개수 저장

			RThreadInfo.Init();
			QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang); 

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			} else {
				CString strMsg(_T(""));
				GetStatisticsMsg(emImportType, &oPara, strMsg);
				if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
					return;
			}
		}

		if (true) { // Read 중간바이너리 & DB Import
			oPara.TBusiness = 1; // oPara.TKeyInt[0]에 레코드 개수가 저장된 상태에서 넘어감

			RThreadInfo.Init();
			QBicSimpleProgressThread::ExecuteThread(ThreadRun, &oPara, false, nLang); 

			if (RThreadInfo.IsOK() == false) {
				ThrowException(RThreadInfo.ErrorMsg());
			}
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("오류 : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		AfxMessageBox(strError);

		return;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());

		return;
	} catch (...) {
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));

		return;
	}

	RelationTableNotify();

	OnOK();
}

unsigned __stdcall KDlgImportZone::ThreadRun( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	if (pParameter != nullptr) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KDlgImportZone* pDlg = (KDlgImportZone*)pPara->TWindow;

			if (0 == pPara->TBusiness)
				pDlg->Execute(pPara);
			else
				pDlg->ExecuteImport(pPara);
		}
	}

	return 1;
}

void KDlgImportZone::Execute(ThreadPara* pPara)
{
	try 
	{
		KDBaseImportTable::GetZoneID (m_pTable, m_setZoneID);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("파일을 불러오는 중..."));
		}
		else {
			AddStatusMessage(_T("Reading File..."));
		}

		//Convert CSV to Binary
		ReadCSVFile(pPara);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		RThreadInfo.SetErrorFlag(-1, strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
	}
}

void KDlgImportZone::ExecuteImport(ThreadPara* pPara)
{
	try 
	{
		KEMImportType emImportType = GetSelectedImportType();
		if (KEMImportUpdate == emImportType)
			UpdateUserData(pPara);
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		RThreadInfo.SetErrorFlag(-1, strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		RThreadInfo.SetErrorFlag(-1, _T("오류가 발생하였습니다."));
	}
}

void KDlgImportZone::GetStatisticsMsg(KEMImportType a_emImportType, ThreadPara* pPara, CString &a_strMsg)
{
	a_strMsg = _T("");
	try 
	{
		if (KEMImportUpdate == a_emImportType) {
			int nIdx(1);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				a_strMsg = _T("입력된 데이터의 내역은 아래와 같습니다.\r\n");
				a_strMsg.AppendFormat(_T("#%d. 레코드 개수: %d 건\r\n"), nIdx++, pPara->TKeyInt32[0]);

				if (pPara->TKeyInt32[2] > 0) { //실패 건수가 있을 경우
					a_strMsg.AppendFormat(_T("#%d. 양호: %d 건, 불량: %d 건\r\n"), nIdx++, pPara->TKeyInt32[1], pPara->TKeyInt32[2]);
				}
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				a_strMsg = _T("Imported data are as follows.\r\n");
				a_strMsg.AppendFormat(_T("#%d. Total number of records : %d\r\n"), nIdx++, pPara->TKeyInt32[0]);

				if (pPara->TKeyInt32[2] > 0) { //실패 건수가 있을 경우
					a_strMsg.AppendFormat(_T("#%d. Imported : %d 건, Skipped : %d\r\n"), nIdx++, pPara->TKeyInt32[1], pPara->TKeyInt32[2]);
				}
			}
		}

		//a_strMsg.Append(_T("\n진행 하시겠습니까?\r\n"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			a_strMsg.Append(_T("\n진행 하시겠습니까?\r\n"));
		}
		else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
			a_strMsg.Append(_T("\nDo you want to continue?\r\n"));
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::ReadCSVFile(ThreadPara* pPara)
{
	TxLogDebugStartMsg();

	TCHAR tcSeparator = GetSelectedSeparator();
	int   nEncoding   = GetSelectedEncoding();
	bool  bContainsColumn(false); {
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bContainsColumn = true;
	}
	CString strFile(_T("")); {
		m_editFile.GetWindowText( strFile );
	}

	QBicTxtReader oReader; {
		oReader.SetEncoding(nEncoding);
		if (oReader.Open(strFile) == false)
			return;
	}

	QBicFile::DeleteFile(m_strMiddleFile);
	std::ofstream ofs(m_strMiddleFile, std::ios::binary);

	QBicFile::DeleteFile(m_strErrFile);
	QBicTxtWriter ofErr; {
		if (ofErr.Open(m_strErrFile) == false)
			return;
	}

	std::vector<TColMatch> vecColMatch; { // 저장필드와 입력파일의 필드idx 매칭정보 저장(Re)
#pragma region Load vecColMatch
		try 
		{
			CXTPReportRecords* pRecords = m_ctrlReportColumn.GetRecords();
			int nRecordCount = pRecords->GetCount();
			for (int i= 0; i< nRecordCount; i++) {
				CXTPReportRecord* pRecord            = pRecords->GetAt(i);
				int nImportColumnIndex = GetImportColumnIndex(pRecord);
				if (0 == nImportColumnIndex || pRecord->IsVisible() == FALSE)
					continue;

				KIOColumn* pIOColumn = GetDefIOColumn(pRecord);

				TColMatch oTColMatch;
				oTColMatch.pIOColumn       = pIOColumn;
				oTColMatch.nImportColIndex = nImportColumnIndex;

				vecColMatch.push_back(oTColMatch);
			}
		} catch (int& ex) {
			CString strError(_T(""));
			strError.Format(_T("Error : %d"), ex);
			TxLogDebug((LPCTSTR)strError);
		} catch (KExceptionPtr ex) {
			TxExceptionPrint(ex);
		} catch (...) {
			TxLogDebugException();
		}
	}
#pragma endregion Load vecColMatch

	int nSuccessCnt(0);
	int nFailedCnt(0);
	int nImportDataCount(0);
	int nCurrentLine(0);
	try
	{
		CString strReadLine(_T(""));
		bool    bEverErrLine(false);

		//CSV파일 헤더 존재 시 고의적으로 한줄 읽어서 버림.
		if( true == bContainsColumn ) {
			oReader.ReadLine(strReadLine);
			++nCurrentLine;
		}

		KEMImportType emImportType = GetSelectedImportType();

		m_setDuplicateSingleID.clear();
		m_setDuplicateMultiID.clear();
		m_mapDuplicateTripleID.clear();

		while( oReader.ReadLine(strReadLine) ) {
			++nCurrentLine;

			if ((strReadLine.Trim()).CompareNoCase(_T("")) == 0) {
				CString strErrorMsg(_T("")); {
					//strErrorMsg.Format(_T("Line : %d =>\t 데이터 없음\r\n"), nCurrentLine);
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strErrorMsg.Format(_T("Line : %d =>\t 데이터 없음\r\n"), nCurrentLine);
					}
					else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
						strErrorMsg.Format(_T("Line : %d =>\t There is no any data\r\n"), nCurrentLine);
					}
				}
				ofErr.Write(strErrorMsg);
				continue;
			}

			CSVRow row;
			ParseCSVLineString( strReadLine, tcSeparator, row );

			if (KEMImportUpdate == emImportType) {
				if (CSV2Binary_Update(vecColMatch, nCurrentLine, bEverErrLine, row, ofs, ofErr)) {
					nSuccessCnt++;
				} else {
					nFailedCnt++;
				}
			}
		}

		if(bEverErrLine) {
			if (KEMImportInsert == emImportType) {
				CString strErrorMsg(_T(""));
				//strErrorMsg.Format(_T("Import 실패\r\n %s 파일에서 내역을 확인 하세요."), IMPORT_ERROR_FILE_NAME);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strErrorMsg.Format(_T("Import 실패\r\n %s 파일에서 내역을 확인 하세요."), IMPORT_ERROR_FILE_NAME);
				}
				else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
					strErrorMsg.Format(_T("Import failed !\r\n Check the details from the %s."), IMPORT_ERROR_FILE_NAME);
				}
				ErrorFileOpen();
				ThrowException(strErrorMsg);
			} else {
				ErrorFileOpen();
			}
		}

		nImportDataCount = nCurrentLine;
		if( bContainsColumn )	{
			--nImportDataCount;
		}
		pPara->TKeyInt32[0] = nImportDataCount;      // 레코드 개수 저장
		pPara->TKeyInt32[1] = nSuccessCnt;           // Update/Add Mode일 경우 성공 데이터의 개수
		pPara->TKeyInt32[2] = nFailedCnt;            // Update/Add Mode일 경우 실패 데이터의 개수

		ofs.close();
		oReader.Close();
		ofErr.Close();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		ofs.close();
		oReader.Close();
		ofErr.Close();
		throw ex;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		ofs.close();
		oReader.Close();
		ofErr.Close();
		ThrowException(ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();
		ofs.close();
		oReader.Close();
		ofErr.Close();
		throw 9;
	}

	TxLogDebugEndMsg();
}

bool KDlgImportZone::CSV2Binary_Update(std::vector<TColMatch> &a_vecColMatch, int a_nCurrentLine,  bool& a_bEverErrLine, CSVRow a_row,  std::ofstream& ofs, QBicTxtWriter& ofErr )
{
	try
	{
		std::map<CString, Integer> mapFieldValue;

		if( !ConfirmPrimaryKey(a_vecColMatch, a_row, mapFieldValue, KEMImportUpdate) ) {
			CString strErrorMsg(_T(""));
			//strErrorMsg.Format(_T("Line : %d =>\t ID 값 추출 실패"), a_nCurrentLine);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strErrorMsg.Format(_T("Line : %d =>\t ID 값 추출 실패"), a_nCurrentLine);
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				strErrorMsg.Format(_T("Line : %d =>\t Failed to read ID value"), a_nCurrentLine);
			}
			ThrowException(strErrorMsg);
		}

		if( false == IsFindTableData(mapFieldValue, KEMImportUpdate) ) {
			CString strErrorMsg(_T(""));
			//strErrorMsg.Format(_T("Line : %d =>\t %s 테이블 상에 ID 값에 대한 데이터 없음"), a_nCurrentLine, m_strTableName);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strErrorMsg.Format(_T("Line : %d =>\t %s 테이블 상에 ID 값에 대한 데이터 없음"), a_nCurrentLine, m_strTableName);
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				strErrorMsg.Format(_T("Line : %d =>\t There is no matching ID in %s Table"), a_nCurrentLine, m_strTableName);
			}
			ThrowException(strErrorMsg);
		}

		CString strImportValue(_T(""));

		int nSizeVec = (int)a_vecColMatch.size();
		for (int i= 0; i< nSizeVec; i++) {
			TColMatch         &oTColMatch     = a_vecColMatch[i];

			const KIOColumn*  pIOColumn       = oTColMatch.pIOColumn;
			KEMIODataType     emDataType      = pIOColumn->DataType();
			CString           strIOColumnName = pIOColumn->Name();

			auto itFind = mapFieldValue.find(strIOColumnName);
			if(itFind != mapFieldValue.end())
				continue;

			if( a_row.size() > (size_t)(oTColMatch.nImportColIndex -1) ) 
				strImportValue = a_row[oTColMatch.nImportColIndex -1];
			else
				strImportValue = _T("");

			WriteOutputStream(strImportValue, emDataType, ofs);
		}

		// where 조건에 쓰일
		auto iter  = mapFieldValue.begin();
		auto itEnd = mapFieldValue.end();

		Integer nxValue(0);
		for (; iter != itEnd; ++iter) {
			nxValue = iter->second;
			strImportValue.Format(_T("%I64d"), nxValue);
			WriteOutputStream(strImportValue, KEMIODataTypeInteger, ofs);
		}

		return true;
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		a_bEverErrLine = true;
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("Line : %d =>\t 알 수 없는 오류"), a_nCurrentLine);
		ofErr.Write(strErrorMsg);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);

		a_bEverErrLine = true;
		ofErr.Write(ex->GetErrorMessage());
	} catch (...) {
		TxLogDebugException();

		a_bEverErrLine = true;
		CString strErrorMsg(_T(""));
		strErrorMsg.Format(_T("Line : %d =>\t 알 수 없는 오류"), a_nCurrentLine);
		ofErr.Write(strErrorMsg);
	}

	return false;
}

bool KDlgImportZone::ConfirmPrimaryKey(std::vector<TColMatch> &a_vecColMatch, CSVRow &a_oImportRow, std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType)
{
	a_mapKeyFieldValue.clear();

	CString strImportValue(_T(""));

	int nSizeVec = (int)a_vecColMatch.size();
	for (int i= 0; i< nSizeVec; i++) {
		TColMatch         &oTColMatch = a_vecColMatch[i];

		const KIOColumn*  pIOColumn       = oTColMatch.pIOColumn;
		KEMIODataType     emDataType      = pIOColumn->DataType();
		CString           strIOColumnName = pIOColumn->Name();

		if( a_oImportRow.size() > (size_t)(oTColMatch.nImportColIndex -1) ) 
			strImportValue = a_oImportRow[oTColMatch.nImportColIndex -1];
		else
			strImportValue = _T("");

		bool bPK(false);
		// 조회 key value 추출
		if ( strIOColumnName.CompareNoCase(COLUMN_ZONE_ID) == 0 ) {
			bPK = true;
		}

		if( bPK ) {
			if( emDataType != KEMIODataTypeInteger )
				continue;

			Integer nxValue = _ttoi64(strImportValue);
			a_mapKeyFieldValue.insert(std::make_pair(strIOColumnName, nxValue));
		}
	}

	if(IsWrongIDCount(a_mapKeyFieldValue, a_emImportType)) {
		a_mapKeyFieldValue.clear();
		return false;
	}

	return true;
}

bool KDlgImportZone::IsFindTableData(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType)
{
	auto iter  = a_mapKeyFieldValue.begin();
	auto itEnd = a_mapKeyFieldValue.end();

	Integer nxID(0);
	for(; iter != itEnd; ++iter) {
		nxID = iter->second;
	}

	return IsZoneID(nxID);
}

bool KDlgImportZone::IsZoneID(Integer a_nxValue)
{
	auto iter = m_setZoneID.find( a_nxValue );
	auto end  = m_setZoneID.end();
	if(iter == end) {
		return false;
	}

	return true;
}

bool KDlgImportZone::IsWrongIDCount(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType)
{
	// 추출 ID 갯수 확인
	int  nIdCount      = (int)(a_mapKeyFieldValue.size());
	bool bWrongIdCount = false;

	if( nIdCount != 1) // zone_id
		bWrongIdCount = true;

	return bWrongIdCount;
}

bool KDlgImportZone::CheckDuplicateID(std::map<CString, Integer> &a_mapKeyFieldValue, KEMImportType a_emImportType)
{
	try 
	{
		std::map<CString, Integer>::iterator iter  = a_mapKeyFieldValue.begin();
		std::map<CString, Integer>::iterator itEnd = a_mapKeyFieldValue.end();

		int nKeyCnt(0);
		nKeyCnt = 1; // TABLE_ZONE 은 PK 개수가 하나

		// single
		Integer nxID = -1;
		while( iter != itEnd ) {
			nxID = iter->second;
			++iter;
		}

		if (m_setDuplicateSingleID.find(nxID) == m_setDuplicateSingleID.end())
			m_setDuplicateSingleID.insert(nxID);
		else
			return true;
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		return true;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		return true;
	} catch (...) {
		TxLogDebugException();
		return true;
	}

	return false;
}

#include <string>
void KDlgImportZone::WriteOutputStream(CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs)
{
	try
	{
		double	dValue  = 0.0;
		Integer nxValue = 0;

		if(a_emDataType == KEMIODataTypeDouble)	{
			dValue = _ttof(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
		} else if(a_emDataType == KEMIODataTypeInteger) {
			nxValue = _ttoi64(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
		} else if(a_emDataType == KEMIODataTypeString) {
			const int STR_LENGTH = 200;
			if(a_strImportValue.GetLength() > STR_LENGTH - 1)
				a_strImportValue.Truncate(STR_LENGTH - 1);

			TCHAR     szStrOut[STR_LENGTH];

			_tcscpy_s(szStrOut, a_strImportValue);
			ofs.write( reinterpret_cast<char*>( szStrOut  ), sizeof(TCHAR) * STR_LENGTH );

			//delete  szStrOut;
		}
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		throw ex;
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
		throw ex;
	} catch (...) {
		TxLogDebugException();
		throw 1;
	}
}

void KDlgImportZone::UpdateUserData(ThreadPara* pPara)
{
	TxLogDebugStartMsg();
	std::ifstream ifs(m_strMiddleFile, std::ios::binary);

	int     nImportDataCount = (int)pPara->TKeyInt32[1]; // 양호건수가 Update 전체건수가 된다.
	double	dValue  (0.0);
	Integer nxValue (0);
	CString strValue(_T(""));

	KDBaseConPtr spDBase = m_pTarget->GetDBaseConnection();

	std::vector<KIOColumn*> vecIOColumn; { // 업데이트에 사용되는 최종 컬럼 정보 수집
#pragma region 업데이트에 사용되는 최종 컬럼 정보 수집
		CXTPReportRecords* pRecords = m_ctrlReportColumn.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			CXTPReportRecord*        pRecord     = pRecords->GetAt(i);
			KIOColumn*               pIOColumn      = GetDefIOColumn(pRecord);
			int                      nSelectedIndex = GetImportColumnIndex(pRecord);

			if( nSelectedIndex == 0 || pRecord->IsVisible() == FALSE )
				continue;

			CString strColumnName = pIOColumn->Name();
			auto itFind = m_setKeyFieldName.find(strColumnName); // Key 필드는 제외 (하단에서 키필드 정보 수집)
			if (itFind != m_setKeyFieldName.end())
				continue;

			vecIOColumn.push_back(pIOColumn);
		}
#pragma endregion 업데이트에 사용되는 최종 컬럼 정보 수집
	}
	int nSizeCol = (int)vecIOColumn.size();

	try
	{
		CString strSQLUpdate = GenerateUpdatePrepareQuery(vecIOColumn);

		spDBase->BeginTransaction();
		KPreparedStatementPtr spPrepared = spDBase->PrepareStatement(strSQLUpdate);

		int nloopCount = 0;

		while(!ifs.eof()) {
			if ( ++nloopCount % 500 == 0) {
				CString strMsg;
				strMsg.Format(_T("Updating data rows ( %d / %d )"), nloopCount, nImportDataCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("읽은 파일 라인수 / 총 라인수 :  %d / %d"), nloopCount, nImportDataCount);
				}
				AddStatusMessage( strMsg );
			}

			int nBindCount(1);
			for (int i= 0; i< nSizeCol; i++) {
				KIOColumn*    pColumn       = vecIOColumn[i];
				KEMIODataType emDataType    = pColumn->DataType();
				CString       strColumnName = pColumn->Name();

				if(emDataType == KEMIODataTypeDouble) {
					ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
					if(ifs.eof())
						break;

					spPrepared->BindDouble(nBindCount++, dValue);
				} else if(emDataType == KEMIODataTypeInteger) {
					ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
					if(ifs.eof())
						break;

					spPrepared->BindInt64(nBindCount++, nxValue);
				} else if(emDataType == KEMIODataTypeString) {
					const int STR_LENGTH = 200;
					TCHAR*  szStringIn = new TCHAR[STR_LENGTH];
					ifs.read( reinterpret_cast<char*>( szStringIn ), sizeof(TCHAR) * STR_LENGTH );
					if(ifs.eof())
						break;
					strValue = szStringIn;
					delete szStringIn;

					spPrepared->BindText(nBindCount++, strValue);
				}
			}

			// 조건절에 사용될 키 필드
			auto iter  = m_setKeyFieldName.begin();
			auto itEnd = m_setKeyFieldName.end();
			for (; iter != itEnd; ++iter) {
				ifs.read(reinterpret_cast<char*>(&nxValue), sizeof(Integer));

				if(ifs.eof())
					break;

				spPrepared->BindInt64(nBindCount++, nxValue);
			}

			if(ifs.eof())
				break;

			spPrepared->ExecuteUpdate();
			spPrepared->Reset();
		}

		spDBase->Commit();
		ifs.close();
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);

		spDBase->RollBack();
		ifs.close();

		ThrowException(_T("Failed to Update Binary Data..."));
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);

		spDBase->RollBack();
		ifs.close();

		throw ex;
	} catch (...) {
		TxLogDebugException();

		spDBase->RollBack();
		ifs.close();

		ThrowException(_T("Failed to Update Binary Data..."));
	}

	TxLogDebugEndMsg();
}

CString KDlgImportZone::GenerateUpdatePrepareQuery(std::vector<KIOColumn*> &a_vecIOColumn)
{
	CString  strUpdateQuery(_T(""));{
		strUpdateQuery.Format(_T(" UPDATE %s SET "), m_strTableName);
		for (size_t i= 0; i< a_vecIOColumn.size(); i++)
		{
			KIOColumn* pIOColumn     = a_vecIOColumn[i];
			CString    strColumnName = pIOColumn->Name();

			if(i > 0)
				strUpdateQuery.Append(_T(", "));
			strUpdateQuery.Append(strColumnName);
			strUpdateQuery.Append(_T(" = ? "));
		}
	}

	CString  strWhereQuery (_T("")); { // 조건절
		auto iter  = m_setKeyFieldName.begin();
		auto itEnd = m_setKeyFieldName.end();
		int nIdx(0);
		for (; iter != itEnd; ++iter, nIdx++) {
			if( 0 == nIdx) {
				strWhereQuery.Append(_T(" WHERE "));
			} else {
				strWhereQuery.Append(_T(" AND "));
			}
			strWhereQuery.Append(*iter);
			strWhereQuery.Append(_T(" = ? "));
		}
	}

	CString strPreparedQuery(_T("")); {
		strPreparedQuery.Format(_T(" %s %s "), strUpdateQuery, strWhereQuery);
	}

	return strPreparedQuery;
}

void KDlgImportZone::ErrorFileOpen()
{
	try
	{
		if (QBicFile::FileExist(m_strErrFile) == false){
			return;
		}
		ShellExecute(NULL, _T("open"), m_strErrFile, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}
}

void KDlgImportZone::ValidateUpdateMode( void )
{
	TxLogDebugStartMsg();
	UpdateData(TRUE);
	try
	{
		m_setKeyFieldName.clear();
		CString strColumnName(_T(""));

		if (KImportCommon::IsEmptyTable(m_pTarget, TABLE_NODE) == true) {// true: 데이터무, false : 데이터 존재
			//ThrowException(_T("Node Data를 먼저 입력하세요."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("노드를 먼저 입력해 주세요."));
			}
			else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("Import Node data first !"));
			}
		}

#pragma region Zone 테이블 Id_Filed Null 체크
		CXTPReportRecords* pRecords = m_ctrlReportColumn.GetRecords();
		int nColumnCount			= pRecords->GetCount();
		int nUpdateColums(0);

		for (int i = 0; i < nColumnCount; ++i) {
			CXTPReportRecord*        pRecord        = pRecords->GetAt(i);
			KIOColumn*               pIOColumn      = nullptr;
			int                      nSelectedIndex = GetImportColumnIndex(pRecord);

			pIOColumn     = GetDefIOColumn( pRecord );
			strColumnName = pIOColumn->Name();

			if( pRecord->IsVisible() == FALSE)
				continue;

			CString strMsg(_T(""));
			if (nSelectedIndex == 0) {
				if( strColumnName.CompareNoCase(COLUMN_ZONE_ID) == 0 ) {
					//strMsg.Format(_T("\"%s\" Field는 Null일 수 없습니다."), pIOColumn->Caption());
					if (KmzSystem::GetLanguage() == KEMKorea) {
						strMsg.Format(_T("\"%s\" 필수 입력 컬럼은 Null 값일 수 없습니다 !"), pIOColumn->Caption());
					}
					else { // if (KmzSystem::GetLanguage() == KEMEnglish) {
						strMsg.Format(_T("\"%s\" Required input column cannot be a NULL value !"), pIOColumn->Caption());
					}
					ThrowException(strMsg);
				}
			} else {
				if( strColumnName.CompareNoCase(COLUMN_ZONE_ID) == 0) {
					m_setKeyFieldName.insert(strColumnName);
				} else {
					nUpdateColums++;
				}
			}
		}

		if (0 == nUpdateColums) {
			//ThrowException(_T("매칭된 업데이트 컬럼 정보가 없습니다.\r\n컬럼을 매칭하세요."));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(_T("매칭된 업데이트 컬럼 정보가 없습니다.\r\n컬럼을 매칭하세요."));
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no any matched information.\r\nPlease match the required column first."));
			}
		}
#pragma endregion Zone 테이블 Id_Filed Null 체크
	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
		ThrowException((LPCTSTR)strError);
	} catch(KExceptionPtr ex) {
		TxExceptionPrint(ex);
		ThrowException(ex->GetErrorMessage());
	} catch(...) {
		TxLogDebugException();
		ThrowException(_T("Fail - Validate Update"));
	}

	TxLogDebugEndMsg();
}

bool KDlgImportZone::AlertTableChange()
{
	UpdateData(TRUE);

	// 	CString strMsg       = _T("");
	// 	bool    bContinue    = false;
	// 
	// 	KEMImportType emImportType = GetSelectedImportType(); 
	// 
	// 	if (KEMImportInsert == emImportType) {
	// 		if (IsEmptyTable(TABLE_NODE) == false)
	// 			strMsg = _T("Node 데이터를 Insert 하면 관련된 테이블의 데이터가 삭제됩니다.\n진행하시겠습니까 ?");
	// 		else
	// 			return true;
	// 	} else {
	// 		return true;
	// 	}
	// 
	// 	if ( IDNO == AfxMessageBox(strMsg, MB_YESNO) ) {
	// 		return false; 
	// 	}

	return true;
}

KIOColumn* KDlgImportZone::GetDefIOColumn( CXTPReportRecord* a_pRecord )
{
	CXTPReportRecordItemText* pColumnItemText = (CXTPReportRecordItemText*)a_pRecord->GetItem( _0_COLUMN_DEFFILED_NAME );
	KIOColumn*                pIOColumn		  = (KIOColumn*)(pColumnItemText->GetItemData());
	return pIOColumn;
}

int KDlgImportZone::GetImportColumnIndex( CXTPReportRecord* a_pRecord )
{
	int nColumnIndex = 0;
	KColumnReportRecordItem* pColumnItem = (KColumnReportRecordItem*)a_pRecord->GetItem(_2_COLUMN_IMPORTFIELD_NAME);
	nColumnIndex = pColumnItem->getColumnIndex();
	return nColumnIndex;
}

void KDlgImportZone::RelationTableNotify()
{
	TxLogDebugStartMsg();
	try
	{
		if (nullptr != m_pTable) {
			m_pTable->Notify();
		}

	} catch (int& ex) {
		CString strError(_T(""));
		strError.Format(_T("Error : %d"), ex);
		TxLogDebug((LPCTSTR)strError);
	} catch (KExceptionPtr ex) {
		TxExceptionPrint(ex);
	} catch (...) {
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}

void KDlgImportZone::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow) {
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KDlgImportZone::OnCbnSelchangeCombo4()
{
	if( PriviewLoadCSV() ) {
		InitializeColumnField();
		UpdateColumnRecord();
		InitializePreview();

		ControlDefaultCheck();
	} 
}

void KDlgImportZone::OnCbnSelchangeCombo5()
{
	if( PriviewLoadCSV() ) {
		InitializeColumnField();
		UpdateColumnRecord();
		InitializePreview();

		ControlDefaultCheck();
	} 
}

void KDlgImportZone::OnBnClickedCheck1()
{
	if( PriviewLoadCSV() ) {
		InitializeColumnField();
		UpdateColumnRecord();
		InitializePreview();

		ControlDefaultCheck();
	} 
}
