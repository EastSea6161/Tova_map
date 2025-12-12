// TableZoneEditFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableZoneEditFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
#include "Project.h"
#include "Purpose.h"
#include "TableEditFieldInfo.h"
#include "InitRelationProcess.h"
#include "KmzSystem.h"

// KTableZoneEditFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableZoneEditFieldDlg, KDialogEx)

KTableZoneEditFieldDlg::KTableZoneEditFieldDlg(KIOTable* a_pIOTable, KIOColumn* a_pIOColumnEdit, KTarget* a_pTarget, KProject* a_pProject, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableZoneEditFieldDlg::IDD, pParent)
	, m_pIOTable     (a_pIOTable)
	, m_pIOColumnEdit(a_pIOColumnEdit)
	, m_pProject     (a_pProject)
	, m_pTarget      (a_pTarget)
	, m_nInitObjectID(-1)
	, m_strInitPAFlag(_T(""))
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

	try
	{
		KTableEditFieldInfo::IsRegisterFieldGenerationModel(m_pIOTable->GetDBaseConnection(), *m_pIOColumnEdit, m_nInitObjectID, m_strInitPAFlag);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}

KTableZoneEditFieldDlg::~KTableZoneEditFieldDlg()
{
	if (nullptr != m_pNameChecker)
	{
		QBicDelete(m_pNameChecker);
	}
}

void KTableZoneEditFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cboPurpose);
}


BEGIN_MESSAGE_MAP(KTableZoneEditFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK,     &KTableZoneEditFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableZoneEditFieldDlg::OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO1, &KTableZoneEditFieldDlg::OnCbnSelchangeComboGroupCode)
END_MESSAGE_MAP()


// KTableZoneEditFieldDlg 메시지 처리기입니다.

BOOL KTableZoneEditFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KTableZoneEditFieldDlg::OnInitDialog()
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
	InitRadioPAFlag(m_strInitPAFlag);

	if (KmzSystem::IsPassengerSystem())
	{
		SetDlgItemText(IDC_STATIC_MODENAME, _T("Purpose:"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_MODENAME, _T("품목:"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableZoneEditFieldDlg::InitComboPurpose()
{
	try
	{
		m_cboPurpose.ResetContent();

		int nInitCurSel(0);
		
		std::vector<KPurpose*>  vecPurpose = m_pProject->GetPurpose();
		size_t nxPurposeCount = vecPurpose.size();

		for(size_t i= 0; i< nxPurposeCount; i++)
		{
			KPurpose* pPurpose = vecPurpose[i];

			int nCur = m_cboPurpose.AddString(pPurpose->PurposeName());
			m_cboPurpose.SetItemData( nCur, (DWORD_PTR)( pPurpose ) );

			if (m_nInitObjectID == pPurpose->PurposeID())
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


void KTableZoneEditFieldDlg::InitRadioPAFlag(CString &a_strInitPAFlag)
{
	try
	{
		if (a_strInitPAFlag.Compare(_T("")) == 0)
		{
			return;
		}

		if (a_strInitPAFlag.CompareNoCase(_T("p")) == 0)
		{
			CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
		}
		else
		{
			CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO4);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTableZoneEditFieldDlg::OnBnClickedOk()
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
		int       nCur     = m_cboPurpose.GetCurSel();
		KPurpose* pPurpose = (KPurpose*)m_cboPurpose.GetItemData(nCur);

		TGenerationModelKey oTGenerationModelKey;
		oTGenerationModelKey.nObject_Group_Code        = (int)pPurpose->PurposeGroup();
		oTGenerationModelKey.nDetail_Object_ID         = m_nInitObjectID;
		oTGenerationModelKey.strZone_Column_Name       = m_pIOColumnEdit->Name();
		oTGenerationModelKey.strPA_Flag                = m_strInitPAFlag;

		CString strNewPAFlag(_T("p"));
		if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
			strNewPAFlag = (_T("p"));
		else
			strNewPAFlag = (_T("a"));

		if (KTableEditFieldInfo::UpdateGenerationModelObjectID(m_pIOTable->GetDBaseConnection(), oTGenerationModelKey, pPurpose->PurposeID(), strNewPAFlag))
		{
			KInitRelationProcess::DBFieldNotify(m_pTarget, m_pIOTable->Name());
		}
	}
	catch (...)
	{
		TxLogDebugException();
		return;
	}

	KDialogEx::OnOK();
}


bool KTableZoneEditFieldDlg::InvalidateInput()
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


void KTableZoneEditFieldDlg::OnCbnSelchangeComboGroupCode()
{
	try
	{
		int       nCur      = m_cboPurpose.GetCurSel();
		KPurpose* pPurpose = (KPurpose*)m_cboPurpose.GetItemData(nCur);

		if (m_nInitObjectID == pPurpose->PurposeID())
			InitRadioPAFlag(m_strInitPAFlag);
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTableZoneEditFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableZoneEditFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}