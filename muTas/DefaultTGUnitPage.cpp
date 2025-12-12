// KDefaultTGUnitPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DefaultTGUnitPage.h"
#include "afxdialogex.h"
#include "Target.h"
#include "GenerationInfo.h"
#include "GUnitExpression.h"
#include "GSimpleExpressionContainer.h"

// KDefaultTGUnitPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KDefaultTGUnitPage, CXTPPropertyPage)

KDefaultTGUnitPage::KDefaultTGUnitPage(KTarget* a_pTarget, KGenerationInfo* a_pGInfo)
: CXTPPropertyPage(KDefaultTGUnitPage::IDD)
	, m_pTarget(a_pTarget)
	, m_pGenerationInfo(a_pGInfo)
	, m_pColumn(nullptr)
{
	m_pKExpression = new KGUnitExpression;
}

KDefaultTGUnitPage::~KDefaultTGUnitPage()
{
	if (m_pKExpression != nullptr)
	{
		QBicDelete(m_pKExpression);
	}
}

void KDefaultTGUnitPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
}


BEGIN_MESSAGE_MAP(KDefaultTGUnitPage, CXTPPropertyPage)
	ON_LBN_DBLCLK(IDC_LIST1, &KDefaultTGUnitPage::OnLbnDblclkList)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// KDefaultTGUnitPage 메시지 처리기입니다.


BOOL KDefaultTGUnitPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	TxLogDebugStartMsg();
	InitList();
	InitDefaultData();
	ResizeComponent();
	TxLogDebugEndMsg();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KDefaultTGUnitPage::InitList()
{
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_SED_VECTOR);
		const KIOColumns* pColumns = pTable->Columns();

		int nColumnCount = pColumns->ColumnCount();
		for (int i = 0; i < nColumnCount; i++)
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
	catch (KExceptionPtr ex)
	{
		TxLogDebug(ex->GetErrorMessage());
	}
	catch(...)
	{
		TxLogDebugException();
	}
}

void KDefaultTGUnitPage::OnLbnDblclkList()
{
	int nSel = m_lstField.GetCurSel();
	if (LB_ERR == nSel)
	{
		return;
	}

	KIOColumn* pColumn = (KIOColumn*)(m_lstField.GetItemData(nSel));
	if (pColumn!= nullptr)
	{
		GetDlgItem(IDC_EDIT2)->SetWindowText(pColumn->Caption());
		m_pColumn = pColumn;
	}
}

void KDefaultTGUnitPage::InitDefaultData()
{
	try
	{
		KGBaseExpressionContainer* pBaseCont = m_pGenerationInfo->ExpressionContainer();
		if (nullptr == pBaseCont)
		{
			return;
		}
		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;
		if (nullptr == pContainer)
		{
			return;
		}
		
		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if (pBaseExp == nullptr)
		{
			return;
		}

		*m_pKExpression = *(KGUnitExpression*)(pBaseExp);

		double dValue = m_pKExpression->Number();
		CString strValue(_T(""));
		strValue.Format(_T("%.3f"), dValue);
		GetDlgItem(IDC_EDIT1)->SetWindowText(strValue);

		int nListCount = m_lstField.GetCount();
		for (int i = 0; i <nListCount; i++)
		{
			KIOColumn* pColumn = (KIOColumn*)m_lstField.GetItemData(i);
			CString strColumnName = pColumn->Name();
			if (strColumnName.CompareNoCase(m_pKExpression->FieldName()) == 0)
			{
				GetDlgItem(IDC_EDIT2)->SetWindowText(pColumn->Caption());
				m_pColumn = pColumn;
			}
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

BOOL KDefaultTGUnitPage::OnApply()
{
	try
	{
		CString strValue(_T(""));
		GetDlgItem(IDC_EDIT1)->GetWindowText(strValue);
		double dValue (0);
		if (strValue.IsEmpty() == true)
		{
			throw -1;
		}
		dValue = _ttof(strValue);
		m_pKExpression->Number(dValue);
		if (m_pColumn == nullptr)
		{
			throw -1;
		}
		m_pKExpression->Field(m_pColumn->Name(), m_pColumn->Caption());

		KGSimpleExpressionContainer* pContainer = 
			(KGSimpleExpressionContainer*)m_pGenerationInfo->ExpressionContainer();
		if(NULL == pContainer)
		{
			pContainer = (KGSimpleExpressionContainer*)(m_pGenerationInfo->CreateExpression());
		}

		KGUnitExpression* pExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
		if(NULL == pExpression)
		{
			pExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
		}

		*pExpression = *m_pKExpression;
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


void KDefaultTGUnitPage::OnSize(UINT nType, int cx, int cy)
{
	CXTPPropertyPage::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void KDefaultTGUnitPage::ResizeComponent()
{
	try
	{
		SetResize(IDC_STATIC,			SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_STATIC2,			SZ_TOP_LEFT,       SZ_TOP_LEFT);
		SetResize(IDC_EDIT1,			SZ_TOP_LEFT,       SZ_TOP_CENTER);
		SetResize(IDC_STATIC3,			SZ_TOP_CENTER,       SZ_TOP_CENTER);
		SetResize(IDC_EDIT2,		SZ_TOP_CENTER,       SZ_TOP_RIGHT);
		SetResize(IDC_LIST1,			SZ_TOP_CENTER,			SZ_BOTTOM_RIGHT);
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
