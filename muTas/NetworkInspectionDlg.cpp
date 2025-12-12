// NetworkInspectionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NetworkInspectionDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Target.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "Launcher.h"
#include "WriteCodeAgrument.h"
#include "ImChampDir.h"
#include "NetworkEditLog.h"
#include "DBaseInterModal.h"
#include "BulkDBaseTurn.h"

enum
{
	_0_A_COLUMN_LINKTYPE     = 0,
	_1_A_COLUMN_HIGHWAY      = 1,
	_2_A_COLUMN_EXPRESSWAY   = 2,
	_3_A_COLUMN_RAMP         = 3
};

enum
{
	_0_B_COLUMN_BUSTYPE       = 0,
	_1_B_COLUMN_SHUTTLE       = 1,
	_2_B_COLUMN_INTRACITY     = 2,
	_3_B_COLUMN_RED           = 3,
	_4_B_COLUMN_OTHER         = 4
};

enum
{
	INSPECTION_MODE_HIGHWAY  = 0,
	INSPECTION_MODE_TRANSIT  = 1,
	INSPECTION_MODE_ALL      = 2
};

enum
{
	HIGHWAY_INSPECTION_TYPE_HIGHWAY    = 1,
	HIGHWAY_INSPECTION_TYPE_EXPRESSWAY = 7,
	HIGHWAY_INSPECTION_TYPE_RAMP       = 8
};

enum
{
	TRANSIT_INSPECTION_TYPE_SHUTTLE    = 0,
	TRANSIT_INSPECTION_TYPE_INTRACITY  = 1,
	TRANSIT_INSPECTION_TYPE_RED        = 2,
	TRANSIT_INSPECTION_TYPE_OTHER      = 3
};

// KNetworkInspectionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KNetworkInspectionDlg, KResizeDialogEx)

	KNetworkInspectionDlg::KNetworkInspectionDlg(KProject* a_pProject, KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KNetworkInspectionDlg::IDD, pParent)
	, m_pProject(a_pProject)
	, m_pTarget(a_pTarget)
	, m_nResultCode(0)
	, m_bUseHighwayInspection(false)
	, m_bUseTransitInspection(false)
	, m_nExistError(-1)
    , m_bApplyTurn(false)
{    
    try
    {
        m_pLinkTable        = m_pTarget->Tables()->FindTable(TABLE_LINK);
        m_pTransitTable     = m_pTarget->Tables()->FindTable(TABLE_TRANSIT);
        m_pTransitLinkTable = m_pTarget->Tables()->FindTable(TABLE_TRANSITLINKS); //★ 필요

        m_pLinkTable->Register(this);
        m_pTransitTable->Register(this);
        m_pTransitLinkTable->Register(this);
    }
    catch (...)
    {
    	m_pLinkTable        = nullptr;
        m_pTransitTable     = nullptr;
        m_pTransitLinkTable = nullptr;
    }    
}

KNetworkInspectionDlg::~KNetworkInspectionDlg()
{
    if (m_pLinkTable != nullptr)
        m_pLinkTable->Unregister(this);

    if (m_pTransitTable != nullptr)
        m_pTransitTable->Unregister(this);

    if (m_pTransitLinkTable != nullptr)
        m_pTransitLinkTable->Unregister(this);
}

void KNetworkInspectionDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                 m_wndReportACtrl);
	DDX_Control(pDX, IDC_REPORT_B,                 m_wndReportBCtrl);
}


BEGIN_MESSAGE_MAP(KNetworkInspectionDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_CHECK_TRANSIT2,                       &KNetworkInspectionDlg::OnBnClickedCheckTransit)
	ON_BN_CLICKED(IDCANCEL,                                &KNetworkInspectionDlg::OnBnClickedCancel)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A,         &KNetworkInspectionDlg::OnReportACheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_B,         &KNetworkInspectionDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_BUTTON4, &KNetworkInspectionDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON5, &KNetworkInspectionDlg::OnBnClickedButtonHighwayError)
	ON_BN_CLICKED(IDC_BUTTON1, &KNetworkInspectionDlg::OnBnClickedButtonHighwayWarning)
	ON_BN_CLICKED(IDC_BUTTON3, &KNetworkInspectionDlg::OnBnClickedButtonTransitWarning)
	ON_BN_CLICKED(IDC_BUTTON2, &KNetworkInspectionDlg::OnBnClickedButtonTransitError)
	ON_WM_COPYDATA()
    ON_BN_CLICKED(IDC_CHECK_HIGHWAY, &KNetworkInspectionDlg::OnBnClickedCheckHighway)
END_MESSAGE_MAP()


void KNetworkInspectionDlg::NotifyProcess( LPCTSTR a_strSubjectName/*=_T("")*/, __int64 a_nxObjectID/*=0*/ )
{
    try
    {
    	Initialize();
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

BOOL KNetworkInspectionDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
    KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    KReportCtrlSetting::Default(m_wndReportACtrl, FALSE, FALSE, FALSE);
    KReportCtrlSetting::Default(m_wndReportBCtrl, FALSE, FALSE, FALSE);
    InitReportAHeader();
    InitReportBHeader();

    AddImChampTooltip(IDC_BUTTON5, _T("Highway Inspection 결과 \r\n 에러메시지 보기"));
    AddImChampTooltip(IDC_BUTTON1, _T("Highway Inspection 결과 \r\n 경고메시지 보기"));
    AddImChampTooltip(IDC_BUTTON3, _T("Transit Inspection 결과 \r\n 경고메시지 보기"));
    AddImChampTooltip(IDC_BUTTON2, _T("Transit Inspection 결과 \r\n 에러메시지 보기"));
    AddImChampTooltip(IDC_BUTTON4, _T("검사 시작"));

    Initialize();

    ResizeComponent();
    return TRUE;
}

void KNetworkInspectionDlg::Initialize()
{
	m_nExistError = -1;

    std::vector<TLinkTypeName>    vecTypeName;
    std::vector<TTransitTypeName> vecBusType;
    LoadUsedLinkType (vecTypeName);
    LoadUsedBusType  (vecBusType);
    UpdateReportAData(vecTypeName);
    UpdateReportBData(vecBusType);
    
    if (vecTypeName.size()< 1)
        CheckDlgButton(IDC_CHECK_HIGHWAY, BST_UNCHECKED);
    else
        CheckDlgButton(IDC_CHECK_HIGHWAY, BST_CHECKED);
    
    CheckControlHighway();

    int nBusCount(0), nRailCount(0);
    TransitCount(nBusCount, nRailCount);

    if (nBusCount > 0 || nRailCount > 0)
        CheckDlgButton(IDC_CHECK_TRANSIT2, BST_CHECKED);
    else
        CheckDlgButton(IDC_CHECK_TRANSIT2, BST_UNCHECKED);        

    /*if (vecBusType.size()< 1)
        CheckDlgButton(IDC_CHECK_TRANSIT2, BST_UNCHECKED);
    else
        CheckDlgButton(IDC_CHECK_TRANSIT2, BST_CHECKED);*/

 	if (IsExsitTransitData() == false || IsExsitTransitLineData() == false)
 	{
		CheckDlgButton(IDC_CHECK_TRANSIT2, BST_UNCHECKED);

		GetDlgItem(IDC_CHECK_TRANSIT2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE); // t.warning
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE); // t.error
		GetDlgItem(IDC_REPORT_B)->EnableWindow(FALSE); 
 	}
    else
    {// edited by nombara 2014. 09. 12: 
     // 최초 다이얼로그 로딩시 transit data가 없다가
     // transit data를 추가했을 경우 EnableWindow(FALSE)를 해제하기 위해
        GetDlgItem(IDC_CHECK_TRANSIT2)->EnableWindow(TRUE);
        GetDlgItem(IDC_STATIC1)->EnableWindow(TRUE);
        GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE); // t.warning
        GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE); // t.error
        GetDlgItem(IDC_REPORT_B)->EnableWindow(TRUE); 
    }
	
	if (CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_ERROR) == FALSE)
	{
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	}
	if (CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_WARNING) == FALSE)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
	if (CheckEmptyOutFile(T_DAT_OUT_TRANSIT_ERROR) == FALSE)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	if (CheckEmptyOutFile(T_DAT_OUT_TRANSIT_WARNING) == FALSE)
	{
		GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	}
}


BOOL KNetworkInspectionDlg::CheckEmptyOutFile(CString a_strFile)
{
	try
	{
		CString strFileName = ConvertSavePath(a_strFile);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFileName) == FALSE)
		{
			throw 1;
		}

		oFileFinder.FindNextFile();
		if (oFileFinder.GetLength() <= 0)
		{
			throw 1;
		}
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}

void KNetworkInspectionDlg::InitReportAHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_0_A_COLUMN_LINKTYPE,       _T("UserLinkType"),   37));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("링크 타입"));
	}

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_1_A_COLUMN_HIGHWAY,        _T("Highway"),        21));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("일반도로"));
	}

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_2_A_COLUMN_EXPRESSWAY,     _T("Expressway"),     21));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("고속도로"));
	}

	pColumn = m_wndReportACtrl.AddColumn(new CXTPReportColumn(_3_A_COLUMN_RAMP,           _T("Ex-Ramp"),        21));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("램프"));
	}

	m_wndReportACtrl.Populate();
}


void KNetworkInspectionDlg::InitReportBHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_0_B_COLUMN_BUSTYPE,   _T("UserBusType"),      22));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("버스 노선 타입"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_1_B_COLUMN_SHUTTLE,   _T("Local Bus"),        19));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("마을버스"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_2_B_COLUMN_INTRACITY, _T("City Bus"),         19));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("일반버스"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_3_B_COLUMN_RED,       _T("Regional Bus"),     21));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("광역버스"));
	}

	pColumn = m_wndReportBCtrl.AddColumn(new CXTPReportColumn(_4_B_COLUMN_OTHER,     _T("Other Bus"),        19));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("기타"));
	}

	m_wndReportBCtrl.Populate();
}


void KNetworkInspectionDlg::ResizeComponent()
{
	SetResize(IDC_REPORT_A,       			      SZ_TOP_LEFT,      SZ_MIDDLE_RIGHT);
	SetResize(IDC_SEPRATOR3,       			      SZ_MIDDLE_LEFT,   SZ_MIDDLE_RIGHT);

    SetResize(IDC_CHECK1,                         SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);

	SetResize(IDC_CHECK_TRANSIT2,       	      SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);
	SetResize(IDC_STATIC1,       			      SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);

	SetResize(IDC_REPORT_B,       			      SZ_MIDDLE_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR4,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_BUTTON5,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON1,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON3,       			      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,       		          SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON4,       			      SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,       			          SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


bool KNetworkInspectionDlg::IsExsitTransitData()
{
	try
	{
		int nTotalTransitCount = 0;
		KBulkDBaseTransit::TotalTransitCount( m_pTarget, nTotalTransitCount);

		if (nTotalTransitCount < 1)
		{
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


bool KNetworkInspectionDlg::IsExsitTransitLineData()
{
	try
	{
		int nTotalTransitLineCount = 0;
		KBulkDBaseTransit::TotalTransitLineCount( m_pTarget, nTotalTransitLineCount );

		if (nTotalTransitLineCount < 1)
		{
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


void KNetworkInspectionDlg::LoadUsedLinkType(std::vector<TLinkTypeName> &a_vecTypeName)
{
	KBulkDBaseLink::UsedLinkInspectionType(m_pTarget, a_vecTypeName);
}


void KNetworkInspectionDlg::LoadUsedBusType(std::vector<TTransitTypeName> &a_vecBusType)
{
	KBulkDBaseTransit::UsedTransitBusType(m_pTarget, a_vecBusType);
}


void KNetworkInspectionDlg::UpdateReportAData(std::vector<TLinkTypeName> &a_vecTypeName)
{
	m_wndReportACtrl.ResetContent();

	try
	{
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString               strLinkTypeName(_T(""));

		size_t nxCount = a_vecTypeName.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord = m_wndReportACtrl.AddRecord(new CXTPReportRecord());

			int     &nType       = a_vecTypeName[i].TType;
			CString &strTypeName = a_vecTypeName[i].TTypeName;

			if (strTypeName.Compare(_T("")) == 0)
				strLinkTypeName.Format(_T("%d:(x)"), nType);
			else
				strLinkTypeName.Format(_T("%d:%s"),  nType, strTypeName);
			
			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strLinkTypeName));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nType);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);
		}

		m_wndReportACtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 
}


void KNetworkInspectionDlg::UpdateReportBData(std::vector<TTransitTypeName> &a_vecBusType)
{
	m_wndReportBCtrl.ResetContent();

	try
	{
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		CString               strBusTypeName(_T(""));

		size_t nxCount = a_vecBusType.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord = m_wndReportBCtrl.AddRecord(new CXTPReportRecord());

			int     &nType       = a_vecBusType[i].nType;
			CString &strTypeName = a_vecBusType[i].strName;

			if (strTypeName.Compare(_T("")) == 0)
				strBusTypeName.Format(_T("%d:(x)"), nType);
			else
				strBusTypeName.Format(_T("%d:%s"),  nType, strTypeName);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strBusTypeName));
			pItem->SetAlignment(DT_LEFT);
			pItem->SetItemData((DWORD_PTR) nType);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(FALSE);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
		}

		m_wndReportBCtrl.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
	}
	catch (...)
	{
		TxLogDebugException();	
	} 
}


bool KNetworkInspectionDlg::InvalidateInputInfo()
{
	try
	{
		UINT nStatusCheckHighway = IsDlgButtonChecked(IDC_CHECK_HIGHWAY);
		UINT nStatusCheckTransit = IsDlgButtonChecked(IDC_CHECK_TRANSIT2);

        // 어떠한 것도 선택하지 않은 경우
		if (BST_UNCHECKED == nStatusCheckHighway && BST_UNCHECKED == nStatusCheckTransit)
		{
			AfxMessageBox(_T("네트워크 검사 항목을 선택해 주세요."));
			throw 1;
		}

        // 모두 선택
		if (BST_CHECKED == nStatusCheckHighway && BST_CHECKED == nStatusCheckTransit)
		{
			m_nInspectionMode = INSPECTION_MODE_ALL;
		}
		else
		{
			if (BST_CHECKED == nStatusCheckHighway)
			{
				m_nInspectionMode = INSPECTION_MODE_HIGHWAY;
			}
			else if (BST_CHECKED == nStatusCheckTransit)
			{
				m_nInspectionMode = INSPECTION_MODE_TRANSIT;
			}
		}

		MakeCheckedReportAData(m_mapDefineLinkType);
		MakeCheckedReportBData(m_mapDefineDetailBusType);

		if (INSPECTION_MODE_ALL == m_nInspectionMode || INSPECTION_MODE_HIGHWAY == m_nInspectionMode)
		{
			if (m_mapDefineLinkType.size() < 1)
			{
				AfxMessageBox(_T("도로망 데이터 검사를 위한 링크 타입 기능 구분을 설정해 주세요."));
				throw 1;
			}
		}

		if (INSPECTION_MODE_ALL == m_nInspectionMode || INSPECTION_MODE_TRANSIT == m_nInspectionMode)
		{
            #pragma region comment
            // Transit Inspection - 화면에서는 버스만 보인다(선택항목이 있기 때문에)
            // Rail 데이터만 존재하는 경우 Transit Inspection을 돌릴 수 없는 문제가 있음
            #pragma endregion comment

            int nBusCount(0), nRailCount(0);
            TransitCount(nBusCount, nRailCount);

			if (m_mapDefineDetailBusType.size() < 1 && nRailCount <= 0)
			{
				AfxMessageBox(_T("대중교통 관련 정보가 존재하지 않습니다."));
				throw 1;
			}
		}

		if (BST_UNCHECKED == nStatusCheckHighway) 
			m_bUseHighwayInspection = false;
		else
			m_bUseHighwayInspection = true;

		if (BST_UNCHECKED == nStatusCheckTransit)
			m_bUseTransitInspection = false;
		else
			m_bUseTransitInspection = true;
	}
	catch(...)
	{
		return false;
	}

	return true;
}


void KNetworkInspectionDlg::MakeCheckedReportAData(std::map<int, int> &a_mapDefineLinkType)
{
	a_mapDefineLinkType.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		for(int i= 0; i< nRecordCount; i++)
		{
			pRecord       = pRecords->GetAt(i);

			pItem         = pRecord->GetItem(_0_A_COLUMN_LINKTYPE);
			int nLinkType = (int)pItem->GetItemData();

			pItem         = pRecord->GetItem(_1_A_COLUMN_HIGHWAY);
			if (pItem->IsChecked() == TRUE)
			{
				a_mapDefineLinkType.insert(std::make_pair(nLinkType, HIGHWAY_INSPECTION_TYPE_HIGHWAY));
				continue;
			}

			pItem         = pRecord->GetItem(_2_A_COLUMN_EXPRESSWAY);
			if (pItem->IsChecked() == TRUE)
			{
				a_mapDefineLinkType.insert(std::make_pair(nLinkType, HIGHWAY_INSPECTION_TYPE_EXPRESSWAY));
				continue;
			}

			pItem         = pRecord->GetItem(_3_A_COLUMN_RAMP);
			if (pItem->IsChecked() == TRUE)
			{
				a_mapDefineLinkType.insert(std::make_pair(nLinkType, HIGHWAY_INSPECTION_TYPE_RAMP));
				continue;
			}
		}
	}
	catch (...)
	{
		a_mapDefineLinkType.clear();
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::MakeCheckedReportBData(std::map<int, int> &a_mapDefineDetailBusType)
{
	a_mapDefineDetailBusType.clear();

	try
	{
		CXTPReportRecords* pRecords = m_wndReportBCtrl.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		for(int i= 0; i< nRecordCount; i++)
		{
			pRecord      = pRecords->GetAt(i);

			pItem        = pRecord->GetItem(_0_B_COLUMN_BUSTYPE);
			int nBusType = (int)pItem->GetItemData();
			int nInspectionType(0);

			pItem        = pRecord->GetItem(_1_B_COLUMN_SHUTTLE);
			if (pItem->IsChecked() == TRUE)
			{
				nInspectionType = TRANSIT_INSPECTION_TYPE_SHUTTLE;
				a_mapDefineDetailBusType.insert(std::make_pair(nBusType, nInspectionType));
				continue;
			}

			pItem   = pRecord->GetItem(_2_B_COLUMN_INTRACITY);
			if (pItem->IsChecked() == TRUE)
			{
				nInspectionType = TRANSIT_INSPECTION_TYPE_INTRACITY;
				a_mapDefineDetailBusType.insert(std::make_pair(nBusType, nInspectionType));
				continue;
			}

			pItem   = pRecord->GetItem(_3_B_COLUMN_RED);
			if (pItem->IsChecked() == TRUE)
			{
				nInspectionType = TRANSIT_INSPECTION_TYPE_RED;
				a_mapDefineDetailBusType.insert(std::make_pair(nBusType, nInspectionType));
				continue;
			}

			pItem   = pRecord->GetItem(_4_B_COLUMN_OTHER);
			if (pItem->IsChecked() == TRUE)
			{
				nInspectionType = TRANSIT_INSPECTION_TYPE_OTHER;
				a_mapDefineDetailBusType.insert(std::make_pair(nBusType, nInspectionType));
				continue;
			}
		}
	}
	catch (...)
	{
		a_mapDefineDetailBusType.clear();
		TxLogDebugException();
	}
}

void KNetworkInspectionDlg::OnBnClickedButtonRun()
{
    if (!InvalidateInputInfo())
    {
        return;
    }

    m_bApplyTurn = false; {
        if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED) {
            m_bApplyTurn = true;
        }
        else {
            m_bApplyTurn = false;
        }
    }

    m_nResultCode = DLL_UNEXPECTED_ERROR;
    m_strErrMsg   = _T("");

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

    QBicListSimpleProgressThread::ExecuteThread(InspectionThreadCaller, this, true, nLang);

    if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
    {
        KNetworkEditLog::SetNetworkLogUpdateInspection(m_pTarget);
    }
    else if (DLL_USER_STOPPING == m_nResultCode)
    {
        //AfxMessageBox(_T("모형 실행을 중단하였습니다."));
    }
    else if (DLL_LOAD_ERROR == m_nResultCode)
    {
        //AfxMessageBox(_T("Not Found Dll"));
    }
    else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
    {
        if (m_strErrMsg.Compare(_T("")) == 0)
        {
            //AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
        }
        else
        {
            //AfxMessageBox(m_strErrMsg);
        }
    }
    else
    {
        //AfxMessageBox(_T("네트워크 검사 수행 중 오류가 발생하였습니다."));
    }
    AfterRunCheckEmptyFile();
}


void KNetworkInspectionDlg::AfterRunCheckEmptyFile()
{
    BOOL bErrorHighway(FALSE);
    BOOL bWarnginHighway(FALSE);
    BOOL bErrorTransit(FALSE);
    BOOL bWarnginTransit(FALSE);
    BOOL bEnable(FALSE);

    bErrorHighway = CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_ERROR);
    GetDlgItem(IDC_BUTTON5)->EnableWindow(bErrorHighway);

    bWarnginHighway = CheckEmptyOutFile(T_DAT_OUT_HIGHWAY_WARNING);
    GetDlgItem(IDC_BUTTON1)->EnableWindow(bWarnginHighway);

    if (IsExsitTransitData() == false || IsExsitTransitLineData() == false)
    {
        CheckDlgButton(IDC_CHECK_TRANSIT2, BST_UNCHECKED);

        GetDlgItem(IDC_CHECK_TRANSIT2)->EnableWindow(FALSE);
        GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE); // t.warning
        GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE); // t.error
        GetDlgItem(IDC_REPORT_B)->EnableWindow(FALSE); 
    }
    else
    {
        bErrorTransit = CheckEmptyOutFile(T_DAT_OUT_TRANSIT_ERROR);
        GetDlgItem(IDC_BUTTON2)->EnableWindow(bErrorTransit);

        bWarnginTransit = CheckEmptyOutFile(T_DAT_OUT_TRANSIT_WARNING);
        GetDlgItem(IDC_BUTTON3)->EnableWindow(bWarnginTransit);
    }

    if (bErrorHighway == TRUE || bErrorTransit == TRUE)
    {
        CString strMsg(_T(""));
        strMsg.Format(_T("오류 관련 버튼을 클릭하여 해당 오류 항목을 먼저 수정해 주세요.\r\n수정이 완료되지 않으면 모형을 실행할 수 없습니다."));
        AfxMessageBox(strMsg);
    }

    if (bErrorHighway == TRUE && bErrorTransit == TRUE)
    {
        m_nExistError = 3;
    }
    else if (bErrorHighway == TRUE )
    {
        m_nExistError = 1;
    }
    else if (bErrorTransit == TRUE)
    {
        m_nExistError = 2;
    }
    else
    {
        m_nExistError = 0;
    }
}


unsigned __stdcall KNetworkInspectionDlg::InspectionThreadCaller(void* p)
{
	QBicListSimpleProgressParameter* pParameter = (QBicListSimpleProgressParameter*)p;
	KNetworkInspectionDlg* pDlg = (KNetworkInspectionDlg*)pParameter->GetParameter();
	pDlg->Run();
	return 0;
}


void KNetworkInspectionDlg::Run()
{
	TxLogDebugStartMsg();

	AddStatusMessage(_T("> Creating Argument File..."));
	if (!CreateArgument())
	{
		AddStatusMessage(m_strErrMsg);
		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		return;
	}
	AddStatusMessage(_T("> Create Argument File End."));
	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> User Stopped..."));
		m_nResultCode = DLL_USER_STOPPING;
		return;
	}

	CString strWindowName(_T(""));
	GetWindowText(strWindowName);

	AddStatusMessage(_T("> Network Inspection Start..."));
	KLauncher::Run(strWindowName, 16);

	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> User Stopped..."));
		m_nResultCode = DLL_USER_STOPPING;
		return;
	}

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		AddStatusMessage(_T("> Network Inspection End."));
		TxLogDebug(_T("Network Inspection Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		AddStatusMessage(_T("> Network Inspection Stopped."));
		TxLogDebug(_T("Network Inspection Stop.."));

		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Network Inspection Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return;
	}
	else
	{
        CString strError(_T(""));
        strError.Format(_T("> Network Inspection Module End with Error... %d"), m_nResultCode);
		AddStatusMessage(strError);
		TxLogDebug((LPCTSTR)strError);

		return;
	}
	if (QBicListSimpleProgressThread::IsUserStopped() == true)
	{
		AddStatusMessage(_T("> User Stopped..."));
		m_nResultCode =  DLL_USER_STOPPING;
		return;
	}

	AddStatusMessage(_T("> Network Inspection Complete."));
	TxLogDebugEndMsg();
}


bool KNetworkInspectionDlg::CreateArgument()
{
	TxLogDebugStartMsg();

	ImChampDir::SetCurrentDirectoryToApp();

	try
	{
		if(! ClearFiles())
		{
			throw 1;
		}
		//TxLogDebug("END CLEAR FILE ------ ");

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(T_ARG_DATA_INSPECTION, nOpenMode);

		CString strDirectory;
		strDirectory.Format(_T("%s\\%s\\"),m_pTarget->GetLocation(), DLL_MODEL_FOLDER_NAME);
		CreateDirectory(strDirectory, NULL);

		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget);

		// .arg 작성
		if( !WriteDataInspectionArgument(of))
		{
			of.Close();
			ThrowException(_T("Argument 생성(WriteDataInspectionArgument) 중 오류가 발생하였습니다. !!!"));
		}
		of.Close();

		AddStatusMessage(_T("> Creating Input Files for Network Inspection Module..."));
		KIOColumns emptyColumns;
		if ( !KBulkDBase2File::ExportDB2LinkFileD(m_pTarget->GetDBaseConnection(), emptyColumns, ConvertSavePath(T_DAT_LINK) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_NODE) ))
		{
			ThrowException(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_TURN)))
		{
			ThrowException(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
		}
		
		if ( !KBulkDBase2File::ExportDB2LineFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_LINEINFO), ConvertSavePath(T_DAT_LINE)))
		{
			ThrowException(_T("Dat 생성(ExportDB2LineFile) 중 오류가 발생하였습니다. !!!"));
		}

		if ( !KDBaseInterModal::InterExportDB2TerminalScheulFile(m_pTarget->GetDBaseConnection(), ConvertSavePath(_T("TerminalSchedule.dat"))))
		{
			ThrowException(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebugException();
		m_strErrMsg = ex->GetErrorMessage();
		return false;
	}
	catch (...)
	{
		m_strErrMsg = _T("Argument 파일 생성 중 오류가 발생하였습니다.");
		TxLogDebugException();
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


bool KNetworkInspectionDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)T_ARG_DATA_INSPECTION);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINEINFO));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINE));

		// OUT file 삭제
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_HIGHWAY_WARNING));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_TRANSIT_ERROR));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_TRANSIT_WARNING));
		nResult = remove((CStringA)ConvertSavePath(T_TXT_OUT_DATAINSPECTION_LOG));
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KNetworkInspectionDlg::WriteDataInspectionArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		KObject* pScenario = m_pTarget->GetParentObject();

		strOut.Format(_T("%s_%s_%s\r\n"), m_pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
		of.WriteString(strOut);

		strOut.Format(_T("InspectionMode\t%d\r\n"), m_nInspectionMode);
		of.WriteString(strOut);

		int nTurn = 0;
		if (m_bApplyTurn)
			nTurn = 1;

		strOut.Format(_T("TurnRestriction\t%d\r\n"), nTurn);
		of.WriteString(strOut);

		if (true == m_bUseHighwayInspection && m_mapDefineLinkType.size() > 0)
		{
			strOut.Format(_T("DefineLinkType\t%d\r\n"), m_mapDefineLinkType.size());
			of.WriteString(strOut);

			AutoType iter  = m_mapDefineLinkType.begin();
			AutoType itEnd = m_mapDefineLinkType.end();

			while(iter != itEnd)
			{
				int nLinkType            = iter->first;
				int nHiwayInspectionType = iter->second;

				strOut.Format(_T("%d\t%d\r\n"), nLinkType, nHiwayInspectionType);
				of.WriteString(strOut);

				++iter;
			}
		}

		if (true == m_bUseTransitInspection /*&& m_mapDefineDetailBusType.size() > 0*/)
		{
			strOut.Format(_T("DefineDetailBusType\t%d\r\n"), m_mapDefineDetailBusType.size());
			of.WriteString(strOut);

			AutoType iter  = m_mapDefineDetailBusType.begin();
			AutoType itEnd = m_mapDefineDetailBusType.end();

			while(iter != itEnd)
			{
				int nBusType               = iter->first;
				int nTransitInspectionType = iter->second;

				strOut.Format(_T("%d\t%d\r\n"), nBusType, nTransitInspectionType);
				of.WriteString(strOut);

				++iter;
			}
		}

		CString strFolder = ConvertSavePath(_T(""));
		strOut.Format(_T("%s\r\n"), strFolder);
		of.WriteString(strOut);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


void KNetworkInspectionDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KNetworkInspectionDlg::AddStatusMessage(CString a_strMsg)
{
	QBicListSimpleProgressPtr spProgressWindow = QBicListSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


BOOL KNetworkInspectionDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	try
	{
		int     nMsgCode      = pCopyDataStruct->dwData;
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


void KNetworkInspectionDlg::OnBnClickedButtonHighwayError()
{
	try
	{
		ResultFileOpen(T_DAT_OUT_HIGHWAY_ERROR);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::OnBnClickedButtonHighwayWarning()
{
	try
	{
		ResultFileOpen(T_DAT_OUT_HIGHWAY_WARNING);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::OnBnClickedButtonTransitWarning()
{
	try
	{
		ResultFileOpen(T_DAT_OUT_TRANSIT_WARNING);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::OnBnClickedButtonTransitError()
{
	try
	{
		ResultFileOpen(T_DAT_OUT_TRANSIT_ERROR);
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::ResultFileOpen(CString a_strFile)
{
	try
	{
		CString strFileName = ConvertSavePath(a_strFile);

		CFileFind oFileFinder;
		if (oFileFinder.FindFile(strFileName) == FALSE)
		{
			CString strMsg(_T(""));
			strMsg.AppendFormat(_T("%s 파일이 존재 하지 않습니다.\r\n"), a_strFile);
			strMsg.AppendFormat(_T("경로:%s"), ConvertSavePath(_T("")));
			ThrowException(strMsg);
		}
		ShellExecute(NULL, _T("open"), strFileName, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


void KNetworkInspectionDlg::OnBnClickedCheckTransit()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_TRANSIT2) == BST_CHECKED)
		{
			bool bExistTransitData     = IsExsitTransitData();
			bool bExistTransitLineData = IsExsitTransitLineData();

			if (false == bExistTransitData || false == bExistTransitLineData)
			{
				CheckDlgButton(IDC_CHECK_TRANSIT2, BST_UNCHECKED);
				MsgNoExistTransitData(bExistTransitData, bExistTransitLineData);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KNetworkInspectionDlg::MsgNoExistTransitData(bool a_bExistTransitData, bool a_bExistTransitLineData)
{
	CString strMsg(_T(""));
	strMsg.Append(_T("대중교통 노선 데이터 검사를 실행할 수 없습니다.\r\n"));
	strMsg.Append(_T("아래 관련 테이블에 데이터가 존재하지 않습니다.\r\n"));

	if (false == a_bExistTransitData)
		strMsg.Append(_T("- 도시부 대중교통 노선운행 자료\r\n"));
	if (false == a_bExistTransitLineData)
		strMsg.Append(_T("- 도시부 대중교통 노선 경로"));

	AfxMessageBox(strMsg);
}


void KNetworkInspectionDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _1_A_COLUMN_HIGHWAY == nIndex || _2_A_COLUMN_EXPRESSWAY == nIndex || _3_A_COLUMN_RAMP == nIndex)
	{
		CXTPReportRecord*     pRecord      = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pClickItem   = pRecord->GetItem(nIndex);
		CXTPReportRecordItem* pItem        = nullptr;

		if (pClickItem->IsChecked() == TRUE)
		{
			int nItemCount = pRecord->GetItemCount();
			for (int i= 0; i< nItemCount; i++)
			{
				if (i == nIndex)
					continue;

				pItem = pRecord->GetItem(i);
				if (pItem->GetHasCheckbox())
				{
					pItem->SetChecked(FALSE);
				}
			}
		}
		else
		{
			pClickItem->SetChecked(TRUE);
		}
	}
}


bool KNetworkInspectionDlg::IsZeroCheckReportA()
{
	CXTPReportRecords* pRecords = m_wndReportACtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for(int i = 0; i < nRecordCount; ++i)
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_2_A_COLUMN_EXPRESSWAY);
		if (pItem->IsChecked() == TRUE)
		{
			return false;
		}

		pItem   = pRecord->GetItem(_3_A_COLUMN_RAMP);
		if (pItem->IsChecked() == TRUE)
		{
			return false;
		}
	}

	return true;
}


void KNetworkInspectionDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct; 

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _1_B_COLUMN_SHUTTLE <= nIndex && nIndex <= _4_B_COLUMN_OTHER)
	{
		CXTPReportRecord*     pRecord      = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pClickItem   = pRecord->GetItem(nIndex);
		CXTPReportRecordItem* pItem        = nullptr;

		bool bExistTransitData     = IsExsitTransitData();
		bool bExistTransitLineData = IsExsitTransitLineData();

		if (false == bExistTransitData || false == bExistTransitLineData)
		{
			pClickItem->SetChecked(FALSE);
			MsgNoExistTransitData(bExistTransitData, bExistTransitLineData);
			return;
		}

		if (pClickItem->IsChecked() == TRUE)
		{
			int nItemCount = pRecord->GetItemCount();
			for (int i= 0; i< nItemCount; i++)
			{
				if (i == nIndex)
					continue;

				pItem = pRecord->GetItem(i);
				if (pItem->GetHasCheckbox())
				{
					pItem->SetChecked(FALSE);
				}
			}
		}
		else
		{
			pClickItem->SetChecked(TRUE);
		}
	}
}

bool KNetworkInspectionDlg::IsZeroCheckReportB()
{
	CXTPReportRecords* pRecords = m_wndReportBCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	CXTPReportRecord*     pRecord = nullptr;
	CXTPReportRecordItem* pItem   = nullptr;

	for(int i = 0; i < nRecordCount; ++i)
	{
		pRecord = pRecords->GetAt(i);
		pItem   = pRecord->GetItem(_1_B_COLUMN_SHUTTLE);
		if (pItem->IsChecked() == TRUE)
			return false;

		pItem   = pRecord->GetItem(_2_B_COLUMN_INTRACITY);
		if (pItem->IsChecked() == TRUE)
			return false;

		pItem   = pRecord->GetItem(_3_B_COLUMN_RED);
		if (pItem->IsChecked() == TRUE)
			return false;

		pItem   = pRecord->GetItem(_4_B_COLUMN_OTHER);
		if (pItem->IsChecked() == TRUE)
			return false;
	}

	return true;
}


CString KNetworkInspectionDlg::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s%s%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, a_strFileName);

	return strSavePath;
}

int KNetworkInspectionDlg::ExistErrorFile()
{
	return m_nExistError;
}

void KNetworkInspectionDlg::TransitCount( int& nBusCount, int& nRailCount )
{
    nBusCount  = 0;
    nRailCount = 0;
    CString strSQL = _T("Select type, count(*) as cnt From Transit Group By type ");

    KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSQL);

    while (spResultSet->Next())
    {
        int nType  = spResultSet->GetValueInt(0);
        int nCount = spResultSet->GetValueInt(1);

        if (nType == TRANSIT_MODE_BUS)
        {
            nBusCount = nCount;
        }
        else if (nType == TRANSIT_MODE_RAIL)
        {
            nRailCount = nCount;
        }
    }
}

void KNetworkInspectionDlg::OnBnClickedCheckHighway()
{
    CheckControlHighway();
}

void KNetworkInspectionDlg::CheckControlHighway()
{
    if (KBulkDBaseTurn::IsTurn(m_pTarget)) {

        if (IsDlgButtonChecked(IDC_CHECK_HIGHWAY) == BST_CHECKED) {
            GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
            CheckDlgButton(IDC_CHECK1, BST_CHECKED);
        }
        else {
            GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
            CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
        }
    }
    else {
        GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
        CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
    }
}
