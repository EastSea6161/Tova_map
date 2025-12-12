#include "StdAfx.h"
#include "IOFrameWnd.h"
#include "IOView.h"
#include "Resource.h"
#include "IOTableView.h"

IMPLEMENT_DYNCREATE(KIOFrameWnd, CXTPFrameWnd)
BEGIN_MESSAGE_MAP(KIOFrameWnd, CXTPFrameWnd)
    ON_WM_CREATE()
    ON_XTP_CREATECONTROL()
    ON_XTP_INITCOMMANDSPOPUP()

	ON_COMMAND(ID_TABLEVIEW_PAGE_FIRST, KIOFrameWnd::OnTableViewPageFirst)
	ON_UPDATE_COMMAND_UI( ID_TABLEVIEW_PAGE_FIRST, KIOFrameWnd::OnUpdateTableViewPageFirst)

	ON_COMMAND(ID_TABLEVIEW_PAGE_PRE, KIOFrameWnd::OnTableViewPagePre)
	ON_UPDATE_COMMAND_UI( ID_TABLEVIEW_PAGE_PRE, KIOFrameWnd::OnUpdateTableViewPagePre)

	ON_UPDATE_COMMAND_UI( ID_TABLEVIEW_PAGE_INFO, KIOFrameWnd::OnUpdateTableViewPageInfo )

	ON_COMMAND(ID_TABLEVIEW_PAGE_NEXT, KIOFrameWnd::OnTableViewPageNext)
	ON_UPDATE_COMMAND_UI( ID_TABLEVIEW_PAGE_NEXT, KIOFrameWnd::OnUpdateTableViewPageNext)

	ON_COMMAND(ID_TABLEVIEW_PAGE_END, KIOFrameWnd::OnTableViewPageEnd)
	ON_UPDATE_COMMAND_UI( ID_TABLEVIEW_PAGE_END, KIOFrameWnd::OnUpdateTableViewPageEnd)

	ON_COMMAND_RANGE(ID_TABLEVIEW_PAGE_1, ID_TABLEVIEW_PAGE_10, &KIOFrameWnd::OnTableViewPageNumber)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TABLEVIEW_PAGE_1, ID_TABLEVIEW_PAGE_10, &KIOFrameWnd::OnUpdateTableViewPageNumber)

END_MESSAGE_MAP()

KIOFrameWnd::KIOFrameWnd(void)
	:	m_pIOView(NULL)
	, m_pBtnBackwardFirst(nullptr)
	, m_pBtnBackward(nullptr)
	, m_pBtnForwardward(nullptr)
	, m_pBtnEnd(nullptr)
	, m_pLblPageInfo(nullptr)
{
}

KIOFrameWnd::~KIOFrameWnd(void)
{
}

KIOView* KIOFrameWnd::GetIOView(void) const
{
	return m_pIOView;
}

BOOL KIOFrameWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CCreateContext cc;
	cc.m_pNewViewClass = RUNTIME_CLASS(KIOView);
	cc.m_pCurrentDoc = NULL;
	cc.m_pNewDocTemplate = NULL;

	m_pIOView = dynamic_cast<KIOView*>(CreateView(&cc, AFX_IDW_PANE_FIRST));
	m_pIOView->SendMessage(WM_INITIALUPDATE);
	m_pIOView->ModifyStyle(WS_BORDER, 0);

	#pragma region codejock_15.3.1
	m_pIOView->ModifyStyle(WS_CLIPSIBLINGS, WS_CLIPCHILDREN);
	#pragma endregion codejock_15.3.1	

	m_pIOView->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);

	SetActiveView(m_pIOView);

	return TRUE;
}

int KIOFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!InitCommandBars())
	{
		return -1;
	}

	CXTPCommandBars* pCommandBars = GetCommandBars();
	pCommandBars->GetCommandBarsOptions()->bShowExpandButtonAlways = false;

	CXTPToolBar* pToolBar = dynamic_cast<CXTPToolBar*>(pCommandBars->Add(_T("Table ToolBar Left"), xtpBarLeft));
	pToolBar->LoadToolBar(IDR_IOMENU_LEFT);
	pToolBar->EnableDocking(xtpFlagAlignLeft | xtpFlagStretched);
	pToolBar->SetShowGripper(TRUE);

	pToolBar = dynamic_cast<CXTPToolBar*>(pCommandBars->Add(_T("Table ToolBar"), xtpBarBottom));
	pToolBar->LoadToolBar(IDR_IOMENU);
	pToolBar->EnableDocking(xtpFlagAlignBottom | xtpFlagStretched);
	pToolBar->SetShowGripper(TRUE);

	return 0;
}

int KIOFrameWnd::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if(lpCreateControl->bToolBar)
	{
		CXTPToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTPToolBar, lpCreateControl->pCommandBar);
		if(NULL == pToolBar)
		{
			return FALSE;
		}

		if(ID_TABLEVIEW_MENU == lpCreateControl->nID)
		{
			lpCreateControl->controlType = xtpControlPopup;
			return TRUE;
		}
		if(ID_TABLEVIEW_SHOW == lpCreateControl->nID)
		{
			CXTPControlLabel* pLabel = (CXTPControlLabel*)CXTPControlLabel::CreateObject();
			pLabel->SetCaption(_T("Show: "));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pLabel->SetCaption(_T("보기: "));
            }

			pLabel->SetStyle(xtpButtonCaption);
			lpCreateControl->pControl = pLabel;
			return TRUE;
		}
		if(ID_TABLEVIEW_SHOWALL == lpCreateControl->nID)
		{
			CXTPControlRadioButton* pRadio = (CXTPControlRadioButton*)CXTPControlRadioButton::CreateObject();
			pRadio->SetCaption(_T("All"));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pRadio->SetCaption(_T("전체"));
            }

			pRadio->SetBeginGroup(TRUE);
			pRadio->SetChecked(TRUE);
			lpCreateControl->pControl = pRadio;
			return TRUE;
		}
		if(ID_TABLEVIEW_SHOWSELECTED == lpCreateControl->nID)
		{
			CXTPControlRadioButton* pRadio = (CXTPControlRadioButton*)CXTPControlRadioButton::CreateObject();
			pRadio->SetCaption(_T("Selected"));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                pRadio->SetCaption(_T("선택된 항목"));
            }

			lpCreateControl->pControl = pRadio;
			return TRUE;
		}
		if(ID_TABLEVIEW_RECORD == lpCreateControl->nID)
		{
			CXTPControlLabel* pLabel = (CXTPControlLabel*)CXTPControlLabel::CreateObject();
			pLabel->SetCaption(_T("Records: "));
			if (KmzSystem::GetLanguage() == KEMKorea) {
				pLabel->SetCaption(_T("라인수 : "));
			}

			pLabel->SetStyle(xtpButtonCaption);
			lpCreateControl->pControl = pLabel;
			return TRUE;
		}
		if( ID_TABLEVIEW_RECORDVALUE == lpCreateControl->nID )
		{
			m_pRecordValueLabel = (CXTPControlLabel*)CXTPControlLabel::CreateObject();
			m_pRecordValueLabel->SetCaption(_T("0 / 0"));
			m_pRecordValueLabel->SetStyle(xtpButtonCaption);
			m_pRecordValueLabel->SetFlags(xtpFlagSkipFocus | xtpFlagNoMovable);
			lpCreateControl->pControl = m_pRecordValueLabel;
			return TRUE;
		}
		if (ID_TABLEVIEW_PAGE_FIRST == lpCreateControl->nID) 
		{
			m_pBtnBackwardFirst = (CXTPControlButton*)CXTPControlButton::CreateObject();
			m_pBtnBackwardFirst->SetStyle(xtpButtonIcon);
			m_pBtnBackwardFirst->SetCaption(_T("<<"));

			HICON hIcon = AfxGetApp()->LoadIcon(IDC_TABLE_BACKWARD_FIRST);
			m_pBtnBackwardFirst->SetCustomIcon(hIcon);

			lpCreateControl->pControl = m_pBtnBackwardFirst;
			return TRUE;
		}
		if (ID_TABLEVIEW_PAGE_PRE == lpCreateControl->nID) 
		{
			m_pBtnBackward = (CXTPControlButton*)CXTPControlButton::CreateObject();
			m_pBtnBackward->SetStyle(xtpButtonIcon);
			m_pBtnBackward->SetCaption(_T("<"));

			HICON hIcon = AfxGetApp()->LoadIcon(IDC_TABLE_BACKWARD);
			m_pBtnBackward->SetCustomIcon(hIcon);

			lpCreateControl->pControl = m_pBtnBackward;
			return TRUE;
		}
		if (ID_TABLEVIEW_PAGE_NEXT == lpCreateControl->nID) 
		{
			m_pBtnForwardward = (CXTPControlButton*)CXTPControlButton::CreateObject();
			m_pBtnForwardward->SetStyle(xtpButtonIcon);
			m_pBtnForwardward->SetCaption(_T(">"));

			HICON hIcon = AfxGetApp()->LoadIcon(IDC_TABLE_FORWARD);
			m_pBtnForwardward->SetCustomIcon(hIcon);

			lpCreateControl->pControl = m_pBtnForwardward;
			return TRUE;
		}
		if (ID_TABLEVIEW_PAGE_END == lpCreateControl->nID) 
		{
			m_pBtnEnd = (CXTPControlButton*)CXTPControlButton::CreateObject(); 
			m_pBtnEnd->SetStyle(xtpButtonIcon);
			m_pBtnEnd->SetCaption(_T(">>"));

			HICON hIcon = AfxGetApp()->LoadIcon(IDC_TABLE_FORWARD_END);
			m_pBtnEnd->SetCustomIcon(hIcon);

			lpCreateControl->pControl = m_pBtnEnd;
			return TRUE;
		}
		if (ID_TABLEVIEW_PAGE_INFO == lpCreateControl->nID) 
		{
			m_pLblPageInfo = (CXTPControlLabel*)CXTPControlLabel::CreateObject();
			m_pLblPageInfo->SetStyle(xtpButtonCaption);
			m_pLblPageInfo->SetCaption(_T("0 / 0"));
			m_pLblPageInfo->SetFlags(xtpFlagSkipFocus | xtpFlagNoMovable);
			
			lpCreateControl->pControl = m_pLblPageInfo;
			return TRUE;
		}

		UINT nControlID = lpCreateControl->nID;
		CString strPage(_T("")); {
			if (ID_TABLEVIEW_PAGE_1 == nControlID) {
				strPage = _T("1");
			} else if (ID_TABLEVIEW_PAGE_2 == nControlID) {
				strPage = _T("2");
			} else if (ID_TABLEVIEW_PAGE_3 == nControlID) {
				strPage = _T("3");
			} else if (ID_TABLEVIEW_PAGE_4 == nControlID) {
				strPage = _T("4");
			} else if (ID_TABLEVIEW_PAGE_5 == nControlID) {
				strPage = _T("5");
			} else if (ID_TABLEVIEW_PAGE_6 == nControlID) {
				strPage = _T("6");
			} else if (ID_TABLEVIEW_PAGE_7 == nControlID) {
				strPage = _T("7");
			} else if (ID_TABLEVIEW_PAGE_8 == nControlID) {
				strPage = _T("8");
			} else if (ID_TABLEVIEW_PAGE_9 == nControlID) {
				strPage = _T("9");
			} else if (ID_TABLEVIEW_PAGE_10 == nControlID) {
				strPage = _T("10");
			} 
		}
		if (strPage.IsEmpty() == false) {
			CXTPControlButton* pBtnPage = (CXTPControlButton*)CXTPControlButton::CreateObject();
			pBtnPage->SetStyle(xtpButtonCaption);
			pBtnPage->SetCaption(strPage);
			pBtnPage->SetWidth(40);
			lpCreateControl->pControl = pBtnPage;
			return TRUE;
		}
	}
	return FALSE;
}

void KIOFrameWnd::OnInitCommandsPopup(CXTPCommandBar* pCommandBar)
{
	KIOTableView* pActiveView = m_pIOView->GetActiveTableView();
	if(NULL == pActiveView)
	{
		return;
	}

	CXTPPopupBar* pPopupBar = (CXTPPopupBar*)pCommandBar;
	CXTPControlPopup* pPopup = pPopupBar->GetControlPopup();

	CMenu menu;
	CMenu* pSubMenu = NULL;

	CString strTableName = pActiveView->GetTableName();    

	if (strTableName.CompareNoCase(TABLE_NODE) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(1);
	}
	else if (strTableName.CompareNoCase(TABLE_LINK) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(2);
	}
	else if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(3);
	}
	else if (strTableName.CompareNoCase(TABLE_TURN) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(4);
	}
	else if (strTableName.CompareNoCase(TABLE_INTERSECTION) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(5);
	}
	else if (strTableName.CompareNoCase(TABLE_TRANSIT) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(6);
		// 2018.08.02 명지대 요청사항 수정
		pSubMenu->DeleteMenu(ID_TABLE_TRANSIT_STATIONODCAL, MF_BYCOMMAND);
		// 2018.08.02 명지대 요청사항 수정
	}
	else if (strTableName.CompareNoCase(TABLE_TRANSITLINKS) == 0) //★ 불필요
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(7);
	}
	else if (strTableName.CompareNoCase(TABLE_TRANSITNODES) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(7);
	}
	else if (strTableName.CompareNoCase(TABLE_ACCESS_LINE_STATION) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(8);
	}
	else if (strTableName.CompareNoCase(TABLE_TERMINAL_SCHEDULE) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(11);
	}
	else if (strTableName.CompareNoCase(TABLE_ACCESS_STOP_NODE) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(14);
	}
	else if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0 || strTableName.CompareNoCase(TABLE_PURPOSE_OD) == 0 ||strTableName.CompareNoCase(TABLE_PARAMETER_OD) == 0 ||
			 strTableName.CompareNoCase(TABLE_AVERAGE_LOADAGE_OD) == 0 || strTableName.CompareNoCase(TABLE_GOODS_TRANSPORTED_OD) == 0 ||
			 strTableName.CompareNoCase(TABLE_VEHICLE_MODE_OD) == 0 || strTableName.CompareNoCase(TABLE_EMPTY_VEHICLE_OD) == 0 || strTableName.CompareNoCase(TABLE_MATCHING_VEHICLECONV) == 0 ||
			 strTableName.CompareNoCase(TABLE_CHAIN_OD) == 0 || strTableName.CompareNoCase(TABLE_INPUT_RATIO_OD) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(9);
	}
	else if (strTableName.CompareNoCase(TABLE_SED_POP) == 0 || strTableName.CompareNoCase(TABLE_SED_VECTOR) == 0 || strTableName.CompareNoCase(TABLE_RATIO_REGION) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(10);
	}
	else if (strTableName.CompareNoCase(TABLE_SUBWAY_TRANSFER_INFO) == 0 || strTableName.CompareNoCase(TABLE_SUBWAY_TRANSFER_SECTION) == 0 || strTableName.CompareNoCase(TABLE_SUBWAY_TRANSFER_SECTION_LINE) == 0)
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);                
		pSubMenu = menu.GetSubMenu(14);
	}
	else
	{
		menu.LoadMenu(ID_TABLEVIEW_MENU);
		pSubMenu = menu.GetSubMenu(8);     
	}

	if(NULL == pPopup || pPopup->GetID() < 0)
	{
		return;
	}

	pCommandBar->LoadMenu(pSubMenu);
}

void KIOFrameWnd::OnTableViewPageFirst( void )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr)    return;

	if (pIOTableView->IsPageView() == true){
		pIOTableView->MovePage(1);
	}
}

void KIOFrameWnd::OnUpdateTableViewPageFirst( CCmdUI* pCmdUI )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) )
	{
		CXTPControlButton* pButton = (CXTPControlButton*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			pButton->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pButton->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
		}
	}
}

void KIOFrameWnd::OnTableViewPagePre( void )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	if (pIOTableView->IsPageView() == true){
		pIOTableView->MovePage(pIOTableView->PageNumber() - 1);
	}
}

void KIOFrameWnd::OnUpdateTableViewPagePre( CCmdUI* pCmdUI )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) ) {
		CXTPControlButton* pButton = (CXTPControlButton*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			pButton->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pButton->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
		}
	}
}

void KIOFrameWnd::OnUpdateTableViewPageInfo( CCmdUI* pCmdUI )
{    
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) ) {
		CXTPControlLabel* pLabel = (CXTPControlLabel*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			int nPageCount  = pIOTableView->PageCount();
			int nPage       = pIOTableView->PageNumber();  

			CString strCaption(_T("")); 
            if (KmzSystem::GetLanguage() == KEMKorea) {
                strCaption.Format(_T("페이지 : %d / %d"), nPage, nPageCount);
            }
            else {
                strCaption.Format(_T("Page: %d / %d"), nPage, nPageCount);
            }
			
			pLabel->SetCaption(strCaption);

			int nPageRecordCount = pIOTableView->PageRecordCount();
			CString strToolTip(_T(""));
            if (KmzSystem::GetLanguage() == KEMKorea) {
                strToolTip.Format(_T("%d Records / Page"), nPageRecordCount);     
            }
            else {
                strToolTip.Format(_T("%d Records / Page"), nPageRecordCount);     
            }
			       
			pLabel->SetTooltip(strToolTip);
			pLabel->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pLabel->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
		}
	}
}

void KIOFrameWnd::OnTableViewPageNext( void )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr)    return;

	if (pIOTableView->IsPageView() == true){
		pIOTableView->MovePage(pIOTableView->PageNumber() + 1);
	}
}

void KIOFrameWnd::OnUpdateTableViewPageNext( CCmdUI* pCmdUI )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr)    return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) ) {
		CXTPControlButton* pButton = (CXTPControlButton*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			pButton->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pButton->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
		}
	}
}

void KIOFrameWnd::OnTableViewPageEnd( void )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr)    return;

	if (pIOTableView->IsPageView() == true) {
		pIOTableView->MovePage(pIOTableView->PageCount());
	}
}

void KIOFrameWnd::OnUpdateTableViewPageEnd( CCmdUI* pCmdUI )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr)    return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) ) {
		CXTPControlButton* pButton = (CXTPControlButton*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			pButton->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pButton->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
		}
	}
}

void KIOFrameWnd::OnTableViewPageNumber( UINT nID )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	if (pIOTableView->IsPageView() == false){
		return ;
	}

	int nCurPage   = pIOTableView->PageNumber();
	int nPageCount = pIOTableView->PageCount();
	int nPageStartNo = PageStartNo();        
	int nPageNo(0);    

	if (ID_TABLEVIEW_PAGE_1 == nID) {   
		nPageNo = nPageStartNo+1;         
	}
	else if (ID_TABLEVIEW_PAGE_2 == nID) {
		nPageNo = nPageStartNo+2; 
	}
	else if (ID_TABLEVIEW_PAGE_3 == nID) {
		nPageNo = nPageStartNo+3; 
	}
	else if (ID_TABLEVIEW_PAGE_4 == nID) {
		nPageNo = nPageStartNo+4; 
	}
	else if (ID_TABLEVIEW_PAGE_5 == nID) {
		nPageNo = nPageStartNo+5; 
	}
	else if (ID_TABLEVIEW_PAGE_6 == nID) {
		nPageNo = nPageStartNo+6; 
	}
	else if (ID_TABLEVIEW_PAGE_7 == nID) {
		nPageNo = nPageStartNo+7; 
	}
	else if (ID_TABLEVIEW_PAGE_8 == nID) {
		nPageNo = nPageStartNo+8; 
	}
	else if (ID_TABLEVIEW_PAGE_9 == nID) {
		nPageNo = nPageStartNo+9; 
	}
	else if (ID_TABLEVIEW_PAGE_10 == nID) {
		nPageNo = nPageStartNo+10; 
	}

	pIOTableView->MovePage(nPageNo);
}


int KIOFrameWnd::PageStartNo()
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return 1;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return 1;

	if (pIOTableView->IsPageView() == false){
		return 1;
	}

	int nPageRecordCount = pIOTableView->PageRecordCount();
	int nCurPage   = pIOTableView->PageNumber();
	int nPageCount = pIOTableView->PageCount();

	int nPageNoCount(10);
	int nPageStartNo(0);

	if (nCurPage % nPageNoCount == 0) {
		nPageStartNo = nPageNoCount * (int)(nCurPage / nPageNoCount - 1);
	}
	else {
		nPageStartNo = nPageNoCount * (int)(nCurPage / nPageNoCount);
	}

	return nPageStartNo;
}

void KIOFrameWnd::OnUpdateTableViewPageNumber( CCmdUI* pCmdUI )
{
	KIOView*         pIOTableViewWnd = GetIOView();                           if (pIOTableViewWnd == nullptr) return;
	KIOTableView*    pIOTableView    = pIOTableViewWnd->GetActiveTableView(); if (pIOTableView == nullptr) return;

	CXTPCommandBar* pToolBar = (CXTPToolBar*)pCmdUI->m_pOther;
	if( (NULL != pToolBar) && (XTPMouseManager()->IsTrackedLock() == FALSE) )
	{
		CXTPControlButton* pButton = (CXTPControlButton*)pToolBar->GetControls()->GetAt( pCmdUI->m_nIndex );
		if (pIOTableView->IsPageView() == true) {
			pButton->SetVisible(TRUE);
			pCmdUI->Enable(TRUE);
		}
		else {
			pButton->SetVisible(FALSE);
			pCmdUI->Enable(FALSE);
			return;
		}
	}

	int nCurPage   = pIOTableView->PageNumber();
	int nPageCount = pIOTableView->PageCount();
	int nPageStartNo = PageStartNo();        
	int nPageNo(0);    

	if (ID_TABLEVIEW_PAGE_1 == pCmdUI->m_nID) {   
		nPageNo = nPageStartNo+1;         
	}
	else if (ID_TABLEVIEW_PAGE_2 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+2; 
	}
	else if (ID_TABLEVIEW_PAGE_3 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+3; 
	}
	else if (ID_TABLEVIEW_PAGE_4 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+4; 
	}
	else if (ID_TABLEVIEW_PAGE_5 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+5; 
	}
	else if (ID_TABLEVIEW_PAGE_6 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+6; 
	}
	else if (ID_TABLEVIEW_PAGE_7 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+7; 
	}
	else if (ID_TABLEVIEW_PAGE_8 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+8; 
	}
	else if (ID_TABLEVIEW_PAGE_9 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+9; 
	}
	else if (ID_TABLEVIEW_PAGE_10 == pCmdUI->m_nID) {
		nPageNo = nPageStartNo+10; 
	}

	CString strCaption(_T(""));
	if (nCurPage == nPageNo) {
		strCaption.Format(_T("%d"), nPageNo);
		pCmdUI->SetCheck(TRUE);
	}
	else {
		strCaption.Format(_T("%d"), nPageNo);
		pCmdUI->SetCheck(FALSE);
	}

	if (nPageNo > nPageCount) {
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}

	pCmdUI->SetText(strCaption);       
}
