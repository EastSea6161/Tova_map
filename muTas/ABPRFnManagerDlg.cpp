// ABPRFnManagerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ABPRFnManagerDlg.h"
#include "afxdialogex.h"

//#include "StdioFileEx.h"
//^^#include "SEDData.h"
#include "DBaseAssignment.h"
#include "DBaseConnector.h"
#include "Target.h"
#include "CustomReportRecordItem.h"
#include "DefineNamesDefaultPara.h"

// KABPRFnManagerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KABPRFnManagerDlg, KDialogEx)

KABPRFnManagerDlg::KABPRFnManagerDlg(KTarget* a_pTarget, int a_nParentFnGroupID, CWnd* pParent /*=NULL*/)
	: KDialogEx(KABPRFnManagerDlg::IDD, pParent),
		m_strImportFileName(_T("")),
		m_strFnName(_T("")),
		m_nFnGroupID(0),
		m_pTarget(a_pTarget),
		m_nParentFnGroupID(a_nParentFnGroupID)
{
}

KABPRFnManagerDlg::~KABPRFnManagerDlg()
{
}

void KABPRFnManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_wndReport);
	DDX_Control(pDX, IDC_REPORT3, m_wndCboReport);
	DDX_Control(pDX, IDC_COMBO,   m_comboFn);
	DDX_Text(pDX, IDC_EDIT_FN_NAME, m_strFnName);
	
}


BEGIN_MESSAGE_MAP(KABPRFnManagerDlg, KDialogEx)
		ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KABPRFnManagerDlg::OnRadioClicked)
		ON_BN_CLICKED(ID_BTN_IMPORT, &KABPRFnManagerDlg::OnBnClickedBtnImport)
		ON_BN_CLICKED(IDOK, &KABPRFnManagerDlg::OnBnClickedOk)
		ON_CBN_SELCHANGE(IDC_COMBO, &KABPRFnManagerDlg::OnSelchangeCombo)
		ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT3, &KABPRFnManagerDlg::OnReportCheckItem)
END_MESSAGE_MAP()


// KABPRFnManagerDlg 메시지 처리기입니다.


BOOL KABPRFnManagerDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
	pRadio->SetCheck(TRUE);

	KReportCtrlSetting::Default(m_wndReport, TRUE);

	KReportCtrlSetting::Default(m_wndCboReport, TRUE);
	m_wndCboReport.SelectionEnable(FALSE);
		
	GetLinkFunctionID();
	
	InitReportCtrl();
	UpdateInitReportCtrl();

	InitCboReportCtrl();
	InitCboReportData();

	UpdateControlState();

	if (!IsExistFunctionList())
	{
		pRadio = (CButton*)GetDlgItem(IDC_RADIO2);
		pRadio->EnableWindow(FALSE);
	}

	if (m_nParentFnGroupID != 0)
	{
		//UpdateCombo;

		int nItemCombo = m_comboFn.GetCount();

		for (int i = 0; i < nItemCombo; ++i)
		{
			int nFnGrooupID = m_comboFn.GetItemData(i);

			if (nFnGrooupID == m_nParentFnGroupID)
			{
				m_comboFn.SetCurSel(i);

				pRadio = (CButton*)GetDlgItem(IDC_RADIO1);
				pRadio->SetCheck(FALSE);

				pRadio = (CButton*)GetDlgItem(IDC_RADIO2);
				pRadio->SetCheck(TRUE);

				GetDlgItem(IDC_STATIC_NAME)->ShowWindow(false);
				GetDlgItem(IDC_EDIT_FN_NAME)->ShowWindow(false);

				GetDlgItem(IDC_STATIC_LIST)->ShowWindow(true);
				GetDlgItem(IDC_COMBO)->ShowWindow(true);

				OnSelchangeCombo();

				break;
			}
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KABPRFnManagerDlg::OnRadioClicked( UINT nID )
{
	UpdateControlState();
}

void KABPRFnManagerDlg::UpdateControlState( void )
{
	InitComboBoxFn();

	CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));

	if(pRadio->GetCheck() == TRUE)
	{
		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_FN_NAME)->ShowWindow(true);

		GetDlgItem(IDC_STATIC_LIST)->ShowWindow(false);
		GetDlgItem(IDC_COMBO)->ShowWindow(false);

		UpdateInitReportCtrl();
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_FN_NAME)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_LIST)->ShowWindow(true);
		GetDlgItem(IDC_COMBO)->ShowWindow(true);
				
		OnSelchangeCombo();
	}

	UpdateEachLinkReport();
}

void KABPRFnManagerDlg::InitReportCtrl( void )
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

void KABPRFnManagerDlg::UpdateInsertReportCtrl( void )
{
	m_wndReport.ResetContent();

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
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%.3f")));

		double dBeta = _ttof(row[2]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%.3f")));

		double dA = _ttof(row[3]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dA, _T("%.3f")));
		
		double dB = _ttof(row[4]);
		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(dB, _T("%.3f")));
	}
	m_wndReport.Populate();
}

void KABPRFnManagerDlg::OnBnClickedBtnImport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//Select File, Read File, Update Record;
	TCHAR szFilter[] = _T("(*.txt)|*.txt|");
	
	CFileDialog fileDlg(true, _T("txt"), _T("*.txt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	
	if( fileDlg.DoModal() != IDOK )
		return;

	m_strImportFileName = fileDlg.GetPathName();
	ReadBPRTextFile();
	UpdateInsertReportCtrl();
}

void KABPRFnManagerDlg::ReadBPRTextFile( void )
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

 
void KABPRFnManagerDlg::OnBnClickedOk()
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
		strValue.Format(_T("%.3f"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(2);
		strValue.Format(_T("%.3f"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(3);
		strValue.Format(_T("%.3f"), pRecordItemNumber->GetValue());
		vecRow.push_back(strValue);

		pRecordItemNumber = (CXTPReportRecordItemNumber*)pReportRecord->GetItem(4);
		strValue.Format(_T("%.3f"), pRecordItemNumber->GetValue());
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

			AddBPRFn(spDBaseConnection);

			//AfxMessageBox(_T("데이터 불러오기를 완료하였습니다."));

			m_wndReport.ResetContent();
			m_vecRows.clear();
			m_strFnName = _T(""); 

			UpdateData(FALSE);

			//Modify 비활성화되었다면 활성화함
			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO2);
			
			if (!pButton->EnableWindow())
				pButton->EnableWindow(TRUE);

			KDialogEx::OnOK();
			return;
		}
		else
		{
			m_nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
			
			EditBPRFn();

			AfxMessageBox(_T("수정되었습니다."));
			KDialogEx::OnOK();
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

void KABPRFnManagerDlg::AddBPRFn( KDBaseConPtr a_spDBaseConnection )
{
	std::vector<CString> vecColumnName;
	GetComboValue(vecColumnName);

	CString strQuery;
	int		nRecords;
	try
	{
		a_spDBaseConnection->BeginTransaction();
		
		strQuery = _T("select count(1) as CNT from function_group");

		KResultSetPtr oResultSetPtr = a_spDBaseConnection->ExecuteQuery(strQuery);

		if (oResultSetPtr->Next())
			nRecords = oResultSetPtr->GetValueInt(0);

		if (nRecords == 0)
		{
			m_nFnGroupID = 100;
		}
		else
		{
			strQuery = _T("select Max(function_group_id) + 1 as NextID from function_group");
			oResultSetPtr = a_spDBaseConnection->ExecuteQuery(strQuery);

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
		strQuery.Format(_T("insert into Function_Group Values( %d, '%s'"), m_nFnGroupID, m_strFnName);

		int nSize = vecColumnName.size();

		for (int i = 0; i < nSize; ++i)
		{
			int nFlag = 0;

			if (!vecColumnName[i].IsEmpty())
				nFlag = 1;

			strQuery.AppendFormat(_T(", %d, '%s'"), nFlag, vecColumnName[i]);
		}

		strQuery.Append(_T(")"));
		a_spDBaseConnection->ExecuteUpdate(strQuery);

		//FnInsert;
		strQuery.Format(_T("insert into Function Values( %d, ?, ?, ?, ?, ?)"), m_nFnGroupID);
		KPreparedStatementPtr prepareStatementPtr = a_spDBaseConnection->PrepareStatement(strQuery);

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
		
		a_spDBaseConnection->Commit();
	}
	catch(KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		a_spDBaseConnection->RollBack();
		ThrowException(_T("Error CreateBPRFn"));
	}
	catch(...)
	{
		TxLogDebugException();
		a_spDBaseConnection->RollBack();
		ThrowException(_T("Error CreateBPRFn"));
	}
}

void KABPRFnManagerDlg::InitComboBoxFn( void )
{
	m_vecFnName.clear();

    KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	try
	{
		CString strQuery = _T("select Function_Group_ID, Function_Group_Name from function_group");
		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(strQuery);

		m_comboFn.ResetContent();

		int nCursorIndex;

		while (oResultSetPtr->Next())
		{
			nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
			m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );

			m_vecFnName.push_back(oResultSetPtr->GetValueString(1));
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

void KABPRFnManagerDlg::OnSelchangeCombo()
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

	UpdateEachLinkReport();
	UpdateInsertReportCtrl();
}

void KABPRFnManagerDlg::EditBPRFn( void )
{
	std::vector<CString> vecColumnName;
	GetComboValue(vecColumnName);

	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
	if (0 == nFnGroupID)
		return;

	m_strFnName = m_vecFnName[m_comboFn.GetCurSel()];
	
    KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
	CString strQuery;
	try
	{
		spDBaseConnection->BeginTransaction();
		/*Step0. FunctionGroup Update*/
		strQuery.Format(_T("DELETE FROM function_group WHERE function_group_id = %d"), m_nFnGroupID);
		spDBaseConnection->ExecuteUpdate(strQuery);

		strQuery.Format(_T("INSERT into Function_Group Values( %d, '%s'"), m_nFnGroupID, m_strFnName);

		int nSize = vecColumnName.size();

		for (int i = 0; i < nSize; ++i)
		{
			int nFlag = 0;

			if (!vecColumnName[i].IsEmpty())
				nFlag = 1;

			strQuery.AppendFormat(_T(", %d, '%s'"), nFlag, vecColumnName[i]);
		}

		strQuery.Append(_T(")"));
		spDBaseConnection->ExecuteUpdate(strQuery);

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

			prepareStatementPtr->BindInt(nBindCount++,    _ttoi(vecRow[0]));
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

int KABPRFnManagerDlg::FnGroupID( void )
{
	return m_nFnGroupID;
}

void KABPRFnManagerDlg::UpdateInitReportCtrl( void )
{
	m_wndReport.ResetContent();

	int nCount = m_vecLinkFunction.size();
	for (int i = 0; i < nCount ; i++)
	{
		CString strFuntionID = m_vecLinkFunction[i];
		int nFunctionID		 = _ttoi(strFuntionID);

		CXTPReportRecord* pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemNumber((double)nFunctionID));
		pRecordItem->SetEditable(FALSE);

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.15, _T("%.3f")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(4.0, _T("%.3f")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));

		pRecordItem	= pReportRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.3f")));
	}

	m_wndReport.Populate();
}

bool KABPRFnManagerDlg::GetLinkFunctionID()
{
	m_vecLinkFunction.clear();

	CString strSQL(_T("Select function_id From link Group By function_id "));

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResult          = spDBaseConnection->ExecuteQuery(strSQL);

		while (spResult->Next())
		{	
			int     nFunctionID   = spResult->GetValueInt(0);
			
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

void KABPRFnManagerDlg::OnReportCheckItem( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (nIndex == 0)
	{
		if (pItemNotify->pItem->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(1)->SetBackgroundColor(RGB(255, 255, 255));
			pItemNotify->pRow->GetRecord()->GetItem(1)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(1)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(1)->SetEditable(FALSE);
		}
	}
	else if(nIndex == 2)
	{
		if (pItemNotify->pItem->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(3)->SetBackgroundColor(RGB(255, 255, 255));
			pItemNotify->pRow->GetRecord()->GetItem(3)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(3)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(3)->SetEditable(FALSE);
		}
	}
	else if(nIndex == 4)
	{
		if (pItemNotify->pItem->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(5)->SetBackgroundColor(RGB(255, 255, 255));
			pItemNotify->pRow->GetRecord()->GetItem(5)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(5)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(5)->SetEditable(FALSE);
		}
	}
	else if(nIndex == 6)
	{
		if (pItemNotify->pItem->IsChecked())
		{
			pItemNotify->pRow->GetRecord()->GetItem(7)->SetBackgroundColor(RGB(255, 255, 255));
			pItemNotify->pRow->GetRecord()->GetItem(7)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(7)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(7)->SetEditable(FALSE);
		}
	}
}

void KABPRFnManagerDlg::InitCboReportCtrl( void )
{
	KIOTables*  pIOTables  = m_pTarget->Tables();
	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
	KIOColumns* pIOColumns = pIOTable->Columns();
	KIOColumn*  pIOColumn  = nullptr;

	int nColumnCnt = pIOColumns->ColumnCount();

	CXTPReportRecordItemEditOptions* pEditOptions = nullptr;
	CXTPReportColumn*                pColumn      = nullptr;
	int nIndex  = 0;

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("α"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	pEditOptions = pColumn->GetEditOptions();

	for (int i = 0; i < nColumnCnt; ++i)
	{
		pIOColumn = pIOColumns->GetColumn(i);

		if (pIOColumn->DataType() != KEMIODataTypeDouble)
			continue;

		pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("β"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	pEditOptions = pColumn->GetEditOptions();

	for (int i = 0; i < nColumnCnt; ++i)
	{
		pIOColumn = pIOColumns->GetColumn(i);

		if (pIOColumn->DataType() != KEMIODataTypeDouble)
			continue;

		pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("a"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	pEditOptions = pColumn->GetEditOptions();

	for (int i = 0; i < nColumnCnt; ++i)
	{
		pIOColumn = pIOColumns->GetColumn(i);

		if (pIOColumn->DataType() != KEMIODataTypeDouble)
			continue;

		pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("Select"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("선택"));
	}

	pColumn = m_wndCboReport.AddColumn(new CXTPReportColumn(nIndex++, _T("b"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	pEditOptions = pColumn->GetEditOptions();

	for (int i = 0; i < nColumnCnt; ++i)
	{
		pIOColumn = pIOColumns->GetColumn(i);

		if (pIOColumn->DataType() != KEMIODataTypeDouble)
			continue;

		pEditOptions->AddConstraint(pIOColumn->Caption(), (DWORD_PTR)pIOColumn);
	}
}

void KABPRFnManagerDlg::InitCboReportData( void )
{
	m_wndCboReport.ResetContent();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	pRecord = m_wndCboReport.AddRecord(new CXTPReportRecord);

	//Alpha CheckBox
	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	pItem->HasCheckbox(TRUE);

	//Alpha Column Combo
	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetEditable(FALSE);
	pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

	//Beta Checkbox
	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	pItem->HasCheckbox(TRUE);

	//Beta Column Combo
	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetEditable(FALSE);
	pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

	//A Checkbox
	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	pItem->HasCheckbox(TRUE);

	//A Column Combo
	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetEditable(FALSE);
	pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

	//B Checkbox
	pItem = pRecord->AddItem(new CXTPReportRecordItem);
	pItem->HasCheckbox(TRUE);

	//b column combo
	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetEditable(FALSE);
	pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

	m_wndCboReport.Populate();
}

void KABPRFnManagerDlg::GetComboValue( std::vector<CString>& a_vecColumn )
{
	a_vecColumn.clear();

	CString strValue;

	CXTPReportRecord*     pRecord = m_wndCboReport.GetRecords()->GetAt(0);
	CXTPReportRecordItem* pItem   = nullptr;

	//Alpha
	pItem    = pRecord->GetItem(0);
	strValue = _T("");

	if (pItem->IsChecked())
	{
		KIOColumn* pIOColumn = (KIOColumn*)pRecord->GetItem(1)->GetItemData();
		strValue = pIOColumn->Name();
	}

	a_vecColumn.push_back(strValue);

	//Beta
	pItem    = pRecord->GetItem(2);
	strValue = _T("");

	if (pItem->IsChecked())
	{
		KIOColumn* pIOColumn = (KIOColumn*)pRecord->GetItem(3)->GetItemData();
		strValue = pIOColumn->Name();
	}

	a_vecColumn.push_back(strValue);

	//a
	pItem    = pRecord->GetItem(4);
	strValue = _T("");

	if (pItem->IsChecked())
	{
		KIOColumn* pIOColumn = (KIOColumn*)pRecord->GetItem(5)->GetItemData();
		strValue = pIOColumn->Name();
	}

	a_vecColumn.push_back(strValue);

	//B
	pItem    = pRecord->GetItem(6);
	strValue = _T("");

	if (pItem->IsChecked())
	{
		KIOColumn* pIOColumn = (KIOColumn*)pRecord->GetItem(7)->GetItemData();
		strValue = pIOColumn->Name();
	}

	a_vecColumn.push_back(strValue);
}

bool KABPRFnManagerDlg::IsExistFunctionList( void )
{
	bool    bExist = false;
	CString strQuery;

	try
	{
		strQuery.Format(_T("SELECT COUNT(*) FROM %s"), KDefaultParaTable::TABLE_DEFAULT_FUNCTION_GROUP);
		
		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("결과가 없습니다."));

		int nCount = spResultSet->GetValueInt(0);

		if (nCount != 0)
			bExist = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return bExist;
}

void KABPRFnManagerDlg::UpdateEachLinkReport( void )
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);

	if (pButton->GetCheck() == BST_CHECKED)
	{
		KIOTables*  pIOTables  = m_pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		int         nColumn    = pIOColumns->ColumnCount();
		KIOColumn*  pIOColumn  = nullptr;

		for (int i = 0; i < nColumn; ++i)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->DataType() == KEMIODataTypeDouble)
				break;
		}
		
		CXTPReportRecord*        pRecord     = m_wndCboReport.GetRecords()->GetAt(0);
		CXTPReportRecordItem*    pCheckItem  = nullptr;
		CCustomReportRecordItem* pCustomItem = nullptr;
			
		//Init Check Column
		pCheckItem = pRecord->GetItem(0);
		pCheckItem->SetChecked(FALSE);
		
		pCheckItem = pRecord->GetItem(2);
		pCheckItem->SetChecked(FALSE);

		pCheckItem = pRecord->GetItem(4);
		pCheckItem->SetChecked(FALSE);

		pCheckItem = pRecord->GetItem(6);
		pCheckItem->SetChecked(FALSE);

		pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(1);
		pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
		pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pCustomItem->SetEditable(FALSE);

		pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(3);
		pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
		pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pCustomItem->SetEditable(FALSE);

		pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(5);
		pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
		pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pCustomItem->SetEditable(FALSE);

		pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(7);
		pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
		pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pCustomItem->SetEditable(FALSE);
	}
	else
	{
		int     nFnID = m_comboFn.GetItemData(m_comboFn.GetCurSel());
		int     nUseAlpha(0), nUseBeta(0), nUseA(0), nUseB(0);
		CString strAlpha(_T("")), strBeta(_T("")), strA(_T("")), strB(_T(""));

		CString strQuery;
		strQuery.Format(_T("SELECT use_alpha_flag, alpha_column_name, use_beta_flag, beta_column_name, use_a_flag, a_column_name, use_b_flag, b_column_name FROM %s WHERE function_group_id = %d"), 
			KDefaultParaTable::TABLE_DEFAULT_FUNCTION_GROUP, nFnID);

		KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("결과가 없습니다."));

		nUseAlpha = spResultSet->GetValueInt(0);
		strAlpha  = spResultSet->GetValueString(1);
		nUseBeta  = spResultSet->GetValueInt(2);
		strBeta   = spResultSet->GetValueString(3);
		nUseA     = spResultSet->GetValueInt(4);
		strA      = spResultSet->GetValueString(5);
		nUseB     = spResultSet->GetValueInt(6);
		strB      = spResultSet->GetValueString(7);

		KIOTables*  pIOTables  = m_pTarget->Tables();
		KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_LINK);
		KIOColumns* pIOColumns = pIOTable->Columns();
		int         nColumn    = pIOColumns->ColumnCount();
		KIOColumn*  pIOColumn  = nullptr;
		KIOColumn*  pSelColumn = nullptr;

		for (int i = 0; i < nColumn; ++i)
		{
			pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->DataType() == KEMIODataTypeDouble)
				break;
		}

		CXTPReportRecord*        pRecord     = m_wndCboReport.GetRecords()->GetAt(0);
		CXTPReportRecordItem*    pCheckItem  = nullptr;
		CCustomReportRecordItem* pCustomItem = nullptr;

		if (nUseAlpha == 0)
		{
			pCheckItem = pRecord->GetItem(0);
			pCheckItem->SetChecked(FALSE);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(1);
			pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
			pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			pCheckItem = pRecord->GetItem(0);
			pCheckItem->SetChecked(TRUE);

			pSelColumn = pIOColumns->GetColumn(strAlpha);
			
			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(1);
			pCustomItem->SetItemData((DWORD_PTR)pSelColumn);
			pCustomItem->SetBackgroundColor(RGB(255, 255, 255));
			pCustomItem->SetEditable(TRUE);
		}

		if (nUseBeta == 0)
		{
			pCheckItem = pRecord->GetItem(2);
			pCheckItem->SetChecked(FALSE);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(3);
			pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
			pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			pCheckItem = pRecord->GetItem(2);
			pCheckItem->SetChecked(TRUE);

			pSelColumn = pIOColumns->GetColumn(strBeta);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(3);
			pCustomItem->SetItemData((DWORD_PTR)pSelColumn);
			pCustomItem->SetBackgroundColor(RGB(255, 255, 255));
			pCustomItem->SetEditable(TRUE);
		}

		if (nUseA == 0)
		{
			pCheckItem = pRecord->GetItem(4);
			pCheckItem->SetChecked(FALSE);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(5);
			pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
			pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			pCheckItem = pRecord->GetItem(4);
			pCheckItem->SetChecked(TRUE);

			pSelColumn = pIOColumns->GetColumn(strA);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(5);
			pCustomItem->SetItemData((DWORD_PTR)pSelColumn);
			pCustomItem->SetBackgroundColor(RGB(255, 255, 255));
			pCustomItem->SetEditable(TRUE);
		}

		if (nUseB == 0)
		{
			pCheckItem = pRecord->GetItem(6);
			pCheckItem->SetChecked(FALSE);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(7);
			pCustomItem->SetItemData((DWORD_PTR)pIOColumn);
			pCustomItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		}
		else
		{
			pCheckItem = pRecord->GetItem(6);
			pCheckItem->SetChecked(TRUE);

			pSelColumn = pIOColumns->GetColumn(strB);

			pCustomItem = (CCustomReportRecordItem*)pRecord->GetItem(7);
			pCustomItem->SetItemData((DWORD_PTR)pSelColumn);
			pCustomItem->SetBackgroundColor(RGB(255, 255, 255));
			pCustomItem->SetEditable(TRUE);
		}
	}

	m_wndCboReport.Populate();
}
