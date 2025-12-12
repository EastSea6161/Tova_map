// AccessStopNodeEditDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AccessStopNodeEditDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "CustomReportRecordItem.h"
#include "MunsellColorCircle.h"
#include "TNodeStation.h"
#include "TDrawNodeInfo.h"
#include "StringChecker.h"


enum
{
	_0_COLUMN_CLASS = 0,
	_1_COLUMN_TIME,
	_2_COLUMN_COLOR,
	_3_COLUMN_SIZE
};


enum
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_NODEID,
	_2_COLUMN_MODE,
	_3_COLUMN_CLASSINZONE,
	_4_COLUMN_CLASSINNETWORK,
	_5_COLUMN_NOOFLINE
};


const int	  nNODEITEM_LENGTH	    = 50000;
const int	  nZONENODEITEM_LENGTH  = 30000;
const int	  nDATA_SIZE		    = 30;	
const int	  nINITCOMBOVALUE       = 0;

const int	  nBusCode				= 3;
const int	  nRailCode				= 4;
const int	  nAllCode				= 0;

const int	  nZoneSize				= 15;
const int	  nZoneCharacterIndex	= 81;
const int	  nStationCharIndex		= 81;

const int	  nZCLASSINDEX			= 0;

const TCHAR*  strBus				=_T("Highway_Transit");
const TCHAR*  strRail				=_T("Rail_Transit");

// KAccessStopNodeEditDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAccessStopNodeEditDlg, KResizeDialogEx)

KAccessStopNodeEditDlg::KAccessStopNodeEditDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAccessStopNodeEditDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
	, m_bClickedZone(false)
	, m_bClickedStation(false)
{

}

KAccessStopNodeEditDlg::~KAccessStopNodeEditDlg()
{
}

void KAccessStopNodeEditDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_CLASS, m_wndReportCtrlClass);
	DDX_Control(pDX, IDC_REPORT_NODE, m_wndReportCtrlNode);
	//DDX_Control(pDX, IDC_BUTTON_ZONE_COLOR, m_btnZoneColor);
	DDX_Control(pDX, IDC_COMBO1, m_cboZoneID);
	DDX_Control(pDX, IDC_COMBO6, m_cboMode);
}


BEGIN_MESSAGE_MAP(KAccessStopNodeEditDlg, KResizeDialogEx)
	
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT_NODE,	OnReportItemValueChangedNode)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT_CLASS,	OnReportItemValueChangedClass)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,	  IDC_REPORT_NODE,	OnReportCheckItemNode)
	ON_NOTIFY(NM_CLICK,                   IDC_REPORT_CLASS, OnReportItemClickClass)
	ON_NOTIFY(NM_DBLCLK,                  IDC_REPORT_NODE,  OnReportItemDoubleClickNode)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ZONE, IDC_RADIO_STOP, &KAccessStopNodeEditDlg::OnBnClickedRadioTimeMethod)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KAccessStopNodeEditDlg::OnCbnSelchangeComboZone)
	ON_BN_CLICKED(IDC_BUTTON_MAP, &KAccessStopNodeEditDlg::OnBnClickedButtonZoneMap)
	ON_BN_CLICKED(IDC_BUTTON1, &KAccessStopNodeEditDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON7, &KAccessStopNodeEditDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KAccessStopNodeEditDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_BUTTON6, &KAccessStopNodeEditDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON4, &KAccessStopNodeEditDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON5, &KAccessStopNodeEditDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDCANCEL, &KAccessStopNodeEditDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO6, &KAccessStopNodeEditDlg::OnCbnSelchangeComboClass)
END_MESSAGE_MAP()


// KAccessStopNodeEditDlg 메시지 처리기입니다.


BOOL KAccessStopNodeEditDlg::OnInitDialog()
{
	try
	{
		KResizeDialogEx::OnInitDialog();

		CheckRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP, IDC_RADIO_ZONE);
		

		KReportCtrlSetting::Default(m_wndReportCtrlClass, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlClass);
		KReportCtrlSetting::Default(m_wndReportCtrlNode, TRUE);
		KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlNode);

		KDBaseAccessibilityModify::GetClassInfo(m_pTarget, m_mapClassInfo, m_mapClassTime);
		KDBaseAccessibilityModify::GetZoneInfo(m_pTarget, m_mapZoneInfo);
		KDBaseAccessibilityModify::GetNodeLineInfo(m_pTarget, m_mapStationLineInfo, m_mapStationModeInfo);
		
		double dRadiousKm(0.0);
		KDBaseAccessibilityModify::GetZoneRadius(m_pTarget, dRadiousKm);
		ClassificationStationClass();

		m_dRadious = dRadiousKm * 1000;

		OnBnClickedRadioTimeMethod(IDC_RADIO_STOP);

		InitReportHeaderClass();
		UpdateReportClass();

		InitZoneComboBox();
		InitModeComboBox();

		ResizeComponent();

		CString strCaption;
		strCaption.Format(_T("::: %s"), m_pTarget->Caption());
		SetDlgItemText(IDC_STATIC_TARGETYEAR, strCaption);

		bool bUseAccessDB = false;
		KDBaseAccessibilityModify::CheckUsingAssessData(m_pTarget, bUseAccessDB);
		if (false == bUseAccessDB)
		{
			AfxMessageBox(_T("MulitiModal Model > Access Node Setting(Generate)을 실행하지 않았습니다."));
		}

		AutoType iter = m_mapZoneInfo.begin();
		AutoType end  = m_mapZoneInfo.end();
		while(iter != end )
		{
			KIDCaption oCaption;
			oCaption.TCaption = iter->second;

			CString strID(_T(""));
			strID.Format(_T("%I64d"), iter->first);
			oCaption.TID = strID;

			m_vecKIDZoneInfo.push_back(oCaption);
			iter++;
		}

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage()); 
	}
	catch(...)
	{
		KLogDebugException();
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAccessStopNodeEditDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{

}


void KAccessStopNodeEditDlg::MapSelectionFeedback( Integer a_nxNodeID )
{
	try
	{
		if (true == m_bClickedZone)
		{
			AutoType iter = m_mapZoneInfo.find(a_nxNodeID);
			AutoType end  = m_mapZoneInfo.end();
			if (iter == end )
			{
				return;
			}

			CString strZoneName = iter->second;

			if (_T("") == strZoneName)
			{
				strZoneName = _T("x");
			}

			CString strComboName;
			strComboName.Format(_T("%I64d(%s)"), a_nxNodeID, strZoneName);
			int nCur = m_cboZoneID.FindString(-1, strComboName);
			m_cboZoneID.SetCurSel(nCur);

			ClearNodeSymbol();
			SearchData();
		}
		else if (true == m_bClickedStation)
		{
			AddReportControlToSelectNode(a_nxNodeID);
			ClearNodeSymbol();
			ChangeDrawStopNodeSymbol();
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::MapSelectionNoneFeedback()
{

}


void KAccessStopNodeEditDlg::InitReportHeaderClass( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(_0_COLUMN_CLASS, _T("Class"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(_1_COLUMN_TIME, _T("Time"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(_2_COLUMN_COLOR, _T("Color"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(_3_COLUMN_SIZE, _T("Size"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::InitReportHeaderNode( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK, _T("Select"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_1_COLUMN_NODEID, _T("NodeID"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_2_COLUMN_MODE, _T("Type"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_3_COLUMN_CLASSINZONE, _T("Class \r\n in Zone"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		AutoType iter = m_mapClassInfo.begin();
		AutoType end  = m_mapClassInfo.end();

		pEditOptions->AddConstraint(_T("Z"), nZCLASSINDEX);
		while(iter != end)
		{
			CString strClassName = iter->first;
			int		nClassID	 = iter->second;
			pEditOptions->AddConstraint(strClassName, nClassID);
			iter++;
		}

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_4_COLUMN_CLASSINNETWORK, _T("Class \r\n in Network"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_5_COLUMN_NOOFLINE, _T("No.\r\n of Line"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::InitReportHeaderZone(void)
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK, _T("Select"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_1_COLUMN_NODEID, _T("ZoneID"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_2_COLUMN_MODE, _T("Type"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_3_COLUMN_CLASSINZONE, _T("Class \r\n in Zone"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		AutoType iter = m_mapClassInfo.begin();
		AutoType end  = m_mapClassInfo.end();

		pEditOptions->AddConstraint(_T("Z"), nZCLASSINDEX);
		while(iter != end)
		{
			CString strClassName = iter->first;
			int		nClassID	 = iter->second;
			pEditOptions->AddConstraint(strClassName, nClassID);
			iter++;
		}

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(_4_COLUMN_CLASSINNETWORK, _T("Class \r\n in Network"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::UpdateReportClass( void )
{
	try
	{
		m_wndReportCtrlClass.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		int nSize = 15;
		int nIdx = 0;
		KMunsellColor* pMunSellColor = new KMunsellColor(0, true);
		COLORREF    colorRef;


		AutoType iter = m_mapClassInfo.begin();
		AutoType end  = m_mapClassInfo.end();

		while(iter != end)
		{
			if (nSize < 0)
			{
				nSize = 1;
			}

			pRecord = m_wndReportCtrlClass.AddRecord(new CXTPReportRecord());

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (iter->first));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);


			double dClassTime = 0;
			AutoType iterFind = m_mapClassTime.find(iter->first);
			AutoType iterend = m_mapClassTime.end();
			if (iterFind != iterend)
			{
				dClassTime = iterFind->second;
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber (dClassTime, _T("%.1f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			colorRef = pMunSellColor->GetColor(nIdx);
			pItem->SetBackgroundColor(colorRef);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber ((double)nSize, _T("%.0f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetEditable(TRUE);

			nSize = (nSize - 1);
			nIdx++;
			iter++;
		}

		pRecord = m_wndReportCtrlClass.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Z")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);

		double dClassT = 0;
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("-")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		colorRef = pMunSellColor->GetColor(nIdx);
		pItem->SetBackgroundColor(colorRef);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber ((double)nSize, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetEditable(TRUE);

		m_wndReportCtrlClass.Populate();

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::OnReportItemClickClass( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (_2_COLUMN_COLOR == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());
			m_wndReportCtrlClass.RedrawControl();

			GetClassDisplayInfo();
			ClearNodeSymbol();
			ChangeDrawStopNodeSymbol();
		}
	}
}


void KAccessStopNodeEditDlg::InitZoneComboBox( void )
{
	try
	{
		m_cboZoneID.ResetContent();
		m_cboZoneID.Clear();
		m_cboZoneID.SetRedraw(FALSE);
		m_cboZoneID.InitStorage(nZONENODEITEM_LENGTH, nDATA_SIZE);

		int nZeroCur = m_cboZoneID.AddString(_T("-----Select-----"));

		AutoType iter = m_mapZoneInfo.begin();
		AutoType end  = m_mapZoneInfo.end();

		while(iter != end)
		{
			Integer nxZoneID	= iter->first;
			CString strZoneName = iter->second;

			if (_T("") == strZoneName)
			{
				strZoneName = _T("x");
			}

			CString strZoneCombo;
			strZoneCombo.Format(_T("%I64d(%s)"), nxZoneID, strZoneName);

			int nComboIndex = m_cboZoneID.AddString(strZoneCombo);
			m_cboZoneID.SetItemData(nComboIndex, (DWORD_PTR)nxZoneID);
			iter++;

		}

		m_cboZoneID.SetCurSel(nZeroCur);
		m_cboZoneID.SetRedraw(TRUE);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::OnCbnSelchangeComboZone()
{
	try
	{
		ClearNodeSymbol();
		ClearDrawBackGroundSymbol();

		int	nCurSel		=  m_cboZoneID.GetCurSel();
		if (nINITCOMBOVALUE == nCurSel)
		{
			return;
		}

		Integer nxZoneID = m_cboZoneID.GetItemData(nCurSel);
		m_pMapView->PanToFeature(TABLE_NODE, COLUMN_NODE_ID, nxZoneID);

		UpdateData(FALSE);

		SearchData();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::InitModeComboBox( void )
{
	try
	{
		int nZeroCur = m_cboMode.AddString(_T("All"));
		m_cboMode.SetItemData(nZeroCur, nAllCode);

		int nComboIndex = m_cboMode.AddString(_T("Bus"));
		m_cboMode.SetItemData(nComboIndex, nBusCode);

		nComboIndex = m_cboMode.AddString(_T("Rail"));
		m_cboMode.SetItemData(nComboIndex, nRailCode);

		m_cboMode.SetCurSel(nZeroCur);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::OnBnClickedButtonZoneMap()
{
	try
	{
		ClearDrawBackGroundSymbol();

		m_bClickedZone = true;

		COLORREF makerColor = m_btnZoneColor.GetColor();

		std::vector<TNodeSelectionInfo> vecTNodeStation;
		AutoType iter = m_mapZoneInfo.begin();
		AutoType end  = m_mapZoneInfo.end();
		while(iter != end)
		{
			TNodeSelectionInfo oTNodeStation;
			oTNodeStation.TNodeID = iter->first;
			oTNodeStation.TType   = 0;
			oTNodeStation.TCharacterIndex = nZoneCharacterIndex;
			oTNodeStation.TSize   = nZoneSize;
			oTNodeStation.TColorRGB = makerColor;
			CString strZoneID;
			strZoneID.Format(_T("%I64d"), iter->first);
			oTNodeStation.TLabel  = strZoneID;

			vecTNodeStation.push_back(oTNodeStation);
			iter++;
		}

		m_pMapView->MapNodeSelectAction(this, vecTNodeStation);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	

}


void KAccessStopNodeEditDlg::OnBnClickedButtonSearch()
{
	try
	{ 
		/*GetClassDisplayInfo();
		GetComboData();
		KDBaseAccessibilityModify::GetIncludeZoneNodeAceessInfo(m_pTarget, m_nxZoneID, m_nMode, m_mapAccessStopNodeInfo);
		UpdateReportNode();

		DrawZoneNodeSymbol();
		DrawStopNodeSymbol();*/

		SelectZoneNodeList();

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::GetClassDisplayInfo( void )
{
	m_mapClassDisplay.clear();

	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrlClass.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemNumber*  pItemNum      = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount ; i++)
		{
			pRecord					= pRecords->GetAt(i);
			pItem					= pRecord->GetItem(_0_COLUMN_CLASS);
			pColumn					= m_wndReportCtrlClass.GetColumns()->GetAt(_0_COLUMN_CLASS);
			CString	 strClassName	= pItem->GetCaption(pColumn);

			pItem					= pRecord->GetItem(_2_COLUMN_COLOR);
			COLORREF color			= pItem->GetBackgroundColor();

			pItemNum				= (CXTPReportRecordItemNumber*)(pRecord->GetItem(_3_COLUMN_SIZE));
			double dSize            = pItemNum->GetValue();

			TClassInfo oClassInfo;
			oClassInfo.clrClass = color;
			oClassInfo.dSize	= dSize;

			m_mapClassDisplay.insert(std::make_pair(strClassName, oClassInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}

}


bool KAccessStopNodeEditDlg::GetComboData( void )
{
	int nCurSel = m_cboZoneID.GetCurSel();
	if (nINITCOMBOVALUE == nCurSel)
	{
		//AfxMessageBox(_T("선택된 Zone이 없습니다."));
		return false;
	}
	else
	{
		m_nxZoneID	=  m_cboZoneID.GetItemData(nCurSel);

		AutoType iter = m_mapZoneInfo.find(m_nxZoneID);
		AutoType end  = m_mapZoneInfo.end();
		if (iter != end)
		{
			m_strZoneName = iter->second;
		}

	}

	nCurSel = m_cboMode.GetCurSel();
	m_nMode = m_cboMode.GetItemData(nCurSel);
	
	return true;
}


void KAccessStopNodeEditDlg::UpdateReportNode( void )
{
	try
	{
		m_wndReportCtrlNode.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = m_mapAccessStopNodeInfo.begin();
		AutoType end  = m_mapAccessStopNodeInfo.end();

		while(iter != end )
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlNode.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);

			TAccessStopNodeInfo oNodeInfo = iter->second;

			Integer nxNodeID = iter->first;
			CString strNodeID(_T(""));
			strNodeID.Format(_T("%I64d"), nxNodeID);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeID));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);

			CString strMode(_T(""));
			if (3 == oNodeInfo.nMode)
			{
				strMode = strBus;
			}
			else if ( 4 == oNodeInfo.nMode)
			{
				strMode = strRail;
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);

			int nClassIndex = nZCLASSINDEX;
			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			AutoType findClass = m_mapClassInfo.find(oNodeInfo.strClassInZone);
			AutoType endClass  = m_mapClassInfo.end();
			if (findClass != endClass)
			{
				nClassIndex = findClass->second;
			}
			pItem->SetItemData(nClassIndex);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oNodeInfo.strClassInNetwork));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);


			int nNoLine = oNodeInfo.nNoLine;
			CString strNoLine(_T(""));
			strNoLine.Format(_T("%d"), nNoLine);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNoLine));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(RGB(224,238,238));
			pItem->SetEditable(FALSE);

			iter++;
		}

		m_wndReportCtrlNode.Populate();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::ClearNodeSymbol( void )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();
		m_bClickedStation = false;
		m_bClickedZone	  = false;
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::SelectStopNodeSymbol( void )
{
	try
	{
		std::vector<TNodeSelectionInfo> vecTNodeStation;

		AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));
		TClassInfo oZClassInfo = iterZClass->second; 

		AutoType iterAllNode = m_mapStationModeInfo.begin();
		AutoType endAllNode  = m_mapStationModeInfo.end();
		while(iterAllNode != endAllNode)
		{
			Integer nxNodeID = iterAllNode->first;
			int		nMode	 = iterAllNode->second;

			TNodeSelectionInfo oNodeStation;
			//Mode에 따른 Station 그릭
			if (nMode == m_nMode || nAllCode == m_nMode)
			{
				CString strNodeID(_T(""));
				strNodeID.Format(_T("%I64d"), nxNodeID);

				//Station 기본 정보 담기
				oNodeStation.TNodeID			= nxNodeID;
				oNodeStation.TType				= 1;
				oNodeStation.TCharacterIndex	= 81;
				oNodeStation.TLabel				= strNodeID;

				std::set<Integer> setReportNode;
				GetReportControlData(setReportNode);

				AutoType iterSet = setReportNode.find(nxNodeID);
				AutoType endSet	 = setReportNode.end();
				if (iterSet == endSet)
				{
					oNodeStation.TColorRGB			= oZClassInfo.clrClass;
					oNodeStation.TSize				= oZClassInfo.dSize;
					vecTNodeStation.push_back(oNodeStation);
				}
			}
			iterAllNode++;
		}

		m_pMapView->MapNodeSelectAction(this, vecTNodeStation);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::DrawZoneNodeSymbol( void )
{
	try
	{
		std::vector<TDrawZoneAccess> vecZoneAccess;

		COLORREF makerColor = m_btnZoneColor.GetColor();

		TDrawZoneAccess oDrawZoneAccess;
		oDrawZoneAccess.TNodeID			 = m_nxZoneID;
		oDrawZoneAccess.TCharacterIndex  = nZoneCharacterIndex;
		oDrawZoneAccess.TSize			 = nZoneSize;
		oDrawZoneAccess.TColorRGB		 = makerColor;
		
		CString strZoneID;
		strZoneID.Format(_T("%I64d"), m_nxZoneID);
		oDrawZoneAccess.TLabel			 = strZoneID;
		oDrawZoneAccess.TRadius			 = m_dRadious;

		vecZoneAccess.push_back(oDrawZoneAccess);

		m_pMapView->DrawCompositeZoneAccess(vecZoneAccess, true);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::DrawStopNodeSymbol( void )
{
	std::vector<TDrawNodeInfo> vecDrawNodeInfo;

	try
	{
		AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));
		TClassInfo oZClassInfo = iterZClass->second; 

		AutoType iterAllNode = m_mapStationModeInfo.begin();
		AutoType endAllNode  = m_mapStationModeInfo.end();
		while(iterAllNode != endAllNode)
		{
			Integer nxNodeID = iterAllNode->first;
			int		nMode	 = iterAllNode->second;

			TDrawNodeInfo oNodeStation;
			//Mode에 따른 Station 그림
			if (nMode == m_nMode || nAllCode == m_nMode)
			{
				CString strNodeID(_T(""));
				strNodeID.Format(_T("%I64d"), nxNodeID);

				//Station 기본 정보 담기
				oNodeStation.TNodeID			= nxNodeID;
				oNodeStation.TCharacterIndex	= nStationCharIndex;
				oNodeStation.TLabel				= strNodeID;

				//Class 정보 찾기
				AutoType iterFindNode = m_mapAccessStopNodeInfo.find(nxNodeID);
				AutoType endFindNode  = m_mapAccessStopNodeInfo.end();
				if (iterFindNode != endFindNode)
				{
					TAccessStopNodeInfo oNodeInfo = iterFindNode->second;
					CString strClass = oNodeInfo.strClassInZone;

					AutoType iterFindClass = m_mapClassDisplay.find(strClass);
					AutoType endFindClass  = m_mapClassDisplay.end();

					if (iterFindClass != endFindClass)
					{
						TClassInfo oClassInfo = iterFindClass->second;
						oNodeStation.TColorRGB		= oClassInfo.clrClass;
						oNodeStation.TSize			= oClassInfo.dSize;
						vecDrawNodeInfo.push_back(oNodeStation);
					}
					else
					{
						oNodeStation.TColorRGB		= oZClassInfo.clrClass;
						oNodeStation.TSize			= oZClassInfo.dSize;
					}
				}
				else
				{
					oNodeStation.TColorRGB			= oZClassInfo.clrClass;
					oNodeStation.TSize				= oZClassInfo.dSize;
				}
			}

			iterAllNode++;
		}

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			 m_pMapView->DrawCompositeNode(vecDrawNodeInfo, true,  false);
		}
		else
		{
			 m_pMapView->DrawCompositeNode(vecDrawNodeInfo, false,  false);
		}
		
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}

}


void KAccessStopNodeEditDlg::ClearDrawBackGroundSymbol( void )
{
	try
	{
		m_wndReportCtrlNode.ResetContent(TRUE);
		m_pMapView->ClearCompositeThemes();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::OnBnClickedButtonAdd()
{
	try
	{
		m_bClickedStation = true;
		SelectStopNodeSymbol();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}	
}


void KAccessStopNodeEditDlg::OnBnClickedCheckLabel()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->ShowCompositeNodeLablel(true);
		}
		else
		{
			m_pMapView->ShowCompositeNodeLablel(false);
		}
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::AddReportControlToSelectNode(Integer a_nxStationID)
{
	try
	{
		int nTotalRowCount = m_wndReportCtrlNode.GetRows()->GetCount();

		CXTPReportRecord*		pRecord = m_wndReportCtrlNode.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		CString strNodeID(_T(""));
		strNodeID.Format(_T("%I64d"), a_nxStationID);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeID));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);

		int nMode = 3;
		AutoType FindMode = m_mapStationModeInfo.find(a_nxStationID);
		AutoType EndMode  = m_mapStationModeInfo.end();
		if (FindMode != EndMode)
		{
			nMode = FindMode->second;
		}

		CString strMode(_T(""));
		if (3 == nMode)
		{
			strMode = _T("Bus");
		}
		else if ( 4 == nMode)
		{
			strMode = _T("Subway");
		}
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);


		pItem = pRecord->AddItem(new CCustomReportRecordItem);
		pItem->SetItemData(nZCLASSINDEX);
		

		CString strClassInNetwork = _T("Z");
		int nLineCount(0);

		AutoType FindCNetwork = m_mapStationLineInfo.find(a_nxStationID);
		AutoType EndCNetwork  = m_mapStationLineInfo.end();
		if (FindCNetwork != EndCNetwork)
		{
			std::set<Integer> setLineInfo = FindCNetwork->second;
			nLineCount = setLineInfo.size();

			AutoType iterCinNet = m_mapClassInNetwork.begin();
			AutoType endCinNet	= m_mapClassInNetwork.end();
			while(iterCinNet != endCinNet)
			{
				if (nLineCount > iterCinNet->second)
				{
					strClassInNetwork = iterCinNet->first;
					break;
				}

				iterCinNet++;
			}
		}

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strClassInNetwork));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);

		CString strNoofLine = _T("");
		strNoofLine.Format(_T("%d"), nLineCount);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNoofLine));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(RGB(224,238,238));
		pItem->SetEditable(FALSE);

		m_wndReportCtrlNode.Populate();

		nTotalRowCount = m_wndReportCtrlNode.GetRows()->GetCount();
		CXTPReportRow* pRow = m_wndReportCtrlNode.GetRows()->GetAt(nTotalRowCount - 1);

		XTP_REPORTRECORDITEM_ARGS args(&m_wndReportCtrlNode, pRow, m_wndReportCtrlNode.GetColumns()->GetAt(_2_COLUMN_MODE));
		m_wndReportCtrlNode.EditItem(&args);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	

}


void KAccessStopNodeEditDlg::ClassificationStationClass( void )
{
	try
	{
		m_nMaxCount = 0;

		AutoType iter = m_mapStationLineInfo.begin();
		AutoType end  = m_mapStationLineInfo.end();
		while(iter != end)
		{
			std::set<Integer> setInfo = iter->second;

			int nCount = setInfo.size();

			if (nCount > m_nMaxCount)
			{
				m_nMaxCount = nCount;
			}

			iter++;
		}

		m_mapClassInNetwork.clear();

		int nClassCount = m_mapClassInfo.size();
		int nCount = m_nMaxCount / nClassCount;

		int nValue = m_nMaxCount;
		AutoType iterClass = m_mapClassInfo.begin();
		AutoType endClass  = m_mapClassInfo.end();
		while(iterClass != endClass)
		{
			CString strClass = iterClass->first;
			nValue = nValue - nCount;
			if (1 >= nValue)
			{
				nValue = 0;
			}
			m_mapClassInNetwork.insert(std::make_pair(strClass, nValue));
			
			iterClass++;
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::OnBnClickedButtonDelete()
{
	try
	{
		std::vector<CXTPReportRecord*> vecRecrod;
		CXTPReportRecords*           pRecords      = nullptr;
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pExtItemCheck = nullptr;

		pRecords			= m_wndReportCtrlNode.GetRecords();
		int nRecordCount	= pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pExtItemCheck    = pRecord->GetItem(_0_COLUMN_CHECK);
			int nCostMethod  = 0;
			if( TRUE == pExtItemCheck->IsChecked() )
			{
				vecRecrod.push_back(pRecord);
			}
		}

		int nvecCount = vecRecrod.size();
		for (int ivec = 0; ivec < nvecCount; ivec++)
		{
			pRecord = vecRecrod[ivec];
			m_wndReportCtrlNode.GetRecords()->RemoveRecord(pRecord);
			m_wndReportCtrlNode.Populate();
			pRecord = nullptr;
		}

		if (nvecCount == 0)
		{
			AfxMessageBox(_T("삭제될 데이터가 없습니다."));
		}

		ClearNodeSymbol();
		ChangeDrawStopNodeSymbol();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}

}


void KAccessStopNodeEditDlg::OnBnClickedButtonApply()
{
	try
	{
		std::vector<TAccessStopNodeInfo> vecAccessStopNodeResult;
		GetNodeApplyData(vecAccessStopNodeResult);
		KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtStation(m_pTarget, m_nxZoneID, m_strZoneName, m_nMode, vecAccessStopNodeResult);
		AfxMessageBox(_T(" > Complete "));
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T(" > Error"));
	}
	catch(...)
	{
		KLogDebugException();
		AfxMessageBox(_T(" > Error"));
	}
}


void KAccessStopNodeEditDlg::GetNodeApplyData(std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeResult)
{
	a_vecAccessStopNodeResult.clear();

	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			TAccessStopNodeInfo oAccessNodeInfo;

			pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_NODEID);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_2_COLUMN_MODE);
			CString strMode	= pItemText->GetValue();
			int nMode(0);

			if (strBus == strMode)
			{
				nMode = nBusCode;
			}
			else
			{
				nMode = nRailCode;
			}

			pItem = pRecord->GetItem(_3_COLUMN_CLASSINZONE);
			int nClassIndex = pItem->GetItemData();
			CString strClassInZone(_T("Z"));

			AutoType iter = m_mapClassInfo.begin();
			AutoType end  = m_mapClassInfo.end();
			while(iter != end )
			{
				if (nClassIndex == iter->second )
				{
					strClassInZone = iter->first;
					break;
				}
				iter++;
			}

			pItemText					= (CXTPReportRecordItemText*)pRecord->GetItem(_4_COLUMN_CLASSINNETWORK);
			CString strClassInNetwork	= pItemText->GetValue();


			pItemText			= (CXTPReportRecordItemText*)pRecord->GetItem(_5_COLUMN_NOOFLINE);
			CString strNoofLine = pItemText->GetValue();
			int nNoofLine		= _ttoi(strNoofLine);

			oAccessNodeInfo.nxNodeID			= nxNodeID;
			oAccessNodeInfo.nMode				= nMode;
			oAccessNodeInfo.strClassInZone		= strClassInZone;
			oAccessNodeInfo.strClassInNetwork	= strClassInNetwork;
			oAccessNodeInfo.nNoLine				= nNoofLine;
			a_vecAccessStopNodeResult.push_back(oAccessNodeInfo);

		}

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::OnBnClickedButtonClear()
{
	try
	{
		ClearDrawBackGroundSymbol();
		ClearNodeSymbol();

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::OnBnClickedCancel()
{
	ClearDrawBackGroundSymbol();
	ClearNodeSymbol();
	KResizeDialogEx::OnCancel();
}


void KAccessStopNodeEditDlg::ResizeComponent( void )
{
	//SetResize(IDC_SEPRATOR5,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR4,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_CLASS,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON1,					SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR7,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_NODE,				SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON7,					SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON6,					SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_SEPRATOR3,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON5,					SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,					SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,						SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KAccessStopNodeEditDlg::OnCbnSelchangeComboClass()
{
	try
	{
		ClearDrawBackGroundSymbol();
		SearchData();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::SearchData( void )
{
	try
	{
		GetClassDisplayInfo();
		if (false == GetComboData())
		{
			return;
		}
		KDBaseAccessibilityModify::GetIncludeNodeAceessInfo(m_pTarget, m_nxZoneID, m_nMode, m_mapAccessStopNodeInfo);
		UpdateReportNode();

		DrawZoneNodeSymbol();
		DrawStopNodeSymbol();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}


void KAccessStopNodeEditDlg::ChangeDrawStopNodeSymbol( void )
{
	try
	{
		std::vector<TAccessStopNodeInfo> vecAccessStopNodeResult;
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;

		m_wndReportCtrlNode.Populate();

		GetNodeApplyData(vecAccessStopNodeResult);

		int nCount = vecAccessStopNodeResult.size();
		for (int i = 0; i < nCount; i++)
		{
			TAccessStopNodeInfo oNodeInfo = vecAccessStopNodeResult[i];
			TDrawNodeInfo oNodeStation;

			oNodeStation.TNodeID = oNodeInfo.nxNodeID;

			CString strLable(_T(""));
			strLable.Format(_T("%I64d"), oNodeInfo.nxNodeID);

			oNodeStation.TLabel				= strLable;
			oNodeStation.TCharacterIndex	= nStationCharIndex;

			AutoType iterFindClass = m_mapClassDisplay.find(oNodeInfo.strClassInZone);
			AutoType endFindClass  = m_mapClassDisplay.end();

			if (iterFindClass != endFindClass)
			{
				TClassInfo oClassInfo		= iterFindClass->second;
				oNodeStation.TColorRGB		= oClassInfo.clrClass;
				oNodeStation.TSize			= oClassInfo.dSize;
				vecDrawNodeInfo.push_back(oNodeStation);
			}
		}

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->DrawCompositeNode(vecDrawNodeInfo, true,  false);
		}
		else
		{
			m_pMapView->DrawCompositeNode(vecDrawNodeInfo, false,  false);
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
	
}


void KAccessStopNodeEditDlg::OnReportItemValueChangedNode( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _3_COLUMN_CLASSINZONE)
		{
			ClearNodeSymbol();
			ChangeDrawStopNodeSymbol();
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::OnReportCheckItemNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _0_COLUMN_CHECK)
		{
			if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(_0_COLUMN_CHECK)->IsChecked() )
			{
				CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_NODEID);
				CString strNodeID = pItemText->GetValue();
				Integer nxNodeID = _ttoi64(strNodeID);

				m_pMapView->PanToFeature(TABLE_NODE, COLUMN_NODE_ID, nxNodeID);
			}
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::OnReportItemValueChangedClass( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _2_COLUMN_COLOR || nIndex == _3_COLUMN_SIZE)
		{
			GetClassDisplayInfo();
			ClearNodeSymbol();
			ChangeDrawStopNodeSymbol();
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::OnReportItemDoubleClickNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == _1_COLUMN_NODEID)
		{
			CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(_1_COLUMN_NODEID);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);

			m_pMapView->PanToFeature(TABLE_NODE, COLUMN_NODE_ID, nxNodeID);
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
	
}


void KAccessStopNodeEditDlg::ActivatedFrameWndNotifyProcess()
{

}


void KAccessStopNodeEditDlg::DeActivatedFrameWndNotifyProcess()
{

}


BOOL KAccessStopNodeEditDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if(VK_ESCAPE == pMsg->wParam )
			return TRUE;

		if(VK_RETURN == pMsg->wParam)
		{
			if (pMsg->hwnd == GetDlgItem(IDC_COMBO1)->m_hWnd)
			{
				GetComboTextData();
				return TRUE;
			}
		}
	}

	return KResizeDialogEx::PreTranslateMessage(pMsg);

}

void KAccessStopNodeEditDlg::GetComboTextData( void )
{
	UpdateData(TRUE);
	CString strComboText;

	m_cboZoneID.GetWindowTextW(strComboText);

	if (false == KStringChecker::IsUnSignedInteger(strComboText) )
	{
		AfxMessageBox(_T("입력값이 잘못되었습니다."));
		return;
	}

	Integer nxComboZoneID = _ttoi64(strComboText);

	AutoType iter = m_mapZoneInfo.find(nxComboZoneID);
	AutoType end  = m_mapZoneInfo.end();
	if (iter == end )
	{
		AfxMessageBox(_T("입력한 ZoneID가 없습니다."));
		return;
	}

	CString strZoneName = iter->second;
	if (_T("") == strZoneName)
	{
		strZoneName = _T("x");
	}

	CString strComboName;
	strComboName.Format(_T("%I64d(%s)"), nxComboZoneID, strZoneName);
	int nCur = m_cboZoneID.FindString(-1, strComboName);
	m_cboZoneID.SetCurSel(nCur);

	ClearNodeSymbol();
	SearchData();
}


void KAccessStopNodeEditDlg::GetReportControlData( std::set<Integer>& a_setNoedID )
{
	a_setNoedID.clear();

	CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
	CXTPReportRecord*            pRecord       = nullptr;
	CXTPReportRecordItemText*    pItemText	   = nullptr;

	int nRecordCount = pRecords->GetCount();
	for (int i = 0; i <nRecordCount; i++)
	{
		TAccessStopNodeInfo oAccessNodeInfo;

		pRecord = pRecords->GetAt(i);
		pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(_1_COLUMN_NODEID);
		CString strNodeID = pItemText->GetValue();
		Integer nxNodeID = _ttoi64(strNodeID);

		a_setNoedID.insert(nxNodeID);
	}
}

void KAccessStopNodeEditDlg::OnBnClickedRadioTimeMethod( UINT nID )
{
	try
	{
		m_wndReportCtrlNode.ResetContent();
		m_wndReportCtrlNode.GetColumns()->Clear();
		m_wndReportCtrlNode.Populate();

		ClearDrawBackGroundSymbol();
		ClearNodeSymbol();

		if (nID == IDC_RADIO_ZONE)
		{
			InitReportHeaderZone();
		}
		else
		{
			InitReportHeaderNode();
		}
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}

void KAccessStopNodeEditDlg::ModelessOKFeedback( UINT nDialogID )
{

}

void KAccessStopNodeEditDlg::SelectZoneNodeList()
{
	try
	{
		CString strSelect(_T(""));
		GetDlgItemText(IDC_EDIT_ID, strSelect);

		m_spImTasIDFind = KImTasIDFindPtr(new KImTasIDFind(this));
		m_spImTasIDFind->SetModelessReceiver(this);
		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);

		if (nChecked == IDC_RADIO_ZONE)
		{
			m_spImTasIDFind->SetIDCaption(m_vecKIDZoneInfo);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneInfo);
		}
		else
		{

		}
		
		m_spImTasIDFind->Create(KImTasIDFind::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);

	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}

void KAccessStopNodeEditDlg::SetStopNodeInfo()
{
	try
	{
		
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		KLogDebugException();
	}
}
