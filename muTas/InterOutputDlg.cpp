// InterOutputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "InterOutputDlg.h"
#include "afxdialogex.h"
#include "KExRecordItem.h"
#include "DBaseAssignment.h"
enum
{
	_0_COLUMN_TABLE_NAME = 0,	
	_1_TYPE_NAME,
	_2_COLUMN_NAME
};
// KInterOutputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(KInterOutputDlg, CDialogEx)

KInterOutputDlg::KInterOutputDlg(KTarget* a_pTarget, CWnd* pParent /*=NULL*/)
	: CDialogEx(KInterOutputDlg::IDD, pParent)
	, m_pTarget(a_pTarget)
{

}

KInterOutputDlg::~KInterOutputDlg()
{
}

void KInterOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPORT1, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(KInterOutputDlg, CDialogEx)
END_MESSAGE_MAP()


// KInterOutputDlg 메시지 처리기입니다.


BOOL KInterOutputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitReportCtrl();
	UpdateReportCtrl();

	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.SetGroupRowsBold(TRUE);    
	m_wndReportCtrl.Populate();   

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void KInterOutputDlg::InitReportCtrl( void )
{
	CXTPReportColumn* pColumn = NULL;
	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_0_COLUMN_TABLE_NAME, _T(""), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	//pColumn->SetGroupable(TRUE);

	m_wndReportCtrl.GetColumns()->GetGroupsOrder()->Add(pColumn);
	pColumn->SetVisible(FALSE);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_1_TYPE_NAME, _T("Name"), 50));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	pColumn = m_wndReportCtrl.AddColumn(new CXTPReportColumn(_2_COLUMN_NAME, _T("Out Field Name"), 80));
	pColumn->SetHeaderAlignment(DT_CENTER);
	pColumn->SetAlignment(xtpColumnIconCenter);

	CXTPReportHeader* pHeader = m_wndReportCtrl.GetReportHeader();
	pHeader->AllowColumnRemove(FALSE);
	pHeader->AllowColumnReorder(FALSE);
	pHeader->AllowColumnSort(FALSE);
}

#define LINKOUTPUT _T("Link")
void KInterOutputDlg::UpdateReportCtrl( void )
{
	int     nRunSeq = KDBaseAssignment::GetNewAssignRunSeq(m_pTarget);
	CString strRunSeq;
	strRunSeq.Format(_T("(%dTime)"), nRunSeq);

	CXTPReportRecord*     pRecord = NULL;
	CXTPReportRecordItem* pItem   = NULL;

	int nNo = 0;
	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(LINKOUTPUT));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time") + strRunSeq));

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("V/C") + strRunSeq));

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Sum_Volume")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Sum_Volume") + strRunSeq));

// 	for (int i=0; i<m_modeColumnCollection.ColumnCount(); i++)
// 	{
// 		KIOColumn* pColumn = m_modeColumnCollection.GetColumn(i);
// 		pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
// 		pRecord->AddItem(new CXTPReportRecordItemText(_T("Link")));
// 		pRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption()));
// 		pRecord->AddItem(new CXTPReportRecordItemText(pColumn->Caption() + strRunSeq));
// 	}

	pRecord = m_wndReportCtrl.AddRecord(new CXTPReportRecord());
	pRecord->AddItem(new CXTPReportRecordItemText(_T("OD")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time")));
	pRecord->AddItem(new CXTPReportRecordItemText(_T("Average_Time") + strRunSeq));

	nNo ++;
}
