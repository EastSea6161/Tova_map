// InterUserFunctionDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterUserFunctionDlg.h"
#include "afxdialogex.h"

#include "InterFunctionUser.h"
#include "ExpressionChecker.h"

#include "Target.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"

//#include "CodeGroup.h"
#include "InterFunctionUserEXP.h"


/* User message define */
#define UM_INTERFUNCTIONEDIT	WM_USER + 1001

/* Define Column index */
#define INTERUSERFUNCTION_TYPE		0
#define INTERUSERFUNCTION_FUNCTION	1

// KInterUserFunctionDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterUserFunctionDlg, KDialogEx)

KInterUserFunctionDlg::KInterUserFunctionDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent /*=NULL*/)
	:	KDialogEx(KInterUserFunctionDlg::IDD, pParent),
		m_pTarget(a_pTarget),
		m_pCodeData(a_pCodeData)
{
	m_pFunctionUser = new KInterFunctionUser;
	m_pExpressionChecker = new KExpressionChecker;
}

KInterUserFunctionDlg::~KInterUserFunctionDlg()
{
	if(NULL != m_pFunctionUser)
	{
		delete m_pFunctionUser;
		m_pFunctionUser = NULL;
	}
}

void KInterUserFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KInterUserFunctionDlg, KDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, IDC_REPORT, &KInterUserFunctionDlg::OnItemButtonClick)
	ON_MESSAGE(UM_INTERFUNCTIONEDIT, &KInterUserFunctionDlg::OnFunctionEdit)
END_MESSAGE_MAP()

KInterFunctionUser* KInterUserFunctionDlg::GetFunction(void)
{
	return m_pFunctionUser;
}

void KInterUserFunctionDlg::SetFunction(KInterFunctionUser* a_pFunctionUser)
{
	ASSERT(NULL != a_pFunctionUser);
	ASSERT(a_pFunctionUser->FunctionType() == KEMInterModalFunctionUser);
	*m_pFunctionUser = *a_pFunctionUser;
}

void KInterUserFunctionDlg::initExpressionChecker(void)
{
	if(NULL != m_pTarget)
	{
		KIOTables* pIOTables = m_pTarget->Tables();
		KIOTable* pTable = pIOTables->FindTable(TABLE_LINK);
		if(NULL != pTable)
		{
			const KIOColumns* pColumns = pTable->Columns();
			int nColumnCount = pColumns->ColumnCount();
			for(int i = 0; i < nColumnCount; ++i)
			{
				KIOColumn* pColumn = pColumns->GetColumn(i);
				if(pColumn->DataType() == KEMIODataTypeDouble)
				{
					m_pExpressionChecker->AddField(pColumn->Caption());
				}
			}
		}
	}

	m_pExpressionChecker->AddFunction(_T("SQRT"));
	m_pExpressionChecker->AddFunction(_T("EXP"));
	m_pExpressionChecker->AddFunction(_T("LN"));
	m_pExpressionChecker->AddFunction(_T("LOG"));
	m_pExpressionChecker->AddFunction(_T("ABS"));

	m_pExpressionChecker->AddOperator(_T("+"));
	m_pExpressionChecker->AddOperator(_T("-"));
	m_pExpressionChecker->AddOperator(_T("*"));
	m_pExpressionChecker->AddOperator(_T("/"));
	m_pExpressionChecker->AddOperator(_T("^"));
	m_pExpressionChecker->AddOperator(_T("("));
	m_pExpressionChecker->AddOperator(_T(")"));
}

void KInterUserFunctionDlg::initReportControl(void)
{
	// report header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTERUSERFUNCTION_TYPE, _T("Type"), 100));
	pColumn->SetEditable(FALSE);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTERUSERFUNCTION_FUNCTION, _T("User Define Function"), 300));

	// Report control 설정
	m_wndReport.AllowEdit(FALSE);
	m_wndReport.EditOnClick(FALSE);
	m_wndReport.SetMultipleSelection(FALSE);
}

void KInterUserFunctionDlg::updateReportRecord(void)
{
	if(NULL != m_pCodeData)
	{
		std::vector<int> codeArray;
		m_pCodeData->GetCodes(codeArray);
		std::vector<int>::iterator itCode, itEnd = codeArray.end();
		for(itCode = codeArray.begin(); itCode != itEnd; ++itCode)
		{
			int nCode = *itCode;
			CString strCodeName = m_pCodeData->SingleCodeValue(nCode);
			CString strType;
			strType.Format(_T("%s(%d)"), strCodeName, nCode);

			KInterFunctionUserEXP* pFuncton = m_pFunctionUser->Function(nCode);

			CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);
			CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strType);
			pItem->SetItemData(nCode);

			pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			if(NULL != pFuncton)
			{
				pItem->SetCaption(pFuncton->GetCaption());
			}

			CXTPReportRecordItemControl* pButton = pItem->GetItemControls()->AddControl(xtpItemControlTypeButton);
			if(!pButton)
			{
				continue;
			}
			pButton->SetAlignment(xtpItemControlUnknown);
			if (KmzSystem::GetLanguage()==KEMKorea) {
				pButton->SetCaption(_T("편집"));
			}
			else {
				pButton->SetCaption(_T("Edit"));
			}
			pButton->SetSize(CSize(22, 0));
		}
	}
	m_wndReport.Populate();
}

void KInterUserFunctionDlg::generateUserFunction(LPCTSTR strFunction, KInterFunctionUserEXP& function)
{
	std::tstring str = strFunction;

	typedef boost::tokenizer<
		boost::char_separator<TCHAR>, 
		std::tstring::const_iterator, 
		std::tstring> ttokenizer;

	boost::char_separator<TCHAR> sep(_T(" "));
	ttokenizer tokens(str, sep);
	for(ttokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
	{
		std::tstring strToken = *it;
		if(m_pExpressionChecker->IsField(strToken.c_str()) == true)
		{
			std::tstring::size_type n = strToken.find_first_not_of(_T("["));
			strToken = (n == std::tstring::npos) ? strToken : strToken.substr(n, strToken.length());
			n = strToken.find_last_not_of(_T("]"));
			strToken = (n == std::tstring::npos) ? strToken : strToken.substr(0, n + 1);

			CString strFieldName = getFieldName(strToken.c_str());
			CString strFieldCaption =strToken.c_str();
			function.AddField(strFieldName, strFieldCaption);
		}
		else if(m_pExpressionChecker->IsFunction(strToken.c_str()) == true)
		{
			function.AddFunction(strToken.c_str());
		}
		else if(m_pExpressionChecker->IsOperator(strToken.c_str()) == true)
		{
			function.AddOperator(strToken.at(0));
		}
		else
		{
			function.AddNumber(_tstof(strToken.c_str()));
		}
	}
}

CString KInterUserFunctionDlg::getFieldName(LPCTSTR strCaption)
{
	CString strRet = _T("");

	KIOTables* pIOTables = m_pTarget->Tables();
	KIOTable* pTable = pIOTables->FindTable(TABLE_LINK);
	if(NULL != pTable)
	{
		const KIOColumns* pColumns = pTable->Columns();
		int nColumnCount = pColumns->ColumnCount();
		for(int i = 0; i < nColumnCount; ++i)
		{
			KIOColumn* pColumn = pColumns->GetColumn(i);
			CString strColumnCaption = pColumn->Caption();
			if(strColumnCaption.Compare(strCaption) == 0)
			{
				strRet = pColumn->Name();
				break;
			}
		}
	}

	return strRet;
}

BOOL KInterUserFunctionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initExpressionChecker();
	initReportControl();
	updateReportRecord();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterUserFunctionDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CXTPReportRecords* pRecords = m_wndReport.GetRecords();
	int nRecordCount = pRecords->GetCount();
	for(int i = 0; i < nRecordCount; ++i)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItem* pItem = pRecord->GetItem(INTERUSERFUNCTION_TYPE);
		int nCode = pItem->GetItemData();

		pItem = pRecord->GetItem(INTERUSERFUNCTION_FUNCTION);
		CString strExpression = pItem->GetCaption(NULL);

		KInterFunctionUserEXP function;
		generateUserFunction(strExpression, function);
		m_pFunctionUser->Function(nCode, &function);
	}

	KDialogEx::OnOK();
}


void KInterUserFunctionDlg::OnItemButtonClick(NMHDR * pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
	if(!(pItemNotify->pRow && pItemNotify->pItem && pItemNotify->pItemControl))
		return;

	PostMessage(UM_INTERFUNCTIONEDIT, (WPARAM)(pItemNotify->pItem));
}

LRESULT KInterUserFunctionDlg::OnFunctionEdit(WPARAM wParam, LPARAM lParam)
{
	CXTPReportRecordItem* pItem = (CXTPReportRecordItem*)wParam;

// 	KAFunctionDlg dlg;
// 
// 	dlg.Expression(pItem->GetCaption(NULL));
// 	dlg.FieldList(m_pExpressionChecker->FieldList());
// 	dlg.FunctionList(m_pExpressionChecker->FunctionList());
// 	dlg.OperatorList(m_pExpressionChecker->OperatorList());
// 
// 	if(dlg.DoModal() == IDOK)
// 	{
// 		pItem->SetCaption(dlg.Expression());
// 	}

	return 0;
}