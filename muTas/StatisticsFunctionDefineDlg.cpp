// StatisticsFunctionDefineDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StatisticsFunctionDefineDlg.h"
#include "afxdialogex.h"
#include "ExpressionElementField.h"
#include "ExpressionElementFunction.h"
#include "ExpressionElementNumber.h"
#include "ExpressionElementOperator.h"
#include "GRegressionExpression.h"
//^^ #include "IOTable.h"
//^^ #include "IOColumns.h"
//^^ #include "IOColumn.h"

const static TCHAR* szOperators[] = {
	_T("+"), _T("-"), _T("*"), _T("/"), 
	_T("^"), _T("("), _T(")")
};

const static TCHAR* szFunctions[] = {
	_T("SQRT"), _T("EXP"), _T("LN"), _T("LOG"), 
	_T("ABS"), _T("POW")
};


// KStatisticsFunctionDefineDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KStatisticsFunctionDefineDlg, KDialogEx)

KStatisticsFunctionDefineDlg::KStatisticsFunctionDefineDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KStatisticsFunctionDefineDlg::IDD, pParent),
	m_strExpression(_T(""))
{

}

KStatisticsFunctionDefineDlg::~KStatisticsFunctionDefineDlg()
{
}

void KStatisticsFunctionDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strExpression);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
	DDX_Control(pDX, IDC_LIST4, m_lstOperator);
	DDX_Control(pDX, IDC_LIST3, m_lstFunction);
}


BEGIN_MESSAGE_MAP(KStatisticsFunctionDefineDlg, KDialogEx)
	ON_LBN_DBLCLK(IDC_LIST1,    &KStatisticsFunctionDefineDlg::OnDblclkList1)
	ON_LBN_DBLCLK(IDC_LIST4,    &KStatisticsFunctionDefineDlg::OnDblclkList2)
	ON_LBN_DBLCLK(IDC_LIST3,    &KStatisticsFunctionDefineDlg::OnDblclkList3)
	ON_BN_CLICKED(IDOK, &KStatisticsFunctionDefineDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KStatisticsFunctionDefineDlg 메시지 처리기입니다.

BOOL KStatisticsFunctionDefineDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initFieldList();
	initOperatorList();
	initFunctionList();

	m_strExpression = m_pExpression->GetCaptionExpression();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KStatisticsFunctionDefineDlg::initFieldList(void)
{
	ASSERT(NULL != m_pTable);

	const KIOColumns* pColumns = m_pTable->Columns();
	ASSERT(NULL != pColumns);

	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn* pColumn = pColumns->GetColumn(i);
		KEMIOColumnType emColumnType = pColumn->ColumnType();

		if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode || pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			continue;
		}

		if(pColumn->DataType() != KEMIODataTypeDouble)
		{
			continue;
		}

		/*if((KEMIOColumnTypeFixed == emColumnType) || (KEMIOColumnTypeUserDefine == emColumnType))*/
		int nIndex = m_lstField.InsertString(-1, pColumn->Caption());
		m_lstField.SetItemData(nIndex, (DWORD_PTR)pColumn);
	}
}


void KStatisticsFunctionDefineDlg::initOperatorList(void)
{
	int nOperator = sizeof(szOperators) / sizeof(TCHAR*);

	for(int i = 0; i < nOperator; ++i)
	{
		m_lstOperator.InsertString(-1, szOperators[i]);
	}
}


void KStatisticsFunctionDefineDlg::initFunctionList(void)
{
	int nFunction = sizeof(szFunctions) / sizeof(TCHAR*);

	for(int i = 0; i < nFunction; ++i)
	{
		m_lstFunction.InsertString(-1, szFunctions[i]);
	}
}


void KStatisticsFunctionDefineDlg::OnDblclkList1()
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


void KStatisticsFunctionDefineDlg::OnDblclkList2()
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


void KStatisticsFunctionDefineDlg::OnDblclkList3()
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


std::tstring KStatisticsFunctionDefineDlg::getFieldName(LPCTSTR a_strElement)
{
	std::tstring str = a_strElement;
	std::tstring strFieldName = _T("");
	if((str.find(_T("[")) != std::tstring::npos) || (str.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type n = str.find_first_not_of(_T("["));
		str = (n == std::tstring::npos) ? str : str.substr(n, str.length());
		n = str.find_last_not_of(_T("]"));
		str = (n == std::tstring::npos) ? str : str.substr(0, n + 1);
	}

	int nListCount = m_lstField.GetCount();
	for(int i = 0; i < nListCount; ++i)
	{
		CString strItem;
		m_lstField.GetText(i, strItem);
		if(strItem.CompareNoCase(str.c_str()) == 0)
		{
			KIOColumn* pColumn = (KIOColumn*)m_lstField.GetItemData(i);
			strFieldName = pColumn->Name();
			break;
		}
	}

	return strFieldName;
}


std::tstring KStatisticsFunctionDefineDlg::getFieldCaption(LPCTSTR a_strElement)
{
	std::tstring strFieldCaption = a_strElement;
	if((strFieldCaption.find(_T("[")) != std::tstring::npos) || 
		(strFieldCaption.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type n = strFieldCaption.find_first_not_of(_T("["));
		strFieldCaption = (n == std::tstring::npos) ? strFieldCaption : 
			strFieldCaption.substr(n, strFieldCaption.length());
		n = strFieldCaption.find_last_not_of(_T("]"));
		strFieldCaption = (n == std::tstring::npos) ? strFieldCaption : 
			strFieldCaption.substr(0, n + 1);
	}
	return strFieldCaption;
}


bool KStatisticsFunctionDefineDlg::isField(LPCTSTR a_strElement)
{
	std::tstring str = a_strElement;
	if((str.find(_T("[")) != std::tstring::npos) || (str.find(_T("]")) != std::tstring::npos))
	{
		std::tstring::size_type n = str.find_first_not_of(_T("["));
		str = (n == std::tstring::npos) ? str : str.substr(n, str.length());
		n = str.find_last_not_of(_T("]"));
		str = (n == std::tstring::npos) ? str : str.substr(0, n + 1);
	}

	int nListCount = m_lstField.GetCount();
	for(int i = 0; i < nListCount; ++i)
	{
		CString strItem;
		m_lstField.GetText(i, strItem);
		if(strItem.CompareNoCase(str.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}


bool KStatisticsFunctionDefineDlg::isOperator(LPCTSTR a_strElement)
{
	std::tstring str = a_strElement;
	static std::tstring strOperator = _T("+-*/^()");
	if(str.length() > 1)
	{
		return false;
	}

	int nOperator = sizeof(szOperators) / sizeof(TCHAR*);
	for(int i = 0; i < nOperator; ++i)
	{
		if(str.compare(szOperators[i]) == 0)
		{
			return true;
		}
	}

	return false;
}


bool KStatisticsFunctionDefineDlg::isFunction(LPCTSTR a_strElement)
{
	std::tstring str = a_strElement;
	int nFunction = sizeof(szFunctions) / sizeof(TCHAR*);
	for(int i = 0; i < nFunction; ++i)
	{
		if(_tcsicmp(str.c_str(), szFunctions[i]) == 0)
		{
			return true;
		}
	}

	return false;
}


void KStatisticsFunctionDefineDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	/*if( FALSE == InvalidateInputInfo())
	{
		return;
	}*/

	m_pExpression->ClearElements();

    m_pExpression->SetCaptionExpression(m_strExpression);

	std::tstring str = (LPCTSTR)m_strExpression;

	typedef boost::tokenizer<
		boost::char_separator<TCHAR>, 
		std::tstring::const_iterator, 
		std::tstring> ttokenizer;

	boost::char_separator<TCHAR> sep(_T(" "));
	ttokenizer tokens(str, sep);
	for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		std::tstring strToken = *it;
		if(isField(strToken.c_str()) == true)
		{
			std::tstring strName = getFieldName(strToken.c_str());
			std::tstring strCaption = getFieldCaption(strToken.c_str());
			m_pExpression->AddField(strName.c_str(), strCaption.c_str());
		}
		else if(isOperator(strToken.c_str()) == true)
		{
			m_pExpression->AddOperator(strToken.at(0));
		}
		else if(isFunction(strToken.c_str()) == true)
		{
			m_pExpression->AddFunction(strToken.c_str());
		}
		else
		{
			m_pExpression->AddNumber(_tstof(strToken.c_str()));
		}
	}
	
	KDialogEx::OnOK();
}


BOOL KStatisticsFunctionDefineDlg::InvalidateInputInfo()
{
	UpdateData(TRUE);

	if (m_strExpression == _T(""))
	{

		AfxMessageBox(_T("입력한 수식이 없습니다.")); 
		return FALSE;
	}

	return TRUE;
}
