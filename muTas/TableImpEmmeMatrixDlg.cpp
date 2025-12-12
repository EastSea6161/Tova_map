// TableImpEmmeMatrixDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableImpEmmeMatrixDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "Scenario.h"
#include "Project.h"

#include "BulkDBaseNode.h"
#include "KxBulkDbase.h"

const CString IMPORT_MATRIX_FILE_NAME = _T("impMatrixData.dat");
// KTableImpEmmeMatrixDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableImpEmmeMatrixDlg, KResizeDialogEx)

KTableImpEmmeMatrixDlg::KTableImpEmmeMatrixDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableImpEmmeMatrixDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_strFileName(_T(""))
	, m_cSeparator(_T(' '))
{

}

KTableImpEmmeMatrixDlg::~KTableImpEmmeMatrixDlg()
{
}

void KTableImpEmmeMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_editFile);
	DDX_Control(pDX, IDC_COMBO1,   m_cboMatrix);
	DDX_Control(pDX, IDC_COMBO2,   m_cboColumn);
}


BEGIN_MESSAGE_MAP(KTableImpEmmeMatrixDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTableImpEmmeMatrixDlg::OnCbnSelChangeMatrix)
	ON_BN_CLICKED(IDOK, &KTableImpEmmeMatrixDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableImpEmmeMatrixDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTableImpEmmeMatrixDlg 메시지 처리기입니다.

void KTableImpEmmeMatrixDlg::InitMatrixComboCtrl( void )
{
	m_cboMatrix.ResetContent();

	KIOTables* pTables = m_pTarget->Tables();
	int        nTables = pTables->GetCount();
	int        nIndex(0);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboMatrix.InsertString(-1, _T("----------------선택----------------"));
	}
	else {
		nIndex = m_cboMatrix.InsertString(-1, _T("------Select------"));
	}
	m_cboMatrix.SetItemData(nIndex, (DWORD_PTR)NULL);

	for (int i = 0; i < nTables; ++i)
	{
		KIOTable* pTable = pTables->GetTableAt(i);

		if (!(pTable->TableDisplayType() == KEMIOTableDisplayTypeMatrix || pTable->TableDisplayType() == KEMIOTableDisplayTypeParameter))
			continue;

		nIndex = m_cboMatrix.InsertString(-1, pTable->Caption());
		m_cboMatrix.SetItemData(nIndex, (DWORD_PTR)pTable);
	}

	m_cboMatrix.SetCurSel(0);
}

void KTableImpEmmeMatrixDlg::OnCbnSelChangeMatrix( void )
{
	m_cboColumn.ResetContent();

	int nIndex = m_cboMatrix.GetCurSel();

	if (nIndex == 0)
	{
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		InitColumnComboCtrl();
	}
}

void KTableImpEmmeMatrixDlg::InitColumnComboCtrl( void )
{
	int       nMatrixIndex = m_cboMatrix.GetCurSel();
	KIOTable* pTable       = (KIOTable*)m_cboMatrix.GetItemData(nMatrixIndex);

	const KIOColumns* pColumns = pTable->Columns();

	int nColumns = pColumns->ColumnCount();
	int nIndex(0);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboColumn.InsertString(-1, _T("--------------선택--------------"));
	}
	else {
		nIndex = m_cboColumn.InsertString(-1, _T("------Select------"));
	}

	for (int i = 0; i < nColumns; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		if (pColumn->PrimaryKey() || pColumn->DataType() != KEMIODataTypeDouble)
			continue;

		nIndex = m_cboColumn.InsertString(-1, pColumn->Caption());
		m_cboColumn.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cboColumn.SetCurSel(0);
}

BOOL KTableImpEmmeMatrixDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitMatrixComboCtrl();

	TCHAR szFilter[] = _T("Emme Files (*.in)|*.in|Txt Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	m_editFile.Initialize( this, BES_XT_CHOOSEFILE );
	m_editFile.SetDlgOpenFile( FALSE );
	m_editFile.SetReadOnly( TRUE );
	m_editFile.SetDlgTitle( _T("Select file") );
	m_editFile.SetDlgFilter( szFilter );

	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject  = (KProject*)pScenario->GetParentObject();

	m_editFile.SetDlgInitialDir(pProject->GetLocation());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableImpEmmeMatrixDlg::OnBnClickedOk()
{
	try
	{
		m_editFile.GetWindowText(m_strFileName);

		if (m_strFileName.IsEmpty())
			ThrowException(_T("file name is empty..."));

		if (m_cboMatrix.GetCurSel() == 0)
			ThrowException(_T("Matrix를 선택하세요."));

		if (m_cboColumn.GetCurSel() == 0)
			ThrowException(_T("Column을 선택하세요."));

		KBulkDBaseNode::ZoneNodeSetQuery(m_pTarget, m_setZoneID);
		
		if (m_setZoneID.size() == 0)
			ThrowException(_T("Zone Data가 없습니다."));
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ImpEmmeMatrixThreadCaller, this, false, nLang);

		if (!m_bThreadOK)
		{
			CString strMsg;
			strMsg.Format(_T("Import에 실패했습니다.\r\n%s"), m_strErrMsg);

			ThrowException(strMsg);
		}			

		KIOTable* pTable = (KIOTable*)m_cboMatrix.GetItemData(m_cboMatrix.GetCurSel());
		pTable->Notify();
		
		AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));

		KResizeDialogEx::OnOK();
	}
	catch (KExceptionPtr ex)
	{
		WriteErrorLog(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KTableImpEmmeMatrixDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KTableImpEmmeMatrixDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if(spProgressWindow)
		spProgressWindow->SetStatus(a_strMsg);
}

unsigned __stdcall KTableImpEmmeMatrixDlg::ImpEmmeMatrixThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KTableImpEmmeMatrixDlg* pDlg = (KTableImpEmmeMatrixDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KTableImpEmmeMatrixDlg::Run( void )
{
	m_bThreadOK = true;
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	
	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("파일 불러오는 중..."));
		}
		else {
			AddStatusMessage(_T("Reading Data..."));
		}

		spDBaseConnection->BeginTransaction();

		Text2Binary();
        
		ImportMatrixData(spDBaseConnection);

		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		m_strErrMsg = ex->GetErrorMessage();
		m_bThreadOK = false;
	}
	catch (...)
	{
		spDBaseConnection->RollBack();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
		m_bThreadOK = false;
	}
}

void KTableImpEmmeMatrixDlg::WriteErrorLog( CString a_strMsg )
{
	UINT         nOpenModeErrFile  = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CString      strTargetLocation = m_pTarget->GetLocation();
	CString      strErrFileName    = strTargetLocation + _T("\\") + IMPORT_ERROR_FILE_NAME;
	CStdioFileEx ofErr(strErrFileName, nOpenModeErrFile);

	try
	{
		ofErr.WriteString(a_strMsg);
		ofErr.WriteString(_T("\r\n"));

		ofErr.Close();
	}
	catch (...)
	{
		ofErr.Close();
		AfxMessageBox(_T("unexpected error - KTableImpEmmeMatrixDlg::WriteErrorLog"));
	}
}

void KTableImpEmmeMatrixDlg::Text2Binary( void )
{
	CString strErrMsg;

	UINT nOpenMode = CFile::modeRead   | CFile::typeText  | CFile::shareDenyNone;
	UINT nCodePage = 949;
		
	remove((CStringA)IMPORT_MATRIX_FILE_NAME);

	CStdioFileEx  csvFile;
	
	std::ofstream ofs(IMPORT_MATRIX_FILE_NAME, std::ios::binary);

	try
	{
		csvFile.SetCodePage( nCodePage );

		if( csvFile.Open( m_strFileName, nOpenMode ) == FALSE )
			ThrowException(_T("File Open Error"));

		int nCurrentLine(0);

		CString strReadLine(_T(""));
		bool    bIsData      = false;
		CString strTemp;

		//header까지 읽고 Skip...
		while (!bIsData)
		{
			if (!csvFile.ReadString(strReadLine))
				break;
			
			++nCurrentLine;

			int nPos = strReadLine.Find(_T("a matrix"));
			
			if (nPos != -1)
				bIsData = true;
		}

		if (!bIsData)
			ThrowException(_T("Header가 정의되어 있지 않습니다."));
		
		m_nDataCount = 0;
		CString strOriginID;
		int     nIndex;

		//Data Text를 Binary로 저장(ZoneID 매칭되는지 검증)
		while (csvFile.ReadString(strReadLine))
		{
			++nCurrentLine;
			strReadLine.Trim();

			if (strReadLine.IsEmpty())
				continue;

			//#1.첫 공백을 찾아 좌측을 OrginID로 Setting
			nIndex = strReadLine.Find(m_cSeparator);

			if (nIndex == -1)
			{
				strErrMsg.Format(_T("Line : %d => Origin_ID 정보를 읽을 수 없습니다."), nCurrentLine);
				ThrowException(strErrMsg);
			}

			strOriginID = strReadLine.Left(nIndex);

			if (!IsValidZoneID(strOriginID))
			{
				strErrMsg.Format(_T("Line : %d => Zone 테이블에 존재하지 않는 Zone_ID('%s')입니다."), nCurrentLine, strOriginID);
				ThrowException(strErrMsg);
			}
			
			//#2. 해당 라인에서 OriginID를 제거함
			strReadLine = strReadLine.Right(strReadLine.GetLength() - nIndex);
			strReadLine.Trim();

			//#3. EmmeMatrix의 기본 구분자가 ':'일 경우(cf: O D : V D : V일때) 공백으로 치환함
			strReadLine.Replace(_T(':'), _T(' '));

			RemoveDoubleSpace(strReadLine);

			CSVRow row;
			ParseCSVLineString(strReadLine, row);
			
			//#4. DZone, Value로 값이 구성되므로 row.size()는 짝수여야함
			if ((int)row.size() % 2 != 0)
			{
				strErrMsg.Format(_T("Line : %d => 데이터 오류\r\n"), nCurrentLine);
				ThrowException(strErrMsg);
			}
			
			Line2Binary(nCurrentLine, strOriginID, row, ofs);

			++m_nDataCount;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		csvFile.Close();
		ofs.close();
		
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		csvFile.Close();
		ofs.close();		
		
		throw 1;
	}
}

void KTableImpEmmeMatrixDlg::ParseCSVLineString( CString& strLine, CSVRow& row )
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

void KTableImpEmmeMatrixDlg::Line2Binary( int a_nLine, CString a_strOriginID, CSVRow a_row, std::ofstream& ofs )
{
	CString strErrMsg;

	try
	{
		CString strDZoneID, strValue;
		int     nSize = a_row.size();

		//DZone Value에 대한 검증
		for (int i = 0; i < nSize; ++(++i))
		{
			strDZoneID = a_row[i];
			strValue   = a_row[i + 1];

			if (!IsValidZoneID(strDZoneID))
			{
				strErrMsg.Format(_T("Line : %d => Zone 테이블에 존재하지 않는 Zone_ID('%s')입니다."), a_nLine, strDZoneID);
				ThrowException(strErrMsg);
			}

			WriteOutputStream(a_strOriginID, KEMIODataTypeInteger, ofs);
			WriteOutputStream(strDZoneID,    KEMIODataTypeInteger, ofs);
			WriteOutputStream(strValue,      KEMIODataTypeDouble,  ofs);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		strErrMsg.Format(_T("Line : %d => 데이터를 확인해 주세요."), a_nLine);
	}
}

void KTableImpEmmeMatrixDlg::WriteOutputStream( CString a_strImportValue, KEMIODataType a_emDataType, std::ofstream& ofs )
{
	try
	{
		double	dValue  = 0.0;
		Integer nxValue = 0;

		if(a_emDataType == KEMIODataTypeDouble)
		{
			dValue = _ttof(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&dValue), sizeof(double));
		}
		else if(a_emDataType == KEMIODataTypeInteger)
		{
			nxValue = _ttoi64(a_strImportValue);
			ofs.write(reinterpret_cast<char*>(&nxValue), sizeof(Integer));
		}
		else if(a_emDataType == KEMIODataTypeString)
		{
			const int STR_LENGTH = 200;

			if(a_strImportValue.GetLength() > STR_LENGTH - 1)
				a_strImportValue.Truncate(STR_LENGTH - 1);

			TCHAR     szStrOut[STR_LENGTH];

			_tcscpy_s(szStrOut, a_strImportValue);
			ofs.write( reinterpret_cast<char*>( szStrOut  ), sizeof(TCHAR) * STR_LENGTH );
		}
	}
	catch (...)
	{
		TxLogDebugException();
		throw;
	}
}

void KTableImpEmmeMatrixDlg::ImportMatrixData( KDBaseConPtr spDBaseConnection )
{
	KIOTable*  pTable   = (KIOTable*)m_cboMatrix.GetItemData(m_cboMatrix.GetCurSel());
	KIOColumn* pColumn  = (KIOColumn*)m_cboColumn.GetItemData(m_cboColumn.GetCurSel());

	std::ifstream ifs(IMPORT_MATRIX_FILE_NAME, std::ios::binary);

	double          dValue;
	CString         strValue;
	Integer         nxOZoneID;
	Integer         nxDZoneID;
	KSDoubleRecord* pRecord;

	try
	{
		KIOColumns oImportColumn;
		oImportColumn.AddNewColumn(pColumn);

		KxBulkDbase oBulkDBase(spDBaseConnection, pTable, &oImportColumn);
		pRecord = new KSDoubleRecord(oImportColumn.ColumnCount());

		int     nLoopCount(1);
		CString strMsg = _T("Saving Data...");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("데이터 저장 중..."));
		}
		AddStatusMessage( strMsg );

		while (!ifs.eof())
		{
			if (nLoopCount++ % 500 == 0)
			{
				strMsg.Format(_T("update matrix data...( %d / %d )"), nLoopCount, m_nDataCount);
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMsg.Format(_T("읽은 파일 라인수 / 총 라인수 :  ( %d / %d )"), nLoopCount, m_nDataCount);
				}
				AddStatusMessage( strMsg );
			}

			int nBindCount(1);

			//#0 - OZoneID
			ifs.read(reinterpret_cast<char*>(&nxOZoneID), sizeof(Integer));
			if(ifs.eof()) break;
			//#1 - DZoneID
			ifs.read(reinterpret_cast<char*>(&nxDZoneID), sizeof(Integer));
			if(ifs.eof()) break;
			//#2 - Value
			ifs.read(reinterpret_cast<char*>(&dValue), sizeof(double));
			if(ifs.eof()) break;

			pRecord->SetAt(0, dValue);

			oBulkDBase.ImportData(nxOZoneID, nxDZoneID, pRecord);
		}

		oBulkDBase.ExecuteFullOuterJoin();
		delete pRecord;
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		delete pRecord;
		throw  ex;
	}
	catch (...)
	{
		delete pRecord;
		throw;
	}
}

void KTableImpEmmeMatrixDlg::RemoveDoubleSpace( CString& strLine )
{
	try
	{
		strLine.Replace(_T("  "), _T(" "));

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

void KTableImpEmmeMatrixDlg::RemoveSpaceColonOnLeft( CString& a_strLine )
{
	try
	{
		CString strTemp = a_strLine;
		int     nResult = 1;

		while (nResult != 0)
		{
			nResult = strTemp.Replace(_T(" :"), _T(":"));
		}

		a_strLine = strTemp;
	}
	catch(KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("unexpected error : KTableImpEmmeMatrixDlg::RemoveSpaceColonOnLeft"));
	}
}

void KTableImpEmmeMatrixDlg::RemoveSpaceColonOnRight( CString& a_strLine )
{
	try
	{
		CString strTemp = a_strLine;
		int     nResult = 1;

		while (nResult != 0)
		{
			nResult = strTemp.Replace(_T(": "), _T(":"));
		}

		a_strLine = strTemp;
	}
	catch(KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		ThrowException(_T("unexpected error : KTableImpEmmeMatrixDlg::RemoveSpaceColonOnRight"));
	}
}

bool KTableImpEmmeMatrixDlg::IsValidZoneID( CString a_strZoneID )
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		if (m_setZoneID.find(nxZoneID) == m_setZoneID.end())
		{
			return false;
		}

		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}
