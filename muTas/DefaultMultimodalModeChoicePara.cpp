// DefaultModeChoicePara.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultMultimodalModeChoicePara.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

enum
{
	_0_COLUMN_Variable,
	_1_COLUMN_Highway,
	_2_COLUMN_Rail,
	_3_COLUMN_Composite
};

// KDefaultMultimodalModeChoicePara 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultMultimodalModeChoicePara, KResizeDialogEx)

KDefaultMultimodalModeChoicePara::KDefaultMultimodalModeChoicePara(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultMultimodalModeChoicePara::IDD, pParent)
{

}

KDefaultMultimodalModeChoicePara::~KDefaultMultimodalModeChoicePara()
{
}

void KDefaultMultimodalModeChoicePara::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KDefaultMultimodalModeChoicePara, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,      &KDefaultMultimodalModeChoicePara::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,  &KDefaultMultimodalModeChoicePara::OnBnClickedCancel)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultMultimodalModeChoicePara::OnBnClickedDefault)
END_MESSAGE_MAP()


// KDefaultMultimodalModeChoicePara 메시지 처리기입니다.


BOOL KDefaultMultimodalModeChoicePara::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KReportCtrlSetting::Default(m_wndReportCtrl, TRUE);

	try
	{
		KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoicePara   (m_mapDefaultModeChoicePara);
		KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoiceSetting(m_mapDefaultModeChoiceSetting);
		KDBaseDefaultParaMultimodalModeChoice::GetVariableCodeName         (m_mapVariableCodeName);

		InitReportHeader();
		ResizeComponent();

		AutoType iterReport  = m_mapDefaultModeChoicePara.begin();
		AutoType endReport   = m_mapDefaultModeChoicePara.end();
		if(iterReport != endReport)
		{
			AutoType iterDefaultReport  = m_mapDefaultModeChoicePara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterDefaultReport != endReport)
			{
				InitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
		else
		{
			SetReportRecord();
			SaveModeChoicePara(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}

		AutoType iterSetting = m_mapDefaultModeChoiceSetting.begin();
		AutoType endSetting  = m_mapDefaultModeChoiceSetting.end();

		if (iterSetting != endSetting)
		{
			AutoType iterDefaultSetting  = m_mapDefaultModeChoiceSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterDefaultSetting != endSetting)
			{
				InitModeChoiceSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitModeChoiceSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
		else
		{
			SetModeChoiceSetting();
			SaveModeChoiceSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		OnBnClickedCancel();
	}
	return TRUE;
}


void KDefaultMultimodalModeChoicePara::ResizeComponent()
{
	SetResize(IDC_SEPRATOR1,   SZ_TOP_LEFT,  SZ_TOP_RIGHT);
	SetResize(IDC_REPORT   ,   SZ_TOP_LEFT,  SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR2,   SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
	SetResize(IDC_SEPRATOR3,   SZ_BOTTOM_LEFT,SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC2,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC3,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC36,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC12,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);

	SetResize(IDC_STATIC5,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC4,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC6,     SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_MEU,    SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_TIME,   SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDC_EDIT_DWTIME, SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDDEFAULT,       SZ_BOTTOM_LEFT,SZ_BOTTOM_LEFT);
	SetResize(IDOK,			   SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,		   SZ_BOTTOM_RIGHT,SZ_BOTTOM_RIGHT);
}

void KDefaultMultimodalModeChoicePara::InitReportHeader()
{
	CXTPReportColumn* pColumn = nullptr;

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_Variable,  _T("Variable"),               30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("설명변수"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_COLUMN_Highway,   _T("Bus Transit(Upper)"), 30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("버스"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_Rail,      _T("Rail Transit(Upper)"),    30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("도시철도"));
	}

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_3_COLUMN_Composite, _T("Composite(Upper)"),       30));
	pColumn->SetHeaderAlignment(xtpColumnTextCenter);
	if (KmzSystem::GetLanguage() == KEMKorea) {
		pColumn->SetCaption(_T("버스+도시철도"));
	}
}

void KDefaultMultimodalModeChoicePara::InitReportRecord(int a_nMasterCode)
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		AutoType iter = m_mapDefaultModeChoicePara.find(a_nMasterCode);
		AutoType end  = m_mapDefaultModeChoicePara.end();

		if ( iter != end )
		{
			std::vector<KModeChoicePara>& vecModeChoicePara = iter->second;
			size_t nUserCount = vecModeChoicePara.size();

			for (size_t i = 0; i < nUserCount; i++)
			{
				KModeChoicePara oModeChoicePara = vecModeChoicePara[i];
				int     nVariableCode 			= oModeChoicePara.nVariableCode;
				double  dhighwayTransit			= oModeChoicePara.dHighwayTransit;
				double  dRailTransit			= oModeChoicePara.dRailTransit;
				double  dCompositeMode			= oModeChoicePara.dCompositeMode;

				int nUserCountCombo = 0;
				nUserCountCombo++;

				AutoType iterCode = m_mapVariableCodeName.find(nVariableCode);
				AutoType endCode  = m_mapVariableCodeName.end();

				if (iterCode != endCode)
				{
					CString strVariable = iterCode->second;
					if (KmzSystem::GetLanguage() == KEMKorea) {
						if ( ( strVariable.CompareNoCase(_T("역/정류장 개수")) != 0 ) && ( strVariable.CompareNoCase(_T("더미")) != 0 ) ) {
							strVariable = _T("평균 ") + strVariable;
						}
					}

					pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());

					pItem = pRecord->AddItem(new CXTPReportRecordItemText (strVariable));
					pItem ->SetItemData((DWORD_PTR) (nUserCountCombo+1));
					pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
					pItem ->SetAlignment(xtpColumnTextCenter);
					pItem ->SetEditable(FALSE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dhighwayTransit, _T("%.9f")));
					pItem ->SetAlignment(xtpColumnTextRight);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRailTransit,    _T("%.9f")));
					pItem ->SetAlignment(xtpColumnTextRight);
					pItem ->SetEditable(TRUE);

					pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dCompositeMode,  _T("%.9f")));
					pItem ->SetAlignment(xtpColumnTextRight);
					pItem ->SetEditable(TRUE);
				}
			    m_wndReportCtrl.Populate();
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

void KDefaultMultimodalModeChoicePara::InitModeChoiceSetting(int a_nMasterCode)
{
	try
	{
		AutoType iter = m_mapDefaultModeChoiceSetting.find(a_nMasterCode);
		AutoType end  = m_mapDefaultModeChoiceSetting.end();

		if ( iter != end )
		{
			KModeChoiceSetting oModeChoiceSetting = iter->second;

			double  dMEU			= oModeChoiceSetting.dMEU;
			double	dBoardingTime	= oModeChoiceSetting.dBoardingTime;
			double  dDwellTime		= oModeChoiceSetting.dDwellTime;

			CEdit* pEdit;
			CString strValue(_T(""));

			strValue.Format(_T("%.7f"), dMEU);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEU);
			SetDlgItemText(IDC_EDIT_MEU, strValue);

			strValue.Format(_T("%.2f"), dBoardingTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
			SetDlgItemText(IDC_EDIT_TIME, strValue);

			strValue.Format(_T("%.2f"), dDwellTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWTIME);
			SetDlgItemText(IDC_EDIT_DWTIME, strValue);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;  
}

void KDefaultMultimodalModeChoicePara::SetModeChoiceSetting()
{
	try
	{
		double  dMEU			= 0.1089390;
		double	dBoardingTime	= 2.0;
		double  dDwellTime		= 0.50;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.7f"), dMEU);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MEU);
		SetDlgItemText(IDC_EDIT_MEU, strValue);

		strValue.Format(_T("%.2f"), dBoardingTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

		strValue.Format(_T("%.2f"), dDwellTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWTIME);
		SetDlgItemText(IDC_EDIT_DWTIME, strValue);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw -1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw -1;
	}
	return;  
}

void KDefaultMultimodalModeChoicePara::SetReportRecord()
{
	try
	{
		m_wndReportCtrl.ResetContent(TRUE);

		CXTPReportRecords*		pRecords		 = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*		pRecord			 = nullptr;
		CXTPReportRecordItem*	pItem			 = nullptr;

		int     nSystemCount		= 9;
		double  dhighwayTransit[9]	= {-0.0213900000, -0.0061910000, -0.0061910000, -0.0213900000, -0.0213900000, -2.7463900000, -0.0008800000, -0.0064570000, -0.0064570000};
		double  dRailTransit[9]		= {-0.0213900000, -0.0061910000, -0.0061910000, -0.0213900000, -0.0213900000, -2.7463900000, -0.0008800000, -0.0064570000, -0.0064570000};
		double  dCompositeMode[9]	= {-0.0213900000, -0.0061910000, -0.0061910000, -0.0213900000, -0.0213900000, -2.7463900000, -0.0008800000, -0.0064570000, -0.0064570000};

		for (int i=0; i<nSystemCount; i++)
		{

			AutoType iterCode = m_mapVariableCodeName.find(i+1);
			CString strVariable = iterCode->second;

			pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
			pItem = pRecord->AddItem(new CXTPReportRecordItemText (strVariable));
			pItem ->SetBackgroundColor(NOTCHANGE_COLUMN_COLOR);
			pItem ->SetAlignment(xtpColumnTextCenter);
			pItem ->SetEditable(FALSE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dhighwayTransit[i], _T("%.9f")));
			pItem ->SetAlignment(xtpColumnTextRight);
			pItem ->SetEditable(TRUE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dRailTransit[i],    _T("%.9f")));
			pItem ->SetAlignment(xtpColumnTextRight);
			pItem ->SetEditable(TRUE);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dCompositeMode[i],  _T("%.9f")));
			pItem ->SetAlignment(xtpColumnTextRight);
			pItem ->SetEditable(TRUE);

			m_wndReportCtrl.Populate();
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

bool KDefaultMultimodalModeChoicePara::InvalidateInputInfo()
{
	try
	{
		double  dInputValue(0.0);
		CString strMEU;
		GetDlgItemText(IDC_EDIT_MEU, strMEU);

		if (!QBicStringChecker::IsNumeric(strMEU))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strMEU);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MEU)->SetFocus();
			return false;
		}

		CString strTime;
		GetDlgItemText(IDC_EDIT_TIME, strTime);

		if (!QBicStringChecker::IsNumeric(strTime))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_TIME)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strTime);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_TIME)->SetFocus();
			return false;
		}

		CString strDWTime;
		GetDlgItemText(IDC_EDIT_DWTIME, strDWTime);

		if (!QBicStringChecker::IsNumeric(strDWTime))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strDWTime);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
			return false;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		throw 1;
	}
	return true;
}

void KDefaultMultimodalModeChoicePara::SaveModeChoiceSetting(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_SETTING);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strMEU  (_T(""));
		GetDlgItemText(IDC_EDIT_MEU, strMEU);
		double dMEU = _ttof(strMEU);

		CString strBoardingTime	 (_T(""));
		GetDlgItemText(IDC_EDIT_TIME,	strBoardingTime);
		double dBoardingTime = _ttof(strBoardingTime);

		CString strDwellTime    (_T(""));
		GetDlgItemText(IDC_EDIT_DWTIME,    strDwellTime);
		double dDwellTime = _ttof(strDwellTime);

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_SETTING);
		strSQL.AppendFormat(_T(" (Master_Code, MEU, BoardingTime, DwellTime) "));
		strSQL.AppendFormat(_T(" VALUES (%d, %.7f, %.2f, %.2f)"), a_nMasterCode, dMEU, dBoardingTime, dDwellTime);

		spDBaseConnection->ExecuteUpdate(strSQL);
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
	return;
}

void KDefaultMultimodalModeChoicePara::SaveModeChoicePara(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{    
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("delete from %s where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format(_T("delete from %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		int	nClassTime = 0;

		CString          strPreparedQuery (_T(""));
		strPreparedQuery.Format(
			_T(" INSERT INTO %s ")
			_T(" ( Master_Code, Variable_Code, Highway_Transit, Rail_Transit, Composite_Mode ) ")
			_T(" VALUES ")
			_T(" ( ?,?,?,?,? ) "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_MODECHOICE_PARA);

		KPreparedStatementPtr spPrepareStatement = spDBaseConnection->PrepareStatement(strPreparedQuery);

		CXTPReportRecords*          pRecords  = m_wndReportCtrl.GetRecords();
		CXTPReportRecord*           pRecord   = nullptr;
		CXTPReportRecordItem*       pItem     = nullptr;
		CXTPReportRecordItemText*   pItemText = nullptr;
		CXTPReportRecordItemNumber* pItemNum  = nullptr;

		int							nRecordCount      = pRecords->GetCount();
		CString 					strHighwayTransit (_T(""));
		CString 					strRailTransit    (_T(""));
		CString 					strCompositeMode  (_T(""));
		double						dHighwayTransit   = 0;
		double						dRailTransit      = 0;
		double						dCompositeMode    = 0;

		for (int i = 0; i < nRecordCount; i++)
		{
			pRecord           = pRecords->GetAt(i);

			pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_1_COLUMN_Highway);
			strHighwayTransit = pItemNum ->GetCaption(NULL);
			dHighwayTransit   = _ttof(strHighwayTransit);

			pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_2_COLUMN_Rail);
			strRailTransit    = pItemNum ->GetCaption(NULL);
			dRailTransit      = _ttof(strRailTransit);

			pItemNum          = (CXTPReportRecordItemNumber*)pRecord->GetItem(_3_COLUMN_Composite);
			strCompositeMode  = pItemNum ->GetCaption(NULL);
			dCompositeMode    = _ttof(strCompositeMode);

			spPrepareStatement->BindInt    (1, a_nMasterCode);
			spPrepareStatement->BindInt    (2, i+1);
			spPrepareStatement->BindDouble (3, dHighwayTransit);
			spPrepareStatement->BindDouble (4, dRailTransit);
			spPrepareStatement->BindDouble (5, dCompositeMode);
						
			spPrepareStatement->ExecuteUpdate();
			spPrepareStatement->Reset();
		}
		spDBaseConnection->Commit();
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		spDBaseConnection->RollBack();
		throw 1;
	}
	catch(...)
	{
		TxLogDebugException();
		spDBaseConnection->RollBack();
		throw 1;
	}
	m_wndReportCtrl.Populate();
}

void KDefaultMultimodalModeChoicePara::OnBnClickedOk()
{
	UpdateData(TRUE);
	try
	{
		if ( !InvalidateInputInfo())
		{
			return;
		}
		SaveModeChoicePara(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveModeChoiceSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("저장중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("저장중 오류가 발생하였습니다."));
		return;
	}	

	KResizeDialogEx::OnOK();
}

void KDefaultMultimodalModeChoicePara::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultMultimodalModeChoicePara::OnBnClickedDefault()
{
	try
	{
		AutoType iterReport  = m_mapDefaultModeChoicePara.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endReport   = m_mapDefaultModeChoicePara.end();
		if(iterReport != endReport)
		{
			InitReportRecord(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoicePara   (m_mapDefaultModeChoicePara);
			InitReportRecord(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}

		AutoType iterSetting = m_mapDefaultModeChoiceSetting.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endSetting  = m_mapDefaultModeChoiceSetting.end();

		if (iterSetting != endSetting)
		{
			InitModeChoiceSetting(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			KDBaseDefaultParaMultimodalModeChoice::LoadDefaultModeChoiceSetting(m_mapDefaultModeChoiceSetting);
			InitModeChoiceSetting(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		AfxMessageBox(_T("프로그램 실행 중 오류가 발생하였습니다."));
		return;
	}	
}
