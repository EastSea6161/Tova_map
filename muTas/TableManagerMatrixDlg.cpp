// TableManagerMatrixDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableManagerMatrixDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

#include "DBaseTableMgr.h"

#include "FormulaInputDlg.h"
#include "FormulaExpression.h"
#include "../kmzModel/KParser.h"
#include "KBulkDBase.h"

//#include "StdioFileEx.h"

enum
{
	_0_A_COLUMN_SELECT     = 0
};

// KTableManagerMatrixDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableManagerMatrixDlg, KResizeDialogEx)

KTableManagerMatrixDlg::KTableManagerMatrixDlg(KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableManagerMatrixDlg::IDD, pParent)
	, m_pProject(a_pProject)
	, m_pVirtualRecord(nullptr)
	, m_pRecordArray(NULL)
{
	m_pUserColumns = new KIOColumns();
}

KTableManagerMatrixDlg::~KTableManagerMatrixDlg()
{
	if (m_pUserColumns != NULL)
	{
		delete m_pUserColumns;
		m_pUserColumns = NULL;
	}

	DeallocateMap();

	if (m_pRecordArray != NULL)
	{
		delete m_pRecordArray;
		m_pRecordArray = NULL;
	}

	TxLogDebugVisitor();
}

void KTableManagerMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A, m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B, m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KTableManagerMatrixDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTableManagerMatrixDlg::OnCbnSelchangeComboDataType)
	ON_BN_CLICKED(IDCANCEL, &KTableManagerMatrixDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &KTableManagerMatrixDlg::OnBnClickedButtonRun)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,  &KTableManagerMatrixDlg::OnReportAItemClick)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KTableManagerMatrixDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KTableManagerMatrixDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &KTableManagerMatrixDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KTableManagerMatrixDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_VECTOR, &KTableManagerMatrixDlg::OnBnClickedButtonVector)
END_MESSAGE_MAP()


// KTableManagerMatrixDlg 메시지 처리기입니다.

BOOL KTableManagerMatrixDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);

	InitUserColumn();

	KReportCtrlSetting::DefaultC(m_wndReportACtrl);
	//KReportCtrlSetting::HideHeader(m_wndReportACtrl);
	KReportCtrlSetting::HorizontalNoLine(m_wndReportACtrl);
	KReportCtrlSetting::VerticalNoLine(m_wndReportACtrl);

	KReportCtrlSetting::Default (m_wndReportBCtrl);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_wndReportBCtrl.GetPaintManager()->m_strNoItems = _T("왼쪽 트리에서 컬럼을 선택한 후 선택완료 버튼을 클릭해주세요.");
	}
	else {
		m_wndReportBCtrl.GetPaintManager()->m_strNoItems = _T("Please Select Column.");
	}
    //m_wndReportBCtrl.GetPaintManager()->m_strNoItems = _T("왼쪽 트리에서 컬럼을 선택후 Run 버튼을 클릭해주세요");
	InitReportAHeader();
	UpdateReportAData();

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTableManagerMatrixDlg::InitUserColumn( void )
{
	KIOColumn* pUserColumn = nullptr;

	pUserColumn = m_pUserColumns->AddNewColumn();
	pUserColumn->Caption(_T("U1"));
	pUserColumn->Name(_T("U1"));
	pUserColumn->DataType(KEMIODataTypeDouble);

	pUserColumn = m_pUserColumns->AddNewColumn();
	pUserColumn->Caption(_T("U2"));
	pUserColumn->Name(_T("U2"));
	pUserColumn->DataType(KEMIODataTypeDouble);

	pUserColumn = m_pUserColumns->AddNewColumn();
	pUserColumn->Caption(_T("U3"));
	pUserColumn->Name(_T("U3"));
	pUserColumn->DataType(KEMIODataTypeDouble);

	pUserColumn = m_pUserColumns->AddNewColumn();
	pUserColumn->Caption(_T("U4"));
	pUserColumn->Name(_T("U4"));
	pUserColumn->DataType(KEMIODataTypeDouble);

	pUserColumn = m_pUserColumns->AddNewColumn();
	pUserColumn->Caption(_T("U5"));
	pUserColumn->Name(_T("U5"));
	pUserColumn->DataType(KEMIODataTypeDouble);
}

void KTableManagerMatrixDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,      SZ_TOP_LEFT,     SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR1,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_VECTOR, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RUN,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CALC,   SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXPORT, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR5,     SZ_TOP_LEFT,     SZ_BOTTOM_LEFT);

	SetResize(IDC_REPORT_B,      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR2,     SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_CLEAR,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

void KTableManagerMatrixDlg::InitReportAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT, _T("::: Select Column"),     100));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼 선택"));
    }

	pColumn->SetHeaderAlignment(DT_LEFT);
	pColumn->SetTreeColumn(TRUE);

	m_wndReportACtrl.Populate();
}

void KTableManagerMatrixDlg::UpdateReportAData()
{
	try
	{
		BOOL    bAllChecked  = TRUE;
		if(IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bAllChecked = TRUE;
		else
			bAllChecked = FALSE;

		std::vector<KScenario*> vecScenario;
		KScenario* pBaseScenario            = m_pProject->GetBaseScenario();
		vecScenario.push_back(pBaseScenario);

		const KTDScenarioMap& oScenarioMap  = m_pProject->GetScenarioMap();
		KTDScenarioMap::const_iterator iter = oScenarioMap.begin();
		KTDScenarioMap::const_iterator end  = oScenarioMap.end();

		while( iter != end )
		{
			KScenario* pScenario = iter->second;
			vecScenario.push_back(pScenario);

			++iter;
		}

		m_wndReportACtrl.ResetContent();

		CXTPReportRecord*     pParRecord    = nullptr;
		CXTPReportRecords*    pSeParRecords = nullptr;
		CXTPReportRecord*     pSeParRecord  = nullptr;
		CXTPReportRecords*    pThParRecords = nullptr;
		CXTPReportRecord*     pThParRecord  = nullptr;
		CXTPReportRecords*    pFrParRecords = nullptr;
		CXTPReportRecord*     pFrParRecord  = nullptr;
		CXTPReportRecordItem* pItem         = nullptr;

		for (size_t i= 0; i< vecScenario.size(); i++)
		{
			KScenario* pScenario       = vecScenario[i];
			CString   &strScenarioName = pScenario->GetName();

			const KTDTargetMap &mapTarget    = pScenario->GetTargetMap();
			if (mapTarget.size() < 1)
			{
				continue;
			}

			//#1
			pParRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());
			pItem      = pParRecord->AddItem(new CXTPReportRecordItemText(strScenarioName));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) pScenario);
			pSeParRecords = pParRecord->GetChilds();

			KTDTargetMap::const_iterator citTargetMap = mapTarget.begin();
			while( mapTarget.end() != citTargetMap )
			{
				KTarget* pTarget = citTargetMap->second;
				if( nullptr == pTarget )
				{
					++citTargetMap;
					continue;
				}

				//#2
				pSeParRecord  = pSeParRecords->Add(new CXTPReportRecord());
				pItem         = pSeParRecord->AddItem(new CXTPReportRecordItemText(pTarget->CaptionTargetYear()));
				pItem->SetAlignment(DT_LEFT);
				pItem->SetItemData((DWORD_PTR) pTarget);
				pThParRecords = pSeParRecord->GetChilds();

 				KIOTables* pIOTables    =  pTarget->Tables();
				if (nullptr == pIOTables)
				{
					++citTargetMap;
					continue;
				}

				size_t     nxTableCount = pIOTables->GetCount();
				for (size_t i= 0; i<nxTableCount; i++)
				{
					KIOTable* pIOTable = pIOTables->GetTableAt(i);
					if (pIOTable->TableType() == KEMIOTableTypeODData)
					{
						//#3~#4
						AddTreeDepth3(pThParRecords, pIOTable, bAllChecked);
					}
				}

				++citTargetMap;
			}
		}

		m_wndReportACtrl.Populate();
		m_wndReportACtrl.ExpandAll(TRUE);
	}
	catch (...)
	{
		return;
	}
}

void KTableManagerMatrixDlg::AddTreeDepth3(CXTPReportRecords* a_pThParRecords, KIOTable* a_pIOTable, BOOL a_bCheck)
{
	try
	{
		const KIOColumns* pColumns = a_pIOTable->Columns();
		int nColCount = pColumns->ColumnCount();

		int nUsableColumnCnt(0);
		for (int k= 0; k< nColCount; k++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(k);

			if(pColumn->DataType() != KEMIODataTypeDouble)
				continue;

			nUsableColumnCnt++;
		}

		if (nUsableColumnCnt < 1)
		{
			return;
		}

		CXTPReportRecords*    pFrParRecords = nullptr;
		CXTPReportRecord*     pFrParRecord  = nullptr;

		//#3
		CXTPReportRecord*     pThParRecord  = a_pThParRecords->Add(new CXTPReportRecord());
		CXTPReportRecordItem* pItem         = pThParRecord->AddItem(new CXTPReportRecordItemText(a_pIOTable->Caption()));
		pItem->SetAlignment(DT_LEFT);
		pItem->SetItemData((DWORD_PTR) a_pIOTable);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(a_bCheck);
		pFrParRecords = pThParRecord->GetChilds();

		for (int k= 0; k< nColCount; k++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(k);

			if(pColumn->DataType() != KEMIODataTypeDouble)
				continue;

			//#4
			pFrParRecord  = pFrParRecords->Add(new CXTPReportRecord());
			pItem         = pFrParRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption()));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) pColumn);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(a_bCheck);
		}
	}
	catch(...)
	{
		return;
	}
}

void KTableManagerMatrixDlg::OnBnClickedButtonRun()
{	
	try
	{
		GetSelectTreeData();

		int nCntSelected(0);
		int nCntTargetTable = m_vecTargetTable.size();

		for (int i = 0; i < nCntTargetTable; ++i)
		{
			TTargetTable        oTargetTable  = m_vecTargetTable[i];
			vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

			int nCntColumnInfo = vecColumnInfo.size();

			for (int j = 0; j < nCntColumnInfo; ++j)
			{
				++nCntSelected;
			}
		}

		if (nCntSelected == 0)
		{
			AfxMessageBox(_T("선택한 컬럼이 없습니다."));
			return;
		}

		Execute();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
}

void KTableManagerMatrixDlg::Execute( void )
{
	InitReportBHeader();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(SrchOperDataThreadProc,this, false, nLang);

	if (m_bOperDataThreadOK)
	{
		UpdateReportBData();

		GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
	}
}

void KTableManagerMatrixDlg::InitReportBHeader( void )
{
	m_wndReportBCtrl.GetColumns()->Clear();
	m_wndReportBCtrl.ResetContent();

	int nIdxColumn(0);

	CXTPReportColumn* pColumn = nullptr;

	//Add OZone, DZone Header
	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("Origin"), 1));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("기점"));
	}
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTooltip(_T("Origin"));
	pColumn->SetAlignment(DT_CENTER);

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("Dest"), 1));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("종점"));
	}
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTooltip(_T("Dest"));
	pColumn->SetAlignment(DT_CENTER);

	//Add Selected Column Header
	int nCntTargetTable = m_vecTargetTable.size();

	for (int i = 0; i < nCntTargetTable; ++i)
	{
		TTargetTable        oTargetTable  = m_vecTargetTable[i];
		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

		int nCntColumnInfo = vecColumnInfo.size();

		for (int j = 0; j < nCntColumnInfo; ++j)
		{
			TColumnInfo oColumnInfo = vecColumnInfo[j];

			CString strDisplayName = oColumnInfo.strDisplayName;
			CString strCaptionName = oColumnInfo.strCaptionName;

			pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, strDisplayName, 1));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetTooltip(strCaptionName);
			pColumn->SetAlignment(DT_CENTER);
		}
	}

	//Add User Define Column Header
	int nCntUserColumns = m_pUserColumns->ColumnCount();

	for (int i = 0; i < nCntUserColumns; ++i)
	{
		KIOColumn* pUserColumn = m_pUserColumns->GetColumn(i);

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, pUserColumn->Name(), 1));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetTooltip(pUserColumn->Caption());
		pColumn->SetAlignment(DT_CENTER);
	}

	m_wndReportBCtrl.Populate();
}

void KTableManagerMatrixDlg::UpdateReportBData( void )
{	
	int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

	m_pVirtualRecord = new KMatrixOperVRecord(m_pRecordArray, nColumns);
	m_wndReportBCtrl.SetVirtualMode(m_pVirtualRecord, m_pRecordArray->GetRecordCount());
	m_wndReportBCtrl.Populate();
}


void KTableManagerMatrixDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KTableManagerMatrixDlg::OnCbnSelchangeComboDataType()
{
	UpdateData(TRUE);

	UpdateReportAData();
}

void KTableManagerMatrixDlg::OnBnClickedCheckAll()
{
	UpdateData(TRUE);

	UpdateReportAData();
}


void KTableManagerMatrixDlg::OnBnClickedButtonClear()
{

	int nCheckedItems = m_vecTargetTable.size();
	int nSelectedColumns(0);

	for (int i = 0; i < nCheckedItems; ++i)
	{
		TTargetTable        oTargetTable  = m_vecTargetTable[i];
		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

		int nCntColumn = vecColumnInfo.size();

		for (int j = 0; j < nCntColumn; ++j)
		{
			++nSelectedColumns;
		}
	}

	int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();
	int nRecords = m_pRecordArray->GetRecordCount();

	for (int i = 0; i < nRecords; ++i)
	{
		KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(i);

		for (int j = nSelectedColumns; j < nColumns; ++j)
		{
			pRecord->SetAt(j, 0.0);
		}
	}


	m_wndReportBCtrl.Populate();
}


void KTableManagerMatrixDlg::OnBnClickedButtonCalc()
{
	//Set FomualField
	std::vector<TFormulaField> vecFomulaField;

	CXTPReportColumns* pColumns = m_wndReportBCtrl.GetColumns();
	int nCntColumns = pColumns->GetCount();

	for (int i = 0; i < nCntColumns; ++i)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(i);

		CString strCaption = pColumn->GetCaption();

		if (strCaption.CompareNoCase(_T("Origin")) == 0)
			continue;

		if (strCaption.CompareNoCase(_T("Dest")) == 0)
			continue;

		TFormulaField oFormulaField;
		oFormulaField.TFieldName    = pColumn->GetCaption();
		oFormulaField.TFieldCaption = pColumn->GetCaption();

		vecFomulaField.push_back(oFormulaField);
	}

	KFormulaExpression oExpression;
	KFormulaInputDlg   oFormulaInputDlg;

	oFormulaInputDlg.UsableFunction(oExpression.Functions());
	oFormulaInputDlg.UsableOperator(oExpression.Operators());
	oFormulaInputDlg.UsableField(vecFomulaField);

	if (oFormulaInputDlg.DoModal() != IDOK)
		return;

	CString strFormulaCaption = oFormulaInputDlg.GetFormulaCaption();

	oExpression.SetCaptionExpression(strFormulaCaption, vecFomulaField);

	vector<CString> vecField = oExpression.GetUsedRealFieldName();

	//vecField와 매칭하는 컬럼의 Index를 가지는 Vector;
	vector<int>     vecIdx;

	int nCntVecField = vecField.size();

	for (int i = 0; i <nCntVecField; ++i)
	{
		CString strColumnName = vecField[i];

		for (int j = 0; j < nCntColumns; ++j)
		{
			CXTPReportColumn* pColumn    = pColumns->GetAt(j);
			CString           strCaption = pColumn->GetCaption();

			if (strColumnName.CompareNoCase(strCaption) == 0)
			{
				vecIdx.push_back(j);
			}
		}
	}

	if ( vecField.size() != vecIdx.size() )
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		return;
	}

	CString strRealFormula = oExpression.GetRealExpression();
	int     nIdxUserColumn = oFormulaInputDlg.IndexUserColumn();

	//앞에 있는 선택 컬럼갯수 더하기
	int nCntChekedItem = m_vecTargetTable.size();

	for (int i = 0; i < nCntChekedItem; ++i)
	{
		TTargetTable        oTargetTable  = m_vecTargetTable[i];
		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

		int nCntColumn = vecColumnInfo.size();

		for (int j = 0; j < nCntColumn; ++j)
		{
			++nIdxUserColumn;
		}
	}

	nIdxUserColumn = nIdxUserColumn + 2;//Oring, Dest Column Add;
	//수식에 Caption 넣기
	CXTPReportColumn* pColumn = pColumns->GetAt(nIdxUserColumn);
	pColumn->SetTooltip(strFormulaCaption);

	double* dValueArray = new double[nCntVecField];

	try
	{
		KParser oParser;
		oParser.SetExpr(strRealFormula);

		for (int i = 0; i < nCntVecField; ++i)
		{
			oParser.DefineVar(vecField[i],  &dValueArray[i]);
		}

		int nRecords = m_pRecordArray->GetRecordCount();

		for (int i = 0; i < nRecords; ++i)
		{
			KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(i);

			for (int i = 0; i < nCntVecField; ++i)
			{
				dValueArray[i] = pRecord->GetAt(vecIdx[i]);
			}

			double dResultValue = oParser.Eval();

			pRecord->SetAt(nIdxUserColumn, dResultValue);
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

	m_wndReportBCtrl.Populate();
	delete[] dValueArray;
}

void KTableManagerMatrixDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_A_COLUMN_SELECT == nIndex)
	{
		try
		{
			BOOL bIsSelectAll = TRUE;

			CXTPReportRecord*     pRecord = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem* pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if(pItem->GetHasCheckbox() == FALSE)
				return;

			if(pRecord->HasChildren() == TRUE)
			{
				BOOL bParentChecked = pItem->IsChecked();
				if(FALSE == bParentChecked)
				{
					bIsSelectAll = FALSE;
				}

				CXTPReportRecords* pChdRecords = pRecord->GetChilds();
				int nCount = pChdRecords->GetCount();
				for (int i= 0; i< nCount; i++)
				{
					CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
					CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_A_COLUMN_SELECT);
					pChdItem->SetChecked(bParentChecked);
				}
				m_wndReportACtrl.RedrawControl();
			}

			CXTPReportRecord*   pParentRecord = pRecord->GetParentRecord();
			BOOL bCheked = pItem->IsChecked();
			if(FALSE == bCheked)
			{
				bIsSelectAll = FALSE;
			}

			CXTPReportRecords*  pChdRecords   = pParentRecord->GetChilds();
			int nCount        = pChdRecords->GetCount();
			int nCheckedCount = 0;
			for (int i= 0; i< nCount; i++)
			{
				CXTPReportRecord*      pChdRecord = pChdRecords->GetAt(i);
				CXTPReportRecordItem*  pChdItem   = pChdRecord->GetItem(_0_A_COLUMN_SELECT);
				if( pChdItem->IsChecked() == TRUE)
				{
					nCheckedCount++;
				}
			}

			CXTPReportRecordItem*  pParentItem = pParentRecord->GetItem(_0_A_COLUMN_SELECT);
			pParentItem->SetChecked(bCheked);

			if (nCheckedCount > 0)
			{
				pParentItem->SetChecked(TRUE);
			}

			if ( FALSE == bIsSelectAll )
			{
				CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
			}
		}
		catch (...)
		{
			return;
		}
	}
}


void KTableManagerMatrixDlg::OnBnClickedButtonExport()
{
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

	fileDlg.m_ofn.lpstrTitle = _T("Export Data");
	//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

	m_strExpFileName = _T("");

	if (fileDlg.DoModal() == IDOK)
	{
		CString strFileName = fileDlg.GetPathName();
		strFileName.Replace(_T(".txt"), _T(""));
		m_strExpFileName = strFileName + _T(".txt");
	}
	else
	{
		return;
	}

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	int nResult = QBicSimpleProgressThread::ExecuteThread(WriteTextThreadProc, this, true, nLang);
}

unsigned __stdcall KTableManagerMatrixDlg::WriteTextThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;
	KTableManagerMatrixDlg* pDlg = (KTableManagerMatrixDlg*)pParameter->GetParameter();

	CString strFileName = pDlg->m_strExpFileName;
	if (strFileName.GetLength() == 0)
		return 0;

	CString strHeaderLine = pDlg->GetExportHeaderLine();

	pDlg->WriteTextFile(strFileName, pDlg->m_pRecordArray, strHeaderLine);

	return 1;
}

void KTableManagerMatrixDlg::WriteTextFile( 
	CString a_strFileName, KSDoubleRecordArray* a_pRecordArray, CString a_strHeaderLine )
{
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(a_strFileName, nOpenMode);

	CString strOut, strData;

	try
	{
		CString strHeaderLine = a_strHeaderLine;
		strHeaderLine.Append(_T("\r\n"));

		of.WriteString(strHeaderLine);

		int nRecords = a_pRecordArray->GetRecordCount();

		for (int i = 0; i < nRecords; ++i)
		{
			KSDoubleRecord* pRecord  = a_pRecordArray->GetRecordAt(i);
			int             nColumns = pRecord->GetCount();

			if (nColumns == 0)
				ThrowException(_T("데이터가 없습니다."));

			int nIndex(0);

			//Origin
			strData.Format(_T("%I64d"), (Integer)pRecord->GetAt(nIndex++));
			strOut = strData;

			//Dest
			strData.Format(_T(", %I64d"), (Integer)pRecord->GetAt(nIndex++));
			strOut.Append(strData);

			for (int j = nIndex; j < nColumns; ++j)
			{
				strData.Format(_T(", %f"), pRecord->GetAt(j));
				strOut.Append(strData);
			}

			strOut.Append(_T("\r\n"));

			of.WriteString(strOut);

			if (QBicSimpleProgressThread::IsUserStopped())
				ThrowException(_T("취소되었습니다."));

		}

		of.Close();

		AfxMessageBox(_T("완료되었습니다."));
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox( ex->GetErrorMessage() );
		of.Close();

		remove((CStringA)a_strFileName);
	}
	catch (...)
	{
		TxLogDebugException();
		of.Close();
	}
}

CString KTableManagerMatrixDlg::GetExportHeaderLine( void )
{
	CString strHeaderLine = _T("");

	CXTPReportColumns* pColumns = m_wndReportBCtrl.GetColumns();

	int nCntColumns = pColumns->GetCount();

	CString strHeader;

	for (int i = 0; i < nCntColumns; ++i)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(i);

		strHeader.Format(_T("\"%s\", "), pColumn->GetCaption());

		strHeaderLine.Append(strHeader);
	}

	strHeaderLine.Truncate(strHeaderLine.GetLength() - 2);

	return strHeaderLine;
}

void KTableManagerMatrixDlg::GetSelectTreeData( void )
{
	m_vecTargetTable.clear();

	try
	{
		KScenario* pScenario = nullptr;
		KTarget*   pTarget   = nullptr;
		KIOTable*  pIOTable  = nullptr;
		KIOColumn* pIOColumn = nullptr;

		int nFIndex(1);

		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_wndReportACtrl.GetRecords();
		int nCount = pRecords->GetCount();
		for (int i= 0; i< nCount; i++)
		{
			CXTPReportRecord*  pParentRecord    = pRecords->GetAt(i);
			pItem            = pParentRecord->GetItem(_0_A_COLUMN_SELECT);
			pScenario        = (KScenario*)pItem->GetItemData();

			CXTPReportRecords* pSeParentRecords = pParentRecord->GetChilds();
			for (int k= 0; k<  pSeParentRecords->GetCount(); k++)
			{
				CXTPReportRecord*  pSeParentRecord = pSeParentRecords->GetAt(k);
				pItem            = pSeParentRecord->GetItem(_0_A_COLUMN_SELECT);
				pTarget          = (KTarget*)pItem->GetItemData();

				CXTPReportRecords* pThParentRecords = pSeParentRecord->GetChilds();
				for (int m= 0; m<  pThParentRecords->GetCount(); m++)
				{
					CXTPReportRecord*  pThParentRecord = pThParentRecords->GetAt(m);
					pItem            = pThParentRecord->GetItem(_0_A_COLUMN_SELECT);

					if( pItem->GetHasCheckbox() == FALSE )
						continue;

					if( pItem->IsChecked() == FALSE )
						continue;

					pIOTable          = (KIOTable*)pItem->GetItemData();

					TTargetTable oTTargetTable;
					oTTargetTable.pScenario = pScenario;
					oTTargetTable.pTarget   = pTarget;
					oTTargetTable.pIOTable  = pIOTable;
					std::vector<TColumnInfo> &vecColumnInfo = oTTargetTable.vecColumnInfo;

					CXTPReportRecords* pFrParentRecords = pThParentRecord->GetChilds();
					for (int n= 0; n<  pFrParentRecords->GetCount(); n++)
					{
						CXTPReportRecord*  pFrParentRecord = pFrParentRecords->GetAt(n);
						pItem            = pFrParentRecord->GetItem(_0_A_COLUMN_SELECT);

						if( pItem->IsChecked() == FALSE )
							continue;

						pIOColumn          = (KIOColumn*)pItem->GetItemData();

						TColumnInfo oTColumnInfo;
						oTColumnInfo.pIOColumn = pIOColumn;
						CString &strDisplayName = oTColumnInfo.strDisplayName;
						CString &strCaptionName = oTColumnInfo.strCaptionName;
						strDisplayName.Format(_T("F%d"), nFIndex);
						strCaptionName.Format(_T("%s-%s-%s-%s"), pScenario->GetName(), pTarget->CaptionTargetYear(), pIOTable->Name(),  pIOColumn->Caption() );

						vecColumnInfo.push_back(oTColumnInfo);

						nFIndex++;
					}

					m_vecTargetTable.push_back(oTTargetTable);
				}
			}
		}
	}
	catch(...)
	{
		m_vecTargetTable.clear();
		ThrowException(_T("Unexpected Error in Tree control ..."));
		return;
	}
}


void KTableManagerMatrixDlg::OnBnClickedButtonVector()
{
	try
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("현재 선택된 행열 데이터를 벡터 데이터로 변환하시겠습니까?\n변환한 데이터는 존 테이블에 저장됩니다."));
		if ( AfxMessageBox(strMsg, MB_YESNO) == IDNO )
		{
			return;
		}

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(MatrixtoVectorThreadProc,this, false, nLang);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KTableManagerMatrixDlg::RunMatrixtoVector()
{
	try
	{
		GetSelectTreeData();

		size_t nCount = m_vecTargetTable.size();

		if (0 == nCount)
		{
			ThrowException(_T("선택한 컬럼이 없습니다."));
		}

		for (size_t i = 0; i < nCount ; i ++)
		{
			TTargetTable oTargetTable = m_vecTargetTable[i];
			ExcuteOD2Vector(oTargetTable);

			KIOTable* pZoneTable = oTargetTable.pTarget->Tables()->FindTable(TABLE_ZONE);
			pZoneTable->Notify();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KTableManagerMatrixDlg::ExcuteOD2Vector( TTargetTable oTargetTable )
{
	try
	{
		KTarget* pTarget	= oTargetTable.pTarget;
		KIOTable* pTable	= oTargetTable.pIOTable;
		std::vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;
		size_t nCount = vecColumnInfo.size();
		for (size_t i = 0; i < nCount; i ++)
		{
			KIDKeyDoubleRecords records(2);
			TColumnInfo oColumnInfo = vecColumnInfo[i];
			KDBaseTableMgr::GetMatrix2VectorValue(oColumnInfo, pTable, records);

			AddStatusMessage(oColumnInfo.strCaptionName);
			KIOColumns rZoneColumnCollection;
			AddZonePAColumn(rZoneColumnCollection, pTarget, oColumnInfo.pIOColumn->Caption());

			KBulkDBase::BulkZoneUpdate(pTarget->Tables()->FindTable(TABLE_ZONE), rZoneColumnCollection, records);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
}


void KTableManagerMatrixDlg::AddZonePAColumn( KIOColumns& rZoneColumnCollection, KTarget* a_pTarget, CString a_strColumnName )
{
	try
	{
		rZoneColumnCollection.Clear();

		KIOTables* pTableManger = a_pTarget->Tables();
		if (nullptr == pTableManger)
		{
			ThrowException(_T("컬럼추가 실패.."));
		}

		KIOTable* pTable			  = pTableManger->FindTable(TABLE_ZONE);
		KIOColumn* pPColumn			  = nullptr;
		KIOColumn* pAColumn			  = nullptr;

		const KIOColumns* pColumnCollection = pTable->Columns();

		int nMaxDisplayCount(0);
		int nCount = pColumnCollection->ColumnCount();

		if (0 == nCount)
		{
			nMaxDisplayCount = 1;
		}
		else
		{
			for (int i = 0; i < nCount; i++)
			{
				KIOColumn* pColumn = pColumnCollection->GetColumn(i);
				int nDisplayCount = pColumn->DisplayOrder();
				if (nMaxDisplayCount < nDisplayCount)
				{
					nMaxDisplayCount = nDisplayCount;
				}
			}
		}

		//Column ==> ozone
		KIOColumn oPColumn;
		CString strPColumnCaption(_T(""));
		strPColumnCaption.Format(_T("Col_Sum(%s)"), a_strColumnName);
		oPColumn.Name(pTable->GetUsableColumnName());
		oPColumn.Caption(strPColumnCaption);
		oPColumn.CodeType(KEMIOCodeTypeIsNothing);
		oPColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oPColumn.DataType(KEMIODataTypeDouble);
		oPColumn.DisplayOrder(nMaxDisplayCount+1);
		oPColumn.ColumnInfo(strPColumnCaption);
		oPColumn.PrimaryKey(false);
		oPColumn.Visible(true);

		pPColumn = pTable->AddColumn(oPColumn);
		if (nullptr == pPColumn)
		{
			ThrowException(_T("컬럼추가 실패.."));
		}

		//Row ==> dzone
		KIOColumn oAColumn;
		CString strAColumnCaption(_T(""));
		strAColumnCaption.Format(_T("Row_Sum(%s)"), a_strColumnName);
		oAColumn.Name(pTable->GetUsableColumnName());
		oAColumn.Caption(strAColumnCaption);
		oAColumn.CodeType(KEMIOCodeTypeIsNothing);
		oAColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oAColumn.DataType(KEMIODataTypeDouble);
		oAColumn.DisplayOrder(nMaxDisplayCount+2);
		oAColumn.ColumnInfo(strPColumnCaption);
		oAColumn.PrimaryKey(false);
		oAColumn.Visible(true);

		pAColumn = pTable->AddColumn(oAColumn);
		if (nullptr == pAColumn)
		{
			ThrowException(_T("컬럼추가 실패.."));
		}

		rZoneColumnCollection.AddNewColumn(pPColumn);
		rZoneColumnCollection.AddNewColumn(pAColumn);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

unsigned __stdcall KTableManagerMatrixDlg::SrchOperDataThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)pParam;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Data Loading..."));
	}

	KTableManagerMatrixDlg* pDlg = (KTableManagerMatrixDlg*)pParameter->GetParameter();
	pDlg->SrchOperData();

	return 0;
}

void KTableManagerMatrixDlg::SrchOperData( void )
{
	m_bOperDataThreadOK = false;

	try
	{
		int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

		DeallocateMap();
		
		KDBaseTableMgr::MatrixResultView((nColumns - 2), m_vecTargetTable, m_mapResultRecord); //orgin, dest 제외한 KSDoubleRecord의 Size

		if ((int)m_mapResultRecord.size() == 0)
			ThrowException(_T("데이터가 없습니다."));

		if (m_pRecordArray != NULL)
		{
			delete m_pRecordArray;
			m_pRecordArray = NULL;
		}

		m_pRecordArray = new KSDoubleRecordArray(nColumns);

		map<KODKey, KSDoubleRecord*>::iterator beginIter = m_mapResultRecord.begin();
		map<KODKey, KSDoubleRecord*>::iterator endIter   = m_mapResultRecord.end();

		while (beginIter != endIter)
		{
			KODKey          oODKey     = beginIter->first;
			KSDoubleRecord* pMapRecord = beginIter->second;

			int nIndex(0);

			KSDoubleRecord* pRecord = m_pRecordArray->AddNewRecord();

			pRecord->SetAt(nIndex++, (double)oODKey.OriginID);
			pRecord->SetAt(nIndex++, (double)oODKey.DestinationID);

			int nCounts = pMapRecord->GetCount();

			for (int i = 0; i < nCounts; ++i)
			{
				pRecord->SetAt(nIndex++, pMapRecord->GetAt(i));
			}

			++beginIter;
		}

		DeallocateMap();

		m_bOperDataThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		m_bOperDataThreadOK = false;
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

unsigned __stdcall KTableManagerMatrixDlg::MatrixtoVectorThreadProc( void* pParam )
{
	try
	{
		QBicSimpleProgressParameter* pParamerter = (QBicSimpleProgressParameter*)pParam;
		KTableManagerMatrixDlg* pDlg = (KTableManagerMatrixDlg*)pParamerter->GetParameter();
		pDlg->RunMatrixtoVector();
		
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return -1;
	}
	catch(...)
	{
		TxLogDebugException();
		return -1;
	}
	return 0;
	
}

void KTableManagerMatrixDlg::AddStatusMessage( CString strMsg )
{
	try
	{
		QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
		if(spProgressWindow)
		{
			spProgressWindow->SetStatus(strMsg);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
	
}

void KTableManagerMatrixDlg::DeallocateMap( void )
{
	if ((int)m_mapResultRecord.size() != 0)
	{
		map<KODKey, KSDoubleRecord*>::iterator beginIter, endIter = m_mapResultRecord.end();

		for (beginIter = m_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
		{
			KSDoubleRecord* pRecord = beginIter->second;
			delete pRecord;
		}

		m_mapResultRecord.clear();
	}
}

