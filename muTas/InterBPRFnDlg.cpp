// InterBPRFnDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "InterBPRFnDlg.h"
#include "afxdialogex.h"

#include "InterBPRFnMgrDlg.h"
#include "DBaseConnector.h"
//^^#include "SEDData.h"

using namespace std;

// KInterBPRFnDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterBPRFnDlg, KDialogEx)

KEMAssignmentBPRFunctionType KInterBPRFnDlg::m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;

KInterBPRFnDlg::KInterBPRFnDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KInterBPRFnDlg::IDD, pParent),
	m_dVOT(1.0),
	m_dAllLinkAlpha(0.15),
	m_dAllLinkBeta(4.0),
	m_nComboFnCursor(0),
	m_nComboLinkAllAlphaCursor(0),
	m_nComboLinkAllBetaCursor(0),
	m_nComboLinkAllACursor(0),
	m_nComboLinkAllBCursor(0),
	m_nFnGroupID(0)
{
	m_pTarget = NULL;
}

KInterBPRFnDlg::~KInterBPRFnDlg()
{
}

void KInterBPRFnDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_VOT,			m_edtVOT);
	DDX_Control(pDX, IDC_EDIT_ALL_ALPHA,	m_edtAllAlpha);
	DDX_Control(pDX, IDC_EDIT_ALL_BETA,		m_edtAllBeta);
	DDX_Control(pDX, IDC_COMBO1,			m_comboFn);
	DDX_Control(pDX, IDC_REPORT,			m_wndReport);
	DDX_Control(pDX, IDC_COMBO2,			m_comboLinkAlpha);
	DDX_Control(pDX, IDC_COMBO3,			m_comboLinkBeta);
	DDX_Control(pDX, IDC_COMBO4,			m_comboLinkA);
	DDX_Control(pDX, IDC_COMBO6,			m_comboLinkB);
}


BEGIN_MESSAGE_MAP(KInterBPRFnDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO3, &KInterBPRFnDlg::OnRadioClicked)
	ON_BN_CLICKED(IDOK, &KInterBPRFnDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_SETBPRFN, &KInterBPRFnDlg::OnBnClickedButtonSetBPRFunction)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KInterBPRFnDlg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDCANCEL, &KInterBPRFnDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterBPRFnDlg 메시지 처리기입니다.


BOOL KInterBPRFnDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if(m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
		pRadio->SetCheck(TRUE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO2));
		pRadio->SetCheck(TRUE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO3));
		pRadio->SetCheck(TRUE);
	}

	m_edtVOT.EnableRealNumber(true);
	m_edtVOT.EnableMinMax(0.0, 10000.0);

	CString strValue;

	strValue.Format(_T("%f"), m_dVOT);
	m_edtVOT.SetWindowText(strValue);

	m_edtAllAlpha.EnableRealNumber(true);
	m_edtAllBeta.EnableRealNumber(true);

	strValue.Format(_T("%f"), m_dAllLinkAlpha);
	m_edtAllAlpha.SetWindowText(strValue);

	strValue.Format(_T("%f"), m_dAllLinkBeta);
	m_edtAllBeta.SetWindowText(strValue);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitComboBoxEachFn();
	InitComboBoxEachLink();
	UpdateCtrlState();

	m_wndReport.GetColumns()->Clear();

	InitReportCtrl();
	UpdateReportCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterBPRFnDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strNumber;

	m_edtVOT.GetWindowText(strNumber);
	m_dVOT = _ttof(strNumber);

	m_edtAllAlpha.GetWindowText(strNumber);
	m_dAllLinkAlpha = _ttof(strNumber);

	m_edtAllBeta.GetWindowText(strNumber);
	m_dAllLinkBeta = _ttof(strNumber);

	m_nComboFnCursor			= m_comboFn.GetCurSel();
	m_nComboLinkAllAlphaCursor	= m_comboLinkAlpha.GetCurSel();
	m_nComboLinkAllBetaCursor	= m_comboLinkBeta.GetCurSel();
	m_nComboLinkAllACursor		= m_comboLinkA.GetCurSel();
	m_nComboLinkAllBCursor		= m_comboLinkB.GetCurSel();

	GenerateLinkQuery();

	KDialogEx::OnOK();
}


void KInterBPRFnDlg::OnBnClickedButtonSetBPRFunction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nComboFnCursor = m_comboFn.GetCurSel();

	KInterBPRFnMgrDlg oDlg(m_pTarget);
	if(oDlg.DoModal() == IDOK)
	{
		m_nFnGroupID = oDlg.FnGroupID();

		InitComboBoxEachFn();
		OnSelchangeCombo1();
	}
}

void KInterBPRFnDlg::InitComboBoxEachFn( void )
{
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

		int nCursorIndex = m_comboFn.InsertString(-1, _T("-------- Select --------"));
		m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		while (oResultSetPtr->Next())
		{
			nCursorIndex = m_comboFn.InsertString(-1, oResultSetPtr->GetValueString(1));
			m_comboFn.SetItemData( nCursorIndex, (DWORD_PTR)oResultSetPtr->GetValueInt(0) );

		}

		if( m_nFnGroupID != 0)
		{
			int nComboCount = m_comboFn.GetCount();

			for (int i = 0; i < nComboCount; ++i)
			{
				int nFnGroupId = m_comboFn.GetItemData(i);

				if ( nFnGroupId == m_nFnGroupID)
					m_comboFn.SetCurSel(i);
			}
		}
		else
		{
			m_comboFn.SetCurSel(m_nComboFnCursor);
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

}


void KInterBPRFnDlg::InitComboBoxEachLink( void )
{
	try
	{
		m_comboLinkAlpha.ResetContent();
		m_comboLinkBeta.ResetContent();
		m_comboLinkA.ResetContent();
		m_comboLinkB.ResetContent();

		int nCursorIndex = m_comboLinkAlpha.InsertString(-1, _T("------ Select ------"));
		m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		nCursorIndex = m_comboLinkBeta.InsertString(-1, _T("------ Select ------"));
		m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		nCursorIndex = m_comboLinkA.InsertString(-1, _T("------ Select ------"));
		m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		nCursorIndex = m_comboLinkB.InsertString(-1, _T("------ Select ------"));
		m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)0 );

		KIOTables* pTables = m_pTarget->Tables();
		KIOTable* pTable = pTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();

		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			if(pColumn->DataType() == KEMIODataTypeDouble)
			{
				CString strColumnName(_T(""));

				nCursorIndex = m_comboLinkAlpha.InsertString(-1, pColumn->Caption());
				m_comboLinkAlpha.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
				strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_ALPHA) == 0)
				{
					m_nComboLinkAllAlphaCursor = nCursorIndex;
				}

				nCursorIndex = m_comboLinkBeta.InsertString(-1, pColumn->Caption());
				m_comboLinkBeta.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
				strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_BETA) == 0)
				{
					m_nComboLinkAllBetaCursor = nCursorIndex;
				}

				nCursorIndex = m_comboLinkA.InsertString(-1, pColumn->Caption());
				m_comboLinkA.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
				strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_A) == 0)
				{
					m_nComboLinkAllACursor = nCursorIndex;
				}

				nCursorIndex = m_comboLinkB.InsertString(-1, pColumn->Caption());
				m_comboLinkB.SetItemData( nCursorIndex, (DWORD_PTR)pColumn);
				strColumnName = pColumn->Name();
				if (strColumnName.CompareNoCase(COLUMN_LINK_B) == 0)
				{
					m_nComboLinkAllBCursor = nCursorIndex;
				}
			}
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

	m_comboLinkAlpha.SetCurSel(m_nComboLinkAllAlphaCursor);
	m_comboLinkBeta.SetCurSel(m_nComboLinkAllBetaCursor);
	m_comboLinkA.SetCurSel(m_nComboLinkAllACursor);
	m_comboLinkB.SetCurSel(m_nComboLinkAllBCursor);
}


void KInterBPRFnDlg::OnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nFnGroupID = m_comboFn.GetItemData(m_comboFn.GetCurSel());

	if( 0 == nFnGroupID )
		return;

	m_vecReportData.clear();

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

			m_vecReportData.push_back(vecRow);
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

	UpdateReportCtrl(); 
}

void KInterBPRFnDlg::InitReportCtrl( void )
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
}

void KInterBPRFnDlg::UpdateReportCtrl( void )
{
	m_wndReport.ResetContent();

	int nRecordCount = m_vecReportData.size();
	CXTPReportRecord* pReportRecord = NULL;

	for(int i = 0; i < nRecordCount; ++i)
	{
		std::vector<CString> vecRow = m_vecReportData[i];
		pReportRecord = m_wndReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pRecordItem = NULL;

		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[0]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[1]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[2]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[3]));
		pRecordItem = pReportRecord->AddItem(new CXTPReportRecordItemText(vecRow[4]));
	}

	m_wndReport.Populate();
}

bool KInterBPRFnDlg::WriteInputArgument( CStdioFileEx& of )
{
	CString strOut;
	try
	{
		if(m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
		{
			of.WriteString(_T("vdf\t2\r\n")); 

			strOut.Format(_T("%d\t%f\r\n"), m_emBPRFnType, m_dVOT);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%f\t%f\t0\t0\t\r\n"), m_emBPRFnType, m_dAllLinkAlpha, m_dAllLinkBeta);
			of.WriteString(strOut);
		}
		else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
		{
			int nRecordCount = m_vecReportData.size();

			int nVDFRows = nRecordCount + 1;

			strOut.Format(_T("vdf\t%d\r\n"), nVDFRows);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%f\r\n"), m_emBPRFnType, m_dVOT);
			of.WriteString(strOut);

			int nIndex;
			double dAlpha, dBeta, dA, dB;

			for (int i = 0; i < nRecordCount; ++i)
			{
				std::vector<CString> vecRow = m_vecReportData[i];

				nIndex	= _ttoi(vecRow[0]);
				dAlpha	= _ttof(vecRow[1]);
				dBeta	= _ttof(vecRow[2]);
				dA		= _ttof(vecRow[3]);
				dB		= _ttof(vecRow[4]);

				strOut.Format(_T("%d\t%g\t%g\t%g\t%g\r\n"), nIndex, dAlpha, dBeta, dA, dB);
				of.WriteString(strOut);
			}

		}
		else if (m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
		{
			strOut.Format(_T("vdf\t%d\r\n"), 2);
			of.WriteString(strOut);

			strOut.Format(_T("%d\t%f\r\n"), m_emBPRFnType, m_dVOT);
			of.WriteString(strOut);

			// 			strOut.Format(_T("%d\t%d\t%d\t%d\t%d\r\n"), 1, 0, 0, 0, 0);
			// 			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;

}

void KInterBPRFnDlg::OnRadioClicked( UINT nID )
{
	switch(nID)
	{
	case IDC_RADIO1:
		m_emBPRFnType = KEMAssignmentBPRFunctionAllLink;
		break;

	case IDC_RADIO2:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachFn;
		break;
	case IDC_RADIO3:
		m_emBPRFnType = KEMAssignmentBPRFunctionEachLink;
		break;
	}

	UpdateCtrlState();
}

void KInterBPRFnDlg::UpdateCtrlState( void )
{
	if (m_emBPRFnType == KEMAssignmentBPRFunctionAllLink)
	{
		m_edtAllAlpha.EnableWindow(TRUE);
		m_edtAllBeta.EnableWindow(TRUE);

		m_comboFn.EnableWindow(FALSE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(FALSE);

		m_comboLinkAlpha.EnableWindow(FALSE);
		m_comboLinkBeta.EnableWindow(FALSE);
		m_comboLinkA.EnableWindow(FALSE);
		m_comboLinkB.EnableWindow(FALSE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachFn)
	{
		m_edtAllAlpha.EnableWindow(FALSE);
		m_edtAllBeta.EnableWindow(FALSE);

		m_comboFn.EnableWindow(TRUE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(TRUE);

		m_comboLinkAlpha.EnableWindow(FALSE);
		m_comboLinkBeta.EnableWindow(FALSE);
		m_comboLinkA.EnableWindow(FALSE);
		m_comboLinkB.EnableWindow(FALSE);
	}
	else if(m_emBPRFnType == KEMAssignmentBPRFunctionEachLink)
	{
		m_edtAllAlpha.EnableWindow(FALSE);
		m_edtAllBeta.EnableWindow(FALSE);

		m_comboFn.EnableWindow(FALSE);
		GetDlgItem(ID_BUTTON_SETBPRFN)->EnableWindow(FALSE);

		m_comboLinkAlpha.EnableWindow(TRUE);
		m_comboLinkBeta.EnableWindow(TRUE);
		m_comboLinkA.EnableWindow(TRUE);
		m_comboLinkB.EnableWindow(TRUE);
	}
}

void KInterBPRFnDlg::Target( KTarget* a_pTarget )
{
	m_pTarget = a_pTarget;
}

bool KInterBPRFnDlg::ExportDB2LinkFile( KDBaseConPtr a_spDBaseConnection, KIOColumns& userColumnCollection, CString a_strAddQuery, CString strFileName/*=_T("link.dat")*/ )
{
	if (m_emBPRFnType != KEMAssignmentBPRFunctionEachLink)
	{
		TxLogDebugStartMsg();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id") ;

		KIOColumn* pColumn = NULL;
		int nColumnCount = userColumnCollection.ColumnCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pColumn = userColumnCollection.GetColumn(i);
			strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
		}

		strSQL = strSQL + _T(" From Link Order By link_id ");

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);

			TNewFixedLinkInter fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= 0.0;
				fixedLink.beta		= 0.0;
				fixedLink.a			= 0.0;
				fixedLink.b			= 0.0;

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLinkInter) );  

				for (int i=0; i<nColumnCount; i++)
				{
					double dValue = spResultSet->GetValueDouble(9+i);
					ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();
		TxLogDebugEndMsg();
		return true;
	}
	else
	{
		TxLogDebugStartMsg();

		CString strSQL = _T(" Select link_id, fnode_id, tnode_id, link_type, length, lane, capa, speed, function_id ") ;
		strSQL.Append(a_strAddQuery);

		KIOColumn* pColumn = NULL;
		int nColumnCount = userColumnCollection.ColumnCount();
		for (int i=0; i<nColumnCount; i++)
		{
			pColumn = userColumnCollection.GetColumn(i);
			strSQL  = strSQL + _T(", ") + pColumn->Name() + _T(" ");
		}

		strSQL = strSQL + _T(" From Link Order By link_id ");

		ofstream ofs(strFileName, ios::binary);
		if (! ofs)
		{
			TxLogDebugException();
			return false;
		}

		try
		{
			KResultSetPtr spResultSet = a_spDBaseConnection->ExecuteQuery(strSQL);
			TNewFixedLinkInter fixedLink;
			while(spResultSet->Next())
			{
				//ofs.write(reinterpret_cast<char*>)
				fixedLink.link_id   = spResultSet->GetValueInt64 (0);
				fixedLink.fnode_id  = spResultSet->GetValueInt64 (1);
				fixedLink.tnode_id  = spResultSet->GetValueInt64 (2);
				fixedLink.link_type = spResultSet->GetValueInt   (3);
				fixedLink.length    = spResultSet->GetValueDouble(4);
				fixedLink.lane      = spResultSet->GetValueInt   (5);
				fixedLink.capa      = spResultSet->GetValueDouble(6);
				fixedLink.speed     = spResultSet->GetValueDouble(7);
				fixedLink.fuNumber	= spResultSet->GetValueInt   (8);
				fixedLink.alpha		= spResultSet->GetValueDouble(9);
				fixedLink.beta		= spResultSet->GetValueDouble(10);
				fixedLink.a			= spResultSet->GetValueDouble(11);
				fixedLink.b			= spResultSet->GetValueDouble(12);

				ofs.write(reinterpret_cast<char*>( &fixedLink ), sizeof(TNewFixedLinkInter) );  

				for (int i=0; i<nColumnCount; i++)
				{
					double dValue = spResultSet->GetValueDouble(13+i);
					ofs.write(reinterpret_cast<char*>( &dValue ), sizeof(double) ); 
				}
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			ofs.close();
			return false;
		}

		ofs.close();
		TxLogDebugEndMsg();
		return true;
	}

	return false;
}


void KInterBPRFnDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

void KInterBPRFnDlg::GenerateLinkQuery( void )
{
	CString strTemp;

	KIOColumn* pColumn = (KIOColumn*)m_comboLinkAlpha.GetItemData(m_comboLinkAlpha.GetCurSel());
	if (pColumn == NULL)
	{
		m_strAddQuery.Append(_T(", 0 as Alpha"));
	}
	else
	{
		strTemp.Format(_T(", %s as Alpha"), pColumn->Name());
		m_strAddQuery.Append( strTemp );
	}

	pColumn = (KIOColumn*)m_comboLinkBeta.GetItemData(m_comboLinkBeta.GetCurSel());
	if (pColumn == NULL)
	{
		m_strAddQuery.Append(_T(", 0 as Beta"));
	}
	else
	{
		strTemp.Format(_T(", %s as Beta"), pColumn->Name());
		m_strAddQuery.Append( strTemp );
	}

	pColumn = (KIOColumn*)m_comboLinkA.GetItemData(m_comboLinkA.GetCurSel());
	if (pColumn == NULL)
	{
		m_strAddQuery.Append(_T(", 0 as A"));
	}
	else
	{
		strTemp.Format(_T(", %s as A"), pColumn->Name());
		m_strAddQuery.Append( strTemp );
	}

	pColumn = (KIOColumn*)m_comboLinkB.GetItemData(m_comboLinkB.GetCurSel());
	if (pColumn == NULL)
	{
		m_strAddQuery.Append(_T(", 0 as B"));
	}
	else
	{
		strTemp.Format(_T(", %s as B"), pColumn->Name());
		m_strAddQuery.Append( strTemp );
	}


}

