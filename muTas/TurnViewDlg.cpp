// TurnViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TurnViewDlg.h"
#include "afxdialogex.h"
#include "MapView.h"

enum
{
	_0_COLUMN_SELECT = 0,
	_1_COLUMN_NODE_FROM,
	_2_COLUMN_NODE_AT,
	_3_COLUMN_NODE_TO,
	_4_COLUMN_VOLUME
};

const double TURNVOLUME_LINE_MIN_SIZE  = 2.0;
const double TURNVOLUME_LINE_MAX_SIZE  = 10.0;

// KTurnViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTurnViewDlg, KResizeDialogEx)

KTurnViewDlg::KTurnViewDlg(std::map<int, CString> &a_mapModeIndexName, std::map<KODKey, std::vector<double>> &a_mapFTNodeVolume, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTurnViewDlg::IDD, pParent)
	, m_mapModeIndexName(a_mapModeIndexName)
	, m_mapFTNodeVolume(a_mapFTNodeVolume)
	, m_pMapView(a_pMapView)
	, m_pParentWnd(pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_DLG_ICO);
    m_bDrawRoad  = false;
}

KTurnViewDlg::~KTurnViewDlg()
{
}

void KTurnViewDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1,                m_cboMode);
	DDX_Control(pDX, IDC_REPORT,                m_ctrlReport);
}


BEGIN_MESSAGE_MAP(KTurnViewDlg, KResizeDialogEx)
    ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1,                 &KTurnViewDlg::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_CHECK_ALL,                 &KTurnViewDlg::OnBnClickedCheckAll)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT, &KTurnViewDlg::OnReportCheckItem)
	ON_NOTIFY(NM_CLICK, IDC_REPORT,              &KTurnViewDlg::OnReportItemClick)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KTurnViewDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK_LINE, &KTurnViewDlg::OnBnClickedCheckLine)
	ON_BN_CLICKED(IDCANCEL, &KTurnViewDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


void KTurnViewDlg::SetPrepareData(Integer &a_nxAtNodeID, CString &a_strAtNodeName, TDrawNodeXY &a_oCenterNodeXY, std::vector<TDrawNodeXY> &a_vecNodes)
{
	m_nxAtNodeID        = a_nxAtNodeID;
	m_strAtNodeName     = a_strAtNodeName;
	m_oCenterNodeXY     = a_oCenterNodeXY;
	m_vecNodes          = a_vecNodes;
}


void KTurnViewDlg::ResizeComponent()
{
	SetResize(IDC_STATIC_VIEW,            SZ_TOP_LEFT,       SZ_BOTTOM_RIGHT);

	SetResize(IDC_CHECK_ALL,			  SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,   			  SZ_TOP_LEFT,       SZ_TOP_LEFT);
	SetResize(IDC_COMBO1,   			  SZ_TOP_LEFT,       SZ_TOP_LEFT);

	SetResize(IDC_REPORT,				  SZ_TOP_LEFT,       SZ_BOTTOM_LEFT);

	SetResize(IDC_CHECK_LABEL,		      SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LINE,	     	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDCANCEL,	     	          SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR4,	     	  SZ_TOP_LEFT,       SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR3,	     	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC_TITLE,	     	  SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
}


BOOL KTurnViewDlg::OnInitDialog()
{
    KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

//     SetIcon(m_hIcon, TRUE);	 // 큰 아이콘을 설정합니다.
//     SetIcon(m_hIcon, FALSE); // 작은 아이콘을 설정합니다.
	
	if (nullptr != m_pParentWnd)
	{
		CRect rectParent;
		m_pParentWnd->GetWindowRect(&rectParent);

		CRect rect;
		GetWindowRect(rect);
		MoveWindow(rectParent.left + rectParent.Width(), rectParent.top + 0, rect.Width(), rect.Height());
	}

	CString strTitle(_T(""));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		strTitle.AppendFormat(_T("교차로 노드ID: %I64d"), m_nxAtNodeID);
	}
	else {
		strTitle.AppendFormat(_T("At Node: %I64d"), m_nxAtNodeID);
	}
	
	if (!m_strAtNodeName.IsEmpty())
	{
		strTitle.AppendFormat(_T(" - %s"), m_strAtNodeName);
	}
	SetDlgItemText(IDC_STATIC_TITLE, strTitle);

	InitComboMode();
	CheckDlgButton(IDC_CHECK_ALL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LABEL, BST_CHECKED);
	CheckDlgButton(IDC_CHECK_LINE, BST_UNCHECKED);

	KReportCtrlSetting::Default(m_ctrlReport, FALSE, FALSE, FALSE);
	InitReportHeader();
	InitReportData();
	m_ctrlReport.Populate();
	UpdateReportData();

	ResizeComponent();
        
    return TRUE;  
}


void KTurnViewDlg::InitComboMode()
{
	try
	{
		m_cboMode.ResetContent();

		AutoType iter  = m_mapModeIndexName.begin();
		AutoType itEnd = m_mapModeIndexName.end();

		//int nCur = m_cboMode.AddString(_T("All"));
		int nCur              = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			nCur = m_cboMode.AddString(_T("전체"));
		}
		else {
			nCur = m_cboMode.AddString(_T("All"));
		}
		m_cboMode.SetItemData(nCur, -1);

		while (iter != itEnd)
		{
			int     nModeIndex   = iter->first;
			CString &strModeName = iter->second;

			nCur = m_cboMode.AddString(strModeName);
			m_cboMode.SetItemData(nCur, (DWORD_PTR) nModeIndex);

			++iter;
		}
		
		if (m_cboMode.GetCount() > 0)
		{
			m_cboMode.SetCurSel(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_SELECT,         _T("Select"), 40,  FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_NODE_FROM,      _T("From Node"), 12));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시작노드"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_NODE_AT,        _T("At Node"),   10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교차노드"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_NODE_TO,        _T("To Node"),   10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("끝노드"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_4_COLUMN_VOLUME,         _T(""),          10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교통량"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::InitReportData()
{
	try
	{
		m_ctrlReport.ResetContent();

		CString strAtNodeID(_T(""));
		strAtNodeID.Format(_T("%I64d"), m_oCenterNodeXY.NodeID);
		CString strFromNodeID(_T(""));
		CString strToNodeID(_T(""));

		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		AutoType iterB  = m_mapFTNodeVolume.begin();
		AutoType itEndB = m_mapFTNodeVolume.end();

		while (iterB != itEndB)
		{
			KODKey               oKODKey   = iterB->first;
			std::vector<double> &vecVolume = iterB->second;

			strFromNodeID.Format(_T("%I64d"), oKODKey.OriginID);
			strToNodeID.Format  (_T("%I64d"), oKODKey.DestinationID);

			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetItemData((DWORD_PTR) m_nxAtNodeID);
			pItem->SetAlignment(xtpColumnIconCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strFromNodeID));
			pItem->SetItemData((DWORD_PTR) oKODKey.OriginID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strAtNodeID));
			pItem->SetItemData((DWORD_PTR) m_nxAtNodeID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strToNodeID));
			pItem->SetItemData((DWORD_PTR) oKODKey.DestinationID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(0.0, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			++iterB;
		}

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::UpdateReportData()
{
	try
	{
		UpdateData(TRUE);
		int nCurSel = m_cboMode.GetCurSel();
		if (nCurSel < 0)
		{
			return;
		}

		m_dMaxVolume = -999999999999.0;
		m_dMinVolume = 999999999999.0;
		 
		int     nModeIndex = (int)m_cboMode.GetItemData(nCurSel);
		CString strModeName(_T(""));
		m_cboMode.GetLBText(nCurSel, strModeName);

		CXTPReportColumns* pColumns = m_ctrlReport.GetColumns();
		CXTPReportColumn*  pColumn  = pColumns->Find(_4_COLUMN_VOLUME);
		//pColumn->SetCaption(strModeName);

		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*           pRecord    = nullptr;
		CXTPReportRecordItem*       pItem      = nullptr;
		CXTPReportRecordItemNumber* pItemNum   = nullptr;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			KODKey oODKey;

			pItem              = pRecord->GetItem(_1_COLUMN_NODE_FROM);
			Integer nxNodeFrom = (Integer)pItem->GetItemData();

			pItem              = pRecord->GetItem(_3_COLUMN_NODE_TO);
			Integer nxNodeTo   = (Integer)pItem->GetItemData();

			oODKey.OriginID      = nxNodeFrom;
			oODKey.DestinationID = nxNodeTo;

			double dVolume(0.0);

			AutoType itFind = m_mapFTNodeVolume.find(oODKey);
			if (itFind != m_mapFTNodeVolume.end())
			{
				std::vector<double> &vecVolume = itFind->second;

				if (nModeIndex < 0) // All
				{
					for (size_t k= 0; k< vecVolume.size(); k++)
					{
						dVolume += vecVolume[k];
					}
				}
				else
				{
					if (nModeIndex < (int)vecVolume.size())
					{
						dVolume = vecVolume[nModeIndex];
					}
				}
			}

			pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_VOLUME);
			pItemNum->SetValue(dVolume);

			if (dVolume >= 0)
			{
				if (m_dMinVolume > dVolume)
				{
					m_dMinVolume = dVolume;
				}
			}

			if (m_dMaxVolume < dVolume)
			{
				m_dMaxVolume = dVolume;
			}
		}

		if (m_dMaxVolume != m_dMinVolume) // Record가 한줄일때 최대 최소 값은 같다.
		{
			m_dMaxVolume += 0.006;
			m_dMinVolume -= 0.005;
			if (m_dMinVolume < 0.0)
			{
				m_dMinVolume = 0.0;
			}
		}

		m_ctrlReport.RedrawControl();

		DrawTurnVolume();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 아이콘을 그립니다.
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CPaintDC dc(this); // device context for painting
        
        CWnd* pStaticView = GetDlgItem(IDC_STATIC_VIEW);
        pStaticView->GetWindowRect(m_oTurnViewRect);
        ScreenToClient(m_oTurnViewRect);

        m_oGraphicTurnVolumeDisp.DrawTurnVolume(pStaticView, m_oTurnViewRect);    
    }    
}


void KTurnViewDlg::Refresh()
{
    InvalidateRect(m_oTurnViewRect);
}


void KTurnViewDlg::SetData()
{
    TDrawNodeXY oCenterNodeXY;
    oCenterNodeXY.NodeID = 0;
    oCenterNodeXY.Label = _T("Center");
    oCenterNodeXY.X = 50000;
    oCenterNodeXY.Y = 50000;

    std::vector<TDrawNodeXY> vecNodes;            
    TDrawNodeXY oDrawNode;
    
    oDrawNode.NodeID = 1;
    oDrawNode.Label = _T("1");
    oDrawNode.X     = 20000;
    oDrawNode.Y     = 20000;
    vecNodes.push_back(oDrawNode);

    oDrawNode.NodeID = 2;
    oDrawNode.Label = _T("2");
    oDrawNode.X     = 20000;
    oDrawNode.Y     = 60000;
    vecNodes.push_back(oDrawNode);

    oDrawNode.NodeID = 3;
    oDrawNode.Label = _T("3");
    oDrawNode.X     = 60000;
    oDrawNode.Y     = 60000;
    vecNodes.push_back(oDrawNode);

    oDrawNode.NodeID = 4;
    oDrawNode.Label = _T("4");
    oDrawNode.X     = 60000;
    oDrawNode.Y     = 50000;
    vecNodes.push_back(oDrawNode);

    
    std::vector<TDrawTurnVolume> vecDrawTurnVolume;
    TDrawTurnVolume oDrawTurnVolume;
    
    oDrawTurnVolume.FromNodeID   = 1;
    oDrawTurnVolume.ToNodeID     = 2;
    oDrawTurnVolume.Label        = _T("3600.67");
    oDrawTurnVolume.LineColor    = Color(200, 255, 0, 0);
    oDrawTurnVolume.DisplayWidth = 10;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    oDrawTurnVolume.FromNodeID   = 1;
    oDrawTurnVolume.ToNodeID     = 3;
    oDrawTurnVolume.Label        = _T("600.67");
    oDrawTurnVolume.LineColor    = Color(200, 255, 0, 0);
    oDrawTurnVolume.DisplayWidth = 15;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    oDrawTurnVolume.FromNodeID   = 1;
    oDrawTurnVolume.ToNodeID     = 4;
    oDrawTurnVolume.Label        = _T("11600.67");
    oDrawTurnVolume.LineColor    = Color(200, 255, 0, 0);
    oDrawTurnVolume.DisplayWidth = 15;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    oDrawTurnVolume.FromNodeID   = 2;
    oDrawTurnVolume.ToNodeID     = 3;
    oDrawTurnVolume.Label        = _T("11600.67");
    oDrawTurnVolume.LineColor    = Color(200, 0, 0, 0);
    oDrawTurnVolume.DisplayWidth = 25;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    oDrawTurnVolume.FromNodeID   = 3;
    oDrawTurnVolume.ToNodeID     = 3;
    oDrawTurnVolume.Label        = _T("600.67");
    oDrawTurnVolume.LineColor    = Color(200, 255, 255, 0);
    oDrawTurnVolume.DisplayWidth = 15;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    oDrawTurnVolume.FromNodeID   = 4;
    oDrawTurnVolume.ToNodeID     = 3;
    oDrawTurnVolume.Label        = _T("600.67");
    oDrawTurnVolume.LineColor    = Color(255, 0, 255, 0);
    oDrawTurnVolume.DisplayWidth = 10;
    vecDrawTurnVolume.push_back(oDrawTurnVolume);

    std::vector<TDrawTurnVolume> XvecDrawTurnVolume;
    m_oGraphicTurnVolumeDisp.InitialData(oCenterNodeXY, vecNodes, vecDrawTurnVolume);    
}


void KTurnViewDlg::OnCbnSelchangeComboMode()
{
	UpdateReportData();
}


void KTurnViewDlg::OnBnClickedCheckAll()
{
	ControlCheckAll();
}


void KTurnViewDlg::ControlCheckAll()
{
	try
	{
		BOOL bCheck(FALSE);
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
			bCheck = TRUE;
		else
			bCheck = FALSE;
		
		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*           pRecord    = nullptr;
		CXTPReportRecordItem*       pItem      = nullptr;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem              = pRecord->GetItem(_0_COLUMN_SELECT);
			pItem->SetChecked(bCheck);
		}

		m_ctrlReport.RedrawControl();

		DrawTurnVolume();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::OnReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	try
	{
		BOOL bChecked = pItemNotify->pItem->IsChecked();

		if (!bChecked)
		{
			CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
		}

		DrawTurnVolume();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		if (nullptr == pClickedItem)
			return;

		int nSelItemIndex = pClickedItem->pItem->GetIndex();
		if (_1_COLUMN_NODE_FROM <= nSelItemIndex && nSelItemIndex <= _3_COLUMN_NODE_TO)
		{
			Integer nxNodeID = (Integer)pClickedItem->pItem->GetItemData();
			m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
		}
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KTurnViewDlg::DrawTurnVolume()
{
	try
	{
		TTurnVolumeSettingInfo oSettingInfo;
		oSettingInfo.dMinSize   = TURNVOLUME_LINE_MIN_SIZE;
		oSettingInfo.dMaxSize   = TURNVOLUME_LINE_MAX_SIZE;
		oSettingInfo.dMinVolume = m_dMinVolume;
		oSettingInfo.dMaxVolume = m_dMaxVolume;

		std::vector<TDrawTurnVolume> vecDrawTurnVolume;

#pragma region 선택된 DrawTurnVolume Data 수집
		CXTPReportRecords* pRecords = m_ctrlReport.GetRecords();
		int nRecordCount = pRecords->GetCount();

		CXTPReportRecord*           pRecord    = nullptr;
		CXTPReportRecordItem*       pItem      = nullptr;
		CXTPReportRecordItemNumber* pItemNum   = nullptr;

		for (int i= 0; i< nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItem              = pRecord->GetItem(_0_COLUMN_SELECT);
			if (!pItem->IsChecked())
			{
				continue;
			}

			pItem              = pRecord->GetItem(_1_COLUMN_NODE_FROM);
			Integer nxNodeFrom = (Integer)pItem->GetItemData();

			pItem              = pRecord->GetItem(_3_COLUMN_NODE_TO);
			Integer nxNodeTo   = (Integer)pItem->GetItemData();

			pItemNum           = (CXTPReportRecordItemNumber*)pRecord->GetItem(_4_COLUMN_VOLUME);
			double dVolume     = pItemNum->GetValue();

			CString strLabel(_T(""));
			strLabel.Format(_T("%.2f"), dVolume);

			TDrawTurnVolume oDrawTurnVolume;

			oDrawTurnVolume.FromNodeID   = nxNodeFrom;
			oDrawTurnVolume.ToNodeID     = nxNodeTo;
			oDrawTurnVolume.Label        = strLabel;

			CalculateValue2Width(dVolume, oSettingInfo, oDrawTurnVolume.DisplayWidth);

			vecDrawTurnVolume.push_back(oDrawTurnVolume);
		}
#pragma endregion 선택된 DrawTurnVolume Data 수집

		m_oGraphicTurnVolumeDisp.InitialData(m_oCenterNodeXY, m_vecNodes, vecDrawTurnVolume); 

		ControlCheckLabel();
		ControlCheckLine();

		Refresh();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::CalculateValue2Width(double a_dVolume, TTurnVolumeSettingInfo &a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxVolume == a_oSettingInfo.dMinVolume)
	{
		if (a_oSettingInfo.dMaxVolume == 0.0)
		{
			dWidth = a_oSettingInfo.dMinSize;
		}
		else
		{
			dWidth = a_oSettingInfo.dMaxSize;
		}
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxVolume - a_oSettingInfo.dMinVolume) * (a_dVolume - a_oSettingInfo.dMinVolume) + a_oSettingInfo.dMinSize;
	}

	if (dWidth > a_oSettingInfo.dMaxSize)
	{
		dWidth = a_oSettingInfo.dMaxSize;
	}

	if (dWidth < a_oSettingInfo.dMinSize)
	{
		dWidth = a_oSettingInfo.dMinSize;
	}
}


void KTurnViewDlg::OnBnClickedCheckLabel()
{
	//m_bDrawRoad = !m_oGraphicTurnVolumeDisp.IsDrawAllPath();
	//m_oGraphicTurnVolumeDisp.SetDrawAllPath(m_bDrawRoad);

// 	m_bDrawRoad = !m_oGraphicTurnVolumeDisp.IsDrawAllPath();
// 	m_oGraphicTurnVolumeDisp.SetDrawAllPath(m_bDrawRoad);
// 
// 	bool bCenterDraw = m_oGraphicTurnVolumeDisp.IsDrawCenterLine();
// 	m_oGraphicTurnVolumeDisp.SetDrawCenterLine(!bCenterDraw);
// 
	ControlCheckLabel();
	Refresh();
}


void KTurnViewDlg::ControlCheckLabel()
{
	try
	{
		bool bShow(false);
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
			bShow = true;
		else
			bShow = false;

		m_oGraphicTurnVolumeDisp.SetLabel(bShow);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::OnBnClickedCheckLine()
{
	ControlCheckLine();
	Refresh();
}


void KTurnViewDlg::ControlCheckLine()
{
	try
	{
		bool bShow(false);
		if (IsDlgButtonChecked(IDC_CHECK_LINE) == BST_CHECKED)
			bShow = true;
		else
			bShow = false;

		m_oGraphicTurnVolumeDisp.SetDrawAllPath(bShow);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnViewDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}
