// ParameterDefineTime.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ParameterDefineTime.h"
#include "afxdialogex.h"
#include "Target.h"
//^^ #include "IOTables.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

//#include "StringChecker.h"

// KParameterDefineTime 대화 상자입니다.

IMPLEMENT_DYNAMIC(KParameterDefineTime, KDialogEx)

KParameterDefineTime::KParameterDefineTime(KTarget* a_pTarget, int a_nTimeMethod, KIOColumn* a_pColumn, TAssignRule a_oAssignRule, CWnd* pParent /*=NULL*/)
	: KDialogEx(KParameterDefineTime::IDD, pParent),
	m_pTarget(a_pTarget)
	, m_nTimeMethod(a_nTimeMethod)
	, m_pIOColumnUseLink(a_pColumn)
	, m_oAssignInfo(a_oAssignRule)
{

}

KParameterDefineTime::~KParameterDefineTime()
{
}

void KParameterDefineTime::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USINGFIELD, m_cboUsingLinkField);
}


BEGIN_MESSAGE_MAP(KParameterDefineTime, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, &KParameterDefineTime::OnBnClickedRadioTimeMethod)
	ON_BN_CLICKED(IDOK, &KParameterDefineTime::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KParameterDefineTime::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &KParameterDefineTime::OnBnClickedDefault)
END_MESSAGE_MAP()


// KParameterDefineTime 메시지 처리기입니다.


BOOL KParameterDefineTime::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KDBaseDefaultParaPathGenerateConstraint::LoadDefaultPathGenerateConstraint(mapDefaultPathGenerateConstraint);
	KDBaseDefaultParaMultimodalPathGeneration::LoadDefaultMultimodalPathInfo    (mapDefaultMultimodalPathInfo);

	AutoType iterPathInfo = mapDefaultMultimodalPathInfo.find(1);
	AutoType endPathInfo  = mapDefaultMultimodalPathInfo.end();
	//KMultimodalPathGenerationInfo & oMultimodalPathInfo = iterPathInfo->second;

	AutoType iter = mapDefaultPathGenerateConstraint.find(1);
	AutoType end  = mapDefaultPathGenerateConstraint.end();
	//KPathGenerateConstraint& oPathGenerateContraint = iter->second;

	if((iterPathInfo==endPathInfo) || (iter==end))
	{
		InitSystemConstraintConditions();
		InitSystemPathInfo();
	}
	else
	{
		DefaultConstraintConditions();
		DefultPathInfo();
	}

	InitComboLinkField();

	CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
	OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);

	if (m_nTimeMethod == 2)
	{
		CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_USINGFIELD);
		OnBnClickedRadioTimeMethod(IDC_RADIO_USINGFIELD);
	}
	else
	{
		CheckRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD, IDC_RADIO_INITTIME);
		OnBnClickedRadioTimeMethod(IDC_RADIO_INITTIME);
	}

	//InitAssignRuleValue();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KParameterDefineTime::OnBnClickedRadioTimeMethod( UINT nID )
{
	if(IDC_RADIO_INITTIME == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(FALSE);
	}
	else if(IDC_RADIO_USINGFIELD == nID)
	{
		GetDlgItem(IDC_COMBO_USINGFIELD)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_USINGFIELD)->SetFocus();
	}
}

void KParameterDefineTime::InitComboLinkField()
{
	m_cboUsingLinkField.ResetContent();

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
	const KIOColumns* pIOColumns = pIOTable->Columns();

	int nColCount = pIOColumns->ColumnCount();

	int nCur		= 0;
	int nCurrentCur = 0;
	for (int i= 0; i< nColCount; i++)
	{
		KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
		KEMIOColumnType  emColumnType   = pIOColumn->ColumnType();
		KEMIODataType    emDataType     = pIOColumn->DataType();

		if(KEMIODataTypeDouble == emDataType)
		{
			CString strDisplayName    = pIOColumn->Caption();
			nCur   = m_cboUsingLinkField.AddString(strDisplayName);
			m_cboUsingLinkField.SetItemData( nCur, (DWORD_PTR)pIOColumn );
			if (pIOColumn == m_pIOColumnUseLink)
			{
				nCurrentCur = nCur;
			}
		}
	}

	m_cboUsingLinkField.SetCurSel(nCurrentCur);
}

void KParameterDefineTime::InitAssignRuleValue()
{
	CString strValue(_T(""));
	strValue.Format(_T("%.2f"), m_oAssignInfo.dDWTimeBusstop);
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
	SetDlgItemText(IDC_EDIT_TIME, strValue);

	strValue.Format(_T("%.2f"), m_oAssignInfo.dDwTimeRailStation);
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DWTIME);
	SetDlgItemText(IDC_EDIT_DWTIME, strValue);

	if (1 == m_oAssignInfo.nInvehicle)
	{
		CheckDlgButton(IDC_CHECK_INVEHICLE, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_INVEHICLE, FALSE);
	}

	if (1 == m_oAssignInfo.nWating)
	{
		CheckDlgButton(IDC_CHECK_WATING, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_WATING, FALSE);
	}

	if (1 == m_oAssignInfo.nTransfer)
	{
		CheckDlgButton(IDC_CHECK_TRANSFER, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_TRANSFER, FALSE);
	}

	if (1 == m_oAssignInfo.nAETime)
	{
		CheckDlgButton(IDC_CHECK_ACCESSEGRESS, TRUE);
	}
	else
	{
		CheckDlgButton(IDC_CHECK_ACCESSEGRESS, FALSE);
	}
}


void KParameterDefineTime::OnBnClickedOk()
{
	try
	{
		if (false == InvalidateInputInfo())
		{
			return;
		}

		CString strDwellbus;
		GetDlgItemText(IDC_EDIT_TIME, strDwellbus);
		m_oAssignInfo.dDWTimeBusstop = _ttof(strDwellbus);

		CString strDWellTime;
		GetDlgItemText(IDC_EDIT_DWTIME, strDWellTime);
		m_oAssignInfo.dDwTimeRailStation = _ttof(strDWellTime);

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_INVEHICLE))
		{
			m_oAssignInfo.nInvehicle = 1;
		}
		else
		{
			m_oAssignInfo.nInvehicle = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_WATING))
		{
			m_oAssignInfo.nWating = 1;
		}
		else
		{
			m_oAssignInfo.nWating = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_TRANSFER))
		{
			m_oAssignInfo.nTransfer = 1;
		}
		else
		{
			m_oAssignInfo.nTransfer = 0;
		}

		if (TRUE == IsDlgButtonChecked(IDC_CHECK_ACCESSEGRESS))
		{
			m_oAssignInfo.nAETime = 1;
		}
		else
		{
			m_oAssignInfo.nAETime = 2;
		}

		int        nCurSel				= -1;
		KIOColumn* pIOColumnLink		= nullptr;


		if( IDC_RADIO_INITTIME == GetCheckedRadioButton(IDC_RADIO_INITTIME, IDC_RADIO_USINGFIELD) )
		{
			m_nTimeMethod  =  1;

			KIOTables* pIOTables = m_pTarget->Tables();
			KIOTable*  pIOTable  = pIOTables->FindTable(TABLE_LINK);
			const KIOColumns* pIOColumns = pIOTable->Columns();

			int nColCount = pIOColumns->ColumnCount();
			for (int i= 0; i< nColCount; i++)
			{
				KIOColumn*       pIOColumn      = pIOColumns->GetColumn(i);
				CString          strColumnName  = pIOColumn->Name();

				if(strColumnName.CompareNoCase(COLUMN_LINK_SPEED) == 0)
				{
					pIOColumnLink = pIOColumn;
				}
			}
		}
		else
		{
			m_nTimeMethod   =  2;
			nCurSel         = m_cboUsingLinkField.GetCurSel();

			if(nCurSel >= 0)
			{
				pIOColumnLink = (KIOColumn*)m_cboUsingLinkField.GetItemData(nCurSel);
			}
			else
			{
				AfxMessageBox(_T("링크통행시간 관련 컬럼을 선택해 주세요."));
				m_cboUsingLinkField.SetFocus();
				return;
			}
		}

		m_pIOColumnUseLink = pIOColumnLink;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		AfxMessageBox(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	KDialogEx::OnOK();
}


void KParameterDefineTime::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}

bool KParameterDefineTime::InvalidateInputInfo()
{
	try
	{
		CString strDwellTimebus;
		GetDlgItemText(IDC_EDIT_TIME, strDwellTimebus);

		if (!QBicStringChecker::IsNumeric(strDwellTimebus))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_TIME)->SetFocus();
			return false;
		}


		CString strDWellTime;
		GetDlgItemText(IDC_EDIT_DWTIME, strDWellTime);

		if (!QBicStringChecker::IsNumeric(strDWellTime))
		{
			AfxMessageBox(_T("숫자를 입력해 주세요."));
			GetDlgItem(IDC_EDIT_DWTIME)->SetFocus();
			return false;
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
	return true;
}

void KParameterDefineTime::GetDefineTimeData( int& a_nTimeMethod, KIOColumn*& a_pColumn, TAssignRule& a_oAssignInfo )
{
	try
	{
		a_nTimeMethod = m_nTimeMethod;
		a_pColumn	  = m_pIOColumnUseLink;
		a_oAssignInfo = m_oAssignInfo;
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

void KParameterDefineTime::InitSystemConstraintConditions()
{
	try
	{
		AutoType iter = mapDefaultPathGenerateConstraint.find(0);

		KPathGenerateConstraint oPathGenerateContraint = iter->second;

		int		nInvehicleTime	= oPathGenerateContraint.nInvehicleTime;
		int		nWatingTime		= oPathGenerateContraint.nWatingTime;
		int		nTransferTime	= oPathGenerateContraint.nTransferTime;
		int		nAETime			= oPathGenerateContraint.nAETime;

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

void KParameterDefineTime::DefaultConstraintConditions()
{
	try
	{
		AutoType iter = mapDefaultPathGenerateConstraint.find(1);

		KPathGenerateConstraint oPathGenerateContraint = iter->second;

		int		nInvehicleTime	= oPathGenerateContraint.nInvehicleTime;
		int		nWatingTime		= oPathGenerateContraint.nWatingTime;
		int		nTransferTime	= oPathGenerateContraint.nTransferTime;
		int		nAETime			= oPathGenerateContraint.nAETime;

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

void KParameterDefineTime::DefultPathInfo()
{
	try
	{
		AutoType iter = mapDefaultMultimodalPathInfo.find(1);

		KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

		double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
		double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.1f"), dDwellTimeBus);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeRail);
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

void KParameterDefineTime::InitSystemPathInfo()
{
	try
	{
		AutoType iter = mapDefaultMultimodalPathInfo.find(0);

		KMultimodalPathGenerationInfo& oMultimodalPathInfo = iter->second;

		double dDwellTimeBus   = oMultimodalPathInfo.dDwellTimeBus;		
		double dDwellTimeRail  = oMultimodalPathInfo.dDwellTimeRail;

		CEdit* pEdit;
		CString strValue(_T(""));

		strValue.Format(_T("%.1f"), dDwellTimeBus);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		SetDlgItemText(IDC_EDIT_TIME, strValue);

		strValue.Format(_T("%.1f"), dDwellTimeRail);
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

void KParameterDefineTime::OnBnClickedDefault()
{
	try
	{	
		AutoType iterPathInfo = mapDefaultMultimodalPathInfo.find(1);
		AutoType endPathInfo  = mapDefaultMultimodalPathInfo.end();
		KMultimodalPathGenerationInfo & oMultimodalPathInfo = iterPathInfo->second;

		AutoType iter = mapDefaultPathGenerateConstraint.find(1);
		AutoType end  = mapDefaultPathGenerateConstraint.end();
		KPathGenerateConstraint& oPathGenerateContraint = iter->second;
		
		if((iterPathInfo==endPathInfo) || (iter==end))
		{
			InitSystemConstraintConditions();
			InitSystemPathInfo();
		}
		else
		{
			DefaultConstraintConditions();
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
