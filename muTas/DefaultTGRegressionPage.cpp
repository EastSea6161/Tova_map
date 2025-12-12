// DefaultTGRegressionPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultTGRegressionPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationInfo.h"
#include "GRegressionExpression.h"
#include "GSimpleExpressionContainer.h"


const static TCHAR* szOperators[] = {
	_T("+"), _T("-"), _T("*"), _T("/"), 
	_T("^"), _T("("), _T(")")
};

const static TCHAR* szFunctions[] = {
	_T("SQRT"), _T("EXP"), _T("LN"), _T("LOG"), 
	_T("ABS"), _T("POW")
};


// KDefaultTGRegressionPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultTGRegressionPage, CXTPPropertyPage)

KDefaultTGRegressionPage::KDefaultTGRegressionPage(KTarget* a_pTarget, KGenerationInfo* a_pGenertaionInfo)
	: CXTPPropertyPage(KDefaultTGRegressionPage::IDD)
	, m_pTarget(a_pTarget)
	, m_pGenerationInfo(a_pGenertaionInfo)
{
	m_pExpression = new KGRegressionExpression;
}

KDefaultTGRegressionPage::~KDefaultTGRegressionPage()
{
	if (m_pExpression != nullptr)
	{
		QBicDelete(m_pExpression)
	}
}

void KDefaultTGRegressionPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
	DDX_Control(pDX, IDC_LIST4, m_lstOperator);
	DDX_Control(pDX, IDC_LIST3, m_lstFunction);
}


BEGIN_MESSAGE_MAP(KDefaultTGRegressionPage, CXTPPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST1, &KDefaultTGRegressionPage::OnLbnSelchangeListField)
	ON_LBN_SELCHANGE(IDC_LIST4, &KDefaultTGRegressionPage::OnLbnSelchangeListOperator)
	ON_LBN_SELCHANGE(IDC_LIST3, &KDefaultTGRegressionPage::OnLbnSelchangeListFunction)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KDefaultTGRegressionPage 메시지 처리기입니다.


BOOL KDefaultTGRegressionPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );
	TxLogDebugStartMsg();
	InitFieldList();
	InitFunctionList();
	InitOperatorList();
	InitDefaultData();

	ResizeComponent();
	TxLogDebugEndMsg();
	return TRUE;  
}

void KDefaultTGRegressionPage::InitDefaultData()
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = m_pGenerationInfo->ExpressionContainer();
		if (pBaseCont == nullptr)
		{
			return;
		}

		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return;
		}

		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if(NULL == pBaseExp)
		{
			return;
		}

		*m_pExpression	= *(KGRegressionExpression*)(pBaseExp);

		GetDlgItem(IDC_EDIT1)->SetWindowText(m_pExpression->GetCaptionExpression());

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

void KDefaultTGRegressionPage::InitFieldList()
{
	KIOTables* pIOTables = m_pTarget->Tables();

	ASSERT(NULL != pIOTables);

	KIOTable* pTable = pIOTables->FindTable(_T("sed_vector"));
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
	ASSERT(NULL != pColumns);

	int nColumnCount = pColumns->ColumnCount();
	for(int i = 0; i < nColumnCount; ++i)
	{
		KIOColumn*      pColumn      = pColumns->GetColumn(i);
		KEMIOColumnType emColumnType = pColumn->ColumnType();

		if(pColumn->CodeType() == KEMIOCodeTypeIsSingleCode || pColumn->CodeType() == KEMIOCodeTypeIsMultiCode)
		{
			continue;
		}

		if((KEMIOColumnTypeFixed == emColumnType) || (KEMIOColumnTypeUserDefine == emColumnType))
		{
			int nIndex = m_lstField.InsertString(-1,  pColumn->Caption());
			m_lstField.SetItemData(nIndex, (DWORD_PTR)pColumn);
		}
	}
}

void KDefaultTGRegressionPage::InitOperatorList()
{
	int nOperator = sizeof(szOperators) / sizeof(TCHAR*);

	for(int i = 0; i < nOperator; ++i)
	{
		m_lstOperator.InsertString(-1, szOperators[i]);
	}
}

void KDefaultTGRegressionPage::InitFunctionList()
{
	int nFunction = sizeof(szFunctions) / sizeof(TCHAR*);

	for(int i = 0; i < nFunction; ++i)
	{
		m_lstFunction.InsertString(-1, szFunctions[i]);
	}
}

void KDefaultTGRegressionPage::OnLbnSelchangeListField()
{
	try
	{
		int nSel = m_lstField.GetCurSel();
		if(LB_ERR != nSel)
		{
			CString strField;
			m_lstField.GetText(nSel, strField);

			CString strExpress(_T(""));
			GetDlgItem(IDC_EDIT1)->GetWindowText(strExpress);

			if(strExpress.IsEmpty() == false)
			{
				strExpress += _T(" ");
			}

			strExpress += _T("[");
			strExpress += strField;
			strExpress += _T("]");

			GetDlgItem(IDC_EDIT1)->SetWindowText(strExpress);
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

void KDefaultTGRegressionPage::OnLbnSelchangeListOperator()
{
	try
	{
		int nSel = m_lstOperator.GetCurSel();
		if(LB_ERR != nSel)
		{
			CString strOperator;
			m_lstOperator.GetText(nSel, strOperator);

			CString strExpress(_T(""));
			GetDlgItem(IDC_EDIT1)->GetWindowText(strExpress);

			if(strExpress.IsEmpty() == false)
			{
				strExpress += _T(" ");
			}
			strExpress += strOperator;
			
			GetDlgItem(IDC_EDIT1)->SetWindowText(strExpress);
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

void KDefaultTGRegressionPage::OnLbnSelchangeListFunction()
{
	try
	{
		int nSel = m_lstFunction.GetCurSel();
		if(LB_ERR != nSel)
		{
			CString strFunction;
			m_lstFunction.GetText(nSel, strFunction);

			CString strExpress(_T(""));
			GetDlgItem(IDC_EDIT1)->GetWindowText(strExpress);

			if(strExpress.IsEmpty() == false)
			{
				strExpress += _T(" ");
			}
			strExpress += strFunction;

			GetDlgItem(IDC_EDIT1)->SetWindowText(strExpress);
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

BOOL KDefaultTGRegressionPage::OnApply()
{

	try
	{
		m_pExpression->ClearElements();
		CString strExpression(_T(""));
		GetDlgItem(IDC_EDIT1)->GetWindowText(strExpression);
		m_pExpression->SetCaptionExpression(strExpression);
		if (strExpression.IsEmpty() == true)
		{
			throw -1;
		}

		std::tstring str = (LPCTSTR)strExpression;
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
				std::tstring strName = GetFieldName(strToken.c_str());
				std::tstring strCaption = GetFieldCaption(strToken.c_str());
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

		KGSimpleExpressionContainer* pContainer = 
			(KGSimpleExpressionContainer*)m_pGenerationInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(m_pGenerationInfo->CreateExpression());
		}

		KGRegressionExpression* pExpression = (KGRegressionExpression*)(pContainer->ExpressionObject());
		if(NULL == pExpression)
		{
			pExpression = (KGRegressionExpression*)(pContainer->CreateExpressionObject());
		}

		*pExpression = *m_pExpression;
	}
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}

	CXTPPropertyPage::OnApply();
	return TRUE;
}


bool KDefaultTGRegressionPage::isField(LPCTSTR a_strElement)
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


bool KDefaultTGRegressionPage::isOperator(LPCTSTR a_strElement)
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


bool KDefaultTGRegressionPage::isFunction(LPCTSTR a_strElement)
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



std::tstring KDefaultTGRegressionPage::GetFieldName(LPCTSTR a_strElement)
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


std::tstring KDefaultTGRegressionPage::GetFieldCaption(LPCTSTR a_strElement)
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

void KDefaultTGRegressionPage::OnSize(UINT nType, int cx, int cy)
{
	CXTPPropertyPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void KDefaultTGRegressionPage::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC,			SZ_TOP_LEFT,				SZ_TOP_LEFT);
		SetResize(IDC_EDIT1,			SZ_TOP_LEFT,				SZ_TOP_RIGHT);

		SetResize(IDC_STATIC2,			SZ_TOP_LEFT,				SZ_TOP_CENTER);
		SetResize(IDC_LIST1,			SZ_TOP_LEFT,				SZ_BOTTOM_CENTER);

		SetResize(IDC_STATIC3,			SZ_TOP_CENTER,				SZ_TOP_CENTER);
		SetResize(IDC_LIST4,			SZ_TOP_CENTER,				CXTPResizePoint(.7, 1));

		SetResize(IDC_STATIC4,			CXTPResizePoint(.7, 0),     CXTPResizePoint(.7, 0));
		SetResize(IDC_LIST3,			CXTPResizePoint(.7, 0),		SZ_BOTTOM_RIGHT);
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