// InterUrbanChoiceParameterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterUrbanChoiceParameterDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "Purpose.h"
#include "DefineNamesModels.h"

// KInterUrbanChoiceParameterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterUrbanChoiceParameterDlg, KResizeDialogEx)

KInterUrbanChoiceParameterDlg::KInterUrbanChoiceParameterDlg(KTarget* a_pTarget, double a_dUrbanMEU, std::map<int, KUrbanChoiceParameter> a_mapUrbanChoiceParameter, std::vector<TMAFnCoefficient> a_vecFnCofficient, TAssignRule a_oGenerationPara, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KInterUrbanChoiceParameterDlg::IDD, pParent)
	,m_pTarget(a_pTarget)
	,m_dUrbanMEU(a_dUrbanMEU)
	,m_mapUrbanChoiceParameter(a_mapUrbanChoiceParameter)
	,m_vecFnCofficient(a_vecFnCofficient)
	,m_pCurrentShowClientDlg(nullptr)
	,m_oUrbanGenerationPara(a_oGenerationPara)
{
}

KInterUrbanChoiceParameterDlg::~KInterUrbanChoiceParameterDlg()
{
	ClearSubPage();
}

void KInterUrbanChoiceParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KInterUrbanChoiceParameterDlg, KResizeDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KInterUrbanChoiceParameterDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK,             &KInterUrbanChoiceParameterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,         &KInterUrbanChoiceParameterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1,      &KInterUrbanChoiceParameterDlg::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_COPY,  &KInterUrbanChoiceParameterDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &KInterUrbanChoiceParameterDlg::OnBnClickedButtonPaste)
END_MESSAGE_MAP()


// KInterUrbanChoiceParameterDlg 메시지 처리기입니다.

void KInterUrbanChoiceParameterDlg::SetDefaultContent( double& a_dUrbanMEU_Default, std::map<int, KUrbanChoiceParameter>& a_mapUrbanChoiceParameter_Default,
	std::vector<TMAFnCoefficient> &a_vecFnCofficient_Default, TAssignRule &a_oUrbanParamter_Default )
{
	m_mapUrbanChoiceParameter_Default = a_mapUrbanChoiceParameter_Default;
	m_dUrbanMEU_Default               = a_dUrbanMEU_Default;
	m_vecFnCofficient_Default         = a_vecFnCofficient_Default;
	m_oUrbanParamter_Default          = a_oUrbanParamter_Default;
}


BOOL KInterUrbanChoiceParameterDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	m_mapSubPage.clear();

	int nTabIndex(0);
	AutoType iter = m_mapUrbanChoiceParameter.begin();
	AutoType end  = m_mapUrbanChoiceParameter.end();
	while(iter != end)
	{
		KUrbanChoiceParameter oUrbanParameter = iter->second;
		//KPurpose* pPurpose = oUrbanParameter.pPurposeColumn;

		CRect oTabRect;
		m_tabCtrl.GetClientRect(&oTabRect);
		KInterUrbanChoiceParaSubPage* pSubPage = new KInterUrbanChoiceParaSubPage;
		pSubPage->SetParameterData(oUrbanParameter);
		pSubPage->SetPageID(iter->first);
		pSubPage->Create(IDD_6873_InterUbanChoiceParaSubPage, &m_tabCtrl);
		pSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		m_tabCtrl.InsertItem(nTabIndex, oUrbanParameter.pPurpose->PurposeName());
		m_mapSubPage.insert(std::make_pair(nTabIndex, pSubPage));
		nTabIndex++;
		++iter;
	}

	DisplayTabControl(0);

	InitControlParamter();

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);
	InitTransitReportControl();
	UpdateTransitReportControl(m_vecFnCofficient);

	ResizeComponet();

	return TRUE; 
}


void KInterUrbanChoiceParameterDlg::SetDefault()
{
	AutoType iter = m_mapSubPage.begin();
	AutoType end  = m_mapSubPage.end();

	while(iter != end)
	{
		KInterUrbanChoiceParaSubPage* pSubPage = iter->second;
		int nPageID = pSubPage->GetPageID();

		AutoType iterFind = m_mapUrbanChoiceParameter_Default.find(nPageID);
		if (iterFind != m_mapUrbanChoiceParameter_Default.end())
		{
			KUrbanChoiceParameter &oUrbanChoiceParameter = iterFind->second;

			pSubPage->UpdateReportData(oUrbanChoiceParameter.mapModeUrbanChoiceValue);
		}
		
		++iter;
	}

	UpdateTransitReportControl(m_vecFnCofficient_Default);

	DefaultControlParamter();
}

void KInterUrbanChoiceParameterDlg::DefaultControlParamter()
{

	CString strValue(_T(""));

	strValue.Format(_T("%.2f"), m_dUrbanMEU_Default);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEU);
	SetDlgItemText(IDC_EDIT_MEU, strValue);

	strValue.Format(_T("%.2f"), m_oUrbanParamter_Default.dDWTimeTaxi);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXITIME);
	SetDlgItemText(IDC_EDIT_TAXITIME, strValue);

	strValue.Format(_T("%.2f"), m_oUrbanParamter_Default.dDWTimeBusstop);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUSTIME);
	SetDlgItemText(IDC_EDIT_BUSTIME, strValue);

	strValue.Format(_T("%.2f"), m_oUrbanParamter_Default.dDwTimeRailStation);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SUBWAYTIME);
	SetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);

	strValue.Format(_T("%.2f"), m_oUrbanParamter_Default.dAlpha);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
	SetDlgItemText(IDC_EDIT_ALPHA, strValue);

	strValue.Format(_T("%.2f"), m_oUrbanParamter_Default.dBeta);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
	SetDlgItemText(IDC_EDIT_BETA, strValue);
}


void KInterUrbanChoiceParameterDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	DisplayTabControl(m_tabCtrl.GetCurSel());
	*pResult = 0;
}

void KInterUrbanChoiceParameterDlg::DisplayTabControl( int a_nTabIndex )
{
	int nIndex = a_nTabIndex;

	if (m_pCurrentShowClientDlg != nullptr)
	{
		m_pCurrentShowClientDlg->ShowWindow(SW_HIDE);
		m_pCurrentShowClientDlg = nullptr;
	}

	AutoType find = m_mapSubPage.find(nIndex);
	AutoType end  = m_mapSubPage.end();
	if (find != end)
	{
		m_pCurrentShowClientDlg = find->second;
		m_pCurrentShowClientDlg->ShowWindow(SW_SHOW);
		SubPageRedrawWindow();
	}
}


void KInterUrbanChoiceParameterDlg::InitControlParamter()
{
	try
	{
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), m_dUrbanMEU);
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEU);
		SetDlgItemText(IDC_EDIT_MEU, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanGenerationPara.dDWTimeTaxi);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TAXITIME);
		SetDlgItemText(IDC_EDIT_TAXITIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanGenerationPara.dDWTimeBusstop);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BUSTIME);
		SetDlgItemText(IDC_EDIT_BUSTIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanGenerationPara.dDwTimeRailStation);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SUBWAYTIME);
		SetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanGenerationPara.dAlpha);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
		SetDlgItemText(IDC_EDIT_ALPHA, strValue);

		strValue.Format(_T("%.2f"), m_oUrbanGenerationPara.dBeta);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
		SetDlgItemText(IDC_EDIT_BETA, strValue);

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


void KInterUrbanChoiceParameterDlg::InitTransitReportControl()
{
	try
	{
		CXTPReportColumn* pColumn = nullptr;

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(0, _T("Function"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("설명변수"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(1, _T("Bus Transit"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("버스"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(2, _T("Rail Transit"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("도시철도"));
		}

		pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(3, _T("Composite Mode"), 40));
		pColumn->SetHeaderAlignment(DT_CENTER);
		if (KmzSystem::GetLanguage() == KEMKorea) {
			pColumn->SetCaption(_T("버스+도시철도"));
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

void KInterUrbanChoiceParameterDlg::UpdateTransitReportControl(std::vector<TMAFnCoefficient> &a_vecFnCofficient)
{
	m_wndReportCtrl.ResetContent();

	try
	{
		CXTPReportRecord*     pRecord = NULL;
		CXTPReportRecordItem* pItem   = NULL;

		TMAFnCoefficient oBusCofficient;
		TMAFnCoefficient oSubwayCofficient;
		TMAFnCoefficient oBusSubwayConfficient;

		int nCofficientCount = a_vecFnCofficient.size();
		for (int i = 0; i < nCofficientCount; i ++)
		{
			TMAFnCoefficient oCofficient = a_vecFnCofficient[i];
			if (oCofficient.TnMode == KTransitMode::HIGHWAY_TRANSIT_CODE)
			{
				oBusCofficient = oCofficient;
			}
			else if(oCofficient.TnMode == KTransitMode::RAIL_TRANSIT_CODE)
			{
				oSubwayCofficient = oCofficient;
			}
			else if (oCofficient.TnMode == KTransitMode::COMPOSIT_MODE_CODE)
			{
				oBusSubwayConfficient = oCofficient;
			}
		}

		//invehicle
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("InVehicle Time")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("차내통행시간"));
		}

		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdInVehcle,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdInVehcle, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdInVehcle, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//waiting
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Waiting Time")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("대기시간"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdWaitingTime,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdWaitingTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdWaitingTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//transfer
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Transfer Time")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("환승시간"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdTransferTime,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdTransferTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdTransferTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//AEWTIme
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Access Walk Time")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("출발 접근시간"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdAccessWalkTime,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdAccessWalkTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdAccessWalkTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//EWTime
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Egress Walk Time")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("도착 접근시간"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdEgressWalkTime,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdEgressWalkTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdEgressWalkTime, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//NO.trans
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Transfers")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("환승횟수"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdNoOfTransfer,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdNoOfTransfer, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdNoOfTransfer, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//COST
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("Cost")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("통행비용"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdCost,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdCost, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdCost, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		//No.stop
		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
		pItem = pRecord->AddItem(new CXTPReportRecordItemText(_T("No. of Stop At Zone")));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			CXTPReportRecordItemText* pItemTxt = (CXTPReportRecordItemText*)pItem;
			pItemTxt->SetValue(_T("역/정류장 개수"));
		}
		pItem->SetAlignment(xtpColumnTextCenter);
		pItem->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
		pItem->SetEditable(FALSE);

		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusCofficient.TdNoOfStop,    _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oSubwayCofficient.TdNoOfStop, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);
		pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(oBusSubwayConfficient.TdNoOfStop, _T("%.10f")));
		pItem->SetAlignment(xtpColumnTextRight);

		m_wndReportCtrl.Populate();

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

void KInterUrbanChoiceParameterDlg::ResizeComponet()
{
	SetResize(IDC_TAB1,				SZ_TOP_LEFT,     SZ_TOP_RIGHT);

	SetResize(IDC_STATIC_MEU,		SZ_TOP_RIGHT,       SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_MEU,			SZ_TOP_RIGHT,      SZ_TOP_RIGHT);
	SetResize(IDC_SEPRATOR21,				SZ_TOP_LEFT,     SZ_TOP_RIGHT);
	
	SetResize(IDC_STATIC_TRANSIT,	SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_REPORT,			SZ_TOP_LEFT,     SZ_BOTTOM_CENTER);

	SetResize(IDC_SEPRATOR11,		SZ_BOTTOM_LEFT,  SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_CPARA,				SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC_TAXTIME,			SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_TAXITIME,			SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC_BUSTIME,			SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_BUSTIME,				SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC_SUBWAYTIME,		SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_SUBWAYTIME,			SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC8,					SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC9,					SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_ALPHA,				SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_STATIC10,					SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(IDC_EDIT_BETA,				SZ_TOP_CENTER,   SZ_TOP_CENTER);

	SetResize(IDC_BUTTON1,			SZ_BOTTOM_LEFT,    SZ_BOTTOM_LEFT);
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);

	SetResize(IDC_STATIC_SHEET, SZ_TOP_CENTER, SZ_TOP_CENTER);
	SetResize(IDC_BUTTON_COPY,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PASTE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
}


void KInterUrbanChoiceParameterDlg::OnSize(UINT nType, int cx, int cy)
{
	KResizeDialogEx::OnSize(nType, cx, cy);

	SubPageRedrawWindow();
// 	if (m_pCurrentShowClientDlg != nullptr)
// 	{        
// 		CRect oTabRect;
// 		m_tabCtrl.GetClientRect(&oTabRect);	
// 		AutoType iter = m_mapSubPage.begin();
// 		AutoType end  = m_mapSubPage.end();
// 		if (iter != end)
// 		{
// 			KInterUrbanChoiceParaSubPage* pSubPage = iter->second;
// 			pSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
// 			++iter;
// 		}
// 	}
}


void KInterUrbanChoiceParameterDlg::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_pCurrentShowClientDlg)
		{        
			CRect oTabRect;
			m_tabCtrl.GetClientRect(&oTabRect);	

			m_pCurrentShowClientDlg->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KInterUrbanChoiceParameterDlg::OnBnClickedOk()
{
	try
	{
		SaveTransitChoiceParameter();
		SaveUrbanChoicePara();
		if (SaveMEUAndControlPara() == false)
		{
			QBicMessageBox::MessageBox(_T("MEU의 값이 잘못되었습니다."), _T("Information"));
			return;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	catch(...)
	{
		TxLogDebugException();
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	KResizeDialogEx::OnOK();
}


void KInterUrbanChoiceParameterDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KInterUrbanChoiceParameterDlg::SaveUrbanChoicePara()
{
	m_mapUrbanChoiceParameter.clear();
	try
	{
		AutoType iter = m_mapSubPage.begin();
		AutoType end  = m_mapSubPage.end();
		while(iter != end)
		{
			KInterUrbanChoiceParaSubPage* pSubPage = iter->second;
			KUrbanChoiceParameter oUrbanParameter;
			pSubPage->GetParameterData(oUrbanParameter);
			m_mapUrbanChoiceParameter.insert(std::make_pair(oUrbanParameter.pPurpose->PurposeID(), oUrbanParameter));
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
		throw -1;
	}
}

void KInterUrbanChoiceParameterDlg::SaveTransitChoiceParameter()
{
	try
	{
		CXTPReportRecords*          pRecords = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord  = nullptr;
		CXTPReportRecordItemNumber* pItemNum = nullptr;

		TMAFnCoefficient oExistBusFnCofficient;
		TMAFnCoefficient oExistSubwayFnCofficient;
		TMAFnCoefficient oExistBusSubwayFnCofficient;

		int nCount = m_vecFnCofficient.size();
		for (int i = 0; i < nCount ; i ++)
		{
			TMAFnCoefficient oCofficient  = m_vecFnCofficient[i];
			if (oCofficient.TnMode == 3 )
			{
				oExistBusFnCofficient = oCofficient;
			}
			if (oCofficient.TnMode == 4)
			{
				oExistSubwayFnCofficient = oCofficient;
			}
			if (oCofficient.TnMode == 5)
			{
				oExistBusSubwayFnCofficient = oCofficient;
			}
		}

		m_vecFnCofficient.clear();

		TMAFnCoefficient oBusFnCofficient;
		oBusFnCofficient.TnMode = KTransitMode::HIGHWAY_TRANSIT_CODE;
		TMAFnCoefficient oSubwayFnCofficient;
		oSubwayFnCofficient.TnMode = KTransitMode::RAIL_TRANSIT_CODE;
		TMAFnCoefficient oBusSubwayFnCofficient;
		oBusSubwayFnCofficient.TnMode = KTransitMode::UPPER_LEVEL;

		int nRecordCount   = pRecords->GetCount();
		for (int i= 0; i< nRecordCount; i++)
		{
			double dBusCoeff(0.0);
			double dSubwayCoeff(0.0);
			double dBusSubwayCodeff(0.0);

			pRecord			= pRecords->GetAt(i);
			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(1);
			dBusCoeff		= pItemNum->GetValue();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			dSubwayCoeff	= pItemNum->GetValue();

			pItemNum		= (CXTPReportRecordItemNumber*)pRecord->GetItem(3);
			dBusSubwayCodeff	= pItemNum->GetValue();


			if (0 == i)
			{
				oBusFnCofficient.TdInVehcle = dBusCoeff;
				oSubwayFnCofficient.TdInVehcle = dSubwayCoeff;
				oBusSubwayFnCofficient.TdInVehcle = dBusSubwayCodeff;
			}
			else if (1 == i)
			{
				oBusFnCofficient.TdWaitingTime = dBusCoeff;
				oSubwayFnCofficient.TdWaitingTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdWaitingTime = dBusSubwayCodeff;
			}
			else if (2 == i)
			{
				oBusFnCofficient.TdTransferTime = dBusCoeff;
				oSubwayFnCofficient.TdTransferTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdTransferTime = dBusSubwayCodeff;
			}
			else if (3 == i)
			{
				oBusFnCofficient.TdAccessWalkTime = dBusCoeff;
				oSubwayFnCofficient.TdAccessWalkTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdAccessWalkTime = dBusSubwayCodeff;
			}
			else if (4 == i)
			{
				oBusFnCofficient.TdEgressWalkTime = dBusCoeff;
				oSubwayFnCofficient.TdEgressWalkTime = dSubwayCoeff;
				oBusSubwayFnCofficient.TdEgressWalkTime = dBusSubwayCodeff;
			}
			else if (5 == i)
			{
				oBusFnCofficient.TdNoOfTransfer = dBusCoeff;
				oSubwayFnCofficient.TdNoOfTransfer = dSubwayCoeff;
				oBusSubwayFnCofficient.TdNoOfTransfer = dBusSubwayCodeff;
			}
			else if (6 == i)
			{
				oBusFnCofficient.TdCost = dBusCoeff;
				oSubwayFnCofficient.TdCost = dSubwayCoeff;
				oBusSubwayFnCofficient.TdCost = dBusSubwayCodeff;
			}
			else if (7 == i)
			{
				oBusFnCofficient.TdNoOfStop = dBusCoeff;
				oSubwayFnCofficient.TdNoOfStop = dSubwayCoeff;
				oBusSubwayFnCofficient.TdNoOfStop = dBusSubwayCodeff;
			}
		}

		m_vecFnCofficient.push_back(oExistBusFnCofficient);
		m_vecFnCofficient.push_back(oExistSubwayFnCofficient);
		m_vecFnCofficient.push_back(oExistBusSubwayFnCofficient);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
}

bool KInterUrbanChoiceParameterDlg::SaveMEUAndControlPara()
{
	try
	{
		CString strValue(_T(""));
		GetDlgItemText(IDC_EDIT_MEU, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}
		m_dUrbanMEU = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_TAXITIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_TAXITIME)->SetFocus();
			return false;
		}
		m_oUrbanGenerationPara.dDWTimeTaxi = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_BUSTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_BUSTIME)->SetFocus();
			return false;
		}
		m_oUrbanGenerationPara.dDWTimeBusstop = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_SUBWAYTIME, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_SUBWAYTIME)->SetFocus();
			return false;
		}
		m_oUrbanGenerationPara.dDwTimeRailStation = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_ALPHA, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_ALPHA)->SetFocus();
			return false;
		}
		m_oUrbanGenerationPara.dAlpha = _ttof(strValue);

		GetDlgItemText(IDC_EDIT_BETA, strValue);
		if (QBicStringChecker::IsNumeric(strValue) == false)
		{
			GetDlgItem(IDC_EDIT_BETA)->SetFocus();
			return false;
		}
		m_oUrbanGenerationPara.dBeta = _ttof(strValue);

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}

	return true;
}

void KInterUrbanChoiceParameterDlg::ClearSubPage()
{
	try
	{
		AutoType iter = m_mapSubPage.begin();
		AutoType end  = m_mapSubPage.end();
		while(iter != end)
		{
			QBicDelete(iter->second);
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

void KInterUrbanChoiceParameterDlg::GetUrbanParameter( double& a_dUrbanMEU, std::map<int, KUrbanChoiceParameter>& a_mapUrbanChoiceParameter, std::vector<TMAFnCoefficient>& a_vecFnCofficient, TAssignRule& a_oGenerationPara )
{
	try
	{
		a_dUrbanMEU = m_dUrbanMEU;
		a_mapUrbanChoiceParameter = m_mapUrbanChoiceParameter;
		a_vecFnCofficient = m_vecFnCofficient;
		a_oGenerationPara = m_oUrbanGenerationPara;
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

void KInterUrbanChoiceParameterDlg::OnBnClickedButtonDefault()
{
	SetDefault();
}


void KInterUrbanChoiceParameterDlg::OnBnClickedButtonCopy()
{
	m_tmpMapModeUrbanChoiceValue.clear();

	try
	{
		KUrbanChoiceParameter oUrbanChoiceParameter;

		int nTabIndex = m_tabCtrl.GetCurSel();

		std::map<int, KInterUrbanChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->GetParameterData(oUrbanChoiceParameter);
			m_tmpMapModeUrbanChoiceValue = oUrbanChoiceParameter.mapModeUrbanChoiceValue;
		}
		else
		{
			throw 1;
		}

		GetDlgItem(IDC_BUTTON_PASTE)->EnableWindow(TRUE);
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}


void KInterUrbanChoiceParameterDlg::OnBnClickedButtonPaste()
{
	try
	{
		int nTabIndex = m_tabCtrl.GetCurSel();

		std::map<int, KInterUrbanChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->UpdateReportData(m_tmpMapModeUrbanChoiceValue);
		}
		else
		{
			throw 1;
		}

		AfxMessageBox(_T("복사되었습니다."));
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch (...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("예상치 못한 오류가 발생하였습니다.\n관리자에게 문의하여 주십시오."));
	}
}
