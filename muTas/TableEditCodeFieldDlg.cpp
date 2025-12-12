// TableEditCodeFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableEditCodeFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
#include "Target.h"
#include "TableEditFieldInfo.h"
#include "InitRelationProcess.h"

// KTableEditCodeFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableEditCodeFieldDlg, KDialogEx)

KTableEditCodeFieldDlg::KTableEditCodeFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableEditCodeFieldDlg::IDD, pParent)
	, m_pIOTable(a_pIOTable)
	, m_pIOColumnEdit(a_pIOColumnEdit)
	, m_pTarget(a_pTarget)
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

		KTableEditFieldInfo::InitGroupCode(m_pTarget, m_mapGroupCode);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

KTableEditCodeFieldDlg::~KTableEditCodeFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		QBicDelete(m_pNameChecker);
	}
}

void KTableEditCodeFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO3, m_cboCodeGroup);
}


BEGIN_MESSAGE_MAP(KTableEditCodeFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK,     &KTableEditCodeFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableEditCodeFieldDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO3, &KTableEditCodeFieldDlg::OnCbnSelchangeComboGroupCode)
END_MESSAGE_MAP()


// KTableEditCodeFieldDlg 메시지 처리기입니다.

BOOL KTableEditCodeFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KTableEditCodeFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, m_pIOColumnEdit->Caption());
	SetDlgItemText(IDC_EDIT2, m_pIOColumnEdit->ColumnInfo());
	CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	InitComboCode();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableEditCodeFieldDlg::InitComboCode()
{
	try
	{
		m_cboCodeGroup.ResetContent();

		int nInitCurSel(0);

		AutoType itGroupCode = m_mapGroupCode.begin();
		while( m_mapGroupCode.end() != itGroupCode )
		{
			int nGroupKey = itGroupCode->first;
			int nCur      = m_cboCodeGroup.AddString(itGroupCode->second);
			m_cboCodeGroup.SetItemData(nCur, nGroupKey);

			if (m_pIOColumnEdit->CodeGroup() == nGroupKey)
			{
				nInitCurSel = nCur;
			}

			++itGroupCode;
		}

		if (m_mapGroupCode.size() > 0)
		{
			m_cboCodeGroup.SetCurSel(nInitCurSel);
			EnableCheckCtrl();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
};


void KTableEditCodeFieldDlg::OnBnClickedOk()
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

	int nCur      = m_cboCodeGroup.GetCurSel();
	int nGroupKey = (int)m_cboCodeGroup.GetItemData(nCur);

	try
	{
		CWaitCursor cw;

		m_pIOTable->BeginTransaction();

		KIOColumn oIOColumnUpdate = *m_pIOColumnEdit;
		oIOColumnUpdate.Caption   (strNewDisplayName);
		oIOColumnUpdate.ColumnInfo(strNewDescription);

		bool bChangedCode(false);
		if (m_pIOColumnEdit->CodeGroup() != nGroupKey) // 변경이 일어 날시
		{
			oIOColumnUpdate.CodeGroup(nGroupKey);
			bChangedCode = true;
		}

		m_pIOTable->UpdateColumn(oIOColumnUpdate);
		m_pIOTable->Commit();

		if (bChangedCode && IsDlgButtonChecked(IDC_CHECK1) == BST_CHECKED)
		{
			KTableEditFieldInfo::DefaultCodeFiledData(m_pIOTable, oIOColumnUpdate);
			m_pIOTable->Notify();
		}

		if (bChangedCode)
		{
			KInitRelationProcess::DelGroupGeneration(m_pTarget, m_pIOTable->Name(), oIOColumnUpdate.Name());
		}
	}
	catch (...)
	{
		m_pIOTable->Rollback();
		TxLogDebugException();
		return;
	}

	KDialogEx::OnOK();
}


bool KTableEditCodeFieldDlg::InvalidateInput()
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


void KTableEditCodeFieldDlg::OnCbnSelchangeComboGroupCode()
{
	EnableCheckCtrl();
}


void KTableEditCodeFieldDlg::EnableCheckCtrl()
{
	try
	{
		int nCur      = m_cboCodeGroup.GetCurSel();
		int nGroupKey = (int)m_cboCodeGroup.GetItemData(nCur);

		if (m_pIOColumnEdit->CodeGroup() != nGroupKey) // 변경이 일어 날시
		{
			GetDlgItem(IDC_CHECK1)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
			CheckDlgButton(IDC_CHECK1, BST_UNCHECKED);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTableEditCodeFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableEditCodeFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
