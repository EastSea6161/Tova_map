// TargetRegionSetting.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TargetRegionSetting.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "KExRecordItem.h"
#include "CustomReportRecordItem.h"
#include "NodeSelectionInfo.h"
#include "TDrawNodeInfo.h"

#include "BoundaryIntersectTransit.h"
#include "ImChampMultiZoneFind.h"
#include "TDrawNodeInfo.h"
#include "BulkDBaseTargetRegion.h"
#include "TCoordinate.h"

enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_ID,
	_2_COLUMN_NAME,
	_3_COLMNN_COUNT
};

const int	  nTRZoneSize				= 7;
const int	  nTRZoneCharacterIndex		= 74;
const COLORREF colTRTargetZoneColor		= RGB(47,157,39);
const COLORREF colTROtherZoneColor		= RGB(255,0,0);

// KTargetRegionSetting 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTargetRegionSetting, KResizeDialogEx)

KTargetRegionSetting::KTargetRegionSetting(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTargetRegionSetting::IDD, pParent),
	m_pTarget(a_pTarget),
	m_pMapView(a_pMapView),
	m_nSelectTRCode(-1)
{
}

KTargetRegionSetting::~KTargetRegionSetting()
{
}

void KTargetRegionSetting::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_TR, m_wndReportCtrlTRegion);
}


BEGIN_MESSAGE_MAP(KTargetRegionSetting, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KTargetRegionSetting::OnRadioClicked)
	ON_BN_CLICKED(IDC_BUTTON_ADD_REGION, &KTargetRegionSetting::OnBnClickedButtonAddRegion)
	ON_BN_CLICKED(IDC_BUTTON_DEL_REGION, &KTargetRegionSetting::OnBnClickedButtonDelRegion)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED, IDC_REPORT_TR, OnReportCheckItemTR)
	ON_BN_CLICKED(IDOK, &KTargetRegionSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTargetRegionSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_POLY, &KTargetRegionSetting::OnBnClickedButtonPoly)
	ON_BN_CLICKED(IDC_BUTTON_POLY_EDIT, &KTargetRegionSetting::OnBnClickedButtonPolyEdit)
	ON_BN_CLICKED(IDC_BUTTON_AREACODE, &KTargetRegionSetting::OnBnClickedButtonAreacode)
	
    ON_BN_CLICKED(IDC_BUTTON1, &KTargetRegionSetting::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_CHECK1, &KTargetRegionSetting::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &KTargetRegionSetting::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_CHECK3, &KTargetRegionSetting::OnBnClickedCheck3)
END_MESSAGE_MAP()


// KTargetRegionSetting 메시지 처리기입니다.


BOOL KTargetRegionSetting::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KReportCtrlSetting::Default(m_wndReportCtrlTRegion, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlTRegion);

		ResizeComponent();

		InitAddEditEnableFalseButton();

		CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT1);
		pButton->SetWindowTextW(_T(""));
		pButton->EnableWindow(FALSE);

		CButton* pBtnRadiusED	= (CButton*)(GetDlgItem(IDC_EDIT_RADIUS));
		pBtnRadiusED->SetWindowTextW(_T("1"));

		GetTargetRegionCode();
		
		KDBaseInterModal::GetZoneFromTargetRegion(m_pTarget, m_mapNodeTargetInfo);
		KDBaseInterModal::GetTargetRegionInfo(m_pTarget, m_mapTargetRegionCode, m_mapTargetRegionInfo);

		GetRegionNodeCount();
		InitReportHeaderTR();
		UpdateReportTR();

        LoadInitData();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return FALSE;
	}
	catch(...)
	{
		TxLogDebugException();
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KTargetRegionSetting::GetTargetRegionCode( void )
{
	try
	{
		m_mapTargetRegionCode.clear();

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		pTTypeCodeGroup->GetCodes(m_mapTargetRegionCode);

		std::set<int> setNodeRegionCode;
		KDBaseInterModal::GetTargetRegionCode(m_pTarget,setNodeRegionCode);

		AutoType iter = setNodeRegionCode.begin();
		AutoType end  = setNodeRegionCode.end();
		while(iter != end)
		{
			int nCode = *iter;
			AutoType find = m_mapTargetRegionCode.find(nCode);
			AutoType fend = m_mapTargetRegionCode.end();
			if (find == fend)
			{
				m_mapTargetRegionCode.insert(std::make_pair(nCode, _T(" - ")));
			}
			++iter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTargetRegionSetting::InitReportHeaderTR( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlTRegion.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReportCtrlTRegion.AddColumn(new CXTPReportColumn(_1_COLUMN_ID, _T("Target Region Code"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도시부지역코드"));
        }

		pColumn = m_wndReportCtrlTRegion.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Name"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("명칭"));
        }

		pColumn = m_wndReportCtrlTRegion.AddColumn(new CXTPReportColumn(_3_COLMNN_COUNT, _T("No. of Nodes"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("노드 개수"));
        }
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}	
}


void KTargetRegionSetting::UpdateReportTR( void )
{
	try
	{

		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);

		AutoType iter = m_mapTargetRegionCode.begin();
		AutoType end  = m_mapTargetRegionCode.end();

		while(iter != end)
		{
			int nRegionCode		= iter->first;
			CString strRegionCode(_T(""));
			strRegionCode.Format(_T("%d"), nRegionCode);
			CString strCodeName	= pCodeGroup->SingleCodeValue(nRegionCode); 

			CXTPReportRecord*		pRecord = m_wndReportCtrlTRegion.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = nullptr;

			if (TARGET_REGION_DEFAULT_CODE_KEY == nRegionCode)
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T(" - ")));
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(DT_CENTER);
				
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strRegionCode));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
				pItem->SetEditable(FALSE);
				pItem->SetAlignment(DT_CENTER);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);

			}
			else
			{
				pItem = pRecord->AddItem(new CXTPReportRecordItem());
				pItem->HasCheckbox(TRUE);
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(xtpColumnIconCenter);
				
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strRegionCode));
				pItem->SetEditable(FALSE);
				pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
				pItem->SetAlignment(DT_CENTER);
				
				pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCodeName));
				pItem->SetEditable(TRUE);
				pItem->SetAlignment(DT_CENTER);
			}

			CString strCount(_T("0"));
			AutoType find = m_mapRegionCount.find(nRegionCode);
			AutoType end  = m_mapRegionCount.end();
			if (find != end)
			{
				strCount.Format(_T("%d"), find->second);
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(strCount));
			pItem->SetEditable(FALSE);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetAlignment(DT_CENTER);
		
			++iter;
		}
		m_wndReportCtrlTRegion.Populate();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
	
}

void KTargetRegionSetting::OnBnClickedButtonAddRegion()
{
	try
	{
		bool bRefresh(false);

		int nTotalRowCount = m_wndReportCtrlTRegion.GetRows()->GetCount();

		int nMaxCode(0);
		GetMaxCode(nMaxCode);

		if (nMaxCode > 9)
		{
			AfxMessageBox(_T("10개 이상의 도시부 지역을 생성할 수 없습니다."));
			return;
		}

		ClearReportTR();

		CXTPReportRecord*     pRecord = m_wndReportCtrlTRegion.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem* pItem   = NULL;

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->HasCheckbox(TRUE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetChecked(TRUE);

		nMaxCode = nMaxCode + 1;
		CString strCode(_T(""));
		strCode.Format(_T("%d"), nMaxCode);

		pItem	= pRecord->AddItem(new CXTPReportRecordItemText(strCode));
		pItem->SetEditable(FALSE);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);

		CString strName(_T(""));
		strName.Format(_T("Region%s"),strCode);
		SetDlgItemText(IDC_EDIT2, strName);
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(strName));
		pItem->SetEditable(TRUE);
		pItem->SetAlignment(DT_CENTER);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("0")));
		pItem->SetEditable(FALSE);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetAlignment(DT_CENTER);

		m_wndReportCtrlTRegion.Populate();

		nTotalRowCount = m_wndReportCtrlTRegion.GetRows()->GetCount();
		CXTPReportRow* pRow = m_wndReportCtrlTRegion.GetRows()->GetAt(nTotalRowCount - 1);
		//pRow->SetSelected(TRUE);

		//추가된 컬럼에 Focus주기
		XTP_REPORTRECORDITEM_ARGS args(&m_wndReportCtrlTRegion, pRow, m_wndReportCtrlTRegion.GetColumns()->GetAt(_2_COLUMN_NAME));
		m_wndReportCtrlTRegion.EditItem(&args);

		m_nSelectTRCode = nMaxCode;

		TTargetRegionInfo oRegionInfo = {};
		m_mapTargetRegionInfo.insert(std::make_pair(nMaxCode, oRegionInfo));

		ClearAllMap(bRefresh);

		InitAddEditEnalbeTrueButton();
		UpdateSelectedNodeCount();

		bRefresh = true;
		DrawTargetZoneNode(bRefresh);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
	}
}

void KTargetRegionSetting::OnBnClickedButtonDelRegion()
{
	CString strMsg(_T(""));
	strMsg.Format(_T("선택된 도시부 지역을 삭제하시겠습니까?\r\n삭제하면 해당 노드 테이블의 Target_Region 코드가 Default 값으로 초기화 됩니다."));
	if ( IDNO == AfxMessageBox(strMsg, MB_YESNO) )
	{
		return;
	}

	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItem*        pExtItemCheck = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	int nCount(0);
	int nTRCode(0);

	bool bRefresh(false);

	try
	{
		std::vector<CXTPReportRecord*> vecRecrod;
		pRecords			= m_wndReportCtrlTRegion.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_CHECK);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
				pItemText   = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_ID);
				CString strCode = pItemText->GetValue();
				nTRCode = _ttoi(strCode);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_wndReportCtrlTRegion.GetRecords()->RemoveRecord(pRecord);
			m_wndReportCtrlTRegion.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("선택된 코드 그룹이 존재하지 않습니다."));
			return;
		}

		KCodeManager* pCodeMgr			= m_pTarget->CodeManager();
		KCodeGroup* pTTypeCodeGroup		= pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		pTTypeCodeGroup->RemoveCode(nTRCode);
		pTTypeCodeGroup->ReadCodeData();

		KDBaseInterModal::DeleteTargetRegionInNodeTable(m_pTarget, nTRCode);
		DeleteRegionCodebyMemory(nTRCode);

		KIOTable* pNodeTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
		pNodeTable->Notify();

		CButton* pMaxButton = (CButton*)GetDlgItem(IDC_EDIT1);
		pMaxButton->SetWindowTextW(_T(""));

		InitAddEditEnableFalseButton();
		bRefresh = true;
		ClearAllMap(bRefresh);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTargetRegionSetting::GetMaxCode(int& a_nMaxCode )
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	a_nMaxCode = 0;

	try
	{
		pRecords			= m_wndReportCtrlTRegion.GetRecords();
		int nRecordCount	= pRecords->GetCount();

		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText   = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_ID);
			CString strValue = pItemText->GetValue();
			int nUserCode = _ttoi(strValue);

			if (nUserCode > a_nMaxCode)
			{
				a_nMaxCode = nUserCode;
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTargetRegionSetting::OnReportCheckItemTR( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		CWaitCursor wc;
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		CXTPReportRecordItemText*    pItemText	   = nullptr;
		CString strCodeID = pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_ID)->GetCaption();
		int nCodeID = _ttoi(strCodeID);

		if (TARGET_REGION_DEFAULT_CODE_KEY == nCodeID)
		{
			return;
		}

		bool bRefresh(false);

		ClearAllMap(bRefresh);
		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_CHECK)->IsChecked())
		{
			
			ClearReportTR();
			pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_CHECK)->SetChecked(TRUE);
			CString strCodeName = pItemNotify->pRow->GetRecord()->GetItem(_2_COLUMN_NAME)->GetCaption();

			SetDlgItemText(IDC_EDIT2, strCodeName);

			m_nSelectTRCode = nCodeID;
			UpdateSelectedNodeCount();
			DrawTargetZoneNode(bRefresh);
			InitAddEditEnalbeTrueButton();
		}
		else
		{
			SetDlgItemText(IDC_EDIT2, _T(""));

			m_nSelectTRCode = -1;
			CButton* pMaxButton = (CButton*)GetDlgItem(IDC_EDIT1);
			pMaxButton->SetWindowTextW(_T(""));
			InitAddEditEnableFalseButton();
		}

		bRefresh = true;
		if (bRefresh) {
			m_pMapView->MapRefresh();
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

void KTargetRegionSetting::ClearReportTR( void )
{
	CXTPReportRecords*           pRecords      = nullptr;
	CXTPReportRecord*            pRecord       = nullptr;

	try
	{
		pRecords			= m_wndReportCtrlTRegion.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pRecord->GetItem(_0_COLUMN_CHECK)->SetChecked(FALSE);		
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTargetRegionSetting::DrawTargetZoneNode( bool a_bRefresh )
{
	try
	{
		std::vector<TDrawZoneAccess> vecZoneAccess;
		std::vector<TDrawNodeInfo>	 vecDrawNodeInfo;

		AutoType iter = m_mapNodeTargetInfo.begin();
		AutoType end  = m_mapNodeTargetInfo.end();
		while(iter != end)
		{	
			TTargetRegionNode oInfo = iter->second;
			TDrawZoneAccess oDrawZoneAccess;
			TDrawNodeInfo	oDrawNodeInfo;
			if (m_nSelectTRCode == oInfo.TTRCode)
			{
				CString strZoneName(_T(""));
				strZoneName.Format(_T("%I64d"), oInfo.TnxZoneID);

				oDrawNodeInfo.TNodeID	= oInfo.TnxNodeID;
				oDrawNodeInfo.TSize		= nTRZoneSize;
				oDrawNodeInfo.TSymbol	= nTRZoneCharacterIndex;	
				oDrawNodeInfo.TColorRGB	= colTRTargetZoneColor;
				oDrawNodeInfo.TLabel	= strZoneName;

				vecZoneAccess.push_back(oDrawZoneAccess);
				vecDrawNodeInfo.push_back(oDrawNodeInfo);
			}
			++iter;
		}
		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  a_bRefresh);
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


void KTargetRegionSetting::DrawExcludeNode( bool a_bRefresh )
{
	try
	{
		SelectZoneNodeSymbol(a_bRefresh);
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

void KTargetRegionSetting::SelectZoneNodeSymbol( bool a_bRefresh )
{
	try
	{
		std::vector<TNodeSelectionInfo> vecNodeZone;
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;

		AutoType iter = m_mapNodeTargetInfo.begin();
		AutoType end =  m_mapNodeTargetInfo.end();
		while (iter != end)
		{
			TNodeSelectionInfo oNodeZone;

			TTargetRegionNode oInfo = iter->second;
			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), oInfo.TnxZoneID);

			if (oInfo.TTRCode == m_nSelectTRCode)
			{
				TDrawNodeInfo oInNode;
				oInNode.TNodeID			= oInfo.TnxNodeID;
				oInNode.TSymbol = nTRZoneCharacterIndex;
				oInNode.TLabel			= strZoneID;
				oInNode.TColorRGB		= colTRTargetZoneColor;

				vecDrawNodeInfo.push_back(oInNode);

			}
			else
			{
				oNodeZone.TNodeID = oInfo.TnxNodeID;
				oNodeZone.TType	  = 1;
				oNodeZone.TCharacterIndex = nTRZoneCharacterIndex;
				oNodeZone.TLabel  = strZoneID;

				vecNodeZone.push_back(oNodeZone);
			}
			++iter;
		}

		m_pMapView->MapNodeSelectAction(this, vecNodeZone, false, true);
		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  false);

		if (a_bRefresh) {
			m_pMapView->MapRefresh();
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

void KTargetRegionSetting::ClearAllMap( bool a_bRefresh )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit(true);
		m_pMapView->CacheClearCompositeThemes(false);

		if (a_bRefresh) {
			m_pMapView->MapRefresh();
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


void KTargetRegionSetting::OnBnClickedOk()
{
	CString strCount;
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT1);
	pButton->GetWindowTextW(strCount);

	int nCount = _ttoi(strCount);

	/*if (nCount < 1)
	{
	AfxMessageBox(_T("적용 가능한 데이터가 존재하지 않습니다."));
	return;
	}*/
	
	if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;

	try
	{
		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(TargetRegionThreadCaller, this, true, nLang);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	AfxMessageBox(_T("변경사항이 저장되었습니다."));
}


void KTargetRegionSetting::OnBnClickedCancel()
{
    m_pMapView->MapRemoveLayer(KLayerID::LayerID_TagetRegionRail());
    m_pMapView->MapRemoveLayer(KLayerID::LayerID_TagetRegionHighway());
    m_pMapView->MapRemoveLayer(KLayerID::LayerID_TagetRegionStnRail());
    m_pMapView->MapRemoveLayer(KLayerID::LayerID_TagetRegionStnHigh());

	ClearAllMap(true);
	KResizeDialogEx::OnCancel();
}

void KTargetRegionSetting::UpdateTargetRegionCode( void )
{

	std::map<int, CString> mapTargetRegionCode;

	CXTPReportRecords*        pRecords    = m_wndReportCtrlTRegion.GetRecords();
	CXTPReportRecord*         pRecord     = nullptr;
	CXTPReportRecordItem*     pItem       = nullptr;
	CXTPReportRecordItemText* pItemText	  = nullptr;

	try
	{
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_ID);
			CString strCodeID = pItemText->GetValue();
			int nCodeID = _ttoi(strCodeID);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_NAME);
			CString strCodeName = pItemText->GetValue();

			if (false == QBicStringChecker::IsUnSignedInteger(strCodeID))
			{
				CString strMsg;
				strMsg.Format(_T("User Code Key : %s 의 값이 잘못되었습니다."), strCodeID);
				ThrowException(strMsg);
			}

			mapTargetRegionCode.insert(std::make_pair(nCodeID, strCodeName));
		}

		KCodeManager* pCodeMgr				= m_pTarget->CodeManager();
		KCodeGroup* pRegionCodeGroup		= pCodeMgr->FindCodeGroup(TARGET_REGION_CODE_GROUP_KEY);
		pRegionCodeGroup->RemoveAllCode();


		AutoType iterCode = mapTargetRegionCode.begin();
		AutoType endCode  = mapTargetRegionCode.end();
		while(iterCode != endCode)
		{
			int nCode		= iterCode->first;
			CString strName = iterCode->second;
			pRegionCodeGroup->AddCode(nCode, strName);
			iterCode++;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KTargetRegionSetting::UpdateSelectedNodeCount( void )
{
	try
	{
// 		int nCount(0);
// 		AutoType iter = m_mapNodeTargetInfo.begin();
// 		AutoType end  = m_mapNodeTargetInfo.end();
// 		while(iter != end)
// 		{
// 			TTargetRegionNode oInfo = iter->second;
// 			if (m_nSelectTRCode == oInfo.TTRCode)
// 			{
// 				nCount++;
// 			}
// 			++iter;
// 		}
// 
// 		CString strCount(_T(""));
// 		strCount.Format(_T("%d"), nCount);
// 		CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT1);
// 		pButton->SetWindowTextW(strCount);

		GetRegionNodeCount();

		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemText*	 pItem		   = nullptr;

		pRecords			= m_wndReportCtrlTRegion.GetRecords();
		int nRecordCount	= pRecords->GetCount();

		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_ID);
			CString strCodeID = pItem->GetValue();
			int nCode = _ttoi(strCodeID);
			AutoType find = m_mapRegionCount.find(nCode);
			AutoType end  = m_mapRegionCount.end();
			if (find != end)
			{
				CString strCount(_T(""));
				strCount.Format(_T("%d"), find->second);
				pItem = (CXTPReportRecordItemText*)pRecord->GetItem(_3_COLMNN_COUNT);
				pItem->SetValue(strCount);

			}
			else
			{
				pItem = (CXTPReportRecordItemText*)pRecord->GetItem(_3_COLMNN_COUNT);
				pItem->SetValue(_T("0"));
			}
		}
		m_wndReportCtrlTRegion.Populate();
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



void KTargetRegionSetting::OnBnClickedButtonPoly()
{
	if (m_nSelectTRCode == -1)
	{
		AfxMessageBox(_T("선택한 도시부 지역이 없습니다."));
		return;
	}
		
    m_pMapView->TargetRegionInsert(m_nSelectTRCode, this);
}

void KTargetRegionSetting::OnBnClickedButtonPolyEdit()
{
    try
    {
        if (m_nSelectTRCode == -1)
        {
            AfxMessageBox(_T("선택한 도시부 지역이 없습니다."));
            return;
        }
        
		TTargetRegionInfo oInfo;

		AutoType iter = m_mapTargetRegionInfo.find(m_nSelectTRCode);
		AutoType end  = m_mapTargetRegionInfo.end();
		if (iter != end)
		{
			oInfo = iter->second;
            if (oInfo.vecTargetRegionCoordinate.size() >= 2)
                m_pMapView->TargetRegionEdit(m_nSelectTRCode, oInfo.vecTargetRegionCoordinate, this);
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

void KTargetRegionSetting::ResizeComponent( void )
{
	SetResize(IDC_STATIC36,					  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_TR,				  SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_ADD_REGION,		  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);	
	SetResize(IDC_BUTTON_DEL_REGION,		  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC2,					  SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC5,					  SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT2,					  SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC3,					  SZ_BOTTOM_RIGHT,    SZ_BOTTOM_RIGHT);

	SetResize(IDC_EDIT1,					  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_RIGHT);

	SetResize(IDC_RADIO1,					  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO2,					  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_AREACODE,			  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);

	SetResize(IDC_BUTTON_POLY,				  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_POLY_EDIT,			  SZ_BOTTOM_LEFT,	 SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC4,					  SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);

	SetResize(IDOK,							  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					      SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}

unsigned __stdcall KTargetRegionSetting::TargetRegionThreadCaller( void* p )
{
	QBicSimpleProgressParameter* pParameter      = (QBicSimpleProgressParameter*)p;
	QBicSimpleProgress*          pProgressWindow = pParameter->GetProgressWindow();

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pProgressWindow->SetStatus(_T("데이터 저장 중..."));
	}
	else {
		pProgressWindow->SetStatus(_T("Saving Data..."));
	}
	KTargetRegionSetting*        pDlg = (KTargetRegionSetting*)pParameter->GetParameter();
	pDlg->Run();

	return 0;
}

void KTargetRegionSetting::Run( void )
{
	try
	{
		UpdateTargetRegionCode();
		KDBaseInterModal::UpdateNodeTargetRegion(m_pTarget, m_mapNodeTargetInfo);
		UpdatePolygonVertexInfo();

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_NODE);
		pTable->Notify();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
}


void KTargetRegionSetting::OnBnClickedButtonAreacode()
{
	bool bRefresh(false); 

	std::set<Integer> setSelectedZone;
	std::vector<Integer> setSelectedNode;
	KImChampMultiZoneFind oZoneFindDlg(m_pTarget);
	oZoneFindDlg.SetUseTransitInclude(true);
	if (oZoneFindDlg.DoModal() == IDOK)
	{
		oZoneFindDlg.GetSelectedID(setSelectedZone);

		AutoType iter = m_mapNodeTargetInfo.begin();
		AutoType end  = m_mapNodeTargetInfo.end();
		while(iter != end)
		{
			Integer nxNodeID = iter->first;
			TTargetRegionNode oInfo = iter->second;
			
			AutoType find		= setSelectedZone.find(oInfo.TnxZoneID);
			AutoType findend	= setSelectedZone.end();
			if (find != findend)
			{
				setSelectedNode.push_back(nxNodeID);
			}
			++iter;
		}

		if (oZoneFindDlg.GetTransitInclude()== true)
		{
			KBoundaryIntersectTransit::AddBoundaryIntersectTransit(m_pTarget, setSelectedNode);
		}

		AddInZoneFromMap(setSelectedNode);

		AutoType find = m_mapTargetRegionInfo.find(m_nSelectTRCode);
		AutoType findend  = m_mapTargetRegionInfo.end();

		if (find != findend)
		{
			TTargetRegionInfo& oInfo = find->second;
			std::vector<TCoordinate> vecTargetRegionCoordinate;
			oInfo.vecTargetRegionCoordinate= vecTargetRegionCoordinate;
		}

		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawTargetZoneNode(bRefresh);
		UpdateSelectedNodeCount();
	}
}

void KTargetRegionSetting::OnRadioClicked( UINT nID )
{
	try
	{
		int  nCheckedRadioBtn  = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2);
		if (nCheckedRadioBtn == IDC_RADIO1)
		{
			CButton* pBtnArea	= (CButton*)(GetDlgItem(IDC_BUTTON_AREACODE));
			pBtnArea->EnableWindow(TRUE);
			
			CButton* pBtnPolyD	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY));
			pBtnPolyD->EnableWindow(FALSE);

			CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
			pBtnPolyE->EnableWindow(FALSE);
		}
		else
		{
			CButton* pBtnArea	= (CButton*)(GetDlgItem(IDC_BUTTON_AREACODE));
			pBtnArea->EnableWindow(FALSE);

			CButton* pBtnPolyD	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY));
			pBtnPolyD->EnableWindow(TRUE);

			AutoType iter = m_mapTargetRegionInfo.find(m_nSelectTRCode);
			AutoType end  = m_mapTargetRegionInfo.end();
			if (iter == end)
			{
				CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
				pBtnPolyE->EnableWindow(FALSE);
			}
			else
			{
				TTargetRegionInfo oInfo = iter->second;
				size_t nCount = oInfo.vecTargetRegionCoordinate.size();

				if (nCount > 2)
				{
					CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
					pBtnPolyE->EnableWindow(TRUE);
				}
				else
				{
					CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
					pBtnPolyE->EnableWindow(FALSE);
				}
			}
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

void KTargetRegionSetting::InitAddEditEnableFalseButton( void )
{

	CButton* pBtnRadio1	= (CButton*)(GetDlgItem(IDC_RADIO1));
	pBtnRadio1->EnableWindow(FALSE);
	pBtnRadio1->SetCheck(FALSE);

	CButton* pBtnRadio2	= (CButton*)(GetDlgItem(IDC_RADIO2));
	pBtnRadio2->EnableWindow(FALSE);
	pBtnRadio2->SetCheck(FALSE);

	CButton* pBtnArea	= (CButton*)(GetDlgItem(IDC_BUTTON_AREACODE));
	pBtnArea->EnableWindow(FALSE);

	CButton* pBtnPolyD	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY));
	pBtnPolyD->EnableWindow(FALSE);

	CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
	pBtnPolyE->EnableWindow(FALSE);
}

void KTargetRegionSetting::InitAddEditEnalbeTrueButton( void )
{
	//
	CButton* pBtnRadio1	= (CButton*)(GetDlgItem(IDC_RADIO1));
	pBtnRadio1->EnableWindow(TRUE);
	pBtnRadio1->SetCheck(TRUE);

	CButton* pBtnArea	= (CButton*)(GetDlgItem(IDC_BUTTON_AREACODE));
	pBtnArea->EnableWindow(TRUE);

	//
	CButton* pBtnRadio2	= (CButton*)(GetDlgItem(IDC_RADIO2));
	pBtnRadio2->EnableWindow(TRUE);
	pBtnRadio2->SetCheck(FALSE);

	CButton* pBtnPolyD	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY));
	pBtnPolyD->EnableWindow(FALSE);

	CButton* pBtnPolyE	= (CButton*)(GetDlgItem(IDC_BUTTON_POLY_EDIT));
	pBtnPolyE->EnableWindow(FALSE);
}

void KTargetRegionSetting::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
    try
	{
		bool bRefresh(false);
		AddInZoneFromMap(a_vecSelection);
		ClearAllMap(bRefresh);

		DrawTargetZoneNode(bRefresh);

		AutoType find = m_mapTargetRegionInfo.find(m_nSelectTRCode);
		AutoType end  = m_mapTargetRegionInfo.end();
		if (find != end)
		{
			TTargetRegionInfo& oInfo = find->second;
			oInfo.vecTargetRegionCoordinate = a_vecCoordinate;
		}

		bRefresh = true;
		if (bRefresh) {
			m_pMapView->MapRefresh();
		}

		UpdateSelectedNodeCount();
        OnBnClickedButtonPolyEdit();
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

void KTargetRegionSetting::MapSelectionFeedback(std::vector<Integer> a_vecSelection)
{

}
void KTargetRegionSetting::MapSelectionFeedback(Integer a_nxNodeID)
{

}
void KTargetRegionSetting::MapSelectionNoneFeedback()
{

}

void KTargetRegionSetting::AddInZoneFromMap( std::vector<Integer> a_vecNodeID )
{
	try
	{
		size_t nCount = a_vecNodeID.size();
		if (nCount < 1)	{
			return;
		}

		AutoType iter = m_mapNodeTargetInfo.begin();
		AutoType end  = m_mapNodeTargetInfo.end();
		while(iter != end)
		{
			TTargetRegionNode& oInfo = iter->second;
			if (oInfo.TTRCode == m_nSelectTRCode)
			{
				oInfo.TTRCode = 0;
			}
			++iter;
		}

		for (size_t i = 0; i < nCount; i++)
		{
			Integer nxNode = a_vecNodeID[i];
			AutoType find = m_mapNodeTargetInfo.find(nxNode);

			if (find != end)
			{
				TTargetRegionNode& oInfo = find->second;
				oInfo.TTRCode = m_nSelectTRCode;
			}
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

void KTargetRegionSetting::UpdatePolygonVertexInfo( void )
{
	AutoType iter = m_mapTargetRegionInfo.begin();
	AutoType end  = m_mapTargetRegionInfo.end();

	while(iter != end)
	{
		int nCode = iter->first;
		
		TTargetRegionInfo oInfo = iter->second;
		KBulkDBaseTargetRegion::InsertTargetRegion(m_pTarget, nCode, oInfo.vecTargetRegionCoordinate);

		++iter;
	}

}

void KTargetRegionSetting::GetRegionNodeCount( void )
{
	try
	{
		m_mapRegionCount.clear();

		AutoType iter = m_mapNodeTargetInfo.begin();
		AutoType end  = m_mapNodeTargetInfo.end();
		while(iter != end)
		{
			TTargetRegionNode oNode = iter->second;
			AutoType find = m_mapRegionCount.find(oNode.TTRCode);
			AutoType end  = m_mapRegionCount.end();
			if (find != end)
			{
				int& nCount = find->second;
				nCount = nCount + 1;
			}
			else
			{
				m_mapRegionCount.insert(std::make_pair(oNode.TTRCode, 1));
			}
			++iter;
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

void KTargetRegionSetting::DeleteRegionCodebyMemory( int& a_nCode )
{
	AutoType iter = m_mapNodeTargetInfo.begin();
	AutoType end  = m_mapNodeTargetInfo.end();
	while(iter != end)
	{
		TTargetRegionNode& oInfo = iter->second;
		if (oInfo.TTRCode == a_nCode) {
			oInfo.TTRCode = 0;
		}
		++iter;
	}
}



void KTargetRegionSetting::LoadInitData()
{
    CString strSQL(_T("")); {
        strSQL.Append(_T(" Select "));
        strSQL.Append(_T("  T1.Transit_ID, T1.Type, T1.Link_ID, T2.Fnode_Id, T2.Tnode_id, T1.Station_YN, T1.Seq "));
        strSQL.Append(_T(" From "));
        strSQL.Append(_T(" ( "));
        strSQL.Append(_T("  Select "));
        strSQL.Append(_T("   T1.Transit_ID, T1.Type, T2.Link_ID, T2.Seq, T2.Station_YN "));
        strSQL.Append(_T("  From "));
        strSQL.Append(_T("   Transit T1, Transit_Links T2 "));
        strSQL.Append(_T("  Where T1.Transit_ID = T2.Transit_ID "));
        strSQL.Append(_T(" ) "));
        strSQL.Append(_T(" T1, Link T2 "));
        strSQL.Append(_T(" Where ")); 
        strSQL.Append(_T("  T1.Link_ID = T2.Link_ID "));
    }

    KDBaseConPtr  spDBase  = m_pTarget->GetDBaseConnection();
    KResultSetPtr spResult = spDBase->ExecuteQuery(strSQL);
        
    while (spResult->Next()) {
        __int64 nxTransitID = spResult->GetValueI64(0);
        int nType           = spResult->GetValueInt(1);
        __int64 nxLinkID    = spResult->GetValueI64(2);
        __int64 nxFNodeID   = spResult->GetValueI64(3);
        __int64 nxTNodeID   = spResult->GetValueI64(4);
        int nStnYN          = spResult->GetValueInt(5);
        int nSeq            = spResult->GetValueInt(6);

        std::set<__int64>& setLink = m_mapTransitLine[nType]; {
            setLink.insert(nxLinkID);
        }

        if (nSeq == 1) {
            std::set<__int64>& setNode = m_mapTransitStn[nType];
            setNode.insert(nxFNodeID);
        }

        if (nStnYN == 1) {
            std::set<__int64>& setNode = m_mapTransitStn[nType];
            setNode.insert(nxTNodeID);
        }
    }
}

void KTargetRegionSetting::OnBnClickedButton1()
{
    CString strRadius(_T("")); {
        GetDlgItemText(IDC_EDIT_RADIUS, strRadius);
    }
    double dRadius(0);
    try {
        dRadius = _ttof(strRadius);
        dRadius = dRadius * 1000;
    }
    catch(...) {
    }

    ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnHigh());
    if (spLayer != nullptr) {
        ITxSymbolPtr spSymbol = spLayer->LayerDispSymbol();
        TxSymbolPointCirclePtr spSymPoint = std::dynamic_pointer_cast<TxSymbolPointCircle>(spSymbol);
        spSymPoint->Radius(dRadius);
        spLayer->SetDrawExpire();
    }

    spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnRail());
    if (spLayer != nullptr) {
        ITxSymbolPtr spSymbol = spLayer->LayerDispSymbol();
        TxSymbolPointCirclePtr spSymPoint = std::dynamic_pointer_cast<TxSymbolPointCircle>(spSymbol);
        spSymPoint->Radius(dRadius);
        spLayer->SetDrawExpire();
    }

    DrawStnRadius(true);
}


void KTargetRegionSetting::OnBnClickedCheck1()
{
    //★ 정류장 부분 처리
    DrawStnRadius(false);

    //★ Highway Transit Line
    bool bDraw(false);
    int  nLayerID = KLayerID::LayerID_TagetRegionHighway();

    if(IsDlgButtonChecked(IDC_CHECK1) == TRUE) {
        bDraw = true;
    }
    
    ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(nLayerID);
    if (spLayer != nullptr) {
        if (bDraw) {
            spLayer->LayerOn(true);
        }
        else {
            spLayer->LayerOn(false);
        }

        m_pMapView->MapRefresh();
        return; //★ 종료
    }

    //★ 
    ITxFeatureLayerPtr spLayerLink = m_pMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayerLink == nullptr)
        return;

    if (bDraw) {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->AddUserLayer(nLayerID);
        std::set<__int64>& setLink = m_mapTransitLine[3];

        TxSymbolLinePtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(2, Gdiplus::Color(200, 255, 0, 0), 0); {
            spLayer->LayerDispSymbol(spSymbol);
        }

        for (auto iter = setLink.begin(); iter != setLink.end(); ++iter) {
            __int64 nxLinkID = *iter;
            ITxGeometryPtr spGeometry = spLayerLink->GetGeometry(nxLinkID);

            if (spGeometry) {
                TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxLinkID, spGeometry));
                spLayer->Insert(spFeature);
                //spLayer->FeatureSymbol(nxLinkID, spSymbol);
            }
        }
    }
    else {
        return;
    }

    //★ 레이어 추가    
    m_pMapView->MapRefresh();
}


void KTargetRegionSetting::OnBnClickedCheck2()
{
    //★ 정류장 부분 처리
    DrawStnRadius(false);

    //★ Rail Transit Line
    bool bDraw(false);
    int  nLayerID = KLayerID::LayerID_TagetRegionRail();

    if(IsDlgButtonChecked(IDC_CHECK2) == TRUE) {
        bDraw = true;
    }

    ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(nLayerID);
    if (spLayer != nullptr) {
        if (bDraw) {
            spLayer->LayerOn(true);
        }
        else {
            spLayer->LayerOn(false);
        }

        m_pMapView->MapRefresh();
        return; //★ 종료
    }

    //★ 
    ITxFeatureLayerPtr spLayerLink = m_pMapView->MapGetFeatureLayer(KLayerID::Link());
    if (spLayerLink == nullptr)
        return;

    if (bDraw) {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->AddUserLayer(nLayerID);
        std::set<__int64>& setLink = m_mapTransitLine[4];

        TxSymbolLinePtr spSymbol = TxSymbolLine::CreateOffsetLineSymbol(3, Gdiplus::Color(100, 255, 0, 0), 0); {
            spLayer->LayerDispSymbol(spSymbol);
        }

        for (auto iter = setLink.begin(); iter != setLink.end(); ++iter) {
            __int64 nxLinkID = *iter;
            ITxGeometryPtr spGeometry = spLayerLink->GetGeometry(nxLinkID);

            if (spGeometry) {
                TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxLinkID, spGeometry));
                spLayer->Insert(spFeature);
                //spLayer->FeatureSymbol(nxLinkID, spSymbol);
            }
        }
    }
    else {
        return;
    }

    //★ 레이어 추가    
    m_pMapView->MapRefresh();
}

void KTargetRegionSetting::OnBnClickedCheck3()
{
    DrawStnRadius(true);
}

void KTargetRegionSetting::DrawStnRadius(bool bRefresh)
{
    int nLayerID = KLayerID::LayerID_TagetRegionStnHigh();

    bool bDrawStn(false);
    if (IsDlgButtonChecked(IDC_CHECK3) == TRUE) {
        bDrawStn = true;
    }

    //★ 전체 Off
    if (bDrawStn == false) {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnHigh());
        if (spLayer != nullptr) {
            spLayer->LayerOn(false);
        }

        spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnRail());
        if (spLayer != nullptr) {
            spLayer->LayerOn(false);
        }

        if (bRefresh)
            m_pMapView->MapRefresh();

        return;
    }

    CString strRadius(_T("")); {
        GetDlgItemText(IDC_EDIT_RADIUS, strRadius);
    }
    double dRadius(0);
    try {
        dRadius = _ttof(strRadius);
        dRadius = dRadius * 1000;
    }
    catch(...) {
    }
    
    if (IsDlgButtonChecked(IDC_CHECK1) == TRUE) {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnHigh());
        if (spLayer != nullptr) {
            spLayer->LayerOn(true);
        }
        else {
            //★ Highway 정류장 Drawing
            ITxFeatureLayerPtr spLayerNode = m_pMapView->MapGetFeatureLayer(KLayerID::Node());

            spLayer = m_pMapView->AddUserLayer(KLayerID::LayerID_TagetRegionStnHigh());
            std::set<__int64>& setNode = m_mapTransitStn[3];

            TxSymbolPointCirclePtr spSymbol = TxSymbolPointCircle::CreatePointSymbol(dRadius, Gdiplus::Color(100, 0, 0, 255), Gdiplus::Color::Blue, 1); {
                spSymbol->UsePixelDist(false);
            }

            spLayer->LayerDispSymbol(spSymbol);

            for (auto iter = setNode.begin(); iter != setNode.end(); ++iter) {
                __int64 nxID = *iter;
                ITxGeometryPtr spGeometry = spLayerNode->GetGeometry(nxID);
                if (spGeometry) {
                    TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
                    spLayer->Insert(spFeature);
                    //spLayer->FeatureSymbol(nxID, spSymbol);
                }
            }
        }
    }
    else {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnHigh());
        if (spLayer != nullptr) {
            spLayer->LayerOn(false);
        }
    }

    if (IsDlgButtonChecked(IDC_CHECK2) == TRUE) {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnRail());
        if (spLayer != nullptr) {
            spLayer->LayerOn(true);
        }
        else {
            //★ Highway 정류장 Drawing
            ITxFeatureLayerPtr spLayerNode = m_pMapView->MapGetFeatureLayer(KLayerID::Node());

            spLayer = m_pMapView->AddUserLayer(KLayerID::LayerID_TagetRegionStnRail());
            std::set<__int64>& setNode = m_mapTransitStn[4];

            TxSymbolPointCirclePtr spSymbol = TxSymbolPointCircle::CreatePointSymbol(dRadius, Gdiplus::Color(100, 0, 255, 255), Gdiplus::Color::Blue, 1); {
                spSymbol->UsePixelDist(false);
            }

            spLayer->LayerDispSymbol(spSymbol);

            for (auto iter = setNode.begin(); iter != setNode.end(); ++iter) {
                __int64 nxID = *iter;
                ITxGeometryPtr spGeometry = spLayerNode->GetGeometry(nxID);
                if (spGeometry) {
                    TxFeaturePtr spFeature = TxFeaturePtr(new TxFeature(nxID, spGeometry));
                    spLayer->Insert(spFeature);
                    //spLayer->FeatureSymbol(nxID, spSymbol);
                }
            }
        }
    }
    else {
        ITxFeatureUserLayerPtr spLayer = m_pMapView->MapGetFeatureUserLayer(KLayerID::LayerID_TagetRegionStnRail());
        if (spLayer != nullptr) {
            spLayer->LayerOn(false);
        }
    }

    if (bRefresh)
        m_pMapView->MapRefresh();
}