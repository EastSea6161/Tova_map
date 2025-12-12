// KPointMarkerSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KPointMarkerSelectDlg.h"


// KPointMarkerSelectDlg 대화 상자입니다.
enum
{
    _0_COLUMN_VAL = 0,
    _1_COLUMN_VAL,
    _2_COLUMN_VAL,
    _3_COLUMN_VAL,
    _4_COLUMN_VAL
}; // m_nReportColCnt 개수와 동기화 시켜라!

IMPLEMENT_DYNAMIC(KPointMarkerSelectDlg, CXTPResizeDialog)

KPointMarkerSelectDlg::KPointMarkerSelectDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(KPointMarkerSelectDlg::IDD, pParent)
	, m_nReportColCnt(5)
{
    m_oQbicDefaultMarkerFont.CreatePointFont(130, _T("QBicF"));
    m_selectedMarkerIntex = 48;

	m_clrBack    = RGB ( 0, 0, 0 );
	m_bColorEdit = false;
}

KPointMarkerSelectDlg::~KPointMarkerSelectDlg()
{
}

void KPointMarkerSelectDlg::DoDataExchange(CDataExchange* pDX)
{
    DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
    CXTPResizeDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SELECTITEM, m_SelectedMarker);
	DDX_Control(pDX, IDC_CLR_BACK,           m_cpBackColor);
	DDX_XTPColorPicker(pDX, IDC_CLR_BACK, m_clrBack);
}


BEGIN_MESSAGE_MAP(KPointMarkerSelectDlg, CXTPResizeDialog)
	ON_WM_PAINT()
    ON_NOTIFY(NM_CLICK, IDC_REPORT, OnReportItemClick)
    ON_BN_CLICKED(IDOK, &KPointMarkerSelectDlg::OnBnClickedOk)
	ON_CPN_XTP_SELENDOK(IDC_CLR_BACK, &KPointMarkerSelectDlg::OnSelEndOkBackClr)    
END_MESSAGE_MAP()


// KPointMarkerSelectDlg 메시지 처리기입니다.
BOOL KPointMarkerSelectDlg::OnInitDialog()
{
    CXTPResizeDialog::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    CXTPReportColumn* pColumn = NULL;
    QBicReportCtrlSetting::Default(m_wndReportCtrl);
    //m_wndReportCtrl.FocusSubItems(FALSE);
    m_wndReportCtrl.SelectionEnable(FALSE);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_VAL, _T("모양1"), 20));
	}
	else {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_VAL, _T("Symbol"), 20));
	}
    pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_VAL, _T("모양2"), 20));
	}
	else {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_VAL, _T("Symbol"), 20));
	}
    //pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_VAL, _T("Symbol"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_VAL, _T("모양3"), 20));
	}
	else {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_VAL, _T("Symbol"), 20));
	}
    //pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_VAL, _T("Symbol"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_VAL, _T("모양4"), 20));
	}
	else {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_VAL, _T("Symbol"), 20));
	}
    //pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_VAL, _T("Symbol"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);

	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_VAL, _T("모양5"), 20));
	}
	else {
		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_VAL, _T("Symbol"), 20));
	}
    //pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_4_COLUMN_VAL, _T("Symbol"), 20));
    pColumn->SetHeaderAlignment(DT_CENTER);    

    CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
    pHeader->AllowColumnRemove(FALSE);
    pHeader->AllowColumnReorder(FALSE);
    pHeader->AllowColumnSort(FALSE); 
    
    CXTPReportRecord*     pRecord = NULL;
    CXTPReportRecordItem* pItem   = NULL;

	std::vector<int> vecQbicFont;
    for (int i=48; i<=57; i++) 
	{
        vecQbicFont.push_back(i);
    }
	
	for (int i = 65; i<=122;i++)
	{
		vecQbicFont.push_back(i);
	}

	/*for (int i= 48; i<= 57; i++) // 0~9
		vecQbicFont.push_back(i);
	for (int i= 65; i<= 90; i++) // A~Z
		vecQbicFont.push_back(i);
	for (int i= 97; i<= 126; i++)// a~z + {|}~
		vecQbicFont.push_back(i);
        */

	int nVecSize = (int)vecQbicFont.size();
    for (int i= 0; i< nVecSize; i++ )
    {
		if (i % m_nReportColCnt == 0)
			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
        
		pItem = pRecord->AddItem(new CXTPReportRecordItemText());  
        CString strSymbol;
        strSymbol.AppendChar(vecQbicFont[i]);
        pItem->SetCaption(strSymbol);
        pItem->SetAlignment(DT_CENTER);
        pItem->SetFont(&m_oQbicDefaultMarkerFont); 
        pItem->SetItemData(vecQbicFont[i]);

		if (m_bColorEdit)
			pItem->SetTextColor(m_clrBack);
    }
       
    m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);      
    m_wndReportCtrl.FocusSubItems(TRUE);
    m_wndReportCtrl.SelectionEnable(TRUE);
    m_wndReportCtrl.Populate();

    SetResize(IDC_REPORT,    SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
    SetResize(IDC_SEPRATOR1, SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);
    SetResize(IDCANCEL,      SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDOK,          SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_SELECTITEM,SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CLR_BACK,  SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);

    SetResize(IDC_STATIC_NO, CXTPResizePoint(0, 1), CXTPResizePoint(0, 1));

    m_SelectedMarker.SetFont(&m_oQbicDefaultMarkerFont);    
	CString strSelectedSymbol;
	strSelectedSymbol.AppendChar(m_selectedMarkerIntex);        
	SetDlgItemText(IDC_SELECTITEM, strSelectedSymbol);

	CString strIndex;
	strIndex.Format(_T("Index:%d"), m_selectedMarkerIntex);
	SetDlgItemText(IDC_STATIC_NO, strIndex);

	m_cpBackColor.SetColor(m_clrBack);
	m_cpBackColor.SetDefaultColor(GetSysColor(COLOR_WINDOW));
	m_cpBackColor.ShowText(FALSE);
	m_cpBackColor.SetTheme(xtpControlThemeDefault);

	CButton* pButton = (CButton*)GetDlgItem(IDC_CLR_BACK);
	if (m_bColorEdit)
		pButton->ShowWindow(SW_SHOW);
	else
		pButton->ShowWindow(SW_HIDE);
	
    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KPointMarkerSelectDlg::OnReportItemClick(NMHDR* pNotifyStruct, LRESULT* pResult)
{
    XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;

    if (!pItemNotify->pRow || !pItemNotify->pColumn)
        return;

    int nIndex = pItemNotify->pColumn->GetItemIndex();

    m_selectedMarkerIntex = (int)pItemNotify->pItem->GetItemData();
    CString strSelectedSymbol;
    strSelectedSymbol.AppendChar(m_selectedMarkerIntex);        
    SetDlgItemText(IDC_SELECTITEM, strSelectedSymbol);

	CString strIndex;
	strIndex.Format(_T("Index:%d"), m_selectedMarkerIntex);
	SetDlgItemText(IDC_STATIC_NO, strIndex);
}

void KPointMarkerSelectDlg::OnBnClickedOk()
{   
    CXTPResizeDialog::OnOK();
}

void KPointMarkerSelectDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    // 그리기 메시지에 대해서는 CXTPResizeDialog::OnPaint()을(를) 호출하지 마십시오.
}

void KPointMarkerSelectDlg::OnSelEndOkBackClr()
{
	m_clrBack = m_cpBackColor.GetColor();
	RedrawMarkerColor();
}

void KPointMarkerSelectDlg::RedrawMarkerColor()
{
	CXTPReportRecords*    pRecords = NULL;
	CXTPReportRecord*     pRecord  = NULL;
	CXTPReportRecordItem* pItem    = NULL;

	pRecords         = m_wndReportCtrl.GetRecords();
	int nRecordCount = pRecords->GetCount();

	for (int i= 0; i< nRecordCount; i++)
	{
		pRecord = pRecords->GetAt(i);
		for (int k= 0; k< m_nReportColCnt; k++)
		{
			pItem = pRecord->GetItem(k);
			if (nullptr != pItem)
				pItem->SetTextColor(m_clrBack);
		}
	}

	m_wndReportCtrl.Populate();
}

void KPointMarkerSelectDlg::SetInitialData( int a_nSelectedMarkerIndex )
{
	m_selectedMarkerIntex = a_nSelectedMarkerIndex;
}

void KPointMarkerSelectDlg::SetInitialData( int a_nSelectedMarkerIndex, COLORREF a_colorMarker )
{
	m_bColorEdit = true;

	m_selectedMarkerIntex = a_nSelectedMarkerIndex;
	m_clrBack = a_colorMarker;
}
