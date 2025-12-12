// InterPathGeneratorMainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterPathGeneratorMainDlg.h"
#include "afxdialogex.h"
#include "Target.h"

#include "ImChampDir.h"
#include "InterPathGeneratorUrbanDlg.h"
#include "DBaseDefineTransitCost.h"
#include "InterPathGeneratorRegionalDlg.h"
#include "Project.h"
#include "ImChampFrameWindow.h"
#include "KExRecordItem.h"
#include "WriteCodeAgrument.h"
#include "Launcher.h"
#include "KBulkDBase2File.h"
#include "DBaseAssignment.h"
#include "DBaseDefaultParaInterPathGenerator.h"
#include "DBaseDefaultParaPathGenerateConstraint.h"
#include "DefineNamesDefaultPara.h"
#include "DefineNamesModels.h"
#include "DBaseDefaultParaHighwayAssign.h"


// KInterPathGeneratorMainDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterPathGeneratorMainDlg, KResizeDialogEx)

KInterPathGeneratorMainDlg::KInterPathGeneratorMainDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterPathGeneratorMainDlg::IDD, pParent),
	m_pTarget(a_pTarget),
	m_oPurpoeInfo(m_pTarget),
	m_nUseTurn(0),
	m_pLinkTimeField(nullptr),
	m_nTimeMethod(1)
{

}

KInterPathGeneratorMainDlg::~KInterPathGeneratorMainDlg()
{

}

void KInterPathGeneratorMainDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_OD, m_wndReportCtrlOD);
	DDX_Control(pDX, IDC_REPORT_URBAN, m_wndReportCtrlUrban);
	DDX_Control(pDX, IDC_REPORT_REGION, m_wndReportCtrlRegional);
	DDX_Control(pDX, IDC_COMBO_USINGFIELD, m_cboUsingLinkField);
}

BEGIN_MESSAGE_MAP(KInterPathGeneratorMainDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, &KInterPathGeneratorMainDlg::OnBnClickedRadioTimeMethod)
	ON_BN_CLICKED(IDC_BUTTON_URBAN, &KInterPathGeneratorMainDlg::OnBnClickedButtonUrban)
	ON_BN_CLICKED(IDC_BUTTON_REGIONAL, &KInterPathGeneratorMainDlg::OnBnClickedButtonRegional)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_OD, OnReportCheckItemMode)
	ON_BN_CLICKED(IDOK, &KInterPathGeneratorMainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KInterPathGeneratorMainDlg::OnBnClickedCancel)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_CHECK1, &KInterPathGeneratorMainDlg::OnBnClickedCheckAllMatrix)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &KInterPathGeneratorMainDlg::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


// KInterPathGeneratorMainDlg 메시지 처리기입니다.


BOOL KInterPathGeneratorMainDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		ResizeComponet();

		CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
		OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);
		InitComboLinkField();

		KReportCtrlSetting::Default(m_wndReportCtrlOD, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlOD);
		KReportCtrlSetting::Default(m_wndReportCtrlUrban, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlUrban);
		KReportCtrlSetting::Default(m_wndReportCtrlRegional, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlRegional);

		CheckDlgButton(IDC_CHECK1, TRUE);

		// turn데이터가 없으면 IDC_CHECK_TURN 비활성화
		try 
		{
			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_TURN);
			if (pIOTable->RecordCount() < 1) {
				CheckDlgButton(IDC_CHECK_TURN, BST_UNCHECKED);
				GetDlgItem(IDC_CHECK_TURN)->EnableWindow(FALSE);
			}
		} catch (...) {
			TxLogDebugException();
		}

		InitReportHeaderOD();
		InitReportHeaderUrban();
		InitReportHeaderRegional();

		UpdateReportDataOD();
		UpdateReportDataUrban();
		UpdateReportDataRegion();

		InitUrbanModeDefine();
		InitRegionalModeDefine();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		TxLogDebugException();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterPathGeneratorMainDlg::InitReportHeaderOD( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlOD.AddColumn(new CXTPReportColumn(0, _T("Select"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrlOD.AddColumn(new CXTPReportColumn(1, _T("Purpose"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통행목적"));
    }

	pColumn = m_wndReportCtrlOD.AddColumn(new CXTPReportColumn(2, _T("O/D Column"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("O/D 데이터 컬럼"));
    }
}

void KInterPathGeneratorMainDlg::InitReportHeaderUrban( void )
{
	CXTPReportColumn* pColumn = nullptr;
	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndReportCtrlUrban.AddColumn(new CXTPReportColumn(1, _T("Max.# of Path"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("탐색 경로 개수"));
    }
}

void KInterPathGeneratorMainDlg::InitReportHeaderRegional( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(0, _T("Mode"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단명"));
    }

	pColumn = m_wndReportCtrlRegional.AddColumn(new CXTPReportColumn(1, _T("Max.# of path"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("탐색 경로 개수"));
    }
}

void KInterPathGeneratorMainDlg::UpdateReportDataOD( void )
{
	try
	{
		std::map<int, std::vector<CString>> mapPurposeODGroup;
		KDBaseInterModal::GetpurposeODAtGrouping(m_pTarget, mapPurposeODGroup);

		KIOTable* pPurposeTable = m_pTarget->Tables()->FindTable(TABLE_PURPOSE_OD);
		const KIOColumns* pColumns = pPurposeTable->Columns();

		AutoType iter = mapPurposeODGroup.begin();
		AutoType end  = mapPurposeODGroup.end();
		while(iter != end)
		{
			int nPurposeID							= iter->first;
			std::vector<CString> vecPurposeColumns	= iter->second;

			KPurpose* pPurpose = nullptr;
			AutoType findPurpose = m_oPurpoeInfo.begin();
			AutoType endPurspoe  = m_oPurpoeInfo.end();
			while(findPurpose != endPurspoe)
			{
				KPurpose* pfindPurpose = *findPurpose;
				if (pfindPurpose->PurposeID() == nPurposeID)
				{
					pPurpose = pfindPurpose;
				}
				findPurpose++;
			}

			CXTPReportRecord* pRecord				= m_wndReportCtrlOD.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem				= nullptr;
			CXTPReportRecordItemComboEx* pComboItem = nullptr;

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(TRUE);
			pItem->SetEditable(TRUE);

			
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(pPurpose->PurposeName()));
			pItem->SetItemData((DWORD_PTR)pPurpose);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
			pItem->SetAlignment(DT_CENTER);

			pComboItem = (CXTPReportRecordItemComboEx*)pRecord->AddItem(new CXTPReportRecordItemComboEx());
			size_t nColumnCount = vecPurposeColumns.size();
			for (size_t i = 0; i< nColumnCount; i++)
			{
				CString strColumnName = vecPurposeColumns[i];
				KIOColumn* pColumn = pColumns->GetColumn(strColumnName);
				pComboItem->AddString(pColumn->Caption(),(DWORD_PTR)pColumn);
			}
			//처음 시작시 모드 check로 변경
			pComboItem->SetAlignment(DT_CENTER);
			//pComboItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pComboItem->SetEditable(TRUE);
			pComboItem->SetCurSel(0);
			++iter;
		}

		m_wndReportCtrlOD.Populate();
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

void KInterPathGeneratorMainDlg::UpdateReportDataUrban( void )
{
	try
	{
		InitUrbanModeData();
		m_wndReportCtrlUrban.ResetContent();

		AutoType iter = m_mapUrbanModePath.begin();
		AutoType end  = m_mapUrbanModePath.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlUrban.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			TUrbanPathInfo oInfo = iter->second;
			CString strName = oInfo.strMode;
			CString strPath(_T(""));
			strPath.Format(_T("%d"), oInfo.nMaxofPath);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
			pItem->SetEditable(FALSE);
			pItem->SetItemData(iter->first);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nMode == P_MODE_AUTO || oInfo.nMode == P_MODE_TAXI)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.nMaxofPath, _T("%.0f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}
			++iter;
		}
		m_wndReportCtrlUrban.Populate();

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

void KInterPathGeneratorMainDlg::UpdateReportDataRegion( void )
{
	try
	{
		InitRegionModeData();
		m_wndReportCtrlRegional.ResetContent();

		AutoType iter = m_mapRegionModePath.begin();
		AutoType end  = m_mapRegionModePath.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlRegional.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= nullptr;

			TRegionPathInfo oInfo = iter->second;
			CString strName = oInfo.strMode;
			CString strPath(_T(""));
			strPath.Format(_T("%d"), oInfo.nMaxofPath);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
			pItem->SetEditable(FALSE);
			pItem->SetItemData(iter->first);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);

			if (oInfo.nMode == -1)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oInfo.nMaxofPath, _T("%.0f")));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}

			++iter;
		}
		m_wndReportCtrlRegional.Populate();

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

void KInterPathGeneratorMainDlg::InitUrbanModeData()
{
	m_mapUrbanModePath.clear();

	std::map<int, std::vector<TUrbanPathInfo>> mapUrbanPathInfo;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanPathInfo(mapUrbanPathInfo);

	AutoType bIter = mapUrbanPathInfo.begin();
	AutoType eIter = mapUrbanPathInfo.end();

	if (bIter == eIter)
	{	//Default 없을 시 처리...
		DefaultUrbanModeData();
		return;
	}

	std::vector<TUrbanPathInfo> vecUrbanPatInfo;

	AutoType fIter = mapUrbanPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)	{ //exist user master cose
		vecUrbanPatInfo = fIter->second;
	}
	else
	{
		fIter = mapUrbanPathInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter) {
			vecUrbanPatInfo = fIter->second;
		}
		else
		{
			TxLogDebugException();
			DefaultUrbanModeData();
			return;
		}
	}

	int nSize = vecUrbanPatInfo.size();
	for (int i = 0; i < nSize; ++i) {
		TUrbanPathInfo oUrbanPathInfo = vecUrbanPatInfo[i];
		m_mapUrbanModePath.insert(std::make_pair(oUrbanPathInfo.nMode, oUrbanPathInfo));
	}
}

void KInterPathGeneratorMainDlg::InitRegionModeData()
{
	m_mapRegionModePath.clear();

	std::map<int, std::vector<TRegionPathInfo>> mapRegionalPathInfo;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathInfo(mapRegionalPathInfo);

	AutoType bIter = mapRegionalPathInfo.begin();
	AutoType eIter = mapRegionalPathInfo.end();

	std::vector<TRegionPathInfo> vecRegionPathInfo;

	AutoType fIter = mapRegionalPathInfo.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		vecRegionPathInfo = fIter->second;
	}
	else
	{
		fIter = mapRegionalPathInfo.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter) {
			vecRegionPathInfo = fIter->second;
		}
		else
		{
			TxLogDebugException();
		}
	}
	
	std::map<int/*nMode*/, int/*nMaxofPath*/> mapModeMaxPath;
	int nSize = vecRegionPathInfo.size();

	for (int i = 0; i < nSize; ++i)
	{
		TRegionPathInfo oRegionPathInfo = vecRegionPathInfo[i];
		mapModeMaxPath.insert(std::make_pair(oRegionPathInfo.nMode, oRegionPathInfo.nMaxofPath));
	}

	std::map<int/*nMode*/, int/*nMaxofPath*/>::iterator fIterPath, eIterPath = mapModeMaxPath.end();

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
	std::map<int,CString> mapRegionMode;
	pCodeGroup->GetCodes(mapRegionMode);

	AutoType iter = mapRegionMode.begin();
	AutoType end  = mapRegionMode.end();
	
	while(iter != end)
	{
		TRegionPathInfo oInfo;
		oInfo.nMode   = iter->first;
		oInfo.strMode = iter->second;

		fIterPath = mapModeMaxPath.find(oInfo.nMode);

		if (fIterPath != eIterPath) {
			oInfo.nMaxofPath = fIterPath->second;
		}
		else {
			oInfo.nMaxofPath = 5;
		}
		
		m_mapRegionModePath.insert(std::make_pair(oInfo.nMode, oInfo));
		
		++iter;
	}

	TRegionPathInfo oAEInfo;
	oAEInfo.nMode = KInterRegionModeID::ACC_EGR_CODE;

    if (KmzSystem::GetLanguage() == KEMKorea) {
        oAEInfo.strMode = _T("출발/도착 접근경로");
    }
    else {
	    oAEInfo.strMode = KInterRegionModeID::ACC_EGR_CODE_NAME;
    }

	fIterPath = mapModeMaxPath.find(oAEInfo.nMode);

	if (fIterPath != eIterPath)
	{
		oAEInfo.nMaxofPath = fIterPath->second;
	}
	else
	{
		oAEInfo.nMaxofPath = 5;
	}

	m_mapRegionModePath.insert(std::make_pair(oAEInfo.nMode, oAEInfo));
}


void KInterPathGeneratorMainDlg::OnBnClickedButtonUrban()
{
	KInterPathGeneratorUrbanDlg oDlg(m_pTarget, m_vecUrbanAutoCost, m_vecUrbanTaxiCost, m_vecTransitCost, m_oUrbanParamter);
	
	if (oDlg.DoModal() == IDOK)
	{
		oDlg.GetParameterValue(m_vecUrbanAutoCost, m_vecUrbanTaxiCost, m_vecTransitCost, m_oUrbanParamter);
	}
}


void KInterPathGeneratorMainDlg::OnBnClickedButtonRegional()
{
	KInterPathGeneratorRegionalDlg oDlg(m_pTarget, m_oRegionalParameter, m_vecRegionalAutoCost, m_vecRegionalTaxiCost, m_mapRegionalProcessTime);
	
	if (oDlg.DoModal() == IDOK)
	{
		oDlg.GetParameterValue(m_oRegionalParameter, m_vecRegionalAutoCost, m_vecRegionalTaxiCost, m_mapRegionalProcessTime);
	}
}

void KInterPathGeneratorMainDlg::OnBnClickedRadioTimeMethod( UINT nID )
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


void KInterPathGeneratorMainDlg::InitComboLinkField()
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

void KInterPathGeneratorMainDlg::InitUrbanModeDefine( void )
{
	try
	{
		InitUrbanAutoCost();
		InitUrbanTransitCost();
		
		//Transit fare
		KDBaseDefineTransitCost::GetTransitModeCostValue2DB(m_pTarget, m_vecTransitCost);

		//Path Generator Parameter
		InitUrbanPathParameter();
		InitUrbanConstraintCondition();


		CString strValue(_T(""));
		strValue.Format(_T("%.2f"), m_oUrbanParamter.dInterValueofTime);
		GetDlgItem(IDC_EDIT_VALTIME)->SetWindowText(strValue);

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

void KInterPathGeneratorMainDlg::InitRegionalModeDefine( void )
{
	try
	{
		InitRegionalAutoCost();

		//regional은 m_vecRegionalTaxiCost를 사용하지 않음
		m_vecRegionalTaxiCost.clear();

		TInterTaxiCost oTaxiLPG;
		oTaxiLPG.nType				= P_FUEL_LPG;
		oTaxiLPG.strName			= P_LPG_NAME;
		oTaxiLPG.dBasefare			= 2300.0;
		oTaxiLPG.dBaseDist			= 2.00;
		oTaxiLPG.dAddFare			= 100;
		oTaxiLPG.dAddDist			= 0.15;
		m_vecRegionalTaxiCost.push_back(oTaxiLPG);

		InitRegionalTerminalProcessTime();

		InitRegionalConstraintCondition();
		
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


void KInterPathGeneratorMainDlg::OnReportCheckItemMode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);
	try
	{
		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if (nIndex == 0)
		{
			if (TRUE == pItemNotify->pRow->GetRecord()->GetItem(0)->IsChecked())
			{
				CXTPReportRecordItem* pItem = pItemNotify->pRow->GetRecord()->GetItem(2);
				pItem->SetBackgroundColor(RGB(255,255,255));
				pItem->SetEditable(TRUE);
			}
			else
			{
				CXTPReportRecordItem* pItem = pItemNotify->pRow->GetRecord()->GetItem(2);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetEditable(FALSE);
			}
		}

		CheckMatrixstate();

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

void KInterPathGeneratorMainDlg::ResizeComponet( void )
{
	try
	{
		SetResize(IDC_STATIC_MATRIX,		SZ_TOP_LEFT, SZ_TOP_LEFT);
		SetResize(IDC_CHECK1,		        SZ_TOP_RIGHT, SZ_TOP_RIGHT);

		SetResize(IDC_REPORT_OD,			CXTPResizePoint(0, 0),   CXTPResizePoint(1, 0.4));
		SetResize(IDC_SEPRATOR3,			CXTPResizePoint(0, 0.4), CXTPResizePoint(1, 0.4));

		SetResize(IDC_STATIC_NUMGENER,		CXTPResizePoint(0, 0.4), CXTPResizePoint(0, 0.4));
		SetResize(IDC_STATIC_Urban,			CXTPResizePoint(0, 0.4), CXTPResizePoint(0, 0.4));
		SetResize(IDC_STATIC_REGION,		CXTPResizePoint(0.5, 0.4), CXTPResizePoint(0.5, 0.4));

		SetResize(IDC_REPORT_URBAN,			CXTPResizePoint(0, 0.4), CXTPResizePoint(0.5, 1));
		SetResize(IDC_REPORT_REGION,		CXTPResizePoint(0.5, 0.4), CXTPResizePoint(1, 1));
		SetResize(IDC_BUTTON_URBAN,			SZ_BOTTOM_LEFT, SZ_BOTTOM_CENTER);
		SetResize(IDC_BUTTON_REGIONAL,		SZ_BOTTOM_CENTER, SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR4,			SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC_HIGHOPTION,	SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO_INITTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO_USINGFIELD,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO_USINGFIELD,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC_VALTIME,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_VALTIME,		    SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
			
		SetResize(IDC_CHECK_TURN,			SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDC_SEPRATOR1,			SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

		SetResize(IDC_BUTTON_DEFAULT,		SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
		SetResize(IDOK,						SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,					SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
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


bool KInterPathGeneratorMainDlg::SaveODMatrix( void )
{
	m_mapPurposeODMatrix.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrlOD.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItem* pItem				= nullptr;
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemComboEx* pComboItem = nullptr;

			pItem = pRecord->GetItem(0);
			if (TRUE == pItem->GetCheckedState())
			{
				pItemTxt = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				KPurpose* pPurpose = (KPurpose*)pItemTxt->GetItemData();

				pComboItem = (CXTPReportRecordItemComboEx*)pRecord->GetItem(2);
				KIOColumn* pColumn = (KIOColumn*)pComboItem->GetItemData(pComboItem->GetCurSel());

				m_mapPurposeODMatrix.insert(std::make_pair(pPurpose->PurposeID(), pColumn));
			}
		}

		int nCount = m_mapPurposeODMatrix.size();
		if (nCount < 1)
		{
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KInterPathGeneratorMainDlg::SaveUrbanMaxPath( void )
{
	m_mapUrbanModePath.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrlUrban.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemNumber* pItemNum	= nullptr;
			TUrbanPathInfo oUrbanInfo				= {};

			pItemTxt				= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oUrbanInfo.nMode		= (int)pItemTxt->GetItemData();
			oUrbanInfo.strMode		= pItemTxt->GetValue(); 

			pItemNum				= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oUrbanInfo.nMaxofPath	= (int)pItemNum->GetValue();

			m_mapUrbanModePath.insert(std::make_pair(oUrbanInfo.nMode, oUrbanInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KInterPathGeneratorMainDlg::SaveRegionalMaxPath( void )
{
	m_mapRegionModePath.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrlRegional.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemNumber* pItemNum	= nullptr;
			TRegionPathInfo oRegionalInfo			= {};

			pItemTxt					= (CXTPReportRecordItemText*)pRecord->GetItem(0);
			oRegionalInfo.nMode			= (int)pItemTxt->GetItemData();
			oRegionalInfo.strMode		= pItemTxt->GetValue(); 

			pItemNum				= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			oRegionalInfo.nMaxofPath	= (int)pItemNum->GetValue();

			m_mapRegionModePath.insert(std::make_pair(oRegionalInfo.nMode, oRegionalInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	return true;
}

bool KInterPathGeneratorMainDlg::SaveHighwayOption( void )
{
	if( IDC_RADIO_INITTIME == GetCheckedRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD) )
	{
		m_nTimeMethod     = 1;
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
				m_pLinkTimeField = pIOColumn;
			}
		}
	}
	else
	{
		m_nTimeMethod     = 2;
		int nCursel         = m_cboUsingLinkField.GetCurSel();
		m_pLinkTimeField = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCursel);
	}


	if (TRUE == IsDlgButtonChecked(IDC_CHECK_TURN))
	{
		m_nUseTurn = 1;
	}
	else
	{
		m_nUseTurn = 0;
	}

	double dValueOfTime(1);
	std::map<int, KODTime> mapODTime;
	KDBaseDefaultParaHighwayAssign::LoadDefaultODTime(mapODTime);
	AutoType Sfind = mapODTime.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
	AutoType Ufind = mapODTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
	AutoType end  = mapODTime.end();
	if (Sfind != end)
	{
		KODTime oInfo = Sfind->second;
		dValueOfTime = oInfo.dTravelTime;
	}
	else if (Ufind != end)
	{
		KODTime oInfo = Ufind->second;
		dValueOfTime = oInfo.dTravelTime;
	}

	CString strValTime(_T(""));
	GetDlgItemText(IDC_EDIT_VALTIME, strValTime);
	double dInterValTime = _ttof(strValTime);

	if (dValueOfTime == 1 && dInterValTime < 1)
	{
		AfxMessageBox(_T("유료도로 통행료 시간가치(VOT)가 1이므로 링크통행시간 값은 1이상이 되어야 합니다."));
		return false;
	}

	if (dValueOfTime > 1 && dInterValTime != 1)
	{
		if (AfxMessageBox(_T("유료도로 통행료 시간가치(VOT)가 1보다 크기 때문에 링크통행시간 값은 1로 저장됩니다."), MB_YESNO) == IDYES )
		{
			dInterValTime = 1;
		}
		else
		{
			return false;
		}
	}

	m_oUrbanParamter.dInterValueofTime = dInterValTime;
	m_oUrbanParamter.dHighwayVOT	   = dValueOfTime;

	return true;
}


void KInterPathGeneratorMainDlg::OnBnClickedOk()
{
	try
	{
		if (SaveODMatrix() == false)
		{
			AfxMessageBox(_T("선택한 통행목적이 없습니다."));
			return;
		}
		SaveUrbanMaxPath();
		SaveRegionalMaxPath();

		if (SaveHighwayOption() == false)
		{
			return;
		}

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(InterGeneratorThreadCaller, this, true, nLang);

		if (m_nResultCode == DLL_PLANNED_TERMINATION)
		{
			SaveResult2DB();
			CopyFile(T_ARG_INTERMODAL_PATH_GENERATOR, ConvertSavePath(T_ARG_INTERMODAL_PATH_GENERATOR), FALSE);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AfxMessageBox(KModelRunMassage::MAG_COMPLECTE);
			}
		}
		else if (m_nResultCode == DLL_USER_STOPPING)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AfxMessageBox(KModelRunMassage::MAG_MODEL_STOP);
			}
			return;
		}
		else
		{
			throw -1;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AfxMessageBox(KModelRunMassage::MAG_ERROR_COMMAND);
		}
		
		TxLogDebugException();
		return;
	}
	KResizeDialogEx::OnOK();
}

void KInterPathGeneratorMainDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


unsigned __stdcall KInterPathGeneratorMainDlg::InterGeneratorThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("복합수단 경로 생성 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Intermodal Path Generator..."));
	}
	KInterPathGeneratorMainDlg*        pDlg = (KInterPathGeneratorMainDlg*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KInterPathGeneratorMainDlg::AddStatusMessage( CString a_strMsg )
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}

void KInterPathGeneratorMainDlg::Run( void )
{
	try
	{
		//KDBaseInterModal::ExportDB2TxtXBusRest(m_pTarget);

		CreateArgument();

		CString strGeneratorPath(_T(""));
		strGeneratorPath.Format(_T("%s\\%s"), DLL_MODEL_FOLDER_NAME, _T("Intermodalgenerator"));

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, strGeneratorPath);

		m_nResultCode = DLL_UNEXPECTED_ERROR;
		CString strWindowName;
		GetWindowText(strWindowName);

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			m_nResultCode = DLL_USER_STOPPING;
			return;
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("복합수단 경로 생성 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Intermodal Path Generator Module..."));
		}

		KLauncher::Run(strWindowName, 18);

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("복합수단 경로 생성 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Intermodal Path Generator Module Complete."));
			}
			TxLogDebug(_T("Model Run Ok.."));
		}
		else if (DLL_USER_STOPPING == m_nResultCode || QBicSimpleProgressThread::IsUserStopped())
		{
            m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			TxLogDebug(_T("Model Stop.."));
			ThrowException(_T("Model Stop..")); 
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> InterModal Path Generator  Dll Not Found ..."));
			TxLogDebug(_T("DLL NOT Found.."));
			ThrowException(_T("DLL Not Found.."));
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
			TxLogDebug(_T("Model Run Error.."));
			ThrowException(_T("InterModal Path Generator End(Error)"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		m_nResultCode = DLL_UNEXPECTED_ERROR;
	}
	catch(...)
	{
		TxLogDebugException();
		m_nResultCode = DLL_UNEXPECTED_ERROR;
	}
}


CString KInterPathGeneratorMainDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH,INTERMODAL_GENERATOR_PATH ,a_strFileName);

	return strSavePath;
}

void KInterPathGeneratorMainDlg::ClearFile( void )
{
	int nResult = -1;
	try
	{
		nResult = remove((CStringA)T_ARG_INTERMODAL_PATH_GENERATOR);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TERMINAL_SCHEDULE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE));
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


void KInterPathGeneratorMainDlg::DeleteAllFolderFile( CString a_strFolderName )
{
	CFileFind finder;
	BOOL bContinue = TRUE;

	if(a_strFolderName.Right(1) != _T("\\"))
		a_strFolderName += _T("\\");

	a_strFolderName += _T("*.*");
	bContinue = finder.FindFile(a_strFolderName);
	while(bContinue)
	{
		bContinue = finder.FindNextFile();
		if(finder.IsDots()) // Ignore this item.
		{
			continue;
		}
		else if(finder.IsDirectory()) // Delete all sub item.
		{
			DeleteAllFolderFile(finder.GetFilePath());
			::RemoveDirectory((LPCTSTR)finder.GetFilePath());
		}
		else // Delete file.
		{
			::DeleteFile((LPCTSTR)finder.GetFilePath());
		}
	}
}


void KInterPathGeneratorMainDlg::CreateArgument( void )
{
	try
	{
		ImChampDir::SetCurrentDirectoryToApp();

		ClearFile();

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(T_ARG_INTERMODAL_PATH_GENERATOR, nOpenMode);

		CString strModelPath(_T(""));
		strModelPath.Format(_T("%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);

		CreateDirectory(strModelPath,NULL);
		CreateDirectory(ConvertSavePath(_T("")), NULL);

		CString strDllPath = ConvertSavePath(_T(""));
		CString strResultPath = strDllPath + INTERMODAL_GEN_RESULT_PATH;

		DeleteAllFolderFile(strResultPath);
		CreateDirectory(strResultPath, NULL);

		ArgumentWriteProjectName(of);
		ArgumentWriteModeOD(of);
		ArgumnetWriteUrbanMaxPath(of);
		ArgumnetWriteRegionMaxPath(of);
		ArgumentWriteTurnRestriction(of);
		ArgumnetWriteTimeMethod(of);
		ArgumentWriteUrbanAuto(of);
		ArgumentWriteUrbanTaxi(of);
		ArgumentWriteUrbanTransit(of);
		ArgumentWriteUrbanControlParameter(of);
		ArgumentWriteRegionalAuto(of);
		ArgumentWriteProcessingTime(of);
		ArgumentWirteRegionControlParameter(of);
		ArgumentWriteNoofClass(of);
		ArgumentWriteFolder(of);
		of.Close();

		KIOColumns oODColumnColetion;
		AutoType iter = m_mapPurposeODMatrix.begin();
		AutoType end  = m_mapPurposeODMatrix.end();
		while(iter != end)
		{
			KIOColumn* pColumn = iter->second;
			oODColumnColetion.AddNewColumn(pColumn);
			++iter;
		}

		if ( !KDBaseInterModal::InterExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("node.dat")) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::EXportDB2LinkFileInterGeneratorPath(m_pTarget->GetDBaseConnection(), m_pLinkTimeField, ConvertSavePath(T_DAT_LINK) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LinkFileC) 중 오류가 발생하였습니다. !!!"));
		}

		if( QBicSimpleProgressThread::IsUserStopped() == true )
		{
			m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
		}

		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_LINEINFO), ConvertSavePath(T_DAT_LINE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, oODColumnColetion, ConvertSavePath(T_DAT_PURPOSEOD) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2AccessStopNodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_OUT_ACC_BUSSUBWAYSTOP_ATZONE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2AccessNodeStopFile) 중 오류가 발생하였습니다. !!!"));
		}

		if (!KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(),  ConvertSavePath(T_DAT_TURN)))
		{
			ThrowException(_T("Argument 생성(회전정보) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDB2TerminalScheulFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_TERMINAL_SCHEDULE)))
		{
			ThrowException(_T("Dat 생성(ExportDB2TerminalFlaeFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDb2AccessTerminalNodeAtZone(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_ACCESSTERMINAL_NODE)))
		{
			ThrowException(_T("Dat 생성(AccessTerminalNoeAtZone) 중 오류가 발생하였습니다. !!!"));
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KInterPathGeneratorMainDlg::ArgumentWriteProjectName( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		KProject* pProject = ImChampFrameWindow::GetProject();
		strOut.Format(_T("%s_%s_%s\r\n"), pProject->GetName(), m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear());
		of.WriteString(strOut);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


void KInterPathGeneratorMainDlg::ArgumentWriteModeOD( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		int nCount = m_mapPurposeODMatrix.size();
		strOut.Format(_T("PurposeOD\t%d\r\n"), nCount);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumnetWriteUrbanMaxPath( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		AutoType iter = m_mapUrbanModePath.begin();
		AutoType end  = m_mapUrbanModePath.end();
		while(iter != end)
		{
			TUrbanPathInfo oInfo = iter->second;
			strOut.Format(_T("%s\t%d\r\n"), oInfo.strMode, oInfo.nMaxofPath);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumnetWriteRegionMaxPath( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		int nCount = m_mapRegionModePath.size();
		strOut.Format(_T("noModeR\t%d\r\n"), nCount);
		of.WriteString(strOut);

		AutoType iter = m_mapRegionModePath.begin();
		AutoType end  = m_mapRegionModePath.end();
		while(iter != end)
		{
			TRegionPathInfo oInfo = iter->second;
			strOut.Format(_T("%s\r\n%d\r\n"), oInfo.strMode, oInfo.nMaxofPath);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteTurnRestriction( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("TurnRestriction\t%d\r\n"), m_nUseTurn);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumnetWriteTimeMethod( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("TimeMethod\t%d\r\n"), m_nTimeMethod);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteUrbanAuto( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		size_t nCount = m_vecUrbanAutoCost.size();
		for (size_t i =0 ; i < nCount; i++)
		{
			TInterAutoCost oAutoInfo = m_vecUrbanAutoCost[i];
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strOut.Format(_T("Auto%s\t%f\t%f\t%f\r\n"), oAutoInfo.strName, oAutoInfo.dPrice, oAutoInfo.dRation, oAutoInfo.dfule);
			}
			else
			{
				strOut.Format(_T("PassengerCar%s\t%f\t%f\t%f\r\n"), oAutoInfo.strName, oAutoInfo.dPrice, oAutoInfo.dRation, oAutoInfo.dfule);
			}
			
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteUrbanTaxi( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		size_t nCount = m_vecUrbanTaxiCost.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TInterTaxiCost oInfo = m_vecUrbanTaxiCost[i];
			strOut.Format(_T("Taxi%s\t%f\t%f\t%f\t%f\r\n"), oInfo.strName, oInfo.dBasefare, oInfo.dBaseDist, oInfo.dAddFare, oInfo.dAddDist);
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteUrbanTransit( CStdioFileEx& of )
{
	try
	{
		int nBusCount(0);
		int nRailCount(0);
		CString strOut(_T(""));
		size_t nCount = m_vecTransitCost.size();
		for (size_t i = 0; i <nCount; i++)
		{
			TTransitCost oInfo = m_vecTransitCost[i];
			if (oInfo.nMode == TRANSIT_MODE_BUS)
			{
				nBusCount++;
			}
			else if (oInfo.nMode == TRANSIT_MODE_RAIL)
			{
				nRailCount++;
			}
		}
		strOut.Format(_T("detailedLineType\t%d\t%d\r\n"), nBusCount, nRailCount);
		of.WriteString(strOut);

		for (size_t i =0 ; i < nCount; i++)
		{
			CString strModeName(_T(""));
			TTransitCost oInfo = m_vecTransitCost[i];
			if (oInfo.nMode == TRANSIT_MODE_BUS)
			{
				strModeName = _T("buscost");
			}
			else if(oInfo.nMode == TRANSIT_MODE_RAIL)
			{
				strModeName = _T("railcost");
			}
			strOut.Format(_T("%s\t%d\t%f\t%f\t%f\t%f\t%d\r\n"),
                             strModeName,       oInfo.nTType, 
                             oInfo.dTbasicRate, oInfo.dTbasicDistance, 
                             oInfo.dTAddRate,   oInfo.dTAddDistance, 
                             oInfo.nTMergeRate );
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteUrbanControlParameter( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));

		strOut.Format(_T("ValueOfTime\t%f\t%f\r\n"),m_oUrbanParamter.dHighwayVOT, m_oUrbanParamter.dInterValueofTime);
		of.WriteString(strOut);
		strOut.Format(_T("TaxiWaitingTime\t%f\r\n"), m_oUrbanParamter.dDWTimeTaxi);
		of.WriteString(strOut);
		strOut.Format(_T("TransitDwellTime\t%f\t%f\r\n"),m_oUrbanParamter.dDWTimeBusstop, m_oUrbanParamter.dDwTimeRailStation);
		of.WriteString(strOut);
		strOut.Format(_T("Constraint\t2\r\n"));
		of.WriteString(strOut);
		strOut.Format(_T("Time\t%f\t%d\t%d\t%d\t%d\r\n"), m_oUrbanParamter.dFirstPathTime, m_oUrbanParamter.nInvehicle, m_oUrbanParamter.nWating, 
			m_oUrbanParamter.nTransfer, m_oUrbanParamter.nAETime);
		of.WriteString(strOut);
		strOut.Format(_T("Length\t%f\r\n"), m_oUrbanParamter.dSecondePathLength);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteRegionalAuto( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		size_t nCount = m_vecRegionalAutoCost.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TInterAutoCost oInfo = m_vecRegionalAutoCost[i];
			
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strOut.Format(_T("Auto%sR\t%f\t%f\t%f\r\n"), oInfo.strName, oInfo.dPrice, oInfo.dRation, oInfo.dfule);
			}
			else
			{
				strOut.Format(_T("PassengerCar%sR\t%f\t%f\t%f\r\n"), oInfo.strName, oInfo.dPrice, oInfo.dRation, oInfo.dfule);
			}
			of.WriteString(strOut);
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteProcessingTime( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		AutoType iter = m_mapRegionalProcessTime.begin();
		AutoType end  = m_mapRegionalProcessTime.end();
		while(iter != end)
		{
			TTerminalProcessTime oInfo = iter->second;
			strOut.Format(_T("%s_ProcessingTime\t%f\t%f\t%d\r\n"), oInfo.strModeName, oInfo.dAccess, oInfo.dEgress, oInfo.nModeTransfer);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWirteRegionControlParameter( CStdioFileEx& of )
{
	try
	{
		CString strOut(_T(""));
		strOut.Format(_T("ConstraintR\t2\r\n"));
		of.WriteString(strOut);
		strOut.Format(_T("TimeR\t%f\t%d\t%d\t%d\t%d\r\n"), m_oRegionalParameter.dFirstPathTime, m_oRegionalParameter.nInvehicle, m_oRegionalParameter.nWating, 
			m_oRegionalParameter.nTransfer, m_oRegionalParameter.nAETime);
		of.WriteString(strOut);
		strOut.Format(_T("CostR\t%f\r\n"), m_oRegionalParameter.dSecondePathLength);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteNoofClass( CStdioFileEx& of )
{
	CString strOut;
	std::map<CString, double> mapNoofClass;

	try
	{
		KDBaseAssignment::GetNoClass(m_pTarget, mapNoofClass);

		int nCount = mapNoofClass.size();

		strOut.Format(_T("noClass\t%d\r\n"), nCount);
		of.WriteString(strOut);

		std::map<CString, double>::iterator iter = mapNoofClass.begin();
		std::map<CString, double>::iterator end  = mapNoofClass.end();

		while(iter != end)
		{
			strOut.Format(_T("%s\t%f\r\n"), iter->first, iter->second);
			of.WriteString(strOut);
			++iter;
		}
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KInterPathGeneratorMainDlg::ArgumentWriteFolder( CStdioFileEx& of )
{
	CString strFolder;
	try
	{
		strFolder.Format(_T("%s"),m_pTarget->GetLocation());
		strFolder = strFolder + DLL_READ_WRITE_PATH + INTERMODAL_GENERATOR_PATH;
		strFolder = strFolder + _T("\r\n");
		of.WriteString(strFolder);
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KInterPathGeneratorMainDlg::SaveResult2DB( void )
{
	KDBaseConPtr spDBaseConnetion = m_pTarget->GetDBaseConnection();
	spDBaseConnetion->BeginTransaction();

	try
	{
		SaveResultPurposeOD(spDBaseConnetion);
		SaveResultUrbanMaxpath(spDBaseConnetion);
		SaveResultRegionMaxpath(spDBaseConnetion);
		SaveResultAutoCostInfo(spDBaseConnetion);
		SaveResultTaxiCostInfo(spDBaseConnetion);
		SaveTerminalTimeInfo(spDBaseConnetion);
		SaveParameterOption(spDBaseConnetion);
		SaveTransitCostInfo(spDBaseConnetion);
		SaveLinkOption(spDBaseConnetion);

		spDBaseConnetion->Commit();

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnetion->RollBack();
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnetion->RollBack();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveResultPurposeOD( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_PURPOSE_OD_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_PURPOSE_OD_INFO);
		strPreparedQuery.Append(_T(" ( purpose_id, purpose_column) Values (?, ?) "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		AutoType iter = m_mapPurposeODMatrix.begin();
		AutoType end  = m_mapPurposeODMatrix.end();
		while(iter != end)
		{
			int nPurposeID = iter->first;
			KIOColumn* pColumn = iter->second;

			spPrepareStatement->BindInt(1, nPurposeID);
			spPrepareStatement->BindText(2, pColumn->Name());

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}	
}

void KInterPathGeneratorMainDlg::SaveResultUrbanMaxpath( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_URBAN_PATH_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_URBAN_PATH_INFO);
		strPreparedQuery.Append(_T(" ( mode, no_path) Values (?, ?)  "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		AutoType iter = m_mapUrbanModePath.begin();
		AutoType end = m_mapUrbanModePath.end();
		while(iter != end)
		{
			TUrbanPathInfo oInfo = iter->second;
			spPrepareStatement->BindText(1, oInfo.strMode);
			spPrepareStatement->BindInt(2, oInfo.nMaxofPath);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveResultRegionMaxpath( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_REGION_PATH_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_REGION_PATH_INFO);
		strPreparedQuery.Append(_T(" (id, name, no_path) Values (?, ?, ?)  "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		AutoType iter = m_mapRegionModePath.begin();
		AutoType end = m_mapRegionModePath.end();
		while(iter != end)
		{
			TRegionPathInfo oInfo = iter->second;
			spPrepareStatement->BindInt(1, oInfo.nMode);
			spPrepareStatement->BindText(2, oInfo.strMode);
			spPrepareStatement->BindInt(3, oInfo.nMaxofPath);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveResultAutoCostInfo( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_AUTO_COST_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_AUTO_COST_INFO);
		strPreparedQuery.Append(_T(" ( type, id, name, price, ration, fule) Values (?, ?, ?, ?, ?, ?) "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		size_t nUrbanCount = m_vecUrbanAutoCost.size();
		for (size_t i = 0; i <nUrbanCount; i++)
		{
			TInterAutoCost oInfo = m_vecUrbanAutoCost[i];
			spPrepareStatement->BindInt(1, INTER_URBAN_CODE);
			spPrepareStatement->BindInt(2, oInfo.nType);
			spPrepareStatement->BindText(3, oInfo.strName);
			spPrepareStatement->BindDouble(4, oInfo.dPrice);
			spPrepareStatement->BindDouble(5, oInfo.dRation);
			spPrepareStatement->BindDouble(6, oInfo.dfule);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		size_t nRegionalCount = m_vecRegionalAutoCost.size();
		for (size_t i = 0; i < nRegionalCount; i++)
		{
			TInterAutoCost oInfo = m_vecRegionalAutoCost[i];
			spPrepareStatement->BindInt(1, INTER_REGIONAL_CODE);
			spPrepareStatement->BindInt(2, oInfo.nType);
			spPrepareStatement->BindText(3, oInfo.strName);
			spPrepareStatement->BindDouble(4, oInfo.dPrice);
			spPrepareStatement->BindDouble(5, oInfo.dRation);
			spPrepareStatement->BindDouble(6, oInfo.dfule);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
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

void KInterPathGeneratorMainDlg::SaveResultTaxiCostInfo( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_TAXI_COST_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TAXI_COST_INFO);
		strPreparedQuery.Append(_T(" ( type, id, name, base_fare, base_dist, add_fare, add_dist) Values ( ?, ?, ?, ?, ?, ?, ? ) "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		size_t nCount = m_vecUrbanTaxiCost.size();
		for (size_t i = 0; i <nCount; i++)
		{
			TInterTaxiCost oInfo = m_vecUrbanTaxiCost[i];
			spPrepareStatement->BindInt(1, INTER_URBAN_CODE);
			spPrepareStatement->BindInt(2, oInfo.nType);
			spPrepareStatement->BindText(3, oInfo.strName);
			spPrepareStatement->BindDouble(4, oInfo.dBasefare);
			spPrepareStatement->BindDouble(5, oInfo.dBaseDist);
			spPrepareStatement->BindDouble(6, oInfo.dAddFare);
			spPrepareStatement->BindDouble(7, oInfo.dAddDist);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveTerminalTimeInfo( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_TERMINAL_TIME_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TERMINAL_TIME_INFO);
		strPreparedQuery.Append(_T(" ( id, name, access_time, egress_time, mode_transfer) Values ( ?, ?, ?, ?, ? ) "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnetion->PrepareStatement(strPreparedQuery);

		AutoType iter = m_mapRegionalProcessTime.begin();
		AutoType end  = m_mapRegionalProcessTime.end();
		while(iter != end)
		{
			TTerminalProcessTime oInfo = iter->second;
			spPrepareStatement->BindInt(1, oInfo.nModeID);
			spPrepareStatement->BindText(2, oInfo.strModeName);
			spPrepareStatement->BindDouble(3, oInfo.dAccess);
			spPrepareStatement->BindDouble(4, oInfo.dEgress);
			spPrepareStatement->BindInt(5, oInfo.nModeTransfer);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveParameterOption( KDBaseConPtr spDBaseConnetion )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_TIME_INFO);
		spDBaseConnetion->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T("")); {
// 			strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TIME_INFO);
// 			strPreparedQuery.Append(_T(" ( type, taxi_time, bus_time, subway_time, constaint_time, constaint_length, invehicle_time, "));
// 			strPreparedQuery.Append(_T(" waiting_time, transfer_time, ae_time, use_turn, inter_vot ) "));
// 			strPreparedQuery.AppendFormat(_T(" Values ( %d, %f, %f, %f, %f, %f, %d, %d, %d, %d, %d, %f )"), INTER_URBAN_CODE,
// 				m_oUrbanParamter.dDWTimeTaxi, m_oUrbanParamter.dDWTimeBusstop, m_oUrbanParamter.dDwTimeRailStation, m_oUrbanParamter.dFirstPathTime,
// 				m_oUrbanParamter.dSecondePathLength, m_oUrbanParamter.nInvehicle, m_oUrbanParamter.nWating, m_oUrbanParamter.nTransfer, m_oUrbanParamter.nAETime,
// 				m_nUseTurn, m_oUrbanParamter.dInterValueofTime);

			strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TIME_INFO);
			strPreparedQuery.Append(_T(" ( type, taxi_time, bus_time, subway_time, constaint_time, constaint_length, invehicle_time, "));
			strPreparedQuery.Append(_T(" waiting_time, transfer_time, ae_time, use_turn, inter_vot ) "));
			strPreparedQuery.AppendFormat(_T(" Values ( %d, %f, %f, %f, %f, %f, %d, %d, %d, %d, %d, %f )"), INTER_URBAN_CODE,
				m_oUrbanParamter.dDWTimeTaxi, 2.0, m_oUrbanParamter.dDwTimeRailStation, m_oUrbanParamter.dFirstPathTime,
				m_oUrbanParamter.dSecondePathLength, m_oUrbanParamter.nInvehicle, m_oUrbanParamter.nWating, m_oUrbanParamter.nTransfer, m_oUrbanParamter.nAETime,
				m_nUseTurn, m_oUrbanParamter.dInterValueofTime);

			/************************************************************************/
			/* 2017.01.17 명지대 요청으로 m_oUrbanParamter.dDWTimeBusstop 대신에 
			2.0 값을 넣어줌.
			이유인 즉, PathGenerator에서 사용되는 dDWTimeBusstop 의 성격과
			모형구동시(KInterUrbanChoiceParameterDlg)에서 TABLE_INTER_TIME_INFO 을 조회해서 사용되는 dDWTimeBusstop의 성격은
			전혀 다른 녀석이기때문에.. 
			*/
			/************************************************************************/
		}
		spDBaseConnetion->ExecuteUpdate(strPreparedQuery);

		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TIME_INFO);
		strPreparedQuery.Append(_T(" ( type, constaint_time, constaint_length, invehicle_time, waiting_time, transfer_time, ae_time ) "));
		strPreparedQuery.AppendFormat(_T(" Values ( %d, %f, %f, %d, %d, %d, %d )"),  INTER_REGIONAL_CODE , 
			m_oRegionalParameter.dFirstPathTime, m_oRegionalParameter.dSecondePathLength, m_oRegionalParameter.nInvehicle, m_oRegionalParameter.nWating, m_oRegionalParameter.nTransfer, m_oUrbanParamter.nAETime );
		spDBaseConnetion->ExecuteUpdate(strPreparedQuery);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

void KInterPathGeneratorMainDlg::SaveTransitCostInfo( KDBaseConPtr spDBaseConnection )
{
	try
	{
		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("delete from %s"), TABLE_INTER_TRANSIT_COST);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		CString strPreparedQuery(_T(""));
		strPreparedQuery.Format(_T(" Insert Into %s "), TABLE_INTER_TRANSIT_COST);
		strPreparedQuery.Append(_T(" (transit_Type, transit_detail_type, base_cost, base_length, add_cost , add_length, cost_method)  Values ( ?, ?, ?, ?, ?, ?, ? ) "));

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		size_t nCount = m_vecTransitCost.size();
		for (size_t i = 0; i < nCount; i++)
		{
			TTransitCost oInfo = m_vecTransitCost[i];

			spPrepareStatement->BindInt(1, oInfo.nMode);
			spPrepareStatement->BindInt(2, oInfo.nTType);
			spPrepareStatement->BindDouble(3, oInfo.dTbasicRate);
			spPrepareStatement->BindDouble(4, oInfo.dTbasicDistance);
			spPrepareStatement->BindDouble(5, oInfo.dTAddRate);
			spPrepareStatement->BindDouble(6, oInfo.dTAddDistance);
			spPrepareStatement->BindInt(7, oInfo.nTMergeRate);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}


BOOL KInterPathGeneratorMainDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	int     nMsgCode = pCopyDataStruct->dwData;
	CString strMsg((LPCTSTR)pCopyDataStruct->lpData);

	m_nResultCode = nMsgCode;
	AddStatusMessage(strMsg);

	return KResizeDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void KInterPathGeneratorMainDlg::OnBnClickedCheckAllMatrix()
{
	try
	{
		BOOL bCheck = IsDlgButtonChecked(IDC_CHECK1);
		if (bCheck == TRUE)
		{
			SetCheckMatrixAll();
		}
		else
		{
			SetUnCheckMatrixAll();
		}

		m_wndReportCtrlOD.RedrawControl();
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

void KInterPathGeneratorMainDlg::SetCheckMatrixAll( void )
{
	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrlOD.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItem* pItem				= nullptr;
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemComboEx* pComboItem = nullptr;

			pItem = pRecord->GetItem(0);
			pItem->SetChecked(TRUE);

			pItem = pRecord->GetItem(2);
			pItem->SetBackgroundColor(RGB(255,255,255));
			pItem->SetEditable(TRUE);
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

void KInterPathGeneratorMainDlg::SetUnCheckMatrixAll( void )
{
	try
	{
		CXTPReportRecords* pRecords = m_wndReportCtrlOD.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItem* pItem				= nullptr;
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemComboEx* pComboItem = nullptr;

			pItem = pRecord->GetItem(0);
			pItem->SetChecked(FALSE);

			pItem = pRecord->GetItem(2);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);
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

void KInterPathGeneratorMainDlg::CheckMatrixstate( void )
{
	try
	{
		bool bCheckAll = true;

		CXTPReportRecords* pRecords = m_wndReportCtrlOD.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for (int i= 0; i < nRecordCount; i++)
		{
			CXTPReportRecord* pRecord				= pRecords->GetAt(i);
			CXTPReportRecordItem* pItem				= nullptr;
			CXTPReportRecordItemText* pItemTxt		= nullptr;
			CXTPReportRecordItemComboEx* pComboItem = nullptr;

			if (pRecord->GetItem(0)->IsChecked() == FALSE)
			{
				bCheckAll = false;
			}
		}

		if (bCheckAll == true)
		{
			CheckDlgButton(IDC_CHECK1, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK1, FALSE);
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


void KInterPathGeneratorMainDlg::OnBnClickedButtonDefault()
{
	try
	{
		UpdateReportDataUrban();
		UpdateReportDataRegion();

		InitUrbanModeDefine();
		InitRegionalModeDefine();

		//AfxMessageBox(_T("초기화되었습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AfxMessageBox(KModelRunMassage::MAG_ERROR_COMMAND);
		}
		TxLogDebugException();
	}
}

void KInterPathGeneratorMainDlg::DefaultUrbanModeData( void )
{
	TUrbanPathInfo oBusInfo;
	oBusInfo.nMode			= P_MODE_BUS;
	oBusInfo.strMode		= _T("Bus_Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oBusInfo.strMode		= _T("버스");
	}
	oBusInfo.nMaxofPath	= 10;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_BUS, oBusInfo));

	TUrbanPathInfo oSubwayInfo;
	oSubwayInfo.nMode			= P_MODE_RAIL;
	oSubwayInfo.strMode			= _T("Rail_Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oSubwayInfo.strMode		= _T("도시철도");
	}
	oSubwayInfo.nMaxofPath		= 10;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_RAIL, oSubwayInfo));

	TUrbanPathInfo oBSInfo;
	oBSInfo.nMode			= P_MODE_BUS_RAIL;
	oBSInfo.strMode			= _T("Composit_Mode");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oBSInfo.strMode		= _T("버스+도시철도");
	}
	oBSInfo.nMaxofPath		= 10;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_BUS_RAIL, oBSInfo));

	TUrbanPathInfo oPRInfo;
	oPRInfo.nMode			= P_MODE_PARK_RIDE;
	oPRInfo.strMode			= _T("Park & Ride");
	oPRInfo.nMaxofPath		= 5;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_PARK_RIDE, oPRInfo));

	TUrbanPathInfo oKRInfo;
	oKRInfo.nMode			= P_MODE_KISS_RIDE;
	oKRInfo.strMode			= _T("Kiss & Ride");
	oKRInfo.nMaxofPath		= 5;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_KISS_RIDE, oKRInfo));

	TUrbanPathInfo oTTInfo;
	oTTInfo.nMode			= P_MODE_TAXI_TRANSIT;
	oTTInfo.strMode			= _T("Taxi & Transit");
	if (KmzSystem::GetLanguage() == KEMKorea) {
		oTTInfo.strMode		= _T("택시 & 대중교통");
	}
	oTTInfo.nMaxofPath		= 5;
	m_mapUrbanModePath.insert(std::make_pair(P_MODE_TAXI_TRANSIT, oTTInfo));
}

void KInterPathGeneratorMainDlg::InitUrbanPathParameter( void )
{
	std::map<int, KPathTimeSetting> mapRegionalPathTimeSetting;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalPathSetting(mapRegionalPathTimeSetting);

	KPathTimeSetting oPathTimeSetting;

	AutoType bIter = mapRegionalPathTimeSetting.begin();
	AutoType eIter = mapRegionalPathTimeSetting.end();
	AutoType fIter = mapRegionalPathTimeSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		oPathTimeSetting = fIter->second;
	}
	else
	{
		fIter = mapRegionalPathTimeSetting.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			oPathTimeSetting = fIter->second;
		}
		else
		{
			TxLogDebugException();
			oPathTimeSetting.dTaxiWaitingTime    = 5.0;
			oPathTimeSetting.dHighwayTransitTime = 0.2;
			oPathTimeSetting.dRailTransitTime    = 0.5;
			oPathTimeSetting.dInterValueofTime = 254.22;
		}
	}

	m_oUrbanParamter.dDWTimeTaxi        = oPathTimeSetting.dTaxiWaitingTime;
	m_oUrbanParamter.dDWTimeBusstop     = oPathTimeSetting.dHighwayTransitTime;
	m_oUrbanParamter.dDwTimeRailStation = oPathTimeSetting.dRailTransitTime;
	m_oUrbanParamter.dInterValueofTime  = oPathTimeSetting.dInterValueofTime;
}

void KInterPathGeneratorMainDlg::InitUrbanConstraintCondition( void )
{
	std::map<int, KPathGenerateConstraint> mapDefaultPathGenerateConstraint;
	KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(mapDefaultPathGenerateConstraint);

	KPathGenerateConstraint oPathGenerateConstraint;

	AutoType bIter = mapDefaultPathGenerateConstraint.begin();
	AutoType eIter = mapDefaultPathGenerateConstraint.end();
	AutoType fIter = mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		oPathGenerateConstraint = fIter->second;
	}
	else
	{
		fIter = mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			oPathGenerateConstraint = fIter->second;
		}
		else
		{
			TxLogDebugException();
			oPathGenerateConstraint.nInvehicleTime = 1;
			oPathGenerateConstraint.nWatingTime    = 1;
			oPathGenerateConstraint.nTransferTime  = 1;
			oPathGenerateConstraint.nAETime        = 1;
			oPathGenerateConstraint.dPathTime      = 0.15;
			oPathGenerateConstraint.dPathLength    = 1.5;
		}
	}

	m_oUrbanParamter.nInvehicle         = oPathGenerateConstraint.nInvehicleTime;
	m_oUrbanParamter.nWating            = oPathGenerateConstraint.nWatingTime;
	m_oUrbanParamter.nTransfer          = oPathGenerateConstraint.nTransferTime;
	m_oUrbanParamter.nAETime            = oPathGenerateConstraint.nAETime;
	m_oUrbanParamter.dFirstPathTime     = oPathGenerateConstraint.dPathTime;
	m_oUrbanParamter.dSecondePathLength = oPathGenerateConstraint.dPathLength;
}

void KInterPathGeneratorMainDlg::InitUrbanAutoCost( void )
{
	m_vecUrbanAutoCost.clear();

	//passenger car fuel cost
	std::map<int, std::vector<TInterAutoCost>> mapUrbanAutoCost;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanAutoCost(mapUrbanAutoCost);

	AutoType bIter = mapUrbanAutoCost.begin();
	AutoType eIter = mapUrbanAutoCost.end();
	AutoType fIter = mapUrbanAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		m_vecUrbanAutoCost = fIter->second;
	}
	else
	{
		fIter = mapUrbanAutoCost.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			m_vecUrbanAutoCost = fIter->second;
		}
		else
		{
			TxLogDebugException();

			TInterAutoCost oGasoline;
			oGasoline.nType			= P_FUEL_GASOLINE;
			oGasoline.strName		= P_GASOLINE_NAME;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oGasoline.strName		= P_GASOLINE_NAME_K;
			}
			oGasoline.dPrice		= 2000.0;
			oGasoline.dRation		= 0.5;
			oGasoline.dfule			= 8.00;
			m_vecUrbanAutoCost.push_back(oGasoline);

			TInterAutoCost oDiesel;
			oDiesel.nType			= P_FUEL_DIESEL;
			oDiesel.strName		= P_DIESEL_NAME;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oDiesel.strName		= P_DIESEL_NAME_K;
			}
			oDiesel.dPrice		= 1700.0;
			oDiesel.dRation		= 0.35;
			oDiesel.dfule			= 12.00;
			m_vecUrbanAutoCost.push_back(oDiesel);

			TInterAutoCost oLPG;
			oLPG.nType			= P_FUEL_LPG;
			oLPG.strName		= P_LPG_NAME;
			oLPG.dPrice			= 1200.0;
			oLPG.dRation		= 0.15;
			oLPG.dfule			= 6.00;
			m_vecUrbanAutoCost.push_back(oLPG);
		}
	}
}

void KInterPathGeneratorMainDlg::InitUrbanTransitCost( void )
{
	m_vecUrbanTaxiCost.clear();

	std::map<int, std::vector<TInterTaxiCost>> mapUrbanTransitCost;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterUrbanTransitCost(mapUrbanTransitCost);

	AutoType bIter = mapUrbanTransitCost.begin();
	AutoType eIter = mapUrbanTransitCost.end();
	AutoType fIter = mapUrbanTransitCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		m_vecUrbanTaxiCost = fIter->second;
	}
	else
	{
        fIter = mapUrbanTransitCost.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

        if (fIter != eIter)
        {
            m_vecUrbanTaxiCost = fIter->second;
        }
        else
        {
            TInterTaxiCost oTaxiLPG;
            oTaxiLPG.nType				= P_FUEL_LPG;
            oTaxiLPG.strName			= P_LPG_NAME;
            oTaxiLPG.dBasefare			= 2300.0;
            oTaxiLPG.dBaseDist			= 2.00;
            oTaxiLPG.dAddFare			= 100;
            oTaxiLPG.dAddDist			= 0.15;
            m_vecUrbanTaxiCost.push_back(oTaxiLPG);
        }
	}	
}

void KInterPathGeneratorMainDlg::InitRegionalConstraintCondition( void )
{
	std::map<int, KPathGenerateConstraint> mapDefaultPathGenerateConstraint;
	KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(mapDefaultPathGenerateConstraint);

	KPathGenerateConstraint oPathGenerateConstraint;

	AutoType bIter = mapDefaultPathGenerateConstraint.begin();
	AutoType eIter = mapDefaultPathGenerateConstraint.end();
	AutoType fIter = mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		oPathGenerateConstraint = fIter->second;
	}
	else
	{
		fIter = mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			oPathGenerateConstraint = fIter->second;
		}
		else
		{
			TxLogDebugException();
			oPathGenerateConstraint.nInvehicleTime = 1;
			oPathGenerateConstraint.nWatingTime    = 1;
			oPathGenerateConstraint.nTransferTime  = 1;
			oPathGenerateConstraint.nAETime        = 1;
			oPathGenerateConstraint.dPathTime      = 0.15;
			oPathGenerateConstraint.dPathCost      = 1.5;
		}
	}

	m_oRegionalParameter.nInvehicle         = oPathGenerateConstraint.nInvehicleTime;
	m_oRegionalParameter.nWating            = oPathGenerateConstraint.nWatingTime;
	m_oRegionalParameter.nTransfer          = oPathGenerateConstraint.nTransferTime;
	m_oRegionalParameter.nAETime            = oPathGenerateConstraint.nAETime;
	m_oRegionalParameter.dFirstPathTime	    = oPathGenerateConstraint.dPathTime;
	m_oRegionalParameter.dSecondePathLength	= oPathGenerateConstraint.dPathCost;
}

void KInterPathGeneratorMainDlg::InitRegionalAutoCost( void )
{	
	m_vecRegionalAutoCost.clear();

	std::map<int, std::vector<TInterAutoCost>> mapRegionalAutoCost;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalAutoCost(mapRegionalAutoCost);

	AutoType bIter = mapRegionalAutoCost.begin();
	AutoType eIter = mapRegionalAutoCost.end();
	AutoType fIter = mapRegionalAutoCost.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		m_vecRegionalAutoCost = fIter->second;
	}
	else
	{
		fIter = mapRegionalAutoCost.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			m_vecRegionalAutoCost = fIter->second;
		}
		else
		{
			TxLogDebugException();
			
			TInterAutoCost oGasoline;
			oGasoline.nType   = P_FUEL_GASOLINE;
			oGasoline.strName = P_GASOLINE_NAME;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oGasoline.strName		= P_GASOLINE_NAME_K;
			}
			oGasoline.dPrice  = 2000.0;
			oGasoline.dRation = 0.5;
			oGasoline.dfule   = 8.00;
			m_vecRegionalAutoCost.push_back(oGasoline);

			TInterAutoCost oDiesel;
			oDiesel.nType   = P_FUEL_DIESEL;
			oDiesel.strName = P_DIESEL_NAME;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				oDiesel.strName		= P_DIESEL_NAME_K;
			}
			oDiesel.dPrice  = 1700.0;
			oDiesel.dRation = 0.35;
			oDiesel.dfule   = 12.00;
			m_vecRegionalAutoCost.push_back(oDiesel);

			TInterAutoCost oLPG;
			oLPG.nType   = P_FUEL_LPG;
			oLPG.strName = P_LPG_NAME;
			oLPG.dPrice  = 1200.0;
			oLPG.dRation = 0.15;
			oLPG.dfule   = 6.00;
			m_vecRegionalAutoCost.push_back(oLPG);
		}
	}
}

void KInterPathGeneratorMainDlg::InitRegionalTerminalProcessTime( void )
{
	m_mapRegionalProcessTime.clear();

	std::map<int, std::vector<TTerminalProcessTime>> mapRegionalTerminalTime;
	KDBaseDefaultParaInterPathGenerator::LoadDefaultInterRegionalTerminalTime(mapRegionalTerminalTime);

	std::vector<TTerminalProcessTime> vecTerminalTime;

	AutoType bIter = mapRegionalTerminalTime.begin();
	AutoType eIter = mapRegionalTerminalTime.end();
	AutoType fIter = mapRegionalTerminalTime.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

	if (fIter != eIter)
	{
		vecTerminalTime = fIter->second;
	}
	else
	{
		fIter = mapRegionalTerminalTime.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

		if (fIter != eIter)
		{
			vecTerminalTime = fIter->second;
		}
		else
		{
			TxLogDebugException();
		}
	}

	std::map<int/*nModeID*/, TTerminalProcessTime> mapTerminalProcessTime;

	int nSize = vecTerminalTime.size();

	for (int i = 0; i < nSize; ++i)
	{
		TTerminalProcessTime oTerminalProcessTime = vecTerminalTime[i];
		mapTerminalProcessTime.insert(std::make_pair(oTerminalProcessTime.nModeID, oTerminalProcessTime));		
	}
	
	std::map<int, int> mapTerminalMatching;
	KDBaseInterModal::GetInterRegionModeDefine(m_pTarget, mapTerminalMatching);

	KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
	KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
	std::map<int,CString> mapRegionMode;
	pCodeGroup->GetCodes(mapRegionMode);

	AutoType iter = mapRegionMode.begin();
	AutoType end  = mapRegionMode.end();
	
	while(iter != end)
	{
		TTerminalProcessTime oInfo;
		oInfo.nModeID		= iter->first;
		oInfo.strModeName	= iter->second;

		AutoType find = mapTerminalMatching.find(iter->first);
		AutoType fend = mapTerminalMatching.end();

		if (find == fend)
		{	//unmatching mapTerminalMatching
			oInfo.dAccess		= 20;
			oInfo.dEgress		= 10;
			oInfo.nModeTransfer = 0;
		}
		else
		{
			std::map<int, TTerminalProcessTime>::iterator fIterDefault, eIterDefault = mapTerminalProcessTime.end();

			if (eIterDefault != (fIterDefault = mapTerminalProcessTime.find(oInfo.nModeID)))
			{
				TTerminalProcessTime oTerminalProcessTime = fIterDefault->second;

				oInfo.dAccess		= oTerminalProcessTime.dAccess;
				oInfo.dEgress		= oTerminalProcessTime.dEgress;
				oInfo.nModeTransfer	= oTerminalProcessTime.nModeTransfer;
			}
			else
			{
				if (find->second == BUS_TRMINAL)
				{
					oInfo.dAccess = 15;
					oInfo.dEgress = 5;
					oInfo.nModeTransfer = 0;
				}
				else if (find->second == INTER_REGIONAL_RAILSTATION)
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
					oInfo.nModeTransfer = 0;
				}
				else if (find->second == AIR_TERMIANL)
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
					oInfo.nModeTransfer = 0;
				}
				else if (find->second == MARIN_PORT)
				{
					oInfo.dAccess = 30;
					oInfo.dEgress = 20;
					oInfo.nModeTransfer = 0;
				}
				else
				{
					oInfo.dAccess = 20;
					oInfo.dEgress = 10;
					oInfo.nModeTransfer = 0;
				}
			}
		}
		m_mapRegionalProcessTime.insert(std::make_pair(oInfo.nModeID, oInfo));
		++iter;
	}
}

void KInterPathGeneratorMainDlg::SaveLinkOption( KDBaseConPtr spDBaseConnection )
{
	CString strSQL(_T(""));

	try
	{
		strSQL.Format(_T("Delete From inter_link_option Where type = 0"));
		spDBaseConnection->ExecuteUpdate(strSQL);

		CString strColumnName(_T(""));
		if (m_pLinkTimeField != nullptr)
		{
			strColumnName = m_pLinkTimeField->Name();
		}

		strSQL.Format(_T("Insert Into inter_link_option ( type,  Travel_Time ) Values ( '0', '%s' )"), strColumnName);
		spDBaseConnection->ExecuteUpdate(strSQL);

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
