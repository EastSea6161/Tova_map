// ODMatrixDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Target.h"
#include "ODMatrixDlg.h"
#include "afxdialogex.h"

#include "IOTableRecord.h"
#include "DBaseMatrixView.h"
#include "KMatrixTableRecord.h"
#include "InformationDlg.h"

const int		DISP_TP_MATRIX = 0;
const int		DISP_TP_VECTOR = 1;

const int		REPORTCTL_COLUMN_WIDTH = 70;

const CString	CAPTION_ITEM_PASSENGER	= _T("Select Column :");
const CString	CAPTION_ITEM_FREIGHT	= _T("품목선택 :");

const CString	MSG_NO_PURPOSE	= _T("선택 가능한 목적이 없습니다.");
const CString	MSG_NO_PRODUCT	= _T("선택 가능한 품목이 없습니다.");
const CString	MSG_NO_DATA		= _T("표출 가능한 데이터가 없습니다.");
const CString	MSG_PROGRESS	= _T("데이터 검색 중입니다...");

const CString	HEADER_ORIGIN = _T("Origin");
const CString	HEADER_DESTINATION = _T("Destination");
const CString	HEADER_SUM = _T("Sum");

const CString	WHOLE_ITEM = _T("전체");

IMPLEMENT_DYNCREATE(KODMatrixDlg, KResizeDialogEx)

KODMatrixDlg::KODMatrixDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KODMatrixDlg::IDD, pParent)
	, m_nTableType(0)
	, m_pTarget(NULL)
	, m_pTable(NULL)
	, m_pVectorRecord(NULL)
	, m_pRecordArray(NULL)
	, m_pMatrixRecord(NULL)
	, m_vecOZoneID(NULL)
	, m_vecDZoneID(NULL)
	, m_strOZoneGroup(_T(""))
	, m_strDZoneGroup(_T(""))
{
}


KODMatrixDlg::KODMatrixDlg(KTarget* a_pTarget, KIOTable* a_pTable, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KODMatrixDlg::IDD, pParent)
	, m_nTableType(0)
	, m_pTarget(a_pTarget)
	, m_pTable(a_pTable)
	, m_pVectorRecord(NULL)
	, m_pRecordArray(NULL)
	, m_pMatrixRecord(NULL)
	, m_vecOZoneID(NULL)
	, m_vecDZoneID(NULL)
	, m_strOZoneGroup(_T(""))
	, m_strDZoneGroup(_T(""))
{
}


KODMatrixDlg::~KODMatrixDlg()
{
	if(NULL != m_pRecordArray)
	{
		delete m_pRecordArray;
	}
	m_pRecordArray = NULL;
}


void KODMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Radio  (pDX, IDC_RADIO1, m_nTableType);
	DDX_Control(pDX, IDC_COMBO1, m_comboPurposeByVector);
	DDX_Control(pDX, IDC_COMBO2, m_comboPurposeByMatrix);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_COMBO3, m_comboOZoneGroup);
	DDX_Control(pDX, IDC_COMBO4, m_comboDZoneGroup);
	DDX_Control(pDX, IDC_COMBO5, m_cboStatistics);
}


BEGIN_MESSAGE_MAP(KODMatrixDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,       &KODMatrixDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &KODMatrixDlg::OnBnClickedRadioType)
	ON_BN_CLICKED(IDC_RADIO2, &KODMatrixDlg::OnBnClickedRadioType)
	ON_BN_CLICKED(IDCANCEL,   &KODMatrixDlg::OnBnClickedCancel)
    ON_NOTIFY(NM_RCLICK, IDC_REPORT, &KODMatrixDlg::OnReportRowRButtonClick)
    ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT, &KODMatrixDlg::OnReportColumnRButtonClick)
    ON_COMMAND(ID_MENU_STATISTICS,     &KODMatrixDlg::OnMenuStatistics)
    ON_COMMAND(ID_MENU_STATISTICSZERO, &KODMatrixDlg::OnMenuStatisticsIncludeZero)
	ON_BN_CLICKED(IDC_BUTTON1, &KODMatrixDlg::OnBnClickedExport)
END_MESSAGE_MAP()


void KODMatrixDlg::OnBnClickedRadioType()
{
	UpdateData();

	if(m_nTableType == DISP_TP_VECTOR)
	{
		m_comboPurposeByVector.ShowWindow(TRUE);
		m_comboPurposeByMatrix.ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_STATS)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO5)->ShowWindow(SW_HIDE);
	}
	else if(m_nTableType == DISP_TP_MATRIX)
	{
		m_comboPurposeByMatrix.ShowWindow(TRUE);
		m_comboPurposeByVector.ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_STATS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO5)->ShowWindow(SW_SHOW);
	}
}


// KODMatrixDlg 메시지 처리기입니다.
BOOL KODMatrixDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetResize(IDC_REPORT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_SEPERATOR, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);    
    SetResize(IDC_BUTTON1, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_RECORDS, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);

	InitGroupCombo();
	InitStatisticsCombo();

	if (KmzSystem::IsPassengerSystem())
		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetDlgItemText(IDC_STATIC_ITEM, _T("컬럼 선택 :"));
		}
		else {
			SetDlgItemText(IDC_STATIC_ITEM, CAPTION_ITEM_PASSENGER);
		}
	else
		SetDlgItemText(IDC_STATIC_ITEM, CAPTION_ITEM_FREIGHT);

	OnBnClickedRadioType();

	KReportCtrlSetting::Default(m_wndReport, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AutoColumnSizeFalse(m_wndReport);

	return TRUE;
}


void KODMatrixDlg::Initialize()
{
	InitPurposeCombo();

	CString strInfoText(_T(""));

	if (KmzSystem::GetLanguage() == KEMKorea) {
        strInfoText.Format(_T("시나리오 : %s      분석 년도 : %s      테이블 명칭 : %s")
			,m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear(), m_pTable->Caption());
    }
	else {
		strInfoText.Format(_T("Scenario : %s     Target Year : %s     OD : %s")
			,m_pTarget->CaptionScenario(), m_pTarget->CaptionTargetYear(), m_pTable->Caption());
	}
	
	SetDlgItemText(IDC_STATIC_INFO, strInfoText);

	m_wndReport.ResetContent();
	m_wndReport.GetColumns()->Clear();
	m_wndReport.Populate();
}


void KODMatrixDlg::InitPurposeCombo(void)
{
	m_comboPurposeByMatrix.ResetContent();
	m_comboPurposeByVector.ResetContent();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_comboPurposeByVector.AddString(WHOLE_ITEM);
	}
	else {
		m_comboPurposeByVector.AddString(_T("All"));
	}
	

	const KIOColumns* pColumnCollection = m_pTable->Columns();
	int nColumnCnt = pColumnCollection->ColumnCount();

	for (int i = 0; i < nColumnCnt; ++i)
	{
		KIOColumn* pColumn = pColumnCollection->GetColumn(i);
		if(!pColumn->PrimaryKey())
		{
			m_comboPurposeByVector.AddString(pColumn->Caption());
			m_comboPurposeByVector.SetItemData(m_comboPurposeByVector.GetCount() - 1, (DWORD_PTR)pColumn);
			
			m_comboPurposeByMatrix.AddString(pColumn->Caption());
			m_comboPurposeByMatrix.SetItemData(m_comboPurposeByMatrix.GetCount() - 1, (DWORD_PTR)pColumn);
		}
	}

	m_comboPurposeByVector.SetCurSel(0);
	m_comboPurposeByMatrix.SetCurSel(0);
}


void KODMatrixDlg::InitGroupCombo(void)
{
	KIOTable* pZoneTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
	const KIOColumns *pColumns = pZoneTable->Columns();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		m_comboOZoneGroup.AddString(WHOLE_ITEM);
		m_comboDZoneGroup.AddString(WHOLE_ITEM);
	}
	else {
		m_comboOZoneGroup.AddString(_T("All"));
		m_comboDZoneGroup.AddString(_T("All"));
	}
	
	int nColumnCnt = pColumns->ColumnCount();
	for (int i = 0; i < nColumnCnt; ++i)
	{
		KIOColumn *pColumn = pColumns->GetColumn(i);
		if ( KEMIOCodeTypeIsSingleCode == pColumn->CodeType() )
		{
			int nIndex = m_comboOZoneGroup.AddString( pColumn->Caption() );
			m_comboOZoneGroup.SetItemData(nIndex, (DWORD_PTR)pColumn);

			nIndex = m_comboDZoneGroup.AddString( pColumn->Caption() );
			m_comboDZoneGroup.SetItemData(nIndex, (DWORD_PTR)pColumn);
		}
	}
	m_comboOZoneGroup.SetCurSel(0);
	m_comboDZoneGroup.SetCurSel(0);
}


void KODMatrixDlg::OnBnClickedOk()
{
	TxLogDebugStartMsg();

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang);

		if (m_bThreadOK)
		{
			if(m_nTableType == DISP_TP_VECTOR)
			{
				UpdateVectorReportControl();
			}
			else if(m_nTableType == DISP_TP_MATRIX)
			{
				UpdateMatrixReportControl();
			}

			UpdateResultInfo();

			GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
	}

	TxLogDebugEndMsg();
}


unsigned __stdcall KODMatrixDlg::ThreadCaller( void* p )
{
    try
    {
        QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
        QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

        KODMatrixDlg* pODMatrixDlg = (KODMatrixDlg*)pParameter->GetParameter();
        pODMatrixDlg->Run();
    }
    catch (KExceptionPtr ex)
    {
        TxExceptionPrint(ex);
    }
    catch (...)
    {
        TxLogDebugException();
    }

    return 0;
}


void KODMatrixDlg::Run()
{
	m_bThreadOK = false;

	try
	{
		m_wndReport.GetColumns()->Clear();
		m_wndReport.ResetContent();

		if( !m_vecOZoneID.empty() )
			m_vecOZoneID.clear();

		if( !m_vecDZoneID.empty() )
			m_vecDZoneID.clear();

		m_strOZoneGroup	= GetColumnTextOZoneGroup();
		m_strDZoneGroup	= GetColumnTextDZoneGroup();

		KDBaseMatrixView::GetListZoneGroup(m_pTarget, m_strOZoneGroup, m_vecOZoneID);

		if (m_vecOZoneID.size() == 0)
		{
			KIOColumn* pColumn = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());

			if (pColumn == nullptr) {
				if (KmzSystem::GetLanguage() == KEMKorea) {
					ThrowException(_T("기점 그룹화에 실패 하였습니다."));
				}
				else {
					ThrowException(_T("Failed to grouping Origins."));
				}
			}
				//ThrowException(_T("기점 그룹화에 실패 하였습니다."));

			CString strMSG;
			strMSG.Format(_T("%s 정보가 없습니다."), pColumn->Caption());

			ThrowException(strMSG);
		}

		KDBaseMatrixView::GetListZoneGroup(m_pTarget, m_strDZoneGroup, m_vecDZoneID);

		if (m_vecDZoneID.size() == 0)
		{
			KIOColumn* pColumn = (KIOColumn*)m_comboDZoneGroup.GetItemData(m_comboDZoneGroup.GetCurSel());

			if (pColumn == nullptr) {
				if (KmzSystem::GetLanguage() == KEMKorea) {
					ThrowException(_T("종점 그룹화에 실패 하였습니다."));
				}
				else {
					ThrowException(_T("Failed to grouping Destinations."));
				}
			}
				//ThrowException(_T("종점 그룹화에 실패 하였습니다."));

			CString strMSG;
			strMSG.Format(_T("%s 정보가 없습니다."), pColumn->Caption());

			ThrowException(strMSG);
		}

		KDBaseMatrixView::GetZoneName(m_pTarget, m_mapZoneName);

		if(m_nTableType == DISP_TP_VECTOR)
		{
			InitVectorReportControl();
			GetVectorData();
		}
		else if(m_nTableType == DISP_TP_MATRIX)
		{
			InitMatrixReportControl();
			GetMatrixData();
		}

		m_bThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		m_bThreadOK = false;
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		m_bThreadOK = false;
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
	}
}


void KODMatrixDlg::UpdateResultInfo( void )
{
	CString strResultText(_T(""));

	if(DISP_TP_MATRIX == m_nTableType)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strResultText.Format(_T("기종점 개수 : ( 기점 - %d / 종점 - %d )"), m_vecOZoneID.size(), m_vecDZoneID.size());
		}
		else {
			strResultText.Format(_T("Records : ( Origin - %d / Destination - %d )"), m_vecOZoneID.size(), m_vecDZoneID.size());
		}
	}
	else if (DISP_TP_VECTOR == m_nTableType)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strResultText.Format(_T("기종점 개수 : %d ( 기점 - %d / 종점 - %d )"), m_pRecordArray->GetRecordCount(), m_vecOZoneID.size(), m_vecDZoneID.size());
		}
		else {
			strResultText.Format(_T("Records : %d ( Origin - %d / Destination - %d )"), m_pRecordArray->GetRecordCount(), m_vecOZoneID.size(), m_vecDZoneID.size());
		}
	}
	SetDlgItemText(IDC_STATIC_RECORDS, strResultText);
}


void KODMatrixDlg::InitVectorReportControl( void )
{
	TxLogDebugStartMsg();

	const KIOColumns* pColumnCollection = m_pTable->Columns();	

	//Fixed Column
	CXTPReportColumn* pReportColumn = new CXTPReportColumn(0, HEADER_ORIGIN, REPORTCTL_COLUMN_WIDTH);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetAutoSize(TRUE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pReportColumn->SetCaption(_T("기점"));
    }
	
	pReportColumn = new CXTPReportColumn(1, HEADER_DESTINATION, REPORTCTL_COLUMN_WIDTH);
	pReportColumn = m_wndReport.AddColumn(pReportColumn);
	pReportColumn->SetAutoSize(TRUE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        pReportColumn->SetCaption(_T("종점"));
    }
	
	KIOColumn* pColumn = (KIOColumn*)m_comboPurposeByVector.GetItemData( m_comboPurposeByVector.GetCurSel() );
	int nColumnCnt = pColumnCollection->ColumnCount();

	if( pColumn == NULL )
	{
		for (int i = 2; i < nColumnCnt; ++i)
		{
			KIOColumn* pColumn = pColumnCollection->GetColumn(i);

			pReportColumn = new CXTPReportColumn(i, pColumn->Caption(), REPORTCTL_COLUMN_WIDTH);
			pReportColumn = m_wndReport.AddColumn(pReportColumn);

			pReportColumn->SetInternalName(pColumn->Name());
			pReportColumn->SetBestFitMode(xtpColumnBestFitModeAllData);
			pReportColumn->SetAutoSize(TRUE);
			pReportColumn->SetHeaderAlignment(DT_CENTER);
			pReportColumn->SetAlignment(DT_RIGHT);
		}
	}
	else
	{
		pReportColumn = new CXTPReportColumn(2, pColumn->Caption(), REPORTCTL_COLUMN_WIDTH);
		pReportColumn = m_wndReport.AddColumn(pReportColumn);

		pReportColumn->SetInternalName(pColumn->Name());
		pReportColumn->SetAutoSize(TRUE);
		pReportColumn->SetHeaderAlignment(DT_CENTER);
		pReportColumn->SetAlignment(DT_RIGHT);
	}

	TxLogDebugEndMsg();
}

void KODMatrixDlg::GetVectorData( void )
{
	CXTPReportColumns* pReportColumns = m_wndReport.GetColumns();

	if(NULL != m_pRecordArray)
	{
		delete m_pRecordArray;
		m_pRecordArray = NULL;
	}

	m_pRecordArray = new KSDoubleRecordArray( pReportColumns->GetCount() );

	KDBaseMatrixView::ODVectorRecord(m_pTable, pReportColumns, m_strOZoneGroup, m_strDZoneGroup, *m_pRecordArray);

	if ( m_comboPurposeByVector.GetCount() < 2)
	{
		if (KmzSystem::IsPassengerSystem()) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(MSG_NO_PURPOSE);
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no selectable purpose."));
			}
		}
		else
			ThrowException(MSG_NO_PRODUCT);
	}

	if (0 == m_pRecordArray->GetRecordCount()) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(MSG_NO_DATA);
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			ThrowException(_T("There is no displayable data."));
		}
	}
		//ThrowException(MSG_NO_DATA);
}

void KODMatrixDlg::UpdateVectorReportControl( void )
{
	KCodeManager*	pCodeManager= NULL;
	KIOTableOwner*	pOwner		= m_pTable->GetOwner();
	
	if(NULL != pOwner)
	{
		pCodeManager = pOwner->GetCodeManager();
	}

	KIOColumn* pColumnOZoneGroup = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());
	KIOColumn* pColumnDZoneGroup = (KIOColumn*)m_comboDZoneGroup.GetItemData(m_comboDZoneGroup.GetCurSel());

	m_pVectorRecord = new KODVectorTableRecord(pCodeManager, m_pRecordArray, pColumnOZoneGroup, pColumnDZoneGroup, m_mapZoneName);
	int nTotalRowCount = m_pRecordArray->GetRecordCount() + 1;	// TotalRecordCount + 합계를 위한 RowCount(1)
	
    m_wndReport.SetVirtualMode(m_pVectorRecord, nTotalRowCount);
	m_wndReport.Populate();
}


void KODMatrixDlg::InitMatrixReportControl( void )
{
	TxLogDebugStartMsg();
	CXTPReportColumn* pReportColumn = nullptr;

	int nColumnIndex(0);
	
	//O/D
	pReportColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, _T("O/D"), 100));
	pReportColumn->SetAutoSize(FALSE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	pReportColumn->SetSortable(FALSE);

	//Statistics
	CString strStatistics;
	m_cboStatistics.GetLBText(m_cboStatistics.GetCurSel(), strStatistics);

	pReportColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, strStatistics, 100));
	pReportColumn->SetAutoSize(FALSE);
	pReportColumn->SetHeaderAlignment(DT_CENTER);
	pReportColumn->SetAlignment(DT_CENTER);
	pReportColumn->SetSortable(FALSE);
		
	KIOColumn* pColumnDZoneGroup = (KIOColumn*)m_comboDZoneGroup.GetItemData(m_comboDZoneGroup.GetCurSel());

	KCodeManager* pCodeManager = NULL;
	KIOTableOwner* pOwner = m_pTable->GetOwner();

	if ( NULL != pOwner)
	{
		pCodeManager = pOwner->GetCodeManager();
	}

	std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();

	int nSizeVecZone = m_vecDZoneID.size();

	if ( NULL == pColumnDZoneGroup || NULL == pCodeManager )
	{
		for (int i = 0; i < nSizeVecZone; ++i)
		{
			CString strColName;

			fIter = m_mapZoneName.find(m_vecDZoneID[i]);

			if (fIter == eIter)
			{
				strColName.Format(_T("%I64d"), m_vecDZoneID[i]);
			}
			else
			{
				strColName.Format(_T("%I64d:%s"), m_vecDZoneID[i], fIter->second);
			}
			
			pReportColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, strColName, REPORTCTL_COLUMN_WIDTH));
			pReportColumn->SetAutoSize(FALSE);
			pReportColumn->SetHeaderAlignment(DT_CENTER);
			pReportColumn->SetAlignment(DT_RIGHT);
		}
	}
	else
	{
		KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pColumnDZoneGroup->CodeGroup());

		for (int i = 0; i < nSizeVecZone; i++)
		{
			CString strColName;
			
			if( KEMIOCodeTypeIsSingleCode == pColumnDZoneGroup->CodeType() && NULL != pCodeGroup)
			{
				strColName = pCodeGroup->SingleCodeValue((int)m_vecDZoneID[i], 1);
			}
			else if( KEMIOCodeTypeIsSingleCode == pColumnDZoneGroup->CodeType() && NULL != pCodeGroup)
			{
				strColName.Format(_T("%I64d"), m_vecDZoneID[i]);
				strColName = pCodeGroup->MultiCodeValue(strColName, 1);
			}
			else
			{
				fIter = m_mapZoneName.find(m_vecDZoneID[i]);

				if (fIter == eIter)
				{
					strColName.Format(_T("%I64d"), m_vecDZoneID[i]);
				}
				else
				{
					strColName.Format(_T("%I64d:%s"), m_vecDZoneID[i], fIter->second);
				}				
			}

			pReportColumn = m_wndReport.AddColumn(new CXTPReportColumn(nColumnIndex++, strColName, REPORTCTL_COLUMN_WIDTH));
			pReportColumn->SetHeaderAlignment(DT_CENTER);
			pReportColumn->SetAlignment(DT_RIGHT);
		}
	}
	
	m_wndReport.SetFreezeColumnsCount(1);
	m_wndReport.Populate();
	TxLogDebugEndMsg();
}

void KODMatrixDlg::GetMatrixData( void )
{
	KIOColumn* pColumn = (KIOColumn*)m_comboPurposeByMatrix.GetItemData(m_comboPurposeByMatrix.GetCurSel());
	int nComboCount = m_comboPurposeByMatrix.GetCount();
	int nCursorIDX = m_comboPurposeByMatrix.GetCurSel();

	if ( m_comboPurposeByMatrix.GetCount() < 1)
	{
		if (KmzSystem::IsPassengerSystem()) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ThrowException(MSG_NO_PURPOSE);
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ThrowException(_T("There is no selectable purpose."));
			}
		}
			//ThrowException(MSG_NO_PURPOSE);
		else
			ThrowException(MSG_NO_PRODUCT);
	}

	KDBaseMatrixView::ODMatrixRecord(m_pTable, m_strOZoneGroup, m_strDZoneGroup, pColumn->Name(), m_mapMatrixData);

	if (0 == m_mapMatrixData.size()) {
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ThrowException(MSG_NO_DATA);
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			ThrowException(_T("There is no displayable data."));
		}
	}
		//ThrowException(MSG_NO_DATA);
}

void KODMatrixDlg::UpdateMatrixReportControl( void )
{
	int nOZoneCount = m_vecOZoneID.size();
	int nDZoneCount = m_vecDZoneID.size();

	//OZone 기준 Sum
	KSDoubleRecord oRecordSum_Oi(nOZoneCount + 1);	// 1은 전체의 합을 저장하기 위한 추가 Index
	KSDoubleRecord oRecordSum_Di(nDZoneCount + 1);

	CString strStatistics;
	m_cboStatistics.GetLBText(m_cboStatistics.GetCurSel(), strStatistics);

	if (strStatistics.CompareNoCase(_T("Sum")) == 0)
	{
		GetStatisticsSum_Oi(oRecordSum_Oi);
		GetStatisticsSum_Di(oRecordSum_Di);
	}
	else if (strStatistics.CompareNoCase(_T("Min")) == 0)
	{
		GetStatisticsMin_Oi(oRecordSum_Oi);
		GetStatisticsMin_Di(oRecordSum_Di);
	}
	else if (strStatistics.CompareNoCase(_T("Max")) == 0)
	{
		GetStatisticsMax_Oi(oRecordSum_Oi);
		GetStatisticsMax_Di(oRecordSum_Di);
	}
	else if (strStatistics.CompareNoCase(_T("Avg")) == 0)
	{
		GetStatisticsAvg_Oi(oRecordSum_Oi);
		GetStatisticsAvg_Di(oRecordSum_Di);
	}

	KIOTableOwner*  pOwner       = m_pTable->GetOwner();
	KCodeManager* pCodeManager = NULL;

	if ( NULL != pOwner )
		pCodeManager = pOwner->GetCodeManager();

	KIOColumn* pColumnOZoneGroup = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());

	m_pMatrixRecord = new KODMatrixTableRecord(
		pCodeManager, m_vecOZoneID, m_vecDZoneID, m_mapMatrixData, pColumnOZoneGroup, 
		oRecordSum_Oi, oRecordSum_Di, strStatistics, m_mapZoneName);

	int nTotalRowCount = m_vecOZoneID.size() + 1;	// TotalRecordCount + 합계를 위한 RowCount(1)

	m_wndReport.SetVirtualMode(m_pMatrixRecord, nTotalRowCount);
	m_wndReport.Populate();
}

CString KODMatrixDlg::GetColumnTextOZoneGroup( void )
{
	CString strGroup(_T(""));
	KIOColumn* pColumn = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());
	if(NULL == pColumn)
	{
		strGroup = COLUMN_ZONE_ID;
	}
	else
	{
		strGroup = pColumn->Name();
	}
	return strGroup;
}


CString KODMatrixDlg::GetColumnTextDZoneGroup( void )
{
	CString strGroup(_T(""));
	KIOColumn* pColumn = (KIOColumn*)m_comboDZoneGroup.GetItemData(m_comboDZoneGroup.GetCurSel());
	if(NULL == pColumn)
	{
		strGroup = COLUMN_ZONE_ID;
	}
	else
	{
		strGroup = pColumn->Name();
	}
	return strGroup;
}


void KODMatrixDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//DestroyWindow();
	KResizeDialogEx::OnCancel();
}


void KODMatrixDlg::PostNcDestroy()
{
// 	KResizeDialogEx::PostNcDestroy();
// 	delete this;
}

void KODMatrixDlg::OnReportColumnRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    try
    {
        if (!m_bThreadOK)
            return;

        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;

        CXTPReportColumn* pClickedColumn = pClickedItem->pColumn;
        if (pClickedColumn == nullptr)
            return;

        int nColumnIndex = pClickedColumn->GetItemIndex();
        
		if (nColumnIndex == 1)
            return;

		if (nColumnIndex == 0)
		{
			m_nSelectedMethod = 2;
		}
		else
		{
			m_nSelectedMethod = 1;
		}        

		m_nSelectedRowIndex    = -1;
		m_nSelectedColumnIndex = nColumnIndex;

		CMenu menu;
		menu.LoadMenu( IDR_POPUP_ODMATRIX );    
		CMenu* pPopup = menu.GetSubMenu(0);

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

void KODMatrixDlg::OnReportRowRButtonClick( NMHDR* pNMHDR, LRESULT* pResult )
{
    try
    {
        if (!m_bThreadOK)
            return;

        XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*) pNMHDR;

        if (!pClickedItem->pRow || !pClickedItem->pColumn)
            return;

        int nRowIndex    = pClickedItem->pRow->GetIndex();
        int nColumnIndex = pClickedItem->pColumn->GetItemIndex();

        if (nColumnIndex != 0 || nRowIndex == 0)
            return;
 

        m_nSelectedMethod      = 0;
        m_nSelectedRowIndex    = nRowIndex;
        m_nSelectedColumnIndex = -1;

        CMenu menu;
        menu.LoadMenu( IDR_POPUP_ODMATRIX );    
        CMenu* pPopup = menu.GetSubMenu(0);

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

void KODMatrixDlg::OnMenuStatistics()
{
    try
    {
        CWaitCursor wc;
        std::vector<double> vecValue;
        if (m_nSelectedMethod == 0)
        {
            if ((int)m_vecOZoneID.size() < m_nSelectedRowIndex)
            {
                return;
            }

            Integer nxOZoneID = m_vecOZoneID[m_nSelectedRowIndex - 1];

            std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
            std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

            while (iter != end)
            {
                KODKey oKey = iter->first;
                if (oKey.OriginID == nxOZoneID)
                {
                    double dValue = iter->second;
                    if (dValue != 0.0)
                    {
                        vecValue.push_back(dValue);
                    }
                }
                ++iter;
            }
        }
        else if (m_nSelectedMethod == 1)
        {
            if ((int)m_vecDZoneID.size() + 1 < m_nSelectedColumnIndex)
            {
                return;
            }

            Integer nxDZoneID = m_vecDZoneID[m_nSelectedColumnIndex - 2];

            std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
            std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

            while (iter != end)
            {
                KODKey oKey = iter->first;
                if (oKey.DestinationID == nxDZoneID)
                {
                    double dValue = iter->second;
                    if (dValue != 0.0)
                    {
                        vecValue.push_back(dValue);
                    }
                }
                ++iter;
            }
        }
		else if (m_nSelectedMethod == 2) // 전체 통계
		{
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
			std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

			while (iter != end)
			{
				double dValue = iter->second;

				if (dValue != 0.0)
					vecValue.push_back(dValue);

				++iter;
			}
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
}

void KODMatrixDlg::OnMenuStatisticsIncludeZero()
{
    try
    {
        CWaitCursor wc;
        std::vector<double> vecValue;
        if (m_nSelectedMethod == 0)
        {
            if ((int)m_vecOZoneID.size() < m_nSelectedRowIndex)
            {
                return;
            }

            Integer nxOZoneID = m_vecOZoneID[m_nSelectedRowIndex - 1];

            std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
            std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

            while (iter != end)
            {
                KODKey oKey = iter->first;
                if (oKey.OriginID == nxOZoneID)
                {
                    double dValue = iter->second;
                    vecValue.push_back(dValue);
                }
                ++iter;
            }
        }
        else if (m_nSelectedMethod == 1)
        {
            if ((int)m_vecDZoneID.size() + 1 < m_nSelectedColumnIndex)
            {
                return;
            }

            Integer nxDZoneID = m_vecDZoneID[m_nSelectedColumnIndex - 2];

            std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
            std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

            while (iter != end)
            {
                KODKey oKey = iter->first;
                if (oKey.DestinationID == nxDZoneID)
                {
                    double dValue = iter->second;
                    vecValue.push_back(dValue);
                }
                ++iter;
            }
        }
		else if (m_nSelectedMethod == 2) // 전체 통계
		{
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.begin();
			std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

			while (iter != end)
			{
				vecValue.push_back(iter->second);

				++iter;
			}
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
}

void KODMatrixDlg::InitStatisticsCombo( void )
{
	m_cboStatistics.ResetContent();

	int nIndex(0);
		
	nIndex = m_cboStatistics.InsertString(-1, _T("Sum"));
	nIndex = m_cboStatistics.InsertString(-1, _T("Avg"));
	nIndex = m_cboStatistics.InsertString(-1, _T("Min"));
	nIndex = m_cboStatistics.InsertString(-1, _T("Max"));

	m_cboStatistics.SetCurSel(0);
}

void KODMatrixDlg::GetStatisticsSum_Oi( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueSumTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nOZoneCounts; ++i)
	{
		double dValSum_Oi(0.0);
		for (int j = 0; j < nDZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[i], m_vecDZoneID[j]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
			dValSum_Oi += iter->second;
		}
		a_oRecordSum_Oi.SetAt((i + 1), dValSum_Oi);
		dValueSumTotal += dValSum_Oi;
	}

	a_oRecordSum_Oi.SetAt(0, dValueSumTotal);
}

void KODMatrixDlg::GetStatisticsSum_Di( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueSumTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nDZoneCounts; ++i)
	{
		double dValSum_Di(0.0);
		for (int j = 0; j < nOZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[j], m_vecDZoneID[i]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
			dValSum_Di += iter->second;
		}
		a_oRecordSum_Oi.SetAt((i + 1), dValSum_Di);
		dValueSumTotal += dValSum_Di;
	}

	a_oRecordSum_Oi.SetAt(0, dValueSumTotal);
}

void KODMatrixDlg::GetStatisticsAvg_Oi( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueAvgTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nOZoneCounts; ++i)
	{
		double dValAvg_Oi(0.0);
		for (int j = 0; j < nDZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[i], m_vecDZoneID[j]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
			dValAvg_Oi += iter->second;
		}
		a_oRecordSum_Oi.SetAt((i + 1), (dValAvg_Oi / nDZoneCounts));
		dValueAvgTotal += dValAvg_Oi;
	}

	a_oRecordSum_Oi.SetAt(0, (dValueAvgTotal / nOZoneCounts));
}

void KODMatrixDlg::GetStatisticsAvg_Di( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueAvgTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nDZoneCounts; ++i)
	{
		double dValAvg_Di(0.0);
		for (int j = 0; j < nOZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[j], m_vecDZoneID[i]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
			dValAvg_Di += iter->second;
		}
		a_oRecordSum_Oi.SetAt((i + 1), (dValAvg_Di / nOZoneCounts));
		dValueAvgTotal += dValAvg_Di;
	}

	a_oRecordSum_Oi.SetAt(0, (dValueAvgTotal / nDZoneCounts));
}

void KODMatrixDlg::GetStatisticsMin_Oi( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueMinTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nOZoneCounts; ++i)
	{
		double dValMin_Oi(0.0);
		for (int j = 0; j < nDZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[i], m_vecDZoneID[j]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );

			if (j == 0)
			{
				dValMin_Oi = iter->second;
			}
			else
			{
				if (dValMin_Oi > iter->second)
					dValMin_Oi = iter->second;
			}
		}

		a_oRecordSum_Oi.SetAt((i + 1), dValMin_Oi);

		if (i == 0)
		{
			dValueMinTotal = dValMin_Oi;
		}
		else
		{
			if (dValueMinTotal > dValMin_Oi)
				dValueMinTotal = dValMin_Oi;
		}
	}

	a_oRecordSum_Oi.SetAt(0, dValueMinTotal);
}

void KODMatrixDlg::GetStatisticsMin_Di( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueMinTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nDZoneCounts; ++i)
	{
		double dValMin_Di(0.0);

		for (int j = 0; j < nOZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[j], m_vecDZoneID[i]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );

			if (j == 0)
			{
				dValMin_Di = iter->second;
			}
			else
			{
				if (dValMin_Di > iter->second)
					dValMin_Di = iter->second;
			}
		}
		
		a_oRecordSum_Oi.SetAt((i + 1), dValMin_Di);

		if (i == 0)
		{
			dValueMinTotal = dValMin_Di;
		}
		else
		{
			if (dValueMinTotal > dValMin_Di)
				dValueMinTotal = dValMin_Di;
		}
	}

	a_oRecordSum_Oi.SetAt(0, dValueMinTotal);
}

void KODMatrixDlg::GetStatisticsMax_Oi( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueMaxTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nOZoneCounts; ++i)
	{
		double dValMax_Oi(0.0);
		for (int j = 0; j < nDZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[i], m_vecDZoneID[j]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );

			if (j == 0)
			{
				dValMax_Oi = iter->second;
			}
			else
			{
				if (dValMax_Oi < iter->second)
					dValMax_Oi = iter->second;
			}
		}

		a_oRecordSum_Oi.SetAt((i + 1), dValMax_Oi);

		if (i == 0)
		{
			dValueMaxTotal = dValMax_Oi;
		}
		else
		{
			if (dValueMaxTotal < dValMax_Oi)
				dValueMaxTotal = dValMax_Oi;
		}
	}

	a_oRecordSum_Oi.SetAt(0, dValueMaxTotal);
}

void KODMatrixDlg::GetStatisticsMax_Di( KSDoubleRecord& a_oRecordSum_Oi )
{
	double dValueMaxTotal(0.0);

	int nOZoneCounts = m_vecOZoneID.size();
	int nDZoneCounts = m_vecDZoneID.size();

	for (int i = 0; i < nDZoneCounts; ++i)
	{
		double dValMax_Di(0.0);

		for (int j = 0; j < nOZoneCounts; ++j)
		{
			KODKey oMatrixOD(m_vecOZoneID[j], m_vecDZoneID[i]);
			std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );

			if (j == 0)
			{
				dValMax_Di = iter->second;
			}
			else
			{
				if (dValMax_Di < iter->second)
					dValMax_Di = iter->second;
			}
		}

		a_oRecordSum_Oi.SetAt((i + 1), dValMax_Di);

		if (i == 0)
		{
			dValueMaxTotal = dValMax_Di;
		}
		else
		{
			if (dValueMaxTotal < dValMax_Di)
				dValueMaxTotal = dValMax_Di;
		}
	}

	a_oRecordSum_Oi.SetAt(0, dValueMaxTotal);
}

void KODMatrixDlg::OnBnClickedExport()
{
	try
	{
		CFileDialog fileDlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST, _T("(*.txt)|*txt|"), NULL);

		fileDlg.m_ofn.lpstrTitle = _T("Export Data");
		//fileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;

		m_strExpFileName = _T("");

		if (fileDlg.DoModal() != IDOK)
			return;

		CString strFileName = fileDlg.GetPathName();
		strFileName.Replace(_T(".txt"), _T(""));
		m_strExpFileName = strFileName + _T(".txt");
		
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ExportThreadCaller, this, false, nLang);

		if (m_bExportThreadOK) {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));
			}
			else {
				AfxMessageBox(_T("Export complete."));
			}
		}
			//AfxMessageBox(_T("데이터 내보내기를 완료하였습니다."));

	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
	}
}

unsigned __stdcall KODMatrixDlg::ExportThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KODMatrixDlg* pODMatrixDlg = (KODMatrixDlg*)pParameter->GetParameter();
	pODMatrixDlg->Export();

	return 0;
}

void KODMatrixDlg::Export( void )
{
	m_bExportThreadOK = false;
	
	UINT         nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx outFile(m_strExpFileName, nOpenMode);

	try
	{
		CString strWriteLine(_T(""));
		
		CXTPReportColumns* pColumns = m_wndReport.GetColumns();
		int                nColumns = pColumns->GetCount();

		for (int i = 0; i < nColumns; ++i)
		{
			strWriteLine.AppendFormat(_T("\"%s\", "), pColumns->GetAt(i)->GetCaption());
		}

		strWriteLine.Truncate(strWriteLine.GetLength() - 2);
		strWriteLine.AppendFormat(_T("\r\n"));

		outFile.WriteString(strWriteLine);

		if(m_nTableType == DISP_TP_VECTOR)
		{
			ExportVector(outFile);		
		}
		else if(m_nTableType == DISP_TP_MATRIX)
		{
			ExportMatrix(outFile);

			
		}

		outFile.Close();
		m_bExportThreadOK = true;
	}
	catch (KExceptionPtr ex)
	{
		outFile.Close();
		m_bExportThreadOK = false;
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		outFile.Close();
		m_bExportThreadOK = false;
		//AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
		}
		else {
			AfxMessageBox(_T("An unexpected error has occurred. \ nPlease contact the administrator."));
		}
	}
}

void KODMatrixDlg::ExportVector( CStdioFileEx& a_outFile )
{
	CString strWriteLine;
	std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();

	KCodeManager*	pCodeManager= NULL;
	KIOTableOwner*	pOwner		= m_pTable->GetOwner();

	if(NULL != pOwner)
	{
		pCodeManager = pOwner->GetCodeManager();
	}

	KIOColumn* pColumnOZoneGroup = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());
	KIOColumn* pColumnDZoneGroup = (KIOColumn*)m_comboDZoneGroup.GetItemData(m_comboDZoneGroup.GetCurSel());

	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int                nRecords = pRecords->GetCount();
	int                nColumn  = m_wndReport.GetColumns()->GetCount();

	for (int i = 0; i < nRecords - 1; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);

		strWriteLine = _T("");

		for (int j = 0; j < nColumn; ++j)
		{
			KSDoubleRecord* pDoubleRecord = m_pRecordArray->GetRecordAt(i);
			double dValue = pDoubleRecord->GetAt(j);

			if (j == 0)//기점
			{
				if( NULL != pColumnOZoneGroup && (pColumnOZoneGroup->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != pCodeManager) )
				{
					KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pColumnOZoneGroup->CodeGroup());

					if(NULL != pCodeGroup)
					{
						strWriteLine.AppendFormat(_T("\"%s\", "), pCodeGroup->SingleCodeValue((int)dValue, 1));
					}
					else
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
				}
				else if( NULL != pColumnOZoneGroup && (pColumnOZoneGroup->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != pCodeManager) )
				{
					KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(pColumnOZoneGroup->CodeGroup());
					if(NULL != pCodeData)
					{
						CString strValue(_T(""));
						strValue.Format(_T("%d"), (int)dValue);
						strWriteLine.AppendFormat(_T("\"%s\", "), pCodeData->MultiCodeValue(strValue, 1));
					}
					else
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
				}
				else
				{
					fIter = m_mapZoneName.find((Integer)dValue);

					if (fIter == eIter)
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
					else
					{
						strWriteLine.AppendFormat(_T("\"%I64d:%s\", "), (Integer)dValue, fIter->second);
					}
				}
			}
			else if (j == 1)//종점
			{
				if( NULL != pColumnDZoneGroup && (pColumnDZoneGroup->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != pCodeManager) )
				{
					KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pColumnDZoneGroup->CodeGroup());
					if(NULL != pCodeGroup)
					{
						strWriteLine.AppendFormat(_T("\"%s\", "), pCodeGroup->SingleCodeValue((int)dValue, 1));
					}
					else
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
				}
				else if( NULL != pColumnDZoneGroup && (pColumnDZoneGroup->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != pCodeManager) )
				{
					KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(pColumnDZoneGroup->CodeGroup());
					if(NULL != pCodeData)
					{
						CString strValue(_T(""));
						strValue.Format(_T("%d"), (int)dValue);
						strWriteLine.AppendFormat(_T("\"%s\", "), pCodeData->MultiCodeValue(strValue, 1));
					}
					else
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
				}
				else
				{
					fIter = m_mapZoneName.find((Integer)dValue);

					if (fIter == eIter)
					{
						strWriteLine.AppendFormat(_T("%I64d, "), (Integer)dValue);
					}
					else
					{
						strWriteLine.AppendFormat(_T("\"%I64d:%s\", "), (Integer)dValue, fIter->second);
					}
				}
			}
			else //Data
			{
				strWriteLine.AppendFormat(_T("%.5f, "), dValue);
			}
		}

		strWriteLine.Truncate(strWriteLine.GetLength() - 2);
		strWriteLine.Append(_T("\r\n"));

		a_outFile.WriteString(strWriteLine);
	}

	// add sum record;
	KSDoubleRecord* pSumRecord = m_pVectorRecord->m_pRecordSum;
	strWriteLine = _T("");

	for (int i = 0; i < nColumn; ++i)
	{
		if (i == 0 || i == 1)
		{
			strWriteLine.Append(_T(", "));
		}
		else
		{
			double dValue = pSumRecord->GetAt(i);
			strWriteLine.AppendFormat(_T("%.5f, "), dValue);
		}
	}

	strWriteLine.Truncate(strWriteLine.GetLength() - 2);
	strWriteLine.Append(_T("\r\n"));

	a_outFile.WriteString(strWriteLine);
}

void KODMatrixDlg::ExportMatrix( CStdioFileEx& a_outFile )
{
	CString strWriteLine, strStatistics;
	m_cboStatistics.GetLBText(m_cboStatistics.GetCurSel(), strStatistics);

	std::map<Integer, CString>::iterator fIter, eIter = m_mapZoneName.end();

	KCodeManager*	pCodeManager = NULL;
	KIOTableOwner*	pOwner		 = m_pTable->GetOwner();

	if(NULL != pOwner)
	{
		pCodeManager = pOwner->GetCodeManager();
	}

	KIOColumn* pColumnOZoneGroup = (KIOColumn*)m_comboOZoneGroup.GetItemData(m_comboOZoneGroup.GetCurSel());

	int nMatrixColumns = m_vecDZoneID.size() + 2;
	int nMatirxRecords = m_vecOZoneID.size() + 1;

	for (int i = 0; i < nMatirxRecords; ++i)
	{
		strWriteLine = _T("");

		for (int j = 0; j < nMatrixColumns; ++j)
		{
			if (i == 0 && j == 0)
			{
				strWriteLine.AppendFormat(_T("\"%s\", "), strStatistics);
			}
			else
			{
				if (j == 0)//Dzone
				{
					if( NULL != pColumnOZoneGroup && (pColumnOZoneGroup->CodeType() == KEMIOCodeTypeIsSingleCode) && (NULL != pCodeManager) )
					{
						KCodeGroup* pCodeGroup = pCodeManager->FindCodeGroup(pColumnOZoneGroup->CodeGroup());
						if(NULL != pCodeGroup)
						{
							strWriteLine.AppendFormat(_T("\"%s\", "), pCodeGroup->SingleCodeValue((int)m_vecOZoneID[i -1], 1));
						}
						else
						{
							strWriteLine.AppendFormat(_T("%I64d, "), m_vecOZoneID[i -1]);
						}
					}
					else if( NULL != pColumnOZoneGroup && (pColumnOZoneGroup->CodeType() == KEMIOCodeTypeIsMultiCode) && (NULL != pCodeManager) )
					{
						KCodeGroup* pCodeData = pCodeManager->FindCodeGroup(pColumnOZoneGroup->CodeGroup());
						if(NULL != pCodeData)
						{
							CString strValue(_T(""));
							strValue.Format(_T("%I64d"), m_vecOZoneID[i - 1]);
							strWriteLine.AppendFormat(_T("\"%s\", "), pCodeData->MultiCodeValue(strValue, 1));
						}
						else
						{
							strWriteLine.AppendFormat(_T("%I64d, "), m_vecOZoneID[i - 1]);
						}
					}
					else
					{
						fIter = m_mapZoneName.find(m_vecOZoneID[i -1]);

						if (fIter == eIter)
						{
							strWriteLine.AppendFormat(_T("%I64d, "), m_vecOZoneID[i - 1]);
						}
						else
						{
							strWriteLine.AppendFormat(_T("\"%I64d:%s\", "), m_vecOZoneID[i - 1], fIter->second);
						}
					}					
				}
				else if (j == 1)
				{
					KSDoubleRecord* pRecord = m_pMatrixRecord->m_pRecordSum_Oi;
					strWriteLine.AppendFormat(_T("%.5f, "), pRecord->GetAt(i));
				}
				else
				{
					if (i == 0)
					{
						KSDoubleRecord* pRecord = m_pMatrixRecord->m_pRecordSum_Di;
						strWriteLine.AppendFormat(_T("%.5f, "), pRecord->GetAt(j - 1));
					}
					else
					{
						KODKey oMatrixOD( m_vecOZoneID[i - 1], m_vecDZoneID[j - 2]);

						std::map<KODKey, double>::iterator iter = m_mapMatrixData.find( oMatrixOD );
						std::map<KODKey, double>::iterator end  = m_mapMatrixData.end();

						if (iter == end)
						{
							strWriteLine.AppendFormat(_T("%.5f, "), 0.0);
						}
						else
						{
							strWriteLine.AppendFormat(_T("%.5f, "), iter->second);
						}
					}
				}
			}
		}

		strWriteLine.Truncate(strWriteLine.GetLength() - 2);
		strWriteLine.Append(_T("\r\n"));

		a_outFile.WriteString(strWriteLine);
	}
}
