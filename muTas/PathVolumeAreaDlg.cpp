// PathVolumeAreaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathVolumeAreaDlg.h"
#include "afxdialogex.h"
#include "Project.h"
#include "Object.h"
#include "Target.h"
#include "MapView.h"
#include "DBasePathVolume.h"
#include "ImChampMultiZoneFind.h"
#include "TCoordinate.h"

#include "KBulkDBase.h"
#include "KBulkDBase2File.h"

#include "DBaseAssignment.h"
#include "InspectionNetwork.h"
#include "Launcher.h"
#include "WriteCodeAgrument.h"

#include "NodeSelectionInfo.h"
#include "ImChampFileExist.h"

#include "ImChampMapDefine.h"
#include "ImChampDir.h"

#include <fstream>
using namespace std;

namespace PathVolumeAreaENUM
{
	enum
	{
		_0_A_COLUMN_SELECT     = 0,
		_1_A_COLUMN_ZONEID     = 1,
		_2_A_COLUMN_NAME       = 2
	};

	enum
	{
		_0_B_COLUMN_SELECT     = 0,
		_1_B_COLUMN_ZONEID     = 1,
		_2_B_COLUMN_NAME       = 2
	};
}

using namespace PathVolumeAreaENUM;
// KPathVolumeAreaDlg 대화 상자입니다.

const int nZoneCharIndex = 53;
const int nZoneSize		 = 11;

IMPLEMENT_DYNAMIC(KPathVolumeAreaDlg, KResizeDialogEx)

KPathVolumeAreaDlg::KPathVolumeAreaDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeAreaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_strRunErrMsg(_T(""))
{
	try
	{
		m_dMaxVolume             = 0.0;
		m_dMinVolume             = 0.0;
		m_dMaxRatio              = 0.0;
		m_dMinRatio              = 0.0;
		m_nModeCount             = 0;
		m_nRunOption             = -1;
		m_nZoneThrough           = -1;

		m_strPreSelectedModeName = _T("");

		m_clrPathVol             = RGB(0, 0,  255);

		m_pModeODTable           = m_pTarget->Tables()->FindTable(TABLE_MODE_OD);	
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

KPathVolumeAreaDlg::~KPathVolumeAreaDlg()
{
	m_pModeODTable->Unregister(this);
	CloseWindowProc();
}

void KPathVolumeAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT_B,                m_ctrlReportB);
	DDX_Control(pDX, IDC_COMBO1,                  m_cboMode);
	DDX_Control(pDX, IDC_CLR_PATH,                m_cpPathColor);
}


BEGIN_MESSAGE_MAP(KPathVolumeAreaDlg, KResizeDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1,      &KPathVolumeAreaDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON2,        &KPathVolumeAreaDlg::OnBnClickedButtonSearchAreaCodeOrg)
	ON_BN_CLICKED(IDC_BUTTON4,        &KPathVolumeAreaDlg::OnBnClickedButtonSearchAreaCodeDest)
	ON_BN_CLICKED(IDC_BUTTON1,        &KPathVolumeAreaDlg::OnBnClickedButtonPolyDrawOrg)
	ON_BN_CLICKED(IDC_BUTTON3,        &KPathVolumeAreaDlg::OnBnClickedButtonPolyDrawDest)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,              &KPathVolumeAreaDlg::OnReportAItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A, &KPathVolumeAreaDlg::OnReportACheckItem)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,              &KPathVolumeAreaDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_B, &KPathVolumeAreaDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KPathVolumeAreaDlg::OnBnClickedCheckASelectAll)
	ON_BN_CLICKED(IDC_CHECK2, &KPathVolumeAreaDlg::OnBnClickedCheckBSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &KPathVolumeAreaDlg::OnBnClickedButtonSearch)
	ON_WM_COPYDATA()
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KPathVolumeAreaDlg::OnBnClickedRadioChart)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KPathVolumeAreaDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KPathVolumeAreaDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeAreaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KPathVolumeAreaDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK3, &KPathVolumeAreaDlg::OnBnClickedCheckZone)
END_MESSAGE_MAP()


// KPathVolumeAreaDlg 메시지 처리기입니다.

void KPathVolumeAreaDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);
		if(strSubjectName.CompareNoCase(TABLE_MODE_OD) == 0)
		{
			GetDBModeInfo();
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


void KPathVolumeAreaDlg::ActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaDlg::DeActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaDlg::ResizeComponent()
{
	SetResize(IDC_STATIC3,			      SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_EDIT1,			      SZ_TOP_LEFT,     SZ_TOP_LEFT);

	SetResize(IDC_CHECK1,	    		  SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_REPORT_A,				  SZ_TOP_LEFT,     SZ_MIDDLE_RIGHT);

	SetResize(IDC_STATIC4,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_BUTTON3,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_BUTTON4,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_CHECK2,			      SZ_MIDDLE_LEFT,  SZ_MIDDLE_LEFT);
	SetResize(IDC_REPORT_B,				  SZ_MIDDLE_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_SEARCH2,		  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR8,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC2,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO1,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_MIN_VALUE3,      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_MAX_VALUE3,	  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC22,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC23,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MIN_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAX_VALUE,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MIN_RATIO,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAX_RATIO,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MINSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MAXSIZE,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC16,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_PATH,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CLEAR,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DRAW,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);		
	SetResize(IDC_CHECK3,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR9,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);		
	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KPathVolumeAreaDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit(false);
		m_pMapView->TxClearCompositeThemes(false);
		m_pMapView->RefreshAll();

		BOOL bSelectAll(FALSE);
		size_t nxCount = a_vecSelection.size();
		if (MAP_NODE_START == m_bPointFlag)
		{
			if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			m_setZoneOrg.clear();
			for (size_t i= 0; i< nxCount; i++)
			{
				 Integer &nxID = a_vecSelection[i];

				 if (m_mapZoneNodeData.find(nxID) == m_mapZoneNodeData.end())
					 continue;

				 m_setZoneOrg.insert(nxID);
			}

			UpdateReport(m_ctrlReportA, m_setZoneOrg, bSelectAll);
		}
		else
		{
			if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			m_setZoneDest.clear();
			for (size_t i= 0; i< nxCount; i++)
			{
				Integer &nxID = a_vecSelection[i];

				if (m_mapZoneNodeData.find(nxID) == m_mapZoneNodeData.end())
					continue;

				m_setZoneDest.insert(nxID);
			}

			UpdateReport(m_ctrlReportB, m_setZoneDest, bSelectAll);
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


BOOL KPathVolumeAreaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReportA, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportB, FALSE, FALSE, FALSE);

	KColorPickerSetting::DefaultCtrl(m_cpPathColor,     m_clrPathVol);

	CWaitCursor wc;

	CString strMinPathVol(_T("0.001"));
	SetDlgItemText(IDC_EDIT1, strMinPathVol);

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckDlgButton(IDC_CHECK2, BST_CHECKED);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	HiddenMinMaxControl();
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK3, BST_CHECKED);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("15.0"));

	GetDBModeInfo();
	GetDBRunOption();
	GetDBModeVolumeFiledName();
	KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
	SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);

	InitReportHeader();
	m_ctrlReportA.Populate();
	m_ctrlReportB.Populate();

	ResizeComponent();

	m_pModeODTable->Register(this);

	try
	{
		AddImChampTooltip(IDC_RADIO2, _T("비율(%) = Path Volume / Link Volume * 100"));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}  

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPathVolumeAreaDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_0_A_COLUMN_SELECT,   _T("Select"),   40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_1_A_COLUMN_ZONEID,   _T("Origin ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("기점 존 노드 ID"));
    }

	pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_2_A_COLUMN_NAME,     _T("Name"),     60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 명칭"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_0_B_COLUMN_SELECT,   _T("Select"),   40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_1_B_COLUMN_ZONEID,   _T("Destination ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("종점 존 노드 ID"));
    }

	pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_2_B_COLUMN_NAME,     _T("Name"),     60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 명칭"));
    }
}


void KPathVolumeAreaDlg::GetDBModeVolumeFiledName()
{
	try
	{
		m_mapModeVolumeFiledName.clear();

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT sum_volume FROM assign_model WHERE run_seq IN (SELECT  seq FROM assign_path_model) "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strSumColumnName(_T(""));
		if( spResult->Next() )
		{
			strSumColumnName	  = spResult->GetValueString(0);
		}
		m_mapModeVolumeFiledName.insert(std::make_pair(0, strSumColumnName));

		strSQL = _T("");
		strSQL.Format (_T(" SELECT mode_id, Column_name FROM assign_path_input WHERE preload = 0 "));

		spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strModeColumnName(_T(""));
		int     nModeID(0);
		while ( spResult->Next() )
		{
			nModeID               = spResult->GetValueInt   (0);
			strModeColumnName	  = spResult->GetValueString(1);

			m_mapModeVolumeFiledName.insert(std::make_pair(nModeID, strModeColumnName));
		}
	}
	catch (...)
	{
		m_mapModeVolumeFiledName.clear();
		TxLogDebugException();
	}
}


bool KPathVolumeAreaDlg::GetDBRunOption()
{
	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT run_option, avrtime_column, zone_through FROM assign_path_model "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		CString strColumnName(_T(""));
		while( spResult->Next() )
		{
			m_nRunOption	  = spResult->GetValueInt (0);
			strColumnName     = spResult->GetValueString(1);
			m_nZoneThrough    = spResult->GetValueInt(2);
		}

		KIOTables*        pIOTables = m_pTarget->Tables();
		KIOTable*         pTable    = pIOTables->FindTable(TABLE_LINK);

		const KIOColumns* pColumns  = pTable->Columns();
		int nIndex      = pColumns->GetColumnIndex(strColumnName);

		KIOColumn* pColumn_LinkTime = NULL;
		if(nIndex < 0)
			pColumn_LinkTime = NULL;
		else
			pColumn_LinkTime = pColumns->GetColumn(nIndex);

		if(NULL != pColumn_LinkTime)
			m_oUserColumnsLink.AddNewColumn(pColumn_LinkTime);
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}


void KPathVolumeAreaDlg::SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode)
{
	try
	{
		AutoType iter  = a_mapZoneNodeData.begin();
		AutoType itEnd = a_mapZoneNodeData.end();

		a_vecDrawGuideNode.clear();
 
		TPreferenceSymbol &oZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		while (iter != itEnd)
		{
			Integer nxNodeID = iter->first;

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxNodeID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxNodeID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = oZoneSymbol.nCharIndex;
			oNodeInfo.TSize           = oZoneSymbol.dSize;
			oNodeInfo.TColorRGB       = oZoneSymbol.clrSymbol;

			a_vecDrawGuideNode.push_back(oNodeInfo);

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::GetDBModeInfo()
{
	try
	{
		m_cboMode.ResetContent();
		m_cboMode.Clear();
		m_mapModeInfo.clear();

		int     nIDXCombo   = 0; //ComboBox의 Index all - 0, 수단1 ... 수단n
		int     nModeID     = 0;
		CString strModeName = _T("All");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strModeName = _T("전체");
		}
		CString strView     = _T("");
		m_nModeCount        = 0;

		int nPreSelectedIndex = -1;
		//int nCur              = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		int nCur              = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG_K);
		}
		else {
			nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
		}
		m_cboMode.SetItemData( nCur, DEFAULT_MODEID );

		strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strView.Format(_T("%s"), strModeName);
		}
		nCur = m_cboMode.AddString(strView);
		m_cboMode.SetItemData( nCur, nIDXCombo );
		m_mapModeInfo.insert(std::make_pair(nIDXCombo, strModeName));

		if( m_strPreSelectedModeName.CompareNoCase(strModeName) == 0 )
		{
			nPreSelectedIndex = nCur;
		}

		nIDXCombo++;

		std::map<int, CString> mapModeName;
		KDBasePathVolume::GetDBModeInfo(m_pTarget, mapModeName);

		AutoType iter  = mapModeName.begin();
		AutoType itEnd = mapModeName.end();
		while (iter != itEnd)
		{
			int     nModeID     = iter->first;
			CString strModeName = iter->second;

			strView.Format(_T("%d: %s"), nIDXCombo, strModeName);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				strView.Format(_T("%s"), strModeName);
			}
			nCur = m_cboMode.AddString(strView);
			m_cboMode.SetItemData( nCur, nIDXCombo );
			m_mapModeInfo.insert(std::make_pair(nIDXCombo, strModeName));

			if( m_strPreSelectedModeName.CompareNoCase(strModeName) == 0 )
			{
				nPreSelectedIndex = nCur;
			}

			m_nModeCount++;
			nIDXCombo++;

			++iter;
		}

		if(m_nModeCount < 1)
		{
			m_cboMode.ResetContent();
			m_cboMode.Clear();
			m_mapModeInfo.clear();

			//int nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
			int nCur              = 0;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG_K);
			}
			else {
				nCur = m_cboMode.AddString(MODE_COMBO_INIT_MSG);
			}
			m_cboMode.SetItemData( nCur, DEFAULT_MODEID );
		}
		else
		{
			if (nPreSelectedIndex >= 0)
			{
				m_cboMode.SetCurSel(nPreSelectedIndex);
			}
			else
			{
				m_cboMode.SetCurSel(1);	
			}
		}

		UpdateData(FALSE);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


int KPathVolumeAreaDlg::GetSelMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nModeID = (int)m_cboMode.GetItemData(nSelCur);

	if(DEFAULT_MODEID == nModeID)
	{
		AfxMessageBox(_T("통행수단을 선택해 주세요."));
		m_cboMode.SetFocus();
	}

	return nModeID;
}


void KPathVolumeAreaDlg::OnCbnSelchangeComboMode()
{
	UpdateData(TRUE);

	int nSelCur = m_cboMode.GetCurSel();
	int nIdx    = m_cboMode.GetItemData(nSelCur);

	AutoType iter = m_mapModeInfo.find(nIdx);
	if (iter != m_mapModeInfo.end())
	{
		m_strPreSelectedModeName = iter->second;
	}

	//ResetVolumeData();
}

void KPathVolumeAreaDlg::OnBnClickedButtonSearchAreaCodeOrg()
{
	try
	{
		KImChampMultiZoneFind oFindDlg(m_pTarget);
		if (oFindDlg.DoModal() == IDOK)
		{
			oFindDlg.GetSelectedID(m_setZoneOrg);

			BOOL bSelectAll(FALSE);
			if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			UpdateReport(m_ctrlReportA, m_setZoneOrg, bSelectAll);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnBnClickedButtonSearchAreaCodeDest()
{
	try
	{
		KImChampMultiZoneFind oFindDlg(m_pTarget);
		if (oFindDlg.DoModal() == IDOK)
		{
			oFindDlg.GetSelectedID(m_setZoneDest);

			BOOL bSelectAll(FALSE);
			if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
				bSelectAll = TRUE;
			else
				bSelectAll = FALSE;

			UpdateReport(m_ctrlReportB, m_setZoneDest, bSelectAll);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll)
{
	a_ctrlReport.ResetContent();

	try
	{
		AutoType iter  = a_setZoneID.begin();
		AutoType itEnd = a_setZoneID.end();

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;
		
		CString strID(_T(""));
		while (iter != itEnd)
		{
			Integer nxID = *iter;

			pRecord = a_ctrlReport.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(a_bSelectAll);
			pItem->SetItemData((DWORD_PTR)nxID);
			pItem->SetAlignment(xtpColumnIconCenter);
			
			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			strID.Format(_T("%I64d"), nxID);
			pItem->SetCaption(strID);
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			CString strName(_T(""));
			AutoType iterZone = m_mapZoneNodeData.find(nxID);
			if (iterZone != m_mapZoneNodeData.end())
			{
				 strName = iterZone->second;
			}
			pItem->SetCaption(strName);
			pItem->SetAlignment(DT_LEFT);

			++iter;
		}
	}
	catch (...)
	{
		a_ctrlReport.ResetContent();
		TxLogDebugException();	
	}

	a_ctrlReport.Populate();

	//DrawSelectedOrgDestination();

	ResetVolumeData();
}


void KPathVolumeAreaDlg::OnBnClickedButtonPolyDrawOrg()
{
	try
	{
		m_bPointFlag = MAP_NODE_START;

		std::vector<TDrawNodeInfo> vecDrawTotalNode = m_vecDrawGuideNode;

		GetSelectedDrawNodeInfo(vecDrawTotalNode, false);

		m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);
		m_pMapView->TargetRegionInsert(0, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnBnClickedButtonPolyDrawDest()
{
	try
	{
		m_bPointFlag = MAP_NODE_END;

		std::vector<TDrawNodeInfo> vecDrawTotalNode = m_vecDrawGuideNode;

		GetSelectedDrawNodeInfo(vecDrawTotalNode, false);

		m_pMapView->TxDrawCompositeNode(vecDrawTotalNode, false, true);
		m_pMapView->TargetRegionInsert(0, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}

	//DrawSelectedOrgDestination();

	ResetVolumeData();
}


void KPathVolumeAreaDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
	}

	//DrawSelectedOrgDestination();

	ResetVolumeData();
}


void KPathVolumeAreaDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_A_COLUMN_SELECT == nIndex)
	{
		return;
	}

	try
	{
		CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem*    pItem       = pRecord->GetItem(_0_A_COLUMN_SELECT);
		Integer nxZoneID = (Integer)pItem->GetItemData();

		m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_0_B_COLUMN_SELECT == nIndex)
	{
		return;
	}

	try
	{
		CXTPReportRecord*	     pRecord     = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem*    pItem       = pRecord->GetItem(_0_B_COLUMN_SELECT);
		Integer nxZoneID = (Integer)pItem->GetItemData();

		m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnBnClickedCheckASelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*    pRecords = m_ctrlReportA.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReportA.RedrawControl();

		DrawSelectedOrgDestination(true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::OnBnClickedCheckBSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED)
			bSelectAll = TRUE;
		else
			bSelectAll = FALSE;

		CXTPReportRecords*    pRecords = m_ctrlReportB.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_B_COLUMN_SELECT);

			pItem->SetChecked(bSelectAll);
		}

		m_ctrlReportB.RedrawControl();

		DrawSelectedOrgDestination(true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

}


void KPathVolumeAreaDlg::OnBnClickedButtonSearch()
{
	try
	{
		m_nResultCode = DLL_UNEXPECTED_ERROR;

		m_nModeID = GetSelMode();
		if(DEFAULT_MODEID == m_nModeID)
			return;

		CString strPathVol(_T(""));
		GetDlgItemText(IDC_EDIT1, strPathVol);

		if (QBicStringChecker::IsNumeric(strPathVol) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		m_dMinPathVol = _ttof(strPathVol);
		if (m_dMinPathVol < 0.0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return;
		}

		GetCheckedOrg(m_vecSelectedZoneOrg);
		if (m_vecSelectedZoneOrg.size() < 1)
		{
			AfxMessageBox(_T("기점을 선택해 주세요."));
			return;
		}
		
		GetCheckedDest(m_vecSelectedZoneDest);
		if (m_vecSelectedZoneDest.size() < 1)
		{
			AfxMessageBox(_T("종점을 선택해 주세요."));
			return;
		}

		if( m_nRunOption < 0 || m_nZoneThrough < 0 || m_oUserColumnsLink.ColumnCount() < 1 )
		{
			CString strMsg(_T(""));
			strMsg.Append(_T("경로 분석 결과 정보가 없습니다.\r\n"));
			strMsg.Append(_T("메뉴 > 모형 > 도로 통행배정 모형을 먼저 실행해 주세요.\r\n"));
			strMsg.Append(_T("(실행 시,\r\n"));
			strMsg.Append(_T(" > 균형 배정법 : SOBA 선택 \r\n"));
			strMsg.Append(_T(" > 실행 옵션 선택에서 경로 교통량 분석을 선택해야 함)\r\n"));

			AfxMessageBox(strMsg);
			return;
		}

		if(false == ImChampFileExist::BushFileCheckMessage(m_pTarget))
		{
			return;
		}

		ResetVolumeData();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(PathVolumeThreadCaller, this, false, nLang);

		if( !m_bResultRun)
		{
			AfxMessageBox(m_strRunErrMsg);
			return;
		}

		UpdateMinMax();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::GetCheckedOrg(std::vector<Integer> &a_vecSelectedZoneOrg)
{
	try
	{
		a_vecSelectedZoneOrg.clear();	

		CXTPReportRecords*    pRecords = m_ctrlReportA.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}
			
			Integer nxOrgZoneID = (Integer)pItem->GetItemData();

			a_vecSelectedZoneOrg.push_back(nxOrgZoneID);  
		}
	}
	catch (...)
	{
		a_vecSelectedZoneOrg.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::GetCheckedDest(std::vector<Integer> &a_vecSelectedZoneDest)
{
	try
	{
		a_vecSelectedZoneDest.clear();

		CXTPReportRecords*    pRecords = m_ctrlReportB.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_B_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
			{
				continue;
			}

			Integer nxDestZoneID = (Integer)pItem->GetItemData();

			a_vecSelectedZoneDest.push_back(nxDestZoneID);  
		}
	}
	catch (...)
	{
		a_vecSelectedZoneDest.clear();
		TxLogDebugException();	
	}
}



void KPathVolumeAreaDlg::ResetVolumeData()
{
	try
	{
		ClearMapDraw(false);
		DrawSelectedOrgDestination(true);
		
		InitVolMinMax();
		InitRatioMinMax();
		SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_VALUE,  _T(""));

		SetDlgItemText(IDC_EDIT_MIN_RATIO,  _T(""));
		SetDlgItemText(IDC_EDIT_MAX_RATIO,  _T(""));

		m_mapLinkVolume.clear();
		m_mapLinkRatio.clear();

		m_vecDrawLinkInfo.clear();
		m_vecDrawLinkInfoNoLabel.clear();
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


unsigned __stdcall KPathVolumeAreaDlg::PathVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("구역간 이용경로 도로 교통량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Area-to-Area Highway Path Analysis Module..."));
	}

	KPathVolumeAreaDlg* pDlg = (KPathVolumeAreaDlg*)pParameter->GetParameter();
	pDlg->GetSearchPathVolume();

	return 0;
}


void KPathVolumeAreaDlg::GetSearchPathVolume()
{
	TxLogDebugStartMsg();
	m_strRunErrMsg = _T("");
	m_bResultRun   = true;
	try
	{
		if(! CreateArgument(m_oUserColumnsLink))
		{
			m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
			ThrowException(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		}

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		//Step 0. Dll load
		CString strWindowName;
		GetWindowText(strWindowName);

		AddStatusMessage(_T("> Analysis Start ..."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("구역간 이용경로 도로 교통량 분석 모듈 실행 중..."));
		}
		else {
			AddStatusMessage(_T("Run Area-to-Area Highway Path Analysis Module..."));
		}

		KLauncher::Run(strWindowName, 29);

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("구역간 이용경로 도로 교통량 분석 모듈 실행이 성공적으로 완료되었습니다."));
			}
			else {
				AddStatusMessage(_T("Area-to-Area Highway Path Analysis Module Complete."));
			}
		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
			}
			else {
				AddStatusMessage(_T("Running Stopped."));
			}
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AddStatusMessage(_T("> Assignment Dll Not Found ..."));
			ThrowException(_T("DLL NOT Found."));
		}
		else
		{
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Running Failed."));
			}
			ThrowException(_T("Path Volume 검색 중 오류가 발생하였습니다."));
		}

		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			m_nResultCode = DLL_USER_STOPPING;
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
		}
		if (! ReadOutPutFile())
		{
			m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
			if (KmzSystem::GetLanguage() == KEMKorea) {
				AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
			}
			else {
				AddStatusMessage(_T("Saving Output Data Failed."));
			}
			ThrowException(_T("Output Volume 정보 입력 실패."));
		}
	}
	catch(KExceptionPtr ex)
	{
		m_strRunErrMsg = ex->GetErrorMessage();
		m_bResultRun = false;
		return;
	}
	catch(...)
	{
		m_strRunErrMsg = _T("Path Volume 검색 중 오류가 발생하였습니다");
		m_bResultRun = false;
		return;
	}

	TxLogDebugEndMsg();
	m_bResultRun = true;
}


bool KPathVolumeAreaDlg::CreateArgument( KIOColumns &a_oUserColumnsLink )
{
	TxLogDebugStartMsg();

	ImChampDir::SetCurrentDirectoryToApp();
	if(! ClearFiles())
	{
		return false;
	}

	bool bResult = false;

	try 
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pTarget, BUSH_PATHVOLUMN_FOLDER_NAME);
	} catch (...) {
		AfxMessageBox(_T("Dat 생성(DefineNetworkType.arg) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	//TxLogDebug("END CLEAR FILE ------ ");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_PATHVOLUME_AREA, nOpenMode);

	// .arg 작성
	bResult = WritePathVolumeArgument(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WritePathVolumeArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	//TxLogDebug("END ARG CREAETE FILE ------ ");
	// 링크파일 생성
	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;

	KDBaseAssignment::GetAssignmentTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);

	KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData);

	CString strFilePath(_T(""));

	strFilePath = ConvertSavePath(_T("link.dat"));

	bResult = KBulkDBase2File::ExportDB2LinkFileExcludeType(m_pTarget->GetDBaseConnection(), a_oUserColumnsLink, setUsableLinkData, strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2LinkFileB) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	strFilePath = ConvertSavePath(_T("node.dat"));

	// 노드파일 생성
	bResult = KBulkDBase2File::ExportDB2NodeFile(m_pTarget->GetDBaseConnection(), strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	strFilePath = ConvertSavePath(_T("turn.dat"));
	// 회전정보 파일 생성
	bResult = KBulkDBase2File::ExportDB2TurnFile(m_pTarget->GetDBaseConnection(), strFilePath);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	//TxLogDebug("end turn CREAETE FILE ------ ");

	TxLogDebugEndMsg();
	return true;
}


bool KPathVolumeAreaDlg::WritePathVolumeArgument(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{
		KObject* pScenario = m_pTarget->GetParentObject();

		strOut.Format(_T("%s_%s_%s\r\n"), m_pProject->GetName(), pScenario->GetName(), m_pTarget->GetName());
		of.WriteString(strOut);

		//선택한 수단값으로 업데이트를 하면되고
		//strOut.Format(_T("OD\t%d\t%d\r\n"), m_nModeCount,  m_nModeID);
		strOut.Format(_T("OD\t%d\t%d\r\n"), m_nModeCount,  m_nModeID);
		of.WriteString(strOut);

		strOut.Format(_T("run_options\t%d\r\n"), m_nRunOption);
		of.WriteString(strOut);

		strOut.Format(_T("ZoneThru\t%d\r\n"), m_nZoneThrough);
		of.WriteString(strOut);

		int nSelectedZoneOrgCnt = (int)m_vecSelectedZoneOrg.size();
		strOut.Format(_T("OZone\t%d\r\n"), nSelectedZoneOrgCnt);
		of.WriteString(strOut);
		for (int i= 0; i< nSelectedZoneOrgCnt; i++)
		{
			strOut.Format(_T("%I64d\r\n"), m_vecSelectedZoneOrg[i]);
			of.WriteString(strOut);
		}

		int nSelectedZoneDestCnt = (int)m_vecSelectedZoneDest.size();
		strOut.Format(_T("DZone\t%d\r\n"), nSelectedZoneDestCnt);
		of.WriteString(strOut);
		for (int i= 0; i< nSelectedZoneDestCnt; i++)
		{
			strOut.Format(_T("%I64d\r\n"), m_vecSelectedZoneDest[i]);
			of.WriteString(strOut);
		}

		strOut.Format(_T("PathVolumeLimit\t%f\r\n"), m_dMinPathVol);
		of.WriteString(strOut);

		CString FolderName(_T(""));
		FolderName.Format(_T("%s\\PATHVOLUME\r\n"), m_pTarget->GetLocation());
		of.WriteString(FolderName);
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KPathVolumeAreaDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)T_ARG_PATHVOLUME_AREA);
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_TURN));

		// 결과 파일 삭제 
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_PATHVOLUME_AREA));
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


CString KPathVolumeAreaDlg::ConvertSavePath(CString a_strFileName)
{
	CString strSavePath(_T(""));
	strSavePath.Format(_T("%s\\%s\\%s"), m_pTarget->GetLocation(), BUSH_PATHVOLUMN_FOLDER_NAME, a_strFileName);

	return strSavePath;
}


void KPathVolumeAreaDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KPathVolumeAreaDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


BOOL KPathVolumeAreaDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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


bool KPathVolumeAreaDlg::ReadOutPutFile()
{
  	InitVolMinMax();
 	InitRatioMinMax();

	try
	{
		CString strPath = ConvertSavePath(T_DAT_OUT_PATHVOLUME_AREA);
		ifstream ifs(strPath, std::ios::binary);
		if (! ifs)
		{ 
			ifs.close();
			return false;
		}

		Integer nxLinkID(0);
		double  dVolume(0.0);
		double  dVolumeDistance(0.0);
		int     nConstValue(0);

		m_mapLinkVolume.clear();
		m_mapLinkRatio.clear();
		while(! ifs.eof())
		{
			ifs.read(reinterpret_cast<char*>( &nxLinkID), sizeof(Integer) );
			ifs.read(reinterpret_cast<char*>( &dVolume), sizeof(double) );

			if (ifs.eof())
			{
				break;
			}

			ifs.read(reinterpret_cast<char*>( &dVolumeDistance), sizeof(double));
			ifs.read(reinterpret_cast<char*>( &nConstValue), sizeof(int));
			
			AutoType iter = m_mapLinkVolume.find(nxLinkID);
			if (iter != m_mapLinkVolume.end())
			{
				double &dSumVolume = iter->second;
				dSumVolume += dVolume;
			}
			else
			{
				m_mapLinkVolume.insert(std::make_pair(nxLinkID, dVolume));
			}
		}

		ifs.close();

		SetVolMinMax(m_mapLinkVolume);

		SetRatioMinMax(m_mapLinkVolume, m_mapLinkRatio);
	}
	catch (...)
	{
		InitVolMinMax();
		InitRatioMinMax();
		TxLogDebugException();
		return false;
	}    

	return true;
}


void KPathVolumeAreaDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KPathVolumeAreaDlg::SetVolMinMax(std::map<Integer, double> &a_mapLinkVolume)
{
	double dMaxVolume = -999999999999.0;
	double dMinVolume = 999999999999.0;

	AutoType iter     = a_mapLinkVolume.begin();
	AutoType itEnd    = a_mapLinkVolume.end();

	double dVolume(0.0);
	while (iter != itEnd)
	{
		dVolume = iter->second;

		if (dVolume >= 0)
		{
			if (dMinVolume > dVolume)
			{
				dMinVolume = dVolume;
			}
		}

		if (dMaxVolume < dVolume)
		{
			dMaxVolume = dVolume;
		}

		++iter;
	}

	if (a_mapLinkVolume.size() < 1)
	{
		InitVolMinMax();
	}
	else
	{
		m_dMaxVolume = dMaxVolume;
		m_dMinVolume = dMinVolume;
	}
}


void KPathVolumeAreaDlg::InitRatioMinMax()
{
	m_dMaxRatio = 0.0;
	m_dMinRatio = 0.0;
}


void KPathVolumeAreaDlg::SetRatioMinMax(std::map<Integer, double> &a_mapLinkVolume, std::map<Integer, double> &a_mapLinkRatio)
{
	a_mapLinkRatio.clear();

	try
	{
		double dMaxRatio = -999999999999.0;
		double dMinRatio = 999999999999.0;

		std::map<Integer, double> mapAllLinkModeVolume;

		AutoType iterField = m_mapModeVolumeFiledName.find(m_nModeID);
		if (iterField != m_mapModeVolumeFiledName.end())
		{
			CString strVolumeFieldName = iterField->second;
			GetModeVolume(strVolumeFieldName, mapAllLinkModeVolume);
		}

		AutoType iter  = a_mapLinkVolume.begin();
		AutoType itEnd = a_mapLinkVolume.end();

		Integer nxLinkID(0);
		double  dVolume(0.0);
		double  dRatio(0.0);
		while (iter != itEnd)
		{
			nxLinkID = iter->first;
			dVolume  = iter->second;
			dRatio   = 0.0;

			AutoType iterMVol = mapAllLinkModeVolume.find(nxLinkID);
			if (iterMVol != mapAllLinkModeVolume.end())
			{
				double dModeVolume = iterMVol->second;

				if (dModeVolume <= 0.0)
				{
					dRatio = 0.0;
				}
				else
				{
					dRatio = (dVolume/dModeVolume) * 100.0;
				}
			}

			a_mapLinkRatio.insert(std::make_pair(nxLinkID, dRatio));

			if (dRatio >= 0)
			{
				if (dMinRatio > dRatio)
				{
					dMinRatio = dRatio;
				}
			}

			if (dMaxRatio < dRatio)
			{
				dMaxRatio = dRatio;
			}

			++iter;
		}

		if (a_mapLinkRatio.size() < 1)
		{
			InitRatioMinMax();
		}
		else
		{
			m_dMaxRatio = dMaxRatio;
			m_dMinRatio = dMinRatio;
		}
	}
	catch (...)
	{
		a_mapLinkRatio.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::GetModeVolume(CString &a_strFieldName, std::map<Integer, double> &a_mapAllLinkModeVolume)
{
	try
	{
		a_mapAllLinkModeVolume.clear();
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL(_T(""));
		strSQL.Format (_T(" SELECT link_id, %s FROM %s "), a_strFieldName, TABLE_LINK);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);

		Integer nxLinkID(0);
		double  dVolume (0.0);
		while( spResult->Next() )
		{
			nxLinkID          = spResult->GetValueInt64 (0);
			dVolume     	  = spResult->GetValueDouble(1);

			a_mapAllLinkModeVolume.insert(std::make_pair(nxLinkID, dVolume));
		}
	}
	catch (...)
	{
		a_mapAllLinkModeVolume.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::UpdateMinMax()
{
	CString strMinVol(_T(""));
	CString strMaxVol(_T(""));

	double dMinVol(0.0);
	dMinVol = m_dMinVolume - 0.5;
	if(dMinVol < 0.0)
		dMinVol = 0.0;

	if (dMinVol < m_dMinVolume)
	{
		dMinVol = m_dMinVolume;
	}

	int nMinVol = (int)dMinVol;
	dMinVol     = (double)nMinVol; // double 버림 처리
	if (dMinVol < m_dMinVolume)
	{
		dMinVol = m_dMinVolume;
		/*strMinVol.Format(_T("%f"), dMinVol);*/
		GetDlgItemText(IDC_EDIT1, strMinVol);
	}
	else
	{
		strMinVol.Format(_T("%.0f"), dMinVol);
	}

	double dMaxVol(0.0);
	if (0.0 == m_dMaxVolume)
	{
		dMaxVol = m_dMaxVolume;
	}
	else
	{
		dMaxVol = m_dMaxVolume + 0.6;
	}

	strMaxVol.Format(_T("%.0f"), dMaxVol);

	/*SetDlgItemText(IDC_EDIT_MIN_VALUE,  strMinVol);*/
	SetDlgItemText(IDC_EDIT_MIN_VALUE,  _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAX_VALUE,  strMaxVol);

	CString strMinRatio(_T(""));
	CString strMaxRatio(_T(""));

	double dMinRatio(0.0);
	dMinRatio = m_dMinRatio - 0.5;
	if(dMinRatio < 0.0)
		dMinRatio = 0.0;

	if (dMinRatio < m_dMinRatio)
	{
		dMinRatio = m_dMinRatio;
	}

	int nMinRatio = (int)dMinRatio;
	dMinRatio     = (double)nMinRatio; // double 버림 처리
	strMinRatio.Format(_T("%.0f"), dMinRatio);

	double dMaxRatio(0.0);
	if (0.0 == m_dMaxRatio)
	{
		dMaxRatio = m_dMaxRatio;
	}
	else
	{
		dMaxRatio = m_dMaxRatio + 0.6;
	}
	
	strMaxRatio.Format(_T("%.0f"), dMaxRatio);

	/*SetDlgItemText(IDC_EDIT_MIN_RATIO,  strMinRatio);*/
	SetDlgItemText(IDC_EDIT_MIN_RATIO,  _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAX_RATIO,  strMaxRatio);
}


void KPathVolumeAreaDlg::HiddenMinMaxControl()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_SHOW);
	}
}


void KPathVolumeAreaDlg::OnBnClickedRadioChart(UINT nID)
{
	HiddenMinMaxControl();

	ClearMapDraw(false);
	DrawSelectedOrgDestination(true);
}


void KPathVolumeAreaDlg::OnBnClickedButtonDraw()
{
	try
	{
		if (m_mapLinkVolume.size() < 1)
		{
			AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
			return;
		}

		if (UserVolumeInputCheck() == false)
		{
			return;
		}

		CWaitCursor cw;

		TPathVolumeSettingInfo oTVolumeSettingInfo;
		oTVolumeSettingInfo.bUseOutRange = false;
		oTVolumeSettingInfo.clrOutRange  = RGB(255, 255, 255);
		oTVolumeSettingInfo.clrPath      = m_cpPathColor.GetColor();

		int nMinID;
		int nMaxID;
		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			nMinID = IDC_EDIT_MIN_VALUE;
			nMaxID = IDC_EDIT_MAX_VALUE;
		}
		else
		{
			nMinID = IDC_EDIT_MIN_RATIO;
			nMaxID = IDC_EDIT_MAX_RATIO;
		}

		CString strValue(_T(""));
		GetDlgItemText(nMinID, strValue);
		oTVolumeSettingInfo.dMinTrip    = _ttof(strValue);
		GetDlgItemText(nMaxID, strValue);
		oTVolumeSettingInfo.dMaxTrip    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		oTVolumeSettingInfo.dMinSize    = _ttof(strValue);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		oTVolumeSettingInfo.dMaxSize    = _ttof(strValue);

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			KPathVolumeInfo::GetDrawVolumeAreaInfo(m_mapLinkVolume, oTVolumeSettingInfo, m_vecDrawLinkInfo, m_vecDrawLinkInfoNoLabel);
		}
		else
		{
			KPathVolumeInfo::GetDrawVolumeAreaInfo(m_mapLinkRatio, oTVolumeSettingInfo, m_vecDrawLinkInfo, m_vecDrawLinkInfoNoLabel);
		}
		DrawVolumeMap();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPathVolumeAreaDlg::UserVolumeInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

		int nMinID;
		int nMaxID;

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			nMinID = IDC_EDIT_MIN_VALUE;
			nMaxID = IDC_EDIT_MAX_VALUE;
		}
		else
		{
			nMinID = IDC_EDIT_MIN_RATIO;
			nMaxID = IDC_EDIT_MAX_RATIO;
		}

		//min value 체크
		GetDlgItemText(nMinID, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(nMinID)->SetFocus();		
			return false;
		}
		dMinValue = _ttof(strValue);

		//IDC_EDIT_MAX_VALUE
		GetDlgItemText(nMaxID, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(nMaxID)->SetFocus();		
			return false;
		}
		dMaxValue = _ttof(strValue);

		//IDC_EDIT_MINSIZE
		GetDlgItemText(IDC_EDIT_MINSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
		}
		dMinSize = _ttof(strValue);

		//IDC_EDIT_MAXSIZE
		GetDlgItemText(IDC_EDIT_MAXSIZE, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAXSIZE)->SetFocus();		
			return false;
		}
		dMaxSize = _ttof(strValue);

		if (dMinValue > dMaxValue)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(nMinID)->SetFocus();		
			return false;
		}

		if (dMinSize > dMaxSize)
		{
			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MINSIZE)->SetFocus();		
			return false;
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
	return true;
}


void KPathVolumeAreaDlg::DrawVolumeMap()
{
	try
	{
		ClearMapDraw(false);
		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
		{
			DrawSelectedOrgDestination(false);
		}

		std::vector<TDrawLinkInfo> vecDrawLinkInfo;
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			vecDrawLinkInfo = m_vecDrawLinkInfo;
		}
		else
		{
			vecDrawLinkInfo = m_vecDrawLinkInfoNoLabel;
		}

		m_pMapView->TxDrawCompositeLink(vecDrawLinkInfo, true, true);
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeAreaDlg::ClearMapDraw(bool bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(false);
		m_pMapView->ClearScreenDrawEdit(false);

        if (bRefresh)
            m_pMapView->MapRefresh();
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


void KPathVolumeAreaDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(false);
	DrawSelectedOrgDestination(true);
}


void KPathVolumeAreaDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KPathVolumeAreaDlg::CloseWindowProc()
{
	ClearMapDraw(true);
}


void KPathVolumeAreaDlg::OnBnClickedCheckLabel()
{
	DrawVolumeMap();
}


void KPathVolumeAreaDlg::DrawSelectedOrgDestination(bool bRefresh)
{
	try
	{
		std::vector<TDrawNodeInfo> vecSelectedDrawNodeInfo;

		GetSelectedDrawNodeInfo(vecSelectedDrawNodeInfo);

		m_pMapView->TxDrawCompositeNode(vecSelectedDrawNodeInfo, false, bRefresh);
		
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaDlg::GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear)
{
	try
	{
		if (a_bClear)
		{
			a_vecSelectedDrawNodeInfo.clear();
		}

		CXTPReportRecords*    pRecords(nullptr);
		CXTPReportRecord*     pRecord (nullptr);
		CXTPReportRecordItem* pItem   (nullptr);
		int                   nRecordCnt(0);
		Integer               nxZoneID(0);

		pRecords   = m_ctrlReportA.GetRecords();
		nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_A_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
				continue;

			nxZoneID = (Integer)pItem->GetItemData();

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			oNodeInfo.TColorRGB       = RGB(255,  66,  0);

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}

		pRecords   = m_ctrlReportB.GetRecords();
		nRecordCnt = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_B_COLUMN_SELECT);

			if (pItem->IsChecked() == FALSE)
				continue;

			nxZoneID = (Integer)pItem->GetItemData();

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID         = nxZoneID;

			CString strNodeLable(_T(""));
			strNodeLable.Format(_T("%I64d"), nxZoneID);
			oNodeInfo.TLabel          = strNodeLable;

			oNodeInfo.TSymbol = nZoneCharIndex;
			oNodeInfo.TSize           = nZoneSize;
			oNodeInfo.TColorRGB       = RGB(0,  128,  192);

			a_vecSelectedDrawNodeInfo.push_back(oNodeInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KPathVolumeAreaDlg::OnBnClickedCheckZone()
{
	try
	{
		DrawVolumeMap();
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
