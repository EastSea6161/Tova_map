// ShortestPathHighway.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "MainFrameWnd.h"
#include "ChildFrm.h"
#include "MapView.h"
#include "ShortestPathHighwayDlg.h"
#include "afxdialogex.h"
#include "Target.h"

#include "..\QBicCommon\ColorRangeGen.h"
#include "KEMNode.h"
#include "KExRecordItem.h"
#include "KBulkDBase.h"
#include "KBulkDBase2File.h"
#include "BulkDBaseTurn.h"
#include "DBaseAssignment.h"
#include "InspectionNetwork.h"
#include "ImChampDir.h"
#include "Launcher.h"
#include "ImChampMapDefine.h"

enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_NODE_END,
	_2_COLUMN_PATH_YN,
	_3_COLUMN_COST,
	_4_COLUMN_COLOR
};


// KShortestPathHighway 대화 상자입니다.

IMPLEMENT_DYNAMIC(KShortestPathHighwayDlg, KResizeDialogEx)

KShortestPathHighwayDlg::KShortestPathHighwayDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KShortestPathHighwayDlg::IDD, pParent)
	, m_nResultCode(0)
	, m_bStartZone(FALSE)
	, m_bEndZone(FALSE)
	, m_strSearchedIDs(_T(""))
    , m_bApplyTurn(false)
{
	m_nLinkColumnCount = 0;

	m_nxStartNodeID = -1;
	m_nxEndNodeID   = -1;

	m_strPreSelectedColumnName = _T("");

	m_oStartNodeIdCaption.TID = _T("-1");
}

KShortestPathHighwayDlg::~KShortestPathHighwayDlg()
{
	CloseWindowProc();
	m_pLinkTable->Unregister(this);
	m_backBrush.DeleteObject();
}

void KShortestPathHighwayDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                 m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1,                 m_cboCriterion);
	DDX_Control(pDX, IDC_CHECK_ZONESTART,        m_chkStartZone);
	DDX_Control(pDX, IDC_CHECK_ZONEEND,          m_chkEndZone);
	DDX_Control(pDX, IDC_CBO_COLORRAMP,          m_cboColorRamp);
}

BEGIN_MESSAGE_MAP(KShortestPathHighwayDlg, KResizeDialogEx)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT, OnReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT, OnReportColumnRButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT, OnReportCheckItem)
	ON_COMMAND(ID_SELECT_CANCEL, &KShortestPathHighwayDlg::OnSelectPathCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KShortestPathHighwayDlg::OnSelectAllPath)
	ON_CBN_SELCHANGE(IDC_COMBO1,                 &KShortestPathHighwayDlg::OnCbnSelchangeCriterion)
	ON_BN_CLICKED(IDC_BUTTON1, &KShortestPathHighwayDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDCANCEL,    &KShortestPathHighwayDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CBO_COLORRAMP,			 &KShortestPathHighwayDlg::OnCbnSelchangeCboColorramp)
	ON_BN_CLICKED(IDC_BUTTON4, &KShortestPathHighwayDlg::OnBnClickedBtnDraw)
	ON_BN_CLICKED(IDC_BUTTON5, &KShortestPathHighwayDlg::OnBnClickedBtnClear)
	ON_CBN_SETFOCUS(IDC_COMBO1, &KShortestPathHighwayDlg::OnCbnSetfocusCriterion)
	ON_BN_SETFOCUS(IDC_CHECK_ZONESTART, &KShortestPathHighwayDlg::OnBnSetfocusCheckZonestart)
	ON_BN_SETFOCUS(IDC_CHECK_ZONEEND, &KShortestPathHighwayDlg::OnBnSetfocusCheckZoneend)
	ON_CBN_SETFOCUS(IDC_CBO_COLORRAMP, &KShortestPathHighwayDlg::OnCbnSetfocusCboColorramp)
    ON_WM_COPYDATA()

	ON_BN_CLICKED(IDC_BUTTON6, &KShortestPathHighwayDlg::OnBnClickedButtonStartNodeSearch)
	ON_BN_CLICKED(IDC_BUTTON7, &KShortestPathHighwayDlg::OnBnClickedButtonEndNodeSearch)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK1, &KShortestPathHighwayDlg::OnBnClickedCheckSelectAll)
	ON_BN_CLICKED(IDC_CHECK_ZONESTART, &KShortestPathHighwayDlg::OnBnClickedCheckZonestart)
	ON_BN_CLICKED(IDC_CHECK_ZONEEND, &KShortestPathHighwayDlg::OnBnClickedCheckZoneend)
END_MESSAGE_MAP()


KMapView* KShortestPathHighwayDlg::GetMapView(KTarget* a_pTarget)
{
	KMapView* pMapView = NULL;
	unsigned int nID = a_pTarget->GetObjectID();

	CMainFrameWnd* pMainFrame  = (CMainFrameWnd*)AfxGetMainWnd();
	CChildFrame*   pChildFrame = pMainFrame->GetChildFrame(nID);

	if(NULL != pChildFrame)
	{
		pMapView = pChildFrame->GetMapView();
	}

	return pMapView;
}


void KShortestPathHighwayDlg::CloseWindowProc()
{
	AllClearShortestPath();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();
}


void KShortestPathHighwayDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
    try
    {
        CString strSubjectName(a_strSubjectName);

        if(strSubjectName.CompareNoCase(TABLE_LINK) == 0)
        {
            InitializeCriterionList();
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


void KShortestPathHighwayDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(!m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KShortestPathHighwayDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}

	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
		}
	}
}


BOOL KShortestPathHighwayDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
    
	CWaitCursor wc;
	KReportCtrlSetting::DefaultB(m_wndReportCtrl);

	InitReportHeader();
	m_wndReportCtrl.Populate();

    InitialCtrl();

	InitialColorRampCtrl();

	InitializeCriterionList();

	ResizeComponent();

	//SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);

	CheckDlgButton(IDC_CHECK1, BST_CHECKED);

	UpdateData(FALSE);

	m_pLinkTable->Register(this);
	m_backBrush.CreateSolidBrush(RGB(255,255,255));
	
	return TRUE;  
}


void KShortestPathHighwayDlg::ResizeComponent()
{
	SetResize(IDC_COMBO1,				  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_CHECK2,                 SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    
	SetResize(IDC_EDIT3,                  SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,                SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON1,				  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR4,			  SZ_TOP_LEFT,   SZ_TOP_RIGHT);			// 가로 라인

	//SetResize(IDC_CHECK_ZONEEND,          SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT4,                  SZ_TOP_LEFT,   SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON7,                SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_CBO_COLORRAMP,		  SZ_TOP_RIGHT , SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,				  SZ_TOP_LEFT,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_SEPRATOR3,			  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);			// 가로 라인

	SetResize(IDC_BUTTON5,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,				  SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDCANCEL,					  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KShortestPathHighwayDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_NODE_END,   _T("Destination"), 65));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetSortable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("종점"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_PATH_YN,    _T("PATH"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetSortable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("경로유무"));
    }

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_COST,       _T("Cost"), 35));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("비용"));
    }

	//pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_COLOR,      _T("색상"), 30));
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_COLOR,      _T("Color"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }
}


void KShortestPathHighwayDlg::InitializeCriterionList()
{
	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);

	KIOTable* pTable = pIOTables->FindTable(TABLE_LINK);
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	
	int nColumnCount = pColumns->ColumnCount();
	
	m_cboCriterion.SetRedraw(FALSE);
	m_cboCriterion.ResetContent();
	int nInputCount = 0;
	int nPreSelectedIndex = -1;
	for(int i= 0; i< nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		
		if( KEMIODataTypeDouble == pColumn->DataType() )
		{
			int nCur = m_cboCriterion.AddString(pColumn->Caption());
			m_cboCriterion.SetItemData( nCur, (DWORD_PTR)( pColumn ) );

			if( m_strPreSelectedColumnName.CompareNoCase(pColumn->Name()) == 0 )
			{
				nPreSelectedIndex = nCur;
			}
			nInputCount++;
		}
	}
	m_cboCriterion.SetRedraw(TRUE);

	if (nInputCount > 0)
	{
		if ( nPreSelectedIndex >= 0)
		{
			m_cboCriterion.SetCurSel(nPreSelectedIndex);
		}
		else
		{
			m_cboCriterion.SetCurSel(NODE_COMBO_IDX_INIT);
		}		
	}    
}


void KShortestPathHighwayDlg::InitialColorRampCtrl()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_COLOR_RAMP_A);
	m_colorRampImageList.Create(200, 18, ILC_COLOR24, 5, 5);
	m_colorRampImageList.Add(&bmp, RGB(0, 0, 0));

	m_cboColorRamp.SetImageList(&m_colorRampImageList);

	COMBOBOXEXITEM cbItem;
	::RtlSecureZeroMemory(&cbItem, sizeof(cbItem));
	cbItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;

	for( int i= 0; i< 5; i++ )
	{
		cbItem.iItem = i;
		cbItem.iImage = i;
		cbItem.iSelectedImage = i;
		m_cboColorRamp.InsertItem(&cbItem);
	}
	m_cboColorRamp.SetCurSel(INIT_COLOR_INDEX);
	SetColorRampInfo(INIT_COLOR_INDEX);
}

void KShortestPathHighwayDlg::InitialCtrl()
{
    if (KBulkDBaseTurn::IsTurn(m_pTarget)) {
        GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
        CheckDlgButton(IDC_CHECK2, BST_CHECKED);
    }
    else {
        GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
        CheckDlgButton(IDC_CHECK2, BST_UNCHECKED);
    }
}

void KShortestPathHighwayDlg::SetColorRampInfo(int nSelectIndex)
{
	switch(nSelectIndex)
	{
	case 0:
		m_clrStart = RGB ( 190, 207, 219 );
		m_clrEnd   = RGB (  84,  93, 100 );
		break;
	case 1 :
		m_clrStart = RGB ( 119, 221, 221 );
		m_clrEnd   = RGB (   8,  38,  67 );
		break;
	case 2 :
		m_clrStart = RGB ( 234, 158,  69 );
		m_clrEnd   = RGB ( 105,   2,   5 );
		break;
	case 3 :
		m_clrStart = RGB ( 255, 255, 102 );
		m_clrEnd   = RGB ( 102,   2, 153 );
		break;
	case 4 :
		m_clrStart = RGB ( 255, 102, 130 );
		m_clrEnd   = RGB (   0,  79, 153 );
		break;
	default:
		return;
		break;
	}

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	KColorRangeGen oColorRangeGen;
	oColorRangeGen.setStartColor(m_clrStart);
	oColorRangeGen.setEndColor  (m_clrEnd);
	oColorRangeGen.setSize(nRecordCount);
	oColorRangeGen.createColorRange();

	int nTotalRowCount = m_wndReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; ++i)
	{
		CXTPReportRow* pRow = m_wndReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItemColor = pRecord->GetItem(_4_COLUMN_COLOR);

		COLORREF    colorRef = oColorRangeGen.getNextColor();

		pItemColor->SetBackgroundColor(colorRef);
	}

	m_wndReportCtrl.RedrawControl();
}


void KShortestPathHighwayDlg::InitializeNode()
{
	CWaitCursor wc;

	m_oStartNodeIdCaption.TID = _T("-1");
	SetDlgItemText(IDC_EDIT3, _T(""));	

	InitDestination();

	m_mapNodeData.clear();
	m_mapZoneNodeData.clear();

	m_vecNodeIDCaption.clear();
	m_vecZoneNodeIDCaption.clear();

	GetNodeInfo();
	GetZoneNodeInfo();

	m_chkStartZone.SetCheck(FALSE);
	m_chkEndZone.SetCheck(FALSE);
}


bool KShortestPathHighwayDlg::GetNodeInfo(void)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT Node_Id, Name, Node_Type FROM %s ORDER BY Node_Id "), TABLE_NODE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return false;
	}

	m_mapNodeData.clear();
	m_vecNodeIDCaption.clear();

	Integer nxNodeId		= 0;
	CString strNodeName		= _T("");
	int		nNodeType		= 0;
	CString strNodeID(_T(""));

	while( spResult->Next() )
	{
		nxNodeId	= spResult->GetValueInt64 (0);
		strNodeName = spResult->GetValueString(1);
		nNodeType	= spResult->GetValueInt   (2);

		m_mapNodeData.insert(std::make_pair(nxNodeId, strNodeName));

		KIDCaption oIDCaption;
		strNodeID.Format(_T("%I64d"), nxNodeId);
		oIDCaption.TID      = strNodeID;
		oIDCaption.TCaption = strNodeName;

		m_vecNodeIDCaption.push_back(oIDCaption);
	}

	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathHighwayDlg::GetZoneNodeInfo(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name, full_name FROM %s ORDER BY zone_id "), TABLE_ZONE);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if(nullptr == spResult)
		{
			return false;
		}

		m_mapZoneNodeData.clear();
		m_vecZoneNodeIDCaption.clear();

		Integer nxNodeId(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));
		CString strNodeFullName(_T(""));

		while( spResult->Next() )
		{
			nxNodeId	    = spResult->GetValueInt64 (0);
			strNodeName     = spResult->GetValueString(1);
			strNodeFullName	= spResult->GetValueString(2);

			m_mapZoneNodeData.insert(std::make_pair(nxNodeId, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxNodeId);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;

			m_vecZoneNodeIDCaption.push_back(oIDCaption);
		}
	}
	catch (...)
	{
		return false;
	}

	TxLogDebugEndMsg();
	return true;
}


void KShortestPathHighwayDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	if( pItemNotify->pItem->IsChecked() == FALSE )
	{
		CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
	}
}


void KShortestPathHighwayDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

    if (!pItemNotify->pRow || !pItemNotify->pColumn)
        return;

	//SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		return;
	}

	if( _4_COLUMN_COLOR == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());
			m_wndReportCtrl.RedrawControl();
		}
	}
	else
	{
		CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();
		CXTPReportRecordItem* pItem   = pRecord->GetItem(_1_COLUMN_NODE_END);
		Integer               nxNodeID= pItem->GetItemData();

		ZoomToNodeFeature(nxNodeID);
	}
}


void KShortestPathHighwayDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			pMapView->PanToFeature(KLayerID::Node(), a_nxNodeID );
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathHighwayDlg::OnReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
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


void KShortestPathHighwayDlg::OnSelectPathCancel()
{
	PathCheckboxControl( FALSE );
	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
}


void KShortestPathHighwayDlg::OnSelectAllPath()
{
	PathCheckboxControl( TRUE );
	CheckDlgButton(IDC_CHECK1, BST_CHECKED);
}


void KShortestPathHighwayDlg::PathCheckboxControl(BOOL a_bCheck)
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


void KShortestPathHighwayDlg::OnCbnSelchangeCriterion()
{
	UpdateData(TRUE);

	CString strCriterionData = _T("");
	int nSelCur	= m_cboCriterion.GetCurSel();

	KIOColumn* pSelColumn      = (KIOColumn*)(m_cboCriterion.GetItemData(nSelCur));
	if(NULL == pSelColumn)
	{
		return;
	}

	m_strPreSelectedColumnName = pSelColumn->Name();

	InitDestination();
}


void KShortestPathHighwayDlg::UpdateEndNodeID( std::set<CString> &a_setSelectedID )
{
	InsertLinkReportData(m_nxStartNodeID, a_setSelectedID );

	AllClearShortestPath();
}


void KShortestPathHighwayDlg::OnBnClickedBtnSearch()
{
    ImChampDir::SetCurrentDirectoryToApp();

	m_nResultCode = DLL_UNEXPECTED_ERROR;

	m_nCriterionIndex = GetSelCriterionIndex(m_oUserColumnsLink);
	if(-1 == m_nCriterionIndex)
	{
		return;
	}

	m_nxStartNodeID = GetSelStartNodeId();
	if(-1 == m_nxStartNodeID)	
	{
		AfxMessageBox(_T("기점을 선택해 주세요."));
		return;
	}

	// 도착정보 초기화
	InitDestination();

    m_bApplyTurn = false; {
        if (IsDlgButtonChecked(IDC_CHECK2) == BST_CHECKED) {
            m_bApplyTurn = true;
        }
        else {
            m_bApplyTurn = false;
        }
    }

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(ShortestPathThreadCaller, this, true, nLang);
	
	if(DLL_PLANNED_TERMINATION == m_nResultCode )// 성공
	{
		CString strMsg(_T(""));
		strMsg.Format(_T("선택된 기점(%I64d)에 대한 경로 탐색이 완료되었습니다.\r\n종점을 선택해 주세요."), m_nxStartNodeID);

		AfxMessageBox(strMsg);	
	}
	else if ( DLL_USER_STOPPING == m_nResultCode)
	{
		AfxMessageBox(_T("모듈 실행이 중단되었습니다."));
		m_mapPathLinkInfo.clear();
		m_mapPathNodeInfo.clear();
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("DLL LOAD ERROR..."));
		m_mapPathLinkInfo.clear();
		m_mapPathNodeInfo.clear();
	}
	else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
		m_mapPathLinkInfo.clear();
		m_mapPathNodeInfo.clear();
	}
	else if (DLL_READ_OUTPUTFILE_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("실행 결과 데이터 입력에 실패하였습니다."));
		m_mapPathLinkInfo.clear();
		m_mapPathNodeInfo.clear();
	}
	else if (DLL_PLANNED_TERMINATION != m_nResultCode)
	{
		AfxMessageBox(_T("경로 탐색 중 오류가 발생하였습니다."));
		m_mapPathLinkInfo.clear();
		m_mapPathNodeInfo.clear();
	}

	
	//jyk: search 한 이후에는 전체 노드를 데이터를 가져 온다. 
	//존 체크가 되있다면, 존 데이터 만을 보여 줄것이고, 체크가 해제되있다면, 일반 데이터 만을 보여 줄것이다.
	//그러나 체크여부와 상관없이 m_setSearchedID에는 전체 노드 데이터를 담는다.
	//이유는 체크상태일땐 m_setSearchedID에서 존 만을 추출하여 보여 줄것이고, 체크 해제 일때는 전체를 보여 주기 위한 컨트롤을 해야 하는데,
	//이를 위해 검색된 원래 데이터set을 가지고 있어야 하기 때문이다.
// 	CWaitCursor cw;
// 	CString strIDs(_T(""));
// 	std::set<CString> setID ;
// 	strIDs = GetInitTotalID(m_mapNodeData, m_setSearchedID); 
// 	if (IsDlgButtonChecked(IDC_CHECK_ZONEEND) == BST_CHECKED)
// 	{
// 		strIDs = GetInitTotalID(m_mapZoneNodeData, setID);
// 	}
// 	else
// 	{
// 		setID = m_setSearchedID;
// 	}
// 	SetDlgItemText(IDC_EDIT4, strIDs);
// 	UpdateEndNodeID(setID);
}


CString KShortestPathHighwayDlg::GetInitTotalID(KTDNodeDataMap &a_mapNodeData, std::set<CString> &a_setID)
{
	a_setID.clear();

	CString strReturn(_T(""));

	try
	{
		AutoType iter  = a_mapNodeData.begin();
		AutoType itEnd = a_mapNodeData.end();

		CString strID(_T(""));
		bool bFirst(true);
		while(iter != itEnd)
		{
			if (bFirst)
			{
				strReturn.AppendFormat(_T("%I64d"),  iter->first);
				bFirst = false;
			}
			else
			{
				strReturn.AppendFormat(_T(",%I64d"), iter->first);
			}

			strID.Format(_T("%I64d"), iter->first);
			a_setID.insert(strID);

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

	return strReturn;
}


unsigned __stdcall KShortestPathHighwayDlg::ShortestPathThreadCaller(void* p)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("도로 최단 경로 탐색 모듈 실행 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Run Highway Path Find Module..."));
	}

	KShortestPathHighwayDlg* pDlg = (KShortestPathHighwayDlg*)pParameter->GetParameter();
	pDlg->GetSearchShortestPath();
	return 0;
}

void KShortestPathHighwayDlg::GetSearchShortestPath()
{
	TxLogDebugStartMsg();

	if(! CreateArgument(m_oUserColumnsLink))
	{
		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		return;
	}

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		m_nResultCode = DLL_USER_STOPPING;
		return;
	}

    CString strWindowName;
    GetWindowText(strWindowName);

	if (QBicSimpleProgressThread::IsUserStopped() == true)
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

    KLauncher::Run(strWindowName, 1);

	if (QBicSimpleProgressThread::IsUserStopped() == true)
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

	if( DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("최단 경로 탐색(도로) 모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Path Find(Highway) Module Complete."));
		}
		TxLogDebug(_T("Shortest Path Run Ok.."));
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Shortest Path Run Error.."));
		return;
	}
	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode =  DLL_USER_STOPPING;
		return;
	}
	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}
	if (! ReadOutPutFile() )
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("데이터 저장 중 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Saving Output Data Failed."));
		}
		TxLogDebug(_T("Output Path 정보 입력 실패..."));
		m_nResultCode = DLL_READ_OUTPUTFILE_ERROR;
		return;
	}

	TxLogDebugEndMsg();
}


void KShortestPathHighwayDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KShortestPathHighwayDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


bool KShortestPathHighwayDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();
    ImChampDir::SetCurDirToTargetModel(m_pTarget);
	if(false == ReadNodeFile())
	{
		return false;
	}

	if(false == ReadLinkFile())
	{
		return false;
	}
    ImChampDir::SetCurrentDirectoryToApp();
	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathHighwayDlg::ReadNodeFile()
{
	TxLogDebugStartMsg();

	try
	{
		// node
		std::ifstream ifs( T_DAT_OUT_HSHORTEST_NODE, std::ios::binary );    

		if ( ifs ) 
		{      
			TxLogDebug(_T("--- start insert node result"));
			int nCount = 0;
			m_mapPathNodeInfo.clear();

			while(! ifs.eof())
			{
				TPathNodeInfo oTPathNodeInfo;
                ifs.read( reinterpret_cast<char*>( &oTPathNodeInfo),        sizeof(TPathNodeInfo)  );
				Integer& nxNodeID = oTPathNodeInfo.nxNodeId;
				m_mapPathNodeInfo.insert( std::make_pair(oTPathNodeInfo.nxNodeId, oTPathNodeInfo));

				KIDCaption oIDCaption;
				CString   &strNodeID = oIDCaption.TID;
				strNodeID.Format(_T("%I64d"), nxNodeID);

				/*CString strMsg;
				strMsg.Format(_T(">> Node : %I64d, %I64d, %f"), oTPathNodeInfo.nxNodeId, oTPathNodeInfo.nxBackLinkId, oTPathNodeInfo.dCost);
				TxLogDebug((LPCTSTR)strMsg );*/

				nCount++;
			}
			CString strResult = _T("");
			strResult.Format(_T("--- end insert node result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			ifs.close();
			m_mapPathNodeInfo.clear();
			TxLogDebugException();
			return false;
		}

		ifs.close();
	}
	catch (...)
	{
		m_mapPathNodeInfo.clear();
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathHighwayDlg::ReadLinkFile()
{
	TxLogDebugStartMsg();

	try
	{
		// link
		std::ifstream ifsLink( T_DAT_OUT_HSHORTEST_LINK, std::ios::binary );    

		if ( ifsLink ) 
		{      
			TxLogDebug(_T("--- start insert link result"));
			int nCount = 0;
			m_mapPathLinkInfo.clear();
			while(! ifsLink.eof())
			{
				TPathLinkInfo oTPathLinkInfo;

                ifsLink.read( reinterpret_cast<char*>(&oTPathLinkInfo),	  sizeof(TPathLinkInfo) );
				m_mapPathLinkInfo.insert( std::make_pair(oTPathLinkInfo.nxLinkId, oTPathLinkInfo));

              /*  CString strMsg;
                strMsg.Format(_T("BackLink : %I64d, %I64d, %f"), oTPathLinkInfo.nxLinkId, oTPathLinkInfo.nxBackLinkId, oTPathLinkInfo.dCost);
                TxLogDebug((LPCTSTR)strMsg );*/
				nCount++;
			}
			CString strResult = _T("");
			strResult.Format(_T("--- end insert link result size : %d"), nCount);
			TxLogDebug( (LPCTSTR)strResult );
		}
		else
		{
			ifsLink.close();
			m_mapPathLinkInfo.clear();
			TxLogDebugException();
			return false;
		}

		ifsLink.close();
	}
	catch (...)
	{
		m_mapPathLinkInfo.clear();
		TxLogDebugException();
		return false;
	}    

	TxLogDebugEndMsg();
	return true;
}


void KShortestPathHighwayDlg::InsertLinkReportData(Integer a_nxStartNodeID, std::set<CString> &a_setSelectedID )
{
	//- 종점 정보 
	BOOL bSelectCheck = TRUE;
	KTDNodeDataMap mapEndNodeData; 

	if (TRUE == m_bEndZone)
	{
		mapEndNodeData = m_mapZoneNodeData;
	}
	else
	{
		mapEndNodeData = m_mapNodeData;
	}

	m_vecPathInfo.clear();
	m_wndReportCtrl.ResetContent(TRUE);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	int nIdx = 0;

	Integer nxSelectEndNode = 0;
	CString strExistPath    = _T("");
	double  dTotalCost      = 0.0;

	AutoType iter = a_setSelectedID.begin();
	AutoType end  = a_setSelectedID.end();

	KMunsellColor* pMunSellColor = new KMunsellColor(0, true);
	COLORREF    colorRef;

	BOOL bSelectAll(FALSE);
	if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
	{
		bSelectAll = TRUE;
	}
	 
	while( iter != end)
	{
		nxSelectEndNode = _ttoi64(*iter);

		AutoType iterExist = m_mapPathNodeInfo.find(nxSelectEndNode);

		// 결과 path 존재 여부
		if(iterExist != m_mapPathNodeInfo.end())
		{
			strExistPath = _T("Y");
			dTotalCost   = iterExist->second.dCost;
		}
		else
		{
			strExistPath = _T("N");
			dTotalCost   = 0.0;
		}

		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

		TPathInfo oTPathInfo;
		oTPathInfo.nxNodeIdStart = a_nxStartNodeID;
		oTPathInfo.nxNodeIdEnd   = nxSelectEndNode;

		m_vecPathInfo.push_back(oTPathInfo);

		// - 선택
		pItem = pRecord->AddItem(new KExRecordItemCheck());
		if(iterExist != m_mapPathNodeInfo.end())
		{
			pItem->HasCheckbox(TRUE);
			pItem->SetAlignment(xtpColumnIconCenter);
		}
		else
		{
			pItem->HasCheckbox(FALSE);
			pItem->SetCaption(_T("-"));
			pItem->SetAlignment(xtpColumnTextCenter);
		}
		pItem->SetChecked(bSelectAll);
		pItem->SetItemData((DWORD_PTR)nIdx);

		// - 종점
		CString strEndNodeViewData(_T(""));
		CString strEndNodeName    (_T(""));

		AutoType iterB = mapEndNodeData.find(nxSelectEndNode);
		if (iterB != mapEndNodeData.end())
		{
			strEndNodeName = iterB->second;
		}

		SetNodeViewData(nxSelectEndNode, strEndNodeName, strEndNodeViewData);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strEndNodeViewData ));
		pItem->SetItemData((DWORD_PTR) nxSelectEndNode);
		pItem->SetAlignment(xtpColumnTextLeft);

		// - 결과 Path 존재 여부
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strExistPath ));
		pItem->SetAlignment(xtpColumnTextCenter);

		// - 비용
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTotalCost, _T("%.2f")));
		pItem->SetAlignment(xtpColumnTextRight);

		// - 색상 
		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		colorRef = pMunSellColor->GetColor(nIdx);
		pItem->SetBackgroundColor(colorRef);

		++nIdx;
		++iter;
	}

	m_wndReportCtrl.Populate();

	m_cboColorRamp.SetCurSel(INIT_COLOR_INDEX);
	//SetColorRampInfo(INIT_COLOR_INDEX);

	delete pMunSellColor;
	UpdateData(FALSE);
}


bool KShortestPathHighwayDlg::CreateArgument( KIOColumns &a_oUserColumnsLink)
{
	TxLogDebugStartMsg();

	if(! ClearFiles())
	{
		return false;
	}
	//TxLogDebug("END CLEAR FILE ------ ");

    ImChampDir::SetCurrentDirectoryToApp();
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_HSHORTESTPATH, nOpenMode);

	bool bResult = false;

	CString strDirectory;
	strDirectory.Format(_T("%s\\%s\\"),m_pTarget->GetLocation(),DLL_MODEL_FOLDER_NAME);
	CreateDirectory(strDirectory, NULL);

	// .arg 작성
	bResult = WriteShortestPathArgument(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(WriteShortestPathArgument) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	of.Close();

	std::set<int> setIncludeNodeCode;
	std::set<int> setIncludeLinkCode;
	std::set<Integer> setUsableNodeData;
	std::set<Integer> setUsableLinkData;
	std::set<Integer> setUsableTurnData;


	KDBaseAssignment::GetHighwayTypeData(m_pTarget, setIncludeNodeCode, setIncludeLinkCode);
    KInspectionNetwork::Inspection(m_pTarget, setIncludeNodeCode, setIncludeLinkCode, setUsableNodeData, setUsableLinkData, setUsableTurnData);

	ImChampDir::SetCurDirToTargetModel(m_pTarget);
    CString strModelLocation = m_pTarget->GetLocation() + _T("\\") + DLL_MODEL_FOLDER_NAME + _T("\\");
    CString strNodeFileName, strLinkFileName, strTurnFileName;

    strNodeFileName = strModelLocation + _T("node.dat");
    strLinkFileName = strModelLocation + _T("link.dat");
    strTurnFileName = strModelLocation + _T("turn.dat");

	// 링크파일 생성
	bResult = KBulkDBase2File::ExportDB2LinkfileExcludeTypeByPathFind(m_pTarget->GetDBaseConnection(), a_oUserColumnsLink, setUsableLinkData, strLinkFileName);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2LinkFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = KBulkDBase2File::ExportDB2NodeFileExcludeType(m_pTarget->GetDBaseConnection(), setUsableNodeData, strNodeFileName);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = KBulkDBase2File::ExportDB2TurnFileExcludeType(m_pTarget->GetDBaseConnection(), setUsableTurnData, strTurnFileName);
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2TurnFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

    ImChampDir::SetCurrentDirectoryToApp();
	TxLogDebugEndMsg();
	return true;
}


bool KShortestPathHighwayDlg::ClearFiles()
{
	TxLogDebugStartMsg();
	
	int nResult = -1;

	try
	{
        ImChampDir::SetCurrentDirectoryToApp();
		nResult = remove(ARG_HSHORTESTPATH);
        ImChampDir::SetCurDirToTargetModel(m_pTarget);
		nResult = remove(DAT_NODE);
		nResult = remove(DAT_LINK);
		nResult = remove(DAT_TURN);

		// 결과 파일 삭제 
		nResult = remove(DAT_OUT_HSHORTEST_NODE);
		nResult = remove(DAT_OUT_HSHORTEST_LINK);

        ImChampDir::SetCurrentDirectoryToApp();
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KShortestPathHighwayDlg::WriteShortestPathArgument(CStdioFileEx& of)
{
	CString strOut = _T("");
	
	try
	{
		strOut.Format(_T("%I64d\t%d\t%d\r\n"), m_nxStartNodeID, m_nLinkColumnCount, m_nCriterionIndex);
		of.WriteString(strOut);

        int nTurn(0); {
            if (m_bApplyTurn) {
                nTurn = 1;
            }
            else {
                nTurn = 0;
            }
        }
        strOut.Format(_T("%d\r\n"), nTurn);
        of.WriteString(strOut);

        CString strFolder = m_pTarget->GetLocation() + _T("\\") + DLL_MODEL_FOLDER_NAME+ _T("\\"); /*+ _T("\\");*/
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


int KShortestPathHighwayDlg::GetSelCriterionIndex( KIOColumns &a_oUserColumnsLink )
{
	UpdateData(TRUE);

	CString strCriterionData = _T("");
	int nSelCur	= m_cboCriterion.GetCurSel();
	
	if(nSelCur == -1)
	{
		AfxMessageBox(_T("경로 탐색 기준을 선택해 주세요."));
		m_cboCriterion.SetFocus();

		return -1;
	}

	KIOColumn* pSelColumn   = (KIOColumn*)(m_cboCriterion.GetItemData(nSelCur));
	LPCTSTR	   lpcszSelName = pSelColumn->Name();

	KIOColumns fixedLinkColumns = KBulkDBase2File::GetFixedLinkColumns();

	int nIndex = fixedLinkColumns.GetColumnIndex(lpcszSelName);
	int nCount = fixedLinkColumns.ColumnCount();

	// none fixed column( user column )
	a_oUserColumnsLink.Clear();
	if( -1 == nIndex )
	{
		a_oUserColumnsLink.AddNewColumn(pSelColumn);
		m_nLinkColumnCount = nCount +1;

		return nCount;
	}
	// fixed column 
	else
	{
		m_nLinkColumnCount = nCount;

		return nIndex;
	}
}


Integer KShortestPathHighwayDlg::GetSelStartNodeId()
{
	Integer nxNodeId      = -1;

	try
	{
		nxNodeId = _ttoi64(m_oStartNodeIdCaption.TID);
	}
	catch (KExceptionPtr ex)
	{
		return -1;
		TxLogDebugException();
	}
	catch (...)
	{
		return -1;
		TxLogDebugException();
	}

	return nxNodeId;
}


void KShortestPathHighwayDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampIDFind::IDD == nDialogID)
	{
		if (m_spImTasIDFind)
		{
			try
			{
				Integer nxOrgZoneID = m_spImTasIDFind->GetSelectedID();
				CString strID(_T(""));
				strID.Format(_T("%I64d"), nxOrgZoneID);

				CString    strNodeView(_T(""));
				KIDCaption oIDCaption;
				if (TRUE == m_bStartZone)
				{
					SetNodeViewData(m_mapZoneNodeData, strID, strNodeView, oIDCaption);
					CheckDlgButton(IDC_CHECK_ZONESTART, BST_CHECKED);
				}
				else
				{
					SetNodeViewData(m_mapNodeData,     strID, strNodeView, oIDCaption);
					CheckDlgButton(IDC_CHECK_ZONESTART, BST_UNCHECKED);
				}

				m_oStartNodeIdCaption = oIDCaption;
				SetDlgItemText(IDC_EDIT3,  strNodeView);

				InitDestination();
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
	else if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				m_strSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();
				SetDestinationEditBox(m_strSearchedIDs);

				m_setSearchedID.clear();
				m_setSearchedID = m_spImTasMultiIDFind->GetSelectedIDSet();
				if (m_setSearchedID.size() < 1)
				{
					m_wndReportCtrl.ResetContent();
					m_wndReportCtrl.Populate();
				}
				else
				{
 					UpdateEndNodeID(m_setSearchedID);
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KShortestPathHighwayDlg::SetDestinationEditBox(CString &a_strSearchedIDs)
{
	try
	{
		if (a_strSearchedIDs.Compare(_T("")) != 0)
		{
			const int nLimitLength(500);
			if (a_strSearchedIDs.GetLength() > nLimitLength)
			{
				a_strSearchedIDs = a_strSearchedIDs.Left(nLimitLength);
				a_strSearchedIDs.Append(_T("..."));
			}
		}
			
		SetDlgItemText(IDC_EDIT4, a_strSearchedIDs);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KShortestPathHighwayDlg::SetNodeViewData(KTDNodeDataMap &a_mapNodeData, CString a_strZoneID, CString &a_strNodeViewData, KIDCaption &a_oIDCaption)
{
	try
	{
		Integer nxZoneID = _ttoi64(a_strZoneID);

		AutoType iter  = a_mapNodeData.find(nxZoneID);
		AutoType itEnd = a_mapNodeData.end();

		CString strName(_T(""));
		if (iter != itEnd)
		{
			strName = iter->second;
		}

		a_oIDCaption.TID      = a_strZoneID;
		a_oIDCaption.TCaption = strName;

		ConvertEmptyNodeName(strName);

		a_strNodeViewData.Format(_T("%s [%s]"), a_strZoneID, strName);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KShortestPathHighwayDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KShortestPathHighwayDlg::OnBnClickedBtnClear()
{
	PathCheckboxControl( FALSE );
	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);

	AllClearShortestPath();
}


void KShortestPathHighwayDlg::AllClearShortestPath()
{
	try
	{
		KMapView* pMapView = GetMapView(m_pTarget);
		if(NULL != pMapView)
		{
			//pMapView->ClearCompositeThemes();

			pMapView->ClearShortestPath();
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathHighwayDlg::OnBnClickedBtnDraw()
{
	try
	{
		DrawShortestPath();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KShortestPathHighwayDlg::DrawShortestPath()
{
	std::vector<TPathInfo> vecCheckedPathInfo;

	CXTPReportRecords*	  pExtRecords   = NULL;
	CXTPReportRecord*     pExtRecord    = NULL;
	CXTPReportRecordItem* pExtItemCheck = NULL;
	CXTPReportRecordItem* pItemNode		= NULL;
	CXTPReportRecordItem* pItemPath     = NULL;
	CXTPReportRecordItem* pExtItemColor = NULL;

	pExtRecords      = m_wndReportCtrl.GetRecords();
	int nRecordCount = pExtRecords->GetCount();

	for(int i= 0; i< nRecordCount; i++)
	{
		pExtRecord    = pExtRecords->GetAt(i);
		pExtItemCheck = pExtRecord->GetItem(_0_COLUMN_CHECK);
		pItemNode     = pExtRecord->GetItem(_1_COLUMN_NODE_END);

		if( pExtItemCheck->GetHasCheckbox() == TRUE && pExtItemCheck->IsChecked() == TRUE )
		{
			pExtItemColor        = pExtRecord->GetItem(_4_COLUMN_COLOR);
			int nArrayIndex      = pExtItemCheck->GetItemData();
			TPathInfo oTLinkInfo = m_vecPathInfo[nArrayIndex];
			oTLinkInfo.color     = pExtItemColor->GetBackgroundColor();

			Integer nxEndNodeID  = pItemNode->GetItemData();
			MakeLinkVector(oTLinkInfo.vecLink, nxEndNodeID);

			vecCheckedPathInfo.push_back(oTLinkInfo);
		}
	}

	int nSelectedCount = vecCheckedPathInfo.size();
	if( nSelectedCount <= 0 )
	{
		AfxMessageBox(_T("선택한 경로 정보가 없습니다."), MB_ICONINFORMATION);
		return;
	}

	/*// 체크된 항목들로 상단 자동 정렬
	CXTPReportColumns* pColumns = m_wndReportCtrl.GetColumns();
	CXTPReportColumn*  pCheckColumn = pColumns->GetAt(_0_COLUMN_CHECK);
	pColumns->SetSortColumn(pCheckColumn, FALSE);
	m_wndReportCtrl.SetTopRow(0);*/

	m_wndReportCtrl.Populate();

	AllClearShortestPath();
	
	KMapView* pMapView = GetMapView(m_pTarget);
	if(NULL != pMapView)
	{
		pMapView->DrawShortestPath (vecCheckedPathInfo);
	}
}


void KShortestPathHighwayDlg::MakeLinkVector(std::vector<Integer/*LinkID*/> &a_vecLink, Integer a_nxNodeID)
{
    std::map<Integer, TPathNodeInfo>::iterator iterNode = m_mapPathNodeInfo.find(a_nxNodeID);
    std::map<Integer, TPathNodeInfo>::iterator endNode  = m_mapPathNodeInfo.end();
	
    if (iterNode == endNode)
    {
        return;
    }

	TPathNodeInfo& oTPathNodeInfo = iterNode->second;
	Integer        nxBackLinkId   = oTPathNodeInfo.nxBackLinkId;	
	
    std::map<Integer, TPathLinkInfo>::iterator iterLink;
    std::map<Integer, TPathLinkInfo>::iterator endLink = m_mapPathLinkInfo.end();
	while(nxBackLinkId > 0)
	{
		a_vecLink.push_back(nxBackLinkId);	

        iterLink = m_mapPathLinkInfo.find(nxBackLinkId);
        if (iterLink != endLink)
        {
            TPathLinkInfo& oTPathLinkInfo = iterLink->second;
            nxBackLinkId = oTPathLinkInfo.nxBackLinkId;
        }
        else
        {
            break;
        }
	}
}


void KShortestPathHighwayDlg::OnCbnSelchangeCboColorramp()
{
	this->SetFocus();
	SetColorRampInfo(m_cboColorRamp.GetCurSel());
}


void KShortestPathHighwayDlg::ConvertEmptyNodeName(CString &a_strNodeName)
{
	a_strNodeName = a_strNodeName.IsEmpty() == TRUE ? _T(" - ") : a_strNodeName;
}


void KShortestPathHighwayDlg::SetNodeViewData(const Integer &a_nxNodeID, CString &a_strNodeName, CString &a_strNodeViewData)
{
	ConvertEmptyNodeName(a_strNodeName);

	a_strNodeViewData.Format(_T("%I64d [%s]"), a_nxNodeID, a_strNodeName);
}


void KShortestPathHighwayDlg::OnCbnSetfocusCriterion()
{
	//SendMessage(DM_SETDEFID, IDC_BUTTON1, 0);
}


void KShortestPathHighwayDlg::OnBnSetfocusCheckZonestart()
{
	//SendMessage(DM_SETDEFID, IDC_BUTTON1, 0);
}


void KShortestPathHighwayDlg::OnBnSetfocusCheckZoneend()
{
	//SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);
}


void KShortestPathHighwayDlg::OnCbnSetfocusCboColorramp()
{
	//SendMessage(DM_SETDEFID, IDC_BUTTON4, 0);
}


BOOL KShortestPathHighwayDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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


BOOL KShortestPathHighwayDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN  == pMsg->message)
	{		
		if (VK_RETURN == pMsg->wParam)
		{
			HWND hwnd = pMsg->hwnd;
			
			if (hwnd == GetDlgItem(IDC_BUTTON6)->m_hWnd   ||
				hwnd == GetDlgItem(IDC_BUTTON7)->m_hWnd   || 
				hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd     ||
				hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd     || 
				hwnd == GetDlgItem(IDC_COMBO1)->m_hWnd)
			{
				OnBnClickedBtnSearch();
				return TRUE;
			}
		}
	}
	else if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd)
		{
			ClickedEditStartNode();
			return TRUE;
		}
		else if (hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd)
		{
			ClickedEditEndNode();
			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KShortestPathHighwayDlg::ClickedEditStartNode()
{
	SelectStartNodeList();
}


void KShortestPathHighwayDlg::ClickedEditEndNode()
{
	SelectEndNodeList();
}


void KShortestPathHighwayDlg::OnBnClickedButtonStartNodeSearch()
{
	SelectStartNodeList();
}


void KShortestPathHighwayDlg::OnBnClickedButtonEndNodeSearch()
{
	SelectEndNodeList();
}


void KShortestPathHighwayDlg::SelectStartNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_START;

		CWaitCursor cw;

		m_spImTasIDFind = KImChampIDFindPtr(new KImChampIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		if (IsDlgButtonChecked(IDC_CHECK_ZONESTART) == BST_CHECKED)
		{
			m_bStartZone = TRUE;
			m_spImTasIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);
		}
		else
		{
			m_bStartZone = FALSE;
			m_spImTasIDFind->SetIDCaption(m_vecNodeIDCaption);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		}

// 		Integer nxNodeID = _ttoi64(m_oStartNodeIdCaption.TID);
// 		m_spImTasIDFind->SetSelectedID(nxNodeID);
		m_spImTasIDFind->SetSelectedID(-1);
		m_spImTasIDFind->Create(KImChampIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KShortestPathHighwayDlg::SelectEndNodeList()
{
	try
	{
		m_bSelectMapNode = MAP_NODE_END;

		if (m_mapPathNodeInfo.size() < 1)
		{
			AfxMessageBox(_T("경로 탐색 결과가 존재하지 않습니다."));
			return;
		}

		CWaitCursor cw;

		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		if (IsDlgButtonChecked(IDC_CHECK_ZONEEND) == BST_CHECKED)
		{
			m_bEndZone = TRUE;
			m_spImTasMultiIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
			m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);
		}
		else
		{
			m_bEndZone = FALSE;
			m_spImTasMultiIDFind->SetIDCaption(m_vecNodeIDCaption);
			m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);
		}

		m_spImTasMultiIDFind->SetSelectedID(m_strSearchedIDs);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);

	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


HBRUSH KShortestPathHighwayDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_EDIT3 || ID == IDC_EDIT4 )
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
	return hbr;
}


void KShortestPathHighwayDlg::OnBnClickedCheckSelectAll()
{
	try
	{
		BOOL bSelectAll(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			bSelectAll = TRUE;
		}

		CXTPReportRecords*    pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord  = nullptr;
		CXTPReportRecordItem* pItem    = nullptr;

		int nCount   = pRecords->GetCount();

		for (int i= 0; i< nCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(_0_COLUMN_CHECK);

			pItem->SetChecked(bSelectAll);
		}

		m_wndReportCtrl.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KShortestPathHighwayDlg::OnBnClickedCheckZonestart()
{
	try
	{
		Integer nxOrgID(GetSelStartNodeId());
		bool    bHave  (false);

		if (IsDlgButtonChecked(IDC_CHECK_ZONESTART) == BST_CHECKED)
		{
			if (m_mapZoneNodeData.find(nxOrgID) != m_mapZoneNodeData.end())
			{
				bHave = true;
			}
			else
			{
				bHave = false;
			}
		}
		else
		{
			if (m_mapNodeData.find(nxOrgID) != m_mapNodeData.end())
			{
				bHave = true;
			}
			else
			{
				bHave = false;
			}
		}

		if (false == bHave)
		{
			m_oStartNodeIdCaption.TID = _T("-1");
			SetDlgItemText(IDC_EDIT3, _T(""));

			InitDestination();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KShortestPathHighwayDlg::InitDestination()
{
	try
	{
		AllClearShortestPath();

		m_wndReportCtrl.ResetContent();
		m_wndReportCtrl.Populate();

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		m_vecPathInfo.clear();
		m_mapPathNodeInfo.clear();
		m_mapPathLinkInfo.clear();

		m_setSearchedID.clear();
		m_strSearchedIDs = _T("");
		SetDestinationEditBox(m_strSearchedIDs);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KShortestPathHighwayDlg::OnBnClickedCheckZoneend()
{
	try
	{
		CWaitCursor cw;

		CString strIDs(_T(""));

		std::set<CString> setPart;

		if (IsDlgButtonChecked(IDC_CHECK_ZONEEND) == BST_CHECKED)
		{
			m_strSearchedIDs = ControlSearchedID(true,  m_setSearchedID,  setPart); // 존 추출
		}
		else
		{
			m_strSearchedIDs = ControlSearchedID(false, m_setSearchedID, setPart);
		}

		SetDestinationEditBox(m_strSearchedIDs);
		UpdateEndNodeID(setPart);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


CString KShortestPathHighwayDlg::ControlSearchedID(bool a_bZone, std::set<CString> &a_setSourceID, std::set<CString> &a_setPartID)
{
	CString strReturn(_T(""));

	try
	{
		AutoType iter  = a_setSourceID.begin();
		AutoType itEnd = a_setSourceID.end();

		CString strID  (_T(""));
		bool    bFirst (true);
		while(iter != itEnd)
		{
			if (a_bZone)
			{
				Integer nxID = _ttoi64(*iter);
				if (m_mapZoneNodeData.find(nxID) == m_mapZoneNodeData.end())
				{
					++iter;
					continue;
				}
			}

			if (bFirst)
			{
				strReturn.AppendFormat(_T("%s"), *iter);
				bFirst = false;
			}
			else
			{
				strReturn.AppendFormat(_T(",%s"), *iter);
			}

			a_setPartID.insert(*iter);

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

	return strReturn;
}

void KShortestPathHighwayDlg::ModelessCancleFeedback(UINT nDialogID)
{

}
