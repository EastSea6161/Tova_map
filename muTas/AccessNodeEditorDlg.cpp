// AccessNodeEditorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AccessNodeEditorDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "MapView.h"
#include "CustomReportRecordItem.h"
#include "IOTransitTableView.h"
#include "ImChampFrameWindow.h"
#include "ImChampMapDefine.h"
#include "IOView.h"

// KAccessNodeEditorDlg 대화 상자입니다.

const int	  nZCLASSINDEX			= 0;

const int	  nAllCode				= 0;
const int	  nBusCode				= 3;
const int	  nRailCode				= 4;

const int	  nZoneSize				= 15;
const int     nStationSize			= 15;
const int	  nZoneCharacterIndex	= 66;
const int	  nStationCharIndex		= 74;


const COLORREF clrZONEBOUNDARTDEFAULT	= RGB(47, 157, 39);
const COLORREF clrZONEDEFAULT			= RGB(76, 76, 76);
const COLORREF clrSTOPDEFAULT			= RGB(0, 87, 102);


IMPLEMENT_DYNAMIC(KAccessNodeEditorDlg, KResizeDialogEx)

KAccessNodeEditorDlg::KAccessNodeEditorDlg(KTarget* a_pTarget, KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KAccessNodeEditorDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pMapView(a_pMapView)
{
    m_bEdited = false;
}

KAccessNodeEditorDlg::~KAccessNodeEditorDlg()
{
	CloseWindowProc();
}

void KAccessNodeEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT_CLASS, m_wndReportCtrlClass);
	DDX_Control(pDX, IDC_REPORT_NODE, m_wndReportCtrlNode);
	DDX_Control(pDX, IDC_COMBO6, m_cboMode);
}


BEGIN_MESSAGE_MAP(KAccessNodeEditorDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ZONE, IDC_RADIO_STOP, &KAccessNodeEditorDlg::OnBnClickedRadioTimeMethod)
	ON_BN_CLICKED(IDC_BUTTON1, &KAccessNodeEditorDlg::OnBnClickedButtonSeach)
	ON_BN_CLICKED(IDC_BUTTON7, &KAccessNodeEditorDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON6, &KAccessNodeEditorDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON4, &KAccessNodeEditorDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL, &KAccessNodeEditorDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO6, &KAccessNodeEditorDlg::OnCbnSelchangeModeCombo)

	ON_NOTIFY(XTP_NM_REPORT_CHECKED,	  IDC_REPORT_NODE,	OnReportCheckItemNode)
	ON_NOTIFY(NM_CLICK,                  IDC_REPORT_NODE,  OnReportItemDoubleClickNode)
	ON_NOTIFY(NM_CLICK,                   IDC_REPORT_CLASS, OnReportItemClickClass)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT_NODE,	OnReportItemValueChangedNode)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT_CLASS,	OnReportItemValueChangedClass)
	ON_BN_CLICKED(IDC_CHECK_LABEL, &KAccessNodeEditorDlg::OnBnClickedCheckLabel)
	ON_BN_CLICKED(IDC_CHECK_RADIUS, &KAccessNodeEditorDlg::OnBnClickedCheckRadius)
	ON_BN_CLICKED(IDC_CHECK_LINE, &KAccessNodeEditorDlg::OnBnClickedCheckLine)
	ON_BN_CLICKED(IDC_CHECK_LINEINFO, &KAccessNodeEditorDlg::OnBnClickedCheckAllLine)
	ON_BN_CLICKED(IDC_CHECK_ALL, &KAccessNodeEditorDlg::OnBnClickedCheckAll)
END_MESSAGE_MAP()


void KAccessNodeEditorDlg::ModelessOKFeedback( UINT nDialogID )
{
	try
	{
		bool bRefresh(false);
		if (KImChampIDFind2Column::IDD == nDialogID)
		{
			Integer nxID = m_spImTasIDFind->GetSelectedID();
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			CString strName(_T(""));
			int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
			if (nChecked == IDC_RADIO_ZONE)
			{
				AutoType find = m_mapZoneName.find(nxID);
				AutoType end  = m_mapZoneName.end();
				if (find != end)
				{
					strName = find->second;
				}
			}
			else
			{
				AutoType find = m_mapAllNodeInfo.find(nxID);
				AutoType end  = m_mapAllNodeInfo.end();
				if (find != end)
				{
					TFixedNodeInfo oNode = find->second;
					strName = oNode.name;
				}
			}

			m_oSelectIdCaption.TID	    = strID;
			m_oSelectIdCaption.TCaption = strName;

			CString strCaption(_T(""));
			if (strName == _T(""))
			{
				strCaption.Format(_T("%s[-]"), strID);
			}
			else
			{
				strCaption.Format(_T("%s[%s]"), strID, strName);
			}
			SetDlgItemText(IDC_EDIT_ID, strCaption);

			Integer nxNodeID = _ttoi64(m_oSelectIdCaption.TID);
			m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID, bRefresh);

			bRefresh = true;
			UpdateReportZoneStation(bRefresh);
            m_bEdited = false;
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

void KAccessNodeEditorDlg::MapSelectionFeedback( std::vector<Integer> a_vecSelection )
{
}

void KAccessNodeEditorDlg::MapSelectionFeedback( Integer a_nxNodeID )
{
	bool bRefresh(false);
	try
	{
		ClearAllMap(bRefresh);
		AddReportControlToSelectNode(a_nxNodeID);
		bRefresh = true;
		DrawZoneStation(bRefresh);
        m_bEdited = true;
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

void KAccessNodeEditorDlg::MapSelectionNoneFeedback()
{
}

BOOL KAccessNodeEditorDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrlClass, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlClass);
	KReportCtrlSetting::Default(m_wndReportCtrlNode, TRUE);
	KReportCtrlSetting::SelectionEnableFalse(m_wndReportCtrlNode);
	
	KDBaseAccessibilityModify::GetClassInfo(m_pTarget, m_mapClassInfo, m_mapClassTime);
	KDBaseAccessibilityModify::GetZoneInfo(m_pTarget, m_mapZoneName, m_mapZoneFullName);
	KDBaseAccessibilityModify::TransitStationNodeViaInfoByAccess(m_pTarget, m_mapStationLineInfo, m_mapStationModeInfo);
	//KDBaseAccessibilityModify::GetAllNodeInfo(m_pTarget, m_mapAllNodeInfo);
	KBulkDBase::BulkNodeSelectAll(m_pTarget, m_mapAllNodeInfo);

	double dRadiousKm(0.0);
	KDBaseAccessibilityModify::GetZoneRadius(m_pTarget, dRadiousKm);
	m_dRadious = dRadiousKm * 1000;

	CheckRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP, IDC_RADIO_ZONE);
	OnBnClickedRadioTimeMethod(IDC_RADIO_ZONE);

// 	bool bUseAccessDB = false;
// 	KDBaseAccessibilityModify::CheckUsingAssessData(m_pTarget, bUseAccessDB);
// 	if (false == bUseAccessDB)
// 	{
// 		AfxMessageBox(_T("경로 만들기 > 대중교통 > 이용가능 역/정류장 생성을 먼저 실행해 주세요."));
// 		KResizeDialogEx::OnCancel();
// 	}

	CheckDlgButton(IDC_CHECK_LINE, TRUE);

	CString strCaption;
	strCaption.Format(_T("::: %s"), m_pTarget->Caption());
	SetDlgItemText(IDC_STATIC_TARGETYEAR, strCaption);
	
	ResizeComponent();
	InitReportHeaderClass();
	UpdateReportClass();
	InitModeComboBox();
    m_bEdited = false;
	return TRUE; 
}


void KAccessNodeEditorDlg::ClassificationStationClass( void )
{
	try
	{
		int nMaxCount = 0;

		AutoType iter = m_mapStationLineInfo.begin();
		AutoType end  = m_mapStationLineInfo.end();
		while(iter != end)
		{
			std::set<Integer> setInfo = iter->second;

			int nCount = setInfo.size();

			if (nCount > nMaxCount)
			{
				nMaxCount = nCount;
			}

			++iter;
		}

		m_mapClassInNetwork.clear();

		int nClassCount = m_mapClassInfo.size();
		int nCount = nMaxCount / nClassCount;

		int nValue = nMaxCount;
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
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}


void KAccessNodeEditorDlg::OnBnClickedRadioTimeMethod( UINT nID )
{
	bool bRefresh(false);
	try
	{
		bRefresh = true;
		ClearAllMap(bRefresh);
		m_wndReportCtrlNode.ResetContent();
		m_wndReportCtrlNode.GetColumns()->Clear();
		m_wndReportCtrlNode.Populate();

		m_oSelectIdCaption.TCaption = _T("");
		m_oSelectIdCaption.TID		= _T("");

		SetDlgItemText(IDC_EDIT_ID, _T(""));
		if (nID == IDC_RADIO_ZONE)
		{
			InitReportHeaderStation();
			CheckDlgButton(IDC_CHECK_LINEINFO, FALSE);
			CheckDlgButton(IDC_CHECK_ALL, FALSE);
			SetDlgItemText(IDC_CHECK_LINEINFO, _T("Check All Transit Line"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				SetDlgItemText(IDC_CHECK_LINEINFO, _T("모든 통과 노선 그리기"));
			}
		}
		else
		{
			InitReportHeaderZone();
			CheckDlgButton(IDC_CHECK_LINEINFO, FALSE);
			CheckDlgButton(IDC_CHECK_ALL, FALSE);
			SetDlgItemText(IDC_CHECK_LINEINFO,_T("View Station Tranist Line"));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				SetDlgItemText(IDC_CHECK_LINEINFO, _T("모든 통과 노선 그리기"));
			}
		}

		if (nullptr != m_spImTasIDFind) {
			if(!m_spImTasIDFind->IsUserClosed()) {
				SeachZoneStation();
			}
		}

        m_bEdited = false;
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


void KAccessNodeEditorDlg::InitReportHeaderClass( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(0, _T("Class"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("접근등급"));
        }

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(1, _T("Time"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("도보접근시간(분)"));
        }

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(2, _T("Color"), 30));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("색상"));
        }

		pColumn = m_wndReportCtrlClass.AddColumn(new CXTPReportColumn(3, _T("Size"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("크기"));
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

void KAccessNodeEditorDlg::InitReportHeaderStation( void )
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(1, _T("Node_ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("노드 ID"));
    }

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(2, _T("Type"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("수단"));
    }

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(3, _T("Class In Zone"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("접근등급"));
    }

	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

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
		++iter;
	}

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(4, _T("Class In Network"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("시스템 접근등급"));
    }

	pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(5, _T("No.of Line"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("통과 노선수"));
    }
}

void KAccessNodeEditorDlg::InitReportHeaderZone( void )
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(0, _T("Select"), 40, FALSE));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetAlignment(xtpColumnIconCenter);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("선택"));
        }

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(1, _T("Zone_ID"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("존 ID"));
        }

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(2, _T("Type"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("수단"));
        }

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(3, _T("Class In Zone"), 50));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("접근등급"));
        }

		pColumn->GetEditOptions()->AddComboButton(TRUE);
		pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
		pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

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
			++iter;
		}

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(4, _T("Class In Network"), 60));
		pColumn->SetHeaderAlignment(DT_CENTER);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("시스템 접근등급"));
        }

		pColumn = m_wndReportCtrlNode.AddColumn(new CXTPReportColumn(5, _T("No.of Line"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		pColumn->SetVisible(FALSE);
        if (KmzSystem::GetLanguage() == KEMKorea) {
            pColumn->SetCaption(_T("통과 노선수"));
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


void KAccessNodeEditorDlg::InitModeComboBox( void )
{
	try
	{
		//int nZeroCur = m_cboMode.AddString(_T("All"));
		int nZeroCur = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
            nZeroCur = m_cboMode.AddString(_T("전체"));
        }
		else {
			nZeroCur = m_cboMode.AddString(_T("All"));
		}
		m_cboMode.SetItemData(nZeroCur, nAllCode);

		//int nComboIndex = m_cboMode.AddString(_T("Highway_Transit"));
		int nComboIndex = 0;
		if (KmzSystem::GetLanguage() == KEMKorea) {
            nComboIndex = m_cboMode.AddString(_T("버스 타입"));
        }
		else {
			nComboIndex = m_cboMode.AddString(_T("Bus_Transit"));
		}
		m_cboMode.SetItemData(nComboIndex, nBusCode);

		//nComboIndex = m_cboMode.AddString(_T("Rail_Transit"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
            nComboIndex = m_cboMode.AddString(_T("철도 타입"));
        }
		else {
			nComboIndex = m_cboMode.AddString(_T("Rail_Transit"));
		}
		m_cboMode.SetItemData(nComboIndex, nRailCode);

		m_cboMode.SetCurSel(nZeroCur);
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

void KAccessNodeEditorDlg::UpdateReportClass( void )
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
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
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
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItem());
			colorRef = pMunSellColor->GetColor(nIdx);
			pItem->SetBackgroundColor(colorRef);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber ((double)nSize, _T("%.0f")));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetEditable(TRUE);

			nSize = (nSize - 1);
			nIdx++;
			++iter;
		}

		pRecord = m_wndReportCtrlClass.AddRecord(new CXTPReportRecord());

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("Z")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		double dClassT = 0;
		pItem = pRecord->AddItem(new CXTPReportRecordItemText (_T("-")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		colorRef = pMunSellColor->GetColor(nIdx);
		pItem->SetBackgroundColor(colorRef);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber ((double)nSize, _T("%.0f")));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetEditable(TRUE);

		m_wndReportCtrlClass.Populate();

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

void KAccessNodeEditorDlg::OnBnClickedButtonSeach()
{
	ClearAllMap(true);
	CheckDlgButton(IDC_CHECK_LINEINFO, FALSE);
	CheckDlgButton(IDC_CHECK_ALL, FALSE);
	SeachZoneStation();
}

void KAccessNodeEditorDlg::SeachZoneStation( void )
{
	try
	{
		CString strSelect(_T(""));
		GetDlgItemText(IDC_EDIT_ID, strSelect);

		m_spImTasIDFind = KImChampIDFind2ColumnPtr(new KImChampIDFind2Column(this));
		m_spImTasIDFind->SetModelessReceiver(this);

		Integer nxSelID = _ttoi64(m_oSelectIdCaption.TID);

		if (nxSelID < 1)
		{
			m_spImTasIDFind->SetSelectedID(-1);
		}
		else
		{
			m_spImTasIDFind->SetSelectedID(nxSelID);
		}

		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		
		int nCurSel  = m_cboMode.GetCurSel();
		int nSelMode = m_cboMode.GetItemData(nCurSel);

		std::map<Integer, int> mapZoneCount;
		KDBaseAccessibilityModify::GetAccessNodeZoneCount(m_pTarget, mapZoneCount, nSelMode);
		std::map<Integer, int> mapNodeCount;
		KDBaseAccessibilityModify::GetAccessZoneNodeCount(m_pTarget, mapNodeCount, nSelMode);

		m_vecKIDZoneInfo.clear();
		if (nChecked == IDC_RADIO_ZONE)
		{
			AutoType iter = m_mapZoneName.begin();
			AutoType end  = m_mapZoneName.end();
			while(iter != end )
			{
				KID2ColumnCaption oCaption;
				CString strID(_T(""));
				strID.Format(_T("%I64d"), iter->first);
				oCaption.TID = strID;

				CString strZoneName = iter->second;
				oCaption.TCaption = strZoneName;
				int nCount(0);
				CString strCheck(_T(""));
				AutoType find = mapZoneCount.find(iter->first);
				AutoType end  = mapZoneCount.end();
				if (find != end)
				{
					strCheck= _T("v");
					nCount = find->second;
				}
				CString strTCaption(_T(""));
				strTCaption.Format(_T("%d"),nCount);
				oCaption.TCaption2= strTCaption;

				m_vecKIDZoneInfo.push_back(oCaption);
				
				++iter;
			}

            if (KmzSystem::GetLanguage() == KEMKorea) {
                m_spImTasIDFind->SetSecondColumnname(_T("역/정류장 개수"));
            }
            else {
			    m_spImTasIDFind->SetSecondColumnname(_T("No. of Station"));
            }
			m_spImTasIDFind->SetIDCaption(m_vecKIDZoneInfo);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, m_mapZoneName, ImChampMapDefineSymbol::ZoneSymbol);
		}
		else
		{

			std::map<Integer, CString>	mapStopNodeInfo;
			std::vector<KID2ColumnCaption>		vecKIDStopNodeInfo;

			AutoType iter = m_mapStationModeInfo.begin();
			AutoType end  = m_mapStationModeInfo.end();
			while(iter != end )
			{
				Integer nxNodeID = iter->first;
				int		nMode	 = iter->second;
				CString strNodeName(_T(""));

				AutoType findNode = m_mapAllNodeInfo.find(nxNodeID);
				AutoType endNode  = m_mapAllNodeInfo.end();
				if (findNode != endNode)
				{
					TFixedNodeInfo oNode = findNode->second;
					strNodeName = oNode.name;
				}
				else
				{
					strNodeName = _T("[ - ]");
				}

				KID2ColumnCaption oCaption;
				CString strNodeID(_T(""));
				strNodeID.Format(_T("%I64d"), nxNodeID);

				oCaption.TID	  = strNodeID;
				oCaption.TCaption = strNodeName;
				CString strNodeCaption(_T(""));
				AutoType find = mapNodeCount.find(nxNodeID);
				AutoType end  = mapNodeCount.end();
				if (find != end)
				{
					strNodeCaption.Format(_T("%d"), find->second);
				}
				else
				{
					strNodeCaption.Format(_T("%d"), 0);
				}

				oCaption.TCaption2 = strNodeCaption;				

				if (nSelMode == nAllCode)
				{
					mapStopNodeInfo.insert(std::make_pair(nxNodeID, strNodeName));
					vecKIDStopNodeInfo.push_back(oCaption);
				}
				else if(nSelMode == nMode)
				{
					mapStopNodeInfo.insert(std::make_pair(nxNodeID, strNodeName));
					vecKIDStopNodeInfo.push_back(oCaption);
				}

				++iter;
			}

            if (KmzSystem::GetLanguage() == KEMKorea) {
                m_spImTasIDFind->SetSecondColumnname(_T("존 개수"));
            }
            else {
                m_spImTasIDFind->SetSecondColumnname(_T("No. of Zone"));
            }
			
			m_spImTasIDFind->SetIDCaption(vecKIDStopNodeInfo);
			m_spImTasIDFind->SetMapNodeSelectInfoA(m_pMapView, mapStopNodeInfo, ImChampMapDefineSymbol::NodeSymbol);
		}

		m_spImTasIDFind->Create(KImChampIDFind2Column::IDD, this);
		m_spImTasIDFind->ShowWindow(SW_SHOW);
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

void KAccessNodeEditorDlg::UpdateReportZoneStation( bool a_bRefresh )
{
	GetDisplayInfo();
	m_wndReportCtrlNode.ResetContent();

	int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
	if (nChecked == IDC_RADIO_ZONE)
	{
		UpdateReportStation();
	}
	else
	{
		UpdateReportZone();
	}

	DrawZoneStation(a_bRefresh);
}

void KAccessNodeEditorDlg::UpdateReportZone( void )
{
	try
	{
		int nCurSel		= m_cboMode.GetCurSel();
		int nMode		= m_cboMode.GetItemData(nCurSel);

		BOOL bSelectAll(FALSE); {
			if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED) {
				bSelectAll = TRUE;
			}
		}
		BOOL bCheckAllTransitLine(FALSE); {
			if (IsDlgButtonChecked(IDC_CHECK_LINEINFO) == BST_CHECKED) {
				bCheckAllTransitLine = TRUE;
			}
		}

		Integer nxZoneID = _ttoi64(m_oSelectIdCaption.TID);

		std::map<Integer, TAccessStopNodeInfo>  mapAccessStopNodeInfo;
		KDBaseAccessibilityModify::GetIncludeZoneAccessInfo(m_pTarget, nxZoneID, nMode, mapAccessStopNodeInfo);

		m_wndReportCtrlNode.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = mapAccessStopNodeInfo.begin();
		AutoType end  = mapAccessStopNodeInfo.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlNode.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bSelectAll);

			TAccessStopNodeInfo oNodeInfo = iter->second;

			Integer nxZoneID = iter->first;
			CString strZoneID(_T(""));
			strZoneID.Format(_T("%I64d"), nxZoneID);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strZoneID));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			CString strMode(_T(""));
			if (3 == oNodeInfo.nMode)
			{
				strMode = _T("Bus_Transit");
			}
			else if ( 4 == oNodeInfo.nMode)
			{
				strMode = _T("Rail_Transit");
			}

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
			pItem->SetAlignment(xtpColumnTextCenter);
            pItem->SetItemData(oNodeInfo.nMode);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
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
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oNodeInfo.strClassInNetwork));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			int nNoLine = oNodeInfo.nNoLine;
			CString strNoLine(_T(""));
			strNoLine.Format(_T("%d"), nNoLine);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			//pItem->HasCheckbox(TRUE);
			pItem->SetCaption(strNoLine);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			++iter;
		}

		m_wndReportCtrlNode.Populate();
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

void KAccessNodeEditorDlg::UpdateReportStation( void )
{
	try
	{
		int nCurSel		= m_cboMode.GetCurSel();
		int nMode		= m_cboMode.GetItemData(nCurSel);

		BOOL bSelectAll(FALSE); {
			if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED) {
				bSelectAll = TRUE;
			}
		}
		BOOL bCheckAllTransitLine(FALSE); {
			if (IsDlgButtonChecked(IDC_CHECK_LINEINFO) == BST_CHECKED) {
				bCheckAllTransitLine = TRUE;
			}
		}

		Integer nxZoneID = _ttoi64(m_oSelectIdCaption.TID);

		std::map<Integer, TAccessStopNodeInfo>  mapAccessStopNodeInfo;
		KDBaseAccessibilityModify::GetIncludeNodeAccessInfo(m_pTarget, nxZoneID, nMode, mapAccessStopNodeInfo);

		m_wndReportCtrlNode.ResetContent(TRUE);
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		AutoType iter = mapAccessStopNodeInfo.begin();
		AutoType end  = mapAccessStopNodeInfo.end();
		while(iter != end)
		{
			CXTPReportRecord*		pRecord = m_wndReportCtrlNode.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bSelectAll);

			TAccessStopNodeInfo oNodeInfo = iter->second;

			Integer nxNodeID = iter->first;
			CString strNodeID(_T(""));
			strNodeID.Format(_T("%I64d"), nxNodeID);
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeID));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			CString strMode(_T(""));
			if (3 == oNodeInfo.nMode)
			{
				strMode = _T("Bus_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("버스 타입");
				}
			}
			else if ( 4 == oNodeInfo.nMode)
			{
				strMode = _T("Rail_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("철도 타입");
				}
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
			pItem->SetItemData(oNodeInfo.nMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
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
			pItem->SetAlignment(DT_CENTER);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (oNodeInfo.strClassInNetwork));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			int nNoLine = oNodeInfo.nNoLine;
			CString strNoLine(_T(""));
			strNoLine.Format(_T("%d"), nNoLine);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheckAllTransitLine);
			pItem->SetCaption(strNoLine);
			//pItem->SetAlignment(xtpColumnTextCenter);
			//pItem->SetEditable(FALSE);

			++iter;
		}

		m_wndReportCtrlNode.Populate();
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

void KAccessNodeEditorDlg::GetDisplayInfo( void )
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
			pItem					= pRecord->GetItem(0);
			pColumn					= m_wndReportCtrlClass.GetColumns()->GetAt(0);
			CString	 strClassName	= pItem->GetCaption(pColumn);

			pItem					= pRecord->GetItem(2);
			COLORREF color			= pItem->GetBackgroundColor();

			pItemNum				= (CXTPReportRecordItemNumber*)(pRecord->GetItem(3));
			double dSize            = pItemNum->GetValue();

			TClassInfo oClassInfo;
			oClassInfo.clrClass = color;
			oClassInfo.dSize	= dSize;

			m_mapClassDisplay.insert(std::make_pair(strClassName, oClassInfo));
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

void KAccessNodeEditorDlg::DrawZoneStation( bool a_bRefresh )
{
	try
	{
		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		if (nChecked == IDC_RADIO_ZONE)
		{
			DrawAccessStation(a_bRefresh);
			GetCheckedLineStation();
		}
		else
		{
			DrawAccessZone(a_bRefresh);
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

void KAccessNodeEditorDlg::DrawAccessStation( bool a_bRefresh )
{
	try
	{
		//Draw Zone
		Integer nxZoneID = _ttoi64(m_oSelectIdCaption.TID);
		CString strLable(_T(""));
		strLable.Format(_T("Zone:%s"), m_oSelectIdCaption.TID);
		
		std::vector<TDrawZoneAccess> vecZoneAccess;
		TDrawZoneAccess oDrawZoneAccess;
		oDrawZoneAccess.TNodeID			 = nxZoneID;
		oDrawZoneAccess.TSymbol  = nZoneCharacterIndex;
		oDrawZoneAccess.TSize			 = nZoneSize;
		oDrawZoneAccess.TColorRGB		 = clrZONEBOUNDARTDEFAULT;
		oDrawZoneAccess.TLabel			 = strLable;

		if (IsDlgButtonChecked(IDC_CHECK_RADIUS) == BST_CHECKED)
		{
			oDrawZoneAccess.TRadius	    = m_dRadious;
		}
		else
		{
			oDrawZoneAccess.TRadius		= 0.1;
		}
		vecZoneAccess.push_back(oDrawZoneAccess);

		TDrawNodeInfo oZoneNodeInfo;
		oZoneNodeInfo.TNodeID			= nxZoneID;
		oZoneNodeInfo.TSymbol	= nZoneCharacterIndex;
		oZoneNodeInfo.TSize				= nZoneSize;
		oZoneNodeInfo.TColorRGB			= clrZONEDEFAULT;
		oZoneNodeInfo.TLabel			= strLable;

		//DrawLine 시작점
		TMapPoint oStartPoint;
		AutoType findNode = m_mapAllNodeInfo.find(nxZoneID);
		AutoType endNode  = m_mapAllNodeInfo.end();
		if (findNode!= endNode)
		{
			TFixedNodeInfo oInfo = findNode->second;
			oStartPoint.Tx = oInfo.x;
			oStartPoint.Ty = oInfo.y;
		}

		//DrawStation
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		vecDrawNodeInfo.push_back(oZoneNodeInfo);

		//DrawLine
		std::vector<TUserLineInfo> vecDrawLineInfo;		

		std::map<Integer, CString> mapDrawStationInfo;
		GetDrawInfo(mapDrawStationInfo);
		AutoType iter = mapDrawStationInfo.begin();
		AutoType end  = mapDrawStationInfo.end();
		while(iter != end)
		{
			Integer nxNodeID = iter->first;
			CString strClass = iter->second;

			TDrawNodeInfo oNodeInfo;
			oNodeInfo.TNodeID			= nxNodeID;
			oNodeInfo.TSymbol	= nStationCharIndex;

			TMapPoint oEndPoint;
			AutoType findEndNode = m_mapAllNodeInfo.find(nxNodeID);
			if (findEndNode != endNode)
			{
				TFixedNodeInfo oInfo = findEndNode->second;
				oEndPoint.Tx = oInfo.x;
				oEndPoint.Ty = oInfo.y;
			}
			
			TUserLineInfo oLineDraw;
			std::vector<TMapPoint>	vecPointInfo;
			vecPointInfo.push_back(oStartPoint);
			vecPointInfo.push_back(oEndPoint);

			AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));
			TClassInfo oZClassInfo = iterZClass->second; 

			AutoType findClass = m_mapClassDisplay.find(strClass);
			AutoType endClass  = m_mapClassDisplay.end();
			if (findClass != endClass)
			{
				TClassInfo oClassInfo = findClass->second;
				CString strNodeCaption(_T(""));
				strNodeCaption.Format(_T("%s:%I64d"), findClass->first, nxNodeID);
				
				oNodeInfo.TLabel	= strNodeCaption;
				oNodeInfo.TColorRGB = oClassInfo.clrClass;
				oNodeInfo.TSize		= oClassInfo.dSize;

				oLineDraw.TColorRGB = oClassInfo.clrClass;
				oLineDraw.TWidth	= 1;
				oLineDraw.TPoints	= vecPointInfo;
			}
			else
			{
				CString strNodeCaption(_T(""));
				strNodeCaption.Format(_T("Z:%I64d"), nxNodeID);

				oNodeInfo.TLabel	= strNodeCaption;
				oNodeInfo.TColorRGB = oZClassInfo.clrClass;
				oNodeInfo.TSize		= oZClassInfo.dSize;

				oLineDraw.TColorRGB = oZClassInfo.clrClass;
				oLineDraw.TWidth	= 1;
				oLineDraw.TPoints	= vecPointInfo;
			}
			vecDrawNodeInfo.push_back(oNodeInfo);
			vecDrawLineInfo.push_back(oLineDraw);
			++iter;
		}
		

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, true,  false);
			m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);
		}
		else
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  false);
			m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);
		}

		if (IsDlgButtonChecked(IDC_CHECK_LINE) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeUserLine(vecDrawLineInfo, false, false);
		}

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

void KAccessNodeEditorDlg::DrawAccessZone( bool a_bRefresh )
{
	try
	{
		//Draw Station
		std::vector<TDrawNodeInfo> vecDrawNodeInfo;
		Integer nxStationID = _ttoi64(m_oSelectIdCaption.TID);
		AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));

		CString strLabel(_T(""));
		strLabel.Format(_T("Station:%s"), m_oSelectIdCaption.TID);

		TDrawNodeInfo oNodeInfo;
		oNodeInfo.TNodeID			= nxStationID;
		oNodeInfo.TSymbol	= nStationCharIndex;
		oNodeInfo.TLabel			= strLabel;
		oNodeInfo.TColorRGB			= clrSTOPDEFAULT;
		oNodeInfo.TSize				= nStationSize;
		
		vecDrawNodeInfo.push_back(oNodeInfo);

		//DrawLine 시작점
		TMapPoint oStartPoint;
		AutoType findNode = m_mapAllNodeInfo.find(nxStationID);
		AutoType endNode  = m_mapAllNodeInfo.end();
		if (findNode!= endNode)
		{
			TFixedNodeInfo oInfo = findNode->second;
			oStartPoint.Tx = oInfo.x;
			oStartPoint.Ty = oInfo.y;
		}

		//DrawLine
		std::vector<TUserLineInfo> vecDrawLineInfo;

		//Draw Zone
		std::vector<TDrawZoneAccess> vecZoneAccess;
		std::map<Integer, CString> mapDrawStationInfo;
		GetDrawInfo(mapDrawStationInfo);

		AutoType iter = mapDrawStationInfo.begin();
		AutoType end  = mapDrawStationInfo.end();
		while(iter != end)
		{
			//바운더리 설정
			TDrawZoneAccess oDrawZoneAccess;
			CString strZoneID(_T(""));

			oDrawZoneAccess.TNodeID			 = iter->first;

			if (IsDlgButtonChecked(IDC_CHECK_RADIUS) == BST_CHECKED)
			{
				oDrawZoneAccess.TRadius	    = m_dRadious;
			}
			else
			{
				oDrawZoneAccess.TRadius		= 0.1;
			}
			
			oDrawZoneAccess.TSymbol	 = nZoneCharacterIndex;
			oDrawZoneAccess.TLabel			 = strZoneID;
			
			//DrawLine End
			TMapPoint oEndPoint;
			AutoType findEndNode = m_mapAllNodeInfo.find(iter->first);
			if (findEndNode != endNode)
			{
				TFixedNodeInfo oInfo = findEndNode->second;
				oEndPoint.Tx = oInfo.x;
				oEndPoint.Ty = oInfo.y;
			}
			TUserLineInfo oLineDraw;
			std::vector<TMapPoint>	vecPointInfo;
			vecPointInfo.push_back(oStartPoint);
			vecPointInfo.push_back(oEndPoint);
			

			//존노드 설정
			TDrawNodeInfo	oZoneNodeInfo;
			oZoneNodeInfo.TNodeID			= iter->first;
			oZoneNodeInfo.TSymbol	= nZoneCharacterIndex;
			oZoneNodeInfo.TSize				= nZoneSize;
			

			AutoType findClass = m_mapClassDisplay.find(iter->second);
			AutoType endClass  = m_mapClassDisplay.end();
			if (findClass != endClass)
			{
				//Zone 설정
				TClassInfo oClassInfo = findClass->second;
				oDrawZoneAccess.TColorRGB	= clrZONEBOUNDARTDEFAULT;
				oDrawZoneAccess.TSize		= oClassInfo.dSize;

				//Line설정
				oLineDraw.TColorRGB = oClassInfo.clrClass;
				oLineDraw.TWidth	= 1;
				oLineDraw.TPoints	= vecPointInfo;
				strZoneID.Format(_T("%s:%I64d"),findClass->first, iter->first);

				//Zonenode 색상
				oZoneNodeInfo.TColorRGB			= oClassInfo.clrClass;
				oZoneNodeInfo.TLabel			= strZoneID;
				oZoneNodeInfo.TSize             = oClassInfo.dSize;
			}

			vecDrawLineInfo.push_back(oLineDraw);
			vecZoneAccess.push_back(oDrawZoneAccess);
			vecDrawNodeInfo.push_back(oZoneNodeInfo);

			++iter;
		}

		if(IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, true,  false);
			m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);
		}
		else
		{
			m_pMapView->TxDrawCompositeNode(vecDrawNodeInfo, false,  false);
			m_pMapView->TxDrawCompositeZoneAccess(vecZoneAccess, false);
		}

		if (IsDlgButtonChecked(IDC_CHECK_LINE) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeUserLine(vecDrawLineInfo, false, false);
		}

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

void KAccessNodeEditorDlg::GetDrawInfo( std::map<Integer, CString>& a_mapDrawInfo )
{
	try
	{
		a_mapDrawInfo.clear();
		CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportColumn*			 pColumn	   = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);

			pItem = pRecord->GetItem(3);
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
				++iter;
			}
			a_mapDrawInfo.insert(std::make_pair(nxNodeID, strClassInZone));
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



void KAccessNodeEditorDlg::OnBnClickedButtonAdd()
{
	bool bRefresh(false);
	try
	{
		if (m_oSelectIdCaption.TID == _T(""))
		{
			AfxMessageBox(_T("선택한 존(역/정류장)이 없습니다."));
			return;
		}

// 		ClearAllMap(bRefresh);
// 		bRefresh = true;
// 		DrawZoneStation(bRefresh);
		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		if (nChecked == IDC_RADIO_ZONE)
		{
			AddStationFromMap();
		}
		else
		{
			AddZoneFromMap();
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

void KAccessNodeEditorDlg::OnBnClickedButtonDel()
{
	try
	{
		DelSelectID();
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

void KAccessNodeEditorDlg::AddZoneFromMap( void )
{
	try
	{
		int nCurSel		= m_cboMode.GetCurSel();
		int nMode		= m_cboMode.GetItemData(nCurSel);

		TPreferenceSymbol &ZoneSymbol = ImChampMapDefineSymbol::ZoneSymbol;

		std::vector<TNodeSelectionInfo> vecTNodeZone;
		AutoType iter = m_mapZoneName.begin();
		AutoType end  = m_mapZoneName.end();
		while(iter != end)
		{
			Integer nxNodeID = iter->first;
			CString strName  = iter->second;

			TNodeSelectionInfo oZone;
			oZone.TNodeID			= nxNodeID;
			oZone.TType				= 1;
			oZone.TCharacterIndex	= ZoneSymbol.nCharIndex;
			oZone.TLabel		    = strName;

			std::map<Integer, CString> mapDrawStationInfo;
			GetDrawInfo(mapDrawStationInfo);
			AutoType find = mapDrawStationInfo.find(nxNodeID);
			AutoType end  = mapDrawStationInfo.end();
			if (find == end)
			{
				AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));
				TClassInfo oZClassInfo = iterZClass->second; 

				oZone.TColorRGB = oZClassInfo.clrClass;
				oZone.TSize	   = oZClassInfo.dSize;
				vecTNodeZone.push_back(oZone);
			}
			++iter;
		}
		m_pMapView->MapNodeSelectAction(this, vecTNodeZone);
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

void KAccessNodeEditorDlg::AddStationFromMap( void )
{
	try
	{
		int nCurSel		= m_cboMode.GetCurSel();
		int nMode		= m_cboMode.GetItemData(nCurSel);

		TPreferenceSymbol &NodeSymbol = ImChampMapDefineSymbol::NodeSymbol;

		std::vector<TNodeSelectionInfo> vecTNodeStation;
		AutoType iterStation = m_mapStationModeInfo.begin();
		AutoType endStation  = m_mapStationModeInfo.end();
		while(iterStation != endStation)
		{
			Integer nxNodeID = iterStation->first;
			int		nSMode	 = iterStation->second;

			TNodeSelectionInfo oStation;
			if (nSMode == nMode || nAllCode == nMode )
			{
                AutoType iter = m_mapStationLineInfo.find(nxNodeID);
				CString strLabel(_T(""));
                if (iter != m_mapStationLineInfo.end())
                {
                    std::set<Integer>& setViaTransitID = iter->second;
				    strLabel.Format(_T("%I64d(%d)"), nxNodeID, setViaTransitID.size());
                }
                else
                {
                    strLabel.Format(_T("%I64d(0)"), nxNodeID); //이런 경우는 없지만...
                }

				oStation.TNodeID			= nxNodeID;
				oStation.TType				= 1;
				oStation.TCharacterIndex	= NodeSymbol.nCharIndex;
				oStation.TLabel				= strLabel;

				std::map<Integer, CString> mapDrawStationInfo;
				GetDrawInfo(mapDrawStationInfo);
				AutoType find = mapDrawStationInfo.find(nxNodeID);
				AutoType end  = mapDrawStationInfo.end();
				if (find == end)
				{
					AutoType iterZClass = m_mapClassDisplay.find(_T("Z"));
					TClassInfo oZClassInfo = iterZClass->second; 

					oStation.TColorRGB = oZClassInfo.clrClass;
					oStation.TSize	   = oZClassInfo.dSize;
					vecTNodeStation.push_back(oStation);
				}
			}
			iterStation++;
		}
		m_pMapView->MapNodeSelectAction(this, vecTNodeStation);
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

void KAccessNodeEditorDlg::AddReportControlToSelectNode( Integer a_nxID )
{
	try
	{
		int nTotalRowCount = m_wndReportCtrlNode.GetRows()->GetCount();

		CXTPReportRecord*		pRecord = m_wndReportCtrlNode.AddRecord(new CXTPReportRecord);
		CXTPReportRecordItem*	pItem	= pRecord->AddItem(new CXTPReportRecordItem);
		pItem->HasCheckbox(TRUE);

		CString strNodeID(_T(""));
		strNodeID.Format(_T("%I64d"), a_nxID);

		pItem = pRecord->AddItem(new CXTPReportRecordItemText (strNodeID));
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		int nMode = 3;
		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		if (nChecked == IDC_RADIO_ZONE)
		{
			AutoType FindMode = m_mapStationModeInfo.find(a_nxID);
			AutoType EndMode  = m_mapStationModeInfo.end();
			if (FindMode != EndMode)
			{
				nMode = FindMode->second;
			}
			CString strMode(_T(""));
			if (3 == nMode)
			{
				strMode = _T("Bus_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("도로 타입");
				}
			}
			else if ( 4 == nMode)
			{
				strMode = _T("Rail_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("철도 타입");
				}
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
			pItem->SetItemData(nMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			pItem->SetItemData(nZCLASSINDEX);
			pItem->SetAlignment(DT_CENTER);

			CString strClassInNetwork = _T("Z");
			int		nLineCount		  = 0;
			GetClassInNetwork(a_nxID, strClassInNetwork, nLineCount);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strClassInNetwork));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			CString strNoofLine = _T("");
			strNoofLine.Format(_T("%d"), nLineCount);

			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetCaption(strNoofLine);

			m_wndReportCtrlNode.Populate();
		}
		else
		{
			Integer nxID = _ttoi64(m_oSelectIdCaption.TID);

			AutoType FindMode = m_mapStationModeInfo.find(nxID);
			AutoType EndMode  = m_mapStationModeInfo.end();
			if (FindMode != EndMode)
			{
				nMode = FindMode->second;
			}
			CString strMode(_T(""));
			if (3 == nMode)
			{
				strMode = _T("Bus_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("도로 타입");
				}
			}
			else if ( 4 == nMode)
			{
				strMode = _T("Rail_Transit");
				if (KmzSystem::GetLanguage() == KEMKorea) {
					strMode = _T("철도 타입");
				}
			}
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strMode));
			pItem->SetItemData(nMode);
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			pItem->SetItemData(nZCLASSINDEX);
			pItem->SetAlignment(DT_CENTER);

			CString strClassInNetwork = _T("Z");
			int		nLineCount		  = 0;
			GetClassInNetwork(nxID, strClassInNetwork, nLineCount);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strClassInNetwork));
			pItem->SetAlignment(xtpColumnTextCenter);
			pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem->SetEditable(FALSE);

			CString strNoofLine = _T("");
			strNoofLine.Format(_T("%d"), nLineCount);


			pItem = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetCaption(strNoofLine);

			m_wndReportCtrlNode.Populate();
		}

		nTotalRowCount = m_wndReportCtrlNode.GetRows()->GetCount();
		CXTPReportRow* pRow = m_wndReportCtrlNode.GetRows()->GetAt(nTotalRowCount - 1);

		XTP_REPORTRECORDITEM_ARGS args(&m_wndReportCtrlNode, pRow, m_wndReportCtrlNode.GetColumns()->GetAt(2));
		m_wndReportCtrlNode.EditItem(&args);
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

void KAccessNodeEditorDlg::GetClassInNetwork( Integer a_nxStationID, CString& a_strNetClass, int& a_nLineCount)
{
	try
	{
		a_nLineCount = 0;
		AutoType FindCNetwork = m_mapStationLineInfo.find(a_nxStationID);
		AutoType EndCNetwork  = m_mapStationLineInfo.end();
		if (FindCNetwork != EndCNetwork)
		{
			std::set<Integer> setLineInfo = FindCNetwork->second;
			a_nLineCount = setLineInfo.size();

			AutoType iterCinNet = m_mapClassInNetwork.begin();
			AutoType endCinNet	= m_mapClassInNetwork.end();
			while(iterCinNet != endCinNet)
			{
				if (a_nLineCount > iterCinNet->second)
				{
					a_strNetClass = iterCinNet->first;
				}
				iterCinNet++;
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

void KAccessNodeEditorDlg::ClearAllMap( bool a_bRefresh )
{
	try
	{
		m_pMapView->ClearScreenDrawEdit(false);
		m_pMapView->TxClearCompositeThemes(false);

		if (a_bRefresh) {
			m_pMapView->MapRefresh();
		}

		if (true == m_pMapView->TransitVisible())
		{
			KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);
			KIOTransitTableView* pTransitTableView = dynamic_cast<KIOTransitTableView*>(pTableView);
			if (pTransitTableView == nullptr)
				return;
			pTransitTableView->OnTransitDeSelectAll();
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

void KAccessNodeEditorDlg::DelSelectID( void )
{
	bool bRefresh(false);
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
			pExtItemCheck    = pRecord->GetItem(0);
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
			AfxMessageBox(_T("삭제 가능한 데이터가 존재하지 않습니다."));
		}
        else
        {
            m_bEdited = true;
        }

		
		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawZoneStation(bRefresh);
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


void KAccessNodeEditorDlg::OnBnClickedButtonApply()
{
	if (AfxMessageBox(_T("변경사항을 저장하시겠습니까?"), MB_OKCANCEL) == IDCANCEL)
		return;
	
	try
	{
		std::vector<TAccessStopNodeInfo> vecAccessStopNodeResult;
		int nCurSel		= m_cboMode.GetCurSel();
		int nMode		= m_cboMode.GetItemData(nCurSel);

		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		if (nChecked == IDC_RADIO_ZONE)
		{
			Integer nxZoneID = _ttoi64(m_oSelectIdCaption.TID);
			GetStationApplyData(vecAccessStopNodeResult, nxZoneID);
			KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtStation(m_pTarget, nxZoneID, /*m_oSelectIdCaption.TCaption,*/ nMode, vecAccessStopNodeResult);
		}
		else
		{
			Integer nxNodeID = _ttoi64(m_oSelectIdCaption.TID);
			GetZoneApplyData(vecAccessStopNodeResult);
			KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtZone(m_pTarget,nxNodeID, nMode, vecAccessStopNodeResult);
		}

        NotifyAccessStopNodeTable();
        m_bEdited = false;

		AfxMessageBox(_T("변경사항이 저장되었습니다."));
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


void KAccessNodeEditorDlg::GetStationApplyData(std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeResult, Integer nxZoneID)
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
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			int nMode = pItemText->GetItemData();

			pItem = pRecord->GetItem(3);
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
				++iter;
			}

			pItemText					= (CXTPReportRecordItemText*)pRecord->GetItem(4);
			CString strClassInNetwork	= pItemText->GetValue();

			pItem = pRecord->GetItem(5);
			CString strNoofLine = pItem->GetCaption();
			int     nNoofLine	= _ttoi(strNoofLine);

			oAccessNodeInfo.nxNodeID			= nxNodeID;
			oAccessNodeInfo.nMode				= nMode;
			oAccessNodeInfo.strClassInZone		= strClassInZone;
			oAccessNodeInfo.strClassInNetwork	= strClassInNetwork;
			oAccessNodeInfo.nNoLine				= nNoofLine;
            
            auto iterZone = m_mapZoneFullName.find(nxZoneID);
            if (iterZone != m_mapZoneFullName.end())
            {
                oAccessNodeInfo.strZoneName = iterZone->second;
            }

			a_vecAccessStopNodeResult.push_back(oAccessNodeInfo);
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

void KAccessNodeEditorDlg::GetZoneApplyData( std::vector<TAccessStopNodeInfo>& a_vecAccessStopNodeReslut )
{
	a_vecAccessStopNodeReslut.clear();
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
			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString strZoneID = pItemText->GetValue();
			Integer nxZoneID  = _ttoi64(strZoneID);

			pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			int nMode = pItemText->GetItemData();

			pItem = pRecord->GetItem(3);
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
				++iter;
			}

			pItemText					= (CXTPReportRecordItemText*)pRecord->GetItem(4);
			CString strClassInNetwork	= pItemText->GetValue();

            pItem = pRecord->GetItem(5);
            CString strNoofLine = pItem->GetCaption();
            int     nNoofLine = _ttoi(strNoofLine);

			Integer nxNodeID  = _ttoi64(m_oSelectIdCaption.TID);

			oAccessNodeInfo.nxZoneID			= nxZoneID;
			oAccessNodeInfo.nxNodeID			= nxNodeID;
			oAccessNodeInfo.nMode				= nMode;
			oAccessNodeInfo.strClassInZone		= strClassInZone;
			oAccessNodeInfo.strClassInNetwork	= strClassInNetwork;
			oAccessNodeInfo.nNoLine				= nNoofLine;

            auto iterZone = m_mapZoneFullName.find(nxZoneID);
            if (iterZone != m_mapZoneFullName.end())
            {
                oAccessNodeInfo.strZoneName = iterZone->second;
            }

			a_vecAccessStopNodeReslut.push_back(oAccessNodeInfo);
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


void KAccessNodeEditorDlg::OnBnClickedCancel()
{
	try
	{		
		if (m_oSelectIdCaption.TID != _T("") && m_bEdited == true)
		{
			if (AfxMessageBox(_T("현재 변경정보를 저장하시겠습니까?"), MB_YESNO) == IDYES)
			{
				std::vector<TAccessStopNodeInfo> vecAccessStopNodeResult;
				int nCurSel		= m_cboMode.GetCurSel();
				int nMode		= m_cboMode.GetItemData(nCurSel);

				int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
				if (nChecked == IDC_RADIO_ZONE)
				{
					Integer nxZoneID = _ttoi64(m_oSelectIdCaption.TID);
					GetStationApplyData(vecAccessStopNodeResult, nxZoneID);
					KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtStation(m_pTarget, nxZoneID, /*m_oSelectIdCaption.TCaption,*/ nMode, vecAccessStopNodeResult);
				}
				else
				{
					Integer nxNodeID = _ttoi64(m_oSelectIdCaption.TID);
					GetZoneApplyData(vecAccessStopNodeResult);
					KDBaseAccessibilityModify::ApplyAccessStopNodeEditAtZone(m_pTarget,nxNodeID, nMode, vecAccessStopNodeResult);
				}

				AfxMessageBox(_T("저장하였습니다."));
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

	CloseWindowProc();

	KResizeDialogEx::OnCancel();
}

void KAccessNodeEditorDlg::CloseWindowProc()
{
	m_pMapView->ClearScreenDrawEdit(false);
	m_pMapView->TxClearCompositeThemes(false);
	m_pMapView->MapRefresh();

	if (nullptr != m_spImTasIDFind)
		m_spImTasIDFind.reset();
}

void KAccessNodeEditorDlg::OnCbnSelchangeModeCombo()
{
	bool bRefresh(false);
	try
	{
		if (m_oSelectIdCaption.TID.IsEmpty()) {
			return;
		} else {
			ClearAllMap(bRefresh);
			bRefresh = true;
			UpdateReportZoneStation(bRefresh);
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

void KAccessNodeEditorDlg::OnReportCheckItemNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 0)
		{
			if ( TRUE == pItemNotify->pRow->GetRecord()->GetItem(nIndex)->IsChecked() )
			{
				CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
				CString strNodeID = pItemText->GetValue();
				Integer nxNodeID = _ttoi64(strNodeID);
				m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
			} else {
				CheckDlgButton(IDC_CHECK_ALL, BST_UNCHECKED);
			}
		}
		else if (nIndex == 5)
		{
			if (FALSE == pItemNotify->pRow->GetRecord()->GetItem(nIndex)->IsChecked()) {
				CheckDlgButton(IDC_CHECK_LINEINFO, BST_UNCHECKED);
			}

			CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
			CString strNodeID = pItemText->GetValue();
			Integer nxNodeID = _ttoi64(strNodeID);
			m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);

			int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
			if (nChecked == IDC_RADIO_ZONE)
			{
				GetCheckedLineStation();
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

void KAccessNodeEditorDlg::OnReportItemClickClass( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if (2 == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());
			m_wndReportCtrlClass.RedrawControl();

			bool bRefresh(false);
			GetDisplayInfo();
			ClearAllMap(bRefresh);
			bRefresh = true;
			DrawZoneStation(bRefresh);
		}
	}
}

void KAccessNodeEditorDlg::OnReportItemValueChangedNode( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		bool bRefresh(false);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 3)
		{
			
			ClearAllMap(bRefresh);
			bRefresh = true;
			DrawZoneStation(bRefresh);
            m_bEdited = true;
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

void KAccessNodeEditorDlg::OnReportItemValueChangedClass( NMHDR* a_pNotifyStruct, LRESULT* a_pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)a_pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		bool bRefresh(false);

		int nIndex = pItemNotify->pColumn->GetItemIndex();
		if (nIndex == 2 || nIndex == 3)
		{
			GetDisplayInfo();
			ClearAllMap(bRefresh);
			bRefresh = true;
			DrawZoneStation(bRefresh);
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


BOOL KAccessNodeEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		HWND hwnd = pMsg->hwnd;
		if (hwnd == GetDlgItem(IDC_EDIT_ID)->m_hWnd)
		{
			SeachZoneStation();
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}


void KAccessNodeEditorDlg::OnBnClickedCheckLabel()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_LABEL) == BST_CHECKED)
		{
			m_pMapView->TxDrawCompositeNodeLabel(true);
		}
		else
		{
			m_pMapView->TxDrawCompositeNodeLabel(false);
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

void KAccessNodeEditorDlg::ResizeComponent( void )
{
	SetResize(IDC_STATIC_TARGETYEAR,		SZ_TOP_LEFT,  SZ_TOP_LEFT);
	
	SetResize(IDC_SEPRATOR4,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_CLASS,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);

	SetResize(IDC_SEPRATOR7,				SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_NODE,				SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_LINEINFO,			SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON7,					SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON6,					SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LABEL,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_RADIUS,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_LINE,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	SetResize(IDC_SEPRATOR3,				SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON4,					SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,						SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KAccessNodeEditorDlg::OnBnClickedCheckRadius()
{
	bool bRefresh(false);
	if (IsDlgButtonChecked(IDC_CHECK_RADIUS) == BST_CHECKED)
	{
		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawZoneStation(bRefresh);
	}
	else
	{
		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawZoneStation(bRefresh);
	}

}


void KAccessNodeEditorDlg::OnBnClickedCheckLine()
{
	bool bRefresh(false);
	if (IsDlgButtonChecked(IDC_CHECK_LINE) == BST_CHECKED)
	{
		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawZoneStation(bRefresh);
	}
	else
	{
		ClearAllMap(bRefresh);
		bRefresh = true;
		DrawZoneStation(bRefresh);
	}
}

void KAccessNodeEditorDlg::TransitTableViewInTransitID( std::set<Integer> a_vecTransit )
{
	try
	{
		int nIndex = 0;

		CString strFilter(_T(""));
		strFilter.Format(_T(" transit_id in ( "));

		AutoType iter = a_vecTransit.begin();
		AutoType end  = a_vecTransit.end();
		while(iter != end)
		{
			if (nIndex == 0)
			{
				strFilter.AppendFormat(_T(" %I64d "), *iter);
			}
			else
			{
				strFilter.AppendFormat(_T(" ,%I64d "), *iter);
			}
			nIndex++;
			++iter;
		}
		strFilter.AppendFormat(_T(" ) "));

		KIOTableView* pTableView = ImChampFrameWindow::ShowTableView(m_pTarget, TABLE_TRANSIT);
		//pTableView->OnTransitDeSelectAll();

		pTableView->SetFilter(strFilter);
		pTableView->ReloadData();
		KIOTransitTableView* pTransitTableView = dynamic_cast<KIOTransitTableView*>(pTableView);
		if (pTransitTableView == nullptr)
			return;
		pTransitTableView->OnTransitSelectAll();
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

void KAccessNodeEditorDlg::OnReportItemDoubleClickNode( NMHDR* pNotifyStruct, LRESULT* pResult )
{
	try
	{
		XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
		ASSERT(NULL != pItemNotify);

		int nIndex = pItemNotify->pColumn->GetItemIndex();

		if (nIndex == 1)
		{
			Integer nxNodeID(0);

			int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
			if (nChecked == IDC_RADIO_ZONE)
			{
				CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
				CString strNodeID = pItemText->GetValue();
				nxNodeID = _ttoi64(strNodeID);
			}
			else
			{
				CXTPReportRecordItemText*    pItemText = (CXTPReportRecordItemText*)pItemNotify->pRow->GetRecord()->GetItem(1);
				CString strNodeID = pItemText->GetValue();
				nxNodeID = _ttoi64(strNodeID);
			}

			m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
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

void KAccessNodeEditorDlg::GetCheckedLineStation()
{
	std::set<Integer> setSelcetLineID;
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		CXTPReportRecordItemText*    pItemText	   = nullptr;

		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			if (TRUE == pRecord->GetItem(5)->IsChecked())
			{
				pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
				CString strNodeID = pItemText->GetValue();
				Integer nxNodeID = _ttoi64(strNodeID);

				AutoType find = m_mapStationLineInfo.find(nxNodeID);
				AutoType end  = m_mapStationLineInfo.end();
				if (find != end)
				{
					std::set<Integer> setLineID = find->second;
					AutoType iterLine = setLineID.begin();
					AutoType endLine  = setLineID.end();
					while(iterLine != endLine)
					{
						Integer nxLineID = *iterLine;
						setSelcetLineID.insert(nxLineID);
						iterLine++;
					}
				}
			}
		}

        if (setSelcetLineID.size() > 0) {
		    TransitTableViewInTransitID(setSelcetLineID);
		} else {
			setSelcetLineID.insert(-1); //아무것도 검색이 되지 않도록..
			TransitTableViewInTransitID(setSelcetLineID);
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


void KAccessNodeEditorDlg::OnBnClickedCheckAllLine()
{
	bool bRefresh(false);
	try
	{
		int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
		if (nChecked == IDC_RADIO_STOP)
		{
			if (IsDlgButtonChecked(IDC_CHECK_LINEINFO) == BST_CHECKED)
			{
				Integer nxNodeID = _ttoi64(m_oSelectIdCaption.TID);
				if (m_oSelectIdCaption.TID == _T(""))
				{
					return;
				}

				AutoType find = m_mapStationLineInfo.find(nxNodeID);
				AutoType end  = m_mapStationLineInfo.end();
				if (find != end)
				{
					std::set<Integer> setLineID = find->second;
					if (setLineID.size() == 0 )
					{
						return;
					}
					TransitTableViewInTransitID(setLineID);
					m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);
				}
			}
			else
			{
				ClearAllMap(bRefresh);
				bRefresh = true;
				DrawZoneStation(bRefresh);
			}
		}
		else
		{
			if (IsDlgButtonChecked(IDC_CHECK_LINEINFO) == BST_CHECKED)
			{
				SetCheckAllLineInfo();
				GetCheckedLineStation();
			}
			else
			{
				SetUnCheckAllLineInfo();
				GetCheckedLineStation();
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

void KAccessNodeEditorDlg::SetCheckAllLineInfo()
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pRecord->GetItem(5)->SetChecked(TRUE);
		}
		m_wndReportCtrlNode.Populate();
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

void KAccessNodeEditorDlg::SetUnCheckAllLineInfo()
{
	try
	{
		CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
		CXTPReportRecord*            pRecord       = nullptr;
		CXTPReportRecordItem*        pItem         = nullptr;
		int nRecordCount = pRecords->GetCount();
		for (int i = 0; i <nRecordCount; i++)
		{
			pRecord = pRecords->GetAt(i);
			pRecord->GetItem(5)->SetChecked(FALSE);
		}
		m_wndReportCtrlNode.Populate();
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


void KAccessNodeEditorDlg::OnBnClickedCheckAll()
{
	try
	{
		if (IsDlgButtonChecked(IDC_CHECK_ALL) == BST_CHECKED)
		{
			CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
			CXTPReportRecord*            pRecord       = nullptr;
			CXTPReportRecordItem*        pItem         = nullptr;
			int nRecordCount = pRecords->GetCount();
			for (int i = 0; i <nRecordCount; i++)
			{
				pRecord = pRecords->GetAt(i);
				pRecord->GetItem(0)->SetChecked(TRUE);
			}
			m_wndReportCtrlNode.Populate();
		}
		else
		{
			CXTPReportRecords*           pRecords      = m_wndReportCtrlNode.GetRecords();
			CXTPReportRecord*            pRecord       = nullptr;
			CXTPReportRecordItem*        pItem         = nullptr;
			int nRecordCount = pRecords->GetCount();
			for (int i = 0; i <nRecordCount; i++)
			{
				pRecord = pRecords->GetAt(i);
				pRecord->GetItem(0)->SetChecked(FALSE);
			}
			m_wndReportCtrlNode.Populate();
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


void KAccessNodeEditorDlg::ActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(!m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_SHOW);
		}
	}
}


void KAccessNodeEditorDlg::DeActivatedFrameWndNotifyProcess()
{
	if(nullptr != m_spImTasIDFind)
	{
		if(!m_spImTasIDFind->IsUserClosed())
		{
			if(m_spImTasIDFind->IsWindowVisible())
				m_spImTasIDFind->ShowWindow(SW_HIDE);
		}
	}
}

void KAccessNodeEditorDlg::ModelessCancleFeedback(UINT nDialogID)
{
	try
	{
		bool bRefresh(false);
		if (KImChampIDFind2Column::IDD == nDialogID)
		{
			if (_T("") == m_oSelectIdCaption.TID)
			{
				bRefresh = true;
				ClearAllMap(bRefresh);
				return;
			}
			Integer nxID = _ttoi64(m_oSelectIdCaption.TID);
			CString strID(_T(""));
			strID.Format(_T("%I64d"), nxID);

			CString strName(_T(""));
			int nChecked = GetCheckedRadioButton(IDC_RADIO_ZONE, IDC_RADIO_STOP);
			if (nChecked == IDC_RADIO_ZONE)
			{
				AutoType find = m_mapZoneName.find(nxID);
				AutoType end  = m_mapZoneName.end();
				if (find != end)
				{
					strName = find->second;
				}
			}
			else
			{
				AutoType find = m_mapAllNodeInfo.find(nxID);
				AutoType end  = m_mapAllNodeInfo.end();
				if (find != end)
				{
					TFixedNodeInfo oNode = find->second;
					strName = oNode.name;
				}
			}

			m_oSelectIdCaption.TID	    = strID;
			m_oSelectIdCaption.TCaption = strName;

			CString strCaption(_T(""));
			if (strName == _T(""))
			{
				strCaption.Format(_T("%s[-]"), strID);
			}
			else
			{
				strCaption.Format(_T("%s[%s]"), strID, strName);
			}
			SetDlgItemText(IDC_EDIT_ID, strCaption);

// 			Integer nxNodeID = _ttoi64(m_oSelectIdCaption.TID);
// 			m_pMapView->PanToFeature(KLayerID::Node(), nxNodeID);

			bRefresh = true;
			UpdateReportZoneStation(bRefresh);
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

void KAccessNodeEditorDlg::NotifyAccessStopNodeTable()
{
#pragma region comment
// 기존 테이블이 열려 있을 경우 Redraw할 수 있도록 수정
#pragma endregion comment
    
    try
    {
        KIOView* pIOView = ImChampFrameWindow::GetTableView(m_pMapView);
        if (pIOView == nullptr) return;

        KIOTableView* pIOTableView = pIOView->FindTableView(TABLE_ACCESS_STOP_NODE);
        if (pIOTableView == nullptr) return;

        // 닫힘 : pIOView->CloseTableView(TABLE_ACCESS_STOP_NODE);
        pIOTableView->ReloadData();
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
