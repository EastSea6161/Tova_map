// TableExpEmmeMatrixDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableExpEmmeMatrixDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "Scenario.h"
#include "Project.h"


//#include "StdioFileEx.h"
//^#include "DBaseConnector.h"

// KTableExpEmmeMatrixDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableExpEmmeMatrixDlg, KResizeDialogEx)

KTableExpEmmeMatrixDlg::KTableExpEmmeMatrixDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableExpEmmeMatrixDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_strFileName(_T(""))
{
}

KTableExpEmmeMatrixDlg::~KTableExpEmmeMatrixDlg()
{
}

void KTableExpEmmeMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_editFile);
	DDX_Control(pDX, IDC_COMBO1,   m_cboMatrix);
	DDX_Control(pDX, IDC_COMBO2,   m_cboColumn);
}


BEGIN_MESSAGE_MAP(KTableExpEmmeMatrixDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTableExpEmmeMatrixDlg::OnCbnSelChangeMatrix)
	ON_BN_CLICKED(IDOK, &KTableExpEmmeMatrixDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableExpEmmeMatrixDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTableExpEmmeMatrixDlg 메시지 처리기입니다.

void KTableExpEmmeMatrixDlg::OnCbnSelChangeMatrix( void )
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

BOOL KTableExpEmmeMatrixDlg::OnInitDialog()
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

void KTableExpEmmeMatrixDlg::InitMatrixComboCtrl( void )
{
	m_cboMatrix.ResetContent();

	KIOTables* pTables = m_pTarget->Tables();
	int              nTables = pTables->GetCount();
	int              nIndex(0);

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

void KTableExpEmmeMatrixDlg::InitColumnComboCtrl( void )
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

void KTableExpEmmeMatrixDlg::OnBnClickedOk()
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

		KIOTable* pTable   = (KIOTable*)m_cboMatrix.GetItemData(m_cboMatrix.GetCurSel());
		int       nRecords = pTable->RecordCount();

		if (nRecords == 0)
			ThrowException(_T("해당 Matrix에 정보가 없습니다."));
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ExpEmmeMatrixThreadCaller, this, false, nLang);

		if (!m_bThreadOK)
			ThrowException(m_strErrMsg);

		AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));

		KResizeDialogEx::OnOK();
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}	
}


void KTableExpEmmeMatrixDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KTableExpEmmeMatrixDlg::Run( void )
{
	m_bThreadOK = true;
	CStdioFileEx outFile;

	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("O/D 파일을 내보내는 중..."));
		}
		else {
			AddStatusMessage(_T("Exporting O/D Matrix Data..."));
		}

		UINT nWriteFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		UINT nCodePage = 949;

		outFile.SetCodePage( nCodePage );

		if (outFile.Open(m_strFileName, nWriteFlags) == FALSE)
			ThrowException(_T("file open error"));

		WriteEmmeHeader(outFile);
		WriteEmmeMatrix(outFile);

		outFile.Close();
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		m_strErrMsg = ex->GetErrorMessage();
		m_bThreadOK = false;
	}
	catch (...)
	{
		outFile.Close();
		m_strErrMsg = _T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오.");
		m_bThreadOK = false;
	}
}

void KTableExpEmmeMatrixDlg::WriteEmmeHeader( CStdioFileEx& ar_outFile )
{
	CString strWriteLine = _T("t matrices\r\n");

	ar_outFile.WriteString(strWriteLine);

	int nIndex = m_cboColumn.GetCurSel();

	KIOColumn* pColumn = (KIOColumn*)m_cboColumn.GetItemData(nIndex);

	CString strColName = pColumn->Name();

	if (strColName.GetLength() > 6)
		strColName = strColName.Left(6);

	strWriteLine.Format(_T("a matrix=mf%02d %s 0\r\n"), nIndex, strColName);

	ar_outFile.WriteString(strWriteLine);
}

void KTableExpEmmeMatrixDlg::WriteEmmeMatrix( CStdioFileEx& ar_outFile )
{
	KIOTable*  pTable  = (KIOTable*)m_cboMatrix.GetItemData(m_cboMatrix.GetCurSel());
	KIOColumn* pColumn = (KIOColumn*)m_cboColumn.GetItemData(m_cboColumn.GetCurSel());

	CString strQuery;
	
	strQuery.Format(_T("Select %s||' '||%s||' : '||ROUND(%s, 3) From %s"),
		COLUMN_OZONE, COLUMN_DZONE, pColumn->Name(), pTable->Name());

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResultSet->Next())
	{
		ar_outFile.WriteString(spResultSet->GetValueString(0));
		ar_outFile.WriteString(_T("\r\n"));
	}
}

void KTableExpEmmeMatrixDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();

	if(spProgressWindow)
		spProgressWindow->SetStatus(a_strMsg);
}

unsigned __stdcall KTableExpEmmeMatrixDlg::ExpEmmeMatrixThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KTableExpEmmeMatrixDlg* pDlg = (KTableExpEmmeMatrixDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}
