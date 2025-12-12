/**
 * @file ProjectNewDlg.cpp
 * @brief KProjectUserNewDlg 구현
 * @author jyk@qbicware.com
 * @date 2011.12.27
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "ProjectUserNewDlg.h"
#include "SelectTargetYearDlg.h"
#include "DefinePurposeDlg.h"
#include "DefineModeDlg.h"
#include "Purpose.h"
#include "Mode.h"
#include "model_common.h"
#include "ProjectInfoDlg.h"

// 2019.03.18 기본 통행목적 수정
//enum KEMDefaultPurpose
//{
//	KEMDefaultPurposeOffice    = 1,
//	KEMDefaultPurposeSchool       ,    
//	KEMDefaultPurposeInstitute    ,
//	KEMDefaultPurposeShop         ,      
//	KEMDefaultPurposeHome         ,
//	KEMDefaultPurposeEtc          ,
//	KEMDefaultPurposeBusiness       
//};
//
//#define DefaultPurposeOffice    _T("출근")
//#define DefaultPurposeSchool    _T("등교")
//#define DefaultPurposeInstitute _T("학원")
//#define DefaultPurposeShop      _T("쇼핑")
//#define DefaultPurposeHome      _T("귀가")
//#define DefaultPurposeEtc       _T("기타")
//#define DefaultPurposeBusiness  _T("업무")
enum KEMDefaultPurpose
{
	KEMDefaultPurposeHBW = 1,		// 가정기반출퇴근
	KEMDefaultPurposeHBS,			// 가정기반등하교
	KEMDefaultPurposeHBO,			// 가정기반기타
	KEMDefaultPurposeNHB			// 비가정기반
};

#define DefaultPurposeHBW		_T("가정기반출퇴근")
#define DefaultPurposeHBS		_T("가정기반등하교")
#define DefaultPurposeHBO		_T("가정기반기타")
#define DefaultPurposeNHB		_T("비가정기반")

#define DefaultPurposeHBW_ENG	_T("HomeBasedWork")
#define DefaultPurposeHBS_ENG	_T("HomeBasedSchool")
#define DefaultPurposeHBO_ENG	_T("HomeBasedOthers")
#define DefaultPurposeNHB_ENG	_T("NonHomeBased")


enum KEMDefaultMode
{
	KEMDefaultModeAuto      = 1,
	KEMDefaultModeTaxi      = 3,
	KEMDefaultModeBus       = 4,
	KEMDefaultModeSubway    = 5,
	KEMDefaultModeBusSubway = 6,
	KEMDefaultModeTruck     = 7,
};

#define DefaultModeAuto       _T("Auto")
#define DefaultModeTaxi       _T("Taxi")
#define DefaultModeBus        _T("Bus")
//#define DefaultModeSubway     _T("Subway")
//#define DefaultModeBusSubway  _T("Bus+Subway")
#define DefaultModeSubway     _T("Rail")
#define DefaultModeBusSubway  _T("BusRail")
#define DefaultModeTruck      _T("Truck")


/******************************************************************************
* KProjectUserNewDlg                                                              *
******************************************************************************/
IMPLEMENT_DYNAMIC(KProjectUserNewDlg, KDialogEx)


/**************************************
* Constructors and Destructor        *
**************************************/
KProjectUserNewDlg::KProjectUserNewDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KProjectUserNewDlg::IDD, pParent)
	, m_strName( _T("") )
	, m_strInvestigator( _T("") )
	, m_strOrganization( _T("") )
	, m_strDescription( _T("") )
	, m_strLocation( _T("") )
	, m_bBaseYear( TRUE )
	, m_nBaseYear( 0 )
	, m_strTargetYear( _T("") )
	, m_bEditedPurpose( false )
	, m_strPurposeName( _T("") )
	, m_strPurposeId( _T("") )
	, m_strPurposeGroup( _T("") )
	, m_bEditedMode( false )
	, m_strModeName( _T("") )
	, m_strLocDisplay( _T("") )
	, m_nMethodology( 0 )
	, m_bEdit( false )
	, m_nMaxPurposeId( 1000 )			//사용자 정의 목적은 1000번 이상부터 시작함.
	, m_nMaxModeId( 1000 )				//사용자 정의 수단은 1000번 이상부터 시작함.
{

}

KProjectUserNewDlg::~KProjectUserNewDlg()
{
}



/**************************************
* Overrides                           *
**************************************/
void KProjectUserNewDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PRJEDT_NAME, m_strName);
	DDX_Text(pDX, IDC_PRJEDT_INVESTIGATOR, m_strInvestigator);
	DDX_Text(pDX, IDC_PRJEDT_ORGRNIZATION, m_strOrganization);
	DDX_Text(pDX, IDC_PRJEDT_DESCRIPTION, m_strDescription);
	DDX_Text(pDX, IDC_PRJEDT_LOCATION, m_strLocation);
	DDX_Text(pDX, IDC_PRJEDT_BASEYEAR, m_nBaseYear);
	DDV_MinMaxInt(pDX, m_nBaseYear, 1900, 10000);
	DDX_Text(pDX, IDC_PRJEDT_TARGETYEAR, m_strTargetYear);
	DDX_Text(pDX, IDC_PRJEDT_PURPOSE, m_strPurposeName);
	DDX_Text(pDX, IDC_PRJEDT_MODE, m_strModeName);
	DDX_Text(pDX, IDC_PRJSTC_LOCATION, m_strLocDisplay);
	DDX_Control(pDX, IDC_PRJBTN_LOCATION, m_btnLocation);
	DDX_Control(pDX, IDC_PRJBTN_MODE, m_btnMode);
	DDX_Control(pDX, IDC_PRJBTN_PURPOSE, m_btnPurpose);
	DDX_Control(pDX, IDC_PRJBTN_TARGETYEAR, m_btnTargetYear);
	DDX_Check(pDX, IDC_PRJCHK_BASEYEAR, m_bBaseYear);
	DDX_Radio(pDX, IDC_PRJRDO_ODBASE, m_nMethodology);
	DDX_Control(pDX, IDC_PRJSPN_BASEYEAR, m_spnBaseYear);
}


BOOL KProjectUserNewDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	if( true == m_bEdit )
	{
		SetWindowText( _T("Edit Project") );
		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetWindowText( _T("프로젝트 설정") );
		}
		
		GetDlgItem( IDC_PRJEDT_LOCATION )->EnableWindow( FALSE );
		GetDlgItem( IDC_PRJBTN_LOCATION )->EnableWindow( FALSE );

		GetDlgItem( IDC_PRJRDO_ODBASE )->EnableWindow( FALSE );
		GetDlgItem( IDC_PRJRDO_PABASE )->EnableWindow( FALSE );

		if( m_nBaseYear > 0 )
		{
			m_bBaseYear = TRUE;
			GetDlgItem( IDC_PRJCHK_BASEYEAR )->EnableWindow( FALSE );
			GetDlgItem( IDC_PRJEDT_BASEYEAR )->EnableWindow( FALSE );
		}

		if( m_strTargetYear.IsEmpty() == false )
		{
			GetDlgItem( IDC_PRJEDT_TARGETYEAR )->EnableWindow( FALSE );
			GetDlgItem( IDC_PRJBTN_TARGETYEAR )->EnableWindow( FALSE );
		}

// 		GetDlgItem(IDC_PRJEDT_PURPOSE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_PRJBTN_PURPOSE)->EnableWindow(FALSE);
// 
// 		GetDlgItem(IDC_PRJEDT_MODE)->EnableWindow(FALSE);
// 		GetDlgItem(IDC_PRJBTN_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_VIEW)->ShowWindow(TRUE);

	}
	else
	{
		CString strPath = theApp.GetProfileString(_T("Common"), _T("ProjectLocation"));
		PathRemoveFileSpec((LPWSTR)(LPCWSTR)strPath);
		if (!strPath.IsEmpty())
			SetDlgItemText(IDC_PRJEDT_LOCATION, strPath);

		GetDlgItem( IDC_PRJRDO_ODBASE )->EnableWindow( FALSE );
		GetDlgItem( IDC_PRJRDO_PABASE )->EnableWindow( FALSE );
	}

	if( m_nBaseYear == 0 )
	{
		CTime time = CTime::GetCurrentTime();
		int nYear = time.GetYear();
		m_spnBaseYear.SetRange(1900, 10000);
		m_spnBaseYear.SetPos(nYear);
		m_nBaseYear = nYear;

		// Time Horizon 설정
		GetDlgItem(IDC_PRJEDT_BASEYEAR)->EnableWindow(m_bBaseYear);
	}

	/// 2010.06.22 : Base Year와 target year는 관계없음
	//GetDlgItem(IDC_PRJBTN_TARGETYEAR)->EnableWindow(m_bBaseYear);
	//GetDlgItem(IDC_PRJEDT_TARGETYEAR)->EnableWindow(m_bBaseYear);

	if (false == m_bEdit)
	{
		UpdateDeafultPurposeData();
		UpdateDefaultModeData();
	}
	else
	{
		UpdatePurposeData();
		UpdateModeData();
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KProjectUserNewDlg::UpdateDeafultPurposeData()
{
	KEMPurposeGroup emGroupDefault = KEMPurposeGroupODBase;
	KPurpose*       pPurpose       = NULL;

	m_vecPurpose.clear();

	// 2019.03.18 기본 통행목적 수정
	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeOffice,    DefaultPurposeOffice);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeSchool,    DefaultPurposeSchool);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeInstitute, DefaultPurposeInstitute);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeShop,      DefaultPurposeShop);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeHome,      DefaultPurposeHome);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeEtc,       DefaultPurposeEtc);
	//m_vecPurpose.push_back(pPurpose);

	//pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeBusiness,  DefaultPurposeBusiness);
	//m_vecPurpose.push_back(pPurpose);

	pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeHBW,    DefaultPurposeHBW_ENG);
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		pPurpose->PurposeName(DefaultPurposeHBW);
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		pPurpose->PurposeName(DefaultPurposeHBW_ENG);
	}
	m_vecPurpose.push_back(pPurpose);

	pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeHBS,    DefaultPurposeHBS_ENG);
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		pPurpose->PurposeName(DefaultPurposeHBS);
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		pPurpose->PurposeName(DefaultPurposeHBS_ENG);
	}
	m_vecPurpose.push_back(pPurpose);

	pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeHBO, DefaultPurposeHBO_ENG);
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		pPurpose->PurposeName(DefaultPurposeHBO);
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		pPurpose->PurposeName(DefaultPurposeHBO_ENG);
	}
	m_vecPurpose.push_back(pPurpose);

	pPurpose = new KPurpose( emGroupDefault, KEMDefaultPurposeNHB,      DefaultPurposeNHB_ENG);
	if (KmzSystem::GetLanguage() == KEMKorea)
	{
		pPurpose->PurposeName(DefaultPurposeNHB);
	}
	else // if (KmzSystem::GetLanguage() == KEMEnglish)
	{
		pPurpose->PurposeName(DefaultPurposeNHB_ENG);
	}
	m_vecPurpose.push_back(pPurpose);

	DisplayPurposeData(m_vecPurpose);
}


void KProjectUserNewDlg::UpdatePurposeData()
{
	DisplayPurposeData(m_vecPurpose);
}


void KProjectUserNewDlg::DisplayPurposeData(std::vector<KPurpose*> &a_vecPurpose)
{
	UpdateData(TRUE);
	m_strPurposeName.Empty();

	std::vector<KPurpose*>::iterator itPurpose, itEnd = a_vecPurpose.end();
	for(itPurpose = a_vecPurpose.begin(); itPurpose != itEnd; ++itPurpose)
	{
		KPurpose* pPurpose = *itPurpose;

		if( false == m_strPurposeName.IsEmpty() )
		{
			m_strPurposeName += _T(", ");
		}
		m_strPurposeName += (pPurpose->PurposeName());
	}

	UpdateData(FALSE);
}


void KProjectUserNewDlg::UpdateDefaultModeData()
{
	KEMModeGroup emGroupDefault = KEMModeGroupODBase;
	KMode*       pMode          = NULL;

	m_vecMode.clear();
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pMode = new KMode(emGroupDefault, KEMDefaultModeAuto, DefaultModeAuto);
	}
	else
	{
		pMode = new KMode(emGroupDefault, KEMDefaultModeAuto, _T("PassengerCar"));
	}
	m_vecMode.push_back(pMode);

	pMode = new KMode( emGroupDefault, KEMDefaultModeTaxi,       DefaultModeTaxi);
	m_vecMode.push_back(pMode);

	pMode = new KMode( emGroupDefault, KEMDefaultModeBus,        DefaultModeBus);
	m_vecMode.push_back(pMode);

	pMode = new KMode( emGroupDefault, KEMDefaultModeSubway,     DefaultModeSubway);
	m_vecMode.push_back(pMode);

	pMode = new KMode( emGroupDefault, KEMDefaultModeBusSubway,  DefaultModeBusSubway);
	m_vecMode.push_back(pMode);

	pMode = new KMode( emGroupDefault, KEMDefaultModeTruck,      DefaultModeTruck);
	m_vecMode.push_back(pMode);

	DisplayModeData(m_vecMode);
}


void KProjectUserNewDlg::UpdateModeData()
{
	DisplayModeData(m_vecMode);
}


void KProjectUserNewDlg::DisplayModeData(std::vector<KMode*> &a_vecMode)
{
	UpdateData(TRUE);
	m_strModeName.Empty();

	std::vector<KMode*>::iterator itMode, itEnd = a_vecMode.end();
	for(itMode = a_vecMode.begin(); itMode != itEnd; ++itMode)
	{
		KMode* pMode = *itMode;

		if( false == m_strModeName.IsEmpty() )
		{
			m_strModeName += _T(", ");
		}
		m_strModeName += (pMode->ModeName());
	}

	UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(KProjectUserNewDlg, KDialogEx)
	ON_BN_CLICKED(IDC_PRJBTN_LOCATION, &KProjectUserNewDlg::OnClickedPrjbtnLocation)
	ON_BN_CLICKED(IDC_PRJBTN_MODE, &KProjectUserNewDlg::OnClickedPrjbtnMode)
	ON_BN_CLICKED(IDC_PRJBTN_PURPOSE, &KProjectUserNewDlg::OnClickedPrjbtnPurpose)
	ON_BN_CLICKED(IDC_PRJBTN_TARGETYEAR, &KProjectUserNewDlg::OnClickedPrjbtnTargetyear)
	ON_BN_CLICKED(IDC_PRJCHK_BASEYEAR, &KProjectUserNewDlg::OnBnClickedPrjchkBaseyear)
	ON_BN_CLICKED(IDOK, &KProjectUserNewDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_VIEW, &KProjectUserNewDlg::OnBnClickedBtnView)
END_MESSAGE_MAP()


/**************************************
* Message handlers                    *
**************************************/
void KProjectUserNewDlg::OnClickedPrjbtnLocation()
{
	UpdateData(TRUE);

	CString strPath = theApp.GetProfileString(_T("Common"), _T("ProjectLocation"));
	PathRemoveFileSpec((LPWSTR)(LPCWSTR)strPath);

	ITEMIDLIST *pidlBrowse;
	TCHAR      szPathName[1024] = {0};
	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = GetSafeHwnd();
	browseInfo.pidlRoot = NULL;

	memset( &browseInfo, 0, sizeof(browseInfo) );
	browseInfo.pszDisplayName = szPathName;
	browseInfo.lpszTitle      = _T("경로 선택");
	browseInfo.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	browseInfo.lParam         = (LPARAM)strPath.GetBuffer( strPath.GetLength() );
	browseInfo.lpfn           = BrowseCallBackProc;
	browseInfo.hwndOwner      = this->GetSafeHwnd();

	pidlBrowse = ::SHBrowseForFolder(&browseInfo);

	if( pidlBrowse )
	{
		::SHGetPathFromIDList(pidlBrowse, szPathName);
		m_strLocation   = szPathName;
		m_strLocDisplay = szPathName;
	}
	UpdateData(FALSE);

	/*if (m_strLocation.GetAt(m_strLocation.GetLength() - 1) == _T('\\'))
	{
		m_strLocation.Delete(m_strLocation.GetLength() - 1);
	}*/
}


void KProjectUserNewDlg::OnClickedPrjbtnTargetyear()
{
	UpdateData(TRUE);
	KSelectTargetYearDlg dlg;
	dlg.m_nBaseYear = m_nBaseYear;
	if( m_strTargetYear.IsEmpty() == false )
	{
		int nPos = 0;
		CString strTarget = m_strTargetYear;
		strTarget.Trim();
		CString strYear = strTarget.Tokenize( _T(","), nPos );
		while( strYear != _T("") )
		{
			int nYear = _ttoi( strYear );
			dlg.addTargetYear( nYear );
			//dlg.addCheckedYear( strYear );
			strYear = m_strTargetYear.Tokenize( _T(","), nPos );
		}
	}

	if( dlg.DoModal() == IDOK )
	{
		m_strTargetYear = dlg.m_strSelectYear;
	}

	UpdateData(FALSE);
}


void KProjectUserNewDlg::OnClickedPrjbtnMode()
{
	UpdateData(TRUE);

	KDefineModeDlg dlg;
	if( m_nMethodology == 0 )
	{
		//dlg.m_strMode += _T("(OD Base):");
		dlg.m_strMode += _T(" : ");
	}
	else if( m_nMethodology == 1 )
	{
		//dlg.m_strMode += _T("(PA Base):");
		dlg.m_strMode += _T(" : ");
	}

	KEMModeGroup emGroupDefault = KEMModeGroupODBase;
	
	dlg.SetMaxModeId(m_nMaxModeId);
	dlg.SetMode(m_vecMode);
	dlg.SetModeGroup(emGroupDefault);

	if(dlg.DoModal() == IDOK)
	{
		m_bEditedMode  = true;
		m_strModeName  = dlg.m_strSelectedModes;
		m_strModeId    = dlg.m_strSelectedModeID;
		m_strModeGroup = dlg.m_strSelectedModeGroup;
		m_nMaxModeId   = dlg.m_nMaxModeId;
	}

	UpdateData(FALSE);
}


void KProjectUserNewDlg::OnClickedPrjbtnPurpose()
{
	UpdateData(TRUE);

	KDefinePurposeDlg dlg;
	if( m_nMethodology == 0 )
	{
		//dlg.m_strPurpose += _T("(OD Base):");
		dlg.m_strPurpose += _T(" : ");
	}
	else if( m_nMethodology == 1 )
	{
		//dlg.m_strPurpose += _T("(PA Base):");
		dlg.m_strPurpose += _T(" : ");
	}

	KEMPurposeGroup emGroupDefault = KEMPurposeGroupODBase;
	
	dlg.SetMaxPurposeId(m_nMaxPurposeId);
	dlg.SetPurpose(m_vecPurpose);
	dlg.SetPurposeGroup(emGroupDefault);

	if( dlg.DoModal() == IDOK )
	{
		m_bEditedPurpose  = true;
		m_strPurposeName  = dlg.m_strSelectedPurposeNames;
		m_strPurposeId    = dlg.m_strSelectedPurposeID;
		m_strPurposeGroup = dlg.m_strSelectedPurposeGroup;
		m_nMaxPurposeId	  = dlg.m_nMaxPurposeId;
	}

	UpdateData(FALSE);
}


void KProjectUserNewDlg::OnBnClickedPrjchkBaseyear()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_PRJEDT_BASEYEAR)->EnableWindow(m_bBaseYear);

	/// 2010.06.22 : Base Year와 target year는 관계없음
	//GetDlgItem(IDC_PRJBTN_TARGETYEAR)->EnableWindow(m_bBaseYear);
	//GetDlgItem(IDC_PRJEDT_TARGETYEAR)->EnableWindow(m_bBaseYear);
	UpdateData(FALSE);
}



void KProjectUserNewDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if(false == IsEmptyProjectName())
	{
		return;
	}

	if(false == IsEmptyPurpose())
	{
		OnClickedPrjbtnPurpose();
		return;
	}

	if(false == IsEmptyMode())
	{
		OnClickedPrjbtnMode();
		return;
	}

	if(false == m_bEdit)
	{
		if(false == IsProjectPath())
		{
			return;
		}

		if (false == IsValidPath())
		{
			return;
		}
	}
	OnOK();
}


bool KProjectUserNewDlg::IsEmptyProjectName()
{
	CString strCheckName = m_strName.Trim();
	if( strCheckName.IsEmpty())
	{
		AfxMessageBox(_T("프로젝트명을 입력해 주세요."));

		GetDlgItem( IDC_PRJEDT_NAME )->SetFocus();

		m_strName = _T("");
		UpdateData(FALSE);
		return false;
	}

	return true;
}


bool KProjectUserNewDlg::IsEmptyPurpose()
{
	if( m_strPurposeName.IsEmpty())
	{
		AfxMessageBox(_T("최소 하나 이상의 통행목적을 입력해 주세요."));
		return false;
	}

	return true;
}


bool KProjectUserNewDlg::IsEmptyMode()
{
	if( m_strModeName.IsEmpty())
	{
		AfxMessageBox(_T("최소 하나 이상의 통행수단을 입력해 주세요."));
		return false;
	}

	return true;
}


bool KProjectUserNewDlg::IsProjectPath()
{
	// 2020.02.09 - Han He : 시스템 root에 프로젝트 생성이 안되는 오류 수정
	CString strLocation = _T("");
	if (m_strLocation.GetAt(m_strLocation.GetLength() - 1) == _T('\\'))
	{
		//m_strLocation.Delete(m_strLocation.GetLength() - 1);
		strLocation = m_strLocation;
	}
	else
	{
		strLocation = m_strLocation + _T("\\");
	}

	strLocation += m_strName;
	CFileFind file;
	CString strFile = _T("*.*");
	BOOL bResult = file.FindFile(strLocation);
	if(TRUE == bResult)
	{
		file.FindNextFile();
		if(file.IsDirectory() == TRUE)
		{
			AfxMessageBox(_T("해당 프로젝트 경로가 이미 존재합니다."));
			return false;
		}
	}
	return true;
}


bool KProjectUserNewDlg::IsValidPath()
{
	CFileFind dirFind;
	bool r=PathIsRoot(m_strLocation);
	// 2020.02.09 - Han He : 시스템 root에 프로젝트 생성이 안되는 오류 수정
	/*if (KmzSystem::GetLanguage() == KEMKorea) {
		if (m_strLocation.GetAt(m_strLocation.GetLength() - 1) == _T('\\'))
		{
			m_strLocation.Delete(m_strLocation.GetLength() - 1);
		}
	}*/
	//r = PathIsRoot(m_strLocation);
	//AfxMessageBox(m_strLocation);

	if( !(dirFind.FindFile(m_strLocation)) && !r )
	{
		CString tmpstr = _T("");
		tmpstr.Format(_T("올바르지 않은 경로입니다.\n%s"), m_strLocation);
		//AfxMessageBox(_T("올바르지 않은 경로입니다.\n"));
		AfxMessageBox(tmpstr);
		return false;
	}

	return true;
}

int CALLBACK KProjectUserNewDlg::BrowseCallBackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lPData )
{
	switch (uMsg)
	{
	case  BFFM_INITIALIZED:
		{
			::SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lPData);
		}
		break;
	}
	return 0;
}


void KProjectUserNewDlg::OnBnClickedBtnView()
{
	KProjectInfoDlg oDlg;
	oDlg.DoModal();
}
