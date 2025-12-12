// TableZoneAddFieldDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "KmzSystem.h"
#include "TableZoneAddFieldDlg.h"
#include "afxdialogex.h"
#include "ExistNameChecker.h"
//^^ #include "IOCommons.h"
#include "Target.h"
#include "Project.h"
//^^ #include "IOTable.h"

// KTableZoneAddFieldDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KTableZoneAddFieldDlg, KDialogEx)

KTableZoneAddFieldDlg::KTableZoneAddFieldDlg(KTarget* a_pTarget, KProject* a_pProject, KIOTable* a_pIOTable, int a_nColumnNumber, int a_nNewDisplayOrder, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableZoneAddFieldDlg::IDD, pParent)
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
{
	m_pNameChecker = new KExistNameChecker;
}

KTableZoneAddFieldDlg::~KTableZoneAddFieldDlg()
{
	if(NULL != m_pNameChecker)
	{
		delete m_pNameChecker;
	}
}

void KTableZoneAddFieldDlg::DoDataExchange(CDataExchange* pDX)
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


BEGIN_MESSAGE_MAP(KTableZoneAddFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTableZoneAddFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &KTableZoneAddFieldDlg::OnBnClickedColumnData)
	ON_BN_CLICKED(IDC_RADIO2, &KTableZoneAddFieldDlg::OnBnClickedColumnCode)

	ON_BN_CLICKED(IDCANCEL, &KTableZoneAddFieldDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_DEF, &KTableZoneAddFieldDlg::OnBnClickedCheckDef)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO3, IDC_RADIO4, &KTableZoneAddFieldDlg::OnBnClickedRadioPA)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

BOOL KTableZoneAddFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KTableZoneAddFieldDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

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
	if (KmzSystem::IsPassengerSystem() == true)
	{
		SetDlgItemText(IDC_CHECK_DEF, _T("Purpose :"));
		if (KmzSystem::GetLanguage() == KEMKorea) {
			SetDlgItemText(IDC_CHECK_DEF, _T("통행목적 :"));
		}
	}
	else
	{
		SetDlgItemText(IDC_CHECK_DEF, _T("품목:"));
	}

	SettingControlByMode();
	CheckRadioButton(IDC_RADIO3, IDC_RADIO4, IDC_RADIO3);
	m_strPAFlag = _T("p");

	for(size_t i= 0; i< m_vecDisplayName.size(); i++ )
	{
		m_pNameChecker->addName( m_vecDisplayName[i] );
	}

	UpdateData(FALSE);
	return TRUE;  
}

void KTableZoneAddFieldDlg::InitGroupCode(std::map<CString, int> &mapGroupCode)
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

void KTableZoneAddFieldDlg::InitialPurposeInfo(void)
{
	try
	{
		std::vector<KPurpose*> vecPurpose = m_pProject->GetPurpose();

		size_t nPurposeCount = vecPurpose.size();

		for(size_t i= 0; i< nPurposeCount; ++i)
		{
			KPurpose* pPurpose = vecPurpose[i];

			int nCur = m_cboPurposeInfo.AddString(pPurpose->PurposeName());
			m_cboPurposeInfo.SetItemData( nCur, (DWORD_PTR)( pPurpose ) );
		}

		if (nPurposeCount > 0)
		{
			m_cboPurposeInfo.SetCurSel(0);
		}
	}
	catch (...)
	{
		TxLogDebugException();	
	}
}


void KTableZoneAddFieldDlg::SettingControlByMode()
{
	UpdateData(TRUE);

	if(m_nColumnMode == COLUMN_MODE_CODE)
	{
		m_comboCodeGroup.EnableWindow(TRUE);
		m_comboDataType.EnableWindow(FALSE);

		m_comboCodeGroup.ShowWindow(SW_SHOW);
		m_comboDataType.ShowWindow(SW_HIDE);

		m_strModeName = _T("Code :");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strModeName = _T("코드 그룹 :");
		}

		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(FALSE);
		//GetDlgItem(IDC_STATIC6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);
	}
	else 
	{
		m_comboCodeGroup.EnableWindow(FALSE);
		m_comboDataType.EnableWindow(TRUE);

		m_comboCodeGroup.ShowWindow(SW_HIDE);
		m_comboDataType.ShowWindow(SW_SHOW);

		m_strModeName = _T("Type :");
		if (KmzSystem::GetLanguage() == KEMKorea) {
			m_strModeName = _T("데이터 유형 :");
		}

		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DEF)->EnableWindow(TRUE);
		//GetDlgItem(IDC_STATIC6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);
		ActiveDefineControl();
	}
}


void KTableZoneAddFieldDlg::OnBnClickedOk()
{
	UpdateData( TRUE );

	if( m_pNameChecker->isNameExists(m_strColumnName) == true ) 
	{
		CString strMsg;
		strMsg.Format(_T("동일한 컬럼명 '%s'이 존재합니다."), m_strColumnName);
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
		m_bCode    = true;
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
				if (KmzSystem::IsPassengerSystem() == true)
				{
					AfxMessageBox( _T("통행목적을 선택해 주세요.") );
				}
				else
				{
					AfxMessageBox( _T("관련 항목을 선택해 주세요.") );
				}

				m_cboPurposeInfo.SetFocus();
				m_pPurpose = nullptr;
				return;
			}
			m_pPurpose= (KPurpose*)m_cboPurposeInfo.GetItemData(nCur);
		}
		else
		{
			m_pPurpose = nullptr;
		}

		m_bCode = false;
		m_nCodeGroup = -1;
	}

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

	if (!CreateAddColumn())
	{
		AfxMessageBox(_T("컬럼 생성 오류"));
		return;
	}

	KDialogEx::OnOK();
}


bool KTableZoneAddFieldDlg::CreateAddColumn()
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
		}
		else
		{
			m_AddColumn.CodeType(KEMIOCodeTypeIsNothing);
		}

		if (m_bCode)
		{
			m_AddColumn.CodeGroup(m_nCodeGroup);
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


void KTableZoneAddFieldDlg::OnBnClickedCancel()
{
	KDialogEx::OnCancel();
}


void KTableZoneAddFieldDlg::OnBnClickedColumnData()
{

	SettingControlByMode();

	UpdateData(FALSE);
}

void KTableZoneAddFieldDlg::OnBnClickedColumnCode()
{

	SettingControlByMode();

	UpdateData(FALSE);
}


void KTableZoneAddFieldDlg::OnBnClickedCheckDef()
{
	ActiveDefineControl();
}


void KTableZoneAddFieldDlg::ActiveDefineControl()
{
	UINT nID = IsDlgButtonChecked(IDC_CHECK_DEF);
	if (BST_CHECKED == nID)
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		m_comboDataType.SetCurSel(0); //double

		//GetDlgItem(IDC_STATIC6)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);

		//GetDlgItem(IDC_STATIC6)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);
	}
}


void KTableZoneAddFieldDlg::OnBnClickedRadioPA(UINT nID)
{
	if (GetCheckedRadioButton(IDC_RADIO3, IDC_RADIO4) == IDC_RADIO3)
	{
		m_strPAFlag = _T("p");
	}
	else
	{
		m_strPAFlag = _T("a");
	}
}


void KTableZoneAddFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
