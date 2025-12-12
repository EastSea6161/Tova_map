// AccessiblityModifyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AccessiblityModifyDlg.h"
#include "afxdialogex.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "IncludeIOTable.h"
#include "MapView.h"
#include "NodeSelectionInfo.h"
#include "PopupMessenger.h"
#include "SCMapAnalysis.h"
#include "CustomReportRecordItem.h"

const int	  nNODEITEM_LENGTH	    = 50000;
const int	  nZONENODEITEM_LENGTH  = 30000;
const int	  nDATA_SIZE		    = 30;	
const int	  nINITCOMBOVALUE       = 0;


// KAccessiblityModifyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KAccessiblityModifyDlg, KResizeDialogEx)

KAccessiblityModifyDlg::KAccessiblityModifyDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAccessiblityModifyDlg::IDD, pParent),
	  m_pTarget(a_pTarget)
{

}

KAccessiblityModifyDlg::~KAccessiblityModifyDlg()
{
}

void KAccessiblityModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
	DDX_Control(pDX, IDC_COMBO1, m_cboZoneID);
	DDX_Control(pDX, IDC_COMBO6, m_cboStationID);
	DDX_Control(pDX, IDC_COMBO8, m_cboClass);
	DDX_Control(pDX, IDC_BUTTON_ZONE_COLOR, m_btnZoneColor);
	DDX_Control(pDX, IDC_BUTTON_NODE_COLOR, m_btnStationColor);
}


BEGIN_MESSAGE_MAP(KAccessiblityModifyDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &KAccessiblityModifyDlg::OnBnClickedButtonZone)
	ON_BN_CLICKED(IDC_BUTTON6, &KAccessiblityModifyDlg::OnBnClickedButtonStation)
	ON_BN_CLICKED(IDC_BUTTON1, &KAccessiblityModifyDlg::OnBnClickedButtonSearch)
	ON_CBN_SELCHANGE(IDC_COMBO1, &KAccessiblityModifyDlg::OnCbnSelchangeComboZone)
	ON_CBN_SELCHANGE(IDC_COMBO6, &KAccessiblityModifyDlg::OnCbnSelchangeComboStation)
	ON_BN_CLICKED(IDC_BUTTON5, &KAccessiblityModifyDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON4, &KAccessiblityModifyDlg::OnBnClickedButtonApply)
	ON_CBN_SELCHANGE(IDC_COMBO8, &KAccessiblityModifyDlg::OnCbnSelchangeComboClass)
	ON_BN_CLICKED(IDCANCEL, &KAccessiblityModifyDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KAccessiblityModifyDlg 메시지 처리기입니다.


BOOL KAccessiblityModifyDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog(); 

	KReportCtrlSetting::Default(m_wndReportCtrl,TRUE);

	try
	{
		ResizeComponent();
		InitComboClassInfo();
		InitComboStationInfo();
		InitComboZoneInfo();
		InitColorButton();
		InitReportHeader();

		m_cboClass.EnableWindow(FALSE);
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KAccessiblityModifyDlg::InitReportHeader()
{
	KLogDebugStartMsg();
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Select"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Line ID"), 100));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Type"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetEditable(FALSE);

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Class"), 50 ));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = FALSE;

		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		
		AutoType iter = m_mapClassInfo.begin();
		AutoType end  = m_mapClassInfo.end();
		
		pEditOptions->AddConstraint(_T("None"), 0);
		while(iter != end)
		{
			CString strClassName = iter->first;
			int		nClassID	 = iter->second;
			pEditOptions->AddConstraint(strClassName, nClassID);
			iter++;
		}

		
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}
	KLogDebugEndMsg();
}


void KAccessiblityModifyDlg::InitComboZoneInfo()
{
	KLogDebugStartMsg();

	m_cboZoneID.ResetContent();
	m_cboZoneID.Clear();
	m_cboZoneID.SetRedraw(FALSE);
	m_cboZoneID.InitStorage(nZONENODEITEM_LENGTH, nDATA_SIZE);

	m_mapZoneInfo.clear();

	try
	{
		KDBaseAccessibilityModify::GetZoneInfo(m_pTarget, m_mapZoneInfo);

		int nZeroCur = m_cboZoneID.AddString(_T("--Select--"));

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
	}
	catch (...)
	{
		KLogDebugException();
	}

	m_cboZoneID.SetRedraw(TRUE);

	KLogDebugEndMsg();
}


void KAccessiblityModifyDlg::InitComboStationInfo()
{

	m_cboStationID.ResetContent();
	m_cboStationID.Clear();
	m_cboStationID.SetRedraw(FALSE);
	m_cboStationID.InitStorage(nNODEITEM_LENGTH, nDATA_SIZE);

	m_setStationInfo.clear();
    
	try
	{
		KDBaseAccessibilityModify::GetSationInfo(m_pTarget, m_setStationInfo, m_mapLineStationInfo);

		int nZeroCur = m_cboStationID.AddString(_T("--Select--"));

		AutoType iter = m_setStationInfo.begin();
		AutoType end  = m_setStationInfo.end();
		while(iter != end)
		{
			Integer nxStationID = *iter;
			CString strStationID;
			strStationID.Format(_T("%I64d"), nxStationID);
			int nIndex = m_cboStationID.AddString(strStationID);
			m_cboStationID.SetItemData(nIndex,(DWORD_PTR)nxStationID);
			iter++;
		}

		m_cboStationID.SetCurSel(nZeroCur);

	}
	catch (...)
	{
		KLogDebugException();
	}

	m_cboStationID.SetRedraw(TRUE);

	KLogDebugEndMsg();
}


void KAccessiblityModifyDlg::InitComboClassInfo()
{
	try
	{
		std::map<CString, double> mapTime;
		KDBaseAccessibilityModify::GetClassInfo(m_pTarget, m_mapClassInfo, mapTime);

		int nZeroCur = m_cboClass.AddString(_T("--select--"));

		AutoType iter = m_mapClassInfo.begin();
		AutoType end  = m_mapClassInfo.end();

		while(iter != end )
		{
			CString strClassName = iter->first;
			int		nClassID		 = iter->second;

			int nIndex = m_cboClass.AddString(strClassName);
			m_cboClass.SetItemData(nIndex, nClassID);

			iter++;
		}

		m_cboClass.SetCurSel(nZeroCur);

	}
	catch (...)
	{
		KLogDebugException();
	}
	KLogDebugEndMsg()
}


void KAccessiblityModifyDlg::MapSelectionNoneFeedback()
{

}


void KAccessiblityModifyDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
}


void KAccessiblityModifyDlg::MapSelectionFeedback( Integer a_nxNodeID )
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

		CString strComboName;
		strComboName.Format(_T("%I64d(%s)"), a_nxNodeID, strZoneName);
		int nCur = m_cboZoneID.FindString(-1, strComboName);
		m_cboZoneID.SetCurSel(nCur);
		
	}
	else if (true == m_bClickedStation)
	{
		CString strComboName;
		strComboName.Format(_T("%I64d"), a_nxNodeID);
		int nCur = m_cboStationID.FindString(-1, strComboName);
		m_cboStationID.SetCurSel(nCur);
	}

	m_pMapView->ZoomToFeature(TABLE_NODE, COLUMN_NODE_ID, a_nxNodeID);
	ClearNodeSymbol();

	UpdateData(FALSE);
}


void KAccessiblityModifyDlg::GetMapView( KMapView* a_pMapView )
{
	m_pMapView = a_pMapView;
}


void KAccessiblityModifyDlg::OnBnClickedButtonZone()
{
	m_bClickedZone = true;

	COLORREF makerColor = m_btnZoneColor.GetColor();

	std::vector<TNodeSelectionInfo> vecTNodeStation;
	AutoType iter = m_mapZoneInfo.begin();
	AutoType end  = m_mapZoneInfo.end();
	while(iter != end)
	{
		TNodeSelectionInfo oTNodeStation;
		oTNodeStation.TNodeID = iter->first;
		oTNodeStation.TType   = 1;
		oTNodeStation.TCharacterIndex = 50;
		oTNodeStation.TSize   = 10;
		oTNodeStation.TColorRGB = makerColor;
		CString strZoneID;
		strZoneID.Format(_T("%I64d"), iter->first);
		oTNodeStation.TLabel  = strZoneID;

		vecTNodeStation.push_back(oTNodeStation);
		iter++;
	}

	m_pMapView->MapNodeSelectAction(this, vecTNodeStation);
}


void KAccessiblityModifyDlg::OnBnClickedButtonStation()
{
	m_bClickedStation = true;

	COLORREF makerColor = m_btnStationColor.GetColor();
	std::vector<TNodeSelectionInfo> vecTNodeStation;
	AutoType iter = m_setStationInfo.begin();
	AutoType end  = m_setStationInfo.end();
	while (iter != end)
	{
		TNodeSelectionInfo oTNodeStation;
		oTNodeStation.TNodeID = *iter;
		oTNodeStation.TType   = 1;
		oTNodeStation.TCharacterIndex = 50;
		oTNodeStation.TSize   = 10;
		oTNodeStation.TColorRGB = makerColor;
		CString strZoneID;
		strZoneID.Format(_T("%I64d"), *iter);
		oTNodeStation.TLabel  = strZoneID;

		vecTNodeStation.push_back(oTNodeStation);
		iter++;
	}
	m_pMapView->MapNodeSelectAction(this,vecTNodeStation);

}


void KAccessiblityModifyDlg::OnBnClickedButtonSearch()
{
	try
	{
		if (false == CheckComboData())
		{
			return;
		}
		m_cboClass.EnableWindow(TRUE);
		updateReportControl();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}

}


void KAccessiblityModifyDlg::InitColorButton()
{
	m_btnStationColor.SetColor(RGB(0,0,255));
	m_btnStationColor.SetWindowText(_T(""));
	m_btnStationColor.ShowText(TRUE);
	m_btnStationColor.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	m_btnZoneColor.SetColor(RGB(255,0,0));
	m_btnZoneColor.SetWindowText(_T(""));
	m_btnZoneColor.ShowText(TRUE);
	m_btnZoneColor.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

}


void KAccessiblityModifyDlg::ClearNodeSymbol()
{
	try
	{
		m_pMapView->ClearScreenDrawEdit();
		m_bClickedStation = false;
		m_bClickedZone	  = false;
	}
	catch (...)
	{
		KLogDebugException();
	}
	
}


void KAccessiblityModifyDlg::OnCbnSelchangeComboZone()
{
	int	nCurSel		=  m_cboZoneID.GetCurSel();
	Integer nxZoneID = m_cboZoneID.GetItemData(nCurSel);
	if (nINITCOMBOVALUE == nCurSel)
	{
		return;
	}

	m_pMapView->ZoomToFeature(TABLE_NODE, COLUMN_NODE_ID, nxZoneID);
	m_cboClass.EnableWindow(FALSE);
}


void KAccessiblityModifyDlg::OnCbnSelchangeComboStation()
{
	int	nCurSel		=  m_cboStationID.GetCurSel();
	Integer nxStationID = m_cboStationID.GetItemData(nCurSel);
	if (nINITCOMBOVALUE == nCurSel)
	{
		return;
	}

	m_pMapView->ZoomToFeature(TABLE_NODE, COLUMN_NODE_ID, nxStationID);
	m_cboClass.EnableWindow(FALSE);
}


bool KAccessiblityModifyDlg::CheckComboData()
{

	int	nCurSel	= m_cboZoneID.GetCurSel();
	if (nINITCOMBOVALUE == nCurSel )
	{
		AfxMessageBox(_T("선택된 Zone 이 없습니다."));
		return false;
	}
	else
	{
		m_nxZoneID	=  m_cboZoneID.GetItemData(nCurSel);
	}


	nCurSel	= m_cboStationID.GetCurSel();
	if (nINITCOMBOVALUE == nCurSel)
	{
		AfxMessageBox(_T("선택된 Station이 없습니다."));
		return false;

	}
	else
	{
		m_nxStationID = m_cboStationID.GetItemData(nCurSel);
	}

	return true;
}


void KAccessiblityModifyDlg::updateReportControl()
{
	m_wndReportCtrl.ResetContent(TRUE);

	AutoType iter = m_mapLineStationInfo.begin();
	AutoType end  = m_mapLineStationInfo.end();

	while(iter != end)
	{
		KODKey oLineStationKey = iter->first;
		Integer nxTransitID = oLineStationKey.OriginID;
		Integer nxStationID = oLineStationKey.DestinationID;
		int		nModeType	= iter->second;

		KODKey oUsekey(nxTransitID, nxStationID);

		if (m_nxStationID == nxStationID)
		{
			std::map<KODKey, int> mapExistAccessInfo;
			KDBaseAccessibilityModify::GetExistingAccessLineInfo(m_pTarget, m_nxZoneID, nxTransitID, m_mapClassInfo, mapExistAccessInfo);
			
			AutoType finditer = mapExistAccessInfo.find(oUsekey);
			AutoType findend  = mapExistAccessInfo.end();

			CXTPReportRecord*		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			if (finditer != findend)
			{
				pItem->SetChecked(TRUE);
			}

			CString strTransitID = _T("");
			strTransitID.Format(_T("%I64d"), nxTransitID);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strTransitID);

			CString strType = _T("");
			strType.Format(_T("%d"), nModeType);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strType);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			if (finditer != findend)
			{
				int nClassID = finditer->second;
				pItem->SetItemData(nClassID);			
			}
		}

		iter++;
	}

	m_wndReportCtrl.Populate();
}


void KAccessiblityModifyDlg::OnBnClickedButtonClear()
{
	m_cboClass.EnableWindow(FALSE);
	m_wndReportCtrl.ResetContent();
	m_cboStationID.SetCurSel(0);
	m_cboZoneID.SetCurSel(0);
	m_cboClass.SetCurSel(0);
	m_pMapView->ClearScreenDrawEdit();
}


void KAccessiblityModifyDlg::OnBnClickedButtonApply()
{
	try
	{
		std::map<Integer, CString> mapResultInfo;

		CXTPReportRecords*           pRecords      = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemNumber*  pItemNum      = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord			= pRecords->GetAt(i);
			pItem			= pRecord->GetItem(0);
			BOOL bCheck		= pItem->IsChecked();

			if (FALSE == bCheck)
			{
				continue;
			}

			pItem				= pRecord->GetItem(1);
			pColumn				= m_wndReportCtrl.GetColumns()->GetAt(1);
			CString strLineID	= pItem->GetCaption(pColumn);
			Integer nxLineID    = _ttoi64(strLineID);

			pItem				= pRecord->GetItem(3);
			pColumn				= m_wndReportCtrl.GetColumns()->GetAt(3);
			CString ClassName	= pItem->GetCaption(pColumn);

			AutoType iter = m_mapClassInfo.find(ClassName);
			AutoType end  = m_mapClassInfo.end();
			if (iter == end)
			{
				CString strMsg(_T(""));
				strMsg.Format(_T("Line ID : %I64d 의 등급을 설정 하세요"), nxLineID);
				ThrowException(strMsg);
			}

			mapResultInfo.insert(std::make_pair(nxLineID, ClassName));
		}

		AutoType iter = mapResultInfo.begin();
		AutoType end  = mapResultInfo.end();
		while(iter != end)
		{
			Integer nxLineID		= iter->first;
			CString strClassName	= iter->second;

			KDBaseAccessibilityModify::UpdateAccessLineInfo(m_pTarget, m_nxZoneID, m_nxStationID, nxLineID, strClassName);
			iter++;
		}

		AfxMessageBox(_T(" Complete.."));

		KIOTable* pAccessTable = m_pTarget->Tables()->FindTable(TABLE_ACCESS_LINE_STATION);
		pAccessTable->Notify();
	}

	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}
}


void KAccessiblityModifyDlg::OnCbnSelchangeComboClass()
{
	try
	{
		int nCursel  = m_cboClass.GetCurSel();
		int nClassID = m_cboClass.GetItemData(nCursel);
		CXTPReportRecords*           pRecords      = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItemNumber*  pItemNum      = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;

		int nRecordCount = pRecords->GetCount();

		for (int i = 0 ; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItem	= pRecord->GetItem(3);

			pItem->SetItemData(nClassID);
		}
		m_wndReportCtrl.RedrawControl();
	}
	catch (KExceptionPtr ex)
	{
		KLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		KLogDebugException();
		throw 1;
	}
	
}


void KAccessiblityModifyDlg::OnBnClickedCancel()
{
	m_pMapView->ClearScreenDrawEdit();
	m_cboClass.EnableWindow(FALSE);
	m_wndReportCtrl.ResetContent();
	m_cboStationID.SetCurSel(0);
	m_cboZoneID.SetCurSel(0);
	m_cboClass.SetCurSel(0);

	KResizeDialogEx::OnCancel();
}

void KAccessiblityModifyDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR5,		  SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR3,		  SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR4,		  SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR7,		  SZ_TOP_LEFT,      SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_BOTTOM_RIGHT);

	SetResize(IDC_COMBO1,			  SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_COMBO6,             SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON2,			  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON6,			  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_COLOR1,	  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_COLOR2,	  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_ZONE_COLOR,  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_NODE_COLOR,  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON1,			  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_COMBO8,			  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_CLASS,		  SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDCANCEL,			      SZ_BOTTOM_RIGHT,     SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON5,			  SZ_BOTTOM_LEFT,     SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON4,			  SZ_BOTTOM_LEFT,     SZ_BOTTOM_LEFT);
}
