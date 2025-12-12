/**
 * @file FieldCalculatorDlg.cpp
 * @brief KFieldCalculatorDlg 구현
 * @author 
 * @date 2010.09.08
 * @remark
 */



#include "stdafx.h"
#include "FieldCalculatorDlg.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
#include "Target.h"

//^#include "DBaseConnector.h"
#include "DBaseFieldCalculator.h"

/******************************************************************************
* KFieldCalculatorDlg                                                         *
******************************************************************************/
IMPLEMENT_DYNAMIC(KFieldCalculatorDlg, KResizeDialogEx)

KFieldCalculatorDlg::KFieldCalculatorDlg(CWnd* pParent /*=NULL*/)
	:	KResizeDialogEx(KFieldCalculatorDlg::IDD, pParent)
	, m_bUpdated( false )
	, m_pTable( NULL )
	, m_bResultGetUniqueValThread(false)
	, m_strUniqueValQuery(_T(""))
{
    m_strInitFilter = _T("");
}

KFieldCalculatorDlg::~KFieldCalculatorDlg()
{
	DeallocateMap();
}

void KFieldCalculatorDlg::addTextToEdit( CEdit& edit, const CString& strText )
{
	edit.ReplaceSel( strText );
	edit.SetFocus();
}

void KFieldCalculatorDlg::convertFieldName( CListCtrl& listCtrl, CString& strSQL )
{
	int nItemCount = listCtrl.GetItemCount();
	for( int i = 0; i < nItemCount; ++i )
	{
		KIOColumn* pColumn = (KIOColumn*)(listCtrl.GetItemData(i));
		CString strDispName;
		strDispName.Format(_T("[%s]"), pColumn->Caption());
		CString strName = pColumn->Name();
		strSQL.Replace(strDispName, strName);
	}
}

CString KFieldCalculatorDlg::ConvertCaptionName(CString strCaptionSQL)
{
    CString strDisplayName = strCaptionSQL;
    try
    {
        const KIOColumns* pColumns = m_pTable->Columns();
        int nCount = pColumns->ColumnCount();
        for (int i=0; i<nCount; i++)
        {
            KIOColumn* pColumn = pColumns->GetColumn(i);
            CString    strName = pColumn->Name();
            CString    strReplace;
            strReplace.Format(_T("[%s]"), pColumn->Caption());
            strDisplayName.Replace(strName, strReplace);
        }
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
    	TxLogDebugException();
    }
    
    return strDisplayName;
}

void KFieldCalculatorDlg::InitFieldCombo( void )
{
	m_cboField.ResetContent();
	if (KmzSystem::GetLanguage() == KEMKorea) {
        m_cboField.InsertString(-1, _T("-----------------선택-----------------"));
    }
	else {
		m_cboField.InsertString(-1, _T("---------Select---------"));
	}

	const KIOColumns* pColumns = m_pTable->Columns();
	int   nColumns                      = pColumns->ColumnCount();

	KIOColumn* pColumn      = nullptr;
	CString    strTableName = m_pTable->Name();
	int        nIndex;

	for (int i = 0 ;i <nColumns; ++i)
	{
		pColumn = pColumns->GetColumn(i);

		if( pColumn->PrimaryKey() || pColumn->DataType() == KEMIODataTypeString )
			continue;

		CString strColumnName = pColumn->Name();

// 		if (strTableName.CompareNoCase(TABLE_NODE) == 0 && strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
// 			continue;
// 
// 		if (strTableName.CompareNoCase(TABLE_LINK) == 0 && strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0)
// 			continue;

		if (strTableName.CompareNoCase(TABLE_TRANSIT) == 0 && strColumnName.CompareNoCase(_T("type")) == 0)
			continue;

		nIndex = m_cboField.InsertString(-1, pColumn->Caption());
		m_cboField.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}

	m_cboField.SetCurSel(0);
}

void KFieldCalculatorDlg::SetResizeCtrl( void )
{

	SetResize(IDC_STATIC_TOP, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT1,      SZ_TOP_LEFT, SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_OPER,  CXTPResizePoint(0.6, 0),   SZ_TOP_RIGHT);
	SetResize(IDC_REPORT4,      SZ_TOP_LEFT,               CXTPResizePoint(0.6, 0.4));
	SetResize(IDC_REPORT,       CXTPResizePoint(0.6, 0),   CXTPResizePoint(1, 0.4));
	SetResize(IDC_STATIC_MID1,  CXTPResizePoint(0, 0.4),   CXTPResizePoint(1, 0.4));
	SetResize(IDC_STATIC_WHERE, CXTPResizePoint(0, 0.4),   CXTPResizePoint(1, 0.4));
	SetResize(IDC_EDIT2,        CXTPResizePoint(0, 0.4),   CXTPResizePoint(1, 0.7));
	SetResize(IDC_STATIC_MID2,  CXTPResizePoint(0, 0.7),   CXTPResizePoint(1, 0.7));
	SetResize(IDC_REPORT2,      CXTPResizePoint(0, 0.7),   CXTPResizePoint(0.6, 1));
	SetResize(IDC_REPORT3,      CXTPResizePoint(0.6, 0.7), CXTPResizePoint(1, 1));
	SetResize(IDC_BUTTON0,      CXTPResizePoint(0.6, 1),   CXTPResizePoint(0.6, 1));

	SetResize(IDC_STATIC_BOTTOM, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON1,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON5,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON6,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON7,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON8,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON9,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON10, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON11, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON12, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
}

BOOL KFieldCalculatorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
    m_editQuery.SetLimitText(0);
    //m_editQuery.SetWindowText(ConvertCaptionName(m_strInitFilter));
	m_editQuery.SetWindowText(m_strInitFilter);

	KReportCtrlSetting::Default(m_wndCalculateReport, FALSE);
	KReportCtrlSetting::VerticalNoLine(m_wndCalculateReport);
	KReportCtrlSetting::HorizontalNoLine(m_wndCalculateReport);
	m_wndCalculateReport.FocusSubItems(FALSE);

	KReportCtrlSetting::Default(m_wndOperatorReport,FALSE);
	KReportCtrlSetting::HideHeader(m_wndOperatorReport);
	KReportCtrlSetting::SelectionEnableFalse(m_wndOperatorReport);
	
	KReportCtrlSetting::Default(m_wndWhereReport, FALSE);
	KReportCtrlSetting::VerticalNoLine(m_wndWhereReport);
	KReportCtrlSetting::HorizontalNoLine(m_wndWhereReport);
	m_wndWhereReport.FocusSubItems(FALSE);

	KReportCtrlSetting::Default(m_wndUniqueReport, FALSE);
	m_wndUniqueReport.FocusSubItems(FALSE);

	SetResizeCtrl();
	InitFieldCombo();

	InitCalculateReport();
	UpdateCalculateRecord();

	InitOperatorReport();
	UpdateOperatorRecord();

	InitWhereReport();
	UpdateWhereRecord();

	InitUniqueReport();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KFieldCalculatorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editCalculate);
	DDX_Control(pDX, IDC_EDIT2, m_editQuery);
	
	DDX_Control(pDX, IDC_REPORT4, m_wndCalculateReport);
	DDX_Control(pDX, IDC_REPORT,  m_wndOperatorReport);
	DDX_Control(pDX, IDC_REPORT2, m_wndWhereReport);
	DDX_Control(pDX, IDC_REPORT3, m_wndUniqueReport);

	DDX_Control(pDX, IDC_COMBO1, m_cboField);
}

BEGIN_MESSAGE_MAP(KFieldCalculatorDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON0,  &KFieldCalculatorDlg::OnBnClickedButtonUniqueVal)
	ON_BN_CLICKED(IDC_BUTTON1,  &KFieldCalculatorDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2,  &KFieldCalculatorDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3,  &KFieldCalculatorDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4,  &KFieldCalculatorDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5,  &KFieldCalculatorDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6,  &KFieldCalculatorDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7,  &KFieldCalculatorDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8,  &KFieldCalculatorDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9,  &KFieldCalculatorDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &KFieldCalculatorDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &KFieldCalculatorDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &KFieldCalculatorDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDOK,         &KFieldCalculatorDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,     &KFieldCalculatorDlg::OnBnClickedCancel)

	ON_NOTIFY(NM_DBLCLK, IDC_REPORT4, &KFieldCalculatorDlg::OnReportCalculateDblClick)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT,  &KFieldCalculatorDlg::OnReportOperatorDblClick)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT2, &KFieldCalculatorDlg::OnReportWhereDblClick)
	ON_NOTIFY(NM_DBLCLK, IDC_REPORT3, &KFieldCalculatorDlg::OnReportUniqueDblClick)

END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KFieldCalculatorDlg::OnBnClickedButtonUniqueVal()
{
	try
	{
		CXTPReportRow* pRow = m_wndWhereReport.GetFocusedRow();

		if (NULL == pRow)
			ThrowException(_T("컬럼을 선택하세요."));

		CXTPReportRecordItem* pItem   = pRow->GetRecord()->GetItem(0);
		KIOColumn*            pColumn = (KIOColumn*)pItem->GetItemData();

		if (pColumn == NULL)
			ThrowException(_T("컬럼 정보가 없습니다."));

		CString strCountQuery;
		
		m_strUniqueValQuery.Format(_T("Select %s, Count(*) From %s Group By %s Order By %s"), 
			pColumn->Name(), m_pTable->Name(), pColumn->Name(), pColumn->Name());
		strCountQuery.Format(_T("Select Count(*) From(%s)"), m_strUniqueValQuery);

		int nRecords(0);
		GetCountUniqueRecord(m_pTable, strCountQuery, nRecords);

		if (nRecords > 20000)
			ThrowException(_T("20000건 이하의 데이터만 조회할 수 있습니다."));


		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(GetUniqueValThreadCaller, this, false, nLang);

		if (!m_bResultGetUniqueValThread)
			ThrowException(_T("데이터 조회에 실패했습니다."));

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


void KFieldCalculatorDlg::OnBnClickedButton1()
{
	addTextToEdit( m_editQuery, _T(" = ") );
}


void KFieldCalculatorDlg::OnBnClickedButton2()
{
	addTextToEdit( m_editQuery, _T(" != ") );
}


void KFieldCalculatorDlg::OnBnClickedButton3()
{
	addTextToEdit( m_editQuery, _T(" > ") );
}


void KFieldCalculatorDlg::OnBnClickedButton4()
{
	addTextToEdit( m_editQuery, _T(" >= ") );
}


void KFieldCalculatorDlg::OnBnClickedButton5()
{
	addTextToEdit( m_editQuery, _T(" < ") );
}


void KFieldCalculatorDlg::OnBnClickedButton6()
{	
	addTextToEdit( m_editQuery, _T(" <= ") );
}


void KFieldCalculatorDlg::OnBnClickedButton7()
{
	addTextToEdit( m_editQuery, _T(" () ") );
	
	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );
}


void KFieldCalculatorDlg::OnBnClickedButton8()
{
	addTextToEdit( m_editQuery, _T(" AND ") );
}


void KFieldCalculatorDlg::OnBnClickedButton9()
{
	addTextToEdit( m_editQuery, _T(" OR ") );
}


void KFieldCalculatorDlg::OnBnClickedButton10()
{
	addTextToEdit( m_editQuery, _T(" '' ") );

	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );
}


void KFieldCalculatorDlg::OnBnClickedButton11()
{
	addTextToEdit( m_editQuery, _T(" '%' ") );

	int nStart, nEnd;
	m_editQuery.GetSel( nStart, nEnd );
	m_editQuery.SetSel( nEnd - 2, nEnd - 2 );
}


void KFieldCalculatorDlg::OnBnClickedButton12()
{
	addTextToEdit( m_editQuery, _T(" LIKE ") );
}


void KFieldCalculatorDlg::OnBnClickedOk()
{
	CString strSet, strWhere;
	m_editCalculate.GetWindowText( strSet );
	strSet.Trim();
	
	try
	{
		m_bResultOK = false;

		//#0. Check Field Combo;
		int nCursorIndex = m_cboField.GetCurSel();

		if (nCursorIndex == 0)
			ThrowException(_T("적용할 Field를 선택하세요."));

		//#1. 수식 존재여부 체크
		if (strSet.IsEmpty())
			ThrowException(_T("수식이 없습니다."));
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}
		QBicSimpleProgressThread::ExecuteThread(FieldCalculatorThreadCaller, this, false, nLang);

		if (m_bResultOK) {
			m_pTable->Notify();
			AfxMessageBox(_T("적용되었습니다."));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KFieldCalculatorDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void KFieldCalculatorDlg::InitCalculateReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIndex(0);

	//Field
	pColumn = m_wndCalculateReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Field"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("입력 컬럼 선택"));
    }

	//Type
	pColumn = m_wndCalculateReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Type"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("데이터 유형"));
    }

	m_wndCalculateReport.Populate();
}

void KFieldCalculatorDlg::UpdateCalculateRecord( void )
{
	m_wndCalculateReport.ResetContent();

	const KIOColumns* pColumns = m_pTable->Columns();
	int   nColumns                      = pColumns->ColumnCount();

	KIOColumn* pColumn      = nullptr;
	CString    strTableName = m_pTable->Name();
	CString    strCaption, strType;
	
	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (int i = 0 ; i < nColumns; ++i)
	{
		pColumn = pColumns->GetColumn(i);

		if( pColumn->PrimaryKey() || pColumn->DataType() == KEMIODataTypeString )
			continue;

		CString strColumnName = pColumn->Name();

// 		if (strTableName.CompareNoCase(TABLE_NODE) == 0 && strColumnName.CompareNoCase(COLUMN_NODE_TYPE) == 0)
// 			continue;
// 
// 		if (strTableName.CompareNoCase(TABLE_LINK) == 0 && strColumnName.CompareNoCase(COLUMN_LINK_TYPE) == 0)
// 			continue;

		strCaption = pColumn->Caption();

		switch(pColumn->DataType())
		{
		case KEMIODataTypeInteger:
			strType = DATATYPESTRING_INTEGER;
			break;
		case KEMIODataTypeDouble:
			strType = DATATYPESTRING_DOUBLE;
			break;;
		case KEMIODataTypeString:
			strType = DATATYPESTRING_STRING;
			break;;
		}

		pRecord = m_wndCalculateReport.AddRecord(new CXTPReportRecord);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
		pItem->SetItemData((DWORD_PTR)pColumn);

		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strType));
	}

	m_wndCalculateReport.Populate();
}

void KFieldCalculatorDlg::InitOperatorReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColumnIndex(0);

	pColumn = m_wndOperatorReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Operators"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);

	pColumn = m_wndOperatorReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("Functions"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
	
	m_wndOperatorReport.Populate();
}

void KFieldCalculatorDlg::UpdateOperatorRecord( void )
{
	m_wndOperatorReport.ResetContent();

	CXTPReportColumns* pColumns = m_wndOperatorReport.GetColumns();
	int                nColumns = pColumns->GetCount();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	vector<CString> vecOperator = m_oExpression.Operators();
	vector<CString> vecFunction = m_oExpression.Functions();

	int nOperators  = vecOperator.size();
	int nFunctions  = vecFunction.size();

	int nMaxSize = nOperators;

	if (nFunctions > nMaxSize)
		nMaxSize = nFunctions;

	for (int i = 0; i < nMaxSize; ++i)
	{
		//Operator Column
		pRecord = m_wndOperatorReport.AddRecord(new CXTPReportRecord);
		
		if (i < nOperators)
		{
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(vecOperator[i]));
		}
		else
		{
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}

		//Function Column
		if (i < nFunctions)
		{
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(vecFunction[i]));
		}
		else
		{
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
		}
	}

	m_wndOperatorReport.Populate();
}

void KFieldCalculatorDlg::InitWhereReport( void )
{
	CXTPReportColumn* pColumn = nullptr;

	int nColIdx(0);

	//Field
	pColumn = m_wndWhereReport.AddColumn(new CXTPReportColumn(nColIdx++, _T("Field"), 2));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼명"));
    }

	//Type
	pColumn = m_wndWhereReport.AddColumn(new CXTPReportColumn(nColIdx++, _T("Type"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("데이터 유형"));
    }

	m_wndWhereReport.Populate();
}

void KFieldCalculatorDlg::UpdateWhereRecord( void )
{
	m_wndWhereReport.ResetContent();

	const KIOColumns* pColumns = m_pTable->Columns();
	int nColumns = pColumns->ColumnCount();

	CString strCaption, strType;

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for (int i = 0; i < nColumns; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		strCaption = pColumn->Caption();

		switch(pColumn->DataType())
		{
		case KEMIODataTypeInteger:
			strType = DATATYPESTRING_INTEGER;
			break;
		case KEMIODataTypeDouble:
			strType = DATATYPESTRING_DOUBLE;
			break;;
		case KEMIODataTypeString:
			strType = DATATYPESTRING_STRING;
			break;;
		}

		pRecord = m_wndWhereReport.AddRecord(new CXTPReportRecord);
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strCaption));
		pItem->SetItemData((DWORD_PTR)pColumn);
		
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strType));
	}

	m_wndWhereReport.Populate();

}

void KFieldCalculatorDlg::GetCountUniqueRecord( KIOTable* a_pTable, CString a_strQuery, int& ar_count )
{
	try
	{
		KDBaseConPtr spDBaseConnection = a_pTable->GetDBaseConnection();

		KResultSetPtr oResultSetPtr = spDBaseConnection->ExecuteQuery(a_strQuery);

		if (oResultSetPtr->Next())
			ar_count = oResultSetPtr->GetValueInt(0);
	}
	catch (KExceptionPtr ex)
	{
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KFieldCalculatorDlg::InitUniqueReport( void )
{
	int nColIdx(0);

	CXTPReportColumn* pColumn = nullptr;

	//Value
	pColumn = m_wndUniqueReport.AddColumn(new CXTPReportColumn(nColIdx++, _T("Value"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("데이터 값"));
    }

	//Count
	pColumn = m_wndUniqueReport.AddColumn(new CXTPReportColumn(nColIdx++, _T("Count"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("중복 횟수"));
    }

	m_wndUniqueReport.Populate();
}

void KFieldCalculatorDlg::ConvertFieldName( CXTPReportControl& wndReport, CString& strQuery )
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

void KFieldCalculatorDlg::OnReportCalculateDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		CString strColumn;
		strColumn.Format(_T("[%s]"), pItem->GetValue());

		addTextToEdit(m_editCalculate, strColumn);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KFieldCalculatorDlg::OnReportOperatorDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(pItemNotify->pColumn->GetItemIndex());

		CString strColumn = pItem->GetValue();
		
		addTextToEdit(m_editCalculate, strColumn);

		if (strColumn.Find(_T("()")) != -1)		//Operator에 () 있을 시 커서를 괄호 사이로 이동.
		{
			int nStart, nEnd;
			m_editCalculate.GetSel( nStart, nEnd );
			m_editCalculate.SetSel( nEnd - 1, nEnd - 1 );
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KFieldCalculatorDlg::OnReportWhereDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		CString strColumn;
		strColumn.Format(_T("[%s]"), pItem->GetValue());

		addTextToEdit(m_editQuery, strColumn);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KFieldCalculatorDlg::OnReportUniqueDblClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*  pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*         pRecord     = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText* pItem       = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		addTextToEdit(m_editQuery, pItem->GetValue());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}

void KFieldCalculatorDlg::DeallocateMap( void )
{
	int nSize = m_mapResultRecord.size();

	if (nSize != 0)
	{
		map<__int64, KSDoubleRecord*>::iterator beginIter, endIter = m_mapResultRecord.end();

		for (beginIter = m_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
		{
			KSDoubleRecord* pReocrd = beginIter->second;

			if (pReocrd != NULL)
				pReocrd->RemoveAll();

			delete pReocrd;
		}

		m_mapResultRecord.clear();
	}
}

unsigned __stdcall KFieldCalculatorDlg::GetUniqueValThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 조회 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Data Loading..."));
	}

	KFieldCalculatorDlg* pDlg = (KFieldCalculatorDlg*)pParameter->GetParameter();
	pDlg->UpdateUniqueRecord();

	return 0;
}

void KFieldCalculatorDlg::UpdateUniqueRecord( void )
{
	m_wndUniqueReport.ResetContent();
	m_bResultGetUniqueValThread = false;

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(m_strUniqueValQuery);

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (spResultSet->Next())
		{
			pRecord = m_wndUniqueReport.AddRecord(new CXTPReportRecord);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(spResultSet->GetValueString(0)));
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(spResultSet->GetValueString(1)));
		}

		m_wndUniqueReport.Populate();

		m_bResultGetUniqueValThread = true;
	}
	catch (KExceptionPtr ex)
	{
		m_bResultGetUniqueValThread = false;	
	}
	catch (...)
	{
		m_bResultGetUniqueValThread = false;
	}
}

unsigned __stdcall KFieldCalculatorDlg::FieldCalculatorThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 적용 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Applying Data..."));
	}

	KFieldCalculatorDlg* pDlg = (KFieldCalculatorDlg*)pParameter->GetParameter();
	pDlg->Apply();

	return 0;
}

void KFieldCalculatorDlg::Apply( void )
{
	CString strSet, strWhere;
	m_editCalculate.GetWindowText( strSet );
	strSet.Trim();

	m_editQuery.GetWindowText( strWhere );
	strWhere.Trim();

	try
	{
		DeallocateMap();

		//#0. Check Field Combo;
		int nCursorIndex = m_cboField.GetCurSel();

		if (nCursorIndex == 0)
			ThrowException(_T("적용할 Field를 선택하세요."));

		//#1. 수식 존재여부 체크
		if (strSet.IsEmpty())
			ThrowException(_T("수식이 없습니다."));

		int nIdxTarget(-1);	//mapResultRecord상의 수정될 컬럼의 Index
		
		KIOColumn* pColumn           = (KIOColumn*)m_cboField.GetItemData(nCursorIndex);
		CString    strTargetName     = pColumn->Name();
		CString    strFormulaCaption = strSet;

		vector<TFormulaField> vecFormulaField;	//Set formulaField
		vector<CString>       vecCalcColumn;	//Set CalcRecord Info;

		int nCntCombo = m_cboField.GetCount();

		for (int i = 0; i < nCntCombo; ++i)
		{
			if (i == 0)		//index(0)은 "---Select---"이므로 Pass
				continue;

			KIOColumn* pColumn       = (KIOColumn*)m_cboField.GetItemData(i);
			CString    strCaption    = pColumn->Caption();
			CString    strColumnName = pColumn->Name();

			if (strColumnName.CompareNoCase(strTargetName) == 0)
				nIdxTarget = (i - 1);

			TFormulaField oFormulaField;
			oFormulaField.TFieldName    = strColumnName;
			oFormulaField.TFieldCaption = strCaption;

			vecFormulaField.push_back(oFormulaField);
			vecCalcColumn.push_back(strColumnName);
		}

		if (nIdxTarget == -1)
			ThrowException(_T("컬럼 매칭 실패..."));

		ConvertFieldName(m_wndWhereReport, strWhere);

		//#2. Get RecordSet
		KDBaseFieldCalculator::GetRecordSet(m_pTable, vecCalcColumn, strWhere, m_mapResultRecord);

		if ((int)m_mapResultRecord.size() == 0)
			ThrowException(_T("Where 절에 해당하는 데이터가 없습니다."));

		m_oExpression.SetCaptionExpression(strFormulaCaption, vecFormulaField);

		vector<CString> vecField       = m_oExpression.GetUsedRealFieldName();
		vector<int>     vecIdx;

		//vecField와 매칭하는 컬럼의 Index를 가지는 Vector;
		int nCntVecField = vecField.size();

		for (int i = 0; i < nCntVecField; ++i)
		{
			CString strColumnName = vecField[i];

			for (int j = 1; j < nCntCombo; ++j)
			{
				KIOColumn* pColumn = (KIOColumn*)m_cboField.GetItemData(j);

				if (strColumnName.CompareNoCase(pColumn->Name()) == 0)
					vecIdx.push_back(j-1);
			}
		}

		if ( vecField.size() != vecIdx.size() )
			ThrowException(_T("데이터 매칭 실패..."));

		CString         strRealFormula = m_oExpression.GetRealExpression();

		double* dValueArray = new double[nCntVecField];

		try
		{
			KParser oParser;
			oParser.SetExpr(strRealFormula);

			for (int i = 0; i< nCntVecField; ++i)
			{
				oParser.DefineVar(vecField[i],  &dValueArray[i]);
			}

			map<__int64, KSDoubleRecord*>::iterator beginIter, endIter = m_mapResultRecord.end();

			for (beginIter = m_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
			{
				KSDoubleRecord* pRecord = beginIter->second;

				for (int i = 0; i < nCntVecField; ++i)
				{
					dValueArray[i] = pRecord->GetAt(vecIdx[i]);
				}

				double dResultValue = oParser.Eval();

				pRecord->SetAt(nIdxTarget, dResultValue);
			}

			delete[] dValueArray;

		}
		catch (KExceptionPtr ex)
		{
			TxLogDebugException();
			delete[] dValueArray;
			throw ex;
		}
		catch (...)
		{
			TxLogDebugException();
			delete[] dValueArray;
			throw 1;
		}

		KDBaseFieldCalculator::ApplyCalcResult(m_pTable, strTargetName, nIdxTarget, m_mapResultRecord);

		/*m_pTable->Notify();*/

		m_bResultOK = true;
	}
	catch(KExceptionPtr ex)
	{
		m_bResultOK = false;
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		m_bResultOK = false;
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KFieldCalculatorDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}
