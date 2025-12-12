// EditVehicleFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableEditVehicleFieldDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "TableEditFieldInfo.h"
#include "FAEditModeDlg.h"

// KEditVehicleFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableEditVehicleFieldDlg, KResizeDialogEx)

KTableEditVehicleFieldDlg::KTableEditVehicleFieldDlg(KTarget* a_pTarget, KIOColumn* a_pColumn, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableEditVehicleFieldDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
	, m_pColumn(a_pColumn)
{

}

KTableEditVehicleFieldDlg::~KTableEditVehicleFieldDlg()
{

}

void KTableEditVehicleFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboMode);
}


BEGIN_MESSAGE_MAP(KTableEditVehicleFieldDlg, KResizeDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KTableEditVehicleFieldDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDOK, &KTableEditVehicleFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableEditVehicleFieldDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KEditVehicleFieldDlg 메시지 처리기입니다.
BOOL KTableEditVehicleFieldDlg::PreTranslateMessage( MSG* a_pMsg )
{
	if (a_pMsg->message == WM_KEYDOWN)
	{
		UINT nID = GetFocus()->GetDlgCtrlID();

		if (nID == IDC_EDIT2 && a_pMsg->wParam == VK_RETURN)
		{
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
			int    nLen  = pEdit->GetWindowTextLength();

			pEdit->SetSel(nLen, nLen);
			pEdit->ReplaceSel(_T("\r\n"));

			return TRUE;
		}
	}

	return KResizeDialogEx::PreTranslateMessage(a_pMsg);
}


BOOL KTableEditVehicleFieldDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
	KTableEditFieldInfo::IsRegisterFieldVehicleConverModel(spDBaseConnection, *m_pColumn, m_nModeCode);
	InitModeCode();

	SetDlgItemText(IDC_EDIT1, m_pColumn->Caption());
	SetDlgItemText(IDC_EDIT2, m_pColumn->ColumnInfo());

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableEditVehicleFieldDlg::InitModeCode( void )
{
	try
	{
		int nSelCur(0);

		m_cboMode.ResetContent();

		KDBaseAssignImport::GetFmodeInfo( m_pTarget, m_mapFAModeInfo);

		AutoType iter = m_mapFAModeInfo.begin();
		AutoType end  = m_mapFAModeInfo.end();
		while(iter != end)
		{
			KFAModeInfo oInfo = iter->second;

			int nCur = m_cboMode.AddString(oInfo.strFomdeName);
			if (oInfo.nFmode_id == m_nModeCode)
			{
				nSelCur = nCur;
			}

			m_cboMode.SetItemData(nCur, (DWORD_PTR)oInfo.nFmode_id);
			++iter;
		}

		if (m_mapFAModeInfo.size() > 0)
		{
			m_cboMode.SetCurSel(nSelCur);
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

void KTableEditVehicleFieldDlg::OnBnClickedButtonEdit()
{
	KFAEditModeDlg oDlg(m_pTarget);
	if (oDlg.DoModal() == IDOK)
	{
		InitModeCode();
	}
}


void KTableEditVehicleFieldDlg::OnBnClickedOk()
{
	
	if (InvalidateInput() == false)
	{
		return;
	}

	CString strNewDisplayName(_T("")); 
	GetDlgItemText(IDC_EDIT1, strNewDisplayName);
	CString strNewDescription(_T("")); 
	GetDlgItemText(IDC_EDIT2, strNewDescription);

	KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
	pTable->BeginTransaction();

	try
	{
		KIOColumn oColumn = *m_pColumn;
		oColumn.Caption(strNewDisplayName);
		oColumn.ColumnInfo(strNewDescription);

		pTable->UpdateColumn(oColumn);

		pTable->Commit();
	}
	catch(...)
	{
		TxLogDebugException();
		pTable->Rollback();
		return;
	}

	int nSelCur = m_cboMode.GetCurSel();
	int nMode	= (int)m_cboMode.GetItemData(nSelCur); 

	try
	{
		TVehicleModelKey oInfo;
		KTableEditFieldInfo::GetVehicleModeInfo(m_pTarget->GetDBaseConnection(), m_pColumn->Name(), oInfo);
		if (nMode != oInfo.nFmode_id)
		{
			KTableEditFieldInfo::UpdateVehicleModelMode(m_pTarget->GetDBaseConnection(), oInfo, nMode);
		}
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return;
	}
	catch(...)
	{
		TxLogDebugException();
		return;
	}
	pTable->Notify();

	KResizeDialogEx::OnOK();
}


void KTableEditVehicleFieldDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}


bool KTableEditVehicleFieldDlg::InvalidateInput()
{
	try
	{
		CString strNewDescription(_T("")); 
		GetDlgItemText(IDC_EDIT2, strNewDescription);
		strNewDescription.Trim();
		SetDlgItemText(IDC_EDIT2, strNewDescription);

		CString strNewDisplayName(_T("")); 
		GetDlgItemText(IDC_EDIT1, strNewDisplayName);
		strNewDisplayName.Trim();
		SetDlgItemText(IDC_EDIT1, strNewDisplayName);

		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
		KIOColumns* pColumns = pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for (int i = 0; i < nColumnCount; i++)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);

			if (pColumn == m_pColumn)
			{
				continue;
			}

			if (strNewDisplayName.CompareNoCase(pColumn->Caption()) == 0)
			{
				AfxMessageBox(_T("입력된 컬럼명이 이미 존재합니다."));
				return false;
			}
		}


		if (strNewDisplayName.IsEmpty() == true)
		{
			AfxMessageBox(_T("컬럼명을 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}

		if (strNewDisplayName.Find(_T(' ')) > -1)
		{
			AfxMessageBox(_T("컬럼명에 공백이 존재합니다. 다시 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}

		CString strErrCh(_T(""));
		if (QBicStringChecker::IsValidColumnName(strNewDisplayName, strErrCh) == false)
		{
			CString strErrMsg(_T(""));
			strErrMsg.Format(_T("명칭에 \'%s\' 문자를 사용할 수 없습니다."), strErrCh);
			AfxMessageBox(strErrMsg);

			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}
	}
	catch (...)
	{
		TxLogDebugException();	
		return false;
	}

	return true;
}
