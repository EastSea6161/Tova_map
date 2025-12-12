// ZoneODGroupGenerationDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ZoneODGroupGenerationDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"
#include "CodeManagementDlg.h"
#include "ImChampMultiZoneFind.h"
#include "MapView.h"
#include "ImChampFrameWindow.h"
#include "TableEditFieldInfo.h"
#include "DBaseImportTable.h"

enum
{
	_0_COLUMN_ID,
	_1_COLUMN_NAME,
	_2_COLUMN_ZONE_ID,
	_3_COLUMN_AREACODE,
	_4_COLUMN_POLYGON,
	_5_COLUMN_COUNT
};

// KZoneODGroupGenerationDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KZoneODGroupGenerationDlg, KResizeDialogEx)

KZoneODGroupGenerationDlg::KZoneODGroupGenerationDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KZoneODGroupGenerationDlg::IDD, pParent)
	, m_pTarget (a_pTarget)
	, m_pMapView(a_pMapView)
{
	KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
	KDBaseImportTable::GetZoneID (pTable, m_setZoneID);
}

KZoneODGroupGenerationDlg::~KZoneODGroupGenerationDlg()
{
	KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
	pTable->Unregister(this);
}

void KZoneODGroupGenerationDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBO_GROUP_NAME, m_cboGroupName);
	DDX_Control(pDX, IDC_REPORT1,        m_ctrlReport);
	DDX_Control(pDX, IDC_COMBO1,         m_cboOutColumnName);
}


BEGIN_MESSAGE_MAP(KZoneODGroupGenerationDlg, KResizeDialogEx)
	ON_NOTIFY (XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT1, &KZoneODGroupGenerationDlg::OnItemButtonClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2,   &KZoneODGroupGenerationDlg::OnBnClickedRadio)
	ON_CBN_SELCHANGE(IDC_CBO_GROUP_NAME,                   &KZoneODGroupGenerationDlg::OnCbnSelchangeCboGroupName)
	ON_BN_CLICKED   (IDC_BUTTON_MODIFY,                    &KZoneODGroupGenerationDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDCANCEL, &KZoneODGroupGenerationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &KZoneODGroupGenerationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KZoneODGroupGenerationDlg 메시지 처리기입니다.
void KZoneODGroupGenerationDlg::ActivatedFrameWndNotifyProcess()
{

}


void KZoneODGroupGenerationDlg::DeActivatedFrameWndNotifyProcess()
{

}


void KZoneODGroupGenerationDlg::InitializeStatus()
{
	try
	{
		SetDlgItemText(IDC_EDIT_COLUMN_NAME, _T(""));
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		ControlRadio();

		m_mapZoneIdCode.clear();
		m_mapCodePolygonInfo.clear();
		m_pSelectRecord = nullptr;

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		KDBaseImportTable::GetZoneID (pTable, m_setZoneID);

		LoadCodeGroup();
		LoadReportData();
		LoadOutColumnName();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC_HBar2,      SZ_TOP_LEFT,      SZ_TOP_RIGHT);
		SetResize(IDC_REPORT1,           SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);
		SetResize(IDC_STATIC4,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO1,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_RADIO2,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_EDIT_COLUMN_NAME,  SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
		SetResize(IDC_COMBO1,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);

		SetResize(IDC_STATIC_HBar1,      SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);

		SetResize(IDOK,                  SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
		SetResize(IDCANCEL,              SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::NotifyProcess(LPCTSTR a_strSubjectName, Integer a_nxObjectID)
{
	try
	{
		CString strSubjectName(a_strSubjectName);

		if(strSubjectName.CompareNoCase(TABLE_ZONE) == 0)
		{
			LoadOutColumnName();
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


BOOL KZoneODGroupGenerationDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, FALSE);
	InitReportHeader();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	ControlRadio();

	LoadCodeGroup();
	LoadReportData();
	m_ctrlReport.Populate();
	LoadOutColumnName();

	ResizeComponent();

	KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
	pTable->Register(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KZoneODGroupGenerationDlg::InitReportHeader()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_0_COLUMN_ID,       _T("Code"),       20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("코드번호"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_1_COLUMN_NAME,     _T("Name"),     25));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("명칭"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_2_COLUMN_ZONE_ID,  _T("ZoneID"),   35));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("존노드ID"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_3_COLUMN_AREACODE, _T("AreaCode"), 55, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("행정구역코드"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_4_COLUMN_POLYGON,  _T("Polygon"),  45, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("구역범위"));
		}

		pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(_5_COLUMN_COUNT,    _T("Count"),    20));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("존 개수"));
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


void KZoneODGroupGenerationDlg::LoadOutColumnName()
{
	try
	{
		CString strPreSelName(_T(""));

		int nPreSel = m_cboOutColumnName.GetCurSel();
		if (nPreSel >=0)
		{
			KIOColumn* pIOColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nPreSel);
			try // delete Coloumn이후 m_cboOutColumnName에 반영이 안되어
			{
				strPreSelName = pIOColumn->Name();
			}
			catch (...)
			{
				TxLogDebugException();	
				strPreSelName = _T("");
			}
		}

		m_cboOutColumnName.ResetContent();

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		const 
			KIOColumns* pIOColumns = pTable->Columns();
		int nCount = pIOColumns->ColumnCount();

		int nSelectIndex(0);
		int nCodeColumnCount(0);
		for (int i=0; i <nCount; i++)
		{
			KIOColumn* pIOColumn = pIOColumns->GetColumn(i);

			if (pIOColumn->CodeType() != KEMIOCodeTypeIsSingleCode || pIOColumn->ColumnType() != KEMIOColumnTypeUserDefine)
				continue;

			int nIndex = m_cboOutColumnName.AddString(pIOColumn->Caption());
			m_cboOutColumnName.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			if (strPreSelName.CompareNoCase(pIOColumn->Name()) == 0)
			{
				nSelectIndex = nIndex;
			}
			nCodeColumnCount++;
		}

		if (nCodeColumnCount > 0)
		{
			m_cboOutColumnName.SetCurSel(nSelectIndex);
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


void KZoneODGroupGenerationDlg::LoadCodeGroup()
{
	try
	{
		int nPreSelectCodeGroupKey(-99);
		int nCurSel = m_cboGroupName.GetCurSel();        
		if (nCurSel >=0)
		{
			KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboGroupName.GetItemData(nCurSel);
			nPreSelectCodeGroupKey = pCodeGroup->CodeGroupKey();
		}

		KCodeManager* pCodeManager = m_pTarget->CodeManager();        

		m_cboGroupName.ResetContent();

		std::vector<KCodeGroup*> vecCodeGroups;
		pCodeManager->GetCodeGroups(vecCodeGroups);

		nCurSel = 0;
		int nIndex(0);
		size_t nxCount = vecCodeGroups.size();
		for (size_t i=0; i<nxCount; i++)
		{
			KCodeGroup* pCodeGroup = vecCodeGroups[i];
			CString strCodeYN = pCodeGroup->SystemCodeYN();
			if ( strCodeYN.CompareNoCase(_T("Y")) == 0 )
				continue;

			int nIndex  = m_cboGroupName.AddString(pCodeGroup->CodeGroupName());
			m_cboGroupName.SetItemData(nIndex, (DWORD_PTR)pCodeGroup);
			int nCodeGroupKey = pCodeGroup->CodeGroupKey();
			if (nCodeGroupKey == nPreSelectCodeGroupKey)
			{
				nCurSel = nIndex;
			}
			nIndex++;
		}

		m_cboGroupName.SetCurSel(nCurSel);
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


void KZoneODGroupGenerationDlg::LoadReportData()
{
	try
	{
		m_ctrlReport.ResetContent();
		m_mapZoneIdCode.clear();
		m_mapCodePolygonInfo.clear();
		ClearAllMap();

		int nCurSel = m_cboGroupName.GetCurSel();  
		if (nCurSel < 0)
			return;

		KCodeGroup* pCodeGroup = (KCodeGroup*)m_cboGroupName.GetItemData(nCurSel);

		std::map<int, CString> mapCode;
		pCodeGroup->GetCodes(mapCode);

		std::map<int, CString>::iterator iter = mapCode.begin();
		std::map<int, CString>::iterator end  = mapCode.end();

		CXTPReportRecord*            pRecord = nullptr;
		CXTPReportRecordItem*        pItem   = nullptr;
		CXTPReportRecordItemControl* pButton = nullptr;

		int nIndex(0);
		while (iter != end)
		{
			CString strTemp;
			strTemp.Format(_T("%d"), iter->first);
			pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord());
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strTemp);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);
			pItem->SetItemData(iter->first);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(iter->second);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);
			pItem->SetItemData(iter->first);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);
			pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(NULL != pButton)
			{
				pButton->SetCaption(_T("Search"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("검색"));
				}
				pButton->SetSize(CSize(55, 0));
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);
			pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(NULL != pButton)
			{
				pButton->SetCaption(_T("Draw"));
				if (KmzSystem::GetLanguage() == KEMKorea) {
					pButton->SetCaption(_T("그리기"));
				}
				pButton->SetSize(CSize(45, 0));
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("-")));
			pItem->SetAlignment(DT_CENTER);
			pItem->SetEditable (FALSE);

			nIndex++;
			++iter;
		}

		m_ctrlReport.Populate();
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


void KZoneODGroupGenerationDlg::OnItemButtonClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	try
	{
		XTP_NM_REPORTITEMCONTROL* pItemNotify    = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
		if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
			return;

		CXTPReportRecordItem*     pItem          = pItemNotify->pItem;
		CXTPReportRecord*         pRecord        = pItemNotify->pRow->GetRecord();
		m_pSelectRecord         = pRecord;
		CXTPReportRecordItem*     pItemID        = pRecord->GetItem(_0_COLUMN_ID);
		int nCodeID        = (int)pItemID->GetItemData();

		if(_3_COLUMN_AREACODE == pItem->GetIndex())
		{
			pItemNotify->pRow->SetSelected(TRUE);	

			std::set<Integer>   setSelectedZone;
			KImChampMultiZoneFind oZoneFindDlg(m_pTarget);
			oZoneFindDlg.SetUseTransitInclude(false);
			
			if (oZoneFindDlg.DoModal() == IDOK)
			{
				oZoneFindDlg.GetSelectedID(setSelectedZone);
				
				UpdateReport(pRecord, setSelectedZone);

				AutoType iter = m_mapCodePolygonInfo.find(nCodeID);
				if (iter != m_mapCodePolygonInfo.end())
				{
					m_mapCodePolygonInfo.erase(iter);
				}
			}

			ClearAllMap();
		}
		else if(_4_COLUMN_POLYGON == pItem->GetIndex())
		{
			pItemNotify->pRow->SetSelected(TRUE);	

			ClearAllMap();

 			AutoType iter = m_mapCodePolygonInfo.find(nCodeID);
			if (iter != m_mapCodePolygonInfo.end())
			{
				TCodeNodePolygon &oTCodeNodePolygon = iter->second;

				m_pMapView->TxDrawCompositeNode(oTCodeNodePolygon.vecDrawNodeInfo, false,  true);

				if (oTCodeNodePolygon.vecCoordinate.size() >= 2)
					m_pMapView->TargetRegionEdit(nCodeID, oTCodeNodePolygon.vecCoordinate, this);
				else
					m_pMapView->TargetRegionInsert(nCodeID, this);
			}
			else
			{
				m_pMapView->TargetRegionInsert(nCodeID, this);
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();		
	}
}


void KZoneODGroupGenerationDlg::ReleasePreZoneID(CString &a_strZoneIDs)
{
	try
	{
		if (a_strZoneIDs.CompareNoCase(_T("-")) != 0)
		{
			std::vector<CString> vecSelectID = KParseString::ParseString(a_strZoneIDs, _T(','));
			for (size_t i= 0; i< vecSelectID.size(); i++)
			{
				CString &strID   = vecSelectID[i];
				Integer nxReleaseZoneID = _ttoi64(strID);

				AutoType itFind = m_mapZoneIdCode.find(nxReleaseZoneID);
				if (itFind != m_mapZoneIdCode.end())
				{
					m_mapZoneIdCode.erase(itFind);
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::ConvertCodeZoneSet(std::map<Integer, int> &a_mapZoneIdCode, std::map<int, TCodeZoneID> &a_mapCodeZoneIdSet)
{
	a_mapCodeZoneIdSet.clear();

	try
	{
		AutoType iter  = a_mapZoneIdCode.begin();
		AutoType itEnd = a_mapZoneIdCode.end();

		while (iter != itEnd)
		{
			Integer nxZoneID = iter->first;
			int     nCodeID  = iter->second;

			AutoType itFind = a_mapCodeZoneIdSet.find(nCodeID);
			if (itFind != a_mapCodeZoneIdSet.end())
			{
				TCodeZoneID &oTCodeZoneID = itFind->second;
				std::set<Integer> &setZoneID = oTCodeZoneID.setZoneID;
				CString           &strZoneIDs =oTCodeZoneID.strZoneIDs;

				setZoneID.insert(nxZoneID);
				strZoneIDs.AppendFormat(_T(",%I64d"), nxZoneID);
			}
			else
			{
				TCodeZoneID oTCodeZoneID;
				std::set<Integer> &setZoneID = oTCodeZoneID.setZoneID;
				CString           &strZoneIDs =oTCodeZoneID.strZoneIDs;

				setZoneID.insert(nxZoneID);
				strZoneIDs.Format(_T("%I64d"), nxZoneID);

				a_mapCodeZoneIdSet.insert(std::make_pair(nCodeID, oTCodeZoneID));
			}

			++iter;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::UpdateReportData(std::map<int, TCodeZoneID> &a_mapCodeZoneIdSet)
{
	try
	{
		CXTPReportRecords*    pRecords = m_ctrlReport.GetRecords();
		int nRecordCnt      = pRecords->GetCount();
		CXTPReportRecord*     pRecord = nullptr;
		CXTPReportRecordItem* pItem   = nullptr;

		for (int i= 0; i< nRecordCnt; i++)
		{
			pRecord = pRecords->GetAt(i);
			
			pItem   = pRecord->GetItem(_0_COLUMN_ID);
			int nCode       = (int)pItem->GetItemData();

			AutoType itFind = a_mapCodeZoneIdSet.find(nCode);
			if (itFind != a_mapCodeZoneIdSet.end())
			{
				TCodeZoneID &oTCodeZoneID = itFind->second;

				pItem   = pRecord->GetItem(_2_COLUMN_ZONE_ID);
				pItem->SetCaption(oTCodeZoneID.strZoneIDs);
				pItem->SetAlignment(DT_LEFT);

				pItem   = pRecord->GetItem(_5_COLUMN_COUNT);
				CString strZoneCount(_T(""));
				strZoneCount.Format(_T("%d"), (int)(oTCodeZoneID.setZoneID.size()));
				pItem->SetCaption(strZoneCount);
			}
			else
			{
				pItem   = pRecord->GetItem(_2_COLUMN_ZONE_ID);
				pItem->SetCaption(_T("-"));
				pItem->SetAlignment(DT_CENTER);

				pItem   = pRecord->GetItem(_5_COLUMN_COUNT);
				pItem->SetCaption(_T("-"));
			}
		}

		m_ctrlReport.RedrawControl();
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::ClearAllMap( void )
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


void KZoneODGroupGenerationDlg::TargetRegionFeedback( std::vector<Integer> a_vecSelection, std::vector<TCoordinate> a_vecCoordinate )
{
	try
	{
		std::set<Integer>          setSelectedZone;
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		    
		for (size_t i= 0; i< a_vecSelection.size(); i++)
		{
			Integer nxNodeID = a_vecSelection[i];

			if (m_setZoneID.find(nxNodeID) == m_setZoneID.end())
			{
				continue;
			}

			setSelectedZone.insert(nxNodeID);
			
			TDrawNodeInfo	oDrawNodeInfo;
			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), nxNodeID);

			oDrawNodeInfo.TNodeID			= nxNodeID;
			oDrawNodeInfo.TSize				= 7;
			oDrawNodeInfo.TSymbol	= 74;	
			oDrawNodeInfo.TColorRGB			= RGB(47,157,39);
			oDrawNodeInfo.TLabel			= strZoneID;

			vecDrawNodeInfo.push_back(oDrawNodeInfo);
		}
		m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  true);

		if (a_vecCoordinate.size() >= 2)
			m_pMapView->TargetRegionEdit(0, a_vecCoordinate, this);

		UpdateReport(m_pSelectRecord, setSelectedZone);

		TempCodePolygonInfo(m_pSelectRecord, a_vecCoordinate, vecDrawNodeInfo);
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


void KZoneODGroupGenerationDlg::UpdateReport(CXTPReportRecord* a_pSelectedRecord, std::set<Integer> &a_setSelectedZone)
{
	CWaitCursor cw;

	try
	{
		CXTPReportRecordItem* pItemID = a_pSelectedRecord->GetItem(_0_COLUMN_ID);
		int nCodeID    = (int)pItemID->GetItemData();

		CXTPReportRecordItem* pItem   = a_pSelectedRecord->GetItem(_2_COLUMN_ZONE_ID);
		CString strZoneIDs  = pItem->GetCaption();
		// Code에 할당된 기존 ZoneId 해제
		ReleasePreZoneID(strZoneIDs);

		AutoType iterB  = m_mapCodePolygonInfo.begin();
		AutoType itEndB = m_mapCodePolygonInfo.end();

		while (iterB != itEndB)
		{
			TCodeNodePolygon           &oTCodeNodePolygon = iterB->second;
			std::vector<TDrawNodeInfo> &vecDrawNodeInfo   = oTCodeNodePolygon.vecDrawNodeInfo;

			size_t nxCount = vecDrawNodeInfo.size();

			for (size_t i= 0; i< nxCount; i++)
			{
				Integer &nxNodeID = vecDrawNodeInfo[i].TNodeID;
				
				if (a_setSelectedZone.find(nxNodeID) != a_setSelectedZone.end())
				{
					nxNodeID = -1;
				}
			}
			++iterB;
		}

		AutoType iter  = a_setSelectedZone.begin();
		AutoType itEnd = a_setSelectedZone.end();

		while (iter != itEnd)
		{
			AutoType itFind = m_mapZoneIdCode.find(*iter);
			if (itFind != m_mapZoneIdCode.end())
				itFind->second = nCodeID;
			else
				m_mapZoneIdCode.insert(std::make_pair(*iter, nCodeID));

			++iter;
		}

		std::map<int, TCodeZoneID> mapCodeZoneIdSet;
		ConvertCodeZoneSet(m_mapZoneIdCode, mapCodeZoneIdSet);
		UpdateReportData  (mapCodeZoneIdSet);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::TempCodePolygonInfo(CXTPReportRecord* a_pSelectedRecord, std::vector<TCoordinate> &a_vecCoordinate, std::vector<TDrawNodeInfo> &a_vecDrawNodeInfo)
{
	try
	{
		CXTPReportRecordItem* pItemID   = a_pSelectedRecord->GetItem(_0_COLUMN_ID);
		int nCodeID    = (int)pItemID->GetItemData();

		AutoType iter = m_mapCodePolygonInfo.find(nCodeID);
		if (iter != m_mapCodePolygonInfo.end())
		{
			TCodeNodePolygon &oTCodeNodePolygon = iter->second;

			std::vector<TCoordinate> &vecTempCoordinate = oTCodeNodePolygon.vecCoordinate;
			vecTempCoordinate.clear();
			size_t nxCount = a_vecCoordinate.size();
			vecTempCoordinate.reserve(nxCount);
			vecTempCoordinate.assign(a_vecCoordinate.begin(), a_vecCoordinate.end());

			std::vector<TDrawNodeInfo> &vecTempDrawNodeInfo = oTCodeNodePolygon.vecDrawNodeInfo;
			vecTempDrawNodeInfo.clear();
			nxCount = a_vecDrawNodeInfo.size();
			vecTempDrawNodeInfo.reserve(nxCount);
			vecTempDrawNodeInfo.assign(a_vecDrawNodeInfo.begin(), a_vecDrawNodeInfo.end());
		}
		else
		{
			TCodeNodePolygon oTCodeNodePolygon;

			std::vector<TCoordinate> &vecTempCoordinate = oTCodeNodePolygon.vecCoordinate;
			vecTempCoordinate.clear();
			size_t nxCount = a_vecCoordinate.size();
			vecTempCoordinate.reserve(nxCount);
			vecTempCoordinate.assign(a_vecCoordinate.begin(), a_vecCoordinate.end());

			std::vector<TDrawNodeInfo> &vecTempDrawNodeInfo = oTCodeNodePolygon.vecDrawNodeInfo;
			vecTempDrawNodeInfo.clear();
			nxCount = a_vecDrawNodeInfo.size();
			vecTempDrawNodeInfo.reserve(nxCount);
			vecTempDrawNodeInfo.assign(a_vecDrawNodeInfo.begin(), a_vecDrawNodeInfo.end());

			m_mapCodePolygonInfo.insert(std::make_pair(nCodeID, oTCodeNodePolygon));
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KZoneODGroupGenerationDlg::OnBnClickedRadio(UINT nID)
{
	ControlRadio();
}


void KZoneODGroupGenerationDlg::ControlRadio()
{
	if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
	{
		GetDlgItem(IDC_EDIT_COLUMN_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_COLUMN_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	}
}


void KZoneODGroupGenerationDlg::OnCbnSelchangeCboGroupName()
{
	try
	{
		LoadReportData();
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


void KZoneODGroupGenerationDlg::OnBnClickedButtonModify()
{
	try
	{
		KCodeManager*      pCodeManager = m_pTarget->CodeManager();
		KCodeManagementDlg dlg;

		dlg.SetTarget(m_pTarget);
		dlg.CodeManager(pCodeManager);
		if (dlg.DoModal() == IDOK)
		{
			LoadCodeGroup();
			LoadReportData();
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

void KZoneODGroupGenerationDlg::OnBnClickedCancel()
{
	ClearAllMap();
	KResizeDialogEx::OnCancel();
}


unsigned __stdcall KZoneODGroupGenerationDlg::ThreadCaller( void* p )
{
	try
	{
		QBicSimpleProgressParameter* pParameter = (QBicSimpleProgressParameter*)p;
		QBicSimpleProgress* pProgressWindow = pParameter->GetProgressWindow();

		KZoneODGroupGenerationDlg* pGroupGeneration = (KZoneODGroupGenerationDlg*)pParameter->GetParameter();
		pGroupGeneration->Run();
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}

	return 0;
}


void KZoneODGroupGenerationDlg::Run()
{
	try
	{
		int         nCurSel       = m_cboGroupName.GetCurSel();
		KCodeGroup* pCodeGroup    = (KCodeGroup*)m_cboGroupName.GetItemData(nCurSel);
		int         nCodeGroupKey = pCodeGroup->CodeGroupKey();

		KIOColumn*  pOutColumn    = nullptr;
		CString     strOutColumnName(_T(""));
		KIOTable*   pTable        = m_pTarget->Tables()->FindTable(TABLE_ZONE);
		bool        bNewColumn(true);

		if (GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1)
		{
			GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
			strOutColumnName.Trim();
			strOutColumnName.Replace(_T(" "), _T(""));

			const KIOColumns* pColumCollection = pTable->Columns();
			int               nColCnt          = pColumCollection->ColumnCount();
			for (int i= 0; i< nColCnt; i++)
			{
				KIOColumn*    pIOColumn        = pColumCollection->GetColumn(i);
				CString       strCaption       = pIOColumn->Caption();

				if (strOutColumnName.CompareNoCase(strCaption) == 0)
				{
					bNewColumn = false;
					pOutColumn = pIOColumn;
					break;
				}
			}

			if ( bNewColumn )
			{
				pOutColumn = AddZoneCodeColumn(nCodeGroupKey, strOutColumnName);            
			}
		}
		else
		{
			bNewColumn = false;

			int nCur   = m_cboOutColumnName.GetCurSel();
			pOutColumn = (KIOColumn*)m_cboOutColumnName.GetItemData(nCur);
		}

		if (false == bNewColumn)
		{
			KIOColumn   oIOColumnUpdate = *pOutColumn;

			if (pOutColumn->CodeGroup() != nCodeGroupKey) // 변경이 일어 날시
			{
				try
				{
					//pTable->BeginTransaction();

					oIOColumnUpdate.CodeGroup(nCodeGroupKey);

					pTable->UpdateColumn(oIOColumnUpdate);
					//pTable->Commit();
				}
				catch (...)
				{
					TxLogDebugException();	
					//pTable->Rollback();
				}
			}
		}

		KTableEditFieldInfo::DefaultCodeFiledData(pTable, *pOutColumn);
		UpdateZoneTable(pOutColumn, m_mapZoneIdCode);    
// 		UpdateResultTable(pOutColumn, nCodeGroupKey);

		m_nRunResult = 1;
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


void KZoneODGroupGenerationDlg::OnBnClickedOk()
{
	if (InputCheck() == false)
		return;

	try
	{
		m_nRunResult = 0;

		// 2019.06.11 Progress 다이얼로그 언어설정 관련 수정
		int nLang = 1;
		if (KmzSystem::GetLanguage() == KEMKorea)
		{
			nLang = 0;
		}

		QBicSimpleProgressThread::ExecuteThread(ThreadCaller, this, false, nLang); 

		if (m_nRunResult != 1)
		{
			AfxMessageBox(_T("수행 중 오류가 발생하였습니다."));
			return;
		}
		try
		{
			CWaitCursor cw;

			KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);
			pTable->Notify();

			ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_ZONE);            
		}
		catch (...)
		{
			TxLogDebugException();
		}

		ClearAllMap();

		KResizeDialogEx::OnOK();

		try
		{
			if (AfxMessageBox(_T("O/D 그룹 생성을 실행하시겠습니까?"), MB_YESNO) == IDYES)
			{
				CMainFrameWnd* pMainFrameWnd = ImChampFrameWindow::GetMainFrameWnd();	
				pMainFrameWnd->OnOdGroupGenerationPart2();
			}
		}
		catch (...)
		{
			TxLogDebugException();	
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


bool KZoneODGroupGenerationDlg::InputCheck()
{
	try
	{
		if (m_mapZoneIdCode.size()< 1)
		{
			AfxMessageBox(_T("구역이 설정되지 않은 코드번호가 존재합니다.\n검색 혹은 그리기로 설정해 주세요."));
			return false;
		}

		if ( m_cboGroupName.GetCurSel() < 0)
		{
			AfxMessageBox(_T("코드 그룹 명칭을 선택해 주세요."));
			GetDlgItem(IDC_CBO_GROUP_NAME)->SetFocus();
			return false;
		}

		CString strOutColumnName(_T(""));
		if ( GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2) == IDC_RADIO1 )
		{
			GetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);
			strOutColumnName.Trim();
			SetDlgItemText(IDC_EDIT_COLUMN_NAME, strOutColumnName);

			if (strOutColumnName.GetLength() == 0)
			{
				AfxMessageBox(_T("컬럼명을 입력해 주세요."));
				GetDlgItem(IDC_EDIT_COLUMN_NAME)->SetFocus();
				return false;
			}

			KIOTable*         pTable           = m_pTarget->Tables()->FindTable(TABLE_ZONE);
			const KIOColumns* pColumCollection = pTable->Columns();
			int               nColCnt          = pColumCollection->ColumnCount();
			for (int i= 0; i< nColCnt; i++)
			{
				KIOColumn*    pIOColumn        = pColumCollection->GetColumn(i);
				CString       strCaption       = pIOColumn->Caption();

				if (strOutColumnName.CompareNoCase(strCaption) == 0)
				{
					CString strMsg(_T(""));
					strMsg.Format(_T("'%s' 컬럼이 이미 존재합니다.\r\n덮어쓰시겠습니까?"), strCaption);
					if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
					{
						GetDlgItem(IDC_EDIT_COLUMN_NAME)->SetFocus();
						return false; 
					}
				}
			}
		}
		else
		{
			int nIndex = m_cboOutColumnName.GetCurSel();
			if (nIndex < 0)
			{
				AfxMessageBox(_T("컬럼을 선택해 주세요."));
				m_cboOutColumnName.SetFocus();
				return false;
			}
		}

		return true;
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}
	return false;
}


KIOColumn* KZoneODGroupGenerationDlg::AddZoneCodeColumn( int a_nCodeGroup, CString a_strColumnName )
{
	KIOColumn* pColumn = nullptr;
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_ZONE);

		const KIOColumns* pColumCollection = pTable->Columns();

		int nMaxDisplayCount(1);
		int nCount = pColumCollection->ColumnCount();

		if (0 == nCount)
		{
			nMaxDisplayCount = 1;
		}
		else
		{
			for(int i = 0; i < nCount; ++i)
			{
				KIOColumn* pColumn = pColumCollection->GetColumn(i);

				if(nMaxDisplayCount < pColumn->DisplayOrder())
				{
					nMaxDisplayCount = pColumn->DisplayOrder();
				}
			}
		}

		KIOColumn oColumn;
		oColumn.Name(pTable->GetUsableColumnName());
		oColumn.Caption(a_strColumnName);
		oColumn.CodeGroup(a_nCodeGroup);
		oColumn.CodeType(KEMIOCodeTypeIsSingleCode);
		oColumn.ColumnType(KEMIOColumnTypeUserDefine);
		oColumn.DataType(KEMIODataTypeInteger);
		oColumn.DisplayOrder(nMaxDisplayCount+1);
		oColumn.ColumnInfo(a_strColumnName);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);

		pColumn = pTable->AddColumn(oColumn, true);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw;
	}

	return pColumn;
}


void KZoneODGroupGenerationDlg::UpdateZoneTable( KIOColumn* a_pColumn, std::map<Integer, int>& ar_mapValue )
{
	try
	{
		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Update Zone Set %s = ? Where Zone_ID = ? "), a_pColumn->Name());

		KDBaseConPtr   spDBaseConnection  = m_pTarget->GetDBaseConnection();
		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strSQL);

		std::map<Integer, int>::iterator iter = ar_mapValue.begin();
		std::map<Integer, int>::iterator end  = ar_mapValue.end();

		while (iter != end)
		{
			Integer nxZoneID   = iter->first;
			int     nCodeValue = iter->second;

			spPrepareStatement->BindInt  (1, nCodeValue);
			spPrepareStatement->BindInt64(2, nxZoneID);            

			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
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
}


void KZoneODGroupGenerationDlg::UpdateResultTable(KIOColumn* a_pColumn, int a_nGroupCode)
{
// 	CString strDelSQL(_T(""));
// 	strDelSQL.AppendFormat(_T(" DELETE FROM Group_Generation_Zone WHERE Output_Column = '%s' "), a_pColumn->Name());
// 	CString strSelectSQL(_T(" SELECT Max(seq) FROM Group_Generation_Zone "));
// 
// 	KDBaseConPtr   spDBaseConnection  = m_pTarget->GetDBaseConnection();
// 
// 	try
// 	{
// 		spDBaseConnection->BeginTransaction();
// 
// 		int nMaxSeq(0);
// 
// 		KResultSetPtr spResultSet = spDBaseConnection->ExecuteQuery(strSelectSQL);
// 		if (spResultSet->Next())
// 			nMaxSeq = spResultSet->GetValueInt(0) + 1;
// 
// 		spDBaseConnection->ExecuteUpdate(strDelSQL);
// 
// 		CString strInsertSQL(_T(""));
// 		strInsertSQL.AppendFormat(_T(" Insert Into Group_Generation_Zone (seq, KCode_Group_Key, Output_Column) Values (%d, %d, '%s') "), nMaxSeq, a_nGroupCode, a_pColumn->Name());
// 		spDBaseConnection->ExecuteUpdate(strInsertSQL);
// 
// 		spDBaseConnection->Commit();
// 	}
// 	catch (...)
// 	{
// 		TxLogDebugException();
// 		spDBaseConnection->RollBack();
// 	}
}