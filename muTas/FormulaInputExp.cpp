// FormulaInputExp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormulaInputExp.h"
#include "afxdialogex.h"


// KFormulaInputExp 대화 상자입니다.

IMPLEMENT_DYNAMIC(KFormulaInputExp, KDialogEx)

KFormulaInputExp::KFormulaInputExp(CWnd* pParent /*=NULL*/)
	: KDialogEx(KFormulaInputExp::IDD, pParent), m_strExpression(_T(""))
{
}

KFormulaInputExp::~KFormulaInputExp()
{
}

void KFormulaInputExp::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX,    IDC_EDIT1,  m_strExpression);
    DDX_Control(pDX, IDC_LIST1,  m_lstField);
    DDX_Control(pDX, IDC_LIST5,  m_lstOperator);
    DDX_Control(pDX, IDC_LIST3,  m_lstFunction);
}

BEGIN_MESSAGE_MAP(KFormulaInputExp, KDialogEx)
    ON_LBN_DBLCLK(IDC_LIST1, &KFormulaInputExp::OnDblclkListField)
    ON_LBN_DBLCLK(IDC_LIST5, &KFormulaInputExp::OnDblclkListOperator)
    ON_LBN_DBLCLK(IDC_LIST3, &KFormulaInputExp::OnDblclkListFunction)
END_MESSAGE_MAP()

BOOL KFormulaInputExp::OnInitDialog()
{
    KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

    SetBackgroundColor(RGB(255,255,255));

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
    
    return TRUE; 
}


void KFormulaInputExp::OnOK()
{   
    UpdateData(TRUE);
    KDialogEx::OnOK();
}


void KFormulaInputExp::OnDblclkListField(void)
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


void KFormulaInputExp::OnDblclkListOperator(void)
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


void KFormulaInputExp::OnDblclkListFunction(void)
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

void KFormulaInputExp::UsableFunction( std::vector<CString> a_vecFunction )
{
    m_vecFunction = a_vecFunction;
}

void KFormulaInputExp::UsableOperator( std::vector<CString> a_vecOperator )
{
    m_vecOperator = a_vecOperator;
}

void KFormulaInputExp::UsableField( std::vector<TFormulaField> a_vecFormulaField )
{
    m_vecFormulaField = a_vecFormulaField;
}

CString KFormulaInputExp::GetFormulaCaption()
{
    return m_strExpression;
}
