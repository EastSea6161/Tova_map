// KTestImportMatchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KTestImportMatchDlg.h"
#include "afxdialogex.h"


// KTestImportMatchDlg 대화 상자입니다.

class KColumnReportRecordItem : public CXTPReportRecordItem
{
public :
	KColumnReportRecordItem( int nIndex ) : m_nIndex( nIndex )
	{

	}

	CString GetCaption(CXTPReportColumn* pColumn)
	{
		CXTPReportRecordItemConstraint* pConstraint = pColumn->GetEditOptions()->FindConstraint( m_nIndex );
		ASSERT(pConstraint);
		return pConstraint->m_strConstraint;
	}

	int getColumnIndex( void )
	{
		return m_nIndex;
	}

	DWORD GetSelectedConstraintData(XTP_REPORTRECORDITEM_ARGS* /*pItemArgs*/)
	{
		return m_nIndex;
	}

	virtual void OnConstraintChanged( XTP_REPORTRECORDITEM_ARGS* pItemArgs, 
		CXTPReportRecordItemConstraint* pConstraint )
	{
		if( pConstraint )
		{
			m_nIndex = pConstraint->m_dwData;
		}
	}
	int m_nIndex;
};


IMPLEMENT_DYNAMIC(KTestImportMatchDlg, KDialogEx)

KTestImportMatchDlg::KTestImportMatchDlg(CWnd* pParent /*=NULL*/)
	: KDialogEx(KTestImportMatchDlg::IDD, pParent)
{
	m_vecAlpha.push_back(_T("A"));
	m_vecAlpha.push_back(_T("B"));
	m_vecAlpha.push_back(_T("C"));
	m_vecAlpha.push_back(_T("D"));
	m_vecAlpha.push_back(_T("E"));
}

KTestImportMatchDlg::~KTestImportMatchDlg()
{
}

void KTestImportMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	KDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT, m_ctrlReport);
	DDX_Control(pDX, IDC_REPORT2, m_ctrlReport2);
}


BEGIN_MESSAGE_MAP(KTestImportMatchDlg, KDialogEx)
	ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT2, OnReportDrop2)
	ON_NOTIFY(XTP_NM_REPORT_BEGINDRAG,            IDC_REPORT2, OnReportBeginDrag2)
	ON_NOTIFY(XTP_NM_REPORT_DROP,                 IDC_REPORT,  OnReportDrop1)
	ON_NOTIFY(XTP_NM_REPORT_RECORDS_DROPPED,      IDC_REPORT,  OnReportRecordsDropped1)
END_MESSAGE_MAP()


// KTestImportMatchDlg 메시지 처리기입니다.


BOOL KTestImportMatchDlg::OnInitDialog()
{
	KDialogEx::OnInitDialog();
	KDialogEx::UseKeyEscEnter(true, true);

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_DLG_ICO) );
	SetIcon( hIcon, TRUE );
	SetIcon( hIcon, FALSE );

 	KReportCtrlSetting::Default(m_ctrlReport, TRUE, FALSE, TRUE);
 	KReportCtrlSetting::Default(m_ctrlReport2, TRUE, FALSE, TRUE);

	CXTPReportColumn* pColumn = NULL;

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(0, _T("No."), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	pColumn->SetEditable(FALSE);

	pColumn = m_ctrlReport.AddColumn(new CXTPReportColumn(1, _T("Select"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	pColumn->SetEditable(FALSE);

	for (size_t i= 0; i< m_vecAlpha.size(); i++)
	{
		int nIndex = i+1;
		CString strAlpha = m_vecAlpha[i];

		pColumn->GetEditOptions()->AddConstraint( strAlpha, nIndex );
	}
	pColumn->SetAlignment( DT_CENTER );
	pColumn->GetEditOptions()->m_bAllowEdit = TRUE;
	pColumn->GetEditOptions()->m_bExpandOnSelect = TRUE;
	pColumn->GetEditOptions()->AddComboButton( TRUE );

	m_ctrlReport.Populate();

	pColumn = m_ctrlReport2.AddColumn(new CXTPReportColumn(0, _T("list"), 100));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetSortable(FALSE);
	pColumn->SetEditable(FALSE);

	m_ctrlReport2.Populate();

	Input();
	
	m_ctrlReport.EnableDragDrop(_T("TestDragDrop"), xtpReportAllowDrop);
	m_ctrlReport2.EnableDragDrop(_T("TestDragDrop"), xtpReportAllowDragCopy | xtpReportAllowDrop);
 	m_ctrlReport.SetDropMarkerFlags(xtpReportDropSelect);
 	m_ctrlReport2.SetDropMarkerFlags(xtpReportDropSelect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void KTestImportMatchDlg::Input()
{
	CXTPReportRecord    * pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	for (size_t i=0; i< m_vecAlpha.size(); i++)
	{
		CString strAlpha = m_vecAlpha[i];
		pRecord = m_ctrlReport.AddRecord(new CXTPReportRecord);

		int nIndex = i+1;

		pRecord->AddItem(new CXTPReportRecordItemNumber((double)nIndex, _T("%.0f")));

		pRecord->AddItem(new KColumnReportRecordItem(nIndex));
	}

	for (size_t i=0; i< m_vecAlpha.size(); i++)
	{
		CString strAlpha = m_vecAlpha[i];
		pRecord = m_ctrlReport2.AddRecord(new CXTPReportRecord);

		//선택
		pItem   = pRecord->AddItem(new CXTPReportRecordItemText(strAlpha));
		pItem->SetAlignment(DT_CENTER);
		int nIndex = i+1;
		pItem->SetItemData(nIndex);
	}

	m_ctrlReport.Populate();
	m_ctrlReport2.Populate();
}

void KTestImportMatchDlg::OnReportDrop2(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);

	CXTPReportRecords* pRecords = pItemNotify->pRecords;
	int nCount = pRecords->GetCount();

	for (int i= 0; i< nCount; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		m_ctrlReport2.RemoveRecordEx(pRecord);
	}
	m_ctrlReport2.Populate();
}

void KTestImportMatchDlg::OnReportBeginDrag2(NMHDR *pNotifyStruct, LRESULT *pResult)
{
// 	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);
// 
// 	CXTPReportRecords* pRecords = pItemNotify->pRecords;
// 	int nCount = pRecords->GetCount();
// 
// 	CString strTest(_T(""));
// 
// 	for (int i= 0; i< nCount; i++)
// 	{
// 		CXTPReportRecord* pRecord = pRecords->GetAt(i);
// 		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
// 	}
}


void KTestImportMatchDlg::OnReportDrop1(NMHDR *pNotifyStruct, LRESULT *pResult)
{
// 	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);
// 
// 	CXTPReportRecords* pRecords = pItemNotify->pRecords;
// 	int nCount = pRecords->GetCount();
// 
// 
// 	for (int i= 0; i< nCount; i++)
// 	{
// 		CXTPReportRecord* pRecord = pRecords->GetAt(i);
// 		CXTPReportRecordItem* pItem = pRecord->GetItem(0);
// 		int nTest = (int)pItem->GetItemData();
// 
// 		m_ctrlReport2.RemoveRecordEx(pRecord);
// 	}
// 	m_ctrlReport2.Populate();
}


void KTestImportMatchDlg::OnReportRecordsDropped1(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	XTP_NM_REPORTDRAGDROP *pItemNotify = reinterpret_cast<XTP_NM_REPORTDRAGDROP *>(pNotifyStruct);
	try
	{
		CXTPReportRecords* pRecords = pItemNotify->pRecords;
		int nCount = pRecords->GetCount();
		if (nCount < 1)
		{
			return;
		}
		
		CXTPReportRecord* pDropRecord = pRecords->GetAt(0);


		CXTPReportRecords* pTargetRecords = m_ctrlReport.GetRecords();

		if (nullptr != pDropRecord)
		{
			int                   nRowIndex = pDropRecord->GetIndex(); // will be Drop Row's Index
			CXTPReportRecordItem* pItem2    = pDropRecord->GetItem(0);
			int     nComboIndex = pItem2->GetItemData();

			pTargetRecords->RemoveRecord(pDropRecord);
			m_ctrlReport.Populate();

			pTargetRecords = m_ctrlReport.GetRecords();
			CXTPReportRecord* pTargetRecord = pTargetRecords->GetAt(nRowIndex); // pItemNotify->pTargetRecord 사용 불가

			if (nullptr != pTargetRecord)
			{
				KColumnReportRecordItem* pItem = (KColumnReportRecordItem*)pTargetRecord->GetItem(1);
				pItem->m_nIndex = nComboIndex;

				m_ctrlReport.RedrawControl();

				CXTPReportRow* pRow = m_ctrlReport.GetRows()->Find(pTargetRecord);

				m_ctrlReport.SetFocusedRow(pRow);
			}
		}

	}
	catch (...)
	{
		TxLogDebugException();	
	}
}
