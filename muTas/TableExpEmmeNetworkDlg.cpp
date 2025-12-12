// TableExpEmmeNetworkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableExpEmmeNetworkDlg.h"
#include "afxdialogex.h"

#include "Target.h"
#include "Scenario.h"
#include "Project.h"


//^#include "DBaseConnector.h"
#include "TableExpEmmeDefineModesDlg.h"

// KTableExpEmmeNetworkDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableExpEmmeNetworkDlg, KResizeDialogEx)

KTableExpEmmeNetworkDlg::KTableExpEmmeNetworkDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableExpEmmeNetworkDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bRunOK(false)
	, m_bDefined(false)
	, m_strFileName(_T(""))
{

}

KTableExpEmmeNetworkDlg::~KTableExpEmmeNetworkDlg()
{
}

void KTableExpEmmeNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILENAME, m_editFile);
	DDX_Control(pDX, IDC_COMBO1,   m_cboNodeUser1);
	DDX_Control(pDX, IDC_COMBO2,   m_cboNodeUser2);
	DDX_Control(pDX, IDC_COMBO3,   m_cboNodeUser3);
	DDX_Control(pDX, IDC_COMBO4,   m_cboLinkUser1);
	DDX_Control(pDX, IDC_COMBO5,   m_cboLinkUser2);
	DDX_Control(pDX, IDC_COMBO6,   m_cboLinkUser3);
}


BEGIN_MESSAGE_MAP(KTableExpEmmeNetworkDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KTableExpEmmeNetworkDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableExpEmmeNetworkDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &KTableExpEmmeNetworkDlg::OnBnClickedBtnDefineModes)
END_MESSAGE_MAP()


// KTableExpEmmeNetworkDlg 메시지 처리기입니다.

void KTableExpEmmeNetworkDlg::InitNodeComboCtrl( void )
{
	m_cboNodeUser1.ResetContent();
	m_cboNodeUser2.ResetContent();
	m_cboNodeUser3.ResetContent();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_cboNodeUser1.InsertString(-1, _T("----------------선택----------------"));
		m_cboNodeUser2.InsertString(-1, _T("----------------선택----------------"));
		m_cboNodeUser3.InsertString(-1, _T("----------------선택----------------"));
	}
	else
	{
		m_cboNodeUser1.InsertString(-1, _T("------------Select------------"));
		m_cboNodeUser2.InsertString(-1, _T("------------Select------------"));
		m_cboNodeUser3.InsertString(-1, _T("------------Select------------"));
	}

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_NODE);

	const KIOColumns* pColumns = pTable->Columns();

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

		nIndex = m_cboNodeUser1.InsertString(-1, pColumn->Caption());
		m_cboNodeUser1.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());

		nIndex = m_cboNodeUser2.InsertString(-1, pColumn->Caption());
		m_cboNodeUser2.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());

		nIndex = m_cboNodeUser3.InsertString(-1, pColumn->Caption());
		m_cboNodeUser3.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());
	}

	m_cboNodeUser1.SetCurSel(0);
	m_cboNodeUser2.SetCurSel(0);
	m_cboNodeUser3.SetCurSel(0);
}

void KTableExpEmmeNetworkDlg::InitLinkComboCtrl( void )
{
	m_cboLinkUser1.ResetContent();
	m_cboLinkUser2.ResetContent();
	m_cboLinkUser3.ResetContent();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_cboLinkUser1.InsertString(-1, _T("----------------선택----------------"));
		m_cboLinkUser2.InsertString(-1, _T("----------------선택----------------"));
		m_cboLinkUser3.InsertString(-1, _T("----------------선택----------------"));
	}
	else
	{
		m_cboLinkUser1.InsertString(-1, _T("------------Select------------"));
		m_cboLinkUser2.InsertString(-1, _T("------------Select------------"));
		m_cboLinkUser3.InsertString(-1, _T("------------Select------------"));
	}

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_LINK);

	const KIOColumns* pColumns = pTable->Columns();

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

		nIndex = m_cboLinkUser1.InsertString(-1, pColumn->Caption());
		m_cboLinkUser1.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());

		nIndex = m_cboLinkUser2.InsertString(-1, pColumn->Caption());
		m_cboLinkUser2.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());

		nIndex = m_cboLinkUser3.InsertString(-1, pColumn->Caption());
		m_cboLinkUser3.SetItemData(nIndex, (DWORD_PTR)pColumn->Name());
	}

	m_cboLinkUser1.SetCurSel(0);
	m_cboLinkUser2.SetCurSel(0);
	m_cboLinkUser3.SetCurSel(0);
}


BOOL KTableExpEmmeNetworkDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitNodeComboCtrl();
	InitLinkComboCtrl();

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


void KTableExpEmmeNetworkDlg::OnBnClickedOk()
{
	m_editFile.GetWindowText( m_strFileName );

	if (m_strFileName.IsEmpty())
	{
		AfxMessageBox( _T("파일명이 비어있습니다.") );
		return;
	}

	if (!m_bDefined)
	{
		AfxMessageBox( _T("통행수단을 정의해 주세요."));
		return;
	}
	
	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(ExpEmmeNetworkThreadCaller, this, false, nLang);

	if (m_bRunOK)
	{
		AfxMessageBox(_T("완료되었습니다."));
	}
	else
	{
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
		return;
	}

	KResizeDialogEx::OnOK();
}

void KTableExpEmmeNetworkDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}
unsigned __stdcall KTableExpEmmeNetworkDlg::ExpEmmeNetworkThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	KTableExpEmmeNetworkDlg* pDlg = (KTableExpEmmeNetworkDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KTableExpEmmeNetworkDlg::GenNodeExportQuery( CString& ar_strQuery )
{
	ar_strQuery = _T("Select ");
	ar_strQuery.Append(_T("'a'||(case when node_type = 0 then '*' else '' end)||' '||IFNULL(node_id, '0')||' '||ROUND(IFNULL(x, '0'), 3)||' '||ROUND(IFNULL(y, '0'), 3)"));

	//User Define #1
	int     nIndex = m_cboNodeUser1.GetCurSel();
	CString strColumnName;

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboNodeUser1.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	//User Define #2
	nIndex = m_cboNodeUser2.GetCurSel();

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboNodeUser2.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	//User Define #3
	nIndex = m_cboNodeUser3.GetCurSel();

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboNodeUser3.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	ar_strQuery.Append(_T(" From node"));
}

void KTableExpEmmeNetworkDlg::GenLinkExportQuery( CString& ar_strQuery )
{
	ar_strQuery = _T("Select ");
	//ar_strQuery.Append(_T("'a'||' '||IFNULL(fnode_id, '0')||' '||IFNULL(tnode_id, '0')||' '||IFNULL(length, '0')||' '||'c,p'||' '||IFNULL(link_type, '0')||' '||IFNULL(lane, '0')||' '||IFNULL(function_id, '0')"));
	ar_strQuery.Append(_T("'a'||' '||IFNULL(fnode_id, '0')||' '||IFNULL(tnode_id, '0')||' '||ROUND(IFNULL(length, '0'), 3)||' '||"));

	//define modes;
	CString strModes(_T(""));

	AutoType beginIter = m_mapDefineModes.begin();
	AutoType endIter   = m_mapDefineModes.end();

	if ((int)m_mapDefineModes.size() != 0)
	{
		strModes = _T("(case link_type ");
	}

	CString strAddMode(_T(""));

	while (beginIter != endIter)
	{
		int     nLinkType     = beginIter->first;
		CString strDefineMode = beginIter->second;

		strAddMode.Format(_T("when %d then '%s' "), nLinkType, strDefineMode);
		strModes.Append(strAddMode);

		++beginIter;
	}

	strModes.Append(_T("else null end)"));

	ar_strQuery.Append(strModes);
	ar_strQuery.Append(_T("||' '||IFNULL((case link_type when 0 then (select max(link_Type) + 1 from link) else link_type end), '0')"));
	ar_strQuery.Append(_T("||' '||IFNULL((case when lane > 9 then 9.9 else lane end), '0')||' '||IFNULL(function_id, '0')"));

	//User Define #1
	int     nIndex = m_cboLinkUser1.GetCurSel();
	CString strColumnName;

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboLinkUser1.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	//User Define #2
	nIndex = m_cboLinkUser2.GetCurSel();

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboLinkUser2.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	//User Define #3
	nIndex = m_cboLinkUser3.GetCurSel();

	ar_strQuery.Append(_T("||' '||"));

	if (nIndex == 0)
	{
		ar_strQuery.Append(_T("'0'"));
	}
	else
	{
		strColumnName = (LPCTSTR)m_cboLinkUser3.GetItemData(nIndex);
		ar_strQuery.Append(_T("ROUND(IFNULL("));
		ar_strQuery.Append(strColumnName);
		ar_strQuery.Append(_T(", '0'), 3)"));
	}

	ar_strQuery.Append(_T(" From link"));
}

void KTableExpEmmeNetworkDlg::WriteEmmeHeader( CStdioFileEx& ar_outFile )
{
	CString strWriteLine;

	KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();
	KProject*  pProject  = (KProject*)pScenario->GetParentObject();

	strWriteLine.Format(_T("c Project : %s\r\n"), pProject->GetName());
	ar_outFile.WriteString(strWriteLine);
	
	strWriteLine.Format(_T("c Scenario : %s\r\n"), pScenario->GetName());
	ar_outFile.WriteString(strWriteLine);

	strWriteLine.Format(_T("c Target : %s\r\n"), m_pTarget->GetName());
	ar_outFile.WriteString(strWriteLine);
}

void KTableExpEmmeNetworkDlg::WriteEmmeNode( CStdioFileEx& ar_outFile )
{
	CString strWriteLine;

	//Node 헤더 작성
	strWriteLine = _T("t nodes init\r\n");
	ar_outFile.WriteString(strWriteLine);

	CString strQuery(_T(""));
	GenNodeExportQuery(strQuery);

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResult->Next())
	{
		strWriteLine = spResult->GetValueString(0);
		strWriteLine.Append(_T("\r\n"));

		ar_outFile.WriteString(strWriteLine);
	}
}

void KTableExpEmmeNetworkDlg::WriteEmmeLink( CStdioFileEx& ar_outFile )
{
	CString strWriteLine;

	//Link 헤더 작성
	strWriteLine = _T("t links init\r\n");
	ar_outFile.WriteString(strWriteLine);

	CString strQuery(_T(""));
	GenLinkExportQuery(strQuery);
	
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

	while (spResultSet->Next())
	{
		strWriteLine = spResultSet->GetValueString(0);
		strWriteLine.Append(_T("\r\n"));

		ar_outFile.WriteString(strWriteLine);
	}
}

void KTableExpEmmeNetworkDlg::Run( void )
{
	CStdioFileEx outFile;

	try
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("네트워크 파일을 내보내는 중..."));
		}
		else {
			AddStatusMessage(_T("Exporting Network Data..."));
		}

		UINT nWriteFlags = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		UINT nCodePage = 949;
		
		outFile.SetCodePage( nCodePage );

		if (outFile.Open(m_strFileName, nWriteFlags) == FALSE)
			ThrowException(_T("file open error"));
		
		WriteEmmeHeader(outFile);
		outFile.WriteString(_T("\r\n"));

		WriteEmmeNode(outFile);
		outFile.WriteString(_T("\r\n"));

		WriteEmmeLink(outFile);

		outFile.Close();
		m_bRunOK = true;
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		m_bRunOK = false;
	}
	catch (...)
	{
		outFile.Close();
		TxLogDebugException();
		m_bRunOK = false;
	}
}

void KTableExpEmmeNetworkDlg::OnBnClickedBtnDefineModes()
{
	KTableExpEmmeDefineModesDlg oDlg(m_pTarget);

	if (oDlg.DoModal() == IDOK)
	{
		m_mapDefineModes.clear();
		oDlg.GetDefineModes(m_mapDefineModes);

		m_bDefined = true;
	}
}

void KTableExpEmmeNetworkDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}