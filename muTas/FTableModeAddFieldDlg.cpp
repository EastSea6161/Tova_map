// FTableModeAddFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FTableModeAddFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
#include "Target.h"
#include "Project.h"
#include "Purpose.h"




// KFTableModeAddFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFTableModeAddFieldDlg, KDialogEx)

KFTableModeAddFieldDlg::KFTableModeAddFieldDlg(
	KTarget* a_pTarget, KProject* a_pProject, KIOTable* a_pIOTable, int a_nColumnNumber, int a_nNewDisplayOrder, 
	KEMFreightOD a_emFreightOD, CWnd* pParent /*=NULL*/)
	: KDialogEx(KFTableModeAddFieldDlg::IDD, pParent)
	, m_bCode( false )
	, m_strColumnName(_T(""))
	, m_strColumnInfo(_T(""))
	, m_nCodeGroup(-1)
	, m_strModeName(_T(""))
	, m_nColumnMode(0)
	, m_pTarget(a_pTarget)
	, m_pProject(a_pProject)
	, m_pIOTable(a_pIOTable)
	, m_nNewColumnNumber(a_nColumnNumber)
	, m_nNewDisplayOrder(a_nNewDisplayOrder)
	, m_pPurpose(nullptr)
	, m_emFreightOD(a_emFreightOD)
{
	m_pNameChecker = new KExistNameChecker;
}

KFTableModeAddFieldDlg::~KFTableModeAddFieldDlg()
{
	if(NULL != m_pNameChecker)
	{
		delete m_pNameChecker;
	}
}

void KFTableModeAddFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strColumnName);
	DDX_Control(pDX, IDC_COMBO1, m_comboDataType);
	DDX_Text(pDX, IDC_EDIT2, m_strColumnInfo);
	DDX_Control(pDX, IDC_COMBO2, m_comboCodeGroup);
	DDX_Text(pDX, IDC_STATIC_MODENAME, m_strModeName);
	DDX_Control(pDX, IDC_EDIT1, m_editColName);
	DDX_Radio(pDX, IDC_RADIO1, m_nColumnMode);
	DDX_Control(pDX, IDC_COMBO3, m_cboPurposeInfo);
}


BEGIN_MESSAGE_MAP(KFTableModeAddFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK,          &KFTableModeAddFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1,    &KFTableModeAddFieldDlg::OnBnClickedColumnData)
	ON_BN_CLICKED(IDC_RADIO2,    &KFTableModeAddFieldDlg::OnBnClickedColumnCode)
	ON_CBN_SELCHANGE(IDC_COMBO3, &KFTableModeAddFieldDlg::OnCbnSelChangeCombo1)
	ON_BN_CLICKED(IDCANCEL,      &KFTableModeAddFieldDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_DEF, &KFTableModeAddFieldDlg::OnBnClickedCheckDef)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// KFTableModeAddFieldDlg 메시지 처리기입니다.
BOOL KFTableModeAddFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KFTableModeAddFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	std::vector< CString > dataTypeArray;

	dataTypeArray.push_back(DATATYPESTRING_DOUBLE);
	dataTypeArray.push_back(DATATYPESTRING_INTEGER);
	dataTypeArray.push_back(DATATYPESTRING_STRING);

	std::vector< CString >::iterator itDataType = dataTypeArray.begin();
	int nIndex = 0;
	while( dataTypeArray.end() != itDataType )
	{
		m_comboDataType.InsertString(nIndex++, *itDataType );
		++itDataType;
	}
	m_comboDataType.SetCurSel( 0 );

	// 초기 데이터 로딩
	InitGroupCode(m_mapGroupCode);

	nIndex = 0;

	std::map<CString, int>::iterator itGroupCode = m_mapGroupCode.begin();
	while( m_mapGroupCode.end() != itGroupCode )
	{

		m_comboCodeGroup.InsertString(nIndex++, itGroupCode->first);
		++itGroupCode;
	}
	m_comboCodeGroup.SetCurSel( 0 );

	CheckDlgButton(IDC_CHECK_DEF, BST_CHECKED);
	InitialPurposeInfo();

	SettingControlByMode();

	for(size_t i= 0; i< m_vecDisplayName.size(); i++ )
	{
		/*CString &strName = m_vecDisplayName[i];*/
		m_pNameChecker->addName( m_vecDisplayName[i] );
	}

	InitArrays();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KFTableModeAddFieldDlg::InitArrays( void )
{
	m_strArray.RemoveAll();
	
	switch (m_emFreightOD)
	{
	case KEMFreightODModeOD:
		m_strArray.Add(_T("ShortRoad"));
		m_strArray.Add(_T("LongRoad"));
		m_strArray.Add(_T("Rail"));
		break;
	case KEMFreightODChainOD:
		m_strArray.Add(_T("Road"));
		m_strArray.Add(_T("Rail"));
		break;
	default:
		TxLogDebugException();
		break;
	}
}


void KFTableModeAddFieldDlg::InitGroupCode(std::map<CString, int> &mapGroupCode)
{
	CString strSQL(_T(""));
	strSQL.Append(_T(" SELECT KCode_Group_Key,	KCode_Group_Name "));
	strSQL.Append(_T(" FROM KCode_Group "));
	strSQL.Append(_T(" ORDER BY KCode_Group_Name "));

	try
	{
		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strSQL);

		while(spResultSet->Next())
		{
			int     nGroupKey = spResultSet->GetValueInt(0);
			CString strName   = spResultSet->GetValueString(1);

			mapGroupCode[strName] = nGroupKey;
		}
	}
	catch (KExceptionPtr ex)
	{
		TxExceptionPrint(ex);
	}
	catch (...)
	{
		TxLogDebugException();
	}    
}


void KFTableModeAddFieldDlg::InitialPurposeInfo(void)
{
	try
	{
		std::vector<KPurpose*> vecPurpose = m_pProject->GetPurpose();

		int nSize = vecPurpose.size();
		int nPos;

		for (int i = 0; i < nSize; ++i)
		{
			KPurpose* pPurpose = vecPurpose[i];

			nPos = m_cboPurposeInfo.InsertString(-1, pPurpose->PurposeName());
			m_cboPurposeInfo.SetItemData(nPos, (DWORD_PTR)pPurpose);
		}

		if (nSize > 0)
		{
			m_cboPurposeInfo.SetCurSel(0);
			OnCbnSelChangeCombo1();
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KFTableModeAddFieldDlg::SettingControlByMode()
{
	UpdateStaticName();
	UpdateNextRunSeq();

	UpdateData(TRUE);
	
	if(m_nColumnMode == COLUMN_MODE_CODE)
	{
		m_comboCodeGroup.EnableWindow(TRUE);
		m_comboDataType.EnableWindow(FALSE);
		
		m_comboCodeGroup.ShowWindow(SW_SHOW);
		m_comboDataType.ShowWindow(SW_HIDE);

		m_strModeName = _T("Code:");

		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(FALSE);
	}
	else 
	{
		m_comboCodeGroup.EnableWindow(FALSE);
		m_comboDataType.EnableWindow(TRUE);

		m_comboCodeGroup.ShowWindow(SW_HIDE);
		m_comboDataType.ShowWindow(SW_SHOW);

		m_strModeName = _T("Type:");

		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(TRUE);
		ActiveDefineControl();	
	}
}

void KFTableModeAddFieldDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	
	if( GetDlgItem(IDC_EDIT1)->IsWindowEnabled() && m_pNameChecker->isNameExists(m_strColumnName) == true ) 
	{
		CString strMsg;
		strMsg.Format(_T("Column name '%s' already exists"), m_strColumnName);
		AfxMessageBox(strMsg);
		GetDlgItem(IDC_EDIT1 )->SetFocus();
		return;
	}

	if(m_nColumnMode == COLUMN_MODE_CODE)
	{
		m_emDataType = KEMIODataTypeInteger;

		if( m_comboCodeGroup.GetCount() > 0 )
		{
			CString strCodeGroupName = _T("");
			int nIdx = m_comboCodeGroup.GetCurSel();
			m_comboCodeGroup.GetLBText(nIdx, strCodeGroupName);

			m_nCodeGroup = m_mapGroupCode.find( strCodeGroupName )->second;
		}
		else 
		{
			m_nCodeGroup = 0;
		}
		m_bCode = true;
		m_pPurpose = nullptr;
	}
	else 
	{
		int nDataType = m_comboDataType.GetCurSel();

		CString strDataType = _T("");
		m_comboDataType.GetLBText(nDataType, strDataType);

		if( strDataType.CompareNoCase(DATATYPESTRING_INTEGER) == 0 )
		{
			m_emDataType = KEMIODataTypeInteger;
		}
		else if( strDataType.CompareNoCase(DATATYPESTRING_DOUBLE) == 0 )
		{
			m_emDataType = KEMIODataTypeDouble;
		}
		else if( strDataType.CompareNoCase(DATATYPESTRING_STRING) == 0 )
		{
			m_emDataType = KEMIODataTypeString;
		}
		else 
		{
			AfxMessageBox( _T("타입을 다시 선택해 주세요.") );
			m_comboDataType.SetFocus();
			return;
		}

		if (IsDlgButtonChecked(IDC_CHECK_DEF) == BST_CHECKED)
		{
			int nCur = m_cboPurposeInfo.GetCurSel();
			if (nCur < 0)
			{
				AfxMessageBox( _T("통행수단을 선택해 주세요.") );
				m_cboPurposeInfo.SetFocus();
				m_pPurpose = nullptr;
				return;
			}
			m_pPurpose = (KPurpose*)m_cboPurposeInfo.GetItemData(nCur);
		}
		else
		{
			m_pPurpose = nullptr;
		}

		m_bCode = false;
		m_nCodeGroup = -1;
	}

	if (GetDlgItem(IDC_EDIT1)->IsWindowEnabled())
	{
		m_strColumnName.Trim();							
		UpdateData(FALSE);
		if( m_strColumnName.IsEmpty() == true )			// 컬럼 네임이 비었는지 체크
		{
			AfxMessageBox(_T("컬럼명을 입력해 주세요."));

			m_editColName.SetFocus();
			return;
		}

		if( m_strColumnName.Find(_T(' ')) > -1 )		// 컬럼 네임에 공백이 있는지 체크  
		{
			AfxMessageBox(_T("컬럼명에 공백이 존재합니다. 다시 입력해 주세요."));

			m_editColName.SetFocus();
			return;
		}

		m_strColumnInfo.Trim();
		if( m_strColumnInfo.IsEmpty() == true )			// 컬럼 정보가 비었는지 체크 
		{

			m_strColumnInfo = m_strColumnName;			// 비었다면, 컬럼 이름을 넣어 주자! ( 디폴트 값만 넣어 줄 뿐 리턴 하지 않는다. )
		}

		UpdateData(FALSE);
	}	

	bool bResult;
	
	if ((IsDlgButtonChecked(IDC_CHECK_DEF) == BST_CHECKED) && (m_nColumnMode != COLUMN_MODE_CODE))
	{
		bResult = CreateAddColumns();
	}
	else
	{
		bResult = CreateAddColumn();
	}
	
	if (!bResult)
	{
		AfxMessageBox(_T("컬럼 생성 오류"));
		return;
	}

	KDialogEx::OnOK();
}


bool KFTableModeAddFieldDlg::CreateAddColumn()
{
	try
	{
		CString strTableName = m_pIOTable->Name();
		CString strName(_T(""));
		strName.Format(_T("%c%d"), strTableName.GetAt(0), m_nNewColumnNumber);

		m_AddColumn.Name      (strName);
		m_AddColumn.Caption   (m_strColumnName);
		m_AddColumn.ColumnInfo(m_strColumnInfo);
		m_AddColumn.DataType  (m_emDataType);

		if (m_bCode)
		{
			m_AddColumn.CodeType(KEMIOCodeTypeIsSingleCode);
			m_AddColumn.CodeGroup(m_nCodeGroup);
		}
		else
		{
			m_AddColumn.CodeType(KEMIOCodeTypeIsNothing);
		}

		m_AddColumn.Visible(true);
		m_AddColumn.FreezeColumn(false);
		m_AddColumn.DisplayOrder(m_nNewDisplayOrder);
		m_AddColumn.ColumnType(KEMIOColumnTypeUserDefine);
		m_AddColumn.PrimaryKey(false);
	}
	catch (...)
	{
		TxLogDebugException();
		return false;
	}

	return true;
}

bool KFTableModeAddFieldDlg::CreateAddColumns()
{
	bool bResult = false;
	
	try
	{
		//Get Commodity String
		KPurpose* pPurpose = (KPurpose*)m_cboPurposeInfo.GetItemData(m_cboPurposeInfo.GetCurSel());
		m_strColumnName = pPurpose->PurposeName();

		m_oColumns.Clear();
						
		CString    strTableName = m_pIOTable->Name();
		CString    strName;
		KIOColumn* pColumn;

		int nRunSeq = GetNextRunSeq();
		int nSize = m_strArray.GetSize();
		int nNewColumnNumber = m_nNewColumnNumber;
		int nNewDisplayOrder = m_nNewDisplayOrder;
		
		for (int i = 0; i < nSize; ++i)
		{
			pColumn = m_oColumns.AddNewColumn();

			strName.Format(_T("%c%d"), strTableName.GetAt(0), nNewColumnNumber++);
			pColumn->Name(strName);
			
			strName.Format(_T("%s-%s(%d)"), m_strColumnName, m_strArray[i], nRunSeq);
			pColumn->Caption(strName);
			
			pColumn->ColumnInfo(m_strColumnInfo);
			pColumn->DataType(m_emDataType);
			pColumn->CodeType(KEMIOCodeTypeIsNothing);
			pColumn->Visible(true);
			pColumn->FreezeColumn(false);
			pColumn->DisplayOrder(nNewDisplayOrder++);
			pColumn->ColumnType(KEMIOColumnTypeUserDefine);
			pColumn->PrimaryKey(false);
		}

		bResult = true;
	}
	catch (...)
	{
		TxLogDebugException();
	}
	
	return bResult;
}

void KFTableModeAddFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KFTableModeAddFieldDlg::OnBnClickedColumnData()
{

	SettingControlByMode();

	UpdateData(FALSE);
}

void KFTableModeAddFieldDlg::OnBnClickedColumnCode()
{

	SettingControlByMode();

	UpdateData(FALSE);
}


void KFTableModeAddFieldDlg::OnBnClickedCheckDef()
{
	ActiveDefineControl();
}


void KFTableModeAddFieldDlg::ActiveDefineControl()
{
	UINT nID = IsDlgButtonChecked(IDC_CHECK_DEF);

	UpdateStaticName();
	UpdateNextRunSeq();
	
	if (BST_CHECKED == nID)
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		m_comboDataType.SetCurSel(0); //double
	}
	else
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);		
	}
}

void KFTableModeAddFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}

int KFTableModeAddFieldDlg::GetNextRunSeq( void )
{
	int nResult;

	try
	{
		KPurpose* pPurpose = (KPurpose*)m_cboPurposeInfo.GetItemData(m_cboPurposeInfo.GetCurSel());

		CString strTableName = m_pIOTable->Name();
		CString strQuery = _T("SELECT IFNULL(MAX(run_seq) + 1, 1) FROM ");

		if (strTableName.CompareNoCase(TABLE_MODE_OD) == 0)
		{
			strQuery.Append(TABLE_ITEMLOGIT_MODEL);
		}
		else if (strTableName.CompareNoCase(TABLE_CHAIN_OD) == 0)
		{
			strQuery.Append(TABLE_CHAIN_MODEL);
		}

		strQuery.AppendFormat(_T(" WHERE mode_group_code = %d AND detail_mode_id = %d"), pPurpose->PurposeGroup(), pPurpose->PurposeID());

		KDBaseConPtr spDBaseConnection = m_pTarget->GetDBaseConnection();
		KResultSetPtr       spResultSet       = spDBaseConnection->ExecuteQuery(strQuery);

		if (!spResultSet->Next())
			ThrowException(_T("DB Error..."));

		nResult = spResultSet->GetValueInt(0);

		return nResult;
	}
	catch (...)
	{
		TxLogDebugException();
		throw 1;
	}	
}

void KFTableModeAddFieldDlg::UpdateStaticName( void )
{
	//Name Control Rename
	if (IsDlgButtonChecked(IDC_RADIO1) && IsDlgButtonChecked(IDC_CHECK_DEF))
	{
		SetDlgItemText(IDC_STATIC_NAME, _T("Next RunSeq:"));
	}
	else
	{
		SetDlgItemText(IDC_STATIC_NAME, _T("Name:"));
	}
}

void KFTableModeAddFieldDlg::UpdateNextRunSeq( void )
{
	CString strValue;
	int     nRunSeq = GetNextRunSeq();

	if (IsDlgButtonChecked(IDC_RADIO1) && IsDlgButtonChecked(IDC_CHECK_DEF))
	{
		strValue.Format(_T("%d"), nRunSeq);
		SetDlgItemText(IDC_EDIT1, strValue);
	}
	else
	{
		SetDlgItemText(IDC_EDIT1, _T(""));
	}	
}

void KFTableModeAddFieldDlg::OnCbnSelChangeCombo1( void )
{
	UpdateNextRunSeq();
}
