// TablePurposeEditFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TablePurposeEditFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
#include "Project.h"
#include "Purpose.h"
#include "TableEditFieldInfo.h"
#include "InitRelationProcess.h"
#include "KmzSystem.h"

// KTablePurposeEditFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTablePurposeEditFieldDlg, KDialogEx)

KTablePurposeEditFieldDlg::KTablePurposeEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTablePurposeEditFieldDlg::IDD, pParent)
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

KTablePurposeEditFieldDlg::~KTablePurposeEditFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		QBicDelete(m_pNameChecker);
	}
}

void KTablePurposeEditFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboPurpose);
}


BEGIN_MESSAGE_MAP(KTablePurposeEditFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTablePurposeEditFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTablePurposeEditFieldDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// KTablePurposeEditFieldDlg 메시지 처리기입니다.

BOOL KTablePurposeEditFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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


BOOL KTablePurposeEditFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, m_pIOColumnEdit->Caption());
	SetDlgItemText(IDC_EDIT2, m_pIOColumnEdit->ColumnInfo());

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	InitComboPurpose();

	if (KmzSystem::IsPassengerSystem())
	{
		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetDlgItemText(IDC_STATIC_MODENAME, _T("통행목적 :"));
		}
		else {
			SetDlgItemText(IDC_STATIC_MODENAME, _T("Purpose :"));
		}
	}
	else
	{
		SetDlgItemText(IDC_STATIC_MODENAME, _T("품목:"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTablePurposeEditFieldDlg::InitComboPurpose()
{
	try
	{
		m_cboPurpose.ResetContent();

		int nInitCurSel(0);

		int nObjectID(-1); 
		KTableEditFieldInfo::IsRegisterFieldDistiributionModel(m_pIOTable->GetDBaseConnection(), *m_pIOColumnEdit, nObjectID);

		std::vector<KPurpose*>  vecPurpose = m_pProject->GetPurpose();
		size_t nxPurposeCount = vecPurpose.size();

		for(size_t i= 0; i< nxPurposeCount; i++)
		{
			KPurpose* pPurpose = vecPurpose[i];

			int nCur = m_cboPurpose.AddString(pPurpose->PurposeName());
			m_cboPurpose.SetItemData( nCur, (DWORD_PTR)( pPurpose ) );

			if (nObjectID == pPurpose->PurposeID())
			{
				nInitCurSel = nCur;
			}
		}

		if (nxPurposeCount > 0)
		{
			m_cboPurpose.SetCurSel(nInitCurSel);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
};


void KTablePurposeEditFieldDlg::OnBnClickedOk()
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
		int nCurObjectID(0);
		if (KTableEditFieldInfo::IsRegisterFieldDistiributionModel(m_pIOTable->GetDBaseConnection(), *m_pIOColumnEdit, nCurObjectID))
		{
			int       nCur     = m_cboPurpose.GetCurSel();
			KPurpose* pPurpose = (KPurpose*)m_cboPurpose.GetItemData(nCur);

			if (nCurObjectID != pPurpose->PurposeID()) // 변경
			{
				TDistributionModelKey oTDistributionModelKey;
				oTDistributionModelKey.nObject_Group_Code        = (int)pPurpose->PurposeGroup();
				oTDistributionModelKey.nDetail_Object_ID           = nCurObjectID;
				oTDistributionModelKey.strPurpose_OD_Column_Name = m_pIOColumnEdit->Name();

				if (KTableEditFieldInfo::UpdateDistributionModelObjectID(m_pIOTable->GetDBaseConnection(), oTDistributionModelKey, pPurpose->PurposeID()))
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


bool KTablePurposeEditFieldDlg::InvalidateInput()
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


void KTablePurposeEditFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTablePurposeEditFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
