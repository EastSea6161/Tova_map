// TableVehicleConvAddFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TableVehicleConvAddFieldDlg.h"
#include "afxdialogex.h"
#include "Target.h"
#include "ExistNameChecker.h"
//#include "CodeManager.h"
//#include "CodeGroup.h"
#include "FAEditModeDlg.h"
#include "TableAddFieldInfo.h"

// KTableVehicleConvAddFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableVehicleConvAddFieldDlg, KResizeDialogEx)

KTableVehicleConvAddFieldDlg::KTableVehicleConvAddFieldDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KTableVehicleConvAddFieldDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KTableVehicleConvAddFieldDlg::~KTableVehicleConvAddFieldDlg()
{
}

void KTableVehicleConvAddFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_cboDataType);
	DDX_Control(pDX, IDC_COMBO1, m_cboCodeGroup);
	DDX_Control(pDX, IDC_COMBO3, m_cboMode);
}

BEGIN_MESSAGE_MAP(KTableVehicleConvAddFieldDlg, KResizeDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_RADIO1, IDC_RADIO2, &KTableVehicleConvAddFieldDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_CHECK_DEF, &KTableVehicleConvAddFieldDlg::OnBnClickedCheckDef)
	ON_BN_CLICKED(IDC_BUTTON1, &KTableVehicleConvAddFieldDlg::OnBnClickedButtonMode)
	ON_BN_CLICKED(IDOK, &KTableVehicleConvAddFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &KTableVehicleConvAddFieldDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KTableVehicleConvAddFieldDlg 메시지 처리기입니다.
BOOL KTableVehicleConvAddFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KTableVehicleConvAddFieldDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	InitGroupCode();
	InitDataType();
	InitModeCode();

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
	CheckRadioButtonState();

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	return TRUE;
}

void KTableVehicleConvAddFieldDlg::InitGroupCode( void )
{
	try
	{
		m_cboCodeGroup.ResetContent();

		std::vector<KCodeGroup*> vecCodeGroup;
		KCodeManager* pCodeManager = m_pTarget->CodeManager();
		pCodeManager->GetCodeGroups(vecCodeGroup);

		size_t nCount = vecCodeGroup.size();
		for (size_t i = 0; i < nCount; i++)
		{
			KCodeGroup* pGroup = vecCodeGroup[i];
			CString strName = pGroup->CodeGroupName();

			int nCur = m_cboCodeGroup.AddString(strName);
			m_cboCodeGroup.SetItemData(nCur, (DWORD_PTR)pGroup);
		}

		if (nCount >0)
		{
			m_cboCodeGroup.SetCurSel(0);
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

void KTableVehicleConvAddFieldDlg::InitModeCode( void )
{
	try
	{
		m_cboMode.ResetContent();

		KDBaseAssignImport::GetFmodeInfo( m_pTarget, m_mapFAModeInfo);

		AutoType iter = m_mapFAModeInfo.begin();
		AutoType end  = m_mapFAModeInfo.end();
		while(iter != end)
		{
			KFAModeInfo oInfo = iter->second;

			int nCur = m_cboMode.AddString(oInfo.strFomdeName);
			m_cboMode.SetItemData(nCur, (DWORD_PTR)oInfo.nFmode_id);
			++iter;
		}

		if (m_mapFAModeInfo.size() > 0)
		{
			m_cboMode.SetCurSel(0);
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

void KTableVehicleConvAddFieldDlg::InitDataType( void )
{
	m_cboDataType.ResetContent();

	try
	{
		std::vector< CString > dataTypeArray;

		dataTypeArray.push_back(DATATYPESTRING_DOUBLE);
		dataTypeArray.push_back(DATATYPESTRING_INTEGER);
		dataTypeArray.push_back(DATATYPESTRING_STRING);

		std::vector< CString >::iterator itDataType = dataTypeArray.begin();
		int nIndex = 0;
		while( dataTypeArray.end() != itDataType )
		{
			m_cboDataType.InsertString(nIndex++, *itDataType );
			++itDataType;
		}
		m_cboDataType.SetCurSel( 0 );

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

void KTableVehicleConvAddFieldDlg::OnBnClickedRadio( UINT nID )
{
	CheckRadioButtonState();
}

void KTableVehicleConvAddFieldDlg::OnBnClickedCheckDef()
{
	CheckModeDefine();
}

void KTableVehicleConvAddFieldDlg::CheckModeDefine( void )
{

	if (IsDlgButtonChecked(IDC_CHECK_DEF) == TRUE)
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		
		m_cboDataType.SetCurSel(0);
	}
	else
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
	}
}


void KTableVehicleConvAddFieldDlg::OnBnClickedButtonMode()
{
	KFAEditModeDlg oDlg(m_pTarget);
	if (oDlg.DoModal() == IDOK)
	{
		InitModeCode();
	}
}

void KTableVehicleConvAddFieldDlg::CheckRadioButtonState( void )
{
	int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2 );

	if (nCheck == IDC_RADIO1)
	{
		GetDlgItem(IDC_STATIC_MODENAME)->SetWindowTextW(_T("Type"));
		GetDlgItem(IDC_COMBO1)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);

		CheckModeDefine();
	}
	else
	{
		GetDlgItem(IDC_STATIC_MODENAME)->SetWindowTextW(_T("Code"));
		GetDlgItem(IDC_COMBO1)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
}


void KTableVehicleConvAddFieldDlg::OnBnClickedOk()
{
	try
	{
		KIOTable* pVehicleTable = m_pTarget->Tables()->FindTable(TABLE_VEHICLE_MODE_OD);
		if (AddNewColumn(pVehicleTable) == false)
		{
			return;
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

	KResizeDialogEx::OnOK();
}


void KTableVehicleConvAddFieldDlg::OnBnClickedCancel()
{
	KResizeDialogEx::OnCancel();
}

bool KTableVehicleConvAddFieldDlg::AddNewColumn(KIOTable* a_pTable)
{
	try
	{
		const KIOColumns* pColumns = a_pTable->Columns();
		int nMaxDisPlayCount(1);
		int nCount = pColumns->ColumnCount();

		CString strName(_T(""));
		GetDlgItemText(IDC_EDIT1, strName);
		if (strName.CompareNoCase(_T("")) == 0)
		{
			AfxMessageBox(_T("입력한 컬럼명이 없습니다."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}

		if (strName.Find(_T(' ')) > -1)
		{
			AfxMessageBox(_T("컬럼명에 공백이 존재합니다.\n다시 입력해 주세요."));
			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}
		
		CString strErrCh(_T(""));
		if (QBicStringChecker::IsValidColumnName(strName, strErrCh) == false)
		{
			CString strErrMsg(_T(""));
			strErrMsg.Format(_T("명칭에 \'%s\' 문자를 사용할 수 없습니다."), strErrCh);
			AfxMessageBox(strErrMsg);

			GetDlgItem(IDC_EDIT1)->SetFocus();
			return false;
		}

		if (nCount != 0)
		{
			for (int i = 0; i < nCount; i++)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				int nDisplayCount = pColumn->DisplayOrder();

				if (strName.CompareNoCase(pColumn->Caption()) == 0 )
				{
					AfxMessageBox(_T("입력한 컬럼명이 이미 존재합니다.\n다시 입력해 주세요."));
					return false;
				}

				if (nMaxDisPlayCount < nDisplayCount)
				{
					nMaxDisPlayCount = nDisplayCount;
				}
			}
		}

		KIOColumn oColumn;
		oColumn.Name(a_pTable->GetUsableColumnName());
		oColumn.Caption(strName);
		oColumn.DisplayOrder(nMaxDisPlayCount +1);
		oColumn.ColumnInfo(strName);
		oColumn.PrimaryKey(false);
		oColumn.Visible(true);

		int nCheck = GetCheckedRadioButton( IDC_RADIO1, IDC_RADIO2 );

		if (nCheck == IDC_RADIO1)
		{
			oColumn.CodeType(KEMIOCodeTypeIsNothing);
			if (IsDlgButtonChecked(IDC_CHECK_DEF) == TRUE)
			{
				oColumn.DataType(KEMIODataTypeDouble);

				int nModeCur = m_cboMode.GetCurSel();
				int nModeID  = (int)m_cboMode.GetItemData(nModeCur);

				KTableAddFieldInfo::UpdateVehicleModel(m_pTarget, a_pTable->GetUsableColumnName(),nModeID );
			}
			else
			{
				CString strDataType = _T("");
				int nDataCur = m_cboDataType.GetCurSel();
				m_cboDataType.GetLBText(nDataCur, strDataType);

				if( strDataType.CompareNoCase(DATATYPESTRING_INTEGER) == 0 )
				{
					oColumn.DataType(KEMIODataTypeInteger);
				}
				else if( strDataType.CompareNoCase(DATATYPESTRING_DOUBLE) == 0 )
				{
					oColumn.DataType(KEMIODataTypeDouble);
				}
				else if( strDataType.CompareNoCase(DATATYPESTRING_STRING) == 0 )
				{
					oColumn.DataType(KEMIODataTypeString);
				}
			}
		}
		else
		{
			oColumn.DataType(KEMIODataTypeInteger);
			oColumn.CodeType(KEMIOCodeTypeIsSingleCode);
			int nCodeCur = m_cboCodeGroup.GetCurSel();
			KCodeGroup* pGroup = (KCodeGroup*)m_cboCodeGroup.GetItemData(nCodeCur);
			oColumn.CodeGroup(pGroup->CodeGroupKey());
		}

		a_pTable->AddColumn(oColumn);
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
		return false;
	}
	catch(...)
	{
		TxLogDebugException();
		return false;
	}
	a_pTable->Notify();

	return true;
}
