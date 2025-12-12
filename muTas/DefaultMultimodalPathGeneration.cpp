// DefaultBusCongestionPara.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultMultimodalPathGeneration.h"
#include "afxdialogex.h"
#include "DBaseConnector.h"
#include "DefineNamesDefaultPara.h"


// KDefaultBusCongestionPara 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultMultimodalPathGeneration, KResizeDialogEx)

KDefaultMultimodalPathGeneration::KDefaultMultimodalPathGeneration(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KDefaultMultimodalPathGeneration::IDD, pParent)
{

}

KDefaultMultimodalPathGeneration::~KDefaultMultimodalPathGeneration()
{
}

void KDefaultMultimodalPathGeneration::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KDefaultMultimodalPathGeneration, KResizeDialogEx)
	ON_BN_CLICKED(IDOK,		 &KDefaultMultimodalPathGeneration::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL,	 &KDefaultMultimodalPathGeneration::OnBnClickedCancel)
	ON_BN_CLICKED(IDDEFAULT, &KDefaultMultimodalPathGeneration::OnBnClickedDefault)
END_MESSAGE_MAP()

// KDefaultBusCongestionPara 메시지 처리기입니다.

BOOL KDefaultMultimodalPathGeneration::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	try
	{
		KDBaseDefaultParaMultimodalPathGeneration::LoadDefaultMultimodalPathInfo(m_mapDefaultMultimodalPathInfo);

		AutoType iter = m_mapDefaultMultimodalPathInfo.find(1);
		AutoType end  = m_mapDefaultMultimodalPathInfo.end();

		if (iter != end)
		{
			InitPathInfo();
		}
		else
		{
			DefultPathInfo();
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

void KDefaultMultimodalPathGeneration::InitPathInfo()
{
	try
	{
		AutoType iter = m_mapDefaultMultimodalPathInfo.find(1);

		KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

		int    nMax_Routes	   = oMultimodalPathInfo.nMax_Routes;
		double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
		double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;
		int    nWalkTime	   = oMultimodalPathInfo.nWalkTime;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%d"), nMax_Routes);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAXROUTES);
		SetDlgItemText(IDC_EDIT_MAXROUTES, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeBus);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWBUS);
		SetDlgItemText(IDC_EDIT_DWBUS, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeRail);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWRAIL);
		SetDlgItemText(IDC_EDIT_DWRAIL, strValue);

		strValue.Format(_T("%d"), nWalkTime);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_WALKTIME);
		SetDlgItemText(IDC_EDIT_WALKTIME, strValue);

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

void KDefaultMultimodalPathGeneration::DefultPathInfo()
{
	try
	{
		AutoType iter = m_mapDefaultMultimodalPathInfo.find(0);
		AutoType end  = m_mapDefaultMultimodalPathInfo.end();

		if ( iter == end)
		{
			return;
		}
		else
		{
			KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

			int    nMax_Routes	   = oMultimodalPathInfo.nMax_Routes;
			double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
			double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;
			int    nWalkTime	   = oMultimodalPathInfo.nWalkTime;

			CEdit* pEdit;
			CString strValue(_T(""));

			strValue.Format(_T("%d"), nMax_Routes);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAXROUTES);
			SetDlgItemText(IDC_EDIT_MAXROUTES, strValue);

			strValue.Format(_T("%.1f"), dDwellTimeBus);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWBUS);
			SetDlgItemText(IDC_EDIT_DWBUS, strValue);

			strValue.Format(_T("%.1f"), dDwellTimeRail);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWRAIL);
			SetDlgItemText(IDC_EDIT_DWRAIL, strValue);

			strValue.Format(_T("%d"), nWalkTime);
			pEdit = (CEdit*)GetDlgItem(IDC_EDIT_WALKTIME);
			SetDlgItemText(IDC_EDIT_WALKTIME, strValue);
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

bool KDefaultMultimodalPathGeneration::InvalidateInputInfo()
{
	try
	{
		int  nInputValue(0);
		CString strMaxRoutes;
		GetDlgItemText(IDC_EDIT_MAXROUTES, strMaxRoutes);

		if (!QBicStringChecker::IsNumeric(strMaxRoutes))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAXROUTES)->SetFocus();
			return false;
		}
		nInputValue = _ttoi(strMaxRoutes);
		if (nInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_MAXROUTES)->SetFocus();
			return false;
		}

		double  dInputValue(0.0);
		CString strDWBus;
		GetDlgItemText(IDC_EDIT_DWBUS, strDWBus);

		if (!QBicStringChecker::IsNumeric(strDWBus))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWBUS)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strDWBus);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWBUS)->SetFocus();
			return false;
		}

		CString strDWRail;
		GetDlgItemText(IDC_EDIT_DWRAIL, strDWRail);

		if (!QBicStringChecker::IsNumeric(strDWRail))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWRAIL)->SetFocus();
			return false;
		}
		dInputValue = _ttof(strDWRail);
		if (dInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWRAIL)->SetFocus();
			return false;
		}
		
		CString strWalkTime;
		GetDlgItemText(IDC_EDIT_WALKTIME, strWalkTime);

		if (!QBicStringChecker::IsNumeric(strWalkTime))
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_WALKTIME)->SetFocus();
			return false;
		}
		nInputValue = _ttoi(strWalkTime);
		if (nInputValue <= 0)
		{
			AfxMessageBox(_T("0보다 큰 숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_WALKTIME)->SetFocus();
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

void KDefaultMultimodalPathGeneration::SavePathInfo()
{
	KDBaseConPtr spDBaseConnection = KDBase::GetProjectDBConnection();

	try
	{
		spDBaseConnection->BeginTransaction();

		CString strDelQuery(_T(""));
		strDelQuery.Format(_T("Delete From %s Where Master_Code = 1"), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_PATH_GENERATION);
		spDBaseConnection->ExecuteUpdate(strDelQuery);

		int nMasterCode	= 1;

		CString strMaxRoute  (_T(""));
		GetDlgItemText(IDC_EDIT_MAXROUTES, strMaxRoute);
		int nMaxRoute = _ttoi(strMaxRoute);

		CString strDWBus	 (_T(""));
		GetDlgItemText(IDC_EDIT_DWBUS,	   strDWBus);
		double dDWBus = _ttof(strDWBus);

		CString strDWRail    (_T(""));
		GetDlgItemText(IDC_EDIT_DWRAIL,    strDWRail);
		double dDWRail = _ttof(strDWRail);

		CString strWalkTime  (_T(""));
		GetDlgItemText(IDC_EDIT_WALKTIME,  strWalkTime);
		int nWalkTime = _ttoi(strWalkTime);

		CString strSQL(_T(""));
		strSQL.AppendFormat(_T("Insert Into %s "), KDefaultParaTable::TABLE_DEFAULT_MULTIMODAL_PATH_GENERATION);
		strSQL.AppendFormat(_T(" (Master_Code, Max_Routes, DwellTime_Bus, DwellTime_Rail, WalkTime) "));
		strSQL.AppendFormat(_T(" VALUES (%d, %d, %.1f, %.1f, %d)"), nMasterCode, nMaxRoute, dDWBus, dDWRail, nWalkTime);

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

void KDefaultMultimodalPathGeneration::OnBnClickedOk()
{
	UpdateData(TRUE);
	try
	{
		if ( !InvalidateInputInfo())
		{
			return;
		}
		SavePathInfo();
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

void KDefaultMultimodalPathGeneration::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

void KDefaultMultimodalPathGeneration::OnBnClickedDefault()
{
	try
	{
		AutoType iter = m_mapDefaultMultimodalPathInfo.find(0);
		AutoType end  = m_mapDefaultMultimodalPathInfo.end();

		if (iter == end)
		{
			AfxMessageBox(_T("설정값 정보가 없습니다."));
			return;
		}
		else
		{
			DefultPathInfo();
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
