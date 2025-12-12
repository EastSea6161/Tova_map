// MullitiModalDefineMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MultiModalDefineMode.h"
#include "afxdialogex.h"
#include "Target.h"

//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "MultiModalInfo.h"
#include "Mode.h"
#include "ModeManager.h"
#include "KExRecordItem.h"

enum
{
	_0_COLUMN_SELECT		= 0,	
	_1_COLUMN_MODE_NAME,
	_2_COLUMN_MATRIX_NAME,
	_3_COLUMN_HIGHWAY,    
	_4_COLUMN_BUS,    
	_5_COLUMN_RAIL,			
	_6_COLUMN_EXCLUSIVE
};


const int MODELOPTION_WITHHIGHWAY	 = 0;
const int MODELOPTION_WOHIGHWAY		 = 1;



// KMullitiModalDefineMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMultiModalDefineMode, KResizeDialogEx)

KMultiModalDefineMode::KMultiModalDefineMode(KTarget* a_pTarget, int a_nModelOption, std::list<KMultiModalInfo>& a_lstMAInfo, int a_nUsingBusSubway, bool a_bUseGeneratorData,std::list<TGenerationPathInfo> a_lstGeneratorInfo, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMultiModalDefineMode::IDD, pParent),
	m_pTarget(a_pTarget),
	m_nModelOption(a_nModelOption),
	m_lstMaInfo(a_lstMAInfo),
	m_nUsingBusRail(a_nUsingBusSubway),
	m_bUseGeneratorData(a_bUseGeneratorData),
	m_lstGeneratorInfo(a_lstGeneratorInfo)
{

}

KMultiModalDefineMode::~KMultiModalDefineMode()
{
}

void KMultiModalDefineMode::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_REPORT, m_wndModeReport);
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KMultiModalDefineMode, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, OnReportCheckItem)
	ON_BN_CLICKED(IDOK, &KMultiModalDefineMode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KMultiModalDefineMode::OnBnClickedCancel)
END_MESSAGE_MAP()


// KMullitiModalDefineMode 메시지 처리기입니다.


BOOL KMultiModalDefineMode::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndModeReport, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndModeReport);
	KDBaseAssignment::GetModeChoiceModelinfo(m_pTarget, m_mapModeChoiceInfo);
	KDBaseAssignment::GetModeChoiceODAtGroupingMode(m_pTarget, m_mapModeChocieColumns);

	ResizeComponent();
	InitReportHeader();
	if (m_bUseGeneratorData == true)
	{
		InitialReportIniData();
	}
	else
	{
		InitExtentReportData();
	}

	return TRUE;  
}

void KMultiModalDefineMode::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT, _T("Select"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_1_COLUMN_MODE_NAME, _T("Mode Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_2_COLUMN_MATRIX_NAME, _T("Matrix Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_3_COLUMN_HIGHWAY, _T("Highway"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도로"));
    }

	if (m_nModelOption == MODELOPTION_WOHIGHWAY ) { 
		pColumn->SetVisible(FALSE);
	}

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_4_COLUMN_BUS, _T("Bus_Network"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("버스"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_5_COLUMN_RAIL, _T("Rail_Network"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("도시철도"));
    }

	pColumn = m_wndModeReport.AddColumn(new CXTPReportColumn(_6_COLUMN_EXCLUSIVE, _T("Exclusive"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
	pColumn->SetEditable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("개별수단"));
    }
} 

void KMultiModalDefineMode::InitExtentReportData()
{

	KModeManager* pModeManager	= m_pTarget->ModeManager();
	KIOTable* pTable			= m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
	const KIOColumns* pColumns	= pTable->Columns();

	AutoType iter = m_mapModeChocieColumns.begin();
	AutoType end  = m_mapModeChocieColumns.end();
	while(iter != end)
	{

		BOOL bUseExistData	= FALSE;
		int  nModeID		= iter->first;
		std::vector<CString> vecModeColumns = iter->second;
		KMode* pMode		= nullptr;

		int nModeCount = pModeManager->GetModeCount();
		for (int i =0; i < nModeCount; i++)
		{
			int nModeIDInModeManager = pModeManager->GetMode(i)->ModeID();
			if (nModeID == nModeIDInModeManager)
			{
				pMode = pModeManager->GetMode(i);
			}
		}

		KMultiModalInfo oMulitiInfo;
		AutoType iterMulti		= m_lstMaInfo.begin();
		AutoType endMulti		= m_lstMaInfo.end();
		while(iterMulti != endMulti)
		{
			KMultiModalInfo oFindMultiInfo = *iterMulti;
			if (oFindMultiInfo.Mode() == pMode)
			{
				oMulitiInfo		= oFindMultiInfo;
				bUseExistData	= TRUE;
			}
			iterMulti++;
		}

		bool bGenerationData(false);
		TGenerationPathInfo oGenInfo = {};
		AutoType iterGen = m_lstGeneratorInfo.begin();
		AutoType endGen	 = m_lstGeneratorInfo.end();
		while(iterGen != endGen)
		{
			TGenerationPathInfo oFindGenInfo = *iterGen;
			if (oFindGenInfo.pMode == pMode)
			{
				oGenInfo		= oFindGenInfo;
				bGenerationData	= true;
			}
			iterGen++;
		}

		if (m_nModelOption == MODELOPTION_WOHIGHWAY && bGenerationData == false)
		{
			++iter;
			continue;
		}

		CXTPReportRecord* pRecord				= m_wndModeReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bUseExistData);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(pMode->ModeName()));
		pItem->SetItemData((DWORD_PTR)pMode);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
		int nSelIndex = 0;
		size_t nColumnCount = vecModeColumns.size();
		for (size_t i = 0; i <nColumnCount; i++)
		{
			CString strColumnName = vecModeColumns[i];
			KIOColumn* pColumn = pColumns->GetColumn(strColumnName);
			int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
			if (pColumn == oMulitiInfo.ModeColumn())
			{
				nSelIndex = nIndex;
			}
		}
		pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItemCombo->SetCurSel(nSelIndex);
		pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItemCombo->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if (TRUE == bUseExistData &&  oMulitiInfo.TransitHighwayType() == 0)
		{
			pItem->SetEditable(FALSE);
			pItem->SetChecked(TRUE);
			pItemCombo->SetBackgroundColor(RGB(255,255,255));
			pItemCombo->SetEditable(TRUE);
		}
		else if (TRUE == bUseExistData || oMulitiInfo.TransitHighwayType() != 0)
		{
			pItem->SetEditable(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (TRUE == bUseExistData )
		{
			if (1 == oMulitiInfo.TransitHighwayType() || 3 == oMulitiInfo.TransitHighwayType())
			{
				pItem->SetEditable(TRUE);
				pItem->SetChecked(TRUE);
			}

		}
		else if ( FALSE == bUseExistData)
		{
			pItem->SetEditable(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if (TRUE == bUseExistData)
		{
			if (2 == oMulitiInfo.TransitHighwayType()|| 3 == oMulitiInfo.TransitHighwayType())
			{
				pItem->SetEditable(TRUE);
				pItem->SetChecked(TRUE);
			}
		}
		else
		{
			pItem->SetEditable(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		if (TRUE == bUseExistData &&  1 == oMulitiInfo.Exclusive())
		{
			pItem->SetChecked(TRUE);
		}
		else if (TRUE == bUseExistData && 0 == oMulitiInfo.Exclusive())
		{
			pItem->SetChecked(FALSE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}

		++iter;
	}

	m_wndModeReport.Populate();
}


void KMultiModalDefineMode::InitialReportIniData()
{

	KModeManager* pModeManager	= m_pTarget->ModeManager();
	KIOTable* pTable			= m_pTarget->Tables()->FindTable(TABLE_MODE_OD);
	const KIOColumns* pColumns	= pTable->Columns();

	AutoType iter = m_mapModeChocieColumns.begin();
	AutoType end  = m_mapModeChocieColumns.end();
	while(iter != end)
	{
		BOOL bUseExistData	= FALSE;
		int  nModeID		= iter->first;
		KMode* pMode		= nullptr;

		int nModeCount = pModeManager->GetModeCount();
		for (int i =0; i < nModeCount; i++)
		{
			int nModeIDInModeManager = pModeManager->GetMode(i)->ModeID();
			if (nModeID == nModeIDInModeManager)
			{
				pMode = pModeManager->GetMode(i);
			}
		}

		TGenerationPathInfo oGenInfo = {};
		AutoType iterGen = m_lstGeneratorInfo.begin();
		AutoType endGen	 = m_lstGeneratorInfo.end();
		while(iterGen != endGen)
		{
			TGenerationPathInfo oFindGenInfo = *iterGen;
			if (oFindGenInfo.pMode == pMode)
			{
				oGenInfo		= oFindGenInfo;
				bUseExistData	= TRUE;
			}
			iterGen++;
		}

		if (m_nModelOption == MODELOPTION_WOHIGHWAY && bUseExistData == FALSE)
		{
			++iter;
			continue;
		}

		std::vector<CString> vecModeColumns = iter->second;

		CXTPReportRecord* pRecord				= m_wndModeReport.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem				= nullptr;
		CXTPReportRecordItemComboEx* pItemCombo = nullptr;

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(bUseExistData);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(pMode->ModeName()));
		pItem->SetItemData((DWORD_PTR)pMode);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItemCombo = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
		int nSelIndex = 0;
		size_t nColumnCount = vecModeColumns.size();
		for (size_t i = 0; i <nColumnCount; i++)
		{
			CString strColumnName = vecModeColumns[i];
			KIOColumn* pColumn = pColumns->GetColumn(strColumnName);
			int nIndex = pItemCombo->AddString(pColumn->Caption(), (DWORD_PTR)pColumn);
			if (pColumn == oGenInfo.pColumn)
			{
				nSelIndex = nIndex;
			}
		}
		pItemCombo->SetCurSel(nSelIndex);
		pItemCombo->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItemCombo->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if ( TRUE == bUseExistData)
		{
			pItem->SetEditable(FALSE);
			pItem->SetChecked(FALSE);
		}
		else
		{
			pItem->SetEditable(FALSE);
			pItem->SetChecked(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (TRUE == bUseExistData && 1 == oGenInfo.nBus)
		{
			pItem->SetChecked(TRUE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (TRUE == bUseExistData && 1 == oGenInfo.nSubway)
		{
			pItem->SetChecked(TRUE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		if (TRUE == bUseExistData && 1 == oGenInfo.nExclusive)
		{
			pItem->SetChecked(TRUE);
		}
		else
		{
			pItem->SetChecked(FALSE);
		}

		++iter;
	}

	m_wndModeReport.Populate();
}



void KMultiModalDefineMode::OnReportItemClick( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{

}


void KMultiModalDefineMode::OnReportCheckItem( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if (nIndex == _0_COLUMN_SELECT)
	{

		if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_SELECT)->IsChecked() )
		{
			pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_SELECT)->GetItemData();
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(TRUE);

			CXTPReportRecordItemComboEx* pComboItem = (CXTPReportRecordItemComboEx*)pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME);

			KIOColumn* pColumn = (KIOColumn*)pComboItem->GetItemData(pComboItem->GetCurSel());

			bool bUseData = false;
			TGenerationPathInfo oInfo;

			AutoType iter = m_lstGeneratorInfo.begin();
			AutoType end  = m_lstGeneratorInfo.end();
			while(iter != end)
			{
				TGenerationPathInfo oFindInfo = *iter;
				if (oFindInfo.pColumn == pColumn)
				{

					bUseData = true;
					oInfo = oFindInfo;
				}
				++iter;
			}

			if (bUseData == true)
			{
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetChecked(FALSE);
			}
			else
			{
				if (MODELOPTION_WITHHIGHWAY  == m_nModelOption)
				{
					pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetChecked(TRUE);
					pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(FALSE);

					pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetEditable(TRUE);
					pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetBackgroundColor(RGB(255,255,255));

				}
				else
				{
					pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_SELECT)->SetChecked(FALSE);
					AfxMessageBox(_T("W/O에서는 Highway를 선택할수 없습니다."));
				}
			}

			if (1 == oInfo.nBus)
			{
				pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_BUS)->SetChecked(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(FALSE);
			}
			if (1 == oInfo.nSubway)
			{
				pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_RAIL)->SetChecked(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(FALSE);
			}
			if (1 == oInfo.nExclusive)
			{
				pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_EXCLUSIVE)->SetChecked(TRUE);
				pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(FALSE);
			}

		}
		else
		{
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_MATRIX_NAME)->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetEditable(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_3_COLUMN_HIGHWAY)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_4_COLUMN_BUS)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_5_COLUMN_RAIL)->SetChecked(FALSE);
			pItemNotify->pRow->GetRecord()->GetItem(_6_COLUMN_EXCLUSIVE)->SetChecked(FALSE);
		}
	}


	m_wndModeReport.RedrawControl();
}


void KMultiModalDefineMode::OnBnClickedOk()
{
	m_lstMaInfo.clear();
	bool bCheckHighway		= false;
	bool bSelectedError		= false;

	CXTPReportRecords* pRecords = m_wndModeReport.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(_0_COLUMN_SELECT);
			
		if (TRUE == pItem->GetCheckedState())
		{
			bool bCheckTransit		= false;
			KMultiModalInfo oMultiModalInfo;

			CXTPReportRecordItemText* pTextItem = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_MODE_NAME);
			KMode* pMode= (KMode*)pTextItem->GetItemData();
			oMultiModalInfo.Mode(pMode);

			CXTPReportRecordItemComboEx* pComboItem = (CXTPReportRecordItemComboEx*)pRecord->GetItem(_2_COLUMN_MATRIX_NAME);
			KIOColumn* pColumn = (KIOColumn*)pComboItem->GetItemData(pComboItem->GetCurSel());
			oMultiModalInfo.ModeColumn(pColumn);

			pItem = pRecord->GetItem(_3_COLUMN_HIGHWAY);
			if (TRUE == pItem->GetCheckedState())
			{
				bCheckTransit = true;
				bCheckHighway = true;
				oMultiModalInfo.Selected(true);
				oMultiModalInfo.TransitHighwayType(0);
				oMultiModalInfo.Occupancy(1.0);
				oMultiModalInfo.PCU(1.0);
				oMultiModalInfo.Mode(pMode);
				oMultiModalInfo.NetworkType(1);

			}

			pItem = pRecord->GetItem(_4_COLUMN_BUS);
			BOOL BBus = pItem->GetCheckedState();

			pItem = pRecord->GetItem(_5_COLUMN_RAIL);
			BOOL BRail = pItem->GetCheckedState();

			if (BBus == TRUE && BRail == TRUE)
			{
				bCheckTransit = true;
				bSelectedError = true;
				oMultiModalInfo.Selected(true);
				oMultiModalInfo.TransitHighwayType(3);
				oMultiModalInfo.Occupancy(1.0);
				oMultiModalInfo.PCU(1.0);
				oMultiModalInfo.NetworkType(4);
			}
			else if (TRUE == BBus)
			{
				bCheckTransit = true;
				bSelectedError = true;
				oMultiModalInfo.Selected(true);
				oMultiModalInfo.TransitHighwayType(1);
				oMultiModalInfo.Occupancy(14.0);
				oMultiModalInfo.PCU(2.0);
				oMultiModalInfo.NetworkType(2);

			}
			else if (TRUE == BRail)
			{
				bCheckTransit = true;
				bSelectedError = true;
				oMultiModalInfo.Selected(true);
				oMultiModalInfo.TransitHighwayType(2);
				oMultiModalInfo.Occupancy(1.0);
				oMultiModalInfo.PCU(1.0);
				oMultiModalInfo.NetworkType(3);
			}

			pItem = pRecord->GetItem(_6_COLUMN_EXCLUSIVE);
			if (TRUE == pItem->GetCheckedState())
			{
				oMultiModalInfo.Exclusive(1);
			}
			else
			{
				oMultiModalInfo.Exclusive(0);
			}

			if (false == bCheckTransit )
			{
				AfxMessageBox(_T("선택한 대중교통/도로 통행수단이 없습니다."));
				return;
			}

			m_lstMaInfo.push_back(oMultiModalInfo);
		}		
	}

	if (MODELOPTION_WITHHIGHWAY  == m_nModelOption)
	{
		if (false == bCheckHighway)
		{
			AfxMessageBox(_T("선택한 도로 통행수단이 없습니다."));
			return;
		}
	}

	if (false == bSelectedError)
	{
		AfxMessageBox(_T("최소 하나 이상의 대중교통 통행수단을 선택해 주세요."));
		return;
	}
	KResizeDialogEx::OnOK();
}


void KMultiModalDefineMode::OnBnClickedCancel()
{	
	KResizeDialogEx::OnCancel();
}

void KMultiModalDefineMode::FindeModeInfo( int a_nModeID, KMode*& a_pMode )
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

void KMultiModalDefineMode::GetMultiModalInfo( std::list<KMultiModalInfo>& a_lstMAInfo )
{
	a_lstMAInfo = m_lstMaInfo;
	return;
}

void KMultiModalDefineMode::ResizeComponent()
{
	
	SetResize(IDC_REPORT,		SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,	SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	
	SetResize(IDOK,				SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,			SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}

bool KMultiModalDefineMode::CheckBusRailExclusive()
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
			pBusItem = pRecord->GetItem(_4_COLUMN_BUS);

			BOOL Bbus = pBusItem->GetCheckedState();

			pSubwayItem = pRecord->GetItem(_5_COLUMN_RAIL);

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
			pBusItem = pRecord->GetItem(_4_COLUMN_BUS);

			BOOL Bbus = pBusItem->GetCheckedState();

			pSubwayItem = pRecord->GetItem(_5_COLUMN_RAIL);

			BOOL BSubway = pSubwayItem->GetCheckedState();

			if (TRUE == Bbus || TRUE == BSubway)
			{
				pExclusiveItem = pRecord->GetItem(_6_COLUMN_EXCLUSIVE);
				pExclusiveItem->SetChecked(BCHECK);
			}
		}
	}

	return bCheBusSubway;
}
