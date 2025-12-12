// ZoningDivisionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ZoningDivisionDlg.h"
#include "afxdialogex.h"
#include "ZoneSelectDlg.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"
#include "CustomReportRecordItem.h"
#include "MapView.h"
#include "BulkDBaseNode.h"
#include "BulkDBaseLink.h"
#include "NetworkShapeInsert.h"
#include "DefineNames.h"
#include "KBulkDBase2File.h"
#include "KBulkDBase.h"
//^#include "DBaseConnector.h"
#include "KmzSystem.h"
#include "TDrawNodeInfo.h"
#include "ImChampMapDefine.h"

#include "Launcher.h"
#include "ImChampDir.h"
#include "ImChampFrameWindow.h"
#include "ZoningMultiScenarioSelectDlg.h"

#include "WriteCodeAgrument.h"


// KZoningDivisionDlg 대화 상자입니다.

#define FILE_TYPE_COUNT_ZONE 2
#define FILE_TYPE_COUNT_OD   3
#define NODE_TYPE_ZONE       0

#define CLASSIFICATION_CODE_DIVISION 1

IMPLEMENT_DYNAMIC(KZoningDivisionDlg, KDialogEx)

bool KZoningDivisionDlg::m_bRunDivisionSucceess = false;

KZoningDivisionDlg::KZoningDivisionDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KZoningDivisionDlg::IDD, pParent)
	, m_bAutoGenZoneConn(FALSE)
{
    m_pTarget  = NULL;
	m_pRunningTarget = NULL;
    m_pMapView = NULL;
	m_nxSelectOrgZoneID = -1;
	m_strSearchedIDs = _T("");

}

KZoningDivisionDlg::~KZoningDivisionDlg()
{
	try
	{
		ClearDrawInfo(true);

		if (nullptr != m_spImTasMultiIDFind)
			m_spImTasMultiIDFind.reset();

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*        pTable        = pIOTables->FindTable(TABLE_ZONE);
		pTable->Unregister(this);

		m_backBrush.DeleteObject();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KZoningDivisionDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_O, m_wndOriginZoneReportCtrl);
	DDX_Control(pDX, IDC_REPORT_D, m_wndDivisionZoneReportCtrl);
	DDX_Check(pDX, IDC_CHECK1, m_bAutoGenZoneConn);
	DDX_Control(pDX, IDC_BTN_MULTI_SCENARIO, m_btn_multi);
}


BEGIN_MESSAGE_MAP(KZoningDivisionDlg, KDialogEx)
	ON_NOTIFY (NM_CLICK,                      IDC_REPORT_O, &KZoningDivisionDlg::OnReportItemClick)
    ON_NOTIFY (XTP_NM_REPORT_SELCHANGED,      IDC_REPORT_O, &KZoningDivisionDlg::OnOriginSelectChanged )
    ON_NOTIFY (XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT_O, &KZoningDivisionDlg::OnOriginValueChanged)
    ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT_D, &KZoningDivisionDlg::OnDivisionItemButtonClick)
    ON_NOTIFY (XTP_NM_REPORT_VALUECHANGED,    IDC_REPORT_D, &KZoningDivisionDlg::OnDivisionValueChanged)
    ON_BN_CLICKED(IDOK,                       &KZoningDivisionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,                   &KZoningDivisionDlg::OnBnClickedCancel)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BUTTON7, &KZoningDivisionDlg::OnBnClickedSelectOriginZone)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_MULTI_SCENARIO, &KZoningDivisionDlg::OnBnClickedBtnMultiScenario)
END_MESSAGE_MAP()


// KZoningDivisionDlg 메시지 처리기입니다.

void KZoningDivisionDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
		{
			InitializeNode();
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


void KZoningDivisionDlg::InitializeNode()
{
	CWaitCursor wc;

	m_mapZoneNodeData.clear();
	m_vecZoneNodeIDCaption.clear();

	m_strSearchedIDs = _T("");
	m_setSearchedID.clear();

	GetZoneNodeInfo();
}


bool KZoningDivisionDlg::GetZoneNodeInfo(void)
{
	TxLogDebugStartMsg();

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

		CString strSQL;
		strSQL.Format (_T(" SELECT zone_id, name FROM %s ORDER BY zone_id "), TABLE_ZONE);

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

		while( spResult->Next() )
		{
			nxNodeId	    = spResult->GetValueInt64 (0);
			strNodeName     = spResult->GetValueString(1);

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


void KZoningDivisionDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(!m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KZoningDivisionDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasMultiIDFind)
	{
		if(!m_spImTasMultiIDFind->IsUserClosed())
		{
			if(m_spImTasMultiIDFind->IsWindowVisible())
				m_spImTasMultiIDFind->ShowWindow(SW_HIDE);
		}
	}
}


BOOL KZoningDivisionDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitReportHeader();

	m_wndOriginZoneReportCtrl.Populate();
	m_wndDivisionZoneReportCtrl.Populate();

	if( !m_imageListSelectIcon.Create( 16, 16, ILC_MASK | ILC_COLOR24, 0, 1) )
	{
		AfxMessageBox( _T("Failed to create image list") );
		return -1;
	}

	CBitmap bmp;
	bmp.LoadBitmap( IDB_BITMAP_RADIO );
	m_imageListSelectIcon.Add( &bmp, CXTPImageManager::GetBitmapMaskColor(bmp, CPoint(0, 0) ) );

	m_wndOriginZoneReportCtrl.SetImageList(&m_imageListSelectIcon);

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*        pTable        = pIOTables->FindTable(TABLE_ZONE);
	pTable->Register(this);

	m_backBrush.CreateSolidBrush(RGB(255,255,255));

	GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);
	UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KZoningDivisionDlg::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	KReportCtrlSetting::Default(m_wndOriginZoneReportCtrl, TRUE, FALSE, FALSE);
	KReportCtrlSetting::Default(m_wndDivisionZoneReportCtrl, TRUE, FALSE, FALSE);

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"),  40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Zone ID"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("존 노드ID"));
    }

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Division Count"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("세분화 개수"));
    }

	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();

	CString strTemp;
	for (int i=2; i<100; i++)
	{
		strTemp.Format(_T("%d"), i);
		pEditOptions->AddConstraint(strTemp, i);
	}

	pColumn = m_wndOriginZoneReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Note"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("진행상태"));
    }

	//report B
	CXTPReportPaintManager *pPaintMgr = m_wndOriginZoneReportCtrl.GetPaintManager();
	pPaintMgr->m_strNoItems.Format(_T(""));
	// 2020.02.11 - Han He : 기존 메세지 삭제
	/*if (KmzSystem::GetLanguage() == KEMKorea) {
		pPaintMgr->m_strNoItems.Format(_T("분할하고자 하는 존을 선택해 주세요"));
	}*/

	pColumn = m_wndDivisionZoneReportCtrl.AddColumn(new CXTPReportColumn(0, _T("No"),  20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("번호"));
    }

	pColumn = m_wndDivisionZoneReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Name"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("새 명칭"));
    }

	pColumn = m_wndDivisionZoneReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Rate"), 30));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("세분화비율(%)"));
    }

	pColumn = m_wndDivisionZoneReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Map"),  50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("진행상태"));
    }

	pPaintMgr = m_wndDivisionZoneReportCtrl.GetPaintManager();
	pPaintMgr->m_strNoItems.Format(_T(""));
	// 2020.02.11 - Han He : 기존 메세지 삭제
	//pPaintMgr->m_strNoItems.Format(_T("Original 존을 선택해 주세요"));
}


void KZoningDivisionDlg::ModelessOKFeedback( UINT nDialogID)
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

				SetDlgItemText(IDC_EDIT4, m_strSearchedIDs);

				// 더하기
				std::set<CString>::iterator iterSelectedZone = m_setSearchedID.begin();
				std::set<CString>::iterator endSelectedZone  = m_setSearchedID.end();

				while (iterSelectedZone != endSelectedZone)
				{
					Integer nxZoneID     = _ttoi64(*iterSelectedZone);
					CString strZoneName  = GetNodeName(m_mapZoneNodeData, nxZoneID);

					if (m_mapZoneDivisionInfo.find(nxZoneID) == m_mapZoneDivisionInfo.end())
					{
						KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = KZoningOriginZoneInfoPtr(new KZoningOriginZoneInfo());
						m_mapZoneDivisionInfo.insert(std::make_pair(nxZoneID, spZoningOriginZoneInfo));

						std::vector<KZoneDivisionInfoPtr> &vecZoneDivisionInfo = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
						for (size_t i= 0; i< vecZoneDivisionInfo.size(); i++)
						{
							CString &strDefaultName = vecZoneDivisionInfo[i]->TZoneName;
							if (strZoneName.IsEmpty())
							{
								strDefaultName = _T("");
							}
							else
							{
								strDefaultName.Format(_T("%s_%d"), strZoneName, (i+1));
							}
						}
					}

					++iterSelectedZone;
				}

				// 빼기
				iterSelectedZone = m_setSearchedID.begin();
				endSelectedZone  = m_setSearchedID.end();

				AutoType iter    = m_mapZoneDivisionInfo.begin();
				AutoType end     = m_mapZoneDivisionInfo.end();

				while(iter != end)
				{
					Integer nxZoneID = iter->first;
					CString strZoneID(_T(""));
					strZoneID.Format(_T("%I64d"), nxZoneID);

					iterSelectedZone = m_setSearchedID.find(strZoneID);
					if (iterSelectedZone == endSelectedZone)
					{
						m_mapZoneDivisionInfo.erase(iter);
						iter = m_mapZoneDivisionInfo.begin();
						end  = m_mapZoneDivisionInfo.end();
					}
					else
					{
						++iter;
					}
				}

				m_wndDivisionZoneReportCtrl.ResetContent();

				DisplayOriginalZone();
			}
			catch (...)
			{
				TxLogDebugException();				
			}
		}
	}
}


CString KZoningDivisionDlg::GetNodeName(std::map<Integer, CString> &a_mapZoneNodeData, Integer a_nxID)
{
	CString strName(_T(""));

	try
	{
		AutoType iter  = a_mapZoneNodeData.find(a_nxID);
		if (iter != a_mapZoneNodeData.end())
		{
			strName = iter->second;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		strName = _T("");
	}

	return strName;
}


void KZoningDivisionDlg::DisplayOriginalZone()
{
	if (KmzSystem::GetLanguage() == KEMKorea) {
		SetDlgItemText(IDC_STATIC_Division, _T("세분화 할 존 :"));
	}
	else {
		SetDlgItemText(IDC_STATIC_Division, _T("Division Zone :"));
	}

    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

    CXTPReportRecord*     pRecord       = NULL;
    CXTPReportRecordItem* pRecordItem   = NULL;

    m_wndOriginZoneReportCtrl.ResetContent(FALSE);
    CString strTemp;
    while (iter != end)
    {
        Integer nxZoneID = iter->first;
        strTemp.Format(_T("%I64d"), nxZoneID);
        pRecord = m_wndOriginZoneReportCtrl.AddRecord(new CXTPReportRecord());

		pRecordItem = pRecord->AddItem(new CXTPReportRecordItem);
		pRecordItem->SetAlignment(xtpColumnIconCenter);
		pRecordItem->SetItemData((DWORD_PTR)nxZoneID);
		pRecordItem->SetIconIndex(0);
        pRecordItem->SetEditable(FALSE);

        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetEditable(FALSE);

        pRecordItem = pRecord->AddItem(new CCustomReportRecordItem());
        pRecordItem->SetItemData(iter->second->TDivisionCount);
        pRecordItem->SetAlignment(DT_CENTER);

        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetEditable(FALSE);
        ++iter;
    }

    UpdateNote();
    m_wndOriginZoneReportCtrl.Populate();

	if (m_mapZoneDivisionInfo.size() > 0)
	{
		 pRecord = m_wndOriginZoneReportCtrl.GetRecords()->GetAt(0); // 첫번째 Record
		 UpdateDivisionZone(pRecord);
	}
}


void KZoningDivisionDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	Integer nxZoneID(0);
	CXTPReportRow* pRow = pItemNotify->pRow;
	if(NULL != pRow)
	{
		CXTPReportRecord*     pRecord  = pRow->GetRecord();
		CXTPReportRecordItem* pItem    = pRecord->GetItem(0);
		Integer               nxZoneID = (Integer)pItem->GetItemData();

		if (pItem->GetIconIndex() == 1) { // 이미 선택된 라인에 대한 클릭이벤트는 무의미 하다.

			if (2 != nIndex) {
				m_pMapView->PanToFeature(KLayerID::Node(), nxZoneID );
			}

			return;
		}
	}

	CXTPReportRecords* pRecords = m_wndOriginZoneReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i=0; i<nRecordCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetIconIndex(0);
	}    

	//CXTPReportRow* pRow = m_wndOriginZoneReportCtrl.GetFocusedRow();
	if(NULL != pRow)
	{
		CXTPReportRecord* pRecord = pRow->GetRecord();
		UpdateDivisionZone(pRecord);
	}
	m_wndOriginZoneReportCtrl.RedrawControl();
}


void KZoningDivisionDlg::UpdateDivisionZone(CXTPReportRecord* a_pRecord)
{
	try
	{
		bool bRefresh(false);
		
		CXTPReportRecordItem* pItem = a_pRecord->GetItem(0);
		pItem->SetIconIndex(1);

		m_wndOriginZoneReportCtrl.RedrawControl();

		m_nxSelectOrgZoneID = (Integer)pItem->GetItemData();
		DisplayDivisionZone(m_nxSelectOrgZoneID);

		CString strTemp;
		if (KmzSystem::GetLanguage() == KEMKorea) {
			strTemp.Format(_T("세분화 할 존 : %I64d"), m_nxSelectOrgZoneID);
		}
		else {
			strTemp.Format(_T("Division Zone : %I64d"), m_nxSelectOrgZoneID);
		}
		SetDlgItemText(IDC_STATIC_Division, strTemp);

		DrawZoneAccess(m_nxSelectOrgZoneID, bRefresh);

		AutoType iter = m_mapZoneDivisionInfo.find(m_nxSelectOrgZoneID);
		AutoType end  = m_mapZoneDivisionInfo.end();
		if (iter == end) return;

		KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = iter->second;
		DrawAddedZone(spZoningOriginZoneInfo->TvecZoneDivisionInfo, bRefresh);

		bRefresh = true;
		m_pMapView->PanToFeature(KLayerID::Node(), m_nxSelectOrgZoneID, bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoningDivisionDlg::DrawZoneAccess(Integer &a_nxZoneID, bool a_bRefresh)
{
	try
	{
		ClearDrawInfo(false);

        TPreferenceSymbol& oZoneSymbol  = ImChampMapDefineSymbol::ZoneSymbol;
        CString strZoneID(_T("")); {
            strZoneID.Format(_T("%I64d"), a_nxZoneID);
        }
        
		std::vector<TDrawZoneAccess> vecZoneAccess;

		TDrawZoneAccess oDrawZoneAccess; {
            oDrawZoneAccess.TNodeID   = a_nxZoneID;		
            oDrawZoneAccess.TSymbol   = oZoneSymbol.nCharIndex;
            oDrawZoneAccess.TColorRGB = oZoneSymbol.clrSymbol;
            oDrawZoneAccess.TSize     = oZoneSymbol.dSize;
            
            oDrawZoneAccess.TLabel  = strZoneID;
            oDrawZoneAccess.TRadius = 1.0;
        }

		vecZoneAccess.push_back(oDrawZoneAccess);

        //★ 노드 드로잉
        std::vector<TDrawNodeInfo> vecNodeInfo;

        //★ 해당 존에 포함된 노드
        std::vector<__int64> vecNode = ZoneNode(a_nxZoneID);		

        for (size_t i=0; i<vecNode.size(); i++) {
            __int64 nxID = vecNode[i];

            if (nxID == a_nxZoneID)
                continue;

            TDrawNodeInfo oNodeInfo; {
                oNodeInfo.TNodeID   = nxID;
                oNodeInfo.TLabel    = _T("");
                oNodeInfo.TColorRGB = oZoneSymbol.clrSymbol;
                oNodeInfo.TSize     = 8;
                oNodeInfo.TSymbol   = 48;
            }		

            vecNodeInfo.push_back(oNodeInfo);
        }

		TDrawNodeInfo oNodeInfo; {
            oNodeInfo.TNodeID   = a_nxZoneID;
            oNodeInfo.TLabel    = strZoneID;
            oNodeInfo.TColorRGB = oZoneSymbol.clrSymbol;
            oNodeInfo.TSize     = oZoneSymbol.dSize;
            oNodeInfo.TSymbol   = oZoneSymbol.nCharIndex;
        }		

		vecNodeInfo.push_back(oNodeInfo);

		m_pMapView->TxDrawCompositeNode(vecNodeInfo, true, false);
		m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);

		if (a_bRefresh) {
			m_pMapView->MapRefresh();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

std::vector<__int64> KZoningDivisionDlg::ZoneNode( __int64 nxZoneID )
{
    std::vector<__int64> vecNode;
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  node_id "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T("  Node "));
        strSQL.AppendFormat(_T(" Where zone_id = '%I64d' "), nxZoneID);
    }

    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
    while (spResult->Next()) {
        __int64 nxID = spResult->GetValueI64(0);
        vecNode.push_back(nxID);
    }

    return vecNode;
}

void KZoningDivisionDlg::ClearDrawInfo(bool a_bRefresh)
{
	try
	{
		m_pMapView->TxClearCompositeThemes(a_bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoningDivisionDlg::OnOriginSelectChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
}


void KZoningDivisionDlg::OnOriginValueChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
    XTP_NM_REPORTRECORDITEM* pItemNotify  = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
    CXTPReportRecord*        pRecord      = pItemNotify->pItem->GetRecord();
    CXTPReportRecordItem*    pItem        = pRecord->GetItem(0);

    Integer nxOriginalZoneID = (Integer)pItem->GetItemData();
    pItem                    = pRecord->GetItem(2);
    int     nDivisionCount   = (int)pItem->GetItemData();
	CString strZoneName      = GetNodeName(m_mapZoneNodeData, nxOriginalZoneID);

    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(nxOriginalZoneID);
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();
    if (iter == end) return;

    KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = iter->second;
    spZoningOriginZoneInfo->TDivisionCount = nDivisionCount;
    // 복사 - TvecZonDivisionInfo.push_back(KZoneDivisionInfoPtr(new KZoneDivisionInfo(50.0)));
    std::vector<KZoneDivisionInfoPtr> vecZonDivisionInfo = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
    spZoningOriginZoneInfo->TvecZoneDivisionInfo.clear();

    int nCount   = (int)vecZonDivisionInfo.size();
    double dRate = (int)(100.0 / nDivisionCount);
    double dSum  = 0.0;

    for (int i=0; i<nDivisionCount; i++)
    {
        //★ 2016-09-20 : 갯수가 변경되면 Clear
        if (true || i >= nCount)
        {
			CString strDefaultName(_T(""));

			if (!strZoneName.IsEmpty())
			{
				strDefaultName.Format(_T("%s_%d"), strZoneName,(i+1));
			}
			
            spZoningOriginZoneInfo->TvecZoneDivisionInfo.push_back(KZoneDivisionInfoPtr(new KZoneDivisionInfo(50.0, strDefaultName)));
        }
        else
        {
            spZoningOriginZoneInfo->TvecZoneDivisionInfo.push_back(vecZonDivisionInfo[i]);
        }

        if (i == nDivisionCount-1)
        {
            spZoningOriginZoneInfo->TvecZoneDivisionInfo[i]->TRate = 100.0 - dRate*i;
        }
        else
        {
            spZoningOriginZoneInfo->TvecZoneDivisionInfo[i]->TRate = dRate;
        }
    }

    DisplayDivisionZone(nxOriginalZoneID);    
    m_wndOriginZoneReportCtrl.Populate();
    UpdateNote();

	DrawAddedZone(spZoningOriginZoneInfo->TvecZoneDivisionInfo, true);
}


void KZoningDivisionDlg::DisplayDivisionZone( Integer a_nxOriginalZoneID )
{
    m_wndDivisionZoneReportCtrl.ResetContent();
    //std::map<Integer, KZoningOriginZoneInfoPtr> m_mapZoneDivisionInfo;
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(a_nxOriginalZoneID);
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

    if (iter == end) return;

    KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = iter->second;
    
    int nCount = (int)spZoningOriginZoneInfo->TDivisionCount;
    std::vector<KZoneDivisionInfoPtr>& vecZonDivisionInfo = spZoningOriginZoneInfo->TvecZoneDivisionInfo;

    CXTPReportRecord*     pRecord       = NULL;
    CXTPReportRecordItem* pRecordItem   = NULL;

    for (int i=0; i<nCount; i++)
    {
        KZoneDivisionInfoPtr spZoneDivisionInfo = vecZonDivisionInfo[i];
        pRecord = m_wndDivisionZoneReportCtrl.AddRecord(new CXTPReportRecord());

        CString strTemp;
        strTemp.Format(_T("%d"), i+1);

        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(strTemp));
        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetItemData((DWORD_PTR)i);
        pRecordItem->SetEditable(FALSE);
                
        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(spZoneDivisionInfo->TZoneName));
        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetItemData((DWORD_PTR)i);
        pRecordItem->SetEditable(TRUE);
                
        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemNumber(spZoneDivisionInfo->TRate, _T("%.2f")));
        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetItemData((DWORD_PTR)i);
        pRecordItem->SetEditable(TRUE);

        pRecordItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("")));
        if (spZoneDivisionInfo->TNodeGeometry == NULL)
        {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pRecordItem->SetCaption(_T("존추가(X)"));
			}
			else {
				pRecordItem->SetCaption(_T("Not Added"));
			} 
        }
        else
        {
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pRecordItem->SetCaption(_T("존추가(O)"));
			}
			else {
				pRecordItem->SetCaption(_T("Add Complete"));
			} 
        }

        pRecordItem->SetAlignment(DT_CENTER);
        pRecordItem->SetItemData((DWORD_PTR)i);
        pRecordItem->SetEditable(FALSE);

        CXTPReportRecordItemControl* pButton = pRecordItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
        if(NULL != pButton)
        {
            pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pButton->SetCaption(_T("지도"));
				pButton->SetSize(CSize(25, 0));
			}
			else {
				pButton->SetCaption(_T("map"));
				pButton->SetSize(CSize(25, 0));
			}
        }
    }

    m_wndDivisionZoneReportCtrl.Populate();
}


void KZoningDivisionDlg::OnDivisionItemButtonClick( NMHDR * pNMHDR, LRESULT* pResult )
{
    XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNMHDR;
    if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
        return;

    pItemNotify->pRow->SetSelected(TRUE); // jyk: 삭제 하면 안됨 - ReceivedZoneInfoFromMap() m_wndDivisionZoneReportCtrl.GetSelectedRows()
    CXTPReportRecordItem* pItem = pItemNotify->pItem;
    m_pMapView->ZoningProcessStart(this);
}


void KZoningDivisionDlg::ReceivedZoneInfoFromMap( TxPointPtr a_spNodeGeometry, TxPolygonPtr a_spDemarcationGeometry )
{
    if (m_nxSelectOrgZoneID < 0 || m_wndDivisionZoneReportCtrl.GetSelectedRows()->GetCount() == 0)
        return;

    CXTPReportRow* pDivisionRow = m_wndDivisionZoneReportCtrl.GetSelectedRows()->GetAt(0);
    
    if(NULL == pDivisionRow)
    {
        return;        
    }

    CXTPReportRecord*     pDivisionRecord = pDivisionRow->GetRecord();
    CXTPReportRecordItem* pDivisionItem   = pDivisionRecord->GetItem(3);

    size_t  nxIndex          = (size_t) pDivisionItem->GetItemData();

    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(m_nxSelectOrgZoneID);
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

    if (iter == end) return;

    KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = iter->second;
    int nCount = spZoningOriginZoneInfo->TvecZoneDivisionInfo.size();

    if (nCount <= (int)nxIndex) return;

    spZoningOriginZoneInfo->TvecZoneDivisionInfo[nxIndex]->TNodeGeometry        = a_spNodeGeometry;
    spZoningOriginZoneInfo->TvecZoneDivisionInfo[nxIndex]->TDemarcationGeometry = a_spDemarcationGeometry;

    pDivisionItem->SetCaption(_T("Add Complete"));
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pDivisionItem->SetCaption(_T("존추가(O)"));
	}
    UpdateNote();
    m_wndOriginZoneReportCtrl.Populate();

	DrawAddedZone(spZoningOriginZoneInfo->TvecZoneDivisionInfo, true);
}


void KZoningDivisionDlg::DrawAddedZone(const std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo, bool a_bRefresh)
{
	try
	{
		size_t nxCount = vecZoneDivisionInfo.size();
		double dX(0.0), dY(0.0);
		std::vector<TUserPointInfo> vecUserPoint;
		TPreferenceSymbol &ZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		for (size_t i= 0; i< nxCount; i++)
		{
			TxPointPtr spPoint = vecZoneDivisionInfo[i]->TNodeGeometry;

			if (nullptr == spPoint)
				continue;

			dX = spPoint->X;
            dY = spPoint->Y;

			TUserPointInfo oTUserPointInfo;
			oTUserPointInfo.Tx = dX;
			oTUserPointInfo.Ty = dY;
			
			oTUserPointInfo.TSize = ZoneSymbol.dSize;

            CString strTemp(_T("")); {
                strTemp.Format(_T("%I64d - %d"), m_nxSelectOrgZoneID, i+1);
            }

			oTUserPointInfo.TLabel = strTemp;

			//oTUserPointInfo.TColorRGB       = ZoneSymbol.clrSymbol;
			oTUserPointInfo.TColorRGB       = RGB(255,128,0);
			oTUserPointInfo.TMarker = ZoneSymbol.nCharIndex;

			vecUserPoint.push_back(oTUserPointInfo);
		}

		m_pMapView->TxDrawCompositeUserPoint(vecUserPoint, true, a_bRefresh);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KZoningDivisionDlg::OnDivisionValueChanged( NMHDR* pNMHDR, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM*    pItemNotify  = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
		CXTPReportRecord*           pRecord      = pItemNotify->pItem->GetRecord();
		CXTPReportRecordItemText*   pItemName    = (CXTPReportRecordItemText*)  pRecord->GetItem(1);
		CXTPReportRecordItemNumber* pItemRate    = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);

		CXTPReportRecord*           pOriginalRecord = GetSelectedOriginRecord();
		CXTPReportRecordItem*       pOriginalItem = pOriginalRecord->GetItem(0);

		Integer nxOriginalZoneID = (Integer)pOriginalItem->GetItemData();
		size_t  nxIndex          = (size_t) pItemName->GetItemData();

		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(nxOriginalZoneID);
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

		if (iter == end) return;

		KZoningOriginZoneInfoPtr spZoningOriginZoneInfo = iter->second;
		int nCount = spZoningOriginZoneInfo->TvecZoneDivisionInfo.size();

		if (nCount <= (int)nxIndex) return;

		spZoningOriginZoneInfo->TvecZoneDivisionInfo[nxIndex]->TZoneName = pItemName->GetValue();
		double dValue = pItemRate->GetValue();
		if (dValue >= 100.0)
		{
			dValue = 0.0;
			pItemRate->SetValue(dValue);
		}
		spZoningOriginZoneInfo->TvecZoneDivisionInfo[nxIndex]->TRate     = dValue;

		if (true) { // 맨 마지막 라인의대한 처리 ( {last value = 100 - (last제외 value들의 합)}  )
			int    nDivisionCount = (int)spZoningOriginZoneInfo->TvecZoneDivisionInfo.size();
			double dTempValue(0.0); // last제외 value들의 합
			for (int i= 0; i< nDivisionCount -1; i++) {
				dTempValue += spZoningOriginZoneInfo->TvecZoneDivisionInfo[i]->TRate;
			}

			double dLastValue(0.0);
			if (dTempValue >= 100.0) {
				dLastValue = 0.0;
			} else {
				dLastValue = 100.0 - dTempValue;
			}

			CXTPReportRecords* pRecords    = m_wndDivisionZoneReportCtrl.GetRecords();
			int                nLastIndex  = pRecords->GetCount() -1;
			CXTPReportRecord*  pLastRecord = pRecords->GetAt(nLastIndex);
			CXTPReportRecordItemNumber* pLastItemRate = (CXTPReportRecordItemNumber*)pLastRecord->GetItem(2);
			pLastItemRate->SetValue(dLastValue);

			spZoningOriginZoneInfo->TvecZoneDivisionInfo[nLastIndex]->TRate = dLastValue;
		}

		UpdateNote();		
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


CXTPReportRecord* KZoningDivisionDlg::GetSelectedOriginRecord()
{
	try
	{
		CXTPReportRecords*    pRecords = m_wndOriginZoneReportCtrl.GetRecords();
		CXTPReportRecord*     pRecord(nullptr);
		CXTPReportRecordItem* pItem(nullptr);

		int nRecordCnt   = pRecords->GetCount();
		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(0);

			if (pItem->GetIconIndex() == 0)
			{
				continue;
			}

			return pRecord;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return nullptr;
}


void KZoningDivisionDlg::UpdateNote()
{
    CXTPReportRecords* pRecords = m_wndOriginZoneReportCtrl.GetRecords();
    int nRecordCount = pRecords->GetCount();

	int nCompleteCount = 0;

    for (int i=0; i<nRecordCount; i++)
    {
        CXTPReportRecord*     pRecord   = pRecords->GetAt(i);
        CXTPReportRecordItem* pItem     = pRecord->GetItem(0);
        CXTPReportRecordItem* pItemNote = pRecord->GetItem(3);

        Integer nxOriginalZoneID = (Integer)pItem->GetItemData();
        std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(nxOriginalZoneID);
        std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();
        if (iter == end) continue;
        
        switch(IsInputDone(iter->second))
        {
        case KEMInputOK:
            pItemNote->SetCaption(_T("Enter Complete"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("입력완료"));
			}
            pItemNote->SetBackgroundColor(RGB(0, 255, 0));
			++nCompleteCount;
            break;
        case KEMInputErrorMap:
            pItemNote->SetCaption(_T("Not Set"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("존 미입력"));
			}
            pItemNote->SetBackgroundColor(RGB(255, 201, 14));
            break;
        case KEMInputErrorRate:
            pItemNote->SetCaption(_T("Incorrect Rate"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pItemNote->SetCaption(_T("비율오류"));
			}
            pItemNote->SetBackgroundColor(RGB(255, 201, 14));
            break;
        default:
            pItemNote->SetCaption(_T("-"));
            pItemNote->SetBackgroundColor(RGB(255, 0, 0));
            break;
        } 
    }    
	m_wndOriginZoneReportCtrl.RedrawControl();

	if ((nRecordCount == 0) || (nRecordCount != nCompleteCount))
	{
		GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(TRUE);
	}
}

KEMInputError KZoningDivisionDlg::IsInputDone( KZoningOriginZoneInfoPtr a_spZoningOriginZoneInfoPtr )
{
    std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo = a_spZoningOriginZoneInfoPtr->TvecZoneDivisionInfo;
    size_t nxCount = vecZoneDivisionInfo.size();

    double dRateSum = 0.0;
    for (size_t i=0; i<nxCount; i++)
    {
        KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
        dRateSum += spZoneDivisionInfo->TRate;

        if (spZoneDivisionInfo->TNodeGeometry == NULL || spZoneDivisionInfo->TDemarcationGeometry == NULL)
        {
            return KEMInputErrorMap;
        }
    }

    if (dRateSum != 100.0)
    {
        return KEMInputErrorRate;
    }

    return KEMInputOK;
}


void KZoningDivisionDlg::OnBnClickedCancel()
{
	ClearDrawInfo(true);

	if (nullptr != m_spImTasMultiIDFind)
		m_spImTasMultiIDFind.reset();

    if (m_pMapView) {
        m_pMapView->SetMapMode(KEMMapModePan);
    }

	KDialogEx::OnCancel();
}


void KZoningDivisionDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pRunningTarget = m_pTarget;

	m_nResultCode = DLL_UNEXPECTED_ERROR;

    CString strError;
    if (!InputCheck(strError)) {
        AfxMessageBox(strError);
        return;
    }

    // 모형을 돌리기 전에 반드시...
    GenerateNewZoneID();
    //GenerateDivisionInfo();

	// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
	int nLang = 1;
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		nLang = 0;
	}

	QBicSimpleProgressThread::ExecuteThread(DivisionZoneThreadCaller, this, false, nLang);

    // GIS 입력은 Thread로 돌리면 정상적 수행이 안됨.
    // GIS 입력 부분은 QBicDonkeyProgress....
    // 

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{

	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 세분화/집합화가 중단되었습니다."));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("User stopped."));
		}
		return;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AfxMessageBox(_T("Not Found Dll"));
		return;
	}
	else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while creating the Argument file !"));
		}
		return;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 세분화 수행 중 오류가 발생하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while executing !"));
		}
		return;
	}

    CWaitCursor wc;

    bool bTemp = ReadOutPutFile();
    if (!bTemp )
    {
        //AfxMessageBox(_T("세분화한 존 정보 입력에 실패하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("세분화한 존 정보 입력에 실패하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while writing output data !"));
		}
        TxLogDebug(_T("Output Division 정보 입력 실패..."));
        return ;
    }

    if(! UpdateDivisionSystemInfo())
    {
        //AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다."));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다 !"));
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			AfxMessageBox(_T("Error occurred while writing output data !"));
		}
        return ;
    }

	if( m_bRunDivisionSucceess)
	{
		// 항목 리셋
		m_wndOriginZoneReportCtrl.ResetContent();
		m_wndDivisionZoneReportCtrl.ResetContent();
		m_mapZoneDivisionInfo.clear();

		m_strSearchedIDs = _T("");
		SetDlgItemText(IDC_EDIT4, m_strSearchedIDs);

		ClearDrawInfo(true);
	} 
    else {
		m_pMapView->MapRefresh();
	}

	NotifyRelationTable();
}

bool KZoningDivisionDlg::InputCheck( CString& ar_strError )
{
    CXTPReportRecords* pRecords = m_wndOriginZoneReportCtrl.GetRecords();
    int nRecordCount = pRecords->GetCount();

    for (int i=0; i<nRecordCount; i++)
    {
        CXTPReportRecord*     pRecord   = pRecords->GetAt(i);
        CXTPReportRecordItem* pItem     = pRecord->GetItem(0);
        CXTPReportRecordItem* pItemNote = pRecord->GetItem(3);

        Integer nxOriginalZoneID = (Integer)pItem->GetItemData();
        std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.find(nxOriginalZoneID);
        std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();
        if (iter == end) continue;

        switch(IsInputDone(iter->second))
        {        
        case KEMInputErrorMap:
           // ar_strError.Format(_T("%I64d - 존 미입력"), nxOriginalZoneID);
			if (KmzSystem::GetLanguage() == KEMKorea) {
				ar_strError.Format(_T("존 %I64d에 대해 세분화 되는 새로운 존을 설정해 주세요."), nxOriginalZoneID);
			}
			else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
				ar_strError.Format(_T("Specify the new zones for %I64d."), nxOriginalZoneID);
			}
            return false;
            break;
        case KEMInputErrorRate:
            ar_strError.Format(_T("%I64d - 비율오류\r\n Rate의 합은 100.0 이어야 합니다."), nxOriginalZoneID);
            return false;
            break;
        default:
            break;
        } 
    }    

    if (nRecordCount == 0)
    {
        //ar_strError = _T("분할하고자 하는 존을 선택해 주세요");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			ar_strError = _T("분할하고자 하는 존을 선택해 주세요");
		}
		else { //if (KmzSystem::GetLanguage() == KEMEnglish) {
			ar_strError = _T("Select the zones to be divided first.");
		}
        return false;
    }

    return true;
}


void KZoningDivisionDlg::GenerateNewZoneID()
{
    Integer nxMaxPlusOneZoneID = KBulkDBaseNode::MaxNodeID(m_pRunningTarget) + 1;
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

    while (iter != end)
    {
        KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
        std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
        size_t nxCount = vecZoneDivisionInfo.size();
        for (size_t i=0; i<nxCount; i++)
        {
            KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
            spZoneDivisionInfo->TZoneID = nxMaxPlusOneZoneID++;
        }

        ++iter;
    }
}


void KZoningDivisionDlg::GenerateDivisionInfo()
{
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

    CString strIndex;
    CString strOriginalZoneInfo;
    CString strDivisionZoneInfo;
    CString strDivisionRate;
    CString strTempZone;
    CString strTempRate;

    int     nIndex = 1;
    while (iter != end)
    {
        strIndex.Format(_T("Index : %d"), nIndex++);
        TxLogDebug((LPCTSTR)strIndex);

        strOriginalZoneInfo.Format(_T("Original Zone : %I64d"), iter->first);
        TxLogDebug((LPCTSTR)strOriginalZoneInfo);

        KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
        std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
        size_t nxCount = vecZoneDivisionInfo.size();

        strDivisionZoneInfo = _T("");
        strDivisionRate     = _T("");

        for (size_t i=0; i<nxCount; i++)
        {
            KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
            strTempZone.Format(_T("%I64d"), spZoneDivisionInfo->TZoneID);
            strTempRate.Format(_T("%f"), spZoneDivisionInfo->TRate);

            if (i != nxCount-1)
            {
                strDivisionZoneInfo = strDivisionZoneInfo + strTempZone + _T("\t");
                strDivisionRate     = strDivisionRate     + strTempRate + _T("\t");
            }
            else
            {
                strDivisionZoneInfo = strDivisionZoneInfo + strTempZone;
                strDivisionRate     = strDivisionRate     + strTempRate;
            }
        }

        TxLogDebug((LPCTSTR)strDivisionZoneInfo);
        TxLogDebug((LPCTSTR)strDivisionRate);
        ++iter;
    }
}


//void KZoningDivisionDlg::SaveGeometry()
//{
//    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
//    std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();
//
//	std::set<Integer>::iterator itDelLink    = m_setDeleteLinkID.begin();
//	std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();
//
//    TxFeatureLayerPtr  spNodeLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Node());
//        spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }
//    
//    TxFeatureLayerPtr  spLinkLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Link());
//        spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }
//
//    TxFeatureLayerPtr  spZoneLayer; {
//        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Zone());
//        spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
//    }    
//   
//    try
//    {
//		while (itDelLink != itDelLinkEnd)
//		{
//			Integer nxLinkID = *itDelLink;			
//            spLinkLayer->Delete(nxLinkID); //oNetworkImportTxt.DeleteLink(nxLinkID);
//			++itDelLink;
//		}
//
//        while (iter != end)
//        {
//            KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
//            std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
//            size_t nxCount = vecZoneDivisionInfo.size();
//
//            spZoneLayer->Delete(iter->first); //oNetworkImportTxt.DeleteZone(iter->first);
//			spNodeLayer->Delete(iter->first); //oNetworkImportTxt.DeleteNode(iter->first);
//			
//            for (size_t i=0; i<nxCount; i++)
//            {
//                KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
//                spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);        //oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
//                spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry); //oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);
//            }
//
//            ++iter;
//        }
//
//        //★ 노드, 존 정보 갱신
//        m_pMapView->ReloadRender(false, 1);
//        m_pMapView->ReloadRender(false, 3);
//    }
//    catch (...)
//    {
//        ThrowException(_T("Shape 입력 오류"));
//    }
//}
void KZoningDivisionDlg::SaveGeometry(KMapView* pMapView)
{
	if (NULL == pMapView)
	{
		return;
	}

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end = m_mapZoneDivisionInfo.end();

	std::set<Integer>::iterator itDelLink = m_setDeleteLinkID.begin();
	std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();

	TxFeatureLayerPtr  spNodeLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Node());
		spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	TxFeatureLayerPtr  spLinkLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
		spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	TxFeatureLayerPtr  spZoneLayer; {
		ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Zone());
		spZoneLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
	}

	try
	{
		while (itDelLink != itDelLinkEnd)
		{
			Integer nxLinkID = *itDelLink;
			spLinkLayer->Delete(nxLinkID); //oNetworkImportTxt.DeleteLink(nxLinkID);
			++itDelLink;
		}

		while (iter != end)
		{
			KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
			std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
			size_t nxCount = vecZoneDivisionInfo.size();


			spZoneLayer->Delete(iter->first); //oNetworkImportTxt.DeleteZone(iter->first);
			spNodeLayer->Delete(iter->first); //oNetworkImportTxt.DeleteNode(iter->first);

			for (size_t i = 0; i<nxCount; i++)
			{
				KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
				spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);        //oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
				spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry); //oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);
			}

			++iter;
		}

		//★ 노드, 존 정보 갱신
		pMapView->ReloadRender(false, 1);
		pMapView->ReloadRender(false, 3);
	}
	catch (...)
	{
		ThrowException(_T("Shape 입력 오류"));
	}
}


void KZoningDivisionDlg::SaveGeometry(KDBaseConPtr spDBConnection)
{
	try
	{
		ITxMapServerPtr spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, spDBConnection->GetConnectionString().m_strUrl));
		if (spMapServer->Connect(spDBConnection) != 1)
			throw 1;

		TxFeatureLayerPtr  spNodeLayer = nullptr;
		TxFeatureLayerPtr  spLinkLayer = nullptr;
		TxFeatureLayerPtr  spZoneLayer = nullptr;


		// 서버의 레이어 정보 조회
		std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
		for (size_t j = 0; j < vecLayerInfo.size(); j++) {
			TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

			if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
				spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
				spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_ZONE, spLayerInfo->Name()) == 0) { // 존 레이어 조회
				spZoneLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}
		}

		if (spNodeLayer == nullptr || spLinkLayer == nullptr || spZoneLayer == nullptr)
		{
			throw _T("some layer(s) is(are) not exists");
		}

		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end = m_mapZoneDivisionInfo.end();

		std::set<Integer>::iterator itDelLink = m_setDeleteLinkID.begin();
		std::set<Integer>::iterator itDelLinkEnd = m_setDeleteLinkID.end();

		try
		{
			while (itDelLink != itDelLinkEnd)
			{
				Integer nxLinkID = *itDelLink;
				spLinkLayer->Delete(nxLinkID); //oNetworkImportTxt.DeleteLink(nxLinkID);
				++itDelLink;
			}

			while (iter != end)
			{
				KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
				std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
				size_t nxCount = vecZoneDivisionInfo.size();

				spZoneLayer->Delete(iter->first); //oNetworkImportTxt.DeleteZone(iter->first);
				spNodeLayer->Delete(iter->first); //oNetworkImportTxt.DeleteNode(iter->first);

				for (size_t i = 0; i<nxCount; i++)
				{
					KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
					spNodeLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TNodeGeometry);        //oNetworkImportTxt.InsertNode(spZoneDivisionInfo->TZoneID, 0, spZoneDivisionInfo->TNodeGeometry);
					spZoneLayer->ProhibitGeoInsert(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry); //oNetworkImportTxt.InsertZone(spZoneDivisionInfo->TZoneID, spZoneDivisionInfo->TDemarcationGeometry);
				}

				++iter;
			}
		}
		catch (...)
		{
			CString strError = spDBConnection->GetConnectionString().m_strUrl;
			strError += _T(" Zone 세분화 처리 중 Layer 처리 오류");
			ThrowException(strError);
		}
	}
	catch (...)
	{
		ThrowException(_T("Shape 입력 오류"));
	}
}



void KZoningDivisionDlg::SaveGeometryCache()
{
}

unsigned __stdcall KZoningDivisionDlg::DivisionZoneThreadCaller(void* pParam)
{
	QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)pParam;

	QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

	KZoningDivisionDlg* pDlg = (KZoningDivisionDlg*)pParameter->GetParameter();;

	if (KmzSystem::GetLanguage() == KEMKorea) {
		//pProgressWindow->SetStatus(_T("존 세분화 모듈 실행 중..."));
		CString strStatus = pDlg->m_pRunningTarget->CaptionScenario();
		strStatus += _T(" - ");
		strStatus += pDlg->m_pRunningTarget->Caption();
		strStatus += _T(" 존 세분화 모듈 실행 중...");
		pProgressWindow->SetStatus(strStatus);
	}
	else {
		//pProgressWindow->SetStatus(_T("Run Zone Division Module..."));
		CString strStatus = pDlg->m_pRunningTarget->CaptionScenario();
		strStatus += _T(" - ");
		strStatus += pDlg->m_pRunningTarget->Caption();
		strStatus += _T(" Run Zone Division Module...");
	}


	m_bRunDivisionSucceess = pDlg->RunDivisionGeneration();

	return 0;

}


bool KZoningDivisionDlg::RunDivisionGeneration()
{
	TxLogDebugStartMsg();

	CString strModelFolder;
	//strModelFolder.Format(_T("%s%s"),m_pTarget->GetLocation(), DLL_READ_WRITE_PATH);
	strModelFolder.Format(_T("%s%s"), m_pRunningTarget->GetLocation(), DLL_READ_WRITE_PATH);
	CreateDirectory(strModelFolder, NULL);
	CreateDirectory(ConvertSavePath(_T("")), NULL);

	if( !CreateArgument())
	{
		m_nResultCode = DLL_CREATE_ARGFILE_ERROR;
		return false;
	}

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		m_nResultCode = DLL_USER_STOPPING;
		return false;
	}

	CString strWindowName(_T(""));
	GetWindowText(strWindowName);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("존 세분화 모듈 실행 중..."));
	}
	else {
		AddStatusMessage(_T("Run Zoning(Division) Module..."));
	}

	KLauncher::Run(strWindowName, 30);

	if (QBicSimpleProgressThread::IsUserStopped() == true)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		m_nResultCode = DLL_USER_STOPPING;
		return false;
	}

	if(DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("존 세분화 모듈 실행이 성공적으로 완료되었습니다."));
		}
		else {
			AddStatusMessage(_T("Zoning(Division) Module Complete."));
		}
		TxLogDebug(_T("Division Run Ok.."));
	}
	else if (DLL_USER_STOPPING == m_nResultCode)
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행을 중단하였습니다."));
		}
		else {
			AddStatusMessage(_T("Running Stopped."));
		}
		TxLogDebug(_T("Division Stop.."));

		return false;
	}
	else if (DLL_LOAD_ERROR == m_nResultCode)
	{
		AddStatusMessage(_T("> Zonning Dll Not Found ..."));
		TxLogDebug(_T("DLL NOT Found.."));

		return false;
	}
	else
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			AddStatusMessage(_T("모듈 실행이 예상치 못한 오류로 중지되었습니다."));
		}
		else {
			AddStatusMessage(_T("Running Failed."));
		}
		TxLogDebug(_T("Division Run Error.."));

		return false;
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
		return false;
	}

	/**************************************************************************/
	/* 2019.03.05 수정 : 처리 완료후 사용된 Zoning.arg 파일을 해당 시나리오 경로에 복사
	/**************************************************************************/
	CString strZoneArg = ConvertSavePath(T_ARG_ZONING);
	QBicFile::CopyFileW(T_ARG_ZONING, strZoneArg);
	/**************************************************************************/


	if (KmzSystem::GetLanguage() == KEMKorea) {
		AddStatusMessage(_T("실행 결과 데이터 저장 중..."));
	}
	else {
		AddStatusMessage(_T("Saving Output Data..."));
	}
	
	TxLogDebugEndMsg();
	return true;
}


bool KZoningDivisionDlg::ReadOutPutFile()
{
	TxLogDebugStartMsg();

	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	try
	{  
		spDBaseConnection->BeginTransaction();
        
		UpsertZoneData       (spDBaseConnection);		
		UpsertSedData        (spDBaseConnection);
		UpsertNodeSystemData (spDBaseConnection);
		DeleteLinkData       (spDBaseConnection);		
		UpsertPurposeODData  (spDBaseConnection);		
		UpsertModeODData     (spDBaseConnection);		
		UpsertParameterODData(spDBaseConnection);			
		DeleteAccessData     (spDBaseConnection);
		
		/**************************************************************************/
		/* 2019.01.15 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
		//SaveGeometry();
		KMapView* pMapView = ImChampFrameWindow::GetCreatedMapView(m_pRunningTarget);
		if (nullptr != pMapView)
		{
			SaveGeometry(pMapView);
		}
		else
		{
			// MapView가 없는 시나리오의 Geometry 저장
			SaveGeometry(spDBaseConnection);
		}

		if (m_bAutoGenZoneConn)
		{
			std::vector<TLinkFTNodeID> arrZoneConnectLink;
			InsertZoneConnectLink(spDBaseConnection, arrZoneConnectLink);
			if (nullptr != pMapView)
			{
				SaveZoneConnectLinkGeometry(arrZoneConnectLink, pMapView);
			}
			else
			{
				SaveZoneConnectLinkGeometry(arrZoneConnectLink, spDBaseConnection);
			}
		}
		/**************************************************************************/

		spDBaseConnection->Commit(); 

		TxLogDebugEndMsg();
		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return false;
}


bool KZoningDivisionDlg::UpdateDivisionSystemInfo()
{
	TxLogDebugStartMsg();

	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	try
	{  
		spDBaseConnection->BeginTransaction();

		UpdateZoneSystemData(spDBaseConnection);

		UpdateSedSystemData (spDBaseConnection);

		spDBaseConnection->Commit(); 
		
		TxLogDebugEndMsg();
		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{   
		TxLogDebugWarning();
	}

	try
	{
		spDBaseConnection->RollBack();
	}
	catch(...)
	{
		TxLogDebugWarning();
	}        

	return false;
}


void KZoningDivisionDlg::UpdateZoneSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format(_T(" UPDATE Zone SET Name = ?  where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter    = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end     = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
		std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
		size_t nxCount = vecZoneDivisionInfo.size();

		for (size_t i=0; i<nxCount; i++)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];

			spPrepareStatement->BindText (1, spZoneDivisionInfo->TZoneName    );
			spPrepareStatement->BindInt64(2, spZoneDivisionInfo->TZoneID      );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}
}


void KZoningDivisionDlg::UpdateSedSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLUpdate(_T(""));

	strSQLUpdate.Format(_T(" UPDATE Sed_Vector SET Name = ? where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter    = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end     = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
		std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
		size_t nxCount = vecZoneDivisionInfo.size();

		for (size_t i=0; i<nxCount; i++)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];

			spPrepareStatement->BindText (1, spZoneDivisionInfo->TZoneName    );
			spPrepareStatement->BindInt64(2, spZoneDivisionInfo->TZoneID      );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}
}


void KZoningDivisionDlg::UpsertNodeSystemData(KDBaseConPtr a_spDBaseConnection)
{
	CString  strSQLDelete(_T(""));
	strSQLDelete.Format(_T(" Delete From Node Where node_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		spPrepareStatement->BindInt64(1, iter->first);

		spPrepareStatement->ExecuteUpdate();
		spPrepareStatement->Reset();

		++iter;
	}

	CString strSQLUpdate(_T(""));
	strSQLUpdate.Format(_T(" UPDATE Node set zone_id = 0 where zone_id = ? "));

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLUpdate);

	iter    = m_mapZoneDivisionInfo.begin();
	end     = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		spPrepareStatement->BindInt64(1, iter->first);

		spPrepareStatement->ExecuteUpdate();
		spPrepareStatement->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert(_T(" Insert Into Node "));

	strSQLInsert     +=  _T(" ( node_id, name, node_type, x, y, zone_id ");

	strSQLInsert     += _T(" ) Values ( ?, ?, ?, ?, ?, ? ");

	strSQLInsert    += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	iter    = m_mapZoneDivisionInfo.begin();
	end     = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
		std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
		size_t nxCount = vecZoneDivisionInfo.size();

		for (size_t i=0; i<nxCount; i++)
		{
			KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];

			spPrepareStatement->BindInt64  (1, spZoneDivisionInfo->TZoneID      );
			spPrepareStatement->BindText   (2, spZoneDivisionInfo->TZoneName    );
			spPrepareStatement->BindInt    (3, NODE_TYPE_ZONE    );

			TxPointPtr spPoint =  spZoneDivisionInfo->TNodeGeometry;
			double    dMapX, dMapY;
			
            dMapX = spPoint->X;
            dMapY = spPoint->Y;

			spPrepareStatement->BindDouble (4, dMapX                            );
			spPrepareStatement->BindDouble (5, dMapY                            );
			spPrepareStatement->BindInt64  (6, spZoneDivisionInfo->TZoneID      );

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}

		++iter;
	}
}


void KZoningDivisionDlg::DeleteLinkData(KDBaseConPtr a_spDBaseConnection)
{
	m_setDeleteLinkID.clear();

	try
	{
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

		while (iter != end)
		{
			CString strSQLSelect;
			strSQLSelect.Format (_T(" SELECT Link_id FROM %s Where fnode_id = %I64d or tnode_id = %I64d "), TABLE_LINK, iter->first, iter->first);

			KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQLSelect);

			while( spResult->Next() )
			{
				Integer nxLinkID	= spResult->GetValueInt64(0);
				m_setDeleteLinkID.insert(nxLinkID);
			}

			++iter;
		}

		// delete 
		CString  strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" Delete From Link Where link_id = ? "));

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		std::set<Integer>::iterator itLink    = m_setDeleteLinkID.begin();
		std::set<Integer>::iterator itLinkEnd = m_setDeleteLinkID.end();

		while (itLink != itLinkEnd)
		{
			spPrepareStatement->BindInt64(1, *itLink);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itLink;
		}

		DeleteTransitData(a_spDBaseConnection, m_setDeleteLinkID);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoningDivisionDlg::DeleteTransitData(KDBaseConPtr a_spDBaseConnection, std::set<Integer> &a_setDeleteLinkID)
{
	try
	{
		AutoType iter  = a_setDeleteLinkID.begin();
		AutoType itEnd = a_setDeleteLinkID.end();

		// collect delete transit_ids
		std::set<Integer> setDeleteTransitID;

		while (iter != itEnd)
		{
			CString strSQLSelect;
			strSQLSelect.Format (_T(" SELECT Transit_id FROM %s WHERE link_id = %I64d "), TABLE_TRANSITLINKS, *iter); //★ 필요

			KResultSetPtr spResult = a_spDBaseConnection->ExecuteQuery(strSQLSelect);

			while( spResult->Next() ) {
				Integer nxTransitID	= spResult->GetValueInt64(0);
				setDeleteTransitID.insert(nxTransitID);
			}

			++iter;
		}

		// delete 
		CString  strSQLDelete(_T(""));

		strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSITLINKS); //★ 필요
		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		AutoType itTransit    = setDeleteTransitID.begin();
		AutoType itTransitEnd = setDeleteTransitID.end();

		while (itTransit != itTransitEnd)
		{
			spPrepareStatement->BindInt64(1, *itTransit);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itTransit;
		}

        strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSITNODES);
        spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

        itTransit    = setDeleteTransitID.begin();
        itTransitEnd = setDeleteTransitID.end();

        while (itTransit != itTransitEnd)
        {
            spPrepareStatement->BindInt64(1, *itTransit);

            spPrepareStatement->ExecuteUpdate();
            spPrepareStatement->Reset();

            ++itTransit;
        }

		strSQLDelete.Format(_T(" DELETE FROM %s WHERE Transit_id = ? "), TABLE_TRANSIT);
		spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		itTransit    = setDeleteTransitID.begin();
		itTransitEnd = setDeleteTransitID.end();

		while (itTransit != itTransitEnd)
		{
			spPrepareStatement->BindInt64(1, *itTransit);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			++itTransit;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoningDivisionDlg::UpsertZoneData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Zone Where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	CString strOriginID(_T("")); 
	while (iter != end)
	{
		spPrepareStatement->BindInt64(1, iter->first);

		spPrepareStatement->ExecuteUpdate();
		spPrepareStatement->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert     (_T(" Insert Into Zone "));

	strSQLInsert     +=  _T(" ( zone_id");

	for (int i=0; i<m_oZoneColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oZoneColumns.GetColumn(i)->Name();
	}

	strSQLInsert     += _T(" ) Values ( ?");

	for (int i=0; i<m_oZoneColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert    += _T(" )");

	
	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_ZONE), std::ios::binary );    

	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert Division Zone result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );

			if (ifs.eof())
			{
				break;
			}

			spPrepareStatement->BindInt64(1, nxOrigin);

			for(int i= 0; i<  m_oZoneColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );

				spPrepareStatement->BindDouble(i+2, dRecord);  
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division Zone result size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::UpsertSedData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Sed_Vector Where zone_id = ? "));

	KPreparedStatementPtr spPrepareStatement2 = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	CString strOriginID(_T("")); 
	while (iter != end)
	{
		spPrepareStatement2->BindInt64(1, iter->first);

		spPrepareStatement2->ExecuteUpdate();
		spPrepareStatement2->Reset();

		++iter;
	}

	strSQLDelete.Format(_T(" Delete From Sed_Pop Where zone_id = ? "));

	spPrepareStatement2 = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	iter = m_mapZoneDivisionInfo.begin();
	end  = m_mapZoneDivisionInfo.end();

	while (iter != end)
	{
		spPrepareStatement2->BindInt64(1, iter->first);

		spPrepareStatement2->ExecuteUpdate();
		spPrepareStatement2->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert     (_T(" Insert Into Sed_Vector "));

	strSQLInsert     +=  _T(" ( zone_id");

	for (int i=0; i<m_oSedVectorColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oSedVectorColumns.GetColumn(i)->Name();
	}

	strSQLInsert     += _T(" ) Values ( ?");

	for (int i=0; i<m_oSedVectorColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert    += _T(" )");


	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_SED), std::ios::binary );    

	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert Division Sed result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );
			
			if (ifs.eof())
			{
				break;
			}

			spPrepareStatement->BindInt64(1, nxOrigin);

			for(int i= 0; i<  m_oSedVectorColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );

				spPrepareStatement->BindDouble(i+2, dRecord);  
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division Sed result size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::UpsertPurposeODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From purpose_od Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	CString strOriginID(_T("")); 
	while (iter != end)
	{
		spPepareStatement->BindInt64(1, iter->first);
		spPepareStatement->BindInt64(2, iter->first);

		spPepareStatement->ExecuteUpdate();
		spPepareStatement->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert     (_T(" Insert Into purpose_od "));

	strSQLInsert     +=  _T(" ( ozone_id, dzone_id");

	for (int i=0; i<m_oPurposeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oPurposeODColumns.GetColumn(i)->Name();
	}

	strSQLInsert     += _T(" ) Values ( ?, ? ");

	for (int i=0; i<m_oPurposeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert    += _T(" )");

	spPepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_PURPOSEOD), std::ios::binary );    
	
	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert Division purpose_od result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read( reinterpret_cast<char*>( &nxDestination),        sizeof(Integer)  );

			spPepareStatement->BindInt64(1, nxOrigin);
			spPepareStatement->BindInt64(2, nxDestination);

			for(int i= 0; i<  m_oPurposeODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );

				spPepareStatement->BindDouble(i+3, dRecord);  
			}

			spPepareStatement->ExecuteUpdate();
			spPepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division purpose_od result size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::UpsertModeODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From mode_od Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	CString strOriginID(_T("")); 
	while (iter != end)
	{
		spPrepareStatement->BindInt64(1, iter->first);
		spPrepareStatement->BindInt64(2, iter->first);

		spPrepareStatement->ExecuteUpdate();
		spPrepareStatement->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert     (_T(" Insert Into mode_od "));

	strSQLInsert     +=  _T(" ( ozone_id, dzone_id");

	for (int i=0; i<m_oModeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oModeODColumns.GetColumn(i)->Name();
	}

	strSQLInsert     += _T(" ) Values ( ?, ? ");

	for (int i=0; i<m_oModeODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert    += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_MODEOD), std::ios::binary );    

	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert Division mode_od result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read( reinterpret_cast<char*>( &nxDestination),        sizeof(Integer)  );

			spPrepareStatement->BindInt64(1, nxOrigin);
			spPrepareStatement->BindInt64(2, nxDestination);

			for(int i= 0; i<  m_oModeODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );

				spPrepareStatement->BindDouble(i+3, dRecord);  
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division mode_od result size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::UpsertParameterODData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	strSQLDelete.Format(_T(" Delete From Parameter_OD Where ozone_id = ? or dzone_id = ? "));

	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
	std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

	CString strOriginID(_T("")); 
	while (iter != end)
	{
		spPrepareStatement->BindInt64(1, iter->first);
		spPrepareStatement->BindInt64(2, iter->first);

		spPrepareStatement->ExecuteUpdate();
		spPrepareStatement->Reset();

		++iter;
	}

	// insert
	CString  strSQLInsert     (_T(" Insert Into Parameter_OD "));

	strSQLInsert     +=  _T(" ( ozone_id, dzone_id");

	for (int i=0; i<m_oParameterODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ");
		strSQLInsert += m_oParameterODColumns.GetColumn(i)->Name();
	}

	strSQLInsert     += _T(" ) Values ( ?, ? ");

	for (int i=0; i<m_oParameterODColumns.ColumnCount(); i++)
	{
		strSQLInsert += _T(", ?");
	}

	strSQLInsert    += _T(" )");

	spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_ODPARAMETER), std::ios::binary );    

	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert Division parameter_od result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nxOrigin;
			ifs.read( reinterpret_cast<char*>( &nxOrigin),             sizeof(Integer)  );

			if (ifs.eof())
			{
				break;
			}

			Integer nxDestination;
			ifs.read( reinterpret_cast<char*>( &nxDestination),        sizeof(Integer)  );

			spPrepareStatement->BindInt64(1, nxOrigin);
			spPrepareStatement->BindInt64(2, nxDestination);

			for(int i= 0; i<  m_oParameterODColumns.ColumnCount(); i++)
			{
				double dRecord = 0.0;
				ifs.read( reinterpret_cast<char*>( &dRecord),    sizeof(double)  );

				spPrepareStatement->BindDouble(i+3, dRecord);  
			}

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert Division parameter_od result size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::DeleteAccessData(KDBaseConPtr a_spDBaseConnection)
{
	// delete 
	CString  strSQLDelete(_T(""));

	if (true) {
		strSQLDelete.Format(_T(" Delete From access_stop_node Where zone_id = ? "));

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

		CString strOriginID(_T("")); 
		for (; iter != end; ++iter)
		{
			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
	}

	if (true) {
		strSQLDelete.Format(_T(" Delete From access_terminal_node Where zone_id = ? "));

		KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLDelete);

		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

		CString strOriginID(_T("")); 
		for (; iter != end; ++iter)
		{
			spPrepareStatement->BindInt64(1, iter->first);

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
	}
}


void KZoningDivisionDlg::NotifyRelationTable()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_NODE);
		pTable->Notify();

		NotifyZoneRelation();

		NotifyLinkRelation();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningDivisionDlg::NotifyZoneRelation()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_ZONE);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_SED_VECTOR);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_SED_POP);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_PURPOSE_OD);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_PARAMETER_OD);
		pTable->Notify();

		pTable = pIOTables->FindTable(TABLE_MODE_OD);
		pTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KZoningDivisionDlg::NotifyLinkRelation()
{
	try
	{
		//KIOTables* pIOTables = m_pTarget->Tables();
		KIOTables* pIOTables = m_pRunningTarget->Tables();
		KIOTable*  pTable    = pIOTables->FindTable(TABLE_LINK);
		pTable->Notify();

		if(KmzSystem::IsPassengerSystem())
		{
			pTable = pIOTables->FindTable(TABLE_TRANSIT);
			if (nullptr != pTable) {
				pTable->Notify();
			}

			pTable = pIOTables->FindTable(TABLE_TRANSITNODES);
			if (nullptr != pTable) {
				pTable->Notify();
			}

			pTable = pIOTables->FindTable(TABLE_TURN);
			if (nullptr != pTable) {
				pTable->Notify();
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


bool KZoningDivisionDlg::CreateArgument()
{
	TxLogDebugStartMsg();

	ImChampDir::SetCurrentDirectoryToApp();

	if(! ClearFiles())
	{
		return false;
	}
	//TxLogDebug("END CLEAR FILE ------ ");
	UINT nOpenMode = CFile::modeCreate | CFile::modeWrite | CFile::typeText;
	CStdioFileEx of(T_ARG_ZONING, nOpenMode);

	bool bResult = false;

	// .arg 작성
	bResult = ArgumentWriteBase(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteBase) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	bResult = ArgumentWriteDivision(of);
	if(! bResult)
	{
		of.Close();
		AfxMessageBox(_T("Argument 생성(ArgumentWriteDivision) 중 오류가 발생하였습니다. !!!"));
		return false;
	}


	of.Close();


	// ZONE.dat 생성
	m_oZoneColumns.Clear();
	GetColumnInfo(TABLE_ZONE, m_oZoneColumns);
	
	//bResult = KBulkDBase2File::ExportDB2ZoneFile(m_pTarget->GetDBaseConnection(), m_oZoneColumns, ConvertSavePath(T_DAT_IN_ZONE));
	bResult = KBulkDBase2File::ExportDB2ZoneFile(m_pRunningTarget->GetDBaseConnection(), m_oZoneColumns, ConvertSavePath(T_DAT_IN_ZONE));
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ZoneFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	
	// SED.dat 생성
	m_oSedVectorColumns.Clear();
	GetColumnInfo(TABLE_SED_VECTOR, m_oSedVectorColumns);

	//bResult = KBulkDBase2File::ExportDB2SedVecFile(m_pTarget->GetDBaseConnection(), m_oSedVectorColumns, ConvertSavePath(T_DAT_IN_SED));
	bResult = KBulkDBase2File::ExportDB2SedVecFile(m_pRunningTarget->GetDBaseConnection(), m_oSedVectorColumns, ConvertSavePath(T_DAT_IN_SED));
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2SedVecFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// MODEOD.dat 생성
	m_oModeODColumns.Clear();
	GetColumnInfo(TABLE_MODE_OD, m_oModeODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_MODE_OD, m_oModeODColumns, ConvertSavePath(T_DAT_IN_MODEOD));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_MODE_OD, m_oModeODColumns, ConvertSavePath(T_DAT_IN_MODEOD));
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// PURPOSEOD.dat 생성
	m_oPurposeODColumns.Clear();
	GetColumnInfo(TABLE_PURPOSE_OD, m_oPurposeODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, m_oPurposeODColumns, ConvertSavePath(T_DAT_IN_PURPOSEOD));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_PURPOSE_OD, m_oPurposeODColumns, ConvertSavePath(T_DAT_IN_PURPOSEOD));
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	// ODParameter.dat 생성 
	m_oParameterODColumns.Clear();
	GetColumnInfo(TABLE_PARAMETER_OD, m_oParameterODColumns);

	//bResult = KBulkDBase2File::ExportDB2ODFile(m_pTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, m_oParameterODColumns, ConvertSavePath(T_DAT_IN_ODPARAMETER));
	bResult = KBulkDBase2File::ExportDB2ODFile(m_pRunningTarget->GetDBaseConnection(), TABLE_PARAMETER_OD, m_oParameterODColumns, ConvertSavePath(T_DAT_IN_ODPARAMETER));
	if(! bResult)
	{
		AfxMessageBox(_T("Dat 생성(ExportDB2ODFile) 중 오류가 발생하였습니다. !!!"));
		return false;
	}

	/**************************************************************************/
	/* 2019.01.16 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
	if (m_bAutoGenZoneConn)
	{
		// node.dat 생성
		bResult = KBulkDBase2File::ExportDB2NodeFile(m_pRunningTarget->GetDBaseConnection(), ConvertSavePath(T_DAT_NODE));
		if (!bResult)
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2NodeFile) 중 오류가 발생하였습니다. !!!"));
			return false;
		}

		// link.dat 생성
		KIOColumns oEmptyLinkColumns;
		bResult = KBulkDBase2File::ExportDB2LinkFileD(m_pRunningTarget->GetDBaseConnection(), oEmptyLinkColumns, ConvertSavePath(T_DAT_LINK));
		if (!bResult)
		{
			AfxMessageBox(_T("Dat 생성(ExportDB2LinkFileD) 중 오류가 발생하였습니다. !!!"));
			return false;
		}
	}
	/**************************************************************************/

	/**************************************************************************/
	/* 2019.03.05 수정 : 존 연결로 자동생성시 DefineNetworkType.arg 파일을 각 시나리오에 출력 */
	CString strCodePath(_T(""));
	strCodePath.Format(_T("%s\\%s"), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME);
	try
	{
		KWriteCodeAgrument::WirteMatchingCodeArument(m_pRunningTarget, strCodePath);
	}
	catch (...) {
		AfxMessageBox(_T("Dat 생성(DefineNetworkType.arg) 중 오류가 발생하였습니다. !!!"));
		return false;
	}
	/**************************************************************************/

	TxLogDebugEndMsg();
	return true;
}


bool KZoningDivisionDlg::ClearFiles()
{
	TxLogDebugStartMsg();

	int nResult = -1;

	try
	{
		nResult = remove((CStringA)ConvertSavePath(T_ARG_ZONING));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_ZONE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_SED));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_MODEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_IN_ODPARAMETER));

		/**************************************************************************/
		/* 2019.01.16 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		nResult = remove((CStringA)ConvertSavePath(T_DAT_NODE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_LINK));
		/**************************************************************************/

		// 결과 파일 삭제 
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ZONE));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_SED));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_MODEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_PURPOSEOD));
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ODPARAMETER));

		/**************************************************************************/
		/* 2019.01.16 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		nResult = remove((CStringA)ConvertSavePath(T_DAT_OUT_ZONECONNECTLINK));
		/**************************************************************************/

	
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
	TxLogDebugEndMsg();
}


bool KZoningDivisionDlg::ArgumentWriteBase(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{	
		//In Out FolderName
		CString strPath = ConvertSavePath(_T(""));
		strOut.Format(_T("%s\r\n"), strPath);
		of.WriteString(strOut);

		// 존 수
		strOut.Format(_T("Nzone\t%d\r\n"), InitZoneCount());
		of.WriteString(strOut);

		// 존 파일 형태수
		strOut.Format(_T("Nzonefile\t%d\r\n"), FILE_TYPE_COUNT_ZONE);
		of.WriteString(strOut);

		// 존 파일 1
		strOut.Format(_T("Zonefile1\t%s\t%d\r\n"), _T("ZONE"),        GetDoubleTypeColumnCount(TABLE_ZONE));
		of.WriteString(strOut);
		 
		// 존 파일 2
		strOut.Format(_T("Zonefile2\t%s\t%d\r\n"), _T("SED"),         GetDoubleTypeColumnCount(TABLE_SED_VECTOR));
		of.WriteString(strOut);

		// OD 파일 형태수
		strOut.Format(_T("Nodfile\t%d\r\n"), FILE_TYPE_COUNT_OD);
		of.WriteString(strOut);

		// od 파일 1
		strOut.Format(_T("ODfile1\t%s\t%d\r\n"),   _T("MODEOD"),      GetDoubleTypeColumnCount(TABLE_MODE_OD));
		of.WriteString(strOut);

		// od 파일 2
		strOut.Format(_T("ODfile2\t%s\t%d\r\n"),   _T("PURPOSEOD"),   GetDoubleTypeColumnCount(TABLE_PURPOSE_OD));
		of.WriteString(strOut);

		// od 파일 3
		strOut.Format(_T("ODfile3\t%s\t%d\r\n"),   _T("ODParameter"), GetDoubleTypeColumnCount(TABLE_PARAMETER_OD));
		of.WriteString(strOut);

		// classification 
		strOut.Format(_T("Classification\t%d\r\n"), CLASSIFICATION_CODE_DIVISION);
		of.WriteString(strOut);

		/**************************************************************************/
		/* 2019.01.17 수정 : 존 연결로 자동생성 기능 추가에 따른 변경                    */
		// 존 연결로 자동 생성 여부
		strOut.Format(_T("AutoGenZoneConn\t%d\r\n"), (m_bAutoGenZoneConn == TRUE ? 1 : 0));
		of.WriteString(strOut);
		/**************************************************************************/
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}


bool KZoningDivisionDlg::ArgumentWriteDivision(CStdioFileEx& of)
{
	CString strOut = _T("");

	try
	{	
		// Division 횟수
		strOut.Format(_T("Division\t%d\r\n"), m_mapZoneDivisionInfo.size());
		of.WriteString(strOut);

		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator iter = m_mapZoneDivisionInfo.begin();
		std::map<Integer, KZoningOriginZoneInfoPtr>::iterator end  = m_mapZoneDivisionInfo.end();

		CString strIndex;
		CString strOriginalZoneInfo;
		CString strDivisionZoneInfo;
		CString strDivisionRate;
		CString strTempZone;
		CString strTempRate;

		int     nIndex = 1;
		while (iter != end)
		{
			//index
			strOut.Format(_T("Index\t%d\r\n"), nIndex++);
			of.WriteString(strOut);

			//original zone
			strOut.Format(_T("OriginZone\t%I64d\r\n"), iter->first);
			of.WriteString(strOut);

			KZoningOriginZoneInfoPtr           spZoningOriginZoneInfo = iter->second;
			std::vector<KZoneDivisionInfoPtr>& vecZoneDivisionInfo    = spZoningOriginZoneInfo->TvecZoneDivisionInfo;
			size_t nxCount = vecZoneDivisionInfo.size();

			strDivisionZoneInfo = _T("");
			strDivisionRate     = _T("");

			// division zone 수
			strOut.Format(_T("nDivisionZone\t%d\r\n"), nxCount);
			of.WriteString(strOut);

			/**************************************************************************/
			/* 2019.03.08 수정 : 존 세분화/집합화 argument 파일 변경에 따른 수정 */
			std::vector<CString> arrDivisionInfo;
			for (size_t i=0; i<nxCount; i++)
			{
				KZoneDivisionInfoPtr spZoneDivisionInfo = vecZoneDivisionInfo[i];
				strTempZone.Format(_T("%I64d"), spZoneDivisionInfo->TZoneID);
				strTempRate.Format(_T("%f"), (spZoneDivisionInfo->TRate/100.0));

				if (i != nxCount-1)
				{
					strDivisionZoneInfo = strDivisionZoneInfo + strTempZone + _T("\t");
					strDivisionRate     = strDivisionRate     + strTempRate + _T("\t");
				}
				else
				{
					strDivisionZoneInfo = strDivisionZoneInfo + strTempZone;
					strDivisionRate     = strDivisionRate     + strTempRate;
				}

				CString str;
				str.Format(_T("%I64d\t%f\t%f\t%f\r\n"), spZoneDivisionInfo->TZoneID,
					(spZoneDivisionInfo->TRate / 100.0),
					spZoneDivisionInfo->TNodeGeometry->X,
					spZoneDivisionInfo->TNodeGeometry->Y);
				arrDivisionInfo.push_back(str);
			}

			//// division zone 번호
			//strOut.Format(_T("DivisionZoneNewID\t%s\r\n"), strDivisionZoneInfo);
			//of.WriteString(strOut);

			//// division rate
			//strOut.Format(_T("DivisionRate\t%s\r\n"), strDivisionRate);
			//of.WriteString(strOut);

			of.WriteString(_T("DivisionInfo\r\n"));
			for (CString& str : arrDivisionInfo)
			{
				of.WriteString(str);
			}
			/**************************************************************************/

			++iter;
		}

	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}



void KZoningDivisionDlg::ReceiveStatusMessageFromDll(void* pParam1, int nStatus, char* strMsg)
{
	AddStatusMessage(CString(strMsg));
}


void KZoningDivisionDlg::AddStatusMessage(CString a_strMsg)
{
	QBicSimpleProgressPtr spProgressWindow = QBicSimpleProgressThread::SimpleProgress();
	if(spProgressWindow)
	{
		spProgressWindow->SetStatus(a_strMsg);
	}
}


int KZoningDivisionDlg::InitZoneCount()
{
	//KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KDBaseConPtr spDBaseConnection = m_pRunningTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT COUNT(*) AS Count FROM %s "), TABLE_ZONE);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return 0;
	}

	int nZoneCount = 0;
	while( spResult->Next() )
	{
		nZoneCount	= spResult->GetValueInt(0);
	}

	return nZoneCount;
}


int KZoningDivisionDlg::GetDoubleTypeColumnCount(CString a_strTableName)
{
	//KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTables*   pIOTables = m_pRunningTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(a_strTableName);
	if(NULL == pTable)
	{
		return 0;
	}

	int nColumnCount = 0;
	const KIOColumns* pColumns = pTable->Columns();
	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if( pColumn->DataType() == KEMIODataTypeDouble )
		{
			nColumnCount++;
		}
	}

	return nColumnCount;
}


void KZoningDivisionDlg::GetColumnInfo(CString a_strTableName, KIOColumns &a_oColumns)
{
	//KIOTables*   pIOTables = m_pTarget->Tables();
	KIOTables*   pIOTables = m_pRunningTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(a_strTableName);
	if(NULL == pTable)
	{
		return;
	}

	int nColumnCount = 0;
	const KIOColumns* pColumns = pTable->Columns();
	for(int i= 0; i< pColumns->ColumnCount(); i++)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		if( pColumn->DataType() == KEMIODataTypeDouble )
		{
			a_oColumns.AddNewColumn(pColumn);
		}
	}
}


BOOL KZoningDivisionDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
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

	return KDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void KZoningDivisionDlg::OnBnClickedSelectOriginZone()
{
	SelectNodeList();
}


void KZoningDivisionDlg::SelectNodeList()
{
	try
	{
		CWaitCursor cw;

		// 항상 초기화해준다. vecIDCaption, vecID를 멤버로 갖고 있으면 좋음...
		m_spImTasMultiIDFind = KImChampMultiIDFindPtr(new KImChampMultiIDFind(this));    
		m_spImTasMultiIDFind->SetModelessReceiver(this);

		m_spImTasMultiIDFind->SetIDCaption(m_vecZoneNodeIDCaption);
		m_spImTasMultiIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneNodeData, ImChampMapDefineSymbol::ZoneSymbol);

		m_spImTasMultiIDFind->SetSelectedID(m_strSearchedIDs);
		m_spImTasMultiIDFind->Create(KImChampMultiIDFind::IDD, this);
		m_spImTasMultiIDFind->ShowWindow(SW_SHOW);
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


BOOL KZoningDivisionDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT4)->m_hWnd)
		{
			SelectNodeList();
			return TRUE;
		}
	}

	return KDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH KZoningDivisionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_EDIT4 )
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);

		hbr = (HBRUSH)m_backBrush;
        //return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
	return hbr;
}

void KZoningDivisionDlg::ModelessCancleFeedback(UINT nDialogID)
{

}

CString KZoningDivisionDlg::ConvertSavePath( CString a_strFileName )
{
	CString strSavePath(_T(""));
	//strSavePath.Format(_T("%s%s%s\\%s"), m_pTarget->GetLocation(), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME,a_strFileName);
	strSavePath.Format(_T("%s%s%s\\%s"), m_pRunningTarget->GetLocation(), DLL_READ_WRITE_PATH, ZONING_FOLDER_NAME, a_strFileName);
	return strSavePath;
}


void KZoningDivisionDlg::OnBnClickedBtnMultiScenario()
{
	UpdateData(TRUE);

	CString strError;
	if (!InputCheck(strError)) {
		AfxMessageBox(strError);
		return;
	}

	// 여러 시나리오 선택을 위해 zone의 set을 설정(모두 일치한 ID가 있을 경우에만 선택 가능)
	std::set<__int64> setZoneId;
	std::transform(m_mapZoneDivisionInfo.begin(), m_mapZoneDivisionInfo.end(),
		std::inserter(setZoneId, setZoneId.end()),
		[](auto pair) { return pair.first; });

	KZoningMultiScenarioSelectDlg dlg(KEMZoningOperationDivision);
	dlg.SetTarget(m_pTarget);
	dlg.SetSelectedZoneNodeSet(setZoneId);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	bool bSuccessAll = true;

	std::vector<KTarget*> arrTarget = dlg.GetSelectedTarget();
	for (KTarget* pTarget : arrTarget)
	{
		m_pRunningTarget = pTarget;

		m_nResultCode = DLL_UNEXPECTED_ERROR;

		// 모형을 돌리기 전에 반드시...
		GenerateNewZoneID();


		GenerateDivisionInfo();

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(DivisionZoneThreadCaller, this, false, nLang);

		// GIS 입력은 Thread로 돌리면 정상적 수행이 안됨.
		// GIS 입력 부분은 QBicDonkeyProgress....
		// 

		if (DLL_PLANNED_TERMINATION == m_nResultCode /*OK*/)
		{

		}
		else if (DLL_USER_STOPPING == m_nResultCode)
		{
			AfxMessageBox(_T("존 세분화/집합화가 중단되었습니다."));
			return;
		}
		else if (DLL_LOAD_ERROR == m_nResultCode)
		{
			AfxMessageBox(_T("Not Found Dll"));
			return;
		}
		else if (DLL_CREATE_ARGFILE_ERROR == m_nResultCode)
		{
			AfxMessageBox(_T("Argument 파일 생성 중 오류가 발생하였습니다."));
			return;
		}
		else
		{
			AfxMessageBox(_T("존 세분화 수행 중 오류가 발생하였습니다."));
			return;
		}

		CWaitCursor wc;

		bool bTemp = ReadOutPutFile();
		if (!bTemp)
		{
			AfxMessageBox(_T("세분화한 존 정보 입력에 실패하였습니다."));
			TxLogDebug(_T("Output Division 정보 입력 실패..."));
			return;
		}

		if (!UpdateDivisionSystemInfo())
		{
			AfxMessageBox(_T("존 관련 정보 입력에 실패하였습니다."));
			return;
		}

		if (m_bRunDivisionSucceess)
		{
			//ClearDrawInfo(true);
			if (m_pRunningTarget == m_pTarget)
			{
				ClearDrawInfo(true);
			}
		}
		else {
			bSuccessAll = false;
			//m_pMapView->MapRefresh();
			KMapView* pMapView = ImChampFrameWindow::GetCreatedMapView(m_pRunningTarget);
			if (nullptr != pMapView)
			{
				pMapView->MapRefresh();
			}
		}

		NotifyRelationTable();
	}

	if (bSuccessAll)
	{
		// 항목 리셋
		m_wndOriginZoneReportCtrl.ResetContent();
		m_wndDivisionZoneReportCtrl.ResetContent();
		m_mapZoneDivisionInfo.clear();

		m_strSearchedIDs = _T("");
		SetDlgItemText(IDC_EDIT4, m_strSearchedIDs);
	}

	GetDlgItem(IDC_BTN_MULTI_SCENARIO)->EnableWindow(FALSE);
}



/**************************************************************************/
/* 2019.01.15 수정 : 여러 시나리오 처리기능 추가에 따른 변경                     */
void KZoningDivisionDlg::InsertZoneConnectLink(KDBaseConPtr a_spDBaseConnection, std::vector<TLinkFTNodeID>& arrZoneConnectLink)
{
	// insert	
	CString  strSQLInsert = _T(
		" Insert Into Link (link_id, fnode_id, tnode_id, link_type, length "
		" , lane, capa, speed, function_id, alpha, beta, a, b) "
		" VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
	);
	
	KPreparedStatementPtr spPrepareStatement = a_spDBaseConnection->PrepareStatement(strSQLInsert);

	std::ifstream ifs( ConvertSavePath(T_DAT_OUT_ZONECONNECTLINK), std::ios::binary );
	
	if ( ifs ) 
	{      
		TxLogDebug(_T("--- start insert zone connect link result"));
		int nCount = 0;

		while(! ifs.eof())
		{
			Integer nLinkId, nFNodeId, nTNodeId;
			int nLinkType, nLane, nFunctionId;
			double dLength, dCapa, dSpeed, dAlpha, dBeta, dA, dB;

			ifs.read(reinterpret_cast<char*>(&nLinkId), sizeof(Integer));
			ifs.read(reinterpret_cast<char*>(&nFNodeId), sizeof(Integer));
			ifs.read(reinterpret_cast<char*>(&nTNodeId), sizeof(Integer));

			ifs.read(reinterpret_cast<char*>(&nLinkType), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&dLength), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&nLane), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&dCapa), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dSpeed), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&nFunctionId), sizeof(int));

			ifs.read(reinterpret_cast<char*>(&dAlpha), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dBeta), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dA), sizeof(double));
			ifs.read(reinterpret_cast<char*>(&dB), sizeof(double));

			int idx = 1;
			spPrepareStatement->BindInt64(idx++, nLinkId);		// 1
			spPrepareStatement->BindInt64(idx++, nFNodeId);		// 2
			spPrepareStatement->BindInt64(idx++, nTNodeId);		// 3
			spPrepareStatement->BindInt(idx++, nLinkType);		// 4
			spPrepareStatement->BindDouble(idx++, dLength);		// 5
			spPrepareStatement->BindInt(idx++, nLane);			// 6
			spPrepareStatement->BindDouble(idx++, dCapa);		// 7
			spPrepareStatement->BindDouble(idx++, dSpeed);		// 8
			spPrepareStatement->BindInt(idx++, nFunctionId);	// 9
			spPrepareStatement->BindDouble(idx++, dAlpha);		// 10
			spPrepareStatement->BindDouble(idx++, dBeta);		// 11
			spPrepareStatement->BindDouble(idx++, dA);			// 12
			spPrepareStatement->BindDouble(idx++, dB);			// 13

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();

			TLinkFTNodeID zoneConnLink = { nLinkId, nFNodeId, nTNodeId };
			arrZoneConnectLink.push_back(zoneConnLink);
	
			nCount++;
		}

		CString strResult = _T("");
		strResult.Format(_T("--- end insert zone connect link size : %d"), nCount);
		TxLogDebug( (LPCTSTR)strResult );
	}
	else
	{
		ifs.close();
		TxLogDebugException();
	}

	ifs.close();
}


void KZoningDivisionDlg::SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KMapView* pMapView)
{
	try
	{
		TxFeatureLayerPtr  spNodeLayer; {
			ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Node());
			spNodeLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
		}

		TxFeatureLayerPtr  spLinkLayer; {
			ITxFeatureLayerPtr spLayer = pMapView->MapGetFeatureLayer(KLayerID::Link());
			spLinkLayer = std::dynamic_pointer_cast<TxFeatureLayer>(spLayer);
		}

		std::map<Integer, ITxGeometryPtr> mapNodeGeom;
		spNodeLayer->GetGeometry(mapNodeGeom);

		for (TLinkFTNodeID& linkInfo : arrZoneConnectLink)
		{
			std::vector<TxPoint> arrPoints;
			std::map<Integer, ITxGeometryPtr>::iterator itFNode = mapNodeGeom.find(linkInfo.TFromNodeID);
			if (mapNodeGeom.end() == itFNode || itFNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("From Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			std::map<Integer, ITxGeometryPtr>::iterator itTNode = mapNodeGeom.find(linkInfo.TToNodeID);
			if (mapNodeGeom.end() == itTNode || itTNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("To Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			TxPointPtr spFNode = std::dynamic_pointer_cast<TxPoint>(itFNode->second);
			TxPointPtr spTNode = std::dynamic_pointer_cast<TxPoint>(itTNode->second);

			TxPolylinePtr spZoneConnectLink = std::make_shared<TxPolyline>(spFNode, spTNode);

			spLinkLayer->ProhibitGeoInsert(linkInfo.TLinkID, spZoneConnectLink);
		}
	}
	catch (...)
	{
		CString strError = _T("Zone 세분화 처리 중 Zone connect link 생성 처리 오류(using mapview)");
		ThrowException(strError);
	}

}


void KZoningDivisionDlg::SaveZoneConnectLinkGeometry(std::vector<TLinkFTNodeID>& arrZoneConnectLink, KDBaseConPtr a_spDBaseConnection)
{
	try
	{
		ITxMapServerPtr spMapServer = ITxMapServerPtr(new TxSQLMapServer(TxMapDataServerSqlite, a_spDBaseConnection->GetConnectionString().m_strUrl));
		if (spMapServer->Connect(a_spDBaseConnection) != 1)
			throw 1;

		TxFeatureLayerPtr  spNodeLayer = nullptr;
		TxFeatureLayerPtr  spLinkLayer = nullptr;

		// 서버의 레이어 정보 조회
		std::vector<TxLayerInfoPtr> vecLayerInfo = spMapServer->LayerInfo();
		for (size_t j = 0; j < vecLayerInfo.size(); j++) {
			TxLayerInfoPtr spLayerInfo = vecLayerInfo[j];

			if (_tcsicmp(TABLE_NODE, spLayerInfo->Name()) == 0) { // 노드 레이어 조회
				spNodeLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}

			if (_tcsicmp(TABLE_LINK, spLayerInfo->Name()) == 0) { // 링크 레이어 조회
				spLinkLayer = TxFeatureLayerPtr(new TxFeatureLayer(spMapServer, spLayerInfo));
			}
		}

		if (spNodeLayer == nullptr || spLinkLayer == nullptr)
		{
			throw _T("some layer(s) is(are) not exists");
		}

		std::map<Integer, ITxGeometryPtr> mapNodeGeom;
		spNodeLayer->GetGeometry(mapNodeGeom);

		for (TLinkFTNodeID& linkInfo : arrZoneConnectLink)
		{
			std::vector<TxPoint> arrPoints;
			std::map<Integer, ITxGeometryPtr>::iterator itFNode = mapNodeGeom.find(linkInfo.TFromNodeID);
			if (mapNodeGeom.end() == itFNode || itFNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("From Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			std::map<Integer, ITxGeometryPtr>::iterator itTNode = mapNodeGeom.find(linkInfo.TToNodeID);
			if (mapNodeGeom.end() == itTNode || itTNode->second == nullptr)
			{
				CString strError;
				strError.Format(_T("To Node not exists : LINK_ID=%I64d, FNODE=%I64d, TNODE=%i64d"),
					linkInfo.TLinkID, linkInfo.TFromNodeID, linkInfo.TToNodeID);
				TxLogDebug(strError);
				continue;
			}

			TxPointPtr spFNode = std::dynamic_pointer_cast<TxPoint>(itFNode->second);
			TxPointPtr spTNode = std::dynamic_pointer_cast<TxPoint>(itTNode->second);

			TxPolylinePtr spZoneConnectLink = std::make_shared<TxPolyline>(spFNode, spTNode);

			spLinkLayer->ProhibitGeoInsert(linkInfo.TLinkID, spZoneConnectLink);
		}
	}
	catch (...)
	{
		CString strError = _T("Zone 세분화 처리 중 Zone connect link 생성 처리 오류(using KDBaseConPtr)");
		ThrowException(strError);
	}
}
/**************************************************************************/


