/**
 * @file TableAddFieldDlg.cpp
 * @brief KTableAddFieldDlg 구현
 * @author 
 * @date 2010.08.23
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "TableAddFieldDlg.h"
#include "ExistNameChecker.h"
//^^ #include "IOCommons.h"
#include "Target.h"

/******************************************************************************
* KTableAddFieldDlg                                                          *
******************************************************************************/
IMPLEMENT_DYNAMIC(KTableAddFieldDlg, KDialogEx)


/**************************************
* Constructors / Destructor           *
**************************************/
KTableAddFieldDlg::KTableAddFieldDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: KDialogEx(KTableAddFieldDlg::IDD, pParent)
    , m_pTarget(a_pTarget)
	, m_bCode( false )
	, m_strColumnName(_T(""))
	, m_strColumnInfo(_T(""))
	, m_nCodeGroup(-1)
	, m_strModeName(_T(""))
	, m_nColumnMode(0)
{
	m_pNameChecker = new KExistNameChecker;
}

KTableAddFieldDlg::~KTableAddFieldDlg()
{
	if(NULL != m_pNameChecker)
	{
		delete m_pNameChecker;
	}
}

BOOL KTableAddFieldDlg::PreTranslateMessage( MSG* a_pMsg )
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

BOOL KTableAddFieldDlg::OnInitDialog()
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

	SettingControlByMode();

	for(size_t i= 0; i< m_vecDisplayName.size(); i++ )
	{
		/*CString &strName = m_vecDisplayName[i];*/
		m_pNameChecker->addName( m_vecDisplayName[i] );
	}

	AddImChampTooltip(IDC_EDIT1, _T("이름에는 다음 문자를 사용할 수 없습니다.\r\n \\/:*?\"<>|[]\'"));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTableAddFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strColumnName);
	DDX_Control(pDX, IDC_COMBO1, m_comboDataType);
	DDX_Text(pDX, IDC_EDIT2, m_strColumnInfo);
	DDX_Control(pDX, IDC_COMBO2, m_comboCodeGroup);
	DDX_Text(pDX, IDC_STATIC_MODENAME, m_strModeName);
	DDX_Control(pDX, IDC_EDIT1, m_editColName);
	DDX_Radio(pDX, IDC_RADIO1, m_nColumnMode);
}

/**************************************
* Message handlers                    *
**************************************/

BEGIN_MESSAGE_MAP(KTableAddFieldDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KTableAddFieldDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &KTableAddFieldDlg::OnBnClickedColumnData)
	ON_BN_CLICKED(IDC_RADIO2, &KTableAddFieldDlg::OnBnClickedColumnCode)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void KTableAddFieldDlg::InitGroupCode(std::map<CString, int> &mapGroupCode)
{ // 시스템 코드를 제외한 유저코드만을 받아온다. (명지대 요청 16.08.16)
    CString strSQL(_T(""));
    strSQL.Append(_T(" SELECT KCode_Group_Key, KCode_Group_Name "));
    strSQL.Append(_T(" FROM KCode_Group WHERE System_Code_YN = 'N' "));
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


void KTableAddFieldDlg::SettingControlByMode()
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
	}

}

void KTableAddFieldDlg::OnBnClickedOk()
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
		m_bCode = true;
	}
	else 
	{
		int nDataType = m_comboDataType.GetCurSel();

		// 		switch( nDataType )
		// 		{
		// 		case 0 : m_emDataType = KEMIODataTypeInteger; break;
		// 		case 1 : m_emDataType = KEMIODataTypeDouble; break;
		// 		case 2 : m_emDataType = KEMIODataTypeString; break;
		// 		}

		CString strDataType = _T("");
		m_comboDataType.GetLBText(nDataType, strDataType);

		if( strDataType == DATATYPESTRING_INTEGER )
		{
			m_emDataType = KEMIODataTypeInteger;
		}
		else if( strDataType == DATATYPESTRING_DOUBLE )
		{
			m_emDataType = KEMIODataTypeDouble;
		}
		else if( strDataType == DATATYPESTRING_STRING )
		{
			m_emDataType = KEMIODataTypeString;
		}
		else 
		{
			AfxMessageBox( _T("타입을 다시 선택해 주세요.") );
			return;
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
		AfxMessageBox(_T("컬럼명에 공백이 존재합니다.\n다시 입력해 주세요."));

		m_editColName.SetFocus();
		return;
	}

	CString strErrCh(_T(""));
	if (QBicStringChecker::IsValidColumnName(m_strColumnName, strErrCh) == false)
	{
		CString strErrMsg(_T(""));
		strErrMsg.Format(_T("명칭에 \'%s\' 문자를 사용할 수 없습니다."), strErrCh);
		AfxMessageBox(strErrMsg);

		m_editColName.SetFocus();
		return;
	}

	m_strColumnInfo.Trim();
	if( m_strColumnInfo.IsEmpty() == true )			// 컬럼 정보가 비었는지 체크 
	{

		m_strColumnInfo = m_strColumnName;			// 비었다면, 컬럼 이름을 넣어 주자! ( 디폴트 값만 넣어 줄 뿐 리턴 하지 않는다. )
	}

	UpdateData(FALSE);
	OnOK();
}

void KTableAddFieldDlg::OnBnClickedColumnData()
{

	SettingControlByMode();

	UpdateData(FALSE);
}

void KTableAddFieldDlg::OnBnClickedColumnCode()
{

	SettingControlByMode();

	UpdateData(FALSE);
}


void KTableAddFieldDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	KDialogEx::OnShowWindow(bShow, nStatus);
	GetDlgItem(IDC_EDIT1)->SetFocus();
}
