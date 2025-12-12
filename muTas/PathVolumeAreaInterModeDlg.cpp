// PathVolumeAreaInterModeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PathVolumeAreaInterModeDlg.h"
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

namespace PathVolumeAreaInterModeENUM
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

using namespace PathVolumeAreaInterModeENUM;
// KPathVolumeAreaInterModeDlg 대화 상자입니다.

const int nZoneCharIndex = 53;
const int nZoneSize		 = 11;

IMPLEMENT_DYNAMIC(KPathVolumeAreaInterModeDlg, KResizeDialogEx)

KPathVolumeAreaInterModeDlg::KPathVolumeAreaInterModeDlg(KTarget* a_pTarget, KProject* a_pProject, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KPathVolumeAreaInterModeDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pMapView(a_pMapView)
	, m_strRunErrMsg(_T(""))
{
	m_dMaxVolume             = 0.0;
	m_dMinVolume             = 0.0;
	m_dMaxRatio              = 0.0;
	m_dMinRatio              = 0.0;

	m_clrAuto                = RGB ( 192, 0,   255 );
	m_clrTaxi                = RGB ( 255, 128,   0 );
	m_clrBus                 = RGB (   0, 128,   0 );
	m_clrRail                = RGB (   0,   0, 255 );
}

KPathVolumeAreaInterModeDlg::~KPathVolumeAreaInterModeDlg()
{
	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();	
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		pIOTable->Unregister(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	CloseWindowProc();
}

void KPathVolumeAreaInterModeDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_A,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT_B,                m_ctrlReportB);
	DDX_Control(pDX, IDC_CLR_AUTO,                m_cpAutoColor);
	DDX_Control(pDX, IDC_CLR_TAXI,                m_cpTaxiColor);
	DDX_Control(pDX, IDC_CLR_BUS,                 m_cpBusColor);
	DDX_Control(pDX, IDC_CLR_RAIL,                m_cpRailColor);
}


BEGIN_MESSAGE_MAP(KPathVolumeAreaInterModeDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2,        &KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearchAreaCodeOrg)
	ON_BN_CLICKED(IDC_BUTTON4,        &KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearchAreaCodeDest)
	ON_BN_CLICKED(IDC_BUTTON1,        &KPathVolumeAreaInterModeDlg::OnBnClickedButtonPolyDrawOrg)
	ON_BN_CLICKED(IDC_BUTTON3,        &KPathVolumeAreaInterModeDlg::OnBnClickedButtonPolyDrawDest)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_A,              &KPathVolumeAreaInterModeDlg::OnReportAItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_A, &KPathVolumeAreaInterModeDlg::OnReportACheckItem)
	ON_NOTIFY(NM_CLICK, IDC_REPORT_B,              &KPathVolumeAreaInterModeDlg::OnReportBItemClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_B, &KPathVolumeAreaInterModeDlg::OnReportBCheckItem)
	ON_BN_CLICKED(IDC_CHECK1, &KPathVolumeAreaInterModeDlg::OnBnClickedCheckASelectAll)
	ON_BN_CLICKED(IDC_CHECK2, &KPathVolumeAreaInterModeDlg::OnBnClickedCheckBSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH2, &KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearch)
	ON_WM_COPYDATA()
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KPathVolumeAreaInterModeDlg::OnBnClickedRadioVolumeType)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KPathVolumeAreaInterModeDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KPathVolumeAreaInterModeDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KPathVolumeAreaInterModeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KPathVolumeAreaInterModeDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK3, &KPathVolumeAreaInterModeDlg::OnBnClickedCheckZone)
END_MESSAGE_MAP()


// KPathVolumeAreaInterModeDlg 메시지 처리기입니다.

void KPathVolumeAreaInterModeDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);
		if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			m_ctrlReportA.ResetContent();
			m_ctrlReportB.ResetContent();
			ResetVolumeData();

			KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
			SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);

			KDBasePathVolume::GetInterModeInfo(m_pTarget, m_mapModeFieldInfo);
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


void KPathVolumeAreaInterModeDlg::ActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaInterModeDlg::DeActivatedFrameWndNotifyProcess()
{

}


void KPathVolumeAreaInterModeDlg::ResizeComponent()
{
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
	SetResize(IDC_STATIC18,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC14,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC15,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_AUTO,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_TAXI,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_BUS,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CLR_RAIL,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_CLEAR,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DRAW,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);		
	SetResize(IDC_CHECK3,			      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_ZONEACCESS,	      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	
	SetResize(IDC_SEPRATOR9,			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);		
	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KPathVolumeAreaInterModeDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
	try
	{
		ClearMapDraw(true);

		BOOL bSelectAll(FALSE);
		size_t nxCount = a_vecSelection.size();
		if (MAP_NODE_START == m_bPointFlag)
		{
			CheckDlgButton(IDC_CHECK1, BST_CHECKED);
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
			CheckDlgButton(IDC_CHECK2, BST_CHECKED);
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


BOOL KPathVolumeAreaInterModeDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReportA, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportB, FALSE, FALSE, FALSE);

	KColorPickerSetting::DefaultCtrl(m_cpAutoColor,     m_clrAuto);
	KColorPickerSetting::DefaultCtrl(m_cpTaxiColor,     m_clrTaxi);
	KColorPickerSetting::DefaultCtrl(m_cpBusColor,      m_clrBus);
	KColorPickerSetting::DefaultCtrl(m_cpRailColor,     m_clrRail);

	CWaitCursor wc;

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
	CheckDlgButton(IDC_CHECK2, BST_CHECKED);
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	HiddenMinMaxControl();
	CheckDlgButton(IDC_CHECK_LABEL, BST_UNCHECKED);
	CheckDlgButton(IDC_CHECK3, BST_CHECKED);

	SetDlgItemText(IDC_EDIT_MINSIZE,   _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAXSIZE,   _T("10.0"));

	KDBasePathVolume::GetZoneNodeInfo(m_pTarget, m_mapZoneNodeData);
	SetGuideNodeInfo(m_mapZoneNodeData, m_vecDrawGuideNode);

	KDBasePathVolume::GetInterModeInfo(m_pTarget, m_mapModeFieldInfo);

	InitReportHeader();
	m_ctrlReportA.Populate();
	m_ctrlReportB.Populate();

	ResizeComponent();

	try
	{
		AddImChampTooltip(IDC_RADIO2, _T("비율(%) = Mode Volume / Sum Mode Volume * 100"));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}  

	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();	
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);

		pIOTable->Register(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KPathVolumeAreaInterModeDlg::InitReportHeader()
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


void KPathVolumeAreaInterModeDlg::SetGuideNodeInfo(std::map<Integer, CString> &a_mapZoneNodeData, std::vector<TDrawNodeInfo> &a_vecDrawGuideNode)
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




void KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearchAreaCodeOrg()
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


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearchAreaCodeDest()
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


void KPathVolumeAreaInterModeDlg::UpdateReport(CXTPReportControl &a_ctrlReport, std::set<Integer> &a_setZoneID, BOOL a_bSelectAll)
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

	ResetVolumeData();
}


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonPolyDrawOrg()
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


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonPolyDrawDest()
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


void KPathVolumeAreaInterModeDlg::OnReportACheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaInterModeDlg::OnReportBCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
	}

	ResetVolumeData();
}


void KPathVolumeAreaInterModeDlg::OnReportAItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KPathVolumeAreaInterModeDlg::OnReportBItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KPathVolumeAreaInterModeDlg::OnBnClickedCheckASelectAll()
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

		ResetVolumeData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterModeDlg::OnBnClickedCheckBSelectAll()
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

		ResetVolumeData();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonSearch()
{
	try
	{
		GetCheckedOrgDestZone(m_ctrlReportA, _0_A_COLUMN_SELECT, m_setSelectedZoneOrg);
		if (m_setSelectedZoneOrg.size() < 1)
		{
			AfxMessageBox(_T("기점을 선택해 주세요."));
			return;
		}

		GetCheckedOrgDestZone(m_ctrlReportB, _0_B_COLUMN_SELECT, m_setSelectedZoneDest);
		if (m_setSelectedZoneDest.size() < 1)
		{
			AfxMessageBox(_T("종점을 선택해 주세요."));
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

		if (m_oInMapResultTripInfo.mapModeSumVolume.size() < 1)
		{
			AfxMessageBox(_T("선택한 구역간 경로 분석 결과가 존재하지 않습니다."));
			return;
		}

		UpdateMinMax();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterModeDlg::GetCheckedOrgDestZone(CXTPReportControl &a_ctrlReport, int a_nCheckBoxItemIndex, std::set<Integer> &a_setSelectedZone)
{
	try
	{
		a_setSelectedZone.clear();	

		CXTPReportRecords*    pRecords = a_ctrlReport.GetRecords();
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

			Integer nxZoneID = (Integer)pItem->GetItemData();

			a_setSelectedZone.insert(nxZoneID);
		}
	}
	catch (...)
	{
		a_setSelectedZone.clear();
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterModeDlg::ResetVolumeData()
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

		TInModePathResultInputValue oEmptyObject;
		m_oInMapResultTripInfo  = oEmptyObject; // 초기화
		m_oInMapResultRatioInfo = oEmptyObject;
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


unsigned __stdcall KPathVolumeAreaInterModeDlg::PathVolumeThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("구역간 수단별 통행량 분석 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Area-to-Area Mode Analysis Module..."));
	}

	KPathVolumeAreaInterModeDlg* pDlg = (KPathVolumeAreaInterModeDlg*)pParameter->GetParameter();
	pDlg->GetSearchPathVolume();

	return 0;
}


void KPathVolumeAreaInterModeDlg::GetSearchPathVolume()
{
	TxLogDebugStartMsg();
	m_strRunErrMsg = _T("");
	m_bResultRun   = true;
	try
	{
		if (QBicSimpleProgressThread::IsUserStopped() == true)
		{
			ThrowException(_T("사용자에 의해 구동이 중지되었습니다."));
		}
		
		m_oInMapResultTripInfo.setOriginZoneID      = m_setSelectedZoneOrg;
		m_oInMapResultTripInfo.setDestinationZoneID = m_setSelectedZoneDest;
		
		m_oInMapResultTripInfo.vecAvgSelectedPoints.clear();
		for (int i= 0; i< 2; i++)
		{
			TMapPoint oTMapPointAvg;
			if (0 == i) // Origin
				KDBasePathVolume::GetSelectedZoneAvgPoint(m_pTarget, m_setSelectedZoneOrg, oTMapPointAvg.Tx,  oTMapPointAvg.Ty);
			else
				KDBasePathVolume::GetSelectedZoneAvgPoint(m_pTarget,m_setSelectedZoneDest, oTMapPointAvg.Tx,  oTMapPointAvg.Ty);
			
			m_oInMapResultTripInfo.vecAvgSelectedPoints.push_back(oTMapPointAvg);
		}

		KDBasePathVolume::GetInterModeVolumeInfo(m_pTarget, m_mapModeFieldInfo, m_setSelectedZoneOrg, m_setSelectedZoneDest, m_oInMapResultTripInfo.mapModeSumVolume);
		SetVolMinMax(m_oInMapResultTripInfo, m_oInMapResultRatioInfo);

		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data..."));
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
		m_strRunErrMsg = _T("모듈 실행 중 예상치 못한 오류가 발생하였습니다.");
		m_bResultRun = false;
		return;
	}

	TxLogDebugEndMsg();
	m_bResultRun = true;
}


void KPathVolumeAreaInterModeDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


void KPathVolumeAreaInterModeDlg::InitVolMinMax()
{
	m_dMaxVolume  = 0.0;
	m_dMinVolume  = 0.0;
}


void KPathVolumeAreaInterModeDlg::SetVolMinMax(TInModePathResultInputValue &a_oMMapResultTripInfo, TInModePathResultInputValue &a_oMMapResultRatioInfo)
{
	if (a_oMMapResultTripInfo.mapModeSumVolume.size() < 1)
	{
		InitVolMinMax();
		InitRatioMinMax();
		return;
	}

	a_oMMapResultRatioInfo = a_oMMapResultTripInfo;

	double dMaxVolume = -999999999999.0;
	double dMinVolume = 999999999999.0;

	double dMaxRatio = -999999999999.0;
	double dMinRatio = 999999999999.0;
	
	double dTotalTrip(0.0);
	double dRatio(0.0);
	
	AutoType iterMode = a_oMMapResultTripInfo.mapModeSumVolume.begin();
	AutoType endMode  = a_oMMapResultTripInfo.mapModeSumVolume.end();
	while (iterMode != endMode)
	{
		int    nModeID   = iterMode->first;
		double &dTrip    = iterMode->second;

#pragma region dMinVolume, dMaxVolume 구하기
		if (dTrip >= 0)
		{
			if (dMinVolume > dTrip)
			{
				dMinVolume = dTrip;
			}
		}

		if (dMaxVolume < dTrip)
		{
			dMaxVolume = dTrip;
		}
#pragma endregion dMinVolume, dMaxVolume 구하기

		dTotalTrip += dTrip;

		++iterMode;
	}

	iterMode = a_oMMapResultTripInfo.mapModeSumVolume.begin();
	endMode  = a_oMMapResultTripInfo.mapModeSumVolume.end();
	a_oMMapResultRatioInfo.mapModeSumVolume.clear();
	while (iterMode != endMode)
	{
		int    nModeID   = iterMode->first;
		double &dTrip    = iterMode->second;

		dRatio = 0.0;
#pragma region dMinRatio, dMaxRatio 구하기

		if (dTotalTrip <= 0.0)
		{
			dRatio = 0.0;
		}
		else
		{
			dRatio = (dTrip/dTotalTrip) * 100.0;
		}

		if (dRatio >= 0.0)
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
#pragma endregion dMinRatio, dMaxRatio 구하기

		a_oMMapResultRatioInfo.mapModeSumVolume.insert(std::make_pair(nModeID, dRatio));

		++iterMode;
	}

	m_dMaxVolume = dMaxVolume;
	m_dMinVolume = dMinVolume;

	m_dMaxRatio  = dMaxRatio;
	m_dMinRatio  = dMinRatio;
}


void KPathVolumeAreaInterModeDlg::InitRatioMinMax()
{
	m_dMaxRatio = 0.0;
	m_dMinRatio = 0.0;
}


void KPathVolumeAreaInterModeDlg::UpdateMinMax()
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
	}
	strMinVol.Format(_T("%.0f"), dMinVol);

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
	dMinRatio = m_dMinRatio - 0.005;
	if(dMinRatio < 0.0)
		dMinRatio = 0.0;

	if (dMinRatio < m_dMinRatio)
	{
		dMinRatio = m_dMinRatio;
	}

	int nMinRatio = (int)dMinRatio;
	dMinRatio     = (double)nMinRatio; // double 버림 처리
	strMinRatio.Format(_T("%.2f"), dMinRatio);

	double dMaxRatio(0.0);
	if (0.0 == m_dMaxRatio)
	{
		dMaxRatio = m_dMaxRatio;
	}
	else
	{
		dMaxRatio = m_dMaxRatio + 0.006;
	}

	strMaxRatio.Format(_T("%.2f"), dMaxRatio);
	/*SetDlgItemText(IDC_EDIT_MIN_RATIO,  strMinRatio);*/
	SetDlgItemText(IDC_EDIT_MIN_RATIO,  _T("0.0"));
	SetDlgItemText(IDC_EDIT_MAX_RATIO,  strMaxRatio);
}


void KPathVolumeAreaInterModeDlg::HiddenMinMaxControl()
{
// 	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
// 	{
// 		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_SHOW);
// 
// 		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_HIDE);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_EDIT_MAX_VALUE)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_EDIT_MIN_VALUE)->ShowWindow(SW_HIDE);
// 
// 		GetDlgItem(IDC_EDIT_MAX_RATIO)->ShowWindow(SW_SHOW);
// 		GetDlgItem(IDC_EDIT_MIN_RATIO)->ShowWindow(SW_SHOW);
// 	}
}


void KPathVolumeAreaInterModeDlg::OnBnClickedRadioVolumeType(UINT nID)
{
	HiddenMinMaxControl();

	ClearMapDraw(false);
	DrawSelectedOrgDestination(true);
}


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonDraw()
{
	try
	{
		if (m_oInMapResultTripInfo.mapModeSumVolume.size() < 1)
		{
			AfxMessageBox(_T("경로 분석 결과가 존재하지 않습니다."));
			return;
		}

		if (UserVolumeInputCheck() == false)
		{
			return;
		}

		CWaitCursor cw;
 		TInModePathResultSettingInfo oTMResltSettingInfo;
 		GetResultSettingInfo(oTMResltSettingInfo);

		ClearMapDraw(false);

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
			KInterModalPathInfo::ResultAreaModeDrawInfo(m_pTarget, m_oInMapResultTripInfo, oTMResltSettingInfo, m_oMapResult);
		else
			KInterModalPathInfo::ResultAreaModeDrawInfo(m_pTarget, m_oInMapResultRatioInfo, oTMResltSettingInfo, m_oMapResult, true);

		bool bLabel(false);
		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bLabel = true;
		else
			bLabel = false;

		 // 기점, 종점
		DrawSelectedOrgDestination(false);

		if (IsDlgButtonChecked(IDC_CHECK_ZONEACCESS) == BST_CHECKED)
		{
			std::vector<TDrawZoneAccess> vecZoneAccess;
			std::vector<TDrawNodeInfo>&  vecDrawZoneNodeInfo = m_oMapResult.vecDrawZoneNodeInfo;
			size_t nxCount = vecDrawZoneNodeInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				TDrawNodeInfo&  oDrawNodeInfo   = vecDrawZoneNodeInfo[i];
				TDrawZoneAccess oDrawZoneAccess;
				oDrawZoneAccess.TNodeID         = oDrawNodeInfo.TNodeID;
				oDrawZoneAccess.TSymbol = oDrawNodeInfo.TSymbol;
				oDrawZoneAccess.TColorRGB       = oDrawNodeInfo.TColorRGB;
				oDrawZoneAccess.TLabel          = oDrawNodeInfo.TLabel;
				oDrawZoneAccess.TSize           = oDrawNodeInfo.TSize;
				oDrawZoneAccess.TRadius         = 1000.0;

				vecZoneAccess.push_back(oDrawZoneAccess);
			}
			m_pMapView->CacheDrawCompositeZoneAccess(vecZoneAccess, false);
		}

		m_pMapView->CacheDrawCompositeUserPoint(m_oMapResult.vecUserPointInfo, bLabel, false);
		m_pMapView->CacheDrawCompositeUserLine(m_oMapResult.vecUserLineInfo, false, true);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KPathVolumeAreaInterModeDlg::UserVolumeInputCheck()
{
	try
	{
		double dMinValue(0.0), dMaxValue(0.0);
		double dMinSize(0.0),  dMaxSize(0.0);

		CString strValue(_T(""));

// 		int nMinID;
// 		int nMaxID;
// 
// 		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
// 		{
// 			nMinID = IDC_EDIT_MIN_VALUE;
// 			nMaxID = IDC_EDIT_MAX_VALUE;
// 		}
// 		else
// 		{
// 			nMinID = IDC_EDIT_MIN_RATIO;
// 			nMaxID = IDC_EDIT_MAX_RATIO;
// 		}
// 
// 		//min value 체크
// 		GetDlgItemText(nMinID, strValue);
// 		if (QBicStringChecker::IsNumeric(strValue) == false)
// 		{
// 			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 			GetDlgItem(nMinID)->SetFocus();		
// 			return false;
// 		}
// 		dMinValue = _ttof(strValue);
// 
// 		//IDC_EDIT_MAX_VALUE
// 		GetDlgItemText(nMaxID, strValue);
// 		if (QBicStringChecker::IsNumeric(strValue) == false)
// 		{
// 			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 			GetDlgItem(nMaxID)->SetFocus();		
// 			return false;
// 		}
// 		dMaxValue = _ttof(strValue);

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

// 		if (dMinValue > dMaxValue)
// 		{
// 			AfxMessageBox(_T("최대값보다 작은 숫자를 입력해 주세요."));
// 			GetDlgItem(nMinID)->SetFocus();		
// 			return false;
// 		}

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


void KPathVolumeAreaInterModeDlg::GetResultSettingInfo(TInModePathResultSettingInfo &a_oTMResltSettingInfo)
{
	try
	{
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

		CString strMinSize(_T(""));
		CString strMaxSize(_T(""));
		GetDlgItemText(IDC_EDIT_MINSIZE, strMinSize);
		GetDlgItemText(IDC_EDIT_MAXSIZE, strMaxSize);
		double dMinSize = _ttof(strMinSize);
		double dMaxSize = _ttof(strMaxSize);

		CString strMinTrip(_T(""));
		CString strMaxTrip(_T(""));
		GetDlgItemText(nMinID, strMinTrip);
		GetDlgItemText(nMaxID, strMaxTrip);
		double dMinTrip = _ttof(strMinTrip);
		double dMaxTrip = _ttof(strMaxTrip);

		a_oTMResltSettingInfo.clrAuto      = m_cpAutoColor.GetColor();
		a_oTMResltSettingInfo.clrTaxi      = m_cpTaxiColor.GetColor();
		a_oTMResltSettingInfo.clrBus       = m_cpBusColor.GetColor();
		a_oTMResltSettingInfo.clrRail      = m_cpRailColor.GetColor();
		a_oTMResltSettingInfo.dMinTrip     = dMinTrip;
		a_oTMResltSettingInfo.dMaxTrip     = dMaxTrip;
		a_oTMResltSettingInfo.dMinSize     = dMinSize;
		a_oTMResltSettingInfo.dMaxSize     = dMaxSize;
		a_oTMResltSettingInfo.bUseOutRange = FALSE;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KPathVolumeAreaInterModeDlg::ClearMapDraw(bool bRefresh)
{
	try
	{
		m_pMapView->CacheClearCompositeThemes(false);
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


void KPathVolumeAreaInterModeDlg::OnBnClickedButtonClear()
{
	ClearMapDraw(false);
	DrawSelectedOrgDestination(true);
}


void KPathVolumeAreaInterModeDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KPathVolumeAreaInterModeDlg::CloseWindowProc()
{
	ClearMapDraw(true);
}


void KPathVolumeAreaInterModeDlg::OnBnClickedCheckLabel()
{
	try
	{
		bool bLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			bLabel = true;
		}
		else
		{
			bLabel = false;
		}

		//m_pMapView->CacheShowCompositeNodeLabel    (bLabel, false);
		//m_pMapView->CacheShowCompositeUserLineLabel(bLabel, true);
		m_pMapView->CacheDrawCompositeUserPointLabel(bLabel, true);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KPathVolumeAreaInterModeDlg::DrawSelectedOrgDestination(bool bRefresh)
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


void KPathVolumeAreaInterModeDlg::GetSelectedDrawNodeInfo(std::vector<TDrawNodeInfo> &a_vecSelectedDrawNodeInfo, bool a_bClear)
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

void KPathVolumeAreaInterModeDlg::OnBnClickedCheckZone()
{
	try
	{
// 		ClearMapDraw();
// 		if (IsDlgButtonChecked(IDC_CHECK3) == BST_CHECKED)
// 		{
// 			DrawSelectedOrgDestination();
// 		}
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
