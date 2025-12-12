// TableModeEditFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableModeEditFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
#include "Project.h"
#include "Mode.h"
#include "TableEditFieldInfo.h"
#include "InitRelationProcess.h"

// KTableModeEditFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableModeEditFieldDlg, KDialogEx)

KTableModeEditFieldDlg::KTableModeEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableModeEditFieldDlg::IDD, pParent)
	, m_pIOTable     (a_pIOTable)
	, m_pIOColumnEdit(a_pIOColumnEdit)
	, m_pProject     (a_pProject)
	, m_pTarget      (a_pTarget)
{
	try
	{
		m_pNameChecker = new KExistNameChecker;

		const KIOColumns* pColumns = m_pIOTable->Columns();
		int nCount      = pColumns->ColumnCount();
		for (int i= 0; i< nCount; i++)
		{
			KIOColumn* pColumn     = pColumns->GetColumn(i);
			// 2019.04.11 수정 : 컬럼 중복체크는 caption을 가지고 한다.
			//CString    strColName  = pColumn->Name();
			//if (strColName.CompareNoCase(m_pIOColumnEdit->Name()) == 0)
			//{
			//	continue;
			//}

			//m_pNameChecker->addName(strColName);
			CString strCaption = pColumn->Caption();
			if (strCaption.CompareNoCase(m_pIOColumnEdit->Caption()) == 0)
			{
				continue;
			}
			m_pNameChecker->addName(strCaption);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

KTableModeEditFieldDlg::~KTableModeEditFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		QBicDelete(m_pNameChecker);
	}
}

void KTableModeEditFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboMode);
}


BEGIN_MESSAGE_MAP(KTableModeEditFieldDlg, KDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &KTableModeEditFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableModeEditFieldDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTableModeEditFieldDlg 메시지 처리기입니다.

BOOL KTableModeEditFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

	return KDialogEx::PreTranslateMessage(a_pMsg);
}


BOOL KTableModeEditFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, m_pIOColumnEdit->Caption());
	SetDlgItemText(IDC_EDIT2, m_pIOColumnEdit->ColumnInfo());

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	InitComboMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableModeEditFieldDlg::InitComboMode()
{
	try
	{
		m_cboMode.ResetContent();

		int nInitCurSel(0);


		int nModeID(-1); 
		KTableEditFieldInfo::IsRegisterFieldModeChoiceModel(m_pIOTable->GetDBaseConnection(), *m_pIOColumnEdit, nModeID);

		std::vector<KMode*>  vecMode = m_pProject->GetMode();
		size_t nxModeCount = vecMode.size();

		for(size_t i= 0; i< nxModeCount; i++)
		{
			KMode* pMode = vecMode[i];

			int nCur = m_cboMode.AddString(pMode->ModeName());
			m_cboMode.SetItemData( nCur, (DWORD_PTR)( pMode ) );

			if (nModeID == pMode->ModeID())
			{
				nInitCurSel = nCur;
			}
		}

		if (nxModeCount > 0)
		{
			m_cboMode.SetCurSel(nInitCurSel);
		}

	}
	catch (...)
	{
		TxLogDebugException();	
	}
};


void KTableModeEditFieldDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (!InvalidateInput())
	{
		return;
	}

	CString strNewDisplayName(_T("")); 
	GetDlgItemText(IDC_EDIT1, strNewDisplayName);
	CString strNewDescription(_T("")); 
	GetDlgItemText(IDC_EDIT2, strNewDescription);

	try
	{
		m_pIOTable->BeginTransaction();

		KIOColumn oIOColumnUpdate = *m_pIOColumnEdit;
		oIOColumnUpdate.Caption   (strNewDisplayName);
		oIOColumnUpdate.ColumnInfo(strNewDescription);

		m_pIOTable->UpdateColumn(oIOColumnUpdate);
		m_pIOTable->Commit();
	}
	catch (...)
	{
		m_pIOTable->Rollback();
		TxLogDebugException();
		return;
	}

	try
	{
		int nCurModeID(0);
		if (KTableEditFieldInfo::IsRegisterFieldModeChoiceModel(m_pIOTable->GetDBaseConnection(), *m_pIOColumnEdit, nCurModeID))
		{
			int       nCur     = m_cboMode.GetCurSel();
			KMode*    pMode    = (KMode*)m_cboMode.GetItemData(nCur);

			if (nCurModeID != pMode->ModeID()) // 변경
			{
				TModeChoiceModelKey oTModeChoiceModelKey;
				oTModeChoiceModelKey.nMode_Group_Code        = (int)pMode->ModeGroup();
				oTModeChoiceModelKey.nDetail_Mode_ID         = nCurModeID;
				oTModeChoiceModelKey.strMode_OD_Column_Name  = m_pIOColumnEdit->Name();

				if (KTableEditFieldInfo::UpdateModeChoiceModelModeID(m_pIOTable->GetDBaseConnection(), oTModeChoiceModelKey, pMode->ModeID()))
				{
					KInitRelationProcess::DBFieldNotify(m_pTarget, m_pIOTable->Name());
				}
			}
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	KDialogEx::OnOK();
}


bool KTableModeEditFieldDlg::InvalidateInput()
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

		if (m_pNameChecker->isNameExists(strNewDisplayName) == true)
		{
			CString strMsg(_T(""));
			strMsg.Format(_T("동일한 컬럼명 '%s'이 존재합니다."), strNewDisplayName);
			AfxMessageBox(strMsg);
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
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


void KTableModeEditFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableModeEditFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
