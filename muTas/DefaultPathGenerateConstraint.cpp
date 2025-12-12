// DefaultMultimodalPathGeneration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultPathGenerateConstraint.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"

// KDefaultMultimodalPathGeneration 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultPathGenerateConstraint, KResizeDialogEx)

KDefaultPathGenerateConstraint::KDefaultPathGenerateConstraint(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultPathGenerateConstraint::IDD, pParent)
{

}

KDefaultPathGenerateConstraint::~KDefaultPathGenerateConstraint()
{
}

void KDefaultPathGenerateConstraint::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDefaultPathGenerateConstraint, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,		 &KDefaultPathGenerateConstraint::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,  &KDefaultPathGenerateConstraint::OnBnClickedCancel)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultPathGenerateConstraint::OnBnClickedDefault)
END_MESSAGE_MAP()


// KDefaultMultimodalPathGeneration 메시지 처리기입니다.


BOOL KDefaultPathGenerateConstraint::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(m_mapDefaultPathGenerateConstraint);
		KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateInvehicle(m_mapDefaultPathGenerateInvehicle);
         
		AutoType iterConstraint = m_mapDefaultPathGenerateConstraint.begin();
		AutoType endConstraint  = m_mapDefaultPathGenerateConstraint.end();
		if(iterConstraint != endConstraint)
		{
			AutoType iterDefault = m_mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			if(iterDefault != endConstraint)
			{
				InitConstraintConditions(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitConstraintConditions(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}
		}
		else
		{
			SetConstraintConditions();
			SaveConstraintConditions(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}

		AutoType iterInvehicle  = m_mapDefaultPathGenerateInvehicle.begin();
		AutoType endInvehicle   = m_mapDefaultPathGenerateInvehicle.end();

		if (iterInvehicle != endInvehicle)
		{
			AutoType iterDefaultInvehicle  = m_mapDefaultPathGenerateInvehicle.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
			AutoType endDefaultInvehicle   = m_mapDefaultPathGenerateInvehicle.end();
			if(iterDefaultInvehicle != endDefaultInvehicle)
			{
				InitInVehicleCongestionPara(KDefaultParameterMasterCode::USER_MASETER_CODE);
			}
			else
			{
				InitInVehicleCongestionPara(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
			}

		}
		else
		{
           SetInVehicleCongestionPara();
		   SaveInVehicleCongestionPara(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
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

void KDefaultPathGenerateConstraint::InitConstraintConditions(int a_nMasterCode)
{
	try
	{
		AutoType iter = m_mapDefaultPathGenerateConstraint.find(a_nMasterCode);

		KPathGenerateConstraint oPathGenerateContraint = iter->second;

		double  dPathTime		= oPathGenerateContraint.dPathTime;
		int		nInvehicleTime	= oPathGenerateContraint.nInvehicleTime;
		int		nWatingTime		= oPathGenerateContraint.nWatingTime;
		int		nTransferTime	= oPathGenerateContraint.nTransferTime;
		int		nAETime			= oPathGenerateContraint.nAETime;
		double	dPathLength		= oPathGenerateContraint.dPathLength;
		double  dPathCost       = oPathGenerateContraint.dPathCost;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), dPathTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
		SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

		strValue.Format(_T("%.2f"), dPathLength);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
		SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

		strValue.Format(_T("%.2f"), dPathCost);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHCOST);
		SetDlgItemText(IDC_EDIT_PATHCOST, strValue);

		if (1 == nInvehicleTime)
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
		}

		if (1 == nWatingTime)
		{
			CheckDlgButton(IDC_CHECK_WATING, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_WATING, FALSE);
		}

		if (1 == nTransferTime)
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
		}

		if (1 == nAETime)
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
		}
		else
		{
			CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
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

void KDefaultPathGenerateConstraint::InitInVehicleCongestionPara(int a_nMasterCode)
{
	try
	{
		AutoType iter = m_mapDefaultPathGenerateInvehicle.find(a_nMasterCode);

		KPathGenerateInvehicle oPathGenerateInvehicle = iter->second;

		double  dAlpha		= oPathGenerateInvehicle.dAlpha;
		double	dBeta		= oPathGenerateInvehicle.dBeta;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), dAlpha);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
		SetDlgItemText(IDC_EDIT_ALPHA, strValue);

		strValue.Format(_T("%.2f"), dBeta);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
		SetDlgItemText(IDC_EDIT_BETA, strValue);
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

void KDefaultPathGenerateConstraint::SetConstraintConditions()
{
	try
	{
		double  dPathTime	= 1.5;
		double	dPathLength	= 1.5;
		double  dPathCost	= 1.5;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), dPathTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHTIME);
		SetDlgItemText(IDC_EDIT_PATHTIME, strValue);

		strValue.Format(_T("%.2f"), dPathLength);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHLENGTH);
		SetDlgItemText(IDC_EDIT_PATHLENGTH, strValue);

		strValue.Format(_T("%.2f"), dPathCost);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PATHCOST);
		SetDlgItemText(IDC_EDIT_PATHCOST, strValue);

		CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
		CheckDlgButton(IDC_CHECK_WATING, TRUE);
		CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
		CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
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

void KDefaultPathGenerateConstraint::SetInVehicleCongestionPara()
{
	try
	{
		double  dAlpha			= 0.15;
		double	dBeta			= 0.15;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.2f"), dAlpha);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ALPHA);
		SetDlgItemText(IDC_EDIT_ALPHA, strValue);

		strValue.Format(_T("%.2f"), dBeta);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BETA);
		SetDlgItemText(IDC_EDIT_BETA, strValue);
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

bool KDefaultPathGenerateConstraint::InvalidateInputInfo()
{
	try
	{
		double  dInputValue(0.0);
		CString strPathTime;
		GetDlgItemText(IDC_EDIT_PATHTIME, strPathTime);

		if (!QBicStringChecker::IsNumeric(strPathTime))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strPathTime);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_PATHTIME)->SetFocus();
			return false;
		}

		CString strPathLength;
		GetDlgItemText(IDC_EDIT_PATHLENGTH, strPathLength);

		if (!QBicStringChecker::IsNumeric(strPathLength))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strPathLength);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_PATHLENGTH)->SetFocus();
			return false;
		}

		CString strAlpha;
		GetDlgItemText(IDC_EDIT_ALPHA, strAlpha);

		if (!QBicStringChecker::IsNumeric(strAlpha))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_ALPHA)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strAlpha);
		if (dInputValue < 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_ALPHA)->SetFocus();
			return false;
		}

		CString strBeta;
		GetDlgItemText(IDC_EDIT_BETA, strBeta);

		if (!QBicStringChecker::IsNumeric(strBeta))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_BETA)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strBeta);
		if (dInputValue < 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));	
			GetDlgItem(IDC_EDIT_BETA)->SetFocus();
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

void KDefaultPathGenerateConstraint::SaveInVehicleCongestionPara(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format (_T(" Delete From %s where Master_Code = 1 "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_INVEHICLE);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else if( a_nMasterCode == KDefaultParameterMasterCode::SYSTEM_MASTER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format (_T(" Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_INVEHICLE);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		CString strAlpha	    (_T(""));
		CString strBeta         (_T(""));

		GetDlgItemText(IDC_EDIT_ALPHA,	strAlpha);
		GetDlgItemText(IDC_EDIT_BETA,   strBeta);

		double dAlpha        = _ttof(strAlpha);
		double dBeta         = _ttof(strBeta);

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_INVEHICLE);
		strSQL.AppendFormat(_T(" (Master_Code, Alpha, Beta) "));
		strSQL.AppendFormat(_T(" VALUES (%d, %.2f, %.2f)"),a_nMasterCode, dAlpha, dBeta);

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
}

void KDefaultPathGenerateConstraint::SaveConstraintConditions(int a_nMasterCode)
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();


		if ( a_nMasterCode == KDefaultParameterMasterCode::USER_MASETER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format (_T(" Delete From %s where Master_Code = 1 "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}
		else if( a_nMasterCode == KDefaultParameterMasterCode::SYSTEM_MASTER_CODE)
		{
			CString strDelQuery(_T(""));
			strDelQuery.Format (_T(" Delete From %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT);
			spDBaseConnection->ExecuteUpdate(strDelQuery);
		}

		int	   nInvehicleTime = 0;
		int	   nWaitingTime   = 0;
		int	   nTransferTime  = 0;
		int	   nAETime		  = 0;

		CString strPathTime	    (_T(""));
		CString strPathLength   (_T(""));
		CString strPathCost		(_T(""));

		GetDlgItemText(IDC_EDIT_PATHTIME,	strPathTime);
		GetDlgItemText(IDC_EDIT_PATHLENGTH, strPathLength);
		GetDlgItemText(IDC_EDIT_PATHCOST,   strPathCost);

		double dPathTime      = _ttof(strPathTime);
		double dPathLength    = _ttof(strPathLength);
		double dPathCost    = _ttof(strPathCost);

	    if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
		{
			nInvehicleTime = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
		{
			nWaitingTime = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
		{
			nTransferTime = 1;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
		{
			nAETime = 1;
		}

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T(" Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_PATH_GENERATE_CONSTRAINT);
		strSQL.AppendFormat(_T(" ( Master_Code, PathTime, InvehicleTime, WaitingTime, TransferTime, AETime, PathLength, PathCost ) "));
		strSQL.AppendFormat(_T(" VALUES ( %d, %.2f, %d, %d, %d, %d, %.2f, %.2f )")
					  	  ,a_nMasterCode, dPathTime, nInvehicleTime, nWaitingTime, nTransferTime, nAETime, dPathLength, dPathCost);
	
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
}

void KDefaultPathGenerateConstraint::OnBnClickedOk()
{
	UpdateData(TRUE);
	try
	{
		if ( !InvalidateInputInfo())
		{
			return;
		}
		SaveInVehicleCongestionPara(KDefaultParameterMasterCode::USER_MASETER_CODE);
		SaveConstraintConditions(KDefaultParameterMasterCode::USER_MASETER_CODE);
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

void KDefaultPathGenerateConstraint::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultPathGenerateConstraint::OnBnClickedDefault()
{
	try
	{
		AutoType iterDefault = m_mapDefaultPathGenerateConstraint.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endDefault = m_mapDefaultPathGenerateConstraint.end();
		if(iterDefault != endDefault)
		{
			InitConstraintConditions(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(m_mapDefaultPathGenerateConstraint);
			InitConstraintConditions(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
		}
		AutoType iterDefaultInvehicle  = m_mapDefaultPathGenerateInvehicle.find(KDefaultParameterMasterCode::USER_MASETER_CODE);
		AutoType endDefaultInvehicle   = m_mapDefaultPathGenerateInvehicle.end();
		if(iterDefaultInvehicle != endDefaultInvehicle)
		{
			InitInVehicleCongestionPara(KDefaultParameterMasterCode::USER_MASETER_CODE);
		}
		else
		{
			KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateInvehicle(m_mapDefaultPathGenerateInvehicle);
			InitInVehicleCongestionPara(KDefaultParameterMasterCode::SYSTEM_MASTER_CODE);
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
