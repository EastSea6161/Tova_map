// GenerationBusPathStep1Dlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GenerationBusPathStep1Dlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "DBaseAssignment.h"
#include "Mode.h"
#include "ModeManager.h"
#include "CustomReportRecordItem.h"
#include "KExRecordItem.h"

enum
{
	_0_COLUMN_SELECT		= 0,
	_1_COLUMN_MODE,
	_2_COLUMN_MATRIX_NAME,
	_3_COLUMN_BUS,    
	_4_COLUMN_RAIL,			
	_5_COLUMN_EXCLUSIVE

};

// KGenerationBusPathStep1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGenerationBusPathStep1Dlg, KResizeDialogEx)

KGenerationBusPathStep1Dlg::KGenerationBusPathStep1Dlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KGenerationBusPathStep1Dlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KGenerationBusPathStep1Dlg::~KGenerationBusPathStep1Dlg()
{
}

void KGenerationBusPathStep1Dlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USINGFIELD, m_cboUsingLinkField);
	DDX_Control(pDX, IDC_REPORT, m_wndModeReport);
	//DDX_Control(pDX, IDC_REPORT3, m_wndColumnReport);
}


BEGIN_MESSAGE_MAP(KGenerationBusPathStep1Dlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, &KGenerationBusPathStep1Dlg::OnBnClickedRadioTimeMethod)
	ON_BN_CLICKED(IDOK, &KGenerationBusPathStep1Dlg::OnBnClickedNext)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItemMode)
	//ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT3, OnReportCheckItemColumn)
END_MESSAGE_MAP()


// KGenerationBusPathStep1Dlg 메시지 처리기입니다.

BOOL KGenerationBusPathStep1Dlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	CWaitCursor wc;

	ResizeComponet();

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndModeReport);
 	CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
 	OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);

	if (m_nTimeMethod == 2)
	{
		CButton*  pButton = (CButton*)GetDlgItem(IDC_RADIO_INITTIME);
		pButton->SetCheck(BST_CHECKED);
	}

	KDBaseAssignment::GetModeChoiceModelinfo(m_pTarget, m_mapModeChoiceInfo);

	std::vector<TNodeTypeInfo>    vecNodeTypeInfo;
	LoadNodeType(vecNodeTypeInfo);
	std::vector<TLinkTypeInfo>    vecLinkTypeInfo;
	LoadLinkType(vecLinkTypeInfo);

	InitComboLinkField();

	InitReportHeader();
	InitReportData();

	return TRUE;  
}


void KGenerationBusPathStep1Dlg::LoadNodeType( std::vector<TNodeTypeInfo> &a_vecNodeTypeInfo )
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{

		CString strSQL(_T(""));
		strSQL.Format(	_T(" SELECT TBL2.node_type AS node_type, TBL1.KCode_Display AS KCode_Display ")
			_T(" FROM ")
			_T(" (SELECT node_type FROM node GROUP BY node_type)TBL2 ")
			_T(" LEFT OUTER JOIN ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL1 ")
			_T(" ON TBL1.KCode_Key = TBL2.node_type "), CODE_GROUP_KEY_NODE          
			);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TNodeTypeInfo oTNodeTypeInfo;

			oTNodeTypeInfo.nNodeType          = pResult->GetValueInt   (0);
			oTNodeTypeInfo.strCodeDisplay     = pResult->GetValueString(1);

			a_vecNodeTypeInfo.push_back(oTNodeTypeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep1Dlg::LoadLinkType( std::vector<TLinkTypeInfo> &a_vecLinkTypeInfo )
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));
		strSQL.Format(	_T(" SELECT TBL2.link_type AS link_type, TBL1.KCode_Display AS KCode_Display ")
			_T(" FROM ")
			_T(" (SELECT link_type FROM link GROUP BY link_type)TBL2 ")
			_T(" LEFT OUTER JOIN ")
			_T(" (SELECT KCode_Key, KCode_Display FROM Kcode_Define WHERE KCode_Group_Key = %d)TBL1 ")
			_T(" ON TBL1.KCode_Key = TBL2.link_type "), CODE_GROUP_KEY_LINK          
			);

		KResultSetPtr pResult = spDBaseConnection->ExecuteQuery(strSQL);

		while( pResult->Next() )
		{
			TLinkTypeInfo oTLinkTypeInfo;

			oTLinkTypeInfo.nLinkType          = pResult->GetValueInt   (0);
			oTLinkTypeInfo.strCodeDisplay     = pResult->GetValueString(1);

			a_vecLinkTypeInfo.push_back(oTLinkTypeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}

	TxLogDebugEndMsg();
}


void KGenerationBusPathStep1Dlg::InitComboLinkField()
{
	m_cboUsingLinkField.ResetContent();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pIOColumns = pIOTable->Columns();

	int nColCount = pIOColumns->ColumnCount();

	int nCur		= 0;
	int nCurrentCur = 0;
	for (int i= 0; i< nColCount; i++)
	{
		KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
		KEMIOColumnType  emColumnType   = pIOColumn->ColumnType();
		KEMIODataType    emDataType     = pIOColumn->DataType();

		if(KEMIODataTypeDouble == emDataType)
		{
			CString strDisplayName    = pIOColumn->Caption();
			nCur   = m_cboUsingLinkField.AddString(strDisplayName);
			m_cboUsingLinkField.SetItemData( nCur, (DWORD_PTR)pIOColumn );
			if (pIOColumn == m_pIOColumnUseLink)
			{
				nCurrentCur = nCur;
			}
		}
	}
	
	m_cboUsingLinkField.SetCurSel(nCurrentCur);
}

void KGenerationBusPathStep1Dlg::OnBnClickedRadioTimeMethod(UINT nID)
{
	if(IDC_RADIO_INITTIME == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(FALSE);
	}
	else if(IDC_RADIO_USINGFIELD == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_USINGFIELD)->SetFocus();
	}
}


void KGenerationBusPathStep1Dlg::OnBnClickedNext()
{
	UpdateData(TRUE);

	int        nCurSel				= -1;
	KIOColumn* pIOColumnLink		= nullptr;

	KDBaseAssignment::GetModeChoiceModelinfo(m_pTarget, m_mapModeChoiceInfo);
	
	int nTimeMethod		= 0;
	int nDistance		= 0;
	if( IDC_RADIO_INITTIME == GetCheckedRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD) )
	{
		nTimeMethod     = KEMTimeMethodInitTime;

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
		const KIOColumns* pIOColumns = pIOTable->Columns();

		int nColCount = pIOColumns->ColumnCount();
		for (int i= 0; i< nColCount; i++)
		{
			KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
			CString          strColumnName  = pIOColumn->Name();
			
			if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) == 0)
			{
				pIOColumnLink = pIOColumn;
			}
		}
	}
	else
	{
		nTimeMethod     = KEMTimeMethodUsingField;
		nCurSel         = m_cboUsingLinkField.GetCurSel();

		if(nCurSel >= 0)
		{
			pIOColumnLink = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCurSel);
		}
		else
		{
			AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
			m_cboUsingLinkField.SetFocus();
			return;
		}

		pIOColumnLink = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCurSel);
	}

	//Step 2.컬럼 및 Mode 저장
	m_lstGenerationInfo.clear();

	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);

		if (TRUE == pItem->GetCheckedState())
		{
			bool bCheckTransit  = false;

			CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_MODE);
			KMode* pMode		= (KMode*)pTextItem->GetItemData();
			KIOColumn* pColumn	= nullptr;

			CXTPReportRecordItemComboEx* pCombo = (CXTPReportRecordItemComboEx*)pRecord->GetItem(_2_COLUMN_MATRIX_NAME);
			pColumn = (KIOColumn*)pCombo->GetItemData(pCombo->GetCurSel());

			if (pColumn == nullptr)
			{
				CString strMessage(_T(""));
				strMessage.Format(_T("선택된 %s 통행수단 관련 데이터가 존재하지 않습니다."), pMode->ModeName());
				AfxMessageBox(strMessage);
				return;
			}

			TGenerationPathInfo oPathInfo;
			oPathInfo.pMode   = pMode;
			oPathInfo.pColumn = pColumn;
			oPathInfo.strModeName  = pMode->ModeName();

			pItem = pRecord->GetItem(_3_COLUMN_BUS);
			if (TRUE == pItem->GetCheckedState())
			{
				bCheckTransit = true;
				oPathInfo.nBus = 1;
			}
			else
			{
				oPathInfo.nBus = 0;
			}

			pItem = pRecord->GetItem(_4_COLUMN_RAIL);
			if (TRUE == pItem->GetCheckedState())
			{
				bCheckTransit = true;
				oPathInfo.nSubway = 1;
			}
			else
			{
				oPathInfo.nSubway = 0;
			}

			pItem = pRecord->GetItem(_5_COLUMN_EXCLUSIVE);
			if (TRUE == pItem->GetCheckedState())
			{
				oPathInfo.nExclusive = 1;
			}
			else
			{
				oPathInfo.nExclusive = 0;
			}

			if (false == bCheckTransit)
			{
				AfxMessageBox(_T("대중교통 관련 수단을 설정해 주세요."));
				return;
			}

			m_lstGenerationInfo.push_back(oPathInfo);
		}
	}

	if (m_lstGenerationInfo.size() < 1)
	{
		AfxMessageBox(_T("선택한 통행수단이 없습니다."));
		return;
	}


	m_nTimeMethod          = nTimeMethod;
	m_pIOColumnUseLink     = pIOColumnLink;

	KResizeDialogEx::OnOK();
}

void KGenerationBusPathStep1Dlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_1_COLUMN_MODE, _T("Mode"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_2_COLUMN_MATRIX_NAME, _T("Matrix"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_3_COLUMN_BUS, _T("Bus_Network"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_4_COLUMN_RAIL, _T("Rail_Network"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시철도"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_5_COLUMN_EXCLUSIVE, _T("Exclusive"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("개별수단"));
    }
}

void KGenerationBusPathStep1Dlg::InitReportData()
{
	std::map<int, std::vector<CString>> mapModeChocieColumns;
	KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, mapModeChocieColumns );

	AutoType iter = mapModeChocieColumns.begin();
	AutoType end  = mapModeChocieColumns.end();
	
	KModeManager* pModeManager = m_pTarget->ModeManager();

	while(iter!= end)
	{
		bool bUseExistData	= false;
		int nModeID			= iter->first;
		KMode* pMode		= nullptr;
		
		int nModeCount = pModeManager->GetModeCount();
		for (int i = 0; i <nModeCount; i++)
		{
			int nModeIDInManager = pModeManager->GetMode(i)->ModeID();
			if (nModeIDInManager == nModeID)
			{
				pMode = pModeManager->GetMode(i);
			}
		}

		std::vector<CString> vecModeColumns = iter->second;

		CXTPReportRecord*     pRecord			= m_wndModeReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		TGenerationPathInfo oInfo = {};
		AutoType iterInfo = m_lstGenerationInfo.begin();
		AutoType endInfo  = m_lstGenerationInfo.end();
		while(iterInfo != endInfo)
		{
			TGenerationPathInfo oFindInfo = *iterInfo;
			if (oFindInfo.pMode == pMode)
			{
				oInfo = oFindInfo;
				bUseExistData = true;
				pItem->SetChecked(TRUE);
			}
			iterInfo++;
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(pMode->ModeName()));
		pItem->SetItemData((DWORD_PTR)pMode);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
		const KIOColumns* pColumns	= pTable->Columns();

		int nSelIndex = 0;
		size_t nColumnCount = vecModeColumns.size();
		for (size_t i = 0; i <nColumnCount; i++)
		{
			CString strColumnName = vecModeColumns[i];
			KIOColumn* pColumn = pColumns->GetColumn(strColumnName);
			int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
			if (pColumn == oInfo.pColumn)
			{
				nSelIndex = nIndex;
			}
		}
		pItemCombo->SetCurSel(nSelIndex);

		if (bUseExistData == false)
		{
			pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemCombo->SetEditable(FALSE);
		}


		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (true == bUseExistData && 1 == oInfo.nBus )
		{
			pItem->SetChecked(TRUE);
			pItem->SetEditable(TRUE);
		}
		else if (true == bUseExistData)
		{
			pItem->SetEditable(TRUE);
		}
		else
		{
			pItem->SetEditable(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (true == bUseExistData && 1 == oInfo.nSubway )
		{
			pItem->SetChecked(TRUE);
			pItem->SetEditable(TRUE);
		}
		else if (true == bUseExistData)
		{
			pItem->SetEditable(TRUE);
		}
		else
		{
			pItem->SetEditable(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (true == bUseExistData && 1 == oInfo.nExclusive )
		{
			pItem->SetChecked(TRUE);
			pItem->SetEditable(FALSE);
		}
		else if (true == bUseExistData)
		{
			pItem->SetEditable(FALSE);
		}
		else
		{
			pItem->SetEditable(FALSE);
		}

		++iter;
	}

	m_wndModeReport.Populate();
}

void KGenerationBusPathStep1Dlg::OnReportCheckItemMode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if (nIndex == _0_COLUMN_SELECT)
	{
		if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_SELECT)->IsChecked() )
		{
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetBackgroundColor(RGB(255,255,255));
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_BUS)->SetEditable(TRUE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_RAIL)->SetEditable(TRUE);
		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_BUS)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_RAIL)->SetEditable(FALSE);

			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_BUS)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_RAIL)->SetChecked(FALSE);
            pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_EXCLUSIVE)->SetChecked(FALSE);
		}
        CheckBusRailExclusive();
	}

	if(_3_COLUMN_BUS == nIndex || _4_COLUMN_RAIL == nIndex )
	{
		BOOL BBus	= pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_BUS)->GetCheckedState();
		BOOL BRail	= pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_RAIL)->GetCheckedState();
		if (FALSE == BBus && FALSE == BRail)
		{
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_EXCLUSIVE)->SetChecked(FALSE);
		}
		else if (BBus == FALSE || BRail == FALSE)
		{
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_EXCLUSIVE)->SetChecked(TRUE);
		}

		if (TRUE == BBus && TRUE == BRail)
		{
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_EXCLUSIVE)->SetChecked(FALSE);
		}
		CheckBusRailExclusive();
	}

	m_wndModeReport.RedrawControl();
}

bool KGenerationBusPathStep1Dlg::CheckBusRailExclusive()
{
	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	bool bCheBusSubway  = false;
	bool bBusCheck		= false;
	bool bSubwayCheck	= false;

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);

		CXTPReportRecordItem* pBusItem		= nullptr; 
		CXTPReportRecordItem* pSubwayItem	= nullptr;
		if (TRUE == pItem->GetCheckedState())
		{
			pBusItem = pRecord->GetItem(_3_COLUMN_BUS);

			BOOL Bbus = pBusItem->GetCheckedState();

			pSubwayItem = pRecord->GetItem(_4_COLUMN_RAIL);

			BOOL BSubway = pSubwayItem->GetCheckedState();

			if (TRUE == Bbus && FALSE == BSubway)
			{
				bBusCheck = true;

			}
			else if (FALSE == Bbus && TRUE == BSubway)
			{
				bSubwayCheck = true;
			}
		}
	}

	BOOL BCHECK = FALSE;
	if (true == bBusCheck || true == bSubwayCheck)
	{
		BCHECK = TRUE;
		bCheBusSubway = true;
	}

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);

		CXTPReportRecordItem* pBusItem			= nullptr; 
		CXTPReportRecordItem* pSubwayItem		= nullptr;
		CXTPReportRecordItem* pExclusiveItem	= nullptr;

		if (TRUE == pItem->GetCheckedState())
		{
			pBusItem = pRecord->GetItem(_3_COLUMN_BUS);

			BOOL Bbus = pBusItem->GetCheckedState();

			pSubwayItem = pRecord->GetItem(_4_COLUMN_RAIL);

			BOOL BSubway = pSubwayItem->GetCheckedState();

			if (TRUE == Bbus || TRUE == BSubway)
			{
				pExclusiveItem = pRecord->GetItem(_5_COLUMN_EXCLUSIVE);
				pExclusiveItem->SetChecked(BCHECK);
			}
		}
	}

	return bCheBusSubway;
}

void KGenerationBusPathStep1Dlg::FindeModeInfo( int a_nModeID, KMode*& a_pMode )
{
	KModeManager* pModeMannger = m_pTarget->ModeManager();
	int nModeCount = pModeMannger->GetModeCount();

	for (int i = 0; i < nModeCount; i++)
	{
		KMode* pMode	= pModeMannger->GetMode(i);
		int	nModeID		= pMode->ModeID();
		if (nModeID == a_nModeID)
		{
			a_pMode = pMode;
			return;
		}
	}
}

void KGenerationBusPathStep1Dlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_STATIC,					  SZ_TOP_LEFT,	SZ_TOP_LEFT);	
		SetResize(IDC_SEPRATOR13,				  SZ_TOP_LEFT,	SZ_TOP_RIGHT);

		SetResize(IDC_REPORT,				      SZ_TOP_LEFT,	SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC2,					  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_SEPRATOR14,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_RADIO_INITTIME,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO_USINGFIELD,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO_USINGFIELD,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDC_SEPRATOR10,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

		SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
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


