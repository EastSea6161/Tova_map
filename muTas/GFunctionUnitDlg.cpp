/**
 * @file GFunctionUnitDlg.cpp
 * @brief KGFunctionUnitDlg 구현 파일
 * @author 
 * @date 2011.04.29
 * @remark
 */


#include "stdafx.h"
#include "KmzApp.h"
#include "GFunctionUnitDlg.h"
#include "afxdialogex.h"
//^^ #include "IOColumn.h"
//^^ #include "IOColumns.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
//^^#include "SEDData.h"
#include "Target.h"



IMPLEMENT_DYNAMIC(KGFunctionUnitDlg, KDialogEx)


KGFunctionUnitDlg::KGFunctionUnitDlg(CWnd* pParent /*=NULL*/)
	:	KDialogEx(KGFunctionUnitDlg::IDD, pParent), 
		m_dNumber(1.0f),
		m_strFieldName(_T("")), 
		m_strFieldCaption(_T(""))
{

}

KGFunctionUnitDlg::~KGFunctionUnitDlg()
{
}

void KGFunctionUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strFieldCaption);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
	DDX_Control(pDX, IDC_EDIT1, m_edtNumber);
}



BEGIN_MESSAGE_MAP(KGFunctionUnitDlg, KDialogEx)
	ON_BN_CLICKED(IDOK, &KGFunctionUnitDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST1, &KGFunctionUnitDlg::OnDblclkList1)
END_MESSAGE_MAP()


void KGFunctionUnitDlg::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}

	m_pTarget = a_pTarget;
}



double KGFunctionUnitDlg::Number(void)
{
	return m_dNumber;
}


void KGFunctionUnitDlg::Number(double a_dNumber)
{
	m_dNumber = a_dNumber;
}


LPCTSTR KGFunctionUnitDlg::FieldName(void)
{
	return m_strFieldName;
}


void KGFunctionUnitDlg::FieldName(LPCTSTR a_strFieldName)
{
	m_strFieldName = a_strFieldName;
}


LPCTSTR KGFunctionUnitDlg::FieldCaption(void)
{
	return m_strFieldCaption;
}


void KGFunctionUnitDlg::FieldCaption(LPCTSTR a_strFieldCaption)
{
	m_strFieldCaption = a_strFieldCaption;
}



// KGFunctionUnitDlg 메시지 처리기입니다.



//Step 1
BOOL KGFunctionUnitDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	initList();
	if(m_lstField.GetCount() > 0)
	{
		m_lstField.GetText(0, m_strFieldCaption);
		KIOColumn* pColumn = (KIOColumn*)(m_lstField.GetItemData(0));
		m_strFieldName = pColumn->Name();
	}
	m_edtNumber.EnableRealNumber(true);
	CString strNumber;
	strNumber.Format(_T("%g"), m_dNumber);
	m_edtNumber.SetWindowText(strNumber);
	UpdateData(FALSE);

	return TRUE;
}


void KGFunctionUnitDlg::initList(void)
{
	KIOTables* pIOTables = m_pTarget->Tables();
	ASSERT(NULL != pIOTables);
	if(NULL == pIOTables)
	{
		AfxMessageBox(_T("사회경제지표 데이터가 존재하지 않습니다."));
	}

	KIOTable* pTable = pIOTables->FindTable(_T("sed_vector"));
	ASSERT(NULL != pTable);

	const KIOColumns* pColumns = pTable->Columns();
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

		if((KEMIOColumnTypeFixed == emColumnType) || (KEMIOColumnTypeUserDefine == emColumnType))
		{
			int nIndex = m_lstField.InsertString(-1, pColumn->Caption());
			m_lstField.SetItemData(nIndex, (DWORD_PTR)pColumn);
		}
	}
}


//Step 2
void KGFunctionUnitDlg::OnDblclkList1()
{
	UpdateData(TRUE);

	int nSel = m_lstField.GetCurSel();
	if(LB_ERR != nSel)
	{
		CString strField;
		m_lstField.GetText(nSel, m_strFieldCaption);
		KIOColumn* pColumn = (KIOColumn*)(m_lstField.GetItemData(nSel));
		m_strFieldName = pColumn->Name();
		UpdateData(FALSE);
	}
}


//Step 3
void KGFunctionUnitDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	CString strNumber;
	m_edtNumber.GetWindowText(strNumber);
	m_dNumber = _ttof(strNumber);
	if (strNumber == _T("") || m_strFieldName == _T(""))
	{
		AfxMessageBox(_T("입력한 수식이 없습니다.")); 
		return;
	}
	KDialogEx::OnOK();
}


