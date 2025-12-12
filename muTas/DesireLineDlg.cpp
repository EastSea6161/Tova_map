// DesireLineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DesireLineDlg.h"
#include "afxdialogex.h"

//#include "StringChecker.h"

#include "MapView.h"
#include "Target.h"
//^#include "DBaseConnector.h"
#include "DBaseMatrixView.h"
//#include "StringChecker.h"
#include "ViewChartDlg.h"
// KDesireLineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDesireLineDlg, KResizeDialogEx)

KDesireLineDlg::KDesireLineDlg(KMapView* a_pMapView, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDesireLineDlg::IDD, pParent)
{
	m_pMapView = a_pMapView;
	m_pTarget  = a_pMapView->GetTarget();
}

KDesireLineDlg::~KDesireLineDlg()
{
}

void KDesireLineDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TABLE, m_cboTable);
	DDX_Control(pDX, IDC_COMBO_FIELD, m_cboField);
	DDX_Control(pDX, IDC_COMBO_ORIGIN_UNIT, m_cboOriginUnit);
	DDX_Control(pDX, IDC_COMBO_DES_UNIT, m_cboDestUnit);
	DDX_Control(pDX, IDC_REPORT_ORIGIN, m_wndReportCtrlOrigin);
	DDX_Control(pDX, IDC_REPORT_DEST,   m_wndReportCtrlDest);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_btnColor);
}


BEGIN_MESSAGE_MAP(KDesireLineDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &KDesireLineDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_DRAW, &KDesireLineDlg::OnBnClickedButtonDraw)
	ON_BN_CLICKED(IDCANCEL, &KDesireLineDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_ORIGIN_UNIT, &KDesireLineDlg::OnCbnSelchangeComboOriginUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_DES_UNIT, &KDesireLineDlg::OnCbnSelchangeComboDesUnit)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLE, &KDesireLineDlg::OnCbnSelchangeComboTable)
	ON_BN_CLICKED(IDC_CHECK_ORIGIN, &KDesireLineDlg::OnBnClickedCheckOrigin)
	ON_BN_CLICKED(IDC_CHECK_DESTINATION, &KDesireLineDlg::OnBnClickedCheckDestination)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD, &KDesireLineDlg::OnCbnSelchangeComboField)
	ON_BN_CLICKED(IDC_CHECK1, &KDesireLineDlg::OnBnClickedCheckShowLabel)
END_MESSAGE_MAP()


// KDesireLineDlg 메시지 처리기입니다.
BOOL KDesireLineDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialControl();
	LoadComboField();
	LoadOriginReportData();
	LoadDestReportData();
	ResizeComponent();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDesireLineDlg::ResizeComponent()
{
	SetResize(IDC_SEPRATOR2,				  SZ_TOP_LEFT,    SZ_TOP_RIGHT);
	SetResize(IDC_REPORT_ORIGIN,			  SZ_TOP_LEFT,    SZ_MIDDLE_RIGHT);
	SetResize(IDC_SEPRATOR4,				  SZ_MIDDLE_LEFT, SZ_MIDDLE_RIGHT);
		
	SetResize(IDC_DESTINATION_STATIC,	      SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_COMBO_DES_UNIT,			  SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_CHECK_DESTINATION,		  SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
	SetResize(IDC_REPORT_DEST,				  SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR1,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_CLEAR,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DRAW,				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK1,    				  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDCANCEL,						  SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}


void KDesireLineDlg::InitialControl()
{
	m_cboTable.ResetContent();
	m_cboField.ResetContent();

	KIOTables* pIOTables    = m_pTarget->Tables();
	size_t     nxTableCount = pIOTables->GetCount();

	int nODTableCount = 0;
	for (size_t i=0; i<nxTableCount; i++)
	{
		KIOTable* pIOTable = pIOTables->GetTableAt(i);
		if (pIOTable->TableType() == KEMIOTableTypeODData )
		{
			int nIndex = m_cboTable.AddString(pIOTable->Caption());
			m_cboTable.SetItemData(nIndex, (DWORD_PTR)pIOTable);
			nODTableCount++;
		}
	}

	if (nODTableCount > 0)
	{
		m_cboTable.SetCurSel(0);	
		LoadComboField();
	}

	SetDlgItemText(IDC_EDIT_MIN_WIDTH, _T("1.0"));
	SetDlgItemText(IDC_EDIT_MAX_WIDTH, _T("10.0"));

	m_btnColor.SetColor( RGB(0,128,128) );
	m_btnColor.SetWindowText( _T("") );
	m_btnColor.ShowText( TRUE );
	m_btnColor.ModifyCPStyle( 0, CPS_XT_NOFILL | CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE );

	KIOTable*   pIOTable   = pIOTables->FindTable(TABLE_ZONE);
	const 
    KIOColumns* pIOColumns = pIOTable->Columns();

	int nColumnCount = pIOColumns->ColumnCount();
	int nCount = 0;
	for (int i=0; i<nColumnCount; i++)
	{
		KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
		if (pIOColumn->DataType() ==  KEMIODataTypeInteger && true == pIOColumn->Visible())
		{
			int nIndex = m_cboOriginUnit.AddString(pIOColumn->Caption());
			m_cboOriginUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			nIndex = m_cboDestUnit.AddString(pIOColumn->Caption());
			m_cboDestUnit.SetItemData(nIndex, (DWORD_PTR)pIOColumn);

			nCount++;
		}
	}

	if (nCount > 0)
	{
		m_cboOriginUnit.SetCurSel(0);
		m_cboDestUnit.SetCurSel(0);
	}

	KReportCtrlSetting::Default(m_wndReportCtrlOrigin);
	KReportCtrlSetting::Default(m_wndReportCtrlDest);
	KReportCtrlSetting::AllowColumnsort(m_wndReportCtrlOrigin);
	KReportCtrlSetting::AllowColumnsort(m_wndReportCtrlDest);

	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(0, _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }
	
	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(1, _T("ID"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReportCtrlOrigin.AddColumn(new CXTPReportColumn(2, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	m_wndReportCtrlOrigin.Populate();
		
	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(0, _T("Select"), 20));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(1, _T("ID"),   30));
	pColumn->SetHeaderAlignment(DT_CENTER);
	
	pColumn = m_wndReportCtrlDest.AddColumn(new CXTPReportColumn(2, _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("명칭"));
    }

	m_wndReportCtrlDest.Populate();
}

void KDesireLineDlg::LoadComboField()
{
	m_cboField.ResetContent();

	int nIndex = m_cboTable.GetCurSel();
	if (nIndex < 0) return;

	KIOTable*   pIOTable = (KIOTable*)m_cboTable.GetItemData(nIndex);
	const 
    KIOColumns* pIOColumns = pIOTable->Columns();

	int nColumnCount = pIOColumns->ColumnCount();
	int nFieldCount  = 0;
	for (int i=0; i<nColumnCount; i++)
	{
		KIOColumn* pIOColumn = pIOColumns->GetColumn(i);
		if (pIOColumn->DataType() ==  KEMIODataTypeDouble )
		{
			nIndex = m_cboField.AddString(pIOColumn->Caption());
			m_cboField.SetItemData(nIndex, (DWORD_PTR)pIOColumn);
			nFieldCount++;
		}
	}

	if (nFieldCount > 0)
	{
		m_cboField.SetCurSel(0);
	}
}

void KDesireLineDlg::OnCbnSelchangeComboTable()
{
	m_mapDesireInfo.clear();
	LoadComboField();
}

void KDesireLineDlg::OnCbnSelchangeComboField()
{
	m_mapDesireInfo.clear();
}

void KDesireLineDlg::LoadOriginReportData()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ORIGIN);   
	m_vecDesireOrigin.clear();
	m_wndReportCtrlOrigin.ResetContent();

	int nIndex = m_cboOriginUnit.GetCurSel();
	if (nIndex < 0) return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboOriginUnit.GetItemData(nIndex);

	CString strSQL;
	// Integer - Primary(zone_id), 그냥숫자값, 코드
	if (pIOColumn->PrimaryKey())
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, max(Ta.name), avg(x), avg(y) "), pIOColumn->Name());
		strSQL.Append      (_T(" From Zone Ta, Node Tb "));
		strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}
	else
	{
		strSQL.Append      (_T(" Select "));
		strSQL.AppendFormat(_T("  Ta.%s, Ta.%s, avg(x), avg(y) "), pIOColumn->Name(), pIOColumn->Name());
		strSQL.Append      (_T(" From Zone Ta, Node Tb "));
		strSQL.Append      (_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{        
		bool bCodeType = pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode ? true:false;
		KCodeGroup*      pCodeData    = NULL;
		KCodeManager*    pCodeManager = pIOTable->GetOwner()->GetCodeManager();

		std::map<int,CString> mapCode;
		if ( bCodeType)
		{			
			pCodeData   = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
			pCodeData->GetCodes(mapCode);
		}

		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			CString strID   = spResultSet->GetValueString(0);
			CString strName = spResultSet->GetValueString(1);
			double  dMapX   = spResultSet->GetValueDouble(2);
			double  dMapY   = spResultSet->GetValueDouble(3);
						
			TDesireOrigin oTDesireOrigin;		

			oTDesireOrigin.TID   = _ttoi64(strID);
			oTDesireOrigin.TMapX = dMapX;
			oTDesireOrigin.TMapY = dMapY;
						
			// 명칭
			if (bCodeType)
			{
				std::map<int,CString>::iterator iter = mapCode.find(_ttoi(strID));
				std::map<int,CString>::iterator end  = mapCode.end();

				if (iter != end)
				{
					strName = iter->second;
				}
			}

			oTDesireOrigin.TName = strName;			
			m_vecDesireOrigin.push_back(oTDesireOrigin);
		}

		CXTPReportRecord    * pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		size_t nxCount = m_vecDesireOrigin.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TDesireOrigin& oTDesireOrigin = m_vecDesireOrigin[i];

			pRecord = m_wndReportCtrlOrigin.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&m_vecDesireOrigin[i]);		

			CString strID;
			strID.Format(_T("%I64d"), oTDesireOrigin.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTDesireOrigin.TName));
			pItem->SetAlignment(xtpColumnTextCenter);			
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 
	
	m_wndReportCtrlOrigin.Populate();
}

void KDesireLineDlg::LoadDestReportData()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_DESTINATION);   
	m_vecDesireDest.clear();
	m_wndReportCtrlDest.ResetContent();

	int nIndex = m_cboDestUnit.GetCurSel();
	if (nIndex < 0) return;

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_ZONE);
	KIOColumn* pIOColumn = (KIOColumn*)m_cboDestUnit.GetItemData(nIndex);

	CString strSQL;
	// Integer - Primary(zone_id), 그냥숫자값, 코드
	if (pIOColumn->PrimaryKey())
	{
		strSQL.Append(_T(" Select "));
		strSQL.AppendFormat(_T(" Ta.%s, max(Ta.name), avg(x), avg(y) "), pIOColumn->Name());
		strSQL.Append(_T(" From Zone Ta, Node Tb "));
		strSQL.Append(_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}
	else
	{
		strSQL.Append(_T(" Select "));
		strSQL.AppendFormat(_T(" Ta.%s, Ta.%s, avg(x), avg(y) "), pIOColumn->Name(), pIOColumn->Name());
		strSQL.Append(_T(" From Zone Ta, Node Tb "));
		strSQL.Append(_T(" Where Ta.zone_id = Tb.node_id "));
		strSQL.AppendFormat(_T(" Group By Ta.%s "), pIOColumn->Name());
	}

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	try
	{        
		bool bCodeType = pIOColumn->CodeType() == KEMIOCodeTypeIsSingleCode ? true:false;
		KCodeGroup*      pCodeData    = NULL;
		KCodeManager*    pCodeManager = pIOTable->GetOwner()->GetCodeManager();

		std::map<int,CString> mapCode;
		if ( bCodeType)
		{			
			pCodeData   = pCodeManager->FindCodeGroup(pIOColumn->CodeGroup());
			pCodeData->GetCodes(mapCode);
		}

		KResultSetPtr    spResultSet  = spDBaseConnection->ExecuteQuery(strSQL);
		while ( spResultSet->Next() )
		{
			CString strID   = spResultSet->GetValueString(0);
			CString strName = spResultSet->GetValueString(1);
			double  dMapX   = spResultSet->GetValueDouble(2);
			double  dMapY   = spResultSet->GetValueDouble(3);

			TDesireDest   oTDesireDest;
			
			oTDesireDest.TID   = _ttoi64(strID);
			oTDesireDest.TMapX = dMapX;
			oTDesireDest.TMapY = dMapY;
			oTDesireDest.TRGB  = m_btnColor.GetColor();

			// 명칭
			if (bCodeType)
			{
				std::map<int,CString>::iterator iter = mapCode.find(_ttoi(strID));
				std::map<int,CString>::iterator end  = mapCode.end();

				if (iter != end)
				{
					strName = iter->second;
				}
			}

			oTDesireDest.TName = strName;	
			m_vecDesireDest.push_back(oTDesireDest);
		}

		CXTPReportRecord    * pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		size_t nxCount = m_vecDesireDest.size();
		for (size_t i=0; i<nxCount; i++)
		{
			TDesireDest& oTDesireDest = m_vecDesireDest[i];

			pRecord = m_wndReportCtrlDest.AddRecord(new CXTPReportRecord());

			pItem   = pRecord->AddItem(new CXTPReportRecordItem);
			pItem->HasCheckbox(TRUE);
			pItem->SetChecked(bCheck);
			pItem->SetAlignment(xtpColumnIconCenter);
			pItem->SetItemData((DWORD_PTR)&m_vecDesireDest[i]);

			CString strID;
			strID.Format(_T("%I64d"), oTDesireDest.TID);
			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strID));
			pItem->SetAlignment(xtpColumnTextCenter);

			pItem   = pRecord->AddItem(new CXTPReportRecordItemText(oTDesireDest.TName));
			pItem->SetAlignment(xtpColumnTextCenter);
		}
	}
	catch (KExceptionPtr ex)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	}
	catch (...)
	{
		AfxMessageBox(_T("존 정보 조회 중 오류가 발생하였습니다."));
		return;
	} 

	m_wndReportCtrlDest.Populate();
}

void KDesireLineDlg::OnCbnSelchangeComboOriginUnit()
{
	LoadOriginReportData();
}


void KDesireLineDlg::OnCbnSelchangeComboDesUnit()
{
	LoadDestReportData();
}

void KDesireLineDlg::OnBnClickedButtonClear()
{
	try
	{
		m_pMapView->ClearDesireThemes();
	}
	catch(...)
	{
	}	
}


void KDesireLineDlg::OnBnClickedButtonDraw()
{
	UpdateData(TRUE);

	CWaitCursor wc;

	try
	{
		std::vector<TDesireMapData> vecDesireMapData;
		GetDesireData(vecDesireMapData);
		if (vecDesireMapData.size() < 1)
		{
			throw 1;
		}

		bool bShowLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bShowLabel = true;
		else
			bShowLabel = false;

		m_pMapView->ShowDesireThemes(vecDesireMapData, bShowLabel);
	}
	catch(...)
	{
		return;
	}	
#pragma region old
// 	CWaitCursor wc;
// 
// 	CString strValue;
// 	GetDlgItemText(IDC_EDIT_MIN_WIDTH, strValue);
// 
// 	if (QBicStringChecker::IsNumeric(strValue) == false)
// 	{
// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 		GetDlgItem(IDC_EDIT_MIN_WIDTH)->SetFocus();		
// 		return;
// 	}
// 
// 	double dMinWidth = _ttof(strValue);
// 	if (dMinWidth < 0.0)
// 	{
// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 		GetDlgItem(IDC_EDIT_MIN_WIDTH)->SetFocus();		
// 		return;
// 	}
// 	
// 	GetDlgItemText(IDC_EDIT_MAX_WIDTH, strValue);
// 
// 	if (QBicStringChecker::IsNumeric(strValue) == false)
// 	{
// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 		GetDlgItem(IDC_EDIT_MAX_WIDTH)->SetFocus();		
// 		return;
// 	}
// 
// 	double dMaxWidth = _ttof(strValue);
// 	if (dMaxWidth < 0.0)
// 	{
// 		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
// 		GetDlgItem(IDC_EDIT_MAX_WIDTH)->SetFocus();		
// 		return;
// 	}
// 	
// 	int nIndex = 0;	
// 	nIndex = m_cboTable.GetCurSel();
// 	if (nIndex < 0)
// 	{
// 		AfxMessageBox(_T("테이블 정보가 존재하지 않습니다."));
// 		m_cboTable.SetFocus();
// 		return;
// 	}
// 	//#
// 	KIOTable* pODTable = (KIOTable*)m_cboTable.GetItemData(nIndex);
// 
// 	nIndex = m_cboField.GetCurSel();
// 	if (nIndex < 0)
// 	{
// 		AfxMessageBox(_T("컬럼 정보가 존재하지 않습니다."));
// 		m_cboField.SetFocus();
// 		return;
// 	}
// 	//#
// 	KIOColumn* pODColumn = (KIOColumn*)m_cboField.GetItemData(nIndex);
// 		
// 	int nOriginUnitIndex = m_cboOriginUnit.GetCurSel();
// 	if (nOriginUnitIndex < 0)
// 	{
// 		AfxMessageBox(_T("기점 존 정보가 존재하지 않습니다."));
// 		m_cboOriginUnit.SetFocus();
// 		return;
// 	}
// 	//#
// 	KIOColumn* pOriginUnitColumn = (KIOColumn*)m_cboOriginUnit.GetItemData(nOriginUnitIndex);
// 
// 	int nDestUnitIndex = m_cboDestUnit.GetCurSel();
// 	if (nDestUnitIndex < 0)
// 	{
// 		AfxMessageBox(_T("종점 존 정보가 존재하지 않습니다."));
// 		m_cboDestUnit.SetFocus();
// 		return;
// 	}
// 	//#
// 	KIOColumn* pDestUnitColumn = (KIOColumn*)m_cboDestUnit.GetItemData(nDestUnitIndex);
// 
// 	if (CheckOriginDestinationSelect() == false)
// 	{
// 		return;
// 	}
// 
// 	// 테이블, 필드 선택이 변경되면 클리어 한다.
// 	KODKey oExistKey(nOriginUnitIndex, nDestUnitIndex);
// 	std::map<KODKey, std::map<KODKey, double>>::iterator iterInfo = m_mapDesireInfo.find(oExistKey);
// 	std::map<KODKey, std::map<KODKey, double>>::iterator endInfo  = m_mapDesireInfo.end();
// 
// 	std::map<KODKey, double> mapDesire;
// 	if (iterInfo == endInfo)
// 	{
// 		if (!KDBaseMatrixView::ODMatrixRecord(pODTable, pOriginUnitColumn->Name(), pDestUnitColumn->Name(), pODColumn->Name(), mapDesire))
// 		{
// 			AfxMessageBox(_T("정보 조회중 오류가 발생하였습니다."));
// 			return;
// 		}	
// 
// 		m_mapDesireInfo.insert(std::make_pair(oExistKey, mapDesire));
// 	}
// 	else
// 	{
// 		mapDesire.insert(iterInfo->second.begin(), iterInfo->second.end());
// 	}
// 	
// 	CXTPReportRecords* pOriginRecords = m_wndReportCtrlOrigin.GetRecords();
// 	int nCount = pOriginRecords->GetCount();
// 		
// 	double dMinValue(0.0);
// 	double dMaxValue(0.0);
// 
// 	CalcMinMax(mapDesire, dMinValue, dMaxValue);
// 	double dWidthScale = (dMaxWidth-dMinWidth) / (dMaxValue-dMinValue);
// 
// 	std::vector<TDesireMapData> vecDesireMapData;
// 	for (int i=0; i<nCount; i++)
// 	{
// 		TDesireMapData        oDesireMapData;
// 
// 		CXTPReportRecord*     pRecord = pOriginRecords->GetAt(i);
// 		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);
// 		
// 		/*if (pItem->GetCheckedState() != xtpReportCheckStateChecked)*/
// 		if (pItem->IsChecked() == FALSE)
// 		{
// 			continue;
// 		}
// 
// 		TDesireOrigin* pTDesireOrigin = (TDesireOrigin*)pItem->GetItemData();
// 
// 		oDesireMapData.TOrigin = *pTDesireOrigin;
// 		std::vector<TDesireDest>& vecDestination = oDesireMapData.TVecDestination;
// 
// 		CXTPReportRecords* pDestRecords = m_wndReportCtrlDest.GetRecords();
// 		int nDestCount   = pDestRecords->GetCount();
// 		for (int iDest=0; iDest<nDestCount; iDest++)
// 		{
// 			CXTPReportRecord*     pDestRecord = pDestRecords->GetAt(iDest);
// 			CXTPReportRecordItem* pDestItem   = pDestRecord->GetItem(0);
// 			
// 			/*if (pDestItem->GetCheckedState() != xtpReportCheckStateChecked)*/
// 			if (pDestItem->IsChecked() == FALSE)
// 			{
// 				continue;
// 			}
// 
// 			TDesireDest* pTDesireDest = (TDesireDest*)pDestItem->GetItemData();
// 			
// 			std::map<KODKey, double>::iterator iter = mapDesire.find(KODKey(pTDesireOrigin->TID, pTDesireDest->TID));
// 			std::map<KODKey, double>::iterator end  = mapDesire.end();
// 			
// 			if (iter != end)
// 			{
// 				double dValue = iter->second;
// 				pTDesireDest->TValue = dValue;
// 				pTDesireDest->TRGB   = m_btnColor.GetColor();
// 				pTDesireDest->TWidth = dWidthScale * dValue;
// 
// 				vecDestination.push_back(*pTDesireDest);
// 			}
// 		}
// 
// 		vecDesireMapData.push_back(oDesireMapData);
// 	}	
// 
// 	if (vecDesireMapData.size() < 1)
// 	{
// 		return;
// 	}
// 
// 	try
// 	{
// 		m_pMapView->ShowDesireThemes(vecDesireMapData);
// 	}
// 	catch(...)
// 	{
// 
// 	}	
#pragma endregion old
}


void KDesireLineDlg::GetDesireData(std::vector<TDesireMapData> &a_vecDesireMapData)
{
	if ( !InvalidateInputInfo())
	{
		return;
	}

	CString strValue(_T(""));

	GetDlgItemText(IDC_EDIT_MIN_WIDTH, strValue);
	double dMinWidth = _ttof(strValue);

	GetDlgItemText(IDC_EDIT_MAX_WIDTH, strValue);
	double dMaxWidth = _ttof(strValue);

	int nIndex = 0;	

	nIndex = m_cboTable.GetCurSel();
	KIOTable* pODTable           = (KIOTable*)m_cboTable.GetItemData(nIndex);

	nIndex = m_cboField.GetCurSel();
	KIOColumn* pODColumn         = (KIOColumn*)m_cboField.GetItemData(nIndex);

	int nOriginUnitIndex = m_cboOriginUnit.GetCurSel();
	KIOColumn* pOriginUnitColumn = (KIOColumn*)m_cboOriginUnit.GetItemData(nOriginUnitIndex);

	int nDestUnitIndex = m_cboDestUnit.GetCurSel();
	KIOColumn* pDestUnitColumn   = (KIOColumn*)m_cboDestUnit.GetItemData(nDestUnitIndex);

	if (CheckOriginDestinationSelect() == false)
	{
		return;
	}

	// 테이블, 필드 선택이 변경되면 클리어 한다.
	KODKey oExistKey(nOriginUnitIndex, nDestUnitIndex);
	std::map<KODKey, std::map<KODKey, double>>::iterator iterInfo = m_mapDesireInfo.find(oExistKey);
	std::map<KODKey, std::map<KODKey, double>>::iterator endInfo  = m_mapDesireInfo.end();

	std::map<KODKey, double> mapDesire;
	if (iterInfo == endInfo)
	{
		if (!KDBaseMatrixView::ODMatrixRecord(pODTable, pOriginUnitColumn->Name(), pDestUnitColumn->Name(), pODColumn->Name(), mapDesire))
		{
			AfxMessageBox(_T("정보 조회중 오류가 발생하였습니다."));
			return;
		}	

		m_mapDesireInfo.insert(std::make_pair(oExistKey, mapDesire));
	}
	else
	{
		mapDesire.insert(iterInfo->second.begin(), iterInfo->second.end());
	}

	CXTPReportRecords* pOriginRecords = m_wndReportCtrlOrigin.GetRecords();
	int nOrgCount    = pOriginRecords->GetCount();

	CXTPReportRecords* pDestRecords = m_wndReportCtrlDest.GetRecords();
	int nDestCount   = pDestRecords->GetCount();

	double dMinValue(0.0);
	double dMaxValue(0.0);

	CalcMinMax(mapDesire, dMinValue, dMaxValue);

	TDesireSettingInfo oSettingInfo;
	oSettingInfo.dMinSize = dMinWidth;
	oSettingInfo.dMaxSize = dMaxWidth;
	oSettingInfo.dMinTrip = dMinValue;
	oSettingInfo.dMaxTrip = dMaxValue;

	for (int i=0; i<nOrgCount; i++)
	{
		TDesireMapData        oDesireMapData;

		CXTPReportRecord*     pRecord = pOriginRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		/*if (pItem->GetCheckedState() != xtpReportCheckStateChecked)*/
		if (pItem->IsChecked() == FALSE)
		{
			continue;
		}

		TDesireOrigin* pTDesireOrigin = (TDesireOrigin*)pItem->GetItemData();

		oDesireMapData.TOrigin = *pTDesireOrigin;
		std::vector<TDesireDest>& vecDestination = oDesireMapData.TVecDestination;

		for (int iDest=0; iDest<nDestCount; iDest++)
		{
			CXTPReportRecord*     pDestRecord = pDestRecords->GetAt(iDest);
			CXTPReportRecordItem* pDestItem   = pDestRecord->GetItem(0);

			/*if (pDestItem->GetCheckedState() != xtpReportCheckStateChecked)*/
			if (pDestItem->IsChecked() == FALSE)
			{
				continue;
			}

			TDesireDest* pTDesireDest = (TDesireDest*)pDestItem->GetItemData();

			std::map<KODKey, double>::iterator iter = mapDesire.find(KODKey(pTDesireOrigin->TID, pTDesireDest->TID));
			std::map<KODKey, double>::iterator end  = mapDesire.end();

			if (iter != end)
			{
				double dValue = iter->second;
				pTDesireDest->TValue = dValue;
				pTDesireDest->TRGB   = m_btnColor.GetColor();
				double dWidth(0.0);
				CalculateValue2Width(dValue, oSettingInfo, dWidth);
				pTDesireDest->TWidth = dWidth;

				vecDestination.push_back(*pTDesireDest);
			}
		}

		a_vecDesireMapData.push_back(oDesireMapData);
	}	
}


void KDesireLineDlg::CalculateValue2Width(double a_dTrip, TDesireSettingInfo &a_oSettingInfo, double& dWidth )
{
	if (a_oSettingInfo.dMaxTrip == a_oSettingInfo.dMinTrip)
	{
		/*dWidth = a_oSettingInfo.dMinSize;*/
		dWidth = a_oSettingInfo.dMaxSize;
	}
	else
	{
		dWidth = ( a_oSettingInfo.dMaxSize - a_oSettingInfo.dMinSize ) / (a_oSettingInfo.dMaxTrip - a_oSettingInfo.dMinTrip) * (a_dTrip - a_oSettingInfo.dMinTrip) + a_oSettingInfo.dMinSize;
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


bool KDesireLineDlg::InvalidateInputInfo()
{
	CString strValue(_T(""));
	GetDlgItemText(IDC_EDIT_MIN_WIDTH, strValue);

	if (QBicStringChecker::IsNumeric(strValue) == false)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MIN_WIDTH)->SetFocus();		
		return false;
	}

	double dMinWidth = _ttof(strValue);
	if (dMinWidth < 0.0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MIN_WIDTH)->SetFocus();		
		return false;
	}

	GetDlgItemText(IDC_EDIT_MAX_WIDTH, strValue);

	if (QBicStringChecker::IsNumeric(strValue) == false)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MAX_WIDTH)->SetFocus();		
		return false;
	}

	double dMaxWidth = _ttof(strValue);
	if (dMaxWidth < 0.0)
	{
		AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
		GetDlgItem(IDC_EDIT_MAX_WIDTH)->SetFocus();		
		return false;
	}

	if (m_cboTable.GetCurSel() < 0)
	{
		AfxMessageBox(_T("테이블 정보가 존재하지 않습니다."));
		m_cboTable.SetFocus();
		return false;
	}
	//#

	if (m_cboField.GetCurSel() < 0)
	{
		AfxMessageBox(_T("컬럼 정보가 존재하지 않습니다."));
		m_cboField.SetFocus();
		return false;
	}
	//#

	if (m_cboOriginUnit.GetCurSel() < 0)
	{
		AfxMessageBox(_T("기점 존 정보가 존재하지 않습니다."));
		m_cboOriginUnit.SetFocus();
		return false;
	}
	//#

	if (m_cboDestUnit.GetCurSel() < 0)
	{
		AfxMessageBox(_T("종점 존 정보가 존재하지 않습니다."));
		m_cboDestUnit.SetFocus();
		return false;
	}
	//#

	return true;
}


void KDesireLineDlg::OnBnClickedCancel()
{
    try
    {
        m_pMapView->ClearDesireThemes();
    }
    catch(...)
    {
        return;
    }	

	KResizeDialogEx::OnCancel();
}


void KDesireLineDlg::OnBnClickedCheckOrigin()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_ORIGIN);    

	CXTPReportRecords* pRecords = m_wndReportCtrlOrigin.GetRecords();
	int nCount = pRecords->GetCount();

	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);
	}
	m_wndReportCtrlOrigin.RedrawControl();
}


void KDesireLineDlg::OnBnClickedCheckDestination()
{
	BOOL bCheck = IsDlgButtonChecked(IDC_CHECK_DESTINATION);    

	CXTPReportRecords* pRecords = m_wndReportCtrlDest.GetRecords();
	int nCount = pRecords->GetCount();

	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
		pItem->SetChecked(bCheck);
	}
	m_wndReportCtrlDest.RedrawControl();
}


bool KDesireLineDlg::CheckOriginDestinationSelect()
{
	int nCount = 0;
	int nSelectedCount = 0;
	CXTPReportRecords* pRecords = NULL;
	
	pRecords = m_wndReportCtrlOrigin.GetRecords();
	nCount   = pRecords->GetCount();
	nSelectedCount = 0;
	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked() == TRUE)
		{
			nSelectedCount ++;
		}
	}

	if (nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택된 기점 정보가 존재하지 않습니다."));
		return false;
	}

	pRecords = m_wndReportCtrlDest.GetRecords();
	nCount   = pRecords->GetCount();
	nSelectedCount = 0;
	for (int i=0; i<nCount; i++)
	{
		CXTPReportRecord*     pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

		if (pItem->IsChecked() == TRUE)
		{
			nSelectedCount ++;
		}
	}

	if (nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택된 종점 정보가 존재하지 않습니다."));
		return false;
	}

	return true;
}


void KDesireLineDlg::CalcMinMax( std::map<KODKey, double>& a_mapDesire, double& a_dMinValue, double& a_dMaxValue )
{
	try
	{
		a_dMinValue = 0.0;
		a_dMaxValue = 0.0;

		CXTPReportRecords* pRecords = NULL;

		pRecords = m_wndReportCtrlOrigin.GetRecords();
		int nCount   = pRecords->GetCount();
		std::vector<Integer> vecOrigin;

		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

			if (pItem->IsChecked() == TRUE)
			{
				TDesireOrigin* pTDesireOrigin = (TDesireOrigin*)pItem->GetItemData();
				vecOrigin.push_back(pTDesireOrigin->TID);
			}
		}

		pRecords = m_wndReportCtrlDest.GetRecords();
		nCount   = pRecords->GetCount();
		std::vector<Integer> vecDest;

		for (int i=0; i<nCount; i++)
		{
			CXTPReportRecord*     pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem   = pRecord->GetItem(0);

			if (pItem->IsChecked() == TRUE)
			{
				TDesireDest* pTDesireDest = (TDesireDest*)pItem->GetItemData();
				vecDest.push_back(pTDesireDest->TID);
			}
		}

		double dTempMin = 99999999999;
		double dTempMax = -1.0 * dTempMin;

		size_t nxOriginCount = vecOrigin.size();
		size_t nxDestCount   = vecDest.size();

		if (nxOriginCount == 0 || nxDestCount == 0)
		{
			return;
		}

		std::map<KODKey, double>::iterator iter ;
		std::map<KODKey, double>::iterator end  = a_mapDesire.end();

		for (size_t i=0; i<nxOriginCount; i++)
		{
			Integer nxOriginID = vecOrigin[i];

			for (size_t m=0; m<nxDestCount; m++)
			{
				Integer nxDestID = vecDest[m];
				if (nxOriginID == nxDestID)
				{
					continue;
				}

				KODKey oKey(nxOriginID, nxDestID);
				iter = a_mapDesire.find(oKey);
				if (iter != end)
				{
					double dValue = iter->second;
					if (dTempMin > dValue)
					{
						dTempMin = dValue;
					}

					if (dTempMax < dValue)
					{
						dTempMax = dValue;
					}
				}
			}
		}

		a_dMinValue = dTempMin;
		a_dMaxValue = dTempMax;
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KDesireLineDlg::OnBnClickedCheckShowLabel()
{
	try
	{
		bool bShowLabel(false);
		if (IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
			bShowLabel = true;
		else
			bShowLabel = false;

		m_pMapView->ShowDesireLabel(bShowLabel);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
