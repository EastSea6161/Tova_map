// InterSetFunctionVDFBPRDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterBPRFunctionDlg.h"
#include "afxdialogex.h"

#include "InterFunctionBPR.h"
//^^ #include "IOTable.h"
//^^ #include "IOTables.h"
#include "Target.h"
//^^ #include "IOColumns.h"

//^^ #include "IOColumn.h"
//#include "CodeGroup.h"
#include "CustomReportRecordItem.h"

#define INTER_BPR_TYPE		0
#define INTER_BPR_ALPHA		1
#define INTER_BPR_BETA		2
#define INTER_BPR_THETA		3
#define INTER_BPR_IOCOLUMN	4

// KInterSetFunctionVDFBPRDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterBPRFunctionDlg, KDialogEx)
KInterBPRFunctionDlg::KInterBPRFunctionDlg(KTarget* a_pTarget, KCodeGroup* a_pCodeData, CWnd* pParent /*=NULL*/)
	:	KDialogEx(KInterBPRFunctionDlg::IDD, pParent),
		m_pTarget(a_pTarget),
		m_pCodeData(a_pCodeData)
{
	m_pFunction = new KInterFunctionBPR;
}

KInterBPRFunctionDlg::~KInterBPRFunctionDlg()
{
	if(NULL != m_pFunction)
	{
		delete m_pFunction;
	}
}

void KInterBPRFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtAlpha);
	DDX_Control(pDX, IDC_EDIT2, m_edtBeta);
	DDX_Control(pDX, IDC_EDIT3, m_edtTheta);
	DDX_Control(pDX, IDC_COMBO1, m_cmbVariable);
	DDX_Control(pDX, IDC_REPORT, m_wndReport);
}


BEGIN_MESSAGE_MAP(KInterBPRFunctionDlg, KDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, &KInterBPRFunctionDlg::OnRadioClicked)
	ON_EN_CHANGE(IDC_EDIT3, &KInterBPRFunctionDlg::OnEnChangeEdit3)
	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, IDC_REPORT, &KInterBPRFunctionDlg::OnReportValueChanged)
	ON_BN_CLICKED(IDCANCEL, &KInterBPRFunctionDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// KInterSetFunctionVDFBPRDlg 메시지 처리기입니다.

void KInterBPRFunctionDlg::SetFunction(KInterFunctionBPR* a_pFunction)
{
	ASSERT(NULL != a_pFunction);
	ASSERT(a_pFunction->FunctionType() == KEMInterModalFunctionBPR);
	*m_pFunction = *a_pFunction;
}

KInterFunctionBPR* KInterBPRFunctionDlg::GetFunction(void)
{
	return m_pFunction;
}

void KInterBPRFunctionDlg::initColumnList(void)
{
	m_ColumnList.clear();

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
				m_ColumnList.push_back(pColumn);
			}
		}
	}
}

void KInterBPRFunctionDlg::updateControlState(void)
{
	if(true == m_pFunction->FunctionForAllLink())
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO1));
		pRadio->SetCheck(TRUE);

		m_edtAlpha.EnableWindow(TRUE);
		m_edtBeta.EnableWindow(TRUE);
		m_edtTheta.EnableWindow(TRUE);
		CString strTheta;
		m_edtTheta.GetWindowText(strTheta);
		double dTheta = _ttof(strTheta);
		if(0.0f == dTheta)
		{
			m_cmbVariable.EnableWindow(FALSE);
		}
		else
		{
			m_cmbVariable.EnableWindow(TRUE);
		}

		m_wndReport.EnableWindow(FALSE);
	}
	else
	{
		CButton* pRadio = (CButton*)(GetDlgItem(IDC_RADIO2));
		pRadio->SetCheck(TRUE);

		m_edtAlpha.EnableWindow(FALSE);
		m_edtBeta.EnableWindow(FALSE);
		m_edtTheta.EnableWindow(FALSE);
		m_cmbVariable.EnableWindow(FALSE);

		m_wndReport.EnableWindow(TRUE);
	}
}

void KInterBPRFunctionDlg::initReportControl(void)
{
	// report header 설정
	m_wndReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);

	//m_wndReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	// Add Columns
	CXTPReportColumn* pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTER_BPR_TYPE, _T("Type"), 100));
	pColumn->SetEditable(FALSE);
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTER_BPR_ALPHA, _T("Alpha(α)"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTER_BPR_BETA, _T("Beta(β)"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTER_BPR_THETA, _T("Theta(θ)"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);

	pColumn = m_wndReport.AddColumn(new CXTPReportColumn(INTER_BPR_IOCOLUMN, _T("Variable"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->GetEditOptions()->AddComboButton(TRUE);
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;

	CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
	std::list<KIOColumn*>::iterator itColumn, itEnd = m_ColumnList.end();
	for(itColumn = m_ColumnList.begin(); itColumn != itEnd; ++itColumn)
	{
		KIOColumn* pColumn = *itColumn;
		pEditOptions->AddConstraint(pColumn->Caption(), (DWORD_PTR)pColumn);
	}

	// Report control 설정
	//m_wndReport.AllowEdit(FALSE);
	//m_wndReport.EditOnClick(FALSE);
	m_wndReport.AllowEdit(TRUE);
	m_wndReport.EditOnClick(TRUE);
	m_wndReport.SetMultipleSelection(FALSE);
}

void KInterBPRFunctionDlg::updateReportRecord(void)
{
	if(NULL != m_pCodeData)
	{
		DWORD_PTR dwDefault = NULL;
		CString strDefault = _T("");

		CXTPReportColumn* pColumn = m_wndReport.GetColumns()->GetAt(INTER_BPR_IOCOLUMN);
		CXTPReportRecordItemEditOptions* pEditOptions = pColumn->GetEditOptions();
		CXTPReportRecordItemConstraints* pConstraints = pEditOptions->GetConstraints();
		if(pConstraints->GetCount() > 0)
		{
			CXTPReportRecordItemConstraint* pConstraint = pConstraints->GetAt(0);
			dwDefault = pConstraint->m_dwData;
			strDefault = pConstraint->m_strConstraint;
		}

		std::vector<int> codeArray;
		m_pCodeData->GetCodes(codeArray);
		std::vector<int>::iterator itCode, itEnd = codeArray.end();
		for(itCode = codeArray.begin(); itCode != itEnd; ++itCode)
		{
			int nCode = *itCode;
			CString strCodeName = m_pCodeData->SingleCodeValue(nCode);
			CString strType;
			strType.Format(_T("%s(%d)"), strCodeName, nCode);

			double dAlpha, dBeta, dTheta;
			KIOColumn* pColumn;
			m_pFunction->GetParameter(nCode, dAlpha, dBeta, dTheta, pColumn);

			CXTPReportRecord* pRecord = m_wndReport.AddRecord(new CXTPReportRecord);

			CXTPReportRecordItem* pItem = pRecord->AddItem(new CXTPReportRecordItemText);
			pItem->SetCaption(strType);
			pItem->SetItemData(nCode);

			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dAlpha, _T("%f")));
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dBeta, _T("%f")));
			pItem = pRecord->AddItem(new CXTPReportRecordItemNumber(dTheta, _T("%f")));
			pItem = pRecord->AddItem(new CCustomReportRecordItem);
			if(NULL == pColumn)
			{
				pItem->SetCaption(strDefault);
				pItem->SetItemData(dwDefault);
			}
			else
			{
				pItem->SetCaption(pColumn->Caption());
				pItem->SetItemData((DWORD_PTR)pColumn);
			}

			if(0.0f == dTheta)
			{
				pItem->SetEditable(FALSE);
			}
		}
	}
	m_wndReport.Populate();
}


BOOL KInterBPRFunctionDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_edtAlpha.EnableRealNumber(true);
	m_edtBeta.EnableRealNumber(true);
	m_edtTheta.EnableRealNumber(true);

	CString strValue;
	double dAlpha, dBeta, dTheta;
	KIOColumn* pColumn;
	m_pFunction->GetDefaultParameter(dAlpha, dBeta, dTheta, pColumn);

	strValue.Format(_T("%f"), dAlpha);
	m_edtAlpha.SetWindowText(strValue);

	strValue.Format(_T("%f"), dBeta);
	m_edtBeta.SetWindowText(strValue);

	strValue.Format(_T("%f"), dTheta);
	m_edtTheta.SetWindowText(strValue);

	int nSel = 0;
	int nIndex = 0;
	std::list<KIOColumn*>::iterator itColumn, itEnd = m_ColumnList.end();
	for(itColumn = m_ColumnList.begin(); itColumn != itEnd; ++itColumn)
	{
		m_cmbVariable.InsertString(nIndex, (*itColumn)->Caption());
		m_cmbVariable.SetItemData(nIndex, (DWORD_PTR)(*itColumn));

		if(*itColumn == pColumn)
		{
			nSel = nIndex;
		}
		++nIndex;		
	}

	if(0.0 == dTheta)
	{
		m_cmbVariable.EnableWindow(FALSE);
	}

	if(NULL != m_pTarget)
	{
		initColumnList();
	}

	initReportControl();
	
	updateReportRecord();

	updateControlState();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterBPRFunctionDlg::OnOK()
{
	if(true == m_pFunction->FunctionForAllLink())
	{
		double dAlpha, dBeta, dTheta;
		KIOColumn* pColumn = NULL;
		CString strNumber;

		m_edtAlpha.GetWindowText(strNumber);
		dAlpha = _ttof(strNumber);

		m_edtBeta.GetWindowText(strNumber);
		dBeta = _ttof(strNumber);

		m_edtTheta.GetWindowText(strNumber);
		dTheta = _ttof(strNumber);

		if(0.0f != dTheta)
		{
			int nSel = m_cmbVariable.GetCurSel();
			if(CB_ERR != nSel)
			{
				pColumn = (KIOColumn*)(m_cmbVariable.GetItemData(nSel));
			}
		}

		m_pFunction->SetDefaultParameter(dAlpha, dBeta, dTheta, pColumn);
		m_pFunction->ClearParameter();
	}
	else
	{
		m_pFunction->ClearParameter();

		CXTPReportRecords* pRecords = m_wndReport.GetRecords();
		int nRecordCount = pRecords->GetCount();
		for(int i = 0; i < nRecordCount; ++i)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItem* pItem = pRecord->GetItem(INTER_BPR_TYPE);
			int nCode = pItem->GetItemData();

			double dAlpha, dBeta, dTheta;
			KIOColumn* pColumn = 0;

			CXTPReportRecordItemNumber* pItemNumber;

			pItemNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(INTER_BPR_ALPHA));
			dAlpha = pItemNumber->GetValue();

			pItemNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(INTER_BPR_BETA));
			dBeta = pItemNumber->GetValue();

			pItemNumber = (CXTPReportRecordItemNumber*)(pRecord->GetItem(INTER_BPR_THETA));
			dTheta = pItemNumber->GetValue();

			if(0 != dTheta)
			{
				pColumn = (KIOColumn*)(pRecord->GetItem(INTER_BPR_IOCOLUMN)->GetItemData());
			}

			m_pFunction->SetParameter(nCode, dAlpha, dBeta, dTheta, pColumn);
		}
	}

	KDialogEx::OnOK();
}

void KInterBPRFunctionDlg::OnRadioClicked(UINT nID)
{
	switch(nID)
	{
	case IDC_RADIO1 : m_pFunction->FunctionForAllLink(true);
		break;
	case IDC_RADIO2 : m_pFunction->FunctionForAllLink(false);
		break;
	}

	updateControlState();
}


void KInterBPRFunctionDlg::OnEnChangeEdit3()
{
	CString strNumber;
	m_edtTheta.GetWindowText(strNumber);

	double dTheta = _ttof(strNumber);
	if(0.0f == dTheta)
	{
		m_cmbVariable.EnableWindow(FALSE);
	}
	else
	{
		m_cmbVariable.EnableWindow(TRUE);
	}
}


void KInterBPRFunctionDlg::OnReportValueChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	ASSERT(pItemNotify != NULL);

	if(pItemNotify->pItem->GetIndex() == INTER_BPR_THETA)
	{
		CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)(pItemNotify->pItem);
		CXTPReportRecord* pRecord = pNumber->GetRecord();
		CXTPReportRecordItem* pItem = pRecord->GetItem(INTER_BPR_IOCOLUMN);

		double dNumber = pNumber->GetValue();
		if(0.0f == dNumber)
		{
			pItem->SetEditable(FALSE);
		}
		else
		{
			pItem->SetEditable(TRUE);
		}
	}


}

void KInterBPRFunctionDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KDialogEx::OnCancel();
}