// TableManagerVectorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableManagerVectorDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Scenario.h"
#include "Target.h"


#include "DBaseTableMgr.h"

#include "FormulaInputDlg.h"
#include "FormulaExpression.h"
#include "../kmzModel/KParser.h"

//#include "StdioFileEx.h"

//^#include "DevideDataInterval.h"
#include "InformationDlg.h"
#include "CopyUserFieldVectorDlg.h"
#include "KBulkDBase.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "BulkDBaseDemarcation.h"

enum
{
	_0_A_COLUMN_SELECT     = 0
};

enum
{
	INDEX_TABLE_NODE = 1,
	INDEX_TABLE_LINK = 2,
	INDEX_TABLE_ZONE = 3
};

// KTableManagerVectorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableManagerVectorDlg, KResizeDialogEx)

KTableManagerVectorDlg::KTableManagerVectorDlg(KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableManagerVectorDlg::IDD, pParent)
	, m_pProject(a_pProject)
	, m_pVirtualRecord(NULL)
	, m_pFieldRecord(nullptr)
	, m_pSelectedTarget(nullptr)
	, m_pSelectedIOTable(nullptr)
	, m_strMsgExportResult(_T(""))
	, m_strRunTableName(_T(""))
{
	m_pUserColumns = new KIOColumns();
}

KTableManagerVectorDlg::~KTableManagerVectorDlg()
{
	if (m_pUserColumns != NULL)
	{
		delete m_pUserColumns;
		m_pUserColumns = NULL;
	}

	DeallocateMap();

	m_wndReportBCtrl.SetVirtualMode(NULL, 0);
	if (nullptr != m_pFieldRecord)
	{
		QBicDelete(m_pFieldRecord);
	}

	if (nullptr != m_pSelectedIOTable)
	{
		m_pSelectedIOTable->Unregister(this);
	}

	TxLogDebugVisitor();
}

void KTableManagerVectorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,              m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B,              m_wndReportBCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_cboDataType);
}


BEGIN_MESSAGE_MAP(KTableManagerVectorDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1,     &KTableManagerVectorDlg::OnCbnSelchangeComboDataType)
	ON_BN_CLICKED(IDCANCEL,          &KTableManagerVectorDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RUN,    &KTableManagerVectorDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_CHECK_ALL,     &KTableManagerVectorDlg::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,  &KTableManagerVectorDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CALC,   &KTableManagerVectorDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &KTableManagerVectorDlg::OnBnClickedBtnExport)

	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,  &KTableManagerVectorDlg::OnReportAItemClick)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT_B, &KTableManagerVectorDlg::OnReportBColumnRButtonClick)

	ON_COMMAND(ID_MENU_STATISTICS, &KTableManagerVectorDlg::OnMenuStatisticsIncludeZero)
	
	ON_BN_CLICKED(IDC_BUTTON_COPY, &KTableManagerVectorDlg::OnBnClickedButtonCopy)
END_MESSAGE_MAP()


// KTableManagerVectorDlg 메시지 처리기입니다.

void KTableManagerVectorDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		int nCurSel =  m_cboDataType.GetCurSel();
		if(nCurSel < 0)
			throw 1;

		int nTableIndex = m_cboDataType.GetItemData(nCurSel);
		if(INDEX_TABLE_NODE ==  nTableIndex)
		{
			//if (strSubjectName.CompareNoCase(TABLE_NODE) == 0)
			if (strSubjectName.CompareNoCase(_T("노드")) == 0)
			{
				UpdateReportAData();
			}
		}
		else if(INDEX_TABLE_LINK == nTableIndex)
		{
			//if (strSubjectName.CompareNoCase(TABLE_LINK) == 0)
			if (strSubjectName.CompareNoCase(_T("링크")) == 0)
			{
				UpdateReportAData();
			}
		}
		else if(INDEX_TABLE_ZONE == nTableIndex)
		{
			//if (strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
			if (strSubjectName.CompareNoCase(_T("존")) == 0)
			{
				UpdateReportAData();
			}
		}
		else
		{
			throw 1;
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
}


BOOL KTableManagerVectorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_COPY)->EnableWindow(FALSE);

	InitComboDataType();
	InitUserColumn();

	KReportCtrlSetting::DefaultC(m_wndReportACtrl);
	//KReportCtrlSetting::HideHeader(m_wndReportACtrl);
	KReportCtrlSetting::HorizontalNoLine(m_wndReportACtrl);
	KReportCtrlSetting::VerticalNoLine(m_wndReportACtrl);
	
	KReportCtrlSetting::Default(m_wndReportBCtrl);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_wndReportBCtrl.GetPaintManager()->m_strNoItems = _T("왼쪽 트리에서 컬럼을 선택한 후 선택완료 버튼을 클릭해주세요.");
	}
	else {
		m_wndReportBCtrl.GetPaintManager()->m_strNoItems = _T("Please Select Column.");
	}
	InitReportAHeader();
	UpdateReportAData();

	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTableManagerVectorDlg::InitComboDataType()
{
	m_cboDataType.ResetContent();

	int nIndex = 0;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboDataType.AddString(_T("노드"));
	}
	else {
		nIndex = m_cboDataType.AddString(TABLE_NODE);
	}
	//int nIndex = m_cboDataType.AddString(TABLE_NODE);
	m_cboDataType.SetItemData(nIndex, INDEX_TABLE_NODE);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboDataType.AddString(_T("링크"));
	}
	else {
		nIndex = m_cboDataType.AddString(TABLE_LINK);
	}
	//nIndex = m_cboDataType.AddString(TABLE_LINK);
	m_cboDataType.SetItemData(nIndex, INDEX_TABLE_LINK);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		nIndex = m_cboDataType.AddString(_T("존"));
	}
	else {
		nIndex = m_cboDataType.AddString(TABLE_ZONE);
	}
	//nIndex = m_cboDataType.AddString(TABLE_ZONE);
	m_cboDataType.SetItemData(nIndex, INDEX_TABLE_ZONE);

	m_cboDataType.SetCurSel(0);
}

void KTableManagerVectorDlg::InitUserColumn()
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

void KTableManagerVectorDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,				      SZ_TOP_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_RUN,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CALC,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_EXPORT,			  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_COPY,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	
	SetResize(IDC_SEPRATOR5,				  SZ_TOP_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDC_REPORT_B,			          SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR2,		     	  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_CLEAR,				  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,			              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


void KTableManagerVectorDlg::InitReportAHeader()
{
	CXTPReportColumn*                pColumn      = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT,  _T("::: Select Column"),     100));
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("컬럼 선택"));
    }

	pColumn->SetHeaderAlignment(DT_LEFT);
	pColumn->SetTreeColumn(TRUE);

	m_wndReportACtrl.Populate();
}

void KTableManagerVectorDlg::UpdateReportAData()
{
	try
	{
		int nCurSel =  m_cboDataType.GetCurSel();
		if(nCurSel < 0)
			throw 1;

		CString strTableName(_T(""));
		int nTableIndex = m_cboDataType.GetItemData(nCurSel);
		if(INDEX_TABLE_NODE ==  nTableIndex)
		{
			strTableName = TABLE_NODE;
		}
		else if(INDEX_TABLE_LINK == nTableIndex)
		{
			strTableName = TABLE_LINK;
		}
		else if(INDEX_TABLE_ZONE == nTableIndex)
		{
			strTableName = TABLE_ZONE;
		}
		else
		{
			throw 1;
		}
		
		BOOL    bAllChecked  = TRUE;
		if(IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bAllChecked = TRUE;
		else
			bAllChecked = FALSE;

		vector<KScenario*> vecScenario;
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
			KScenario*           pScenario     = vecScenario[i];
			const KTDTargetMap   &mapTarget    = pScenario->GetTargetMap();
			if (mapTarget.size() < 1)
			{
				continue;
			}

			//#1
			pParRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

			CString   &strScenarioName = pScenario->GetName();
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

				KIOTables* pIOTables =  pTarget->Tables();
				if (nullptr == pIOTables)
				{
					++citTargetMap;
					continue;
				}

				KIOTable*  pIOTable  =  pIOTables->FindTable(strTableName);

				//#3 ~ #4				
				AddTreeDepth3(pThParRecords, pIOTable, bAllChecked);

				if( strTableName.CompareNoCase(TABLE_ZONE) == 0 )
				{
					pIOTable = pIOTables->FindTable(TABLE_SED_VECTOR); //TABLE_SED_VECTOR, TABLE_SED_POP
					AddTreeDepth3(pThParRecords, pIOTable, bAllChecked);
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

void KTableManagerVectorDlg::AddTreeDepth3(CXTPReportRecords* a_pThParRecords, KIOTable* a_pIOTable, BOOL a_bCheck)
{
	try
	{
		const KIOColumns* pColumns = a_pIOTable->Columns();
		int nColCount   = pColumns->ColumnCount();

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

void KTableManagerVectorDlg::OnBnClickedButtonRun()
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
					vector<TColumnInfo> &vecColumnInfo = oTTargetTable.vecColumnInfo;

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
		AfxMessageBox(_T("Unexpected Error in Tree control ..."));
		return;
	}

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

	Run();
}

void KTableManagerVectorDlg::Run( void )
{
	InitReportBHeader();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(SrchOperDataThreadProc, this, true, nLang);

	if (m_bOperDataThreadOK)
	{
		UpdateReportBData();

		GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_COPY)->EnableWindow(TRUE);
	}
}

void KTableManagerVectorDlg::InitReportBHeader()
{
	m_wndReportBCtrl.SetVirtualMode(NULL, 0);

	m_wndReportBCtrl.GetColumns()->Clear();
	m_wndReportBCtrl.ResetContent();

	CXTPReportColumn* pColumn = nullptr;

	int nChekedItems = m_vecTargetTable.size();
	int nIdxColumn(0);

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, _T("ID"), 1));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetTooltip(_T("ID"));
	pColumn->SetAlignment(DT_CENTER);

	for (int i = 0; i < nChekedItems; ++i)
	{
		TTargetTable        oTargetTable  = m_vecTargetTable[i];
		vector<TColumnInfo> vecColumnInfo = oTargetTable.vecColumnInfo;

		int nCntColumn = vecColumnInfo.size();

		for (int j = 0; j < nCntColumn; ++j)
		{
			TColumnInfo oColumnInfo = vecColumnInfo[j];

			pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, oColumnInfo.strDisplayName, 1));
			pColumn->SetHeaderAlignment(DT_CENTER);
			pColumn->SetTooltip(oColumnInfo.strCaptionName);
			pColumn->SetAlignment(DT_CENTER);
		}
	}
	//add usercolumn;
	int nCntUserColumn = m_pUserColumns->ColumnCount();

	for (int i = 0; i < nCntUserColumn; ++i)
	{
		KIOColumn* pUserColumn = m_pUserColumns->GetColumn(i);

		pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(nIdxColumn++, pUserColumn->Name(), 1));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetTooltip(pUserColumn->Caption());
		pColumn->SetAlignment(DT_CENTER);
	}

	m_wndReportBCtrl.Populate();
}

void KTableManagerVectorDlg::UpdateReportBData()
{
	int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();
	
#pragma region KSDoubleRecordArray 사용 버전
// 	m_pVirtualRecord = new KVectorOperVRecord(m_pRecordArray, nColumns);
// 	m_wndReportBCtrl.SetVirtualMode(m_pVirtualRecord, m_pRecordArray->GetRecordCount());
#pragma endregion KSDoubleRecordArray 사용 버전

	m_pVirtualRecord = new KVectorOperVRecord(m_pFieldRecord, m_vecFieldRecordKey, nColumns);
	m_wndReportBCtrl.SetVirtualMode(m_pVirtualRecord, m_pFieldRecord->GetRecordCount());
	
	m_wndReportBCtrl.Populate();
}

void KTableManagerVectorDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KTableManagerVectorDlg::OnCbnSelchangeComboDataType()
{
	UpdateData(TRUE);
	ResetResultData();
	UpdateReportAData();
}

void KTableManagerVectorDlg::ResetResultData()
{
	try
	{
		m_vecTargetTable.clear();
		
		m_vecFieldRecordKey.clear();

		m_wndReportBCtrl.SetVirtualMode(NULL, 0);
		m_wndReportBCtrl.GetColumns()->Clear();
		m_wndReportBCtrl.ResetContent();
		m_wndReportBCtrl.Populate();
		if (nullptr != m_pFieldRecord)
		{
			QBicDelete(m_pFieldRecord);
		}

		GetDlgItem(IDC_BUTTON_CALC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_COPY)->EnableWindow(FALSE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KTableManagerVectorDlg::OnBnClickedCheckAll()
{
	UpdateData(TRUE);

	UpdateReportAData();
}

void KTableManagerVectorDlg::OnBnClickedButtonClear()
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

	nSelectedColumns = nSelectedColumns + 1;//ID Column Add;
	int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

#pragma region KSDoubleRecordArray 사용 버전
// 	int nRecords = m_pRecordArray->GetRecordCount();
// 	for (int i = 0; i < nRecords; ++i)
// 	{
// 		KSDoubleRecord* pRecord = m_pRecordArray->GetRecordAt(i);
// 
// 		for (int j = nSelectedColumns; j < nColumns; ++j)
// 		{
// 			pRecord->SetAt(j, 0.0);
// 		}
// 	}
#pragma endregion KSDoubleRecordArray 사용 버전

	std::map<Integer, KSDoubleRecord*>::const_iterator iter = m_pFieldRecord->begin();
	std::map<Integer, KSDoubleRecord*>::const_iterator end  = m_pFieldRecord->end();

	while(iter != end)
	{
		KSDoubleRecord* pRecord = iter->second;

		for (int j= nSelectedColumns; j< nColumns; j++)
		{
			pRecord->SetAt(j, 0.0);
		}

		++iter;
	}  

	for (int i= nSelectedColumns; i< nColumns; i++)
	{
		CXTPReportColumn* pColumnUser = m_wndReportBCtrl.GetColumns()->GetAt(i);
		CString strCaption = pColumnUser->GetCaption();
		
		pColumnUser->SetTooltip(strCaption);
	}

	m_wndReportBCtrl.RedrawControl();
	//m_wndReportBCtrl.Populate();
}

void KTableManagerVectorDlg::OnBnClickedButtonCalc()
{
	//Set FomualField
	std::vector<TFormulaField> vecFomulaField;

	CXTPReportColumns* pColumns = m_wndReportBCtrl.GetColumns();
	int nCntColumns = pColumns->GetCount();

	for (int i = 0; i < nCntColumns; ++i)
	{
		CXTPReportColumn* pColumn = pColumns->GetAt(i);

		CString strCaption = pColumn->GetCaption();

		if (strCaption.CompareNoCase(_T("ID")) == 0)
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
	
	nIdxUserColumn = nIdxUserColumn + 1;//ID Column Add;
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

		std::map<Integer, KSDoubleRecord*>::const_iterator iter = m_pFieldRecord->begin();
		std::map<Integer, KSDoubleRecord*>::const_iterator end  = m_pFieldRecord->end();

		while(iter != end)
		{
			KSDoubleRecord* pRecord = iter->second;

			for (int j = 0; j < nCntVecField; ++j)
			{
				dValueArray[j] = pRecord->GetAt(vecIdx[j]);
			}

			double dResultValue = oParser.Eval();

			pRecord->SetAt(nIdxUserColumn, dResultValue);

			++iter;
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

void KTableManagerVectorDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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

void KTableManagerVectorDlg::OnBnClickedBtnExport()
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

	m_strMsgExportResult = _T("");

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(WriteTextThreadProc, this, true, nLang);

	if (m_strMsgExportResult.IsEmpty() == false)
	{
		AfxMessageBox(m_strMsgExportResult);
	}
}


void KTableManagerVectorDlg::WriteTextFile( 
	CString a_strFileName, KIDKeyDoubleRecords* a_pFieldRecord, 
	CString a_strHeaderLine, CString &a_strMsgExportResult)
{
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(a_strFileName, nOpenMode);

	CString strOut, strData;

	try
	{
		CString strHeaderLine = a_strHeaderLine;
		strHeaderLine.Append(_T("\r\n"));

		of.WriteString(strHeaderLine);

		std::map<Integer, KSDoubleRecord*>::const_iterator iter = a_pFieldRecord->begin();
		std::map<Integer, KSDoubleRecord*>::const_iterator end  = a_pFieldRecord->end();

		while(iter != end)
		{
			KSDoubleRecord* pRecord  = iter->second;
			int             nColumns = pRecord->GetCount();

			if (0 == nColumns)
				ThrowException(_T("데이터가 없습니다."));

			//Key
			strData.Format(_T("%I64d"), (Integer)pRecord->GetAt(0));
			strOut = strData;

			for (int j = 1; j < nColumns; ++j)
			{
				strData.Format(_T(", %f"), pRecord->GetAt(j));
				strOut.Append(strData);
			}

			strOut.Append(_T("\r\n"));

			of.WriteString(strOut);

			if (QBicSimpleProgressThread::IsUserStopped())
				ThrowException(_T("취소되었습니다."));

			++iter;
		}  

		of.Close();

		//AfxMessageBox(_T("완료되었습니다."));
		a_strMsgExportResult = _T("완료되었습니다.");
	}
	catch (KExceptionPtr ex)
	{
		//AfxMessageBox( ex->GetErrorMessage() );
		a_strMsgExportResult = ex->GetErrorMessage();
		of.Close();

		remove((CStringA)a_strFileName);
	}
	catch (...)
	{
		TxLogDebugException();
		of.Close();

		remove((CStringA)a_strFileName);
	}
}


CString KTableManagerVectorDlg::GetExportHeaderLine( void )
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

void KTableManagerVectorDlg::OnReportBColumnRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;

		CXTPReportColumn* pClickedColumn = pClickedItem->pColumn;

		if (pClickedColumn == nullptr)
			return;

		int nColumnIndex = pClickedColumn->GetItemIndex();

		if (nColumnIndex == 0)
			return;

		m_nSelectedMethod      = 1;
		m_nSelectedColumnIndex = nColumnIndex;

		CMenu menu;
		menu.LoadMenu( IDR_POPUP_ODMATRIX );    
		CMenu* pPopup = menu.GetSubMenu(1);

		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx( pPopup, TPM_RIGHTBUTTON, pClickedItem->pt.x, pClickedItem->pt.y, this );
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	*pResult = 0;
}

void KTableManagerVectorDlg::OnMenuStatisticsIncludeZero()
{
	try
	{
		CWaitCursor wc;
		std::vector<double> vecValue;
		if (m_nSelectedMethod == 1)
		{
			int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

			if (nColumns < m_nSelectedColumnIndex)
				ThrowException(_T("Overflow Column Index..."));

			std::map<Integer, KSDoubleRecord*>::const_iterator iter = m_pFieldRecord->begin();
			std::map<Integer, KSDoubleRecord*>::const_iterator end  = m_pFieldRecord->end();

			while(iter != end)
			{
				KSDoubleRecord* pRecord  = iter->second;

				double dValue = pRecord->GetAt(m_nSelectedColumnIndex);
				vecValue.push_back(dValue);

				++iter;
			}  
		}
		else
		{
			ThrowException(_T("Unmatching SelectedMethod..."));
		}

		KDevideDataInterval oData;
		oData.SetDataCalculate(vecValue);

		CString strMsg(_T(""));
		strMsg.AppendFormat(_T("Count : %d \r\n"), oData.GetDataCount());
		strMsg.AppendFormat(_T("Sum : %.3f \r\n"), oData.GetSum());
		strMsg.AppendFormat(_T("Average : %.3f \r\n"), oData.GetAverage());
		strMsg.AppendFormat(_T("Min : %.3f \r\n"), oData.GetMinValue());
		strMsg.AppendFormat(_T("Max : %.3f \r\n"), oData.GetMaxValue());
		strMsg.AppendFormat(_T("Variance : %.3f \r\n"), oData.GetVariance());
		strMsg.AppendFormat(_T("Std. Deviation : %.3f \r\n"), oData.GetStdDev());
		strMsg.AppendFormat(_T("Std. Error : %.3f \r\n"), oData.GetStdErr());

		KInformationDlg dlg;
		dlg.SetTextString(strMsg);
		dlg.DoModal();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	m_nSelectedMethod = -1;
}

void KTableManagerVectorDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

unsigned __stdcall KTableManagerVectorDlg::WriteTextThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;
	KTableManagerVectorDlg* pDlg = (KTableManagerVectorDlg*)pParameter->GetParameter();

	CString strFileName = pDlg->m_strExpFileName;
	if (strFileName.GetLength() == 0)
		return 0;

	CString strHeaderLine = pDlg->GetExportHeaderLine();

	pDlg->WriteTextFile(strFileName, pDlg->m_pFieldRecord, strHeaderLine, pDlg->m_strMsgExportResult);

	return 1;
}

unsigned __stdcall KTableManagerVectorDlg::SrchOperDataThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 로딩 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Data Loading..."));
	}

	KTableManagerVectorDlg* pDlg = (KTableManagerVectorDlg*)pParameter->GetParameter();;

	pDlg->SrchOperData();

	return 0;
}

void KTableManagerVectorDlg::SrchOperData( void )
{
	m_bOperDataThreadOK = false;

	try
	{
		int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

		DeallocateMap();		
				
		KDBaseTableMgr::NetworkResultView((nColumns - 1), m_vecTargetTable, m_mapResultRecord);//map의 key 값에 데이터가 담기므로 1을 뺌

		if ((int)m_mapResultRecord.size() == 0)
			ThrowException(_T("데이터가 없습니다."));

		if (nullptr != m_pFieldRecord)
		{
			QBicDelete(m_pFieldRecord);
		}

		m_pFieldRecord = new KIDKeyDoubleRecords(nColumns);

		map<__int64, KSDoubleRecord*>::iterator beginIter, endIter = m_mapResultRecord.end();

		for (beginIter = m_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
		{
			Integer         nxID       = beginIter->first;
			KSDoubleRecord* pMapRecord = beginIter->second;
			int nCount    = pMapRecord->GetCount();

			int nAddIndex(0);
			KSDoubleRecord* pAddRecord = m_pFieldRecord->AddRecord(nxID);

			pAddRecord->SetAt(nAddIndex++, (double)beginIter->first);
			for (int i= 0; i< nCount; i++)
			{
				pAddRecord->SetAt(nAddIndex++, pMapRecord->GetAt(i));
			}
		}

		m_vecFieldRecordKey.clear();
		std::map<Integer, KSDoubleRecord*>::const_iterator iter = m_pFieldRecord->begin();
		std::map<Integer, KSDoubleRecord*>::const_iterator end  = m_pFieldRecord->end();

		while(iter != end)
		{
			Integer nxID = iter->first;
			m_vecFieldRecordKey.push_back(nxID);

			++iter;
		}  

		DeallocateMap();
		
		m_bOperDataThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		m_bOperDataThreadOK = false;
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		m_bOperDataThreadOK = false;
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}

void KTableManagerVectorDlg::DeallocateMap( void )
{
	if ((int)m_mapResultRecord.size() != 0)
	{
		map<__int64, KSDoubleRecord*>::iterator beginIter, endIter = m_mapResultRecord.end();

		for (beginIter = m_mapResultRecord.begin(); beginIter != endIter; ++beginIter)
		{
			KSDoubleRecord* pRecord = beginIter->second;
			delete pRecord;
		}

		m_mapResultRecord.clear();
	}
}


void KTableManagerVectorDlg::OnBnClickedButtonCopy()
{
	try
	{
		int nCurSel =  m_cboDataType.GetCurSel();
		if(nCurSel < 0)
			return;

		CString strTableName(_T(""));
#pragma region Get Table Name
		int nTableIndex = m_cboDataType.GetItemData(nCurSel);
		if(INDEX_TABLE_NODE ==  nTableIndex)
		{
			strTableName = TABLE_NODE;
		}
		else if(INDEX_TABLE_LINK == nTableIndex)
		{
			strTableName = TABLE_LINK;
		}
		else if(INDEX_TABLE_ZONE == nTableIndex)
		{
			strTableName = TABLE_ZONE;
		}
		else
		{
			return;
		}
#pragma endregion Get Table Name

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		m_vecUserColumnInfo.clear();
		GetCurUserColumnInfo(m_vecUserColumnInfo);

		KCopyUserFieldVectorDlg oDlg(m_pProject, strTableName, m_vecUserColumnInfo);
		if (oDlg.DoModal() == IDOK) // vecUserColumnInfo에 추가 가능한 Column 정보들만 다시 입력 된다.
		{
			m_pSelectedTarget      = oDlg.GetSelectedTarget();
			KIOTables*  pIOTables  = m_pSelectedTarget->Tables();
			m_pSelectedIOTable     = pIOTables->FindTable(strTableName);

			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("데이터 저장 중..."));
			}
			else {
				AddStatusMessage(_T("Saving Data..."));
			}

			m_pSelectedIOTable->BeginTransaction();
			try
			{
				ThreadPara oPara(this);
				if (true) {
					oPara.TBusiness = 0; // CopyUserColumn

					RThreadInfo.Init();

					QBicSimpleProgressThread::ExecuteThread(CopyUserColumnThreadProc, &oPara, true, nLang);

					if (RThreadInfo.IsOK() == false) {
						ThrowException(RThreadInfo.ErrorMsg());
					}

					m_pSelectedIOTable->Commit();
				}
			}
			catch (KExceptionPtr ex) {
				TxExceptionPrint(ex);
				m_pSelectedIOTable->Rollback();
				return;
			}
			catch (...) {
				TxLogDebugException();
				m_pSelectedIOTable->Rollback();
				return;
			}
			
			bool bResultCopyColumn(false);
			try
			{
				ThreadPara oPara(this);
				if (true) {
					oPara.TBusiness = 1; // CopyUserColumnData

					RThreadInfo.Init();
					QBicSimpleProgressThread::ExecuteThread(CopyUserColumnThreadProc, &oPara, true, nLang);

					if (RThreadInfo.IsOK() == false) {
						ThrowException(RThreadInfo.ErrorMsg());
					}
					else {
						bResultCopyColumn = true;
					}
				}
			}
			catch (KExceptionPtr ex) {
				TxExceptionPrint(ex);
				bResultCopyColumn = false;
			}
			catch (...) {
				TxLogDebugException();
				bResultCopyColumn = false;
			}

			if (bResultCopyColumn) {
				bool bAddField(false);
				for (size_t i= 0; i< m_vecUserColumnInfo.size(); i++)
				{
					TUserColumnInfo &oTUserColumnInfo = m_vecUserColumnInfo[i];
					if (TRUE == oTUserColumnInfo.bNew)
					{
						bAddField = true;
						break;
					}
				}

				if (bAddField) // 테이블에 새로운 필드가 추가 되었다면 (Tree를 다시 그린다.)
				{
					m_pSelectedIOTable->Register(this);
					m_pSelectedIOTable->Notify();
					m_pSelectedIOTable->Unregister(this);
				}
				else
				{
					m_pSelectedIOTable->Notify();
				}
			}
			else {
				m_pSelectedIOTable->BeginTransaction();
				try
				{
					ThreadPara oPara(this);
					if (true) {
						oPara.TBusiness = 2; /// DeleteCopiedUserColumn

						RThreadInfo.Init();
						QBicSimpleProgressThread::ExecuteThread(CopyUserColumnThreadProc, &oPara, true, nLang);

						if (RThreadInfo.IsOK() == false) {
							ThrowException(RThreadInfo.ErrorMsg());
						}
					}
					m_pSelectedIOTable->Commit();
				}
				catch (KExceptionPtr ex) {
					TxExceptionPrint(ex);
					m_pSelectedIOTable->Rollback();
				}
				catch (...) {
					TxLogDebugException();
					m_pSelectedIOTable->Rollback();
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


unsigned __stdcall KTableManagerVectorDlg::CopyUserColumnThreadProc( void* pParam )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;
	if (nullptr != pParameter) {
		ThreadPara* pPara = (ThreadPara*)pParameter->GetParameter(); {
			KTableManagerVectorDlg* pDlg = (KTableManagerVectorDlg*)pPara->TWindow;

			if (0 == pPara->TBusiness) {
				pDlg->RunCopyPart1(pPara);
			}
			else if (1 == pPara->TBusiness) {
				pDlg->RunCopyPart2(pPara);
			}
			else if (2 == pPara->TBusiness) {
				pDlg->RunCopyPart3(pPara);
			}
		}
	}

	return 1;
}


void KTableManagerVectorDlg::RunCopyPart1(ThreadPara* pPara)
{
	try
	{
		if (CopyUserColumn(m_pSelectedIOTable, m_vecUserColumnInfo) == false)
		{
			throw 1;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("복사 오류"));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error"));
		}
	}
}


void KTableManagerVectorDlg::RunCopyPart2(ThreadPara* pPara)
{
	try
	{
		if (CopyUserColumnData(m_pSelectedTarget, m_pSelectedIOTable, m_vecUserColumnInfo) == false)
		{
			throw 1; // DeleteCopiedUserColumn 를 수행해야 한다.
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("복사 오류"));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error"));
		}
	}
}

void KTableManagerVectorDlg::RunCopyPart3(ThreadPara* pPara)
{
	try
	{
		if (false == DeleteCopiedUserColumn(m_pSelectedIOTable, m_vecUserColumnInfo)) {
			throw 1;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		RThreadInfo.SetErrorFlag(-1, ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		if (KmzSystem::GetLanguage() == KEMKorea) {
			RThreadInfo.SetErrorFlag(-1, _T("복사 오류"));
		}
		else {
			RThreadInfo.SetErrorFlag(-1, _T("Error"));
		}
	}
}

void KTableManagerVectorDlg::GetCurUserColumnInfo(std::vector<TUserColumnInfo> &a_vecUserColumnInfo)
{
	try
	{
		a_vecUserColumnInfo.clear();

		int nCheckedItems = m_vecTargetTable.size();
		int nSelectedColumns(0);

		for (int i = 0; i < nCheckedItems; ++i)
		{
			TTargetTable        &oTargetTable  = m_vecTargetTable[i];
			vector<TColumnInfo> &vecColumnInfo = oTargetTable.vecColumnInfo;

			int nCntColumn = vecColumnInfo.size();

			for (int j = 0; j < nCntColumn; ++j)
			{
				++nSelectedColumns;
			}
		}

		nSelectedColumns = nSelectedColumns + 1;//ID Column Add;
		int nColumns     = m_wndReportBCtrl.GetColumns()->GetCount();

		for (int i= nSelectedColumns; i< nColumns; i++)
		{
			CXTPReportColumn*           pColumnUser = m_wndReportBCtrl.GetColumns()->GetAt(i);
			CString strUserFieldName  = pColumnUser->GetCaption();
			CString strFunction       = pColumnUser->GetTooltip();

			bool bFunction(false);
			if (strUserFieldName.CompareNoCase(strFunction) != 0) // Tooltip과 Caption이 다르다는 것은 식이 할당되었다는 의미
				bFunction = true;

			TUserColumnInfo oTUserColumnInfo;
			oTUserColumnInfo.strUserFieldName = strUserFieldName;
			oTUserColumnInfo.bFunction        = bFunction;

			a_vecUserColumnInfo.push_back(oTUserColumnInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_vecUserColumnInfo.clear();
	}
}


bool KTableManagerVectorDlg::CopyUserColumn(KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecUserColumnInfo)
{
	try
	{
		CWaitCursor wc;
		// table 이름에서 제일 큰 숫자를 가져온다.
		int nColumnNumber = 0;
		int nDisplayOrder = 0;
		const KIOColumns* pColumns = a_pIOTable->Columns();
		int nCount      = pColumns->ColumnCount();

		for(int i = 0; i < nCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);

			CString strName = pColumn->Name();
			CString strNumber = _T("");
			for(int j = 0; j < strName.GetLength(); ++j)
			{
				TCHAR c = strName.GetAt(j);
				if((_T('0') <= c) && (_T('9') >= c))
				{
					strNumber.AppendChar(c);
				}
				else
				{
					strNumber = _T("");
				}
			}

			int nNumber = _ttoi(strNumber);
			if(nColumnNumber <  nNumber)
			{
				nColumnNumber = nNumber;
			}

			if(nDisplayOrder < pColumn->DisplayOrder())
			{
				nDisplayOrder = pColumn->DisplayOrder();
			}
		}

		KProject* pProject             = m_pProject;
		CString   strTableName         = a_pIOTable->Name();
		bool      bSetDefaultValueZero = true;	//add Field시 Default 값을 '0'으로 초기화, false 시 null

		try
		{
			for (size_t i= 0; i< a_vecUserColumnInfo.size(); i++)
			{
				TUserColumnInfo &oTUserColumnInfo = a_vecUserColumnInfo[i];
				if (FALSE == oTUserColumnInfo.bNew)
				{
					continue;
				}

				nColumnNumber++;
				nDisplayOrder++;

				KIOColumn oIOColumnAdd;

				CString strName;
				strName.Format(_T("%c%d"), strTableName.GetAt(0), nColumnNumber);
				oIOColumnAdd.Name(strName);
				oIOColumnAdd.Caption(oTUserColumnInfo.strCopyFieldName);
				oIOColumnAdd.ColumnInfo(oTUserColumnInfo.strCopyFieldName);

				oIOColumnAdd.DataType(KEMIODataTypeDouble);
				oIOColumnAdd.CodeType(KEMIOCodeTypeIsNothing);
				oIOColumnAdd.Visible(true);
				oIOColumnAdd.FreezeColumn(false);
				oIOColumnAdd.DisplayOrder(nDisplayOrder);
				oIOColumnAdd.ColumnType(KEMIOColumnTypeUserDefine);
				oIOColumnAdd.PrimaryKey(false);

				if(a_pIOTable->AddColumn(oIOColumnAdd, bSetDefaultValueZero) == NULL)
				{
					throw 1;
				}

				oTUserColumnInfo.oIOColumnUpdate = oIOColumnAdd;
			}
		}
		catch (...)
		{
			TxLogDebugException();	
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


bool KTableManagerVectorDlg::CopyUserColumnData(KTarget* a_pTarget, KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecUserColumnInfo)
{
	try
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

		nSelectedColumns = nSelectedColumns + 1;//ID Column Add;
		int nColumns = m_wndReportBCtrl.GetColumns()->GetCount();

		std::map<CString, int> mapUserColumnIndex; 
		for (int i= nSelectedColumns; i< nColumns; i++)
		{
			CXTPReportColumn*    pColumnUser = m_wndReportBCtrl.GetColumns()->GetAt(i);
			CString strCaption = pColumnUser->GetCaption();
			mapUserColumnIndex.insert(std::make_pair(strCaption, i));
		}

		KIOColumns       oUpdateDataColumnCollection;
		std::vector<int> vecUpdateUserColumnIndex;

		for (size_t i= 0; i< a_vecUserColumnInfo.size(); i++)
		{
			TUserColumnInfo &oTUserColumnInfo = a_vecUserColumnInfo[i];

			AutoType itFind = mapUserColumnIndex.find(oTUserColumnInfo.strUserFieldName);
			if (itFind != mapUserColumnIndex.end())
			{
				int nUserColumnIndex = itFind->second;
				vecUpdateUserColumnIndex.push_back(nUserColumnIndex);
				oUpdateDataColumnCollection.AddNewColumn(&(oTUserColumnInfo.oIOColumnUpdate));
			}
		}

		CString strTableName   = a_pIOTable->Name();
		std::set<Integer> setIDKey;
		if (strTableName.CompareNoCase(TABLE_NODE) == 0)
		{
			KBulkDBaseNode::AllNodeID(a_pTarget, setIDKey);
		}
		else if (strTableName.CompareNoCase(TABLE_LINK) == 0)
		{
			KBulkDBaseLink::AllLinkId(a_pTarget, setIDKey);
		}
		else if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
		{
			KBulkDBaseDemarcation::AllZoneID(a_pTarget, setIDKey);
		}

		int nAddColumnCnt  = (int)vecUpdateUserColumnIndex.size();
		KIDKeyDoubleRecords oAddRecords(nAddColumnCnt);

		std::map<Integer, KSDoubleRecord*>::const_iterator iter = m_pFieldRecord->begin();
		std::map<Integer, KSDoubleRecord*>::const_iterator end  = m_pFieldRecord->end();

		while(iter != end)
		{
			Integer         nxID        = iter->first;
			KSDoubleRecord* pTempRecord = iter->second;

			if (setIDKey.find(nxID) == setIDKey.end())
			{
				++iter;
				continue;
			}

			KSDoubleRecord* pAddRecord  = oAddRecords.AddRecord(nxID);
			for (int i= 0; i< nAddColumnCnt; i++)
			{
				pAddRecord->SetAt(i, pTempRecord->GetAt(vecUpdateUserColumnIndex[i]));
			}
			
			++iter;
		}  
		
		if (strTableName.CompareNoCase(TABLE_NODE) == 0)
		{
			return KBulkDBase::BulkNodeUpdate( a_pIOTable, oUpdateDataColumnCollection, oAddRecords, true );
		}
		else if (strTableName.CompareNoCase(TABLE_LINK) == 0)
		{
			return KBulkDBase::BulkLinkUpdate( a_pIOTable, oUpdateDataColumnCollection, oAddRecords, true );
		}
		else if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
		{
			return KBulkDBase::BulkZoneUpdate( a_pIOTable, oUpdateDataColumnCollection, oAddRecords, true );
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return false;
}


bool KTableManagerVectorDlg::DeleteCopiedUserColumn(KIOTable* a_pIOTable, std::vector<TUserColumnInfo> &a_vecAddUserColumnInfo)
{
	try
	{
		for (size_t i= 0; i< a_vecAddUserColumnInfo.size(); i++)
		{
			TUserColumnInfo &oTUserColumnInfo = a_vecAddUserColumnInfo[i];
			if (FALSE == oTUserColumnInfo.bNew)
			{
				continue;
			}

			KIOColumn       &oIOColumnAdd           = oTUserColumnInfo.oIOColumnUpdate;
			a_pIOTable->RemoveColumn(oIOColumnAdd.Name());
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}
