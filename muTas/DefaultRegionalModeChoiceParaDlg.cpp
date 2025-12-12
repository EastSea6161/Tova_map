// DefaultRegionalModeChoiceParaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultRegionalModeChoiceParaDlg.h"
#include "afxdialogex.h"
#include "DBaseDefaultParaRegionalModeChoice.h"
#include "DefineNamesDefaultPara.h"
//#include "CodeGroup.h"
//#include "CodeManager.h"

// KDefaultRegionalModeChoiceParaDlg 대화 상자입니다.
const double MEU = 0.1;

#define TAB_LEFT_MARGIN    5/*5*/
#define TAB_TOP_MARGIN    25
#define TAB_WIDTH_MARGIN  TAB_LEFT_MARGIN*2
#define TAB_HEIGHT_MARGIN 30

IMPLEMENT_DYNAMIC(KDefaultRegionalModeChoiceParaDlg, KResizeDialogEx)

KDefaultRegionalModeChoiceParaDlg::KDefaultRegionalModeChoiceParaDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultRegionalModeChoiceParaDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_bExistSystemMEU(false)
	, m_bExistSystemChoiceParam(false)
	, m_purposeInfos(m_pTarget)
	, m_pCurrentShowClientDlg(nullptr)
	, m_dMEU(0.0)
{
	m_mapSubPage.clear();
}

KDefaultRegionalModeChoiceParaDlg::~KDefaultRegionalModeChoiceParaDlg()
{
	ClearSubPage();
}

void KDefaultRegionalModeChoiceParaDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(KDefaultRegionalModeChoiceParaDlg, KResizeDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &KDefaultRegionalModeChoiceParaDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK,             &KDefaultRegionalModeChoiceParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,         &KDefaultRegionalModeChoiceParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1,      &KDefaultRegionalModeChoiceParaDlg::OnBnClickedBtnInitialize)
	ON_BN_CLICKED(IDC_BUTTON_COPY,  &KDefaultRegionalModeChoiceParaDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &KDefaultRegionalModeChoiceParaDlg::OnBnClickedButtonPaste)
END_MESSAGE_MAP()


// KDefaultRegionalModeChoiceParaDlg 메시지 처리기입니다.


BOOL KDefaultRegionalModeChoiceParaDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitMEU();
	InitRegionalChoiceParameter();

	int nTabIndex(0);

	AutoType iter = m_mapRegionalChoiceParameter.begin();
	AutoType end  = m_mapRegionalChoiceParameter.end();
	
	while(iter != end)
	{
		KRegionalChoiceParameter oRegionalParameter = iter->second;

		CRect oTabRect;
		m_tabCtrl.GetClientRect(&oTabRect);
		KDefaultRegionalModeChoiceParaSubPage* pSubPage = new KDefaultRegionalModeChoiceParaSubPage;
		pSubPage->SetParameter(oRegionalParameter);
		pSubPage->Create(IDD_8510_Default_Regional_ModeChoiceParaSubPage, &m_tabCtrl);
		pSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);
		m_tabCtrl.InsertItem(nTabIndex, oRegionalParameter.pPurpose->PurposeName());
		m_mapSubPage.insert(std::make_pair(nTabIndex, pSubPage));

		nTabIndex++;
		++iter;
	}
	
	DisplayTabControl(0);
	ResizeComponent();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KDefaultRegionalModeChoiceParaDlg::OnBnClickedOk()
{
	try
	{
		if (!InvalidateInputInfo())
			return;
		
		if (!m_bExistSystemMEU)
			SaveSystemDefaultMEU();

		SaveUserDefaultMEU();

		SaveSystemDefaultRegionChoicePara();
		SaveUserDefaultRegionChoicePara();
			
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		QBicMessageBox::MessageBox(_T("저장중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	
	KResizeDialogEx::OnOK();
}


void KDefaultRegionalModeChoiceParaDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KResizeDialogEx::OnCancel();
}

void KDefaultRegionalModeChoiceParaDlg::InitMEU( void )
{
	KDBaseDefaultParaRegionalModeChoice::LoadDefaultRegionalModeChoiceMEU(m_mapMEU);

	AutoType bIter = m_mapMEU.begin();
	AutoType eIter = m_mapMEU.end();

	if (bIter != eIter)
	{
		AutoType fIter = m_mapMEU.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

		if (fIter != eIter)
		{
			m_dMEU = fIter->second;
		}
		else
		{
			fIter = m_mapMEU.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

			if (fIter != eIter)
			{
				m_dMEU              = fIter->second;
				m_bExistSystemMEU = true;
			}
			else
			{	
				//이도저도 아닌 버그 버그다 버그 버그...
				m_dMEU = MEU;
			}
		}
	}
	else
	{
		m_dMEU = MEU;
	}

	CString strValue;
	strValue.Format(_T("%.2f"), m_dMEU);
	SetDlgItemText(IDC_EDIT_MEU, strValue);
}

bool KDefaultRegionalModeChoiceParaDlg::InvalidateInputInfo( void )
{
	try
	{
		CString strValue;
		
		//Check MEU Value
		GetDlgItemText(IDC_EDIT_MEU, strValue);

		if (!QBicStringChecker::IsNumeric(strValue))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}

	return true;
}

void KDefaultRegionalModeChoiceParaDlg::SaveSystemDefaultMEU( void )
{
	KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceMEU(
		KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, MEU);
}

void KDefaultRegionalModeChoiceParaDlg::SaveUserDefaultMEU( void )
{
	CString strValue;
	GetDlgItemText(IDC_EDIT_MEU, strValue);

	double dMEU = _ttof(strValue);

	KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceMEU(
		KDefaultParameterMasterCode::USER_MASETER_CODE, dMEU);
}

void KDefaultRegionalModeChoiceParaDlg::InitRegionalChoiceParameter( void )
{
	try
	{
		std::list<KPurpose*>::iterator bIter, eIter  = m_purposeInfos.end();

		for (bIter = m_purposeInfos.begin(); bIter != eIter; ++bIter)
		{
			KPurpose* pPurpose = *bIter;

			std::map<int, TRegionalParameter> mapRegionalParameter;
			InitRegionalChoiceValule(pPurpose->PurposeID(), mapRegionalParameter);

			KRegionalChoiceParameter oRegionalInfo;
			//oRegionalInfo.pColumn					 = pPurpposeColumn;
			oRegionalInfo.mapModeRegionalChoiceValue = mapRegionalParameter;
			oRegionalInfo.pPurpose                   = *bIter;

			m_mapRegionalChoiceParameter.insert(std::make_pair(pPurpose->PurposeID(), oRegionalInfo));
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDefaultRegionalModeChoiceParaDlg::InitRegionalChoiceValule( int a_nPurposeID, std::map<int, TRegionalParameter>& a_mapRegionalChoiceValue )
{
	a_mapRegionalChoiceValue.clear();

	try
	{
		std::map<int, CString> mapModeCode;
		KCodeManager* pCodeMgr   = m_pTarget->CodeManager();
		KCodeGroup*   pCodeGroup = pCodeMgr->FindCodeGroup(TRANSIT_REGIONAL_MODE_CODE_GROUP_KEY);
		pCodeGroup->GetCodes(mapModeCode);

		std::map<int, int> mapRegionModeCode;
		KDBaseInterModal::GetInterRegionModeDefine(m_pTarget, mapRegionModeCode);
		
		TRegionalParameter oAutoInfo;
		oAutoInfo.TnModeCode	= -1;
		oAutoInfo.TstrModeName	= _T("PassengerCar");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			oAutoInfo.TstrModeName = _T("Auto");
		}

		DefaultInitRegionalChoiceValue(a_nPurposeID, oAutoInfo.TnModeCode, oAutoInfo);
		
		a_mapRegionalChoiceValue.insert(std::make_pair(oAutoInfo.TnModeCode, oAutoInfo));

		AutoType iterCode = mapModeCode.begin();
		AutoType endCode  = mapModeCode.end();
		
		while(iterCode != endCode)
		{
			CString strModeName = iterCode->second;

			AutoType iterRegion = mapRegionModeCode.find(iterCode->first);
			AutoType endRegion  = mapRegionModeCode.end();
			if (iterRegion != endRegion)
			{
				int nTerminalCode = iterRegion->second; 
				TRegionalParameter oInfo;

				oInfo.TnModeCode	= iterRegion->first;
				oInfo.TstrModeName	= strModeName;

				DefaultInitRegionalChoiceValue(a_nPurposeID, oInfo.TnModeCode, oInfo);
				
				a_mapRegionalChoiceValue.insert(std::make_pair(oInfo.TnModeCode, oInfo));
			}

			iterCode++;
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

void KDefaultRegionalModeChoiceParaDlg::ClearSubPage( void )
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

void KDefaultRegionalModeChoiceParaDlg::OnTcnSelchangeTab1( NMHDR *pNMHDR, LRESULT *pResult )
{
	DisplayTabControl(m_tabCtrl.GetCurSel());
	*pResult = 0;
}

void KDefaultRegionalModeChoiceParaDlg::DisplayTabControl( int a_nTabIndex )
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
	}
}

void KDefaultRegionalModeChoiceParaDlg::OnSize( UINT nType, int cx, int cy )
{
	KResizeDialogEx::OnSize(nType, cx, cy);

	SubPageRedrawWindow();
}

void KDefaultRegionalModeChoiceParaDlg::ResizeComponent( void )
{
	SetResize(IDC_STATIC_MEU,   SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_EDIT_MEU,     SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_TAB1,         SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);
	SetResize(IDOK,             SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,         SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_COPY,  SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_PASTE, SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON1,      SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_SHEET, SZ_TOP_RIGHT,    SZ_TOP_RIGHT);
}

void KDefaultRegionalModeChoiceParaDlg::DefaultInitRegionalChoiceValue( 
	int a_nPurposeID, int a_nAlternativeID, TRegionalParameter& a_oRegionalParameter )
{
	std::map<int/*Master_ID*/, TRegionalParameter> mapRegionalChoiceValue;

	KDBaseDefaultParaRegionalModeChoice::
		LoadDefaultRegionalModeChoiceParam(a_nPurposeID, a_nAlternativeID, mapRegionalChoiceValue);

	AutoType bIter = mapRegionalChoiceValue.begin();
	AutoType eIter = mapRegionalChoiceValue.end();


	if (bIter != eIter)
	{
		AutoType fIter = mapRegionalChoiceValue.find(KDefaultParameterMasterCode::USER_MASETER_CODE);

		if (fIter != eIter)
		{
			TRegionalParameter oRegionalParameter = fIter->second;

			a_oRegionalParameter.TdInvehicleTime  = oRegionalParameter.TdInvehicleTime;
			a_oRegionalParameter.TdWaitingTime	  = oRegionalParameter.TdWaitingTime;
			a_oRegionalParameter.TdTransferTime	  = oRegionalParameter.TdTransferTime;
			a_oRegionalParameter.TdProcessTime	  = oRegionalParameter.TdProcessTime;
			a_oRegionalParameter.TdFrequency	  = oRegionalParameter.TdFrequency;
			a_oRegionalParameter.TdCost  = oRegionalParameter.TdCost;
			a_oRegionalParameter.TdDummy = oRegionalParameter.TdDummy;
		}
		else
		{
			fIter = mapRegionalChoiceValue.find(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);

			TRegionalParameter oRegionalParameter = fIter->second;

			a_oRegionalParameter.TdInvehicleTime  = oRegionalParameter.TdInvehicleTime;
			a_oRegionalParameter.TdWaitingTime	  = oRegionalParameter.TdWaitingTime;
			a_oRegionalParameter.TdTransferTime	  = oRegionalParameter.TdTransferTime;
			a_oRegionalParameter.TdProcessTime	  = oRegionalParameter.TdProcessTime;
			a_oRegionalParameter.TdFrequency	  = oRegionalParameter.TdFrequency;
			a_oRegionalParameter.TdCost  = oRegionalParameter.TdCost;
			a_oRegionalParameter.TdDummy = oRegionalParameter.TdDummy;

			if (fIter == eIter)
				TxLogDebugException();
		}
	}
	else
	{
		//Default 정의

		if (a_nAlternativeID == BUS_TRMINAL)
		{
			a_oRegionalParameter.TdInvehicleTime	= 0.00;
			a_oRegionalParameter.TdWaitingTime		= 0.00;
			a_oRegionalParameter.TdTransferTime		= 0.00;
			a_oRegionalParameter.TdProcessTime		= 0.00;
			a_oRegionalParameter.TdFrequency		= 0.00;
			a_oRegionalParameter.TdCost				= 0.00;
			a_oRegionalParameter.TdDummy			= 0.00;
		}
		else if(a_nAlternativeID == INTER_REGIONAL_RAILSTATION && a_oRegionalParameter.TstrModeName != _T("KTX"))
		{
			a_oRegionalParameter.TdInvehicleTime	= 0.00;
			a_oRegionalParameter.TdWaitingTime		= 0.00;
			a_oRegionalParameter.TdTransferTime		= 0.00;
			a_oRegionalParameter.TdProcessTime		= 0.00;
			a_oRegionalParameter.TdFrequency		= 0.00;
			a_oRegionalParameter.TdCost				= 0.00;
			a_oRegionalParameter.TdDummy			= 0.00;
		}
		else if(a_nAlternativeID == INTER_REGIONAL_RAILSTATION && a_oRegionalParameter.TstrModeName == _T("KTX"))
		{
			a_oRegionalParameter.TdInvehicleTime	= 0.00;
			a_oRegionalParameter.TdWaitingTime		= 0.00;
			a_oRegionalParameter.TdTransferTime		= 0.00;
			a_oRegionalParameter.TdProcessTime		= 0.00;
			a_oRegionalParameter.TdFrequency		= 0.00;
			a_oRegionalParameter.TdCost				= 0.00;
			a_oRegionalParameter.TdDummy			= 0.00;
		}
		else if (a_nAlternativeID == AIR_TERMIANL)
		{
			a_oRegionalParameter.TdInvehicleTime	= 0.00;
			a_oRegionalParameter.TdWaitingTime		= 0.00;
			a_oRegionalParameter.TdTransferTime		= 0.00;
			a_oRegionalParameter.TdProcessTime		= 0.00;
			a_oRegionalParameter.TdFrequency		= 0.00;
			a_oRegionalParameter.TdCost				= 0.00;
			a_oRegionalParameter.TdDummy			= 0.00;
		}
		else
		{
			a_oRegionalParameter.TdInvehicleTime	= 0.00;
			a_oRegionalParameter.TdWaitingTime		= 0.00;
			a_oRegionalParameter.TdTransferTime		= 0.00;
			a_oRegionalParameter.TdProcessTime		= 0.00;
			a_oRegionalParameter.TdFrequency		= 0.00;
			a_oRegionalParameter.TdCost				= 0.00;
			a_oRegionalParameter.TdDummy			= 0.00;
		}
	}
}

void KDefaultRegionalModeChoiceParaDlg::SaveUserDefaultRegionChoicePara( void )
{
	try
	{
		AutoType bIter = m_mapSubPage.begin();
		AutoType eIter = m_mapSubPage.end();

		while (bIter != eIter)
		{
			KDefaultRegionalModeChoiceParaSubPage* pSubPage = bIter->second;
			KRegionalChoiceParameter oRegionalParameter;
			pSubPage->GetParameter(oRegionalParameter);

			KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceParam(
				KDefaultParameterMasterCode::USER_MASETER_CODE, oRegionalParameter);

			++bIter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}

void KDefaultRegionalModeChoiceParaDlg::SaveSystemDefaultRegionChoicePara( void )
{
	try
	{
		AutoType bIter = m_mapSubPage.begin();
		AutoType eIter = m_mapSubPage.end();

		while (bIter != eIter)
		{
			KDefaultRegionalModeChoiceParaSubPage* pSubPage = bIter->second;
			KRegionalChoiceParameter oRegionalParameter;
			pSubPage->GetParameter(oRegionalParameter);

			std::map<int, TRegionalParameter>& mapValue = oRegionalParameter.mapModeRegionalChoiceValue;

			AutoType beginIter = mapValue.begin();
			AutoType endIter   = mapValue.end();

			while (beginIter != endIter)
			{
				int                 nAlternativeID     = beginIter->first;
				TRegionalParameter& oRegionalParameter = beginIter->second;

				if (nAlternativeID == BUS_TRMINAL)
				{
					oRegionalParameter.TdInvehicleTime		= 0.00;
					oRegionalParameter.TdWaitingTime		= 0.00;
					oRegionalParameter.TdTransferTime		= 0.00;
					oRegionalParameter.TdProcessTime		= 0.00;
					oRegionalParameter.TdFrequency			= 0.00;
					oRegionalParameter.TdCost				= 0.00;
					oRegionalParameter.TdDummy				= 0.00;
				}
				else if(nAlternativeID == INTER_REGIONAL_RAILSTATION && oRegionalParameter.TstrModeName != _T("KTX"))
				{
					oRegionalParameter.TdInvehicleTime		= 0.00;
					oRegionalParameter.TdWaitingTime		= 0.00;
					oRegionalParameter.TdTransferTime		= 0.00;
					oRegionalParameter.TdProcessTime		= 0.00;
					oRegionalParameter.TdFrequency			= 0.00;
					oRegionalParameter.TdCost				= 0.00;
					oRegionalParameter.TdDummy				= 0.00;
				}
				else if(nAlternativeID == INTER_REGIONAL_RAILSTATION && oRegionalParameter.TstrModeName == _T("KTX"))
				{
					oRegionalParameter.TdInvehicleTime		= 0.00;
					oRegionalParameter.TdWaitingTime		= 0.00;
					oRegionalParameter.TdTransferTime		= 0.00;
					oRegionalParameter.TdProcessTime		= 0.00;
					oRegionalParameter.TdFrequency			= 0.00;
					oRegionalParameter.TdCost				= 0.00;
					oRegionalParameter.TdDummy				= 0.00;
				}
				else if (nAlternativeID == AIR_TERMIANL)
				{
					oRegionalParameter.TdInvehicleTime		= 0.00;
					oRegionalParameter.TdWaitingTime		= 0.00;
					oRegionalParameter.TdTransferTime		= 0.00;
					oRegionalParameter.TdProcessTime		= 0.00;
					oRegionalParameter.TdFrequency			= 0.00;
					oRegionalParameter.TdCost				= 0.00;
					oRegionalParameter.TdDummy				= 0.00;
				}
				else
				{
					oRegionalParameter.TdInvehicleTime		= 0.00;
					oRegionalParameter.TdWaitingTime		= 0.00;
					oRegionalParameter.TdTransferTime		= 0.00;
					oRegionalParameter.TdProcessTime		= 0.00;
					oRegionalParameter.TdFrequency			= 0.00;
					oRegionalParameter.TdCost				= 0.00;
					oRegionalParameter.TdDummy				= 0.00;
				}

				++beginIter;
			}

			KDBaseDefaultParaRegionalModeChoice::SaveDefaultRegionalModeChoiceParam(
				KDefaultParameterMasterCode::SYSTEM_MASTER_CODE, oRegionalParameter);

			++bIter;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw ex;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}
}


void KDefaultRegionalModeChoiceParaDlg::OnBnClickedBtnInitialize()
{
	try
	{
		//MEU Initialize
		CString strValue;
		strValue.Format(_T("%.2f"), m_dMEU);
		SetDlgItemText(IDC_EDIT_MEU, strValue);

		//Tab Initialize
		AutoType bIter = m_mapSubPage.begin();
		AutoType eIter = m_mapSubPage.end();

		while (bIter != eIter)
		{
			KDefaultRegionalModeChoiceParaSubPage* pPage = bIter->second;
			pPage->UpdateReportData();

			++bIter;
		}

	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		QBicMessageBox::MessageBox(_T("초기화중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
	catch (...)
	{
		TxLogDebugException();
		QBicMessageBox::MessageBox(_T("초기화중 오류가 생겼습니다."), _T("Information"));
		KResizeDialogEx::OnCancel();
	}
}


void KDefaultRegionalModeChoiceParaDlg::OnBnClickedButtonCopy()
{
	try
	{
		int nTabIndex = m_tabCtrl.GetCurSel();
		std::map<int, KDefaultRegionalModeChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->GetParameter(m_tmpRegionalChoiceParameter);
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


void KDefaultRegionalModeChoiceParaDlg::OnBnClickedButtonPaste()
{
	try
	{
		int nTabIndex = m_tabCtrl.GetCurSel();
		std::map<int, KDefaultRegionalModeChoiceParaSubPage*>::iterator fIter = m_mapSubPage.find(nTabIndex);

		if (fIter != m_mapSubPage.end())
		{
			fIter->second->UpdateReportData(m_tmpRegionalChoiceParameter.mapModeRegionalChoiceValue);
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

void KDefaultRegionalModeChoiceParaDlg::SubPageRedrawWindow( void )
{
	if (m_pCurrentShowClientDlg != nullptr)
	{        
		CRect oTabRect;
		m_tabCtrl.GetClientRect(&oTabRect);	
		AutoType iter = m_mapSubPage.begin();
		AutoType end  = m_mapSubPage.end();

		while (iter != end)
		{
			KDefaultRegionalModeChoiceParaSubPage* pSubPage = iter->second;
			pSubPage->SetWindowPos(NULL, TAB_LEFT_MARGIN, TAB_TOP_MARGIN, oTabRect.Width()-TAB_WIDTH_MARGIN, oTabRect.Height()-TAB_HEIGHT_MARGIN, SWP_NOZORDER);

			++iter;
		}
	}
}
