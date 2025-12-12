// TurnVolumeViewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TurnVolumeViewDlg.h"
#include "afxdialogex.h"
#include "Scenario.h"
#include "Target.h"
#include "MapView.h"
#include "Mode.h"
#include "ModeManager.h"
#include "BulkDBaseNode.h"
#include "IDCaption.h"

enum
{
	_0_A_COLUMN_NODEID = 0,
	_1_A_COLUMN_NAME,
	_2_A_COLUMN_DRAWBTN
};

enum
{
	_0_B_COLUMN_NODE_FROM = 0,
	_1_B_COLUMN_NODE_AT,
	_2_B_COLUMN_NODE_TO
};

#define UM_FUNCTIONDRAW			WM_USER + 1001

// KTurnVolumeViewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTurnVolumeViewDlg, KResizeDialogEx)

KTurnVolumeViewDlg::KTurnVolumeViewDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTurnVolumeViewDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{
	CWaitCursor cw;

	ModeIndexInfo (m_mapModeIndexName);
	ReadOutPutFile(m_mapAtNodeVolume);
}

KTurnVolumeViewDlg::~KTurnVolumeViewDlg()
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

void KTurnVolumeViewDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1,                m_ctrlReportA);
	DDX_Control(pDX, IDC_REPORT2,                m_ctrlReportB);
}


BEGIN_MESSAGE_MAP(KTurnVolumeViewDlg, KResizeDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT1, &KTurnVolumeViewDlg::OnItemButtonClick)
	ON_NOTIFY(NM_CLICK, IDC_REPORT1,                      &KTurnVolumeViewDlg::OnReportItemClick)
	ON_MESSAGE(UM_FUNCTIONDRAW,                           &KTurnVolumeViewDlg::OnFunctionDraw)
	ON_NOTIFY(NM_CLICK, IDC_REPORT2,                      &KTurnVolumeViewDlg::OnReportItemClickB)
	ON_BN_CLICKED(IDCANCEL,                               &KTurnVolumeViewDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON6,                            &KTurnVolumeViewDlg::OnBnClickedButtonSelNode)
	ON_BN_CLICKED(IDC_BUTTON1, &KTurnVolumeViewDlg::OnBnClickedButtonMap)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &KTurnVolumeViewDlg::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &KTurnVolumeViewDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


void KTurnVolumeViewDlg::ActivatedFrameWndNotifyProcess()
{
	try
	{
		AutoType iter  = m_mapAtNodeViewDlg.begin();
		AutoType itEnd = m_mapAtNodeViewDlg.end();

		while (iter != itEnd)
		{
			KTurnViewDlg* pViewDlg = iter->second;

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


void KTurnVolumeViewDlg::DeActivatedFrameWndNotifyProcess()
{
	try
	{
		AutoType iter  = m_mapAtNodeViewDlg.begin();
		AutoType itEnd = m_mapAtNodeViewDlg.end();

		while (iter != itEnd)
		{
			KTurnViewDlg* pViewDlg = iter->second;

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


void KTurnVolumeViewDlg::ModeIndexInfo(std::map<int, CString> &a_mapModeIndexName)
{
	a_mapModeIndexName.clear();

	try
	{
		CString strModes      (_T(""));
		int     nFlagBusVolume(0);
		if (!LoadAssignTurnVolumeInput(strModes, nFlagBusVolume))
			return;

		std::vector<CString> vecMode = KParseString::ParseString(strModes, _T(','));
		bool bUseBusVolume = false;
		if (1 == nFlagBusVolume)
			bUseBusVolume = true;
		else
			bUseBusVolume = false;

		KModeManager*       pModeManager = m_pTarget->ModeManager();
		int nAllModeCount = pModeManager->GetModeCount();
		
		int nIndex(0);
		for (size_t i= 0; i< vecMode.size(); i++)
		{
			CString strModeID     = vecMode[i];
			int     nModeID       = _ttoi(strModeID);
			CString strModeName(_T(""));

			for (int k= 0; k< nAllModeCount; k++)
			{
				KMode* pMode = pModeManager->GetMode(k);
				 if (pMode->ModeID() == nModeID)
				 {
					 strModeName = pMode->ModeName();
					 break;
				 }
			}

			a_mapModeIndexName.insert(std::make_pair(nIndex++, strModeName));
		}

		if (bUseBusVolume)
		{
			a_mapModeIndexName.insert(std::make_pair(nIndex, _T("BusVol")));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapModeIndexName.clear();
	}
}


bool KTurnVolumeViewDlg::LoadAssignTurnVolumeInput(CString &a_strModes, int &a_nFlagBusVolume)
{
	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));

		strSQL.Format(_T(" SELECT mode, busvolume FROM %s "), TABLE_ASSIGN_TURN_VOLUME_INPUT);

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		if (spResult->Next())
		{
			a_strModes       = spResult->GetValueString(0);
			a_nFlagBusVolume = spResult->GetValueInt(1);

			return true;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return false;
}


void KTurnVolumeViewDlg::ReadOutPutFile(std::map<Integer, std::map<KODKey, std::vector<double>>> &a_mapAtNodeVolume)
{
	TxLogDebugStartMsg();

	CString strFilePath(_T(""));
	strFilePath.Format(_T("%s\\%s\\%s\\%s"), m_pTarget->GetLocation(), DLL_MODEL_FOLDER_NAME, DLL_HIGHWAYASSIGN_FOLDER_NAME, T_DAT_OUT_TURNVOLUME);

	std::ifstream ifs(strFilePath, std::ios::binary);

	try
	{
		if (ifs)
		{
			a_mapAtNodeVolume.clear();

			while (! ifs.eof())
			{
				Integer nxNodeFrom(0);
				Integer nxNodeAt  (0);
				Integer nxNodeTo  (0);
				int     nIndexCnt (0);

				ifs.read( reinterpret_cast<char*>( &nxNodeFrom),        sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &nxNodeAt),          sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &nxNodeTo),          sizeof(Integer) );
				ifs.read( reinterpret_cast<char*>( &nIndexCnt),         sizeof(int)     );
				
				if (ifs.eof())
				{
					break;
				}

				std::vector<double> vecVolume;
				for (int i= 0; i< nIndexCnt; i++)
				{
					double dVolume(0.0);
					ifs.read( reinterpret_cast<char*>( &dVolume),        sizeof(double) );
					vecVolume.push_back(dVolume);
				}

				AutoType itFind = a_mapAtNodeVolume.find(nxNodeAt);
				if (itFind != a_mapAtNodeVolume.end())
				{
					std::map<KODKey, std::vector<double>> &mapFTNodeVolume = itFind->second;

					KODKey oFTNode;
					oFTNode.OriginID      = nxNodeFrom;
					oFTNode.DestinationID = nxNodeTo;

					mapFTNodeVolume.insert(std::make_pair(oFTNode, vecVolume));
				}
				else
				{
					std::map<KODKey, std::vector<double>> mapFTNodeVolume;

					KODKey oFTNode;
					oFTNode.OriginID      = nxNodeFrom;
					oFTNode.DestinationID = nxNodeTo;

					mapFTNodeVolume.insert(std::make_pair(oFTNode, vecVolume));

					a_mapAtNodeVolume.insert(std::make_pair(nxNodeAt, mapFTNodeVolume));
				}
			}
		}
		else
		{
			throw 1;
		}

		ifs.close();
	}
	catch (...)
	{
		TxLogDebugException();
		ifs.close();
		a_mapAtNodeVolume.clear();
	}

	TxLogDebugEndMsg();
}


void KTurnVolumeViewDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR1,			  SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	SetResize(IDC_REPORT1,				  SZ_TOP_LEFT,     SZ_MIDDLE_RIGHT);
	SetResize(IDC_REPORT2,				  SZ_MIDDLE_LEFT,  SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_SEPRATOR2,		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_BTN_EXPORT,		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_CLEAR,		      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,				      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
}


// KTurnVolumeViewDlg 메시지 처리기입니다.

void KTurnVolumeViewDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
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
			ClearAtNodeView();
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


void KTurnVolumeViewDlg::InitializeNode()
{
	try
	{
		KBulkDBaseNode::Nodes( m_pTarget, m_mapAllNodeName );

		m_mapNodeData.clear();
		m_vecNodeIDCaption.clear();
		m_vecNodeSelectionInfo.clear();

		TPreferenceSymbol &oTNodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		AutoType iter  = m_mapAtNodeVolume.begin();
		AutoType itEnd = m_mapAtNodeVolume.end();
		
		std::set<Integer> setDeleteNodeID;
		CString strNodeID(_T(""));

		while (iter != itEnd)
		{
			Integer nxAtNodeID                                     = iter->first;
			std::map<KODKey, std::vector<double>> &mapFTNodeVolume = iter->second;

			AutoType itFind = m_mapAllNodeName.find(nxAtNodeID);
			if (itFind == m_mapAllNodeName.end())
			{
				setDeleteNodeID.insert(nxAtNodeID);

				++iter;
				continue;
			}

			CString &strNodeName = itFind->second;
			m_mapNodeData.insert(std::make_pair(nxAtNodeID, strNodeName));

			KIDCaption oIDCaption;
			strNodeID.Format(_T("%I64d"), nxAtNodeID);
			oIDCaption.TID      = strNodeID;
			oIDCaption.TCaption = strNodeName;
			m_vecNodeIDCaption.push_back(oIDCaption);
			
			TNodeSelectionInfo oNodeSelectInfo;
			oNodeSelectInfo.TNodeID = nxAtNodeID;
			oNodeSelectInfo.TCharacterIndex = oTNodeSymbol.nCharIndex;
			oNodeSelectInfo.TSize           = oTNodeSymbol.dSize;
			oNodeSelectInfo.TColorRGB       = oTNodeSymbol.clrSymbol;
			m_vecNodeSelectionInfo.push_back(oNodeSelectInfo);
			
			AutoType iterB  = mapFTNodeVolume.begin();
			AutoType itEndB = mapFTNodeVolume.end();

			std::set<KODKey> setDeleteODKey;

			while (iterB != itEndB)
			{
				KODKey               oKODKey   = iterB->first;

				AutoType itFindA = m_mapAllNodeName.find(oKODKey.OriginID);
				AutoType itFindB = m_mapAllNodeName.find(oKODKey.DestinationID);
				if (itFindA == m_mapAllNodeName.end() || itFindB == m_mapAllNodeName.end())
				{
					setDeleteODKey.insert(oKODKey);
				}

				++iterB;
			}

			AutoType iterG  = setDeleteODKey.begin();
			AutoType itEndG = setDeleteODKey.end();
			while (iterG != itEndG)
			{
				AutoType itFindC = mapFTNodeVolume.find(*iterG);
				if (itFindC != mapFTNodeVolume.end())
				{
					mapFTNodeVolume.erase(itFindC);
				}

				++iterG;
			}

			++iter;
		}

		AutoType iterD  = setDeleteNodeID.begin();
		AutoType itEndD = setDeleteNodeID.end();
		while (iterD != itEndD)
		{
			AutoType itFind = m_mapAtNodeVolume.find(*iterD);
			if (itFind != m_mapAtNodeVolume.end())
			{
				m_mapAtNodeVolume.erase(itFind);
			}

			++iterD;
		}

		ClearList();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTurnVolumeViewDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReportA, FALSE, FALSE, TRUE);
	KReportCtrlSetting::Default(m_ctrlReportB, FALSE, FALSE, FALSE);

	InitReportHeaderA();
	InitReportHeaderB();

	m_ctrlReportA.Populate();
	m_ctrlReportB.Populate();

	ResizeComponent();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
	pIOTable->Register(this);

	InitializeNode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTurnVolumeViewDlg::InitReportHeaderA()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_0_A_COLUMN_NODEID,    _T("At Node"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("노드ID"));
        }

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_1_A_COLUMN_NAME,      _T("Name"),   60));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("명칭"));
        }

		pColumn = m_ctrlReportA.AddColumn(new CXTPReportColumn(_2_A_COLUMN_DRAWBTN,   _T("Info"),   60, FALSE));
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


void KTurnVolumeViewDlg::InitReportHeaderB()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_0_B_COLUMN_NODE_FROM,    _T("From Node"), 10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시작노드"));
        }

		pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_1_B_COLUMN_NODE_AT,      _T("At Node"),   10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교차노드"));
        }

		pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn(_2_B_COLUMN_NODE_TO,      _T("To Node"),   10));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("끝노드"));
        }

		AutoType iter  = m_mapModeIndexName.begin();
		AutoType itEnd = m_mapModeIndexName.end();
		
		int nIndex(1);
		while (iter != itEnd)
		{
			CString strModeName = iter->second;

			pColumn = m_ctrlReportB.AddColumn(new CXTPReportColumn((_2_B_COLUMN_NODE_TO + nIndex),     strModeName,   10));
			pColumn->SetHeaderAlignment(DT_CENTER);

			nIndex++;

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::OnBnClickedButtonSelNode()
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


void KTurnVolumeViewDlg::SelectNodeList()
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


void KTurnVolumeViewDlg::ModelessOKFeedback( UINT nDialogID)
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
				UpdateReportDataB(m_setSearchedID);

				std::vector<Integer> vecDeleteViewID;
				CString strAtNodeID(_T(""));

				AutoType iter  = m_mapAtNodeViewDlg.begin();
				AutoType itEnd = m_mapAtNodeViewDlg.end();

				while (iter != itEnd)
				{
					Integer nxAtNodeID  = iter->first;
					strAtNodeID.Format(_T("%I64d"), nxAtNodeID);

					if (m_setSearchedID.find(strAtNodeID) == m_setSearchedID.end())
					{
						vecDeleteViewID.push_back(nxAtNodeID);
					}

					++iter;
				}

				for (size_t i= 0; i< vecDeleteViewID.size(); i++)
				{
					AutoType itFind = m_mapAtNodeViewDlg.find(vecDeleteViewID[i]);
					if (itFind != m_mapAtNodeViewDlg.end())
					{
						KTurnViewDlg* pDlg = itFind->second;

						if( nullptr != pDlg)
						{
							QBicDelete(pDlg);
						}

						m_mapAtNodeViewDlg.erase(itFind);
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


void KTurnVolumeViewDlg::UpdateReportDataA(std::set<CString> &a_setSeletedNode)
{
	try
	{
		m_ctrlReportA.ResetContent();

		AutoType iter  = a_setSeletedNode.begin();
		AutoType itEnd = a_setSeletedNode.end();

		Integer nxNodeAt(0);
		CString strNodeID(_T(""));
		CString strNodeName(_T(""));

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (iter != itEnd)
		{
			strNodeID    = *iter;
			nxNodeAt     = _ttoi64(strNodeID);

			AutoType itFindName = m_mapAllNodeName.find(nxNodeAt);
			if (itFindName != m_mapAllNodeName.end())
				strNodeName = itFindName->second;
			else
				strNodeName = _T("");

			pRecord = m_ctrlReportA.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetCaption(strNodeID);
			pItem->SetItemData((DWORD_PTR) nxNodeAt);
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText( strNodeName ));
			pItem->SetAlignment(DT_CENTER);

			pItem   = pRecord->AddItem(new CXTPReportRecordItem());
			pItem->SetItemData((DWORD_PTR) nxNodeAt);
			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(nullptr != pButton)
			{
                if (KmzSystem::GetLanguage() == KEMKorea) {
                    pButton->SetAlignment(xtpItemControlUnknown);
                    pButton->SetCaption(_T("보기"));
                    pButton->SetSize(CSize(60, 0));
                }
                else {
                    pButton->SetAlignment(xtpItemControlUnknown);
                    pButton->SetCaption(_T("Show"));
                    pButton->SetSize(CSize(60, 0));
                }
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


void KTurnVolumeViewDlg::UpdateReportDataB( std::set<CString> &a_setSeletedNode )
{
	try
	{
		m_ctrlReportB.ResetContent();

		AutoType iter  = a_setSeletedNode.begin();
		AutoType itEnd = a_setSeletedNode.end();

		size_t nxIndexCnt = m_mapModeIndexName.size();

		Integer nxNodeAt(0);
		CString strAtNodeID(_T(""));
		CString strFromNodeID(_T(""));
		CString strToNodeID(_T(""));

		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		while (iter != itEnd)
		{
			strAtNodeID    = *iter;
			nxNodeAt       = _ttoi64(strAtNodeID);

			AutoType itFind = m_mapAtNodeVolume.find(nxNodeAt);
			if (itFind == m_mapAtNodeVolume.end())
			{
				++iter;
				continue;
			}

			std::map<KODKey, std::vector<double>> &mapFTNodeVolume =  itFind->second;
			AutoType iterB  = mapFTNodeVolume.begin();
			AutoType itEndB = mapFTNodeVolume.end();

			while (iterB != itEndB)
			{
				KODKey               oKODKey   = iterB->first;
				std::vector<double> &vecVolume = iterB->second;

				strFromNodeID.Format(_T("%I64d"), oKODKey.OriginID);
				strToNodeID.Format  (_T("%I64d"), oKODKey.DestinationID);

				pRecord = m_ctrlReportB.AddRecord(new CXTPReportRecord());

				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strFromNodeID));
				pItem->SetItemData((DWORD_PTR)oKODKey.OriginID);
				pItem->SetAlignment(DT_CENTER);

				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strAtNodeID));
				pItem->SetItemData((DWORD_PTR)nxNodeAt);
				pItem->SetAlignment(DT_CENTER);

				pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strToNodeID));
				pItem->SetItemData((DWORD_PTR)oKODKey.DestinationID);
				pItem->SetAlignment(DT_CENTER);

				for (size_t i= 0; i< nxIndexCnt; i++)
				{
					double dVolume(0.0);
					if ( i < vecVolume.size())
					{
						dVolume = vecVolume[i];
					}
					 
					pItem   = pRecord->AddItem(new CXTPReportRecordItemNumber(dVolume, _T("%.2f")));
					pItem->SetAlignment(DT_RIGHT);
				}

				++iterB;
			}

			++iter;
		}

		m_ctrlReportB.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
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


void KTurnVolumeViewDlg::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	PostMessage(UM_FUNCTIONDRAW, (WPARAM)(pItemNotify->pItem));
}


LRESULT KTurnVolumeViewDlg::OnFunctionDraw(WPARAM wParam, LPARAM lParam)
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)wParam;

	try
	{
		Integer nxNodeAt = (Integer)pItem->GetItemData();
		ShowTurnVolumeView(nxNodeAt);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return 0;
}


void KTurnVolumeViewDlg::ShowTurnVolumeView(Integer &a_nxNodeID)
{
	try
	{
		Integer nxNodeAt = a_nxNodeID;

		AutoType itFind = m_mapAtNodeVolume.find(nxNodeAt);
		if (itFind == m_mapAtNodeVolume.end())
		{
			throw 1;
		}

		std::map<Integer, TFixedNode> mapNodePoint;
		LoadNetworkCrossNodeInfo(nxNodeAt, mapNodePoint);

		std::vector<TDrawNodeXY> vecNodes;  
		TDrawNodeXY oCenterNodeXY;

#pragma region TDrawNodeXY Data 수집
		AutoType iterB  = mapNodePoint.begin();
		AutoType itEndB = mapNodePoint.end();

		while (iterB != itEndB)
		{
			Integer      nxNodeID     = iterB->first;
			TFixedNode  &oTFixedNode  = iterB->second;

			if (nxNodeID == nxNodeAt)
			{
				oCenterNodeXY.NodeID = nxNodeID;
				oCenterNodeXY.Label = _T("Center");
				oCenterNodeXY.X = oTFixedNode.x;
				oCenterNodeXY.Y = oTFixedNode.y;
			}
			else
			{
				TDrawNodeXY oDrawNode;

				oDrawNode.NodeID = nxNodeID;
				CString strLabel(_T(""));
				strLabel.Format(_T("%I64d"), nxNodeID);
				oDrawNode.Label = strLabel;
				oDrawNode.X = oTFixedNode.x;
				oDrawNode.Y = oTFixedNode.y;

				vecNodes.push_back(oDrawNode);
			}

			++iterB;
		}
#pragma endregion TDrawNodeXY Data 수집

		CString strAtNodeName(_T(""));
		AutoType itFindName = m_mapAllNodeName.find(nxNodeAt);
		if (itFindName != m_mapAllNodeName.end())
		{
			strAtNodeName = itFindName->second;
		}

		std::map<KODKey, std::vector<double>> &mapFTNodeVolume = itFind->second;

		AutoType iterD  = m_mapAtNodeViewDlg.find(nxNodeAt);
		AutoType itEndD = m_mapAtNodeViewDlg.end();

		if (iterD != itEndD)
		{
			KTurnViewDlg* pDlg = iterD->second;

			if( nullptr != pDlg)
			{
				QBicDelete(pDlg);
			}

			m_mapAtNodeViewDlg.erase(iterD);
		}

		KTurnViewDlg* pTurnViewDlg = new KTurnViewDlg(m_mapModeIndexName, mapFTNodeVolume, m_pMapView, this);
		pTurnViewDlg->SetPrepareData(nxNodeAt, strAtNodeName, oCenterNodeXY, vecNodes);
		pTurnViewDlg->Create(KTurnViewDlg::IDD);
		pTurnViewDlg->ShowWindow(SW_SHOW);

		m_mapAtNodeViewDlg.insert(std::make_pair(nxNodeAt, pTurnViewDlg));
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::OnReportItemClickB(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pClickedItem = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		if (nullptr == pClickedItem)
			return;

		int nSelItemIndex = pClickedItem->pItem->GetIndex();
		if (_0_B_COLUMN_NODE_FROM <= nSelItemIndex && nSelItemIndex <= _2_B_COLUMN_NODE_TO)
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


bool KTurnVolumeViewDlg::LoadNetworkCrossNodeInfo(Integer &a_nxAtNodeID, std::map<Integer, TFixedNode> &a_mapNodePoint)
{
	a_mapNodePoint.clear();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	try
	{
		CString strSQL(_T(""));

		strSQL.Append      (_T(" SELECT TBL2.node_id, TBL2.x, TBL2.y "));
		strSQL.AppendFormat(_T(" FROM %s TBL2, "), TABLE_NODE);
		strSQL.AppendFormat(_T(" (SELECT fnode_id, tnode_id FROM %s WHERE tnode_id = %I64d OR fnode_id = %I64d)TBL1 "), TABLE_LINK, a_nxAtNodeID, a_nxAtNodeID);
		strSQL.Append      (_T(" WHERE TBL2.node_id = fnode_id OR TBL2.node_id = tnode_id "));
		strSQL.Append      (_T(" GROUP BY TBL2.node_id "));

		KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
		while (spResult->Next())
		{
			Integer nxNodeID       = spResult->GetValueInt64 (0);
			double  dX             = spResult->GetValueDouble(1);
			double  dY             = spResult->GetValueDouble(2);

			TFixedNode oTFixedNode;
			oTFixedNode.node_id = nxNodeID;
			oTFixedNode.x       = dX;
			oTFixedNode.y       = dY;

			a_mapNodePoint.insert(std::make_pair(nxNodeID, oTFixedNode));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		a_mapNodePoint.clear();
		return false;
	}

	return true;
}


void KTurnVolumeViewDlg::OnBnClickedButtonMap()
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


void KTurnVolumeViewDlg::MapSelectionFeedback( Integer a_nxID)
{
	try
	{
		//m_pMapView->ClearScreenDrawEdit();

		AddSearchID(a_nxID);
		ShowTurnVolumeView(a_nxID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
	try
	{
		//m_pMapView->ClearScreenDrawEdit();

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
		ShowTurnVolumeView(nxID);
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


void KTurnVolumeViewDlg::AddSearchID(Integer &a_nxID)
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
		UpdateReportDataB(m_setSearchedID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::OnBnClickedCancel()
{
	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}


void KTurnVolumeViewDlg::CloseWindowProc()
{
	try
	{
		ClearAtNodeView();

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		m_pMapView->ClearScreenDrawEdit();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTurnVolumeViewDlg::ClearAtNodeView()
{
	try
	{
		AutoType iter  = m_mapAtNodeViewDlg.begin();
		AutoType itEnd = m_mapAtNodeViewDlg.end();

		while (iter != itEnd)
		{
			KTurnViewDlg* pDlg = iter->second;
			if (nullptr != pDlg)
			{
				QBicDelete(pDlg);
			}

			++iter;
		}

		m_mapAtNodeViewDlg.clear();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


BOOL KTurnVolumeViewDlg::PreTranslateMessage(MSG* pMsg)
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


void KTurnVolumeViewDlg::OnBnClickedBtnExport()
{
	try
	{
		CXTPReportRecords* pRecords = m_ctrlReportB.GetRecords();
		int nRecordCnt   = pRecords->GetCount();
		if (nRecordCnt < 1)
		{
			return;
		}

		CWaitCursor cw;

		KScenario* pScenario = (KScenario*)m_pTarget->GetParentObject();

		CString strInitFileName(_T(""));
		strInitFileName.Format(_T("%s_%s_TurnVolume"), pScenario->GetName(), m_pTarget->GetName());

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
			CXTPReportColumns* pColumns = m_ctrlReportB.GetColumns();
			int nColCount    = pColumns->GetCount();
			for (int i= 0; i< nColCount; i++)
			{
				CXTPReportColumn*    pColumn = pColumns->GetAt(i);
				CString strColName = pColumn->GetCaption();

				strColName.Replace(_T(" "), _T("_"));

				if (i > 0)
				{
					strExport.Append(_T("\t"));
				}

				strExport.AppendFormat(_T("%s"), strColName);
			}
			strExport.Append(_T("\r\n"));
			of.WriteString(strExport);

			int nValueCnt = nColCount - 3;

			CXTPReportRecord*           pRecord  = nullptr;
			CXTPReportRecordItem*       pItem    = nullptr;
			CXTPReportRecordItemNumber* pItemNum = nullptr;

			for (int i= 0; i< nRecordCnt; i++)
			{
				pRecord = pRecords->GetAt(i);

				pItem   = pRecord->GetItem(_0_B_COLUMN_NODE_FROM);
				Integer nxNodeFrom = (Integer)pItem->GetItemData();

				pItem   = pRecord->GetItem(_1_B_COLUMN_NODE_AT);
				Integer nxNodeAt   = (Integer)pItem->GetItemData();

				pItem   = pRecord->GetItem(_2_B_COLUMN_NODE_TO);
				Integer nxNodeTo   = (Integer)pItem->GetItemData();

				strExport.Format(_T("%I64d\t%I64d\t%I64d"), nxNodeFrom, nxNodeAt, nxNodeTo);

				for (int k= 1; k<= nValueCnt; k++)
				{
					pItemNum = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_B_COLUMN_NODE_TO+k);
					double dVolume = pItemNum->GetValue();

					strExport.AppendFormat(_T("\t%f"), dVolume);
				}

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


void KTurnVolumeViewDlg::OnBnClickedBtnClear()
{
	ClearList();
	ClearAtNodeView();

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

	m_pMapView->ClearScreenDrawEdit();
}


void KTurnVolumeViewDlg::ClearList()
{
	try
	{
		m_strSearchedIDs = _T("");
		m_setSearchedID.clear();
		SetDlgItemText(IDC_EDIT3, m_strSearchedIDs);

		UpdateReportDataA(m_setSearchedID);
		UpdateReportDataB(m_setSearchedID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}