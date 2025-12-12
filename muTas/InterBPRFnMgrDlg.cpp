// InterBPRFnMgrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterBPRFnMgrDlg.h"
#include "afxdialogex.h"

//#include "StdioFileEx.h"
//^^#include "SEDData.h"
#include "DBaseAssignment.h"
#include "DBaseConnector.h"
#include "Target.h"

// KInterBPRFnMgrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterBPRFnMgrDlg, KDialogEx)

KInterBPRFnMgrDlg::KInterBPRFnMgrDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KInterBPRFnMgrDlg::IDD, pParent),
	m_strImportFileName(_T("")),
	m_strFnName(_T("")),
	m_nFnGroupID(0),
	m_pTarget(a_pTarget)
{
}

KInterBPRFnMgrDlg::~KInterBPRFnMgrDlg()
{
}

void KInterBPRFnMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Text(pDX, IDC_EDIT_FN_NAME, m_strFnName);
	DDX_Control(pDX, IDC_COMBO, m_comboFn);
}


BEGIN_MESSAGE_MAP(KInterBPRFnMgrDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KInterBPRFnMgrDlg::OnRadioClicked)
	ON_BN_CLICKED(ID_BTN_IMPORT, &KInterBPRFnMgrDlg::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDOK, &KInterBPRFnMgrDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO, &KInterBPRFnMgrDlg::OnSelchangeCombo)
END_MESSAGE_MAP()


// KInterBPRFnMgrDlg 메시지 처리기입니다.


BOOL KInterBPRFnMgrDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
	pRadio->SetCheck(TRUE);

	UpdateControlState();
	InitReportCtrl();
	GetLinkFunctionID();
	UpdateInitReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterBPRFnMgrDlg::OnRadioClicked( UINT nID )
{
	UpdateControlState();
}

void KInterBPRFnMgrDlg::UpdateControlState( void )
{
	CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));

	if(pRadio->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_FN_NAME)->ShowWindow(true);

		GetDlgItem(IDC_STATIC_LIST)->ShowWindow(false);
		GetDlgItem(IDC_COMBO)->ShowWindow(false);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_FN_NAME)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_LIST)->ShowWindow(true);
		GetDlgItem(IDC_COMBO)->ShowWindow(true);

		InitComboBoxFn();		
	}
}

void KInterBPRFnMgrDlg::InitReportCtrl( void )
{
	CXTPReportColumn* pReportColumn = NULL;
	int nColumnItemIndex(0);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("Function No."), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pReportColumn->SetCaption(_T("함수번호"));
    }

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("α"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);


	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("β"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("a"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	pReportColumn = new CXTPReportColumn(nColumnItemIndex++, _T("b"), 100);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_RIGHT);

	m_wndReport.AllowEdit(TRUE);
}

void KInterBPRFnMgrDlg::UpdateReportCtrl( void )
{
	int nRecordCount = m_vecRows.size();

	for (int i = 0; i < nRecordCount; ++i)
	{
		std::vector<CString> row = m_vecRows[i];
		int nColumnCount = row.size();

		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		__int64 nxNo = _ttoi64(row[0]);
		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nxNo));
		pRecordItem->SetEditable(FALSE);

		double dAlpha = _ttof(row[1]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%g")));

		double dBeta = _ttof(row[2]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%g")));

		double dA = _ttof(row[3]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dA, _T("%g")));

		double dB = _ttof(row[4]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dB, _T("%g")));
	}
	m_wndReport.Populate();
}

void KInterBPRFnMgrDlg::OnBnClickedBtnImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Select File, Read File, Update Record;
	TCHAR szFilter[] = _T("(*.txt)|*.txt|");

	CFileDialog fileDlg(true, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);

	if( fileDlg.DoModal() != IDOK )
		return;

	m_strImportFileName = fileDlg.GetPathName();
	ReadBPRTextFile();
	UpdateReportCtrl();
}

void KInterBPRFnMgrDlg::ReadBPRTextFile( void )
{
	UINT nOpenMode = CFile::modeRead | CFile::typeText | CFile::shareDenyNone;
	UINT nCodePage = 949;
	CStdioFileEx csvFile;
	try
	{
		m_vecRows.clear();

		CString strReadLine;
		csvFile.SetCodePage(nCodePage);

		if(csvFile.Open(m_strImportFileName, nOpenMode) != TRUE)
			ThrowException(_T("File Open Error"));

		while (csvFile.ReadString(strReadLine))
		{
			CString strRow, strQuote;
			int nSub(0);
			std::vector<CString> vecRow;
			bool bQuote = false;
			while ( AfxExtractSubString(strRow, strReadLine, nSub++, _T('\t')))
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
						vecRow.push_back(strRow);
					}
				}
				else
				{
					TCHAR chFirst, chLast;
					if( strRow.IsEmpty() == TRUE )
					{
						vecRow.push_back( strRow );
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
							vecRow.push_back(strRow);
						}
					}
					else
					{
						chFirst = strRow.GetAt( 0 );
						chLast = strRow.GetAt( strRow.GetLength() - 1 );

						if( (chFirst == _T('"')) && (chLast == _T('"')) )
						{
							strRow.Remove( _T('"') );
							vecRow.push_back( strRow );
						}
						else if( chFirst == _T('"') )
						{
							bQuote = true;
							strQuote = strRow;
						}
						else
						{
							vecRow.push_back( strRow );
						}
					}
				}
			}
			m_vecRows.push_back(vecRow);
		}
		csvFile.Close();
	}
	catch(KExceptionPtr ex)
	{
		csvFile.Close();
		TxLogDebug(ex->GetErrorMessage());
		ThrowException(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		csvFile.Close();
		TxLogDebugException();
		return;
	}
}

void KInterBPRFnMgrDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// KDialogEx::OnOK();
	UpdateData(TRUE);
	//Name 중복체크

	m_vecRows.clear();

	CXTPReportRecords* pReportRecords= m_wndReport.GetRecords();
	int nReportCount = pReportRecords->GetCount();
	CString strValue;

	for (int i = 0; i < nReportCount; ++i)
	{
		CXTPReportRecord* pReportRecord = pReportRecords->GetAt(i);
		CXTPReportRecordItemNumber*	pRecordItemNumber = NULL;

		std::vector<CString> vecRow;

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(0);
		strValue.Format(_T("%d"), (int)pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(1);
		strValue.Format(_T("%g"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(2);
		strValue.Format(_T("%g"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(3);
		strValue.Format(_T("%g"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(4);
		strValue.Format(_T("%g"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		m_vecRows.push_back(vecRow);
	}

	try
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));

		if(pRadio->GetCheck() == TRUE)
		{
			KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
			if (spDBaseConnection == NULL)
			{
				TxLogDebugException();
				return;
			}

			if (KDBaseAssignment::BPRFnNameExist(spDBaseConnection, m_strFnName))
			{
				ThrowException(_T("중복된 Function 명칭입니다."));
			}

			if (m_strFnName == _T(""))
			{
				ThrowException(_T("Function Name이 없습니다."));
			}

			AddBPRFn();

			AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));

			m_wndReport.ResetContent();
			m_vecRows.clear();
			m_strFnName = _T(""); 

			UpdateData(FALSE);

			KDialogEx::OnOK();
			return;
		}
		else
		{
			EditBPRFn();

			AfxMessageBox(_T("수정을 완료하였습니다."));

			m_wndReport.ResetContent();
			m_vecRows.clear();

			OnSelchangeCombo();
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다. 로그 파일을 확인해 주세요."));
	}
}

void KInterBPRFnMgrDlg::AddBPRFn( void )
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}
	CString strQuery;
	int		nRecords;
	try
	{
		spDBaseConnection->BeginTransaction();

		strQuery = _T("select count(1) as CNT from function_group");

		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		if (oResultSetPtr->Next())
			nRecords = oResultSetPtr->GetValueInt(0);

		if (nRecords == 0)
		{
			m_nFnGroupID = 100;
		}
		else
		{
			strQuery = _T("select Max(function_group_id) + 1 as NextID from function_group");
			oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

			if (oResultSetPtr->Next())
			{
				m_nFnGroupID = oResultSetPtr->GetValueInt(0);
			}
			else
			{
				ThrowException(_T("Error"));
			}
		}
		//FnGroupInsert
		strQuery.Format(_T("insert into Function_Group(function_group_id, function_group_name) Values( %d, '%s')"), m_nFnGroupID, m_strFnName);
		spDBaseConnection->ExecuteUpdate(strQuery);

		//FnInsert;
		strQuery.Format(_T("insert into Function Values( %d, ?, ?, ?, ?, ?)"), m_nFnGroupID);
		KPreparedStatementPtr prepareStatementPtr = spDBaseConnection->PrepareStatement(strQuery);

		int nVecRecord = m_vecRows.size();
		int nBindCount;
		for (int i = 0; i < nVecRecord; ++i)
		{
			nBindCount = 1;
			std::vector<CString> vecRow = m_vecRows[i];

			prepareStatementPtr->BindInt(nBindCount++, _ttoi(vecRow[0]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[1]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[2]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[3]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[4]));

			prepareStatementPtr->ExecuteUpdate();
			prepareStatementPtr->Reset();
		}

		spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		ThrowException(_T("Error CreateBPRFn"));
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		ThrowException(_T("Error CreateBPRFn"));
	}
}

void KInterBPRFnMgrDlg::InitComboBoxFn( void )
{
	//조회해서 값... 넣어주장...
	//선택시 GETITEMDATA가 0이 아니면 OnselChange에서... 불러..

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}

	try
	{
		CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		m_comboFn.ResetContent();

		int nCursorIndex = m_comboFn.InsertString(-1, _T("----- Select -----"));
		m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		while (oResultSetPtr->Next())
		{
			nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
			m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );
		}
		m_comboFn.SetCurSel(0);
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KInterBPRFnMgrDlg::OnSelchangeCombo()
{
	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());

	if (0 == nFnGroupID)
	{
		m_wndReport.ResetContent();
		m_vecRows.clear();
		return;
	}

	m_vecRows.clear();

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}

	try
	{
		CString strQuery;
		strQuery.Format(_T("select Function_ID, Alpha, Beta, A, B from Function where Function_Group_ID = %d"), nFnGroupID);

		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		while (oResultSetPtr->Next())
		{
			std::vector<CString> vecRow;

			vecRow.push_back( oResultSetPtr->GetValueString(0));
			vecRow.push_back( oResultSetPtr->GetValueString(1));
			vecRow.push_back( oResultSetPtr->GetValueString(2));
			vecRow.push_back( oResultSetPtr->GetValueString(3));
			vecRow.push_back( oResultSetPtr->GetValueString(4));

			m_vecRows.push_back(vecRow);
		}
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	m_wndReport.ResetContent();

	UpdateReportCtrl();
}

void KInterBPRFnMgrDlg::EditBPRFn( void )
{
	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
	if (0 == nFnGroupID)
		return;

	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	if (spDBaseConnection == NULL)
	{
		TxLogDebugException();
		return;
	}

	CString strQuery;
	try
	{
		spDBaseConnection->BeginTransaction();

		/*Step1. Delete Data*/
		strQuery.Format(_T("delete from Function where Function_Group_ID = %d"), nFnGroupID);
		spDBaseConnection->ExecuteUpdate(strQuery);

		/*Step2. Insert Data*/
		strQuery.Format(_T("insert into Function Values( %d, ?, ?, ?, ?, ?)"), nFnGroupID);
		KPreparedStatementPtr prepareStatementPtr = spDBaseConnection->PrepareStatement(strQuery);
		int nVecRecord = m_vecRows.size();
		int nBindCount;

		for (int i = 0; i < nVecRecord; ++i)
		{
			nBindCount = 1;
			std::vector<CString> vecRow = m_vecRows[i];

			prepareStatementPtr->BindInt(nBindCount++, _ttoi(vecRow[0]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[1]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[2]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[3]));
			prepareStatementPtr->BindDouble(nBindCount++, _ttof(vecRow[4]));

			prepareStatementPtr->ExecuteUpdate();
			prepareStatementPtr->Reset();
		}
		spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		spDBaseConnection->RollBack();
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		spDBaseConnection->RollBack();
		TxLogDebugException();
		return;
	}

}


int KInterBPRFnMgrDlg::FnGroupID( void )
{
	return m_nFnGroupID;
}

bool KInterBPRFnMgrDlg::GetLinkFunctionID( void )
{
	m_vecLinkFunction.clear();

	CString strSQL(_T("Select function_id From link Group By function_id "));

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{	
			int nFunctionID = spResult->GetValueInt(0);

			CString strFunctionID(_T(""));
			strFunctionID.Format(_T("%d"), nFunctionID);
			
			m_vecLinkFunction.push_back(strFunctionID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		m_vecLinkFunction.clear();
		return false;
	}

	return true;
}

void KInterBPRFnMgrDlg::UpdateInitReportCtrl( void )
{
	int nCount = m_vecLinkFunction.size();
	for (int i = 0; i < nCount ; i++)
	{
		CString strFuntionID = m_vecLinkFunction[i];
		int nFunctionID		 = _ttoi(strFuntionID);

		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nFunctionID));
		pRecordItem->SetEditable(FALSE);

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.15, _T("%g")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(4.0, _T("%g")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%g")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%g")));
	}

	m_wndReport.Populate();
}