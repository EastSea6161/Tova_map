// TableEditFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableEditFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"

#include "DBaseConnector.h"
#include "DefInterUrbanModeChoiceParaTypes.h"
#include "DBaseDefaultParaInterUrbanModeChoice.h"


// KTableEditFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableEditFieldDlg, KDialogEx)

KTableEditFieldDlg::KTableEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableEditFieldDlg::IDD, pParent)
	, m_pIOTable(a_pIOTable)
	, m_pIOColumnEdit(a_pIOColumnEdit)
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

KTableEditFieldDlg::~KTableEditFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		QBicDelete(m_pNameChecker);
	}
}

void KTableEditFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(KTableEditFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK,     &KTableEditFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableEditFieldDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// KTableEditFieldDlg 메시지 처리기입니다.

BOOL KTableEditFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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


BOOL KTableEditFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	SetDlgItemText(IDC_EDIT1, m_pIOColumnEdit->Caption());
	SetDlgItemText(IDC_EDIT2, m_pIOColumnEdit->ColumnInfo());

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableEditFieldDlg::OnBnClickedOk()
{
	if (!InvalidateInput())
	{
		return;
	}

	CString strNewDisplayName(_T("")); 
	GetDlgItemText(IDC_EDIT1, strNewDisplayName);
	CString strNewDescription(_T("")); 
	GetDlgItemText(IDC_EDIT2, strNewDescription);

	// 2019.04.11 수정 
	// 존테이블의 경우 컬럼 삭제시 해당 컬럼이 존특성변수로 설정된 컬럼인지 확인후 처리
	// 삭제시에는 해당 존 특성변수를 설정 해제 함
	CString strBeforeName = m_pIOColumnEdit->Caption();
	std::vector<int> arrZoneVarId;

	CString strTableName = m_pIOTable->Name();
	if (strNewDisplayName != strBeforeName)
	{
		if (strTableName.CompareNoCase(TABLE_ZONE) == 0)
		{
			std::map<int, TZoneVariableInfo> mapZoneVar;

			KDBaseDefaultParaInterUrbanModeChoice::ReadZoneVariableInfo(mapZoneVar);
			for (auto& aZoneVar : mapZoneVar)
			{
				if (aZoneVar.second.strZoneColDispName == strBeforeName)
				{
					arrZoneVarId.push_back(aZoneVar.second.nId);
				}
			}

			if (arrZoneVarId.size() > 0)
			{
				if (AfxMessageBox(_T("해당 컬럼은 존특성변수로 설정되어 있습니다.\r\n그래도 편집하시겠습니까?"), MB_YESNO) == IDNO)
				{
					return;
				}
			}
		}
	}

	try
	{
		m_pIOTable->BeginTransaction();

		KIOColumn oIOColumnUpdate = *m_pIOColumnEdit;
		oIOColumnUpdate.Caption(strNewDisplayName);
		oIOColumnUpdate.ColumnInfo(strNewDescription);

		m_pIOTable->UpdateColumn(oIOColumnUpdate);
		m_pIOTable->Commit();

		if (arrZoneVarId.size() == 1)
		{
			KDBaseConPtr spDBConn = KDBase::GetProjectDBConnection();
			try
			{
				spDBConn->BeginTransaction();
				if (KDBaseDefaultParaInterUrbanModeChoice::UpdateZoneVariableInfoUnselected(spDBConn, arrZoneVarId)
					&& KDBaseDefaultParaInterUrbanModeChoice::DeleteUrbanModeChoicePara(spDBConn, arrZoneVarId))
				{
					spDBConn->Commit();
				}
				else
				{
					spDBConn->RollBack();
				}
			}
			catch (...)
			{
				spDBConn->RollBack();
			}
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


bool KTableEditFieldDlg::InvalidateInput()
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


void KTableEditFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableEditFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
