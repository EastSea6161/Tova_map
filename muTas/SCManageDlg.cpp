// SCManageDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SCManageDlg.h"
#include "Target.h"
//^^ #include "IOTable.h"
#include "MapView.h"
//^#include "DBaseConnector.h"
#include "..\QBicCommon\ColorRangeGen.h"
#include "KExRecordItem.h"
//^#include "MunsellColorCircle.h"
#include "BulkDBaseScreenCordon.h"

enum 
{
	_0_COLUMN_CHECK = 0,
	_1_COLUMN_ID,
	_2_COLUMN_NAME,
	_3_COLUMN_COLOR
};


#define MunsellColorIndex 0

// KSCManageDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KSCManageDlg, KResizeDialogEx)

KSCManageDlg::KSCManageDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KSCManageDlg::IDD, pParent)
{
}

KSCManageDlg::~KSCManageDlg()
{
	CloseWindowProc();
}

void KSCManageDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT,  m_ScreenReportCtrl);
	DDX_Control(pDX, IDC_REPORT3, m_CordonReportCtrl);

	DDX_Control(pDX, IDC_CBO_COLORRAMP,  m_cboScreenColorRamp);
	DDX_Control(pDX, IDC_CBO_COLORRAMP2, m_cboCordonColorRamp);
}


BEGIN_MESSAGE_MAP(KSCManageDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDCANCEL, &KSCManageDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_CBO_COLORRAMP,  &KSCManageDlg::OnCbnSelchangeCboScreenColorramp)
	ON_CBN_SELCHANGE(IDC_CBO_COLORRAMP2, &KSCManageDlg::OnCbnSelchangeCboCordonColorramp)
	ON_COMMAND(ID_SELECT_CANCEL, &KSCManageDlg::OnScreenSelectCancel)
	ON_COMMAND(ID_SELECT_ALL,    &KSCManageDlg::OnScreenSelectAll)
	ON_COMMAND(ID_SELECT_CANCEL_SECOND, &KSCManageDlg::OnCordonSelectCancel)
	ON_COMMAND(ID_SELECT_ALL_SECOND,    &KSCManageDlg::OnCordonSelectAll)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT,  OnScreenReportColumnRButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, IDC_REPORT3, OnCordonReportColumnRButtonClick)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT,  OnScreenReportCheckItem)
	ON_NOTIFY(XTP_NM_REPORT_CHECKED,       IDC_REPORT3, OnCordonReportCheckItem)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT,  OnScreenReportItemClick)
	ON_NOTIFY(NM_CLICK,                    IDC_REPORT3, OnCordonReportItemClick)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED,    IDC_REPORT,  &KSCManageDlg::OnScreenSelChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,  IDC_REPORT,  &KSCManageDlg::OnScreenValueChanged)
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED,    IDC_REPORT3, &KSCManageDlg::OnCordonSelChanged)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,  IDC_REPORT3, &KSCManageDlg::OnCordonValueChanged)

	ON_BN_CLICKED(IDC_BUTTON1, &KSCManageDlg::OnBnClickedButtonDelScreenLine)
	ON_BN_CLICKED(IDC_BUTTON2, &KSCManageDlg::OnBnClickedButtonDelCordonLine)
	ON_BN_CLICKED(IDC_BUTTON3, &KSCManageDlg::OnBnClickedButtonSaveScreen)
	ON_BN_CLICKED(IDC_BUTTON4, &KSCManageDlg::OnBnClickedButtonSaveCordon)
	ON_BN_CLICKED(IDC_BUTTON5, &KSCManageDlg::OnBnClickedButtonRefreshScreen)
	ON_BN_CLICKED(IDC_BUTTON6, &KSCManageDlg::OnBnClickedButtonRefreshCordon)
END_MESSAGE_MAP()


void KSCManageDlg::CloseWindowProc()
{
	m_pMapView->ClearScreenLineDraw(false);
	m_pMapView->ClearCordonLineDraw(true);
}

void KSCManageDlg::UpdateDrawState()
{
	m_pMapView->ClearScreenLineDraw(false);
	m_pMapView->ClearCordonLineDraw(true);

	try
	{
		CXTPReportRecord*      pLastSelectedRecord = NULL;
		CXTPReportRecord*      pRecord  = nullptr;
		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_ScreenReportCtrl.GetRecords();
		int nScreenRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nScreenRecordCnt; i++)
		{
			pRecord   = pRecords->GetAt(i);
			pItem     = pRecord->GetItem(_0_COLUMN_CHECK);
			if( TRUE == pItem->IsChecked() )
			{
				pLastSelectedRecord = pRecord;
				ControlScreenLineDraw(pRecord, TRUE, false);
			}
		}

		// 한번의 redraw 를 위한 강제 redraw 구문
		if(NULL != pLastSelectedRecord) {
			ControlScreenLineDraw(pLastSelectedRecord, TRUE, true);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}

	try
	{
		CXTPReportRecord*      pLastSelectedRecord = NULL;
		CXTPReportRecord*      pRecord  = nullptr;
		CXTPReportRecordItem*  pItem    = nullptr;
		CXTPReportRecords*     pRecords = m_CordonReportCtrl.GetRecords();
		int nCordonRecordCnt = pRecords->GetCount();

		for (int i= 0; i< nCordonRecordCnt; i++)
		{
			pRecord   = pRecords->GetAt(i);
			pItem     = pRecord->GetItem(_0_COLUMN_CHECK);
			if( TRUE == pItem->IsChecked() )
			{
				pLastSelectedRecord = pRecord;
				ControlCordonLineDraw(pRecord, TRUE, false);
			}
		}

		// 한번의 redraw 를 위한 강제 redraw 구문
		if(NULL != pLastSelectedRecord) {
			ControlCordonLineDraw(pLastSelectedRecord, TRUE, true);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

// KSCManageDlg 메시지 처리기입니다.

BOOL KSCManageDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitialColorRampCtrl();
	
	KReportCtrlSetting::Default(m_ScreenReportCtrl, TRUE, FALSE, FALSE); 
	KReportCtrlSetting::AllowColumnsort(m_ScreenReportCtrl);
	KReportCtrlSetting::Default(m_CordonReportCtrl, TRUE, FALSE, FALSE); 
	KReportCtrlSetting::AllowColumnsort(m_CordonReportCtrl);

	InitReportHeader();
	InitReportData();
// 	m_ScreenReportCtrl.Populate();
// 	m_CordonReportCtrl.Populate();

	SetResizeComponent();

	OnBnClickedButtonRefreshScreen();
	OnBnClickedButtonRefreshCordon();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KSCManageDlg::SetResizeComponent()
{
	
	SetResize(IDC_CBO_COLORRAMP,      SZ_TOP_RIGHT,     SZ_TOP_RIGHT);
	SetResize(IDC_REPORT,             SZ_TOP_LEFT,      SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON5,            SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);
	SetResize(IDC_BUTTON1,            SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT);
	SetResize(IDC_BUTTON3,            SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT);
	SetResize(IDC_SEPRATOR6,          SZ_MIDDLE_LEFT,   SZ_MIDDLE_RIGHT);

	SetResize(IDC_STATIC_CORDON,      SZ_MIDDLE_LEFT,   SZ_MIDDLE_LEFT);
	SetResize(IDC_CBO_COLORRAMP2,     SZ_MIDDLE_RIGHT,  SZ_MIDDLE_RIGHT);
	SetResize(IDC_REPORT3,            SZ_MIDDLE_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON6,            SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON2,            SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON4,            SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	
	SetResize(IDC_SEPRATOR,           SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(IDOK,                   SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,               SZ_BOTTOM_RIGHT,  SZ_BOTTOM_RIGHT);
}


void KSCManageDlg::InitialColorRampCtrl()
{
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_COLOR_RAMP_B);
	m_colorRampImageList.Create(200, 18, ILC_COLOR24, 6, 6);
	m_colorRampImageList.Add(&bmp, RGB(0, 0, 0));

	m_cboScreenColorRamp.SetImageList(&m_colorRampImageList);
	m_cboCordonColorRamp.SetImageList(&m_colorRampImageList);

	COMBOBOXEXITEM cbItem;
	::RtlSecureZeroMemory(&cbItem, sizeof(cbItem));
	cbItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE ;

	for ( int i=0; i<6; i++ )
	{
		cbItem.iItem = i;
		cbItem.iImage = i;
		cbItem.iSelectedImage = i;

		m_cboScreenColorRamp.InsertItem(&cbItem);
		m_cboCordonColorRamp.InsertItem(&cbItem);
	}

	m_cboScreenColorRamp.SetCurSel(0);
	m_cboCordonColorRamp.SetCurSel(0);
}


void KSCManageDlg::InitReportHeader()
{
	InitScreenReportHeader();
	InitCordonReportHeader();
}


void KSCManageDlg::InitScreenReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ScreenReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_ScreenReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_ID,         _T("ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_ScreenReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME,       _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("이름"));
    }

	pColumn = m_ScreenReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_COLOR,      _T("Color"), 80, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }
}


void KSCManageDlg::InitCordonReportHeader()
{
	CXTPReportColumn* pColumn = NULL;

	pColumn = m_CordonReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_CHECK,      _T("Select"), 40, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("선택"));
    }

	pColumn = m_CordonReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_ID,         _T("ID"), 40));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(FALSE);

	pColumn = m_CordonReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME,       _T("Name"), 60));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetEditable(TRUE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("이름"));
    }

	pColumn = m_CordonReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_COLOR,      _T("Color"), 80, FALSE));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
    if (KmzSystem::GetLanguage() == KEMKorea) {
        pColumn->SetCaption(_T("색상"));
    }
}


void KSCManageDlg::InitReportData()
{
	InitScreenReportData(true);
	InitCordonReportData(true);
}


void KSCManageDlg::InitScreenReportData(bool a_bColorRefresh)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT ID, Name FROM %s WHERE TYPE = %d "), TABLE_SCREEN_CORDON, TYPE_SCREEN);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	int nIdx = 0;
	int nColorIdx = 0;
	std::set<Integer>::iterator iterKey;

	KMunsellColor* pMunSellColor = new KMunsellColor(0, false);

	while( spResult->Next() )
	{
		nColorIdx++;
		Integer nxID	    = spResult->GetValueInt64 (0);
		CString strName     = spResult->GetValueString(1);

		iterKey = m_setScreenKey.find(nxID);
		if( iterKey != m_setScreenKey.end() )
		{
			continue;
		}
		
		m_setScreenKey.insert(nxID);

		pRecord = m_ScreenReportCtrl.AddRecord(new CXTPReportRecord());
		
		// - 선택
		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)nxID);
		
		// - ID
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxID, _T("%.0f")));
		/*pItem->SetItemData((DWORD_PTR) iter->first);*/
		pItem->SetAlignment(xtpColumnTextCenter);
		
		// - 명칭
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strName ));
		pItem->SetAlignment(xtpColumnTextLeft);

		COLORREF   colorRef = pMunSellColor->GetColor(nColorIdx);
		// - 색상 
		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->SetBackgroundColor(colorRef);

		++nIdx;
	}
	
	m_ScreenReportCtrl.Populate();

	if(a_bColorRefresh)
	{
		SetColorRampInfo(TYPE_SCREEN, m_cboScreenColorRamp.GetCurSel());
	}

	TxLogDebugEndMsg();
}


void KSCManageDlg::InitCordonReportData(bool a_bColorRefresh)
{
	TxLogDebugStartMsg();

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();

	CString strSQL;
	strSQL.Format (_T(" SELECT ID, Name FROM %s WHERE TYPE = %d "), TABLE_SCREEN_CORDON, TYPE_CORDON);

	KResultSetPtr spResult = spDBaseConnection->ExecuteQuery(strSQL);
	if(nullptr == spResult)
	{
		return;
	}

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;
	int nIdx = 0;
	int nColorIdx = 0;
	std::set<Integer>::iterator iterKey;

	KMunsellColor* pMunSellColor = new KMunsellColor(0, false);

	while( spResult->Next() )
	{
		nColorIdx++;
		Integer nxID	    = spResult->GetValueInt64 (0);
		CString strName     = spResult->GetValueString(1);

		iterKey = m_setCordonKey.find(nxID);
		if( iterKey != m_setCordonKey.end() )
		{
			continue;
		}

		m_setCordonKey.insert(nxID);

		pRecord = m_CordonReportCtrl.AddRecord(new CXTPReportRecord());

		// - 선택
		pItem = pRecord->AddItem(new KExRecordItemCheck());
		pItem->HasCheckbox(TRUE);
		pItem->SetChecked(FALSE);
		pItem->SetAlignment(xtpColumnIconCenter);
		pItem->SetItemData((DWORD_PTR)nxID);

		// - ID
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber((double)nxID, _T("%.0f")));
		/*pItem->SetItemData((DWORD_PTR) iter->first);*/
		pItem->SetAlignment(xtpColumnTextCenter);

		// - 명칭
		pItem = pRecord->AddItem(new CXTPReportRecordItemText( strName ));
		pItem->SetAlignment(xtpColumnTextLeft);

		COLORREF   colorRef = pMunSellColor->GetColor(nColorIdx);
		// - 색상 
		pItem = pRecord->AddItem(new CXTPReportRecordItem());
		pItem->SetBackgroundColor(colorRef);

		++nIdx;
	}

	m_CordonReportCtrl.Populate();

	if(a_bColorRefresh)
	{
		SetColorRampInfo(TYPE_CORDON, m_cboCordonColorRamp.GetCurSel());
	}

	TxLogDebugEndMsg();
}


void KSCManageDlg::SetColorRampInfo(KEMScreenCordonType a_emScreenCordonType, int nSelectIndex)
{
	CXTPReportControl* pReportCtrl = NULL;
	if(TYPE_SCREEN == a_emScreenCordonType)
	{
		pReportCtrl = &m_ScreenReportCtrl;
	}
	else
	{
		pReportCtrl = &m_CordonReportCtrl;
	}

	switch (nSelectIndex)
	{
	case 0:
		break;
	case 1 :
		m_clrStart = RGB ( 190, 207, 219 );
		m_clrEnd   = RGB (  84,  93, 100 );
		break;
	case 2 :
		m_clrStart = RGB ( 119, 221, 221 );
		m_clrEnd   = RGB (   8,  38,  67 );
		break;
	case 3 :
		m_clrStart = RGB ( 234, 158,  69 );
		m_clrEnd   = RGB ( 105,   2,   5 );
		break;
	case 4 :
		m_clrStart = RGB ( 255, 255, 102 );
		m_clrEnd   = RGB ( 102,   2, 153 );
		break;
	case 5 :
		m_clrStart = RGB ( 255, 102, 130 );
		m_clrEnd   = RGB (   0,  79, 153 );
		break;
	default:
		return;
		break;
	}

	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords = pReportCtrl->GetRecords();
	int nRecordCount = pRecords->GetCount();

	KColorRangeGen oColorRangeGen;
	oColorRangeGen.setStartColor(m_clrStart);
	oColorRangeGen.setEndColor  (m_clrEnd);
	oColorRangeGen.setSize(nRecordCount);
	oColorRangeGen.createColorRange();

	KMunsellColor* pMunSellColor = new KMunsellColor(0, false);

	Integer  nxID      = -1;
	bool     bSelected = false;
	COLORREF colorRefLastSelected;
	int nTotalRowCount = pReportCtrl->GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow* pRow = pReportCtrl->GetRows()->GetAt(i);
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItem* pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		CXTPReportRecordItem* pItemColor = pRecord->GetItem(_3_COLUMN_COLOR);

		COLORREF   colorRef;
		if( MunsellColorIndex == nSelectIndex )
		{
			colorRef = pMunSellColor->GetColor(i);
		}
		else
		{
			colorRef = oColorRangeGen.getNextColor();       
		}

		pItemColor->SetBackgroundColor(colorRef);

		if(pItemCheck->IsChecked() == TRUE)
		{
			nxID = pItemCheck->GetItemData();
			if(TYPE_SCREEN == a_emScreenCordonType)
			{
				m_pMapView->UpdateScreenLineDraw(nxID, colorRef, false);
			}
			else
			{
				m_pMapView->UpdateCordonLineDraw(nxID, colorRef, false);
			}
			bSelected = true;
			colorRefLastSelected = colorRef;
		}
	}

	// 한번의 redraw 를 위한 강제 redraw 구문
	if(true == bSelected)
	{
		if(TYPE_SCREEN == a_emScreenCordonType)
		{
			m_pMapView->UpdateScreenLineDraw(nxID, colorRefLastSelected, true);
		}
		else
		{
			m_pMapView->UpdateCordonLineDraw(nxID, colorRefLastSelected, true);
		}
	}

	pReportCtrl->RedrawControl();
	delete pMunSellColor;
}


void KSCManageDlg::OnCbnSelchangeCboScreenColorramp()
{
	SetColorRampInfo(TYPE_SCREEN, m_cboScreenColorRamp.GetCurSel());

	this->SetFocus();
}


void KSCManageDlg::OnCbnSelchangeCboCordonColorramp()
{
	SetColorRampInfo(TYPE_CORDON, m_cboCordonColorRamp.GetCurSel());

	this->SetFocus();
}


void KSCManageDlg::OnScreenReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KSCManageDlg::OnCordonReportColumnRButtonClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItem = (XTP_NM_REPORTRECORDITEM*)pNMHDR;
	CMenu menu;
	menu.LoadMenu( IDR_POPUP_SELECT_CHECK_SECOND);
	CMenu* pPopup = menu.GetSubMenu(0);

	CXTPReportColumn* pClickedJeader = pItem->pColumn;
	int nIndex = pClickedJeader->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		CXTPMDIFrameWnd* pFrame = (CXTPMDIFrameWnd*)AfxGetMainWnd();
		CXTPCommandBars* pCommandBars = pFrame->GetCommandBars();
		pCommandBars->TrackPopupMenuEx(pPopup, TPM_RIGHTBUTTON, pItem->pt.x, pItem->pt.y, this);
	}

	*pResult = 0;
}


void KSCManageDlg::OnScreenSelectCancel()
{
	CheckboxControlScreen( FALSE );
}


void KSCManageDlg::OnScreenSelectAll()
{
	CheckboxControlScreen( TRUE );
}


void KSCManageDlg::OnCordonSelectCancel()
{
	CheckboxControlCordon( FALSE );
}


void KSCManageDlg::OnCordonSelectAll()
{
	CheckboxControlCordon( TRUE );
}


void KSCManageDlg::CheckboxControlScreen(BOOL a_bCheck)
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_ScreenReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	bool bRedraw = false;
	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		pItemCheck->SetChecked(a_bCheck);

		if(i == nRecordCount -1)
		{
			bRedraw = true;
		}

		if(a_bCheck == FALSE)// 제거는 all clear 전용 함수 사용
		{
			continue;
		}
		ControlScreenLineDraw(pRecord, a_bCheck, bRedraw);
	}

	if(a_bCheck == FALSE)
	{
		m_pMapView->ClearScreenLineDraw(true);
	}

	m_ScreenReportCtrl.RedrawControl();
}


void KSCManageDlg::CheckboxControlCordon( BOOL a_bCheck )
{
	CXTPReportRecords*	  pRecords    = NULL;
	CXTPReportRecord*     pRecord     = NULL;
	CXTPReportRecordItem* pItemCheck  = NULL;

	pRecords = m_CordonReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	bool bRedraw = false;
	for(int i= 0; i< nRecordCount; i++)
	{
		pRecord    = pRecords->GetAt(i);

		pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		pItemCheck->SetChecked(a_bCheck);

		if(i == nRecordCount -1)
		{
			bRedraw = true;
		}

		if(a_bCheck == FALSE)// 제거는 all clear 전용 함수 사용
		{
			continue;
		}
		ControlCordonLineDraw(pRecord, a_bCheck, bRedraw);
	}

	if(a_bCheck == FALSE)
	{
		m_pMapView->ClearCordonLineDraw(true);
	}

	m_CordonReportCtrl.RedrawControl();
}


void KSCManageDlg::ControlScreenLineDraw(CXTPReportRecord* a_pRecord, BOOL a_bAddNRemove, bool a_bReDraw)
{
	CXTPReportRecordItem* pItemCheck   = a_pRecord->GetItem(_0_COLUMN_CHECK);
	CXTPReportRecordItem* pItemColor   = a_pRecord->GetItem(_3_COLUMN_COLOR);
	COLORREF colorRef   = pItemColor->GetBackgroundColor();
	Integer  nxID       = pItemCheck->GetItemData();

	if(FALSE == a_bAddNRemove)
	{
		m_pMapView->RemoveScreenLineDraw(nxID, a_bReDraw);
	}
	else
	{
		m_pMapView->AddScreenLineDraw(nxID, colorRef, a_bReDraw);
	}
}


void KSCManageDlg::ControlCordonLineDraw(CXTPReportRecord* a_pRecord, BOOL a_bAddNRemove, bool a_bReDraw)
{
	CXTPReportRecordItem* pItemCheck   = a_pRecord->GetItem(_0_COLUMN_CHECK);
	CXTPReportRecordItem* pItemColor   = a_pRecord->GetItem(_3_COLUMN_COLOR);
	COLORREF colorRef   = pItemColor->GetBackgroundColor();
	Integer  nxID       = pItemCheck->GetItemData();

	if(FALSE == a_bAddNRemove)
	{
		m_pMapView->RemoveCordonLineDraw(nxID, a_bReDraw);
	}
	else
	{
		m_pMapView->AddCordonLineDraw(nxID, colorRef, a_bReDraw);
	}
}


void KSCManageDlg::OnScreenReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{

	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	CXTPReportRecordItem* pItem   = pItemNotify->pItem;
 	CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();

	ControlScreenLineDraw(pRecord,  pItem->IsChecked());
}


void KSCManageDlg::OnCordonReportCheckItem(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
	ASSERT(NULL != pItemNotify);

	CXTPReportRecordItem* pItem   = pItemNotify->pItem;
	CXTPReportRecord*	  pRecord = pItemNotify->pRow->GetRecord();

	ControlCordonLineDraw(pRecord,  pItem->IsChecked());
}


void KSCManageDlg::OnScreenReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItemCheck   = pRecord->GetItem(_0_COLUMN_CHECK);
	
	int nIndex = pItemNotify->pColumn->GetItemIndex();
	if( _0_COLUMN_CHECK == nIndex)
	{
		return;
	}

	if( _3_COLUMN_COLOR == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());

			if(pItemCheck->IsChecked() == TRUE)
			{
				Integer  nxID       = pItemCheck->GetItemData();
				m_pMapView->UpdateScreenLineDraw(nxID, dlg.GetColor(), true);
			}
		}

		m_ScreenReportCtrl.RedrawControl();
	}
}


void KSCManageDlg::OnCordonReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

	if (!pItemNotify->pRow || !pItemNotify->pColumn)
		return;

	CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
	CXTPReportRecordItem* pItemCheck   = pRecord->GetItem(_0_COLUMN_CHECK);

	int nIndex = pItemNotify->pColumn->GetItemIndex();

	if( _0_COLUMN_CHECK == nIndex)
	{
		return;
	}

	if( _3_COLUMN_COLOR == nIndex)
	{
		CXTColorDialog dlg(pItemNotify->pItem->GetBackgroundColor(), pItemNotify->pItem->GetBackgroundColor());
		if( IDOK == dlg.DoModal())
		{
			pItemNotify->pItem->SetBackgroundColor(dlg.GetColor());

			if(pItemCheck->IsChecked() == TRUE)
			{
				Integer  nxID       = pItemCheck->GetItemData();
				m_pMapView->UpdateCordonLineDraw(nxID, dlg.GetColor(), true);
			}
		}

		m_CordonReportCtrl.RedrawControl();
	}
}


void KSCManageDlg::OnBnClickedCancel()
{
	CloseWindowProc();
	KResizeDialogEx::OnCancel();
}


void KSCManageDlg::OnBnClickedButtonDelScreenLine()
{
	int nSelect = -1;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		nSelect = AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO);
	}
	else {
		nSelect = AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO);
	}
	if( IDNO == nSelect )
	{
		return;
	}

	Integer nxID           = 0;
	int     nSelectedCount = 0;
	int     nTotalRowCount = m_ScreenReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow*        pRow       = m_ScreenReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord*     pRecord    = pRow->GetRecord();
		CXTPReportRecordItem* pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);
		
		if(pItemCheck->IsChecked() == TRUE)
		{
			nSelectedCount++;

			nxID = pItemCheck->GetItemData();
			if(false == KBulkDBaseScreenCordon::DeleteScreenCordon(m_pTarget,  nxID))
			{
				continue;
			}
			ControlScreenLineDraw(pRecord, FALSE, true);
			pRecord->Delete();
		
			std::set<Integer>::iterator itKey = m_setScreenKey.find(nxID);
			if(m_setScreenKey.end() != itKey)
			{
				m_setScreenKey.erase(itKey);
			}
		}
	}

	if(nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 정보가 없습니다."));
		return;
	}

	m_ScreenReportCtrl.Populate();
}


void KSCManageDlg::OnBnClickedButtonDelCordonLine()
{
	int nSelect = -1;
	if (KmzSystem::GetLanguage() == KEMKorea) {
		nSelect = AfxMessageBox(_T("삭제하시겠습니까?"), MB_YESNO);
	}
	else {
		nSelect = AfxMessageBox(_T("Are you sure want to delete?"), MB_YESNO);
	}
	if( IDNO == nSelect )
	{
		return;
	}

	Integer nxID           = 0;
	int     nSelectedCount = 0;
	int     nTotalRowCount = m_CordonReportCtrl.GetRows()->GetCount();
	for(int i= 0; i< nTotalRowCount; i++)
	{
		CXTPReportRow*        pRow       = m_CordonReportCtrl.GetRows()->GetAt(i);
		CXTPReportRecord*     pRecord    = pRow->GetRecord();
		CXTPReportRecordItem* pItemCheck = pRecord->GetItem(_0_COLUMN_CHECK);

		if(pItemCheck->IsChecked() == TRUE)
		{
			nSelectedCount++;

			nxID = pItemCheck->GetItemData();
			if(false == KBulkDBaseScreenCordon::DeleteScreenCordon(m_pTarget,  nxID))
			{
				continue;
			}
			ControlCordonLineDraw(pRecord, FALSE, true);
			pRecord->Delete();

			std::set<Integer>::iterator itKey = m_setCordonKey.find(nxID);
			if(m_setCordonKey.end() != itKey)
			{
				m_setCordonKey.erase(itKey);
			}
		}
	}

	if(nSelectedCount < 1)
	{
		AfxMessageBox(_T("선택한 정보가 없습니다."));
		return;
	}

	m_CordonReportCtrl.Populate();
}


void KSCManageDlg::OnScreenSelChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
// 	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
// 
// 	if (!pItemNotify->pRow || !pItemNotify->pColumn)
// 		return;
// 
// 	CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
// 	CXTPReportRecordItem* pItemTest    = pRecord->GetItem(_2_COLUMN_NAME);
// 	CString strCaption = pItemTest->GetCaption(NULL);
// 
// 	int nIndex = pItemNotify->pColumn->GetItemIndex();
// 	if( _0_COLUMN_CHECK == nIndex)
// 	{
// 		return;
// 	}
// 
// 
// 	CXTPReportRow* pRow = m_ScreenReportCtrl.GetFocusedRow();
// 	if(NULL != pRow)
// 	{
// 		CXTPReportRecord*     pRecord   = pRow->GetRecord();
// 		CXTPReportRecordItem* pItem     = pRecord->GetItem(_2_COLUMN_NAME);
// 		strCaption = pItemTest->GetCaption(NULL);
// 	}
}


void KSCManageDlg::OnScreenValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
 	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CString strName = pItemNotify->pItem->GetCaption(NULL);
// 	TModeManageInfo* pEditInfo = (TModeManageInfo*)(pItemNotify->pItem->GetItemData());
}


void KSCManageDlg::OnCordonSelChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
// 	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
// 
// 	if (!pItemNotify->pRow || !pItemNotify->pColumn)
// 		return;
// 
// 	CXTPReportRecord*	  pRecord      = pItemNotify->pRow->GetRecord();
// 	CXTPReportRecordItem* pItemTest    = pRecord->GetItem(_2_COLUMN_NAME);
// 	CString strCaption = pItemTest->GetCaption(NULL);
// 
// 	int nIndex = pItemNotify->pColumn->GetItemIndex();
// 	if( _0_COLUMN_CHECK == nIndex)
// 	{
// 		return;
// 	}
// 
// 
// 	CXTPReportRow* pRow = m_ScreenReportCtrl.GetFocusedRow();
// 	if(NULL != pRow)
// 	{
// 		CXTPReportRecord*     pRecord   = pRow->GetRecord();
// 		CXTPReportRecordItem* pItem     = pRecord->GetItem(_2_COLUMN_NAME);
// 		strCaption = pItemTest->GetCaption(NULL);
// 	}
}


void KSCManageDlg::OnCordonValueChanged(NMHDR*  pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;

	CString strName = pItemNotify->pItem->GetCaption(NULL);
	// 	TModeManageInfo* pEditInfo = (TModeManageInfo*)(pItemNotify->pItem->GetItemData());
}


void KSCManageDlg::OnBnClickedButtonSaveScreen()
{
	CXTPReportRecords*    pRecords = m_ScreenReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nTotalRecordCount = pRecords->GetCount(); 
	if(nTotalRecordCount < 1)
	{
		return;
	}

	std::vector<TScreenDBInfo> vecScreenInfo;
	for(int i= 0; i< nTotalRecordCount; i++)
	{
		TScreenDBInfo oTSCreenData;

		pRecord = pRecords->GetAt(i);

		pItem                 = pRecord->GetItem(_0_COLUMN_CHECK);
		oTSCreenData.nxID     = pItem->GetItemData();
		pItem                 = pRecord->GetItem(_2_COLUMN_NAME);   
		oTSCreenData.strName  = pItem->GetCaption(NULL);

		vecScreenInfo.push_back(oTSCreenData);
	}

	if( true == KBulkDBaseScreenCordon::UpdateScreenMaster( m_pTarget, vecScreenInfo ) )
	{
		AfxMessageBox(_T("저장하였습니다."));
		ApplyUpdatedScreenInfo();
	}
}


void KSCManageDlg::ApplyUpdatedScreenInfo()
{
	m_pMapView->ClearScreenLineDraw(false);

	CXTPReportRecords*    pRecords = m_ScreenReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecord*     pLastSelectedRecord = NULL;

	int nTotalRecordCount = pRecords->GetCount(); 
	for(int i= 0; i< nTotalRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);
		pItem     = pRecord->GetItem(_0_COLUMN_CHECK);
		if( TRUE == pItem->IsChecked() )
		{
			pLastSelectedRecord = pRecord;
			ControlScreenLineDraw(pRecord, TRUE, false);
		}
	}

	// 한번의 redraw 를 위한 강제 redraw 구문
	if(NULL != pLastSelectedRecord)
	{
		ControlScreenLineDraw(pLastSelectedRecord, TRUE, true);
	}
}


void KSCManageDlg::OnBnClickedButtonSaveCordon()
{
	CXTPReportRecords*    pRecords = m_CordonReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	int nTotalRecordCount = pRecords->GetCount(); 
	if(nTotalRecordCount < 1)
	{
		return;
	}
	
	std::vector<TCordonDBInfo> vecCordonInfo;
	for(int i= 0; i< nTotalRecordCount; i++)
	{
		TCordonDBInfo oTCordonData;

		pRecord = pRecords->GetAt(i);

		pItem                 = pRecord->GetItem(_0_COLUMN_CHECK);
		oTCordonData.nxID     = pItem->GetItemData();
		pItem                 = pRecord->GetItem(_2_COLUMN_NAME);   
		oTCordonData.strName  = pItem->GetCaption(NULL);

		vecCordonInfo.push_back(oTCordonData);
	}

	if( true == KBulkDBaseScreenCordon::UpdateCordonMaster( m_pTarget, vecCordonInfo ) )
	{
		AfxMessageBox(_T("저장하였습니다."));
		ApplyUpdatedCordonInfo();
	}
}


void KSCManageDlg::ApplyUpdatedCordonInfo()
{
	m_pMapView->ClearCordonLineDraw(false);

	CXTPReportRecords*    pRecords = m_CordonReportCtrl.GetRecords();
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;
	CXTPReportRecord*     pLastSelectedRecord = NULL;

	int nTotalRecordCount = pRecords->GetCount(); 
	for(int i= 0; i< nTotalRecordCount; i++)
	{
		pRecord   = pRecords->GetAt(i);
		pItem     = pRecord->GetItem(_0_COLUMN_CHECK);
		if( TRUE == pItem->IsChecked() )
		{
			pLastSelectedRecord = pRecord;
			ControlCordonLineDraw(pRecord, TRUE, false);
		}
	}

	// 한번의 redraw 를 위한 강제 redraw 구문
	if(NULL != pLastSelectedRecord)
	{
		ControlCordonLineDraw(pLastSelectedRecord, TRUE, true);
	}
}


void KSCManageDlg::OnBnClickedButtonRefreshScreen()
{
	InitScreenReportData(false);
	m_ScreenReportCtrl.Populate();
}


void KSCManageDlg::OnBnClickedButtonRefreshCordon()
{
	InitCordonReportData(false);
	m_CordonReportCtrl.Populate();
}

void KSCManageDlg::ScreenLineRefresh()
{
    OnBnClickedButtonRefreshScreen();
}

void KSCManageDlg::CordonLineRefresh()
{
    OnBnClickedButtonRefreshCordon();
}

void KSCManageDlg::ScreenLineSelect( __int64 nxID )
{

}

void KSCManageDlg::CordonLineSelect( __int64 nxID )
{

}
