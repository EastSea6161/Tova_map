/**
 * @file GSimpleUnitPage.cpp
 * @brief KGSimpleUnitPage 구현 파일
 * @author 
 * @date 2011.05.27
 * @remark
 */
#include "stdafx.h"
#include "KmzApp.h"
#include "GSimpleUnitPage.h"
#include "afxdialogex.h"
#include "GenerationInfo.h"
#include "PAPurpose.h"
#include "GUnitExpression.h"
#include "GSimpleExpressionContainer.h"
#include "Target.h"
#include "ImTasDBase.h"
#include "DBaseConnector.h"


// KGSimpleUnitPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(KGSimpleUnitPage, CXTPPropertyPage)

KGSimpleUnitPage::KGSimpleUnitPage(KGFormulaDefaultDlgPtr* a_spKGFormulaDefaultDlg)
	:	CXTPPropertyPage(KGSimpleUnitPage::IDD), 
		m_dNumber(1.0f),
		m_strFieldName(_T("")), 
		m_strFieldCaption(_T("")),
		m_spKGFormulaDefaultDlg(a_spKGFormulaDefaultDlg)
{
	m_pKExpression = new KGUnitExpression;
}

KGSimpleUnitPage::~KGSimpleUnitPage()
{
	if(NULL != m_pKExpression)
	{
		delete m_pKExpression;
	}
}

void KGSimpleUnitPage::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strFieldCaption);
	DDX_Control(pDX, IDC_LIST1, m_lstField);
	DDX_Control(pDX, IDC_EDIT1, m_edtNumber);
}


BEGIN_MESSAGE_MAP(KGSimpleUnitPage, CXTPPropertyPage)
	ON_LBN_DBLCLK(IDC_LIST1, &KGSimpleUnitPage::OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &KGSimpleUnitPage::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


void KGSimpleUnitPage::SetGenerationInfo(KGenerationInfo* a_pKGInfo)
{
	m_pKGInfo = a_pKGInfo;
	ASSERT(m_pKGInfo);

	KGBaseExpressionContainer* pBaseCont = m_pKGInfo->ExpressionContainer();
	if(NULL == pBaseCont)
	{
		m_pKExpression->Number(m_oParameterInfo.TdUnitValue);
		m_pKExpression->Field(ColumnCaptionToName(m_oParameterInfo.TstrUnitColumnName), m_oParameterInfo.TstrUnitColumnName);
		return;
	}
	ASSERT(pBaseCont->ContainerType() == KEMGExpressionContainerSimple);
	KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;

	KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
	if(NULL == pBaseExp)
	{
		return;
	}
	ASSERT(pBaseExp->GenerationModel() == KEMGenerationUnit);

	*m_pKExpression = *(KGUnitExpression*)(pBaseExp);
}



// KGSimpleUnitPage 메시지 처리기입니다.
//Step 1
BOOL KGSimpleUnitPage::OnInitDialog()
{
	CXTPPropertyPage::OnInitDialog();

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

	m_dNumber = m_pKExpression->Number();
	m_strFieldName = m_pKExpression->FieldName();
	m_strFieldCaption = m_pKExpression->FieldCaption();

	m_edtNumber.EnableRealNumber(true);
	CString strNumber;
	strNumber.Format(_T("%f"), m_dNumber);
	m_edtNumber.SetWindowText(strNumber);
	UpdateData(FALSE);

	if(nullptr == (*m_spKGFormulaDefaultDlg))
	{
		(*m_spKGFormulaDefaultDlg) = KGFormulaDefaultDlgPtr(new KGFormulaDefaultDlg(m_pTarget, m_pParentSheet));
		(*m_spKGFormulaDefaultDlg)->Create(KGFormulaDefaultDlg::IDD, this);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KGSimpleUnitPage::initList(void)
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
void KGSimpleUnitPage::OnDblclkList1()
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
BOOL KGSimpleUnitPage::OnApply()
{
	if( FALSE == InvalidateInputInfo())
	{
		return FALSE;
	}

	KGSimpleExpressionContainer* pContainer = 
		(KGSimpleExpressionContainer*)m_pKGInfo->ExpressionContainer();
	if(NULL == pContainer)
	{
		pContainer = (KGSimpleExpressionContainer*)(m_pKGInfo->CreateExpression());
	}

	KGUnitExpression* pExpression = (KGUnitExpression*)(pContainer->ExpressionObject());
	if(NULL == pExpression)
	{
		pExpression = (KGUnitExpression*)(pContainer->CreateExpressionObject());
	}

	*pExpression = *m_pKExpression;

	CXTPPropertyPage::OnApply();
	return TRUE;
}


BOOL KGSimpleUnitPage::InvalidateInputInfo()
{
	UpdateData(TRUE);
	CString strNumber;
	m_edtNumber.GetWindowText(strNumber);
	m_dNumber = _ttof(strNumber);

	m_pKExpression->Number(m_dNumber);
	m_pKExpression->Field(m_strFieldName, m_strFieldCaption);
	if (strNumber == _T("") || m_strFieldName == _T(""))
	{
		if(m_pParentSheet != NULL)
		{
			m_pParentSheet->SetActivePage(this);
		}

		AfxMessageBox(_T("입력한 수식이 없습니다.")); 

		if(strNumber == _T(""))
		{
			GetDlgItem(IDC_EDIT1)->SetFocus();
		}
		else if(m_strFieldName == _T(""))
		{
			GetDlgItem(IDC_EDIT2)->SetFocus();
		}

		return FALSE;
	}

	return TRUE;
}

void KGSimpleUnitPage::SetTarget( KTarget* a_pTarget )
{
	if (a_pTarget == NULL)
	{
		return;
	}

	m_pTarget = a_pTarget;
}

CString KGSimpleUnitPage::ColumnCaptionToName( CString a_strCaption )
{
	CString strName(_T(""));
	try
	{
		KIOTable* pTable = m_pTarget->Tables()->FindTable(TABLE_SED_VECTOR);

		const KIOColumns* pColumns = pTable->Columns();

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
				if (a_strCaption.CompareNoCase(pColumn->Caption()) == 0)
				{
					strName = pColumn->Name();
					return strName;
				}
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

	return strName;
}


void KGSimpleUnitPage::OnBnClickedButtonDefault()
{
	try
	{
		if (KmzSystem::IsPassengerSystem())
		{
			SetPassengerDefault();
		}
		else
		{
			SetFreightDefault();
		}
		
	}
	catch (...)
	{
		TxLogDebugException();
	}
	UpdateData(FALSE);
}


void KGSimpleUnitPage::SetFreightDefault()
{
	try
	{
		if(m_oParameterInfo.TdUnitValue == 0 && m_oParameterInfo.TstrUnitColumnName == _T(""))
		{
			AfxMessageBox(_T("설정한 수식이 존재하지 않습니다."));
		}
		else
		{
			CString strNumber(_T(""));
			strNumber.Format(_T("%f"), m_oParameterInfo.TdUnitValue);
			m_edtNumber.SetWindowText(strNumber);

			for (int i= 0; i< m_lstField.GetCount(); i++)
			{
				CString strTempFieldCaption(_T(""));
				m_lstField.GetText(i, strTempFieldCaption);

				if(strTempFieldCaption.CompareNoCase(m_oParameterInfo.TstrUnitColumnName) == 0)
				{
					KIOColumn* pColumn = (KIOColumn*)(m_lstField.GetItemData(i));
					m_strFieldName = pColumn->Name();
					m_strFieldCaption = m_oParameterInfo.TstrUnitColumnName;
					break;
				}
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

void KGSimpleUnitPage::SetPassengerDefault()
{
	try
	{

		KGBaseExpressionContainer* pBaseCont = m_pDefaultGInfo->ExpressionContainer();
		if(NULL == pBaseCont)
		{
			return;
		}

		ASSERT(pBaseCont->ContainerType() == KEMGExpressionContainerSimple);
		KGSimpleExpressionContainer* pContainer = (KGSimpleExpressionContainer*)pBaseCont;

		KGBaseExpression* pBaseExp = pContainer->ExpressionObject();
		if(NULL == pBaseExp)
		{
			return;
		}
		ASSERT(pBaseExp->GenerationModel() == KEMGenerationUnit);

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

