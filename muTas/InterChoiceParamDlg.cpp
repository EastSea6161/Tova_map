// InterChoiceParamDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterChoiceParamDlg.h"
#include "afxdialogex.h"

#include "InterInputInfo.h"
#include "Purpose.h"

// KInterChoiceParamDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterChoiceParamDlg, CDialogEx)

KInterChoiceParamDlg::KInterChoiceParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(KInterChoiceParamDlg::IDD, pParent),
	m_pRecordArray(NULL)
{
}

KInterChoiceParamDlg::~KInterChoiceParamDlg()
{
}

void KInterChoiceParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndParamReport);
}


BEGIN_MESSAGE_MAP(KInterChoiceParamDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &KInterChoiceParamDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// KInterChoiceParamDlg 메시지 처리기입니다.

void KInterChoiceParamDlg::PurposeInputInfo( std::list<KInterInputInfo*> a_lstInterInputInfo )
{
	m_lstInterInputInfo = a_lstInterInputInfo;
}

void KInterChoiceParamDlg::InitParamReport( void )
{
	m_wndParamReport.GetReportHeader()->AllowColumnRemove(FALSE);
	m_wndParamReport.GetReportHeader()->AllowColumnSort(FALSE);
	m_wndParamReport.GetReportHeader()->AllowColumnReorder(FALSE);
	m_wndParamReport.GetPaintManager()->SetColumnStyle(xtpReportColumnFlat);
	m_wndParamReport.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
	
	CXTPReportColumn* pCXTPColumn  = NULL;
	int 			  nColumnIndex = 0;

	pCXTPColumn = m_wndParamReport.AddColumn(
		new CXTPReportColumn(nColumnIndex++, _T("Parameter"), 1));
	pCXTPColumn->SetHeaderAlignment(DT_CENTER);

	std::list<KInterInputInfo*>::iterator iter, end = m_lstInterInputInfo.end();

	for (iter = m_lstInterInputInfo.begin(); iter != end; ++iter)
	{
		KInterInputInfo* pInputInfo    = *iter;
		KPurpose* 		 pPurpose 	   = pInputInfo->GetPurpose();
		CString          strColumnName = pPurpose->PurposeName();
		
		if (!pInputInfo->GetSelected())
			continue;
				
		pCXTPColumn = m_wndParamReport.AddColumn(
			new CXTPReportColumn(nColumnIndex++, strColumnName, 1));
		pCXTPColumn->SetHeaderAlignment(DT_CENTER);
		pCXTPColumn->SetAlignment(DT_RIGHT);
	}
	m_wndParamReport.AllowEdit(TRUE);
	m_wndParamReport.EditOnClick(TRUE);
	m_wndParamReport.SetMultipleSelection(FALSE);
}

void KInterChoiceParamDlg::UpdateParamReport( void )
{
	int nRecordCount = m_vecParameterColumn.size();

	for (int i = 0; i < nRecordCount; ++i)
	{
		CString 		strColumnName = m_vecParameterColumn[i];
		KSDoubleRecord* pRecord 	  = m_pRecordArray->GetRecordAt(i);
		int 			nColumnCount  = pRecord->GetCount();

		CXTPReportRecord* pCXTPRecord = m_wndParamReport.AddRecord(new CXTPReportRecord);

		CXTPReportRecordItem* pCXTPItem = pCXTPRecord->AddItem(new CXTPReportRecordItemText);
		pCXTPItem->SetCaption(strColumnName);
		pCXTPItem->SetItemData((DWORD_PTR)pRecord);

		for (int j = 0; j < nColumnCount; ++j)
		{
			pCXTPItem = pCXTPRecord->AddItem(
				new CXTPReportRecordItemNumber(pRecord->GetAt(j), _T("%.5f")));
		}
	}
	m_wndParamReport.Populate();
}


BOOL KInterChoiceParamDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitParamReport();
	UpdateParamReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KInterChoiceParamDlg::OnBnClickedOk()
{
	CXTPReportRecords* pCXTPRecords = m_wndParamReport.GetRecords();
	int 			   nRecordCount = pCXTPRecords->GetCount();

	CXTPReportRecord*           pCXTPRecord;
	CXTPReportRecordItem*       pCXTPItem;
	CXTPReportRecordItemNumber* pCXTPItemNumber;

	for (int i = 0; i < nRecordCount; ++i)
	{
		pCXTPRecord = pCXTPRecords->GetAt(i);
		pCXTPItem = pCXTPRecord->GetItem(0);

		KSDoubleRecord* pRecord = (KSDoubleRecord*)pCXTPItem->GetItemData();
		
		int nColumnCount = pRecord->GetCount();

		for (int j = 0; j < nColumnCount; ++j)
		{
			pCXTPItemNumber = (CXTPReportRecordItemNumber*)pCXTPRecord->GetItem(1);
			double dParamValue = pCXTPItemNumber->GetValue();

			pRecord->SetAt(j, dParamValue);
		}
	}
	CDialogEx::OnOK();
}

void KInterChoiceParamDlg::ParameterColumn( std::vector<CString> a_vecParameterColumn )
{
	m_vecParameterColumn = a_vecParameterColumn;
}

void KInterChoiceParamDlg::ParameterValue( KSDoubleRecordArray* a_pRecordArray )
{
	m_pRecordArray = a_pRecordArray;
}



