/**
 * @file SelectByAttributeDlg.cpp
 * @brief KSelectByAttributeDlg 구현
 * @author 
 * @date 2010.09.08
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "SelectByAttributeDlg.h"
#include "Target.h"

#include "ChildFrm.h"
#include "MainFrameWnd.h"
#include "IOView.h"
#include "IOTableView.h"

/******************************************************************************
* KSelectByAttributeDlg                                                       *
******************************************************************************/
IMPLEMENT_DYNAMIC(KSelectByAttributeDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KSelectByAttributeDlg::KSelectByAttributeDlg(KIOTable* a_pIOTable, CWnd* pParent /*=NULL*/)
	:	KDialogEx(KSelectByAttributeDlg::IDD, pParent),
		m_pIOTable(a_pIOTable),
		m_strFilter(_T(""))
{

}


KSelectByAttributeDlg::~KSelectByAttributeDlg()
{
}



/**************************************
* Operations                          *
**************************************/
void KSelectByAttributeDlg::addTextToEdit( const CString& strText )
{
	m_editQuery.ReplaceSel( strText, TRUE );
	m_editQuery.SetFocus();
}


void KSelectByAttributeDlg::UpdateUniqueRecord( const CString& strSQL )
{
	m_ctrlReportUnique.ResetContent();

	try
	{
		KDBaseConPtr   spDBaseConnection = m_pIOTable->GetDBaseConnection();
		KResultSetPtr  spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (spResultSet->Next())
		{
			pRecord = m_ctrlReportUnique.AddRecord(new CXTPReportRecord);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(spResultSet->GetValueString(0)));
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(spResultSet->GetValueString(1)));
		}

		m_ctrlReportUnique.Populate();
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


void KSelectByAttributeDlg::ConvertFieldName( CXTPReportControl& wndReport, CString& strQuery )
{
	CXTPReportRecords* pRecords = wndReport.GetRecords();
	int nRecords = pRecords->GetCount();

	for (int i = 0; i < nRecords; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		KIOColumn* pColumn = (KIOColumn*)pItem->GetItemData();
		CString strDispName;
		strDispName.Format(_T("[%s]"), pColumn->Caption());
		CString strName = pColumn->Name();
		strQuery.Replace(strDispName, strName);
	}
}



/**************************************
* Overrides                           *
**************************************/
BOOL KSelectByAttributeDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if (!m_strFilter.IsEmpty())
		addTextToEdit(m_strFilter);

	KReportCtrlSetting::Default(m_ctrlReportFiled, FALSE);
	KReportCtrlSetting::VerticalNoLine(m_ctrlReportFiled);
	KReportCtrlSetting::HorizontalNoLine(m_ctrlReportFiled);
	m_ctrlReportFiled.FocusSubItems(FALSE);

	KReportCtrlSetting::Default(m_ctrlReportUnique, FALSE);
	m_ctrlReportUnique.FocusSubItems(FALSE);

	InitFieldReport();
	UpdateFieldRecord();

	InitUniqueReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KSelectByAttributeDlg::SetFilter( CString a_strFilter )
{
	m_strFilter = a_strFilter;
}


void KSelectByAttributeDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editQuery);

	DDX_Control(pDX, IDC_REPORT, m_ctrlReportFiled);
	DDX_Control(pDX, IDC_REPORT3, m_ctrlReportUnique);
}



BEGIN_MESSAGE_MAP(KSelectByAttributeDlg, KDialogEx)
	ON_BN_CLICKED(IDC_BUTTON0, &KSelectByAttributeDlg::OnBnClickedButton0)
	ON_BN_CLICKED(IDC_BUTTON1, &KSelectByAttributeDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KSelectByAttributeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &KSelectByAttributeDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &KSelectByAttributeDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &KSelectByAttributeDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &KSelectByAttributeDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &KSelectByAttributeDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &KSelectByAttributeDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &KSelectByAttributeDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &KSelectByAttributeDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &KSelectByAttributeDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &KSelectByAttributeDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDOK, &KSelectByAttributeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KSelectByAttributeDlg::OnBnClickedCancel)

	ON_NOTIFY(NM_DBLCLK, IDC_REPORT, &KSelectByAttributeDlg::OnReportFieldDblClick)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT3, &KSelectByAttributeDlg::OnReportUniqueDblClick)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/

void KSelectByAttributeDlg::OnReportFieldDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		CString strColumn;
		strColumn.Format(_T("[%s]"), pItem->GetValue());

		addTextToEdit( strColumn );
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KSelectByAttributeDlg::OnReportUniqueDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		addTextToEdit( pItem->GetValue() );
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KSelectByAttributeDlg::OnBnClickedButton0()
{
	CWaitCursor wait;

	CXTPReportRow* pRow = m_ctrlReportFiled.GetFocusedRow();

	if (NULL == pRow)
		return;

	CXTPReportRecordItem* pItem   = pRow->GetRecord()->GetItem(0);
	KIOColumn*            pColumn = (KIOColumn*)pItem->GetItemData();

	if (pColumn == NULL)
		return;

	CString strSQL;
	strSQL.Format( _T("SELECT %s, COUNT(*) FROM %s GROUP BY %s ORDER BY %s"), 
		pColumn->Name(), m_pIOTable->Name(), pColumn->Name(), pColumn->Name());

	UpdateUniqueRecord(strSQL);
}


void KSelectByAttributeDlg::OnBnClickedButton1()
{
	addTextToEdit( _T(" = ") );
}


void KSelectByAttributeDlg::OnBnClickedButton2()
{
	addTextToEdit( _T(" != ") );
}


void KSelectByAttributeDlg::OnBnClickedButton3()
{
	addTextToEdit( _T(" > ") );
}


void KSelectByAttributeDlg::OnBnClickedButton4()
{
	addTextToEdit( _T(" >= ") );
}


void KSelectByAttributeDlg::OnBnClickedButton5()
{
	addTextToEdit( _T(" < ") );
}


void KSelectByAttributeDlg::OnBnClickedButton6()
{
	addTextToEdit( _T(" <= ") );
}


void KSelectByAttributeDlg::OnBnClickedButton7()
{
	addTextToEdit( _T(" () ") );
	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );
}


void KSelectByAttributeDlg::OnBnClickedButton8()
{
	addTextToEdit( _T(" AND ") );
}


void KSelectByAttributeDlg::OnBnClickedButton9()
{
	addTextToEdit( _T(" OR ") );
}


void KSelectByAttributeDlg::OnBnClickedButton10()
{
	addTextToEdit( _T(" '' ") );
	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );
}


void KSelectByAttributeDlg::OnBnClickedButton11()
{
	addTextToEdit( _T(" '%' ") );
	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );}


void KSelectByAttributeDlg::OnBnClickedButton12()
{
	addTextToEdit( _T(" LIKE ") );
}

void KSelectByAttributeDlg::OnBnClickedOk()
{
	m_editQuery.GetWindowText(m_strFilter);
	m_strOrignalFilter = m_strFilter;

	if( m_strFilter.IsEmpty() == false )
	{
		ConvertFieldName(m_ctrlReportFiled, m_strFilter);
	}

	CChildFrame*  pChildFrame;
	KIOView*      pIOView;
	KIOTableView* pTableView;

	try
	{
		pChildFrame = (CChildFrame*)((CMainFrameWnd*)AfxGetMainWnd())->MDIGetActive();

		if (pChildFrame == nullptr)
		{
			TxLogDebug(_T("MDI 객체 호출에 실패했습니다."));
			throw 1;
		}

		pIOView = pChildFrame->GetIOView();

		if (pIOView == nullptr)
		{
			TxLogDebug(_T("IOView 객체 호출에 실패했습니다."));
			throw 1;
		}

		pTableView  = pIOView->FindTableView(m_pIOTable->Name());

		if (pTableView == nullptr)
		{
			TxLogDebug(_T("TableView 호출에 실패했습니다."));
			throw 1;
		}

		bool bIsSqlSyntaxError(false);// sql syntax 오류 확인 플래그
		bIsSqlSyntaxError = pTableView->SetFilter(m_strFilter);
		pTableView->SetOriginalFilter(m_strOrignalFilter);

		if (bIsSqlSyntaxError) {
			pTableView->SetFilter(_T(""));
			pTableView->SetOriginalFilter(_T(""));

			AfxMessageBox(_T("입력한 수식에 오류가 존재합니다."));
			m_editQuery.SetFocus();

			return;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());

		pTableView->SetFilter(_T(""));
		pTableView->SetOriginalFilter(_T(""));

		AfxMessageBox(_T("입력한 수식에 오류가 존재합니다."));
		m_editQuery.SetFocus();

		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	KDialogEx::OnOK();
}


void KSelectByAttributeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}


void KSelectByAttributeDlg::InitFieldReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColIdx(0);

	//Field
	pColumn = m_ctrlReportFiled.AddColumn(new CXTPReportColumn(nColIdx++, _T("Field"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("컬럼명"));
	}

	//Type
	pColumn = m_ctrlReportFiled.AddColumn(new CXTPReportColumn(nColIdx++, _T("Type"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("데이터 유형"));
	}

	m_ctrlReportFiled.Populate();
}


void KSelectByAttributeDlg::InitUniqueReport( void )
{
	int nColIdx(0);

	CXTPReportColumn* pColumn = nullptr;

	//Value
	pColumn = m_ctrlReportUnique.AddColumn(new CXTPReportColumn(nColIdx++, _T("Value"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("데이터 값"));
	}

	//Count
	pColumn = m_ctrlReportUnique.AddColumn(new CXTPReportColumn(nColIdx++, _T("Count"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("중복 횟수"));
	}

	m_ctrlReportUnique.Populate();
}


void KSelectByAttributeDlg::UpdateFieldRecord( void )
{
	m_ctrlReportFiled.ResetContent();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	const KIOColumns* pColumns = m_pIOTable->Columns();
	int nColumnCount = pColumns->ColumnCount();

	for (int i= 0; i< nColumnCount; i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		CString strColumn, strtype;
		strColumn = pColumn->Caption();
		switch(pColumn->DataType())
		{
		case KEMIODataTypeInteger : strtype = DATATYPESTRING_INTEGER; break;
		case KEMIODataTypeDouble : strtype = DATATYPESTRING_DOUBLE; break;
		case KEMIODataTypeString : strtype = DATATYPESTRING_STRING; break;
		}

		pRecord = m_ctrlReportFiled.AddRecord(new CXTPReportRecord);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strColumn));
		pItem->SetItemData((DWORD_PTR)pColumn);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strtype));
	}

	m_ctrlReportFiled.Populate();

}