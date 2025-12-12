// MultiStatisticsBaseDefineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiStatisticsBaseDefineDlg.h"
#include "MultiStatisticsDlg.h"
#include "StatisticsFunctionDefineDlg.h"
#include "CustomReportRecordItem.h"
#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
//^#include "DBaseConnector.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"
#include "GRegressionExpression.h"
#include "GenerationInfo.h"
#include "KExRecordItem.h"
#include <fstream>
#include <iostream>

#include "Launcher.h"
#include "ImChampDir.h"

enum
{
	_0_COLUMN_CHECK = 0,    
	_1_COLUMN_BASE,
	_2_COLUMN_COMPARE
};

#define DEFAULT_INIT_GRID_RECORDCOUNT 3
#define EDIT_FUNCTION_TEXT _T("Edit Function") 
#define STATISTIC_MAIN_DATAFIELD_CNT 7	/*1.RMSE,	2.PRMSE,	3.MARE, 4.MAE,	5.RSQUARE,  6.CHISQUARE,	7.T-TEST    */
#define STATISTIC_PART_DATAFIELD_CNT 7 /* 1.합계,	2.평균,		3.최소,	4.최대,	5.분산,		6.표준편차,		7.표준오차	*/

// KMultiStatisticsBaseDefineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultiStatisticsBaseDefineDlg, KResizeDialogEx)

bool KMultiStatisticsBaseDefineDlg::m_bRunStatisticSucceess = false;

KMultiStatisticsBaseDefineDlg::KMultiStatisticsBaseDefineDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMultiStatisticsBaseDefineDlg::IDD, pParent)
	, m_strFunction(_T(""))
	, m_bChkZero(TRUE)
	, m_bChkGroup(FALSE)
{
	m_pExpression      = new KGRegressionExpression();
	m_strFilter = _T("");
	m_pSelectedColumns = new KIOColumns();
}

KMultiStatisticsBaseDefineDlg::~KMultiStatisticsBaseDefineDlg()
{
	delete m_pExpression;
	delete m_pSelectedColumns;
}

void KMultiStatisticsBaseDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Text(pDX, IDC_EDIT_FUNCTION, m_strFunction);
	DDX_Check(pDX, IDC_CHECK1, m_bChkZero);
	DDX_Check(pDX, IDC_CHECK2, m_bChkGroup);
	DDX_Control(pDX, IDC_COMBO1, m_cboGroup);
}


BEGIN_MESSAGE_MAP(KMultiStatisticsBaseDefineDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK, &KMultiStatisticsBaseDefineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_FUNCTION, &KMultiStatisticsBaseDefineDlg::OnBnClickedBtnFunction)
	ON_COMMAND(ID_SELECT_CANCEL, &KMultiStatisticsBaseDefineDlg::OnSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KMultiStatisticsBaseDefineDlg::OnSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT, OnReportColumnRButtonClick)
	ON_BN_CLICKED(IDC_CHECK2, &KMultiStatisticsBaseDefineDlg::OnBnClickedCheckGroupFiled)
	ON_BN_CLICKED(IDC_BUTTON1, &KMultiStatisticsBaseDefineDlg::OnBnClickedButtonAdd)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// KMultiStatisticsBaseDefineDlg 메시지 처리기입니다.

BOOL KMultiStatisticsBaseDefineDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();    
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if(m_pTable != NULL)
	{
		SetDlgItemText(IDC_STATIC_NAME, m_pTable->Caption());

		CString strColumnGroup = _T("");
		strColumnGroup.Format(_T(" %s "), m_pTable->Caption());
		SetDlgItemText(IDC_STATIC_COLUMN, strColumnGroup);
	}

	if(m_pTarget != NULL)
	{
		CString strTargetYear = _T("");
		strTargetYear.Format(_T("[ %s ]"), m_pTarget->Caption());
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strTargetYear);
	}

	KReportCtrlSetting::Default(m_wndReportCtrl, FALSE, FALSE, FALSE);
    InitReportHeader();
	InitReportData();    
	m_wndReportCtrl.Populate();
 
 	InitGroupFiled();
 	m_cboGroup.SetCurSel(0);
// 
// 	DisableGroupFiled(m_bChkGroup);

	SetResizeComponent();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMultiStatisticsBaseDefineDlg::SetResizeComponent()
{
	SetResize(IDC_STATIC_COLUMN,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_REPORT,           SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON1,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_FUNCTION,  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_EDIT_FUNCTION,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BTN_FUNCTION,     SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR,         SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK1,           SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK2,           SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO1,           SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                 SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,             SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KMultiStatisticsBaseDefineDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;
	CXTPReportRecordItemEditOptions* pBaseEditOptions = NULL;
	CXTPReportRecordItemEditOptions* pCompareEditOptions = NULL;

	m_wndReportCtrl.SelectionEnable(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 60, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_BASE,       _T("Base Column"), 50));
	pBaseEditOptions = pColumn->GetEditOptions();
	pBaseEditOptions->AddComboButton(TRUE);
	pBaseEditOptions->m_bAllowEdit = TRUE;
	pBaseEditOptions->m_bExpandOnSelect = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기준 컬럼"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_COMPARE,    _T("Compare Column"), 50));
	pCompareEditOptions = pColumn->GetEditOptions();
	pCompareEditOptions->AddComboButton(TRUE);
	pCompareEditOptions->m_bAllowEdit = TRUE;
	pCompareEditOptions->m_bExpandOnSelect = TRUE;
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비교 컬럼"));
    }

	InputGridComboData(pBaseEditOptions, pCompareEditOptions);
	
	// header 설정
	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);

//	m_wndReportCtrl.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
	/*
	m_wndReportCtrl.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
	*/
	m_wndReportCtrl.AllowEdit(TRUE);
	m_wndReportCtrl.EditOnClick(TRUE);
	m_wndReportCtrl.SetMultipleSelection(FALSE);

/*
	pItem->GetEditOptions(NULL)->m_bSelectTextOnEdit = TRUE;
	pItem->GetEditOptions(NULL)->m_dwEditStyle |= ES_NUMBER;
#define CBS_SIMPLE            0x0001L
#define CBS_DROPDOWN          0x0002L
#define CBS_DROPDOWNLIST      0x0003L
#define CBS_OWNERDRAWFIXED    0x0010L
#define CBS_OWNERDRAWVARIABLE 0x0020L
#define CBS_AUTOHSCROLL       0x0040L
#define CBS_OEMCONVERT        0x0080L
#define CBS_SORT              0x0100L
#define CBS_HASSTRINGS        0x0200L
#define CBS_NOINTEGRALHEIGHT  0x0400L
#define CBS_DISABLENOSCROLL   0x0800L
#if(WINVER >= 0x0400)
#define CBS_UPPERCASE         0x2000L
#define CBS_LOWERCASE         0x4000L*/
}


void KMultiStatisticsBaseDefineDlg::InputGridComboData(	CXTPReportRecordItemEditOptions* a_pBaseEditOptions, CXTPReportRecordItemEditOptions* a_pCompareEditOptions )
{
	KIOColumn* pColumn = NULL;
	CString strFirstCaption = EDIT_FUNCTION_TEXT;

	// 마지막을 제외한 나머지 항목은 실제 Filed 항목 들을 셋팅
	const 
		KIOColumns* pColumns = m_pTable->Columns();

	int nInputCount(0);
	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		pColumn = pColumns->GetColumn(i);

        if (pColumn->PrimaryKey())
            continue;

		if(/*pColumn->CodeType() == KEMIOCodeTypeIsSingleCode ||*/ pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			continue;
		}

		if( KEMIODataTypeString == pColumn->DataType())
		{
			continue;
		}

		a_pBaseEditOptions->AddConstraint(    pColumn->Caption(), (DWORD_PTR)pColumn ); 
		a_pCompareEditOptions->AddConstraint( pColumn->Caption(), (DWORD_PTR)pColumn );

		nInputCount++;
	}

	if (nInputCount < 1)
	{
		a_pBaseEditOptions->AddConstraint   ( _T("null"), (DWORD_PTR)nullptr ); 
		a_pCompareEditOptions->AddConstraint( _T("null"), (DWORD_PTR)nullptr );
	}

	// 마지막 항목은 Function Editor 라는 의미의 데이터를 셋팅
	/*pColumn = NULL;
	a_pBaseEditOptions->AddConstraint(    strFirstCaption, (DWORD_PTR)pColumn ); 
	a_pCompareEditOptions->AddConstraint( strFirstCaption, (DWORD_PTR)pColumn );*/
}


void KMultiStatisticsBaseDefineDlg::InitReportData()
{
	m_wndReportCtrl.ResetContent(TRUE);
	for(int i= 0; i< DEFAULT_INIT_GRID_RECORDCOUNT; i++)
	{
		AddReportData();
	}
}


void KMultiStatisticsBaseDefineDlg::AddReportData()
{
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

	pItem = pRecord->AddItem(new KExRecordItemCheck());
	pItem->HasCheckbox(TRUE);
	pItem->SetChecked(FALSE);
	pItem->SetAlignment(xtpColumnIconCenter);

	pItem = pRecord->AddItem(new CCustomReportRecordItem);
	pItem->SetAlignment(DT_LEFT);

	pItem = pRecord->AddItem(new  CCustomReportRecordItem);
	pItem->SetAlignment(DT_LEFT);
}


void KMultiStatisticsBaseDefineDlg::InitGroupFiled()
{
	const KIOColumns* pColumns = m_pTable->Columns();

	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);

		if( pColumn->PrimaryKey() == true )
		{
			continue;
		}

		if( pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)
			/*if( pColumn->DataType() == KEMIODataTypeInteger && pColumn->CodeType() == KEMIOCodeTypeIsSingleCode)*/
		{
			int nCur = m_cboGroup.AddString(pColumn->Caption());
			m_cboGroup.SetItemData(nCur, DWORD_PTR(pColumn));
		}
	}

	if(m_cboGroup.GetCount() < 1)
	{
		GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		m_bChkGroup = FALSE;
		UpdateData(FALSE);
	}
}


void KMultiStatisticsBaseDefineDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
		
	std::set<int> setSelectedColumnIndex;
	std::set<int>::iterator iter;
	
	CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem = NULL; 

	int nCheckCount = 0;
	m_vecMultiColumn.clear();
	m_pSelectedColumns->Clear();
	for(int i= 0; i< pRecords->GetCount(); i++)
	{
		pRecord = pRecords->GetAt(i);

		pItem   = pRecord->GetItem(_0_COLUMN_CHECK);
		if( FALSE == pItem->IsChecked())
		{
			continue;
		}

		pItem   =  pRecord->GetItem(_1_COLUMN_BASE);
		KIOColumn* pBaseColumn    = (KIOColumn*)pItem->GetItemData();

		pItem   =  pRecord->GetItem((_2_COLUMN_COMPARE));
		KIOColumn* pCompareColumn = (KIOColumn*)pItem->GetItemData();

		if (nullptr == pBaseColumn)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("%d번째 선택 항목의 기준 컬럼을 선택해 주세요."), i+1);
			AfxMessageBox(strMsg);
			return;
		}

		if (nullptr == pCompareColumn)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("%d번째 선택 항목의 비교 컬럼을 선택해 주세요."), i+1);
			AfxMessageBox(strMsg);
			return;
		}

		TMultiColumn oTMultiColumn;
		oTMultiColumn.pBaseColumn    = pBaseColumn;
		oTMultiColumn.pCompareColumn = pCompareColumn;

		m_vecMultiColumn.push_back(oTMultiColumn);

		iter = setSelectedColumnIndex.find( pBaseColumn->DisplayOrder() ); 
		if( iter == setSelectedColumnIndex.end())
		{
			m_pSelectedColumns->AddNewColumn(pBaseColumn);
		}
		iter = setSelectedColumnIndex.find( pCompareColumn->DisplayOrder() );
		if( iter == setSelectedColumnIndex.end())
		{
			m_pSelectedColumns->AddNewColumn(pCompareColumn);
		}

		nCheckCount++;
	}
/*
	if(pSelectedColumns->ColumnCount() < 1 && m_strFunction.Compare(_T("")) == 0)
	{
		AfxMessageBox(_T("컬럼을 선택하거나 수식을 설정해 주세요."));
		return;
	}
*/
	if(nCheckCount < 1)
	{
		AfxMessageBox(_T("컬럼 정보를 선택해 주세요."));
		return;
	}

	KMultiStatisticsDlg dlg(this);
	dlg.SetInitValue  (m_pTable, m_pSelectedColumns, m_strFilter);
	dlg.SetMultiColumn(m_vecMultiColumn);

	m_pGroupColumn = GetCurGroupField();
	m_nGroupFieldTypeCount = GroupFiledTypeCount(m_pGroupColumn);

	dlg.SetGroupColumn(m_pGroupColumn);
	dlg.SetGroupTypeCount(m_nGroupFieldTypeCount);


	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}
	QBicSimpleProgressThread::ExecuteThread(StatisticThreadCaller, this, false, nLang);

	if( m_bRunStatisticSucceess )
	{
		dlg.SetStatisticMainData(m_vecStatisticMain);
		dlg.SetStatisticPartData(m_mapStatisticPart);

		dlg.DoModal();
	}


	/*KResizeDialogEx::OnOK();*/
}


int KMultiStatisticsBaseDefineDlg::GroupFiledTypeCount(KIOColumn* a_pGroupColumn)
{
	if(NULL == a_pGroupColumn)
	{
		return 0;
	}

    KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}

	strSQL.Format (_T(" SELECT COUNT(*) AS GROUP_CNT FROM ( SELECT %s FROM %s %s GROUP BY %s ) "), a_pGroupColumn->Name(), m_pTable->Name(), strWhere, a_pGroupColumn->Name());

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return 0;
	}

	int nTypeCount = 0;
	while( pResult->Next() )
	{
		nTypeCount = pResult->GetValueInt(0);
	}

	return nTypeCount;
}


unsigned __stdcall KMultiStatisticsBaseDefineDlg::StatisticThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 통계량 연산 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Calculating for Statistics..."));
	}

	KMultiStatisticsBaseDefineDlg* pDlg = (KMultiStatisticsBaseDefineDlg*)pParameter->GetParameter();

	m_bRunStatisticSucceess = pDlg->GetStatisticsData();

	return 0;
}


bool KMultiStatisticsBaseDefineDlg::GetStatisticsData()
{
	TxLogDebugStartMsg();

	if(! CreateArgument())
	{
		TxLogDebugWarning();
		return false;
	}
    
	m_nResultCode = DLL_UNEXPECTED_ERROR;
	CString strWindowName;
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("간편 통계(적합도) 모듈 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Statistics(Goodness of Fit) Module..."));
	}

	KLauncher::Run(strWindowName, 10);

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Statistics(Goodness of Fit) Module Complete."));
		}
		TxLogDebug(_T("Calculate statistics Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Model Stop.."));
        m_nResultCode = DLL_USER_STOPPING;
		return false;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return false;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Calculate statistics Run Error.."));
		return false;
	}

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("모듈 실행 결과 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Reading Output File..."));
	}

	if (! ReadOutPutFile() )
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Output 통계 분석 정보 입력 실패..."));

		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KMultiStatisticsBaseDefineDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KMultiStatisticsBaseDefineDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

bool KMultiStatisticsBaseDefineDlg::CreateArgument()
{
	TxLogDebugStartMsg();
	if(! ClearFiles())
	{
		return false;
	}

	try
	{
		CString strFileName;
		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), T_ARG_STATISTICS);
		
		//TxLogDebug("END CLEAR FILE ------ ");
		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(strFileName, nOpenMode);

		bool bResult = false;

		// .arg 작성
		bResult = WriteStatisticsArgument(of);
		if(! bResult)
		{
			of.Close();
			AfxMessageBox(_T("Argument(Statistics) 파일 생성 중 오류가 발생하였습니다."));
			return false;
		}

		of.Close();
		//TxLogDebug("END ARG CREAETE FILE ------ ");

		bResult = BulkSelect(m_pTable->GetDBaseConnection());;
		if(! bResult)
		{
			AfxMessageBox(_T("StaData.dat 파일 생성 중 오류가 발생하였습니다."));
			return false;
		}
		//TxLogDebug("END COLLECTION DATA ------ ");
	}
	catch(...)
	{
		AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KMultiStatisticsBaseDefineDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		CString strFileName;

		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), ARG_STATISTICS);
		nResult = remove((CStringA)strFileName);

		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), DAT_IN_STATISTICS);
		nResult = remove((CStringA)strFileName);

		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), DAT_OUT_STATISTICS);
		nResult = remove((CStringA)strFileName);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KMultiStatisticsBaseDefineDlg::WriteStatisticsArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	size_t nMultiAnlysisCount =  m_vecMultiColumn.size();
	int nIncludeZero = (m_bChkZero == TRUE)? 1 : 0; // 0: not include zero  1: include zero 

	try
	{
		of.WriteString(_T("noMultiAnalysis\t"));
		strOut.Format(_T("%d\r\n"), nMultiAnlysisCount);
		of.WriteString(strOut);

		of.WriteString(_T("IncludeZero\t"));
		strOut.Format(_T("%d\r\n"), nIncludeZero);
		of.WriteString(strOut);

		of.WriteString(_T("noGroup\t"));
		strOut.Format(_T("%d\r\n"), m_nGroupFieldTypeCount);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KMultiStatisticsBaseDefineDlg::ReadOutPutFile()
{
	bool bResult = false;
	TxLogDebugStartMsg();
	if(NULL == m_pGroupColumn)
	{
		bResult = ReadNormalOutPutFile();
	}
	else 
	{
		bResult = ReadGroupOutPutFile();
	}

	TxLogDebugEndMsg();
	return bResult;
}


bool KMultiStatisticsBaseDefineDlg::ReadNormalOutPutFile()
{
	TxLogDebugStartMsg();
	try
	{
		CString strFileName;
		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), T_DAT_OUT_STATISTICS);

		std::ifstream ifs(strFileName, std::ios::binary);    

		if (!ifs.is_open())
		{
			throw -1;
		}

		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert statistics result"));

			int nCount = 0;
			m_vecStatisticMain.clear();
			m_mapStatisticPart.clear();

			//- Read Main Data

			for(size_t i= 0; i< m_vecMultiColumn.size(); i++)
			{
				TStatisticResultMain oTStatisticsResultMain;
				oTStatisticsResultMain.bGroupInfo = false;
				oTStatisticsResultMain.nxGroupID  = 0;
				std::vector<double> &vecData = oTStatisticsResultMain.vecData;

				for(int k = 0; k< STATISTIC_MAIN_DATAFIELD_CNT; k++)
				{
					double dValue = 0.0;
					ifs.read( reinterpret_cast<char*>( &dValue),        sizeof(double)  );

					vecData.push_back(dValue);
				}

				m_vecStatisticMain.push_back(oTStatisticsResultMain);
				nCount++;
			}

			//- Read Part Data
			for(size_t i= 0; i< m_vecMultiColumn.size(); i++)
			{
				std::vector<TStatisticResultPart> vecStatisticPart;

				for(int j= 0; j< 2; j++)
				{
					TStatisticResultPart oTStatisticResultPart;
					oTStatisticResultPart.bGroupInfo = false;
					oTStatisticResultPart.nxGroupID  = 0;
					std::vector<double> &vecData = oTStatisticResultPart.vecData;

					for(int k= 0; k< STATISTIC_PART_DATAFIELD_CNT; k++)
					{
						double dValue = 0.0;
						ifs.read( reinterpret_cast<char*>( &dValue),        sizeof(double)  );

						vecData.push_back(dValue);
					}

					vecStatisticPart.push_back(oTStatisticResultPart);
					nCount++;
				}

				m_mapStatisticPart.insert(std::make_pair((int)i, vecStatisticPart));
			}

			/*if(!ifs.eof())
			{
				CString strMsg = _T("");
				strMsg.Format(_T("--- error statistics result size "));
				TxLogDebug((LPCTSTR)strMsg );

				ifs.close();
				return false;
			} 파일 끝 검사 */

			CString strResult = _T("");
			strResult.Format(_T("--- end insert statistics result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			ifs.close();
			TxLogDebugException();
			return false;
		}

		ifs.close();
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


bool KMultiStatisticsBaseDefineDlg::ReadGroupOutPutFile()
{
	TxLogDebugStartMsg();
	try
	{
		CString strFileName;
		strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), T_DAT_OUT_STATISTICS);

		std::ifstream ifs(strFileName, std::ios::binary);    

		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert statistics result"));

			int nCount = 0;
			m_vecStatisticMain.clear();
			m_mapStatisticPart.clear();

			//- Read Main Data
			for(size_t i= 0; i< m_vecMultiColumn.size(); i++)
			{
				for(int j= 0; j< m_nGroupFieldTypeCount; j++)
				{
					// - get group type ID
					Integer nxGroupID = 0;
					ifs.read( reinterpret_cast<char*>( &nxGroupID),        sizeof(Integer)  );

					TStatisticResultMain oTStatisticsResultMain;
					oTStatisticsResultMain.bGroupInfo = true;
					oTStatisticsResultMain.nxGroupID  = nxGroupID;
					std::vector<double> &vecData = oTStatisticsResultMain.vecData;

					// - get data
					for(int k= 0; k< STATISTIC_MAIN_DATAFIELD_CNT; k++)
					{
						double dValue = 0.0;
						ifs.read( reinterpret_cast<char*>( &dValue),       sizeof(double)   );

						vecData.push_back(dValue);
					}

					m_vecStatisticMain.push_back(oTStatisticsResultMain);
					nCount++;
				}
			}

			//- Read Part Data
			for(size_t i= 0; i< m_vecMultiColumn.size(); i++)
			{
				std::vector<TStatisticResultPart> vecStatisticPart;

				for(int j= 0; j< 2; j++)
				{
					for(int m= 0; m< m_nGroupFieldTypeCount; m++)
					{
						// - get group type ID
						Integer nxGroupID = 0;
						ifs.read( reinterpret_cast<char*>( &nxGroupID),        sizeof(Integer)  );

						TStatisticResultPart oTStatisticResultPart;
						oTStatisticResultPart.bGroupInfo = true;
						oTStatisticResultPart.nxGroupID  = nxGroupID;
						std::vector<double> &vecData = oTStatisticResultPart.vecData;

						for(int k= 0; k< STATISTIC_PART_DATAFIELD_CNT; k++)
						{
							double dValue = 0.0;
							ifs.read( reinterpret_cast<char*>( &dValue),        sizeof(double)  );

							vecData.push_back(dValue);
						}

						vecStatisticPart.push_back(oTStatisticResultPart);
						nCount++;
					}
				}

				m_mapStatisticPart.insert(std::make_pair((int)i, vecStatisticPart));
			}

// 			if(!ifs.eof())
// 			{
// 				CString strMsg = _T("");
// 				strMsg.Format(_T("--- error statistics result size "));
// 				TxLogDebug((LPCTSTR)strMsg );
// 
// 				ifs.close();
// 				return false;
// 			}

			CString strResult = _T("");
			strResult.Format(_T("--- end insert statistics result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			ifs.close();
			TxLogDebugException();
			return false;
		}

		ifs.close();
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


bool KMultiStatisticsBaseDefineDlg::BulkSelect(KDBaseConPtr spDBaseConnection)
{
	int     nSelectedColCnt = m_pSelectedColumns->ColumnCount();
	CString strColumns = _T("");

	if( NULL != m_pGroupColumn )
	{
		strColumns += m_pGroupColumn->Name();
		strColumns += _T(", ");
	}

	for(int i= 0; i< nSelectedColCnt; i++)
	{
		KIOColumn* pSelectedColumn = m_pSelectedColumns->GetColumn(i);
		strColumns += pSelectedColumn->Name();

		if(i< nSelectedColCnt-1)
		{
			strColumns += (_T(", "));
		}
	}

	TxLogDebugStartMsg(); 

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}

	strSQL.Format(_T(" Select %s From %s %s"), strColumns, m_pTable->Name(), strWhere);

	CString strFileName;
	strFileName.Format(_T("%s\\%s"), ImChampDir::GetAppDirectory(), T_DAT_IN_STATISTICS);
	std::ofstream ofs(strFileName, std::ios::binary);
	
	if (!ofs)
	{
		TxLogDebugException();
		return false;
	}

	KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);
    while(spResultSet->Next())
    {
        int nIndex = 0;
        if( NULL != m_pGroupColumn )
        {
            nIndex = 1;
            Integer nxGroupData = spResultSet->GetValueInt64(0);
            ofs.write(reinterpret_cast<char*>( &nxGroupData ), sizeof(Integer) ); 
        }

        std::map<Integer, double> mapRecord;
        for(int i= 0; i< nSelectedColCnt; i++)
        {
            KIOColumn* pSelectedColumn = m_pSelectedColumns->GetColumn(i);
            int        nDisplayOrder   = pSelectedColumn->DisplayOrder();

            double dValue = spResultSet->GetValueDouble(i + nIndex);
            mapRecord.insert(std::make_pair(nDisplayOrder, dValue));
        }

        for(size_t i= 0; i< m_vecMultiColumn.size(); i++)
        {
            TMultiColumn oTMultiColumn  = m_vecMultiColumn[i];
            KIOColumn*   pBaseColumn    = oTMultiColumn.pBaseColumn;
            KIOColumn*   pCompareColumn = oTMultiColumn.pCompareColumn;

            int nBaseOrder    = pBaseColumn->DisplayOrder();
            int nCompareOrder = pCompareColumn->DisplayOrder();

            std::map<Integer, double>::iterator iterBase    = mapRecord.find(nBaseOrder);
            std::map<Integer, double>::iterator iterCompare = mapRecord.find(nCompareOrder);

            double dBaseValue    = 0.0;
            double dCompareValue = 0.0;
            if( iterBase != mapRecord.end() )
            {
                dBaseValue = iterBase->second;
            }
            if( iterCompare != mapRecord.end())
            {
                dCompareValue = iterCompare->second;
            }

            ofs.write(reinterpret_cast<char*>( &dBaseValue ),    sizeof(double) ); 
            ofs.write(reinterpret_cast<char*>( &dCompareValue ), sizeof(double) ); 
        }
    }
	
	ofs.close();

	TxLogDebugEndMsg();
	return true;
}


bool KMultiStatisticsBaseDefineDlg::GetExpressionGroupData( std::map<Integer, std::vector<double>> &a_mapExpressionData )
{
	KIOColumns oReqColumns;
	KIOColumn* pColumn = NULL;

	const std::list<KExpressionElement*>& elementList = m_pExpression->GetExpressionList();
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = elementList.end();
	for(itElement = elementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		if(pElement->ElementType() == KEMExpressionElementField)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			pColumn = oReqColumns.AddNewColumn();
			pColumn->Name(pField->FieldName());
			pColumn->Caption(pField->FieldCaption());
		}
	}

	int nReqColumnCount = oReqColumns.ColumnCount();
	if( nReqColumnCount < 1 )
	{
		return false;
	}

	KParseVariables       variables;
	KParseVariableInfo*   pVarInfo = NULL;
	CString               strColumnName =_T("");

	for(int j = 0; j < nReqColumnCount; ++j)
	{
		pVarInfo = variables.AddNewVariable();
		pColumn = oReqColumns.GetColumn(j);
		pVarInfo->SetVariable(pColumn->Name(), j);

		strColumnName += ( pColumn->Name() );

		if(j < nReqColumnCount -1)
		{
			strColumnName += ( _T(", ") );
		}
	}		

	KSDoubleRecord*     pRecord = NULL;
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}

	strSQL.Format(_T(" SELECT %s, %s FROM %s %s "), m_pGroupColumn->Name(), strColumnName, m_pTable->Name(), strWhere);

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	std::map<Integer, KSDoubleRecordArray*> mapDoubleRecordArray;
	while( pResult->Next() )
	{
		Integer nxGroup = pResult->GetValueInt64(0);

		std::map<Integer, KSDoubleRecordArray*>::iterator iterKey = mapDoubleRecordArray.find(nxGroup);
		if(iterKey != mapDoubleRecordArray.end())
		{
			KSDoubleRecordArray*    pRecordArray = iterKey->second;
			pRecord = pRecordArray->AddNewRecord();
		}
		else
		{
			KSDoubleRecordArray*   pRecordArray = new KSDoubleRecordArray(nReqColumnCount);

			pRecord = pRecordArray->AddNewRecord();
			mapDoubleRecordArray.insert( std::make_pair(nxGroup, pRecordArray));

			std::vector<double> vecData;
			a_mapExpressionData.insert(  std::make_pair(nxGroup, vecData));
		}

		// ZERO 값을 포함 하지 않을 경우 처리
		if(FALSE == m_bChkZero)
		{
			int nZeroValCount = 0;
			for(int j = 0; j < nReqColumnCount; ++j)
			{
				if(pResult->GetValueDouble(1+j) == 0.0)
				{
					nZeroValCount++;
				}
			}

			if(nReqColumnCount == nZeroValCount)
			{
				continue;
			}
		}

		for(int j = 0; j < nReqColumnCount; ++j)
		{
			pRecord->SetAt( j, pResult->GetValueDouble(1+j) );
		}
	}

	CString     strFunction = m_pExpression->GetExpression();
	KGeneration gen;

	std::map<Integer, KSDoubleRecordArray*>::iterator iterBegin = mapDoubleRecordArray.begin();
	std::map<Integer, KSDoubleRecordArray*>::iterator iterEnd   = mapDoubleRecordArray.end();
	std::map<Integer, std::vector<double>>::iterator iterExData  = a_mapExpressionData.begin();

	bool bResult = true;
	while( iterBegin != iterEnd )
	{
		KSDoubleRecordArray *pRecordArray = iterBegin->second;
		std::vector<double> &vecData      = iterExData->second;

		int nRet = gen.MuParseExecute(strFunction, variables, *pRecordArray, vecData, NULL);
		if(1 != nRet)
		{
			bResult = false;
		}

		delete pRecordArray;

		++iterExData;
		++iterBegin;
	}

	return bResult;
}


bool KMultiStatisticsBaseDefineDlg::GetExpressionNormalData( std::vector<double> &a_vecData )
{
	KIOColumns oReqColumns;
	KIOColumn* pColumn = NULL;

	const std::list<KExpressionElement*>& elementList = m_pExpression->GetExpressionList();
	std::list<KExpressionElement*>::const_iterator itElement, itEnd = elementList.end();
	for(itElement = elementList.begin(); itElement != itEnd; ++itElement)
	{
		KExpressionElement* pElement = *itElement;
		if(pElement->ElementType() == KEMExpressionElementField)
		{
			KExpressionElementField* pField = (KExpressionElementField*)pElement;
			pColumn = oReqColumns.AddNewColumn();
			pColumn->Name(pField->FieldName());
			pColumn->Caption(pField->FieldCaption());
		}
	}

	int nReqColumnCount = oReqColumns.ColumnCount();
	if( nReqColumnCount < 1 )
	{
		return false;
	}

	KParseVariables       variables;
	KParseVariableInfo*   pVarInfo = NULL;
	CString               strColumnName =_T("");

	for(int j = 0; j < nReqColumnCount; ++j)
	{
		pVarInfo = variables.AddNewVariable();
		pColumn = oReqColumns.GetColumn(j);
		pVarInfo->SetVariable(pColumn->Name(), j);

		strColumnName += ( pColumn->Name() );

		if(j < nReqColumnCount -1)
		{
			strColumnName += ( _T(", ") );
		}
	}		

	KSDoubleRecordArray oRecordArray(nReqColumnCount);
	KSDoubleRecord*     pRecord = NULL;
	KDBaseConPtr spDBaseConnection = m_pTable->GetDBaseConnection();

	CString strSQL(_T(""));
	CString strWhere(_T(" Where 1 = 1 "));

	if (m_strFilter.GetLength() > 0)
	{
		strWhere += " AND ";
		strWhere += m_strFilter;
	}

	strSQL.Format(_T(" SELECT %s FROM %s %s "), strColumnName, m_pTable->Name(), strWhere);

	KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(pResult == NULL)
	{
		return false;
	}

	while( pResult->Next() )
	{
		pRecord = oRecordArray.AddNewRecord();
		for(int j = 0; j < nReqColumnCount; ++j)
		{
			pRecord->SetAt( j, pResult->GetValueDouble(j) );
		}
	}

	CString     strFunction = m_pExpression->GetExpression();
	KGeneration gen;

	int nRet = gen.MuParseExecute(strFunction, variables, oRecordArray, a_vecData, NULL);
	if(1 != nRet)
	{
		return false;
	}

	return true;
}


void KMultiStatisticsBaseDefineDlg::OnBnClickedBtnFunction()
{
	KStatisticsFunctionDefineDlg dlg;
	dlg.SetTable(m_pTable);
	dlg.SetExpression(m_pExpression);

	if(IDOK == dlg.DoModal())
	{
		m_strFunction =  m_pExpression->GetCaption();
		UpdateData(FALSE);
	}
}


void KMultiStatisticsBaseDefineDlg::OnSelectCancel()
{
	CheckboxControl( FALSE );
}


void KMultiStatisticsBaseDefineDlg::OnSelectAll()
{
	CheckboxControl( TRUE );
}


void KMultiStatisticsBaseDefineDlg::CheckboxControl(BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		pItemCheck->SetChecked(a_bCheck);
	}

	m_wndReportCtrl.RedrawControl();
}


void KMultiStatisticsBaseDefineDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KMultiStatisticsBaseDefineDlg::OnBnClickedCheckGroupFiled()
{
	UpdateData(TRUE);
	DisableGroupFiled(m_bChkGroup);
}


void KMultiStatisticsBaseDefineDlg::DisableGroupFiled(BOOL a_bDisable)
{
	m_cboGroup.EnableWindow(a_bDisable);
}


KIOColumn* KMultiStatisticsBaseDefineDlg::GetCurGroupField()
{
	UpdateData(TRUE);
	KIOColumn* pColumn = NULL;

	if( TRUE == m_bChkGroup )
	{
		int nCur = m_cboGroup.GetCurSel();

		if(nCur < 0)
		{
			return NULL;

		}

		pColumn = (KIOColumn* )m_cboGroup.GetItemData(nCur);
	}
	else
	{
		pColumn = NULL;
	}

	return pColumn;
}


void KMultiStatisticsBaseDefineDlg::OnBnClickedButtonAdd()
{
	AddReportData();
	m_wndReportCtrl.Populate();
}

BOOL KMultiStatisticsBaseDefineDlg::OnCopyData( CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct )
{
	try
	{
		int     nMsgCode = pCopyDataStruct->dwData;
		CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

		m_nResultCode = nMsgCode;
		AddStatusMessage(strMsg);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}   

	return KResizeDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

