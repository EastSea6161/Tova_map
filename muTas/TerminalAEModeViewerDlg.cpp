// TerminalAEModeViewerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TerminalAEModeViewerDlg.h"
#include "afxdialogex.h"
#include "Scenario.h"
#include "Target.h"
#include "MapView.h"
#include "BulkDBaseNode.h"
#include "IDCaption.h"

enum
{
	_0_A_COLUMN_NODEID = 0,
	_1_A_COLUMN_NAME,
	_2_A_COLUMN_TOTAL,
	_3_A_COLUMN_DRAWBTN
};

#define UM_FUNCTIONDRAW			WM_USER + 1001

// KTerminalAEModeViewerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTerminalAEModeViewerDlg, KResizeDialogEx)

KTerminalAEModeViewerDlg::KTerminalAEModeViewerDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTerminalAEModeViewerDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{
}

KTerminalAEModeViewerDlg::~KTerminalAEModeViewerDlg()
{
	try
	{
		CloseWindowProc();	

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_NODE);
		pTable->Unregister(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KTerminalAEModeViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT4,                m_ctrlReportAcc);
	DDX_Control(pDX, IDC_REPORT5,                m_ctrlReportEgr);
	
}


BEGIN_MESSAGE_MAP(KTerminalAEModeViewerDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT1, &KTerminalAEModeViewerDlg::OnItemButtonClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1,                      &KTerminalAEModeViewerDlg::OnReportItemClick)
	ON_MESSAGE(UM_FUNCTIONDRAW,                           &KTerminalAEModeViewerDlg::OnFunctionDraw)
	ON_BN_CLICKED(IDCANCEL,                               &KTerminalAEModeViewerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6,                            &KTerminalAEModeViewerDlg::OnBnClickedButtonSelNode)
	ON_BN_CLICKED(IDC_BUTTON1, &KTerminalAEModeViewerDlg::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &KTerminalAEModeViewerDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &KTerminalAEModeViewerDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()

void KTerminalAEModeViewerDlg::ActivatedFrameWndNotifyProcess()
{
	try
	{
		AutoType iter  = m_mapTerminalChartDlg.begin();
		AutoType itEnd = m_mapTerminalChartDlg.end();

		while (iter != itEnd)
		{
			KTerminalAEModeChartDlg* pViewDlg = iter->second;

			if (nullptr != pViewDlg)
			{
				if(!pViewDlg->IsUserClosed())
				{
					if(!pViewDlg->IsWindowVisible())
						pViewDlg->ShowWindow(SW_SHOW);
				}
			}

			++iter;
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
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::DeActivatedFrameWndNotifyProcess()
{
	try
	{
		AutoType iter  = m_mapTerminalChartDlg.begin();
		AutoType itEnd = m_mapTerminalChartDlg.end();

		while (iter != itEnd)
		{
			KTerminalAEModeChartDlg* pViewDlg = iter->second;

			if (nullptr != pViewDlg)
			{			
				if (!pViewDlg->IsUserClosed())
				{
					if (pViewDlg->IsWindowVisible())
						pViewDlg->ShowWindow(SW_HIDE);
				}	
			}

			++iter;
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
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR1,			  SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	SetResize(IDC_REPORT1,				  SZ_TOP_LEFT,     CXTPResizePoint(1.0, 0.4));

	SetResize(IDC_SEPRATOR3,		      CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(1.0, 0.4));
	SetResize(IDC_STATIC5,		          CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(0.0, 0.4));

	SetResize(IDC_REPORT4,				  CXTPResizePoint(0.0, 0.4),  CXTPResizePoint(1.0, 0.7));

	SetResize(IDC_SEPRATOR4,	   	      CXTPResizePoint(0.0, 0.7),  CXTPResizePoint(1.0, 0.7));
	SetResize(IDC_STATIC7,		          CXTPResizePoint(0.0, 0.7),  CXTPResizePoint(0.0, 0.7));

	SetResize(IDC_REPORT5,				  CXTPResizePoint(0.0, 0.7),  CXTPResizePoint(1.0, 1.0));

	SetResize(IDC_SEPRATOR2,		      CXTPResizePoint(0.0, 1.0),  CXTPResizePoint(1.0, 1.0));

	SetResize(IDC_BTN_EXPORT,		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_CLEAR,		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,				      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}



// KTerminalAEModeViewerDlg 메시지 처리기입니다.

void KTerminalAEModeViewerDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			InitializeNode();
			if (nullptr != m_spImTasMultiIDFind)
			{
				m_spImTasMultiIDFind.reset();
			}
			ClearTerminalView();
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


void KTerminalAEModeViewerDlg::InitializeNode()
{
	try
	{
		m_mapTerminalResultInfo.clear();
		KDBaseTerminalAEModeViewer::GetTerminalResultModeVolume(m_pTarget, m_mapTerminalResultInfo);

		m_mapNodeData.clear();
		m_vecNodeIDCaption.clear();
		m_vecNodeSelectionInfo.clear();

		TPreferenceSymbol &oTNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		AutoType iter  = m_mapTerminalResultInfo.begin();
		AutoType itEnd = m_mapTerminalResultInfo.end();

		std::set<Integer> setDeleteNodeID;
		CString strNodeID(_T(""));

		while (iter != itEnd)
		{
			Integer             nxTerminalID          = iter->first;
			TTerminalResultInfo &oTTerminalResultInfo = iter->second;

			m_mapNodeData.insert(std::make_pair(nxTerminalID, oTTerminalResultInfo.strTerminalName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxTerminalID);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = oTTerminalResultInfo.strTerminalName;
			m_vecNodeIDCaption.push_back(oIDCaption);

			TNodeSelectionInfo oNodeSelectInfo;
			oNodeSelectInfo.TNodeID = nxTerminalID;
			oNodeSelectInfo.TCharacterIndex = oTNodeSymbol.nCharIndex;
			oNodeSelectInfo.TSize           = oTNodeSymbol.dSize;
			oNodeSelectInfo.TColorRGB       = oTNodeSymbol.clrSymbol;
			m_vecNodeSelectionInfo.push_back(oNodeSelectInfo);

			++iter;
		}

		ClearList();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

// KTerminalAEModeViewerDlg 메시지 처리기입니다.


BOOL KTerminalAEModeViewerDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReportA,   FALSE, FALSE, TRUE);
	KReportCtrlSetting::Default(m_ctrlReportAcc, FALSE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_ctrlReportEgr, FALSE, FALSE, FALSE);

	InitReportHeaderA();
	InitReportHeaderAcc();
	InitReportHeaderEgr();
	m_ctrlReportA.Populate();
	m_ctrlReportAcc.Populate();
	m_ctrlReportEgr.Populate();

	ResizeComponent();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
	pIOTable->Register(this);

	InitializeNode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTerminalAEModeViewerDlg::InitReportHeaderA()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_0_A_COLUMN_NODEID,    _T("Terminal"), 25));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("터미널 노드"));
        }

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_1_A_COLUMN_NAME,      _T("Name"),   50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("터미널 명칭"));
        }

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_2_A_COLUMN_TOTAL,     _T("Total"),   25));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("접근통행량"));
        }

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_3_A_COLUMN_DRAWBTN,   _T("Info"),   60, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("주제도"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::InitReportHeaderAcc()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(0,    _T("Terminal"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("터미널 노드"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(1,      _T("PassengerCar"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("승용차"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(2,     _T("Taxi"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("택시"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(3,   _T("Bus"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("버스"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(4,   _T("Rail"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도시철도"));
        }

		pColumn = m_ctrlReportAcc.AddColumn(new CXTPReportColumn(5,   _T("Bus+Rail"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("버스+도시철도"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::InitReportHeaderEgr()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(0,    _T("Terminal"), 20));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("터미널 노드"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(1,      _T("PassengerCar"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("승용차"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(2,     _T("Taxi"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("택시"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(3,   _T("Bus"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("버스"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(4,   _T("Rail"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도시철도"));
        }

		pColumn = m_ctrlReportEgr.AddColumn(new CXTPReportColumn(5,   _T("Bus+Rail"),   16));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("버스+도시철도"));
        }
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::OnBnClickedButtonSelNode()
{
	try
	{
		SelectNodeList();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::SelectNodeList()
{
	try
	{
		CWaitCursor cw;

		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		if (nullptr != m_spImTasMultiIDFind)
		{
			m_spImTasMultiIDFind.reset();
		}

		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(m_vecNodeIDCaption);
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapNodeData, ImChampMapDefineSymbol::NodeSymbol);

		m_spImTasMultiIDFind->SetSelectedID(m_strSearchedIDs);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KTerminalAEModeViewerDlg::ModelessOKFeedback( UINT nDialogID)
{
	if (KImChampMultiIDFind::IDD == nDialogID)
	{
		if (m_spImTasMultiIDFind)
		{
			try
			{
				m_setSearchedID.clear();
				m_setSearchedID  = m_spImTasMultiIDFind->GetSelectedIDSet();
				m_strSearchedIDs = m_spImTasMultiIDFind->GetSelectedID();

				SetDlgItemText(IDC_EDIT3, m_strSearchedIDs);

				UpdateReportDataA(m_setSearchedID);
				UpdateReportDataAcc(m_setSearchedID);
				UpdateReportDataEgr(m_setSearchedID);

				std::vector<Integer> vecDeleteViewID;
				CString strTerminalID(_T(""));

				AutoType iter  = m_mapTerminalChartDlg.begin();
				AutoType itEnd = m_mapTerminalChartDlg.end();

				while (iter != itEnd)
				{
					Integer nxTerminalID  = iter->first;
					strTerminalID.Format(_T("%I64d"), nxTerminalID);

					if (m_setSearchedID.find(strTerminalID) == m_setSearchedID.end())
					{
						vecDeleteViewID.push_back(nxTerminalID);
					}

					++iter;
				}

				for (size_t i= 0; i< vecDeleteViewID.size(); i++)
				{
					AutoType itFind = m_mapTerminalChartDlg.find(vecDeleteViewID[i]);
					if (itFind != m_mapTerminalChartDlg.end())
					{
						KTerminalAEModeChartDlg* pDlg = itFind->second;

						if( nullptr != pDlg)
						{
							QBicDelete(pDlg);
						}

						m_mapTerminalChartDlg.erase(itFind);
					}
				}
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


void KTerminalAEModeViewerDlg::UpdateReportDataA(std::set<CString> &a_setSeletedNode)
{
	try
	{
		m_ctrlReportA.ResetContent();

		AutoType iter  = a_setSeletedNode.begin();
		AutoType itEnd = a_setSeletedNode.end();

		Integer nxTerminalID(0);
		CString strTerminalID(_T(""));
		CString strTerminalName(_T(""));

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (iter != itEnd)
		{
			strTerminalID    = *iter;
			nxTerminalID     = _ttoi64(strTerminalID);

			AutoType itFind = m_mapTerminalResultInfo.find(nxTerminalID);
			if (itFind == m_mapTerminalResultInfo.end())
			{
				++iter;
				continue;
			}

			TTerminalResultInfo &oTTerminalResultInfo = itFind->second;
			strTerminalName = oTTerminalResultInfo.strTerminalName;

			pRecord = m_ctrlReportA.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetCaption(strTerminalID);
			pItem->SetItemData((DWORD_PTR) nxTerminalID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText( strTerminalName ));
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dTotal, _T("%.2f")));
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetItemData((DWORD_PTR) nxTerminalID);
			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(nullptr != pButton)
			{
				pButton->SetAlignment(xtpItemControlUnknown);
				pButton->SetCaption(_T("Show"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("보기"));
				}
				pButton->SetSize(CSize(60, 0));
			}

			++iter;
		}

		m_ctrlReportA.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::UpdateReportDataAcc(std::set<CString> &a_setSeletedNode)
{
	try
	{
		m_ctrlReportAcc.ResetContent();

		AutoType iter  = a_setSeletedNode.begin();
		AutoType itEnd = a_setSeletedNode.end();

		Integer nxTerminalID(0);
		CString strTerminalID(_T(""));
		CString strTerminalName(_T(""));

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (iter != itEnd)
		{
			strTerminalID    = *iter;
			nxTerminalID     = _ttoi64(strTerminalID);

			AutoType itFind = m_mapTerminalResultInfo.find(nxTerminalID);
			if (itFind == m_mapTerminalResultInfo.end())
			{
				++iter;
				continue;
			}

			TTerminalResultInfo &oTTerminalResultInfo = itFind->second;

			pRecord = m_ctrlReportAcc.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetCaption(strTerminalID);
			pItem->SetItemData((DWORD_PTR) nxTerminalID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dAccAuto, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dAccTaxi, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dAccBus,  _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dAccRail, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dAccTransit, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		m_ctrlReportAcc.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::UpdateReportDataEgr(std::set<CString> &a_setSeletedNode)
{
	try
	{
		m_ctrlReportEgr.ResetContent();

		AutoType iter  = a_setSeletedNode.begin();
		AutoType itEnd = a_setSeletedNode.end();

		Integer nxTerminalID(0);
		CString strTerminalID(_T(""));
		CString strTerminalName(_T(""));

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (iter != itEnd)
		{
			strTerminalID    = *iter;
			nxTerminalID     = _ttoi64(strTerminalID);

			AutoType itFind = m_mapTerminalResultInfo.find(nxTerminalID);
			if (itFind == m_mapTerminalResultInfo.end())
			{
				++iter;
				continue;
			}

			TTerminalResultInfo &oTTerminalResultInfo = itFind->second;

			pRecord = m_ctrlReportEgr.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetCaption(strTerminalID);
			pItem->SetItemData((DWORD_PTR) nxTerminalID);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dEgrAuto, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dEgrTaxi, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dEgrBus,  _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dEgrRail, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(oTTerminalResultInfo.dEgrTransit, _T("%.2f")));
			pItem->SetAlignment(DT_RIGHT);

			++iter;
		}

		m_ctrlReportEgr.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::OnBnClickedButtonMap()
{
	try
	{
		if (m_pMapView != nullptr)
		{
			m_pMapView->MapNodeSelectAction(this, m_vecNodeSelectionInfo);
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


void KTerminalAEModeViewerDlg::MapSelectionFeedback( Integer a_nxID)
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();

		AddSearchID(a_nxID);
		ShowTerminalVolumeView(a_nxID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();

		Integer nxID(0);
		size_t nxCount = a_vecSelection.size();
		for (size_t i=0; i<nxCount; i++)
		{
			nxID = a_vecSelection[i];
		}	

		if (nxCount < 1)
		{
			return;
		}

		AddSearchID(nxID);
		ShowTerminalVolumeView(nxID);
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


void KTerminalAEModeViewerDlg::AddSearchID(Integer &a_nxID)
{
	try
	{
		CString strID(_T(""));
		strID.Format(_T("%I64d"), a_nxID);

		AutoType itFind = m_setSearchedID.find(strID);
		if (itFind != m_setSearchedID.end())
		{
			return;
		}

		m_setSearchedID.insert(strID);
		m_strSearchedIDs = _T("");

		AutoType iter = m_setSearchedID.begin();
		AutoType end  = m_setSearchedID.end();

		bool bFirst(true);
		while(iter != end)
		{
			if (bFirst)
			{
				m_strSearchedIDs.AppendFormat(_T("%s"), *iter);
				bFirst = false;
			}
			else
			{
				m_strSearchedIDs.AppendFormat(_T(",%s"), *iter);
			}

			++iter;
		}    	

		SetDlgItemText(IDC_EDIT3, m_strSearchedIDs);

		UpdateReportDataA(m_setSearchedID);
		UpdateReportDataAcc(m_setSearchedID);
		UpdateReportDataEgr(m_setSearchedID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}


void KTerminalAEModeViewerDlg::CloseWindowProc()
{
	try
	{
		ClearTerminalView();

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		m_pMapView->ClearScreenDrawEdit();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::ClearTerminalView()
{
	try
	{
		AutoType iter  = m_mapTerminalChartDlg.begin();
		AutoType itEnd = m_mapTerminalChartDlg.end();

		while (iter != itEnd)
		{
			KTerminalAEModeChartDlg* pDlg = iter->second;
			if (nullptr != pDlg)
			{
				QBicDelete(pDlg);
			}

			++iter;
		}

		m_mapTerminalChartDlg.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTerminalAEModeViewerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT3)->m_hWnd)
		{
			SelectNodeList();
			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);
}


void KTerminalAEModeViewerDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		if (nullptr == pClickedItem)
			return;

		CXTPReportRow* pRow = pClickedItem->pRow;
		if (nullptr == pRow)
			return;

		CXTPReportRecord*         pRecord         = pRow->GetRecord();
		CXTPReportRecordItem*     pItem           = pRecord->GetItem(_0_A_COLUMN_NODEID);

		Integer nxNodeID = (Integer)pItem->GetItemData();
		m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KTerminalAEModeViewerDlg::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	PostMessage(UM_FUNCTIONDRAW, (WPARAM)(pItemNotify->pItem));
}


LRESULT KTerminalAEModeViewerDlg::OnFunctionDraw(WPARAM wParam, LPARAM lParam)
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)wParam;

	try
	{
		Integer nxTerminalID = (Integer)pItem->GetItemData();
		ShowTerminalVolumeView(nxTerminalID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return 0;
}


void KTerminalAEModeViewerDlg::ShowTerminalVolumeView(Integer &a_nxTerminalID)
{
	try
	{
		Integer nxTerminalID = a_nxTerminalID;

		AutoType itFind = m_mapTerminalResultInfo.find(nxTerminalID);
		if (itFind == m_mapTerminalResultInfo.end())
		{
			throw 1;
		}

		TTerminalResultInfo &oTTerminalResultInfo = itFind->second;

		AutoType iterD  = m_mapTerminalChartDlg.find(nxTerminalID);
		AutoType itEndD = m_mapTerminalChartDlg.end();

		if (iterD != itEndD)
		{
			KTerminalAEModeChartDlg* pDlg = iterD->second;

			if( nullptr != pDlg)
			{
				QBicDelete(pDlg);
			}

			m_mapTerminalChartDlg.erase(iterD);
		}

		KTerminalAEModeChartDlg* pTerminalChartDlg = new KTerminalAEModeChartDlg(oTTerminalResultInfo, this);
		pTerminalChartDlg->Create(KTerminalAEModeChartDlg::IDD);
		pTerminalChartDlg->ShowWindow(SW_SHOW);

		m_mapTerminalChartDlg.insert(std::make_pair(nxTerminalID, pTerminalChartDlg));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::OnBnClickedBtnExport()
{
	try
	{
		CXTPReportRecords* pRecords = m_ctrlReportA.GetRecords();
		int nRecordCnt   = pRecords->GetCount();
		if (nRecordCnt < 1)
		{
			return;
		}

		CWaitCursor cw;

		KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();

		CString strInitFileName(_T(""));
		strInitFileName.Format(_T("%s_%s_TerminalModeVolume"), pScenario->GetName(), m_pTarget->GetName());

		TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|");
		CFileDialog fileDlg( FALSE, _T("txt"), strInitFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );

		CString strLocation = m_pTarget->GetLocation();
		if (!strLocation.IsEmpty())
		{
			fileDlg.m_ofn.lpstrInitialDir = strLocation;
		}
		if (fileDlg.DoModal() != IDOK)
		{
			return;
		}
		CString strFilePath = fileDlg.GetPathName();

		UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
		CStdioFileEx of(strFilePath, nOpenMode);

		CString strExport(_T(""));
		try
		{
			// write header 
			CString strHeader(_T(""));
			strHeader.Append(_T("Terminal_ID\tName\t"));
			strHeader.Append(_T("Acc_Auto\tAcc_Taxi\tAcc_Bus\tAcc_Subway\tAcc_Transit\t"));
			strHeader.Append(_T("Egr_Auto\tEgr_Taxi\tEgr_Bus\tEgr_Subway\tEgr_Transit\t"));
			strHeader.Append(_T("Total"));

			strExport.AppendFormat(_T("%s"), strHeader);
			strExport.Append(_T("\r\n"));
			of.WriteString(strExport);

			CXTPReportRecord*           pRecord  = nullptr;
			CXTPReportRecordItem*       pItem    = nullptr;
			CXTPReportRecordItemNumber* pItemNum = nullptr;
			CXTPReportRecords*          pRecords = m_ctrlReportA.GetRecords();
			int nRecordCnt            = pRecords->GetCount();

			for (int i= 0; i< nRecordCnt; i++)
			{
				pRecord = pRecords->GetAt(i);

				pItem   = pRecord->GetItem(_0_A_COLUMN_NODEID);
				Integer nxTerminalID = (Integer)pItem->GetItemData();

				AutoType itFind = m_mapTerminalResultInfo.find(nxTerminalID);
				if (itFind == m_mapTerminalResultInfo.end())
				{
					continue;
				}

				TTerminalResultInfo &oTTerminalResultInfo = itFind->second;

				// write body
				strExport.Format(_T("%I64d\t%s\t"), oTTerminalResultInfo.nxTerminalID, oTTerminalResultInfo.strTerminalName);
				strExport.AppendFormat(_T("%f\t%f\t%f\t%f\t%f\t"), oTTerminalResultInfo.dAccAuto, oTTerminalResultInfo.dAccTaxi, oTTerminalResultInfo.dAccBus, oTTerminalResultInfo.dAccRail, oTTerminalResultInfo.dAccTransit);
				strExport.AppendFormat(_T("%f\t%f\t%f\t%f\t%f\t"), oTTerminalResultInfo.dEgrAuto, oTTerminalResultInfo.dEgrTaxi, oTTerminalResultInfo.dEgrBus, oTTerminalResultInfo.dEgrRail, oTTerminalResultInfo.dEgrTransit);
				strExport.AppendFormat(_T("%f"), oTTerminalResultInfo.dTotal);

				strExport.Append(_T("\r\n"));

				of.WriteString(strExport);
			}

			of.Close();
		}
		catch (...)
		{
			TxLogDebugException();	
			of.Close();
			return;
		}

		ShellExecute(NULL, _T("open"), strFilePath, NULL, NULL, SW_SHOW); // 기본 프로그램으로 잡혀 있는 프로그램으로 자동 오픈
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTerminalAEModeViewerDlg::OnBnClickedBtnClear()
{
	ClearList();
	ClearTerminalView();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	m_pMapView->ClearScreenDrawEdit();
}


void KTerminalAEModeViewerDlg::ClearList()
{
	try
	{
		m_strSearchedIDs = _T("");
		m_setSearchedID.clear();
		SetDlgItemText(IDC_EDIT3, m_strSearchedIDs);

		UpdateReportDataA(m_setSearchedID);
		UpdateReportDataAcc(m_setSearchedID);
		UpdateReportDataEgr(m_setSearchedID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}