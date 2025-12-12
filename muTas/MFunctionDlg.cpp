// MFunctionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KmzApp.h"
#include "MFunctionDlg.h"
#include "afxdialogex.h"


// KMFunctionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KMFunctionDlg, KResizeDialogEx)

KMFunctionDlg::KMFunctionDlg(CWnd* pParent /*=NULL*/)
	: KResizeDialogEx(KMFunctionDlg::IDD, pParent)
	, m_strExpression(_T(""))
{
    m_strDefault   = _T("");
    m_nFlagFreight = 0;
}

KMFunctionDlg::~KMFunctionDlg()
{
}

void KMFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	KResizeDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,    IDC_EDIT1, m_strExpression);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
	DDX_Control(pDX, IDC_LIST5, m_lstOperator);
	DDX_Control(pDX, IDC_LIST3, m_lstFunction);
}


BEGIN_MESSAGE_MAP(KMFunctionDlg, KResizeDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1, &KMFunctionDlg::OnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST5, &KMFunctionDlg::OnDblclkList2)
	ON_LBN_DBLCLK(IDC_LIST3, &KMFunctionDlg::OnDblclkList3)
    ON_BN_CLICKED(IDC_BUTTON1, &KMFunctionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()



LPCTSTR KMFunctionDlg::Expression(void)
{
	return m_strExpression;
}


void KMFunctionDlg::Expression(LPCTSTR strExpression)
{
	m_strExpression = strExpression;
}


void KMFunctionDlg::FieldList(const std::list<std::tstring>& fieldList)
{
	m_FieldList = fieldList;
}


void KMFunctionDlg::FunctionList(const std::list<std::tstring>& functionList)
{
	m_FunctionList = functionList;
}


void KMFunctionDlg::OperatorList(const std::list<std::tstring>& operatorList)
{
	m_OperatorList = operatorList;
}


void KMFunctionDlg::updateFieldListBox(void)
{
	std::list<std::tstring>::iterator itField, itEnd = m_FieldList.end();
	for(itField = m_FieldList.begin(); itField != itEnd; ++itField)
	{
		std::tstring strField = *itField;
		m_lstField.AddString(strField.c_str());
	}
}


void KMFunctionDlg::updateFunctionListBox(void)
{
	std::list<std::tstring>::iterator itFunction, itEnd = m_FunctionList.end();
	for(itFunction = m_FunctionList.begin(); itFunction != itEnd; ++itFunction)
	{
		std::tstring strFunction = *itFunction;
		m_lstFunction.AddString(strFunction.c_str());
	}
}


void KMFunctionDlg::updateOperatorListBox(void)
{
	std::list<std::tstring>::iterator itOperator, itEnd = m_OperatorList.end();
	for(itOperator = m_OperatorList.begin(); itOperator != itEnd; ++itOperator)
	{
		std::tstring strOperator = *itOperator;
		m_lstOperator.AddString(strOperator.c_str());
	}
}


// KMFunctionDlg 메시지 처리기입니다.
BOOL KMFunctionDlg::OnInitDialog()
{
	KResizeDialogEx::OnInitDialog();
	KResizeDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	updateFieldListBox();
	updateOperatorListBox();
	updateFunctionListBox();

	UpdateData(FALSE);
	ResizeComponent();

    if (m_nFlagFreight == 1)
        GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KMFunctionDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	UpdateData(TRUE);
	KResizeDialogEx::OnOK();
}


void KMFunctionDlg::OnDblclkList1(void)
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


void KMFunctionDlg::OnDblclkList2(void)
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


void KMFunctionDlg::OnDblclkList3(void)
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

void KMFunctionDlg::ResizeComponent()
{
	SetResize(IDC_STATIC_EXP,		SZ_TOP_LEFT,     SZ_TOP_LEFT);
	SetResize(IDC_EDIT1,			SZ_TOP_LEFT,     CXTPResizePoint(1, 0.3));

	SetResize(IDC_STATIC_VARI,		CXTPResizePoint(0, 0.3),     CXTPResizePoint(0, 0.3));
	SetResize(IDC_STATIC_OPER,		CXTPResizePoint(0.5, 0.3),     CXTPResizePoint(0.5, 0.3));
	SetResize(IDC_STATIC_FUN,		CXTPResizePoint(0.7, 0.3),     CXTPResizePoint(0.7, 0.3));

	SetResize(IDC_LIST1,			CXTPResizePoint(0, 0.3),     CXTPResizePoint(0.5, 1));
	SetResize(IDC_LIST5,			CXTPResizePoint(0.5, 0.3),     CXTPResizePoint(0.7, 1));
	SetResize(IDC_LIST3,			CXTPResizePoint(0.7, 0.3),     CXTPResizePoint(1, 1));

	SetResize(IDC_STATIC_HOR2,			SZ_BOTTOM_LEFT,    SZ_BOTTOM_RIGHT);
	SetResize(IDOK,						SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,					SZ_BOTTOM_RIGHT,   SZ_BOTTOM_RIGHT);

    SetResize(IDC_BUTTON1,		CXTPResizePoint(0, 1.0),   CXTPResizePoint(0, 1.0));
}

void KMFunctionDlg::OnBnClickedButton1()
{
    if (m_strDefault.GetLength() > 0)
    {
        m_strExpression = m_strDefault;
        UpdateData(FALSE);
    }
}
