// IntersectionTopologyAnalysisDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "IntersectionTopologyAnalysisDlg.h"
#include "afxdialogex.h"

#include "IntersectionNodeType.h"
#include "IntersectionThreeBasicPage.h"
#include "IntersectionBasicPage.h"
#include "IntersectionTopologyAnalysis.h"
#include "BulkDBaseIntersection.h"
#include "ImChampMultiZoneFind.h"
#include "Target.h"
#include "MapView.h"
#include "TCoordinate.h"

#include "BulkDBaseTurn.h"

// KIntersectionTopologyAnalysisDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KIntersectionTopologyAnalysisDlg, KResizeDialogEx)

KIntersectionTopologyAnalysisDlg::KIntersectionTopologyAnalysisDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KIntersectionTopologyAnalysisDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{

}

KIntersectionTopologyAnalysisDlg::~KIntersectionTopologyAnalysisDlg()
{
	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
		pIOTable->Unregister(this);

		pIOTable             = pIOTables->FindTable(TABLE_LINK);
		pIOTable->Unregister(this);

		ClearAllMap();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

void KIntersectionTopologyAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,                m_ctrlReport);
}


BEGIN_MESSAGE_MAP(KIntersectionTopologyAnalysisDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO3, &KIntersectionTopologyAnalysisDlg::OnBnClickedRadioSelectIntersections)
	ON_BN_CLICKED   (IDCANCEL,                          &KIntersectionTopologyAnalysisDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2,                          &KIntersectionTopologyAnalysisDlg::OnBnClickedButtonSearchAreaCode)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT,  &KIntersectionTopologyAnalysisDlg::OnReportItemClick)
	ON_BN_CLICKED(IDC_BUTTON1, &KIntersectionTopologyAnalysisDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON3, &KIntersectionTopologyAnalysisDlg::OnBnClickedButtonDrawPolygon)
	ON_BN_CLICKED(IDC_BUTTON4, &KIntersectionTopologyAnalysisDlg::OnBnClickedButtonEditPolygon)
END_MESSAGE_MAP()


// KIntersectionTopologyAnalysisDlg 메시지 처리기입니다.

void KIntersectionTopologyAnalysisDlg::InitializeSet()
{
	try
	{
		CWaitCursor cw;

		KBulkDBaseIntersection::GetAllNodeInfo             (m_pTarget, m_mapNodeName, m_mapNodeGroupZone);
		KBulkDBaseIntersection::FindArrivedLink            (m_pTarget, m_mapNodeArrivedLink);
		KBulkDBaseIntersection::FindDirectionCrossRoad     (m_pTarget, m_vecIntersectionNodeType);
		KBulkDBaseIntersection::GetAllIntersectionBoundInfo(m_pTarget, m_mapAllIntersectionBoundInfo);

		CString strWhereProhibitTurn(_T(" WHERE turn_type = '0' "));
		KBulkDBaseTurn::ExistTurnAtNode                    (m_pTarget, m_setProhibitTurnNodeID, strWhereProhibitTurn);

		m_ctrlReport.ResetContent();
		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::ActivatedFrameWndNotifyProcess()
{

}


void KIntersectionTopologyAnalysisDlg::DeActivatedFrameWndNotifyProcess()
{

}

 
void KIntersectionTopologyAnalysisDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_REPORT,				      SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
		SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

		SetResize(IDC_STATIC3,	    			  SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
		SetResize(IDC_BUTTON1,				      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,			              SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_LINK) == 0 || strSubjectName.CompareNoCase(TABLE_NODE) == 0)
		{
			InitializeSet();
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


void KIntersectionTopologyAnalysisDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
	try
	{
		std::set<Integer> setSelectedNodeID;

		setSelectedNodeID.insert(a_vecSelection.begin(), a_vecSelection.end());

		UpdateReport        (setSelectedNodeID);
		DrawIntersectionNode(setSelectedNodeID);

		m_vecSelAreaCoordinate = a_vecCoordinate;
		EditPolygon(m_vecSelAreaCoordinate);
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


BOOL KIntersectionTopologyAnalysisDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, FALSE, FALSE, FALSE);
	KReportCtrlSetting::AllowColumnsort(m_ctrlReport);
	InitReportHeader();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	UpdateRadioControls();

	ResizeComponent();

	try
	{
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_NODE);
		pIOTable->Register(this);

		pIOTable             = pIOTables->FindTable(TABLE_LINK);
		pIOTable->Register(this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KIntersectionTopologyAnalysisDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("ID"),     40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("노드 ID"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("Name"),   60));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("노드 명칭"));
        }

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(2, _T("Type"),   40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("교차로유형"));
        }

		m_ctrlReport.Populate();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnBnClickedRadioSelectIntersections(UINT nID)
{
	try
	{
		UpdateRadioControls();
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KIntersectionTopologyAnalysisDlg::UpdateRadioControls()
{
	try
	{
		int nID = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3);

		if (IDC_RADIO1 == nID)
		{
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);

			UpdateReport();

			ClearAllMap();
		}
		else if (IDC_RADIO2 == nID)
		{
			GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);

			ClearAllMap();
		}
		else if (IDC_RADIO3 == nID)
		{
			GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::UpdateReport()
{
	int nRecordCnt(0);

	try
	{
		m_ctrlReport.ResetContent();
		
		size_t nxCount = m_vecIntersectionNodeType.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIntersectionNodeType &oIntersectionNodeType = m_vecIntersectionNodeType[i];

#pragma region 교차로 중 조건에 부합되는 교차로만
			if (!FilterIntersectionNode(oIntersectionNodeType))
			{
				continue;
			}
#pragma endregion 교차로 중 조건에 부합되는 교차로만

			AddReportRecord(&oIntersectionNodeType);
		}
		m_ctrlReport.Populate();
		nRecordCnt = m_ctrlReport.GetRecords()->GetCount();
	}
	catch (...)
	{
		TxLogDebugException();	

		m_ctrlReport.ResetContent();
		nRecordCnt = 0;
	}

	CString strTotalCnt(_T(""));
	strTotalCnt.Format(_T("Total : %d"), nRecordCnt);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        strTotalCnt.Format(_T("개수 : %d"), nRecordCnt);
    }
	SetDlgItemText(IDC_STATIC3, strTotalCnt);
}


void KIntersectionTopologyAnalysisDlg::UpdateReport(std::set<Integer> &a_setPartSeletedID)
{
	int nRecordCnt(0);

	try
	{
		m_ctrlReport.ResetContent();

		size_t nxCount = m_vecIntersectionNodeType.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIntersectionNodeType &oIntersectionNodeType = m_vecIntersectionNodeType[i];

			if (a_setPartSeletedID.find(oIntersectionNodeType.TNodeID) == a_setPartSeletedID.end())
			{
				continue;
			}

#pragma region 교차로 중 조건에 부합되는 교차로만
			if (!FilterIntersectionNode(oIntersectionNodeType))
			{
				continue;
			}
#pragma endregion 교차로 중 조건에 부합되는 교차로만

			AddReportRecord(&oIntersectionNodeType);
		}

		m_ctrlReport.Populate();
		nRecordCnt = m_ctrlReport.GetRecords()->GetCount();
	}
	catch (...)
	{
		TxLogDebugException();	

		m_ctrlReport.ResetContent();
		nRecordCnt = 0;
	}

	CString strTotalCnt(_T(""));
	strTotalCnt.Format(_T("Total : %d"), nRecordCnt);
	if (KmzSystem::GetLanguage() == KEMKorea) {
        strTotalCnt.Format(_T("개수 : %d"), nRecordCnt);
    }
	SetDlgItemText(IDC_STATIC3, strTotalCnt);
}


bool KIntersectionTopologyAnalysisDlg::FilterIntersectionNode(KIntersectionNodeType &a_oIntersectionNodeType)
{
	try
	{
		AutoType iter = m_mapNodeArrivedLink.find(a_oIntersectionNodeType.TNodeID);
		if (iter == m_mapNodeArrivedLink.end())
			return false;

		std::vector<Integer> vecLinkID = iter->second;
		if ((int)vecLinkID.size() != a_oIntersectionNodeType.TDirectionCount)
		{
			return false;
		}

		if (m_setProhibitTurnNodeID.find(a_oIntersectionNodeType.TNodeID) != m_setProhibitTurnNodeID.end()) // Turn 금지 교차로 제외 - "2014-01 명지대 요청"
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


void KIntersectionTopologyAnalysisDlg::AddReportRecord(KIntersectionNodeType* a_pIntersectionNodeType)
{
	try
	{
		CXTPReportRecord*     pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());
		CXTPReportRecordItem* pItem   = nullptr;

		Integer &nxNodeID = a_pIntersectionNodeType->TNodeID;
		CString strNodeID(_T(""));
		strNodeID.Format(_T("%I64d"), nxNodeID);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strNodeID ));
		pItem->SetItemData((DWORD_PTR)a_pIntersectionNodeType);
		pItem->SetAlignment(DT_CENTER);

		CString  strNodeName(_T(""));
		AutoType iter = m_mapNodeName.find(nxNodeID);
		if (iter != m_mapNodeName.end())
		{
			strNodeName = iter->second;
		}
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strNodeName ));
		pItem->SetAlignment(DT_LEFT);

		double dDirCount = (double)a_pIntersectionNodeType->TDirectionCount;
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dDirCount, _T("%.0f")));
		pItem->SetAlignment(DT_CENTER);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::DrawIntersectionNode(std::set<Integer> &a_setPartSeletedID)
{
	try
	{
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		TPreferenceSymbol         &NodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		size_t nxCount = m_vecIntersectionNodeType.size();
		for (size_t i= 0; i< nxCount; i++)
		{
			KIntersectionNodeType &oIntersectionNodeType = m_vecIntersectionNodeType[i];

			if (a_setPartSeletedID.find(oIntersectionNodeType.TNodeID) == a_setPartSeletedID.end())
			{
				continue;
			}

#pragma region 교차로 중 조건에 부합되는 교차로만
			if (!FilterIntersectionNode(oIntersectionNodeType))
			{
				continue;
			}
#pragma endregion 교차로 중 조건에 부합되는 교차로만

			TDrawNodeInfo	oDrawNodeInfo;

			CString strNodeID(_T(""));
			strNodeID.Format(_T("%I64d"), oIntersectionNodeType.TNodeID);

			oDrawNodeInfo.TNodeID			= oIntersectionNodeType.TNodeID;
			oDrawNodeInfo.TSize				= NodeSymbol.dSize;
			oDrawNodeInfo.TSymbol	= NodeSymbol.nCharIndex;	
			oDrawNodeInfo.TColorRGB			= NodeSymbol.clrSymbol;
			oDrawNodeInfo.TLabel			= strNodeID;

			vecDrawNodeInfo.push_back(oDrawNodeInfo);
		}
 		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  true);

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


void KIntersectionTopologyAnalysisDlg::OnBnClickedButtonSearchAreaCode()
{
	try
	{
		KImChampMultiZoneFind oFindDlg(m_pTarget);
		oFindDlg.SetUseTransitInclude(false);

		if (oFindDlg.DoModal() == IDOK)
		{
			std::set<Integer> setAreaZoneID;
			oFindDlg.GetSelectedID(setAreaZoneID);

			std::set<Integer> setAreaNodeID;
			GetNodeIdGroupZone(setAreaZoneID, setAreaNodeID);

			UpdateReport(setAreaNodeID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::GetNodeIdGroupZone(const std::set<Integer> &a_setZoneID, std::set<Integer> &a_setNodeIdInZone)
{
	a_setNodeIdInZone.clear();

	try
	{
		AutoType iter  = a_setZoneID.begin();
		AutoType itEnd = a_setZoneID.end();

		while (iter != itEnd)
		{
			AutoType iterSetNode = m_mapNodeGroupZone.find(*iter);
			if (iterSetNode != m_mapNodeGroupZone.end())
			{
				std::set<Integer> &setNodeID = iterSetNode->second;

				a_setNodeIdInZone.insert(setNodeID.begin(), setNodeID.end());
			}

			++iter;
		}
	}
	catch (...)
	{
		a_setNodeIdInZone.clear();
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

		if (!pItemNotify->pRow || !pItemNotify->pColumn)
			return;

		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if( 0 == nIndex)
		{
			CXTPReportRecord*	   pRecord  = pItemNotify->pRow->GetRecord();
			CXTPReportRecordItem*  pItem    = pRecord->GetItem(nIndex);
			KIntersectionNodeType* a_pIntersectionNodeType = (KIntersectionNodeType*)pItem->GetItemData();

			ZoomToNodeFeature(a_pIntersectionNodeType->TNodeID);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::ZoomToNodeFeature( Integer &a_nxNodeID )
{
	try
	{
		m_pMapView->PanToFeature( KLayerID::Node(), a_nxNodeID );
	}
	catch (...)
	{
		TxLogDebugException();
	}
}


void KIntersectionTopologyAnalysisDlg::GetTempedIntersectionNodeInfo(std::vector<KIntersectionNodeType> &a_vecIntersectionNodeType)
{
	try
	{
		a_vecIntersectionNodeType.clear();

		CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
		CXTPReportRecord*     pRecord(nullptr);
		CXTPReportRecordItem* pItem  (nullptr);

		size_t nxCount      = pRecords->GetCount();
		for (size_t i= 0; i< nxCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem   = pRecord->GetItem(0);
			KIntersectionNodeType* a_pIntersectionNodeType = (KIntersectionNodeType*)pItem->GetItemData();

			a_vecIntersectionNodeType.push_back(*a_pIntersectionNodeType);
		}
	}
	catch (...)
	{
		a_vecIntersectionNodeType.clear();
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnBnClickedButtonRun()
{
	try
	{
		CWaitCursor cw;

		std::vector<KIntersectionNodeType>                  vecIntersectionNodeType;
		std::map<Integer, TLinkFTNodeXY>                    mapLinkFTNode;

		GetTempedIntersectionNodeInfo(vecIntersectionNodeType);
		if (vecIntersectionNodeType.size() < 1)
		{
			AfxMessageBox(_T("3지/4지 교차로 정보가 존재하지 않습니다."));
			return;
		}

		KBulkDBaseLink::LinkFTNodeGeometry(m_pTarget, mapLinkFTNode);
		if (mapLinkFTNode.size() < 1)
		{
			AfxMessageBox(_T("링크 정보가 존재하지 않습니다."));
			return;
		}

		AnalysisIntersection(vecIntersectionNodeType, mapLinkFTNode);

		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_INTERSECTION);
		pIOTable->Notify();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::AnalysisIntersection(std::vector<KIntersectionNodeType> &a_vecIntersectionNodeType, std::map<Integer, TLinkFTNodeXY> &a_mapLinkFTNode)
{
	try
	{
        ITxFeatureLayerPtr spLayer = m_pMapView->MapGetFeatureLayer(KLayerID::Link());

		std::map<Integer, std::map<int, TIntersectionInfo>> mapAllIntersectionInfo;
		std::map<Integer, std::vector<TSignalIndication>>   mapAllIntersectionSignalSeq;

		std::vector<Integer> vecInterNodeID;
		for (size_t i= 0; i< a_vecIntersectionNodeType.size(); i++)
		{
			vecInterNodeID.push_back(a_vecIntersectionNodeType[i].TNodeID);
		}

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3) == IDC_RADIO1)
		{
			KBulkDBaseIntersection::DeleteAllIntersection(m_pTarget); // 전체 삭제
		}
		else
		{
			KBulkDBaseIntersection::DeleteIntersection                (m_pTarget, vecInterNodeID);
			KBulkDBaseIntersection::DeleteIntersectionBoundInfo       (m_pTarget, vecInterNodeID);
			KBulkDBaseIntersection::DeleteIntersectionDefaultExtension(m_pTarget, vecInterNodeID);
			KBulkDBaseIntersection::DeleteIntersectionSignalIndication(m_pTarget, vecInterNodeID);
		}
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicDonkeyProgress oDonkeyProgress(this, _T("Intersection Topology Analysis"), TRUE, FALSE, FALSE, nLang);   
		CString strDonkeyMessage(_T(""));

		size_t nxCount = a_vecIntersectionNodeType.size();
		for (size_t i=0; i<nxCount; i++)
		{
			strDonkeyMessage.Format(_T("[%d / %d] 계산 중입니다. "), i, nxCount);
			//if (i%10 == 0)
			oDonkeyProgress.SendTextEvent(strDonkeyMessage);

			std::vector<Integer>   vecLinkID;
			KIntersectionNodeType& oIntersectionNodeType = a_vecIntersectionNodeType[i];
			Integer     nxNodeID = oIntersectionNodeType.TNodeID;

			AutoType iter = m_mapNodeArrivedLink.find(nxNodeID);
			AutoType end  = m_mapNodeArrivedLink.end();

			if (iter == end)
				continue;

			vecLinkID = iter->second;
			//KBulkDBaseIntersection::FindArrivedLink(m_pTarget, nxNodeID, vecLinkID);

			if ((int)vecLinkID.size() != oIntersectionNodeType.TDirectionCount)
			{
				continue;
			}

			std::vector<KLinkIDGeometry> vecLinkGeometry;
			for (size_t nK=0; nK<vecLinkID.size(); nK++)
			{
                TxFeaturePtr spFeature = spLayer->GetFeature(vecLinkID[nK]);
				KLinkIDGeometry oLinkIDGeometry(vecLinkID[nK], spFeature->Geometry());
				vecLinkGeometry.push_back(oLinkIDGeometry);
			}

#pragma region Intersection 관련 테이블 디폴트 데이터 생성 관련
			std::map<Integer, TBoundInfo> mapBoundInfo;
			AutoType iterB = m_mapAllIntersectionBoundInfo.find(nxNodeID);
			if (iterB != m_mapAllIntersectionBoundInfo.end())
			{
				mapBoundInfo = iterB->second;
			}
			std::map<int, TIntersectionInfo> mapIntersectionInfo;
			std::vector<TSignalIndication>   vecSignalSeq;
#pragma endregion Intersection 관련 테이블 디폴트 데이터 생성 관련

			if (oIntersectionNodeType.TDirectionCount == 3)
			{
				std::vector<Integer>   vecDirectionLinkID;
				int nType;
				KIntersectionTopologyAnalysis::Analysis3DirectionCrossRoad(vecLinkGeometry, vecDirectionLinkID, nType);

				Insert3DirectionCrossRoad(a_mapLinkFTNode, nxNodeID, vecDirectionLinkID, nType, mapBoundInfo, mapIntersectionInfo, vecSignalSeq);
			}
			else if (oIntersectionNodeType.TDirectionCount == 4)
			{            
				std::vector<Integer>   vecDirectionLinkID;
				KIntersectionTopologyAnalysis::Analysis4DirectionCrossRoad(vecLinkGeometry, vecDirectionLinkID);

				Insert4DirectionCrossRoad(a_mapLinkFTNode, nxNodeID, vecDirectionLinkID, mapBoundInfo, mapIntersectionInfo, vecSignalSeq);
			}

			if (mapIntersectionInfo.size() > 0)
			{
				mapAllIntersectionInfo.insert(std::make_pair(nxNodeID, mapIntersectionInfo));
			}
			mapAllIntersectionSignalSeq.insert(std::make_pair(nxNodeID, vecSignalSeq));
		}

		KBulkDBaseIntersection::InsertIntersectionBoundInfo       (m_pTarget, mapAllIntersectionInfo);
		KBulkDBaseIntersection::InsertIntersectionSignalIndication(m_pTarget, mapAllIntersectionSignalSeq);
		KBulkDBaseIntersection::InsertIntersectionDefaultExtension(m_pTarget, a_vecIntersectionNodeType);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::Insert3DirectionCrossRoad(std::map<Integer, TLinkFTNodeXY> &a_mapLinkFTNode, Integer a_nxNodeID, std::vector<Integer>& a_vecDirectionLinkID, int a_nType,
																 std::map<Integer, TBoundInfo>    &a_mapBoundInfo,  std::map<int, TIntersectionInfo> &a_mapIntersectionInfo, 
																 std::vector<TSignalIndication>   &a_vecSignalSeq)
{
	try
	{
		Integer nxLinkID;
		size_t  nxCount = a_vecDirectionLinkID.size();

		if (nxCount != 4) 
		{
			TxLogDebugException();
			return;
		}

		std::map<Integer, TLinkFTNodeXY>::iterator iter ;
		std::map<Integer, TLinkFTNodeXY>::iterator end  = a_mapLinkFTNode.end();

		nxLinkID = a_vecDirectionLinkID[0];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		TLinkFTNodeXY oLinkFTNode         = iter->second;
		Integer      nxFirstBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[1];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxSecondBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[2];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxThirdBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[3];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxFourthBoundNodeID = oLinkFTNode.TFromNodeID;

		KBulkDBaseIntersection::InsertIntersection3Direction(m_pTarget, a_nxNodeID, nxFirstBoundNodeID, nxSecondBoundNodeID, nxThirdBoundNodeID, nxFourthBoundNodeID, a_nType);

		if (a_mapBoundInfo.size() > 0)
		{
			TIntersectionBase oTIntersectionBase;
			oTIntersectionBase.nxNodeID      = a_nxNodeID;
			oTIntersectionBase.nxFirstBNode  = nxFirstBoundNodeID;
			oTIntersectionBase.nxSecondBNode = nxSecondBoundNodeID;
			oTIntersectionBase.nxThirdBNode  = nxThirdBoundNodeID;
			oTIntersectionBase.nxFourthBNode = nxFourthBoundNodeID;
			oTIntersectionBase.nCrossType    = a_nType;

			for(int i= 0; i< 5; i++)
			{
				TSignalIndication oTSignal;
				oTSignal.nSignalLampType = KEMDirSeq_NONE;
				oTSignal.nGreenLampDuration  = 0;
				oTSignal.nYellowLampDuration = 0;

				a_vecSignalSeq.push_back(oTSignal);
			}
			KIntersectionThreeBasicPage::DefaultModuleDataSet(a_vecSignalSeq, oTIntersectionBase);

			KIntersectionThreeBasicPage::DefaultModuleLaneBoundDataSet(oTIntersectionBase, a_mapBoundInfo, a_mapIntersectionInfo);
		}		
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::Insert4DirectionCrossRoad(std::map<Integer, TLinkFTNodeXY> &a_mapLinkFTNode, Integer a_nxNodeID, std::vector<Integer>& a_vecDirectionLinkID,
	                                                             std::map<Integer, TBoundInfo>    &a_mapBoundInfo,  std::map<int, TIntersectionInfo> &a_mapIntersectionInfo,
	                                                             std::vector<TSignalIndication>   &a_vecSignalSeq)
{
	try
	{
		Integer nxLinkID;
		size_t  nxCount = a_vecDirectionLinkID.size();

		if (nxCount != 4) 
		{
			TxLogDebugException();
			return;
		}

		std::map<Integer, TLinkFTNodeXY>::iterator iter ;
		std::map<Integer, TLinkFTNodeXY>::iterator end  = a_mapLinkFTNode.end();

		nxLinkID = a_vecDirectionLinkID[0];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		TLinkFTNodeXY oLinkFTNode         = iter->second;
		Integer      nxFirstBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[1];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxSecondBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[2];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxThirdBoundNodeID = oLinkFTNode.TFromNodeID;

		nxLinkID = a_vecDirectionLinkID[3];
		iter = a_mapLinkFTNode.find(nxLinkID);
		if (iter == end)
		{
			TxLogDebugException();
			return;
		}

		oLinkFTNode = iter->second;
		Integer       nxFourthBoundNodeID = oLinkFTNode.TFromNodeID;

		KBulkDBaseIntersection::InsertIntersection4Direction(m_pTarget, a_nxNodeID, nxFirstBoundNodeID, nxSecondBoundNodeID, nxThirdBoundNodeID, nxFourthBoundNodeID);

		if (a_mapBoundInfo.size() > 0)
		{
			TIntersectionBase oTIntersectionBase;
			oTIntersectionBase.nxNodeID      = a_nxNodeID;
			oTIntersectionBase.nxFirstBNode  = nxFirstBoundNodeID;
			oTIntersectionBase.nxSecondBNode = nxSecondBoundNodeID;
			oTIntersectionBase.nxThirdBNode  = nxThirdBoundNodeID;
			oTIntersectionBase.nxFourthBNode = nxFourthBoundNodeID;
			oTIntersectionBase.nCrossType    = 0;

			for(int i= 0; i< 6; i++)
			{
				TSignalIndication oTSignal;
				oTSignal.nSignalLampType = KEMDirSeq_NONE;
				oTSignal.nGreenLampDuration  = 0;
				oTSignal.nYellowLampDuration = 0;

				a_vecSignalSeq.push_back(oTSignal);
			}

			std::map<int, TIntersectionInfo> mapEmptyIntersectionInfo; //jyk 호출 함수로 부터 디폴트 값을 추출 하기 위한 empty 설정
			KIntersectionBasicPage::DefaultModuleDataSet(a_vecSignalSeq, mapEmptyIntersectionInfo);

			KIntersectionBasicPage::DefaultModuleLaneBoundDataSet(oTIntersectionBase, a_mapBoundInfo, a_mapIntersectionInfo);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnBnClickedButtonDrawPolygon()
{
	try
	{
		m_pMapView->TargetRegionInsert(-1, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnBnClickedButtonEditPolygon()
{
	try
	{
		EditPolygon(m_vecSelAreaCoordinate);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::EditPolygon(std::vector<TCoordinate> &a_vecSelAreaCoordinate)
{
	try
	{
		if (a_vecSelAreaCoordinate.size()< 2)
		{
			return;
		}

		CWaitCursor cw;

		m_pMapView->TargetRegionEdit(-1, a_vecSelAreaCoordinate, this);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KIntersectionTopologyAnalysisDlg::OnBnClickedCancel()
{
	ClearAllMap();
	KResizeDialogEx::OnCancel();
}


void KIntersectionTopologyAnalysisDlg::ClearAllMap( void )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();
		m_pMapView->CacheClearCompositeThemes(false);
		m_pMapView->RefreshAll();
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