// GenerationParaSubPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultGenerationParaSubPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "model_common.h"
#include "KXmlDefaultGeneration.h"
#include "DBaseGeneration.h"


// KGenerationParaSubPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultGenerationParaSubPage, KDialogEx)

KDefaultGenerationParaSubPage::KDefaultGenerationParaSubPage(KTarget* a_pTarget, int a_nPage, int a_nPurspoeID, std::map<KIntKey, KGenerationInfo*> a_mapPGenerationInfo, std::map<KIntKey, KGenerationInfo*> a_mapAGeneratinoInfo, CWnd* pParent /*=NULL*/)
	: KDialogEx(KDefaultGenerationParaSubPage::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_nPageID(a_nPage)
	, m_nPurposeID(a_nPurspoeID)
	, m_mapPGenerationInfo(a_mapPGenerationInfo)
	, m_mapAGenerationInfo(a_mapAGeneratinoInfo)
{
	m_pProCurrentPage = nullptr;
	m_pAttCurrentPage = nullptr;
}

KDefaultGenerationParaSubPage::~KDefaultGenerationParaSubPage()
{
	ClearDlgPage();
}

void KDefaultGenerationParaSubPage::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_PRO, m_cboProModel);
	//DDX_Control(pDX, IDC_COMBO_ATT, m_cboAttModel);
}

BEGIN_MESSAGE_MAP(KDefaultGenerationParaSubPage, KDialogEx)
	//ON_BN_CLICKED(IDC_CHECK_PROZONE, &KDefaultGenerationParaSubPage::OnBnClickedCheckProzone)
	//ON_BN_CLICKED(IDC_CHECK_ATTZONE, &KDefaultGenerationParaSubPage::OnBnClickedCheckAttzone)
	//ON_CBN_SELCHANGE(IDC_COMBO_PRO, &KDefaultGenerationParaSubPage::OnCbnSelchangeComboPro)
	//ON_CBN_SELCHANGE(IDC_COMBO_ATT, &KDefaultGenerationParaSubPage::OnCbnSelchangeComboAtt)

	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO6, &KDefaultGenerationParaSubPage::OnBnClickedRadioEditProduction)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO7, IDC_RADIO12, &KDefaultGenerationParaSubPage::OnBnClickedRadioEditAttrection)

	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KGenerationParaSubPage 메시지 처리기입니다.


BOOL KDefaultGenerationParaSubPage::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	TxLogDebugStartMsg();
	SetBackgroundColor(RGB(255,255,255));
	
	ResizeComponent();

	CreateInsertPage();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO6, IDC_RADIO1);
	CheckRadioButton(IDC_RADIO7, IDC_RADIO12, IDC_RADIO7);
	
	ChangeRadioButtonPro();
	ChangeRadioButtonAtt();

	if (KDBaseGeneration::IsPopTableNotEmpty(m_pTarget) == false)
	{
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO9)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO12)->EnableWindow(FALSE);
	}

	//InitProductionCombo();
	//InitAttrectionCombo();

	//ChangeButtonStateAtt();
	//ChangeButtonStatePro();
	TxLogDebugEndMsg();
	
	//KRedrawWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 
// void KDefaultGenerationParaSubPage::InitProductionCombo()
// {
// 	m_cboProModel.ResetContent();
// 	int nFirstCursel = m_cboProModel.AddString(_T("Unit"));
// 	m_cboProModel.SetItemData(nFirstCursel, KEMGenerationUnit);
// 
// 	int nOtherCursel = m_cboProModel.AddString(_T("Regression"));
// 	m_cboProModel.SetItemData(nOtherCursel, KEMGenerationRegression);
// 
// 	nOtherCursel = m_cboProModel.AddString(_T("Cross Classification"));
// 	m_cboProModel.SetItemData(nOtherCursel, KEMGenerationCrossClassification);
// 
// 	m_cboProModel.SetCurSel(nFirstCursel);
// }
// 
// void KDefaultGenerationParaSubPage::InitAttrectionCombo()
// {
// 	m_cboAttModel.ResetContent();
// 	int nFirstCursel = m_cboAttModel.AddString(_T("Unit"));
// 	m_cboAttModel.SetItemData(nFirstCursel, KEMGenerationUnit);
// 
// 	int nOtherCursel = m_cboAttModel.AddString(_T("Regression"));
// 	m_cboAttModel.SetItemData(nOtherCursel, KEMGenerationRegression);
// 
// 	nOtherCursel = m_cboAttModel.AddString(_T("Cross Classification"));
// 	m_cboAttModel.SetItemData(nOtherCursel, KEMGenerationCrossClassification);
// 
// 	m_cboAttModel.SetCurSel(nFirstCursel);
// }
//  
// void KDefaultGenerationParaSubPage::OnBnClickedCheckProzone()
// {
// 	ChangeButtonStatePro();
// }
// 
// 
// void KDefaultGenerationParaSubPage::OnBnClickedCheckAttzone()
// {
// 	ChangeButtonStateAtt();
// }
// 
// 
// void KDefaultGenerationParaSubPage::OnCbnSelchangeComboPro()
// {
// 	ChangeButtonStatePro();
// }
// 
// 
// void KDefaultGenerationParaSubPage::OnCbnSelchangeComboAtt()
// {
// 	ChangeButtonStateAtt();
// }

void KDefaultGenerationParaSubPage::CreateInsertPage()
{
	try
	{
		CWnd* pWndPro = GetDlgItem(IDC_FRAME_PRO);
		CRect rectPro;
		pWndPro->GetWindowRect(&rectPro);
		ScreenToClient(&rectPro);

		CWnd* pWndAtt = GetDlgItem(IDC_FRAME_ATT);
		CRect rectAtt;
		pWndAtt->GetWindowRect(&rectAtt);
		ScreenToClient(&rectAtt);

		//NZoneGroup Unit
		KIntKey oNZUintIntKey(0, 1);
		KGenerationInfo* pPNUnitInfo = nullptr;
		AutoType findNZUP = m_mapPGenerationInfo.find(oNZUintIntKey);
		AutoType Pend  = m_mapPGenerationInfo.end();
		if (findNZUP != Pend)
		{
			pPNUnitInfo = findNZUP->second;
		}

		KDefaultTGUnitPage*	pProUnitPage = new KDefaultTGUnitPage(m_pTarget, pPNUnitInfo);
		pProUnitPage->Create(IDD_8515_DefaultTripGUintPage, this);
		pProUnitPage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oNZUintIntKey, pProUnitPage));

		KGenerationInfo* pANUnitInfo = nullptr;
		AutoType findNZUA = m_mapAGenerationInfo.find(oNZUintIntKey);
		AutoType Aend  = m_mapAGenerationInfo.end();
		if(findNZUA != Aend)
		{
			pANUnitInfo = findNZUA->second;
		}

		KDefaultTGUnitPage*	pAttUnitPage = new KDefaultTGUnitPage(m_pTarget, pANUnitInfo);
		pAttUnitPage->Create(IDD_8515_DefaultTripGUintPage, this);
		pAttUnitPage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oNZUintIntKey, pAttUnitPage));
	
		//NZoneGroup Regression
		KIntKey oNZRegressionIntKey(0,2);

		KGenerationInfo* pPNRegressionInfo = nullptr;
		AutoType findNZRP = m_mapPGenerationInfo.find(oNZRegressionIntKey);
		if (findNZRP != Pend)
		{
			pPNRegressionInfo = findNZRP->second;
		}

		KDefaultTGRegressionPage* pProRegressionpage = new KDefaultTGRegressionPage(m_pTarget, pPNRegressionInfo);
		pProRegressionpage->Create(IDD_8515_DefaultTripGRegressionPage, this);
		pProRegressionpage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oNZRegressionIntKey, pProRegressionpage));
		
		KGenerationInfo* pANRegressionInfo = nullptr;
		AutoType findNZRA = m_mapAGenerationInfo.find(oNZRegressionIntKey);
		if(findNZRA != Aend)
		{
			pANRegressionInfo = findNZRA->second;
		}

		KDefaultTGRegressionPage* spAttRegressionpage = new KDefaultTGRegressionPage(m_pTarget, pANRegressionInfo);
		spAttRegressionpage->Create(IDD_8515_DefaultTripGRegressionPage, this);
		spAttRegressionpage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oNZRegressionIntKey, spAttRegressionpage));

		//NZoneGroup CrossClassfication
		KIntKey oNZCrossClassifyIntKey(0,3);

		KGenerationInfo* pPNCategoryInfo = nullptr;
		AutoType findNZCP = m_mapPGenerationInfo.find(oNZCrossClassifyIntKey);
		if (findNZCP != Pend)
		{
			pPNCategoryInfo = findNZCP->second;
		}
		KDefaultTGCrossPage* pProCrossClasspage = new KDefaultTGCrossPage(m_pTarget, pPNCategoryInfo);
		pProCrossClasspage->Create(IDD_8515_DefaultTripGCrossClassficationPage, this);
		pProCrossClasspage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oNZCrossClassifyIntKey, pProCrossClasspage));

		KGenerationInfo* pANCategoryInfo = nullptr;
		AutoType findNZCA = m_mapAGenerationInfo.find(oNZCrossClassifyIntKey);
		if(findNZCA != Aend)
		{
			pANCategoryInfo = findNZCA->second;
		}
		KDefaultTGCrossPage* pAttCrossClasspage = new KDefaultTGCrossPage(m_pTarget, pANCategoryInfo);
		pAttCrossClasspage->Create(IDD_8515_DefaultTripGCrossClassficationPage, this);
		pAttCrossClasspage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oNZCrossClassifyIntKey, pAttCrossClasspage));

		//ZoneGroup Unit
		KIntKey oYZUnitIntKey(1,1);

		KGenerationInfo* pPZUnitInfo = nullptr;
		AutoType findZUP = m_mapPGenerationInfo.find(oYZUnitIntKey);
		if (findZUP != Pend)
		{
			pPZUnitInfo = findZUP->second;
		}
		KDefaultTGZoneGroupPage* pProUnitZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pPZUnitInfo);
		pProUnitZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pProUnitZonepage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oYZUnitIntKey, pProUnitZonepage));

		KGenerationInfo* pAZUnitInfo = nullptr;
		AutoType findZUA = m_mapAGenerationInfo.find(oYZUnitIntKey);
		if(findZUA != Aend)
		{
			pAZUnitInfo = findZUA->second;
		}
		KDefaultTGZoneGroupPage* pAttUnitZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pAZUnitInfo);
		pAttUnitZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pAttUnitZonepage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oYZUnitIntKey, pAttUnitZonepage));


		//ZoneGroup Regression
		KIntKey oYZRegressionIntKey(1,2);
		
		KGenerationInfo* pPZRegressionInfo = nullptr;
		AutoType findZRP = m_mapPGenerationInfo.find(oYZRegressionIntKey);
		if (findZRP != Pend)
		{
			pPZRegressionInfo = findZRP->second;
		}
		KDefaultTGZoneGroupPage* pProRegressionZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pPZRegressionInfo);
		pProRegressionZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pProRegressionZonepage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oYZRegressionIntKey, pProRegressionZonepage));

		KGenerationInfo* pAZRegressionInfo = nullptr;
		AutoType findZRA = m_mapAGenerationInfo.find(oYZRegressionIntKey);
		if(findZRA != Aend)
		{
			pAZRegressionInfo = findZRA->second;
		}
		KDefaultTGZoneGroupPage* pAttRegressionZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pAZRegressionInfo);
		pAttRegressionZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pAttRegressionZonepage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oYZRegressionIntKey, pAttRegressionZonepage));

		//ZoneGroup Classfication
		KIntKey oYZCrossIntKey(1,3);

		KGenerationInfo* pPZClassificationInfo = nullptr;
		AutoType findZCP = m_mapPGenerationInfo.find(oYZCrossIntKey);
		if (findZCP != Pend)
		{
			pPZClassificationInfo = findZCP->second;
		}
		KDefaultTGZoneGroupPage* pProCrossZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pPZClassificationInfo);
		pProCrossZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pProCrossZonepage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_HIDEWINDOW);
		m_mapProductionSubPage.insert(std::make_pair(oYZCrossIntKey, pProCrossZonepage));

		KGenerationInfo* pAZClassificationInfo = nullptr;
		AutoType findZCA = m_mapAGenerationInfo.find(oYZCrossIntKey);
		if(findZCA != Aend)
		{
			pAZClassificationInfo = findZCA->second;
		}
		KDefaultTGZoneGroupPage* pAttCrossZonepage = new KDefaultTGZoneGroupPage(m_pTarget, pAZClassificationInfo);
		pAttCrossZonepage->Create(IDD_8515_DefaultTripGZoneGroupPage, this);
		pAttCrossZonepage->SetWindowPos(NULL, rectAtt.left, rectAtt.top, rectAtt.Width(), rectAtt.Height(), SWP_HIDEWINDOW);
		m_mapAttectionSubPage.insert(std::make_pair(oYZCrossIntKey, pAttCrossZonepage));

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
// 
// void KDefaultGenerationParaSubPage::ChangeButtonStatePro()
// {
// 	int nCheckProZone = 0;
// 	if (IsDlgButtonChecked(IDC_CHECK_PROZONE) == BST_CHECKED)
// 	{
// 		nCheckProZone = 1;
// 	}
// 
// 	int nProModel = m_cboProModel.GetItemData(m_cboProModel.GetCurSel());
// 
// 	KIntKey SelIntKey(nCheckProZone, nProModel);
// 
// 	if (nullptr != m_pProCurrentPage)
// 	{
// 		m_pProCurrentPage->ShowWindow(SW_HIDE);
// 	}
// 	
// 	AutoType iter = m_mapProductionSubPage.find(SelIntKey);
// 	if (iter != m_mapProductionSubPage.end())
// 	{
// 		m_pProCurrentPage = iter->second;
// 		m_pProCurrentPage->ShowWindow(SW_SHOW);
// 	}
// 	SubPageRedrawWindow();
// }
// 
// void KDefaultGenerationParaSubPage::ChangeButtonStateAtt()
// {
// 	int nCheckAttZone = 0;
// 	if (IsDlgButtonChecked(IDC_CHECK_ATTZONE) == BST_CHECKED)
// 	{
// 		nCheckAttZone = 1;
// 	}
// 
// 	int nCursel = m_cboAttModel.GetCurSel();
// 	int nAttModel = m_cboAttModel.GetItemData(nCursel);
// 
// 	KIntKey SelIntKey(nCheckAttZone, nAttModel);
// 
// 	if (nullptr != m_pAttCurrentPage)
// 	{
// 		m_pAttCurrentPage->ShowWindow(SW_HIDE);
// 	}
// 
// 	AutoType iter = m_mapAttectionSubPage.find(SelIntKey);
// 	if (iter != m_mapAttectionSubPage.end())
// 	{
// 		m_pAttCurrentPage = iter->second;
// 		m_pAttCurrentPage->ShowWindow(SW_SHOW);
// 	}
// 	SubPageRedrawWindow();
// }


void KDefaultGenerationParaSubPage::SubPageRedrawWindow()
{
	try
	{
		if (nullptr != m_pProCurrentPage)
		{        
			CWnd* pWndPro = GetDlgItem(IDC_FRAME_PRO);
			CRect rectPro;
			pWndPro->GetWindowRect(&rectPro);
			ScreenToClient(&rectPro);
			m_pProCurrentPage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_SHOWWINDOW);
		}

		if (nullptr != m_pAttCurrentPage)
		{
			CWnd* pWndPro = GetDlgItem(IDC_FRAME_ATT);
			CRect rectPro;
			pWndPro->GetWindowRect(&rectPro);
			ScreenToClient(&rectPro);
			m_pAttCurrentPage->SetWindowPos(NULL, rectPro.left, rectPro.top, rectPro.Width(), rectPro.Height(), SWP_SHOWWINDOW);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

HBRUSH KDefaultGenerationParaSubPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = KDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    UINT ID = pWnd->GetDlgCtrlID();
    if( ID == IDC_CHECK_PROZONE || ID == IDC_CHECK_ATTZONE || ID == IDC_RADIO1 || ID == IDC_RADIO2 || ID == IDC_RADIO3 || ID == IDC_RADIO4 
		|| ID == IDC_RADIO5 || ID == IDC_RADIO6 || ID == IDC_RADIO7 || ID == IDC_RADIO8 || ID == IDC_RADIO9 || ID == IDC_RADIO10 || ID == IDC_RADIO11 || ID == IDC_RADIO12)
    {        
        /*hbr = (HBRUSH)m_pbkBrush.GetSafeHandle();
        pDC->SetBkMode(TRANSPARENT);*/
        pDC->SetBkMode(TRANSPARENT);
        return (HBRUSH)GetStockObject(WHITE_BRUSH);
    }
	return hbr;
}

void KDefaultGenerationParaSubPage::SaveDefaultData()
{
	try
	{
		AutoType Piter = m_mapProductionSubPage.begin();
		AutoType Pend  = m_mapProductionSubPage.end();
		while(Piter != Pend)
		{
			CXTPPropertyPage* pPage = Piter->second;
			pPage->OnApply();
			Piter++;
		}

		AutoType Aiter = m_mapAttectionSubPage.begin();
		AutoType Aend  = m_mapAttectionSubPage.end();
		while(Aiter != Aend)
		{
			CXTPPropertyPage* pPage = Aiter->second;
			pPage->OnApply();
			Aiter++;
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

void KDefaultGenerationParaSubPage::WriteDefaultData(KXmlManager& a_oxmlManager, TiXmlElement* a_pGroupElement)
{
	try
	{
		SaveDefaultData();

		TiXmlElement* pPurposeElement = a_oxmlManager.CreateChildElement(_T("Purpose"), a_pGroupElement);
		a_oxmlManager.SetAttributeInt(_T("ID"), pPurposeElement, m_nPurposeID);


		TiXmlElement* pProductionElemnt = a_oxmlManager.CreateChildElement(_T("Production"), pPurposeElement);

		AutoType Piter = m_mapPGenerationInfo.begin();
		AutoType Pend  = m_mapPGenerationInfo.end();
		while(Piter != Pend)
		{
			KGenerationInfo* pInfo = Piter->second;
			KXmlDefaultGeneration oDGeneration(m_pTarget);
			oDGeneration.WriteDefaultData(a_oxmlManager, pProductionElemnt, pInfo);
			Piter++;
		}

		TiXmlElement* pAttrectionElemnt = a_oxmlManager.CreateChildElement(_T("Attraction"), pPurposeElement);
		AutoType Aiter = m_mapAGenerationInfo.begin();
		AutoType Aend  = m_mapAGenerationInfo.end();
		while(Aiter != Aend)
		{
			KGenerationInfo* pInfo = Aiter->second;
			KXmlDefaultGeneration oDGeneration(m_pTarget);
			oDGeneration.WriteDefaultData(a_oxmlManager, pAttrectionElemnt, pInfo);

			Aiter++;
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

void KDefaultGenerationParaSubPage::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC_PRO,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_SEPRATOR13,		SZ_TOP_LEFT,       SZ_TOP_CENTER);
		
		SetResize(IDC_STATIC1,			SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_STATIC25,			SZ_TOP_LEFT,       SZ_TOP_LEFT);

		SetResize(IDC_RADIO1,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_RADIO2,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_RADIO3,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_RADIO4,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_RADIO5,		SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_RADIO6,		SZ_TOP_LEFT,       SZ_TOP_LEFT);


		SetResize(IDC_STATIC_ATT,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_SEPRATOR14,		SZ_TOP_CENTER,       SZ_TOP_RIGHT);
		
		SetResize(IDC_STATIC26,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_STATIC27,		SZ_TOP_CENTER,       SZ_TOP_CENTER);

		SetResize(IDC_RADIO7,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_RADIO8,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_RADIO9,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_RADIO10,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_RADIO11,		SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_RADIO12,		SZ_TOP_CENTER,       SZ_TOP_CENTER);

		SetResize(IDC_FRAME_PRO,		SZ_TOP_LEFT,		SZ_BOTTOM_CENTER);
		SetResize(IDC_FRAME_ATT,		SZ_TOP_CENTER,       SZ_BOTTOM_RIGHT);

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


void KDefaultGenerationParaSubPage::OnSize(UINT nType, int cx, int cy)
{
	KDialogEx::OnSize(nType, cx, cy);

	SubPageRedrawWindow();
	//KRedrawWindow();
}


void KDefaultGenerationParaSubPage::KRedrawWindow()
{
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE|RDW_ERASE);
}

void KDefaultGenerationParaSubPage::InitDefaultData()
{
	try
	{


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

void KDefaultGenerationParaSubPage::InitProductioinPage()
{

	int nCheckRadio = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO6);

	KDefaultTGUnitPage*			pUnitPage		= nullptr;
	KDefaultTGRegressionPage*	pRegressionPage = nullptr;
	KDefaultTGCrossPage*		pCrossPage		= nullptr;
	KDefaultTGZoneGroupPage*	pZoneGroupPage	= nullptr;

	switch(nCheckRadio)
	{
	case IDC_RADIO1:
		pUnitPage = (KDefaultTGUnitPage*)m_pProCurrentPage;
		pUnitPage->InitDefaultData();
		break;

	case IDC_RADIO2:
		pRegressionPage = (KDefaultTGRegressionPage*)m_pProCurrentPage;
		pRegressionPage->InitDefaultData();
		break;

	case IDC_RADIO3:
		pCrossPage = (KDefaultTGCrossPage*)m_pProCurrentPage;
		pCrossPage->InitDefaultData();
		break;

	case IDC_RADIO4:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pProCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;

	case IDC_RADIO5:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pProCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;

	case IDC_RADIO6:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pProCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;
	default:
		break;
	}
}

void KDefaultGenerationParaSubPage::InitAttationPage()
{
	
	int nCheckRadio = GetCheckedRadioButton(IDC_RADIO7, IDC_RADIO12);

	KDefaultTGUnitPage*			pUnitPage		= nullptr;
	KDefaultTGRegressionPage*	pRegressionPage = nullptr;
	KDefaultTGCrossPage*		pCrossPage		= nullptr;
	KDefaultTGZoneGroupPage*	pZoneGroupPage	= nullptr;

	switch(nCheckRadio)
	{
	case IDC_RADIO7:
		pUnitPage = (KDefaultTGUnitPage*)m_pAttCurrentPage;
		pUnitPage->InitDefaultData();
		break;

	case IDC_RADIO8:
		pRegressionPage = (KDefaultTGRegressionPage*)m_pAttCurrentPage;
		pRegressionPage->InitDefaultData();
		break;

	case IDC_RADIO9:
		pCrossPage = (KDefaultTGCrossPage*)m_pAttCurrentPage;
		pCrossPage->InitDefaultData();
		break;

	case IDC_RADIO10:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pAttCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;

	case IDC_RADIO11:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pAttCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;

	case IDC_RADIO12:
		pZoneGroupPage =(KDefaultTGZoneGroupPage*)m_pAttCurrentPage;
		pZoneGroupPage->InitDefaultData();
		break;

	default:
		break;
	}
}

void KDefaultGenerationParaSubPage::ClearDlgPage()
{
	try
	{
		AutoType iterP = m_mapProductionSubPage.begin();
		AutoType endP  = m_mapProductionSubPage.end();
		while(iterP != endP)
		{
			CXTPPropertyPage* pPage = iterP->second;
			if (pPage != nullptr)
			{
				QBicDelete(pPage);
			}
			iterP++;
		}

		AutoType iterA = m_mapAttectionSubPage.begin();
		AutoType endA  = m_mapAttectionSubPage.end();
		while(iterA != endA)
		{
			CXTPPropertyPage* pPage = iterA->second;
			if (pPage != nullptr)
			{
				QBicDelete(pPage);
			}
			iterA++;
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

void KDefaultGenerationParaSubPage::OnBnClickedRadioEditProduction( UINT nID )
{
	ChangeRadioButtonPro();
}

void KDefaultGenerationParaSubPage::OnBnClickedRadioEditAttrection( UINT nID )
{
	ChangeRadioButtonAtt();
}

void KDefaultGenerationParaSubPage::ChangeRadioButtonPro()
{
	try
	{
		int nCheckRadio = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO6);

		KIntKey oKey;

		switch(nCheckRadio)
		{
		case IDC_RADIO1:
			oKey.First		= 0;
			oKey.Second		= 1;
			break;

		case IDC_RADIO2:
			oKey.First		= 0;
			oKey.Second		= 2;
			break;

		case IDC_RADIO3:
			oKey.First		= 0;
			oKey.Second		= 3;
			break;

		case IDC_RADIO4:
			oKey.First		= 1;
			oKey.Second		= 1;
			break;

		case IDC_RADIO5:
			oKey.First		= 1;
			oKey.Second		= 2;
			break;

		case IDC_RADIO6:
			oKey.First		= 1;
			oKey.Second		= 3;
			break;
		default:
			break;
		}

		AutoType iter = m_mapProductionSubPage.find(oKey);
		if (iter != m_mapProductionSubPage.end())
		{
			if (nullptr != m_pProCurrentPage)
			{
				m_pProCurrentPage->ShowWindow(SW_HIDE);
			}

			m_pProCurrentPage = iter->second;
			m_pProCurrentPage->ShowWindow(SW_SHOW);
		}

		SubPageRedrawWindow();

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

void KDefaultGenerationParaSubPage::ChangeRadioButtonAtt()
{
	try
	{
		int nCheckRadio = GetCheckedRadioButton(IDC_RADIO7, IDC_RADIO12);

		KIntKey oKey;

		switch(nCheckRadio)
		{
		case IDC_RADIO7:
			oKey.First		= 0;
			oKey.Second		= 1;
			break;

		case IDC_RADIO8:
			oKey.First		= 0;
			oKey.Second		= 2;
			break;

		case IDC_RADIO9:
			oKey.First		= 0;
			oKey.Second		= 3;
			break;

		case IDC_RADIO10:
			oKey.First		= 1;
			oKey.Second		= 1;
			break;

		case IDC_RADIO11:
			oKey.First		= 1;
			oKey.Second		= 2;
			break;

		case IDC_RADIO12:
			oKey.First		= 1;
			oKey.Second		= 3;
			break;

		default:
			break;
		}
		AutoType iter = m_mapAttectionSubPage.find(oKey);
		if (iter != m_mapAttectionSubPage.end())
		{
			if (nullptr != m_pAttCurrentPage)
			{
				m_pAttCurrentPage->ShowWindow(SW_HIDE);
			}

			m_pAttCurrentPage = iter->second;
			m_pAttCurrentPage->ShowWindow(SW_SHOW);
		}
		
		SubPageRedrawWindow();
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

