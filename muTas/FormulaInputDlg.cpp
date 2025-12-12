// FormulaInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormulaInputDlg.h"
#include "afxdialogex.h"


// KFormulaInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFormulaInputDlg, KDialogEx)

KFormulaInputDlg::KFormulaInputDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KFormulaInputDlg::IDD, pParent), m_strExpression(_T(""))
{

}

KFormulaInputDlg::~KFormulaInputDlg()
{
}

void KFormulaInputDlg::DoDataExchange(CDataExchange* pDX)
{
    KDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX,    IDC_EDIT1,  m_strExpression);
    DDX_Control(pDX, IDC_LIST1,  m_lstField);
    DDX_Control(pDX, IDC_LIST2,  m_lstOperator);
    DDX_Control(pDX, IDC_LIST3,  m_lstFunction);
	DDX_Control(pDX, IDC_COMBO1, m_cboUserColumn);
}

BEGIN_MESSAGE_MAP(KFormulaInputDlg, KDialogEx)
    ON_LBN_DBLCLK(IDC_LIST1, &KFormulaInputDlg::OnDblclkListField)
    ON_LBN_DBLCLK(IDC_LIST2, &KFormulaInputDlg::OnDblclkListOperator)
    ON_LBN_DBLCLK(IDC_LIST3, &KFormulaInputDlg::OnDblclkListFunction)
END_MESSAGE_MAP()

// KFormulaInputDlg 메시지 처리기입니다.
BOOL KFormulaInputDlg::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetBackgroundColor(RGB(255,255,255));
    // TODO:  여기에 추가 초기화 작업을 추가합니다.
    //updateFieldListBox();
    size_t nxCount = m_vecFormulaField.size();
    for (size_t i=0; i<nxCount; i++)
    {
        TFormulaField& oFormulaField = m_vecFormulaField[i];
        m_lstField.AddString(oFormulaField.TFieldCaption);
    }

    nxCount = m_vecOperator.size();
    for (size_t i=0; i<nxCount; i++)
    {
        m_lstOperator.AddString(m_vecOperator[i]);
    }
    
    nxCount = m_vecFunction.size();
    for (size_t i=0; i<nxCount; i++)
    {
        m_lstFunction.AddString(m_vecFunction[i]);
    }

	InitComboCtrl();

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KFormulaInputDlg::InitComboCtrl( void )
{
	m_cboUserColumn.ResetContent();

	int nIdxCursor;

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("---"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("U1"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("U2"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("U3"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("U4"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	nIdxCursor = m_cboUserColumn.InsertString(-1, _T("U5"));
	m_cboUserColumn.SetItemData(nIdxCursor, 0);

	m_cboUserColumn.SetCurSel(0);
}

void KFormulaInputDlg::OnOK()
{
	int nIdxCursor = m_cboUserColumn.GetCurSel();

	if (nIdxCursor == 0)
	{
		AfxMessageBox(_T("컬럼을 선택해 주세요."));
		return;
	}
    
	m_nIdxUserColumn = nIdxCursor - 1;//---selecte -- Index 제외

    UpdateData(TRUE);
    KDialogEx::OnOK();
}


void KFormulaInputDlg::OnDblclkListField(void)
{
    UpdateData(TRUE);
    int nSel = m_lstField.GetCurSel();
    if(LB_ERR != nSel)
    {
        CString strField;
        m_lstField.GetText(nSel, strField);
        if(m_strExpression.IsEmpty() == false)
        {
            m_strExpression += _T(" ");
        }
        m_strExpression += _T("[");
        m_strExpression += strField;
        m_strExpression += _T("]");
        UpdateData(FALSE);
    }
}


void KFormulaInputDlg::OnDblclkListOperator(void)
{
    UpdateData(TRUE);
    int nSel = m_lstOperator.GetCurSel();
    if(LB_ERR != nSel)
    {
        CString strOperator;
        m_lstOperator.GetText(nSel, strOperator);
        if(m_strExpression.IsEmpty() == false)
        {
            m_strExpression += _T(" ");
        }
        m_strExpression += strOperator;
        UpdateData(FALSE);
    }
}


void KFormulaInputDlg::OnDblclkListFunction(void)
{
    UpdateData(TRUE);
    int nSel = m_lstFunction.GetCurSel();
    if(LB_ERR != nSel)
    {
        CString strFunction;
        m_lstFunction.GetText(nSel, strFunction);
        if(m_strExpression.IsEmpty() == false)
        {
            m_strExpression += _T(" ");
        }
        m_strExpression += strFunction;
        UpdateData(FALSE);
    }
}

void KFormulaInputDlg::UsableFunction( std::vector<CString> a_vecFunction )
{
    m_vecFunction = a_vecFunction;
}

void KFormulaInputDlg::UsableOperator( std::vector<CString> a_vecOperator )
{
    m_vecOperator = a_vecOperator;
}

void KFormulaInputDlg::UsableField( std::vector<TFormulaField> a_vecFormulaField )
{
    m_vecFormulaField = a_vecFormulaField;
}

CString KFormulaInputDlg::GetFormulaCaption()
{
    return m_strExpression;
}

